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
 * \brief bootloader 串行数据操作标准设备实现（uart驱动）
 *
 * \internal
 * \par modification history:
 * - 1.00 18-12-11  yrh, first implementation.
 * \endinternal
 */
#include "am_boot_serial_uart.h"
#include "am_boot_serial_byte.h"
#include "am_int.h"

static serial_byte_receive_func_t __g_uart_byte_receive_callback;

static int __uart_send(void *p_arg, const uint8_t *buffer, uint32_t byte_count)
{
    am_boot_serial_uart_dev_t  *p_dev = (am_boot_serial_uart_dev_t *)p_arg;
    return am_uart_poll_send(p_dev->uart_handle,buffer,byte_count);
}

static int __uart_receive(void *p_arg, uint8_t *buffer, uint32_t requested_bytes)
{
    am_boot_serial_uart_dev_t *p_dev = (am_boot_serial_uart_dev_t  *)p_arg;
    return am_uart_poll_receive(p_dev->uart_handle, buffer, requested_bytes);

}

static void __uart_irq_handler(void *p_arg, char inchar)
{
    __g_uart_byte_receive_callback(inchar);
}

static int __uart_int_callback_enable(void *p_arg, serial_byte_receive_func_t callback_fun)
{
    am_boot_serial_uart_dev_t *p_dev = (am_boot_serial_uart_dev_t  *)p_arg;
    __g_uart_byte_receive_callback = callback_fun;
    /* 使能串口中断模式  */
    am_uart_ioctl(p_dev->uart_handle, AM_UART_MODE_SET, (void *)AM_UART_MODE_INT);
    /* 注册发送回调函数  */
    am_uart_callback_set(p_dev->uart_handle, AM_UART_CALLBACK_RXCHAR_PUT, __uart_irq_handler, NULL);

    return AM_OK;
}

const static struct am_boot_serial_byte_funcs  __g_serial_uart_funcs = {
    __uart_send,
    __uart_receive,
    __uart_int_callback_enable,
};
static am_boot_serial_uart_dev_t __g_serial_uart_dev;

am_boot_serial_handle_t am_boot_serial_uart_init(am_uart_handle_t uart_handle,
                                                 uint32_t         baund)
{
    int ret;

    __g_serial_uart_dev.serial_serv.p_funcs = &__g_serial_uart_funcs;
    __g_serial_uart_dev.serial_serv.p_drv   = &__g_serial_uart_dev;

    if(uart_handle == NULL) {
        return NULL;
    }
    ret = am_uart_ioctl(uart_handle,
                        AM_UART_BAUD_SET,
                        (void *)baund);
    if(ret != AM_OK) {
        return NULL;
    }
    __g_serial_uart_dev.uart_handle = uart_handle;

    return &(__g_serial_uart_dev.serial_serv);
}

/* end of file */
