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
 * \brief UARTCMD ISO14443类命令处理函数
 *
 * \internal
 * \par Modification History
 * - 1.00 15-01-11  win, first implementation.
 * \endinternal
 */
#ifndef __AM_UARTCMD_CMD_ISO14443_H
#define __AM_UARTCMD_CMD_ISO14443_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_uartcmd_cmd.h"
#include "am_uartcmd_cmd_define.h"

#include "am_uartcmd_ic_iso14443.h"
#include "am_reader_card.h"

/**
 * \brief ISO14443类命令初始化函数
 *
 * \param[in] handle       : UARTCMD服务句柄
 * \param[in] p_cmd        : 设备控制类命令结构体指针
 * \param[in] p_iso_handle : ISO标准协议类型句柄
 * \param[in] cmd_class    : 命令类型
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
int uartcmd_cmd_iso14443_init (am_uartcmd_handle_t      handle,
                               uartcmd_cmd_t           *p_cmd,
                               am_ic_iso14443_handle_t  iso_handle,
                               am_reader_card_handle_t  reader_card_handle,
                               uint8_t                  cmd_class);

/**
 * \brief ISO14443类命令解除初始化函数
 *
 * \param[in] handle : UARTCMD服务句柄
 * \param[in] p_cmd  : 设备控制类命令结构体指针
 *
 * return 无
 */
void uartcmd_cmd_iso14443_deinit (am_uartcmd_handle_t      handle,
                                  uartcmd_cmd_t           *p_cmd);

#ifdef __cplusplus
}
#endif

#endif /* __UARTCMD_CMD_ISO14443_H */

/* end of file */
