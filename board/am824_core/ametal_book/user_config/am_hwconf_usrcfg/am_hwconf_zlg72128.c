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
 * \brief zlg72128 AMetal平台适配ZLG72128通用驱动包
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-21 adw, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "zlg72128.h"
#include "lpc82x_pin.h"
#include "am_lpc82x_inst_init.h"

/** \brief  定义一个  ZLG72128  实例*/
static zlg72128_dev_t      __g_zlg72128_dev;

static const zlg72128_devinfo_t __g_zlg72128_devinfo = {
    {
        0x30,        /* 设备从机地址 */
        PIO0_6,      /* 复位引脚 */
        AM_TRUE,     /* 通常情况下，均会使用中断引脚 */
        PIO0_1,      /* ZLG72128 的 KEY_INT 引脚与 LPC824 的 PIO0_1 连接 */
        10,          /* 使用中断引脚时，该值无意义。若不使用中断引脚，则查询间隔为 5ms */
        am_lpc82x_i2c2_inst_init,  /* I2C句柄获取函数 */
        am_lpc82x_i2c2_inst_deinit /* I2C解初始化函数 */
    }
};

zlg72128_handle_t  am_zlg72128_inst_init (void)
{
     return zlg72128_init(&__g_zlg72128_dev,
                          &__g_zlg72128_devinfo);
}
