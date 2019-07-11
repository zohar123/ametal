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
 * \brief 自动波特率服务，可以实现与未知波特率的串口设备自动匹配波特率
 *
 *
 * \internal
 * \par Modification history
 * - 1.00 18-10-25  ipk, first implementation
 * \endinternal
 */

#ifndef __AM_AUTO_BAUDRATE_H__
#define __AM_AUTO_BAUDRATE_H__

/**
 * \brief bootloader 自动波特率检测 驱动函数结构体
 */
struct am_boot_autobaud_drv_funcs {

    /** \brief 获取波特率*/
    int (*pfn_get_baudrate) (void *p_drv, uint32_t *p_baud);

};

/**
 * \brief bootloader memory 标准服务结构体
 */
typedef struct am_boot_autobaud_serv {
    const struct am_boot_autobaud_drv_funcs *p_funcs;  /**< \brief 设备驱动函数结构体指针    */
    void                                    *p_drv;    /**< \brief 设备驱动函数第一个参数 */
} am_boot_autobaud_serv_t;

/** \brief bootloader 自动波特率 标准服务操作句柄类型定义 */
typedef am_boot_autobaud_serv_t  *am_boot_autobaud_handle_t;


/**
 * \brief 获取接收数据的波特率函数
 *
 * \param[in]  handle     : 自动波特率服务句柄
 * \param[out] p_baudrate : 指向获取的波特率地址
 *
 * \retval AM_OK    : 获取成功
 *         AM_ERROR : 获取失败
 */
am_static_inline
int am_boot_baudrate_get (am_boot_autobaud_handle_t handle,
                          uint32_t                 *p_baudrate)
{
    if(handle && handle->p_funcs->pfn_get_baudrate) {
        return handle->p_funcs->pfn_get_baudrate(handle->p_drv, p_baudrate);
    }
    return -AM_EINVAL;
}
#endif /* __AM_AUTO_BAUDRATE_H__ */
/* end of file */
