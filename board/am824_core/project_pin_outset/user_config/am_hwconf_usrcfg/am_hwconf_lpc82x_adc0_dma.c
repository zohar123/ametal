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
 * \brief LPC82X ADC (DMA 方式) 用户配置文件
 * \sa am_hwconf_lpc82x_adc0_dma.c
 *
 * \internal
 * \par Modification history
 * - 1.01 15-12-14  hgo, add calibrate func
 * - 1.00 15-07-15  bob, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_lpc82x.h"
#include "am_lpc82x_adc_dma.h"
#include "hw/amhw_lpc82x_clk.h"
#include "hw/amhw_lpc82x_adc.h"
#include "hw/amhw_lpc82x_syscon.h"

/**
 * \addtogroup am_if_src_hwconf_lpc82x_adc0_dma
 * \copydoc am_hwconf_lpc82x_adc0_dma.c
 * @{
 */

/**
 * \brief ADC0 平台初始化
 */
am_local void __lpc82x_adc0_dma_plfm_init (void)
{
    amhw_lpc82x_syscon_powerup(AMHW_LPC82X_SYSCON_PD_ADC0);
    amhw_lpc82x_clk_periph_enable(AMHW_LPC82X_CLK_ADC0);

    /* 配置通道，引脚配置为消极模式 INACTIVE */
    am_gpio_pin_cfg(__ADC_IN0_PIN,  AM_LPC82X_GPIO_FUNC_OTHER1 | AM_GPIO_FLOAT);
    am_gpio_pin_cfg(__ADC_IN1_PIN,  AM_LPC82X_GPIO_FUNC_OTHER2 | AM_GPIO_FLOAT);
    am_gpio_pin_cfg(__ADC_IN2_PIN,  AM_LPC82X_GPIO_FUNC_OTHER2 | AM_GPIO_FLOAT);
    am_gpio_pin_cfg(__ADC_IN3_PIN,  AM_LPC82X_GPIO_FUNC_OTHER2 | AM_GPIO_FLOAT);
    am_gpio_pin_cfg(__ADC_IN4_PIN,  AM_LPC82X_GPIO_FUNC_OTHER1 | AM_GPIO_FLOAT);
    am_gpio_pin_cfg(__ADC_IN5_PIN,  AM_LPC82X_GPIO_FUNC_OTHER1 | AM_GPIO_FLOAT);
    am_gpio_pin_cfg(__ADC_IN6_PIN,  AM_LPC82X_GPIO_FUNC_OTHER1 | AM_GPIO_FLOAT);
    am_gpio_pin_cfg(__ADC_IN7_PIN,  AM_LPC82X_GPIO_FUNC_OTHER1 | AM_GPIO_FLOAT);
    am_gpio_pin_cfg(__ADC_IN8_PIN,  AM_LPC82X_GPIO_FUNC_OTHER1 | AM_GPIO_FLOAT);
    am_gpio_pin_cfg(__ADC_IN9_PIN,  AM_LPC82X_GPIO_FUNC_OTHER1 | AM_GPIO_FLOAT);
    am_gpio_pin_cfg(__ADC_IN10_PIN, AM_LPC82X_GPIO_FUNC_OTHER1 | AM_GPIO_FLOAT);
    am_gpio_pin_cfg(__ADC_IN11_PIN, AM_LPC82X_GPIO_FUNC_OTHER1 | AM_GPIO_FLOAT);

    /* ADC 自动矫正 */
    amhw_lpc82x_adc_calibrate(LPC82X_ADC0,
                              amhw_lpc82x_clk_system_clkrate_get());
}

/**
 * \brief ADC0 平台解初始化
 */
am_local void __lpc82x_adc0_dma_plfm_deinit (void)
{
    amhw_lpc82x_clk_periph_disable(AMHW_LPC82X_CLK_ADC0);
    amhw_lpc82x_syscon_powerdown(AMHW_LPC82X_SYSCON_PD_ADC0);
}

/** \brief ADC0 (DMA 方式) 设备信息 */
am_local am_const am_lpc82x_adc_dma_devinfo_t __g_lpc82x_adc0_dma_devinfo = {
    LPC82X_ADC0_BASE,                 /* ADC0 寄存器块基地址 */
    CLK_ADC0,                         /* ADC0 时钟号 */
    2500,                             /* ADC 参考电压，单位：mV */
    INUM_ADC0_SEQA,                   /* ADC0 序列 A 中断号 */
    DMA_CHAN_0,                       /* ADC DMA 通道 0 */
    __lpc82x_adc0_dma_plfm_init,      /* 平台初始化函数 */
    __lpc82x_adc0_dma_plfm_deinit,    /* 平台解初始化函数 */
};

/** \brief ADC0 (DMA 方式) 设备实例 */
am_local am_lpc82x_adc_dma_dev_t __g_lpc82x_adc0_dma_dev;

/**
 * \brief ADC0 (DMA 方式) 实例初始化
 */
am_adc_handle_t am_lpc82x_adc0_dma_inst_init (void)
{
    return am_lpc82x_adc_dma_init(&__g_lpc82x_adc0_dma_dev,
                                  &__g_lpc82x_adc0_dma_devinfo);
}

/**
 * \brief ADC0 (DMA 方式) 实例解初始化
 */
void am_lpc82x_adc0_dma_inst_deinit (am_adc_handle_t handle)
{
    am_lpc82x_adc_dma_deinit(handle);
}

/**
 * @}
 */

/* end of file */
