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
 *
 *
 * \internal
 * \par Modification history
 * - 1.00 18-12-13  yrh, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_int.h"
#include "hw/amhw_lpc84x_clk.h"
#include "hw/amhw_lpc84x_syscon.h"
#include "am_lpc84x.h"
#include "am_lpc84x_inst_init.h"
#include "am_lpc84x_boot_flash.h"
#include "am_arm_boot.h"
#include "am_boot_flash.h"
#include "am_boot_mem_flash.h"
#include "lpc84x_regbase.h"
#include "am_boot_firmware_flash.h"
#include "am_lpc84x_clk.h"
#include "am_arm_nvic.h"
#include "am_boot_msg_flag.h"

static am_boot_flash_handle_t flash_handle;
/*******************************************************************************
 * flash配置
 ******************************************************************************/
/**
 * < \brief bootloader flash 设备信息
 */
static am_lpc84x_boot_flash_devinfo_t __g_flash_devinfo = {

    {
        /** \brief flash的起始地址 */
        0x0,
        /** \brief flash的总的大小 */
        64 * 1024,
        /** \brief flash扇区大小 */
        1024,
        /** \brief flash页大小 */
        64,
    },

    /** \brief 平台初始化函数 */
    NULL,
    /** \brief 平台初解始化函数 */
    NULL,
};

/**
 * < \brief bootloader flash 设备实例
 */
static am_lpc84x_boot_flash_dev_t __g_flash_dev;

/**
 * \brief bootloader flash实例初始化，获得其标准服务句柄
 *
 * \param 无
 *
 * \return bootloader flash标准服务句柄，若为 NULL，表明初始化失败
 */
am_boot_flash_handle_t am_lpc84x_boot_flash_inst_init(void)
{
    return am_lpc84x_boot_flash_init(&__g_flash_dev, &__g_flash_devinfo);
}

/*******************************************************************************
 * 以flag方式的消息传递
 ******************************************************************************/

static am_boot_msg_flag_devinfo_t __g_boot_msg_flag_info = {
    /** \brief 备份区起始地址 */
    0x9C00,
    /** \brief 应用区大小 */
    25 * 1024,
    /** \brief 标志存放地址 */
    0x3800 - 64,
    /** \brief 标志占用的实际大小，一般为内存最小的写入单元，如果使用flash内存，则为最小的擦除单元*/
    64,

    NULL,
};

am_boot_msg_handle_t am_lpc84x_boot_msg_flag_inst_init()
{
    am_boot_mem_handle_t mem_handle = am_boot_mem_flash_init(flash_handle);
    return am_boot_msg_flag_init(&__g_boot_msg_flag_info,
                                  mem_handle);
}

/*******************************************************************************
 * 固件以flash的方式存储配置
 ******************************************************************************/
static am_boot_firmware_flash_devinfo_t __g_firmware_flash_devinfo = {
    /** \brief 固件在内存中存放的起始地址 */
    0x9C00,
    /**< \brief 固件存放区的总大小 */
    25 * 1024,
    /** \brief 驱动存放固件缓冲区大小 , 值为内存写入最小大小的整数倍 */
    64,
    /** \brief 平台初始化函数 */
    NULL,
    /** \brief 平台解初始化函数 */
    NULL,
};

static am_boot_firmware_flash_dev_t __g_firmware_flash_dev;

am_boot_firmware_handle_t am_lpc84x_boot_firmware_flash_inst_init()
{
    flash_handle = am_lpc84x_boot_flash_inst_init();
    return am_boot_firmware_flash_init (&__g_firmware_flash_dev,
                                        &__g_firmware_flash_devinfo,
                                         flash_handle);
}


/*******************************************************************************
 * 标准bootloader用户配置
 ******************************************************************************/

/* 在整个bootloader中用到的资源都要接初始化并释放，常见的如定时器，串口等 */
void __lpc84x_boot_plfm_deinit()
{
    volatile uint32_t i = 1000000;

    uint8_t  channel_nums = 4;
    /* 串口解初始化 ，解初始化前需要小段的延时，保证寄存器任务已经完成*/
    while(i--){

    }
    /* 串口解初始化 */
    amhw_lpc_usart_disable((amhw_lpc_usart_t *)LPC84X_USART0_BASE);
    am_int_disable(INUM_USART0);
    amhw_lpc84x_syscon_periph_reset(AMHW_LPC84X_RESET_UART0);
    amhw_lpc84x_clk_periph_disable(AMHW_LPC84X_CLK_UART0);

    am_gpio_pin_cfg(PIO0_28, PIO0_28_GPIO);
    am_gpio_pin_cfg(PIO1_20, PIO1_20_GPIO);

    /* 软件定时器解初始化*/
    for (i = 0; i < channel_nums; i++) {
        amhw_lpc_mrt_intval_set((amhw_lpc_mrt_t *)LPC84X_MRT_BASE, i, 0, AM_TRUE);

        amhw_lpc_mrt_int_flag_clr((amhw_lpc_mrt_t *)LPC84X_MRT_BASE, i);
        amhw_lpc_mrt_int_pending_clr((amhw_lpc_mrt_t *)LPC84X_MRT_BASE, i);

        amhw_lpc_mrt_int_disable((amhw_lpc_mrt_t *)LPC84X_MRT_BASE, i);
    }

    am_int_disable(INUM_MRT);
    amhw_lpc84x_syscon_periph_reset(AMHW_LPC84X_RESET_MRT);
    amhw_lpc84x_clk_periph_disable(AMHW_LPC84X_CLK_MRT);

    /* 时钟解初始化 */
    amhw_lpc84x_clk_periph_disable(AMHW_LPC84X_CLK_SWM);
    amhw_lpc84x_swm_fixed_func_disable(LPC84X_SWM,
                                      AMHW_LPC84X_SWM_PIO0_8_XTALIN);
    amhw_lpc84x_swm_fixed_func_disable(LPC84X_SWM,
                                      AMHW_LPC84X_SWM_PIO0_9_XTALOUT);
    /* 切换主时钟源 */
    amhw_lpc84x_clk_mainclk_set( AMHW_LPC84X_MAIN_CLK_PLLIN_SRC_MAIN_CLK_PRE_PLL);

    am_arm_nvic_deinit();
    __disable_irq();
}

/**
 * < \brief bootloader标准设备信息
 */
static am_arm_boot_devinfo_t __g_lpc84x_boot_devinfo = {
    /**< \brief 应用代码区起始地址*/
    0x3800,
    /** \brief ram起始地址 */
    0x10000000,
    /** \brief ram大小 */
    16 * 1024,
    /** \brief 平台初始化函数 */
    NULL,
    /** \brief 平台解初始化函数 */
    __lpc84x_boot_plfm_deinit,
};

int am_lpc84x_boot_inst_init()
{
    return am_arm_boot_init(&__g_lpc84x_boot_devinfo);
}

/** @} */

/* end of file */
