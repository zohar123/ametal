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
 * \brief netconf.h
 *
 * \internal
 * \par Modification History
 * - 1.00 19-06-11  wyy, first implementation
 * \endinternal
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NETCONF_H
#define __NETCONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "am_zlgxx.h"

/* Exported types ------------------------------------------------------------*/
/* DHCP状态 */
#define AM_ETH_DHCP_START                 1
#define AM_ETH_DHCP_WAIT_ADDRESS          2
#define AM_ETH_DHCP_ADDRESS_ASSIGNED      3
#define AM_ETH_DHCP_TIMEOUT               4
#define AM_ETH_DHCP_LINK_DOWN             5

//#define USE_DHCP       /* enable DHCP, if disabled static address is used */

/* 调试信息输出 */
//#define SERIAL_DEBUG 
/* 远端IP地址和端口 */
#define AM_ETH_DEST_IP_ADDR0               192
#define AM_ETH_DEST_IP_ADDR1               168
#define AM_ETH_DEST_IP_ADDR2                40
#define AM_ETH_DEST_IP_ADDR3               179
#define AM_ETH_DEST_PORT                 50000
#define AM_ETH_LOCAL_SERVER_PORT         60000

/* MAC地址：网卡地址 */
#define AM_ETH_MAC_ADDR0                     22
#define AM_ETH_MAC_ADDR1                     33
#define AM_ETH_MAC_ADDR2                     44
#define AM_ETH_MAC_ADDR3                     55
#define AM_ETH_MAC_ADDR4                     66
#define AM_ETH_MAC_ADDR5                     77

/*静态IP地址 */
#define AM_ETH_IP_ADDR0                    192
#define AM_ETH_IP_ADDR1                    168
#define AM_ETH_IP_ADDR2                     40
#define AM_ETH_IP_ADDR3                    110

/* 子网掩码 */
#define AM_ETH_NETMASK_ADDR0               255
#define AM_ETH_NETMASK_ADDR1               255
#define AM_ETH_NETMASK_ADDR2               255
#define AM_ETH_NETMASK_ADDR3                 0

/* 网关 */
#define AM_ETH_GW_ADDR0                    192
#define AM_ETH_GW_ADDR1                    168
#define AM_ETH_GW_ADDR2                     40
#define AM_ETH_GW_ADDR3                      1

/* 检测PHY链路状态的实际间隔(单位：ms) */
#ifndef AM_ETH_LINK_TIMER_INTERVAL
#define AM_ETH_LINK_TIMER_INTERVAL        1000
#endif

void am_eth_lwip_init(void);
void am_eth_lwip_pkg_handle(void);
void am_eth_lwip_periodic_handle(__IO uint32_t localtime);

#ifdef __cplusplus
}
#endif

#endif /* __NETCONF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

