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
 * \brief 固件接收（串口驱动）
 *
 * \internal
 * \par Modification history
 * - 1.00 14-11-25  yrh, first implementation.
 * \endinternal
 */
#include "ametal.h"
#include "am_boot.h"
#include "am_uart.h"
#include "am_vdebug.h"
#include "am_boot_firmware.h"
#include "am_softimer.h"
#include "am_boot_firmware_recv_uart.h"

/** \brief 串口数据接收缓存大小  */
#define UART_CALLBACK_BUF_SIZE      512

#define READ_BUF_SIZE               4

/** \brief 固件传输超时标志 */
volatile static uint8_t firmware_timeout = 0;

volatile static uint32_t write_offset = 0, read_offset = 0;

/** \brief bootloader启动延时软件定时器结构体 */
static am_softimer_t timeout_timer;

/** \brief 串口中断回调数据接收缓存 */
static uint8_t  uart_callback_buffer[UART_CALLBACK_BUF_SIZE] = {0};

/**
 * \brief 定时器回调函数
 */
static void __timer_handle(void *p_arg)
{
    firmware_timeout = 1;
}

am_local void __uart_callback(void *p_arg, char inchar)
{
    uart_callback_buffer[write_offset++] = (uint8_t)inchar;
    write_offset &= UART_CALLBACK_BUF_SIZE - 1;
}

/**
 * \brief  从串口接收缓冲区读数据
 */
static int __read_data(uint8_t *p_buffer, uint32_t byte_count)
{
    uint32_t current_bytes_read = 0;
    am_softimer_start(&timeout_timer, 15000);
    while (current_bytes_read != byte_count) {
        if(firmware_timeout == 1) {
            firmware_timeout = 0;
            am_softimer_stop(&timeout_timer);
            return AM_ERROR;
        }

        if(read_offset != write_offset) {
            p_buffer[current_bytes_read++] = uart_callback_buffer[read_offset++];
            read_offset &= UART_CALLBACK_BUF_SIZE - 1;
        }
    }
    am_softimer_stop(&timeout_timer);
    return AM_OK;
}

int __firmware_recv_uart(void *p_drv)
{
    am_boot_firwa_recv_uart_dev_t *p_dev = (am_boot_firwa_recv_uart_dev_t *)p_drv;
    am_boot_firmware_verify_info_t firmware_head;
    int      ret;
    uint32_t read_count = 0, left_size, read_sum = 0;

    ret = am_boot_firmware_store_start(p_dev->firmware_handle);
    if(ret != AM_OK) {
        return AM_ERROR;
    }

    /* 使能串口中断模式  */
    am_uart_ioctl(p_dev->uart_handle, AM_UART_MODE_SET, (void *)AM_UART_MODE_INT);
    /* 注册发送回调函数  */
    am_uart_callback_set(p_dev->uart_handle, AM_UART_CALLBACK_RXCHAR_PUT, __uart_callback, NULL);

    /* 启动延时回调定时器初始化  */
    am_softimer_init(&timeout_timer, __timer_handle, NULL);

    am_kprintf("ready for firmware\r\n");
    /* 从串口数据缓冲区中读取固件的头部信息  */
    if(AM_OK != __read_data((uint8_t *)&firmware_head, sizeof(firmware_head))) {
        am_kprintf("firmware transmission is timeout\r\n");
        return AM_ERROR;
    }
    if(firmware_head.flag != 0xdeadbeef) {
        return AM_ERROR;
    }

    read_count = firmware_head.len / (uint32_t)READ_BUF_SIZE;
    while(read_count--) {
        uint8_t buf_t[READ_BUF_SIZE] = {0};
        ret = __read_data((uint8_t *)buf_t, READ_BUF_SIZE);
        if(ret != AM_OK) {
            am_kprintf("firmware transmission is timeout\r\n");
             return AM_ERROR;
        }
        ret = am_boot_firmware_store_bytes(p_dev->firmware_handle, buf_t, sizeof(buf_t));
        if(ret != AM_OK) {
            am_kprintf("firmware store is error\r\n");
            return AM_ERROR;
        }
        read_sum += sizeof(buf_t);
    }

    uint8_t buf_t[READ_BUF_SIZE] = {0};
    left_size = firmware_head.len - read_sum;
    ret = __read_data((uint8_t *)buf_t, left_size);
    if(ret != AM_OK) {
        am_kprintf("firmware transmission is timeout\r\n");
        return AM_ERROR;
    }
    ret = am_boot_firmware_store_bytes(p_dev->firmware_handle, buf_t, left_size);
    if(ret != AM_OK) {
        am_kprintf("firmware store is error\r\n");
        return AM_ERROR;
    }

    /* 固件存放结束  */
    am_boot_firmware_store_final(p_dev->firmware_handle);
    am_kprintf("firmware receive successful\r\n");
    /* 固件校验  */
    ret = am_boot_firmware_verify(p_dev->firmware_handle, &firmware_head);
    if(ret != AM_OK) {
        am_kprintf("firmware verify error\r\n");
        return AM_ERROR;
    } else {
        am_kprintf("firmware verify successful\r\n");
    }

    return AM_OK;
}

static const struct am_boot_firwa_recv_drv_funcs __g_firwa_recv_uart_drv_funcs = {
    __firmware_recv_uart,
};

static am_boot_firwa_recv_uart_dev_t  __g_firwa_recv_uart_dev;

am_boot_firwa_recv_handle_t am_boot_firwa_recv_uart_init(
    am_boot_firmware_handle_t firmware_handle, am_uart_handle_t uart_handle)
{
    if(firmware_handle == NULL || uart_handle == NULL) {
        return NULL;
    }

    __g_firwa_recv_uart_dev.firmware_handle = firmware_handle;
    __g_firwa_recv_uart_dev.uart_handle     = uart_handle;
    __g_firwa_recv_uart_dev.isa.p_funcs     = &__g_firwa_recv_uart_drv_funcs;
    __g_firwa_recv_uart_dev.isa.p_drv       = &__g_firwa_recv_uart_dev;

    return &__g_firwa_recv_uart_dev.isa;
}
/* end of file */
