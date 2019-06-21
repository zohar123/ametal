/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2019 Guangzhou ZHIYUAN Electronics Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
*******************************************************************************/

/**
 * \file
 * \brief bootloader 单区使用串口通信,使用串口助手发送固件的例程
 *
 *
 * \par 源代码
 * \snippet demo_std_single_bootloader.c src_std_single_bootloader
 *
 * \internal
 * \par Modification history
 * - 1.00 19-05-29 yrh, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_single_bootloader
 * \copydoc demo_std_single_bootloader.c
 */

/** [src_std_bootloader] */
#include "am_boot_firmware_recv.h"
#include "am_boot_firmware.h"
#include "am_boot_enter_check.h"
#include "am_boot.h"
#include "am_vdebug.h"

void demo_std_single_bootloader_entry(am_boot_firwa_recv_handle_t  firwa_recv_handle,
                                      am_boot_enter_check_handle_t enter_check_handle)
{
    int ret;
    /* 判断是否进入应用程序，并判断应用程序区程序是否有效 */
    if(am_boot_enter_check(enter_check_handle) && am_boot_app_is_ready()) {
        /* 跳转到应用程序 */
        am_boot_go_application();
    }

    while(1) {
        /* 接收固件 */
        ret = am_boot_firmware_recv(firwa_recv_handle);
        if(ret == AM_OK) {
            /* 接收成功就是跳出循环 */
            break;
        } else {
            /* 接收失败，重新接收固件 */
            am_kprintf("boot : firmware receive fail,restart receive firmware\r\n");
        }
    }
    if(am_boot_app_is_ready()) {
        am_boot_go_application();
    }
}

/** [src_std_single_bootloader] */

/* end of file */
