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
 *   3. 调试串口打印测试结果。
 *
 * \par 源代码
 * \snippet demo_zmf159_core_std_sdcard.c src_zmf159_core_std_sdcard
 *
 * \internal
 * \par Modification history
 * - 1.00 19-06-14  ipk, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zmf159_core_std_sdcard
 * \copydoc demo_zmf159_core_std_sdcard.c
 */

/** [src_zmf159_core_std_sdcard] */
#include "ametal.h"
#include "am_sdio.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_sdcard.h"
#include "demo_std_entries.h"
#include "am_zmf159_inst_init.h"

/**
 * \brief 例程入口
 */
void demo_zmf159_core_std_sdcard_entry (void)
{
    AM_DBG_INFO("demo zmf159_core std sdcard!\r\n");

    demo_std_sdcard_entry(am_sdcard_inst_init());
}
/** [src_zmf159_core_std_sdcard] */

/* end of file */
