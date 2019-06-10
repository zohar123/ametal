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
 * \brief bootloader 固件接收的接口
 *
 * \internal
 * \par Modification history
 * - 1.00 19-5-23  yrh, first implementation
 * \endinternal
 */

#ifndef __AM_BOOT_FIRMWARE_RECV_H
#define __AM_BOOT_FIRMWARE_RECV_H

#include "am_common.h"

/**
 * \brief bootloader 固件接收驱动函数结构体
 */
struct am_boot_firwa_recv_drv_funcs {

    /** \brief 固件接收操作 */
    int (*pfn_firwa_recv) (void *p_drv);
};

/**
 * \brief bootloader 固件接收标准服务结构体
 */
typedef struct am_boot_firwa_recv_serv {
    /**< \brief 设备驱动函数结构体指针 */
    const struct am_boot_firwa_recv_drv_funcs *p_funcs;

    /**< \brief 设备驱动函数第一个参数 */
    void                                     *p_drv;
} am_boot_firwa_recv_serv_t;

/** \brief bootloader 固件接收标准服务操作句柄类型定义 */
typedef am_boot_firwa_recv_serv_t  *am_boot_firwa_recv_handle_t;

/**
 * \brief 固件接收
 *
 * \retval AM_OK    接收成功
 * \retval AM_ERROR 接收失败
 */
am_static_inline
int am_boot_firmware_recv(am_boot_firwa_recv_handle_t handle)
{
    if(handle && handle->p_funcs && handle->p_funcs->pfn_firwa_recv) {
        return handle->p_funcs->pfn_firwa_recv(handle->p_drv);
    }
    return -AM_EINVAL;
}
/* end of file */

#endif /* __AM_BOOT_FIRMWARE_RECV_H */

