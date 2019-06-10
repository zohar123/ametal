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
 * \brief 进入应用程序检测（串口指定命令实现）
 *
 * \internal
 * \par Modification history
 * - 1.00 19-5-23  yrh, first implementation
 * \endinternal
 */

#ifndef __AM_BOOT_ENTER_CHECK_UART_H
#define __AM_BOOT_ENTER_CHECK_UART_H

#include "am_common.h"
#include "am_uart.h"
#include "am_boot_enter_check.h"

/**
 * \brief 进入应用代码检测设备（串口命令驱动）
 */
typedef struct am_boot_enter_check_uart_cmd_dev {
    /**< \brief 标准的进入应用程序检测服务  */
    am_boot_enter_check_serv_t enter_check_serv;
    /**< \brief 串口标准服务操作句柄  */
    am_uart_handle_t           uart_handle;
}am_boot_enter_check_uart_cmd_dev_t;

/**
 * \brief 进入应用代码检测设备初始化（串口命令驱动）
 */
am_boot_enter_check_handle_t am_boot_enter_check_uart_cmd_init(am_uart_handle_t uart_handle);

#endif /* __AM_BOOT_ENTER_CHECK_UART_H */
