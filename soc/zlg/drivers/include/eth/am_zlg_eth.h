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

#include "stdint.h"
#include "am_delay.h"
#include "amhw_zlg_eth.h"

#define __IO  volatile

#define ASSERT_PARAM_ENABLE 1

typedef struct {
    __IO uint32_t status;   /**< \brief status */
    uint32_t ctl_buff_size; /**< \brief Control and Buffer1, Buffer2 lengths */
    uint32_t buff_1_addr;   /**< \brief Buffer1 address pointer */
    uint32_t buff_2_next_desc_addr; /**< \brief Buf2 or next desc addr pointer */
} eth_dma_desc_type_t;

typedef struct {
    uint32_t length;
    uint32_t buffer;
    __IO eth_dma_desc_type_t *descriptor;
} frame_type_def_t;

typedef struct {
    __IO eth_dma_desc_type_t *fs_rx_desc; /**< \brief First Segment Rx Desc */
    __IO eth_dma_desc_type_t *ls_rx_desc; /**< \brief Last Segment Rx Desc */
    __IO uint32_t seg_count;              /**< \brief Segment count */
} eth_dma_rx_frame_info_t;

#define AM_ZLG_ETH_MAX_PACKET_SIZE          1524    
#define AM_ZLG_ETH_HEADER                   14    
#define AM_ZLG_ETH_CRC                      4  
#define AM_ZLG_ETH_EXTRA                    2  
#define AM_ZLG_ETH_VLAN_TAG                 4  
#define AM_ZLG_ETH_MIN_ETH_PAYLOAD          46  
#define AM_ZLG_ETH_MAX_ETH_PAYLOAD          1500  
#define AM_ZLG_ETH_JUMBO_FRAME_PAYLOAD      9000  

#define CUSTOM_DRIVER_BUFFERS_CONFIG

#ifdef  CUSTOM_DRIVER_BUFFERS_CONFIG
#define AM_ZLG_ETH_ETH_RX_BUF_SIZE          AM_ZLG_ETH_MAX_PACKET_SIZE  
#define AM_ZLG_ETH_TX_BUF_SIZE              AM_ZLG_ETH_MAX_PACKET_SIZE  
#define AM_ZLG_ETH_RXBUFNB                  4                    
#define AM_ZLG_ETH_TXBUFNB                  4                    
#endif

#ifndef AM_ZLG_ETH_ETH_RX_BUF_SIZE
#define AM_ZLG_ETH_ETH_RX_BUF_SIZE         AM_ZLG_ETH_MAX_PACKET_SIZE
#endif

/** \brief Ethernet driver receive buffers are used (in a chained linked list)*/
#ifndef AM_ZLG_ETH_RXBUFNB
#define AM_ZLG_ETH_RXBUFNB                 5     
#endif

#ifndef AM_ZLG_ETH_TX_BUF_SIZE 
#define AM_ZLG_ETH_TX_BUF_SIZE             AM_ZLG_ETH_MAX_PACKET_SIZE
#endif

/** \brief ethernet driver transmit buffers are used (in a chained linked list)*/
#ifndef AM_ZLG_ETH_TXBUFNB
#define AM_ZLG_ETH_TXBUFNB                 5     
#endif

#define AM_ZLG_ETH_DMA_RX_DESC_FRAME_LEN_SHIFT         16

/** \brief OWN bit: descriptor is owned by DMA engine */
#define AM_ZLG_ETH_DMA_TX_DESC_OWN                     ((uint32_t)0x80000000)  

/** \brief Interrupt on Completion */
#define AM_ZLG_ETH_DMA_TX_DESC_IC                      ((uint32_t)0x80000000)  

/** \brief Last Segment */
#define AM_ZLG_ETH_ETH_DMA_TX_DESC_LS                  ((uint32_t)0x40000000) 

/** \brief First Segment */
#define AM_ZLG_ETH_DMA_TX_DESC_FS                      ((uint32_t)0x20000000)  

/** \brief Disable CRC */
#define AM_ZLG_ETH_DMA_TX_DESC_DC                      ((uint32_t)0x08000000)  

/** \brief Disable Padding */
#define AM_ZLG_ETH_DMA_TX_DESC_DP                      ((uint32_t)0x04000000)  

/** \brief Transmit Time Stamp Enable */
#define AM_ZLG_ETH_DMA_TX_DESC_TTSE                    ((uint32_t)0x02000000)  

/** \brief Checksum Insertion Control: 4 cases */
#define AM_ZLG_ETH_DMA_TX_DESC_CIC                     ((uint32_t)0x00C00000) 

#define AM_ZLG_ETH_DMA_TX_DESC_CIC_BYPASS              ((uint32_t)0x00000000) 
#define AM_ZLG_ETH_DMA_TX_DESC_CIC_IPV4_HEADER         ((uint32_t)0x00400000)
#define AM_ZLG_ETH_DMA_TX_DESC_CIC_TCPUDPICMP_SEGMENT  ((uint32_t)0x00800000) 
#define AM_ZLG_ETH_DMA_TX_DESC_CIC_TCPUDPICMP_FULL     ((uint32_t)0x00C00000) 
#define AM_ZLG_ETH_DMA_TX_DESC_TER                     ((uint32_t)0x00200000) 
#define AM_ZLG_ETH_DMA_TX_DESC_TCH                     ((uint32_t)0x01000000) 
#define AM_ZLG_ETH_DMA_TX_DESC_TTSS                    ((uint32_t)0x00020000) 
#define AM_ZLG_ETH_DMA_TX_DESC_IHE                     ((uint32_t)0x00010000) 
#define AM_ZLG_ETH_DMA_TX_DESC_ES                      ((uint32_t)0x00008000) 
#define AM_ZLG_ETH_DMA_TX_DESC_JT                      ((uint32_t)0x00004000) 
#define AM_ZLG_ETH_DMA_TX_DESC_FF                      ((uint32_t)0x00002000) 
#define AM_ZLG_ETH_DMA_TX_DESC_PCE                     ((uint32_t)0x00001000) 
#define AM_ZLG_ETH_DMA_TX_DESC_LCA                     ((uint32_t)0x00000800) 
#define AM_ZLG_ETH_DMA_TX_DESC_NC                      ((uint32_t)0x00000400) 
#define AM_ZLG_ETH_DMA_TX_DESC_LCO                     ((uint32_t)0x00000200) 
#define AM_ZLG_ETH_DMA_TX_DESC_EC                      ((uint32_t)0x00000100) 
#define AM_ZLG_ETH_DMA_TX_DESC_VF                      ((uint32_t)0x00000080) 
#define AM_ZLG_ETH_DMA_TX_DESC_CC                      ((uint32_t)0x00000078) 
#define AM_ZLG_ETH_DMA_TX_DESC_ED                      ((uint32_t)0x00000004) 
#define AM_ZLG_ETH_DMA_TX_DESC_UF                      ((uint32_t)0x00000002) 
#define AM_ZLG_ETH_DMA_TX_DESC_DB                      ((uint32_t)0x00000001) 

#define AM_ZLG_ETH_DMA_TX_DESC_TBS1                    ((uint32_t)0x00000fff)  

#define AM_ZLG_ETH_DMAPTPTxDesc_TTSH                   ((uint32_t)0xFFFFFFFF)

#define AM_ZLG_ETH_DMA_RX_DESC_OWN                     ((uint32_t)0x80000000)  
#define AM_ZLG_ETH_DMA_RX_DESC_AFM                     ((uint32_t)0x40000000)  
#define AM_ZLG_ETH_DMA_RX_DESC_FL                      ((uint32_t)0x3FFF0000)  
#define AM_ZLG_ETH_DMA_RX_DESC_ES                      ((uint32_t)0x00008000)  
#define AM_ZLG_ETH_DMA_RX_DESC_DE                      ((uint32_t)0x00004000)  
#define AM_ZLG_ETH_DMA_RX_DESC_SAF                     ((uint32_t)0x00002000)  
#define AM_ZLG_ETH_DMA_RX_DESC_LE                      ((uint32_t)0x00001000)  
#define AM_ZLG_ETH_DMA_RX_DESC_OE                      ((uint32_t)0x00000800)  
#define AM_ZLG_ETH_DMA_RX_DESC_VLAN                    ((uint32_t)0x00000400)  
#define AM_ZLG_ETH_DMA_RX_DESC_FS                      ((uint32_t)0x00000200)  
#define AM_ZLG_ETH_DMA_RX_DESC_LS                      ((uint32_t)0x00000100)  
#define AM_ZLG_ETH_DMA_RX_IPV4HCE                      ((uint32_t)0x00000080)  
#define AM_ZLG_ETH_DMA_RX_DESC_LC                      ((uint32_t)0x00000040)  
#define AM_ZLG_ETH_DMA_RX_DESC_FT                      ((uint32_t)0x00000020)  
#define AM_ZLG_ETH_DMA_RX_DESC_RWT                     ((uint32_t)0x00000010)  
#define AM_ZLG_ETH_DMA_RX_DESC_RE                      ((uint32_t)0x00000008)  
#define AM_ZLG_ETH_DMA_RX_DESC_DBE                     ((uint32_t)0x00000004)  
#define AM_ZLG_ETH_DMA_RX_DESC_CE                      ((uint32_t)0x00000002)  
#define AM_ZLG_ETH_DMA_RX_DESC_MAMPCE                  ((uint32_t)0x00000001)  

#define AM_ZLG_ETH_DMA_RX_DESC_RCH                     ((uint32_t)0x01000000)  

#define AM_ZLG_ETH_AUTO_NEGOTIATION_ENABLE             ((uint32_t)0x00000001)
#define AM_ZLG_ETH_AUTO_NEGOTIATION_DISABLE            ((uint32_t)0x00000000)
#define IS_AM_ZLG_ETH_AUTO_NEGOTIATION(CMD) \
                            (((CMD) == AM_ZLG_ETH_AUTO_NEGOTIATION_ENABLE) || \
                            ((CMD) == AM_ZLG_ETH_AUTO_NEGOTIATION_DISABLE))

#define AM_ZLG_ETH_WATCH_DOG_ENABLE                     ((uint32_t)0x00000000)
#define AM_ZLG_ETH_WATCH_DOG_DISABLE                    ((uint32_t)0x00800000)
#define IS_AM_ZLG_ETH_WATCH_DOG(CMD)(   \
                            ((CMD) == AM_ZLG_ETH_WATCH_DOG_ENABLE) || \
                            ((CMD) == AM_ZLG_ETH_WATCH_DOG_DISABLE))
                                
#define AM_ZLG_ETH_JABBER_ENABLE                        ((uint32_t)0x00000000)
#define AM_ZLG_ETH_JABBER_DISABLE                       ((uint32_t)0x00400000)
#define IS_AM_ZLG_ETH_JABBER(CMD)   (  \
                            ((CMD) == AM_ZLG_ETH_JABBER_ENABLE) || \
                            ((CMD) == AM_ZLG_ETH_JABBER_DISABLE))

#define AM_ZLG_ETH_INTER_FRAME_GAP_96_BIT               ((uint32_t)0x00000000)  
#define AM_ZLG_ETH_INTER_FRAME_GAP_88_BIT               ((uint32_t)0x00020000)  
#define AM_ZLG_ETH_INTER_FRAME_GAP_80_BIT               ((uint32_t)0x00040000)  
#define AM_ZLG_ETH_INTER_FRAME_GAP_72_BIT               ((uint32_t)0x00060000)  
#define AM_ZLG_ETH_INTER_FRAME_GAP_64_BIT               ((uint32_t)0x00080000)  
#define AM_ZLG_ETH_INTER_FRAME_GAP_56_BIT               ((uint32_t)0x000A0000)  
#define AM_ZLG_ETH_INTER_FRAME_GAP_48_BIT               ((uint32_t)0x000C0000)  
#define AM_ZLG_ETH_INTER_FRAME_GAP_40_BIT               ((uint32_t)0x000E0000)  
#define IS_AM_ZLG_ETH_INTER_FRAME_GAP(GAP)  ( \
                             ((GAP) == AM_ZLG_ETH_INTER_FRAME_GAP_96_BIT) || \
                             ((GAP) == AM_ZLG_ETH_INTER_FRAME_GAP_88_BIT) || \
                             ((GAP) == AM_ZLG_ETH_INTER_FRAME_GAP_80_BIT) || \
                             ((GAP) == AM_ZLG_ETH_INTER_FRAME_GAP_72_BIT) || \
                             ((GAP) == AM_ZLG_ETH_INTER_FRAME_GAP_64_BIT) || \
                             ((GAP) == AM_ZLG_ETH_INTER_FRAME_GAP_56_BIT) || \
                             ((GAP) == AM_ZLG_ETH_INTER_FRAME_GAP_48_BIT) || \
                             ((GAP) == AM_ZLG_ETH_INTER_FRAME_GAP_40_BIT))

#define AM_ZLG_ETH_CARRIER_SENSE_ENABLE                 ((uint32_t)0x00000000)
#define AM_ZLG_ETH_CARRIER_SENSE_DISABLE                ((uint32_t)0x00010000)
#define IS_AM_ZLG_ETH_CARRIER_SENSE(CMD)    (   \
                            ((CMD) == AM_ZLG_ETH_CARRIER_SENSE_ENABLE) || \
                            ((CMD) == AM_ZLG_ETH_CARRIER_SENSE_DISABLE))

#define AM_ZLG_ETH_SPEED_10M                            ((uint32_t)0x00000000)
#define AM_ZLG_ETH_SPEED_100M                           ((uint32_t)0x00004000)
#define IS_AM_ZLG_ETH_SPEED(SPEED)    ( \
                            ((SPEED) == AM_ZLG_ETH_SPEED_10M) || \
                            ((SPEED) == AM_ZLG_ETH_SPEED_100M))

#define AM_ZLG_ETH_RECEIVE_OWN_ENABLE                   ((uint32_t)0x00000000)
#define AM_ZLG_ETH_RECEIVE_OWN_DISABLE                  ((uint32_t)0x00002000)
#define IS_AM_ZLG_ETH_RECEIVE_OWN(CMD) (    \
                            ((CMD) == AM_ZLG_ETH_RECEIVE_OWN_ENABLE) || \
                            ((CMD) == AM_ZLG_ETH_RECEIVE_OWN_DISABLE))

#define AM_ZLG_ETH_LOOPBACK_MODE_ENABLE                 ((uint32_t)0x00001000)
#define AM_ZLG_ETH_LOOPBACK_MODE_DISABLE                ((uint32_t)0x00000000)
#define IS_AM_ZLG_ETH_LOOPBACK_MODE(CMD) (  \
                            ((CMD) == AM_ZLG_ETH_LOOPBACK_MODE_ENABLE) || \
                            ((CMD) == AM_ZLG_ETH_LOOPBACK_MODE_DISABLE))

#define AM_ZLG_ETH_MODE_FULLDUPLEX                      ((uint32_t)0x00000800)
#define AM_ZLG_ETH_MODE_HALFDUPLEX                      ((uint32_t)0x00000000)
#define IS_AM_ZLG_ETH_DUPLEX_MODE(MODE) (   \
                            ((MODE) == AM_ZLG_ETH_MODE_FULLDUPLEX) || \
                            ((MODE) == AM_ZLG_ETH_MODE_HALFDUPLEX))

#define AM_ZLG_ETH_CHECKSUM_OFFLOAD_ENABLE              ((uint32_t)0x00000400)
#define AM_ZLG_ETH_CHECKSUM_OFFLOAD_DISABLE             ((uint32_t)0x00000000)
#define IS_AM_ZLG_ETH_CHECKSUM_OFFLOAD(CMD) \
                            (((CMD) == AM_ZLG_ETH_CHECKSUM_OFFLOAD_ENABLE) || \
                            ((CMD) == AM_ZLG_ETH_CHECKSUM_OFFLOAD_DISABLE))

#define AM_ZLG_ETH_RETRY_TRANSMISSION_ENABLE            ((uint32_t)0x00000000)
#define AM_ZLG_ETH_RETRY_TRANSMISSION_DISABLE           ((uint32_t)0x00000200)
#define IS_AM_ZLG_ETH_RETRY_TRANSMISSION(CMD) \
                            (((CMD) == AM_ZLG_ETH_RETRY_TRANSMISSION_ENABLE) || \
                            ((CMD) == AM_ZLG_ETH_RETRY_TRANSMISSION_DISABLE))

#define AM_ZLG_ETH_AUTOMATIC_PAD_CRC_STRIP_ENABLE        ((uint32_t)0x00000080)
#define AM_ZLG_ETH_AUTOMATIC_PAD_CRC_STRIP_DISABLE       ((uint32_t)0x00000000)
#define IS_AM_ZLG_ETH_AUTOMATIC_PADCRC_STRIP(CMD) ( \
                     ((CMD) == AM_ZLG_ETH_AUTOMATIC_PAD_CRC_STRIP_ENABLE) || \
                     ((CMD) == AM_ZLG_ETH_AUTOMATIC_PAD_CRC_STRIP_DISABLE))

#define AM_ZLG_ETH_BACK_OFF_LIMIT_10                    ((uint32_t)0x00000000)
#define AM_ZLG_ETH_BACK_OFF_LIMIT_8                     ((uint32_t)0x00000020)
#define AM_ZLG_ETH_BACK_OFF_LIMIT_4                     ((uint32_t)0x00000040)
#define AM_ZLG_ETH_BACK_OFF_LIMIT_1                     ((uint32_t)0x00000060)
#define IS_AM_ZLG_ETH_BACK_OFF_LIMIT(LIMIT) (   \
                    ((LIMIT) == AM_ZLG_ETH_BACK_OFF_LIMIT_10) || \
                    ((LIMIT) == AM_ZLG_ETH_BACK_OFF_LIMIT_8)  || \
                    ((LIMIT) == AM_ZLG_ETH_BACK_OFF_LIMIT_4) || \
                    ((LIMIT) == AM_ZLG_ETH_BACK_OFF_LIMIT_1))

#define AM_ZLG_ETH_DEFERRAL_CHECK_ENABLE                ((uint32_t)0x00000010)
#define AM_ZLG_ETH_DEFERRAL_CHECK_DISABLE               ((uint32_t)0x00000000)
#define IS_AM_ZLG_ETH_DEFERRAL_CHECK(CMD) \
                   (((CMD) == AM_ZLG_ETH_DEFERRAL_CHECK_ENABLE) || \
                    ((CMD) == AM_ZLG_ETH_DEFERRAL_CHECK_DISABLE))

#define AM_ZLG_ETH_RECEIVE_ALL_ENABLE                   ((uint32_t)0x80000000)
#define AM_ZLG_ETH_RECEIVE_ALL_DISABLE                  ((uint32_t)0x00000000)
#define IS_AM_ZLG_ETH_RECEIVE_ALL(CMD) (    \
                    ((CMD) == AM_ZLG_ETH_RECEIVE_ALL_ENABLE) || \
                    ((CMD) == AM_ZLG_ETH_RECEIVE_ALL_DISABLE))

#define AM_ZLG_ETH_SOURCE_ADDR_FILTER_NORMAL_ENABLE      ((uint32_t)0x00000200)
#define AM_ZLG_ETH_SOURCE_ADDR_FILTER_INVERSE_ENABLE     ((uint32_t)0x00000300)
#define AM_ZLG_ETH_SOURCE_ADDR_FILTER_DISABLE            ((uint32_t)0x00000000)
#define IS_AM_ZLG_ETH_SOURCE_ADDR_FILTER(CMD) \
                    (((CMD) == AM_ZLG_ETH_SOURCE_ADDR_FILTER_NORMAL_ENABLE) || \
                    ((CMD) == AM_ZLG_ETH_SOURCE_ADDR_FILTER_INVERSE_ENABLE) || \
                    ((CMD) == AM_ZLG_ETH_SOURCE_ADDR_FILTER_DISABLE))

#define AM_ZLG_ETH_PASS_CTL_FRAMES_BLOCKALL              ((uint32_t)0x00000040)  
#define AM_ZLG_ETH_PASS_CTL_FRAMES_FORWARDALL            ((uint32_t)0x00000080)  
#define AM_ZLG_ETH_PASS_CONTROL_FRAMES_FORWARD_PASSED_ADDR_FILTER \
                                                       ((uint32_t)0x000000C0)
#define IS_AM_ZLG_ETH_CONTROL_FRAMES(PASS) \
        (((PASS) == AM_ZLG_ETH_PASS_CTL_FRAMES_BLOCKALL) || \
         ((PASS) == AM_ZLG_ETH_PASS_CTL_FRAMES_FORWARDALL) || \
         ((PASS) == AM_ZLG_ETH_PASS_CONTROL_FRAMES_FORWARD_PASSED_ADDR_FILTER))

#define AM_ZLG_ETH_BROADCAST_FRAMES_RECEPTION_ENABLE    ((uint32_t)0x00000000)
#define AM_ZLG_ETH_BROADCAST_FRAMES_RECEPTION_DISABLE   ((uint32_t)0x00000020)
#define IS_AM_ZLG_ETH_BROADCAST_FRAMES_RECEPTION(CMD) \
                (((CMD) == AM_ZLG_ETH_BROADCAST_FRAMES_RECEPTION_ENABLE) || \
                ((CMD) == AM_ZLG_ETH_BROADCAST_FRAMES_RECEPTION_DISABLE))
                    
#define AM_ZLG_ETH_DESTINATION_ADDR_FILTER_NORMAL       ((uint32_t)0x00000000)
#define AM_ZLG_ETH_DESTINATION_ADDR_FILTER_INVERSE      ((uint32_t)0x00000008)
#define IS_AM_ZLG_ETH_DESTINATION_ADDR_FILTER(FILTER) \
                (((FILTER) == AM_ZLG_ETH_DESTINATION_ADDR_FILTER_NORMAL) || \
                 ((FILTER) == AM_ZLG_ETH_DESTINATION_ADDR_FILTER_INVERSE))

#define AM_ZLG_ETH_PROMISCUOUS_MODE_ENABLE              ((uint32_t)0x00000001)
#define AM_ZLG_ETH_PROMISCUOUS_MODE_DISABLE             ((uint32_t)0x00000000)
#define IS_AM_ZLG_ETH_PROMISCUOUS_MODE(CMD) \
                (((CMD) == AM_ZLG_ETH_PROMISCUOUS_MODE_ENABLE) || \
                 ((CMD) == AM_ZLG_ETH_PROMISCUOUS_MODE_DISABLE))

#define AM_ZLG_ETH_MULTICAST_FRAMES_FILTER_PERFECT_HASHTABLE \
                                                        ((uint32_t)0x00000404)
#define AM_ZLG_ETH_MULTICAST_FRAMES_FILTER_HASHTABLE    \
                                                        ((uint32_t)0x00000004)
#define AM_ZLG_ETH_MULTICAST_FRAMES_FILTER_PERFECT      \
                                                        ((uint32_t)0x00000000)
#define AM_ZLG_ETH_MULTICAST_FRAMES_FILTER_NONE         \
                                                        ((uint32_t)0x00000010)
#define IS_AM_ZLG_ETH_MULTICAST_FRAMES_FILTER(FILTER) \
           (((FILTER) == AM_ZLG_ETH_MULTICAST_FRAMES_FILTER_PERFECT_HASHTABLE) || \
            ((FILTER) == AM_ZLG_ETH_MULTICAST_FRAMES_FILTER_HASHTABLE) || \
            ((FILTER) == AM_ZLG_ETH_MULTICAST_FRAMES_FILTER_PERFECT) || \
            ((FILTER) == AM_ZLG_ETH_MULTICAST_FRAMES_FILTER_NONE))

#define AM_ZLG_ETH_UNICAST_FRAMES_FILTER_PERFECT_HASHTABLE   \
                                                        ((uint32_t)0x00000402)
#define AM_ZLG_ETH_UNICAST_FRAMES_FILTER_HASHTABLE      \
                                                        ((uint32_t)0x00000002)
#define AM_ZLG_ETH_UNICAST_FRAMES_FILTER_PERFECT        \
                                                        ((uint32_t)0x00000000)
#define IS_AM_ZLG_ETH_UNICAST_FRAMES_FILTER(FILTER) \
           (((FILTER) == AM_ZLG_ETH_UNICAST_FRAMES_FILTER_PERFECT_HASHTABLE) || \
            ((FILTER) == AM_ZLG_ETH_UNICAST_FRAMES_FILTER_HASHTABLE) || \
            ((FILTER) == AM_ZLG_ETH_UNICAST_FRAMES_FILTER_PERFECT))

#define IS_AM_ZLG_ETH_PAUSE_TIME(TIME)                      ((TIME) <= 0xFFFF)

#define AM_ZLG_ETH_ZERO_QUANTA_PAUSE_ENABLE              ((uint32_t)0x00000000)
#define AM_ZLG_ETH_ZERO_QUANTA_PAUSE_DISABLE             ((uint32_t)0x00000080)
#define IS_AM_ZLG_ETH_ZERO_QUANTA_PAUSE(CMD)    \
                            (((CMD) == AM_ZLG_ETH_ZERO_QUANTA_PAUSE_ENABLE) || \
                             ((CMD) == AM_ZLG_ETH_ZERO_QUANTA_PAUSE_DISABLE))

#define AM_ZLG_ETH_PAUSE_LOW_THRESHOLD_MINUS4             ((uint32_t)0x00000000)  
#define AM_ZLG_ETH_PAUSE_LOW_THRESHOLD_MINUS28            ((uint32_t)0x00000010)  
#define AM_ZLG_ETH_PAUSE_LOW_THRESHOLD_MINUS144           ((uint32_t)0x00000020)  
#define AM_ZLG_ETH_PAUSE_LOW_THRESHOLD_MINUS256           ((uint32_t)0x00000030)  
#define IS_AM_ZLG_ETH_PAUSE_LOW_THRESHOLD(THRESHOLD)    \
                    (((THRESHOLD) == AM_ZLG_ETH_PAUSE_LOW_THRESHOLD_MINUS4) || \
                     ((THRESHOLD) == AM_ZLG_ETH_PAUSE_LOW_THRESHOLD_MINUS28) || \
                     ((THRESHOLD) == AM_ZLG_ETH_PAUSE_LOW_THRESHOLD_MINUS144) || \
                     ((THRESHOLD) == AM_ZLG_ETH_PAUSE_LOW_THRESHOLD_MINUS256))

#define AM_ZLG_ETH_UNICAST_PAUSE_FRAME_DETECT_ENABLE      ((uint32_t)0x00000008)
#define AM_ZLG_ETH_UNICAST_PAUSE_FRAME_DETECT_DISABLE     ((uint32_t)0x00000000)
#define IS_AM_ZLG_ETH_UNICAST_PAUSE_FRAME_DETECT(CMD)   \
                    (((CMD) == AM_ZLG_ETH_UNICAST_PAUSE_FRAME_DETECT_ENABLE) || \
                     ((CMD) == AM_ZLG_ETH_UNICAST_PAUSE_FRAME_DETECT_DISABLE))

#define AM_ZLG_ETH_RECEIVE_FLOW_CRL_ENABLE                ((uint32_t)0x00000004)
#define AM_ZLG_ETH_RECEIVE_FLOW_CRL_DISABLE               ((uint32_t)0x00000000)
#define IS_AM_ZLG_ETH_RECEIVE_FLOW_CRL(CMD) \
                    (((CMD) == AM_ZLG_ETH_RECEIVE_FLOW_CRL_ENABLE) || \
                     ((CMD) == AM_ZLG_ETH_RECEIVE_FLOW_CRL_DISABLE))

#define AM_ZLG_ETH_TRANSMIT_FLOW_CRL_ENABLE               ((uint32_t)0x00000002)
#define AM_ZLG_ETH_TRANSMIT_FLOW_CRL_DISABLE              ((uint32_t)0x00000000)
#define IS_AM_ZLG_ETH_TRANSMIT_FLOW_CRL(CMD)    \
                    (((CMD) == AM_ZLG_ETH_TRANSMIT_FLOW_CRL_ENABLE) || \
                     ((CMD) == AM_ZLG_ETH_TRANSMIT_FLOW_CRL_DISABLE))

#define AM_ZLG_ETH_VLAN_TAG_COMPARISON_12BIT              ((uint32_t)0x00010000)
#define AM_ZLG_ETH_VLAN_TAG_COMPARISON_16BIT              ((uint32_t)0x00000000)
#define IS_AM_ZLG_ETH_VLAN_TAG_COMPARISON(COMPARISON)   \
                    (((COMPARISON) == AM_ZLG_ETH_VLAN_TAG_COMPARISON_12BIT) || \
                     ((COMPARISON) == AM_ZLG_ETH_VLAN_TAG_COMPARISON_16BIT))
                     
#define IS_AM_ZLG_ETH_VLAN_TAG_IDENTIFIER(IDENTIFIER)   ((IDENTIFIER) <= 0xFFFF)

#define ETH_MAC_FLAG_TST                                ((uint32_t)0x00000200)  
#define ETH_MAC_FLAG_MMCT                               ((uint32_t)0x00000040)  
#define ETH_MAC_FLAG_MMCR                               ((uint32_t)0x00000020)  
#define ETH_MAC_FLAG_MMC                                ((uint32_t)0x00000010)  
#define ETH_MAC_FLAG_PMT                                ((uint32_t)0x00000008)  
#define IS_ETH_MAC_GET_FLAG(FLAG) \
            (((FLAG) == ETH_MAC_FLAG_TST) || ((FLAG) == ETH_MAC_FLAG_MMCT) || \
            ((FLAG) == ETH_MAC_FLAG_MMCR) || ((FLAG) == ETH_MAC_FLAG_MMC) || \
            ((FLAG) == ETH_MAC_FLAG_PMT))

#define AM_ZLG_ETH_MAC_Address0     ((uint32_t)0x00000000)
#define AM_ZLG_ETH_MAC_Address1     ((uint32_t)0x00000008)
#define AM_ZLG_ETH_MAC_Address2     ((uint32_t)0x00000010)
#define AM_ZLG_ETH_MAC_Address3     ((uint32_t)0x00000018)
#define IS_ETH_MAC_ADDRESS0123(ADDRESS) (   \
                                ((ADDRESS) == AM_ZLG_ETH_MAC_Address0) || \
                                ((ADDRESS) == AM_ZLG_ETH_MAC_Address1) || \
                                ((ADDRESS) == AM_ZLG_ETH_MAC_Address2) || \
                                ((ADDRESS) == AM_ZLG_ETH_MAC_Address3))
#define IS_ETH_MAC_ADDRESS123(ADDRESS) (    \
                                ((ADDRESS) == AM_ZLG_ETH_MAC_Address1) || \
                                ((ADDRESS) == AM_ZLG_ETH_MAC_Address2) || \
                                ((ADDRESS) == AM_ZLG_ETH_MAC_Address3))

#define IS_ETH_DMATxDESC_GET_FLAG(FLAG) (   \
                                ((FLAG) == AM_ZLG_ETH_DMA_TX_DESC_OWN) || \
                                ((FLAG) == AM_ZLG_ETH_DMA_TX_DESC_IC) || \
                                ((FLAG) == AM_ZLG_ETH_ETH_DMA_TX_DESC_LS) || \
                                ((FLAG) == AM_ZLG_ETH_DMA_TX_DESC_FS) || \
                                ((FLAG) == AM_ZLG_ETH_DMA_TX_DESC_DC) || \
                                ((FLAG) == AM_ZLG_ETH_DMA_TX_DESC_DP) || \
                                ((FLAG) == AM_ZLG_ETH_DMA_TX_DESC_TTSE) || \
                                ((FLAG) == AM_ZLG_ETH_DMA_TX_DESC_TER) || \
                                ((FLAG) == AM_ZLG_ETH_DMA_TX_DESC_TCH) || \
                                ((FLAG) == AM_ZLG_ETH_DMA_TX_DESC_TTSS) || \
                                ((FLAG) == AM_ZLG_ETH_DMA_TX_DESC_IHE) || \
                                ((FLAG) == AM_ZLG_ETH_DMA_TX_DESC_ES) || \
                                ((FLAG) == AM_ZLG_ETH_DMA_TX_DESC_JT) || \
                                ((FLAG) == AM_ZLG_ETH_DMA_TX_DESC_FF) || \
                                ((FLAG) == AM_ZLG_ETH_DMA_TX_DESC_PCE) || \
                                ((FLAG) == AM_ZLG_ETH_DMA_TX_DESC_LCA) || \
                                ((FLAG) == AM_ZLG_ETH_DMA_TX_DESC_NC) || \
                                ((FLAG) == AM_ZLG_ETH_DMA_TX_DESC_LCO) || \
                                ((FLAG) == AM_ZLG_ETH_DMA_TX_DESC_EC) || \
                                ((FLAG) == AM_ZLG_ETH_DMA_TX_DESC_VF) || \
                                ((FLAG) == AM_ZLG_ETH_DMA_TX_DESC_CC) || \
                                ((FLAG) == AM_ZLG_ETH_DMA_TX_DESC_ED) || \
                                ((FLAG) == AM_ZLG_ETH_DMA_TX_DESC_UF) || \
                                ((FLAG) == AM_ZLG_ETH_DMA_TX_DESC_DB))

#define AM_ZLG_ETH_DMATxDesc_ChecksumByPass             ((uint32_t)0x00000000)  
#define AM_ZLG_ETH_DMATxDesc_ChecksumIPV4Header         ((uint32_t)0x00400000)  
#define AM_ZLG_ETH_DMATxDesc_ChecksumTCPUDPICMPSegment  ((uint32_t)0x00800000)  
#define AM_ZLG_ETH_DMATxDesc_ChecksumTCPUDPICMPFull     ((uint32_t)0x00C00000)  
#define IS_ETH_DMA_TXDESC_CHECKSUM(CHECKSUM)    \
           (((CHECKSUM) == AM_ZLG_ETH_DMATxDesc_ChecksumByPass) || \
            ((CHECKSUM) == AM_ZLG_ETH_DMATxDesc_ChecksumIPV4Header) || \
            ((CHECKSUM) == AM_ZLG_ETH_DMATxDesc_ChecksumTCPUDPICMPSegment) || \
            ((CHECKSUM) == AM_ZLG_ETH_DMATxDesc_ChecksumTCPUDPICMPFull))

/*******************************************************************************
  Ethernet DMA defines
*******************************************************************************/
#define AM_ZLG_ETH_DROP_TCPIP_CHECKSUM_ERROR_FRAME_ENABLE ((uint32_t)0x00000000)
#define AM_ZLG_ETH_DROP_TCPIP_CHECKSUM_ERROR_FRAME_DISABLE ((uint32_t)0x04000000)
#define IS_AM_ZLG_ETH_DROP_TCPIP_CHECKSUM_ERROR_FRAME(CMD)  \
               (((CMD) == AM_ZLG_ETH_DROP_TCPIP_CHECKSUM_ERROR_FRAME_ENABLE) || \
                ((CMD) == AM_ZLG_ETH_DROP_TCPIP_CHECKSUM_ERROR_FRAME_DISABLE))

#define AM_ZLG_ETH_RECEIVE_STORE_FORWARD_ENABLE           ((uint32_t)0x02000000)
#define AM_ZLG_ETH_RECEIVE_STORE_FORWARD_DISABLE          ((uint32_t)0x00000000)
#define IS_AM_ZLG_ETH_RECEIVE_STORE_FORWARD(CMD) (  \
                ((CMD) == AM_ZLG_ETH_RECEIVE_STORE_FORWARD_ENABLE) || \
                ((CMD) == AM_ZLG_ETH_RECEIVE_STORE_FORWARD_DISABLE))

#define AM_ZLG_ETH_FLUSH_RECEIVED_FRAME_ENABLE            ((uint32_t)0x00000000)
#define AM_ZLG_ETH_FLUSH_RECEIVED_FRAME_DISABLE           ((uint32_t)0x01000000)
#define IS_AM_ZLG_ETH_FLUSH_RECEIVED_FRAME(CMD) (   \
                ((CMD) == AM_ZLG_ETH_FLUSH_RECEIVED_FRAME_ENABLE) || \
                ((CMD) == AM_ZLG_ETH_FLUSH_RECEIVED_FRAME_DISABLE))

#define AM_ZLG_ETH_TRANSMIT_STORE_FORWARD_ENABLE          ((uint32_t)0x00200000)
#define AM_ZLG_ETH_TRANSMIT_STORE_FORWARD_DISABLE         ((uint32_t)0x00000000)
#define IS_AM_ZLG_ETH_TRANSMIT_STORE_FORWARD(CMD) ( \
                ((CMD) == AM_ZLG_ETH_TRANSMIT_STORE_FORWARD_ENABLE) || \
                ((CMD) == AM_ZLG_ETH_TRANSMIT_STORE_FORWARD_DISABLE))

#define AM_ZLG_ETH_TRANSMIT_THRESHOLD_CTL_64BYTES         ((uint32_t)0x00000000)  
#define AM_ZLG_ETH_TRANSMIT_THRESHOLD_CTL_128BYTES        ((uint32_t)0x00004000)  
#define AM_ZLG_ETH_TRANSMIT_THRESHOLD_CTL_192BYTES        ((uint32_t)0x00008000)  
#define AM_ZLG_ETH_TRANSMIT_THRESHOLD_CTL_256BYTES        ((uint32_t)0x0000C000)  
#define AM_ZLG_ETH_TRANSMIT_THRESHOLD_CTL_40BYTES         ((uint32_t)0x00010000)  
#define AM_ZLG_ETH_TRANSMIT_THRESHOLD_CTL_32BYTES         ((uint32_t)0x00014000)  
#define AM_ZLG_ETH_TRANSMIT_THRESHOLD_CTL_24BYTES         ((uint32_t)0x00018000)  
#define AM_ZLG_ETH_TRANSMIT_THRESHOLD_CTL_16BYTES         ((uint32_t)0x0001C000)  
#define IS_AM_ZLG_ETH_TRANSMIT_THRESHOLD_CTL(THRESHOLD) (   \
                ((THRESHOLD) == AM_ZLG_ETH_TRANSMIT_THRESHOLD_CTL_64BYTES) || \
                ((THRESHOLD) == AM_ZLG_ETH_TRANSMIT_THRESHOLD_CTL_128BYTES) || \
                ((THRESHOLD) == AM_ZLG_ETH_TRANSMIT_THRESHOLD_CTL_192BYTES) || \
                ((THRESHOLD) == AM_ZLG_ETH_TRANSMIT_THRESHOLD_CTL_256BYTES) || \
                ((THRESHOLD) == AM_ZLG_ETH_TRANSMIT_THRESHOLD_CTL_40BYTES) || \
                ((THRESHOLD) == AM_ZLG_ETH_TRANSMIT_THRESHOLD_CTL_32BYTES) || \
                ((THRESHOLD) == AM_ZLG_ETH_TRANSMIT_THRESHOLD_CTL_24BYTES) || \
                ((THRESHOLD) == AM_ZLG_ETH_TRANSMIT_THRESHOLD_CTL_16BYTES))

#define AM_ZLG_ETH_FORWARD_ERROR_FRAMES_ENABLE            ((uint32_t)0x00000080)
#define AM_ZLG_ETH_FORWARD_ERROR_FRAMES_DISABLE           ((uint32_t)0x00000000)
#define IS_AM_ZLG_ETH_FORWARD_ERROR_FRAMES(CMD) (   \
                ((CMD) == AM_ZLG_ETH_FORWARD_ERROR_FRAMES_ENABLE) || \
                ((CMD) == AM_ZLG_ETH_FORWARD_ERROR_FRAMES_DISABLE))

#define AM_ZLG_ETH_FORWARD_UNDERSIZED_GOOD_FRAMES_ENABLE  ((uint32_t)0x00000040)
#define AM_ZLG_ETH_FORWARD_UNDERSIZED_GOOD_FRAMES_DISABLE ((uint32_t)0x00000000)     
#define IS_AM_ZLG_ETH_FORWARD_UNDERSIZED_GOOD_FRAMES(CMD) ( \
                ((CMD) == AM_ZLG_ETH_FORWARD_UNDERSIZED_GOOD_FRAMES_ENABLE) || \
                ((CMD) == AM_ZLG_ETH_FORWARD_UNDERSIZED_GOOD_FRAMES_DISABLE))

#define AM_ZLG_ETH_RECEIVE_THRESHOLD_CTL_64BYTES      ((uint32_t)0x00000000)  
#define AM_ZLG_ETH_RECEIVE_THRESHOLD_CTL_32BYTES      ((uint32_t)0x00000008)  
#define AM_ZLG_ETH_RECEIVE_THRESHOLD_CTL_96BYTES      ((uint32_t)0x00000010)  
#define AM_ZLG_ETH_RECEIVE_THRESHOLD_CTL_128BYTES     ((uint32_t)0x00000018)  
#define IS_AM_ZLG_ETH_RECEIVE_THRESHOLD_CTL(THRESHOLD) (    \
                ((THRESHOLD) == AM_ZLG_ETH_RECEIVE_THRESHOLD_CTL_64BYTES) || \
                ((THRESHOLD) == AM_ZLG_ETH_RECEIVE_THRESHOLD_CTL_32BYTES) || \
                ((THRESHOLD) == AM_ZLG_ETH_RECEIVE_THRESHOLD_CTL_96BYTES) || \
                ((THRESHOLD) == AM_ZLG_ETH_RECEIVE_THRESHOLD_CTL_128BYTES))

#define AM_ZLG_ETH_SEC_FRAME_OPERATE_ENABLE           ((uint32_t)0x00000004)
#define AM_ZLG_ETH_SEC_FRAME_OPERATE_DISABLE          ((uint32_t)0x00000000)  
#define IS_AM_ZLG_ETH_SEC_FRAME_OPERATE(CMD) (  \
                ((CMD) == AM_ZLG_ETH_SEC_FRAME_OPERATE_ENABLE) || \
                ((CMD) == AM_ZLG_ETH_SEC_FRAME_OPERATE_DISABLE))

#define AM_ZLG_ETH_ADDRESS_ALIGNED_BEATS_ENABLE       ((uint32_t)0x02000000)
#define AM_ZLG_ETH_ADDRESS_ALIGNED_BEATS_DISABLE      ((uint32_t)0x00000000) 
#define IS_AM_ZLG_ETH_ADDRESS_ALIGNED_BEATS(CMD) (  \
                ((CMD) == AM_ZLG_ETH_ADDRESS_ALIGNED_BEATS_ENABLE) || \
                ((CMD) == AM_ZLG_ETH_ADDRESS_ALIGNED_BEATS_DISABLE))

#define AM_ZLG_ETH_FIXED_BURST_ENABLE     ((uint32_t)0x00010000)
#define AM_ZLG_ETH_FIXED_BURST_DISABLE    ((uint32_t)0x00000000) 
#define IS_AM_ZLG_ETH_FIXED_BURST(CMD) (    \
                ((CMD) == AM_ZLG_ETH_FIXED_BURST_ENABLE) || \
                ((CMD) == AM_ZLG_ETH_FIXED_BURST_DISABLE))

#define AM_ZLG_ETH_RX_DMA_BURST_LENGTH_1BEAT          ((uint32_t)0x00020000)   
#define AM_ZLG_ETH_RX_DMA_BURST_LENGTH_2BEAT          ((uint32_t)0x00040000)  
#define AM_ZLG_ETH_RX_DMA_BURST_LENGTH_4BEAT          ((uint32_t)0x00080000)  
#define AM_ZLG_ETH_RX_DMA_BURST_LENGTH_8BEAT          ((uint32_t)0x00100000)  
#define AM_ZLG_ETH_RX_DMA_BURST_LENGTH_16BEAT         ((uint32_t)0x00200000)  
#define AM_ZLG_ETH_RX_DMA_BURST_LENGTH_32BEAT         ((uint32_t)0x00400000)  
#define AM_ZLG_ETH_RX_DMA_BURST_LENGTH_4XPBL_4BEAT    ((uint32_t)0x01020000)  
#define AM_ZLG_ETH_RX_DMA_BURST_LENGTH_4XPBL_8BEAT    ((uint32_t)0x01040000)  
#define AM_ZLG_ETH_RX_DMA_BURST_LENGTH_4XPBL_16BEAT   ((uint32_t)0x01080000)  
#define AM_ZLG_ETH_RX_DMA_BURST_LENGTH_4XPBL_32BEAT   ((uint32_t)0x01100000)  
#define AM_ZLG_ETH_RX_DMA_BURST_LENGTH_4XPBL_64BEAT   ((uint32_t)0x01200000)  
#define AM_ZLG_ETH_RX_DMA_BURST_LENGTH_4XPBL_128BEAT  ((uint32_t)0x01400000)  

#define IS_AM_ZLG_ETH_RX_DMA_BURST_LENGTH(LENGTH) ( \
                ((LENGTH) == AM_ZLG_ETH_RX_DMA_BURST_LENGTH_1BEAT) || \
                ((LENGTH) == AM_ZLG_ETH_RX_DMA_BURST_LENGTH_2BEAT) || \
                ((LENGTH) == AM_ZLG_ETH_RX_DMA_BURST_LENGTH_4BEAT) || \
                ((LENGTH) == AM_ZLG_ETH_RX_DMA_BURST_LENGTH_8BEAT) || \
                ((LENGTH) == AM_ZLG_ETH_RX_DMA_BURST_LENGTH_16BEAT) || \
                ((LENGTH) == AM_ZLG_ETH_RX_DMA_BURST_LENGTH_32BEAT) || \
                ((LENGTH) == AM_ZLG_ETH_RX_DMA_BURST_LENGTH_4XPBL_4BEAT) || \
                ((LENGTH) == AM_ZLG_ETH_RX_DMA_BURST_LENGTH_4XPBL_8BEAT) || \
                ((LENGTH) == AM_ZLG_ETH_RX_DMA_BURST_LENGTH_4XPBL_16BEAT) || \
                ((LENGTH) == AM_ZLG_ETH_RX_DMA_BURST_LENGTH_4XPBL_32BEAT) || \
                ((LENGTH) == AM_ZLG_ETH_RX_DMA_BURST_LENGTH_4XPBL_64BEAT) || \
                ((LENGTH) == AM_ZLG_ETH_RX_DMA_BURST_LENGTH_4XPBL_128BEAT))

#define AM_ZLG_ETH_TX_DMA_BURST_LENGTH_1BEAT          ((uint32_t)0x00000100)   
#define AM_ZLG_ETH_TX_DMA_BURST_LENGTH_2BEAT          ((uint32_t)0x00000200)  
#define AM_ZLG_ETH_TX_DMA_BURST_LENGTH_4BEAT          ((uint32_t)0x00000400)  
#define AM_ZLG_ETH_TX_DMA_BURST_LENGTH_8BEAT          ((uint32_t)0x00000800)  
#define AM_ZLG_ETH_TX_DMA_BURST_LENGTH_16BEAT         ((uint32_t)0x00001000)  
#define AM_ZLG_ETH_TX_DMA_BURST_LENGTH_32BEAT         ((uint32_t)0x00002000)  
#define AM_ZLG_ETH_TX_DMA_BURST_LENGTH_4XPBL_4BEAT    ((uint32_t)0x01000100)  
#define AM_ZLG_ETH_TX_DMA_BURST_LENGTH_4XPBL_8BEAT    ((uint32_t)0x01000200)  
#define AM_ZLG_ETH_TX_DMA_BURST_LENGTH_4XPBL_16BEAT   ((uint32_t)0x01000400)  
#define AM_ZLG_ETH_TX_DMA_BURST_LENGTH_4XPBL_32BEAT   ((uint32_t)0x01000800)  
#define AM_ZLG_ETH_TX_DMA_BURST_LENGTH_4XPBL_64BEAT   ((uint32_t)0x01001000)  
#define AM_ZLG_ETH_TX_DMA_BURST_LENGTH_4XPBL_128BEAT  ((uint32_t)0x01002000)  

#define IS_AM_ZLG_ETH_TX_DMA_BURST_LENGTH(LENGTH) ( \
                ((LENGTH) == AM_ZLG_ETH_TX_DMA_BURST_LENGTH_1BEAT) || \
                ((LENGTH) == AM_ZLG_ETH_TX_DMA_BURST_LENGTH_2BEAT) || \
                ((LENGTH) == AM_ZLG_ETH_TX_DMA_BURST_LENGTH_4BEAT) || \
                ((LENGTH) == AM_ZLG_ETH_TX_DMA_BURST_LENGTH_8BEAT) || \
                ((LENGTH) == AM_ZLG_ETH_TX_DMA_BURST_LENGTH_16BEAT) || \
                ((LENGTH) == AM_ZLG_ETH_TX_DMA_BURST_LENGTH_32BEAT) || \
                ((LENGTH) == AM_ZLG_ETH_TX_DMA_BURST_LENGTH_4XPBL_4BEAT) || \
                ((LENGTH) == AM_ZLG_ETH_TX_DMA_BURST_LENGTH_4XPBL_8BEAT) || \
                ((LENGTH) == AM_ZLG_ETH_TX_DMA_BURST_LENGTH_4XPBL_16BEAT) || \
                ((LENGTH) == AM_ZLG_ETH_TX_DMA_BURST_LENGTH_4XPBL_32BEAT) || \
                ((LENGTH) == AM_ZLG_ETH_TX_DMA_BURST_LENGTH_4XPBL_64BEAT) || \
                ((LENGTH) == AM_ZLG_ETH_TX_DMA_BURST_LENGTH_4XPBL_128BEAT))
/** 
 * @brief  ETH DMA Descriptor SkipLength
 */
#define IS_AM_ZLG_ETH_DMA_DESC_SKIP_LENGTH(LENGTH) ((LENGTH) <= 0x1F)

/**
 * @}
 */

/** @defgroup ETH_DMA_Arbitration 
 * @{
 */
#define AM_ZLG_ETH_DMA_ARBITRATION_ROUND_ROBIN_RXTX_1_1  ((uint32_t)0x00000000)
#define AM_ZLG_ETH_DMA_ARBITRATION_ROUNDROBIN_RXTX_2_1   ((uint32_t)0x00004000)
#define AM_ZLG_ETH_DMA_ARBITRATION_ROUNDROBIN_RXTX_3_1   ((uint32_t)0x00008000)
#define AM_ZLG_ETH_DMA_ARBITRATION_ROUNDROBIN_RXTX_4_1   ((uint32_t)0x0000C000)
#define AM_ZLG_ETH_DMA_ARBITRATION_RX_PRIOR_TX           ((uint32_t)0x00000002)
#define IS_AM_ZLG_ETH_DMA_ARBITRATION_ROUNDROBIN_RXTX(RATIO) (  \
                ((RATIO) == AM_ZLG_ETH_DMA_ARBITRATION_ROUND_ROBIN_RXTX_1_1) || \
                ((RATIO) == AM_ZLG_ETH_DMA_ARBITRATION_ROUNDROBIN_RXTX_2_1) || \
                ((RATIO) == AM_ZLG_ETH_DMA_ARBITRATION_ROUNDROBIN_RXTX_3_1) || \
                ((RATIO) == AM_ZLG_ETH_DMA_ARBITRATION_ROUNDROBIN_RXTX_4_1) || \
                ((RATIO) == AM_ZLG_ETH_DMA_ARBITRATION_RX_PRIOR_TX))

#define AM_ZLG_ETH_PERIPH_BASE            ((uint32_t)0x40000000) 
#define AM_ZLG_ETH_AHB1PERIPH_BASE        (AM_ZLG_ETH_PERIPH_BASE + 0x00020000)
#define AM_ZLG_ETH_BASE                   (AM_ZLG_ETH_AHB1PERIPH_BASE + 0x8000)
#define AM_ZLG_ETH_MAC_BASE               (AM_ZLG_ETH_BASE)
#define AM_ZLG_ETH_MACCR_TE                   ((uint32_t)0x00000008)
#define AM_ZLG_ETH_MACCR_RE                   ((uint32_t)0x00000004)

#define AM_ZLG_ETH_MACHTHR_HTH                ((uint32_t)0xFFFFFFFF)

#define AM_ZLG_ETH_MACHTLR_HTL                ((uint32_t)0xFFFFFFFF) 

#define AM_ZLG_ETH_MACMIIAR_PA                ((uint32_t)0x0000F800) 
#define AM_ZLG_ETH_MACMIIAR_MR                ((uint32_t)0x000007C0)  
#define AM_ZLG_ETH_MACMIIAR_CR_DIV42          ((uint32_t)0x00000000)  
#define AM_ZLG_ETH_MACMIIAR_CR_DIV62          ((uint32_t)0x00000004)  
#define AM_ZLG_ETH_MACMIIAR_CR_DIV16          ((uint32_t)0x00000008)  
#define AM_ZLG_ETH_MACMIIAR_CR_DIV26          ((uint32_t)0x0000000C)  
#define AM_ZLG_ETH_MACMIIAR_CR_DIV102         ((uint32_t)0x00000010)  
#define AM_ZLG_ETH_MACMIIAR_MW                ((uint32_t)0x00000002)  
#define AM_ZLG_ETH_MACMIIAR_MB                ((uint32_t)0x00000001)   
#define AM_ZLG_ETH_DMABMR_USP                 ((uint32_t)0x00800000)  
#define AM_ZLG_ETH_DMABMR_EDE                 ((uint32_t)0x00000080)  
#define AM_ZLG_ETH_DMABMR_SR                  ((uint32_t)0x00000001)  
#define AM_ZLG_ETH_DMASR_RBUS                 ((uint32_t)0x00000080)  
#define AM_ZLG_ETH_DMASR_TUS                  ((uint32_t)0x00000020)  
#define AM_ZLG_ETH_DMASR_TBUS                 ((uint32_t)0x00000004)  
#define AM_ZLG_ETH_DMAOMR_FTF                 ((uint32_t)0x00100000)  
#define AM_ZLG_ETH_DMAOMR_ST                  ((uint32_t)0x00002000)  
#define AM_ZLG_ETH_DMAOMR_SR                  ((uint32_t)0x00000002)  

/** \brief ETHERNET MAC address offsets */
#define AM_ZLG_ETH_MAC_ADDR_HBASE             (AM_ZLG_ETH_MAC_BASE + 0x40)
#define AM_ZLG_ETH_MAC_ADDR_LBASE             (AM_ZLG_ETH_MAC_BASE + 0x44)

/** \brief ETHERNET DMAOMR register Mask */
#define AM_ZLG_ETH_DMAOMR_CLEAR_MASK          ((uint32_t)0xF8DE3F23)

/** \brief ETHERNET MACMIIAR register Mask */
#define AM_ZLG_ETH_MACMIIAR_CR_MASK           ((uint32_t)0xFFFFFFE3)

/** \brief ETHERNET MACCR register Mask */
#define AM_ZLG_ETH_MACCR_CLEAR_MASK           ((uint32_t)0xFF20810F)  

/** \brief ETHERNET errors */
#define  AM_ZLG_ETH_ERROR                     ((uint32_t)0)
#define  AM_ZLG_ETH_SUCCESS                   ((uint32_t)1)

void am_zlg_eth_init(amhw_zlg_eth_t *p_hw_eth);
void am_zlg_eth_check_link_status(amhw_zlg_eth_t *p_hw_eth, uint16_t phy_addr);

typedef enum am_zlg_eth_link_st {
    AM_ZLG_ETH_LINK_ST_UP    = 0x00,    
    AM_ZLG_ETH_LINK_ST_DOWN  = 0x01   
}am_zlg_eth_link_st_t;

extern __IO am_zlg_eth_link_st_t g_eth_link_status;

#define USE_Delay

#ifdef USE_Delay
#define AM_ZLG_ETH_REG_WRITE_DELAY ((uint32_t)0x00000001)
#define AM_ZLG_ETH_DELAY(value)    am_mdelay(value);
#else
#define AM_ZLG_ETH_REG_WRITE_DELAY ((uint32_t)0x0000FFFF)
#define AM_ZLG_ETH_DELAY(value)
#endif

#if (1 == ASSERT_PARAM_ENABLE)
#define assert_param(expr) \
    do \
	{ \
    	if(!(expr)) \
    	{ \
    	    printf("assert_param failed at line %d in %s\r\n",    \
    	                                        __LINE__, __FILE__); \
    	    while(1); \
    	} \
    } while(0)
#else
#define assert_param(expr)
#endif

void am_zlg_eth_start(amhw_zlg_eth_t *p_hw_eth);
void am_zlg_eth_stop(amhw_zlg_eth_t *p_hw_eth);

uint32_t am_zlg_eth_check_frame_received(void);
uint32_t am_zlg_eth_prepare_transmit_desc(amhw_zlg_eth_t *p_hw_eth, uint16_t frame_length);

frame_type_def_t am_zlg_eth_get_rec_frame(void);

void am_zlg_eth_dma_rx_desc_init(   \
                amhw_zlg_eth_t *p_hw_eth, 
                eth_dma_desc_type_t *p_dma_rx_desc_tab,
                uint8_t *rx_buff, uint32_t rx_buff_cnt);
void am_zlg_eth_dma_tx_desc_init(   \
                amhw_zlg_eth_t *p_hw_eth,
                eth_dma_desc_type_t *p_dma_tx_desc_tab,
                uint8_t* tx_buff, uint32_t tx_buff_cnt);

/** \brief Ethernet Flags for g_eth_status variable */
#define AM_ZLG_ETH_INIT_FLAG           0x01 
#define AM_ZLG_ETH_LINK_FLAG           0x10 

#ifdef __cplusplus
}
#endif

#endif
