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
 * \brief am_zlg_phy.c
 *
 * \internal
 * \par Modification History
 * - 1.00 19-06-11  wyy, first implementation
 * \endinternal
 */

/*******************************************************************************
  includes
*******************************************************************************/

#include "am_zlg_phy.h"
#include "amhw_zlg_phy.h"
#include "amhw_zlg_eth.h"
#include "am_zlg_eth.h"

uint16_t am_zlg_phy_read_reg(amhw_zlg_eth_t *p_hw_eth, uint16_t phy_addr, uint16_t phy_reg)
{
    uint16_t ret_value = 0;
    assert_param(IS_ETH_PHY_ADDRESS(phy_addr));
    assert_param(IS_ETH_PHY_REG(phy_reg));
    ret_value = amhw_zlg_phy_read_reg(p_hw_eth, phy_addr, phy_reg);
    return ret_value;
}

uint32_t am_zlg_phy_write_reg(amhw_zlg_eth_t *p_hw_eth, uint16_t phy_addr, uint16_t phy_reg,
        uint16_t phy_value)
{
    uint32_t ret_value = 0;
    assert_param(IS_ETH_PHY_ADDRESS(phy_addr));
    assert_param(IS_ETH_PHY_REG(phy_reg));
    ret_value = amhw_zlg_phy_write_reg(p_hw_eth, phy_addr, phy_reg, phy_value);
    return ret_value;
}

