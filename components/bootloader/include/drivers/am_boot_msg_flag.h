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
 * \brief bootloader消息（通过标志传达驱动）
 *
 * \internal
 * \par modification history:
 * - 1.00 18-11-30  yrh, first implementation.
 * \endinternal
 */

#ifndef __AM_BOOT_MSG_FLAG_H
#define __AM_BOOT_MSG_FLAG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_boot_msg.h"
#include "am_boot_memory.h"



/**
 * \brief 消息设备信息（标志实现）
 */
typedef struct am_boot_msg_flag_devinfo {
    /**< \brief 固件备份区起始地址  */
    uint32_t  upt_sta_addr;
    /** \brief 应用区大小 */
    uint32_t  app_area_size;
    /** \brief 标志存放起始地址 */
    uint32_t  flag_addr;
    /** \brief 标志占用的实际大小，一般为内存最小的写入单元，如果使用flash内存，则为最小的擦除单元*/
    uint32_t  flag_occupy_size;
    /** \brief 固件拷贝，主要从固件的备份区，拷贝到应用程序的执行区*/
    int     (*pfn_firwa_cpy)(uint32_t dst_addr, uint32_t src_addr);
}am_boot_msg_flag_devinfo_t;

/**
 * \brief 通过标志传递消息的设备
 */
typedef struct am_boot_msg_flag_dev {
    am_boot_msg_serv_t            isa;          /**< \brief 标准的消息服务  */
    am_boot_msg_flag_devinfo_t   *p_devinfo;    /**< \brief 设备信息结构体 */
    am_boot_mem_handle_t          mem_handle;   /**< \brief memory的标准服务操作句柄  */
    uint32_t                      app_sta_addr; /**< \brief 应用程序内存存放的起始地址  */
} am_boot_msg_flag_dev_t;


/**
 * \brief 消息设备初始化（标志实现）
 *
 * \return flash 标准服务操作句柄
 */
am_boot_msg_handle_t am_boot_msg_flag_init(am_boot_msg_flag_devinfo_t *p_devinfo,
                                           am_boot_mem_handle_t        mem_handle);


#ifdef __cplusplus
}
#endif

#endif /* __AM_BOOT_MSG_FLAG_H */

/* end of file */
