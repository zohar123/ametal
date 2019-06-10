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
 * \brief F-RAM(fm25clxx) 配置文件
 *
 * \internal
 * \par Modification history
 * - 1.00 19-06-06  zjr, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_mtd.h"
#include "am_fm25clxx.h"
#include "am_lpc84x.h"
#include "am_lpc84x_inst_init.h"

/**
 * \addtogroup am_if_src_hwconf_microport_fm25clxx_flash
 * \copydoc am_hwconf_microport_fm25clxx_flash.c
 * @{
 */

/** \brief 设备信息 */
struct am_fm25clxx_devinfo fm25clxx_devinfo = {
    AM_SPI_MODE_0,
    PIO0_0,
    3000000
};

/*******************************************************************************
  FRAM(fm25clxx) 实例初始化
*******************************************************************************/

/** \brief 设备定义 */
am_local am_fm25clxx_dev_t __g_fm25clxx_flash_dev;

/** \brief 实例初始化 */
am_fm25clxx_handle_t am_fm25clxx_inst_init (void)
{
    return am_fm25clxx_init(&__g_fm25clxx_flash_dev,
                            &fm25clxx_devinfo,
                             am_lpc84x_spi0_int_inst_init());
}

/** \brief 实例初始化 */
void am_fm25clxx_inst_deinit (am_fm25clxx_handle_t handle)
{
    am_fm25clxx_deinit (handle);
}

/*******************************************************************************
  FRAM 实例初始化（将 FM24C02 用作标准的 NVRAM 设备）
*******************************************************************************/

/** \brief MicroPort-EEPROM 设备实例 */
am_local am_nvram_dev_t __g_fram_nvram_dev;

/**
 * \brief MicroPort-EEPROM 实例初始化（将 FM24C02 用作标准的 NVRAM 设备）
 */
int am_fm25clxx_fram_nvram_inst_init (void)
{
    am_fm25clxx_handle_t handle = am_fm25clxx_inst_init();

    return am_fm25clxx_nvram_init(handle,
                                 &__g_fram_nvram_dev,
                                  "fram_nvram");
}
/**
 * @}
 */

/* end of file */
