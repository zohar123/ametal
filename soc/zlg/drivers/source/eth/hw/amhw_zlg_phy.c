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
 * \brief amhw_zlg_phy.c
 *
 * \internal
 * \par Modification History
 * - 1.00 19-06-11  wyy, first implementation
 * \endinternal
 */

/*******************************************************************************
  includes
*******************************************************************************/

#include "amhw_zlg_phy.h"
#include "am_zlg_eth.h"
#include "am_zlg_phy.h"
#include "amhw_zlg_eth_reg.h"
#include "zmf159_regbase.h"
/******************************************************************************/
/*                                PHY functions                               */
/******************************************************************************/

/**
 * \brief  Read a PHY register
 * \param PHYAddress: PHY device address, is the index of one of supported 32 PHY devices.
 *   This parameter can be one of the following values: 0,..,31
 * \param phy_reg: PHY register address, is the index of one of the 32 PHY register.
 *   This parameter can be one of the following values:
 *     \arg AM_ZLG_PHY_BCR: Transceiver Basic Control Register
 *     \arg AM_ZLG_PHY_BSR: Transceiver Basic status Register
 *     \arg AM_ZLG_PHY_SR : Transceiver status Register
 *     \arg More PHY register could be read depending on the used PHY
 * \retval AM_ZLG_ETH_ERROR: in case of timeout
 *         MAC MIIDR register value: Data read from the selected PHY register (correct read )
 */
uint16_t amhw_zlg_phy_read_reg(uint16_t phy_addr, uint16_t phy_reg)
{
    uint32_t tmp_reg = 0;
    __IO uint32_t timeout = 0;

    tmp_reg = ETH->MACMIIAR;
    tmp_reg &= ~AM_ZLG_ETH_MACMIIAR_CR_MASK;
    tmp_reg |= (((uint32_t) phy_addr << 11) & AM_ZLG_ETH_MACMIIAR_PA);
    tmp_reg |= (((uint32_t) phy_reg << 6) & AM_ZLG_ETH_MACMIIAR_MR); 
    tmp_reg &= ~AM_ZLG_ETH_MACMIIAR_MW; 
    tmp_reg |= AM_ZLG_ETH_MACMIIAR_MB; 
    ETH->MACMIIAR = tmp_reg;
    do {
        timeout++;
        tmp_reg = ETH->MACMIIAR;
    } while ((tmp_reg & AM_ZLG_ETH_MACMIIAR_MB)
            && (timeout < (uint32_t) AM_ZLG_PHY_READ_TO));
    if (timeout == AM_ZLG_PHY_READ_TO) {
        return (uint16_t) AM_ZLG_ETH_ERROR;
    }
    return (uint16_t) (ETH->MACMIIDR);
}

/**
 * \brief  Write to a PHY register
 * \param PHYAddress: PHY device address, is the index of one of supported 32 PHY devices.
 *   This parameter can be one of the following values: 0,..,31
 * \param phy_reg: PHY register address, is the index of one of the 32 PHY register.
 *   This parameter can be one of the following values:
 *     \arg AM_ZLG_PHY_BCR    : Transceiver Control Register
 *     \arg More PHY register could be written depending on the used PHY
 * \param  phy_value: the value to write
 * \retval AM_ZLG_ETH_ERROR: in case of timeout
 *         AM_ZLG_ETH_SUCCESS: for correct write
 */
uint32_t amhw_zlg_phy_write_reg(uint16_t phy_addr, uint16_t phy_reg,
        uint16_t phy_value)
{
    uint32_t tmp_reg = 0;
    __IO uint32_t timeout = 0;
    __assert_param(IS_ETH_PHY_ADDRESS(phy_addr));
    __assert_param(IS_ETH_PHY_REG(phy_reg));

    tmp_reg = ETH->MACMIIAR;
    tmp_reg &= ~AM_ZLG_ETH_MACMIIAR_CR_MASK;
    tmp_reg |= (((uint32_t) phy_addr << 11) & AM_ZLG_ETH_MACMIIAR_PA);
    tmp_reg |= (((uint32_t) phy_reg << 6) & AM_ZLG_ETH_MACMIIAR_MR); 
    tmp_reg |= AM_ZLG_ETH_MACMIIAR_MW; 
    tmp_reg |= AM_ZLG_ETH_MACMIIAR_MB; 
    ETH->MACMIIDR = phy_value;
    ETH->MACMIIAR = tmp_reg;
    do {
        timeout++;
        tmp_reg = ETH->MACMIIAR;
    } while ((tmp_reg & AM_ZLG_ETH_MACMIIAR_MB)
            && (timeout < (uint32_t) AM_ZLG_ETH_PHY_WRITE_TO));
    if (timeout == AM_ZLG_ETH_PHY_WRITE_TO) {
        return AM_ZLG_ETH_ERROR;
    }
    return AM_ZLG_ETH_SUCCESS;
}
