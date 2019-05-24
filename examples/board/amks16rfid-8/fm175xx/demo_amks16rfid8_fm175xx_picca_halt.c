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
 * \brief fm175xx使用不同的方法读取卡片信息，通过驱动接口实现
 *
 * - 操作步骤：
 *   1. 正确连接并配置好串口。
 *   2. 正确连接好天线。
 *   3. 将A类卡置于天线感应区。
 *
 * - 实验现象：
 *   1. 如果卡片支持halt指令，则每次重新将卡放置天线感应区后，串口只能打印出两次卡片信息。
 *   2. 如果卡片不支持halt指令，则将卡放置天线感应区后，串口能连续不断的打印出卡片信息。
 *
 * \par 源代码
 * \snippet demo_amks16rfid8_fm175xx_picca_halt.c src_amks16rfid8_fm175xx_picca_halt
 *
 * \internal
 * \par Modification history
 * - 1.00 17-11-17  sdq, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_kl26_if_dr_fm175xx_picca_halt
 * \copydoc demo_amks16rfid8_fm175xx_picca_halt.c
 */

/** [src_amks16rfid8_fm175xx_picca_halt] */
#include "am_fm175xx.h"
#include "am_fm175xx_reg.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "kl26_pin.h"
#include "am_kl26_inst_init.h"
#include "am_fm175xx_ant.h"
#include "am_hwconf_fm175xx.h"
#include "demo_components_entries.h"
#include "demo_amks16rfid8_entries.h"

/**
 * \name 定义核心板相关固定引脚
 * @{
 */
#define __CD4051_PIN_EN  PIOB_19         /**< \brief   CD4051通道控制芯片 使能引脚  */
#define __CD4051_PIN_S2  PIOB_18         /**< \brief   CD4051通道控制芯片S2引脚  */
#define __CD4051_PIN_S1  PIOB_17         /**< \brief   CD4051通道控制芯片S1引脚  */
#define __CD4051_PIN_S0  PIOB_16         /**< \brief   CD4051通道控制芯片S0引脚  */

#define __ANT_ENABLE_PIN PIOE_29         /**<\brief   天线升压芯片使能引脚  */

/* 定义 fm175xx 天线切换信息 */
static am_antenna_info_t  __g_antenna_info = {
    {
        __CD4051_PIN_EN,
        __CD4051_PIN_S0,
        __CD4051_PIN_S1,
        __CD4051_PIN_S2
    },
    AM_FM175XX_ANTENNA_EIGHT
};

/**
 * \brief A类卡读休眠卡例程
 */
void demo_amks16rfid8_fm175xx_picca_halt (void)
{
    am_fm175xx_handle_t handle;

    /* B版本核心板存在此引脚   需要拉高使能天线
     * 若为A版本核心板则可对该行代码进行注释
     */
    am_gpio_pin_cfg(__ANT_ENABLE_PIN, AM_GPIO_OUTPUT_INIT_HIGH);

    am_cd4051_pin_init(&__g_antenna_info);
    /* 选择天线1 */
    am_cd4051_channel_selected(&__g_antenna_info, 0);
    
    handle = am_fm175xx_inst_init();

    demo_fm175xx_picca_halt(handle);

}

/** [src_amks16rfid8_fm175xx_picca_halt] */

/* end of file */
