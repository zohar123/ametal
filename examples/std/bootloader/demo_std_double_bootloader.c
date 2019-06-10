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
 * \brief bootloader 双区bootloader 通过串口传输，串口助手发送固件的例程
 *
 *
 * \par 源代码
 * \snippet demo_std_double_bootloader.c src_std_double_bootloader
 *
 * \internal
 * \par Modification history
 * - 1.00 19-06-04 yrh, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_double_bootloader
 * \copydoc demo_std_double_bootloader.c
 */

/** [src_std_double_bootloader] */
#include "am_boot_msg_flag.h"
#include "am_boot_firmware_recv.h"
#include "am_boot_firmware.h"
#include "am_boot_enter_check.h"
#include "am_boot.h"
#include "am_vdebug.h"

void demo_std_double_bootloader_entry(am_boot_firwa_recv_handle_t  firwa_recv_handle,
                                      am_boot_enter_check_handle_t enter_check_handle,
                                      am_boot_msg_handle_t         msg_handle)
{
    int ret;
    /* 是否进入应用程序检测  */
    if(am_boot_enter_check(enter_check_handle)) {
        /* 消息处理 */
        ret = am_boot_msg_dispose(msg_handle);
        if(ret == AM_OK) {
            /* 判断应用程序是否有效 */
            if(am_boot_app_is_ready()) {
                /* 跳转到应用程序 */
                am_boot_go_application();
            }
        }
    }

    while(1) {
        /* 固件接收 */
        ret = am_boot_firmware_recv(firwa_recv_handle);
        if(ret == AM_OK) {
            /* 固件接收成功，设置标志为应用程序区代码有效 */
            am_boot_msg_set(msg_handle, (void *)AM_BOOTLOADER_FLAG_APP);
            break;
        } else {
            /* 如果固件接收失败，重新接收固件 */
            am_kprintf("boot : firmware receive fail,restart receive firmware\r\n");
        }
    }

    if(am_boot_app_is_ready()) {
        am_boot_go_application();
    }
}
/** [src_std_double_bootloader] */

/* end of file */
