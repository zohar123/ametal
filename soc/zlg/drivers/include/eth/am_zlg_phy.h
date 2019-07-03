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
 * \brief am_zlg_phy.h
 *
 * \internal
 * \par Modification History
 * - 1.00 19-06-11  wyy, first implementation
 * \endinternal
 */

#ifndef __AM_ZLG_PHY_H
#define __AM_ZLG_PHY_H

#ifdef __cplusplus
extern "C" {
#endif
#include "stdint.h"
#include "amhw_zlg_eth_reg.h"

#ifdef USE_Delay
#define AM_ZLG_PHY_RESET_DELAY    ((uint32_t)0x00000032)
#define AM_ZLG_PHY_CONFIG_DELAY   ((uint32_t)0x00000032)
#else
#define AM_ZLG_PHY_RESET_DELAY    ((uint32_t)0x000FFFFF)
#define AM_ZLG_PHY_CONFIG_DELAY   ((uint32_t)0x00FFFFFF)
#endif

/** \brief PHY status register Offset */
#define AM_ZLG_PHY_SR                 ((uint16_t)0x0010) 

/** \brief PHY Speed mask  1:10Mb/s  0:100Mb/s */
#define AM_ZLG_PHY_SPEED_STATUS       ((uint16_t)0x0002) 

/** \brief PHY Duplex mask 1:Full duplex  0:Half duplex */
#define AM_ZLG_PHY_DUPLEX_STATUS      ((uint16_t)0x0004)

uint16_t am_zlg_phy_read_reg(amhw_zlg_eth_t *p_hw_eth, uint16_t phy_addr, uint16_t phy_reg);
uint32_t am_zlg_phy_write_reg(amhw_zlg_eth_t *p_hw_eth, uint16_t phy_addr, uint16_t phy_reg,
        uint16_t phy_value);
        
#ifdef __cplusplus
}
#endif

#endif /* __AM_ZLG_PHY_H */
