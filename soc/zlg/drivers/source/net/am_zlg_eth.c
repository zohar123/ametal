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
 * \brief am_zlg_eth.c
 *
 * \internal
 * \par Modification History
 * - 1.00 19-06-11  wyy, first implementation
 * \endinternal
 */

/***************************************************************************
 includes
 ***************************************************************************/
#include "am_zlg_eth.h"
#include <string.h>
#include "am_clk.h"
#include "zmf159_clk.h"
#include "am_zlg_enet_reg.h"
#include "am_vdebug.h"
#include "stdint.h"
#include "am_zlg_eth.h"

#if defined   (__CC_ARM) /*!< ARM Compiler */
__align(4)
eth_dma_desc_type_t g_dma_rx_desc_tab[AM_ETH_RXBUFNB];/* Ethernet Rx MA Descriptor */
__align(4)
eth_dma_desc_type_t g_dma_tx_desc_tab[AM_ETH_TXBUFNB];/* Ethernet Tx DMA Descriptor */
__align(4)
uint8_t g_rx_buff[AM_ETH_RXBUFNB][AM_ETH_ETH_RX_BUF_SIZE]; /* Ethernet Receive Buffer */
__align(4)
uint8_t g_tx_buff[AM_ETH_TXBUFNB][AM_ETH_TX_BUF_SIZE]; /* Ethernet Transmit Buffer */

#elif defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4
eth_dma_desc_type_t g_dma_rx_desc_tab[AM_ETH_RXBUFNB];/* Ethernet Rx MA Descriptor */
#pragma data_alignment=4
eth_dma_desc_type_t g_dma_tx_desc_tab[AM_ETH_TXBUFNB];/* Ethernet Tx DMA Descriptor */
#pragma data_alignment=4
uint8_t g_rx_buff[AM_ETH_RXBUFNB][AM_ETH_ETH_RX_BUF_SIZE]; /* Ethernet Receive Buffer */
#pragma data_alignment=4
uint8_t g_tx_buff[AM_ETH_TXBUFNB][AM_ETH_TX_BUF_SIZE]; /* Ethernet Transmit Buffer */

#elif defined (__GNUC__) /*!< GNU Compiler */
eth_dma_desc_type_t g_dma_rx_desc_tab[AM_ETH_RXBUFNB] __attribute__ ((aligned (4))); /* Ethernet Rx DMA Descriptor */
eth_dma_desc_type_t g_dma_tx_desc_tab[AM_ETH_TXBUFNB] __attribute__ ((aligned (4))); /* Ethernet Tx DMA Descriptor */
uint8_t g_rx_buff[AM_ETH_RXBUFNB][AM_ETH_ETH_RX_BUF_SIZE] __attribute__ ((aligned (4))); /* Ethernet Receive Buffer */
uint8_t g_tx_buff[AM_ETH_TXBUFNB][AM_ETH_TX_BUF_SIZE] __attribute__ ((aligned (4))); /* Ethernet Transmit Buffer */

#elif defined  (__TASKING__) /*!< TASKING Compiler */
__align(4)
eth_dma_desc_type_t g_dma_rx_desc_tab[AM_ETH_RXBUFNB];/* Ethernet Rx MA Descriptor */
__align(4)
eth_dma_desc_type_t g_dma_tx_desc_tab[AM_ETH_TXBUFNB];/* Ethernet Tx DMA Descriptor */
__align(4)
uint8_t g_rx_buff[AM_ETH_RXBUFNB][AM_ETH_ETH_RX_BUF_SIZE]; /* Ethernet Receive Buffer */
__align(4)
uint8_t g_tx_buff[AM_ETH_TXBUFNB][AM_ETH_TX_BUF_SIZE]; /* Ethernet Transmit Buffer */

#endif /* __CC_ARM */

/* Global pointers on Tx and Rx descriptor used to track transmit and receive descriptors */
__IO eth_dma_desc_type_t *g_dma_tx_desc_to_set;
__IO eth_dma_desc_type_t *g_dma_rx_desc_to_get;

/* Structure used to hold the last received packet descriptors info */
eth_dma_rx_frame_info_t g_rx_frame_desc;
__IO eth_dma_rx_frame_info_t *gp_rx_frame_desc;
__IO uint32_t g_frame_rx_index;

/******************************************************************************/
/*                           Global ETH MAC/DMA functions                     */
/******************************************************************************/
/**
 * @brief  Fills each p_eth_regs_config member with its default value.
 * @param  p_eth_regs_config: pointer to a ETH_InitTypeDef structure which will be initialized.
 * @retval None
 */
void am_eth_usr_config_init(am_eth_regs_conf *p_eth_regs_config)
{
    /* p_eth_regs_config members default value */
    /*------------------------   MAC Configuration   ---------------------------*/

    /* PHY Auto-negotiation enabled */
    p_eth_regs_config->eth_auto_negotiation = AM_ETH_AUTO_NEGOTIATION_ENABLE;
    /* MAC watchdog enabled: cuts off long frame */
    p_eth_regs_config->eth_watch_dog = AM_ETH_WATCH_DOG_ENABLE;
    /* MAC Jabber enabled in Half-duplex mode */
    p_eth_regs_config->eth_jabber = AM_ETH_JABBER_ENABLE;
    /* Ethernet interframe gap set to 96 bits */
    p_eth_regs_config->eth_inter_fame_gap = AM_ETH_INTER_FRAME_GAP_96_BIT;
    /* PHY speed configured to 100Mbit/s */
    p_eth_regs_config->eth_speed = AM_ETH_SPEED_100M;
    /* Receive own Frames in Half-Duplex mode enabled */
    p_eth_regs_config->eth_receive_own = AM_ETH_RECEIVE_OWN_ENABLE;
    /* MAC MII loopback disabled */
    p_eth_regs_config->eth_loop_back_mode = AM_ETH_LOOPBACK_MODE_DISABLE;
    /* Full-Duplex mode selected */
    p_eth_regs_config->eth_mode = AM_ETH_MODE_FULLDUPLEX;
    /* IPv4 and TCP/UDP/ICMP frame Checksum Offload disabled */
    p_eth_regs_config->eth_checksum_off_load = AM_ETH_CHECKSUM_OFFLOAD_DISABLE;
    /* Retry Transmission enabled for half-duplex mode */
    p_eth_regs_config->eth_retry_transmission =
            AM_ETH_RETRY_TRANSMISSION_ENABLE;
    /* Automatic PAD/CRC strip disabled*/
    p_eth_regs_config->eth_automatic_pad_crc_strip =
            AM_ETH_AUTOMATIC_PAD_CRC_STRIP_DISABLE;
    /* half-duplex mode retransmission Backoff time_limit = 10 slot times*/
    p_eth_regs_config->eth_back_off_limit = AM_ETH_BACK_OFF_LIMIT_10;
    /* half-duplex mode Deferral check disabled */
    p_eth_regs_config->eth_deferal_check = AM_ETH_DEFERRAL_CHECK_DISABLE;
    /* Receive all frames disabled */
    p_eth_regs_config->eth_receive_all = AM_ETH_RECEIVE_ALL_DISABLE;
    /* Source address filtering (on the optional MAC addresses) disabled */
    p_eth_regs_config->eth_source_addr_filter =
            AM_ETH_SOURCE_ADDR_FILTER_DISABLE;
    /* Do not forward control frames that do not pass the address filtering */
    p_eth_regs_config->eth_pass_ctl_frames = AM_ETH_PASS_CTL_FRAMES_BLOCKALL;
    /* Disable reception of Broadcast frames */
    p_eth_regs_config->eth_broadcast_frames_reception =
            AM_ETH_BROADCAST_FRAMES_RECEPTION_ENABLE;
    /* Normal Destination address filtering (not reverse addressing) */
    p_eth_regs_config->eth_destination_addr_filter =
            AM_ETH_DESTINATION_ADDR_FILTER_NORMAL;
    /* Promiscuous address filtering mode disabled */
    p_eth_regs_config->eth_promiscuous_mode = AM_ETH_PROMISCUOUS_MODE_DISABLE;
    /* Perfect address filtering for multicast addresses */
    p_eth_regs_config->eth_multicast_frames_filter =
            AM_ETH_MULTICAST_FRAMES_FILTER_PERFECT;
    /* Perfect address filtering for unicast addresses */
    p_eth_regs_config->eth_unicast_frames_filter =
            AM_ETH_UNICAST_FRAMES_FILTER_PERFECT;
    /* Initialize hash table high and low regs */
    p_eth_regs_config->eth_hash_table_high = 0x0;
    p_eth_regs_config->eth_hash_table_low = 0x0;
    /* Flow control config (flow control disabled)*/
    p_eth_regs_config->eth_pause_time = 0x0;
    p_eth_regs_config->eth_zero_quanta_pause = AM_ETH_ZERO_QUANTA_PAUSE_DISABLE;
    p_eth_regs_config->eth_pause_low_threshold =
            AM_ETH_PAUSE_LOW_THRESHOLD_MINUS4;
    p_eth_regs_config->eth_unicast_pause_frame_detect =
            AM_ETH_UNICAST_PAUSE_FRAME_DETECT_DISABLE;
    p_eth_regs_config->eth_receive_flow_ctl = AM_ETH_RECEIVE_FLOW_CRL_DISABLE;
    p_eth_regs_config->eth_tramsmit_flow_ctl = AM_ETH_TRANSMIT_FLOW_CRL_DISABLE;
    /* VLANtag config (VLAN field not checked) */
    p_eth_regs_config->eth_vlan_tag_comparison =
            AM_ETH_VLAN_TAG_COMPARISON_16BIT;
    p_eth_regs_config->eth_vlan_tag_identifier = 0x0;

    /*---------------------- DMA Configuration   -------------------------------*/
    /* Store and forward mode enabled for transmit */
    p_eth_regs_config->eth_transmit_store_forward =
            AM_ETH_TRANSMIT_STORE_FORWARD_ENABLE;
    /* Threshold TXFIFO level set to 64 bytes (used when threshold mode is enabled) */
    p_eth_regs_config->eth_transmit_threshold_ctl =
            AM_ETH_TRANSMIT_THRESHOLD_CTL_64BYTES;
    /* Disable forwarding frames with errors (short frames, CRC,...)*/
    p_eth_regs_config->eth_forward_error_frames =
            AM_ETH_FORWARD_ERROR_FRAMES_DISABLE;
    /* Disable undersized good frames */
    p_eth_regs_config->eth_forward_under_size_good_frames =
            AM_ETH_FORWARD_UNDERSIZED_GOOD_FRAMES_DISABLE;
    /* Threshold RXFIFO level set to 64 bytes (used when Cut through mode is enabled) */
    p_eth_regs_config->eth_receive_threshold_ctl =
            AM_ETH_RECEIVE_THRESHOLD_CTL_64BYTES;
    /* Disable Operate on second frame (transmit a second frame to FIFO without
     waiting status of previous frame*/
    p_eth_regs_config->eth_sec_frame_operate = AM_ETH_SEC_FRAME_OPERATE_ENABLE;
    /* Enabled Fixed Burst Mode (mix of INC4, INC8, INC16 and SINGLE DMA transactions */
    p_eth_regs_config->eth_fixed_burst = AM_ETH_FIXED_BURST_ENABLE;
    /* DMA transfer max burst length = 32 beats = 32 x 32bits */
    p_eth_regs_config->eth_dma_burst_len = AM_ETH_TX_DMA_BURST_LENGTH_32BEAT;
    /* DMA Ring mode skip length = 0 */
    p_eth_regs_config->eth_desc_skip_len = 0x0;
    /* Equal priority (round-robin) between transmit and receive DMA engines */
    p_eth_regs_config->eth_dma_arbitration =
            AM_ETH_DMA_ARBITRATION_ROUNDROBIN_RXTX_2_1;
#ifdef CHECKSUM_BY_HARDWARE
    /* 开启ipv4和TCP/UDP/ICMP的帧校验和卸载   */
    p_eth_regs_config->eth_checksum_offload = AM_ETH_CHECKSUM_OFFLOAD_ENABLE;
#endif
}

void am_eth_check_usr_config(am_eth_regs_conf *p_eth_regs_config)
{
    /* Check the parameters */
    /* MAC --------------------------*/
    __assert_param(
            IS_AM_ETH_AUTO_NEGOTIATION(p_eth_regs_config->eth_auto_negotiation));
    __assert_param(IS_AM_ETH_WATCH_DOG(p_eth_regs_config->eth_watch_dog));
    __assert_param(IS_AM_ETH_JABBER(p_eth_regs_config->eth_jabber));
    __assert_param(
            IS_AM_ETH_INTER_FRAME_GAP(p_eth_regs_config->eth_inter_fame_gap));
    __assert_param(IS_AM_ETH_SPEED(p_eth_regs_config->eth_speed));
    __assert_param(IS_AM_ETH_RECEIVE_OWN(p_eth_regs_config->eth_receive_own));
    __assert_param(
            IS_AM_ETH_LOOPBACK_MODE(p_eth_regs_config->eth_loop_back_mode));
    __assert_param(IS_AM_ETH_DUPLEX_MODE(p_eth_regs_config->eth_mode));
    __assert_param(
            IS_AM_ETH_CHECKSUM_OFFLOAD(p_eth_regs_config->eth_checksum_off_load));
    __assert_param(
            IS_AM_ETH_RETRY_TRANSMISSION(p_eth_regs_config->eth_retry_transmission));
    __assert_param(
            IS_AM_ETH_AUTOMATIC_PADCRC_STRIP(p_eth_regs_config->eth_automatic_pad_crc_strip));
    __assert_param(
            IS_AM_ETH_BACK_OFF_LIMIT(p_eth_regs_config->eth_back_off_limit));
    __assert_param(
            IS_AM_ETH_DEFERRAL_CHECK(p_eth_regs_config->eth_deferal_check));
    __assert_param(IS_AM_ETH_RECEIVE_ALL(p_eth_regs_config->eth_receive_all));
    __assert_param(
            IS_AM_ETH_SOURCE_ADDR_FILTER(p_eth_regs_config->eth_source_addr_filter));
    __assert_param(
            IS_AM_ETH_CONTROL_FRAMES(p_eth_regs_config->eth_pass_ctl_frames));
    __assert_param(
            IS_AM_ETH_BROADCAST_FRAMES_RECEPTION(p_eth_regs_config->eth_broadcast_frames_reception));
    __assert_param(
            IS_AM_ETH_DESTINATION_ADDR_FILTER(p_eth_regs_config->eth_destination_addr_filter));
    __assert_param(
            IS_AM_ETH_PROMISCUOUS_MODE(p_eth_regs_config->eth_promiscuous_mode));
    __assert_param(
            IS_AM_ETH_MULTICAST_FRAMES_FILTER(p_eth_regs_config->eth_multicast_frames_filter));
    __assert_param(
            IS_AM_ETH_UNICAST_FRAMES_FILTER(p_eth_regs_config->eth_unicast_frames_filter));
    __assert_param(IS_AM_ETH_PAUSE_TIME(p_eth_regs_config->eth_pause_time));
    __assert_param(
            IS_AM_ETH_ZERO_QUANTA_PAUSE(p_eth_regs_config->eth_zero_quanta_pause));
    __assert_param(
            IS_AM_ETH_PAUSE_LOW_THRESHOLD(p_eth_regs_config->eth_pause_low_threshold));
    __assert_param(
            IS_AM_ETH_UNICAST_PAUSE_FRAME_DETECT(p_eth_regs_config->eth_unicast_pause_frame_detect));
    __assert_param(
            IS_AM_ETH_RECEIVE_FLOW_CRL(p_eth_regs_config->eth_receive_flow_ctl));
    __assert_param(
            IS_AM_ETH_TRANSMIT_FLOW_CRL(p_eth_regs_config->eth_tramsmit_flow_ctl));
    __assert_param(
            IS_AM_ETH_VLAN_TAG_COMPARISON(p_eth_regs_config->eth_vlan_tag_comparison));
    __assert_param(
            IS_AM_ETH_VLAN_TAG_IDENTIFIER(p_eth_regs_config->eth_vlan_tag_identifier));

    /* DMA --------------------------*/
    __assert_param(
            IS_AM_ETH_TRANSMIT_STORE_FORWARD(p_eth_regs_config->eth_transmit_store_forward));
    __assert_param(
            IS_AM_ETH_TRANSMIT_THRESHOLD_CTL(p_eth_regs_config->eth_transmit_threshold_ctl));
    __assert_param(
            IS_AM_ETH_FORWARD_ERROR_FRAMES(p_eth_regs_config->eth_forward_error_frames));
    __assert_param(
            IS_AM_ETH_FORWARD_UNDERSIZED_GOOD_FRAMES(p_eth_regs_config->eth_forward_under_size_good_frames));
    __assert_param(
            IS_AM_ETH_RECEIVE_THRESHOLD_CTL(p_eth_regs_config->eth_receive_threshold_ctl));
    __assert_param(
            IS_AM_ETH_SEC_FRAME_OPERATE(p_eth_regs_config->eth_sec_frame_operate));
    __assert_param(IS_AM_ETH_FIXED_BURST(p_eth_regs_config->eth_fixed_burst));
    __assert_param(
            IS_AM_ETH_TX_DMA_BURST_LENGTH(p_eth_regs_config->eth_dma_burst_len));
    __assert_param(
            IS_AM_ETH_DMA_DESC_SKIP_LENGTH(p_eth_regs_config->eth_desc_skip_len));
    __assert_param(
            IS_AM_ETH_DMA_ARBITRATION_ROUNDROBIN_RXTX(p_eth_regs_config->eth_dma_arbitration));

}
/**
 * @brief  Initializes the ETHERNET peripheral according to the specified
 *   parameters in the p_eth_regs_config .
 * @param p_eth_regs_config: pointer to a ETH_InitTypeDef structure that contains
 *   the configuration information for the specified ETHERNET peripheral.
 * @param PHYAddress: external PHY address
 * @retval AM_ETH_ERROR: Ethernet initialization failed
 *         AM_ETH_SUCCESS: Ethernet successfully initialized
 */
uint32_t __am_eth_reg_init(am_eth_regs_conf* p_eth_regs_config, uint16_t phy_addr)
{
    uint32_t reg_value = 0, tmp_reg = 0;
    uint32_t clk_value = 0;
    __IO uint32_t timeout = 0, err = AM_ETH_SUCCESS;
    AM_DBG_INFO("\r\n __am_eth_reg_init! \r\n");
    am_eth_check_usr_config(p_eth_regs_config);

    /*-------------------------------- MAC Config ------------------------------*/
    tmp_reg = ETH->MACMIIAR;
    tmp_reg &= AM_MACMIIAR_CR_MASK;
    clk_value = am_clk_rate_get(CLK_AHB1);
    AM_DBG_INFO("\r\n am_clk_rate_get(CLK_AHB1) = %d \r\n", clk_value);
    if ((clk_value >= 20000000) && (clk_value < 35000000)) {
        tmp_reg |= (uint32_t) AM_ETH_MACMIIAR_CR_DIV16;
    } else if ((clk_value >= 35000000) && (clk_value < 60000000)) {
        tmp_reg |= (uint32_t) AM_ETH_MACMIIAR_CR_DIV26;
    } else if ((clk_value >= 60000000) && (clk_value < 100000000)) {
        tmp_reg |= (uint32_t) AM_ETH_MACMIIAR_CR_DIV42;
    } else if ((clk_value >= 100000000) && (clk_value < 150000000)) {
        tmp_reg |= (uint32_t) AM_ETH_MACMIIAR_CR_DIV62;
    } else {
        tmp_reg |= (uint32_t) AM_ETH_MACMIIAR_CR_DIV102;
    }
    ETH->MACMIIAR = (uint32_t) tmp_reg;

    if (!(__am_eth_write_phy_reg(phy_addr, AM_PHY_BCR, AM_PHY_RESET))) {
        err = AM_ETH_ERROR;
        goto error;
    }
    __AM_ETH_DELAY__(AM_PHY_RESET_DELAY);

    if (p_eth_regs_config->eth_auto_negotiation
            != AM_ETH_AUTO_NEGOTIATION_DISABLE) {
        AM_DBG_INFO("wait for link stable.........\r\n");
        AM_DBG_INFO(".............................\r\n");
        /* We wait for linked status...*/
        do {
            timeout++;
        } while (!(__am_eth_read_phy_reg(phy_addr, AM_PHY_BSR)
                & AM_PHY_LINKED_STATUS));
        timeout = 0;
        /* Enable Auto-Negotiation */
        if (!(__am_eth_write_phy_reg(phy_addr, AM_PHY_BCR,
                AM_PHY_AUTO_NEGOTIATION))) {
            /* Return ERROR in case of write timeout */
            err = AM_ETH_ERROR;
        }
        do {
            timeout++;
        } while (!(__am_eth_read_phy_reg(phy_addr, AM_PHY_BSR)
                & AM_PHY_AUTO_NEGO_COMPLETE)
                && (timeout < (uint32_t) AM_PHY_READ_TO));
        if (timeout == AM_PHY_READ_TO) {
            err = AM_ETH_ERROR;
            goto error;
        }
        timeout = 0;
        reg_value = __am_eth_read_phy_reg(phy_addr, AM_PHY_SR);
        if ((reg_value & AM_PHY_DUPLEX_STATUS) != (uint32_t) RESET) {
            p_eth_regs_config->eth_mode = AM_ETH_MODE_FULLDUPLEX;
            AM_DBG_INFO("Set mode by auto AM_ETH_MODE_FULLDUPLEX.........\r\n");
        } else {
            p_eth_regs_config->eth_mode = AM_ETH_MODE_HALFDUPLEX;
            AM_DBG_INFO("Set mode by auto AM_ETH_MODE_HALFDUPLEX.........\r\n");
        }
        if (reg_value & AM_PHY_SPEED_STATUS) {
            p_eth_regs_config->eth_speed = AM_ETH_SPEED_10M;
            AM_DBG_INFO("Set mode by auto AM_ETH_SPEED_10M.........\r\n");
        } else {
            p_eth_regs_config->eth_speed = AM_ETH_SPEED_100M;
            AM_DBG_INFO("Set mode by auto AM_ETH_SPEED_100M.........\r\n");
        }
    } else {
        if (!__am_eth_write_phy_reg(phy_addr, AM_PHY_BCR,
                ((uint16_t) (p_eth_regs_config->eth_mode >> 3)
                        | (uint16_t) (p_eth_regs_config->eth_speed >> 1)))) {
            AM_DBG_INFO("Set mode by mannual.........\r\n");
            err = AM_ETH_ERROR;
        }
        __AM_ETH_DELAY__(AM_PHY_CONFIG_DELAY);
    }
    error: if (err == AM_ETH_ERROR) /* Auto-negotiation failed */
    {
        p_eth_regs_config->eth_mode = AM_ETH_MODE_FULLDUPLEX;
        p_eth_regs_config->eth_speed = AM_ETH_SPEED_100M;
    }
    /*------------------------ ETHERNET MACCR Configuration --------------------*/
    tmp_reg = ETH->MACCR;
    tmp_reg &= AM_MACCR_CLEAR_MASK;
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
    AM_BIT_SET(tmp_reg, 15); /* set PS bit, 选择端口为 MII(10/100Mbps) */
    ETH->MACCR = (uint32_t) tmp_reg;
    tmp_reg = ETH->MACCR;
    __AM_ETH_DELAY__(AM_ETH_REG_WRITE_DELAY);
    ETH->MACCR = tmp_reg;
    /*----------------------- ETHERNET MACFFR Configuration --------------------*/
    ETH->MACFFR = (uint32_t) (p_eth_regs_config->eth_receive_all
            | p_eth_regs_config->eth_source_addr_filter
            | p_eth_regs_config->eth_pass_ctl_frames
            | p_eth_regs_config->eth_broadcast_frames_reception
            | p_eth_regs_config->eth_destination_addr_filter
            | p_eth_regs_config->eth_promiscuous_mode
            | p_eth_regs_config->eth_multicast_frames_filter
            | p_eth_regs_config->eth_unicast_frames_filter);
    tmp_reg = ETH->MACFFR;
    __AM_ETH_DELAY__(AM_ETH_REG_WRITE_DELAY);
    ETH->MACFFR = tmp_reg;

    /*--------------- ETHERNET MACHTHR and MACHTLR Configuration ---------------*/
    ETH->MACHTHR = (uint32_t) p_eth_regs_config->eth_hash_table_high;
    ETH->MACHTLR = (uint32_t) p_eth_regs_config->eth_hash_table_low;
    /*-------------------------------- DMA Config ------------------------------*/
    /*----------------------- ETHERNET DMAOMR Configuration --------------------*/
    tmp_reg = ETH->DMAOMR;
    tmp_reg &= AM_DMAOMR_CLEAR_MASK;
    tmp_reg |= (uint32_t) (p_eth_regs_config->eth_transmit_store_forward
            | p_eth_regs_config->eth_transmit_threshold_ctl
            | p_eth_regs_config->eth_forward_error_frames
            | p_eth_regs_config->eth_forward_under_size_good_frames
            | p_eth_regs_config->eth_receive_threshold_ctl
            | p_eth_regs_config->eth_sec_frame_operate);
    ETH->DMAOMR = (uint32_t) tmp_reg;
    tmp_reg = ETH->DMAOMR;
    __AM_ETH_DELAY__(AM_ETH_REG_WRITE_DELAY);
    ETH->DMAOMR = tmp_reg;

    /*----------------------- ETHERNET DMABMR Configuration --------------------*/
    ETH->DMABMR = (uint32_t) (p_eth_regs_config->eth_fixed_burst
            | p_eth_regs_config->eth_dma_burst_len
            | (p_eth_regs_config->eth_desc_skip_len << 2)
            | p_eth_regs_config->eth_dma_arbitration |
            AM_ETH_DMABMR_USP); /* Enable use of separate PBL for Rx and Tx */
    tmp_reg = ETH->DMABMR;
    __AM_ETH_DELAY__(AM_ETH_REG_WRITE_DELAY);
    ETH->DMABMR = tmp_reg;
    if (err == AM_ETH_SUCCESS) {
        return AM_ETH_SUCCESS;
    } else /* Auto-negotiation failed */
    {
        return AM_ETH_ERROR;
    }
}

/**
 * @brief  Enables ENET MAC and DMA reception/transmission
 * @param  None
 * @retval None
 */
void am_eth_start(void)
{
    __am_eth_mac_transmission_cmd(ENABLE);
    __am_eth_mac_reception_cmd(ENABLE);
    __am_eth_flush_transmit_fifo();
    __am_eth_dma_transmission_cmd(ENABLE);
    __am_eth_dma_reception_cmd(ENABLE);
}

/**
 * @brief  Disables ENET MAC and DMA reception/transmission
 * @param  None
 * @retval None
 */
void am_eth_stop(void)
{
    __am_eth_dma_transmission_cmd(DISABLE);
    __am_eth_dma_reception_cmd(DISABLE);
    __am_eth_mac_reception_cmd(DISABLE);
    __am_eth_flush_transmit_fifo();
    __am_eth_mac_transmission_cmd(DISABLE);
}

/**
 * @brief  Enables or disables the MAC transmission.
 * @param  new_state: new state of the MAC transmission.
 *   This parameter can be: ENABLE or DISABLE.
 * @retval None
 */
void __am_eth_mac_transmission_cmd(func_state_t new_state)
{
    __IO uint32_t tmp_reg = 0;
    __assert_param(IS_FUNCTIONAL_STATE(new_state));

    if (new_state != DISABLE) {
        ETH->MACCR |= AM_ETH_MACCR_TE;
    } else {
        ETH->MACCR &= ~AM_ETH_MACCR_TE;
    }
    tmp_reg = ETH->MACCR;
    __AM_ETH_DELAY__(AM_ETH_REG_WRITE_DELAY);
    ETH->MACCR = tmp_reg;
}

/**
 * @brief  Enables or disables the MAC reception.
 * @param  new_state: new state of the MAC reception.
 *   This parameter can be: ENABLE or DISABLE.
 * @retval None
 */
void __am_eth_mac_reception_cmd(func_state_t new_state)
{
    __IO uint32_t tmp_reg = 0;
    __assert_param(IS_FUNCTIONAL_STATE(new_state));

    if (new_state != DISABLE) {
        ETH->MACCR |= AM_ETH_MACCR_RE;
    } else {
        ETH->MACCR &= ~AM_ETH_MACCR_RE;
    }
    tmp_reg = ETH->MACCR;
    __AM_ETH_DELAY__(AM_ETH_REG_WRITE_DELAY);
    ETH->MACCR = tmp_reg;
}

/**
 * @brief  Configures the selected MAC address.
 * @param  mac_addr: The MAC address to configure.
 *   This parameter can be one of the following values:
 *     @arg AM_ETH_MAC_Address0 : MAC Address0
 *     @arg AM_ETH_MAC_Address1 : MAC Address1
 *     @arg AM_ETH_MAC_Address2 : MAC Address2
 *     @arg AM_ETH_MAC_Address3 : MAC Address3
 * @param  addr: Pointer on MAC address buffer data (6 bytes).
 * @retval None
 */
void __am_eth_mac_addr_config(uint32_t mac_addr, uint8_t *addr)
{
    uint32_t tmp_reg;
    __assert_param(IS_ETH_MAC_ADDRESS0123(mac_addr));
    tmp_reg = ((uint32_t) addr[5] << 8) | (uint32_t) addr[4];
    (*(__IO uint32_t *) (AM_ETH_MAC_ADDR_HBASE + mac_addr)) = tmp_reg;
    tmp_reg = ((uint32_t) addr[3] << 24) | ((uint32_t) addr[2] << 16)
            | ((uint32_t) addr[1] << 8) | addr[0];
    (*(__IO uint32_t *) (AM_ETH_MAC_ADDR_LBASE + mac_addr)) = tmp_reg;
}

/******************************************************************************/
/*                           DMA Descriptors functions                        */
/******************************************************************************/
/**
 * @brief  This function should be called to get the received frame (to be used
 *   with polling method only).
 * @param  none
 * @retval Structure of type frame_type_def_t
 */
frame_type_def_t am_eth_get_rec_frame(void)
{
    uint32_t frame_length = 0;
    frame_type_def_t frame = { 0, 0, 0 };
    frame_length = ((g_dma_rx_desc_to_get->status & AM_ETH_DMA_RX_DESC_FL)
            >> AM_ETH_DMA_RX_DESC_FRAME_LEN_SHIFT) - 4;
    frame.length = frame_length;
    frame.descriptor = gp_rx_frame_desc->fs_rx_desc;
    frame.buffer = (gp_rx_frame_desc->fs_rx_desc)->buff_1_addr;
    g_dma_rx_desc_to_get =
            (eth_dma_desc_type_t*) (g_dma_rx_desc_to_get->buff_2_next_desc_addr);
    return (frame);
}

/**
 * @brief  Prepares DMA Tx descriptors to transmit an ethernet frame
 * @param  frame_length : length of the frame to send
 * @retval error status
 */
uint32_t am_eth_prepare_transmit_desc(u16 frame_length)
{
    uint32_t buf_count = 0, size = 0, i = 0;
    __IO eth_dma_desc_type_t *dma_tx_desc;
    if ((g_dma_tx_desc_to_set->status & AM_ETH_DMA_TX_DESC_OWN)
            != (u32) RESET) {
        return AM_ETH_ERROR;
    }
    dma_tx_desc = g_dma_tx_desc_to_set;
    if (frame_length > AM_ETH_TX_BUF_SIZE) {
        buf_count = frame_length / AM_ETH_TX_BUF_SIZE;
        if (frame_length % AM_ETH_TX_BUF_SIZE)
            buf_count++;
    } else
        buf_count = 1;

    if (buf_count == 1) {
        dma_tx_desc->ctl_buff_size |= AM_ETH_DMA_TX_DESC_FS
                | AM_ETH_ETH_DMA_TX_DESC_LS | AM_ETH_DMA_TX_DESC_IC;
        dma_tx_desc->ctl_buff_size |= (frame_length & AM_ETH_DMA_TX_DESC_TBS1);
        dma_tx_desc->status |= AM_ETH_DMA_TX_DESC_OWN;
        dma_tx_desc =
                (eth_dma_desc_type_t *) (dma_tx_desc->buff_2_next_desc_addr);
    } else {
        for (i = 0; i < buf_count; i++) {
            dma_tx_desc->ctl_buff_size &= ~(AM_ETH_DMA_TX_DESC_FS
                    | AM_ETH_ETH_DMA_TX_DESC_LS);

            if (i == 0) {
                dma_tx_desc->ctl_buff_size |= AM_ETH_DMA_TX_DESC_FS;
            }

            dma_tx_desc->ctl_buff_size |= (AM_ETH_TX_BUF_SIZE
                    & AM_ETH_DMA_TX_DESC_TBS1);

            if (i == (buf_count - 1)) {
                dma_tx_desc->ctl_buff_size |= AM_ETH_ETH_DMA_TX_DESC_LS;
                size = frame_length - (buf_count - 1) * AM_ETH_TX_BUF_SIZE;
                dma_tx_desc->ctl_buff_size |= (size & AM_ETH_DMA_TX_DESC_TBS1);
            }

            dma_tx_desc->status |= AM_ETH_DMA_TX_DESC_OWN;
            dma_tx_desc =
                    (eth_dma_desc_type_t *) (dma_tx_desc->buff_2_next_desc_addr);
        }
    }
    g_dma_tx_desc_to_set = dma_tx_desc;

    if ((ETH->DMASR & AM_ETH_DMASR_TBUS) != (u32) RESET) {
        ETH->DMASR = AM_ETH_DMASR_TBUS;
        ETH->DMATPDR = 0;
    }
    return AM_ETH_SUCCESS;
}

/**
 * @brief  Initializes the DMA Rx descriptors in chain mode.
 * @param  p_dma_rx_desc_tab: Pointer on the first Rx desc list
 * @param  rx_buff: Pointer on the first RxBuffer list
 * @param  rx_buff_cnt: Number of the used Rx desc in the list
 * @retval None
 */
void am_eth_dma_rx_desc_init(eth_dma_desc_type_t *p_dma_rx_desc_tab,
        uint8_t *rx_buff, uint32_t rx_buff_cnt)
{
    uint32_t i = 0;
    eth_dma_desc_type_t *DMARxDesc;
    AM_DBG_INFO("\r\n am_eth_dma_rx_desc_init! \r\n");
    g_dma_rx_desc_to_get = p_dma_rx_desc_tab;
    AM_DBG_INFO(
            "\r\n g_dma_rx_desc_to_get p_dma_rx_desc_tab = 0x%X , 0x%X , 0x%X, 0x%X\r\n",
            p_dma_rx_desc_tab->status, p_dma_rx_desc_tab->ctl_buff_size,
            p_dma_rx_desc_tab->buff_1_addr,
            p_dma_rx_desc_tab->buff_2_next_desc_addr);
    for (i = 0; i < rx_buff_cnt; i++) {
        DMARxDesc = p_dma_rx_desc_tab + i;
        DMARxDesc->status = AM_ETH_DMA_RX_DESC_OWN;

        DMARxDesc->ctl_buff_size = AM_ETH_DMA_RX_DESC_RCH
                | (uint32_t) AM_ETH_ETH_RX_BUF_SIZE;
        DMARxDesc->buff_1_addr =
                (uint32_t) (&rx_buff[i * AM_ETH_ETH_RX_BUF_SIZE]);

        if (i < (rx_buff_cnt - 1)) {
            DMARxDesc->buff_2_next_desc_addr = (uint32_t) (p_dma_rx_desc_tab + i
                    + 1);
        } else {
            DMARxDesc->buff_2_next_desc_addr = (uint32_t) (p_dma_rx_desc_tab);
        }
    }

    ETH->DMARDLAR = (uint32_t) p_dma_rx_desc_tab;

    gp_rx_frame_desc = &g_rx_frame_desc;
}

/**
 * @brief  This function polls for a frame reception
 * @param  None
 * @retval Returns 1 when a frame is received, 0 if none.
 */
uint32_t am_eth_check_frame_received(void)
{
    if (((g_dma_rx_desc_to_get->status & AM_ETH_DMA_RX_DESC_OWN)
            == (uint32_t) RESET)
            && ((g_dma_rx_desc_to_get->status & AM_ETH_DMA_RX_DESC_LS)
                    != (uint32_t) RESET)) {
        gp_rx_frame_desc->seg_count++;
        if (gp_rx_frame_desc->seg_count == 1) {
            gp_rx_frame_desc->fs_rx_desc = g_dma_rx_desc_to_get;
        }
        gp_rx_frame_desc->ls_rx_desc = g_dma_rx_desc_to_get;
        return 1;
    } else if (((g_dma_rx_desc_to_get->status & AM_ETH_DMA_RX_DESC_OWN)
            == (uint32_t) RESET)
            && ((g_dma_rx_desc_to_get->status & AM_ETH_DMA_RX_DESC_FS)
                    != (uint32_t) RESET)
            && ((g_dma_rx_desc_to_get->status & AM_ETH_DMA_RX_DESC_LS)
                    == (uint32_t) RESET)) {
        gp_rx_frame_desc->fs_rx_desc = g_dma_rx_desc_to_get;
        gp_rx_frame_desc->ls_rx_desc = NULL;
        gp_rx_frame_desc->seg_count = 1;
        g_dma_rx_desc_to_get =
                (eth_dma_desc_type_t*) (g_dma_rx_desc_to_get->buff_2_next_desc_addr);
    } else if (((g_dma_rx_desc_to_get->status & AM_ETH_DMA_RX_DESC_OWN)
            == (uint32_t) RESET)
            && ((g_dma_rx_desc_to_get->status & AM_ETH_DMA_RX_DESC_FS)
                    == (uint32_t) RESET)
            && ((g_dma_rx_desc_to_get->status & AM_ETH_DMA_RX_DESC_LS)
                    == (uint32_t) RESET)) {
        (gp_rx_frame_desc->seg_count)++;
        g_dma_rx_desc_to_get =
                (eth_dma_desc_type_t*) (g_dma_rx_desc_to_get->buff_2_next_desc_addr);
    }
    return 0;
}

/**
 * @brief  Initializes the DMA Tx descriptors in chain mode.
 * @param  p_dma_tx_desc_tab: Pointer on the first Tx desc list
 * @param  tx_buff: Pointer on the first TxBuffer list
 * @param  tx_buff_cnt: Number of the used Tx desc in the list
 * @retval None
 */
void am_eth_dma_tx_desc_init(eth_dma_desc_type_t *p_dma_tx_desc_tab,
        uint8_t* tx_buff, uint32_t tx_buff_cnt)
{
    uint32_t i = 0;
    eth_dma_desc_type_t *dma_tx_desc;
    AM_DBG_INFO("\r\n am_eth_dma_tx_desc_init! \r\n");
    g_dma_tx_desc_to_set = p_dma_tx_desc_tab;
    AM_DBG_INFO(
            "\r\n g_dma_tx_desc_to_set p_dma_tx_desc_tab = 0x%X , 0x%X , 0x%X, 0x%X\r\n",
            p_dma_tx_desc_tab->status, p_dma_tx_desc_tab->ctl_buff_size,
            p_dma_tx_desc_tab->buff_1_addr,
            p_dma_tx_desc_tab->buff_2_next_desc_addr);
    for (i = 0; i < tx_buff_cnt; i++) {
        dma_tx_desc = p_dma_tx_desc_tab + i;
        dma_tx_desc->ctl_buff_size = AM_ETH_DMA_TX_DESC_TCH; // add frame length

        dma_tx_desc->buff_1_addr =
                (uint32_t) (&tx_buff[i * AM_ETH_TX_BUF_SIZE]);
        if (i < (tx_buff_cnt - 1)) {
            dma_tx_desc->buff_2_next_desc_addr = (uint32_t) (p_dma_tx_desc_tab
                    + i + 1);
        } else {
            dma_tx_desc->buff_2_next_desc_addr = (uint32_t) p_dma_tx_desc_tab;
        }
    }
    ETH->DMATDLAR = (uint32_t) p_dma_tx_desc_tab;
}

/******************************************************************************/
/*                           DMA functions                                    */
/******************************************************************************/
/**
 * @brief  Resets all MAC subsystem internal registers and logic.
 * @param  None
 * @retval None
 */
void __am_eth_soft_reset(void)
{
    ETH->DMABMR |= AM_ETH_DMABMR_SR;
}

/**
 * @brief  Checks whether the ETHERNET software reset bit is set or not.
 * @param  None
 * @retval The new state of DMA Bus Mode register SR bit (SET or RESET).
 */
flag_status __am_eth_get_soft_reset_status(void)
{
    flag_status bitstatus = RESET;
    if ((ETH->DMABMR & AM_ETH_DMABMR_SR) != (uint32_t) RESET) {
        bitstatus = SET;
    } else {
        bitstatus = RESET;
    }
    return bitstatus;
}

/**
 * @brief  Clears the ETHERNET transmit FIFO.
 * @param  None
 * @retval None
 */
void __am_eth_flush_transmit_fifo(void)
{
    __IO uint32_t tmp_reg = 0;
    ETH->DMAOMR |= AM_ETH_DMAOMR_FTF;
    tmp_reg = ETH->DMAOMR;
    __AM_ETH_DELAY__(AM_ETH_REG_WRITE_DELAY);
    ETH->DMAOMR = tmp_reg;
}

/**
 * @brief  Enables or disables the DMA transmission.
 * @param  new_state: new state of the DMA transmission.
 *   This parameter can be: ENABLE or DISABLE.
 * @retval None
 */
void __am_eth_dma_transmission_cmd(func_state_t new_state)
{
    __assert_param(IS_FUNCTIONAL_STATE(new_state));

    if (new_state != DISABLE) {
        ETH->DMAOMR |= AM_ETH_DMAOMR_ST;
    } else {
        ETH->DMAOMR &= ~AM_ETH_DMAOMR_ST;
    }
}

/**
 * @brief  Enables or disables the DMA reception.
 * @param  new_state: new state of the DMA reception.
 *   This parameter can be: ENABLE or DISABLE.
 * @retval None
 */
void __am_eth_dma_reception_cmd(func_state_t new_state)
{
    __assert_param(IS_FUNCTIONAL_STATE(new_state));

    if (new_state != DISABLE) {
        ETH->DMAOMR |= AM_ETH_DMAOMR_SR;
    } else {
        ETH->DMAOMR &= ~AM_ETH_DMAOMR_SR;
    }
}
/******************************************************************************/
/*                                PHY functions                               */
/******************************************************************************/

/**
 * @brief  Read a PHY register
 * @param PHYAddress: PHY device address, is the index of one of supported 32 PHY devices.
 *   This parameter can be one of the following values: 0,..,31
 * @param phy_reg: PHY register address, is the index of one of the 32 PHY register.
 *   This parameter can be one of the following values:
 *     @arg AM_PHY_BCR: Transceiver Basic Control Register
 *     @arg AM_PHY_BSR: Transceiver Basic status Register
 *     @arg AM_PHY_SR : Transceiver status Register
 *     @arg More PHY register could be read depending on the used PHY
 * @retval AM_ETH_ERROR: in case of timeout
 *         MAC MIIDR register value: Data read from the selected PHY register (correct read )
 */
uint16_t __am_eth_read_phy_reg(uint16_t phy_addr, uint16_t phy_reg)
{
    uint32_t tmp_reg = 0;
    __IO uint32_t timeout = 0;
    __assert_param(IS_ETH_PHY_ADDRESS(phy_addr));
    __assert_param(IS_ETH_PHY_REG(phy_reg));

    tmp_reg = ETH->MACMIIAR;
    tmp_reg &= ~AM_MACMIIAR_CR_MASK;
    tmp_reg |= (((uint32_t) phy_addr << 11) & AM_ETH_MACMIIAR_PA);/* Set the PHY device address */
    tmp_reg |= (((uint32_t) phy_reg << 6) & AM_ETH_MACMIIAR_MR); /* Set the PHY register address */
    tmp_reg &= ~AM_ETH_MACMIIAR_MW; /* Set the read mode */
    tmp_reg |= AM_ETH_MACMIIAR_MB; /* Set the MII Busy bit */
    ETH->MACMIIAR = tmp_reg;
    do {
        timeout++;
        tmp_reg = ETH->MACMIIAR;
    } while ((tmp_reg & AM_ETH_MACMIIAR_MB)
            && (timeout < (uint32_t) AM_PHY_READ_TO));
    if (timeout == AM_PHY_READ_TO) {
        return (uint16_t) AM_ETH_ERROR;
    }
    return (uint16_t) (ETH->MACMIIDR);
}

/**
 * @brief  Write to a PHY register
 * @param PHYAddress: PHY device address, is the index of one of supported 32 PHY devices.
 *   This parameter can be one of the following values: 0,..,31
 * @param phy_reg: PHY register address, is the index of one of the 32 PHY register.
 *   This parameter can be one of the following values:
 *     @arg AM_PHY_BCR    : Transceiver Control Register
 *     @arg More PHY register could be written depending on the used PHY
 * @param  PHYValue: the value to write
 * @retval AM_ETH_ERROR: in case of timeout
 *         AM_ETH_SUCCESS: for correct write
 */
uint32_t __am_eth_write_phy_reg(uint16_t phy_addr, uint16_t phy_reg,
        uint16_t PHYValue)
{
    uint32_t tmp_reg = 0;
    __IO uint32_t timeout = 0;
    /* Check the parameters */
    __assert_param(IS_ETH_PHY_ADDRESS(phy_addr));
    __assert_param(IS_ETH_PHY_REG(phy_reg));

    tmp_reg = ETH->MACMIIAR;
    tmp_reg &= ~AM_MACMIIAR_CR_MASK;
    tmp_reg |= (((uint32_t) phy_addr << 11) & AM_ETH_MACMIIAR_PA); /* Set the PHY device address */
    tmp_reg |= (((uint32_t) phy_reg << 6) & AM_ETH_MACMIIAR_MR); /* Set the PHY register address */
    tmp_reg |= AM_ETH_MACMIIAR_MW; /* Set the write mode */
    tmp_reg |= AM_ETH_MACMIIAR_MB; /* Set the MII Busy bit */
    ETH->MACMIIDR = PHYValue;
    ETH->MACMIIAR = tmp_reg;
    do {
        timeout++;
        tmp_reg = ETH->MACMIIAR;
    } while ((tmp_reg & AM_ETH_MACMIIAR_MB)
            && (timeout < (uint32_t) AM_ETH_PHY_WRITE_TO));
    if (timeout == AM_ETH_PHY_WRITE_TO) {
        return AM_ETH_ERROR;
    }
    return AM_ETH_SUCCESS;
}
