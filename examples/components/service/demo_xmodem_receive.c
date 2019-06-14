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
 * \brief xmodem接收演示例程，通过标准接口实现
 *
 * - 操作流程:
 *    1、将开发板串口连接到主机，并且在调用此demo之前
 *      初始化相关串口。
 *    2、打开支持Xmodem的上位机(例如Tera_term、Xshell)
 *      启动代码后，在上位机上选择相关的文件使用Xmodem协议传输。
 *
 * - 实验现象:
 *   文件将以每帧规定字节(128或1024字节)发送到终端，终端对数据进行接收，传输过程
 *   中可能出现一下几种现象:
 *   1、若是接收到帧数据，将通过串口将获取到的一帧数据打印按以下格式打印出来:
 *     “Current : %d data: %s”
 *
 *   2、若是发送方取消发送，将通过串口打印信息“The sender cancels the transmission!”
 *
 *   3、若是文件接收成功，将通过串口打印信息“File received successfully !”
 *
 *   4、若是数据出错，将通过串口打印出错的帧序号信息“Current data err !”
 *
 *   5、若是未能连接到上位机，将通过串口打印“Connection Failed!”
 *
 *   6、若是超时仍然未能接收到数据，将打印信息“No data was received for the timeout!”
 *
 *   7、若是当前帧校验码出错，将打印信息“Current data check code err!”
 *
 * \note 打印信息的串口需要与Xmodem传输的串口分开，否则打印信息会干扰Xmodem传输
 *       若Xmodem传输所用串口为uart0，则打印信息串口应为uart1，若是串口资源有限，
 *       可以将打印信息注释掉，在文件128行对接收到的数据进行打印时，数据中间可能
 *       包含字符'\0'，打印过程将终止，不会打印剩下的数据。
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-6  xgg, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_xmodem_receive
 * \copydoc demo_xmodem_receive.c
 */
/** [src_xmodem_receive] */
#include "ametal.h"
#include "am_xmodem.h"
#include "am_vdebug.h"

/*******************************************************************************
全局变量
*******************************************************************************/
/* 接收回调标志*/
volatile static int __g_rx_flag  = 0;

/* 当前接收事件*/
volatile static int __g_rx_event = 0;

/* 保存接收到的数据缓存指针*/
volatile static void *__gp_rx_buff = NULL;

/**
 * \breif 用户回调函数
 * \param[in] p_arg    : 用户决定传入回调函数的参数，若用户需在回调函数中
 *                       使用Xmodem句柄，可将句柄传入此参数。
 * \param[in] p_frames : Xmodem接收到的一帧数据保存的地址，用户可直接
 *                       使用该地址获取接收到的一帧数据
 * \param[in] event    : 先判断event值的正负，若大于0则这一帧数据接收
 *                       成功，且event保存的是接收到的字节数，成功接受到一
 *                       帧数据后，可以调用am_xmodem_rec_ack_set接口
 *                       请求接受下一帧数据，若event值为负，则接收失败，可
 *                       根据event的值判断是以下那种错误。
 *                       -AM_ETIME       : 这一帧数据超时未接收到
 *                       -AM_EBADMSG     : 未超时但重发用户规定的次数后数据仍然错误
 *                       -AM_DATA_CAN    : 发送方紧急取消发送导致传输结束
 *                       -AM_DATA_SUC    : 整个文件传输完毕
 *                       -AM_XMODEM_DOWN : 下位机启动后未在超时时间内连接上上位机
 *                       -AM_XMODEM_CHECK_ERR : 这一帧数据校验码出错
 *
 * \note 具体的event事件如何处理可参考am_xmodem.h中am_xmodem_user_rx_t接收
 *    回调函数的注释
 */
static void demo_rec_callback(void *p_arg, void *p_frames, int event)
{
    /* 接收回调标志置1*/
    __g_rx_flag  = 1;
    /* 保存接收事件*/
    __g_rx_event = event;
    /* 保存接收数据缓存的指针*/
    __gp_rx_buff = p_frames;
}

/* 例程入口*/
void demo_xmodem_rec_entry (am_xmodem_rec_handle_t  handle)
{
    /* 保存当前已接收到的帧数*/
    uint32_t index_rx_frames = 0;

    if (handle == NULL) {
        return;
    }

    /* 注册接收回调函数*/
    am_xmodem_rec_cb_reg(handle, demo_rec_callback, (void *)handle);

    /* 发送启动传输字符*/
    am_xmodem_rec_start(handle);

    while (1) {
        /* 判断当前是否有回调事件产生*/
        if (__g_rx_flag) {

            __g_rx_flag = 0;

            if (__g_rx_event > 0) {
                __g_rx_event = 0;
                index_rx_frames++;
                /* 将接受到的数据打印出来*/
                am_kprintf("Current %d data: %s\r\n",
                           index_rx_frames,
                           __gp_rx_buff);
                am_xmodem_rec_ack_set(handle);
                continue;
            }

            /* 判断当前回调事件*/
            switch (__g_rx_event) {

            /* 文件接收完成后，打印成功接收文件信息*/
            case -AM_DATA_SUC:
                /* 将事件标志清0*/
                __g_rx_event    = 0;
                index_rx_frames = 0;
                am_kprintf("File received successfully !\r\n");
                return;

            /* 数据异常，取消发送并打印那一帧数据出错*/
            case -AM_EBADMSG:
                /* 将事件标志清0*/
                __g_rx_event = 0;
                /* 帧数加1才是出错的帧序号*/
                index_rx_frames++;
                am_kprintf("Current %d data err !\r\n", index_rx_frames);
                am_xmodem_rec_can_set(handle);
                return;

            /* 未能连接到上位机*/
            case -AM_XMODEM_DOWN:
                /* 将事件标志清0*/
                __g_rx_event = 0;
                am_kprintf("Connection Failed !\r\n");
                return;

            /* 发送方取消发送*/
            case -AM_DATA_CAN:
                /* 将事件标志清0*/
                __g_rx_event    = 0;
                index_rx_frames = 0;
                am_kprintf("The sender cancels the transmission!\r\n");
                return;

            /* 超时未能接收到数据*/
            case -AM_ETIME:
                /* 将事件标志清0*/
                __g_rx_event    = 0;
                index_rx_frames = 0;
                am_kprintf("No data was received for the timeout!\r\n");
                return;

            /* 当前帧校验码出错*/
            case -AM_XMODEM_CHECK_ERR:
                /* 将事件标志清0*/
                __g_rx_event = 0;
                index_rx_frames++;
                am_kprintf("Current %d data check code err!\r\n", index_rx_frames);
                am_xmodem_rec_can_set(handle);
                return;

            default:
                break;
            }
        }
        /* 用户其他操作*/
    }
}

/** [src_xmodem_receive] */

/* end of file */
