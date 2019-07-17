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
 * \brief uartcmd读卡ID，通过标准接口实现
 *
 * - 操作：
 * - 1. 准备两块AMKS16RFID-8评估板，称为板1和板2；
 *   2. 板1需插入天线，执行projects_keil5\examples_uart_cmd_read_card\demo_kl26_uartcmd_handle中的例程；
 *   3. 将板1的PIOC_3与板2的PIOC_4连接，板1的PIOC_4与板2的PIOC_3连接，板1与板2共地；
 *   4. 将板2的PIOE_20与电脑串口接收端连接；
 *   5. 两块板子上电后按下两块板子的复位键，然后先松开板1的复位键，1秒后再松开板2
 *      的复位键。
 *   6. 将A类卡片至于板1的天线感应区。
 *
 * - 实现现象：
 * - 1. 第5步操作做完后板1上的两个板载LED会交替闪烁，否则重复第5步操作或检查其他问题。
 *   2. 第6步操作如果读取到卡片，则板2的串口（PIOE_20)会打印卡号。
 *
 * \par 源代码
 * \snippet demo_kl26_std_uartcmd_picca_auto_detect_entry.c src_kl26_std_uartcmd_picca_auto_detect_entry
 *
 * \internal
 * \par Modification history
 * - 1.00 18-05-17  sdq, first implementation.
 * \endinternal
 */ 

/**
 * \addtogroup demo_kl26_if_std_uartcmd_picca_auto_detect
 * \copydoc demo_kl26_std_uartcmd_picca_auto_detect_entry.c
 */

/** [src_kl26_std_uartcmd_picca_auto_detect_entry] */
#include <am_uartcmd_send.h>
#include "ametal.h"
#include "am_board.h"
#include "am_kl26.h"
#include "am_int.h"
#include "am_kl26_inst_init.h"
#include "am_buzzer.h"
#include "string.h"

#ifndef AM_VDEBUG
#define AM_VDEBUG           /* 开启调试模式 */
#endif

#include "am_vdebug.h"


static uint8_t __g_uartcmd_uart_txbuf[128];
static uint8_t __g_uartcmd_uart_rxbuf[128];

static const am_uartcmd_uart_dev_info_t __g_uartcmd_uart_devinfo = {
    __g_uartcmd_uart_rxbuf,
    __g_uartcmd_uart_txbuf,
    128,
    128,
    AM_UARTCMD_BAUDRATE_115200,
    AM_UARTCMD_MODE_AUTO_CHECK,
    0x59
};


const am_uartcmd_auto_detect_info_t __g_uartcmd_auto_detect_info = {
    0,
    AM_UARTCMD_MIFARE_CARD_TX_TX1,
    AM_UARTCMD_MIFARE_CARD_REQ_IDLE,
    AM_UARTCMD_MIFARE_CARD_AUTH_DIRECT,       /* 不验证时后续参数可以填空 */
    AM_UARTCMD_IC_KEY_TYPE_B,
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
    6,
    0x0C
};

static void __error_happen(int line)
{
    AM_DBG_INFO("error_happen %d\r\n", line);
    am_led_on(0);
    while(1);
}

/**
 * \brief 自动检测回调函数
 */
static void __uartcmd_callback (void *p_arg)
{
    uint32_t *detect_flag = (uint32_t *)p_arg;

    *detect_flag = 1;
}

/**
 * \uartcmd 自动检测测试例程
 */
void demo_amks16rfid8_uartcmd_picca_auto_detect_entry (void)
{
    int      ret;
    int      i;
    am_uart_handle_t       uart1_handle;
    am_uartcmd_uart_dev_t  uartcmd_uart_dev;
    am_uartcmd_send_handle_t    uartcmd_handle;
    volatile uint32_t      detect_flag = 0;     /* 自动检测完成标志 */
    am_uartcmd_mifare_card_info_t card_info;

    uart1_handle  = am_kl26_uart1_inst_init();
    uartcmd_handle = am_uartcmd_uart_new_init(&uartcmd_uart_dev,
                                               uart1_handle,
                                              &__g_uartcmd_uart_devinfo);

    if (uartcmd_handle == NULL) {
        __error_happen(__LINE__);
    }
    
    /* 设置自动检测回调函数 */
    am_uartcmd_mifare_card_auto_detect_cb_set(uartcmd_handle,
                                              __uartcmd_callback,
                                              (void *)&detect_flag);

    /* 开始自动检测 */
    am_uartcmd_mifare_card_auto_detect_start(uartcmd_handle,
                                             &__g_uartcmd_auto_detect_info);

    while (1) {
        if (detect_flag) {
            detect_flag = 0;
            
            ret = am_uartcmd_mifare_card_auto_detect_read(uartcmd_handle,
                                                          &card_info);

            if (ret == AM_OK) {

                AM_DBG_INFO("检测到卡，卡ID为 %d bytes ： \r\n", card_info.uid_len);

                for (i = 0; i < card_info.uid_len; i++) {
                    AM_DBG_INFO("%02x   ", card_info.uid[i]);
                }
                AM_DBG_INFO ("\r\n");

                am_led_on(1);
            } else {
                am_led_off(1);
                AM_DBG_INFO("失败！%d \r\n", ret);
            }

            /* 一次自动检测完成后需要重新启动自动检测 */
            am_uartcmd_mifare_card_auto_detect_start(uartcmd_handle,
                                                     &__g_uartcmd_auto_detect_info);
        }
    }
}

/** [src_kl26_std_uartcmd_picca_auto_detect_entry] */

/* end of file */
