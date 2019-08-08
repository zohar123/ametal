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
 * \brief 进入应用程序标准接口实现（按键驱动）
 *
 * \internal
 * \par Modification history
 * - 1.00 14-11-25  yrh, first implementation.
 * \endinternal
 */

#include "am_boot_enter_check_key.h"
#include "am_gpio.h"

static void __delay(void)
{
    volatile int i= 10000;
    while(i--){

    }
}

am_bool_t __boot_enter_check_key(void *p_drv)
{
    am_boot_enter_check_key_dev_t *p_dev = (am_boot_enter_check_key_dev_t *)p_drv;
    am_gpio_pin_cfg(p_dev->pin, p_dev->flags);
    if(am_gpio_get(p_dev->pin)){
        __delay();
        if(am_gpio_get(p_dev->pin)) {
            return AM_TRUE;
        }
    }
    return  AM_FALSE;
}
static const struct am_boot_enter_check_drv_funcs __g_enter_check_drv_funcs = {
    __boot_enter_check_key,
    NULL,
};

static am_boot_enter_check_key_dev_t __g_enter_check_key_dev;

am_boot_enter_check_handle_t am_boot_enter_check_key_init(int pin, uint32_t flags)
{
    __g_enter_check_key_dev.flags = flags;
    __g_enter_check_key_dev.pin   = pin;
    __g_enter_check_key_dev.enter_check_serv.p_drv   = &__g_enter_check_key_dev;
    __g_enter_check_key_dev.enter_check_serv.p_funcs = &__g_enter_check_drv_funcs;

    return &__g_enter_check_key_dev.enter_check_serv;
}

/* end of file */
