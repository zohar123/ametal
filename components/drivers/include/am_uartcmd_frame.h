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
 
#ifndef __AM_UARTCMD_FRAME_H
#define __AM_UARTCMD_FRAME_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_list.h"
#include "am_uartcmd.h"

/**
 * \brief 帧处理通用函数
 */
struct uartcmd_frame_funcs {
    
    /** \brief 帧检测函数 */
    int (*pfn_frame_check)(const void *p_buf, uint32_t nbytes);
    
    /** \brief 帧处理函数 */
    int (*pfn_frame_execute)(am_uartcmd_handle_t  handle,
                             void                *p_buf,
                             uint32_t             nbytes);
    
    /** \brief 获取帧中的数据信息的长度和帧长 */
    int (*pfn_frame_info_get)(const void *p_buf,
                              uint32_t   *p_frame_len,
                              uint32_t   *p_info_len);

};

/**
 * \brief 通用帧类型
 */
typedef struct uartcmd_frame {
    
    /** \brief 帧处理通用函数 */
    struct uartcmd_frame_funcs *p_funcs;
    
    /** \brief 帧类型 */
    uint8_t                frame_type;
    
    /** \brief 链表结点 */
    struct am_list_head    node;

} uartcmd_frame_t;

/**
 * \brief 注册一个新的帧类型
 * 
 * \param[in] handle  : UARTCMD服务句柄
 * \param[in] p_frame : 命令帧结构体指针
 * 
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
int uartcmd_frame_class_register (am_uartcmd_handle_t  handle,
                                  uartcmd_frame_t     *p_frame);

/**
 * \brief 注销一个新的帧类型
 *
 * \param[in] handle  : UARTCMD服务句柄
 * \param[in] p_frame : 命令帧结构体指针
 * 
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
int uartcmd_frame_class_unregister (am_uartcmd_handle_t  handle,
                                    uartcmd_frame_t     *p_frame);

/**
 * \brief 帧处理函数
 *
 * \param[in] handle : UARTCMD服务句柄
 * 
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
int uartcmd_frame_execute (am_uartcmd_handle_t handle);

/**
 * \brief 处理上一个帧的命令（用于自动检测模式）
 *
 * \param[in] handle : UARTCMD服务句柄
 * 
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
int uartcmd_frame_last_execute (am_uartcmd_handle_t handle);

#ifdef __cplusplus
}
#endif

#endif /* __UARTCMD_FRAME_H */

/* end of file */
