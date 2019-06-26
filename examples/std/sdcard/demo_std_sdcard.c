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
 * \brief SD卡多块读写例程
 *
 * - 操作步骤：
 *   1. 将SD卡插入卡槽
 *
 * - 实验现象：
 *   1. 擦除SD卡指定块；
 *   2. 写入块数据；
 *   2. 读出刚才写入的数据；
 *   3. 调试串口打印测试结果，若读取的数据与写入的数据不一致，则打印"sd card test error"并停止。
 *
 * - 注意：
 *   1. 运行此demo可能会破坏SD卡原有的文件系统，注意备份保存SD卡中原本的重要文件
 *
 * \par 源代码
 * \snippet demo_std_sdcard.c src_std_sdcard
 *
 * \internal
 * \par Modification history
 * - 1.00 19-06-14  ipk, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_sdcard
 * \copydoc demo_std_sdcard.c
 */

/** [src_std_std_sdcard] */
#include "ametal.h"
#include "am_sdio.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_sdcard.h"

#define __READ_BLOCKS  2       /* 读数据块个数 */
#define __WRITE_BLOCKS 6       /* 写数据块个数 */
#define __ERASE_BLOCKS 10      /* 擦除数据块个数 */

#define __BLOCK_SIZE   512     /* 数据块大小 */

static uint8_t __g_wr_buf[__BLOCK_SIZE * __WRITE_BLOCKS] = {0}; /* 写数据缓存定义 */
static uint8_t __g_rd_buf[__BLOCK_SIZE * __READ_BLOCKS]  = {0}; /* 读数据缓存定义 */
/**
 * \brief 例程入口
 */
void demo_std_sdcard_entry (am_sdcard_handle_t handle)
{
    uint16_t           i;
    am_err_t           ret;
    uint32_t           count = 0;

    if (handle == NULL) {
        am_kprintf( "sd card init failed\r\n");
        while(1);
    }

    /* 写数据缓存赋初值 */
    for (i = 0; i < sizeof(__g_wr_buf); i++) {
        __g_wr_buf[i] = i;
    }

    /* 擦除指定块 */
    ret = am_sdcard_blocks_erase(handle, 1000, __ERASE_BLOCKS);
    if (ret != AM_OK) {
        am_kprintf( "sdcard blocks erase failed\r\n");
    }

    /* 指定块写入数据 */
    ret = am_sdcard_blocks_write(handle, __g_wr_buf, 1000, __WRITE_BLOCKS);
    if (ret != AM_OK) {
        am_kprintf( "sdcard blocks write failed\r\n");
    }

    count = 0;
    while(1) {

        /* 指定块读取数据 */
        ret = am_sdcard_blocks_read(handle,
                                    __g_rd_buf,
                                    1000 + count,
                                    __READ_BLOCKS);

        if (ret == AM_OK) {
            for(i = 0; i < __BLOCK_SIZE * __READ_BLOCKS; i++) {

                am_kprintf("%02x ", __g_rd_buf[i]);

            }
            am_kprintf( "\r\n\r\n");
        }
        count += __READ_BLOCKS;

        am_mdelay(1000);
    }

}
/** [src_demo_std_sdcard] */

/* end of file */
