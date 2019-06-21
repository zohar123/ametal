/**
 * \file
 * \brief bootloader 双区应用程序例程
 *
 * - 例程现象：
 *   1. LED灯闪烁，串口循环打印。
 *   2. 等待用户输入升级命令（在应用中升级固件）
 *   
 *
 * \note
 *    固件处理：
 *
 *    打开在ametal/tools/bootloader/固件校验/ 文件夹，目录下有一个bin_chek_sum.bin的执行文件，
 *    将本工程编译出的bin文件拷贝到和bin_chek_sum.bin同一个目录下，
 *    在该目录下打开cmd，在命令行中依次输入：
 *         bin_chek_sum.bin  xxx.bin（编译后的bin文件名称） xxx.bin（目标输出文件，名字任意取）
 *
 *    执行命令后会生成一个由用户自己取名的目标输出文件，这个文件就是需要发送的固件。
 *
 * \par 源代码
 * \snippet demo_am845_core_double_application.c src_am845_core_double_application
 *
 * \internal
 * \par Modification history
 * - 1.00 19-06-09  win, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am845_core_double_application
 * \copydoc demo_am845_core_double_application.c
 */

/** [src_am845_core_double_application] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_led.h"
#include "am_delay.h"
#include "am_boot_enter_check_uart_cmd.h"
#include "am_boot_firmware_recv_uart.h"
#include "am_boot_firmware.h"
#include "am_lpc84x_inst_init.h"
#include "demo_std_entries.h"
#include "am_appconf_lpc84x.h"

void demo_am845_core_double_application_entry (void)
{
    am_uart_handle_t             uart_handle;
    am_boot_firmware_handle_t    firmware_handle;
    am_boot_enter_check_handle_t check_handle;
    am_boot_firwa_recv_handle_t  firwa_recv_handle;
    am_boot_msg_handle_t         msg_handle;

    am_lpc84x_boot_inst_init();
    firmware_handle = am_lpc84x_boot_firmware_flash_inst_init();

    uart_handle = am_lpc84x_usart0_inst_init();
    am_debug_init(uart_handle, 9600);
    check_handle = am_boot_enter_check_uart_cmd_init(uart_handle);
    firwa_recv_handle = am_boot_firwa_recv_uart_init(firmware_handle, uart_handle);
    msg_handle = am_lpc84x_boot_msg_flag_inst_init();

    while(1) {
        /* 此demo需要循环调用 */
        demo_std_double_application_entry(
            firwa_recv_handle,
            check_handle,
            msg_handle);

        am_led_toggle(0);
        am_mdelay(1000);
        AM_DBG_INFO("application : am845_core application running!\r\n");
    }
}
/** [src_am845_core_double_application] */

/* end of file */
