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
 * \brief FM25CLxx 例程
 *
 * - 实验现象：
 *   1. 串口打印出测试结果。
 *
 * \par 源代码
 * \snippet demo_fm25clxx.c
 *
 * \internal
 * \par Modification history
 * - 1.00  19-06-06 zjr, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_fm25clxx
 * \copydoc demo_fm25clxx.c
 */

/** [src_fm25clxx] */
#include "ametal.h"
#include "am_delay.h"
#include "am_fm25clxx.h"
#include "am_vdebug.h"

#define __BUF_SIZE 16 /**< \brief 缓冲区大小 */

/**
 * \brief 例程入口
 */
void demo_fm25clxx_entry (am_fm25clxx_handle_t fm25clxx_handle, int32_t test_lenth)
{
    int     ret;
    uint8_t i;
    uint8_t wr_buf[__BUF_SIZE] = {0}; /* 写数据缓存定义 */
    uint8_t rd_buf[__BUF_SIZE] = {0}; /* 读数据缓存定义 */

    if (__BUF_SIZE < test_lenth) {
        test_lenth = __BUF_SIZE;
    }

    /* 填充发送缓冲区 */
    for (i = 0;i < test_lenth; i++) {
        wr_buf[i] = i;
    }

    /* 写数据 */
    ret = am_fm25clxx_write(fm25clxx_handle, 0x1000, &wr_buf[0], test_lenth);

    if (ret != AM_OK) {
        AM_DBG_INFO("am_mx25xx_write error(id: %d).\r\n", ret);
        return;
    }
    am_mdelay(5);

    /* 读数据 */
    ret = am_fm25clxx_read(fm25clxx_handle, 0x1000, &rd_buf[0], test_lenth);

    if (ret != AM_OK) {
        AM_DBG_INFO("am_mx25xx_read error(id: %d).\r\n", ret);
        return;
    }

    /* 校验写入和读取的数据是否一致 */
    for (i = 0; i < test_lenth; i++) {
        AM_DBG_INFO("Read FLASH the %2dth data is %2x\r\n", i ,rd_buf[i]);

        /* 校验失败 */
        if(wr_buf[i] != rd_buf[i]) {
            AM_DBG_INFO("verify failed at index %d.\r\n", i);
            break;
        }
    }

    if (test_lenth == i) {
        AM_DBG_INFO("verify success!\r\n");
    }

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_mx25xx] */

/* end of file */
