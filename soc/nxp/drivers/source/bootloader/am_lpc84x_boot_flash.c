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
 * \brief bootloader flash drivers implementation
 *
 * \internal
 * \par Modification history
 * - 1.00 18-11-15  yrh, first implementation
 * \endinternal
 */
#include "am_lpc84x_boot_flash.h"
#include "am_int.h"
#include "hw/amhw_lpc84x_iap.h"
#include "am_boot_flash.h"

static int __boot_flash_erase_region(void     *p_drv,
                                     uint32_t  start_addr,
                                     uint32_t  length);
static int __boot_flash_program(void      *p_drv,
                                uint32_t   dst_addr,
                                void      *p_src,
                                uint32_t   size);
static void __boot_flash_info_get(void *p_drv, am_boot_flash_info_t **pp_flash_info);
/**
 * \brief BootLoader flash标准层接口实现
 */
static struct am_boot_flash_drv_funcs __g_flash_drv_funcs = {
    __boot_flash_erase_region,
    __boot_flash_program,
    __boot_flash_info_get,
    NULL,
};
/**
 * \brief flash区域擦除
 */
static int __boot_flash_erase_region(void     *p_drv,
                                     uint32_t  start_addr,
                                     uint32_t  length)
{
    int key;
    amhw_lpc84x_iap_stat_t ret;
    if(length == 0) {
        return AM_OK;
    }
    am_lpc84x_boot_flash_dev_t *p_dev = (am_lpc84x_boot_flash_dev_t *)p_drv;
    uint32_t page_size   = p_dev->p_devinfo->base_info.flash_page_size;
    uint32_t sector_size = p_dev->p_devinfo->base_info.flash_sector_size;
    uint32_t flash_end   = p_dev->p_devinfo->base_info.flash_start_addr + \
                           p_dev->p_devinfo->base_info.flash_size;

    if(start_addr + length > flash_end) {
        return AM_ERROR;
    }

    uint32_t start_page = start_addr / page_size;
    uint32_t end_page   = (start_addr + length - 1) / page_size;

    uint32_t start_sector = start_addr / sector_size;
    uint32_t end_sector   = (start_addr + length - 1) / sector_size;

    key = am_int_cpu_lock();
    ret = amhw_lpc84x_iap_prepare(start_sector, end_sector);
    if(ret != AMHW_LPC84X_IAP_STAT_SUCCESS) {
        am_int_cpu_unlock(key);
        return AM_ERROR;
    }
    if(length < 1024) {
        ret = amhw_lpc84x_iap_erase_page (start_page, end_page);
    } else {
        ret = amhw_lpc84x_iap_erase_sector(start_sector, end_sector);
    }

    am_int_cpu_unlock(key);
    if(ret != AMHW_LPC84X_IAP_STAT_SUCCESS) {
        return AM_ERROR;
    }
    return AM_OK;
}

/**
 * \brief 对flash的写操作
 */
static int __boot_flash_program(void      *p_drv,
                                uint32_t   dst_addr,
                                void      *p_src,
                                uint32_t   size)
{
    int key, count = 0, left_size = 0, i;
    amhw_lpc84x_iap_stat_t ret;
    am_lpc84x_boot_flash_dev_t *p_dev = (am_lpc84x_boot_flash_dev_t *)p_drv;
    if(p_src == NULL) {
        return AM_ERROR;
    }
    if(size == 0) {
        return AM_OK;
    }
    if(size < p_dev->p_devinfo->base_info.flash_page_size) {
        return AM_ERROR;
    }

    uint32_t flash_end   = p_dev->p_devinfo->base_info.flash_start_addr + \
                           p_dev->p_devinfo->base_info.flash_size;

    if(dst_addr + size > flash_end) {
        return AM_ERROR;
    }

    uint32_t sector_size  = p_dev->p_devinfo->base_info.flash_sector_size;
    uint32_t start_sector = dst_addr / sector_size;
    uint32_t end_sector   = (dst_addr + size - 1) / sector_size;

    count = size / 1024;
    left_size = size & (1024 - 1);
    key = am_int_cpu_lock();
    ret = amhw_lpc84x_iap_prepare(start_sector, end_sector);
    if(ret != AMHW_LPC84X_IAP_STAT_SUCCESS) {
        am_int_cpu_unlock(key);
        return ret;
    }
    for(i = 0; i < count; i++) {
        ret = amhw_lpc84x_iap_copy (dst_addr + i * 1024,
                                   (uint32_t)p_src + i * 1024,
                                    1024);
        if(ret != AMHW_LPC84X_IAP_STAT_SUCCESS) {
            return ret;
        }
    }

    if(left_size >= p_dev->p_devinfo->base_info.flash_page_size) {
        ret = amhw_lpc84x_iap_copy (dst_addr + i * 1024,
                                   (uint32_t)p_src + i * 1024,
                                   left_size);
        if(ret != AMHW_LPC84X_IAP_STAT_SUCCESS) {
            return ret;
        }
    }

    am_int_cpu_unlock(key);

    return AM_OK;
}

/**
 * \brief 获取flash的信息
 */
static void __boot_flash_info_get(void *p_drv, am_boot_flash_info_t **pp_flash_info)
{
    am_lpc84x_boot_flash_dev_t *p_dev = (am_lpc84x_boot_flash_dev_t *)p_drv;

    *pp_flash_info = &p_dev->p_devinfo->base_info;
}

/**
 * \brief BootLoader flash初始化函数
 */
am_boot_flash_handle_t am_lpc84x_boot_flash_init(am_lpc84x_boot_flash_dev_t     *p_dev,
                                                 am_lpc84x_boot_flash_devinfo_t *p_devinfo)
{
    if(p_dev == NULL || p_devinfo == NULL) {
        return NULL;
    }
    p_dev->p_devinfo          = p_devinfo;
    p_dev->flash_serv.p_funcs = &__g_flash_drv_funcs;
    p_dev->flash_serv.p_drv   = p_dev;

    p_dev->p_devinfo = p_devinfo;

    return &p_dev->flash_serv;
}

/* end of file */
