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
 * \brief ZLG217 kboot KinetisFlashTool 用户配置文件
 *
 *
 *
 * \internal
 * \par Modification history
 * - 1.00 18-12-13  yrh, first implementation
 * \endinternal
 */


/**
 * \addtogroup am_if_src_bootconf_zlg217_kft
 * \copydoc am_bootconf_zlg217_kft.c
 * @{
 */

#include "am_arm_boot.h"
#include "am_boot_kft.h"
#include "ametal.h"
#include "am_gpio.h"
#include "am_zlg217.h"
#include "am_zlg217_inst_init.h"
#include "am_boot_serial_uart.h"
#include "am_zlg_boot_flash.h"
#include "am_boot.h"
#include "am_boot_autobaud_soft.h"
#include "am_boot_flash.h"
#include "am_zlg_tim_cap.h"
#include "am_zlg_flash.h"
#include "zlg217_regbase.h"
#include "am_int.h"
#include "am_zlg217_clk.h"
#include "am_arm_nvic.h"

/**
 * \name 自动波特率检测宏定义
 * @{
 */

/** \brief 需要用到的定时器位数 */
#define     TIMER_WIDTH           16
static am_cap_handle_t   cap_handle;
/** @} */

/*******************************************************************************
 * 自动波特率检测配置
 ******************************************************************************/

/** \brief 自动波特率的平台初始化 */
void __zlg217_plfm_autobaud_init (void)
{
    amhw_zlg_tim_prescale_set((amhw_zlg_tim_t *)ZLG217_TIM2_BASE, (uint16_t)8);
}

/** \brief 自动波特率的平台解初始化  */
void __zlg217_plfm_autobaud_deinit (void)
{
    am_zlg217_tim1_cap_inst_deinit (cap_handle);
}

/** \brief 自动波特率的设备信息实例 */
static am_boot_autobaud_soft_devinfo_t __g_zlg217_boot_autobaud_devinfo = {

    2,                            /**< \brief CAP捕获通道号 */
    TIMER_WIDTH,                  /**< \brief TIMER定时器位数 */
    10,                           /**< \brief 接收一次数据的超时时间(ms)*/
    AM_CAP_TRIGGER_FALL,          /**< \brief CAP捕获触发方式 */
    __zlg217_plfm_autobaud_init,  /**< \brief 平台初始化函数 */
    __zlg217_plfm_autobaud_deinit,/**< \brief 平台解初始化函数 */
};

/** \brief 自动波特率功能的设备实例 */
am_boot_autobaud_soft_dev_t  __g_zlg217_boot_autobaud_dev;

/** \brief 实例初始化，获得自动波特率服务句柄 */
am_boot_autobaud_handle_t am_zlg217_boot_autobaud_inst_init (void)
{
    cap_handle = am_zlg217_tim1_cap_inst_init();

    am_zlg_tim_cap_dev_t *p_cap_dev = (am_zlg_tim_cap_dev_t *)cap_handle;
    int cap_pin =
        p_cap_dev->p_devinfo->p_ioinfo[__g_zlg217_boot_autobaud_devinfo.cap_chanel].gpio;

    return am_boot_autobaud_soft_init(&__g_zlg217_boot_autobaud_dev,
                                      &__g_zlg217_boot_autobaud_devinfo,
                                       cap_handle,
                                       cap_pin);
}

/*******************************************************************************
 * flash配置
 ******************************************************************************/
/**
 * < \brief bootloader flash 设备信息
 */
static am_zlg_boot_flash_devinfo_t __g_flash_devinfo = {

    {
        /** \brief flash的起始地址 */
        0x08000000,
        /** \brief flash的总的大小 */
        128 * 1024,
        /** \brief flash扇区大小 */
        1024,
        /** \brief flash页大小 */
        4,
    },
    /** \brief flash寄存器的基地址 */
    ZLG217_FLASH_BASE,
    /** \brief 平台初始化函数 */
    NULL,
    /** \brief 平台初解始化函数 */
    NULL,
};

/**
 * < \brief bootloader flash 设备实例
 */
static am_zlg_boot_flash_dev_t __g_flash_dev;

/**
 * \brief bootloader flash实例初始化，获得其标准服务句柄
 *
 * \param 无
 *
 * \return bootloader flash标准服务句柄，若为 NULL，表明初始化失败
 */
am_boot_flash_handle_t am_zlg217_boot_kft_flash_inst_init(void)
{
    return am_zlg_boot_flash_init(&__g_flash_dev, &__g_flash_devinfo);
}

/*******************************************************************************
 * 标准bootloader接口配置
 ******************************************************************************/

static void __zlg217_boot_kft_plfm_deinit()
{
    volatile uint32_t i = 1000000;
    /* 串口解初始化 ，解初始化前需要小段的延时，保证寄存器任务已经完成*/
    while(i--){

    }
    am_clk_disable(CLK_UART1);
    am_gpio_pin_cfg(PIOA_9, AM_GPIO_INPUT);
    am_gpio_pin_cfg(PIOA_10,AM_GPIO_INPUT);

    amhw_zlg_uart_disable((amhw_zlg_uart_t *)ZLG217_UART1_BASE);
    am_int_disable(INUM_UART1);

    /* 定时器解初始化  */
    amhw_zlg_tim_int_disable(ZLG217_TIM4, AMHW_ZLG_TIM_UIE);
    amhw_zlg_tim_disable(ZLG217_TIM4);
    am_int_disable(INUM_TIM4);
    am_zlg217_clk_reset(CLK_TIM4);
    am_clk_disable(CLK_TIM4);

    am_arm_nvic_deinit();
    __disable_irq();
}

/**
 *  \brief bootloader标准设备信息
 */
static am_arm_boot_devinfo_t __g_zlg217_boot_devinfo = {
    /**< \brief 应用代码区起始地址*/
    0x08007000,
    /** \brief ram起始地址 */
    0x20000000,
    /** \brief ram结束地址 */
    20 * 1024,
    /** \brief 平台初始化函数 */
    NULL,
    /** \brief 平台解初始化函数 */
    __zlg217_boot_kft_plfm_deinit,
};

/**
 *  \brief bootloader标准设备实例
 */
int am_zlg217_boot_kft_inst_init()
{
    return am_arm_boot_init(&__g_zlg217_boot_devinfo);
}

/** @} */

/* end of file */
