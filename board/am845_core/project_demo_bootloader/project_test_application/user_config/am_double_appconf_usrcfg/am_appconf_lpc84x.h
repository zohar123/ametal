#include "am_boot_msg.h"
#include "am_boot_flash.h"
#include "am_boot_firmware.h"
#include "am_boot_firmware_recv.h"
#include "am_boot_enter_check.h"

/**
 * \brief bootloader flash实例初始化，获得其标准服务句柄
 */
am_boot_flash_handle_t am_lpc84x_boot_flash_inst_init(void);

/**
 * \brief bootloader 通过标志传递消息 实例初始化
 */
am_boot_msg_handle_t am_lpc84x_boot_msg_flag_inst_init(void);

/**
 * \brief bootloader 固件存放到flash 实例初始化
 */
am_boot_firmware_handle_t am_lpc84x_boot_firmware_flash_inst_init(void);

/**
 * \brief bootloader 标准接口实例初始化
 */
int am_lpc84x_boot_inst_init(void);

/* end of file */
