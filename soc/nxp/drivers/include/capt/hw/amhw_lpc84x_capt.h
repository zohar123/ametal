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

#ifndef __AMHW_LPC84X_CAPT_H
#define __AMHW_LPC84X_CAPT_H

#include "ametal.h"
#include "lpc84x_periph_map.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup amhw_lpc84x_if_capt
 * \copydoc amhw_lpc84x_capt.h
 * @{
 */

/**
 * \brief capacitive touch Register Layout Typedef
 */
typedef struct amhw_lpc84x_capt {
    __IO uint32_t ctrl;              /**< \brief control Register*/
    __IO uint32_t status;            /**< \brief status Register*/
    __IO uint32_t poll_tcnt;         /**< \brief polling counter and measurement counter */
         uint32_t reserved0;
    __IO uint32_t intenset;          /**< \brief Interrupt enable Register */
    __IO uint32_t intenclr;          /**< \brief Interrupt enable clear Register*/
    __I  uint32_t intstat;           /**< \brief Interrupt status Register */
         uint32_t reserved1;
    __I  uint32_t touch;             /**< \brief touch event Register*/
         uint8_t  reserved_2[4056];
    __I  uint32_t id;                /**< \brief BLOCK ID */
} amhw_lpc84x_capt_t;

/**
 * \name CTRL - Configuration and control to setup the functional clock,
 *              the rules, and the pin selections and rules.
 * @{
 */
#define AMHW_LPC84X_CAPT_CTRL_POLLMODE(x)       ((((uint32_t)(x)) << (0U)) & (0x3U))

#define AMHW_LPC84X_CAPT_CTRL_TYPE(x)           ((((uint32_t)(x)) << (2U)) & (0xCU))

#define AMHW_LPC84X_CAPT_CTRL_TRIGGER(x)        ((((uint32_t)(x)) << (4U)) & (0x10U))

#define AMHW_LPC84X_CAPT_CTRL_WAIT(x)           ((((uint32_t)(x)) << (5U)) & (0x20U))

#define AMHW_LPC84X_CAPT_CTRL_DMA(x)            ((((uint32_t)(x)) << (6U)) & (0xC0U))

#define AMHW_LPC84X_CAPT_CTRL_FDIV(x)           ((((uint32_t)(x)) << (8U)) & (0xF00U))

#define AMHW_LPC84X_CAPT_CTRL_XPINUSE(x)        ((((uint32_t)(x)) << (12U)) & (0x3000U))

#define AMHW_LPC84X_CAPT_CTRL_INCHANGE(x)       ((((uint32_t)(x)) << (15U)) & (0x8000U))

#define AMHW_LPC84X_CAPT_CTRL_XPINSEL(x)        ((((uint32_t)(x)) << (16U)) & (0xFFFF0000U))
/** @} */

/**
 * @name POLL_TCNT - This sets up the polling counter
 *                    and measurement counter rules.
 * @{
 */
#define AMHW_LPC84X_CAPT_POLL_TCNT_TCNT(x)         ((((uint32_t)(x)) << (0U)) & (0xFFFU))

#define AMHW_LPC84X_CAPT_POLL_TCNT_TOUT(x)         ((((uint32_t)(x)) << (12U)) & (0xF000U))

#define AMHW_LPC84X_CAPT_POLL_TCNT_POLL(x)         ((((uint32_t)(x)) << (16U)) & (0xFF0000U))

#define AMHW_LPC84X_CAPT_POLL_TCNT_MDELAY(x)       ((((uint32_t)(x)) << (24U)) & (0x3000000U))

#define AMHW_LPC84X_CAPT_POLL_TCNT_RDELAY(x)       ((((uint32_t)(x)) << (26U)) & (0xC000000U))

#define AMHW_LPC84X_CAPT_POLL_TCNT_TCHLOW_ER(x)    (((((uint32_t)(x)) << (31U))) & (0x80000000U))
/** @} */

/**
 * \name INTENSET - Interrupt enable
 * @{
 */
#define AMHW_LPC84X_CAPT_INTENSET_YESTOUCH(x)      ((((uint32_t)(x)) << (0U)) & (0x1U))

#define AMHW_LPC84X_CAPT_INTENSET_NOTOUCH(x)       ((((uint32_t)(x)) << (1U)) & (0x2U))

#define AMHW_LPC84X_CAPT_INTENSET_POLLDONE(x)      ((((uint32_t)(x)) << (2U)) & (0x4U))

#define AMHW_LPC84X_CAPT_INTENSET_TIMEOUT(x)       ((((uint32_t)(x)) << (3U)) & (0x8U))

#define AMHW_LPC84X_CAPT_INTENSET_OVERUN(x)        (((((uint32_t)(x)) << (4U))) & (0x10U))
/** @} */


/**
 * \name TOUCH - Last touch event (touch or no-touch) in context.
 * @{
 */
#define AMHW_LPC84X_CAPT_TOUCH_COUNT_MASK          (0xFFFU)
#define AMHW_LPC84X_CAPT_TOUCH_COUNT_SHIFT         (0U)

#define AMHW_LPC84X_CAPT_TOUCH_XVAL_MASK           (0xF000U)
#define AMHW_LPC84X_CAPT_TOUCH_XVAL_SHIFT          (12U)

#define AMHW_LPC84X_CAPT_TOUCH_ISTOUCH_MASK        (0x10000U)
#define AMHW_LPC84X_CAPT_TOUCH_ISTOUCH_SHIFT       (16U)

#define AMHW_LPC84X_CAPT_TOUCH_ISTO_MASK           (0x20000U)
#define AMHW_LPC84X_CAPT_TOUCH_ISTO_SHIFT          (17U)

#define AMHW_LPC84X_CAPT_TOUCH_SEQ_MASK            (0xF00000U)
#define AMHW_LPC84X_CAPT_TOUCH_SEQ_SHIFT           (20U)

#define AMHW_LPC84X_CAPT_TOUCH_CHANGE_MASK         (0x80000000U)
#define AMHW_LPC84X_CAPT_TOUCH_CHANGE_SHIFT        (31U)
/** @} */

typedef enum amhw_lpc84x_capt_pollmode {
    KCAPT_INACTIVE     = 0U,
    KCAPT_POLL_NOW     = 1U,
    KCAPT_CONTINUOUS   = 2U,
} amhw_lpc84x_capt_pollmode_t;

/**
* \brief Set the CAPT polling mode.
*
* \param[in] p_hw_capt: CAPT peripheral base address.
* \param[in] mode:      The selection of polling mode.
*
*/
void amhw_lpc84x_capt_pollmode_set (amhw_lpc84x_capt_t         *p_hw_capt,
                                    amhw_lpc84x_capt_pollmode_t mdoe);

typedef enum amhw_lpc84x_capt_trigger_mode
{
    /**< \brief YH port pin trigger mode. */
    KCAPT_YHPORT_TRIGGERMODE     = 0U,

    /**< \brief Analog comparator trigger mode. */
    KCAPT_COMPARATOR_TRIGGERMODE = 1U,

} amhw_lpc84x_capt_trigger_mode_t;

/**
* \brief Set the CAPT trigger mode.
*
* \param[in] p_hw_capt: CAPT peripheral base address.
* \param[in] mode:      The selection of trigger mode.
*
*/
void amhw_lpc84x_capt_trigger_mode_set (amhw_lpc84x_capt_t             *p_hw_capt,
                                        amhw_lpc84x_capt_trigger_mode_t mdoe);

/**
* \brief Controls when the next X measurement in the sequence may commence
*
* \param[in] p_hw_capt: CAPT peripheral base address.
* \param[in] flag:      0 : The next X measurement starts at the normal time.
*                       1 : When the ISTOUCH bit in the TOUCH register equals ¡®1¡¯, the
*                           module will wait until the TOUCH register is read before starting
*                           the next measurement. Other-wise, measurements continue.
*
*
*/
void amhw_lpc84x_capt_wait_set (amhw_lpc84x_capt_t *p_hw_capt,
                                am_bool_t           flag);

typedef enum amhw_lpc84x_capt_dma_mode
{
    KCAPT_DMA_TRIGGER_ON_TOUCH = 1U,  /**< \brief Trigger on touch. */
    KCAPT_DMA_TRIGGER_ON_BOTH  = 2U,  /**< \brief Trigger on both touch and no-touch. */
    KCAPT_DMA_TRIGGER_ON_ALL   = 3U,  /**< \brief Trigger on all touch, no-touch and time-out.  */
} amhw_lpc84x_capt_dma_mode_t;

/**
* \brief Enable DMA feature.
*
* \param[in] base CAPT peripheral base address.
* \param[in] mode Select how DMA triggers are generated.
*
*/
void amhw_lpc84x_capt_dma_enable(amhw_lpc84x_capt_t         *p_hw_capt,
                                 amhw_lpc84x_capt_dma_mode_t mode);

/**
* \brief Enable DMA feature.
*
* \param[in] base CAPT peripheral base address.
* \param[in] mode Select how DMA triggers are generated.
*
*/
void amhw_lpc84x_capt_dma_disable(amhw_lpc84x_capt_t         *p_hw_capt,
                                  amhw_lpc84x_capt_dma_mode_t mode);

/**
* \brief CAPT fdiv set
*
* \param[in] base CAPT peripheral base address.
* \param[in] div  0x0-0xf,Function clock divider. The function clock is divided by FDIV+1
*                 to produce the divided FCLK for the module.
*
*/
void amhw_lpc84x_capt_fdiv_set (amhw_lpc84x_capt_t *p_hw_capt,
                                uint8_t             div);

typedef enum amhw_lpc84x_capt_xpins_mode
{
    /**< \brief Xpins enabled in the XPINSEL field are controlled to HIGH-Z mode when not active. */
    KCAPT_INACTIVE_XPINS_HIGHZ     = 0U,

    /**< \brief Xpins enabled in the XPINSEL field are controlled to be driven low mode when not active. */
    KCAPT_INACTIVE_XPINS_DRIVENLOW = 1U,

} amhw_lpc84x_capt_xpins_mode_t;

/**
* \brief Determines how X pins enabled in the XPINSEL field are controlled when not active.
*
* \param[in] base CAPT peripheral base address.
* \param[in] mode  Xpins mode.
*
*/
void amhw_lpc84x_capt_xpin_mode (amhw_lpc84x_capt_t           *p_hw_capt,
                                 amhw_lpc84x_capt_xpins_mode_t mode);

typedef enum amhw_lpc84x_capt_xpins {
    KCAPT_X0PIN = 1U << 0U,                /**< \brief CAPT_X0 pin. */
    KCAPT_X1PIN = 1U << 1U,                /**< \brief CAPT_X1 pin. */
    KCAPT_X2PIN = 1U << 2U,                /**< \brief CAPT_X2 pin. */
    KCAPT_X3PIN = 1U << 3U,                /**< \brief CAPT_X3 pin. */
    KCAPT_X4PIN = 1U << 4U,                /**< \brief CAPT_X4 pin. */
    KCAPT_X5PIN = 1U << 5U,                /**< \brief CAPT_X5 pin. */
    KCAPT_X6PIN = 1U << 6U,                /**< \brief CAPT_X6 pin. */
    KCAPT_X7PIN = 1U << 7U,                /**< \brief CAPT_X7 pin. */
    KCAPT_X8PIN = 1U << 8U,                /**< \brief CAPT_X8 pin. */
    KCAPT_X9PIN = 1U << 9U,                /**< \brief CAPT_X9 pin. */
    KCAPT_X10PIN = 1U << 10U,              /**< \brief CAPT_X10 pin. */
    KCAPT_X11PIN = 1U << 11U,              /**< \brief CAPT_X11 pin. */
    KCAPT_X12PIN = 1U << 12U,              /**< \brief CAPT_X12 pin. */
    KCAPT_X13PIN = 1U << 13U,              /**< \brief CAPT_X13 pin. */
    KCAPT_X14PIN = 1U << 14U,              /**< \brief CAPT_X14 pin. */
    KCAPT_X15PIN = 1U << 15U,              /**< \brief CAPT_X15 pin. */
} amhw_lpc84x_capt_xpins_t;

/**
* \brief Selects which of the available X pins are enabled.
*        Writing ¡®1¡¯ to a bit enables the corresponding X pin, ¡®0¡¯ disables.
*
* \param[in] base CAPT peripheral base address.
* \param[in] pins  Xpins.
*
*/
am_static_inline
void amhw_lpc84x_capt_xpin_enable (amhw_lpc84x_capt_t      *p_hw_capt,
                                   amhw_lpc84x_capt_xpins_t pins)
{
    p_hw_capt->ctrl |= AMHW_LPC84X_CAPT_CTRL_XPINSEL(pins);
}

/**
* \brief Selects which of the available X pins are disabled.
*
* \param[in] base CAPT peripheral base address.
* \param[in] pins  Xpins.
*
*/
am_static_inline
void amhw_lpc84x_capt_xpin_disable (amhw_lpc84x_capt_t      *p_hw_capt,
                                    amhw_lpc84x_capt_xpins_t pins)
{
    p_hw_capt->ctrl &= ~AMHW_LPC84X_CAPT_CTRL_XPINSEL(pins);
}

/**
* \brief CAPT ctrl reg write
*
* \param[in] base  CAPT peripheral base address.
* \param[in] flag  write ctrl reg value.
*
*/
am_static_inline
void amhw_lpc84x_capt_ctrl_write (amhw_lpc84x_capt_t *p_hw_capt,
                                  uint32_t            flag)
{
    p_hw_capt->ctrl = flag;
}

typedef enum amhw_lpc84x_capt_int {
    KCAPT_YESTOUCH_INT    = (1UL << 0),
    KCAPT_NOTOUCH_INT     = (1UL << 1),
    KCAPT_POLLDONE_INT    = (1UL << 2),
    KCAPT_TIMEOUT_INT     = (1UL << 3),
    KCAPT_OVERRUN_INT     = (1UL << 4)
} amhw_lpc84x_capt_int_t;

/**
* \brief Enable interrupt features.
*
* \param[in] base CAPT peripheral base address.
* \param[in] mask The mask of enabling interrupt features.
*            Please refer to "amhw_lpc84x_capt_int_t".
*/
am_static_inline
void amhw_lpc84x_capt_int_enable(amhw_lpc84x_capt_t      *p_hw_capt,
                                 amhw_lpc84x_capt_int_t   mask)
{
    p_hw_capt->intenset |= mask;
}

/**
* \brief Disable the interrupts' status flags.
*
* \param[in] p_hw_capt: CAPT peripheral base address.
*
* \param[in] mask : The mask of clearing the interrupts' status flags,
*                   please refer to "amhw_lpc84x_capt_int_t".
*
*/
am_static_inline
void amhw_lpc84x_capt_int_disable(amhw_lpc84x_capt_t      *p_hw_capt,
                                  amhw_lpc84x_capt_int_t   mask)
{
    p_hw_capt->intenclr |= mask;
}

/**
* \brief Get Clear the interrupts' status flags.
*
* \param[in] p_hw_capt: CAPT peripheral base address.
*
* \param[in] mask : The mask of clearing the interrupts' status flags,
*                   please refer to "amhw_lpc84x_capt_int_t".
*
*/
am_static_inline
void amhw_lpc84x_capt_int_clear(amhw_lpc84x_capt_t      *p_hw_capt,
                                amhw_lpc84x_capt_int_t   mask)
{
    p_hw_capt->status = mask;
}

/**
* \brief Get CAPT interrupts' status flags.
*
* \param[in] p_hw_capt: CAPT peripheral base address.
*
* \return The mask of interrupts' status flags. please refer to "amhw_lpc84x_int_t".
*
*/
am_static_inline
uint32_t amhw_lpc84x_capt_intstat_get(amhw_lpc84x_capt_t *p_hw_capt)
{
    return p_hw_capt->intstat;
}

typedef enum amhw_lpc84x_capt_status {

    KCAPT_YESTOUCH_STATUS = (1UL << 0),
    KCAPT_NOSTOUCH_STATUS = (1UL << 1),
    KCAPT_POLLDONE_STATUS = (1UL << 2),
    KCAPT_TIMEOUT_STATUS  = (1UL << 3),
    KCAPT_OVERRUN_STATUS  = (1UL << 4),
    KCAPT_BUSY_STATUS     = (1UL << 8),
    KCAPT_XMAX_STATUS     = 0xF0000U,

} amhw_lpc84x_capt_status_t;

/**
* \brief Get CAPT status flags.
*
* \param[in] p_hw_capt: CAPT peripheral base address.
* \return:  The mask of CAPT status flags. Please refer to "amhw_lpc84x_capt_status_t"
*           Or use CAPT_GET_XMAX_NUMBER(mask) to get XMAX number.
*/
am_static_inline
uint32_t amhw_lpc84x_capt_status_get(amhw_lpc84x_capt_t *p_hw_capt)
{
    return p_hw_capt->status;
}

/**
* \brief Set Sets the count threshold in divided FCLKs between touch and no-touch.
*
* \param[in] p_hw_capt: CAPT peripheral base address.
* \param[in] count:     The count threshold.
*
*/
am_static_inline
void amhw_lpc84x_capt_threshold_set(amhw_lpc84x_capt_t *p_hw_capt,
                                    uint32_t            count)
{
    p_hw_capt->poll_tcnt = ((p_hw_capt->poll_tcnt & ~0xFFFU) |
                            AMHW_LPC84X_CAPT_POLL_TCNT_TCNT(count));
}

/**
* \brief Sets the count value at which a time-out event
*        occurs if a measurement has not triggered.
*
* \param[in] p_hw_capt: CAPT peripheral base address.
* \param[in] tout:      The time-out count value is calculated as 2^TOUT.
*                       TOUT must be less than 13, so the legal val-ues are 0, 1, ... 12.
*
*/
am_static_inline
void amhw_lpc84x_capt_timeout_set(amhw_lpc84x_capt_t *p_hw_capt,
                                  uint8_t             tout)
{
    p_hw_capt->poll_tcnt = ((p_hw_capt->poll_tcnt & ~0xF000U) |
                            AMHW_LPC84X_CAPT_POLL_TCNT_TOUT(tout));
}

/**
* \brief Sets the time delay between polling rounds (successive sets of Xmeasurements).
*        After each polling round completes, the modulewill wait 4096 x POLL divided
*        FCLKs before starting the next polling round.
*
* \param[in] p_hw_capt: CAPT peripheral base address.
* \param[in] delay:     delay value
*
*/
am_static_inline
void amhw_lpc84x_capt_poll_delay_set(amhw_lpc84x_capt_t *p_hw_capt,
                                     uint8_t             delay)
{
    p_hw_capt->poll_tcnt = ((p_hw_capt->poll_tcnt & ~0xFF0000U) |
                             AMHW_LPC84X_CAPT_POLL_TCNT_POLL(delay));
}

typedef enum amhw_lpc84x_capt_meas_delay
{
    KCAPT_MEASURE_NOWAIT     = 0U,         /**< \brief Don¡¯t wait. */
    KCAPT_MEASURE_WAIT3FCLKS = 1U,         /**< \brief Wait 3 divided FCLKs. */
    KCAPT_MEASURE_WAIT5FCLKS = 2U,         /**< \brief Wait 5 divided FCLKs. */
    KCAPT_MEASURE_WAIT9FCLKS = 3U,         /**< \brief Wait 9 divided FCLKs. */
} amhw_lpc84x_capt_meas_delay_t;

/**
* \brief Specifies the time delay after entering step 3 (measure voltage state),
*        before sampling the YH port pin or analog comarator output.
*
* \param[in] p_hw_capt: CAPT peripheral base address.
* \param[in] mdelay:    Wait cycle
*
*/
am_static_inline
void amhw_lpc84x_capt_meas_delay_set(amhw_lpc84x_capt_t           *p_hw_capt,
                                     amhw_lpc84x_capt_meas_delay_t mdelay)
{
    p_hw_capt->poll_tcnt = ((p_hw_capt->poll_tcnt & ~0x3000000U) |
                            AMHW_LPC84X_CAPT_POLL_TCNT_MDELAY(mdelay));
}

typedef enum amhw_lpc84x_capt_reset_delay
{
    KCAPT_RESET_WAIT1FCLKS = 0U,           /**< \brief 1 divided FCLK. */
    KCAPT_RESET_WAIT2FCLKS = 1U,           /**< \brief 2 divided FCLK. */
    KCAPT_RESET_WAIT4FCLKS = 2U,           /**< \brief 4 divided FCLK. */
    KCAPT_RESET_WAIT8FCLKS = 3U,           /**< \brief 8 divided FCLK. */
} amhw_lpc84x_capt_reset_delay_t;

/**
* \brief Specifies the number of divided FCLKs the module will remain in
*        step 0 before starting the next X measurement in the polling round.
*
* \param[in] p_hw_capt: CAPT peripheral base address.
* \param[in] rdelay:    Wait cycle
*
*/
am_static_inline
void amhw_lpc84x_capt_reset_delay_set(amhw_lpc84x_capt_t            *p_hw_capt,
                                      amhw_lpc84x_capt_reset_delay_t rdelay)
{
    p_hw_capt->poll_tcnt = ((p_hw_capt->poll_tcnt & ~0xC000000U) |
                            AMHW_LPC84X_CAPT_POLL_TCNT_RDELAY(rdelay));
}

/**
* \brief Specifies whether a touched sensor triggers at a lower or
*        higher count than an untouched sensor.
*
* \param[in] p_hw_capt: CAPT peripheral base address.
* \param[in] flag:    0: Trigger at count > TCNT is a touch. Trigger
*                        at count TCNT is a no-touch.
*                     1: Trigger at count TCNT is a touch. Trigger
*                        at count > TCNT is a no-touch.
*/
am_static_inline
void amhw_lpc84x_capt_tchlow_er_set(amhw_lpc84x_capt_t *p_hw_capt,
                                    am_bool_t           flag)
{
    p_hw_capt->poll_tcnt = ((p_hw_capt->poll_tcnt & ~0x80000000U) |
                            AMHW_LPC84X_CAPT_POLL_TCNT_TCHLOW_ER(flag));
}

/**
* \brief Contains the count value reached at trigger or time-out.
*
* \param[in] p_hw_capt: CAPT peripheral base address.
* \return:   Contains the count value.
*/
am_static_inline
uint16_t amhw_lpc84x_capt_touch_count_get(amhw_lpc84x_capt_t *p_hw_capt)
{
    return (uint16_t)((p_hw_capt->touch & AMHW_LPC84X_CAPT_TOUCH_COUNT_MASK) >>
                      AMHW_LPC84X_CAPT_TOUCH_COUNT_SHIFT);
}

/**
* \brief Contains the index of the X pin for the current measurement,
*        or lowest X for amultiple-pin poll now measurement
*
* \param[in] p_hw_capt: CAPT peripheral base address.
* \return:   Contains the index of the X pin.
*/
am_static_inline
uint8_t amhw_lpc84x_capt_touch_xval_get(amhw_lpc84x_capt_t *p_hw_capt)
{
    return (uint8_t)((p_hw_capt->touch & AMHW_LPC84X_CAPT_TOUCH_XVAL_MASK) >>
                     AMHW_LPC84X_CAPT_TOUCH_XVAL_SHIFT);
}

/**
* \brief ¡®1¡¯ if the trigger is due to a touch event,
*        ¡®0¡¯ if the trigger is due to a no-touchevent.
*
* \param[in] p_hw_capt: CAPT peripheral base address.
* \return:   is touch or no-touch.
*/
am_static_inline
am_bool_t amhw_lpc84x_capt_istouch_get(amhw_lpc84x_capt_t *p_hw_capt)
{
    return (am_bool_t)((p_hw_capt->touch & AMHW_LPC84X_CAPT_TOUCH_ISTOUCH_MASK) >>
                       AMHW_LPC84X_CAPT_TOUCH_ISTOUCH_SHIFT);
}

/**
* \brief ¡®1¡¯ if the measurement resulted in a time-out event, ¡®0¡¯ otherwise.
*
* \param[in] p_hw_capt: CAPT peripheral base address.
* \return:   ¡®1¡¯ if the measurement resulted in a time-out event, ¡®0¡¯ otherwise.
*/
am_static_inline
am_bool_t amhw_lpc84x_capt_isto_get(amhw_lpc84x_capt_t *p_hw_capt)
{
    return (am_bool_t)((p_hw_capt->touch & AMHW_LPC84X_CAPT_TOUCH_ISTO_MASK) >>
                       AMHW_LPC84X_CAPT_TOUCH_ISTO_SHIFT);
}

/**
* \brief Contains the 4-bit sequence number, which increments at the end of each polling round.
*
* \param[in] p_hw_capt: CAPT peripheral base address.
* \return:   sequence number.
*/
am_static_inline
uint8_t amhw_lpc84x_capt_seq_get(amhw_lpc84x_capt_t *p_hw_capt)
{
    return (uint8_t)((p_hw_capt->touch & AMHW_LPC84X_CAPT_TOUCH_SEQ_MASK) >>
                     AMHW_LPC84X_CAPT_TOUCH_SEQ_SHIFT);
}

/**
* \brief Will be ¡®1¡¯ for one bus clock at the end of each X measurement
*        while the data are changing, otherwise ¡®0¡¯.
*
*
* \param[in] p_hw_capt: CAPT peripheral base address.
* \return:   Touch data read while this bit is ¡®1¡¯ are invalid.
*/
am_static_inline
am_bool_t amhw_lpc84x_capt_change_get(amhw_lpc84x_capt_t *p_hw_capt)
{
    return (am_bool_t)((p_hw_capt->touch & AMHW_LPC84X_CAPT_TOUCH_CHANGE_MASK) >>
                       AMHW_LPC84X_CAPT_TOUCH_CHANGE_SHIFT);
}

/**
* \brief CAPT touch reg value get
*
*
* \param[in] p_hw_capt: CAPT peripheral base address.
* \return:   Touch reg value.
*/
am_static_inline
uint32_t amhw_lpc84x_capt_touch_get(amhw_lpc84x_capt_t *p_hw_capt)
{
    return p_hw_capt->touch;
}

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  /* __AMHW_LPC84X_CAPT_H */

/* end of file */
