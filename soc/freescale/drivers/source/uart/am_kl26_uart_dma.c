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
 * \brief UART驱动层实现函数（DMA  双缓冲模式）
 *
 * \internal
 * \par Modification history
 * - 1.01 18-12-13  htf, first implementation.
 * \endinternal
 */

/*******************************************************************************
includes
*******************************************************************************/

#include <am_kl26_uart_dma.h>
#include "am_int.h"
#include "am_clk.h"
#include "am_kl26_dma.h"
#include "kl26_periph_map.h"
#include "am_kl26_uart_dma.h"

/**
 * \brief 串口模式（查询或中断）设置
 */
static int  __uart_dma_mode_set (am_kl26_uart_dma_dev_t  *p_dev, uint32_t new_mode);


static int uart_rx_dma_tran_cfg (am_kl26_uart_dma_dev_t   *p_dev,
                                 uint32_t                  dma_tran_len);
/**
 * \brief 串口模式中断模式函数申明
 */
static void __uart_tx_handler (am_fsl_uart_dev_t *p_dev);
static void __uart_irq_handler (void *p_arg);
/**
 * \brief 串口设置
 */
static int  __uart_dma_opt_set (void *p_drv, uint32_t opts);


/* Kl26 串口驱动函数声明 */
static int __uart_ioctl (void *p_drv, int, void *);

static int __uart_tx_startup (void *p_drv);

static int __uart_callback_set (void *p_drv,
                                int   callback_type,
                                void *pfn_callback,
                                void *p_arg);

static int __uart_poll_getchar (void *p_drv, char *p_char);

static int __uart_poll_putchar (void *p_drv, char outchar);


static const struct am_uart_drv_funcs __g_uart_drv_funcs = {
    __uart_ioctl,
    __uart_tx_startup,
    __uart_callback_set,
    __uart_poll_getchar,
    __uart_poll_putchar,
};

static int count1 = 0,count2= 0;

/**
 * \brief 串口发送中断服务
 */
void __uart_tx_handler (am_fsl_uart_dev_t *p_dev)
{
    amhw_fsl_uart_t *p_hw_uart = p_dev->p_devinfo->p_hw_uart;
    char             data;
    uint32_t         int_stat;

    if(p_dev->p_devinfo->ver == AM_FSL_UART_VER0){
        int_stat = amhw_fsl_uart_ver0_intstat_get(p_hw_uart);
    }else{
        int_stat = amhw_fsl_uart_ver1_intstat_get(p_hw_uart);
    }

    if (((int_stat & AMHW_FSL_UART_INTSTAT_S1_TDRE) != 0) || /* 是否为发送Tx中断 */
        ((int_stat & AMHW_FSL_UART_INTSTAT_S1_IDLE) != 0)) {

        /* 获取发送数据并发送 */
        if ((p_dev->pfn_txchar_get(p_dev->txget_arg, &data)) == AM_OK) {
            amhw_fsl_uart_txdata_write(p_hw_uart, data);
        } else {
            /* 没有数据传送就关闭发送中断 */
            amhw_fsl_uart_int_disable(p_hw_uart, AMHW_FSL_UART_INT_C2_TIE);

            /* 禁止能485发送控制引脚 */
            if (p_dev->p_devinfo->uart_int_485_send) {
                p_dev->p_devinfo->uart_int_485_send(AM_FALSE);
            }
        }
    }
}


/**
 * \brief 获取DMA接收数据
 */
static void __dma_buffer_data_get(void  *p_drv, int flag)
{
    int len = 0;
    am_kl26_uart_dma_dev_t *p_dev = (am_kl26_uart_dma_dev_t  *)p_drv;
    const am_kl26_uart_dma_devinfo_t *p_devinfo = p_dev->p_devinfo;
    amhw_kl26_dma_t                  *p_hw_dma = p_devinfo->p_hw_dma;

    len = p_hw_dma->dma_chan[p_devinfo->dma_chan_rx].dma_dsr_bcr & 0xfffff;
    /* 保证当前DMA的缓存区中有数据  */
    if(len != (p_devinfo->dmabuffer_size / 2)){

        int key = am_int_cpu_lock();
        /* 停止DMA传输  */
        p_hw_dma->dma_chan[p_devinfo->dma_chan_rx].dma_dsr_bcr |= 1 << 24;
        /* 重写传输字节数  */
        p_hw_dma->dma_chan[p_devinfo->dma_chan_rx].dma_dsr_bcr &= 0x00000;
        p_hw_dma->dma_chan[p_devinfo->dma_chan_rx].dma_dsr_bcr |= (p_devinfo->dmabuffer_size / 2);
        /* 若flag = 2   则表示为DMA产生的满中断，在此种情况下  需要重启DMA的外设请求传输  该串口才会正常工作 */
        if(flag == 2){
            /* 重新开启外设请求传输 */
            p_hw_dma->dma_chan[p_devinfo->dma_chan_rx].dma_dcr &= ~(0x40000000);
            p_hw_dma->dma_chan[p_devinfo->dma_chan_rx].dma_dcr |= (0x40000000);
        }
        /* 开启DMA传输  */
        p_hw_dma->dma_chan[p_devinfo->dma_chan_rx].dma_dsr_bcr &= ~(1 << 24);

        am_int_cpu_unlock(key);

        /*判断接收的数据长度*/
        len = (p_devinfo->dmabuffer_size / 2) - len;

        /* DMA设置为若缓冲区满，则DMA会自动从偏移为0的地址开始存放数据  
         * 若读取的数据的地址大于设置的总的大小  那么该情况下需要分两次进行读取，第一次为当前读位置至缓冲区的最高偏移地址
         * 第二次读取数据为偏移为0的地址至总共需要读取的数据减去第一次取出的数据即可。
         */
        if((p_dev->count + len ) > (p_devinfo->dmabuffer_size - 1)){
            am_rngbuf_put(&p_dev->rngbuf,
            (const char *)&p_dev->p_dma_buffer[p_dev->count],
                           p_dev->dma_size-p_dev->count);

            len = len - (p_dev->dma_size - p_dev->count);
            p_dev->count = 0;
        }
        am_rngbuf_put(&p_dev->rngbuf, (const char *)&p_dev->p_dma_buffer[p_dev->count], len);
        p_dev->count += len;
    }
}


void am_softimer_cb(void  *p_drv)
{
    int count;
    char data;
    am_kl26_uart_dma_dev_t *p_dev = (am_kl26_uart_dma_dev_t  *)p_drv;
    am_fsl_uart_dev_t      *p_uart_dev = &p_dev->uart_dev;
    count1++;
    am_softimer_stop(&p_dev->timer);
    __dma_buffer_data_get(p_drv, 1);
    count = am_rngbuf_nbytes(&p_dev->rngbuf);

    for( ; count > 0 ; count --){
        am_rngbuf_get(&p_dev->rngbuf, &data, 1);
        p_uart_dev->pfn_rxchar_put(p_uart_dev->rxput_arg, data);
    }
}

/**
 * \brief 设备控制函数
 *
 * 其中包含设置获取波特率，模式设置（中断/查询），获取支持的模式，硬件选项设置等功能。
 *
 */
static int __uart_ioctl (void *p_drv, int request, void *p_arg)
{
    int                     status     =  AM_OK;
    am_kl26_uart_dma_dev_t *p_dev      = (am_kl26_uart_dma_dev_t *) p_drv;
    am_fsl_uart_dev_t      *p_uart_dev = &p_dev->uart_dev;

    switch (request) {

    /* 波特率设置 */
    case AM_UART_BAUD_SET:

        if(p_uart_dev->p_devinfo->ver == AM_FSL_UART_VER0){
            status = amhw_fsl_uart_ver0_baudrate_set(p_uart_dev->p_devinfo->p_hw_uart,
                                     am_clk_rate_get(p_uart_dev->p_devinfo->clk_id),
                                     (uint32_t)p_arg);
        }else{
            status = amhw_fsl_uart_ver1_baudrate_set(p_uart_dev->p_devinfo->p_hw_uart,
                                      am_clk_rate_get(p_uart_dev->p_devinfo->clk_id),
                                     (uint32_t)p_arg);
        }


        if (status > 0) {
            p_uart_dev->baud_rate = status;
            status = AM_OK;
        } else {
            status = -AM_EIO;
        }
        break;

     /* 波特率获取 */
    case AM_UART_BAUD_GET:
        *(int *)p_arg = p_uart_dev->baud_rate;
        break;

    /* 模式设置 */
    case AM_UART_MODE_SET:
        status = (__uart_dma_mode_set(p_dev, (int)p_arg) == AM_OK)
                 ? AM_OK : -AM_EIO;
        break;

    /* 模式获取 */
    case AM_UART_MODE_GET:
        *(int *)p_arg = p_uart_dev->channel_mode;
        break;

    /* 获取串口可设置的模式 */
    case AM_UART_AVAIL_MODES_GET:
        *(int *)p_arg = AM_UART_MODE_INT | AM_UART_MODE_POLL;
        break;

    /* 串口选项设置 */
    case AM_UART_OPTS_SET:
        status = (__uart_dma_opt_set(p_dev, (int)p_arg) == AM_OK) ? AM_OK : -AM_EIO;
        break;

    /* 串口选项获取 */
    case AM_UART_OPTS_GET:
        *(int *)p_arg = p_uart_dev->options;
        break;
    default:
        status = -AM_EIO;
        break;
    }

    return (status);
}

/**
 * \brief 启动串口发送(仅限中断模式)
 */
int __uart_tx_startup (void *p_drv)
{
    am_fsl_uart_dev_t   *p_dev     = (am_fsl_uart_dev_t *)p_drv;
    amhw_fsl_uart_t     *p_hw_uart = p_dev->p_devinfo->p_hw_uart;

    /* 使能485发送控制引脚 */
    if (p_dev->p_devinfo->uart_int_485_send) {
        p_dev->p_devinfo->uart_int_485_send(AM_TRUE);
    }

    /* 使能发送中断 */
    amhw_fsl_uart_int_enable(p_hw_uart, AMHW_FSL_UART_INT_C2_TIE);

    return AM_OK;
}

/**
 * \brief 配置中断服务回调函数
 */
static int __uart_callback_set (void  *p_drv,
                                int    callback_type,
                                void  *pfn_callback,
                                void  *p_arg)
{
    am_fsl_uart_dev_t *p_dev = (am_fsl_uart_dev_t *)p_drv;

    switch (callback_type) {

    /* 设置发送回调函数中的获取发送字符回调函数 */
    case AM_UART_CALLBACK_TXCHAR_GET:
        p_dev->pfn_txchar_get = (am_uart_txchar_get_t)pfn_callback;
        p_dev->txget_arg      = p_arg;
        return (AM_OK);

    /* 设置接收回调函数中的存放接收字符回调函数 */
    case AM_UART_CALLBACK_RXCHAR_PUT:
        p_dev->pfn_rxchar_put = (am_uart_rxchar_put_t)pfn_callback;
        p_dev->rxput_arg      = p_arg;
        return (AM_OK);

    /* 设置串口异常回调函数 */
    case AM_UART_CALLBACK_ERROR:
        p_dev->pfn_err = (am_uart_err_t)pfn_callback;
        p_dev->err_arg = p_arg;
        return (AM_OK);

    default:
        return (-AM_ENOTSUP);
    }
}

/**
 * \brief 轮询模式下发送一个字符
*/
static int __uart_poll_putchar (void *p_drv, char outchar)
{
    am_fsl_uart_dev_t *p_dev     = (am_fsl_uart_dev_t *)p_drv;
    amhw_fsl_uart_t   *p_hw_uart = p_dev->p_devinfo->p_hw_uart;

    uint32_t  idle_stat;
    if(p_dev->p_devinfo->ver == AM_FSL_UART_VER0){
         idle_stat = (uint32_t)(amhw_fsl_uart_ver0_intstat_get(p_hw_uart)>>7);
    }else{
         idle_stat = (uint32_t)(amhw_fsl_uart_ver1_intstat_get(p_hw_uart)>>7);
    }

    /* 发送模块是否空闲, 0:忙; 1: 空闲 */
    if(((uint8_t)idle_stat & amhw_fsl_uart_stat1_tdre_get(p_hw_uart)) == 0) {
        return (-AM_EAGAIN);
    } else {
        /* 变为发送模式 */
        if (p_dev->p_devinfo->uart_int_485_send != NULL) {
            /* set to transmit mode */
            p_dev->p_devinfo->uart_int_485_send(AM_TRUE);
        }
        /* 发送一个字符 */
        amhw_fsl_uart_txdata_write(p_hw_uart, outchar);

        /* 等待可以发送 */
        if (p_dev->p_devinfo->uart_int_485_send) {
            /* 如果用485发送数据则必须等待当前字节发送完成才能改变数据方向  */
            while(!amhw_fsl_uart_stat1_tc_get(p_hw_uart));
            /* 变为接收模式 */
            p_dev->p_devinfo->uart_int_485_send(AM_FALSE);
        } else {
            while(!amhw_fsl_uart_stat1_tdre_get(p_hw_uart));
        }
    }
    return (AM_OK);
}


/**
 * \brief 轮询模式下接收字符
 */
static int __uart_poll_getchar (void *p_drv, char *p_char)
{
    int                     len;
    uint8_t                *p_inchar   = (uint8_t *)p_char;
    am_kl26_uart_dma_dev_t *p_dev      = (am_kl26_uart_dma_dev_t *)p_drv;

    __dma_buffer_data_get(p_dev, 1);

    len = am_rngbuf_nbytes(&p_dev->rngbuf);
    if(len >0 ){
        len = am_rngbuf_get(&p_dev->rngbuf, (char *)p_inchar, 1);
        if(len == 1){
            return (AM_OK);
        }else{
            return (-AM_EAGAIN);
        }
    }else{
        return (-AM_EAGAIN);
    }
}

/**
 * \brief 配置串口模式
 */
static int __uart_dma_mode_set (am_kl26_uart_dma_dev_t  *p_dev, uint32_t new_mode)
{
    am_fsl_uart_dev_t *p_uart_dev = &p_dev->uart_dev;
    amhw_fsl_uart_t   *p_hw_uart  = p_uart_dev->p_devinfo->p_hw_uart;

    /* 仅支持以下模式 */
    if ((new_mode != AM_UART_MODE_POLL) && (new_mode != AM_UART_MODE_INT)) {
        return (AM_ERROR);
    }

    if (new_mode == AM_UART_MODE_INT) {

        am_int_connect(p_uart_dev->p_devinfo->inum, __uart_irq_handler, (void *)p_dev);
        amhw_fsl_uart_int_disable(p_dev->p_devinfo->p_uart_devinfo->p_hw_uart, AMHW_FSL_UART_INT_ALL);
        amhw_fsl_uart_int_enable(p_dev->p_devinfo->p_uart_devinfo->p_hw_uart, AMHW_FSL_UART_INT_C2_ILIE);
        /* 若非UART0 则需要开启接收完成中断， 进行DMA触发 */
        if(p_hw_uart != KL26_UART0){
            amhw_fsl_uart_int_enable(p_dev->p_devinfo->p_uart_devinfo->p_hw_uart, AMHW_FSL_UART_INT_C2_IRIE);
        }
        am_int_enable(p_uart_dev->p_devinfo->inum);

    } else {
        /* 关闭所有串口中断 */
        amhw_fsl_uart_int_disable(p_hw_uart, AMHW_FSL_UART_INT_ALL);
    }

    p_uart_dev->channel_mode = new_mode;

    return (AM_OK);
}

/**
 * \brief 串口选项配置
 */
static int __uart_dma_opt_set (void *p_drv, uint32_t options)
{
    uint8_t            cfg_flags = 0;
    am_fsl_uart_dev_t *p_dev     = (am_fsl_uart_dev_t *)p_drv;
    amhw_fsl_uart_t   *p_hw_uart = p_dev->p_devinfo->p_hw_uart;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    p_hw_uart = p_dev->p_devinfo->p_hw_uart;

    /* 在改变UART寄存器值前 接收发送禁能 */
    amhw_fsl_uart_disable(p_hw_uart);

    /* 配置数据长度 */
    switch (options & AM_UART_CSIZE) {

    case AM_UART_CS8:
        cfg_flags &= ~(1 << 4);
        cfg_flags |= AMHW_FSL_UART_C1_M_8BIT;
        break;

    default:
        break;
    }

    /* 配置停止位 */
    if (options & AM_UART_STOPB) {
        cfg_flags &= ~(0x01 << 5);
        cfg_flags |= AMHW_FSL_UART_BDH_SBNS_STOP_2;
    } else {
        cfg_flags &= ~(0x01 << 5);
        cfg_flags |= AMHW_FSL_UART_BDH_SBNS_STOP_1;
    }

    /* 配置检验方式 */
    if (options & AM_UART_PARENB) {
        cfg_flags &= ~(0x03 << 0);

        if (options & AM_UART_PARODD) {
            cfg_flags |= AMHW_FSL_UART_C1_PARITY_ODD;
        } else {
            cfg_flags |= AMHW_FSL_UART_C1_PARITY_EVEN;
        }
    } else {
        cfg_flags &= ~(0x03 << 0);
        cfg_flags |= AMHW_FSL_UART_C1_PARITY_NO;
    }

    /* 保存和生效配置 */
    amhw_fsl_uart_stop_bit_set (p_hw_uart, (cfg_flags & 0x20));
    amhw_fsl_uart_data_mode_set(p_hw_uart, (cfg_flags & 0x10));
    amhw_fsl_uart_parity_set(p_hw_uart,    (cfg_flags & 0x03));

    amhw_fsl_uart_enable(p_hw_uart);

    p_dev->options = options;

    return (AM_OK);
}

/******************************************************************************
  UART interrupt request handler
******************************************************************************/
/**
 * \brief 串口接收空闲服务
 */
static void __uart_dma_irq_idle_handler (am_kl26_uart_dma_dev_t *p_dev)
{
    char               data;
    int                count      = 0;
    am_fsl_uart_dev_t *p_uart_dev = &p_dev->uart_dev;
    am_softimer_stop(&p_dev->timer);
    count1++;
    /* 接收若为中断接收     则循环调用用户数据获取回调函数    直至接收缓冲区为空*/
    __dma_buffer_data_get(p_dev, 1);

    count = am_rngbuf_nbytes(&p_dev->rngbuf);

    for( ; count > 0 ; count --){
        am_rngbuf_get(&p_dev->rngbuf, &data, 1);
        p_uart_dev->pfn_rxchar_put(p_uart_dev->rxput_arg, data);
    }
}

/**
 * \brief 串口中断服务函数
 */
void __uart_irq_handler (void *p_arg)
{
    am_kl26_uart_dma_dev_t *p_dev      = (am_kl26_uart_dma_dev_t *)p_arg;
    am_fsl_uart_dev_t      *p_uart_dev = (&p_dev->uart_dev);
    amhw_fsl_uart_t        *p_hw_uart  = p_uart_dev->p_devinfo->p_hw_uart;
    uint32_t  uart_int_stat;


    if(p_uart_dev->p_devinfo->ver == AM_FSL_UART_VER0){
        uart_int_stat = amhw_fsl_uart_ver0_intstat_get(p_hw_uart);
    }else{
        uart_int_stat = amhw_fsl_uart_ver1_intstat_get(p_hw_uart);
    }
    if (uart_int_stat & AMHW_FSL_UART_INTSTAT_S1_IDLE) {
        /*串口出发空闲中断说明此次传输完成     需对传输数据进行处理*/
        if(p_hw_uart != KL26_UART0){
            /* 此句代码不可删除   KL26非UART0时，需读串口数据寄存器以清除空闲中断 */
            char a = p_hw_uart->data;
            //*((unsigned int *)0x4006A004) |= 0xc0;
        }
        __uart_dma_irq_idle_handler((void *)p_dev);
    }
    else if (uart_int_stat & AMHW_FSL_UART_INTSTAT_S1_TDRE) {
        __uart_tx_handler((am_fsl_uart_dev_t *)p_dev);
    } else  {

    }
    /* 其他中断 */
    if ((p_uart_dev->other_int_enable & uart_int_stat) != 0) {

        uart_int_stat &= p_uart_dev->other_int_enable;

        if (p_uart_dev->pfn_err != NULL) {
            p_uart_dev->pfn_err(p_uart_dev->err_arg,
                           AM_FSL_UART_ERRCODE_UART_OTHER_INT,
                           (void *)uart_int_stat,
                           1);
        }
    }
}
/* DMA传输完成中断函数 */
static void __uart_dma_isr_handler(void *p_arg , uint8_t flag)
{
    int  count = 0;
    char data;
    am_kl26_uart_dma_dev_t *p_dev = (am_kl26_uart_dma_dev_t  *)p_arg;
    am_fsl_uart_dev_t *p_uart_dev = &p_dev->uart_dev;
    am_softimer_stop(&p_dev->timer);
    __dma_buffer_data_get(p_arg,2);
    am_softimer_start(&p_dev->timer, 4);
    count = am_rngbuf_nbytes(&p_dev->rngbuf);
    count2 ++;
    for( ; count > 0 ; count --){
        am_rngbuf_get(&p_dev->rngbuf, &data, 1);
        p_uart_dev->pfn_rxchar_put(p_uart_dev->rxput_arg, data);
    }
}

static uint8_t uart_dma_src_request_set (amhw_fsl_uart_t   *p_hw_uart){
    if (p_hw_uart == KL26_UART0) {
        return DMA_REQUEST_MUX0_UART0RX;
    } else if (p_hw_uart == KL26_UART1) {
        return DMA_REQUEST_MUX0_UART1RX;
    } else {
        return DMA_REQUEST_MUX0_UART2RX;
    }
}

/**
 * \brief UART接收DMA传输配置
 */
static int uart_rx_dma_tran_cfg (am_kl26_uart_dma_dev_t   *p_dev,
                                 uint32_t                  dma_tran_len)
{
    uint32_t flags;
    am_fsl_uart_dev_t *p_uart_dev = (am_fsl_uart_dev_t *)(&p_dev->uart_dev);
    amhw_fsl_uart_t   *p_hw_uart  = p_uart_dev->p_devinfo->p_hw_uart;
    const am_kl26_uart_dma_devinfo_t *p_devinfo = p_dev->p_devinfo;
    static amhw_kl26_dma_xfer_desc_t  desc;    /**< \brief DMA 重载描述符 */

    /* DMA传输配置 */
    flags = KL26_DMA_DCR_PER_REQUEST_ENABLE     |  /* 外设请求源使能        */
            KL26_DMA_DCR_SINGLE_TRANSFERS       |  /* 单次传输                    */
            KL26_DMA_DCR_AUTO_ALIGN_DISABLE     |  /* 自动对齐禁能            */
            KL26_DMA_DCR_SOURCE_SIZE_8_BIT      |  /* 源地址1字节读取     */
            KL26_DMA_DCR_DESTINATION_SIZE_8_BIT |  /* 目的地址1字节写入 */
            KL26_DMA_DCR_REQUEST_NO_AFFECTED    |  /* 请求无影响                */
            KL26_DMA_DCR_NO_LINKING             |  /* 无通道连接                */
            KL26_DMA_DCR_INTERRUTP_ENABLE       |  /* DMA中断使能           */
            KL26_DMA_DCR_START_DISABLE ;           /* DMA开始传输使能   */

    /* 连接DMA中断服务函数 */
    am_kl26_dma_isr_connect(p_devinfo->dma_chan_rx, __uart_dma_isr_handler, (void *)p_dev);

    am_int_enable(p_devinfo->dma_chan_rx);

    am_kl26_dma_chan_cfg(p_devinfo->dma_chan_rx,
                         KL26_DMA_TRIGGER_DISABLE |            /**< \brief DMA正常模式 */
                         uart_dma_src_request_set(p_hw_uart)); /**< \brief 请求源      */

    /* 建立通道描述符 */
    am_kl26_dma_xfer_desc_build(&desc,                          /* 通道描述符   */
                      (uint32_t)&p_hw_uart->data,               /* 源端数据缓冲 */
                       (uint32_t)p_dev->p_dma_buffer,           /* 目标数据缓冲 */
                       (uint32_t)(p_devinfo->dmabuffer_size/2), /* 传输字节数  此处设置为DMA缓冲区大小的1/2是为了使DMA在接收到了设定大小的1/2时产生数据满中断 */
                                 flags);                        /* 传输配置     */

    /* 启动DMA传输，马上开始传输 */
    if (am_kl26_dma_chan_start(&desc,
                               KL26_DMA_PER_TO_MER,          /* 外设到 内存   */
                      (uint8_t)p_devinfo->dma_chan_rx) == AM_ERROR) {
        return AM_ERROR;
    }

    /* 设置DMA环形缓冲区大小   */
    amhw_kl26_dma_destination_buffer_set(p_devinfo->p_hw_dma,
                                         p_devinfo->dma_circular_size,
                                         p_devinfo->dma_chan_rx);

    return AM_OK;
}
/**
 * \brief 默认回调函数
 *
 * \returns AW_ERROR
 */
static int __uart_dummy_callback (void *p_arg, char *p_outchar)
{
    return (AM_ERROR);
}

static void uart_hw_dma_init (am_kl26_uart_dma_dev_t   *p_dev)
{
    am_fsl_uart_dev_t *p_uart_dev =&p_dev->uart_dev;
    amhw_fsl_uart_t   *p_hw_uart  = p_uart_dev->p_devinfo->p_hw_uart;


    if(p_dev->p_devinfo->p_uart_devinfo->p_hw_uart== KL26_UART0){
        amhw_fsl_uart_ver0_dma_rx_enable(p_dev->p_devinfo->p_uart_devinfo->p_hw_uart);
    }else {
        amhw_fsl_uart_ver1_dma_rx_enable(p_dev->p_devinfo->p_uart_devinfo->p_hw_uart);
    }

    /* 非uart0接收时DMA传输需要使能中断 */
    if (p_hw_uart != KL26_UART0) {
       /* 使能接收中断，中断信号触发DMA传输 */
       amhw_fsl_uart_int_enable(p_hw_uart, AMHW_FSL_UART_INT_C2_IRIE);
    }

    /* 使能串口 */
    amhw_fsl_uart_enable(p_hw_uart);
}

/**
 * \brief 串口初始化函数
 */
am_uart_handle_t am_uart_dma_init (
                     am_kl26_uart_dma_dev_t           *p_dev,
                     const am_kl26_uart_dma_devinfo_t *p_devinfo)
{
    uint32_t                      tmp;
    am_fsl_uart_dev_t            *p_uart_dev = &p_dev->uart_dev;
    amhw_fsl_uart_t              *p_hw_uart;
    const am_fsl_uart_devinfo_t  *p_uart_devinfo =
             (const am_fsl_uart_devinfo_t *)p_devinfo->p_uart_devinfo;

    if (p_devinfo == NULL || p_devinfo->p_uart_devinfo->p_hw_uart == NULL) {
        return NULL;
    }
    p_dev->p_dma_buffer     = p_devinfo->p_dma_buffer;

    p_dev->buffer_size      = p_devinfo->recbuffer_size;
    p_dev->p_rec_buffer     = p_devinfo->p_rec_buffer;
    p_dev->dma_size         = p_devinfo->dmabuffer_size;
    p_dev->count            = 0;

    p_dev->p_devinfo        = p_devinfo;
    p_uart_dev->p_devinfo   = p_devinfo->p_uart_devinfo;

    p_hw_uart = p_uart_dev->p_devinfo->p_hw_uart;
    /* 获取配置参数 */
    p_uart_dev->p_devinfo         = (const  am_fsl_uart_devinfo_t *)p_uart_devinfo;
    p_uart_dev->uart_serv.p_funcs = (struct am_uart_drv_funcs *)&__g_uart_drv_funcs;
    p_uart_dev->uart_serv.p_drv   = p_dev;
    p_uart_dev->baud_rate         = p_uart_devinfo->baud_rate;
    p_uart_dev->options           = 0;

    /* 初始化默认回调函数 */
    p_uart_dev->pfn_txchar_get    = (int (*) (void *, char*))__uart_dummy_callback;
    p_uart_dev->txget_arg         = NULL;
    p_uart_dev->pfn_rxchar_put    = (int (*) (void *, char ))__uart_dummy_callback;
    p_uart_dev->rxput_arg         = NULL;
    p_uart_dev->pfn_err           =
                          (int (*) (void *, int, void*, int))__uart_dummy_callback;

    p_uart_dev->err_arg           = NULL;

    p_uart_dev->other_int_enable  = p_uart_devinfo->other_int_enable  &
                                  ~(AMHW_FSL_UART_INT_C2_TCIE |
                                    AMHW_FSL_UART_INT_C2_IRIE |
                                    AMHW_FSL_UART_INT_C2_ILIE |
                                    AMHW_FSL_UART_INT_C2_TIE);

    if (p_uart_dev->p_devinfo->pfn_plfm_init) {
        p_uart_dev->p_devinfo->pfn_plfm_init();
    }

    /* 获取串口数据长度配置选项 */
    tmp = p_uart_devinfo->cfg_flags;
    tmp = (tmp >> 4) & 0x01;

    switch (tmp) {

    case 0:
        p_uart_dev->options |= AM_UART_CS7;
        break;

    case 1:
        p_uart_dev->options |= AM_UART_CS8;
        break;

    default:
        p_uart_dev->options |= AM_UART_CS8;
        break;
    }

    /* 获取串口检验方式配置选项 */
    tmp = p_uart_devinfo->cfg_flags;
    tmp = (tmp >> 0) & 0x03;

    if (tmp == 2) {
        p_uart_dev->options |= AM_UART_PARENB;
    } else if (tmp == 3) {
        p_uart_dev->options |= (AM_UART_PARENB | AM_UART_PARODD);
    }else{
    }
    /* 获取串口停止位配置选项 */
    if (p_uart_devinfo->cfg_flags & AMHW_FSL_UART_BDH_SBNS_STOP_2) {
        p_uart_dev->options |= AM_UART_STOPB;
    }else{

    }
    __uart_dma_opt_set (p_dev, p_uart_dev->options);

    /* 设置波特率 */
    if(p_uart_devinfo->ver == AM_FSL_UART_VER0){
        p_uart_dev->baud_rate  = amhw_fsl_uart_ver0_baudrate_set(p_hw_uart,
                am_clk_rate_get(p_uart_dev->p_devinfo->clk_id),
                p_uart_devinfo->baud_rate);
    }else{
        p_uart_dev->baud_rate  = amhw_fsl_uart_ver1_baudrate_set(p_hw_uart,
                am_clk_rate_get(p_uart_dev->p_devinfo->clk_id),
                p_uart_devinfo->baud_rate);
    }

    am_softimer_init(&p_dev->timer, am_softimer_cb, (void *)p_dev);

    amhw_fsl_uart_int_disable(p_hw_uart, AMHW_FSL_UART_INT_ALL);

    /* 默认轮询模式  */
    __uart_dma_mode_set(p_dev, AM_UART_MODE_POLL);

    am_rngbuf_init(&p_dev->rngbuf, (char *)p_dev->p_rec_buffer, p_dev->buffer_size);

    uart_hw_dma_init(p_dev);

    uart_rx_dma_tran_cfg(p_dev, p_dev->buffer_size);

    /* uart使能 */
    amhw_fsl_uart_enable(p_hw_uart);

    return &(p_dev->uart_dev.uart_serv);
}

/**
 * \brief DMA串口设备去初始化
 */
void am_uart_dma_deinit (am_fsl_uart_dev_t  *p_drv)
{
    am_kl26_uart_dma_dev_t *p_dev  = p_drv->uart_serv.p_drv;
    am_fsl_uart_dev_t *p_uart_dev  = &p_dev->uart_dev;
    amhw_fsl_uart_t   *p_hw_uart   = p_uart_dev->p_devinfo->p_hw_uart;
    p_uart_dev->uart_serv.p_funcs  = NULL;
    p_uart_dev->uart_serv.p_drv    = NULL;

    if (p_uart_dev->channel_mode == AM_UART_MODE_INT) {
        /* 默认为轮询模式 */
        __uart_dma_mode_set(p_dev, AM_UART_MODE_POLL);
    }
    /* 关闭串口 */
    amhw_fsl_uart_disable(p_hw_uart);
    am_kl26_dma_chan_stop(p_dev->p_devinfo->dma_chan_rx);
    am_kl26_dma_isr_disconnect(p_dev->p_devinfo->dma_chan_rx, NULL, NULL);

    if (p_uart_dev->p_devinfo->pfn_plfm_deinit) {
        p_uart_dev->p_devinfo->pfn_plfm_deinit();
    }

    am_int_disable(p_uart_dev->p_devinfo->inum);
}

/* end of file */

