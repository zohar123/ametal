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
 * \brief bootloader memory 操作通用操作接口
 *
 *   该类主要描述了BootLoader关于内存操作的，属于抽象类，根据不同的驱动方式，由子类实现对其操作，
 *   列如，使用flash驱动的内存操作，使用ram驱动内存操作。
 *
 *
 * \par 使用示例
 * \code
 *
 *
 * \endcode
 *
 * \internal
 * \par modification history:
 * - 1.00 18-11-30  yrh, first implementation.
 * \endinternal
 */


#ifndef __AM_BOOT_MEMORY_H
#define __AM_BOOT_MEMORY_H

#include "am_common.h"

#ifdef __cplusplus
extern "C" {
#endif



/**
 * \brief bootloader memory 驱动函数结构体
 */
struct am_boot_mem_drv_funcs {

    /** \brief 内存读操作 */
    int (*pfn_read) (void *p_drv, uint32_t address,  uint8_t *p_buf, uint32_t length);

    /** \brief 内存写操作 */
    int (*pfn_write) (void *p_drv, uint32_t address, uint8_t *p_buf, uint32_t length);
};

/**
 * \brief bootloader memory 标准服务结构体
 */
typedef struct am_boot_mem_serv {
    const struct am_boot_mem_drv_funcs *p_funcs;  /**< \brief 设备驱动函数结构体指针    */
    void                               *p_drv;    /**< \brief 设备驱动函数第一个参数 */
} am_boot_mem_serv_t;

/** \brief bootloader memory 标准服务操作句柄类型定义 */
typedef am_boot_mem_serv_t  *am_boot_mem_handle_t;

/**
 * \brief 内存读
 *
 * \param[in] handle  : 内存操作标准服务句柄
 * \param[in] address : 读数据的地址
 * \param[in] p_buf   : 存放读出的数据
 * \param[in] length  : 读数据的长度
 *
 * \retval AM_OK    : 读取成功
 * \retval AM_ERROR : 读取失败
 */
am_static_inline
int am_boot_mem_read(am_boot_mem_handle_t handle,
                     uint32_t             address,
                     uint8_t             *p_buf,
                     uint32_t             length)
{
    if(handle && handle->p_funcs && handle->p_funcs->pfn_read) {
        return handle->p_funcs->pfn_read(handle->p_drv, address, p_buf, length);
    }
    return -AM_EINVAL;
}

/**
 * \brief 内存写
 *
 * \param[in] handle  : 内存操作标准服务句柄
 * \param[in] address : 内存写的起始地址
 * \param[in] p_buf   : 写入数据存放的位置
 * \param[in] length  : 写入数据的长度
 *
 *
 * \retval AM_OK    : 写入成功
 * \retval AM_ERROR : 写入失败
 */
am_static_inline
int am_boot_mem_write(am_boot_mem_handle_t handle,
                      uint32_t             address,
                      uint8_t             *p_buf,
                      uint32_t             length)
{
    if(handle && handle->p_funcs && handle->p_funcs->pfn_write) {
           return handle->p_funcs->pfn_write(handle->p_drv, address, p_buf, length);
    }
    return -AM_EINVAL;
}


#ifdef __cplusplus
}
#endif

#endif /* __AM_BOOT_MEMORY_H */
