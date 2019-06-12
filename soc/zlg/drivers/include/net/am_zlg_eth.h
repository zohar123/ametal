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
 * \brief am_zlg_eth.h
 *
 * \internal
 * \par Modification History
 * - 1.00 19-06-11  wyy, first implementation
 * \endinternal
 */
#ifndef __AM_ZLG_ETH_ETH_H
#define __AM_ZLG_ETH_ETH_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "am_zlg_eth_conf.h"

#define ASSERT_PARAM_ENABLE 1

typedef struct {
    /**
     * @brief / * MAC
     */
    uint32_t eth_auto_negotiation; /*!< Selects or not the AutoNegotiation mode for the external PHY
     The AutoNegotiation allows an automatic setting of the Speed (10/100Mbps)
     and the mode (half/full-duplex).
     This parameter can be a value of @ref eth_auto_negotiation */

    uint32_t eth_watch_dog; /*!< Selects or not the Watchdog timer
     When enabled, the MAC allows no more then 2048 bytes to be received.
     When disabled, the MAC can receive up to 16384 bytes.
     This parameter can be a value of @ref eth_watch_dog */

    uint32_t eth_jabber; /*!< Selects or not Jabber timer
     When enabled, the MAC allows no more then 2048 bytes to be sent.
     When disabled, the MAC can send up to 16384 bytes.
     This parameter can be a value of @ref eth_jabber */

    uint32_t eth_inter_fame_gap; /*!< Selects the minimum IFG between frames during transmission
     This parameter can be a value of @ref eth_inter_fame_gap */

    uint32_t eth_speed; /*!< Sets the Ethernet speed: 10/100 Mbps
     This parameter can be a value of @ref eth_speed */

    uint32_t eth_receive_own; /*!< Selects or not the ReceiveOwn
     ReceiveOwn allows the reception of frames when the TX_EN signal is asserted
     in Half-Duplex mode
     This parameter can be a value of @ref eth_receive_own */

    uint32_t eth_loop_back_mode; /*!< Selects or not the internal MAC MII Loopback mode
     This parameter can be a value of @ref eth_loop_back_mode */

    uint32_t eth_mode; /*!< Selects the MAC duplex mode: Half-Duplex or Full-Duplex mode
     This parameter can be a value of @ref eth_mode */

    uint32_t eth_checksum_off_load; /*!< Selects or not the IPv4 checksum checking for received frame payloads' TCP/UDP/ICMP headers.
     This parameter can be a value of @ref ETH_Checksum_Offload */

    uint32_t eth_retry_transmission; /*!< Selects or not the MAC attempt retries transmission, based on the settings of BL,
     when a collision occurs (Half-Duplex mode)
     This parameter can be a value of @ref ETH_Retry_Transmission */

    uint32_t eth_automatic_pad_crc_strip; /*!< Selects or not the Automatic MAC Pad/CRC Stripping
     This parameter can be a value of @ref ETH_Automatic_Pad_CRC_Strip */

    uint32_t eth_back_off_limit; /*!< Selects the BackOff limit value
     This parameter can be a value of @ref ETH_Back_Off_Limit */

    uint32_t eth_deferal_check; /*!< Selects or not the deferral check function (Half-Duplex mode)
     This parameter can be a value of @ref ETH_Deferral_Check */

    uint32_t eth_receive_all; /*!< Selects or not all frames reception by the MAC (No filtering)
     This parameter can be a value of @ref ETH_Receive_All */

    uint32_t eth_source_addr_filter; /*!< Selects the Source Address Filter mode
     This parameter can be a value of @ref ETH_Source_Addr_Filter */

    uint32_t eth_pass_ctl_frames; /*!< Sets the forwarding mode of the control frames (including unicast and multicast PAUSE frames)
     This parameter can be a value of @ref ETH_Pass_Control_Frames */

    uint32_t eth_broadcast_frames_reception; /*!< Selects or not the reception of Broadcast Frames
     This parameter can be a value of @ref ETH_Broadcast_Frames_Reception */

    uint32_t eth_destination_addr_filter; /*!< Sets the destination filter mode for both unicast and multicast frames
     This parameter can be a value of @ref ETH_Destination_Addr_Filter */

    uint32_t eth_promiscuous_mode; /*!< Selects or not the Promiscuous Mode
     This parameter can be a value of @ref ETH_Promiscuous_Mode */

    uint32_t eth_multicast_frames_filter; /*!< Selects the Multicast Frames filter mode: None/HashTableFilter/PerfectFilter/PerfectHashTableFilter
     This parameter can be a value of @ref ETH_Multicast_Frames_Filter */

    uint32_t eth_unicast_frames_filter; /*!< Selects the Unicast Frames filter mode: HashTableFilter/PerfectFilter/PerfectHashTableFilter
     This parameter can be a value of @ref ETH_Unicast_Frames_Filter */

    uint32_t eth_hash_table_high; /*!< This field holds the higher 32 bits of Hash table.  */

    uint32_t eth_hash_table_low; /*!< This field holds the lower 32 bits of Hash table.  */

    uint32_t eth_pause_time; /*!< This field holds the value to be used in the Pause Time field in the
     transmit control frame */

    uint32_t eth_zero_quanta_pause; /*!< Selects or not the automatic generation of Zero-Quanta Pause Control frames
     This parameter can be a value of @ref ETH_Zero_Quanta_Pause */

    uint32_t eth_pause_low_threshold; /*!< This field configures the threshold of the PAUSE to be checked for
     automatic retransmission of PAUSE Frame
     This parameter can be a value of @ref ETH_Pause_Low_Threshold */

    uint32_t eth_unicast_pause_frame_detect; /*!< Selects or not the MAC detection of the Pause frames (with MAC Address0
     unicast address and unique multicast address)
     This parameter can be a value of @ref ETH_Unicast_Pause_Frame_Detect */

    uint32_t eth_receive_flow_ctl; /*!< Enables or disables the MAC to decode the received Pause frame and
     disable its transmitter for a specified time (Pause Time)
     This parameter can be a value of @ref ETH_Receive_Flow_Control */

    uint32_t eth_tramsmit_flow_ctl; /*!< Enables or disables the MAC to transmit Pause frames (Full-Duplex mode)
     or the MAC back-pressure operation (Half-Duplex mode)
     This parameter can be a value of @ref ETH_Transmit_Flow_Control */

    uint32_t eth_vlan_tag_comparison; /*!< Selects the 12-bit VLAN identifier or the complete 16-bit VLAN tag for
     comparison and filtering
     This parameter can be a value of @ref ETH_VLAN_Tag_Comparison */

    uint32_t eth_vlan_tag_identifier; /*!< Holds the VLAN tag identifier for receive frames */

    /**
     * @brief / * DMA
     */

    uint32_t eth_transmit_store_forward; /*!< Enables or disables Transmit store and forward mode
     This parameter can be a value of @ref ETH_Transmit_Store_Forward */

    uint32_t eth_transmit_threshold_ctl; /*!< Selects or not the Transmit Threshold Control
     This parameter can be a value of @ref ETH_Transmit_Threshold_Control */

    uint32_t eth_forward_error_frames; /*!< Selects or not the forward to the DMA of erroneous frames
     This parameter can be a value of @ref ETH_Forward_Error_Frames */

    uint32_t eth_forward_under_size_good_frames; /*!< Enables or disables the Rx FIFO to forward Undersized frames (frames with no Error
     and length less than 64 bytes) including pad-bytes and CRC)
     This parameter can be a value of @ref ETH_Forward_Undersized_Good_Frames */

    uint32_t eth_receive_threshold_ctl; /*!< Selects the threshold level of the Receive FIFO
     This parameter can be a value of @ref ETH_Receive_Threshold_Control */

    uint32_t eth_sec_frame_operate; /*!< Selects or not the Operate on second frame mode, which allows the DMA to process a second
     frame of Transmit data even before obtaining the status for the first frame.
     This parameter can be a value of @ref ETH_Second_Frame_Operate */

    uint32_t eth_fixed_burst; /*!< Enables or disables the AHB Master interface fixed burst transfers
     This parameter can be a value of @ref ETH_Fixed_Burst */

    uint32_t eth_dma_burst_len; /*!< Indicates the maximum number of beats to be transferred in one Tx DMA transaction
     This parameter can be a value of @ref ETH_Tx_DMA_Burst_Length */

    uint32_t eth_desc_skip_len; /*!< Specifies the number of word to skip between two unchained descriptors (Ring mode) */

    uint32_t eth_dma_arbitration; /*!< Selects the DMA Tx/Rx arbitration
     This parameter can be a value of @ref ETH_DMA_Arbitration */
} am_eth_regs_conf;

typedef struct {
    __IO uint32_t status; /*!< status */
    uint32_t ctl_buff_size; /*!< Control and Buffer1, Buffer2 lengths */
    uint32_t buff_1_addr; /*!< Buffer1 address pointer */
    uint32_t buff_2_next_desc_addr; /*!< Buffer2 or next descriptor address pointer */
} eth_dma_desc_type_t;

typedef struct {
    u32 length;
    u32 buffer;
    __IO eth_dma_desc_type_t *descriptor;
} frame_type_def_t;

typedef struct {
    __IO eth_dma_desc_type_t *fs_rx_desc; /*!< First Segment Rx Desc */
    __IO eth_dma_desc_type_t *ls_rx_desc; /*!< Last Segment Rx Desc */
    __IO uint32_t seg_count; /*!< Segment count */
} eth_dma_rx_frame_info_t;

#define AM_ETH_MAX_PACKET_SIZE        1524    /*!< AM_ETH_HEADER + AM_ETH_EXTRA + AM_ETH_VLAN_TAG + AM_ETH_MAX_ETH_PAYLOAD + AM_ETH_CRC */
#define AM_ETH_HEADER                   14    /*!< 6 byte Dest addr, 6 byte Src addr, 2 byte length/type */
#define AM_ETH_CRC                       4    /*!< Ethernet CRC */
#define AM_ETH_EXTRA                     2    /*!< Extra bytes in some cases */   
#define AM_ETH_VLAN_TAG                  4    /*!< optional 802.1q VLAN Tag */
#define AM_ETH_MIN_ETH_PAYLOAD          46    /*!< Minimum Ethernet payload size */
#define AM_ETH_MAX_ETH_PAYLOAD        1500    /*!< Maximum Ethernet payload size */
#define AM_ETH_JUMBO_FRAME_PAYLOAD    9000    /*!< Jumbo frame payload size */      

#ifndef AM_ETH_ETH_RX_BUF_SIZE
#define AM_ETH_ETH_RX_BUF_SIZE         AM_ETH_MAX_PACKET_SIZE
#endif

/* 5 Ethernet driver receive buffers are used (in a chained linked list)*/
#ifndef AM_ETH_RXBUFNB
#define AM_ETH_RXBUFNB             5     /*  5 Rx buffers of size AM_ETH_ETH_RX_BUF_SIZE */
#endif

#ifndef AM_ETH_TX_BUF_SIZE 
#define AM_ETH_TX_BUF_SIZE         AM_ETH_MAX_PACKET_SIZE
#endif

/* 5 ethernet driver transmit buffers are used (in a chained linked list)*/
#ifndef AM_ETH_TXBUFNB
#define AM_ETH_TXBUFNB             5      /* 5  Tx buffers of size AM_ETH_TX_BUF_SIZE */
#endif

#define  AM_ETH_DMA_RX_DESC_FRAME_LEN_SHIFT           16

#define AM_ETH_DMA_TX_DESC_OWN                     ((uint32_t)0x80000000)  /*!< OWN bit: descriptor is owned by DMA engine */
#define AM_ETH_DMA_TX_DESC_IC                      ((uint32_t)0x80000000)  /*!< Interrupt on Completion */
#define AM_ETH_ETH_DMA_TX_DESC_LS                  ((uint32_t)0x40000000)  /*!< Last Segment */
#define AM_ETH_DMA_TX_DESC_FS                      ((uint32_t)0x20000000)  /*!< First Segment */
#define AM_ETH_DMA_TX_DESC_DC                      ((uint32_t)0x08000000)  /*!< Disable CRC */
#define AM_ETH_DMA_TX_DESC_DP                      ((uint32_t)0x04000000)  /*!< Disable Padding */
#define AM_ETH_DMA_TX_DESC_TTSE                    ((uint32_t)0x02000000)  /*!< Transmit Time Stamp Enable */
#define AM_ETH_DMA_TX_DESC_CIC                     ((uint32_t)0x00C00000)  /*!< Checksum Insertion Control: 4 cases */
#define AM_ETH_DMA_TX_DESC_CIC_BYPASS              ((uint32_t)0x00000000)  /*!< Do Nothing: Checksum Engine is bypassed */ 
#define AM_ETH_DMA_TX_DESC_CIC_IPV4_HEADER          ((uint32_t)0x00400000)  /*!< IPV4 header Checksum Insertion */ 
#define AM_ETH_DMA_TX_DESC_CIC_TCPUDPICMP_SEGMENT  ((uint32_t)0x00800000)  /*!< TCP/UDP/ICMP Checksum Insertion calculated over segment only */ 
#define AM_ETH_DMA_TX_DESC_CIC_TCPUDPICMP_FULL     ((uint32_t)0x00C00000)  /*!< TCP/UDP/ICMP Checksum Insertion fully calculated */ 
#define AM_ETH_DMA_TX_DESC_TER                     ((uint32_t)0x00200000)  /*!< Transmit End of Ring */
#define AM_ETH_DMA_TX_DESC_TCH                     ((uint32_t)0x01000000)  /*!< Second Address Chained */
#define AM_ETH_DMA_TX_DESC_TTSS                    ((uint32_t)0x00020000)  /*!< Tx Time Stamp status */
#define AM_ETH_DMA_TX_DESC_IHE                     ((uint32_t)0x00010000)  /*!< IP Header Error */
#define AM_ETH_DMA_TX_DESC_ES                      ((uint32_t)0x00008000)  /*!< Error summary: OR of the following bits: UE || ED || EC || LCO || NC || LCA || FF || JT */
#define AM_ETH_DMA_TX_DESC_JT                      ((uint32_t)0x00004000)  /*!< Jabber Timeout */
#define AM_ETH_DMA_TX_DESC_FF                      ((uint32_t)0x00002000)  /*!< Frame Flushed: DMA/MTL flushed the frame due to SW flush */
#define AM_ETH_DMA_TX_DESC_PCE                     ((uint32_t)0x00001000)  /*!< Payload Checksum Error */
#define AM_ETH_DMA_TX_DESC_LCA                     ((uint32_t)0x00000800)  /*!< Loss of Carrier: carrier lost during transmission */
#define AM_ETH_DMA_TX_DESC_NC                      ((uint32_t)0x00000400)  /*!< No Carrier: no carrier signal from the transceiver */
#define AM_ETH_DMA_TX_DESC_LCO                     ((uint32_t)0x00000200)  /*!< Late Collision: transmission aborted due to collision */
#define AM_ETH_DMA_TX_DESC_EC                      ((uint32_t)0x00000100)  /*!< Excessive Collision: transmission aborted after 16 collisions */
#define AM_ETH_DMA_TX_DESC_VF                      ((uint32_t)0x00000080)  /*!< VLAN Frame */
#define AM_ETH_DMA_TX_DESC_CC                      ((uint32_t)0x00000078)  /*!< Collision Count */
#define AM_ETH_DMA_TX_DESC_ED                      ((uint32_t)0x00000004)  /*!< Excessive Deferral */
#define AM_ETH_DMA_TX_DESC_UF                      ((uint32_t)0x00000002)  /*!< Underflow Error: late data arrival from the memory */
#define AM_ETH_DMA_TX_DESC_DB                      ((uint32_t)0x00000001)  /*!< Deferred Bit */

#define AM_ETH_DMA_TX_DESC_TBS1  ((uint32_t)0x00000fff)  /*!< Transmit Buffer1 Size */

/* Bit definition of TDES7 register */
#define ETH_DMAPTPTxDesc_TTSH  ((uint32_t)0xFFFFFFFF)  /* Transmit Time Stamp High */

#define AM_ETH_DMA_RX_DESC_OWN         ((uint32_t)0x80000000)  /*!< OWN bit: descriptor is owned by DMA engine  */
#define AM_ETH_DMA_RX_DESC_AFM         ((uint32_t)0x40000000)  /*!< DA Filter Fail for the rx frame  */
#define AM_ETH_DMA_RX_DESC_FL          ((uint32_t)0x3FFF0000)  /*!< Receive descriptor frame length  */
#define AM_ETH_DMA_RX_DESC_ES          ((uint32_t)0x00008000)  /*!< Error summary: OR of the following bits: DE || OE || IPC || LC || RWT || RE || CE */
#define AM_ETH_DMA_RX_DESC_DE          ((uint32_t)0x00004000)  /*!< Descriptor error: no more descriptors for receive frame  */
#define AM_ETH_DMA_RX_DESC_SAF         ((uint32_t)0x00002000)  /*!< SA Filter Fail for the received frame */
#define AM_ETH_DMA_RX_DESC_LE          ((uint32_t)0x00001000)  /*!< Frame size not matching with length field */
#define AM_ETH_DMA_RX_DESC_OE          ((uint32_t)0x00000800)  /*!< Overflow Error: Frame was damaged due to buffer overflow */
#define AM_ETH_DMA_RX_DESC_VLAN        ((uint32_t)0x00000400)  /*!< VLAN Tag: received frame is a VLAN frame */
#define AM_ETH_DMA_RX_DESC_FS          ((uint32_t)0x00000200)  /*!< First descriptor of the frame  */
#define AM_ETH_DMA_RX_DESC_LS          ((uint32_t)0x00000100)  /*!< Last descriptor of the frame  */ 
#define AM_ETH_DMA_RX_IPV4HCE     ((uint32_t)0x00000080)  /*!< IPC Checksum Error: Rx Ipv4 header checksum error   */    
#define AM_ETH_DMA_RX_DESC_LC          ((uint32_t)0x00000040)  /*!< Late collision occurred during reception   */
#define AM_ETH_DMA_RX_DESC_FT          ((uint32_t)0x00000020)  /*!< Frame type - Ethernet, otherwise 802.3    */
#define AM_ETH_DMA_RX_DESC_RWT         ((uint32_t)0x00000010)  /*!< Receive Watchdog Timeout: watchdog timer expired during reception    */
#define AM_ETH_DMA_RX_DESC_RE          ((uint32_t)0x00000008)  /*!< Receive error: error reported by MII interface  */
#define AM_ETH_DMA_RX_DESC_DBE         ((uint32_t)0x00000004)  /*!< Dribble bit error: frame contains non int multiple of 8 bits  */
#define AM_ETH_DMA_RX_DESC_CE          ((uint32_t)0x00000002)  /*!< CRC error */
#define AM_ETH_DMA_RX_DESC_MAMPCE      ((uint32_t)0x00000001)  /*!< Rx MAC Address/Payload Checksum Error: Rx MAC address matched/ Rx Payload Checksum Error */

#define AM_ETH_DMA_RX_DESC_RCH   ((uint32_t)0x01000000)  /*!< Second Address Chained */

/**--------------------------------------------------------------------------**/
/** 
 * @brief                     Description of common PHY registers
 */
/**--------------------------------------------------------------------------**/

/**
 * @}
 */

/** @defgroup PHY_Read_write_Timeouts 
 * @{
 */
#define AM_PHY_READ_TO                     ((uint32_t)0x0004FFFF)
#define AM_ETH_PHY_WRITE_TO                    ((uint32_t)0x0004FFFF)

/**
 * @}
 */

/** @defgroup PHY_Register_address 
 * @{
 */
#define AM_PHY_BCR                          0          /*!< Transceiver Basic Control Register */
#define AM_PHY_BSR                          1          /*!< Transceiver Basic status Register */

#define IS_ETH_PHY_ADDRESS(ADDRESS) ((ADDRESS) <= 0x20)
#define IS_ETH_PHY_REG(REG) (((REG) == AM_PHY_BCR) || \
                             ((REG) == AM_PHY_BSR) || \
                             ((REG) == AM_PHY_SR))
/**
 * @}
 */

/** @defgroup PHY_basic_Control_register 
 * @{
 */
#define AM_PHY_RESET                       ((uint16_t)0x8000)      /*!< PHY Reset */
#define AM_PHY_AUTO_NEGOTIATION             ((uint16_t)0x1000)      /*!< Enable auto-negotiation function */

/**
 * @}
 */

/** @defgroup PHY_basic_status_register 
 * @{
 */
#define AM_PHY_AUTO_NEGO_COMPLETE           ((uint16_t)0x0020)      /*!< Auto-Negotiation process completed */
#define AM_PHY_LINKED_STATUS               ((uint16_t)0x0004)      /*!< Valid link established */

/**
 * @}
 */

/**--------------------------------------------------------------------------**/
/** 
 * @brief                                  MAC defines
 */
/**--------------------------------------------------------------------------**/

/** @defgroup ETH_AutoNegotiation 
 * @{
 */
#define AM_ETH_AUTO_NEGOTIATION_ENABLE     ((uint32_t)0x00000001)
#define AM_ETH_AUTO_NEGOTIATION_DISABLE    ((uint32_t)0x00000000)
#define IS_AM_ETH_AUTO_NEGOTIATION(CMD) (((CMD) == AM_ETH_AUTO_NEGOTIATION_ENABLE) || \
                                     ((CMD) == AM_ETH_AUTO_NEGOTIATION_DISABLE))

/**
 * @}
 */

/** @defgroup ETH_watchdog 
 * @{
 */
#define AM_ETH_WATCH_DOG_ENABLE       ((uint32_t)0x00000000)
#define AM_ETH_WATCH_DOG_DISABLE      ((uint32_t)0x00800000)
#define IS_AM_ETH_WATCH_DOG(CMD) (((CMD) == AM_ETH_WATCH_DOG_ENABLE) || \
                              ((CMD) == AM_ETH_WATCH_DOG_DISABLE))

/**
 * @}
 */

/** @defgroup ETH_Jabber 
 * @{
 */
#define AM_ETH_JABBER_ENABLE    ((uint32_t)0x00000000)
#define AM_ETH_JABBER_DISABLE   ((uint32_t)0x00400000)
#define IS_AM_ETH_JABBER(CMD) (((CMD) == AM_ETH_JABBER_ENABLE) || \
                            ((CMD) == AM_ETH_JABBER_DISABLE))

/**
 * @}
 */

/** @defgroup ETH_Inter_Frame_Gap 
 * @{
 */
#define AM_ETH_INTER_FRAME_GAP_96_BIT   ((uint32_t)0x00000000)  /*!< minimum IFG between frames during transmission is 96Bit */
#define AM_ETH_INTER_FRAME_GAP_88_BIT   ((uint32_t)0x00020000)  /*!< minimum IFG between frames during transmission is 88Bit */
#define AM_ETH_INTER_FRAME_GAP_80_BIT   ((uint32_t)0x00040000)  /*!< minimum IFG between frames during transmission is 80Bit */
#define AM_ETH_INTER_FRAME_GAP_72_BIT   ((uint32_t)0x00060000)  /*!< minimum IFG between frames during transmission is 72Bit */
#define AM_ETH_INTER_FRAME_GAP_64_BIT   ((uint32_t)0x00080000)  /*!< minimum IFG between frames during transmission is 64Bit */
#define AM_ETH_INTER_FRAME_GAP_56_BIT   ((uint32_t)0x000A0000)  /*!< minimum IFG between frames during transmission is 56Bit */
#define AM_ETH_INTER_FRAME_GAP_48_BIT   ((uint32_t)0x000C0000)  /*!< minimum IFG between frames during transmission is 48Bit */
#define AM_ETH_INTER_FRAME_GAP_40_BIT   ((uint32_t)0x000E0000)  /*!< minimum IFG between frames during transmission is 40Bit */
#define IS_AM_ETH_INTER_FRAME_GAP(GAP) (((GAP) == AM_ETH_INTER_FRAME_GAP_96_BIT) || \
                                     ((GAP) == AM_ETH_INTER_FRAME_GAP_88_BIT) || \
                                     ((GAP) == AM_ETH_INTER_FRAME_GAP_80_BIT) || \
                                     ((GAP) == AM_ETH_INTER_FRAME_GAP_72_BIT) || \
                                     ((GAP) == AM_ETH_INTER_FRAME_GAP_64_BIT) || \
                                     ((GAP) == AM_ETH_INTER_FRAME_GAP_56_BIT) || \
                                     ((GAP) == AM_ETH_INTER_FRAME_GAP_48_BIT) || \
                                     ((GAP) == AM_ETH_INTER_FRAME_GAP_40_BIT))

/**
 * @}
 */

/** @defgroup ETH_Carrier_Sense 
 * @{
 */
#define AM_ETH_CARRIER_SENSE_ENABLE   ((uint32_t)0x00000000)
#define AM_ETH_CARRIER_SENSE_DISABLE  ((uint32_t)0x00010000)
#define IS_AM_ETH_CARRIER_SENSE(CMD) (((CMD) == AM_ETH_CARRIER_SENSE_ENABLE) || \
                                   ((CMD) == AM_ETH_CARRIER_SENSE_DISABLE))

/**
 * @}
 */

/** @defgroup ETH_Speed 
 * @{
 */
#define AM_ETH_SPEED_10M        ((uint32_t)0x00000000)
#define AM_ETH_SPEED_100M       ((uint32_t)0x00004000)
#define IS_AM_ETH_SPEED(SPEED) (((SPEED) == AM_ETH_SPEED_10M) || \
                             ((SPEED) == AM_ETH_SPEED_100M))

/**
 * @}
 */

/** @defgroup ETH_Receive_Own 
 * @{
 */
#define AM_ETH_RECEIVE_OWN_ENABLE     ((uint32_t)0x00000000)
#define AM_ETH_RECEIVE_OWN_DISABLE    ((uint32_t)0x00002000)
#define IS_AM_ETH_RECEIVE_OWN(CMD) (((CMD) == AM_ETH_RECEIVE_OWN_ENABLE) || \
                                 ((CMD) == AM_ETH_RECEIVE_OWN_DISABLE))

/**
 * @}
 */

/** @defgroup ETH_Loop_Back_Mode 
 * @{
 */
#define AM_ETH_LOOPBACK_MODE_ENABLE        ((uint32_t)0x00001000)
#define AM_ETH_LOOPBACK_MODE_DISABLE       ((uint32_t)0x00000000)
#define IS_AM_ETH_LOOPBACK_MODE(CMD) (((CMD) == AM_ETH_LOOPBACK_MODE_ENABLE) || \
                                   ((CMD) == AM_ETH_LOOPBACK_MODE_DISABLE))

/**
 * @}
 */

/** @defgroup ETH_Duplex_Mode 
 * @{
 */
#define AM_ETH_MODE_FULLDUPLEX       ((uint32_t)0x00000800)
#define AM_ETH_MODE_HALFDUPLEX       ((uint32_t)0x00000000)
#define IS_AM_ETH_DUPLEX_MODE(MODE) (((MODE) == AM_ETH_MODE_FULLDUPLEX) || \
                                  ((MODE) == AM_ETH_MODE_HALFDUPLEX))

/**
 * @}
 */

/** @defgroup ETH_Checksum_Offload 
 * @{
 */
#define AM_ETH_CHECKSUM_OFFLOAD_ENABLE     ((uint32_t)0x00000400)
#define AM_ETH_CHECKSUM_OFFLOAD_DISABLE    ((uint32_t)0x00000000)
#define IS_AM_ETH_CHECKSUM_OFFLOAD(CMD) (((CMD) == AM_ETH_CHECKSUM_OFFLOAD_ENABLE) || \
                                      ((CMD) == AM_ETH_CHECKSUM_OFFLOAD_DISABLE))

/**
 * @}
 */

/** @defgroup ETH_Retry_Transmission 
 * @{
 */
#define AM_ETH_RETRY_TRANSMISSION_ENABLE   ((uint32_t)0x00000000)
#define AM_ETH_RETRY_TRANSMISSION_DISABLE  ((uint32_t)0x00000200)
#define IS_AM_ETH_RETRY_TRANSMISSION(CMD) (((CMD) == AM_ETH_RETRY_TRANSMISSION_ENABLE) || \
                                        ((CMD) == AM_ETH_RETRY_TRANSMISSION_DISABLE))

/**
 * @}
 */

/** @defgroup ETH_Automatic_Pad_CRC_Strip 
 * @{
 */
#define AM_ETH_AUTOMATIC_PAD_CRC_STRIP_ENABLE     ((uint32_t)0x00000080)
#define AM_ETH_AUTOMATIC_PAD_CRC_STRIP_DISABLE    ((uint32_t)0x00000000)
#define IS_AM_ETH_AUTOMATIC_PADCRC_STRIP(CMD) (((CMD) == AM_ETH_AUTOMATIC_PAD_CRC_STRIP_ENABLE) || \
                                            ((CMD) == AM_ETH_AUTOMATIC_PAD_CRC_STRIP_DISABLE))

/**
 * @}
 */

/** @defgroup ETH_Back_Off_Limit 
 * @{
 */
#define AM_ETH_BACK_OFF_LIMIT_10  ((uint32_t)0x00000000)
#define AM_ETH_BACK_OFF_LIMIT_8   ((uint32_t)0x00000020)
#define AM_ETH_BACK_OFF_LIMIT_4   ((uint32_t)0x00000040)
#define AM_ETH_BACK_OFF_LIMIT_1   ((uint32_t)0x00000060)
#define IS_AM_ETH_BACK_OFF_LIMIT(LIMIT) (((LIMIT) == AM_ETH_BACK_OFF_LIMIT_10) || \
                                     ((LIMIT) == AM_ETH_BACK_OFF_LIMIT_8) || \
                                     ((LIMIT) == AM_ETH_BACK_OFF_LIMIT_4) || \
                                     ((LIMIT) == AM_ETH_BACK_OFF_LIMIT_1))

/**
 * @}
 */

/** @defgroup ETH_Deferral_Check 
 * @{
 */
#define AM_ETH_DEFERRAL_CHECK_ENABLE       ((uint32_t)0x00000010)
#define AM_ETH_DEFERRAL_CHECK_DISABLE      ((uint32_t)0x00000000)
#define IS_AM_ETH_DEFERRAL_CHECK(CMD) (((CMD) == AM_ETH_DEFERRAL_CHECK_ENABLE) || \
                                    ((CMD) == AM_ETH_DEFERRAL_CHECK_DISABLE))

/**
 * @}
 */

/** @defgroup ETH_Receive_All 
 * @{
 */
#define AM_ETH_RECEIVE_ALL_ENABLE     ((uint32_t)0x80000000)
#define AM_ETH_RECEIVE_ALL_DISABLE    ((uint32_t)0x00000000)
#define IS_AM_ETH_RECEIVE_ALL(CMD) (((CMD) == AM_ETH_RECEIVE_ALL_ENABLE) || \
                                 ((CMD) == AM_ETH_RECEIVE_ALL_DISABLE))

/**
 * @}
 */

/** @defgroup ETH_Source_Addr_Filter 
 * @{
 */
#define AM_ETH_SOURCE_ADDR_FILTER_NORMAL_ENABLE       ((uint32_t)0x00000200)
#define AM_ETH_SOURCE_ADDR_FILTER_INVERSE_ENABLE      ((uint32_t)0x00000300)
#define AM_ETH_SOURCE_ADDR_FILTER_DISABLE             ((uint32_t)0x00000000)
#define IS_AM_ETH_SOURCE_ADDR_FILTER(CMD) (((CMD) == AM_ETH_SOURCE_ADDR_FILTER_NORMAL_ENABLE) || \
                                        ((CMD) == AM_ETH_SOURCE_ADDR_FILTER_INVERSE_ENABLE) || \
                                        ((CMD) == AM_ETH_SOURCE_ADDR_FILTER_DISABLE))

/**
 * @}
 */

/** @defgroup ETH_Pass_Control_Frames 
 * @{
 */
#define AM_ETH_PASS_CTL_FRAMES_BLOCKALL                ((uint32_t)0x00000040)  /*!< MAC filters all control frames from reaching the application */
#define AM_ETH_PASS_CTL_FRAMES_FORWARDALL              ((uint32_t)0x00000080)  /*!< MAC forwards all control frames to application even if they fail the Address Filter */
#define AM_ETH_PASS_CONTROL_FRAMES_FORWARD_PASSED_ADDR_FILTER ((uint32_t)0x000000C0)  /*!< MAC forwards control frames that pass the Address Filter. */ 
#define IS_AM_ETH_CONTROL_FRAMES(PASS) (((PASS) == AM_ETH_PASS_CTL_FRAMES_BLOCKALL) || \
                                     ((PASS) == AM_ETH_PASS_CTL_FRAMES_FORWARDALL) || \
                                     ((PASS) == AM_ETH_PASS_CONTROL_FRAMES_FORWARD_PASSED_ADDR_FILTER))

/**
 * @}
 */

/** @defgroup ETH_Broadcast_Frames_Reception 
 * @{
 */
#define AM_ETH_BROADCAST_FRAMES_RECEPTION_ENABLE      ((uint32_t)0x00000000)
#define AM_ETH_BROADCAST_FRAMES_RECEPTION_DISABLE     ((uint32_t)0x00000020)
#define IS_AM_ETH_BROADCAST_FRAMES_RECEPTION(CMD) (((CMD) == AM_ETH_BROADCAST_FRAMES_RECEPTION_ENABLE) || \
                                                ((CMD) == AM_ETH_BROADCAST_FRAMES_RECEPTION_DISABLE))

/**
 * @}
 */

/** @defgroup ETH_Destination_Addr_Filter 
 * @{
 */
#define AM_ETH_DESTINATION_ADDR_FILTER_NORMAL    ((uint32_t)0x00000000)
#define AM_ETH_DESTINATION_ADDR_FILTER_INVERSE   ((uint32_t)0x00000008)
#define IS_AM_ETH_DESTINATION_ADDR_FILTER(FILTER) (((FILTER) == AM_ETH_DESTINATION_ADDR_FILTER_NORMAL) || \
                                                ((FILTER) == AM_ETH_DESTINATION_ADDR_FILTER_INVERSE))

/**
 * @}
 */

/** @defgroup ETH_Promiscuous_Mode 
 * @{
 */
#define AM_ETH_PROMISCUOUS_MODE_ENABLE     ((uint32_t)0x00000001)
#define AM_ETH_PROMISCUOUS_MODE_DISABLE    ((uint32_t)0x00000000)
#define IS_AM_ETH_PROMISCUOUS_MODE(CMD) (((CMD) == AM_ETH_PROMISCUOUS_MODE_ENABLE) || \
                                      ((CMD) == AM_ETH_PROMISCUOUS_MODE_DISABLE))

/**
 * @}
 */

/** @defgroup ETH_Multicast_Frames_Filter 
 * @{
 */
#define AM_ETH_MULTICAST_FRAMES_FILTER_PERFECT_HASHTABLE    ((uint32_t)0x00000404)
#define AM_ETH_MULTICAST_FRAMES_FILTER_HASHTABLE           ((uint32_t)0x00000004)
#define AM_ETH_MULTICAST_FRAMES_FILTER_PERFECT             ((uint32_t)0x00000000)
#define AM_ETH_MULTICAST_FRAMES_FILTER_NONE                ((uint32_t)0x00000010)
#define IS_AM_ETH_MULTICAST_FRAMES_FILTER(FILTER) (((FILTER) == AM_ETH_MULTICAST_FRAMES_FILTER_PERFECT_HASHTABLE) || \
                                                ((FILTER) == AM_ETH_MULTICAST_FRAMES_FILTER_HASHTABLE) || \
                                                ((FILTER) == AM_ETH_MULTICAST_FRAMES_FILTER_PERFECT) || \
                                                ((FILTER) == AM_ETH_MULTICAST_FRAMES_FILTER_NONE))

/**
 * @}
 */

/** @defgroup ETH_Unicast_Frames_Filter 
 * @{
 */
#define AM_ETH_UNICAST_FRAMES_FILTER_PERFECT_HASHTABLE ((uint32_t)0x00000402)
#define AM_ETH_UNICAST_FRAMES_FILTER_HASHTABLE        ((uint32_t)0x00000002)
#define AM_ETH_UNICAST_FRAMES_FILTER_PERFECT          ((uint32_t)0x00000000)
#define IS_AM_ETH_UNICAST_FRAMES_FILTER(FILTER) (((FILTER) == AM_ETH_UNICAST_FRAMES_FILTER_PERFECT_HASHTABLE) || \
                                              ((FILTER) == AM_ETH_UNICAST_FRAMES_FILTER_HASHTABLE) || \
                                              ((FILTER) == AM_ETH_UNICAST_FRAMES_FILTER_PERFECT))

/**
 * @}
 */

/** @defgroup ETH_Pause_Time 
 * @{
 */
#define IS_AM_ETH_PAUSE_TIME(TIME) ((TIME) <= 0xFFFF)

/**
 * @}
 */

/** @defgroup ETH_Zero_Quanta_Pause 
 * @{
 */
#define AM_ETH_ZERO_QUANTA_PAUSE_ENABLE     ((uint32_t)0x00000000)
#define AM_ETH_ZERO_QUANTA_PAUSE_DISABLE    ((uint32_t)0x00000080)
#define IS_AM_ETH_ZERO_QUANTA_PAUSE(CMD)   (((CMD) == AM_ETH_ZERO_QUANTA_PAUSE_ENABLE) || \
                                        ((CMD) == AM_ETH_ZERO_QUANTA_PAUSE_DISABLE))
/**
 * @}
 */

/** @defgroup ETH_Pause_Low_Threshold 
 * @{
 */
#define AM_ETH_PAUSE_LOW_THRESHOLD_MINUS4        ((uint32_t)0x00000000)  /*!< Pause time minus 4 slot times */
#define AM_ETH_PAUSE_LOW_THRESHOLD_MINUS28       ((uint32_t)0x00000010)  /*!< Pause time minus 28 slot times */
#define AM_ETH_PAUSE_LOW_THRESHOLD_MINUS144      ((uint32_t)0x00000020)  /*!< Pause time minus 144 slot times */
#define AM_ETH_PAUSE_LOW_THRESHOLD_MINUS256      ((uint32_t)0x00000030)  /*!< Pause time minus 256 slot times */
#define IS_AM_ETH_PAUSE_LOW_THRESHOLD(THRESHOLD) (((THRESHOLD) == AM_ETH_PAUSE_LOW_THRESHOLD_MINUS4) || \
                                               ((THRESHOLD) == AM_ETH_PAUSE_LOW_THRESHOLD_MINUS28) || \
                                               ((THRESHOLD) == AM_ETH_PAUSE_LOW_THRESHOLD_MINUS144) || \
                                               ((THRESHOLD) == AM_ETH_PAUSE_LOW_THRESHOLD_MINUS256))

/**
 * @}
 */

/** @defgroup ETH_Unicast_Pause_Frame_Detect 
 * @{
 */
#define AM_ETH_UNICAST_PAUSE_FRAME_DETECT_ENABLE  ((uint32_t)0x00000008)
#define AM_ETH_UNICAST_PAUSE_FRAME_DETECT_DISABLE ((uint32_t)0x00000000)
#define IS_AM_ETH_UNICAST_PAUSE_FRAME_DETECT(CMD) (((CMD) == AM_ETH_UNICAST_PAUSE_FRAME_DETECT_ENABLE) || \
                                                ((CMD) == AM_ETH_UNICAST_PAUSE_FRAME_DETECT_DISABLE))

/**
 * @}
 */

/** @defgroup ETH_Receive_Flow_Control 
 * @{
 */
#define AM_ETH_RECEIVE_FLOW_CRL_ENABLE       ((uint32_t)0x00000004)
#define AM_ETH_RECEIVE_FLOW_CRL_DISABLE      ((uint32_t)0x00000000)
#define IS_AM_ETH_RECEIVE_FLOW_CRL(CMD) (((CMD) == AM_ETH_RECEIVE_FLOW_CRL_ENABLE) || \
                                         ((CMD) == AM_ETH_RECEIVE_FLOW_CRL_DISABLE))

/**
 * @}
 */

/** @defgroup ETH_Transmit_Flow_Control 
 * @{
 */
#define AM_ETH_TRANSMIT_FLOW_CRL_ENABLE      ((uint32_t)0x00000002)
#define AM_ETH_TRANSMIT_FLOW_CRL_DISABLE     ((uint32_t)0x00000000)
#define IS_AM_ETH_TRANSMIT_FLOW_CRL(CMD) (((CMD) == AM_ETH_TRANSMIT_FLOW_CRL_ENABLE) || \
                                          ((CMD) == AM_ETH_TRANSMIT_FLOW_CRL_DISABLE))

/**
 * @}
 */

/** @defgroup ETH_VLAN_Tag_Comparison 
 * @{
 */
#define AM_ETH_VLAN_TAG_COMPARISON_12BIT    ((uint32_t)0x00010000)
#define AM_ETH_VLAN_TAG_COMPARISON_16BIT    ((uint32_t)0x00000000)
#define IS_AM_ETH_VLAN_TAG_COMPARISON(COMPARISON) (((COMPARISON) == AM_ETH_VLAN_TAG_COMPARISON_12BIT) || \
                                                ((COMPARISON) == AM_ETH_VLAN_TAG_COMPARISON_16BIT))
#define IS_AM_ETH_VLAN_TAG_IDENTIFIER(IDENTIFIER) ((IDENTIFIER) <= 0xFFFF)

/**
 * @}
 */

/** @defgroup ETH_MAC_Flags 
 * @{
 */
#define ETH_MAC_FLAG_TST     ((uint32_t)0x00000200)  /*!< Time stamp trigger flag (on MAC) */
#define ETH_MAC_FLAG_MMCT    ((uint32_t)0x00000040)  /*!< MMC transmit flag  */
#define ETH_MAC_FLAG_MMCR    ((uint32_t)0x00000020)  /*!< MMC receive flag */
#define ETH_MAC_FLAG_MMC     ((uint32_t)0x00000010)  /*!< MMC flag (on MAC) */
#define ETH_MAC_FLAG_PMT     ((uint32_t)0x00000008)  /*!< PMT flag (on MAC) */
#define IS_ETH_MAC_GET_FLAG(FLAG) (((FLAG) == ETH_MAC_FLAG_TST) || ((FLAG) == ETH_MAC_FLAG_MMCT) || \
                                   ((FLAG) == ETH_MAC_FLAG_MMCR) || ((FLAG) == ETH_MAC_FLAG_MMC) || \
                                   ((FLAG) == ETH_MAC_FLAG_PMT))
/**
 * @}
 */

/** @defgroup ETH_MAC_addresses 
 * @{
 */
#define AM_ETH_MAC_Address0     ((uint32_t)0x00000000)
#define AM_ETH_MAC_Address1     ((uint32_t)0x00000008)
#define AM_ETH_MAC_Address2     ((uint32_t)0x00000010)
#define AM_ETH_MAC_Address3     ((uint32_t)0x00000018)
#define IS_ETH_MAC_ADDRESS0123(ADDRESS) (((ADDRESS) == AM_ETH_MAC_Address0) || \
                                         ((ADDRESS) == AM_ETH_MAC_Address1) || \
                                         ((ADDRESS) == AM_ETH_MAC_Address2) || \
                                         ((ADDRESS) == AM_ETH_MAC_Address3))
#define IS_ETH_MAC_ADDRESS123(ADDRESS) (((ADDRESS) == AM_ETH_MAC_Address1) || \
                                        ((ADDRESS) == AM_ETH_MAC_Address2) || \
                                        ((ADDRESS) == AM_ETH_MAC_Address3))

/**--------------------------------------------------------------------------**/
/** 
 * @brief                      Ethernet DMA Descriptors defines
 */
/**--------------------------------------------------------------------------**/
/**
 * @}
 */

/** @defgroup ETH_DMA_Tx_descriptor_flags
 * @{
 */
#define IS_ETH_DMATxDESC_GET_FLAG(FLAG) (((FLAG) == AM_ETH_DMA_TX_DESC_OWN) || \
                                         ((FLAG) == AM_ETH_DMA_TX_DESC_IC) || \
                                         ((FLAG) == AM_ETH_ETH_DMA_TX_DESC_LS) || \
                                         ((FLAG) == AM_ETH_DMA_TX_DESC_FS) || \
                                         ((FLAG) == AM_ETH_DMA_TX_DESC_DC) || \
                                         ((FLAG) == AM_ETH_DMA_TX_DESC_DP) || \
                                         ((FLAG) == AM_ETH_DMA_TX_DESC_TTSE) || \
                                         ((FLAG) == AM_ETH_DMA_TX_DESC_TER) || \
                                         ((FLAG) == AM_ETH_DMA_TX_DESC_TCH) || \
                                         ((FLAG) == AM_ETH_DMA_TX_DESC_TTSS) || \
                                         ((FLAG) == AM_ETH_DMA_TX_DESC_IHE) || \
                                         ((FLAG) == AM_ETH_DMA_TX_DESC_ES) || \
                                         ((FLAG) == AM_ETH_DMA_TX_DESC_JT) || \
                                         ((FLAG) == AM_ETH_DMA_TX_DESC_FF) || \
                                         ((FLAG) == AM_ETH_DMA_TX_DESC_PCE) || \
                                         ((FLAG) == AM_ETH_DMA_TX_DESC_LCA) || \
                                         ((FLAG) == AM_ETH_DMA_TX_DESC_NC) || \
                                         ((FLAG) == AM_ETH_DMA_TX_DESC_LCO) || \
                                         ((FLAG) == AM_ETH_DMA_TX_DESC_EC) || \
                                         ((FLAG) == AM_ETH_DMA_TX_DESC_VF) || \
                                         ((FLAG) == AM_ETH_DMA_TX_DESC_CC) || \
                                         ((FLAG) == AM_ETH_DMA_TX_DESC_ED) || \
                                         ((FLAG) == AM_ETH_DMA_TX_DESC_UF) || \
                                         ((FLAG) == AM_ETH_DMA_TX_DESC_DB))

/**
 * @}
 */

/** @defgroup ETH_DMA_Tx_descriptor_Checksum_Insertion_Control
 * @{
 */
#define AM_ETH_DMATxDesc_ChecksumByPass             ((uint32_t)0x00000000)   /*!< Checksum engine bypass */
#define AM_ETH_DMATxDesc_ChecksumIPV4Header         ((uint32_t)0x00400000)   /*!< IPv4 header checksum insertion  */
#define AM_ETH_DMATxDesc_ChecksumTCPUDPICMPSegment  ((uint32_t)0x00800000)   /*!< TCP/UDP/ICMP checksum insertion. Pseudo header checksum is assumed to be present */
#define AM_ETH_DMATxDesc_ChecksumTCPUDPICMPFull     ((uint32_t)0x00C00000)   /*!< TCP/UDP/ICMP checksum fully in hardware including pseudo header */
#define IS_ETH_DMA_TXDESC_CHECKSUM(CHECKSUM) (((CHECKSUM) == AM_ETH_DMATxDesc_ChecksumByPass) || \
                                              ((CHECKSUM) == AM_ETH_DMATxDesc_ChecksumIPV4Header) || \
                                              ((CHECKSUM) == AM_ETH_DMATxDesc_ChecksumTCPUDPICMPSegment) || \
                                              ((CHECKSUM) == AM_ETH_DMATxDesc_ChecksumTCPUDPICMPFull))

/**--------------------------------------------------------------------------**/
/** 
 * @brief                           Ethernet DMA defines
 */
/**--------------------------------------------------------------------------**/
/**
 * @}
 */

/** @defgroup ETH_Drop_TCP_IP_Checksum_Error_Frame 
 * @{
 */
#define AM_ETH_DROP_TCPIP_CHECKSUM_ERROR_FRAME_ENABLE   ((uint32_t)0x00000000)
#define AM_ETH_DROP_TCPIP_CHECKSUM_ERROR_FRAME_DISABLE  ((uint32_t)0x04000000)
#define IS_AM_ETH_DROP_TCPIP_CHECKSUM_ERROR_FRAME(CMD) (((CMD) == AM_ETH_DROP_TCPIP_CHECKSUM_ERROR_FRAME_ENABLE) || \
                                               ((CMD) == AM_ETH_DROP_TCPIP_CHECKSUM_ERROR_FRAME_DISABLE))
/**
 * @}
 */

/** @defgroup ETH_Receive_Store_Forward 
 * @{
 */
#define AM_ETH_RECEIVE_STORE_FORWARD_ENABLE      ((uint32_t)0x02000000)
#define AM_ETH_RECEIVE_STORE_FORWARD_DISABLE     ((uint32_t)0x00000000)
#define IS_AM_ETH_RECEIVE_STORE_FORWARD(CMD) (((CMD) == AM_ETH_RECEIVE_STORE_FORWARD_ENABLE) || \
                                           ((CMD) == AM_ETH_RECEIVE_STORE_FORWARD_DISABLE))
/**
 * @}
 */

/** @defgroup ETH_Flush_Received_Frame 
 * @{
 */
#define AM_ETH_FLUSH_RECEIVED_FRAME_ENABLE       ((uint32_t)0x00000000)
#define AM_ETH_FLUSH_RECEIVED_FRAME_DISABLE      ((uint32_t)0x01000000)
#define IS_AM_ETH_FLUSH_RECEIVED_FRAME(CMD) (((CMD) == AM_ETH_FLUSH_RECEIVED_FRAME_ENABLE) || \
                                         ((CMD) == AM_ETH_FLUSH_RECEIVED_FRAME_DISABLE))
/**
 * @}
 */

/** @defgroup ETH_Transmit_Store_Forward 
 * @{
 */
#define AM_ETH_TRANSMIT_STORE_FORWARD_ENABLE     ((uint32_t)0x00200000)
#define AM_ETH_TRANSMIT_STORE_FORWARD_DISABLE    ((uint32_t)0x00000000)
#define IS_AM_ETH_TRANSMIT_STORE_FORWARD(CMD) (((CMD) == AM_ETH_TRANSMIT_STORE_FORWARD_ENABLE) || \
                                            ((CMD) == AM_ETH_TRANSMIT_STORE_FORWARD_DISABLE))
/**
 * @}
 */

/** @defgroup ETH_Transmit_Threshold_Control 
 * @{
 */
#define AM_ETH_TRANSMIT_THRESHOLD_CTL_64BYTES     ((uint32_t)0x00000000)  /*!< threshold level of the MTL Transmit FIFO is 64 Bytes */
#define AM_ETH_TRANSMIT_THRESHOLD_CTL_128BYTES    ((uint32_t)0x00004000)  /*!< threshold level of the MTL Transmit FIFO is 128 Bytes */
#define AM_ETH_TRANSMIT_THRESHOLD_CTL_192BYTES    ((uint32_t)0x00008000)  /*!< threshold level of the MTL Transmit FIFO is 192 Bytes */
#define AM_ETH_TRANSMIT_THRESHOLD_CTL_256BYTES    ((uint32_t)0x0000C000)  /*!< threshold level of the MTL Transmit FIFO is 256 Bytes */
#define AM_ETH_TRANSMIT_THRESHOLD_CTL_40BYTES     ((uint32_t)0x00010000)  /*!< threshold level of the MTL Transmit FIFO is 40 Bytes */
#define AM_ETH_TRANSMIT_THRESHOLD_CTL_32BYTES     ((uint32_t)0x00014000)  /*!< threshold level of the MTL Transmit FIFO is 32 Bytes */
#define AM_ETH_TRANSMIT_THRESHOLD_CTL_24BYTES     ((uint32_t)0x00018000)  /*!< threshold level of the MTL Transmit FIFO is 24 Bytes */
#define AM_ETH_TRANSMIT_THRESHOLD_CTL_16BYTES     ((uint32_t)0x0001C000)  /*!< threshold level of the MTL Transmit FIFO is 16 Bytes */
#define IS_AM_ETH_TRANSMIT_THRESHOLD_CTL(THRESHOLD) (((THRESHOLD) == AM_ETH_TRANSMIT_THRESHOLD_CTL_64BYTES) || \
                                                      ((THRESHOLD) == AM_ETH_TRANSMIT_THRESHOLD_CTL_128BYTES) || \
                                                      ((THRESHOLD) == AM_ETH_TRANSMIT_THRESHOLD_CTL_192BYTES) || \
                                                      ((THRESHOLD) == AM_ETH_TRANSMIT_THRESHOLD_CTL_256BYTES) || \
                                                      ((THRESHOLD) == AM_ETH_TRANSMIT_THRESHOLD_CTL_40BYTES) || \
                                                      ((THRESHOLD) == AM_ETH_TRANSMIT_THRESHOLD_CTL_32BYTES) || \
                                                      ((THRESHOLD) == AM_ETH_TRANSMIT_THRESHOLD_CTL_24BYTES) || \
                                                      ((THRESHOLD) == AM_ETH_TRANSMIT_THRESHOLD_CTL_16BYTES))
/**
 * @}
 */

/** @defgroup ETH_Forward_Error_Frames 
 * @{
 */
#define AM_ETH_FORWARD_ERROR_FRAMES_ENABLE       ((uint32_t)0x00000080)
#define AM_ETH_FORWARD_ERROR_FRAMES_DISABLE      ((uint32_t)0x00000000)
#define IS_AM_ETH_FORWARD_ERROR_FRAMES(CMD) (((CMD) == AM_ETH_FORWARD_ERROR_FRAMES_ENABLE) || \
                                          ((CMD) == AM_ETH_FORWARD_ERROR_FRAMES_DISABLE))
/**
 * @}
 */

/** @defgroup ETH_Forward_Undersized_Good_Frames 
 * @{
 */
#define AM_ETH_FORWARD_UNDERSIZED_GOOD_FRAMES_ENABLE   ((uint32_t)0x00000040)
#define AM_ETH_FORWARD_UNDERSIZED_GOOD_FRAMES_DISABLE  ((uint32_t)0x00000000)     
#define IS_AM_ETH_FORWARD_UNDERSIZED_GOOD_FRAMES(CMD) (((CMD) == AM_ETH_FORWARD_UNDERSIZED_GOOD_FRAMES_ENABLE) || \
                                                    ((CMD) == AM_ETH_FORWARD_UNDERSIZED_GOOD_FRAMES_DISABLE))

/**
 * @}
 */

/** @defgroup ETH_Receive_Threshold_Control 
 * @{
 */
#define AM_ETH_RECEIVE_THRESHOLD_CTL_64BYTES      ((uint32_t)0x00000000)  /*!< threshold level of the MTL Receive FIFO is 64 Bytes */
#define AM_ETH_RECEIVE_THRESHOLD_CTL_32BYTES      ((uint32_t)0x00000008)  /*!< threshold level of the MTL Receive FIFO is 32 Bytes */
#define AM_ETH_RECEIVE_THRESHOLD_CTL_96BYTES      ((uint32_t)0x00000010)  /*!< threshold level of the MTL Receive FIFO is 96 Bytes */
#define AM_ETH_RECEIVE_THRESHOLD_CTL_128BYTES     ((uint32_t)0x00000018)  /*!< threshold level of the MTL Receive FIFO is 128 Bytes */
#define IS_AM_ETH_RECEIVE_THRESHOLD_CTL(THRESHOLD) (((THRESHOLD) == AM_ETH_RECEIVE_THRESHOLD_CTL_64BYTES) || \
                                                     ((THRESHOLD) == AM_ETH_RECEIVE_THRESHOLD_CTL_32BYTES) || \
                                                     ((THRESHOLD) == AM_ETH_RECEIVE_THRESHOLD_CTL_96BYTES) || \
                                                     ((THRESHOLD) == AM_ETH_RECEIVE_THRESHOLD_CTL_128BYTES))
/**
 * @}
 */

/** @defgroup ETH_Second_Frame_Operate 
 * @{
 */
#define AM_ETH_SEC_FRAME_OPERATE_ENABLE       ((uint32_t)0x00000004)
#define AM_ETH_SEC_FRAME_OPERATE_DISABLE      ((uint32_t)0x00000000)  
#define IS_AM_ETH_SEC_FRAME_OPERATE(CMD) (((CMD) == AM_ETH_SEC_FRAME_OPERATE_ENABLE) || \
                                          ((CMD) == AM_ETH_SEC_FRAME_OPERATE_DISABLE))

/**
 * @}
 */

/** @defgroup ETH_Address_Aligned_Beats 
 * @{
 */
#define AM_ETH_ADDRESS_ALIGNED_BEATS_ENABLE      ((uint32_t)0x02000000)
#define AM_ETH_ADDRESS_ALIGNED_BEATS_DISABLE     ((uint32_t)0x00000000) 
#define IS_AM_ETH_ADDRESS_ALIGNED_BEATS(CMD) (((CMD) == AM_ETH_ADDRESS_ALIGNED_BEATS_ENABLE) || \
                                           ((CMD) == AM_ETH_ADDRESS_ALIGNED_BEATS_DISABLE))

/**
 * @}
 */

/** @defgroup ETH_Fixed_Burst 
 * @{
 */
#define AM_ETH_FIXED_BURST_ENABLE     ((uint32_t)0x00010000)
#define AM_ETH_FIXED_BURST_DISABLE    ((uint32_t)0x00000000) 
#define IS_AM_ETH_FIXED_BURST(CMD) (((CMD) == AM_ETH_FIXED_BURST_ENABLE) || \
                                 ((CMD) == AM_ETH_FIXED_BURST_DISABLE))

/**
 * @}
 */

/** @defgroup ETH_Rx_DMA_Burst_Length 
 * @{
 */
#define AM_ETH_RX_DMA_BURST_LENGTH_1BEAT          ((uint32_t)0x00020000)  /*!< maximum number of beats to be transferred in one RxDMA transaction is 1 */
#define AM_ETH_RX_DMA_BURST_LENGTH_2BEAT          ((uint32_t)0x00040000)  /*!< maximum number of beats to be transferred in one RxDMA transaction is 2 */
#define AM_ETH_RX_DMA_BURST_LENGTH_4BEAT          ((uint32_t)0x00080000)  /*!< maximum number of beats to be transferred in one RxDMA transaction is 4 */
#define AM_ETH_RX_DMA_BURST_LENGTH_8BEAT          ((uint32_t)0x00100000)  /*!< maximum number of beats to be transferred in one RxDMA transaction is 8 */
#define AM_ETH_RX_DMA_BURST_LENGTH_16BEAT         ((uint32_t)0x00200000)  /*!< maximum number of beats to be transferred in one RxDMA transaction is 16 */
#define AM_ETH_RX_DMA_BURST_LENGTH_32BEAT         ((uint32_t)0x00400000)  /*!< maximum number of beats to be transferred in one RxDMA transaction is 32 */                
#define AM_ETH_RX_DMA_BURST_LENGTH_4XPBL_4BEAT    ((uint32_t)0x01020000)  /*!< maximum number of beats to be transferred in one RxDMA transaction is 4 */
#define AM_ETH_RX_DMA_BURST_LENGTH_4XPBL_8BEAT    ((uint32_t)0x01040000)  /*!< maximum number of beats to be transferred in one RxDMA transaction is 8 */
#define AM_ETH_RX_DMA_BURST_LENGTH_4XPBL_16BEAT   ((uint32_t)0x01080000)  /*!< maximum number of beats to be transferred in one RxDMA transaction is 16 */
#define AM_ETH_RX_DMA_BURST_LENGTH_4XPBL_32BEAT   ((uint32_t)0x01100000)  /*!< maximum number of beats to be transferred in one RxDMA transaction is 32 */
#define AM_ETH_RX_DMA_BURST_LENGTH_4XPBL_64BEAT   ((uint32_t)0x01200000)  /*!< maximum number of beats to be transferred in one RxDMA transaction is 64 */
#define AM_ETH_RX_DMA_BURST_LENGTH_4XPBL_128BEAT  ((uint32_t)0x01400000)  /*!< maximum number of beats to be transferred in one RxDMA transaction is 128 */

#define IS_AM_ETH_RX_DMA_BURST_LENGTH(LENGTH) (((LENGTH) == AM_ETH_RX_DMA_BURST_LENGTH_1BEAT) || \
                                           ((LENGTH) == AM_ETH_RX_DMA_BURST_LENGTH_2BEAT) || \
                                           ((LENGTH) == AM_ETH_RX_DMA_BURST_LENGTH_4BEAT) || \
                                           ((LENGTH) == AM_ETH_RX_DMA_BURST_LENGTH_8BEAT) || \
                                           ((LENGTH) == AM_ETH_RX_DMA_BURST_LENGTH_16BEAT) || \
                                           ((LENGTH) == AM_ETH_RX_DMA_BURST_LENGTH_32BEAT) || \
                                           ((LENGTH) == AM_ETH_RX_DMA_BURST_LENGTH_4XPBL_4BEAT) || \
                                           ((LENGTH) == AM_ETH_RX_DMA_BURST_LENGTH_4XPBL_8BEAT) || \
                                           ((LENGTH) == AM_ETH_RX_DMA_BURST_LENGTH_4XPBL_16BEAT) || \
                                           ((LENGTH) == AM_ETH_RX_DMA_BURST_LENGTH_4XPBL_32BEAT) || \
                                           ((LENGTH) == AM_ETH_RX_DMA_BURST_LENGTH_4XPBL_64BEAT) || \
                                           ((LENGTH) == AM_ETH_RX_DMA_BURST_LENGTH_4XPBL_128BEAT))

/**
 * @}
 */

/** @defgroup ETH_Tx_DMA_Burst_Length 
 * @{
 */
#define AM_ETH_TX_DMA_BURST_LENGTH_1BEAT          ((uint32_t)0x00000100)  /*!< maximum number of beats to be transferred in one TxDMA (or both) transaction is 1 */
#define AM_ETH_TX_DMA_BURST_LENGTH_2BEAT          ((uint32_t)0x00000200)  /*!< maximum number of beats to be transferred in one TxDMA (or both) transaction is 2 */
#define AM_ETH_TX_DMA_BURST_LENGTH_4BEAT          ((uint32_t)0x00000400)  /*!< maximum number of beats to be transferred in one TxDMA (or both) transaction is 4 */
#define AM_ETH_TX_DMA_BURST_LENGTH_8BEAT          ((uint32_t)0x00000800)  /*!< maximum number of beats to be transferred in one TxDMA (or both) transaction is 8 */
#define AM_ETH_TX_DMA_BURST_LENGTH_16BEAT         ((uint32_t)0x00001000)  /*!< maximum number of beats to be transferred in one TxDMA (or both) transaction is 16 */
#define AM_ETH_TX_DMA_BURST_LENGTH_32BEAT         ((uint32_t)0x00002000)  /*!< maximum number of beats to be transferred in one TxDMA (or both) transaction is 32 */                
#define AM_ETH_TX_DMA_BURST_LENGTH_4XPBL_4BEAT    ((uint32_t)0x01000100)  /*!< maximum number of beats to be transferred in one TxDMA (or both) transaction is 4 */
#define AM_ETH_TX_DMA_BURST_LENGTH_4XPBL_8BEAT    ((uint32_t)0x01000200)  /*!< maximum number of beats to be transferred in one TxDMA (or both) transaction is 8 */
#define AM_ETH_TX_DMA_BURST_LENGTH_4XPBL_16BEAT   ((uint32_t)0x01000400)  /*!< maximum number of beats to be transferred in one TxDMA (or both) transaction is 16 */
#define AM_ETH_TX_DMA_BURST_LENGTH_4XPBL_32BEAT   ((uint32_t)0x01000800)  /*!< maximum number of beats to be transferred in one TxDMA (or both) transaction is 32 */
#define AM_ETH_TX_DMA_BURST_LENGTH_4XPBL_64BEAT   ((uint32_t)0x01001000)  /*!< maximum number of beats to be transferred in one TxDMA (or both) transaction is 64 */
#define AM_ETH_TX_DMA_BURST_LENGTH_4XPBL_128BEAT  ((uint32_t)0x01002000)  /*!< maximum number of beats to be transferred in one TxDMA (or both) transaction is 128 */

#define IS_AM_ETH_TX_DMA_BURST_LENGTH(LENGTH) (((LENGTH) == AM_ETH_TX_DMA_BURST_LENGTH_1BEAT) || \
                                           ((LENGTH) == AM_ETH_TX_DMA_BURST_LENGTH_2BEAT) || \
                                           ((LENGTH) == AM_ETH_TX_DMA_BURST_LENGTH_4BEAT) || \
                                           ((LENGTH) == AM_ETH_TX_DMA_BURST_LENGTH_8BEAT) || \
                                           ((LENGTH) == AM_ETH_TX_DMA_BURST_LENGTH_16BEAT) || \
                                           ((LENGTH) == AM_ETH_TX_DMA_BURST_LENGTH_32BEAT) || \
                                           ((LENGTH) == AM_ETH_TX_DMA_BURST_LENGTH_4XPBL_4BEAT) || \
                                           ((LENGTH) == AM_ETH_TX_DMA_BURST_LENGTH_4XPBL_8BEAT) || \
                                           ((LENGTH) == AM_ETH_TX_DMA_BURST_LENGTH_4XPBL_16BEAT) || \
                                           ((LENGTH) == AM_ETH_TX_DMA_BURST_LENGTH_4XPBL_32BEAT) || \
                                           ((LENGTH) == AM_ETH_TX_DMA_BURST_LENGTH_4XPBL_64BEAT) || \
                                           ((LENGTH) == AM_ETH_TX_DMA_BURST_LENGTH_4XPBL_128BEAT))
/** 
 * @brief  ETH DMA Descriptor SkipLength
 */
#define IS_AM_ETH_DMA_DESC_SKIP_LENGTH(LENGTH) ((LENGTH) <= 0x1F)

/**
 * @}
 */

/** @defgroup ETH_DMA_Arbitration 
 * @{
 */
#define AM_ETH_DMA_ARBITRATION_ROUND_ROBIN_RXTX_1_1   ((uint32_t)0x00000000)
#define AM_ETH_DMA_ARBITRATION_ROUNDROBIN_RXTX_2_1   ((uint32_t)0x00004000)
#define AM_ETH_DMA_ARBITRATION_ROUNDROBIN_RXTX_3_1   ((uint32_t)0x00008000)
#define AM_ETH_DMA_ARBITRATION_ROUNDROBIN_RXTX_4_1   ((uint32_t)0x0000C000)
#define AM_ETH_DMA_ARBITRATION_RX_PRIOR_TX             ((uint32_t)0x00000002)
#define IS_AM_ETH_DMA_ARBITRATION_ROUNDROBIN_RXTX(RATIO) (((RATIO) == AM_ETH_DMA_ARBITRATION_ROUND_ROBIN_RXTX_1_1) || \
                                                       ((RATIO) == AM_ETH_DMA_ARBITRATION_ROUNDROBIN_RXTX_2_1) || \
                                                       ((RATIO) == AM_ETH_DMA_ARBITRATION_ROUNDROBIN_RXTX_3_1) || \
                                                       ((RATIO) == AM_ETH_DMA_ARBITRATION_ROUNDROBIN_RXTX_4_1) || \
                                                       ((RATIO) == AM_ETH_DMA_ARBITRATION_RX_PRIOR_TX))

/**
 * @}
 */
/* ETHERNET MAC address offsets */
#define AM_ETH_MAC_ADDR_HBASE   (AM_ETH_MAC_BASE + 0x40)  /* ETHERNET MAC address high offset */
#define AM_ETH_MAC_ADDR_LBASE    (AM_ETH_MAC_BASE + 0x44)  /* ETHERNET MAC address low offset */

/* ETHERNET DMAOMR register Mask */
#define AM_DMAOMR_CLEAR_MASK   ((uint32_t)0xF8DE3F23)

/* ETHERNET MACMIIAR register Mask */
#define AM_MACMIIAR_CR_MASK    ((uint32_t)0xFFFFFFE3)

/* ETHERNET MACCR register Mask */
#define AM_MACCR_CLEAR_MASK    ((uint32_t)0xFF20810F)  

/* ETHERNET errors */
#define  AM_ETH_ERROR              ((uint32_t)0)
#define  AM_ETH_SUCCESS            ((uint32_t)1)

/** @defgroup ETH_Exported_Functions
 * @{
 */
uint32_t __am_eth_reg_init(am_eth_regs_conf* p_eth_init_strruct,
        uint16_t phy_addr);
void am_eth_usr_config_init(am_eth_regs_conf* p_eth_init_strruct);
void __am_eth_soft_reset(void);
flag_status __am_eth_get_soft_reset_status(void);
void am_eth_start(void);
void am_eth_stop(void);

/** 
 * @brief  PHY
 */
uint16_t __am_eth_read_phy_reg(uint16_t phy_addr, uint16_t phy_reg);
uint32_t __am_eth_write_phy_reg(uint16_t phy_addr, uint16_t phy_reg,
        uint16_t PHYValue);

/** 
 * @brief  MAC
 */
void __am_eth_mac_transmission_cmd(func_state_t new_state);
void __am_eth_mac_reception_cmd(func_state_t new_state);
void __am_eth_mac_addr_config(uint32_t mac_addr, uint8_t *addr);
/** 
 * @brief  DMA Tx/Rx descriptors
 */
void am_eth_dma_rx_desc_init(eth_dma_desc_type_t *p_dma_rx_desc_tab,
        uint8_t *rx_buff, uint32_t rx_buff_cnt);
void am_eth_dma_tx_desc_init(eth_dma_desc_type_t *p_dma_tx_desc_tab,
        uint8_t* tx_buff, uint32_t tx_buff_cnt);
uint32_t am_eth_check_frame_received(void);
uint32_t am_eth_prepare_transmit_desc(u16 frame_length);
frame_type_def_t am_eth_get_rec_frame(void);

/** 
 * @brief  DMA
 */
void __am_eth_flush_transmit_fifo(void);
void __am_eth_dma_transmission_cmd(func_state_t new_state);
void __am_eth_dma_reception_cmd(func_state_t new_state);

#if (1 == ASSERT_PARAM_ENABLE)
#define __assert_param(expr) \
    do \
	{ \
    	if(!(expr)) \
    	{ \
    	    am_kprintf("__assert_param failed at line %d in %s\r\n", __LINE__, __FILE__); \
    	    while(1); \
    	} \
    } while(0)
#else
#define __assert_param(expr)
#endif

#ifdef __cplusplus
}
#endif

#endif
