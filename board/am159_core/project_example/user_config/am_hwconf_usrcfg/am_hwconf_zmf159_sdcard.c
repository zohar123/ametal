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
 * \brief Miniport-595 用户配置文件
 * \sa am_hwconf_miniport_595.c
 *
 * \internal
 * \par Modification history
 * - 1.00 17-07-13  tee, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_zmf159.h"
#include "am_gpio.h"
#include "am_sdcard.h"
#include "am_clk.h"
#include "am_zmf159_sdio.h"
#include "am_zmf159_clk.h"
#include "hw/amhw_zmf159_sdio.h"

/**
 * \addtogroup am_if_src_hwconf_miniport_595
 * \copydoc am_hwconf_miniport_595.c
 * @{
 */

/*******************************************************************************
   Public functions
*******************************************************************************/
static  am_sd_card_devinfo_t __g_sdcard_info = {
		AMHW_ZLG_ADIO_WIDTH_4BIT,
		0x200

};
static am_sdcard_dev_t    __g_sdcard_dev;

/* MiniPort-595 实例初始化 */
am_sdcard_handle_t am_sdcard_inst_init (void)
{

    return  am_sdcard_sdio_init(&__g_sdcard_dev,
                                &__g_sdcard_info,
							    am_zmf159_sdio_inst_init());

}

/**
 * @}
 */

/* end of file */
