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

/*******************************************************************************
  includes
*******************************************************************************/

#include "am_zlg_eth.h"
#include "am_clk.h"
#include "zmf159_clk.h"
#include "amhw_zlg_eth_reg.h"
#include "am_vdebug.h"
#include "am_zlg_phy.h"
#include "netif.h"
#include "eth_conf.h"
#include "lwip/dhcp.h"
#include "zmf159_pin.h"
#include "am_gpio.h"
#include "am_delay.h"
#include "amhw_zlg_eth.h"
#include "amhw_zlg_phy.h"
#include "zmf159_periph_map.h"

#if defined   (__CC_ARM)     /**< \brief ARM Compiler */
__align(4)
eth_dma_desc_type_t g_dma_rx_desc_tab[AM_ZLG_ETH_RXBUFNB];
__align(4)
eth_dma_desc_type_t g_dma_tx_desc_tab[AM_ZLG_ETH_TXBUFNB];
__align(4)
uint8_t g_rx_buff[AM_ZLG_ETH_RXBUFNB][AM_ZLG_ETH_ETH_RX_BUF_SIZE];
__align(4)
uint8_t g_tx_buff[AM_ZLG_ETH_TXBUFNB][AM_ZLG_ETH_TX_BUF_SIZE];

#elif defined ( __ICCARM__ ) /**< \brief  IAR Compiler */
#pragma data_alignment=4
eth_dma_desc_type_t g_dma_rx_desc_tab[AM_ZLG_ETH_RXBUFNB];
#pragma data_alignment=4
eth_dma_desc_type_t g_dma_tx_desc_tab[AM_ZLG_ETH_TXBUFNB];
#pragma data_alignment=4
uint8_t g_rx_buff[AM_ZLG_ETH_RXBUFNB][AM_ZLG_ETH_ETH_RX_BUF_SIZE];
#pragma data_alignment=4
uint8_t g_tx_buff[AM_ZLG_ETH_TXBUFNB][AM_ZLG_ETH_TX_BUF_SIZE];

#elif defined (__GNUC__)     /**< \brief  GNU Compiler */
eth_dma_desc_type_t g_dma_rx_desc_tab[AM_ZLG_ETH_RXBUFNB] 
        __attribute__ ((aligned (4))); 
eth_dma_desc_type_t g_dma_tx_desc_tab[AM_ZLG_ETH_TXBUFNB]
        __attribute__ ((aligned (4)));
uint8_t g_rx_buff[AM_ZLG_ETH_RXBUFNB][AM_ZLG_ETH_ETH_RX_BUF_SIZE] 
        __attribute__ ((aligned (4)));
uint8_t g_tx_buff[AM_ZLG_ETH_TXBUFNB][AM_ZLG_ETH_TX_BUF_SIZE] 
        __attribute__ ((aligned (4)));

#elif defined  (__TASKING__) /**< \brief  TASKING Compiler */
__align(4)
eth_dma_desc_type_t g_dma_rx_desc_tab[AM_ZLG_ETH_RXBUFNB];
__align(4)
eth_dma_desc_type_t g_dma_tx_desc_tab[AM_ZLG_ETH_TXBUFNB];
__align(4)
uint8_t g_rx_buff[AM_ZLG_ETH_RXBUFNB][AM_ZLG_ETH_ETH_RX_BUF_SIZE]; 
__align(4)
uint8_t g_tx_buff[AM_ZLG_ETH_TXBUFNB][AM_ZLG_ETH_TX_BUF_SIZE];
#endif 

/** \brief public variable declaration */
am_eth_regs_conf g_eth_regs_config;
__IO uint32_t g_eth_status = 0;
__IO am_zlg_eth_link_st_t g_eth_link_status = AM_ZLG_ETH_LINK_ST_UP;

__IO eth_dma_desc_type_t *g_dma_tx_desc_to_set;
__IO eth_dma_desc_type_t *g_dma_rx_desc_to_get;

/** \brief Structure used to hold the last received packet descriptors info */
eth_dma_rx_frame_info_t g_rx_frame_desc;
__IO eth_dma_rx_frame_info_t *gp_rx_frame_desc;
__IO uint32_t g_frame_rx_index;

/** \brief extern variable declaration */
extern struct netif g_eth_netif;
amhw_zlg_eth_t *gp_hw_eth = NULL;

#ifdef USE_DHCP
extern __IO uint8_t am_eth_dhcp_state;
#endif

static void am_eth_check_usr_config(am_eth_regs_conf *p_eth_regs_config)
{
    /** \brief  MAC */
    assert_param(
IS_AM_ZLG_ETH_AUTO_NEGOTIATION(p_eth_regs_config->eth_auto_negotiation));
    assert_param(IS_AM_ZLG_ETH_WATCH_DOG(p_eth_regs_config->eth_watch_dog));
    assert_param(IS_AM_ZLG_ETH_JABBER(p_eth_regs_config->eth_jabber));
    assert_param(
IS_AM_ZLG_ETH_INTER_FRAME_GAP(p_eth_regs_config->eth_inter_fame_gap));
    assert_param(IS_AM_ZLG_ETH_SPEED(p_eth_regs_config->eth_speed));
    assert_param(
IS_AM_ZLG_ETH_RECEIVE_OWN(p_eth_regs_config->eth_receive_own));
    assert_param(
IS_AM_ZLG_ETH_LOOPBACK_MODE(p_eth_regs_config->eth_loop_back_mode));
    assert_param(IS_AM_ZLG_ETH_DUPLEX_MODE(p_eth_regs_config->eth_mode));
    assert_param(
IS_AM_ZLG_ETH_CHECKSUM_OFFLOAD(p_eth_regs_config->eth_checksum_off_load));
    assert_param(
IS_AM_ZLG_ETH_RETRY_TRANSMISSION(p_eth_regs_config->eth_retry_transmission));
    assert_param(
IS_AM_ZLG_ETH_AUTOMATIC_PADCRC_STRIP(p_eth_regs_config->eth_automatic_pad_crc_strip));
    assert_param(
IS_AM_ZLG_ETH_BACK_OFF_LIMIT(p_eth_regs_config->eth_back_off_limit));
    assert_param(
IS_AM_ZLG_ETH_DEFERRAL_CHECK(p_eth_regs_config->eth_deferal_check));
    assert_param(
IS_AM_ZLG_ETH_RECEIVE_ALL(p_eth_regs_config->eth_receive_all));
    assert_param(
IS_AM_ZLG_ETH_SOURCE_ADDR_FILTER(p_eth_regs_config->eth_source_addr_filter));
    assert_param(
IS_AM_ZLG_ETH_CONTROL_FRAMES(p_eth_regs_config->eth_pass_ctl_frames));
    assert_param(
IS_AM_ZLG_ETH_BROADCAST_FRAMES_RECEPTION(p_eth_regs_config->eth_broadcast_frames_reception));
    assert_param(
IS_AM_ZLG_ETH_DESTINATION_ADDR_FILTER(p_eth_regs_config->eth_destination_addr_filter));
    assert_param(
IS_AM_ZLG_ETH_PROMISCUOUS_MODE(p_eth_regs_config->eth_promiscuous_mode));
    assert_param(
IS_AM_ZLG_ETH_MULTICAST_FRAMES_FILTER(p_eth_regs_config->eth_multicast_frames_filter));
    assert_param(
IS_AM_ZLG_ETH_UNICAST_FRAMES_FILTER(p_eth_regs_config->eth_unicast_frames_filter));
    assert_param(IS_AM_ZLG_ETH_PAUSE_TIME(p_eth_regs_config->eth_pause_time));
    assert_param(
IS_AM_ZLG_ETH_ZERO_QUANTA_PAUSE(p_eth_regs_config->eth_zero_quanta_pause));
    assert_param(
IS_AM_ZLG_ETH_PAUSE_LOW_THRESHOLD(p_eth_regs_config->eth_pause_low_threshold));
    assert_param(
IS_AM_ZLG_ETH_UNICAST_PAUSE_FRAME_DETECT(p_eth_regs_config->eth_unicast_pause_frame_detect));
    assert_param(
IS_AM_ZLG_ETH_RECEIVE_FLOW_CRL(p_eth_regs_config->eth_receive_flow_ctl));
    assert_param(
IS_AM_ZLG_ETH_TRANSMIT_FLOW_CRL(p_eth_regs_config->eth_tramsmit_flow_ctl));
    assert_param(
IS_AM_ZLG_ETH_VLAN_TAG_COMPARISON(p_eth_regs_config->eth_vlan_tag_comparison));
    assert_param(
IS_AM_ZLG_ETH_VLAN_TAG_IDENTIFIER(p_eth_regs_config->eth_vlan_tag_identifier));

    /** \brief  DMA */
    assert_param(
IS_AM_ZLG_ETH_TRANSMIT_STORE_FORWARD(p_eth_regs_config->eth_transmit_store_forward));
    assert_param(
IS_AM_ZLG_ETH_TRANSMIT_THRESHOLD_CTL(p_eth_regs_config->eth_transmit_threshold_ctl));
    assert_param(
IS_AM_ZLG_ETH_FORWARD_ERROR_FRAMES(p_eth_regs_config->eth_forward_error_frames));
    assert_param(
IS_AM_ZLG_ETH_FORWARD_UNDERSIZED_GOOD_FRAMES(p_eth_regs_config->eth_forward_under_size_good_frames));
    assert_param(
IS_AM_ZLG_ETH_RECEIVE_THRESHOLD_CTL(p_eth_regs_config->eth_receive_threshold_ctl));
    assert_param(
IS_AM_ZLG_ETH_SEC_FRAME_OPERATE(p_eth_regs_config->eth_sec_frame_operate));
    assert_param(
IS_AM_ZLG_ETH_FIXED_BURST(p_eth_regs_config->eth_fixed_burst));
    assert_param(
IS_AM_ZLG_ETH_TX_DMA_BURST_LENGTH(p_eth_regs_config->eth_dma_burst_len));
    assert_param(
IS_AM_ZLG_ETH_DMA_DESC_SKIP_LENGTH(p_eth_regs_config->eth_desc_skip_len));
    assert_param(
IS_AM_ZLG_ETH_DMA_ARBITRATION_ROUNDROBIN_RXTX(p_eth_regs_config->eth_dma_arbitration));

}

/**
 * \brief  Enables ENET MAC and DMA reception/transmission
 * \param  None
 * \retval None
 */
void am_zlg_eth_start(amhw_zlg_eth_t *p_hw_eth)
{
    amhw_zlg_eth_mac_transmission_cmd(p_hw_eth, ENABLE);
    amhw_zlg_eth_mac_reception_cmd(p_hw_eth, ENABLE);
    amhw_zlg_eth_flush_transmit_fifo(p_hw_eth);
    amhw_zlg_eth_dma_transmission_cmd(p_hw_eth, ENABLE);
    amhw_zlg_eth_dma_reception_cmd(p_hw_eth, ENABLE);
}

/**
 * \brief  Disables ENET MAC and DMA reception/transmission
 * \param  None
 * \retval None
 */
void am_zlg_eth_stop(amhw_zlg_eth_t *p_hw_eth)
{
    amhw_zlg_eth_dma_transmission_cmd(p_hw_eth, DISABLE);
    amhw_zlg_eth_dma_reception_cmd(p_hw_eth, DISABLE);
    amhw_zlg_eth_mac_reception_cmd(p_hw_eth, DISABLE);
    amhw_zlg_eth_flush_transmit_fifo(p_hw_eth);
    amhw_zlg_eth_mac_transmission_cmd(p_hw_eth, DISABLE);
}

/**
 * \brief  This function should be called to get the received frame (to be used
 *   with polling method only).
 * \param  none
 * \retval Structure of type frame_type_def_t
 */
frame_type_def_t am_zlg_eth_get_rec_frame(void)
{
    uint32_t frame_length = 0;
    frame_type_def_t frame = { 0, 0, 0 };
    frame_length = ((g_dma_rx_desc_to_get->status & AM_ZLG_ETH_DMA_RX_DESC_FL)
            >> AM_ZLG_ETH_DMA_RX_DESC_FRAME_LEN_SHIFT) - 4;
    frame.length = frame_length;
    frame.descriptor = gp_rx_frame_desc->fs_rx_desc;
    frame.buffer = (gp_rx_frame_desc->fs_rx_desc)->buff_1_addr;
    g_dma_rx_desc_to_get =
            (eth_dma_desc_type_t*) (g_dma_rx_desc_to_get->buff_2_next_desc_addr);
    return (frame);
}

/**
 * \brief  Prepares DMA Tx descriptors to transmit an ethernet frame
 * \param  frame_length : length of the frame to send
 * \retval error status
 */
uint32_t am_zlg_eth_prepare_transmit_desc(amhw_zlg_eth_t *p_hw_eth, uint16_t frame_length)
{
    uint32_t buf_count = 0, size = 0, i = 0;
    __IO eth_dma_desc_type_t *dma_tx_desc;
    if ((g_dma_tx_desc_to_set->status & AM_ZLG_ETH_DMA_TX_DESC_OWN)
            != (uint32_t) RESET) {
        return AM_ZLG_ETH_ERROR;
    }
    dma_tx_desc = g_dma_tx_desc_to_set;
    if (frame_length > AM_ZLG_ETH_TX_BUF_SIZE) {
        buf_count = frame_length / AM_ZLG_ETH_TX_BUF_SIZE;
        if (frame_length % AM_ZLG_ETH_TX_BUF_SIZE)
            buf_count++;
    } else
        buf_count = 1;

    if (buf_count == 1) {
        dma_tx_desc->ctl_buff_size |= AM_ZLG_ETH_DMA_TX_DESC_FS
                | AM_ZLG_ETH_ETH_DMA_TX_DESC_LS | AM_ZLG_ETH_DMA_TX_DESC_IC;
        dma_tx_desc->ctl_buff_size |= (frame_length
                & AM_ZLG_ETH_DMA_TX_DESC_TBS1);
        dma_tx_desc->status |= AM_ZLG_ETH_DMA_TX_DESC_OWN;
        dma_tx_desc =
                (eth_dma_desc_type_t *) (dma_tx_desc->buff_2_next_desc_addr);
    } else {
        for (i = 0; i < buf_count; i++) {
            dma_tx_desc->ctl_buff_size &= ~(AM_ZLG_ETH_DMA_TX_DESC_FS
                    | AM_ZLG_ETH_ETH_DMA_TX_DESC_LS);

            if (i == 0) {
                dma_tx_desc->ctl_buff_size |= AM_ZLG_ETH_DMA_TX_DESC_FS;
            }

            dma_tx_desc->ctl_buff_size |= (AM_ZLG_ETH_TX_BUF_SIZE
                    & AM_ZLG_ETH_DMA_TX_DESC_TBS1);

            if (i == (buf_count - 1)) {
                dma_tx_desc->ctl_buff_size |= AM_ZLG_ETH_ETH_DMA_TX_DESC_LS;
                size = frame_length - (buf_count - 1) * AM_ZLG_ETH_TX_BUF_SIZE;
                dma_tx_desc->ctl_buff_size |= (size
                        & AM_ZLG_ETH_DMA_TX_DESC_TBS1);
            }

            dma_tx_desc->status |= AM_ZLG_ETH_DMA_TX_DESC_OWN;
            dma_tx_desc =
                    (eth_dma_desc_type_t *) (dma_tx_desc->buff_2_next_desc_addr);
        }
    }
    g_dma_tx_desc_to_set = dma_tx_desc;

    if ((p_hw_eth->DMASR & AM_ZLG_ETH_DMASR_TBUS) != (uint32_t) RESET) {
        p_hw_eth->DMASR = AM_ZLG_ETH_DMASR_TBUS;
        p_hw_eth->DMATPDR = 0;
    }
    return AM_ZLG_ETH_SUCCESS;
}

/**
 * \brief  Initializes the DMA Rx descriptors in chain mode.
 * \param  p_dma_rx_desc_tab: Pointer on the first Rx desc list
 * \param  rx_buff: Pointer on the first RxBuffer list
 * \param  rx_buff_cnt: Number of the used Rx desc in the list
 * \retval None
 */
void am_zlg_eth_dma_rx_desc_init(amhw_zlg_eth_t *p_hw_eth, eth_dma_desc_type_t *p_dma_rx_desc_tab,
        uint8_t *rx_buff, uint32_t rx_buff_cnt)
{
    uint32_t i = 0;
    eth_dma_desc_type_t *DMARxDesc;
    g_dma_rx_desc_to_get = p_dma_rx_desc_tab;
    for (i = 0; i < rx_buff_cnt; i++) {
        DMARxDesc = p_dma_rx_desc_tab + i;
        DMARxDesc->status = AM_ZLG_ETH_DMA_RX_DESC_OWN;

        DMARxDesc->ctl_buff_size = AM_ZLG_ETH_DMA_RX_DESC_RCH
                | (uint32_t) AM_ZLG_ETH_ETH_RX_BUF_SIZE;
        DMARxDesc->buff_1_addr = (uint32_t) (&rx_buff[i
                * AM_ZLG_ETH_ETH_RX_BUF_SIZE]);

        if (i < (rx_buff_cnt - 1)) {
            DMARxDesc->buff_2_next_desc_addr = (uint32_t) (p_dma_rx_desc_tab + i
                    + 1);
        } else {
            DMARxDesc->buff_2_next_desc_addr = (uint32_t) (p_dma_rx_desc_tab);
        }
    }

    p_hw_eth->DMARDLAR = (uint32_t) p_dma_rx_desc_tab;

    gp_rx_frame_desc = &g_rx_frame_desc;
}

/**
 * \brief  This function polls for a frame reception
 * \param  None
 * \retval Returns 1 when a frame is received, 0 if none.
 */
uint32_t am_zlg_eth_check_frame_received(void)
{
    if (((g_dma_rx_desc_to_get->status & AM_ZLG_ETH_DMA_RX_DESC_OWN)
            == (uint32_t) RESET)
            && ((g_dma_rx_desc_to_get->status & AM_ZLG_ETH_DMA_RX_DESC_LS)
                    != (uint32_t) RESET)) {
        gp_rx_frame_desc->seg_count++;
        if (gp_rx_frame_desc->seg_count == 1) {
            gp_rx_frame_desc->fs_rx_desc = g_dma_rx_desc_to_get;
        }
        gp_rx_frame_desc->ls_rx_desc = g_dma_rx_desc_to_get;
        return 1;
    } else if (((g_dma_rx_desc_to_get->status & AM_ZLG_ETH_DMA_RX_DESC_OWN)
            == (uint32_t) RESET)
            && ((g_dma_rx_desc_to_get->status & AM_ZLG_ETH_DMA_RX_DESC_FS)
                    != (uint32_t) RESET)
            && ((g_dma_rx_desc_to_get->status & AM_ZLG_ETH_DMA_RX_DESC_LS)
                    == (uint32_t) RESET)) {
        gp_rx_frame_desc->fs_rx_desc = g_dma_rx_desc_to_get;
        gp_rx_frame_desc->ls_rx_desc = NULL;
        gp_rx_frame_desc->seg_count = 1;
        g_dma_rx_desc_to_get =
                (eth_dma_desc_type_t*) (g_dma_rx_desc_to_get->buff_2_next_desc_addr);
    } else if (((g_dma_rx_desc_to_get->status & AM_ZLG_ETH_DMA_RX_DESC_OWN)
            == (uint32_t) RESET)
            && ((g_dma_rx_desc_to_get->status & AM_ZLG_ETH_DMA_RX_DESC_FS)
                    == (uint32_t) RESET)
            && ((g_dma_rx_desc_to_get->status & AM_ZLG_ETH_DMA_RX_DESC_LS)
                    == (uint32_t) RESET)) {
        (gp_rx_frame_desc->seg_count)++;
        g_dma_rx_desc_to_get =
                (eth_dma_desc_type_t*) (g_dma_rx_desc_to_get->buff_2_next_desc_addr);
    }
    return 0;
}

/**
 * \brief  Initializes the DMA Tx descriptors in chain mode.
 * \param  p_dma_tx_desc_tab: Pointer on the first Tx desc list
 * \param  tx_buff: Pointer on the first TxBuffer list
 * \param  tx_buff_cnt: Number of the used Tx desc in the list
 * \retval None
 */
void am_zlg_eth_dma_tx_desc_init(amhw_zlg_eth_t *p_hw_eth, eth_dma_desc_type_t *p_dma_tx_desc_tab,
        uint8_t* tx_buff, uint32_t tx_buff_cnt)
{
    uint32_t i = 0;
    eth_dma_desc_type_t *dma_tx_desc;
    g_dma_tx_desc_to_set = p_dma_tx_desc_tab;
    for (i = 0; i < tx_buff_cnt; i++) {
        dma_tx_desc = p_dma_tx_desc_tab + i;
        dma_tx_desc->ctl_buff_size = AM_ZLG_ETH_DMA_TX_DESC_TCH;

        dma_tx_desc->buff_1_addr = (uint32_t) (&tx_buff[i
                * AM_ZLG_ETH_TX_BUF_SIZE]);
        if (i < (tx_buff_cnt - 1)) {
            dma_tx_desc->buff_2_next_desc_addr = (uint32_t) (p_dma_tx_desc_tab
                    + i + 1);
        } else {
            dma_tx_desc->buff_2_next_desc_addr = (uint32_t) p_dma_tx_desc_tab;
        }
    }
    p_hw_eth->DMATDLAR = (uint32_t) p_dma_tx_desc_tab;
}

/**
 * \brief  Configures the Ethernet Interface
 * \param  None
 * \retval None
 */
static void am_eth_mac_dma_init(amhw_zlg_eth_t *p_hw_eth)
{
    AM_DBG_INFO("\r\n am_eth_mac_dma_config! \r\n");

    amhw_zlg_eth_soft_reset(p_hw_eth);
    while (amhw_zlg_eth_get_soft_reset_status(p_hw_eth) == SET)
        ;
    am_eth_check_usr_config(&g_eth_regs_config);
    g_eth_status = amhw_zlg_eth_reg_init(p_hw_eth, &g_eth_regs_config,
            ETHERNET_PHY_ADDRESS);
}

/**
 * \brief  am_zlg_eth_bsp_config
 * \param  None
 * \retval None
 */
void am_zlg_eth_init(amhw_zlg_eth_t *p_hw_eth)
{
    am_eth_mac_dma_init(p_hw_eth);
    am_zlg_eth_lwip_init();
    if (AMHW_ZLG_ETH_GET_PHY_LINK_STATUS(p_hw_eth)) {
        g_eth_status |= AM_ZLG_ETH_LINK_FLAG;
    }
}

/** 
 * \brief  This function is called periodically each second
 * \param  phy addr
 * \retval None
 */
void am_zlg_eth_check_link_status(amhw_zlg_eth_t *p_hw_eth, uint16_t phy_addr)
{
    static uint8_t status = 0;
    uint32_t tmp = AMHW_ZLG_ETH_GET_PHY_LINK_STATUS(p_hw_eth);

    if (tmp && !status) {
        netif_set_link_up(&g_eth_netif);
        status = 1;
    }
    if (!tmp && status) {
        g_eth_link_status = AM_ZLG_ETH_LINK_ST_DOWN;
        netif_set_link_down(&g_eth_netif);
        status = 0;
    }
}

/**
 * \brief  Link callback function, this function is called on change of link status.
 * \param  The network interface
 * \retval None
 */
void am_zlg_eth_link_callback(struct netif *netif)
{
    AM_DBG_INFO("\r\n am_zlg_eth_link_callback! \r\n");
    __IO uint32_t timeout = 0;
    uint32_t reg_value;
    amhw_zlg_eth_speed_t eth_speed;
    amhw_zlg_eth_mode_t eth_mode;
    struct ip_addr ipaddr;
    struct ip_addr netmask;
    struct ip_addr gw;
    amhw_zlg_eth_t *p_hw_eth = gp_hw_eth;

    if (netif_is_link_up(netif)) {
        if (g_eth_regs_config.eth_auto_negotiation
                != AM_ZLG_ETH_AUTO_NEGOTIATION_DISABLE) {
            timeout = 0;
            am_zlg_phy_write_reg(p_hw_eth, ETHERNET_PHY_ADDRESS, AM_ZLG_PHY_BCR,
            AM_ZLG_PHY_AUTO_NEGOTIATION);
            do {
                timeout++;
            } while (!(am_zlg_phy_read_reg(p_hw_eth, ETHERNET_PHY_ADDRESS,
                    AM_ZLG_PHY_BSR) & AM_ZLG_PHY_AUTO_NEGO_COMPLETE)
                    && (timeout < (uint32_t) AM_ZLG_PHY_READ_TO));
            timeout = 0;
            reg_value = am_zlg_phy_read_reg(p_hw_eth, ETHERNET_PHY_ADDRESS,
                    AM_ZLG_PHY_SR);
            if ((reg_value & AM_ZLG_PHY_DUPLEX_STATUS) != (uint16_t) RESET) {
                eth_mode = AM_ZLG_ETH_MODE_FULLDUPLEX;
            } else {
                eth_mode = AM_ZLG_ETH_MODE_HALFDUPLEX;
            }
            if (reg_value & AM_ZLG_PHY_SPEED_STATUS) {
                eth_speed = AM_ZLG_ETH_SPEED_10M;
            } else {
                eth_speed = AM_ZLG_ETH_SPEED_100M;
            }
            amhw_zlg_eth_set_speed(p_hw_eth, eth_speed);
            amhw_zlg_eth_set_mode(p_hw_eth, eth_mode);
        }
        am_zlg_eth_start(p_hw_eth);

#ifdef USE_DHCP
        ipaddr.addr = 0;
        netmask.addr = 0;
        gw.addr = 0;
        am_eth_dhcp_state = AM_ZLG_ETH_DHCP_START;
#else
        IP4_ADDR(&ipaddr, g_eth_local_ip[0], g_eth_local_ip[1],
                g_eth_local_ip[2], g_eth_local_ip[3]);
        IP4_ADDR(&netmask, g_eth_mask[0], g_eth_mask[1], g_eth_mask[2],
                g_eth_mask[3]);
        IP4_ADDR(&gw, g_eth_gateway[0], g_eth_gateway[1], g_eth_gateway[2],
                g_eth_gateway[3]);
#endif 
        netif_set_addr(&g_eth_netif, &ipaddr, &netmask, &gw);
        netif_set_up(&g_eth_netif);
        g_eth_link_status = AM_ZLG_ETH_LINK_ST_UP;
    } else {
        am_zlg_eth_stop(p_hw_eth);
#ifdef USE_DHCP
        am_eth_dhcp_state = AM_ZLG_ETH_DHCP_LINK_DOWN;
        am_dhcp_stop(netif);
#endif 
        netif_set_down(&g_eth_netif);
    }
}

