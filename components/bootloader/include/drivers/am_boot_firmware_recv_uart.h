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
 * \brief 固件接收标准设备实现（uart驱动）
 *
 * \internal
 * \par modification history:
 * - 1.00 18-11-30  yrh, first implementation.
 * \endinternal
 */

#ifndef __AM_BOOT_FIRMWARE_RECV_UART_H
#define __AM_BOOT_FIRMWARE_RECV_UART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_boot_flash.h"
#include "am_boot_firmware.h"
#include "am_boot_firmware_recv.h"
/**
 * \brief 固件接收的设备（串口传输实现）
 */
typedef struct am_boot_firwa_recv_uart_dev {
    /**< \brief 标准的固件接收服务  */
    am_boot_firwa_recv_serv_t        isa;
    /**< \brief 固件存放的标准服务操作句柄  */
    am_boot_firmware_handle_t        firmware_handle;
    /**< \brief 串口的标准服务操作句柄  */
    am_uart_handle_t                 uart_handle;
} am_boot_firwa_recv_uart_dev_t;

/**
 * \brief 固件接收设备初始化（串口传输实现）
 */
am_boot_firwa_recv_handle_t am_boot_firwa_recv_uart_init(
    am_boot_firmware_handle_t firmware_handle, am_uart_handle_t uart_handle);


#ifdef __cplusplus
}
#endif

#endif /* __AM_BOOT_FIRMWARE_RECV_UART_H */

/* end of file */
