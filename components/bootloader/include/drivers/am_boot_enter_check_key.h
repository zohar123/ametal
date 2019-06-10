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
 * \brief 进入应用程序检测(按键实现)
 *
 * \internal
 * \par Modification history
 * - 1.00 19-5-23  yrh, first implementation
 * \endinternal
 */

#ifndef __AM_BOOT_ENTER_CHECK_KEY_H
#define __AM_BOOT_ENTER_CHECK_KEY_H

#include "am_common.h"
#include "am_boot_enter_check.h"

typedef struct am_boot_enter_check_key_dev {
    /**< \brief 标准的进入应用程序检测服务  */
    am_boot_enter_check_serv_t enter_check_serv;
    /**< \brief 按键对应的引脚  */
    int                        pin;
    /**< \brief 引脚配置  */
    uint32_t                   flags;
}am_boot_enter_check_key_dev_t;


am_boot_enter_check_handle_t am_boot_enter_check_key_init(int pin, uint32_t flags);

#endif /* __AM_BOOT_ENTER_CHECK_KEY_H */
