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
 * \brief eth_conf.c
 *
 * \internal
 * \par Modification History
 * - 1.00 19-06-11  wyy, first implementation
 * \endinternal
 */

/*******************************************************************************
  includes
*******************************************************************************/

#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/tcp.h"
#include "lwip/tcp_impl.h"
#include "lwip/udp.h"
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "ethernetif.h"
#include "eth_conf.h"
#include "amhw_zlg_phy.h"

#define MAX_DHCP_TRIES        4

struct netif g_eth_netif;
uint32_t TCPTimer  = 0;
uint32_t ARPTimer  = 0;
uint32_t LinkTimer = 0;
uint32_t IPaddress = 0;

#ifdef USE_DHCP
uint32_t DHCPfineTimer   = 0;
uint32_t DHCPcoarseTimer = 0;
__IO uint8_t am_eth_dhcp_state;
void am_zlg_eth_lwip_dhcp_process_handle(void);
#endif

extern __IO uint32_t g_eth_status;
extern void am_zlg_eth_link_callback(struct netif *netif);
/**
 * \brief  Initializes the lwIP stack
 * \param  None
 * \retval None
 */
void am_zlg_eth_lwip_init(void)
{
    struct ip_addr ipaddr;
    struct ip_addr netmask;
    struct ip_addr gw;

    /* Initializes the dynamic memory heap defined by MEM_SIZE.*/
    mem_init();

    /* Initializes the memory pools defined by MEMP_NUM_x.*/
    memp_init();

#ifdef USE_DHCP
    ipaddr.addr  = 0;
    netmask.addr = 0;
    gw.addr      = 0;
#else
    IP4_ADDR(&ipaddr, g_eth_local_ip[0], g_eth_local_ip[1], g_eth_local_ip[2],
            g_eth_local_ip[3]);
    IP4_ADDR(&netmask, g_eth_mask[0], g_eth_mask[1], g_eth_mask[2],
            g_eth_mask[3]);
    IP4_ADDR(&gw, g_eth_gateway[0], g_eth_gateway[1], g_eth_gateway[2],
            g_eth_gateway[3]);
#endif  
    netif_add(&g_eth_netif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init,
            &ethernet_input);
    netif_set_default(&g_eth_netif);
    if (g_eth_status == (AM_ZLG_ETH_INIT_FLAG | AM_ZLG_ETH_LINK_FLAG)) {
        g_eth_netif.flags |= NETIF_FLAG_LINK_UP;
        netif_set_up(&g_eth_netif);
#ifdef USE_DHCP
        am_eth_dhcp_state = AM_ZLG_ETH_DHCP_START;
#else
#ifdef SERIAL_DEBUG
        printf("\n  Static IP address   \n");
        printf("IP: %d.%d.%d.%d\n", g_eth_local_ip[0], g_eth_local_ip[1], 
            g_eth_local_ip[2], g_eth_local_ip[3]);
        printf("NETMASK: %d.%d.%d.%d\n", g_eth_mask[0], g_eth_mask[1], 
            g_eth_mask[2], g_eth_mask[3]);
        printf("Gateway: %d.%d.%d.%d\n", g_eth_gateway[0], g_eth_gateway[1], 
            g_eth_gateway[2], g_eth_gateway[3]);
#endif 
#endif 
    } else {
        netif_set_down(&g_eth_netif);
#ifdef USE_DHCP
        am_eth_dhcp_state = AM_ZLG_ETH_DHCP_LINK_DOWN;
#endif 
#ifdef SERIAL_DEBUG
        printf("\n  Network Cable is  \n");
        printf("    not connected   \n");
#endif 
    }
    netif_set_link_callback(&g_eth_netif, am_zlg_eth_link_callback);
}

/**
 * \brief  Called when a frame is received
 * \param  None
 * \retval None
 */
void am_zlg_eth_lwip_pkg_handle(amhw_zlg_eth_t *p_hw_eth)
{
    am_zlg_eth_ethernetif_input(p_hw_eth, &g_eth_netif);
}

/**
 * \brief  LwIP periodic tasks
 * \param  localtime the current LocalTime value
 * \retval None
 */
void am_zlg_eth_lwip_periodic_handle(amhw_zlg_eth_t *p_hw_eth, __IO uint32_t localtime)
{
    if ((localtime + TCP_TMR_INTERVAL >= 4294967295UL)
            || (localtime + ARP_TMR_INTERVAL >= 4294967295UL)
            || (localtime + AM_ZLG_ETH_LINK_TIMER_INTERVAL >= 4294967295UL)
#ifdef USE_DHCP
            || (localtime + DHCP_FINE_TIMER_MSECS >= 4294967295UL)
            || (localtime + DHCP_COARSE_TIMER_MSECS >= 4294967295UL)
#endif
            ) {
        TCPTimer = 0;
        ARPTimer = 0;
        LinkTimer = 0;
#ifdef USE_DHCP
        DHCPfineTimer = 0;
        DHCPcoarseTimer = 0;
#endif
        localtime = 0;

        tcp_tmr();
        etharp_tmr();
        am_zlg_eth_check_link_status(p_hw_eth, ETHERNET_PHY_ADDRESS);
        
#ifdef USE_DHCP
        dhcp_fine_tmr();
        if ((am_eth_dhcp_state != AM_ZLG_ETH_DHCP_ADDRESS_ASSIGNED) &&
                (am_eth_dhcp_state != AM_ZLG_ETH_DHCP_TIMEOUT) &&
                (am_eth_dhcp_state != AM_ZLG_ETH_DHCP_LINK_DOWN))
        {
            am_zlg_eth_lwip_dhcp_process_handle();
        }
        dhcp_coarse_tmr();
#endif
    }

#if LWIP_TCP
    /* TCP periodic process every 250 ms */
    if (localtime - TCPTimer >= TCP_TMR_INTERVAL) {
        TCPTimer = localtime;
        tcp_tmr();
    }
#endif

    /* ARP periodic process every 5s */
    if ((localtime - ARPTimer) >= ARP_TMR_INTERVAL) {
        ARPTimer = localtime;
        etharp_tmr();
    }

    /* Check link status periodically */
    if ((localtime - LinkTimer) >= AM_ZLG_ETH_LINK_TIMER_INTERVAL) {
        LinkTimer = localtime;
        am_zlg_eth_check_link_status(p_hw_eth, ETHERNET_PHY_ADDRESS);
    }

#ifdef USE_DHCP
    /* Fine DHCP periodic process every 500ms */
    if (localtime - DHCPfineTimer >= DHCP_FINE_TIMER_MSECS)
    {
        DHCPfineTimer = localtime;
        dhcp_fine_tmr();
        if ((am_eth_dhcp_state != AM_ZLG_ETH_DHCP_ADDRESS_ASSIGNED) &&
                (am_eth_dhcp_state != AM_ZLG_ETH_DHCP_TIMEOUT) &&
                (am_eth_dhcp_state != AM_ZLG_ETH_DHCP_LINK_DOWN))
        {
#ifdef SERIAL_DEBUG
            LED1_TOGGLE;
            printf("\nFine DHCP periodic process every 500ms\n");
#endif /* SERIAL_DEBUG */

            /* process DHCP state machine */
            am_zlg_eth_lwip_dhcp_process_handle();
        }
    }

    /* DHCP Coarse periodic process every 60s */
    if (localtime - DHCPcoarseTimer >= DHCP_COARSE_TIMER_MSECS)
    {
        DHCPcoarseTimer = localtime;
        dhcp_coarse_tmr();
    }

#endif
}

#ifdef USE_DHCP
/**
 * \brief  am_zlg_eth_lwip_dhcp_process_handle
 * \param  None
 * \retval None
 */
void am_zlg_eth_lwip_dhcp_process_handle(void)
{
    struct ip_addr ipaddr;
    struct ip_addr netmask;
    struct ip_addr gw;

    switch (am_eth_dhcp_state)
    {
        case AM_ZLG_ETH_DHCP_START:
        {
            am_eth_dhcp_state = AM_ZLG_ETH_DHCP_WAIT_ADDRESS;
            dhcp_start(&g_eth_netif);
            IPaddress = 0;
#ifdef SERIAL_DEBUG
            printf("\n     Looking for    \n");
            printf("     DHCP server    \n");
            printf("     please wait... \n");
#endif 
        }
        break;

        case AM_ZLG_ETH_DHCP_WAIT_ADDRESS:
        {
            IPaddress = g_eth_netif.ip_addr.addr;

            if (IPaddress!=0)
            {
                am_eth_dhcp_state = AM_ZLG_ETH_DHCP_ADDRESS_ASSIGNED;

                am_dhcp_stop(&g_eth_netif);

#ifdef SERIAL_DEBUG
                printf("\n  IP address assigned \n");
                printf("    by a DHCP server   \n");
                printf("IP: %d.%d.%d.%d\n",(uint8_t)(IPaddress),
                    (uint8_t)(IPaddress >> 8), (uint8_t)(IPaddress >> 16),
                    (uint8_t)(IPaddress >> 24));
                printf("NETMASK: %d.%d.%d.%d\n", g_eth_mask[0], g_eth_mask[1], 
                    g_eth_mask[2], g_eth_mask[3]);
                printf("Gateway: %d.%d.%d.%d\n", g_eth_gateway[0], 
                    g_eth_gateway[1], g_eth_gateway[2], g_eth_gateway[3]);
#endif
            }
            else
            {
                if (g_eth_netif.dhcp->tries > MAX_DHCP_TRIES)
                {
                    am_eth_dhcp_state = AM_ZLG_ETH_DHCP_TIMEOUT;

                    am_dhcp_stop(&g_eth_netif);

                    IP4_ADDR(&ipaddr, g_eth_local_ip[0] , g_eth_local_ip[1], 
                          g_eth_local_ip[2], g_eth_local_ip[3]);
                    IP4_ADDR(&netmask, g_eth_mask[0], g_eth_mask[1], 
                          g_eth_mask[2], g_eth_mask[3]);
                    IP4_ADDR(&gw, g_eth_gateway[0], g_eth_gateway[1], 
                          g_eth_gateway[2], g_eth_gateway[3]);
                    netif_set_addr(&g_eth_netif, &ipaddr , &netmask, &gw);

#ifdef SERIAL_DEBUG
                    printf("\n    DHCP timeout    \n");
                    printf("  Static IP address   \n");
                    printf("IP: %d.%d.%d.%d\n", &g_eth_local_ip[0], 
                       g_eth_local_ip[1], g_eth_local_ip[2], g_eth_local_ip[3]);
                    printf("NETMASK: %d.%d.%d.%d\n", &g_eth_mask[0], 
                       g_eth_mask[1], g_eth_mask[2], g_eth_mask[3]);
                    printf("Gateway: %d.%d.%d.%d\n",&g_eth_gateway[0], 
                       g_eth_gateway[1], g_eth_gateway[2], g_eth_gateway[3]);
#endif
                }
            }
        }
        break;
        default: break;
    }
}
#endif
