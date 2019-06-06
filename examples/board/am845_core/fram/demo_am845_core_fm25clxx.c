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
 * \brief F-RAM 例程，通过标准接口实现
 *
 *
 * - 实验现象：
 *   1. 主机写数据到 F-RAM；
 *   2. 主机从 F-RAM 读取数据，并通过串口打印处理；
 *   3. 串口打印出测试结果。
 *
 * \internal
 * \par Modification history
 * - 1.00  19-06-06  zjr, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am845_core_std_gpio
 * \copydoc demo_am845_core_std_gpio.c
 */

/** [src_am845_core_std_gpio] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_lpc84x_inst_init.h"
#include "demo_components_entries.h"


/**
 * \brief 例程入口
 */
void demo_am845_core_fm25clxx_entry (void)
{
    AM_DBG_INFO("demo am845_core fm25clxx!\r\n");

    demo_fm25clxx_entry(am_fm25clxx_inst_init(), 8);
}
/** [src_am845_core_std_gpio] */

/* end of file */
