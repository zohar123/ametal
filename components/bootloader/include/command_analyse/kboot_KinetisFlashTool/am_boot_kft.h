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
 * \brief bootloader kboot KinetisFlashTool 初始化
 *
 *
 * \internal
 * \par Modification history
 * - 1.00 18-11-25  yrh, first implementation
 * \endinternal
 */
#ifndef __AM_BOOT_KFT_H
#define __AM_BOOT_KFT_H

#include "am_boot_kft_command.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct am_boot_kft_dev {
    am_boot_kft_command_handle_t  command_handle;
}am_boot_kft_dev_t;

/**< \brief Bootloader status codes */
enum am_boot_kft_bootloader_status
{
    AM_BOOT_KFT_STATUS_UNKNOWN_COMMAND      = AM_BOOT_KFT_MAKE_STATUS(AM_BOOT_KFT_STATUS_GROUP_BOOTLOADER, 0),
    AM_BOOT_KFT_STATUS_SECURITY_VIOLATION   = AM_BOOT_KFT_MAKE_STATUS(AM_BOOT_KFT_STATUS_GROUP_BOOTLOADER, 1),
    AM_BOOT_KFT_STATUS_ABORT_DATA_PHASE     = AM_BOOT_KFT_MAKE_STATUS(AM_BOOT_KFT_STATUS_GROUP_BOOTLOADER, 2),
    AM_BOOT_KFT_STATUS_PING                 = AM_BOOT_KFT_MAKE_STATUS(AM_BOOT_KFT_STATUS_GROUP_BOOTLOADER, 3),
    AM_BOOT_KFT_STATUS_NO_RESPONSE          = AM_BOOT_KFT_MAKE_STATUS(AM_BOOT_KFT_STATUS_GROUP_BOOTLOADER, 4),
    AM_BOOT_KFT_STATUS_NO_RESPONSE_EXPECTED = AM_BOOT_KFT_MAKE_STATUS(AM_BOOT_KFT_STATUS_GROUP_BOOTLOADER, 5)
};

/**
 * \brief bootloader 组件初始化
 */
int am_boot_kft_init(am_boot_mem_handle_t     memory_handle,
                     am_boot_flash_handle_t   flash_handle,
                     am_boot_serial_handle_t  serial_handle,
                     uint32_t                 ram_start_addr,
                     uint32_t                 ram_size);


/**
 * \brief bootloader kboot KinetisFlashTool 命令处理(命令状态机)
 *
 * \note 该函数需要用户在应用程序里面循环调用，上位机会一直发送信息给bootloader,状态机需要一直去解析
 */
int am_boot_kft_command_pump(void);

#if defined(__cplusplus)
}
#endif

#endif /* __AM_BOOT_KFT_H */

/* end of file */
