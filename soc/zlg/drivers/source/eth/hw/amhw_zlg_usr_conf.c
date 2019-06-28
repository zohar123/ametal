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
 * \brief amhw_zlg_usr_conf.c
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
#include "amhw_zlg_eth_reg.h"
#include "zmf159_pin.h"
#include "am_gpio.h"
#include "zmf159_regbase.h"

/**
 * \brief  Configures the different GPIO ports.
 * \param  None
 * \retval None
 */
void amhw_zlg_eth_gpio_config(void)
{
    printf("\r\n __am_eth_gpio_config! \r\n");
    /* enable enet clk */
    AM_REG_BITS_SET32(0x40021028, 25, 4, 0xf);

    /* reset mac peripheral */
    AM_REG_BITS_SET32(0x40021014, 25, 1, 0x1);
    am_udelay(10000);
    AM_REG_BITS_SET32(0x40021014, 25, 1, 0x0);

    /* ENET_nRST */
    am_gpio_pin_cfg(PIOC_0, PIOC_0_OUT_PP | AM_GPIO_OUTPUT_INIT_HIGH);

    amhw_zlg_eth_reset_eth();

    /* ENET_MDIO */
    am_gpio_pin_cfg(PIOA_2,
            PIOA_2_ETH_MDIO | AMHW_ZLG_ETH_PIN_CFG_AF_PP(PIOA_2));

    /* ENET_MDC */
    am_gpio_pin_cfg(PIOC_1,
            PIOC_1_ETH_MDC | AMHW_ZLG_ETH_PIN_CFG_AF_PP(PIOC_1));

    /* ETH_CRS */
    am_gpio_pin_cfg(PIOA_0,
    PIOA_0_ETH_CRS | AMHW_ZLG_ETH_PIN_CFG_INPUT_FLOAT(PIOA_0));

    /* ETH_RX_CLK */
    am_gpio_pin_cfg(PIOA_1,
    PIOA_1_ETH_RX_CLK | AMHW_ZLG_ETH_PIN_CFG_INPUT_FLOAT(PIOA_1));

    /* ETH_COL */
    am_gpio_pin_cfg(PIOA_3,
    PIOA_3_ETH_COL | AMHW_ZLG_ETH_PIN_CFG_INPUT_FLOAT(PIOA_3));
    /* ETH_RX_DV */
    am_gpio_pin_cfg(PIOA_7,
    PIOA_7_ETH_RX_DV | AMHW_ZLG_ETH_PIN_CFG_INPUT_FLOAT(PIOA_7));

    /* ETH_RXD2 */
    am_gpio_pin_cfg(PIOB_0,
    PIOB_0_ETH_RXD2 | AMHW_ZLG_ETH_PIN_CFG_INPUT_FLOAT(PIOB_0));

    /* ETH_RXD3 */
    am_gpio_pin_cfg(PIOB_1,
    PIOB_1_ETH_RXD3 | AMHW_ZLG_ETH_PIN_CFG_INPUT_FLOAT(PIOB_1));

    /* ETH_TXD3 */
    am_gpio_pin_cfg(PIOE_2,
            PIOE_2_ETH_TXD3 | AMHW_ZLG_ETH_PIN_CFG_AF_PP(PIOE_2));

    /* ETH_RX_ER */
    am_gpio_pin_cfg(PIOB_10,
    PIOB_10_ETH_RX_ER | AMHW_ZLG_ETH_PIN_CFG_INPUT_FLOAT(PIOB_10));

    /* ETH_TX_EN */
    am_gpio_pin_cfg(PIOB_11,
            PIOB_11_ETH_TX_EN | AMHW_ZLG_ETH_PIN_CFG_AF_PP(PIOB_11));

    /* ETH_TXD0 */
    am_gpio_pin_cfg(PIOB_12,
            PIOB_12_ETH_TXD0 | AMHW_ZLG_ETH_PIN_CFG_AF_PP(PIOB_12));

    /* ETH_TXD1 */
    am_gpio_pin_cfg(PIOB_13,
            PIOB_13_ETH_TXD1 | AMHW_ZLG_ETH_PIN_CFG_AF_PP(PIOB_13));

    /* ETH_TXD2 */
    am_gpio_pin_cfg(PIOC_2,
            PIOC_2_ETH_TXD2 | AMHW_ZLG_ETH_PIN_CFG_AF_PP(PIOC_2));

    /* ETH_TX_CLK */
    am_gpio_pin_cfg(PIOC_3,
    PIOC_3_ETH_TX_CLK | AMHW_ZLG_ETH_PIN_CFG_INPUT_FLOAT(PIOC_3));

    /* ETH_RXD0 */
    am_gpio_pin_cfg(PIOC_4,
    PIOC_4_ETH_RXD0 | AMHW_ZLG_ETH_PIN_CFG_INPUT_FLOAT(PIOC_4));

    /* ETH_RXD1 */
    am_gpio_pin_cfg(PIOC_5,
    PIOC_5_ETH_RXD1 | AMHW_ZLG_ETH_PIN_CFG_INPUT_FLOAT(PIOC_5));
}

/**
 * \brief  Fills each p_eth_regs_config member with its default value.
 * \param  p_eth_regs_config: pointer to a ETH_InitTypeDef structure 
 *         which will be initialized.
 * \retval None
 */
void amhw_zlg_eth_usr_config_init(am_eth_regs_conf *p_eth_regs_config)
{
    /* Full-Duplex mode selected */
    p_eth_regs_config->eth_mode = AM_ZLG_ETH_MODE_FULLDUPLEX;
    /* PHY speed configured to 100Mbit/s */
    p_eth_regs_config->eth_speed = AM_ZLG_ETH_SPEED_100M;

    p_eth_regs_config->eth_auto_negotiation =
            AM_ZLG_ETH_AUTO_NEGOTIATION_ENABLE;
    p_eth_regs_config->eth_watch_dog = AM_ZLG_ETH_WATCH_DOG_ENABLE;
    p_eth_regs_config->eth_jabber = AM_ZLG_ETH_JABBER_ENABLE;
    p_eth_regs_config->eth_inter_fame_gap = AM_ZLG_ETH_INTER_FRAME_GAP_96_BIT;
    p_eth_regs_config->eth_receive_own = AM_ZLG_ETH_RECEIVE_OWN_ENABLE;
    p_eth_regs_config->eth_loop_back_mode = AM_ZLG_ETH_LOOPBACK_MODE_DISABLE;
    p_eth_regs_config->eth_checksum_off_load =
            AM_ZLG_ETH_CHECKSUM_OFFLOAD_DISABLE;
    p_eth_regs_config->eth_retry_transmission =
            AM_ZLG_ETH_RETRY_TRANSMISSION_ENABLE;
    p_eth_regs_config->eth_automatic_pad_crc_strip =
            AM_ZLG_ETH_AUTOMATIC_PAD_CRC_STRIP_DISABLE;
    p_eth_regs_config->eth_back_off_limit = AM_ZLG_ETH_BACK_OFF_LIMIT_10;
    p_eth_regs_config->eth_deferal_check = AM_ZLG_ETH_DEFERRAL_CHECK_DISABLE;
    p_eth_regs_config->eth_receive_all = AM_ZLG_ETH_RECEIVE_ALL_DISABLE;
    p_eth_regs_config->eth_source_addr_filter =
            AM_ZLG_ETH_SOURCE_ADDR_FILTER_DISABLE;
    p_eth_regs_config->eth_pass_ctl_frames =
            AM_ZLG_ETH_PASS_CTL_FRAMES_BLOCKALL;
    p_eth_regs_config->eth_broadcast_frames_reception =
            AM_ZLG_ETH_BROADCAST_FRAMES_RECEPTION_ENABLE;
    p_eth_regs_config->eth_destination_addr_filter =
            AM_ZLG_ETH_DESTINATION_ADDR_FILTER_NORMAL;
    p_eth_regs_config->eth_promiscuous_mode =
            AM_ZLG_ETH_PROMISCUOUS_MODE_DISABLE;
    p_eth_regs_config->eth_multicast_frames_filter =
            AM_ZLG_ETH_MULTICAST_FRAMES_FILTER_PERFECT;
    p_eth_regs_config->eth_unicast_frames_filter =
            AM_ZLG_ETH_UNICAST_FRAMES_FILTER_PERFECT;
    p_eth_regs_config->eth_hash_table_high = 0x0;
    p_eth_regs_config->eth_hash_table_low = 0x0;
    p_eth_regs_config->eth_pause_time = 0x0;
    p_eth_regs_config->eth_zero_quanta_pause =
            AM_ZLG_ETH_ZERO_QUANTA_PAUSE_DISABLE;
    p_eth_regs_config->eth_pause_low_threshold =
            AM_ZLG_ETH_PAUSE_LOW_THRESHOLD_MINUS4;
    p_eth_regs_config->eth_unicast_pause_frame_detect =
            AM_ZLG_ETH_UNICAST_PAUSE_FRAME_DETECT_DISABLE;
    p_eth_regs_config->eth_receive_flow_ctl =
            AM_ZLG_ETH_RECEIVE_FLOW_CRL_DISABLE;
    p_eth_regs_config->eth_tramsmit_flow_ctl =
            AM_ZLG_ETH_TRANSMIT_FLOW_CRL_DISABLE;
    p_eth_regs_config->eth_vlan_tag_comparison =
            AM_ZLG_ETH_VLAN_TAG_COMPARISON_16BIT;
    p_eth_regs_config->eth_vlan_tag_identifier = 0x0;

    /*---------------------- DMA Configuration   ----------------------------*/
    p_eth_regs_config->eth_transmit_store_forward =
            AM_ZLG_ETH_TRANSMIT_STORE_FORWARD_ENABLE;
    p_eth_regs_config->eth_transmit_threshold_ctl =
            AM_ZLG_ETH_TRANSMIT_THRESHOLD_CTL_64BYTES;
    p_eth_regs_config->eth_forward_error_frames =
            AM_ZLG_ETH_FORWARD_ERROR_FRAMES_DISABLE;
    p_eth_regs_config->eth_forward_under_size_good_frames =
            AM_ZLG_ETH_FORWARD_UNDERSIZED_GOOD_FRAMES_DISABLE;
    p_eth_regs_config->eth_receive_threshold_ctl =
            AM_ZLG_ETH_RECEIVE_THRESHOLD_CTL_64BYTES;
    p_eth_regs_config->eth_sec_frame_operate =
            AM_ZLG_ETH_SEC_FRAME_OPERATE_ENABLE;
    p_eth_regs_config->eth_fixed_burst = AM_ZLG_ETH_FIXED_BURST_ENABLE;
    p_eth_regs_config->eth_dma_burst_len =
            AM_ZLG_ETH_TX_DMA_BURST_LENGTH_32BEAT;
    p_eth_regs_config->eth_desc_skip_len = 0x0;
    p_eth_regs_config->eth_dma_arbitration =
            AM_ZLG_ETH_DMA_ARBITRATION_ROUNDROBIN_RXTX_2_1;
}

/* remote IP addr & port */
#define AM_ZLG_ETH_DEST_IP_ADDR0               192
#define AM_ZLG_ETH_DEST_IP_ADDR1               168
#define AM_ZLG_ETH_DEST_IP_ADDR2               40
#define AM_ZLG_ETH_DEST_IP_ADDR3               179
#define AM_ZLG_ETH_DEST_PORT                   50000

/* MAC addr */
#define AM_ZLG_ETH_MAC_ADDR0                   22
#define AM_ZLG_ETH_MAC_ADDR1                   33
#define AM_ZLG_ETH_MAC_ADDR2                   44
#define AM_ZLG_ETH_MAC_ADDR3                   55
#define AM_ZLG_ETH_MAC_ADDR4                   66
#define AM_ZLG_ETH_MAC_ADDR5                   77

/*static ip addr */
#define AM_ZLG_ETH_LOCAL_IP_ADDR0              192
#define AM_ZLG_ETH_LOCAL_IP_ADDR1              168
#define AM_ZLG_ETH_LOCAL_IP_ADDR2              40
#define AM_ZLG_ETH_LOCAL_IP_ADDR3              110
#define AM_ZLG_ETH_LOCAL_SERVER_PORT           60000

/* subnet netmask */
#define AM_ZLG_ETH_NETMASK_ADDR0               255
#define AM_ZLG_ETH_NETMASK_ADDR1               255
#define AM_ZLG_ETH_NETMASK_ADDR2               255
#define AM_ZLG_ETH_NETMASK_ADDR3               0

/* gateway */
#define AM_ZLG_ETH_GW_ADDR0                    192
#define AM_ZLG_ETH_GW_ADDR1                    168
#define AM_ZLG_ETH_GW_ADDR2                    40
#define AM_ZLG_ETH_GW_ADDR3                    1

uint8_t g_eth_dest_ip[4] = { AM_ZLG_ETH_DEST_IP_ADDR0, AM_ZLG_ETH_DEST_IP_ADDR1,
            AM_ZLG_ETH_DEST_IP_ADDR2, AM_ZLG_ETH_DEST_IP_ADDR3 };
uint32_t g_eth_dest_port = AM_ZLG_ETH_DEST_PORT;

uint8_t g_eth_local_ip[4] = { AM_ZLG_ETH_LOCAL_IP_ADDR0,
            AM_ZLG_ETH_LOCAL_IP_ADDR1, AM_ZLG_ETH_LOCAL_IP_ADDR2,
            AM_ZLG_ETH_LOCAL_IP_ADDR3 };
uint32_t g_eth_local_port = AM_ZLG_ETH_LOCAL_SERVER_PORT;

uint8_t g_eth_mac[6] = { AM_ZLG_ETH_MAC_ADDR0, AM_ZLG_ETH_MAC_ADDR1,
            AM_ZLG_ETH_MAC_ADDR2, AM_ZLG_ETH_MAC_ADDR3 };
uint8_t g_eth_mask[4] = { AM_ZLG_ETH_NETMASK_ADDR0, AM_ZLG_ETH_NETMASK_ADDR1,
            AM_ZLG_ETH_NETMASK_ADDR2, AM_ZLG_ETH_NETMASK_ADDR3 };
uint8_t g_eth_gateway[4] = { AM_ZLG_ETH_GW_ADDR0, AM_ZLG_ETH_GW_ADDR1,
            AM_ZLG_ETH_GW_ADDR2, AM_ZLG_ETH_GW_ADDR3 };

