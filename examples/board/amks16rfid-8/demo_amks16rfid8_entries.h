/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2017 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/
/**
 * \file
 * \brief amks16rfid8 所有例程函数入口声明，本文只包含fm175xx相关demo，
 *  其他demo请参照am116_core中的demo
 * \sa demo_amks16rfid8_entries.h
 *
 * \internal
 * \par Modification history
 * - 1.00 18-12-03  htf, first implementation.
 * \endinternal
 */

/**
 * \brief CPU卡测试例程
 */
void demo_amks16rfid8_fm175xx_cpu_card (void);

/**
 * \brief LPCD模式
 */
void demo_amks16rfid8_fm175xx_lpcd_read_id (void);

/**
 * \brief 8天线读取A类卡卡号例程
 */
void demo_amks16rfid8_fm175xx_picca_8ante_read_id (void);

/**
 * \brief A类卡块读写例程
 */
void demo_amks16rfid8_fm175xx_picca_block (void);

/**
 * \brief A类卡读休眠卡例程
 */
void demo_amks16rfid8_fm175xx_picca_halt (void);

/**
 * \brief A类卡读卡类型和卡号例程
 */
void demo_amks16rfid8_fm175xx_picca_read_id (void);

/**
 * \brief A类卡值操作例程
 */
void demo_amks16rfid8_fm175xx_picca_val_operate (void);

/**
 * \brief LPCD模式  配合MCU低功耗模式
 */
void demo_amks16rfid8_dr_fm175xx_lpcd_vlps_read_id (void);


void demo_amks16rfid8_uartcmd_handle_entry (void);

/**
 * \uartcmd 8天线读取卡片ID
 */
void demo_amks16rfid8_uartcmd_picca_8ante_read_id_entry (void);

/**
 * \uartcmd 自动检测测试例程
 */
void demo_amks16rfid8_uartcmd_picca_auto_detect_entry (void);

/**
 * \brief uartcmd 读取写ultralight块
 */
void demo_amks16rfid8_uartcmd_ultralight_write_block_entry (void);

/* end of file */
