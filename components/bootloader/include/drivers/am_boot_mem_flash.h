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
 * \brief bootloader 内存操作（flash驱动）
 *
 * \internal
 * \par modification history:
 * - 1.00 18-11-30  yrh, first implementation.
 * \endinternal
 */

#ifndef __AM_BOOT_FLASH_MEMORY_H
#define __AM_BOOT_FLASH_MEMORY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_boot_memory.h"
#include "am_boot_flash.h"

/**
 * \brief 内存操作设备（flash实现）
 */
typedef struct am_boot_mem_flash_dev {
    am_boot_mem_serv_t            isa;          /**< \brief 标准的 memory 服务  */
    am_boot_flash_handle_t        flash_handle; /**< \brief flash的标准服务操作句柄  */
} am_boot_mem_flash_dev_t;

/**
 * \brief 内存操作设备初始化（flash实现）
 *
 * \return flash 标准服务操作句柄
 */
am_boot_mem_handle_t am_boot_mem_flash_init(am_boot_flash_handle_t flash_handle);


#ifdef __cplusplus
}
#endif

#endif /* __AM_BOOT_FLASH_MEMORY_H */

/* end of file */
