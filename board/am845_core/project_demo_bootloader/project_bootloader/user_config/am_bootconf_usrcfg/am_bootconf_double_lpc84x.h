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
 * \brief lpc845 kboot KinetisFlashTool 用户配置文件
 *
 *
 *
 * \internal
 * \par Modification history
 * - 1.00 18-12-13  yrh, first implementation
 * \endinternal
 */

/**
 * \brief bootloader flash实例初始化，获得其标准服务句柄
 *
 * \param 无
 *
 * \return bootloader flash标准服务句柄，若为 NULL，表明初始化失败
 */

#include "am_boot_firmware.h"
#include "am_boot_flash.h"
#include "am_boot_msg_flag.h"
/**
 * \brief bootloader flash实例初始化，获得其标准服务句柄
 *
 * \param 无
 *
 * \return bootloader flash标准服务句柄，若为 NULL，表明初始化失败
 */
am_boot_flash_handle_t am_lpc84x_boot_flash_inst_init(void);

/**
 * \brief bootloader 标准实例初始化
 *
 *
 * \return 若为 AM_OK，表明初始化成功
 */
int am_lpc84x_boot_inst_init(void);

/**
 * \brief bootloader 固件存放 实例初始化
 */
am_boot_firmware_handle_t am_lpc84x_boot_firmware_flash_inst_init(void);

/**
 * \brief bootloader 通过标志设置消息  实例初始化
 */
am_boot_msg_handle_t am_lpc84x_boot_msg_flag_inst_init(void);

/*end of file*/
