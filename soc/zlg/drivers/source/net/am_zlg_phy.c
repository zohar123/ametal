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
 * \brief am_zlg_phy.c
 *
 * \internal
 * \par Modification History
 * - 1.00 19-06-11  wyy, first implementation
 * \endinternal
 */

/***************************************************************************
 includes
 ***************************************************************************/
#include "lwip/opt.h"
#include "am_zlg_eth.h"
#include "am_zlg_phy.h"
#include "netif.h"
#include "netconf.h"
#include "lwip/dhcp.h"
#include "zmf159_pin.h"
#include "am_zlg_enet_reg.h"
#include "am_gpio.h"
#include "zmf159_regbase.h"
#include "am_vdebug.h"
#include "am_delay.h"

/* macro definition */
#define AM_ETH_GET_PHY_LINK_STATUS()		  (__am_eth_read_phy_reg(ETHERNET_PHY_ADDRESS, AM_PHY_BSR) & 0x00000004)
#define AM_ETH_PIN_CFG_AF_PP(PIN_NAME)        (PIN_NAME##_SPEED_50MHz | PIN_NAME##_AF_PP)
#define AM_ETH_PIN_CFG_INPUT_FLOAT(PIN_NAME)  (PIN_NAME##_INPUT_FLOAT)

/* public variable declaration */
am_eth_regs_conf g_eth_regs_config;
__IO uint32_t g_eth_status = 0;
__IO uint8_t g_eth_link_status = 0;

/* extern variable declaration */
extern struct netif g_eth_netif;

#ifdef USE_DHCP
extern __IO uint8_t am_eth_dhcp_state;
#endif

/* Private function prototypes */
static void __am_eth_gpio_config(void);
static void __am_eth_mac_dma_config(void);

/**
 * @brief  am_eth_bsp_config
 * @param  None
 * @retval None
 */
void am_eth_bsp_config(void)
{
    __am_eth_gpio_config();
    __am_eth_mac_dma_config();
    if (AM_ETH_GET_PHY_LINK_STATUS()) {
        g_eth_status |= AM_ETH_LINK_FLAG;
    }
}

/**
 * @brief  Configures the Ethernet Interface
 * @param  None
 * @retval None
 */
static void __am_eth_mac_dma_config(void)
{
    AM_DBG_INFO("\r\n __am_eth_mac_dma_config! \r\n");

    __am_eth_soft_reset();
    while (__am_eth_get_soft_reset_status() == SET);
    am_eth_usr_config_init(&g_eth_regs_config);
    g_eth_status = __am_eth_reg_init(&g_eth_regs_config, ETHERNET_PHY_ADDRESS);
}

static void enet_reset_phy(void)
{
    am_udelay(1000);
    am_gpio_set(PIOC_0, 0);
    am_udelay(100000);
    am_gpio_set(PIOC_0, 1);
    am_udelay(1000);
}

/**
 * @brief  Configures the different GPIO ports.
 * @param  None
 * @retval None
 */
static void __am_eth_gpio_config(void)
{
    AM_DBG_INFO("\r\n __am_eth_gpio_config! \r\n");
    /* 使能enet的时钟 */
    AM_REG_BITS_SET32(0x40021028, 25, 4, 0xf);

    /* 复位mac外设 */
    AM_REG_BITS_SET32(0x40021014, 25, 1, 0x1);

    am_udelay(10000);

    AM_REG_BITS_SET32(0x40021014, 25, 1, 0x0);

    /* ENET_nRST */
    am_gpio_pin_cfg(PIOC_0, PIOC_0_OUT_PP | AM_GPIO_OUTPUT_INIT_HIGH);

    enet_reset_phy();

    /* ENET_MDIO */
    am_gpio_pin_cfg(PIOA_2, PIOA_2_ETH_MDIO | AM_ETH_PIN_CFG_AF_PP(PIOA_2));

    /* ENET_MDC */
    am_gpio_pin_cfg(PIOC_1, PIOC_1_ETH_MDC | AM_ETH_PIN_CFG_AF_PP(PIOC_1));

    /* ETH_CRS */
    am_gpio_pin_cfg(PIOA_0,
            PIOA_0_ETH_CRS | AM_ETH_PIN_CFG_INPUT_FLOAT(PIOA_0));

    /* ETH_RX_CLK */
    am_gpio_pin_cfg(PIOA_1,
            PIOA_1_ETH_RX_CLK | AM_ETH_PIN_CFG_INPUT_FLOAT(PIOA_1));

    /* ETH_COL */
    am_gpio_pin_cfg(PIOA_3,
            PIOA_3_ETH_COL | AM_ETH_PIN_CFG_INPUT_FLOAT(PIOA_3));
    /* ETH_RX_DV */
    am_gpio_pin_cfg(PIOA_7,
            PIOA_7_ETH_RX_DV | AM_ETH_PIN_CFG_INPUT_FLOAT(PIOA_7));

    /* ETH_RXD2 */
    am_gpio_pin_cfg(PIOB_0,
            PIOB_0_ETH_RXD2 | AM_ETH_PIN_CFG_INPUT_FLOAT(PIOB_0));

    /* ETH_RXD3 */
    am_gpio_pin_cfg(PIOB_1,
            PIOB_1_ETH_RXD3 | AM_ETH_PIN_CFG_INPUT_FLOAT(PIOB_1));

    /* ETH_TXD3 */
    am_gpio_pin_cfg(PIOE_2, PIOE_2_ETH_TXD3 | AM_ETH_PIN_CFG_AF_PP(PIOE_2));

    /* ETH_RX_ER */
    am_gpio_pin_cfg(PIOB_10,
            PIOB_10_ETH_RX_ER | AM_ETH_PIN_CFG_INPUT_FLOAT(PIOB_10));

    /* ETH_TX_EN */
    am_gpio_pin_cfg(PIOB_11, PIOB_11_ETH_TX_EN | AM_ETH_PIN_CFG_AF_PP(PIOB_11));

    /* ETH_TXD0 */
    am_gpio_pin_cfg(PIOB_12, PIOB_12_ETH_TXD0 | AM_ETH_PIN_CFG_AF_PP(PIOB_12));

    /* ETH_TXD1 */
    am_gpio_pin_cfg(PIOB_13, PIOB_13_ETH_TXD1 | AM_ETH_PIN_CFG_AF_PP(PIOB_13));

    /* ETH_TXD2 */
    am_gpio_pin_cfg(PIOC_2, PIOC_2_ETH_TXD2 | AM_ETH_PIN_CFG_AF_PP(PIOC_2));

    /* ETH_TX_CLK */
    am_gpio_pin_cfg(PIOC_3,
            PIOC_3_ETH_TX_CLK | AM_ETH_PIN_CFG_INPUT_FLOAT(PIOC_3));

    /* ETH_RXD0 */
    am_gpio_pin_cfg(PIOC_4,
            PIOC_4_ETH_RXD0 | AM_ETH_PIN_CFG_INPUT_FLOAT(PIOC_4));

    /* ETH_RXD1 */
    am_gpio_pin_cfg(PIOC_5,
            PIOC_5_ETH_RXD1 | AM_ETH_PIN_CFG_INPUT_FLOAT(PIOC_5));
}

/* This function is called periodically each second */
/* It checks link status for ethernet controller */
void am_eth_check_link_status(uint16_t phy_addr)
{
    static uint8_t status = 0;
    uint32_t tmp = AM_ETH_GET_PHY_LINK_STATUS();

    if (tmp && !status) {
        netif_set_link_up(&g_eth_netif);
        status = 1;
    }
    if (!tmp && status) {
        g_eth_link_status = 1;
        netif_set_link_down(&g_eth_netif);
        status = 0;
    }
}

/**
 * @brief  Link callback function, this function is called on change of link status.
 * @param  The network interface
 * @retval None
 */
void am_eth_link_callback(struct netif *netif)
{
    AM_DBG_INFO("\r\n am_eth_link_callback! \r\n");
    __IO uint32_t timeout = 0;
    uint32_t tmp_reg;
    uint16_t reg_value;
    struct ip_addr ipaddr;
    struct ip_addr netmask;
    struct ip_addr gw;

    if (netif_is_link_up(netif)) {
        if (g_eth_regs_config.eth_auto_negotiation
                != AM_ETH_AUTO_NEGOTIATION_DISABLE) {
            timeout = 0;
            __am_eth_write_phy_reg(ETHERNET_PHY_ADDRESS, AM_PHY_BCR,
            AM_PHY_AUTO_NEGOTIATION);
            do {
                timeout++;
            } while (!(__am_eth_read_phy_reg(ETHERNET_PHY_ADDRESS, AM_PHY_BSR)
                    & AM_PHY_AUTO_NEGO_COMPLETE)
                    && (timeout < (uint32_t) AM_PHY_READ_TO));
            timeout = 0;
            reg_value = __am_eth_read_phy_reg(ETHERNET_PHY_ADDRESS, AM_PHY_SR);
            if ((reg_value & AM_PHY_DUPLEX_STATUS) != (uint16_t) RESET) {
                g_eth_regs_config.eth_mode = AM_ETH_MODE_FULLDUPLEX;
            } else {
                g_eth_regs_config.eth_mode = AM_ETH_MODE_HALFDUPLEX;
            }
            if (reg_value & AM_PHY_SPEED_STATUS) {
                g_eth_regs_config.eth_speed = AM_ETH_SPEED_10M;
            } else {
                g_eth_regs_config.eth_speed = AM_ETH_SPEED_100M;
            }

            tmp_reg = ETH->MACCR;
            tmp_reg |= (uint32_t) (g_eth_regs_config.eth_speed
                    | g_eth_regs_config.eth_mode);
            ETH->MACCR = (uint32_t) tmp_reg;

            __AM_ETH_DELAY__(AM_ETH_REG_WRITE_DELAY);
            tmp_reg = ETH->MACCR;
            ETH->MACCR = tmp_reg;
        }
        am_eth_start();

#ifdef USE_DHCP
        ipaddr.addr = 0;
        netmask.addr = 0;
        gw.addr = 0;
        am_eth_dhcp_state = AM_ETH_DHCP_START;
#else
        IP4_ADDR(&ipaddr, AM_ETH_IP_ADDR0, AM_ETH_IP_ADDR1, AM_ETH_IP_ADDR2,
                AM_ETH_IP_ADDR3);
        IP4_ADDR(&netmask, AM_ETH_NETMASK_ADDR0, AM_ETH_NETMASK_ADDR1,
                AM_ETH_NETMASK_ADDR2, AM_ETH_NETMASK_ADDR3);
        IP4_ADDR(&gw, AM_ETH_GW_ADDR0, AM_ETH_GW_ADDR1, AM_ETH_GW_ADDR2,
                AM_ETH_GW_ADDR3);
#endif /* USE_DHCP */
        netif_set_addr(&g_eth_netif, &ipaddr, &netmask, &gw);
        netif_set_up(&g_eth_netif);
        g_eth_link_status = 0;
    } else {
        am_eth_stop();
#ifdef USE_DHCP
        am_eth_dhcp_state = AM_ETH_DHCP_LINK_DOWN;
        am_dhcp_stop(netif);
#endif /* USE_DHCP */
        netif_set_down(&g_eth_netif);
    }
}
