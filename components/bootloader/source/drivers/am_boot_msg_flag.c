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
 * \brief BootLoader消息（标志驱动）
 *
 * \internal
 * \par Modification history
 * - 1.00 19-06-04  yrh, first implementation.
 * \endinternal
 */
#include "am_boot_msg_flag.h"
#include "am_boot.h"
#include "am_common.h"
#include "am_vdebug.h"

static am_boot_msg_flag_dev_t __g_msg_flag_dev;
static uint32_t buf[256] = {0};
static int __set_msg_flag(void *p_drv, void *par)
{
    am_boot_msg_flag_dev_t *p_dev = (am_boot_msg_flag_dev_t *)p_drv;
    int write_size = p_dev->p_devinfo->flag_occupy_size;
    if(write_size > 1024) {
        return AM_ERROR;
    }

    uint32_t flag = (uint32_t)par;
    buf[0] = flag;

    return am_boot_mem_write(p_dev->mem_handle,
                             p_dev->p_devinfo->flag_addr,
                             (uint8_t *)&buf,
                             write_size);

}

static int __boot_msg_dispose(void *p_drv)
{
    am_boot_msg_flag_dev_t *p_dev = (am_boot_msg_flag_dev_t *)p_drv;
    int      ret;
    uint32_t flag;
    uint32_t app_addr = p_dev->app_sta_addr;
    uint32_t upt_addr = p_dev->p_devinfo->upt_sta_addr;
    uint32_t app_area_size = p_dev->p_devinfo->app_area_size;

    am_boot_mem_read(p_dev->mem_handle,
                     p_dev->p_devinfo->flag_addr,
                    (uint8_t *)&flag,
                     sizeof(uint32_t));

    if(flag == AM_BOOTLOADER_FLAG_APP) {
        return AM_OK;
    } else if(flag == AM_BOOTLOADER_FLAG_UPDATE) {
        am_kprintf("cpoying firmware from backup\r\n");
        if(p_dev->p_devinfo->pfn_firwa_cpy == NULL) {

            int i, w_size= 0, count = app_area_size / sizeof(buf);
            int left_size = (app_area_size & (sizeof(buf) - 1));

            for(i = 0; i <= count; i++) {
                if(i != count) {
                    w_size = sizeof(buf);
                } else {
                    w_size = left_size;
                }
                ret = am_boot_mem_read(p_dev->mem_handle,
                                       upt_addr + i * sizeof(buf),
                                       (uint8_t *)buf,
                                       w_size);
                if (ret != AM_OK) {
                   return AM_ERROR;
                }

                ret = am_boot_mem_write(p_dev->mem_handle,
                                        app_addr + i * sizeof(buf),
                                        (uint8_t *)buf,
                                        w_size);
                if (ret != AM_OK) {
                   return AM_ERROR;
                }
            }

        } else {
            ret = p_dev->p_devinfo->pfn_firwa_cpy(app_addr, upt_addr);
            if (ret != AM_OK) {
               return AM_ERROR;
            }
        }
        ret = __set_msg_flag(p_dev, (void *)AM_BOOTLOADER_FLAG_APP);
        if (ret != AM_OK) {
           return AM_ERROR;
        }
        return AM_OK;
    } else {
        return AM_ERROR;
    }
}

static const struct am_boot_msg_drv_funcs __g_boot_msg_flag_drv_funcs = {
    __boot_msg_dispose,
    __set_msg_flag,
};

am_boot_msg_handle_t am_boot_msg_flag_init(am_boot_msg_flag_devinfo_t *p_devinfo,
                                           am_boot_mem_handle_t        mem_handle)
{
    if (mem_handle == NULL) {
        return NULL;
    }

    __g_msg_flag_dev.isa.p_funcs = &__g_boot_msg_flag_drv_funcs;
    __g_msg_flag_dev.p_devinfo   = p_devinfo;
    __g_msg_flag_dev.isa.p_drv   = &__g_msg_flag_dev;
    __g_msg_flag_dev.mem_handle  = mem_handle;

    __g_msg_flag_dev.app_sta_addr = am_boot_get_app_addr();

    return &__g_msg_flag_dev.isa;
}

/* end of file */
