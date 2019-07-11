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
 * \brief UART 波特率检测例程，通过标准接口实现(本例程波特率支持范围 1200~12800)
 *
 * - 实验现象：
 *   1. 串口输出 "uart connect success"；
 *   2. 串口输出接收到的字符串。
 *
 * \note： 如果无法检测到上位机的波特率，会一直循环检测，串口助手也看不到任何回应
 *
 * \par 源代码
 * \snippet demo_amks16z_core_uart_auto_baudrate.c src_amks16z_core_uart_auto_baudrate
 *
 * \internal
 * \par Modification history
 * - 1.00 19-07-10  yrh, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_amks16z_core_if_uart_auto_baudrate
 * \copydoc demo_amks16z_core_uart_auto_baudrate.c
 */

/** [src_amks16z_core_uart_auto_baudrate] */
#include "ametal.h"
#include "am_uart.h"
#include "am_baudrate_detect.h"
#include "am_kl26_inst_init.h"
#include "am_board.h"
#include "demo_components_entries.h"

static const uint8_t __ch[] = {"uart connect success\r\n"};

/**
 * \brief 串口波特率检测例程入口
 */
void demo_amks16z_core_uart_auto_baudrate_entry (void)
{
    am_uart_handle_t uart_handle;
    am_baudrate_detect_handle_t baudrate_hande;

    uint32_t baudrate;
    baudrate_hande = am_kl26_baudrate_detect_inst_init();

    /** \brief 需要发送的字符串 */

    while(AM_OK != am_baudrate_get(baudrate_hande, &baudrate)) {
    }
    /* 波特率获取成功后要解初始化，才能重新初始化串口 */
    am_kl26_baudrate_detect_inst_deinit(baudrate_hande);

    /* UART初始化 ,只能用串口2，获取波特率获取的是串口2的接收引脚*/
    uart_handle = am_kl26_uart2_inst_init();

    am_uart_ioctl(uart_handle, AM_UART_BAUD_SET, (void *)baudrate);

    am_uart_poll_send(uart_handle,
                      __ch,
                      sizeof(__ch));

    uint8_t uart1_buf[5];    /* 数据缓冲区 */

    while(1) {
        /* 接收字符 */
        am_uart_poll_receive(uart_handle, uart1_buf, 1);

        /* 发送刚刚接收的字符 */
        am_uart_poll_send(uart_handle, uart1_buf, 1);
    }
}

/** [src_amks16z_core_uart_auto_baudrate] */

/* end of file */
