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
                           uint8_t           cmd,
                           uint32_t          cmd_arg,
                           uint8_t           rsp_type,
                           void             *p_rsp,
                           void             *p_data,
                           uint32_t          nblks,
                           uint32_t          blk_size,
                           uint8_t           is_read)
{
    am_sdio_msg_t     msg;
    am_wait_t         msg_wait;
    am_sdio_trans_t   trans;
    uint8_t           opt;

    if (p_dev == NULL || nblks <= 0) {
        return -AM_EINVAL;
    }

    am_sdio_msg_init(&msg, __message_complete, &msg_wait);

    opt = is_read ? AM_SDIO_WR : AM_SDIO_OW;

    am_sdio_mktrans(&trans,
                    opt,
                    cmd,
                    cmd_arg,
                    rsp_type,
                    p_rsp,
                    p_data,
                    blk_size,
                    nblks,
                    0);

    am_sdio_trans_add_tail(&msg, &trans);

    am_sdio_msg_start(p_dev, &msg);

    am_wait_on(&msg_wait);

    return msg.status;
}

/**
 * \brief SDIO写命令
 */
int am_sdio_cmd_write (am_sdio_device_t *p_dev,
                       uint8_t           cmd,
                       uint32_t          cmd_arg,
                       uint8_t           rsp_type,
                       void             *p_rsp)
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
                    AM_SDIO_OW,
                    cmd,
                    cmd_arg,
                    rsp_type,
                    p_rsp,
                    NULL,
                    0,
                    0,
                    0);

    am_sdio_trans_add_tail(&msg, &trans);
    am_sdio_msg_start(p_dev, &msg);

    am_wait_on(&msg_wait);

    return msg.status;
}

/**
 * \brief SDIO先写命令再读数据
 */
int am_sdio_write_then_read (am_sdio_device_t *p_dev,
                             uint8_t           cmd,
                             uint32_t          cmd_arg,
                             uint8_t           rsp_type,
                             void             *p_rsp,
                             void             *p_data,
                             uint32_t          blk_size,
                             uint32_t          nblks)
{
    return __sdio_rw_sync(p_dev,
                          cmd,
                          cmd_arg,
                          rsp_type,
                          p_rsp,
                          p_data,
                          nblks,
                          blk_size,
                          1);
}

/**
 * \brief SDIO先写命令再写数据
 */
int am_sdio_write_then_write (am_sdio_device_t *p_dev,
                              uint8_t           cmd,
                              uint32_t          cmd_arg,
                              uint8_t           rsp_type,
                              uint32_t         *p_rsp,
                              uint8_t          *p_data,
                              uint32_t          blk_size,
                              uint32_t          nblks)
{
    return __sdio_rw_sync(p_dev,
                          cmd,
                          cmd_arg,
                          rsp_type,
                          p_rsp,
                          p_data,
                          nblks,
                          blk_size,
                          0);
}

/* end of file */
