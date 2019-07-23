/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2019 Guangzhou ZHIYUAN Electronics Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
*******************************************************************************/

/**
 * \file
 * \brief ZLG72128 标准通用用户配置文件
 * \sa am_hwconf_zlg72128_std.c
 *
 * \internal
 * \par Modification history
 * - 1.00 19-04-09 xgg, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_input.h"
#include "am_lpc82x.h"
#include "am_zlg72128_std.h"
#include "am_lpc82x_inst_init.h"

/**
 * \addtogroup am_if_src_hwconf_miniport_zlg72128
 * \copydoc am_hwconf_moniport_zlg72128.c
 * @{
 */

/** \brief ZLG72128 按键编码信息 */
am_local am_const int __g_miniport_zlg72128_codes[] = {
    KEY_0,    /* 按键 KEY0 的键值 */
    KEY_1,    /* 按键 KEY1 的键值 */
    KEY_2,    /* 按键 KEY2 的键值 */
    KEY_3     /* 按键 KEY3 的键值 */
};

/** \brief MiniPort-ZLG72128 设备信息 */
am_local const am_zlg72128_std_devinfo_t __g_miniport_zlg72128_devinfo = {
    {
        {
            0x30,
            PIO0_6,            /*  复位引脚 */
            AM_TRUE,           /*  使用中断引脚*/
            PIO0_1,            /*  中断引脚*/
            5,                 /*  查询时间间隔，使用中断引脚时，该值无意义*/
            am_lpc82x_i2c2_inst_init,   /* I2C句柄获取函数*/
            am_lpc82x_i2c2_inst_deinit  /* I2C解初始化函数*/
        }
    },
    {
        0                          /* 数码管显示器的编号 */
    },
    500,                           /* 一个闪烁周期内，点亮的时间为 500ms */
    500,                           /* 一个闪烁周期内，熄灭的时间为 500ms */
    AM_ZLG72128_STD_KEY_ROW_0 | AM_ZLG72128_STD_KEY_ROW_3, /* 实际使用的行标志 */
    AM_ZLG72128_STD_KEY_COL_0 | AM_ZLG72128_STD_KEY_COL_1, /* 实际使用的列标志 */
    __g_miniport_zlg72128_codes,    /* 按键编码信息 */
    2                               /* 数码管个数为 2 */
};

/** \brief ZLG72128 设备实例 */
am_local am_zlg72128_std_dev_t __g_miniport_zlg72128_dev;

/**
 * \brief ZLG72128 标准通用管理初始化
 */
int am_miniport_zlg72128_inst_init (void)
{
    return am_zlg72128_std_init(&__g_miniport_zlg72128_dev,
                                &__g_miniport_zlg72128_devinfo);
}

/**
 * @}
 */

/* end of file */
