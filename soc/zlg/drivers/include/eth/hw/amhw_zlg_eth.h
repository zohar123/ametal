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
 * \brief amhw_zlg_eth.h
 *
 * \internal
 * \par Modification History
 * - 1.00 19-06-11  wyy, first implementation
 * \endinternal
 */
#ifndef  __AMHW_ZLG_ETH_H
#define  __AMHW_ZLG_ETH_H
#ifdef __cplusplus
extern "C" {
#endif 

#define AMHW_ZLG_ETH_GET_PHY_LINK_STATUS()		    \
        (am_zlg_phy_read_reg(ETHERNET_PHY_ADDRESS, AM_ZLG_PHY_BSR) & 0x00000004)
#define AMHW_ZLG_ETH_PIN_CFG_AF_PP(PIN_NAME)        \
        (PIN_NAME##_SPEED_50MHz | PIN_NAME##_AF_PP)
#define AMHW_ZLG_ETH_PIN_CFG_INPUT_FLOAT(PIN_NAME)  (PIN_NAME##_INPUT_FLOAT)

typedef struct {
    uint32_t eth_auto_negotiation;          /**< \brief auto negot for phy */
    uint32_t eth_watch_dog;                 /**< \brief watchdog timer */
    uint32_t eth_jabber;                    /**< \brief jabber timer */
    uint32_t eth_inter_fame_gap;            /**< \brief IFG frames during trans */
    uint32_t eth_speed;                     /**< \brief enet speed: 10/100 Mbps */
    uint32_t eth_receive_own;               /**< \brief the reception of frames */
    uint32_t eth_loop_back_mode;            /**< \brief MAC MII Loopback mode */
    uint32_t eth_mode;                      /**< \brief half/Full-Duplex mode */
    uint32_t eth_checksum_off_load;         /**< \brief IPv4 checksum checking */
    uint32_t eth_retry_transmission;        /**< \brief MAC attempt retries */
    uint32_t eth_automatic_pad_crc_strip;   /**< \brief automatic MAC Pad/CRC */
    uint32_t eth_back_off_limit;            /**< \brief backOff limit value */
    uint32_t eth_deferal_check;             /**< \brief deferral check function */
    uint32_t eth_receive_all;               /**< \brief all frames reception */
    uint32_t eth_source_addr_filter;        /**< \brief source Address Filter mode */
    uint32_t eth_pass_ctl_frames;           /**< \brief forwarding mode */
    uint32_t eth_broadcast_frames_reception;/**< \brief reception of Broadcast */
    uint32_t eth_destination_addr_filter;   /**< \brief destination filter mode */
    uint32_t eth_promiscuous_mode;          /**< \brief promiscuous Mode */
    uint32_t eth_multicast_frames_filter;   /**< \brief multicast Frames filter */
    uint32_t eth_unicast_frames_filter;     /**< \brief unicast Frames filter */
    uint32_t eth_hash_table_high;           /**< \brief the higher 32 bits */
    uint32_t eth_hash_table_low;            /**< \brief the lower 32 bits */
    uint32_t eth_pause_time;                /**< \brief the Pause Time field */
    uint32_t eth_zero_quanta_pause;         /**< \brief zero-Quanta Pause Control */
    uint32_t eth_pause_low_threshold;       /**< \brief threshold of the PAUSE */
    uint32_t eth_unicast_pause_frame_detect;/**< \brief MAC detection frames */
    uint32_t eth_receive_flow_ctl;          /**< \brief received Pause frame */
    uint32_t eth_tramsmit_flow_ctl;         /**< \brief transmit Pause frames */
    uint32_t eth_vlan_tag_comparison;       /**< \brief 12/16-bit VLAN identifier */
    uint32_t eth_vlan_tag_identifier;       /**< \brief VLAN tag identifier */
    uint32_t eth_transmit_store_forward;    /**< \brief Transmit forward mode */
    uint32_t eth_transmit_threshold_ctl;    /**< \brief Transmit Threshold Control */
    uint32_t eth_forward_error_frames;      /**< \brief DMA of erroneous frames */
    uint32_t eth_forward_under_size_good_frames;/**< \brief Rx FIFO to forward */
    uint32_t eth_receive_threshold_ctl;     /**< \brief threshold of the Receive */
    uint32_t eth_sec_frame_operate;         /**< \brief operate second frame mode */
    uint32_t eth_fixed_burst;               /**< \brief AHB Master interface */
    uint32_t eth_dma_burst_len;             /**< \brief maximum number of beats */
    uint32_t eth_desc_skip_len;             /**< \brief skip between two desc */
    uint32_t eth_dma_arbitration;           /**< \brief DMA Tx/Rx arbitration */
} am_eth_regs_conf;

typedef enum {
    RESET = 0, SET = !RESET
} flag_status_t;

typedef enum {
    DISABLE = 0, ENABLE = !DISABLE
} func_state_t;
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

typedef enum amhw_zlg_eth_speed {
    AMHW_ZLG_ETH_SPEED_10M = ((uint32_t) 0x00000000),
    AMHW_ZLG_ETH_SPEED_100M = ((uint32_t) 0x00004000),
} amhw_zlg_eth_speed_t;

#define IS_AMHW_ZLG_ETH_SPEED(SPEED) (((SPEED) == AMHW_ZLG_ETH_SPEED_10M) || \
                             ((SPEED) == AMHW_ZLG_ETH_SPEED_100M))

typedef enum amhw_zlg_eth_ {
    AMHW_ZLG_ETH_MODE_HALFDUPLEX = ((uint32_t) 0x00000000),
    AMHW_ZLG_ETH_MODE_FULLDUPLEX = ((uint32_t) 0x00000800),
} amhw_zlg_eth_mode_t;
#define IS_AMHW_ZLG_ETH_MODE(MODE) (((MODE) == AMHW_ZLG_ETH_MODE_HALFDUPLEX) || \
                             ((MODE) == AMHW_ZLG_ETH_MODE_FULLDUPLEX))

void amhw_zlg_eth_soft_reset(void);
void amhw_zlg_eth_mac_addr_config(uint32_t mac_addr, uint8_t *addr);
void amhw_zlg_eth_mac_transmission_cmd(func_state_t new_state);
void amhw_zlg_eth_mac_reception_cmd(func_state_t new_state);
void amhw_zlg_eth_flush_transmit_fifo(void);
void amhw_zlg_eth_dma_transmission_cmd(func_state_t new_state);
void amhw_zlg_eth_dma_reception_cmd(func_state_t new_state);
void amhw_zlg_eth_reset_eth(void);
extern void amhw_zlg_eth_gpio_config(void);
extern void amhw_zlg_eth_set_speed(amhw_zlg_eth_speed_t eth_speed);
extern void amhw_zlg_eth_set_mode(amhw_zlg_eth_mode_t eth_mode);
extern void amhw_zlg_eth_usr_config_init( \
                                       am_eth_regs_conf *p_eth_regs_config);
uint32_t amhw_zlg_eth_reg_init(am_eth_regs_conf* p_eth_regs_config,
        uint16_t phy_addr);

flag_status_t amhw_zlg_eth_get_soft_reset_status(void);

#ifdef __cplusplus
}
#endif 
#endif
