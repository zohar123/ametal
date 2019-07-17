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
 *   6. 将Mifare UltraLight卡片至于板1的天线感应区。
 *
 * - 实现现象：看串口（PIOE_20）打印信息。
 *
 * \par 源代码
 * \snippet demo_kl26_std_uartcmd_ultralight_write_block.c src_kl26_std_uartcmd_ultralight_write_block
 *
 * \internal
 * \par Modification history
 * - 1.00 18-03-19  sdq, first implementation.
 * \endinternal
 */ 

/**
 * \addtogroup demo_kl26_if_std_uartcmd_ultralight_write_block
 * \copydoc demo_kl26_std_uartcmd_ultralight_write_block.c
 */

/** [src_kl26_std_uartcmd_ultralight_write_block] */
#include <am_uartcmd_send.h>
#include "ametal.h"
#include "am_board.h"
#include "am_kl26.h"
#include "am_int.h"
#include "am_kl26_inst_init.h"

#include "string.h"

#ifndef AM_VDEBUG
#define AM_VDEBUG           /* 开启调试模式 */
#endif

#include "am_vdebug.h"

static uint8_t __g_uartcmd_uart_txbuf[128];
static uint8_t __g_uartcmd_uart_rxbuf[128];

static const am_uartcmd_uart_dev_info_t __g_uartcmd_uart_devinfo = {
    __g_uartcmd_uart_rxbuf,     /* 用于串口接收的缓冲区      */
    __g_uartcmd_uart_txbuf,     /* 用于串口发送的缓冲区      */
    128,                       /* 接收数据缓冲区的大小      */
    128,                       /* 发送数据缓冲区的大小      */
    AM_UARTCMD_BAUDRATE_115200, /* 与UARTCMD通信的波特率      */
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
 * \brief uartcmd 读取写ultralight块
 */
void demo_amks16rfid8_uartcmd_ultralight_write_block_entry (void)
{
    int      ret;
    int      i;
    uint16_t atq;
    uint8_t  saq;
    uint8_t  uid_len;
    uint8_t  uid[10];   /* UID最大长度为10 */
    am_uart_handle_t          uart1_handle;
    am_uartcmd_uart_dev_t     uartcmd_uart_dev;
    am_uartcmd_send_handle_t  uartcmd_handle;
    uint8_t ntag213[6] = {0x06, 0x1b, 0xff, 0xff, 0xff, 0xff};
    uint8_t rx_buf[16] = { 0 };
    uint8_t block_num = 4;                       /* 卡块号 */
    uint8_t block_dat[4] = {0x88, 0x88, 0x88, 0x88};   /* 块数据 */
    /* 使用UART通讯模式 */
    uart1_handle  = am_kl26_uart1_inst_init();
    uartcmd_handle = am_uartcmd_uart_new_init(&uartcmd_uart_dev,
                                               uart1_handle,
                                              &__g_uartcmd_uart_devinfo);

    if (uartcmd_handle == NULL) {
        __error_happen(__LINE__);
    }

    while (1) {
        /* 轮询激活卡片 */
        ret = uartcmd_card_active(uartcmd_handle,
                                  AM_UARTCMD_MIFARE_CARD_REQ_IDLE, /* 请求空闲的卡 */
                                  &atq,
                                  &saq,
                                  &uid_len,
                                  uid,
                                  10);
        if (ret == AM_OK) {
            AM_DBG_INFO("检测到卡，卡ID为 %d bytes ： \r\n", uid_len);
            for (i = 0; i < uid_len; i++) {
                AM_DBG_INFO("%02x   ", uid[i]);
            }
            AM_DBG_INFO ("\r\n");

            ret = am_uartcmd_mifare_card_cmd_trans(uartcmd_handle,
                                                   ntag213,
                                                   sizeof(ntag213),
                                                   rx_buf,
                                                   sizeof(rx_buf));

            if (ret == AM_OK) {
                AM_DBG_INFO("NTAG213密钥验证成功 \r\n", uid_len);
                AM_DBG_INFO("往块 %d 中写数据：%02x %02x %02x %02x\r\n",
                            block_num,
                            block_dat[0],
                            block_dat[1],
                            block_dat[2],
                            block_dat[3]);

                ret = am_uartcmd_ultralight_card_write(uartcmd_handle,
                                                       block_num,
                                                       block_dat);
                if (ret == AM_OK) {
                    AM_DBG_INFO("写块数据成功 \r\n");
                    AM_DBG_INFO("读块 %d 数据 \r\n", block_num);

                    ret = am_uartcmd_mifare_card_read(uartcmd_handle,
                                                      block_num,
                                                      rx_buf);
                    if (ret == AM_OK) {
                        AM_DBG_INFO("读块数据成功 ：");
                        for (i = 0; i < sizeof(rx_buf); i++) {
                            AM_DBG_INFO("%02x ", rx_buf[i]);
                        }
                        AM_DBG_INFO("\r\n");
                    } else {
                        AM_DBG_INFO("读块数据失败 \r\n");
                    }
                } else {
                    AM_DBG_INFO("写块数据失败 \r\n");
                }
            } else {
                AM_DBG_INFO("NTAG213密钥验证失败 \r\n");
            }
        }
        am_mdelay(500);
    }
}

/** [src_kl26_std_uartcmd_ultralight_write_block] */

/* end of file */
