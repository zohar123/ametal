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
 * \brief bootloader kboot KinetisFlashTool 总初始化实现
 *
 *
 * \internal
 * \par Modification history
 * - 1.00 18-11-25  yrh, first implementation
 * \endinternal
 */
#include "am_boot_mem_flash.h"
#include "am_boot_kft.h"
#include "am_boot_kft_serial_packet.h"
#include "am_arm_nvic.h"

/* 单实例设备 */
static am_boot_kft_dev_t  __g_boot_kft_dev;

/**
 * \brief bootloader 组件初始化
 */
int am_boot_kft_init(am_boot_mem_handle_t    memory_handle,
                     am_boot_flash_handle_t  flash_handle,
                     am_boot_serial_handle_t serial_handle,
                     uint32_t                ram_start_addr,
                     uint32_t                ram_size)
{

    am_boot_kft_property_devinfo_t     porperty_devifo;
    am_boot_flash_info_t  *p_info;
    am_boot_flash_info_get(flash_handle, &p_info);
    porperty_devifo.flash_size       = p_info->flash_size;
    porperty_devifo.flash_start_addr = p_info->flash_start_addr;
    porperty_devifo.ram_size         = ram_size;
    porperty_devifo.ram_start_addr   = ram_start_addr;

    am_boot_kft_property_handle_t  property_handle = am_boot_kft_property_init(
        flash_handle, &porperty_devifo);

    am_boot_kft_packet_handle_t packet_handle = am_boot_kft_packet_init(serial_handle);

    __g_boot_kft_dev.command_handle = am_boot_kft_command_init(packet_handle,
                                                               memory_handle,
                                                               property_handle,
                                                               flash_handle);
    return AM_OK;
}

/**
 * \brief bootloader kboot KinetisFlashTool 命令处理(命令状态机)
 *
 * \note 该函数需要用户在应用程序里面循环调用，上位机会一直发送信息给bootloader，状态机需要一直去解析
 */
int am_boot_kft_command_pump()
{

    __g_boot_kft_dev.command_handle->p_funcs->pfn_pump(
        __g_boot_kft_dev.command_handle->p_drv);

    return AM_OK;
}

/* end of file */
