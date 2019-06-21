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
 * \brief bootloader flash操作驱动，服务bootloader的flash标准接口
 *
 * \internal
 * \par Modification history
 * - 1.00 18-11-21  yrh, first implementation
 * \endinternal
 */

#ifndef __AM_LPC84X_BOOT_FLASH_H
#define __AM_LPC84X_BOOT_FLASH_H

#include "hw/amhw_lpc84x_iap.h"
#include "am_boot_flash.h"

typedef struct am_lpc84x_boot_flash_devinfo {
    /**< \brief flash基本信息 */
    am_boot_flash_info_t  base_info;

    void (*pfn_plfm_init)(void);    /**< \brief 平台初始化函数 */

    void (*pfn_plfm_deinit)(void);  /**< \brief 平台去初始化函数 */

} am_lpc84x_boot_flash_devinfo_t;

typedef struct am_lpc84x_boot_flash_dev {
    am_boot_flash_serv_t            flash_serv;     /**< \brief flash标准服务 */
    am_lpc84x_boot_flash_devinfo_t *p_devinfo;      /**< \brief 设备信息结构体 */
}am_lpc84x_boot_flash_dev_t;

/**
 * \brief 初始化flash，返回flash标准服务操作句柄
 *
 * \param[in] p_dev     : 指向flash设备的指针
 * \param[in] p_devinfo : 指向flash设备信息常量的指针
 *
 * \return flash标准服务操作句柄，值为NULL时表明初始化失败
 */
am_boot_flash_handle_t am_lpc84x_boot_flash_init(am_lpc84x_boot_flash_dev_t     *p_dev,
                                                 am_lpc84x_boot_flash_devinfo_t *p_devinfo);


#endif /* __AM_LPC84X_BOOT_FLASH_H */

/* end of file */
