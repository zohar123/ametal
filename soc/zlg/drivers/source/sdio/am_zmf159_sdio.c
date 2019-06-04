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
 * - 1.01 17-12-28  pea, add a timeout mechanism, enable TX_ABRT and STOP_DET
 *                  interrupt
 * - 1.00 17-04-24  sdy, first implementation
 * \endinternal
 */

/*******************************************************************************
  头文件
*******************************************************************************/
#include "ametal.h"
#include "am_clk.h"
#include "zmf159_clk.h"
#include "am_int.h"
#include "am_sdio.h"
#include "am_zmf159_sdio.h"
#include "hw/amhw_zmf159_sdio.h"

/*******************************************************************************
  宏定义
*******************************************************************************/

#define __SDIO_EVT_NONE            (0xFFu)          /* 无事件 */
#define __SDIO_EVT_MSG_LAUNCH      (0xFEu)          /* 开始处理一个新的消息 */
#define __SDIO_EVT_TRANS_LAUNCH    (0xFDu)          /* 开始处理一个新的传输 */

#define __SDIO_ST_MSG_START        (0x11u)          /* 消息传输开始状态 */
#define __SDIO_ST_IDLE             (0x10u)          /* 空闲状态 */
#define __SDIO_ST_TRANS_START      (0x12u)          /* 单个传输开始状态 */
#define __SDIO_ST_SEND_CMD         (0x13u)          /* 发送命令状态 */
#define __SDIO_ST_M_SEND_DATA      (0x14u)          /* 发送数据状态 */
#define __SDIO_ST_M_RECV_DATA      (0x15u)          /* 接收数据状态 */

/** \brief SDIO 消息处理函数 */
static int __sdio_msg_start (void *p_drv);

/** \brief SDIO 硬件初始化 */
static int __sdio_hard_init (am_zlg_sdio_dev_t *p_dev);

/** \brief SDIO 中断处理函数 */
static void __sdio_irq_handler (void *p_arg);

/** \brief SDIO 中断处理函数 */
static int __sdio_send_cmd (void *p_drv, am_sdio_cmd_t *cmd_msg);
static int __sdio_msg_send(void *p_drv,  struct am_sdio_message  *p_msg  , uint16_t len);
static int __sdio_msg_recv(void *p_drv,  struct am_sdio_message  *p_msg  , uint16_t len);

/**
 * \brief SDIO 驱动函数定义
 */
static am_const struct am_sdio_drv_funcs __g_sdio_drv_funcs = {
     __sdio_msg_send,
     __sdio_msg_recv,
     __sdio_send_cmd,
     __sdio_msg_start
};

/** \brief SDIO 中断处理函数 */
static void __sdio_irq_handler (void *p_arg)
{
    am_zlg_sdio_dev_t *p_dev     = (am_zlg_sdio_dev_t *)p_arg;
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
        am_wait_done(&p_dev->wait);
        amhw_zlg_sdio_clr_int_status(p_hw_sdio, ZMF159_SDIO_INT_DAT_ERR);
    }

    if (ZMF159_SDIO_INT_CMD_ERR & status) {
        am_wait_done(&p_dev->wait);
        amhw_zlg_sdio_clr_int_status(p_hw_sdio, ZMF159_SDIO_INT_CMD_ERR);
    }

    if (ZMF159_SDIO_INT_MB_COMPLE & status) {
        am_wait_done(&p_dev->wait);
        amhw_zlg_sdio_clr_int_status(p_hw_sdio, ZMF159_SDIO_INT_MB_COMPLE);
    }

    if (ZMF159_SDIO_INT_MB_TIMEOUT & status) {
        am_wait_done(&p_dev->wait);
        amhw_zlg_sdio_clr_int_status(p_hw_sdio, ZMF159_SDIO_INT_MB_TIMEOUT);
    }

    if (ZMF159_SDIO_INT_CR_TIMEOUT & status) {
        am_wait_done(&p_dev->wait);
        amhw_zlg_sdio_clr_int_status(p_hw_sdio, ZMF159_SDIO_INT_CR_TIMEOUT);
    }

    if (ZMF159_SDIO_INT_CRC & status) {
        am_wait_done(&p_dev->wait);
        amhw_zlg_sdio_clr_int_status(p_hw_sdio, ZMF159_SDIO_INT_CRC);
    }
}

static int __sdio_hard_init (am_zlg_sdio_dev_t *p_dev)
{
    uint8_t  card_clkdiv = 0;

    amhw_zlg_sdio_t  *p_hw_sdio = (amhw_zlg_sdio_t *)p_dev->p_devinfo->regbase;

    /* 使能CRC控制 */
    amhw_zlg_sdio_set_cardctrl_enable(p_hw_sdio, AM_TRUE);

    /* 使能clk时钟 */
    amhw_zlg_sdio_set_cardclk_enable(p_hw_sdio, AM_TRUE);

    /* 配置分频值 */
    card_clkdiv = ( am_clk_rate_get (CLK_PLLOUT)/(p_dev->p_devinfo->speed * 2)) - 1;

    amhw_zlg_sdio_set_cardclk_div (p_hw_sdio, card_clkdiv);

    /* 设置传输速度和时钟基础时钟分频  */
    if(p_dev->p_devinfo->speed_mode == AMHW_ZLG_SDIO_SPEED_MODE_HIGH) {
        amhw_zlg_sdio_speed_mode_set (p_hw_sdio, AMHW_ZLG_SDIO_SPEED_MODE_HIGH);
    } else {
        amhw_zlg_sdio_speed_mode_set (p_hw_sdio, AMHW_ZLG_SDIO_SPEED_MODE_LOW);
    }
    amhw_zlg_sdio_prot_clk_set (p_hw_sdio, AMHW_ZLG_ADIO_PORT_CLK_MODE_1_2BASECLK);

    /*使能CRC */
    amhw_zlg_sdio_set_crcctl_enable(p_hw_sdio, ZMF159_SDIO_CMDCRC | ZMF159_SDIO_DATCRC);

    amhw_zlg_sdio_bus_width_set(p_hw_sdio, p_dev->p_devinfo->width);

    amhw_zlg_sdio_speed_mode_set ( p_hw_sdio, p_dev->p_devinfo->speed_mode);

    amhw_zlg_sdio_prot_clk_set (p_hw_sdio, 0);

    amhw_zlg_sdio_output_mode_set (p_hw_sdio, AMHW_ZLG_SDIO_OUTPUT_SELECTION_PP_MODE);

    amhw_zlg_sdio_signal_mode_set (p_hw_sdio, AMHW_ZLG_SDIO_SIGNAL_AUTO_MODE);

    amhw_zlg_sdio_port_operation_set(p_hw_sdio, AMHW_ZLG_SDIO_PORT_OPER_SDIO_MODE);

    amhw_zlg_sdio_set_int_enable (p_hw_sdio, ZMF159_SDIO_INT_CMD_COMPLE);

//    amhw_zlg_sdio_out_8nullclk(p_hw_sdio);

//    amhw_zlg_sdio_autotr_enable(p_hw_sdio);

    return AM_OK;
}

/** \brief SDIO 命令发送函数 */
static int __sdio_send_cmd (void *p_drv, am_sdio_cmd_t *cmd_msg)
{
    int                ret;
    am_zlg_sdio_dev_t *p_dev     = (am_zlg_sdio_dev_t *)p_drv;
    amhw_zlg_sdio_t   *p_hw_sdio = (amhw_zlg_sdio_t *)p_dev->p_devinfo->regbase;

    amhw_zlg_sdio_write_cmd(p_hw_sdio, 4, 0x40 | cmd_msg->cmd);
    amhw_zlg_sdio_write_cmd(p_hw_sdio, 3, ((cmd_msg->arg & 0xff000000) >> 24));
    amhw_zlg_sdio_write_cmd(p_hw_sdio, 2, ((cmd_msg->arg & 0xff0000) >> 16));
    amhw_zlg_sdio_write_cmd(p_hw_sdio, 1, ((cmd_msg->arg & 0xff00) >> 8));
    amhw_zlg_sdio_write_cmd(p_hw_sdio, 0, (cmd_msg->arg & 0xff));

    amhw_zlg_sdio_autotr_enable(p_hw_sdio, 1);

    p_dev->int_status = ZMF159_SDIO_INT_CMD_COMPLE;
    if ((ret = am_wait_on_timeout(&p_dev->wait, 10)) != AM_OK) {
        return ret;
    }
//    while(1) {
//        if(p_hw_sdio->clr_mmc_int & 0x1) {                                          //judge which interrupt generation
//            p_hw_sdio->clr_mmc_int = 0x1;                                           //write 1 clear interrup
//            break;
//        }
//    }

    if (cmd_msg->rsp_type == SDIO_RESPONSE_SHORT) {

        amhw_zlg_sdio_pclkg_set(p_hw_sdio, AMHW_ZLG_SDIO_RESIVE_MODE);
        amhw_zlg_sdio_autotr_enable(p_hw_sdio, 1);

        p_dev->int_status = ZMF159_SDIO_INT_CMD_COMPLE;
        if ((ret = am_wait_on_timeout(&p_dev->wait, 10)) != AM_OK) {
            return ret;
        }
//        while(1) {
//            if(p_hw_sdio->clr_mmc_int & 0x1) {                            //judge which interrupt generation
//                p_hw_sdio->clr_mmc_int = 0x1;                                           //write 1 clear interrup
//                break;
//            }
//        }

        amhw_zlg_sdio_pclkg_set(p_hw_sdio, AMHW_ZLG_SDIO_SEND_MODE);
        cmd_msg->p_rsp[0] =  amhw_zlg_sdio_read_cmd(p_hw_sdio, 3) << 24  |
                             amhw_zlg_sdio_read_cmd(p_hw_sdio, 2) << 16  |
                             amhw_zlg_sdio_read_cmd(p_hw_sdio, 1) << 8   |
                             amhw_zlg_sdio_read_cmd(p_hw_sdio, 0);
        cmd_msg->p_rsp[1] =  amhw_zlg_sdio_read_cmd(p_hw_sdio, 4);

    } else if (cmd_msg->rsp_type == SDIO_RESPONSE_LONG) {

        amhw_zlg_sdio_get_cid_csd(p_hw_sdio);
        amhw_zlg_sdio_pclkg_set(p_hw_sdio, AMHW_ZLG_SDIO_RESIVE_MODE);
        amhw_zlg_sdio_autotr_enable(p_hw_sdio, 1);

        p_dev->int_status = ZMF159_SDIO_INT_CMD_COMPLE;
        if ((ret = am_wait_on_timeout(&p_dev->wait, 10)) != AM_OK) {
            return ret;
        }
//        while(1) {
//            if(p_hw_sdio->clr_mmc_int & 0x1) {                               //judge which interrupt generation
//                p_hw_sdio->clr_mmc_int = 0x1;                                //write 1 clear interrup
//                break;
//            }
//        }

        p_hw_sdio->mmc_io &= ~(1 << 4);
        amhw_zlg_sdio_pclkg_set(p_hw_sdio, AMHW_ZLG_SDIO_SEND_MODE);
        cmd_msg->p_rsp[0] =  amhw_zlg_sdio_read_cmd(p_hw_sdio, 3) << 24  |
                             amhw_zlg_sdio_read_cmd(p_hw_sdio, 2) << 16  |
                             amhw_zlg_sdio_read_cmd(p_hw_sdio, 1) << 8   |
                             amhw_zlg_sdio_read_cmd(p_hw_sdio, 0);
        cmd_msg->p_rsp[1] =  amhw_zlg_sdio_read_cmd(p_hw_sdio, 7) << 24  |
                             amhw_zlg_sdio_read_cmd(p_hw_sdio, 6) << 16  |
                             amhw_zlg_sdio_read_cmd(p_hw_sdio, 5) << 8   |
                             amhw_zlg_sdio_read_cmd(p_hw_sdio, 4);
        cmd_msg->p_rsp[2] =  amhw_zlg_sdio_read_cmd(p_hw_sdio, 11) << 24  |
                             amhw_zlg_sdio_read_cmd(p_hw_sdio, 10) << 16  |
                             amhw_zlg_sdio_read_cmd(p_hw_sdio, 9)  << 8   |
                             amhw_zlg_sdio_read_cmd(p_hw_sdio, 8);
        cmd_msg->p_rsp[3] =  amhw_zlg_sdio_read_cmd(p_hw_sdio, 15) << 24  |
                             amhw_zlg_sdio_read_cmd(p_hw_sdio, 14) << 16  |
                             amhw_zlg_sdio_read_cmd(p_hw_sdio, 13) << 8   |
                             amhw_zlg_sdio_read_cmd(p_hw_sdio, 12);
    }

    return AM_OK;
}

static int __sdio_msg_recv(void *p_drv, struct am_sdio_message  *p_msg, uint16_t len)
{
    int                i         = 0;
    uint32_t           data      = 0;
    uint8_t           *p_buf     = p_msg->p_data;
    am_zlg_sdio_dev_t *p_dev     = (am_zlg_sdio_dev_t *)p_drv;
    amhw_zlg_sdio_t   *p_hw_sdio = (amhw_zlg_sdio_t *)p_dev->p_devinfo->regbase;
    am_sdio_timeout_obj_t timeout;
    am_bool_t             timeout_flg;

    p_hw_sdio->mmc_io |= 0x02;

    am_adio_timeout_set(&timeout, 10);
    do {                                                 //wait FIFO full interrupt
        if((p_hw_sdio->buf_ctll & 0x1)) {                //judge which interrupt generation
            break;
        }
        timeout_flg = am_sdio_timeout(&timeout);
    } while(!timeout_flg);

    if (timeout_flg) {
        return -AM_ETIME;
    }

    p_hw_sdio->buf_ctll = 0x000;

    for (i = 0; i < len ;) {
        if(p_hw_sdio->buf_ctll & 0x2) {
            break;
        }
        data =  p_hw_sdio->data_buf[0];
       *p_buf++ = data;
       *p_buf++ = data >> 8;
       *p_buf++ = data >> 16;
       *p_buf++ = data >> 24;
        i += 4;
    }
    return i;
}

static int __sdio_msg_send(void *p_drv, struct am_sdio_message  *p_msg, uint16_t len)
{
    am_zlg_sdio_dev_t *p_dev     = (am_zlg_sdio_dev_t *)p_drv;
    amhw_zlg_sdio_t   *p_hw_sdio = (amhw_zlg_sdio_t *)p_dev->p_devinfo->regbase;
    int i = 0;
    uint32_t          *p_data    = (uint32_t *)p_msg->p_data;

    for (i = 0; i < 128; i++) {
        p_hw_sdio->data_buf[0] = *p_data++;
    }

    p_dev->int_status = ZMF159_SDIO_INT_DAT_COMPLE;
    return am_wait_on_timeout(&p_dev->wait, 10);

}

/*  状态机内部状态切换 */
#define __SDIO_NEXT_STATE(s, e) \
    do { \
        p_dev->state = (s); \
        new_event = (e); \
    } while(0)

static int __sdio_msg_start (void *p_drv)
{
    return AM_OK;
}


am_sdio_handle_t am_zmf159_sdio_init (am_zlg_sdio_dev_t           *p_dev,
                                     const am_zlg_sdio_devinfo_t *p_devinfo)
{
    if (p_dev == NULL || p_devinfo == NULL) {
        return NULL;
    }

    p_dev->sdio_serv.p_funcs = (struct am_sdio_drv_funcs *)&__g_sdio_drv_funcs;
    p_dev->sdio_serv.p_drv   = p_dev;

    p_dev->p_cur_msg         = NULL;
    p_dev->p_devinfo         = p_devinfo;
    p_dev->sdio_serv.p_drv   = p_dev;
    p_dev->is_complete       = AM_FALSE;
    p_dev->data_ptr          = 0;
    p_dev->state             = __SDIO_ST_IDLE;
    p_dev->busy              = AM_FALSE;
    p_dev->is_abort          = AM_FALSE;

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    am_list_head_init(&(p_dev->msg_list));

    am_wait_init(&p_dev->wait);

    __sdio_hard_init (p_dev);

    /* 连接中断 */
    am_int_connect(p_dev->p_devinfo->inum, __sdio_irq_handler, (void *)p_dev);
    am_int_enable(p_dev->p_devinfo->inum);

    return &(p_dev->sdio_serv);
}

/* end of file */
