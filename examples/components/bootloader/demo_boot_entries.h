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
 * \brief STD所有例程函数入口声明
 * \sa    demo_std_entries.h
 *
 * \internal
 * \par Modification history
 * - 1.00 17-05-02  sdy, first implementation
 * \endinternal
 */

#ifndef __DEMO_BOOT_ENTRIES_H
#define __DEMO_BOOT_ENTRIES_H


#include "am_boot_firmware_recv.h"
#include "am_boot_enter_check.h"
#include "am_boot_flash.h"
#include "am_boot_memory.h"
#include "am_boot_msg.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief bootloader 单区例程
 *
 * \return 无
 */
void demo_std_single_bootloader_entry(am_boot_firwa_recv_handle_t  firwa_recv_handle,
                                      am_boot_enter_check_handle_t enter_check_handle);

/**
 * \brief bootloader 基于nxp KinetisFlashTool的例程
 *
 * \return 无
 */
void demo_std_bootloader_kft_entry(am_boot_flash_handle_t    flash_handle,
                                   am_boot_mem_handle_t      memory_handle,
                                   uint32_t                  baund,
                                   am_uart_handle_t          uart_handle,
                                   uint32_t                  ram_start_addr,
                                   uint32_t                  ram_size);

/**
 * \brief bootloader 双区的例程
 *
 * \return 无
 */
void demo_std_double_bootloader_entry(am_boot_firwa_recv_handle_t  firwa_recv_handle,
                                      am_boot_enter_check_handle_t enter_check_handle,
                                      am_boot_msg_handle_t         msg_handle);

/**
 * \brief bootloader 双区应用的例程
 *
 * \return 无
 */
void demo_std_double_application_entry(am_boot_firwa_recv_handle_t  firwa_recv_handle,
                                       am_boot_enter_check_handle_t check_handle,
                                       am_boot_msg_handle_t         msg_handle);
#ifdef __cplusplus
}
#endif

#endif /* __DEMO_BOOT_ENTRIES_H */

/* end of file */
