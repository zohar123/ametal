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
 * \brief ZMF159 SDIO 用户配置文件
 * \sa am_hwconf_zmf159_sdio.c
 *
 * \internal
 * \par Modification history
 * - 1.00 19-06-13  ipk, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_zmf159.h"
#include "am_gpio.h"
#include "am_clk.h"
#include "am_zmf159_sdio.h"
#include "am_zmf159_clk.h"
#include "hw/amhw_zmf159_sdio.h"

/**
 * \addtogroup am_if_src_hwconf_zmf159_sdio
 * \copydoc am_hwconf_zmf159_sdio.c
 * @{
 */

/** \brief SDIO 平台初始化函数 */
static void __zmf159_sdio_plfm_init (void)
{

    am_gpio_pin_cfg(PIOC_8, PIOC_8_SDIO_D0 | PIOC_8_INPUT_PU | PIOC_8_SPEED_20MHz);
    am_gpio_pin_cfg(PIOC_9, PIOC_9_SDIO_D1 | PIOC_9_INPUT_PU | PIOC_9_SPEED_20MHz);
    am_gpio_pin_cfg(PIOC_10, PIOC_10_SDIO_D2 | PIOC_10_INPUT_PU | PIOC_10_SPEED_20MHz);
    am_gpio_pin_cfg(PIOC_11, PIOC_11_SDIO_D3 | PIOC_11_INPUT_PU | PIOC_11_SPEED_20MHz);
    am_gpio_pin_cfg(PIOC_12, PIOC_12_SDIO_CK | PIOC_12_INPUT_PU | PIOC_12_SPEED_20MHz);
    am_gpio_pin_cfg(PIOD_2, PIOD_2_SDIO_CMD | PIOD_2_INPUT_PU | PIOD_2_SPEED_20MHz);

    am_clk_enable(CLK_SDIO);
    am_zmf159_clk_reset(CLK_SDIO);
}

/** \brief 解除SDIO 平台初始化函数 */
static void __zmf159_sdio_plfm_deinit (void)
{
    am_clk_disable(CLK_SDIO);
}

/**
 * \brief I2C1 设备信息
 */
static const am_zmf159_sdio_devinfo_t __g_sdio_devinfo = {

    ZMF159_SDIO_BASE,                 /**< \brief 寄存器块基址 */
    INUM_SDIO,                        /**< \brief 中断编号 */
    CLK_SDIO,                         /**< \brief 时钟ID值 */
    AMHW_ZLG_SDIO_SPEED_MODE_LOW,     /**< \brief 低速模式 */
    __zmf159_sdio_plfm_init,          /**< \brief 平台初始化 */
    __zmf159_sdio_plfm_deinit         /**< \brief 平台去初始化 */
};

static am_zmf159_sdio_dev_t __g_sdio_dev;        /**< \brief  设备实例 */
static am_sdio_handle_t  __g_sdio_handle = NULL; /**< \brief  标准服务句柄 */

/** \brief SDIO 实例初始化，获得SDIO标准服务句柄 */
am_sdio_handle_t am_zmf159_sdio_inst_init (void)
{
    if (NULL == __g_sdio_handle) {
        __g_sdio_handle = am_zmf159_sdio_init(&__g_sdio_dev, &__g_sdio_devinfo);
    }

    return __g_sdio_handle;
}

/** \brief I2C1 实例解初始化 */
void am_zmf159_sdio_inst_deinit (am_sdio_handle_t handle)
{
    am_zmf159_sdio_deinit(handle);
    __g_sdio_handle = NULL;
}


/** @} */

/* end of file */
