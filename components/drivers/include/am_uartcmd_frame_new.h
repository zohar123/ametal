/*******************************************************************************
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
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

#ifndef __AM_UARTCMD_FRAME_NEW_H
#define __AM_UARTCMD_FRAME_NEW_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_uartcmd_frame.h"

/**
 * \brief 新帧初始化函数
 *
 * \param[in] handle  : UARTCMD服务句柄
 * \param[in] p_frame : 通用帧类型结构体
 * \param[in] type    : 帧的类型
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
int uartcmd_frame_new_init (am_uartcmd_handle_t  handle,
                            uartcmd_frame_t     *p_frame,
                            uint8_t              type);

/**
 * \brief 新帧去初始化函数
 *
 * \param[in] handle  : UARTCMD服务句柄
 * \param[in] p_frame : 通用帧类型结构体
 * \param[in] type    : 帧的类型
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
int uartcmd_frame_new_deinit (am_uartcmd_handle_t  handle,
                              uartcmd_frame_t     *p_frame);

/**
 * \brief 新帧校验和计算
 *
 * \param[in] p_buf : 存放帧的缓冲区
 * \param[in] len   : 从地址字节开始到信息的最后字节（不包括校验和字节）
 *
 * \return 校验和
 */
uint32_t uartcmd_frame_new_sum_cacl (const uint8_t *p_buf, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif /* __AM_UARTCMD_FRAME_OLD_H */

/* end of file */
