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
 * \brief bootloader 串行数据处理标准设备实现（uart驱动）
 *
 * \internal
 * \par modification history:
 * - 1.00 18-12-11  yrh, first implementation.
 * \endinternal
 */

#ifndef __AM_BOOT_SERIAL_UART_H
#define __AM_BOOT_SERIAL_UART_H

#include "am_boot_serial_byte.h"
#include "am_boot_autobaud.h"
#include "am_uart.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief serial数据操作的设备（uart驱动 ）
 */
typedef struct am_boot_serial_uart_dev {
    am_boot_serial_byte_serv_t    serial_serv;      /**< \brief 标准的 串行数据操作 服务    */
    am_uart_handle_t              uart_handle;      /**< \brief 标准的uart操作句柄  */
} am_boot_serial_uart_dev_t;

/**
 * \brief serial数据操作的设备初始化（uart驱动 ）
 */
am_boot_serial_handle_t am_boot_serial_uart_init(am_uart_handle_t uart_handle,
                                                 uint32_t         baund);
/**
 * \brief serial数据操作的设备解初始化（uart驱动 ）
 */
void am_boot_serial_uart_deinit(void);


#ifdef __cplusplus
}
#endif

#endif /* __AM_BOOT_SERIAL_UART_H */

/* end of file */
