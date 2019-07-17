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
 
#ifndef __AM_UARTCMD_CMD_H
#define __AM_UARTCMD_CMD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_list.h"
#include "am_uartcmd.h"

struct uartcmd_cmd_info;

/**
 * \brief 命令类型结构体 
 */
typedef struct uartcmd_cmd {
    
    /** \brief 命令类型 */
    uint8_t cmd_class;
    
    /** \brief 命令执行函数 */
    int (*pfn_cmd_execute)(am_uartcmd_handle_t      handle,
                           struct uartcmd_cmd_info *p_cmd_info);
    
    /** \brief ISO协议标准句柄 */
    void     *p_iso_handle; 
    
    /** \brief 链表结点 */
    struct am_list_head node;
    
    /** \brief 指向只支持单一命令的句柄 */
    void     *p_other_handle;

    /** \brief 用于扩展配置信息 */
    void     *p_arg;

} uartcmd_cmd_t;

/**
 * \brief 命令信息结构体
 */
typedef struct uartcmd_cmd_info {
    uartcmd_cmd_t *p_cmd;       /**< \brief 与命令相关联的命令类型 */
    uint32_t       cmd_code;    /**< \brief 命令代码 */
    uint8_t        slot_index;  /**< \brief 卡座索引号 */
    uint8_t       *p_rx_buf;    /**< \brief 存放命令帧数据信息的缓冲区 */
    uint32_t       rx_nbytes;   /**< \brief 数据信息的字节数 */
    uint8_t       *p_tx_buf;    /**< \brief 存放回应帧数据信息的缓冲区 */
    uint32_t      *p_tx_nbytes; /**< \brief 数据信息的字节数 */
} uartcmd_cmd_info_t;

/**
 * \brief 注册一个新的命令类型
 *
 * \param[in] handle : UARTCMD服务句柄
 * \param[in] p_cmd  : 命令类型结构体指针
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
int uartcmd_cmd_class_register (am_uartcmd_handle_t handle, uartcmd_cmd_t *p_cmd);

/**
 * \brief 注销一个新的命令类型
 *
 * \param[in] handle : UARTCMD服务句柄
 * \param[in] p_cmd  : 命令类型结构体指针
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
int uartcmd_cmd_class_unregister (am_uartcmd_handle_t handle, uartcmd_cmd_t *p_cmd);

/**
 * \brief 命令执行函数
 *
 * \param[in] handle      : UARTCMD服务句柄
 * \param[in] cmd_class   : 命令类别
 * \param[in] cmd_code    : 命令代码
 * \param[in] slot_index  : 卡槽索引 
 * \param[in] p_rx_buf    : 接收数据缓存区
 * \param[in] rx_nbytes   : 接收数据的字节数
 * \param[in] p_tx_buf    : 存放返回信息的缓冲区
 * \param[in] p_tx_nbytes : 返回信息的字节数
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数信息
 */
int uartcmd_cmd_execute (am_uartcmd_handle_t  handle,
                         uint32_t             cmd_class,
                         uint32_t             cmd_code,
                         uint8_t              slot_index,
                         uint8_t             *p_rx_buf,
                         uint32_t             rx_nbytes,
                         uint8_t             *p_tx_buf,
                         uint32_t            *p_tx_nbytes);

#ifdef __cplusplus
}
#endif

#endif /* __UARTCMD_CMD_H */

/* end of file */
