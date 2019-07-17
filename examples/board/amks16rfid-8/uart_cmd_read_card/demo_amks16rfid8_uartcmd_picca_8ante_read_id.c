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
 *   2. 第6步操作如果读取到卡片，则板2的串口（PIOE_20)会打印天线的序号及卡号，并且蜂鸣器鸣叫一声。
 *
 * \par 源代码
 * \snippet demo_kl26_std_uartcmd_picca_8ante_read_id.c src_kl26_std_uartcmd_picca_8ante_read_id
 *
 * \internal
 * \par Modification history
 * - 1.00 18-03-16  sdq, first implementation.
 * \endinternal
 */ 

/**
 * \addtogroup demo_kl26_if_std_uartcmd_8ante_picca_read_id
 * \copydoc demo_kl26_std_uartcmd_picca_8ante_read_id.c
 */

/** [src_kl26_std_uartcmd_picca_8ante_read_id] */
#include <am_uartcmd_send.h>
#include "ametal.h"
#include "am_board.h"
#include "am_kl26.h"
#include "am_int.h"
#include "am_buzzer.h"
#include "string.h"
#include "am_kl26_inst_init.h"

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

static void __error_happen(int line)
{
    AM_DBG_INFO("error_happen %d\r\n", line);
    am_led_on(0);
    while(1);
}

/**
 * \uartcmd 8天线读取卡片ID
 */
void demo_amks16rfid8_uartcmd_picca_8ante_read_id_entry (void)
{
    int      ret;
    int      i, j;
    uint16_t atq;
    uint8_t  saq;
    uint8_t  uid_len;
    uint8_t  uid[10];
    am_uart_handle_t      uart1_handle;
    am_uartcmd_uart_dev_t  uartcmd_uart_dev;
    am_uartcmd_send_handle_t    uartcmd_handle;

    uart1_handle  = am_kl26_uart1_inst_init();
    uartcmd_handle = am_uartcmd_uart_new_init(&uartcmd_uart_dev,
                                               uart1_handle,
                                              &__g_uartcmd_uart_devinfo);

    if (uartcmd_handle == NULL) {
        __error_happen(__LINE__);
    }

    while (1) {
        for (j = 0; j < 8; j++) {
            am_uartcmd_ante_select(uartcmd_handle, j);
            ret = uartcmd_card_active(uartcmd_handle,
                                      AM_UARTCMD_MIFARE_CARD_REQ_ALL,
                                      &atq,
                                      &saq,
                                      &uid_len,
                                      uid,
                                      10);
            AM_DBG_INFO("ante %d\r\n", j);
            if (ret == AM_OK) {
                for (i = 0; i < uid_len; i++) {
                    AM_DBG_INFO("%02x   ", uid[i]);
                }
                AM_DBG_INFO ("\r\n\r\n");
                
                am_uartcmd_picca_halt(uartcmd_handle);
                am_buzzer_beep_async(100);
            }
        }
    }
}

/** [src_kl26_std_uartcmd_picca_8ante_read_id] */

/* end of file */
