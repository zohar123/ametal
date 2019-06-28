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

/*******************************************************************************
  includes
*******************************************************************************/
#include "am_vdebug.h"
#include "am_zlg_phy.h"
#include "stdint.h"
#include "eth_conf.h"
#include "am_zmf159_inst_init.h"
#include "am_timer.h"

//#define TCP_CLIENT_TEST
#define TCP_SERVER_TEST

#if defined(TCP_CLIENT_TEST)
extern void am_eth_tcp_client_init(void);
extern void am_eth_tcp_client_close(void);
#elif defined (TCP_SERVER_TEST)
extern void am_eth_tcp_server_init(void);
extern void am_eth_tcp_server_close(void);
#else
#error "Please define TCP_CLIENT_TEST or TCP_SERVICE_TEST"
#endif

__IO uint32_t g_local_time = 0;

#define __AM_ZLG_ETH_TICK_PERIOD_MS             10 /**< \brief tick time */
#define __AM_ZLG_ETH_LED1                       1  /**< \brief led num */

#define __AM_ZLG_ETH_OCCUPY                     1  /**< \brief net occupyed */
#define __AM_ZLG_ETH_RELEASE                    0  /**< \brief net release */

static void __time_update(void)
{
    g_local_time += __AM_ZLG_ETH_TICK_PERIOD_MS;
}

static void __tim_timing_callback (void *p_arg)
{
    am_led_toggle (__AM_ZLG_ETH_LED1);
    __time_update();
}

static void __am_eth_timer_init(void)
{
    am_timer_handle_t handle;
    handle = am_zmf159_tim1_timing_inst_init();
    am_timer_callback_set(handle, 0, __tim_timing_callback, NULL);
    /* 设置定时时间为 0.01s */
    am_timer_enable_us(handle, 0, 1000000 / 100);
}

void demo_zlg_hw_eth_entry(void)
{
    uint8_t flag = __AM_ZLG_ETH_RELEASE;
    
    __am_eth_timer_init();
	am_zlg_eth_bsp_config();
    am_zlg_eth_lwip_init();
    while (1) {
        if ((AM_ZLG_ETH_LINK_ST_UP == g_eth_link_status) 
            && (__AM_ZLG_ETH_RELEASE == flag)) {
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

            flag = __AM_ZLG_ETH_OCCUPY;
        }
        if ((AM_ZLG_ETH_LINK_ST_DOWN == g_eth_link_status) 
            && (__AM_ZLG_ETH_OCCUPY == flag)) {
#if defined(TCP_CLIENT_TEST)        		
            printf("disconnect to tcp server\n");
            am_eth_tcp_client_close();
#endif 

#if defined(TCP_SERVER_TEST)        		
            printf("close tcp server\n");
            am_eth_tcp_server_close();
#endif 
            flag = __AM_ZLG_ETH_RELEASE;
        }
        if (am_zlg_eth_check_frame_received()) {
            am_zlg_eth_lwip_pkg_handle();
        }
        am_zlg_eth_lwip_periodic_handle(g_local_time);
    }
	
}
