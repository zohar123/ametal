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
 * - 1.00 15-11-30  win, first implementation.
 * \endinternal
 */
#ifndef __AM_UARTCMD_CMD_MIFARE_H
#define __AM_UARTCMD_CMD_MIFARE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_uartcmd_cmd.h"
#include "am_uartcmd_cmd_define.h"

#include "am_uartcmd_ic_mifare.h"
#include "am_uartcmd_ic_iso14443.h"
#include "am_uartcmd_cpu_plus.h"
#include "am_reader_card.h"

typedef struct uartcmd_cmd_mifare_var {

    /** \brief 卡片类型 */
    uint8_t card_type;

    /** \brief CPU PLUS标准服务句柄 */
    am_ic_cpu_plus_handle_t cpu_plus_handle;

    /** \brief Mifare标准服务句柄 */
    am_ic_mifare_handle_t   mifare_handle;

    /** \brief ISO14443标准服务句柄 */
    am_ic_iso14443_handle_t iso14443_handle;

    /** \brief 读卡芯片服务句柄 */
    am_reader_card_handle_t reader_card_handle;

    /** \brief 用于自动检测 */
    uartcmd_auto_detest_info_t auto_detest;

    /** \brief 自动检测数据存放的缓冲区地址 */
    uint8_t  auto_buf[260];

    /** \brief 临时缓冲区 */
    uint8_t  auto_tmp_buf[260];

    /** \brief 临时缓冲区计数 */
    uint32_t auto_tmp_count;

    /** \brief 自动检测数据的长度，前提是有数据存在 */
    uint32_t auto_nbytes;

    /** \brief 自动检卡命令的部分参数 */
    struct am_uartcmd_frame auto_frame;

} uartcmd_cmd_mifare_var_t;

/**
 * \brief Mifare 类命令初始化
 *
 * \param[in] handle             : UARTCMD服务句柄
 * \param[in] p_cmd              : 设备控制类命令结构体指针
 * \param[in] p_var              : Mifare命令参数结构体指针
 * \param[in] mifare_handle      : Mifare服务句柄
 * \param[in] iso14443_handle    : ISO14443服务句柄
 * \param[in] cpu_plus_handle    : CPU PLUS卡服务句柄
 * \param[in] reader_card_handle : 读卡器服务句柄
 * \param[in] cmd_class          : 命令类型
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
int uartcmd_cmd_mifare_init (am_uartcmd_handle_t       handle,
                             uartcmd_cmd_t            *p_cmd,
                             uartcmd_cmd_mifare_var_t *p_var,
                             am_ic_mifare_handle_t     mifare_handle,
                             am_ic_iso14443_handle_t   iso14443_handle,
                             am_ic_cpu_plus_handle_t   cpu_plus_handle,
                             am_reader_card_handle_t   reader_card_handle,
                             uint8_t                   cmd_class);

/**
 * \brief Mifare 类命令去除初始化
 *
 * \param[in] handle : UARTCMD服务句柄
 * \param[in] p_cmd  : 设备控制类命令结构体指针
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
void uartcmd_cmd_mifare_deinit (am_uartcmd_handle_t handle, uartcmd_cmd_t *p_cmd);

/**
 * \brief 自动检测模式
 */
int uartcmd_cmd_mifare_auto_detest (am_uartcmd_handle_t handle);

#ifdef __cplusplus
}
#endif

#endif /* __UARTCMD_CMD_MIFARE_H */
