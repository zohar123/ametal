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
 * \brief 进入应用程序检测标准接口
 *
 * \internal
 * \par Modification history
 * - 1.00 19-5-23  yrh, first implementation
 * \endinternal
 */

#ifndef __AM_BOOT_ENTER_CHECK_H
#define __AM_BOOT_ENTER_CHECK_H

#include "am_common.h"

/**
 * \brief bootloader 进入应用程序检测驱动函数结构体
 */
struct am_boot_enter_check_drv_funcs {

    /** \brief 应用程序进入检测 */
    am_bool_t (*pfn_startup_check) (void *p_drv);
};

/**
 * \brief 应用程序进入检测标准服务结构体
 */
typedef struct am_boot_enter_check_serv {
    /**< \brief 设备驱动函数结构体指针 */
    const struct am_boot_enter_check_drv_funcs *p_funcs;

    /**< \brief 设备驱动函数第一个参数 */
    void                                       *p_drv;
} am_boot_enter_check_serv_t;

/** \brief 进入应用程序检测标准服务操作句柄类型定义 */
typedef am_boot_enter_check_serv_t  *am_boot_enter_check_handle_t;

/**
 * \brief 进入应用程序检测
 *
 * \retval AM_TRUE  进入应用代码
 * \retval AM_FALSE 进入BootLoader
 */
am_static_inline
am_bool_t am_boot_enter_check(am_boot_enter_check_handle_t handle)
{
    if(handle && handle->p_funcs && handle->p_funcs->pfn_startup_check) {
        return handle->p_funcs->pfn_startup_check(handle->p_drv);
    }
    return AM_FALSE;
}

#endif /* __AM_BOOT_ENTER_CHECK_H */
