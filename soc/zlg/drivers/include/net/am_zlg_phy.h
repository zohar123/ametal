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
 * \brief am_zlg_phy.h
 *
 * \internal
 * \par Modification History
 * - 1.00 19-06-11  wyy, first implementation
 * \endinternal
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AM_ZLG_PHY_H
#define __AM_ZLG_PHY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_zlg_eth.h"
#include "netif.h"

#define ETHERNET_PHY_ADDRESS       0x05 

/* ETH_MDIO */
#define ETH_MDIO_GPIO_CLK               RCC_AHB1Periph_GPIOA
#define ETH_MDIO_PORT                   GPIOA
#define ETH_MDIO_PIN                    GPIO_Pin_2
#define ETH_MDIO_AF                     GPIO_AF_ETH
#define ETH_MDIO_SOURCE                 GPIO_PinSource2

/* ETH_MDC */
#define ETH_MDC_GPIO_CLK                RCC_AHB1Periph_GPIOC
#define ETH_MDC_PORT                    GPIOC
#define ETH_MDC_PIN                     GPIO_Pin_1
#define ETH_MDC_AF                      GPIO_AF_ETH
#define ETH_MDC_SOURCE                  GPIO_PinSource1

/* ETH_RMII_REF_CLK */
#define ETH_RMII_REF_CLK_GPIO_CLK       RCC_AHB1Periph_GPIOA
#define ETH_RMII_REF_CLK_PORT           GPIOA
#define ETH_RMII_REF_CLK_PIN            GPIO_Pin_1
#define ETH_RMII_REF_CLK_AF             GPIO_AF_ETH
#define ETH_RMII_REF_CLK_SOURCE         GPIO_PinSource1

/* ETH_RMII_CRS_DV */
#define ETH_RMII_CRS_DV_GPIO_CLK        RCC_AHB1Periph_GPIOA
#define ETH_RMII_CRS_DV_PORT            GPIOA
#define ETH_RMII_CRS_DV_PIN             GPIO_Pin_7
#define ETH_RMII_CRS_DV_AF              GPIO_AF_ETH
#define ETH_RMII_CRS_DV_SOURCE          GPIO_PinSource7

/* ETH_RMII_RXD0 */
#define ETH_RMII_RXD0_GPIO_CLK          RCC_AHB1Periph_GPIOC
#define ETH_RMII_RXD0_PORT              GPIOC
#define ETH_RMII_RXD0_PIN               GPIO_Pin_4
#define ETH_RMII_RXD0_AF                GPIO_AF_ETH
#define ETH_RMII_RXD0_SOURCE            GPIO_PinSource4

/* ETH_RMII_RXD1 */
#define ETH_RMII_RXD1_GPIO_CLK          RCC_AHB1Periph_GPIOC
#define ETH_RMII_RXD1_PORT              GPIOC
#define ETH_RMII_RXD1_PIN               GPIO_Pin_5
#define ETH_RMII_RXD1_AF                GPIO_AF_ETH
#define ETH_RMII_RXD1_SOURCE            GPIO_PinSource5

/* ETH_RMII_TX_EN */
#define ETH_RMII_TX_EN_GPIO_CLK         RCC_AHB1Periph_GPIOG
#define ETH_RMII_TX_EN_PORT             GPIOG
#define ETH_RMII_TX_EN_PIN              GPIO_Pin_11
#define ETH_RMII_TX_EN_AF               GPIO_AF_ETH
#define ETH_RMII_TX_EN_SOURCE           GPIO_PinSource11

/* ETH_RMII_TXD0 */
#define ETH_RMII_TXD0_GPIO_CLK          RCC_AHB1Periph_GPIOG
#define ETH_RMII_TXD0_PORT              GPIOG
#define ETH_RMII_TXD0_PIN               GPIO_Pin_13
#define ETH_RMII_TXD0_AF                GPIO_AF_ETH
#define ETH_RMII_TXD0_SOURCE            GPIO_PinSource13

/* ETH_RMII_TXD1 */
#define ETH_RMII_TXD1_GPIO_CLK          RCC_AHB1Periph_GPIOG
#define ETH_RMII_TXD1_PORT              GPIOG
#define ETH_RMII_TXD1_PIN               GPIO_Pin_14
#define ETH_RMII_TXD1_AF                GPIO_AF_ETH
#define ETH_RMII_TXD1_SOURCE            GPIO_PinSource14

/* Ethernet Flags for g_eth_status variable */
#define AM_ETH_INIT_FLAG           0x01 /* Ethernet Init Flag */
#define AM_ETH_LINK_FLAG           0x10 /* Ethernet Link Flag */

void am_eth_bsp_config(void);
void am_eth_link_callback(struct netif *netif);
void am_eth_check_link_status(uint16_t phy_addr);
extern __IO uint8_t g_eth_link_status;

#ifdef __cplusplus
}
#endif

#endif /* __AM_ZLG_PHY_H */
