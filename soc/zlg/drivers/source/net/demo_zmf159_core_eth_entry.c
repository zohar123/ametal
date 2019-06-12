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
 * \brief demo_zmf159_core_eth_entry.c
 *
 * \internal
 * \par Modification History
 * - 1.00 19-06-11  wyy, first implementation
 * \endinternal
 */

/***************************************************************************
 includes
 ***************************************************************************/
#include "am_vdebug.h"
#include "demo_zmf159_core_entries.h"
#include "am_zlg_phy.h"
#include "stdint.h"
#include "netconf.h"

__IO uint32_t g_local_time = 0;

#define AM_ETH_TICK_PERIOD_MS  10

void time_update(void)
{
    g_local_time += AM_ETH_TICK_PERIOD_MS;
}

//#define TCP_CLIENT_TEST
#define TCP_SERVER_TEST

#if defined(TCP_CLIENT_TEST)
#include "tcp_echoclient.h"
#elif defined (TCP_SERVER_TEST)
#include "tcp_echoserver.h"
#else
#error "Please define TCP_CLIENT_TEST or TCP_SERVICE_TEST"
#endif

void demo_zmf159_core_eth_entry(void)
{
    uint8_t flag = 0;
    printf("demo_zmf159_core_eth_entry!\r\n");
    am_eth_std_tim1_timing_entry();
    am_eth_bsp_config();
    am_eth_lwip_init();
    while (1) {
        if ((0 == g_eth_link_status) && (0 == flag)) {
#if defined(TCP_CLIENT_TEST)
            printf("connect to tcp server\n");
            /*connect to tcp server */
            am_eth_tcp_client_init();
#endif 

#if defined(TCP_SERVER_TEST)
            printf("create tcp server\n");
            /*create tcp server */
            am_eth_tcp_server_init();
#endif

            flag = 1;
        }
        if ((1 == g_eth_link_status) && (1 == flag)) {
#if defined(TCP_CLIENT_TEST)        		
            printf("disconnect to tcp server\n");
            am_eth_tcp_client_close();
#endif 

#if defined(TCP_SERVER_TEST)        		
            printf("close tcp server\n");
            am_eth_tcp_server_close();
#endif 
            flag = 0;
        }
        if (am_eth_check_frame_received()) {
            am_eth_lwip_pkg_handle();
        }
        am_eth_lwip_periodic_handle(g_local_time);
    }
}

