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
 * \brief  Xmodem协议 应用接口文件
 *
 * \internal
 * \par Modification History
 * - 1.00 18-8-31 , xgg, first implementation.
 * \endinternal
 */
#ifndef __AM_XMODEM_H
#define __AM_XMODEM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_crc.h"
#include "am_uart.h"
#include "am_errno.h"
#include "am_softimer.h"

/**
 * @addtogroup am_if_am_xmodem
 * @copydoc am_xmodem.h
 * @{
 */

/**
 * \name Xmodem 工作模式
 *
 * @{
 */
#define AM_XMODEM_1K_MODE     0    /**< \brief 数据段是1K字节的工作模式*/
#define AM_XMODEM_128_MODE    1    /**< \brief 数据段是128字节的工作模式*/

/** @} */

/**
 * \name Xmodem校验方式
 *
 * @{
 */
#define AM_XMODEM_CRC_MODE    0    /**< \brief 采用CRC校验 */
#define AM_XMODEM_SUM_MODE    1    /**< \brief 采用累积和校验 */

/** @} */

/**
 * \name Xmodem接受过程中产生的事件
 * 以下宏定义会在Xmodem接受回调函数中传入，用户需要根据传入的event事件决定
 * 下一步的操作。
 * @{
 */
#define AM_DATA_CAN          0x19  /**< \brief 发送方取消发送*/
#define AM_DATA_SUC          0x20  /**< \brief 文件接收成功 */
#define AM_XMODEM_DOWN       0x25  /**< \brief Xmodem传输未连接*/
#define AM_XMODEM_CHECK_ERR  0x26  /**< \brief 校验码出错*/
/** @} */

/**
 * \name Xmodem发送过程中产生的事件
 *
 * @{
 */
#define AM_XMODEM_ACK        0x06 /**< \brief 接收方的确认信号，表示一帧数据接收完毕 */
#define AM_XMODEM_NAK        0x15 /**< \brief Xmodem工作在128字节工作模式 */
#define AM_XMODEM_1k         'C'  /**< \brief Xmodem工作在1K字节工作模式*/
#define AM_XMODEM_CAN        0x18 /**< \brief 接收方紧急取消传输 */
#define AM_XMODEM_NAK_TIME   0x40 /**< \brief 已重发到最大重发次数 */
#define AM_XMODEM_MOU_SUC    0x41 /**< \brief 文件发送完成 */

/** @} */

/*******************************************************************************
  状态机函数指针类型定义
*******************************************************************************/
/** \brief 定义Xmodem接收状态机函数指针类型 */
typedef am_bool_t (*pfn_xmodem_rx_t)(void *p_arg, char inchar);

/** \brief 定义Xmodem发送状态机函数指针类型 */
typedef am_bool_t (*pfn_xmodem_tx_t)(void *p_arg, char *p_outchar);

/******************************************************************************/
/**
 * \breif 用户接收回调函数
 *
 * \param[in] p_arg    : 用户决定传入回调函数的参数，若用户需在回调函数中
 *                       使用Xmodem句柄，可将句柄传入此参数。
 * \param[in] p_frames : Xmodem接收到的一帧数据保存的地址，用户可直接
 *                       使用该地址获取接收到的一帧数据
 * \param[in] event    : 先判断event值得正负，若大于0则这一帧数据接收
 *                       成功，event保存的是接收到的字节数，若event
 *                       值为负，并且值为-AM_DATA_SUC则整个文件接收成
 *                       功，否则接受失败可根据event的值判断是以下那种错误。
 *                       -AM_ETIME            : 当前帧数据超时未接收到
 *                       -AM_EBADMSG          : 未超时但重发最大次数后数据仍然错误
 *                       -AM_DATA_CAN         : 发送方紧急取消发送导致传输结束
 *                       -AM_XMODEM_DOWN      : 未能连接上位机
 *                       -AM_XMODEM_CHECK_ERR : 校验码出错
 * \note 当底层驱动发生各种驱动本身无法独自处理的事件时，驱动会通过此函数来通知
 *       用户，由用户决定如何处理对应的事件，这里有以下几种情况用户需要特别注意:
 * 1、event > 0 时表明当前驱动已经收到了一帧数据，帧数据的大小保存在event中,并且
 *   用户获取到此事件时,如果需要继续获取下一帧数据则应该调用am_xmodem_rec_ack_set
 *   函数。
 * 2、event == -AM_XMODEM_DOWN表明终端未能建立Xmodem连接，若用户需要重新建立，收
 *   到当前事件时可以调用am_xmodem_rec_start函数重新建立连接。或者无操作直接退出。
 * 3、event == -AM_ETIME 表明终端未能在规定规定时间能接收到帧数据，建议用户可以
 *    无操作退出。
 * 4、event == -AM_DATA_CAN 表明发送方紧急取消了传输，用户可以重新启动，或者直接
 *    退出。
 * 5、event == -AM_EBADMSG 表明当前数据帧有错误，已经重发最大次数后，数据任然错
 *    误，用户可以考虑上位机发送的数据是否有问题。
 * 6、event == -AM_XMODEM_CHECK_ERR 校验码出错，用户需检查重新进行校验。
 *
 * \return 无
 */
typedef void (*am_xmodem_user_rx_t) (void    *p_arg,
                                     void    *frames,
                                     int      event);

/** \brief xmodem协议接收设备信息结构体 */
typedef struct am_xmodem_rec_dev_info {
    char      *frames_info;     /**< \brief 保存Xmodem一帧信息的数组指针 */
    uint8_t    nak_max_times;   /**< \brief 重发最大次数 */
    uint32_t   frames_bytes;    /**< \brief 一帧数据的字节数 */
    uint32_t   data_mode;       /**< \brief 工作模式  */
    uint32_t   parity_mode;     /**< \brief 校验模式 */
    uint32_t   rx_timeout;      /**< \brief 接收超时时间 */
    /** \brief 启动间隔时间
     * 用户在调用am_xmodem_rec_start接口启动接受后，会每隔
     * start_time的时间发送启动字符，直到xmodem建立连接，如果
     * 在重发了nak_max_times后，仍然不能建立连接，则通过回调函数
     * 发送event = -AM_XMODEM_DOWN，通知用户Xmodem未能
     * 连接。
     */
    uint32_t   start_time;
} am_xmodem_rec_dev_info_t;

/**
 * \brief xmodem接收设备结构体
 */
typedef struct am_xmodem_rec_dev {
    int                  rx_bytes;       /**< \brief 当前接收到的字节数 */
    char                 fra_sum_parity; /**< \brief 保存累积和校验位 */
    uint16_t             fra_crc_pry;    /**< \brief 保存CRC校验码 */
    volatile uint8_t     char_get_state; /**< \brief 字符获取状态标志位 */
    volatile uint8_t     nak_state;      /**< \brief 当前重发状态 */
    volatile uint8_t     frames_num;     /**< \brief 当前这一帧数据的序列号 */
    volatile uint8_t     con_state;      /**< \brief Xmodem通信连接状态*/
    am_softimer_t        rx_softimer;    /**< \brief 接收超时定时器 */
    am_uart_handle_t     uart_handle;    /**< \brief 串口句柄 */
    am_xmodem_user_rx_t  pfn_rx_callback;/**< \brief 注册用户接受回调函数 */
    am_crc_handle_t      crc_handle;     /**< \brief 标准CRC句柄*/
    am_crc_pattern_t     crc_pattern;    /**< \brief 定义 CRC 模型 */
    void                *p_arg;          /**< \brief 用户回调函数类型 */
    volatile pfn_xmodem_rx_t  p_rec_func;/**< \brief 接收状态机函数句柄*/

    /* 接收设备信息结构体指针*/
    const am_xmodem_rec_dev_info_t *p_rec_devinfo;
} am_xmodem_rec_dev_t;

/** \brief xmodem接受标准服务操作句柄类型定义 */
typedef am_xmodem_rec_dev_t *am_xmodem_rec_handle_t;

/******************************************************************************/

/**
 * \brief 用户发送回调函数
 *
 * \param[in] p_arg : 用户需要传入的参数,由自己定义
 * \param[in] event : 回调传回的事件，用户可根据事件来自主进行下一步操作
 * 1、event == AM_XMODEM_NAK时,Xmodem工作在128字节工作模式；
 *   若户需要开始发送第一帧数据，可在回调中直接调用am_xmodem_tx_pack接口
 *   或者设立标志，在主循环中调用此接口，如果不发送任何数据，可以无操作。
 * 2、event == AM_XMODEM_1K时,Xmodem工作在1K字节工作模式；
 *   同上若用户需要开始发送第一帧数据，可调用am_xmodem_tx_pack接口或无操作
 * 3、event == AM_XMODEM_NAK_TIME时,重发次数达到规定值(内部已取消传输);
 *   此事件表明数据已发送到接收方，但接收方要求重发，此时驱动包内部已经按要求重
 *   发到了最大次数并且取消了发送，可考虑接收方是否出现异常。
 * 4、event == AM_XMODEM_MOU_SUC时,文件传输完毕;
 *   当前驱动已经将传入的文件发送完毕，Xmodem传输已经断开。用户可以选择无操作。
 * 5、event == AM_XMODEM_CAN时,表明接收方取消了传输;
 *   此事件表明接收方断开了Xmodem连接，用户可以考虑无操作直接退出
 * 6、event == AM_XMODEM_ACK时，一帧数据发送完毕;
 *   此事件表明当前这一帧数据已经发送完毕，用户可以调用am_xmodem_tx_pack函数继续
 *   发送下一帧
 * 7、event == -AM_ETIME时,接收方接收到一帧数据后未在规定的时间内回应;
 *
 * \return 无
 */
typedef void (*am_xmodem_user_tx_t) (void *p_arg, int event);


/** \brief Xmodem协议发送设备信息结构体*/
typedef struct am_xmodem_tx_dev_info {
    uint32_t   tx_timeout;       /**< \brief 发送超时时间*/
    uint8_t    nak_times_max;    /**< \brief 允许最大的重发次数*/
} am_xmodem_tx_dev_info_t;

/**
 * \brief Xmodem发送设备
 */
typedef struct am_xmodem_tx_dev {
    char                           fra_sum_parity; /**< \brief 保存累积和校验位 */
    char                          *p_tx_buf;       /**< \brief 文件指针 */
    uint8_t                        nake_state;     /**< \brief 当前重发状态 */
    uint8_t                        tx_state;       /**< \brief 控制获取字符发送状态*/
    int16_t                        fra_crc_pry;    /**< \brief 保存CRC校验码 */
    uint32_t                       frame_tx_bytes; /**< \brief 发送的一帧数据大小 */
    uint32_t                       doc_bytes;      /**< \brief 文件大小 */
    uint32_t                       ctrlz_num;      /**< \brief 填充的字节数*/
    volatile uint8_t               state_flag;     /**< \brief 用于分辨接收模式字符*/
    volatile uint32_t              tx_bytes;       /**< \brief 当前一帧已发送多少 */
    volatile uint32_t              frames_num;     /**< \brief 当前这一帧数据的序列号 */
    pfn_xmodem_tx_t                p_tx_func;      /**< \brief 状态机函数句柄*/
    am_softimer_t                  tx_softimer;    /**< \brief 接收超时定时器 */
    void                          *p_arg;          /**< \brief 用户回调函数类型 */
    am_xmodem_user_tx_t            pfn_tx_callback;/**< \brief 注册用户发送回调函数 */
    am_crc_handle_t                crc_handle;     /**< \brief CRC句柄*/
    am_crc_pattern_t               crc_pattern;    /**< \brief CRC模型*/
    am_uart_handle_t               uart_handle;    /**< \brief 串口句柄 */
    const am_xmodem_tx_dev_info_t *p_tx_devinfo;   /**< \brief 发送设备信息结构体指针*/
}am_xmodem_tx_dev_t;

/** \brief xmodem接受标准服务操作句柄类型定义 */
typedef am_xmodem_tx_dev_t *am_xmodem_tx_handle_t;

/******************************************************************************
 Xmodem 接受驱动接口
*******************************************************************************/
/**
 * \brief 设备初始化函数
 *
 * \param[in] p_dev       : xmodem设备
 * \param[in] p_decinfo   : 设备信息结构体
 * \param[in] uart_handle : 串口句柄
 * \param[in] crc_handle  : CRC句柄
 *
 * \return 不为NULL，则返回值为Xmodem接收句柄；
 *         若为NULL，表明参数无效或定时器初始化失败
 */
am_xmodem_rec_handle_t  am_xmodem_rec_init (
                                 am_xmodem_rec_dev_t            *p_dev,
                                 const am_xmodem_rec_dev_info_t *p_devinfo,
                                 am_uart_handle_t                uart_handle,
                                 am_crc_handle_t                 crc_handle);

/**
 * \brief xmodem接收回调函数注册
 *
 * \param[in] handle     : xmodem设备句柄
 * \param[in] pfn_rec_cb : 用户回调函数
 * \param[in] p_arg      : 用户传入的参数
 *
 * \retval AM_OK      : 执行成功
 * \retval -AM_EINVAL : 参数无效
 */
am_err_t am_xmodem_rec_cb_reg(am_xmodem_rec_handle_t  handle,
                               am_xmodem_user_rx_t     pfn_rec_cb,
                               void                   *p_arg);

/**
 * \brief 开始接受函数(Xmodem启动接收函数)
 *
 * \param[in] handle : xmodem设备句柄
 *
 * \retval AM_OK      : 执行成功
 * \retval -AM_EINVAL : 参数无效
 */
am_err_t am_xmodem_rec_start (am_xmodem_rec_handle_t handle);

/**
 * \brief 继续接收函数
 *
 * \param[in] handle : xmodem设备句柄
 *
 * \retval AM_OK      : 执行成功
 * \retval -AM_EINVAL : 参数无效
 */
am_err_t am_xmodem_rec_ack_set (am_xmodem_rec_handle_t handle);

/**
 * \brief 取消传输函数
 *
 * \param[in] p_dev xmodem设备句柄
 *
 * \retval AM_OK      : 执行成功
 * \retval -AM_EINVAL : 参数无效
 */
am_err_t am_xmodem_rec_can_set (am_xmodem_rec_handle_t handle);

/******************************************************************************
 Xmodem 发送驱动接口
*******************************************************************************/
/**
 * \brief 发送设备初始化函数
 *
 * \param[in] p_dev         : Xmodem发送设备
 * \param[in] p_tra_devinfo : Xmodem发送设备信息结构体
 * \param[in] uart_handle   : 串口句柄
 * \param[in] crc_handle    : CRC句柄
 *
 * \return 不为NULL返回值为Xmodem发送句柄；为NULL表明参数无效或定时器初始化失败
 */
am_xmodem_tx_handle_t  am_xmodem_tx_init (
                              am_xmodem_tx_dev_t            *p_dev,
                              const am_xmodem_tx_dev_info_t *p_tra_devinfo,
                              am_uart_handle_t               uart_handle,
                              am_crc_handle_t                crc_handle);

/**
 * \brief 帧数据发送函数
 *
 * \param[in] handle    : Xmodem发送句柄
 * \param[in] p_doc     : 用户需要发送的数组的指针
 * \param[in] pack_size : 需要发送的数组长度，若回调函数传入的事件为AM_XMODEM_NAK,
 *                        则pack_size传入的值为128,若回调函数传入的事件为
 *                        AM_XMODEM_1K,则pack_size传入的值为1024，如果文件最后一
 *                        帧数据不满足128或者1024则pack_size的值用户可以自己定义
 *                        但必须为文件剩下的字节数。
 *
 * \retval AM_OK      : 执行成功
 * \retval -AM_EINVAL : 参数无效
 */
am_err_t am_xmodem_tx_pack (am_xmodem_tx_handle_t  handle,
                            char                  *p_doc,
                            uint32_t               pack_size);

/**
 * \brief xmodem发送回调函数注册
 *
 * \param[in] handle    : Xmodem发送句柄
 * \param[in] pfn_tx_cb : 用户回调函数指针
 * \param[in] p_arg     : 用户传入设备参数
 *
 * \retval AM_OK      : 执行成功
 * \retval -AM_EINVAL : 参数无效
 */
am_err_t am_xmodem_tx_cb_reg (am_xmodem_tx_handle_t handle,
                              am_xmodem_user_tx_t  pfn_tx_cb,
                              void                *p_arg);

/**
 * \brief 文件发送结束函数
 *
 * \param[in] handle : Xmodem发送句柄
 *
 * \retval AM_OK      : 执行成功
 * \retval -AM_EINVAL : 参数无效
 */
am_err_t am_xmodem_tx_over (am_xmodem_tx_handle_t handle);

/**
 * \brief 用户取消发送函数
 *
 * \param[in] handle : Xmodem发送句柄
 *
 * \retval AM_OK      : 执行成功
 * \retval -AM_EINVAL : 参数无效
 */
am_err_t am_xmodem_tx_can_set (am_xmodem_tx_handle_t handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_XMODEM_H */

/* end of file */
