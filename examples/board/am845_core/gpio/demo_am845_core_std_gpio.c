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
 * \brief GPIO 例程，通过标准接口实现
 *
 * - 操作步骤：
 *   1. 使输入引脚为低电平。
 *
 * - 实验现象：
 *   2. 输入引脚为低电平时，输出引脚状态翻转。
 *
 * \par 源代码
 * \snippet demo_std_gpio.c src_std_gpio
 *
 * \internal
 * \par Modification History
 * - 1.00 17-04-15  nwt, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am845_core_std_gpio
 * \copydoc demo_am845_core_std_gpio.c
 */

/** [src_am845_core_std_gpio] */
#include "ametal.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_lpc84x_gpio.h"
#include "demo_std_entries.h"

/** \brief LED0 引脚 */
#define __LED0_PIN  PIO1_8

/** \brief 按键引脚 */
#define __KEY_PIN  PIO1_11

/**
 * \brief 例程入口
 */
void demo_am845_core_std_gpio_entry (void)
{

    AM_DBG_INFO("demo am845_core std gpio!\r\n");

    demo_std_gpio_entry(__KEY_PIN, __LED0_PIN);
}
/** [src_am845_core_std_gpio] */

/* end of file */
