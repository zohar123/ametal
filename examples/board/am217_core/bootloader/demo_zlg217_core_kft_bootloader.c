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
 * \brief bootloader 例程，本demo是作为bootloader，该demo需要配合kboot的上位机KinetiesFlashTools。
 *
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 18-12-18  yrh, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_led.h"
#include "zlg217_pin.h"
#include "am_boot.h"
#include "am_boot_mem_flash.h"
#include "am_boot_kft.h"
#include "am_boot_enter_check_key.h"
#include "demo_boot_entries.h"
#include "am_zlg217_inst_init.h"
#include "am_bootconf_zlg217.h"
#include "am_boot_autobaud_soft.h"

#define RAM_START_ADDR 0x20000000
#define RAM_SIZE       20 * 1024

void demo_zlg217_core_bootloader_kft_entry (void)
{
    am_uart_handle_t             uart_handle;
    am_boot_flash_handle_t       flash_handle;
    am_boot_mem_handle_t         memory_handle;
    am_boot_autobaud_handle_t    autobaud_handle;
    am_boot_enter_check_handle_t enter_check_handle;

    flash_handle  = am_zlg217_boot_kft_flash_inst_init();
    memory_handle = am_boot_mem_flash_init(flash_handle);

    am_zlg217_boot_kft_inst_init();

    enter_check_handle = am_boot_enter_check_key_init(
        PIOC_7, AM_GPIO_INPUT | AM_GPIO_PULLUP);

    if (am_boot_enter_check(enter_check_handle) && am_boot_app_is_ready()) {
        am_boot_go_application();
    }

    autobaud_handle = am_zlg217_boot_autobaud_inst_init();

    int ret;
    uint32_t baund;
    while(1) {
       ret = am_boot_baudrate_get(autobaud_handle, &baund);
       if (ret == AM_OK) {
           if(baund <= 7200 && baund > 3000) {
               baund = 4800;
           } else if(baund <=384000  && baund > 192000) {
               baund = 256000;
           }
           break;
       }
    }

    am_boot_autobaud_soft_deinit(autobaud_handle);
    /* 串口初始化应该在获取波特率之后 */
    uart_handle = am_zlg217_uart1_inst_init();

    demo_std_bootloader_kft_entry(flash_handle,
                                  memory_handle,
                                  baund,
                                  uart_handle,
                                  RAM_START_ADDR,
                                  RAM_SIZE);
}

/* end of file */
