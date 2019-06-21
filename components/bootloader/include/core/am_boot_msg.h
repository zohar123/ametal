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
 * \brief  BootLoader消息标准接口
 *
 * \internal
 * \par Modification history
 * - 1.00 19-5-23  yrh, first implementation
 * \endinternal
 */
#ifndef __AM_BOOT_MSG_H
#define __AM_BOOT_MSG_H

#include "am_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \brief 双区用户程序有效 */
#define AM_BOOTLOADER_FLAG_APP     (0x12345678)
/** \brief 双区升级程序有效 */
#define AM_BOOTLOADER_FLAG_UPDATE  (0x87654321)
/** \brief 双区无代码 */
#define AM_BOOTLOADER_FLAG_NO      (0xFFFFFFFF)
/**
 * \brief bootloader 消息驱动函数结构体
 */
struct am_boot_msg_drv_funcs {
    /** \brief 消息处理 */
    int (*pfn_msg_dispose) (void *p_drv);

    /** \brief 设置消息操作 */
    int (*pfn_msg_set) (void *p_drv, void *par);
};

/**
 * \brief bootloader 消息标准服务结构体
 */
typedef struct am_boot_msg_serv {
    const struct am_boot_msg_drv_funcs *p_funcs;  /**< \brief 设备驱动函数结构体指针    */
    void                               *p_drv;    /**< \brief 设备驱动函数第一个参数 */
} am_boot_msg_serv_t;

/** \brief bootloader 消息标准服务操作句柄类型定义 */
typedef am_boot_msg_serv_t * am_boot_msg_handle_t;



/**
 * \brief 设置应用程序与BootLoader传递的信息
 *
 * \param[in] handle : 消息操作标准服务句柄
 * \param[in] par    : 设置消息的内容
 *
 * \retval AM_OK    : 成功
 * \retval AM_ERROR : 失败
 */
am_static_inline
int am_boot_msg_set(am_boot_msg_handle_t handle, void *par)
{
    if(handle && handle->p_funcs && handle->p_funcs->pfn_msg_set) {
           return handle->p_funcs->pfn_msg_set(handle->p_drv, par);
    }
    return -AM_EINVAL;
}

/**
 * \brief 对消息做相应的回应处理
 *
 * \param[in] handle : 消息操作标准服务句柄
 *
 * \retval AM_OK    : 处理成功
 * \retval AM_ERROR : 处理失败
 */
am_static_inline
int am_boot_msg_dispose(am_boot_msg_handle_t handle)
{
    if(handle && handle->p_funcs && handle->p_funcs->pfn_msg_dispose) {
           return handle->p_funcs->pfn_msg_dispose(handle->p_drv);
    }
    return -AM_EINVAL;
}
#ifdef __cplusplus
}
#endif

#endif /* __AM_BOOT_MSG_H */
