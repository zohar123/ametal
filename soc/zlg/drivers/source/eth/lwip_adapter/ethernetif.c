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
 * \brief ethernetif.c
 *
 * \internal
 * \par Modification History
 * - 1.00 19-06-11  wyy, first implementation
 * \endinternal
 */

/*******************************************************************************
  includes
*******************************************************************************/
#include "lwip/opt.h"
#include "lwip/mem.h"
#include "netif/etharp.h"
#include "ethernetif.h"
#include "am_zlg_eth.h"
#include "eth_conf.h"
#include "amhw_zlg_eth_reg.h"
#include "am_vdebug.h"
#include "string.h"
#include "zmf159_periph_map.h"

/** \brief Network interface name */
#define IFNAME0 's'
#define IFNAME1 't'

/** \brief Ethernet Rx & Tx DMA Descriptors */
extern eth_dma_desc_type_t g_dma_rx_desc_tab[AM_ZLG_ETH_RXBUFNB],
        g_dma_tx_desc_tab[AM_ZLG_ETH_TXBUFNB];

/** \brief Ethernet Driver Receive buffers  */
extern uint8_t g_rx_buff[AM_ZLG_ETH_RXBUFNB][AM_ZLG_ETH_ETH_RX_BUF_SIZE];

/** \brief Ethernet Driver Transmit buffers */
extern uint8_t g_tx_buff[AM_ZLG_ETH_TXBUFNB][AM_ZLG_ETH_TX_BUF_SIZE];

/** \brief Global pointers to track current transmit and receive descriptors */
extern eth_dma_desc_type_t *g_dma_tx_desc_to_set;

/** \brief Global pointer for last received frame infos */
extern eth_dma_rx_frame_info_t *gp_rx_frame_desc;
extern amhw_zlg_eth_t *gp_hw_eth;

/**
 * \brief In this function, the hardware should be initialized.
 *        Called from ethernetif_init().
 *
 * \param netif the already initialized lwip network interface structure
 *        for this ethernetif
 * \retval None
 */
static void low_level_init(struct netif *netif)
{
    amhw_zlg_eth_t *p_hw_eth = gp_hw_eth;
    /** \brief set MAC hardware address length */
    netif->hwaddr_len = ETHARP_HWADDR_LEN;

    /** \brief set MAC hardware address */
    netif->hwaddr[0] = g_eth_mac[0];
    netif->hwaddr[1] = g_eth_mac[1];
    netif->hwaddr[2] = g_eth_mac[2];
    netif->hwaddr[3] = g_eth_mac[3];
    netif->hwaddr[4] = g_eth_mac[4];
    netif->hwaddr[5] = g_eth_mac[5];

    /** \brief initialize MAC address in ethernet MAC */
    amhw_zlg_eth_mac_addr_config(AM_ZLG_ETH_MAC_Address0, netif->hwaddr);

    /** \brief maximum transfer unit */
    netif->mtu = 1500;

    /** \brief don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
    netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;

    /** \brief Initialize Tx Descriptors list: Chain Mode */
    am_zlg_eth_dma_tx_desc_init(p_hw_eth, g_dma_tx_desc_tab, &g_tx_buff[0][0],
            AM_ZLG_ETH_TXBUFNB);
            
    /** \brief Initialize Rx Descriptors list: Chain Mode  */
    am_zlg_eth_dma_rx_desc_init(p_hw_eth, g_dma_rx_desc_tab, &g_rx_buff[0][0],
            AM_ZLG_ETH_RXBUFNB);

    /** \brief Enable MAC and DMA transmission and reception */
    am_zlg_eth_start(p_hw_eth);

}

/**
 * \brief This function should do the actual transmission of the packet. 
 *        The packet is contained in the pbuf that is passed to the function. 
 *        This pbuf might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @retval ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become availale since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */
static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
    err_t err_val;
    struct pbuf *q;
    uint8_t *buffer = (uint8_t *) (g_dma_tx_desc_to_set->buff_1_addr);
    __IO eth_dma_desc_type_t *dma_tx_desc;
    uint16_t frame_length = 0;
    uint32_t buffer_offset = 0;
    uint32_t bytes_left_to_copy = 0;
    uint32_t pay_load_offset = 0;
    amhw_zlg_eth_t *p_hw_eth = gp_hw_eth;
    dma_tx_desc = g_dma_tx_desc_to_set;
    buffer_offset = 0;

    /** \brief copy frame from pbufs to driver buffers */
    for (q = p; q != NULL; q = q->next) {
        /** \brief Is this buffer available? If not, goto error */
        if ((dma_tx_desc->status & AM_ZLG_ETH_DMA_TX_DESC_OWN)
                != (uint32_t) RESET) {
            err_val = ERR_BUF;
            goto error;
        }

        /** \brief Get bytes in current lwIP buffer */
        bytes_left_to_copy = q->len;
        pay_load_offset = 0;

        /** \brief Check if the length of data to copy is bigger than Tx buffer size*/
        while ((bytes_left_to_copy + buffer_offset) > AM_ZLG_ETH_TX_BUF_SIZE) {
            /** \brief Copy data to Tx buffer*/
            memcpy((u8_t*) ((u8_t*) buffer + buffer_offset),
                    (u8_t*) ((u8_t*) q->payload + pay_load_offset),
                    (AM_ZLG_ETH_TX_BUF_SIZE - buffer_offset));

            /** \brief Point to next descriptor */
            dma_tx_desc =
                    (eth_dma_desc_type_t *) (dma_tx_desc->buff_2_next_desc_addr);

            /** \brief Check if the buffer is available */
            if ((dma_tx_desc->status & AM_ZLG_ETH_DMA_TX_DESC_OWN)
                    != (uint32_t) RESET) {
                err_val = ERR_USE;
                goto error;
            }

            buffer = (uint8_t *) (dma_tx_desc->buff_1_addr);

            bytes_left_to_copy = bytes_left_to_copy
                    - (AM_ZLG_ETH_TX_BUF_SIZE - buffer_offset);
            pay_load_offset = pay_load_offset
                    + (AM_ZLG_ETH_TX_BUF_SIZE - buffer_offset);
            frame_length = frame_length
                    + (AM_ZLG_ETH_TX_BUF_SIZE - buffer_offset);
            buffer_offset = 0;
        }

        /** \brief Copy the remaining bytes */
        memcpy((u8_t*) ((u8_t*) buffer + buffer_offset),
                (u8_t*) ((u8_t*) q->payload + pay_load_offset),
                bytes_left_to_copy);
        buffer_offset = buffer_offset + bytes_left_to_copy;
        frame_length = frame_length + bytes_left_to_copy;
    }

    /** \brief Prepare transmit descriptors to give to DMA*/
    am_zlg_eth_prepare_transmit_desc(p_hw_eth, frame_length);

    err_val = ERR_OK;

    error: if ((p_hw_eth->DMASR & AM_ZLG_ETH_DMASR_TUS) != (uint32_t) RESET) {
        /** \brief Clear TUS ETHERNET DMA flag */
        p_hw_eth->DMASR = AM_ZLG_ETH_DMASR_TUS;

        /** \brief Resume DMA transmission */
        p_hw_eth->DMATPDR = 0;
    }
    return err_val;
}

/**
 * \brief Should allocate a pbuf and transfer the bytes of the incoming
 *        packet from the interface into the pbuf.
 *
 * \param netif the lwip network interface structure for this ethernetif
 * \retval a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
static struct pbuf * am_zlg_eth_low_level_input(amhw_zlg_eth_t *p_hw_eth, struct netif *netif)
{
    struct pbuf *p, *q;
    uint32_t len;
    frame_type_def_t frame;
    uint8_t *buffer;
    __IO eth_dma_desc_type_t *DMARxDesc;
    uint32_t buffer_offset = 0;
    uint32_t pay_load_offset = 0;
    uint32_t bytes_left_to_copy = 0;
    uint32_t i = 0;

    /** \brief get received frame */
    frame = am_zlg_eth_get_rec_frame();

    /** \brief Obtain the size of the packet and put it into the "len" variable */
    len = frame.length;
    buffer = (uint8_t *) frame.buffer;

    /** \brief We allocate a pbuf chain of pbufs from the Lwip buffer pool */
    p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);

    if (p != NULL) {
        DMARxDesc = frame.descriptor;
        buffer_offset = 0;
        for (q = p; q != NULL; q = q->next) {
            bytes_left_to_copy = q->len;
            pay_load_offset = 0;

            while ((bytes_left_to_copy + buffer_offset)
                    > AM_ZLG_ETH_ETH_RX_BUF_SIZE) {
                /** \brief Copy data to pbuf*/
                memcpy((u8_t*) ((u8_t*) q->payload + pay_load_offset),
                        (u8_t*) ((u8_t*) buffer + buffer_offset),
                        (AM_ZLG_ETH_ETH_RX_BUF_SIZE - buffer_offset));

                /** \brief Point to next descriptor */
                DMARxDesc =
                        (eth_dma_desc_type_t *) (DMARxDesc->buff_2_next_desc_addr);
                buffer = (unsigned char *) (DMARxDesc->buff_1_addr);

                bytes_left_to_copy = bytes_left_to_copy
                        - (AM_ZLG_ETH_ETH_RX_BUF_SIZE - buffer_offset);
                pay_load_offset = pay_load_offset
                        + (AM_ZLG_ETH_ETH_RX_BUF_SIZE - buffer_offset);
                buffer_offset = 0;
            }
            /** \brief Copy remaining data in pbuf */
            memcpy((u8_t*) ((u8_t*) q->payload + pay_load_offset),
                    (u8_t*) ((u8_t*) buffer + buffer_offset),
                    bytes_left_to_copy);
            buffer_offset = buffer_offset + bytes_left_to_copy;
        }
    }

    /** \brief Release descriptors to DMA */
    DMARxDesc = frame.descriptor;

    /** \brief Set Own bit in Rx descriptors: gives the buffers back to DMA */
    for (i = 0; i < gp_rx_frame_desc->seg_count; i++) {
        DMARxDesc->status = AM_ZLG_ETH_DMA_RX_DESC_OWN;
        DMARxDesc = (eth_dma_desc_type_t *) (DMARxDesc->buff_2_next_desc_addr);
    }

    /** \brief Clear Segment_Count */
    gp_rx_frame_desc->fs_rx_desc = NULL;
    gp_rx_frame_desc->ls_rx_desc = NULL;
    gp_rx_frame_desc->seg_count = 0;

    if ((p_hw_eth->DMASR & AM_ZLG_ETH_DMASR_RBUS) != (uint32_t) RESET) {
        /** \brief Clear RBUS ETHERNET DMA flag */
        p_hw_eth->DMASR = AM_ZLG_ETH_DMASR_RBUS;
        
        /** \brief Resume DMA reception */
        p_hw_eth->DMARPDR = 0;
    }
    return p;
}

/**
 * \brief This function should be called when a packet is ready to be read
 *        from the interface. It uses the function am_zlg_eth_low_level_input() that
 *        should handle the actual reception of bytes from the network
 *        interface. Then the type of the received packet is determined and
 *        the appropriate input function is called.
 *
 * \param netif the lwip network interface structure for this ethernetif
 * \retval ERR_MEM or ERR_OK
 */
err_t am_zlg_eth_ethernetif_input(amhw_zlg_eth_t *p_hw_eth, struct netif *netif)
{
    err_t err;
    struct pbuf *p;

    /** \brief move received packet into a new pbuf */
    p = am_zlg_eth_low_level_input(p_hw_eth, netif);

    /** \brief no packet could be read, silently ignore this */
    if (p == NULL)
        return ERR_MEM;

    /** \brief entry point to the LwIP stack */
    err = netif->input(p, netif);

    if (err != ERR_OK) {
        LWIP_DEBUGF(NETIF_DEBUG, ("am_zlg_eth_ethernetif_input: IP input error\n"));
        pbuf_free(p);
    }
    return err;
}

/**
 * \brief Should be called at the beginning of the program to set up the
 *        network interface. It calls the function low_level_init() to do the
 *        actual setup of the hardware.
 *
 *        This function should be passed as a parameter to netif_add().
 *
 * \param netif the lwip network interface structure for this ethernetif
 * \return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t ethernetif_init(struct netif *netif)
{
    LWIP_ASSERT("netif != NULL", (netif != NULL));

#if LWIP_NETIF_HOSTNAME
    /** \brief Initialize interface hostname */
    netif->hostname = "lwip";
#endif

    netif->name[0] = IFNAME0;
    netif->name[1] = IFNAME1;
    netif->output = etharp_output;
    netif->linkoutput = low_level_output;

    /** \brief initialize the hardware */
    low_level_init(netif);

    return ERR_OK;
}
