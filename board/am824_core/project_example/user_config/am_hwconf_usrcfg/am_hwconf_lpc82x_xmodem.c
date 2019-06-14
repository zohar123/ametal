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
 * \brief LPC82x Xmodem 用户配置文件
 * \sa am_hwconf_lpc82x_xmodem.c
 *
 * \internal
 * \par Modification history
 * - 1.00 18-08-31  xgg, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "am_uart.h"
#include "am_xmodem.h"
#include "am_lpc82x.h"
#include "am_lpc82x_inst_init.h"

/**
 * \addtogroup am_if_src_hwconf_lpc82x_xmodem
 * \copydoc am_hwconf_lpc82x_xmodem.c
 * @{
 */

/* Xmodem发送工作模式*/
#define __MODE   AM_XMODEM_128_MODE
#if (__MODE == AM_XMODEM_128_MODE)
#define  __BUFF_SIZE      128
#elif (__MODE == AM_XMODEM_1K_MODE)
#define  __BUFF_SIZE      1024
#endif

/* 数据缓存区*/
static char __g_xmodem_rec_buf[__BUFF_SIZE];

/* 发送设备实例*/
static am_xmodem_tx_dev_t  __g_xmodem_tx_dev;

/* 接收设备实例*/
static am_xmodem_rec_dev_t __g_xmodem_rec_dev;

/* 定义Xmodem接收设备信息结构体 */
static const am_xmodem_rec_dev_info_t  __g_xmodem_dev_info = {
    __g_xmodem_rec_buf,         /**< \brief 接收缓存区*/
    10,                         /**< \brief 最大重发次数*/
    sizeof(__g_xmodem_rec_buf), /**< \brief 接收缓存区大小 */
    __MODE,                     /**< \brief 接收工作模式*/
    AM_XMODEM_CRC_MODE,         /**< \brief 校验方式*/
    3000,                       /**< \brief 接收超时时间*/
    8000                        /**< \brief 启动间隔时间*/
};

/* 定义Xmodem发送设备信息结构体*/
static const am_xmodem_tx_dev_info_t __g_xmodem_tx_dev_info = {
   1000,        /**< \brief 发送超时时间*/
   3,           /**< \brief 最大重发次数*/
};


/* Xmodem发送实例初始化，获取Xmodem发送操作句柄*/
am_xmodem_tx_handle_t   am_lpc82x_xmodem_tx_inst_init (void)
{
    am_crc_handle_t   crc_handle   = NULL;
    am_uart_handle_t  lpc82x_uart1 = NULL;

    lpc82x_uart1 = am_lpc82x_usart1_inst_init();

    crc_handle = am_lpc82x_crc_inst_init();

    return am_xmodem_tx_init(&__g_xmodem_tx_dev,
                             &__g_xmodem_tx_dev_info,
                             lpc82x_uart1,
                             crc_handle);
}

/* Xmodem接收实例初始化，获取Xmodem接收操作句柄*/
am_xmodem_rec_handle_t   am_lpc82x_xmodem_rec_inst_init (void)
{
    am_crc_handle_t   crc_handle   = NULL;
    am_uart_handle_t  lpc82x_uart1 = NULL;

    lpc82x_uart1 = am_lpc82x_usart1_inst_init();

    crc_handle = am_lpc82x_crc_inst_init();

    return am_xmodem_rec_init(&__g_xmodem_rec_dev,
                              &__g_xmodem_dev_info,
                              lpc82x_uart1,
                              crc_handle);
}

/**
 * @}
 */

/* end of file */
