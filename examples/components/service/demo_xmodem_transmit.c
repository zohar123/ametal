/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief xmodem发送演示例程，通过标准接口实现
 *
 * - 操作步骤:
 *    1、通过串口将主机与开发板连接，设置串口波特率等
 *    2、将需要传输的数据保存在相应大小的数组中，打开
 *      上位机，接收到上位机发来的启动字符后，将数据按规
 *      定的帧大小传到上位机。
 *
 * - 实验现象:
 *    接收方将接收到终端发送的__TRANS_DATA_LEN个字节的有效数据在传输过程中可能会
 *    通过串口打印以下信息：
 *    1、当前帧发送成功会打印“The current data have been sent!”
 *
 *    2、超时未能接收到回应会打印“The timeout did not receive response!”
 *
 *    3、接收方回应异常，已经重发到最大次数会打印“Have been sent nak maxtimes!”
 *
 *    4、文件发送完成会打印“File sent successfully!”
 *
 * \note 打印信息的串口需要与Xmodem传输的串口分开，否则打印信息会干扰Xmodem传输
 *       若Xmodem传输所用串口为uart0，则打印信息串口应为uart1，若是串口资源有限，
 *       可以将打印信息注释掉。
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-5 xgg, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_xmodem_transmit
 * \copydoc demo_xmodem_transmit.c
 */

/** [src_xmodem_transmit] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_xmodem.h"

/******************************************************************************
宏定义
*******************************************************************************/
#define __TRANS_DATA_LEN   (1024+3)     /**< \brief 文件大小(单位为byte)*/
/*******************************************************************************
全局变量
*******************************************************************************/
/* xmodem发送回调标志*/
volatile static int  __g_tx_flag = 0;

/* xmodem发送事件标志*/
volatile static int __g_tx_event = 0;

/* 数据缓存区*/
static char __g_xmodem_test[__TRANS_DATA_LEN];

/**
 * \brief 发送回调函数
 *
 * \param[in] p_arg : 用户自定义参数
 * \param[in] event : 当前传输事件，可参考am_xmodem.h中关于
 *                    am_xmodem_user_tx_t回调函数类型的
 *                    注释。
 * \note 用户需要根据event来决定当前处理的操作
 */
static void demo_tx_callback (void *p_arg, int event)
{
    /* 发送回调标志置1*/
    __g_tx_flag  = 1;
    /* 保存发送事件*/
    __g_tx_event = event;
}

/* 例程入口*/
void  demo_xmodem_tx_entry (am_xmodem_tx_handle_t  handle)
{
    uint32_t i = 0;

    /* 一帧数据的长度*/
    uint32_t frames_len   = 0;

    /* 当前已发送的字节数*/
    uint32_t index_bytes  = 0;

    /* 当前已经发送帧数*/
    uint32_t index_frames = 0;

    if (handle == NULL) {
        return;
    }

    /* 数据缓存区填充*/
    for (i = 0; i < __TRANS_DATA_LEN; i++) {
        __g_xmodem_test[i] = i;
    }

    /* 注册发送回调函数*/
    am_xmodem_tx_cb_reg(handle, demo_tx_callback, (void *)handle);

    while (1) {

        if (__g_tx_flag) {

            __g_tx_flag = 0;

            switch (__g_tx_event) {

            /* 传输启动状态*/
            case AM_XMODEM_NAK:
                frames_len = 128;
                am_xmodem_tx_pack(handle, __g_xmodem_test, frames_len);
                break;

            case AM_XMODEM_1k:
                frames_len = 1024;
                am_xmodem_tx_pack(handle, __g_xmodem_test, frames_len);
                break;

            /* 超时未能接收到回应*/
            case -AM_ETIME:
                /* 事件标志清0*/
                __g_tx_event = 0;
                index_bytes  = 0;
                index_frames = 0;
                am_kprintf("The timeout did not receive response!\r\n");
                return;

            /* 接收方回应异常，已达到最大重发次数*/
            case AM_XMODEM_NAK_TIME:
                __g_tx_event = 0;
                index_bytes  = 0;
                index_frames = 0;
                am_kprintf("Have been sent nak maxtimes! \r\n");
                return;

            /* 文件发送完成*/
            case AM_XMODEM_MOU_SUC:
                __g_tx_event = 0;
                index_bytes  = 0;
                index_frames = 0;
                am_kprintf("File sent successfully!\r\n");
                am_xmodem_tx_over(handle);
                return;

            case AM_XMODEM_ACK:
                __g_tx_event = 0;
                index_frames++;
                index_bytes += frames_len;
                /* 收到上一帧回应后，一帧传输才算完成，打印信息*/
                am_kprintf("The current %d data have been sent!\r\n", index_frames);
                /* 发送下一帧数据*/
                if (__TRANS_DATA_LEN - index_bytes < frames_len) {
                    am_xmodem_tx_pack(handle,
                                      &__g_xmodem_test[index_bytes],
                                      __TRANS_DATA_LEN - index_bytes);
                } else {
                    am_xmodem_tx_pack(handle,
                                      &__g_xmodem_test[index_bytes],
                                      frames_len);
                }
                break;

            default :
                break;
            }
        }
    }
}

/** [src_xmodem_transmit] */

/* end of file */
