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

#ifndef __ETHERNETIF_H__
#define __ETHERNETIF_H__


#include "lwip/err.h"
#include "lwip/netif.h"
#include "amhw_zlg_eth_reg.h"

err_t ethernetif_init(struct netif *netif);
err_t am_zlg_eth_ethernetif_input(amhw_zlg_eth_t *p_hw_eth, struct netif *netif);

#endif
