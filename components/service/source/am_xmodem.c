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
 * \brief  Xmodem协议 驱动
 *
 * \internal
 * \par Modification History
 * - 1.00 18-8-31 , xgg, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "string.h"
#include "am_types.h"
#include "am_xmodem.h"

/*******************************************************************************
local defined
*******************************************************************************/
/* 文件发送完成*/
#define __AM_XMODEM_DOC_EOT     0x51

/* 文件结束信号已发送并应答*/
#define __AM_XMODEM_EOT_ACK     0x52

/* 数据位为128字节工作模式传输的起始位 */
#define __AM_XMODEM_SOH         0x01

/* 数据位为1K字节工作模式传输的起始位*/
#define __AM_XMODEM_STX         0x02

/* 文件传输完毕的结束信号 */
#define __AM_XMODEM_EOT         0x04

/* 填充数据 */
#define __AM_XMODEM_CTRLZ       0x1A

/* 串口获取发送字符关闭状态*/
#define __AM_XMODEM_CHAR_DOWN   0x00

/* 串口获取发送字符打开状态*/
#define __AM_XMODEM_CHAR_UP     0x01

/* Xmodem已经建立连接*/
#define __AM_XMODEM_CON_UP      0x02

/* Xmodem未建立连接*/
#define __AM_XMODEM_CON_DOWN    0x03

/* 正在发送数据*/
#define __AM_XMODEM_DATA_TX     0x04

/* 正在发送传输标志*/
#define __AM_XMODEM_FLAG_TX     0x05

/* 禁止获取发送字符*/
#define __AM_XMODEM_STOP_TX     0x06

/* Xmodem发送就绪*/
#define __AM_XMODEM_TX_READY    0x00

/* Xmodem发送启动*/
#define __AM_XMODEM_TX_START    0x01
/******************************************************************************/
/** \brief 帧头接受函数*/
am_local am_bool_t am_xmodem_rec_frames (am_xmodem_rec_dev_t *p_dev,
                                         char                 inchar);

/** \brief 开始接受函数*/
am_local void  __xmodem_rx_startup (am_xmodem_rec_dev_t  *p_dev);

/******************************************************************************/
/**
 * \brief 发送方紧急取消发送函数
 */
am_local void __xmodem_rec_can (am_xmodem_rec_dev_t *p_dev, char inchar)
{
    p_dev->rx_bytes = -AM_DATA_CAN;
    /* 回调通知用户 发送方紧急取消传输*/
    p_dev->pfn_rx_callback(p_dev->p_arg,
                           p_dev->p_rec_devinfo->frames_info,
                           p_dev->rx_bytes);
}

/******************************************************************************/
/**
 * \brief 接收方检验判断函数
 */
am_local int __xmodem_data_check(am_xmodem_rec_dev_t *p_dev)
{
    int       i;
    char      cks = 0;
    uint32_t  crc;
    uint16_t  tcrc;
    if (p_dev->p_rec_devinfo->parity_mode == AM_XMODEM_CRC_MODE)
    {
        am_crc_cal(p_dev->crc_handle,
                   (uint8_t *)p_dev->p_rec_devinfo->frames_info,
                   p_dev->p_rec_devinfo->frames_bytes);
        am_crc_final(p_dev->crc_handle, &crc);
        tcrc = p_dev->fra_crc_pry;
        /* 将自己计算的校验码和传输得到的校验码进行比较*/
        if ((uint16_t)crc == tcrc) {
            return AM_TRUE;
        }
    }
    else
    {
        /* 将数据累加求和*/
        for (i = 0; i < p_dev->p_rec_devinfo->frames_bytes; i++)
        {
            cks += p_dev->p_rec_devinfo->frames_info[i];
        }
        /* 将自行计算的累加和与传输过来的累加和进行比较*/
        if (cks == p_dev->fra_sum_parity)
            return AM_TRUE;
    }
    return AM_FALSE;
}

/******************************************************************************/
/**
 * \brief 接收到EOT结束函数
 */
am_local am_bool_t __xmodem_rec_eot (am_xmodem_rec_dev_t *p_dev, char inchar)
{
    /* 发送字符获取成功*/
    p_dev->char_get_state = __AM_XMODEM_CHAR_UP;

    /* 接受到结束字符后回应发送方 */
    p_dev->p_rec_devinfo->frames_info[0] = AM_XMODEM_ACK;

    /* 使能中断*/
    am_uart_tx_startup(p_dev->uart_handle);

    /* 发送成功*/
    p_dev->rx_bytes = -AM_DATA_SUC;

    /* 回调通知用户 传输结束*/
    p_dev->pfn_rx_callback(p_dev->p_arg,
                           p_dev->p_rec_devinfo->frames_info,
                           p_dev->rx_bytes);

    return AM_TRUE;
}

/******************************************************************************/
/**
 * \brief SUM校验函数
 */
am_local am_bool_t __xmodem_rec_sum (am_xmodem_rec_dev_t *p_dev, char inchar)
{
    p_dev->fra_sum_parity = inchar;
    if (AM_TRUE == __xmodem_data_check(p_dev))
       {
          if (p_dev->pfn_rx_callback != NULL) {
              p_dev->frames_num++;
              p_dev->pfn_rx_callback(p_dev->p_arg,
                                     p_dev->p_rec_devinfo->frames_info,
                                     p_dev->rx_bytes);
              p_dev->rx_bytes = 0;

              return AM_TRUE;
          }
       }
    return AM_FALSE;
}

/******************************************************************************/
/**
 * \brief CRC校验函数
 */
am_local am_bool_t __xmodem__rec_crc (am_xmodem_rec_dev_t *p_dev, char inchar)
{
    uint8_t u_inchar = (uint8_t)inchar;

    if (p_dev->rx_bytes == p_dev->p_rec_devinfo->frames_bytes) {
        p_dev->fra_crc_pry |= (u_inchar << 8);
        p_dev->rx_bytes ++;
        return AM_TRUE;
    }

    if (p_dev->rx_bytes == p_dev->p_rec_devinfo->frames_bytes + 1) {
        p_dev->fra_crc_pry |= u_inchar;
        p_dev->rx_bytes--;
        if (AM_TRUE == __xmodem_data_check(p_dev)) {
            p_dev->pfn_rx_callback(p_dev->p_arg,
                                   p_dev->p_rec_devinfo->frames_info,
                                   p_dev->p_rec_devinfo->frames_bytes);
            p_dev->rx_bytes = 0;
            p_dev->frames_num++;
            p_dev->fra_crc_pry = 0;
            return AM_TRUE;
        }
    }

    return AM_FALSE;
}

/*******************************************************************************
      状态机函数
*******************************************************************************/
/**
 * \brief 数据段保存函数
 */
am_local am_bool_t __xmodem_rec_data_rec (am_xmodem_rec_dev_t *p_dev, char inchar)
{
    /* 数据接收*/
    if (p_dev->rx_bytes < p_dev->p_rec_devinfo->frames_bytes) {
        p_dev->p_rec_devinfo->frames_info[p_dev->rx_bytes] = inchar;
        p_dev->rx_bytes++;
        return AM_TRUE;
    }
    /* 数据接收完毕，接收sum校验码*/
    if (AM_XMODEM_SUM_MODE == p_dev->p_rec_devinfo->parity_mode) {
        return __xmodem_rec_sum(p_dev, inchar);

    }
    /* 数据接收完毕，接收crc校验码*/
    if (AM_XMODEM_CRC_MODE == p_dev->p_rec_devinfo->parity_mode) {
        return __xmodem__rec_crc(p_dev, inchar);
    }
    return AM_FALSE;
}

/**
 * \brief 序列号反码判断
 */
am_local am_bool_t __xmodem_rec_rad (am_xmodem_rec_dev_t *p_dev, char inchar)
{
    if (inchar == ~ (char)p_dev->frames_num) {
        /* 更改到下一个接受状态*/
        p_dev->p_rec_func = (pfn_xmodem_rx_t)__xmodem_rec_data_rec;
        return AM_TRUE;
    }

    return AM_FALSE;
}

/**
 * \brief 序列号接受函数
 */
am_local am_bool_t __xmodem_rec_pack (am_xmodem_rec_dev_t *p_dev, char inchar)
{
    if (p_dev->frames_num != (uint8_t)inchar) {
        return AM_FALSE;
    }
    p_dev->p_rec_func = (pfn_xmodem_rx_t)__xmodem_rec_rad;

    return AM_TRUE;
}

/**
 * \brief 状态机开始传输函数
 */
am_local am_bool_t am_xmodem_rec_frames (am_xmodem_rec_dev_t *p_dev, char inchar)
{
    if ((uint8_t)inchar == __AM_XMODEM_SOH ||
        (uint8_t)inchar == __AM_XMODEM_STX) {

        p_dev->con_state = __AM_XMODEM_CON_UP;

        p_dev->p_rec_func = (pfn_xmodem_rx_t)__xmodem_rec_pack;

        return AM_TRUE;
    }
    return AM_FALSE;
}

/*****************************************************************************/
/**
 * \brief 重新接收上一帧函数
 */
am_local void __xmodem_rx_nak_set (am_xmodem_rec_dev_t  *p_dev)
{
    /* 计算重发次数 */
    ++p_dev->nak_state;

    /* 判断是否达到最大重发次数*/
    if (p_dev->nak_state == p_dev->p_rec_devinfo->nak_max_times) {
        /* 要求最大次数的重发完成 */
        p_dev->nak_state = AM_OK;
        p_dev->rx_bytes  = -AM_EBADMSG;
        /* 未能接受到正确的数据，调用用户回调函数通知用户数据错误 */
        p_dev->pfn_rx_callback(p_dev->p_arg,
                               p_dev->p_rec_devinfo->frames_info,
                               p_dev->rx_bytes);
        return;
    }
    p_dev->p_rec_func     = (pfn_xmodem_rx_t)am_xmodem_rec_frames;
    p_dev->char_get_state = __AM_XMODEM_CHAR_UP;
    /* 向发送方发送NAK重发信号*/
    p_dev->p_rec_devinfo->frames_info[0] = AM_XMODEM_NAK;

    am_uart_tx_startup(p_dev->uart_handle);
}

/******************************************************************************/
/**
 * \brief 取消发送函数
 */
am_err_t am_xmodem_rec_can_set (am_xmodem_rec_handle_t handle)
{
   uint8_t i = 0;

   if (handle == NULL) {
      return -AM_EINVAL;
   }

   /* 重置Xmodem接收状态机函数*/
   handle->p_rec_func = (pfn_xmodem_rx_t)am_xmodem_rec_frames;

   /* 取消发送命令，连发三次防止发送方未能接到*/
   for (i = 0; i < 3; i++) {
      /* 保存发送的取消标志*/
      handle->p_rec_devinfo->frames_info[0] = AM_XMODEM_CAN;
      handle->char_get_state = __AM_XMODEM_CHAR_UP;
      /* 开启发送中断 */
      am_uart_tx_startup(handle->uart_handle);
      while(handle->char_get_state == __AM_XMODEM_CHAR_UP);
   }

   return AM_OK;
}

/******************************************************************************/
/**
 * \brief 继续接收函数
 */
am_err_t am_xmodem_rec_ack_set (am_xmodem_rec_handle_t handle)
{
    if (handle == NULL) {
       return -AM_EINVAL;
    }

    /* 重置重发次数*/
    handle->nak_state      = 0;

    /* 当前Xmodem连接已经建立*/
    handle->con_state      = __AM_XMODEM_CON_UP;

    /* 重置接受状态机函数*/
    handle->p_rec_func     = (pfn_xmodem_rx_t)am_xmodem_rec_frames;

    /* 当前为接受状态态*/
    handle->char_get_state = __AM_XMODEM_CHAR_UP;

    /* 发送ACK确认信号， 响应发送方发送上一帧数据 */
    handle->p_rec_devinfo->frames_info[0] = AM_XMODEM_ACK;

    /* 开启发送中断 */
    am_uart_tx_startup(handle->uart_handle);

    /* 定时等待Xmodem上位机的回应*/
    am_softimer_start(&handle->rx_softimer, \
                       handle->p_rec_devinfo->rx_timeout);

    return AM_OK;
}

/******************************************************************************/
/**
 * \brief xmodem接收回调函数注册
 */
am_err_t am_xmodem_rec_cb_reg (am_xmodem_rec_handle_t handle,
                                am_xmodem_user_rx_t   pfn_rec_cb,
                                void                 *p_arg)
{
    if (pfn_rec_cb == NULL || pfn_rec_cb == NULL) {
       return -AM_EINVAL;
    }

    handle->p_arg = p_arg;

    handle->pfn_rx_callback = pfn_rec_cb;

    return AM_OK;
}

/******************************************************************************/
/**
 * \brief 软件定时器回调函数
 *
 * \param[in] p_arg 指向XMODEM 设备结构体的指针
 *
 * \return 无
 */
am_local void __xmodem_rec_softimer_callback (void *p_arg)
{
    am_xmodem_rec_dev_t *p_dev = (am_xmodem_rec_dev_t *)p_arg;
    /* 关闭软件定时器 */
    am_softimer_stop(&p_dev->rx_softimer);
    //如果未能超时没建立连接，则重新连接
    if (p_dev->con_state == __AM_XMODEM_CON_DOWN) {
        ++p_dev->nak_state;
        if (p_dev->nak_state == p_dev->p_rec_devinfo->nak_max_times) {
            p_dev->nak_state = 0;
            p_dev->pfn_rx_callback(p_dev->p_arg,
                                   p_dev->p_rec_devinfo->frames_info,
                                   -AM_XMODEM_DOWN);
            return;
        }
        am_xmodem_rec_start(p_dev);
        return;
    }
    /*设立标志位，超时未接收到数据*/
    p_dev->rx_bytes = -AM_ETIME;
    if (p_dev->pfn_rx_callback != NULL) {
        p_dev->pfn_rx_callback(p_dev->p_arg,
                               p_dev->p_rec_devinfo->frames_info,
                               p_dev->rx_bytes);
    }
}

/******************************************************************************/
/**
 * \brief 字符获取函数
 */
am_local int __xmodem_rec_getchar (am_xmodem_rec_dev_t  *p_dev, char *p_data)
{
    /* 接受状态时，只发送数组的第一个字节中的数据 */
    if (p_dev->char_get_state == __AM_XMODEM_CHAR_UP) {

        /* 获取需要发送的第一个字符*/
       *p_data = p_dev->p_rec_devinfo->frames_info[0];
        /* 发送一次后，将不再允许获取发送字符*/
        p_dev->char_get_state = __AM_XMODEM_CHAR_DOWN;

        return AM_TRUE;
    }
    return AM_OK;
}

/******************************************************************************/
/**
 * \brief Xmodem获取一个字符发送函数
 */
am_local int __xmodem_rec_txchar_get (void *p_arg, char *p_outchar)
{
    am_xmodem_rec_dev_t *p_dev = (am_xmodem_rec_dev_t *)p_arg;
    if (__xmodem_rec_getchar(p_dev, p_outchar) != AM_TRUE) {
        return -AM_EEMPTY;
    }
    return AM_OK;
}

/******************************************************************************/
/**
 * \brief 数据错误状态函数
 */
am_local am_bool_t __xmodem_rec_data_err (am_xmodem_rec_dev_t *p_dev,
                                          char                 inchar)
{
    int ret = 0;
    /* 判断错误信息，若发送方紧急取消发送，则回调通知用户*/
    if (inchar == AM_XMODEM_CAN) {
        p_dev->rx_bytes++;
        if (p_dev->rx_bytes > 2) {
            __xmodem_rec_can(p_dev, inchar);
        }
        return AM_TRUE;
    }
    if (inchar == __AM_XMODEM_EOT &&
        p_dev->p_rec_func == (pfn_xmodem_rx_t)am_xmodem_rec_frames) {

        return __xmodem_rec_eot(p_dev, inchar);
    }
    if (inchar == AM_XMODEM_ACK) {
        return AM_TRUE;
    }
    if (p_dev->rx_bytes == p_dev->p_rec_devinfo->frames_bytes) {
        ret = -AM_XMODEM_CHECK_ERR;
        if (p_dev->pfn_rx_callback != NULL) {
            p_dev->pfn_rx_callback(p_dev->p_arg,
                                   p_dev->p_rec_devinfo->frames_info,
                                   ret);
        }
        return AM_TRUE;
    } else if (p_dev->p_rec_func == (pfn_xmodem_rx_t)__xmodem_rec_data_rec) {
        __xmodem_rx_nak_set(p_dev);
        return AM_TRUE;
    }
    //如果不是以上错误，则不做任何处理，直接返回
    return AM_FALSE;
}

/******************************************************************************/
/**
 * \brief Xmodem数据接收函数
 */
am_local void __xmodem_rx_char (void *p_arg, char inchar)
{
    am_xmodem_rec_dev_t *p_dev = (am_xmodem_rec_dev_t *)p_arg;

    //获取当前状态函数
    pfn_xmodem_rx_t pfn_rx_func = (pfn_xmodem_rx_t)p_dev->p_rec_func;

    //若失败则数据错误，请求重发或取消
    if (AM_FALSE == pfn_rx_func(p_dev, inchar)) {
        if (AM_TRUE == __xmodem_rec_data_err(p_dev, inchar)) {
            am_softimer_stop (&p_dev->rx_softimer);
        }
    }else {
        am_softimer_stop (&p_dev->rx_softimer);
    }
}

/******************************************************************************/
/**
 * \brief Xmodem开始接收函数
 */
am_local void  __xmodem_rx_startup (am_xmodem_rec_dev_t  *p_dev)
{
    /* 根据工作模式来选择发送给发送方的字符*/
    switch (p_dev->p_rec_devinfo->parity_mode) {
       /* 工作模式为数据位为128字节时，发送字符NAK*/
       case AM_XMODEM_SUM_MODE:
                 p_dev->p_rec_devinfo->frames_info[0] = AM_XMODEM_NAK;
                 break;
       /* 工作模式为数据位为1K字节时，发送字符C*/
       case AM_XMODEM_CRC_MODE:
                 p_dev->p_rec_devinfo->frames_info[0] = 'C';
                 break;
    }
    /* 发送启动字符*/
    p_dev->char_get_state = __AM_XMODEM_CHAR_UP;
    /* 打开发送中断*/
    am_uart_tx_startup(p_dev->uart_handle);
    /* 打开定时器*/
    am_softimer_start(&p_dev->rx_softimer, p_dev->p_rec_devinfo->start_time);
}

/******************************************************************************/
/**
 * \brief Xmodem文件接收函数
 */
am_err_t am_xmodem_rec_start (am_xmodem_rec_handle_t handle)
{
    /* 开始接收 */
    if (handle != NULL) {
        __xmodem_rx_startup(handle);
        return AM_OK;
    }
    return -AM_EINVAL;
}

/******************************************************************************/
/**
 * \brief 设置CRC-16
 */
static void __crc_16_pattern_set(am_crc_handle_t   crc_handle,
                                 am_crc_pattern_t *p_crc_pattern)
{
    /* 定义 CRC-16 模型 */
    p_crc_pattern->width     = 16;       /* CRC 宽度(如：CRC5 的宽度为 5) */
    p_crc_pattern->poly      = 0x1021;   /* CRC 生成多项式 */
    p_crc_pattern->initvalue = 0x0000;   /* CRC 初始值 */
    p_crc_pattern->refin     = AM_FALSE; /* 如果输入数据需要位反转，则该值为 TRUE */
    p_crc_pattern->refout    = AM_FALSE; /* 如果输出结果值需要位反转，则该值为 TRUE */
    p_crc_pattern->xorout    = 0x0000;   /* 输出异或值 */

    /* 初始化CRC*/
    am_crc_init(crc_handle, p_crc_pattern);
}

/**
 * \brief 接收设备初始化函数
 */
am_xmodem_rec_handle_t  am_xmodem_rec_init (
                            am_xmodem_rec_dev_t            *p_dev,
                            const am_xmodem_rec_dev_info_t *p_rec_devinfo,
                            am_uart_handle_t                uart_handle,
                            am_crc_handle_t                 crc_handle)
{
    if ((NULL == p_dev) || (NULL == p_rec_devinfo) || (NULL == uart_handle)) {
        return NULL;
    }
    p_dev->p_rec_devinfo = (am_xmodem_rec_dev_info_t *)p_rec_devinfo;
    /* 帧序列号初始值为1*/
    p_dev->frames_num      = 1;
    /* 初始化累积和校验位为0 */
    p_dev->fra_sum_parity  = 0;
    /* 初始化CRC校验 */
    p_dev->fra_crc_pry     = 0;
    /* 初始化回调函数为NULL*/
    p_dev->pfn_rx_callback = NULL;
    /* 初始化当前接收字节数 */
    p_dev->rx_bytes        = 0;
    /* 初始化当前重发状态为0 */
    p_dev->nak_state       = 0;
    /* Xmodem当前连接状态*/
    p_dev->con_state       = __AM_XMODEM_CON_DOWN;
    /* 保存Xmodem需要的串口句柄*/
    p_dev->uart_handle     = uart_handle;
    /* 保存标准CRC句柄*/
    p_dev->crc_handle      = crc_handle;
    /* 发送字符获取默认为0 */
    p_dev->char_get_state  = __AM_XMODEM_CHAR_DOWN;
    /** 状态机函数句柄*/
    p_dev->p_rec_func      = (pfn_xmodem_rx_t)am_xmodem_rec_frames;

    __crc_16_pattern_set(crc_handle, &p_dev->crc_pattern);

    /* 使能串口中断模式 */
    am_uart_ioctl(p_dev->uart_handle,
                  AM_UART_MODE_SET,
                  (void *)AM_UART_MODE_INT);
    /* 注册发送回调函数 */
    am_uart_callback_set(p_dev->uart_handle,
                         AM_UART_CALLBACK_TXCHAR_GET,
                         __xmodem_rec_txchar_get,
                         (void *)(p_dev));
    /* 注册接收回调函数 */
    am_uart_callback_set(p_dev->uart_handle,
                         AM_UART_CALLBACK_RXCHAR_PUT,
                         __xmodem_rx_char,
                         (void *)(p_dev));

    if (p_rec_devinfo->rx_timeout != 0) {
        if (am_softimer_init(&p_dev->rx_softimer,
                              __xmodem_rec_softimer_callback,
                              p_dev) != AM_OK) {
            return NULL;
        }
    }
    return (am_xmodem_rec_handle_t)(p_dev);
}

/********************************************************************************
Xmodem发送驱动
********************************************************************************/

/**
 * \brief 获取文件发送结束字符
 */
am_local void __xmodem_tx_eot (am_xmodem_tx_dev_t *p_dev,
                               uint8_t            *p_outchar)
{
     p_dev->tx_state = __AM_XMODEM_FLAG_TX;
    *p_outchar = __AM_XMODEM_EOT;
     p_dev->state_flag  = __AM_XMODEM_DOC_EOT;
}

/******************************************************************************/
/**
 * \brief 文件发送结束函数
 */
am_err_t am_xmodem_tx_over (am_xmodem_tx_handle_t handle)
{
    if (handle == NULL) {
       return -AM_EINVAL;
    }
    /* 开启获取字符发送标志位*/
    handle->tx_state = __AM_XMODEM_FLAG_TX;
    /* 开启定时器*/
    am_softimer_start(&handle->tx_softimer,
                       handle->p_tx_devinfo->tx_timeout);
    /* 切换到文件发送结束状态*/
    handle->p_tx_func = (pfn_xmodem_tx_t)__xmodem_tx_eot;
    /* 开启发送中断 */
    am_uart_tx_startup(handle->uart_handle);
    /* 开启计时器*/
    am_softimer_start(&handle->tx_softimer,
                       handle->p_tx_devinfo->tx_timeout);
    return AM_OK;
}

/******************************************************************************/
/**
 * \brief 获取取消发送函数字符
 */
am_local am_bool_t __xmodem_tx_can_get (am_xmodem_tx_handle_t handle,
                                        char                 *p_outchar)
{
    if (handle->tx_bytes < 3) {
        *p_outchar = AM_XMODEM_CAN;
         handle->tx_bytes++;
         return AM_TRUE;
    }
    handle->tx_bytes = 0;
    handle->tx_state = __AM_XMODEM_STOP_TX;
    return AM_FALSE;
}

/******************************************************************************/
/**
 * \brief 用户取消发送函数
 */
am_err_t am_xmodem_tx_can_set (am_xmodem_tx_handle_t handle)
{
    if (handle == NULL) {
        return -AM_EINVAL;
    }
    handle->tx_bytes = 0;
    handle->tx_state = __AM_XMODEM_DATA_TX;
    /* 切换到紧急取消发送文件状态，并连续发送三次紧急取消信号*/
    handle->p_tx_func = (pfn_xmodem_tx_t)__xmodem_tx_can_get;
    am_uart_tx_startup(handle->uart_handle);
    handle->state_flag = __AM_XMODEM_EOT_ACK;

    return AM_OK;
}

/******************************************************************************/
/**
 * \brief 字符获取函数
 */
am_local int __xmodem_tx_getchar (am_xmodem_tx_dev_t  *p_dev, char *p_data)
{
    pfn_xmodem_tx_t pfn_tx_func = (pfn_xmodem_tx_t)p_dev->p_tx_func;
    if (p_dev->tx_state != __AM_XMODEM_STOP_TX) {
        pfn_tx_func(p_dev, p_data);
        if (p_dev->tx_state == __AM_XMODEM_FLAG_TX) {
            p_dev->tx_state = __AM_XMODEM_STOP_TX;
        }
        return AM_TRUE;
    }
    return AM_OK;
}

/******************************************************************************/
/**
 * \brief Xmodem发送模式获取一个字符发送函数
 */
am_local int __xmodem_tx_char_get (void *p_arg, char *p_outchar)
{
    am_xmodem_tx_dev_t *p_dev = (am_xmodem_tx_dev_t *)p_arg;
    if (__xmodem_tx_getchar(p_dev, p_outchar) != AM_TRUE) {
        return -AM_EEMPTY;
    }
    return AM_OK;
}

/******************************************************************************/
/**
 * \brief xmodem发送回调函数注册
 */
am_err_t am_xmodem_tx_cb_reg (am_xmodem_tx_handle_t handle,
                              am_xmodem_user_tx_t   pfn_tx_cb,
                              void                 *p_arg)
{
    if (pfn_tx_cb == NULL || pfn_tx_cb == NULL) {
       return -AM_EINVAL;
    }

    handle->p_arg           = p_arg;
    handle->pfn_tx_callback = pfn_tx_cb;

    return AM_OK;
}

/**
 * \brief 生成CRC校验码函数
 */
am_local uint16_t __xmodem_tx_crc_set (am_xmodem_tx_dev_t *p_dev,
                                       char               *ptr,
                                       uint32_t            count)
{
    uint8_t  i           = 0;
    uint8_t  data        = 0x1A;
    uint32_t crc         = 0;
    uint32_t ctrlz_count = 0;

    //标准CRC计算
    am_crc_cal(p_dev->crc_handle,
               (uint8_t *)ptr,
               p_dev->doc_bytes);

    if (p_dev->doc_bytes < p_dev->frame_tx_bytes) {
        ctrlz_count = p_dev->frame_tx_bytes - p_dev->doc_bytes;
        for (i = 0; i < ctrlz_count; i++) {
            am_crc_cal(p_dev->crc_handle,
                       &data,
                       1);
        }
    }

    am_crc_final(p_dev->crc_handle, &crc);

    return (uint16_t)(crc);
}

/*******************************************************************************
状态机函数
*******************************************************************************/
/**
 * \brief 发送校验码函数
 */
am_local void __xmodem_tx_frames_parity (am_xmodem_tx_dev_t *p_dev,
                                         char               *p_outchar)
{
    uint16_t crc_high = 0;
    /* 若工作模式为1K则获取CRC校验码*/
    if (p_dev->frame_tx_bytes == 1024) {
        crc_high = __xmodem_tx_crc_set(p_dev,
                                       (char *)p_dev->p_tx_buf,
                                        p_dev->doc_bytes);
        if (p_dev->fra_crc_pry == crc_high >> 8) {
            p_dev->fra_crc_pry = crc_high;
            p_dev->tx_state = __AM_XMODEM_STOP_TX;
        } else {
            p_dev->fra_crc_pry = crc_high >> 8;
        }
       *p_outchar = p_dev->fra_crc_pry;
    }
    /* 若工作模式为128则获取SUM校验码*/
    if (p_dev->frame_tx_bytes == 128) {
        int i;
        char cks = 0;
        /* 将数据累加求和*/
        for (i = 0; i < p_dev->doc_bytes; i++)
        {
            cks += p_dev->p_tx_buf[i];
        }
        if (p_dev->ctrlz_num != 0) {
            for (i = 0; i < p_dev->ctrlz_num; i++) {
                cks += __AM_XMODEM_CTRLZ;
            }
            p_dev->ctrlz_num = 0;
        }
        p_dev->fra_sum_parity = cks;
       *p_outchar = p_dev->fra_sum_parity;
        p_dev->tx_state = __AM_XMODEM_STOP_TX;
    }
}

/******************************************************************************/
/**
 * \brief 数据不足一帧填充函数
 */
am_local am_bool_t __xmodem_tx_ctrlz_set (am_xmodem_tx_dev_t *p_dev,
                                          char               *p_outchar)
{
   *p_outchar = __AM_XMODEM_CTRLZ;
    p_dev->tx_bytes++;
    if (p_dev->tx_bytes == p_dev->frame_tx_bytes) {
        p_dev->tx_bytes   = 0;
        p_dev->state_flag = AM_XMODEM_MOU_SUC;
        p_dev->p_tx_func  = (pfn_xmodem_tx_t)__xmodem_tx_frames_parity;
        return AM_TRUE;
     }
    return AM_FALSE;
}

/******************************************************************************/
/**
 * \brief 发送数据段函数
 */
am_local am_bool_t __xmodem_tx_frames_data (am_xmodem_tx_dev_t *p_dev,
                                            char               *p_outchar)
{
    if (p_dev->doc_bytes == p_dev->frame_tx_bytes) {
       *p_outchar = p_dev->p_tx_buf[p_dev->tx_bytes];
        p_dev->tx_bytes++;
        if (p_dev->tx_bytes == p_dev->frame_tx_bytes) {
            p_dev->tx_bytes  = 0;
            p_dev->p_tx_func = (pfn_xmodem_tx_t)__xmodem_tx_frames_parity;
        }
        return AM_TRUE;
    }
    if (p_dev->doc_bytes < p_dev->frame_tx_bytes ) {
        if (p_dev->tx_bytes < p_dev->doc_bytes) {
           *p_outchar = p_dev->p_tx_buf[p_dev->tx_bytes];
            p_dev->tx_bytes++;
            if (p_dev->tx_bytes == p_dev->doc_bytes) {
                p_dev->p_tx_func  = (pfn_xmodem_tx_t)__xmodem_tx_ctrlz_set;
            }
            return AM_TRUE;
        }
    }
    return AM_FALSE;
}

/******************************************************************************/
/**
 * \brief 发送序列号反码函数
 */
am_local am_bool_t __xmodem_tx_frames_pack_rmoc (am_xmodem_tx_dev_t *p_dev,
                                                 char               *p_outchar)
{
    /* 中断发送函数获取序列号反码*/
   *p_outchar = ~p_dev->frames_num;

    /* 切换到数据发送状态*/
    p_dev->p_tx_func = (pfn_xmodem_tx_t)__xmodem_tx_frames_data;

    return AM_TRUE;
}

/******************************************************************************/
/**
 * \brief 发送序列号函数
 */
am_local am_bool_t  __xomdem_tx_frames_packetno (am_xmodem_tx_dev_t *p_dev,
                                                 char               *p_outchar)
{
    /* 序列号加1*/
    p_dev->frames_num++;

    /* 中断发送函数获取序列号*/
   *p_outchar = p_dev->frames_num;

    /* 切换到获取序列号反码状态*/
    p_dev->p_tx_func = (pfn_xmodem_tx_t)__xmodem_tx_frames_pack_rmoc;

    return AM_TRUE;
}

/******************************************************************************/
/**
 * \brief 发送帧头函数
 */
am_local am_bool_t __xmodem_tx_frames_head (am_xmodem_tx_dev_t *p_dev,
                                            char               *p_outchar)
{
    if (p_dev->frame_tx_bytes == 1024) {
       /* 1K工作模式的帧头为STX*/
       *p_outchar = __AM_XMODEM_STX;
    }
    if (p_dev->frame_tx_bytes == 128) {
        /* 128工作模式的帧头为SOH*/
       *p_outchar = __AM_XMODEM_SOH;
    }
    /* 切换到序列号发送状态*/
    p_dev->p_tx_func = (pfn_xmodem_tx_t)__xomdem_tx_frames_packetno;

    return AM_TRUE;
}

/******************************************************************************/
/**
 * \brief 开始发送函数
 */
am_err_t am_xmodem_tx_pack (am_xmodem_tx_handle_t handle,
                            char                 *p_doc,
                            uint32_t              pack_size)
{
    if (handle == NULL || p_doc == NULL) {
        return -AM_EINVAL;
    }
    if (handle->state_flag != __AM_XMODEM_TX_START) {
        handle->state_flag = __AM_XMODEM_TX_START;
    }
    if (pack_size < handle->frame_tx_bytes) {
        handle->ctrlz_num = handle->frame_tx_bytes - pack_size;
    }
    /* 获取发送文件的指针*/
    handle->p_tx_buf  = p_doc;
    /* 一次发送的模块大小*/
    handle->doc_bytes = pack_size;
    /* 允许获取发送字符*/
    handle->tx_state  = __AM_XMODEM_DATA_TX;
    /* 切换到帧头发送状态*/
    handle->p_tx_func = (pfn_xmodem_tx_t)__xmodem_tx_frames_head;
    /* 打开发送中断*/
    am_uart_tx_startup(handle->uart_handle);
    /* 开启计时器*/
    am_softimer_start(&handle->tx_softimer,
                       handle->p_tx_devinfo->tx_timeout);
    return AM_OK;
}

/******************************************************************************/
am_local void __xmodem_tx_eot_ack_char (am_xmodem_tx_dev_t *p_dev,
                                        char               *p_outchar)
{
     *p_outchar = AM_XMODEM_ACK;
}

/******************************************************************************/
am_local int __xmodem_tx_eot_ack (am_xmodem_tx_dev_t *p_dev)
{
    if (p_dev == NULL) {
        return -AM_EINVAL;
    }
    p_dev->tx_state   = __AM_XMODEM_FLAG_TX;
    p_dev->state_flag = __AM_XMODEM_EOT_ACK;
    p_dev->p_tx_func  = (pfn_xmodem_tx_t)__xmodem_tx_eot_ack_char;
    am_uart_tx_startup(p_dev->uart_handle);
    return AM_TRUE;
}

/******************************************************************************/
/**
 * \brief 接收方要求重发函数
 */
am_local void __xmodem_tx_nak (am_xmodem_tx_dev_t *p_dev)
{
    if (p_dev->nake_state < p_dev->p_tx_devinfo->nak_times_max) {
        p_dev->tx_bytes   = 0;
        p_dev->tx_state   = __AM_XMODEM_FLAG_TX;
        p_dev->frames_num = p_dev->frames_num - 1;
        p_dev->p_tx_func  = (pfn_xmodem_tx_t)__xmodem_tx_frames_head;
        am_uart_tx_startup(p_dev->uart_handle);
    }
    if (p_dev->nake_state == p_dev->p_tx_devinfo->nak_times_max) {
        am_xmodem_tx_can_set(p_dev);
        p_dev->nake_state = 0;
        /* 通知用户当前数据包重发次数达到规定的最大重发次数, 并已经取消了发送*/
        p_dev->pfn_tx_callback(p_dev->p_arg, AM_XMODEM_NAK_TIME);
    }
    p_dev->nake_state++;
}

/******************************************************************************/
/**
 * \brief 应答信号判断函数
 */
am_local int __xmodem_tx_result_mode (am_xmodem_tx_dev_t *p_dev,
                                      char                inchar)
{
    if (p_dev->state_flag == __AM_XMODEM_TX_READY) {
        p_dev->state_flag = __AM_XMODEM_TX_START;
        if (inchar == AM_XMODEM_1k) {
            p_dev->frame_tx_bytes = 1024;
        }
        if (inchar == AM_XMODEM_NAK) {
            p_dev->frame_tx_bytes = 128;
        }
        if (p_dev->frame_tx_bytes == 128 || p_dev->frame_tx_bytes == 1024) {
           p_dev->tx_state = __AM_XMODEM_FLAG_TX;
           p_dev->pfn_tx_callback(p_dev->p_arg, inchar);
           return AM_TRUE;
        }
    }
    if (p_dev->state_flag == __AM_XMODEM_TX_START) {
        if (inchar == AM_XMODEM_ACK) {
            p_dev->pfn_tx_callback(p_dev->p_arg, inchar);
        }
        if (inchar == AM_XMODEM_NAK) {
            __xmodem_tx_nak(p_dev);
        }
        if (inchar == AM_XMODEM_CAN) {
           p_dev->pfn_tx_callback(p_dev->p_arg, inchar);
        }
        return AM_TRUE;
    }
    if (p_dev->state_flag == AM_XMODEM_MOU_SUC) {
        p_dev->pfn_tx_callback(p_dev->p_arg, AM_XMODEM_MOU_SUC);
    }
    return AM_FALSE;
}

/******************************************************************************/
/**
 * \brief 模块发送完或者接收方准备好发送通知用户
 */
am_local int __xmodem_tx_user_inform (am_xmodem_tx_dev_t *p_dev,
                                      char                inchar)
{
    /*当前状态为文件发送结束时，接收到的应答信号将自动回复*/
    if (p_dev->state_flag == __AM_XMODEM_DOC_EOT) {
        return __xmodem_tx_eot_ack(p_dev);
    }
    if (p_dev->tx_bytes == 0 ) {
        p_dev->tx_state = __AM_XMODEM_FLAG_TX;
        return __xmodem_tx_result_mode(p_dev, inchar);
    }
    return AM_FALSE;
}

/******************************************************************************/
/**
 * \brief 中断接收字符函数
 */
am_local void __xmodem_tx_char (void *p_arg, uint8_t inchar)
{
    am_xmodem_tx_dev_t *p_dev = (am_xmodem_tx_dev_t *)p_arg;

    am_softimer_stop(&p_dev->tx_softimer);

    if (AM_FALSE == __xmodem_tx_user_inform(p_dev, inchar)) {
        return;
    }
}

/******************************************************************************/
/**
 * \brief Xmodem发送超时函数
 */
am_local void __xmodem_tx_time_callback(void *p_arg)
{
    am_xmodem_tx_dev_t *p_dev = (am_xmodem_tx_dev_t *)p_arg;
    /* 关闭软件定时器 */
    am_softimer_stop(&p_dev->tx_softimer);
    if (p_dev->pfn_tx_callback != NULL) {
        am_xmodem_tx_can_set(p_dev);
        p_dev->pfn_tx_callback(p_dev->p_arg, -AM_ETIME);
     }
}

/******************************************************************************/
/**
 * \brief 发送设备初始化函数
 */
am_xmodem_tx_handle_t  am_xmodem_tx_init (
                           am_xmodem_tx_dev_t            *p_dev,
                           const am_xmodem_tx_dev_info_t *p_tx_devinfo,
                           am_uart_handle_t               uart_handle,
                           am_crc_handle_t                crc_handle)
{
    if ((NULL == p_dev) || (NULL == p_tx_devinfo) || (NULL == uart_handle)) {
        return NULL;
    }

    p_dev->ctrlz_num    = 0;    /**< \brief 初始化填充量*/
    p_dev->tx_bytes     = 0;    /**< \brief 初始化发送文件大小*/
    p_dev->p_tx_buf     = NULL; /**< \brief 初始化文件指针 */
    p_dev->state_flag   = 0;    /**< \brief 默认第一个接收到的字符为模式判断字符 */
    p_dev->nake_state   = 0;    /**< \brief 初始化重发状态为0*/
    p_dev->p_tx_func    = NULL; /**< \brief 初始化状态机函数指针 */
    p_dev->crc_handle   = crc_handle;   /**< \brief CRC句柄*/
    p_dev->uart_handle  = uart_handle;  /**< \brief 串口句柄*/
    p_dev->tx_state     = __AM_XMODEM_DATA_TX;
    p_dev->p_tx_devinfo = (am_xmodem_tx_dev_info_t *)p_tx_devinfo;

    /* 初始化CRC模型*/
    __crc_16_pattern_set(crc_handle, &p_dev->crc_pattern);

    /* 使能串口中断模式 */
    am_uart_ioctl(p_dev->uart_handle,
                  AM_UART_MODE_SET,
                  (void *)AM_UART_MODE_INT);

    am_uart_callback_set(p_dev->uart_handle,
                         AM_UART_CALLBACK_RXCHAR_PUT,
                         __xmodem_tx_char,
                         (void *)(p_dev));

    /* 注册发送回调函数 */
    am_uart_callback_set(p_dev->uart_handle,
                         AM_UART_CALLBACK_TXCHAR_GET,
                         __xmodem_tx_char_get,
                         (void *)(p_dev));

    if (p_tx_devinfo->tx_timeout != 0) {
        if (am_softimer_init(&p_dev->tx_softimer,
                             __xmodem_tx_time_callback,
                             (void *)p_dev) != AM_OK) {
            return NULL;
        }
    }
    return (am_xmodem_tx_handle_t)(p_dev);
}

