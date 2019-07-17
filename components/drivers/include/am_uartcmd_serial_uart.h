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
 * \brief 
 *
 * \internal
 * \par Modification History
 * - 1.00 15-11-12  win, first implementation.
 * \endinternal
 */
 
#ifndef __AM_UARTCMD_SERIAL_UART_H
#define __AM_UARTCMD_SERIAL_UART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_timer.h"
#include "am_softimer.h"
#include "am_uart.h"
#include "am_uartcmd_serial.h"
#include "am_uartcmd_flash_define.h"

/**
 * \brief 串口超时定时器结构体
 */
typedef struct uartcmd_serial_uart_timer {
    
    /** \brief 定时器超时句柄 */
    am_softimer_t *p_softtimer;
    
    /** \brief 定时器计数值 */
    uint32_t       timeout_ms;
    
} uartcmd_serial_uart_timer_t;

/**
 * \brief UARTCMD 串行设备 UART 设备节点生成函数
 *
 * \param[in] p_serial      : 串行设备结构体指针
 * \param[in] p_uart_timer  : 串口超时定时器结构体指针
 * \param[in] uart_handle   : 串口句柄
 * \param[in] am_softimer_t : 软件定时器结构体指针
 * \param[in] timer_ms      : 定时器超时时间,单位（ms）
 * \param[in] serial_id     : 串行设备表示
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int uartcmd_serial_uart_mkdev (uartcmd_serial_t            *p_serial,
                               uartcmd_serial_uart_timer_t *p_uart_timer,
                               am_uart_handle_t             uart_handle,
                               am_softimer_t               *p_softtimer,
                               uint32_t                     timeout_ms,
                               uint8_t                      serial_id)
{
    if (p_serial == NULL || p_uart_timer == NULL) {
        return -AM_EINVAL;
    }

    p_uart_timer->p_softtimer = p_softtimer;
    p_uart_timer->timeout_ms  = timeout_ms;

    p_serial->p_serial_handle = uart_handle;
    p_serial->serial_id       = serial_id & UARTCMD_FLASH_COMM_MODE_MASK;
    p_serial->p_arg           = (void *)p_uart_timer;

    return AM_OK;
}

/**
 * \brief 串行设备 UART 初始化函数
 *
 * \param[in] handle   : UARTCMD服务句柄
 * \param[in] p_serial : 串行设备结构体
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
int uartcmd_serial_uart_init (am_uartcmd_handle_t  handle,
                              uartcmd_serial_t    *p_serial);

/**
 * \brief 串行设备 UART 解除初始化函数
 *
 * \param[in] p_serial     : 串行设备结构体
 * \param[in] uart_handle  : 串口句柄
 * \param[in] timer_handle : 定时器句柄
 * \param[in] serial_id    : 串行设备ID，用于区分不同的设备
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
int uartcmd_serial_uart_deinit (am_uartcmd_handle_t  handle,
                                uartcmd_serial_t    *p_serial);

#ifdef __cplusplus
}
#endif

#endif /* __UARTCMD_SERIAL_UART_H */

/* end of file */
