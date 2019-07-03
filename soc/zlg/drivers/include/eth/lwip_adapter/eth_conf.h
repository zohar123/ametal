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
 * \brief eth_conf.h
 *
 * \internal
 * \par Modification History
 * - 1.00 19-06-11  wyy, first implementation
 * \endinternal
 */

#ifndef __ETH_CONF_H
#define __ETH_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "am_zlg_eth.h"

#define AM_ZLG_ETH_DHCP_START                 1
#define AM_ZLG_ETH_DHCP_WAIT_ADDRESS          2
#define AM_ZLG_ETH_DHCP_ADDRESS_ASSIGNED      3
#define AM_ZLG_ETH_DHCP_TIMEOUT               4
#define AM_ZLG_ETH_DHCP_LINK_DOWN             5

/** \brief enable DHCP, if disabled static address is used */
//#define USE_DHCP      

//#define SERIAL_DEBUG 
extern uint8_t g_eth_dest_ip[4];
extern uint32_t g_eth_dest_port;

extern uint8_t g_eth_local_ip[4];
extern uint32_t g_eth_local_port;

extern uint8_t g_eth_mac[6];
extern uint8_t g_eth_mask[4];
extern uint8_t g_eth_gateway[4];

#ifndef AM_ZLG_ETH_LINK_TIMER_INTERVAL
#define AM_ZLG_ETH_LINK_TIMER_INTERVAL        1000
#endif

void am_zlg_eth_lwip_init(void);
void am_zlg_eth_lwip_pkg_handle(amhw_zlg_eth_t *p_hw_eth);
void am_zlg_eth_lwip_periodic_handle(amhw_zlg_eth_t *p_hw_eth, __IO uint32_t localtime);

#ifdef __cplusplus
}
#endif

#endif 
