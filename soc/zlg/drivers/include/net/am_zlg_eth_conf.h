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
 * \brief am_zlg_eth_conf.h
 *
 * \internal
 * \par Modification History
 * - 1.00 19-06-11  wyy, first implementation
 * \endinternal
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AM_ZLG_ETH_CONF_H
#define __AM_ZLG_ETH_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "am_zlgxx.h"
#include "am_delay.h"

#define USE_Delay

#ifdef USE_Delay
#define __AM_ETH_DELAY__(value)    am_mdelay(value);
#else
#define __AM_ETH_DELAY__(value)
#endif

#define CUSTOM_DRIVER_BUFFERS_CONFIG

#ifdef  CUSTOM_DRIVER_BUFFERS_CONFIG
#define AM_ETH_ETH_RX_BUF_SIZE    AM_ETH_MAX_PACKET_SIZE  /* buffer size for receive */
#define AM_ETH_TX_BUF_SIZE    AM_ETH_MAX_PACKET_SIZE  /* buffer size for transmit */
#define AM_ETH_RXBUFNB        4                    /* 4 Rx buffers of size AM_ETH_ETH_RX_BUF_SIZE */
#define AM_ETH_TXBUFNB        4                    /* 4 Tx buffers of size AM_ETH_TX_BUF_SIZE */
#endif

/* PHY configuration section **************************************************/
#ifdef USE_Delay
/* PHY Reset delay */
//#define AM_PHY_RESET_DELAY    ((uint32_t)0x000000FF)
#define AM_PHY_RESET_DELAY    ((uint32_t)0x00000032)
/* PHY Configuration delay */
//#define AM_PHY_CONFIG_DELAY   ((uint32_t)0x00000FFF)
#define AM_PHY_CONFIG_DELAY   ((uint32_t)0x00000032)
/* Delay when writing to Ethernet registers*/
//#define AM_ETH_REG_WRITE_DELAY ((uint32_t)0x00000001)
#define AM_ETH_REG_WRITE_DELAY ((uint32_t)0x00000032)
#else
/* PHY Reset delay */
#define AM_PHY_RESET_DELAY    ((uint32_t)0x000FFFFF)
/* PHY Configuration delay */
#define AM_PHY_CONFIG_DELAY   ((uint32_t)0x00FFFFFF)
/* Delay when writing to Ethernet registers*/
#define AM_ETH_REG_WRITE_DELAY ((uint32_t)0x0000FFFF)
#endif

#define AM_PHY_SR                 ((uint16_t)0x0010) /* PHY status register Offset */
#define AM_PHY_SPEED_STATUS       ((uint16_t)0x0002) /* PHY Speed mask  1:10Mb/s       0:100Mb/s*/
#define AM_PHY_DUPLEX_STATUS      ((uint16_t)0x0004) /* PHY Duplex mask 1:Full duplex  0:Half duplex*/

#ifdef __cplusplus
}
#endif

#endif /* __AM_ZLG_ETH_CONF_H */
