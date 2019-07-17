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
 * - 1.00 15-09-  win, first implementation.
 * \endinternal
 */
#ifndef __AM_UARTCMD_COMM_H
#define __AM_UARTCMD_COMM_H

#ifdef __cplusplus
extern "C" {
#endif

//#define UARTCMD_COMM_MODE_UART        0x00
//#define UARTCMD_COMM_MODE_I2C         0x01
//#define UARTCMD_COMM_MODE_SPI       0x02

#define UARTCMD_PROTOCOL_OLD_FRAME
#define UARTCMD_PROTOCOL_OLD_LENGTH   6     /**< \brief 旧通讯帧协议长度 */
#define UARTCMD_PROTOCOL_LENGTH       10    /**< \brief 通讯帧协议长度 */

#define UARTCMD_COMM_STATUS_IDLE           0x00  /**< \brief UARTCMD 处于空闲状态 */
#define UARTCMD_COMM_STATUS_RX_BUSY        0x01  /**< \brief 帧处于接收状态 */
#define UARTCMD_COMM_STATUS_RX_OK          0x02  /**< \brief 帧接收完毕 */
#define UARTCMD_COMM_STATUS_TX_BUSY        0x03  /**< \brief 帧处于发送状态 */
#define UARTCMD_COMM_STATUS_TX_OK          0x04  /**< \brief 帧发送完毕 */
#define UARTCMD_COMM_STATUS_EXECUTING      0x05  /**< \brief 帧正在处理 */
#define UARTCMD_COMM_STATUS_EXECUTED       0x06  /**< \brief 帧处理结束 */

#define UARTCMD_RX_TIMEOUT            4440  /**< \brief 帧接收超时4.44ms */

#define UARTCMD_FRAME_EXT             0x03  /**< \brief 帧结束符 */

#define UARTCMD_VERSION_SIZE          20    /**< \brief 描述版本的字节数 */
#define UARTCMD_SN_SIZE               16    /**< \brief 描述序列号的字节数 */
#define UARTCMD_RECORD_SIZE           128   /**< \brief 测试记录文件的大小 */


#define UARTCMD_COMM_MODE_MASK        0x0F  /**< \brief 通信模式掩码 */
#define UARTCMD_COMM_MODE_AUTO        0x00  /**< \brief 自动检测 */
#define UARTCMD_COMM_MODE_I2C         0x01  /**< \brief I2C */
#define UARTCMD_COMM_MODE_UART        0x02  /**< \brief UART */
#define UARTCMD_COMM_MODE_WG          0x03  /**< \brief WG */

#ifdef __cplusplus
}
#endif

#endif /* __UARTCMD_COMM_H */

/* end of file */
