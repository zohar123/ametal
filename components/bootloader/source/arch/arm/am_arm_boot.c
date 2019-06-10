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
 * \brief bootloader drivers implementation
 *
 * \internal
 * \par Modification history
 * - 1.00 18-11-15  yrh, first implementation
 * \endinternal
 */
#include "am_arm_boot.h"
#include "am_arm_nvic.h"
#include "am_int.h"
#include "am_boot.h"
#include "hw/amhw_arm_nvic.h"

/** \brief boot设备实例 */
static am_arm_boot_dev_t __g_boot_dev;

static void __set_msp(uint32_t top_mstack)
{

#if   defined ( __GNUC__ )
    __asm volatile ("MSR msp, %0\n" : : "r" (top_mstack) : "sp");
#endif

#if   defined ( __CC_ARM )
    register uint32_t __regProcessStackPointer  __asm("msp");
    __regProcessStackPointer = top_mstack;
#endif

}

static void __set_psp(uint32_t top_pstack)
{

#if   defined ( __GNUC__ )
    __asm volatile ("MSR psp, %0\n" : : "r" (top_pstack) : "sp");
#endif

#if   defined ( __CC_ARM )
    register uint32_t __regMainStackPointer     __asm("psp");
    __regMainStackPointer = top_pstack;
#endif

}

static void __system_reset()
{
    __asm volatile ("dsb");
    AMHW_ARM_SCB->aircr = ((0x5FA << 16)      |
                           (AMHW_ARM_SCB->aircr & (7UL << 8)) |
                           (1UL << 2));
    __asm volatile ("dsb");
    while(1);
}

/**
 * \brief 判断应用代码是否有效
 */
am_bool_t __boot_app_is_ready(void)
{
    uint32_t app_start_addr = __g_boot_dev.p_devinfo->app_reg_sta_addr;
    if ((!app_start_addr) || (app_start_addr == 0xffffffff)) {
        return AM_FALSE;
    }

    uint32_t ram_start_addr = __g_boot_dev.p_devinfo->ram_start_addr;
    uint32_t ram_size       = __g_boot_dev.p_devinfo->ram_size;

    if((*(uint32_t *)app_start_addr) > (ram_start_addr + ram_size )||
       (*(uint32_t *)app_start_addr) < ram_start_addr) {
        return AM_FALSE;
    }

    return AM_TRUE;
}

/**
 * \brief 系统重启
 */
void __boot_reset(void)
{
    __system_reset();
}

/**
 * \brief 获取应用代码存放的起始地址
 *
 * \retval > 0 返回应用代码存放的起始地址
 *         < 0 无法获取地址
 */
static int __boot_get_app_addr(void)
{

    return __g_boot_dev.p_devinfo->app_reg_sta_addr;
}

/**
 * \brief 跳转到应用代码
 */
int __boot_go_application(void)
{

    uint32_t app_start_addr = __g_boot_dev.p_devinfo->app_reg_sta_addr;
    uint32_t stack_addr = *(uint32_t *)(app_start_addr);

    void (*farewell_bootloader)(void);

    farewell_bootloader = (void (*)(void))(*(uint32_t *)(app_start_addr + 4));

    __g_boot_dev.p_devinfo->pfn_plfm_deinit();

    /* 设置栈指针 */
    __set_msp(stack_addr);
    __set_psp(stack_addr);

    /* 跳转到应用代码 */
    farewell_bootloader();
    /*如果正常跳转，代码不会执行到这里*/
    return -AM_ERROR;
}

am_local struct am_boot_drv_funcs __g_drv_funcs = {
    __boot_app_is_ready,
    __boot_reset,
    __boot_go_application,
    __boot_get_app_addr,
};

/**
 * \brief BootLoader初始化函数
 */
int am_arm_boot_init(am_arm_boot_devinfo_t *p_devinfo)
{
    if(p_devinfo == NULL) {
        return -AM_ENXIO;
    }

    __g_boot_dev.p_devinfo         = p_devinfo;
    __g_boot_dev.boot_serv.p_funcs = &__g_drv_funcs;
    __g_boot_dev.boot_serv.drv     = &__g_boot_dev;

    am_boot_init(&__g_boot_dev.boot_serv);

    return AM_OK;
}

/* end of file */
