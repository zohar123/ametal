/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief lpc84x capacitive touch
 *
 * \internal
 * \par Modification history
 * - 1.00 19-03-25  ipk, first implementation.
 * \endinternal
 */

#include "hw/amhw_lpc84x_capt.h"

void amhw_lpc84x_capt_pollmode_set (amhw_lpc84x_capt_t         *p_hw_capt,
                                    amhw_lpc84x_capt_pollmode_t mdoe)
{
    while ((1U << 15) & p_hw_capt->ctrl);

    p_hw_capt->ctrl = (p_hw_capt->ctrl & (~0x3U)) |
                       AMHW_LPC84X_CAPT_CTRL_POLLMODE(KCAPT_INACTIVE);

    p_hw_capt->ctrl = (p_hw_capt->ctrl & (~0x3U)) |
                       AMHW_LPC84X_CAPT_CTRL_POLLMODE(mdoe);
}

void amhw_lpc84x_capt_trigger_mode_set (amhw_lpc84x_capt_t             *p_hw_capt,
                                        amhw_lpc84x_capt_trigger_mode_t mdoe)
{
    while ((1U << 15) & p_hw_capt->ctrl);

    p_hw_capt->ctrl = (p_hw_capt->ctrl & (~0x10U)) |
                       AMHW_LPC84X_CAPT_CTRL_TRIGGER(mdoe);
}

void amhw_lpc84x_capt_wait_set (amhw_lpc84x_capt_t *p_hw_capt,
                                am_bool_t           flag)
{
    while ((1U << 15) & p_hw_capt->ctrl);

    p_hw_capt->ctrl = (p_hw_capt->ctrl & (~0x20U)) |
                       AMHW_LPC84X_CAPT_CTRL_WAIT(flag);
}

void amhw_lpc84x_capt_dma_enable(amhw_lpc84x_capt_t         *p_hw_capt,
                                 amhw_lpc84x_capt_dma_mode_t mode)
{
    /* Before writing into CTRL register, INCHANGE(bit 15)should equal '0'. */
    while ((1U << 15) & p_hw_capt->ctrl);

    p_hw_capt->ctrl = (p_hw_capt->ctrl & ~0xC0U) | AMHW_LPC84X_CAPT_CTRL_DMA(mode);
}

void amhw_lpc84x_capt_dma_disable(amhw_lpc84x_capt_t         *p_hw_capt,
                                  amhw_lpc84x_capt_dma_mode_t mode)
{
    /* Before writing into CTRL register, INCHANGE(bit 15)should equal '0'. */
    while ((1U << 15) & p_hw_capt->ctrl);

    p_hw_capt->ctrl &= ~0xC0U;
}

void amhw_lpc84x_capt_fdiv_set (amhw_lpc84x_capt_t *p_hw_capt,
                                uint8_t             div)
{
    while ((1U << 15) & p_hw_capt->ctrl);

    p_hw_capt->ctrl = (p_hw_capt->ctrl & (~0xF00U)) |
                       AMHW_LPC84X_CAPT_CTRL_FDIV(div);
}

void amhw_lpc84x_capt_xpin_mode (amhw_lpc84x_capt_t           *p_hw_capt,
                                 amhw_lpc84x_capt_xpins_mode_t mode)
{
    while ((1U << 15) & p_hw_capt->ctrl);

    p_hw_capt->ctrl = (p_hw_capt->ctrl & (~0x3000U)) |
                       AMHW_LPC84X_CAPT_CTRL_XPINUSE(mode);
}

/* end of file */
