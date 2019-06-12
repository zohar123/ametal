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
 * \brief SDIO 读/写函数实现
 * 
 * \internal
 * \par Modification history
 * - 1.00 19-06-12  ipk, first implementation.
 * \endinternal
 */

#include "am_sdio.h"
#include "am_wait.h"

/**
 * \brief SDIO消息完成回调函数
 */
static void __message_complete (void *p_arg)
{
    am_wait_t *p_wait = (am_wait_t *)p_arg;
     
    /* 结束等待序列 */
    am_wait_done(p_wait);
}

static int __sdio_rw_sync (am_sdio_device_t *p_dev,
                           uint8_t          *p_buf,
                           uint32_t          addr_start,
                           uint32_t          blk_num,
                           uint8_t           is_read)
{
    am_sdio_msg_t     msg;
    uint32_t          rsp1[4];
    am_wait_t         msg_wait;
    am_sdio_trans_t   trans;
    uint8_t           cmd;
    uint8_t           opt;

    if (p_dev == NULL || blk_num <= 0) {
        return -AM_EINVAL;
    }

    am_sdio_msg_init(&msg, __message_complete, &msg_wait);

    if (is_read) {
        opt = AM_SDIO_WR;
        cmd = (blk_num == 1) ? AM_SDIO_CMD17 : AM_SDIO_CMD18;
    } else {
        opt = AM_SDIO_OW;
        cmd = (blk_num == 1) ? AM_SDIO_CMD24 : AM_SDIO_CMD25;
    }

    am_sdio_mktrans(&trans,
                    cmd,
                    addr_start,
                    opt,
                    p_buf,
                    p_dev->blk_size,
                    blk_num,
                    AM_SDIO_RESPONSE_SHORT,
                    rsp1,
                    0);

    am_sdio_trans_add_tail(&msg, &trans);

    am_sdio_msg_start(p_dev, &msg);

    am_wait_on(&msg_wait);

    if (p_dev->mode == AM_SDIO_SPI_M) {
        if ((rsp1[0] & (AM_SDIO_SPI_R1_IN_IDLE_STATE |
                        AM_SDIO_SPI_R1_ALL_ERROR)) != 0) {
            return -AM_EIO;
        }
    } else {
        if ((rsp1[0] & 0xFFFF0000) != 0) {
            return -AM_EIO;
        }
    }

    return msg.status;
}

/**
 * \brief SDIO写命令
 */
int am_sdio_cmd_write (am_sdio_device_t *p_dev,
                       uint8_t           cmd,
                       uint32_t          cmd_arg,
                       uint8_t           rsp_type,
                       uint32_t         *p_rsp)
{
    am_sdio_msg_t     msg;
    am_sdio_trans_t   trans;
    am_wait_t         msg_wait;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    am_wait_init(&msg_wait);
    am_sdio_msg_init(&msg, __message_complete, &msg_wait);

    am_sdio_mktrans(&trans,
                    cmd,
                    cmd_arg,
                    AM_SDIO_OW,
                    NULL,
                    0,
                    0,
                    rsp_type,
                    p_rsp,
                    0);

    am_sdio_trans_add_tail(&msg, &trans);
    am_sdio_msg_start(p_dev, &msg);

    am_wait_on(&msg_wait);

    return msg.status;
}

/**
 * \brief SDIO数据块写入
 */
int am_sdio_blocks_write (am_sdio_device_t *p_dev,
                          uint8_t          *p_buf,
                          uint32_t          addr_start,
                          uint32_t          blk_num)
{
    return __sdio_rw_sync(p_dev,
                          p_buf,
                          addr_start,
                          blk_num,
                          0);
}

/**
 * \brief SDIO数据块读取
 */
int am_sdio_blocks_read (am_sdio_device_t *p_dev,
                         uint8_t          *p_buf,
                         uint32_t          addr_start,
                         uint32_t          blk_num)
{
    return __sdio_rw_sync(p_dev,
                          p_buf,
                          addr_start,
                          blk_num,
                          1);
}

/* end of file */
