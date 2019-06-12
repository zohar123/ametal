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
 * \brief am_zlg_enet_reg.h
 *
 * \internal
 * \par Modification History
 * - 1.00 19-06-11  wyy, first implementation
 * \endinternal
 */
#ifndef  __AM_ZLG_ENET_REG_H
#define  __AM_ZLG_ENET_REG_H

#include "stdint.h"
#include "zmf159_regbase.h"
#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus  */

#pragma pack(push)
#pragma pack(1)

/**
 * \brief ETH register definitions
 */
typedef struct zlg_eth_reg {
    volatile uint32_t MACCR; /* 0x0000  MAC Configuration Register */
    volatile uint32_t MACFFR; /* 0x0004  MAC Frame Filter Register */
    volatile uint32_t MACHTHR; /* 0x0008  MAC Hash Table High Register */
    volatile uint32_t MACHTLR; /* 0x000C  MAC Hash Table Low Register */
    volatile uint32_t MACMIIAR; /* 0x0010  GMII Address Register */
    volatile uint32_t MACMIIDR; /* 0x0014  GMII Data Register */
    volatile uint32_t MACFCR; /* 0x0018  Flow Control Register */
    volatile uint32_t MACVLANTR; /* 0x001C  VLAN Tag Register */
    volatile uint32_t MACVR; /* 0x0020  Version Register */
    volatile uint32_t RESERVED0[7]; /* 0x0024 - 0x003C */

    volatile uint32_t MACA0HR; /* 0x0040  MAC Address0 High Register */
    volatile uint32_t MACA0LR; /* 0x0044  MAC Address0 Low Register */
    volatile uint32_t MACA1HR; /* 0x0048  MAC Address1 High Register */
    volatile uint32_t MACA1LR; /* 0x004C  MAC Address1 Low Register */
    volatile uint32_t MACA2HR; /* 0x0050  MAC Address2 High Register */
    volatile uint32_t MACA2LR; /* 0x0054  MAC Address2 Low Register */
    volatile uint32_t MACA3HR; /* 0x0058  MAC Address3 High Register */
    volatile uint32_t MACA3LR; /* 0x005C  MAC Address3 Low Register */
    volatile uint32_t MACA4HR; /* 0x0060  MAC Address4 High Register */
    volatile uint32_t MACA4LR; /* 0x0064  MAC Address4 Low Register */
    volatile uint32_t MACA5HR; /* 0x0068  MAC Address5 High Register */
    volatile uint32_t MACA5LR; /* 0x006C  MAC Address5 Low Register */
    volatile uint32_t MACA6HR; /* 0x0070  MAC Address6 High Register */
    volatile uint32_t MACA6LR; /* 0x0074  MAC Address6 Low Register */
    volatile uint32_t MACA7HR; /* 0x0078  MAC Address7 High Register */
    volatile uint32_t MACA7LR; /* 0x007C  MAC Address7 Low Register */
    volatile uint32_t MACA8HR; /* 0x0080  MAC Address8 High Register */
    volatile uint32_t MACA8LR; /* 0x0084  MAC Address8 Low Register */
    volatile uint32_t MACA9HR; /* 0x0088  MAC Address9 High Register */
    volatile uint32_t MACA9LR; /* 0x008C  MAC Address9 Low Register */
    volatile uint32_t MACA10HR; /* 0x0090  MAC Address10 High Register */
    volatile uint32_t MACA10LR; /* 0x0094  MAC Address10 Low Register */
    volatile uint32_t MACA11HR; /* 0x0098  MAC Address11 High Register */
    volatile uint32_t MACA11LR; /* 0x009C  MAC Address11 Low Register */
    volatile uint32_t MACA12HR; /* 0x00A0  MAC Address12 High Register */
    volatile uint32_t MACA12LR; /* 0x00A4  MAC Address12 Low Register */
    volatile uint32_t MACA13HR; /* 0x00A8  MAC Address13 High Register */
    volatile uint32_t MACA13LR; /* 0x00AC  MAC Address13 Low Register */
    volatile uint32_t MACA14HR; /* 0x00B0  MAC Address14 High Register */
    volatile uint32_t MACA14LR; /* 0x00B4  MAC Address14 Low Register */
    volatile uint32_t MACA15HR; /* 0x00B8  MAC Address15 High Register */
    volatile uint32_t MACA15LR; /* 0x00BC  MAC Address15 Low Register */

    volatile uint32_t MACACR; /* 0x00C0  AN Control Register */
    volatile uint32_t MACASR; /* 0x00C4  AN status Register */
    volatile uint32_t MACAAR; /* 0x00C8  Auto-Negotiation Advertisement Register */
    volatile uint32_t MACALPAR; /* 0x00CC  Auto-Negotiation Link Partner Ability Register */
    volatile uint32_t MACAER; /* 0x00D0  Auto-Negotiation Expansion Register */
    volatile uint32_t MACTESR; /* 0x00D4  TBI Extended status Register */
    volatile uint32_t MACMSR; /* 0x00D8  SGMII/RGMII status Register */

    volatile uint32_t RESERVED1[969]; /* 0x00DC - 0x0FFC */

    volatile uint32_t DMABMR; /* 0x1000  Bus Mode Register */
    volatile uint32_t DMATPDR; /* 0x1004  Transmit Poll Demand Register */
    volatile uint32_t DMARPDR; /* 0x1008  Receive Poll Demand Register */
    volatile uint32_t DMARDLAR; /* 0x100C  Receive Descriptor List Address Register */
    volatile uint32_t DMATDLAR; /* 0x1010  Transmit Descriptor List Address Register */
    volatile uint32_t DMASR; /* 0x1014  status Register */
    volatile uint32_t DMAOMR; /* 0x1018  Operation Mode Register */
    volatile uint32_t DMAIER; /* 0x101C  Interrupt Enable Register */
    volatile uint32_t DMAMFBOCR; /* 0x1020  Missed Frame and Buffer Overflow Counter Register */

    volatile uint32_t RESERVED2[9]; /* 0x1024 - 0x1044 */

    volatile uint32_t DMACHTDR; /* 0x1048  Current Host Transmit Descriptor Register */
    volatile uint32_t DMACHRDR; /* 0x104C  Current Host Receive Descriptor Register */
    volatile uint32_t DMACHTBAR; /* 0x1050  Current Host Transmit Buffer Address Register */
    volatile uint32_t DMACHRBAR; /* 0x1054  Current Host Receive Buffer Address Register */

} zlg_eth_reg_t;

#define ETH                 ((zlg_eth_reg_t *) ZMF159_ETH_BASE)  

#ifdef __cplusplus
}
#endif  /* __cplusplus  */

#endif
