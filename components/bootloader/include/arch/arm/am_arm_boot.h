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
 * \brief bootloader标准接口实现（arm驱动）
 *
 * \internal
 * \par Modification history
 * - 1.00 18-11-15  yrh, first implementation
 * \endinternal
 */

#ifndef __AM_ARM_BOOT_H
#define __AM_ARM_BOOT_H

#include "am_boot.h"
#include "am_boot_flash.h"

typedef struct am_arm_boot_devinfo {
    /**< \brief 应用代码区起始地址*/
    uint32_t  app_reg_sta_addr;

    /**< \brief ram起始地址 */
    uint32_t  ram_start_addr;

    /**< \brief ram的大小 */
    uint32_t  ram_size;

    /**< \brief 平台初始化函数 */
    void    (*pfn_plfm_init)(void);

    /**< \brief 平台去初始化函数 */
    void    (*pfn_plfm_deinit)(void);
}am_arm_boot_devinfo_t;

typedef struct am_arm_boot_dev {
    am_boot_serv_t         boot_serv;
    am_arm_boot_devinfo_t *p_devinfo;
}am_arm_boot_dev_t;

/**
 * \brief 初始化BootLoader，返回BootLoader标准服务操作句柄
 *
 * \param[in] p_devinfo : 指向BootLoader设备信息常量的指针
 *
 * \return BootLoader标准服务操作句柄，值为NULL时表明初始化失败
 */
int am_arm_boot_init(am_arm_boot_devinfo_t *p_devinfo);


#endif /* __AM_ARM_BOOT_H */
