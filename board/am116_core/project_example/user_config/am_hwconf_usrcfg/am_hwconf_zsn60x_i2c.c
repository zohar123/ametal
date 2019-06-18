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
 * \brief ZSN60x(I2C模式)配置文件
 *
 * \internal
 * \par Modification history
 * - 1.00 19-04-22  htf, first implementation.
 * \endinternal
 */
#include "ametal.h"
#include "am_i2c.h"
#include "zsn60x.h"
#include "zlg116_pin.h"
#include "am_zsn60x.h"
#include "zsn60x_platform.h"
#include "am_zlg116_inst_init.h"
#include "am_hwconf_zsn60x_i2c.h"

/**< \brief ZSN60x(I2C模式) 设备实例 */
static   zsn60x_i2c_dev_t          __g_zsn60x_i2c_dev;
/**< \brief ZSN60x(I2C模式) 设备信息 */
static const zsn60x_i2c_devinfo_t  __g_i2c_info = {
    0xb2,
    {
        -1,
        PIOA_12,
        am_zlg116_i2c1_inst_init,
        am_zlg116_i2c1_inst_deinit
    }
};
/** \brief ZSN60x实例初始化，获得ZSN60x标准服务句柄(i2c模式) */
zsn60x_handle_t  am_zsn60x_i2c_inst_init (void)
{
    return zsn60x_i2c_init(&__g_zsn60x_i2c_dev, &__g_i2c_info);
}

