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
 * \brief amhw_zlg_eth_reg.h
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
#endif 

#pragma pack(push)
#pragma pack(4)

typedef struct amhw_zlg_eth {
    volatile uint32_t MACCR;    /**< \brief 0x0000  MAC Configuration Register */
    volatile uint32_t MACFFR;   /**< \brief 0x0004  MAC Frame Filter Register */
    volatile uint32_t MACHTHR;  /**< \brief 0x0008  MAC Hash Table High Register */
    volatile uint32_t MACHTLR;  /**< \brief 0x000C  MAC Hash Table Low Register */
    volatile uint32_t MACMIIAR; /**< \brief 0x0010  GMII Address Register */
    volatile uint32_t MACMIIDR; /**< \brief 0x0014  GMII Data Register */
    volatile uint32_t MACFCR;   /**< \brief 0x0018  Flow Control Register */
    volatile uint32_t MACVLANTR;/**< \brief 0x001C  VLAN Tag Register */
    volatile uint32_t MACVR;    /**< \brief 0x0020  Version Register */
    volatile uint32_t RESERVED0[7]; /**< \brief 0x0024 - 0x003C */

    volatile uint32_t MACA0HR; /**< \brief 0x0040  MAC Address0 High Register */
    volatile uint32_t MACA0LR; /**< \brief 0x0044  MAC Address0 Low Register */
    volatile uint32_t MACA1HR; /**< \brief 0x0048  MAC Address1 High Register */
    volatile uint32_t MACA1LR; /**< \brief 0x004C  MAC Address1 Low Register */
    volatile uint32_t MACA2HR; /**< \brief 0x0050  MAC Address2 High Register */
    volatile uint32_t MACA2LR; /**< \brief 0x0054  MAC Address2 Low Register */
    volatile uint32_t MACA3HR; /**< \brief 0x0058  MAC Address3 High Register */
    volatile uint32_t MACA3LR; /**< \brief 0x005C  MAC Address3 Low Register */
    volatile uint32_t MACA4HR; /**< \brief 0x0060  MAC Address4 High Register */
    volatile uint32_t MACA4LR; /**< \brief 0x0064  MAC Address4 Low Register */
    volatile uint32_t MACA5HR; /**< \brief 0x0068  MAC Address5 High Register */
    volatile uint32_t MACA5LR; /**< \brief 0x006C  MAC Address5 Low Register */
    volatile uint32_t MACA6HR; /**< \brief 0x0070  MAC Address6 High Register */
    volatile uint32_t MACA6LR; /**< \brief 0x0074  MAC Address6 Low Register */
    volatile uint32_t MACA7HR; /**< \brief 0x0078  MAC Address7 High Register */
    volatile uint32_t MACA7LR; /**< \brief 0x007C  MAC Address7 Low Register */
    volatile uint32_t MACA8HR; /**< \brief 0x0080  MAC Address8 High Register */
    volatile uint32_t MACA8LR; /**< \brief 0x0084  MAC Address8 Low Register */
    volatile uint32_t MACA9HR; /**< \brief 0x0088  MAC Address9 High Register */
    volatile uint32_t MACA9LR; /**< \brief 0x008C  MAC Address9 Low Register */
    volatile uint32_t MACA10HR;/**< \brief 0x0090  MAC Address10 High Register */
    volatile uint32_t MACA10LR;/**< \brief 0x0094  MAC Address10 Low Register */
    volatile uint32_t MACA11HR;/**< \brief 0x0098  MAC Address11 High Register */
    volatile uint32_t MACA11LR;/**< \brief 0x009C  MAC Address11 Low Register */
    volatile uint32_t MACA12HR;/**< \brief 0x00A0  MAC Address12 High Register */
    volatile uint32_t MACA12LR;/**< \brief 0x00A4  MAC Address12 Low Register */
    volatile uint32_t MACA13HR;/**< \brief 0x00A8  MAC Address13 High Register */
    volatile uint32_t MACA13LR;/**< \brief 0x00AC  MAC Address13 Low Register */
    volatile uint32_t MACA14HR;/**< \brief 0x00B0  MAC Address14 High Register */
    volatile uint32_t MACA14LR;/**< \brief 0x00B4  MAC Address14 Low Register */
    volatile uint32_t MACA15HR;/**< \brief 0x00B8  MAC Address15 High Register */
    volatile uint32_t MACA15LR;/**< \brief 0x00BC  MAC Address15 Low Register */

    volatile uint32_t MACACR;  /**< \brief 0x00C0  AN Control Register */
    volatile uint32_t MACASR;  /**< \brief 0x00C4  AN status Register */
    volatile uint32_t MACAAR;  /**< \brief 0x00C8  Auto-Negot Adv Register */
    volatile uint32_t MACALPAR;/**< \brief 0x00CC Link Partner Ability Register */
    volatile uint32_t MACAER;  /**< \brief 0x00D0  Expansion Register */
    volatile uint32_t MACTESR; /**< \brief 0x00D4  TBI Extended st Register */
    volatile uint32_t MACMSR;  /**< \brief 0x00D8  SGMII/RGMII st Register */

    volatile uint32_t RESERVED1[969]; /* 0x00DC - 0x0FFC */

    volatile uint32_t DMABMR;   /**< \brief 0x1000  Bus Mode Register */
    volatile uint32_t DMATPDR;  /**< \brief 0x1004  Transmit Poll Register */
    volatile uint32_t DMARPDR;  /**< \brief 0x1008  Receive Poll Register */
    volatile uint32_t DMARDLAR; /**< \brief 0x100C  Rec DescList Addr Register */
    volatile uint32_t DMATDLAR; /**< \brief 0x1010  Tran Desc Register */
    volatile uint32_t DMASR;    /**< \brief 0x1014  status Register */
    volatile uint32_t DMAOMR;   /**< \brief 0x1018  Operation Mode Register */
    volatile uint32_t DMAIER;   /**< \brief 0x101C  Interrupt Enable Register */
    volatile uint32_t DMAMFBOCR;/**< \brief 0x1020  Missed Frame Register */

    volatile uint32_t RESERVED2[9]; /**< \brief 0x1024 - 0x1044 */

    volatile uint32_t DMACHTDR; /**< \brief 0x1048  CurHostTrandesc Register */
    volatile uint32_t DMACHRDR; /**< \brief 0x104C  Receive DescRegister */
    volatile uint32_t DMACHTBAR;/**< \brief 0x1050  Buffer Addr Register */
    volatile uint32_t DMACHRBAR;/**< \brief 0x1054 Receive Buf Addr Register */

} amhw_zlg_eth_t;
#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif
