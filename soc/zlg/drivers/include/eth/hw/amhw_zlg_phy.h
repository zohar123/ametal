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
 * \brief amhw_zlg_phy.h
 *
 * \internal
 * \par Modification History
 * - 1.00 19-06-11  wyy, first implementation
 * \endinternal
 */
 /***************************************************************************
 includes
 ***************************************************************************/
#include "stdint.h"
#include "amhw_zlg_eth_reg.h"

#define ETHERNET_PHY_ADDRESS        0x05 

#define AM_ZLG_PHY_READ_TO             ((uint32_t)0x0004FFFF)
#define AM_ZLG_ETH_PHY_WRITE_TO        ((uint32_t)0x0004FFFF)

/** \brief Transceiver Basic Control Register */
#define AM_ZLG_PHY_BCR                  0 

/** \brief Transceiver Basic status Register */
#define AM_ZLG_PHY_BSR                  1 

#define IS_ETH_PHY_ADDRESS(ADDRESS) ((ADDRESS) <= 0x20)
#define IS_ETH_PHY_REG(REG)         (((REG) == AM_ZLG_PHY_BCR) || \
                                    ((REG) == AM_ZLG_PHY_BSR) || \
                                    ((REG) == AM_ZLG_PHY_SR))

/** \brief PHY Reset */                             
#define AM_ZLG_PHY_RESET                ((uint16_t)0x8000)     

/** \brief Enable auto-negotiation function */            
#define AM_ZLG_PHY_AUTO_NEGOTIATION     ((uint16_t)0x1000)

/** \brief Auto-Negotiation process completed */            
#define AM_ZLG_PHY_AUTO_NEGO_COMPLETE   ((uint16_t)0x0020)

/** \brief Valid link established */ 
#define AM_ZLG_PHY_LINKED_STATUS        ((uint16_t)0x0004)
 
uint16_t amhw_zlg_phy_read_reg(amhw_zlg_eth_t *p_hw_eth, uint16_t phy_addr, uint16_t phy_reg);
uint32_t amhw_zlg_phy_write_reg(amhw_zlg_eth_t *p_hw_eth, uint16_t phy_addr, uint16_t phy_reg,
        uint16_t phy_value);
