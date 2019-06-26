/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
*******************************************************************************/

/**
 * \file
 * \brief SDIO驱动，服务SDIO标准接口
 *
 * \internal
 * \par Modification History
 * - 1.00 19-06-04  ipk, first implementation
 * \endinternal
 */

/*******************************************************************************
  头文件
*******************************************************************************/
#include "ametal.h"
#include "am_clk.h"
#include "zmf159_clk.h"
#include "am_delay.h"
#include "am_int.h"
#include "am_sdio.h"
#include "am_byteops.h"
#include "am_zmf159_sdio.h"
#include "hw/amhw_zmf159_sdio.h"

/** \brief SDIO 中断处理函数 */
static void __sdio_irq_handler (void *p_arg);

static int __sdio_setup (void *p_drv, struct am_sdio_device *p_device);
static int __sdio_msg_start (void                  *p_drv,
                             struct am_sdio_device *p_dev,
                             struct am_sdio_msg    *p_msg);

/**
 * \brief SDIO 驱动函数定义
 */
static am_const struct am_sdio_drv_funcs __g_sdio_drv_funcs = {
     __sdio_setup,
     __sdio_msg_start
};

/** \brief SDIO 中断处理函数 */
static void __sdio_irq_handler (void *p_arg)
{
    am_zmf159_sdio_dev_t *p_dev     = (am_zmf159_sdio_dev_t *)p_arg;
    amhw_zlg_sdio_t   *p_hw_sdio = (amhw_zlg_sdio_t *)p_dev->p_devinfo->regbase;
    uint16_t           status    = 0;

    amhw_zlg_sdio_get_int_status( p_hw_sdio, &status);

    if (ZMF159_SDIO_INT_CMD_COMPLE & status) {
        if (p_dev->int_status == ZMF159_SDIO_INT_CMD_COMPLE) {
            p_dev->int_status = 0;
            am_wait_done(&p_dev->wait);
        }
        amhw_zlg_sdio_clr_int_status(p_hw_sdio, ZMF159_SDIO_INT_CMD_COMPLE);
    }

    if (ZMF159_SDIO_INT_DAT_COMPLE & status) {
        if (p_dev->int_status == ZMF159_SDIO_INT_DAT_COMPLE) {
            p_dev->int_status = 0;
            am_wait_done(&p_dev->wait);
        }
        amhw_zlg_sdio_clr_int_status(p_hw_sdio, ZMF159_SDIO_INT_DAT_COMPLE);
    }

    if (ZMF159_SDIO_INT_DAT_ERR & status) {
        amhw_zlg_sdio_clr_int_status(p_hw_sdio, ZMF159_SDIO_INT_DAT_ERR);
    }

    if (ZMF159_SDIO_INT_CMD_ERR & status) {
        amhw_zlg_sdio_clr_int_status(p_hw_sdio, ZMF159_SDIO_INT_CMD_ERR);
    }

    if (ZMF159_SDIO_INT_MB_COMPLE & status) {
        if (p_dev->int_status == ZMF159_SDIO_INT_MB_COMPLE) {
            p_dev->int_status = 0;
            am_wait_done(&p_dev->wait);
        }
        amhw_zlg_sdio_clr_int_status(p_hw_sdio, ZMF159_SDIO_INT_MB_COMPLE);
    }

    if (ZMF159_SDIO_INT_MB_TIMEOUT & status) {
        amhw_zlg_sdio_clr_int_status(p_hw_sdio, ZMF159_SDIO_INT_MB_TIMEOUT);
    }

    if (ZMF159_SDIO_INT_CR_TIMEOUT & status) {
        amhw_zlg_sdio_clr_int_status(p_hw_sdio, ZMF159_SDIO_INT_CR_TIMEOUT);
    }

    if (ZMF159_SDIO_INT_CRC & status) {
        amhw_zlg_sdio_clr_int_status(p_hw_sdio, ZMF159_SDIO_INT_CRC);
    }
}

static int __sdio_cmd_send (am_zmf159_sdio_dev_t *p_dev,
                            am_sdio_trans_t   *p_trans)
{
    int              ret;
    amhw_zlg_sdio_t *p_hw_sdio = (amhw_zlg_sdio_t *)p_dev->p_devinfo->regbase;

    amhw_zlg_sdio_pclkg_set(p_hw_sdio, AMHW_ZLG_SDIO_SEND_MODE);

    amhw_zlg_sdio_write_cmd(p_hw_sdio, 4, 0x40 | p_trans->cmd);
    amhw_zlg_sdio_write_cmd(p_hw_sdio, 3, (uint8_t)((p_trans->arg >> 24) & 0xff));
    amhw_zlg_sdio_write_cmd(p_hw_sdio, 2, (uint8_t)((p_trans->arg >> 16) & 0xff));
    amhw_zlg_sdio_write_cmd(p_hw_sdio, 1, (uint8_t)((p_trans->arg >>  8) & 0xff));
    amhw_zlg_sdio_write_cmd(p_hw_sdio, 0, (uint8_t)(p_trans->arg  & 0xff));

    p_dev->int_status = ZMF159_SDIO_INT_CMD_COMPLE;
    amhw_zlg_sdio_autotr_enable(p_hw_sdio, 1);

    if ((ret = am_wait_on_timeout(&p_dev->wait, 10)) != AM_OK) {
        return ret;
    }

    amhw_zlg_sdio_pclkg_set(p_hw_sdio, AMHW_ZLG_SDIO_RECEIVE_MODE);

    if (p_trans->rsp_type == AM_SDIO_RESPONSE_SHORT) {

        p_dev->int_status = ZMF159_SDIO_INT_CMD_COMPLE;
        amhw_zlg_sdio_autotr_enable(p_hw_sdio, 1);

        if ((ret = am_wait_on_timeout(&p_dev->wait, 10)) != AM_OK) {
            return ret;
        }

        if (p_trans->p_rsp) {
            p_trans->p_rsp[0] =  (uint32_t)amhw_zlg_sdio_read_cmd(p_hw_sdio, 3) << 24  |
                                 (uint32_t)amhw_zlg_sdio_read_cmd(p_hw_sdio, 2) << 16  |
                                 (uint32_t)amhw_zlg_sdio_read_cmd(p_hw_sdio, 1) << 8   |
                                 (uint32_t)amhw_zlg_sdio_read_cmd(p_hw_sdio, 0);
            p_trans->p_rsp[1] =  (uint32_t)amhw_zlg_sdio_read_cmd(p_hw_sdio, 4);
        }

    } else if (p_trans->rsp_type == AM_SDIO_RESPONSE_LONG) {

        amhw_zlg_sdio_get_cid_csd(p_hw_sdio);

        p_dev->int_status = ZMF159_SDIO_INT_CMD_COMPLE;
        amhw_zlg_sdio_autotr_enable(p_hw_sdio, 1);

        if ((ret = am_wait_on_timeout(&p_dev->wait, 10)) != AM_OK) {
            return ret;
        }

        if (p_trans->p_rsp) {
            p_trans->p_rsp[0] =  (uint32_t)amhw_zlg_sdio_read_cmd(p_hw_sdio, 2) << 24  |
                                 (uint32_t)amhw_zlg_sdio_read_cmd(p_hw_sdio, 1) << 16  |
                                 (uint32_t)amhw_zlg_sdio_read_cmd(p_hw_sdio, 0) << 8   |
                                 (uint32_t)amhw_zlg_sdio_read_cmd(p_hw_sdio, 0);
            p_trans->p_rsp[1] =  (uint32_t)amhw_zlg_sdio_read_cmd(p_hw_sdio, 6) << 24  |
                                 (uint32_t)amhw_zlg_sdio_read_cmd(p_hw_sdio, 5) << 16  |
                                 (uint32_t)amhw_zlg_sdio_read_cmd(p_hw_sdio, 4) << 8   |
                                 (uint32_t)amhw_zlg_sdio_read_cmd(p_hw_sdio, 3);
            p_trans->p_rsp[2] =  (uint32_t)amhw_zlg_sdio_read_cmd(p_hw_sdio, 10) << 24 |
                                 (uint32_t)amhw_zlg_sdio_read_cmd(p_hw_sdio, 9) << 16 |
                                 (uint32_t)amhw_zlg_sdio_read_cmd(p_hw_sdio, 8)  << 8  |
                                 (uint32_t)amhw_zlg_sdio_read_cmd(p_hw_sdio, 7);
            p_trans->p_rsp[3] =  (uint32_t)amhw_zlg_sdio_read_cmd(p_hw_sdio, 14) << 24 |
                                 (uint32_t)amhw_zlg_sdio_read_cmd(p_hw_sdio, 13) << 16 |
                                 (uint32_t)amhw_zlg_sdio_read_cmd(p_hw_sdio, 12) << 8  |
                                 (uint32_t)amhw_zlg_sdio_read_cmd(p_hw_sdio, 11);
        }

        p_hw_sdio->mmc_io &= ~(1ul << 4);
    }

    return AM_OK;
}

static int __sdio_data_send (am_zmf159_sdio_dev_t *p_dev,
                             am_sdio_trans_t      *p_trans)
{
    int                ret;
    uint32_t           i           = 0;
    uint32_t          *p_data      = (uint32_t *)p_trans->p_data;
    amhw_zlg_sdio_t   *p_hw_sdio   = (amhw_zlg_sdio_t *)p_dev->p_devinfo->regbase;

    am_sdio_timeout_obj_t timeout;

    amhw_zlg_sdio_set_cnt(p_hw_sdio, p_trans->blk_size);

    amhw_zlg_sdio_fifo_status_enable(p_hw_sdio);

    amhw_zlg_sdio_buf_dir_set(p_hw_sdio, AMHW_ZLG_SDIO_BUF_DIR_WRITE);

    p_dev->int_status = ZMF159_SDIO_INT_DAT_COMPLE;

    if (p_trans->nblock > 1) {

        amhw_zlg_sdio_nblocks_tranfer_dir_set(p_hw_sdio, AMHW_ZLG_SDIO_WRITE_MODE);
        amhw_zlg_sdio_nblocks_cnt_tranfer_set(p_hw_sdio, p_trans->nblock);
        amhw_zlg_sdio_nblocks_auto_tranfer_enable(p_hw_sdio, 1);

        amhw_zlg_sdio_nblocks_cmd_tranfer_enable(p_hw_sdio, 1);

        p_dev->int_status = ZMF159_SDIO_INT_MB_COMPLE;
    } else {
        amhw_zlg_sdio_transfdir_set(p_hw_sdio, AMHW_ZLG_SDIO_WRITE_MODE);
        amhw_zlg_sdio_autodattr_enable(p_hw_sdio);
    }

    am_sdio_timeout_set(&timeout, 10);
    do {
        if (am_sdio_timeout(&timeout)) {

            amhw_zlg_sdio_nblocks_cmd_tranfer_enable(p_hw_sdio, 0);
            amhw_zlg_sdio_nblocks_auto_tranfer_enable(p_hw_sdio, 0);
            amhw_zlg_sdio_autodattr_disable(p_hw_sdio);

            return -AM_ETIME;
        }
    } while(!amhw_zlg_sdio_check_fifo_isempty(p_hw_sdio));

    for (i = 0; i < p_trans->nblock * p_trans->blk_size; i += 4) {
        while(amhw_zlg_sdio_check_fifo_isfull(p_hw_sdio));
        amhw_zlg_sdio_data_buf_write(p_hw_sdio, *p_data++);
    }

    ret = am_wait_on_timeout(&p_dev->wait, 100);

    return ret;
}

static int __sdio_data_recv(am_zmf159_sdio_dev_t *p_dev,
                            am_sdio_trans_t   *p_trans)
{
    int                   ret;
    uint32_t              i           = 0;
    uint32_t             *p_buf       = (uint32_t *)p_trans->p_data;
    amhw_zlg_sdio_t      *p_hw_sdio   = (amhw_zlg_sdio_t *)p_dev->p_devinfo->regbase;

    amhw_zlg_sdio_set_cnt(p_hw_sdio, p_trans->blk_size);

    amhw_zlg_sdio_fifo_status_enable(p_hw_sdio);
    amhw_zlg_sdio_buf_dir_set(p_hw_sdio, AMHW_ZLG_SDIO_BUF_DIR_READ);

    p_dev->int_status = ZMF159_SDIO_INT_DAT_COMPLE;

    if (p_trans->nblock > 1) {

        amhw_zlg_sdio_nblocks_tranfer_dir_set(p_hw_sdio, AMHW_ZLG_SDIO_READ_MODE);
        amhw_zlg_sdio_nblocks_cnt_tranfer_set(p_hw_sdio, p_trans->nblock);

        am_mdelay(4);
        amhw_zlg_sdio_nblocks_auto_tranfer_enable(p_hw_sdio, 1);

        amhw_zlg_sdio_nblocks_cmd_tranfer_enable(p_hw_sdio, 1);

        p_dev->int_status = ZMF159_SDIO_INT_MB_COMPLE;
    } else {
        amhw_zlg_sdio_transfdir_set(p_hw_sdio, AMHW_ZLG_SDIO_READ_MODE);
        amhw_zlg_sdio_autodattr_enable(p_hw_sdio);
    }

    for (i = 0; i < p_trans->blk_size * p_trans->nblock; i += 4) {
        while(amhw_zlg_sdio_check_fifo_isempty(p_hw_sdio));
        *p_buf++ = amhw_zlg_sdio_data_buf_read(p_hw_sdio);
    }

    ret = am_wait_on_timeout(&p_dev->wait, 100);

    return ret;
}

static int __sdio_transfer (void *p_drv, am_sdio_trans_t *p_trans)
{
    int                ret   = AM_ERROR;
    am_zmf159_sdio_dev_t *p_dev = (am_zmf159_sdio_dev_t *)p_drv;

    ret = __sdio_cmd_send(p_dev, p_trans);

    if (ret != AM_OK) {
        return ret;
    }

    if (p_trans->p_data) {
        if (p_trans->opt == AM_SDIO_OW) {
            ret = __sdio_data_send(p_dev, p_trans);
        } else {
            ret = __sdio_data_recv(p_dev, p_trans);
        }
    }

    return ret;
}

static int __sdio_setup (void *p_drv, struct am_sdio_device *p_device)
{
    uint8_t            card_clkdiv;
    am_zmf159_sdio_dev_t *p_zlg_sdio_dev = (am_zmf159_sdio_dev_t *)p_drv;
    amhw_zlg_sdio_t   *p_hw_sdio      = (amhw_zlg_sdio_t *)p_zlg_sdio_dev->p_devinfo->regbase;

    /* 使能clk时钟 */
    amhw_zlg_sdio_set_cardclk_enable(p_hw_sdio, AM_TRUE);

    /* 配置分频值 */
    card_clkdiv = (am_clk_rate_get (CLK_PLLOUT)/(p_device->speed * 2)) - 1;

    amhw_zlg_sdio_set_cardclk_div (p_hw_sdio, card_clkdiv);

    /* 使能CRC控制 */
    if (p_device->crc_enable) {
        amhw_zlg_sdio_set_cardctrl_enable(p_hw_sdio, AM_TRUE);
        amhw_zlg_sdio_set_crcctl_enable(p_hw_sdio, ZMF159_SDIO_CMDCRC | ZMF159_SDIO_DATCRC);
    }

    if (p_device->mode == AM_SDIO_SPI_M) {
        amhw_zlg_sdio_port_operation_set(p_hw_sdio, AMHW_ZLG_SDIO_PORT_OPER_SPI_MODE);
    } else {
        amhw_zlg_sdio_port_operation_set(p_hw_sdio, AMHW_ZLG_SDIO_PORT_OPER_SDIO_MODE);

        if (p_device->mode == AM_SDIO_SD_1B_M) {
            amhw_zlg_sdio_bus_width_set(p_hw_sdio, AMHW_ZLG_SDIO_WIDTH_1BIT);
        } else if (p_device->mode == AM_SDIO_SD_4B_M) {
            amhw_zlg_sdio_bus_width_set(p_hw_sdio, AMHW_ZLG_SDIO_WIDTH_4BIT);
        } else {
            return AM_ERROR;
        }
    }

    amhw_zlg_sdio_speed_mode_set ( p_hw_sdio, p_zlg_sdio_dev->p_devinfo->speed_mode);

    amhw_zlg_sdio_prot_clk_set (p_hw_sdio, AMHW_ZLG_SDIO_PORT_CLK_MODE_1_2BASECLK);

    amhw_zlg_sdio_output_mode_set (p_hw_sdio, AMHW_ZLG_SDIO_OUTPUT_SELECTION_PP_MODE);

    amhw_zlg_sdio_signal_mode_set (p_hw_sdio, AMHW_ZLG_SDIO_SIGNAL_AUTO_MODE);

    amhw_zlg_sdio_set_int_enable (p_hw_sdio, ZMF159_SDIO_INT_CMD_COMPLE |
                                             ZMF159_SDIO_INT_DAT_COMPLE |
                                             ZMF159_SDIO_INT_MB_COMPLE);

    return AM_OK;
}

static int __sdio_msg_start (void                  *p_drv,
                             struct am_sdio_device *p_dev,
                             struct am_sdio_msg    *p_msg)
{
    am_sdio_trans_t   *p_trans = NULL;
    int                ret     = AM_ERROR;

    while(!(am_list_empty(&p_msg->trans_list))) {
        p_trans = am_sdio_msg_out(p_msg);
        do {
            ret = __sdio_transfer(p_drv, p_trans);

            if (ret != AM_OK) {
                p_msg->status = ret;

                return p_msg->status;
            }

        } while ((ret != AM_OK) && (p_trans->retries--));

    }

    if (ret == AM_OK) {
        p_msg->status = AM_OK;
    }

    if (p_msg->pfn_complete) {
        p_msg->pfn_complete(p_msg->p_arg);
    }

    return p_msg->status;
}

am_sdio_handle_t am_zmf159_sdio_init (am_zmf159_sdio_dev_t           *p_dev,
                                      const am_zmf159_sdio_devinfo_t *p_devinfo)
{
    if (p_dev == NULL || p_devinfo == NULL) {
        return NULL;
    }

    p_dev->sdio_serv.p_funcs = (struct am_sdio_drv_funcs *)&__g_sdio_drv_funcs;
    p_dev->sdio_serv.p_drv   = p_dev;

    p_dev->p_devinfo         = p_devinfo;
    p_dev->sdio_serv.p_drv   = p_dev;

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    am_wait_init(&p_dev->wait);

    /* 连接中断 */
    am_int_connect(p_dev->p_devinfo->inum, __sdio_irq_handler, (void *)p_dev);
    am_int_enable(p_dev->p_devinfo->inum);

    return &(p_dev->sdio_serv);
}

void am_zmf159_sdio_deinit (am_sdio_handle_t handle)
{
    am_zmf159_sdio_dev_t *p_dev = (am_zmf159_sdio_dev_t *)handle;

    if (p_dev->p_devinfo->pfn_plfm_init) {
        p_dev->p_devinfo->pfn_plfm_init();
    }

    am_int_disconnect(p_dev->p_devinfo->inum, __sdio_irq_handler, (void *)p_dev);
    am_int_disable(p_dev->p_devinfo->inum);
}

/* end of file */
