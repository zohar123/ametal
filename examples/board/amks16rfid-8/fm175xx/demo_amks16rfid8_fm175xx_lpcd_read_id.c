/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2017 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief FM175xx LPCD模式
 *
 * - 操作步骤：
 *   1. 正确连接并配置好串口。
 *   2. 正确连接好天线。
 *
 * - 实验现象：
 *   1. 在执行操作步骤1 2后， 随后卡进入低功耗自动卡片检测（LPCD）模式
 *   2. 将卡放置感应区，FM175xx设备将产生卡进场中断标志，执行相应设定操作。
 *   3. 移开卡片后，达到设定的AUTO_WUP_TIME的时间时，FM175xx自动退出LPCD模式，自动进行调校。
 *
 * \par 源代码
 * \snippet demo_amks16rfid8_m175xx_picca_lpcd_mode.c demo_amks16rfid8_fm175xx_picca_lpcd_mode
 *
 * \internal
 * \par Modification history
 * - 1.00 18-08-20  htf, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_amks16rfid8_fm175xx_picca_lpcd_mode
 * \copydoc demo_amks16rfid8_fm175xx_picca_lpcd_mode.c
 */

/** [src_demo_amks16rfid8_fm175xx_picca_lpcd_mode] */

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
 * \brief LPCD模式
 */
void demo_amks16rfid8_fm175xx_lpcd_read_id (void)
{
    am_fm175xx_handle_t handle;

    /* B版本核心板存在此引脚   需要拉高使能天线
     * 若为A版本核心板则可对该行代码进行注释
     */
    am_gpio_pin_cfg(__ANT_ENABLE_PIN, AM_GPIO_OUTPUT_INIT_HIGH);

    /* 初始化天线并选择天线1   注意此处一定要先选择天线*/
    am_cd4051_pin_init(&__g_antenna_info);
    am_cd4051_channel_selected(&__g_antenna_info, 0);

    handle = am_fm175xx_inst_init();

    demo_fm175xx_picca_lpcd_mode(handle);
}

/** [demo_amks16rfid8_fm175xx_picca_lpcd_mode] */

/* end of file */
