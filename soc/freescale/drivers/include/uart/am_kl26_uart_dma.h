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
 * \brief UART驱动，服务于DMA双缓冲模式
 *
 *
 * \internal
 * \par Modification History
 * - 1.01 18-12-13  htf, first implementation.
 * \endinternal
 */

#ifndef __AM_KL26_UART_DMA_H
#define __AM_KL26_UART_DMA_H

#include "am_uart.h"
#include "hw/amhw_fsl_uart.h"
#include "am_fsl_uart.h"
#include "am_rngbuf.h"
#include "hw/amhw_kl26_dma.h"
#include "am_softimer.h"

typedef struct am_kl26_uart_dma_devinfo {

    const am_fsl_uart_devinfo_t      *p_uart_devinfo;    /**< \brief 串口设备信息 */
    amhw_kl26_dma_t                  *p_hw_dma;          /**< \brief DMA寄存器块的基地址 */
    uint8_t                           dma_chan_rx;       /**< \brief DMA接收通道号 */
    char                             *p_dma_buffer;      /**< \brief DMA 映射的内存 */
    char                             *p_rec_buffer;      /**< \brief 串口接收到数据的缓冲区 */
    amhw_kl26_dma_buffer_size_set_t   dma_circular_size; /**< \brief DMA 自动环形缓冲大小 */
    uint16_t                          dmabuffer_size;    /**< \brief DMA 缓存大小 */
    uint16_t                          recbuffer_size;    /**< \brief 串口接收缓存大小 */
}am_kl26_uart_dma_devinfo_t;


typedef struct am_kl26_uart_dma_dev {
    am_fsl_uart_dev_t                 uart_dev;       /**< \brief 串口设备 */
    const am_kl26_uart_dma_devinfo_t *p_devinfo;      /**< \brief DMA串口设备信息 */

    struct am_rngbuf  rngbuf;                         /**< \brief 环形缓冲区*/
    uint32_t          buffer_size;                    /**< \brief 缓存区大小*/
    uint32_t          dma_size;                       /**< \brief DMA缓冲区大小 */

    char             *p_rec_buffer;                   /**< \brief 串口接收缓冲区 */
    char             *p_dma_buffer;                   /**< \brief DMA接收缓冲区 */
    uint32_t          count ;                         /**< \brief DMA环形缓冲区计数值*/
    am_softimer_t     timer;                          /**< \brief 定时器，用于在没触发DMA中断读取数据*/

}am_kl26_uart_dma_dev_t;

/**
 * \brief 初始化UART，返回UART标准服务操作句柄
 *
 * \param[in] p_dev     : 指向串口设备的指针
 * \param[in] p_devinfo : 指向串口设备信息常量的指针
 *
 * \return DMA UART标准服务操作句柄，值为NULL时表明初始化失败
 */
am_uart_handle_t am_uart_dma_init (
                     am_kl26_uart_dma_dev_t           *p_dev,
                     const am_kl26_uart_dma_devinfo_t *p_devinfo);

/**
 * \brief 不使用UART时，解初始化UART，释放相关资源
 * \param[in] p_dev : 指向DMA串口设备的指针
 */
void am_uart_dma_deinit (am_fsl_uart_dev_t  *p_drv);

#endif



