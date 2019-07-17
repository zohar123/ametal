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
 * - 1.00 15-11-12  win, first implementation.
 * \endinternal
 */
 
#ifndef __AM_UARTCMD_SERIAL_H
#define __AM_UARTCMD_SERIAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_uartcmd.h"

#define UARTCMD_SERIAL_SPEED_SET  1 /**< \brief 设置串行设备速率 */
#define UARTCMD_SERIAL_SPEED_GET  2 /**< \brief 获取串行设备速率 */
#define UARTCMD_SERIAL_ADDR_SET   3 /**< \brief 设置串行设备地址 */
#define UARTCMD_SERIAL_ADDR_GET   4 /**< \brief 获取串行设备地址 */

/** \brief 提前声明结构体 */
struct uartcmd_serial;

/**
 * \brief 串行设备通用函数
 */
struct uartcmd_serial_funcs {

    /** \brief 使能该设备 */
    int (*pfn_serial_enable)(struct uartcmd_serial *p_serial);

    /** \brief 禁能该设备 */
    int (*pfn_serial_disable)(struct uartcmd_serial *p_serial);

    /** \brief 启动发送 */
    int (*pfn_serial_tx_startup)(struct uartcmd_serial *p_serial);

    /** \brief 设备参数设置 */
    int (*pfn_serial_ioctl)(struct uartcmd_serial *p_serial,
                            int                    request,
                            void                  *p_arg);
                            
    /** \brief 串行设备执行命令帧状态 */
    int (*pfn_serial_status_set) (struct uartcmd_serial *p_serial,
                                  int                    status);

};

/**
 * \brief 串行设备结构体
 */
typedef struct uartcmd_serial {
    
    /** \brief 串行设备句柄 */
    void               *p_serial_handle;
    
    /** \brief 用于确定当前需要使用的是何种串行设备 */
    uint8_t             serial_id;   
    
    uint32_t            rx_count;    /**< \brief 接收计数 */
    uint32_t            tx_count;    /**< \brief 发送计数 */
    
    /** \brief 链表头节点 */
    struct am_list_head node;
    
    /** \brief UARTCMD 服务句柄 */
    am_uartcmd_handle_t uartcmd_handle;
    
    /** \brief 用于设备附加信息 */
    void               *p_arg;
    
    /** \brief 串行设备通用函数 */
    struct uartcmd_serial_funcs *p_funcs;
    
} uartcmd_serial_t;

/**
 * \brief 注册一个串行设备
 *
 * \param[in] handle   : UARTCMD服务句柄
 * \param[in] p_serial : 串行设备结构体指针
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
int uartcmd_serial_register (am_uartcmd_handle_t handle, uartcmd_serial_t *p_serial);

/**
 * \brief 注销一个串行设备
 *
 * \param[in] handle   : UARTCMD服务句柄
 * \param[in] p_serial : 串行设备结构体指针
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
int uartcmd_serial_unregister (am_uartcmd_handle_t handle, uartcmd_serial_t *p_serial);

/**
 * \brief 启动串行设备的发送
 *        该函数由UARTCMD模块调用，用于UARTCMD发送回应帧
 *
 * \param[in] handle : UARTCMD服务句柄
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
int uartcmd_serial_tx_startup (am_uartcmd_handle_t handle);

/**
 * \brief 串行设备通用控制函数
 *
 * \param[in] handle  : UARTCMD服务句柄
 * \param[in] request : 控制指令，由具体设备定义
 *                      - UARTCMD_SERIAL_SPEED_SET 设置串行设备速率
 *                      - UARTCMD_SERIAL_SPEED_GET 获取串行设备速率
 *                      - UARTCMD_SERIAL_ADDR_SET  设置串行设备地址
 *                      - UARTCMD_SERIAL_ADDR_GET  获取串行设备地址
 * \param[in] p_arg   : 该指令对应的参数
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
int uartcmd_serial_ioctl (am_uartcmd_handle_t handle, int request, void *p_arg);

/**
 * \brief 串行设备执行命令帧状态设置
 *
 * \param[in] handle  : UARTCMD服务句柄
 * \param[in] stauts  : 状态值
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
int uartcmd_serial_status_set (am_uartcmd_handle_t handle, int status);

#ifdef __cplusplus
}
#endif

#endif /* __AM_UARTCMD_SERIAL_H */

/* end of file */
