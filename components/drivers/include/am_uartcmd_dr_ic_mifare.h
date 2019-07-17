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
 * \brief Mifare卡标准服务驱动层
 *
 * \internal
 * \par Modification History
 * - 1.00 16-01-13  hetaigang, first implementation.
 * \endinternal
 */
#ifndef __AM_DR_IC_MIFARE_H
#define __AM_DR_IC_MIFARE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_uartcmd_ic_mifare.h"
#include "am_uartcmd_includes.h"

/**
 * \brief Mifare卡设备结构体
 */
typedef struct amdr_ic_mifare_dev {

    /** \brief Mifare卡标准服务结构体 */
    am_ic_mifare_serv_t     serv;

    /** \brief ISO14443标准服务句柄 */
    am_ic_iso14443_handle_t iso14443_handle;

    /** \brief 读卡芯片驱动结构体 */
    am_fm175xx_dev_t       *p_fm17550s_dev;
} amdr_ic_mifare_dev_t;

/**
 * \brief Mifare卡驱动初始化
 *
 * \param[in] p_dev           : Mifare卡设备结构体指针
 * \param[in] iso14443_handle : ISO14443标准服务句柄
 *
 * \return Mifare标准服务句柄
 */
am_ic_mifare_handle_t amdr_ic_mifare_init (amdr_ic_mifare_dev_t    *p_dev,
                                           am_ic_iso14443_handle_t  iso14443_handle,
                                           am_fm175xx_dev_t        *p_fm17550s_dev);

/**
 * \brief Mifare驱动解除初始化
 * \param[in] p_dev : Mifare卡设备结构体指针
 * \return 无
 */
void amdr_ic_mifare_deinit (amdr_ic_mifare_dev_t *p_dev);

#ifdef __cplusplus
}
#endif

#endif /* __AMDR_IC_MIFARE_H */

/* end of file */
