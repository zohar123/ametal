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
 * \brief amhw_zlg_eth.c
 *
 * \internal
 * \par Modification History
 * - 1.00 19-06-11  wyy, first implementation
 * \endinternal
 */

/*******************************************************************************
  includes
*******************************************************************************/

#include "am_zlg_eth.h"
#include "am_clk.h"
#include "zmf159_clk.h"
#include "amhw_zlg_eth_reg.h"
#include "am_vdebug.h"
#include "am_zlg_phy.h"
#include "zmf159_pin.h"
#include "am_gpio.h"
#include "amhw_zlg_eth.h"
#include "amhw_zlg_phy.h"
#include "zmf159_periph_map.h"


/**
 * \brief  Initializes the ETHERNET peripheral according to the specified
 *   parameters in the p_eth_regs_config .
 * \param p_eth_regs_config: pointer to a ETH_InitTypeDef structure that contains
 *   the configuration information for the specified ETHERNET peripheral.
 * \param PHYAddress: external PHY address
 * \retval AM_ZLG_ETH_ERROR: Ethernet initialization failed
 *         AM_ZLG_ETH_SUCCESS: Ethernet successfully initialized
 */
uint32_t amhw_zlg_eth_reg_init(amhw_zlg_eth_t *p_hw_eth, am_eth_regs_conf* p_eth_regs_config,
        uint16_t phy_addr)
{
    uint32_t reg_value = 0, tmp_reg = 0;
    int clk_value = 0;
    __IO uint32_t timeout = 0, err = AM_ZLG_ETH_SUCCESS;
    printf("\r\n amhw_zlg_eth_reg_init! \r\n");

    tmp_reg = p_hw_eth->MACMIIAR;
    tmp_reg &= AM_ZLG_ETH_MACMIIAR_CR_MASK;
    clk_value = am_clk_rate_get(CLK_AHB1);
    printf("\r\n am_clk_rate_get(CLK_AHB1) = %d \r\n", clk_value);
    if ((clk_value >= 20000000) && (clk_value < 35000000)) {
        tmp_reg |= (uint32_t) AM_ZLG_ETH_MACMIIAR_CR_DIV16;
    } else if ((clk_value >= 35000000) && (clk_value < 60000000)) {
        tmp_reg |= (uint32_t) AM_ZLG_ETH_MACMIIAR_CR_DIV26;
    } else if ((clk_value >= 60000000) && (clk_value < 100000000)) {
        tmp_reg |= (uint32_t) AM_ZLG_ETH_MACMIIAR_CR_DIV42;
    } else if ((clk_value >= 100000000) && (clk_value < 150000000)) {
        tmp_reg |= (uint32_t) AM_ZLG_ETH_MACMIIAR_CR_DIV62;
    } else {
        tmp_reg |= (uint32_t) AM_ZLG_ETH_MACMIIAR_CR_DIV102;
    }
    p_hw_eth->MACMIIAR = (uint32_t) tmp_reg;

    if (!(am_zlg_phy_write_reg(p_hw_eth, phy_addr, AM_ZLG_PHY_BCR, AM_ZLG_PHY_RESET))) {
        err = AM_ZLG_ETH_ERROR;
        goto error;
    }
    AM_ZLG_ETH_DELAY(AM_ZLG_PHY_RESET_DELAY);

    if (p_eth_regs_config->eth_auto_negotiation
            != AM_ZLG_ETH_AUTO_NEGOTIATION_DISABLE) {
        printf("wait for link stable.........\r\n");
        printf(".............................\r\n");
        do {
            timeout++;
        } while (!(am_zlg_phy_read_reg(p_hw_eth, phy_addr, AM_ZLG_PHY_BSR)
                & AM_ZLG_PHY_LINKED_STATUS));
        timeout = 0;
        if (!(am_zlg_phy_write_reg(p_hw_eth, phy_addr, AM_ZLG_PHY_BCR,
        AM_ZLG_PHY_AUTO_NEGOTIATION))) {
            err = AM_ZLG_ETH_ERROR;
        }
        do {
            timeout++;
        } while (!(am_zlg_phy_read_reg(p_hw_eth, phy_addr, AM_ZLG_PHY_BSR)
                & AM_ZLG_PHY_AUTO_NEGO_COMPLETE)
                && (timeout < (uint32_t) AM_ZLG_PHY_READ_TO));
        if (timeout == AM_ZLG_PHY_READ_TO) {
            err = AM_ZLG_ETH_ERROR;
            goto error;
        }
        timeout = 0;
        reg_value = am_zlg_phy_read_reg(p_hw_eth, phy_addr, AM_ZLG_PHY_SR);
        if ((reg_value & AM_ZLG_PHY_DUPLEX_STATUS) != (uint32_t) RESET) {
            p_eth_regs_config->eth_mode = AM_ZLG_ETH_MODE_FULLDUPLEX;
            printf(
                    "Set mode by auto AM_ZLG_ETH_MODE_FULLDUPLEX.........\r\n");
        } else {
            p_eth_regs_config->eth_mode = AM_ZLG_ETH_MODE_HALFDUPLEX;
            printf(
                    "Set mode by auto AM_ZLG_ETH_MODE_HALFDUPLEX.........\r\n");
        }
        if (reg_value & AM_ZLG_PHY_SPEED_STATUS) {
            p_eth_regs_config->eth_speed = AM_ZLG_ETH_SPEED_10M;
            printf("Set mode by auto AM_ZLG_ETH_SPEED_10M.........\r\n");
        } else {
            p_eth_regs_config->eth_speed = AM_ZLG_ETH_SPEED_100M;
            printf("Set mode by auto AM_ZLG_ETH_SPEED_100M.........\r\n");
        }
    } else {
        if (!am_zlg_phy_write_reg(p_hw_eth, phy_addr, AM_ZLG_PHY_BCR,
                ((uint16_t) (p_eth_regs_config->eth_mode >> 3)
                        | (uint16_t) (p_eth_regs_config->eth_speed >> 1)))) {
            printf("Set mode by mannual.........\r\n");
            err = AM_ZLG_ETH_ERROR;
        }
        AM_ZLG_ETH_DELAY(AM_ZLG_PHY_CONFIG_DELAY);
    }
    error: if (err == AM_ZLG_ETH_ERROR)
    {
        p_eth_regs_config->eth_mode = AM_ZLG_ETH_MODE_FULLDUPLEX;
        p_eth_regs_config->eth_speed = AM_ZLG_ETH_SPEED_100M;
    }

    tmp_reg = p_hw_eth->MACCR;
    tmp_reg &= AM_ZLG_ETH_MACCR_CLEAR_MASK;
    tmp_reg |= (uint32_t) (p_eth_regs_config->eth_watch_dog
            | p_eth_regs_config->eth_jabber
            | p_eth_regs_config->eth_inter_fame_gap
            | p_eth_regs_config->eth_speed | p_eth_regs_config->eth_receive_own
            | p_eth_regs_config->eth_loop_back_mode
            | p_eth_regs_config->eth_mode
            | p_eth_regs_config->eth_checksum_off_load
            | p_eth_regs_config->eth_retry_transmission
            | p_eth_regs_config->eth_automatic_pad_crc_strip
            | p_eth_regs_config->eth_back_off_limit
            | p_eth_regs_config->eth_deferal_check);
    AM_BIT_SET(tmp_reg, 15); 
    p_hw_eth->MACCR = (uint32_t) tmp_reg;
    tmp_reg = p_hw_eth->MACCR;
    AM_ZLG_ETH_DELAY(AM_ZLG_ETH_REG_WRITE_DELAY);
    p_hw_eth->MACCR = tmp_reg;
    
    p_hw_eth->MACFFR = (uint32_t) (p_eth_regs_config->eth_receive_all
            | p_eth_regs_config->eth_source_addr_filter
            | p_eth_regs_config->eth_pass_ctl_frames
            | p_eth_regs_config->eth_broadcast_frames_reception
            | p_eth_regs_config->eth_destination_addr_filter
            | p_eth_regs_config->eth_promiscuous_mode
            | p_eth_regs_config->eth_multicast_frames_filter
            | p_eth_regs_config->eth_unicast_frames_filter);
    tmp_reg = p_hw_eth->MACFFR;
    AM_ZLG_ETH_DELAY(AM_ZLG_ETH_REG_WRITE_DELAY);
    p_hw_eth->MACFFR = tmp_reg;

    p_hw_eth->MACHTHR = (uint32_t) p_eth_regs_config->eth_hash_table_high;
    p_hw_eth->MACHTLR = (uint32_t) p_eth_regs_config->eth_hash_table_low;
    tmp_reg = p_hw_eth->DMAOMR;
    tmp_reg &= AM_ZLG_ETH_DMAOMR_CLEAR_MASK;
    tmp_reg |= (uint32_t) (p_eth_regs_config->eth_transmit_store_forward
            | p_eth_regs_config->eth_transmit_threshold_ctl
            | p_eth_regs_config->eth_forward_error_frames
            | p_eth_regs_config->eth_forward_under_size_good_frames
            | p_eth_regs_config->eth_receive_threshold_ctl
            | p_eth_regs_config->eth_sec_frame_operate);
    p_hw_eth->DMAOMR = (uint32_t) tmp_reg;
    tmp_reg = p_hw_eth->DMAOMR;
    AM_ZLG_ETH_DELAY(AM_ZLG_ETH_REG_WRITE_DELAY);
    p_hw_eth->DMAOMR = tmp_reg;

    p_hw_eth->DMABMR = (uint32_t) (p_eth_regs_config->eth_fixed_burst
            | p_eth_regs_config->eth_dma_burst_len
            | (p_eth_regs_config->eth_desc_skip_len << 2)
            | p_eth_regs_config->eth_dma_arbitration |
            AM_ZLG_ETH_DMABMR_USP);
    tmp_reg = p_hw_eth->DMABMR;
    AM_ZLG_ETH_DELAY(AM_ZLG_ETH_REG_WRITE_DELAY);
    p_hw_eth->DMABMR = tmp_reg;
    if (err == AM_ZLG_ETH_SUCCESS) {
        return AM_ZLG_ETH_SUCCESS;
    } else
    {
        return AM_ZLG_ETH_ERROR;
    }
}

/**
 * \brief  Enables or disables the MAC transmission.
 * \param  new_state: new state of the MAC transmission.
 *   This parameter can be: ENABLE or DISABLE.
 * \retval None
 */
void amhw_zlg_eth_mac_transmission_cmd(amhw_zlg_eth_t *p_hw_eth, func_state_t new_state)
{
    __IO uint32_t tmp_reg = 0;
    assert_param(IS_FUNCTIONAL_STATE(new_state));

    if (new_state != DISABLE) {
        p_hw_eth->MACCR |= AM_ZLG_ETH_MACCR_TE;
    } else {
        p_hw_eth->MACCR &= ~AM_ZLG_ETH_MACCR_TE;
    }
    tmp_reg = p_hw_eth->MACCR;
    AM_ZLG_ETH_DELAY(AM_ZLG_ETH_REG_WRITE_DELAY);
    p_hw_eth->MACCR = tmp_reg;
}

/**
 * \brief  Enables or disables the MAC reception.
 * \param  new_state: new state of the MAC reception.
 *   This parameter can be: ENABLE or DISABLE.
 * \retval None
 */
void amhw_zlg_eth_mac_reception_cmd(amhw_zlg_eth_t *p_hw_eth, func_state_t new_state)
{
    __IO uint32_t tmp_reg = 0;
    assert_param(IS_FUNCTIONAL_STATE(new_state));

    if (new_state != DISABLE) {
        p_hw_eth->MACCR |= AM_ZLG_ETH_MACCR_RE;
    } else {
        p_hw_eth->MACCR &= ~AM_ZLG_ETH_MACCR_RE;
    }
    tmp_reg = p_hw_eth->MACCR;
    AM_ZLG_ETH_DELAY(AM_ZLG_ETH_REG_WRITE_DELAY);
    p_hw_eth->MACCR = tmp_reg;
}

/**
 * \brief  Configures the selected MAC address.
 * \param  mac_addr: The MAC address to configure.
 *   This parameter can be one of the following values:
 *     \arg AM_ZLG_ETH_MAC_Address0 : MAC Address0
 *     \arg AM_ZLG_ETH_MAC_Address1 : MAC Address1
 *     \arg AM_ZLG_ETH_MAC_Address2 : MAC Address2
 *     \arg AM_ZLG_ETH_MAC_Address3 : MAC Address3
 * \param  addr: Pointer on MAC address buffer data (6 bytes).
 * \retval None
 */
void amhw_zlg_eth_mac_addr_config(uint32_t mac_addr, uint8_t *addr)
{
    uint32_t tmp_reg;
    assert_param(IS_ETH_MAC_ADDRESS0123(mac_addr));
    tmp_reg = ((uint32_t) addr[5] << 8) | (uint32_t) addr[4];
    (*(__IO uint32_t *) (AM_ZLG_ETH_MAC_ADDR_HBASE + mac_addr)) = tmp_reg;
    tmp_reg = ((uint32_t) addr[3] << 24) | ((uint32_t) addr[2] << 16)
            | ((uint32_t) addr[1] << 8) | addr[0];
    (*(__IO uint32_t *) (AM_ZLG_ETH_MAC_ADDR_LBASE + mac_addr)) = tmp_reg;
}

/**
 * \brief  Resets all MAC subsystem internal registers and logic.
 * \param  None
 * \retval None
 */
void amhw_zlg_eth_soft_reset(amhw_zlg_eth_t *p_hw_eth)
{
    p_hw_eth->DMABMR |= AM_ZLG_ETH_DMABMR_SR;
}

/**
 * \brief  Checks whether the ETHERNET software reset bit is set or not.
 * \param  None
 * \retval The new state of DMA Bus Mode register SR bit (SET or RESET).
 */
flag_status_t amhw_zlg_eth_get_soft_reset_status(amhw_zlg_eth_t *p_hw_eth)
{
    flag_status_t bitstatus = RESET;
    if ((p_hw_eth->DMABMR & AM_ZLG_ETH_DMABMR_SR) != (uint32_t) RESET) {
        bitstatus = SET;
    } else {
        bitstatus = RESET;
    }
    return bitstatus;
}

/**
 * \brief  Clears the ETHERNET transmit FIFO.
 * \param  None
 * \retval None
 */
void amhw_zlg_eth_flush_transmit_fifo(amhw_zlg_eth_t *p_hw_eth)
{
    __IO uint32_t tmp_reg = 0;
    p_hw_eth->DMAOMR |= AM_ZLG_ETH_DMAOMR_FTF;
    tmp_reg = p_hw_eth->DMAOMR;
    AM_ZLG_ETH_DELAY(AM_ZLG_ETH_REG_WRITE_DELAY);
    p_hw_eth->DMAOMR = tmp_reg;
}

/**
 * \brief  Enables or disables the DMA transmission.
 * \param  new_state: new state of the DMA transmission.
 *   This parameter can be: ENABLE or DISABLE.
 * \retval None
 */
void amhw_zlg_eth_dma_transmission_cmd(amhw_zlg_eth_t *p_hw_eth, func_state_t new_state)
{
    assert_param(IS_FUNCTIONAL_STATE(new_state));

    if (new_state != DISABLE) {
        p_hw_eth->DMAOMR |= AM_ZLG_ETH_DMAOMR_ST;
    } else {
        p_hw_eth->DMAOMR &= ~AM_ZLG_ETH_DMAOMR_ST;
    }
}

/**
 * \brief  Enables or disables the DMA reception.
 * \param  new_state: new state of the DMA reception.
 *   This parameter can be: ENABLE or DISABLE.
 * \retval None
 */
void amhw_zlg_eth_dma_reception_cmd(amhw_zlg_eth_t *p_hw_eth, func_state_t new_state)
{
    assert_param(IS_FUNCTIONAL_STATE(new_state));

    if (new_state != DISABLE) {
        p_hw_eth->DMAOMR |= AM_ZLG_ETH_DMAOMR_SR;
    } else {
        p_hw_eth->DMAOMR &= ~AM_ZLG_ETH_DMAOMR_SR;
    }
}

void amhw_zlg_eth_set_speed(amhw_zlg_eth_t *p_hw_eth, amhw_zlg_eth_speed_t eth_speed)
{
    uint32_t tmp_reg;
    assert_param(IS_AMHW_ZLG_ETH_SPEED(eth_speed));
    tmp_reg = p_hw_eth->MACCR;
    tmp_reg |= (uint32_t) (eth_speed);
    p_hw_eth->MACCR = (uint32_t) tmp_reg;
}

void amhw_zlg_eth_set_mode(amhw_zlg_eth_t *p_hw_eth, amhw_zlg_eth_mode_t eth_mode)
{
    uint32_t tmp_reg;
    assert_param(IS_AMHW_ZLG_ETH_MODE(eth_mode));
    tmp_reg = p_hw_eth->MACCR;
    tmp_reg |= (uint32_t) (eth_mode);
    p_hw_eth->MACCR = (uint32_t) tmp_reg;
}

