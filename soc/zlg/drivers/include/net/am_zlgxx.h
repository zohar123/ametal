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
 * \brief am_zlgxx.h
 *
 * \internal
 * \par Modification History
 * - 1.00 19-06-11  wyy, first implementation
 * \endinternal
 */
#ifndef __AM_ZLG_XX_H
#define __AM_ZLG_XX_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include "stdint.h"
#define __IO  volatile
#define   __I volatile const       /*!< Defines 'read only' permissions                 */
typedef int32_t s32;
typedef int16_t s16;
typedef int8_t s8;

typedef const int32_t sc32; /*!< Read Only */
typedef const int16_t sc16; /*!< Read Only */
typedef const int8_t sc8; /*!< Read Only */

typedef __IO int32_t vs32;
typedef __IO int16_t vs16;
typedef __IO int8_t vs8;

typedef __I int32_t vsc32; /*!< Read Only */
typedef __I int16_t vsc16; /*!< Read Only */
typedef __I int8_t vsc8; /*!< Read Only */

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef const uint32_t uc32; /*!< Read Only */
typedef const uint16_t uc16; /*!< Read Only */
typedef const uint8_t uc8; /*!< Read Only */

typedef __IO uint32_t vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t vu8;

typedef __I uint32_t vuc32; /*!< Read Only */
typedef __I uint16_t vuc16; /*!< Read Only */
typedef __I uint8_t vuc8; /*!< Read Only */

typedef enum {
    RESET = 0, SET = !RESET
} flag_status;

typedef enum {
    DISABLE = 0, ENABLE = !DISABLE
} func_state_t;
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

#define AM_ETH_PERIPH_BASE           ((uint32_t)0x40000000) /*!< Peripheral base address in the alias region                                */
#define AM_ETH_AHB1PERIPH_BASE       (AM_ETH_PERIPH_BASE + 0x00020000)
#define AM_ETH_BASE              (AM_ETH_AHB1PERIPH_BASE + 0x8000)
#define AM_ETH_MAC_BASE          (AM_ETH_BASE)
#define AM_ETH_MACCR_TE      ((uint32_t)0x00000008)  /* Transmitter enable */
#define AM_ETH_MACCR_RE      ((uint32_t)0x00000004)  /* Receiver enable */

/* Bit definition for Ethernet MAC Hash Table High Register */
#define AM_ETH_MACHTHR_HTH   ((uint32_t)0xFFFFFFFF)  /* Hash table high */

/* Bit definition for Ethernet MAC Hash Table Low Register */
#define AM_ETH_MACHTLR_HTL   ((uint32_t)0xFFFFFFFF)  /* Hash table low */

#define AM_ETH_MACMIIAR_PA   ((uint32_t)0x0000F800)  /* Physical layer address */ 
#define AM_ETH_MACMIIAR_MR   ((uint32_t)0x000007C0)  /* MII register in the selected PHY */ 
#define AM_ETH_MACMIIAR_CR_DIV42   ((uint32_t)0x00000000)  /* HCLK:60-100 MHz; MDC clock= HCLK/42 */
#define AM_ETH_MACMIIAR_CR_DIV62   ((uint32_t)0x00000004)  /* HCLK:100-150 MHz; MDC clock= HCLK/62 */
#define AM_ETH_MACMIIAR_CR_DIV16   ((uint32_t)0x00000008)  /* HCLK:20-35 MHz; MDC clock= HCLK/16 */
#define AM_ETH_MACMIIAR_CR_DIV26   ((uint32_t)0x0000000C)  /* HCLK:35-60 MHz; MDC clock= HCLK/26 */
#define AM_ETH_MACMIIAR_CR_DIV102  ((uint32_t)0x00000010)  /* HCLK:150-168 MHz; MDC clock= HCLK/102 */
#define AM_ETH_MACMIIAR_MW   ((uint32_t)0x00000002)  /* MII write */ 
#define AM_ETH_MACMIIAR_MB   ((uint32_t)0x00000001)  /* MII busy */ 
#define AM_ETH_DMABMR_USP       ((uint32_t)0x00800000)  /* Use separate PBL */
#define AM_ETH_DMABMR_EDE       ((uint32_t)0x00000080)  /* Enhanced Descriptor Enable */
#define AM_ETH_DMABMR_SR        ((uint32_t)0x00000001)  /* Software reset */
#define AM_ETH_DMASR_RBUS       ((uint32_t)0x00000080)  /* Receive buffer unavailable status */
#define AM_ETH_DMASR_TUS        ((uint32_t)0x00000020)  /* Transmit underflow status */
#define AM_ETH_DMASR_TBUS       ((uint32_t)0x00000004)  /* Transmit buffer unavailable status */
#define AM_ETH_DMAOMR_FTF       ((uint32_t)0x00100000)  /* Flush transmit FIFO */
#define AM_ETH_DMAOMR_ST        ((uint32_t)0x00002000)  /* Start/stop transmission command */
#define AM_ETH_DMAOMR_SR        ((uint32_t)0x00000002)  /* Start/stop receive */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __AM_ZLG_XX_H */

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
