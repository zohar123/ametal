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
 * \brief bootloader 使用kboot KinetisFlashTool上位机例程
 *
 *
 * \par 源代码
 * \snippet demo_std_bootloader_kft.c src_std_bootloader_kft
 *
 * \internal
 * \par Modification history
 * - 1.00 19-0-29 yrh, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_bootloader_kft
 * \copydoc demo_std_bootloader_kft.c
 */

/** [src_std_bootloader_kft] */
#include "am_boot_serial_uart.h"
#include "am_boot_flash.h"
#include "am_boot_memory.h"
#include "am_boot_kft.h"

void demo_std_bootloader_kft_entry(am_boot_flash_handle_t    flash_handle,
                                   am_boot_mem_handle_t      memory_handle,
                                   uint32_t                  baund,
                                   am_uart_handle_t          uart_handle,
                                   uint32_t                  ram_start_addr,
                                   uint32_t                  ram_size)
{
    /* 基于串口的串行数据传输 */
    am_boot_serial_handle_t serial_handle = am_boot_serial_uart_init(
        uart_handle, baund);
    /* 命令解析模块初始化 */
    am_boot_kft_init(memory_handle,
                     flash_handle,
                     serial_handle,
                     ram_start_addr,
                     ram_size);
    /* 循环检测上位机消息 */
    while(1) {
        am_boot_kft_command_pump();
    }
}
/** [src_std_bootloader_kft] */

/* end of file */
