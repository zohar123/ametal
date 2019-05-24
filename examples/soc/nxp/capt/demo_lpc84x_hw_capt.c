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
 * - 操作步骤：
 *   1. PIO0_31  连接电容触摸引脚 CAPTX0；
 *   2. PIO1_0   连接电容触摸引脚 CAPTX1；
 *   3. PIO1_1   连接电容触摸引脚 CAPTX2；
 *   3. PIO0_30  连接模拟比较ACMP。
 *
 * * - 实验现象：
 *   1. 用手触摸ACMP 与CAPTX0时，打印X0的触摸计数值为4095，yestouch开始计数，notouch停止计数；
 *   1. 用手触摸ACMP 与CAPTX1时，打印X1的触摸计数值为4095，yestouch开始计数，notouch停止计数；；
 *   1. 用手触摸ACMP 与CAPTX2时，打印X2的触摸计数值为4095，yestouch开始计数，notouch停止计数；；
 *
 * \internal
 * \par Modification history
 * - 1.00 19-03-25  ipk, first implementation.
 * \endinternal
 */

#include "hw/amhw_lpc84x_clk.h"
#include "hw/amhw_lpc84x_syscon.h"
#include "hw/amhw_lpc84x_capt.h"
#include "am_vdebug.h"
#include "am_int.h"
#include "am_clk.h"
#include <string.h>

#define  __LPX84X_CAPT_CLK        (4000000)  /**< \brief CAPT频率*/
#define  __USE_AVERAGED           1          /**< \brief 是否取均值 */
#define  __TOUCH_TRIGGERS_LOWER   0          /**< \brief 是否高于阈值触发*/
#define  __NUM_SENSORS            (3)        /**< \brief 触摸按键个数 */
#define  __LOW_SENSOR             (0)        /**< \brief 最低位触摸引脚为X0 */
#define  __NUM_CALIB_SAMPLES      (1000)     /**< \brief 计算触发阈值时的采样次数 */

volatile uint32_t  x_data[__NUM_SENSORS];    /**< \brief 触摸计数值 */

uint32_t timeout_cnt, overrun_cnt, yestouch_cnt, notouch_cnt;

/**
 * \brief CAPT中断函数
 */
static void __capt_irq_handler (void *p_arg)
{
    uint32_t  status;

    uint8_t   n;

    amhw_lpc84x_capt_t *p_hw_capt = (amhw_lpc84x_capt_t *)p_arg;

    status      = amhw_lpc84x_capt_status_get(p_hw_capt);

    for (n = 0; n != __NUM_SENSORS; n++) {
        if (amhw_lpc84x_capt_touch_xval_get(p_hw_capt) == (uint32_t)n) {
            x_data[n] = amhw_lpc84x_capt_touch_count_get(p_hw_capt);
            break;
        }
    }

    if (status & KCAPT_NOSTOUCH_STATUS) {
        notouch_cnt++;
    }
    if (status & (KCAPT_YESTOUCH_STATUS)) {
        yestouch_cnt++;
    }
    if (status & KCAPT_TIMEOUT_STATUS) {
        timeout_cnt++;
    }
    if (status & KCAPT_OVERRUN_STATUS) {
        overrun_cnt++;
    }

    amhw_lpc84x_capt_int_clear(p_hw_capt, status);
}

/**
 * \brief 计算touch的触发阈值
 */
static uint32_t __compute_notouch_baseline(amhw_lpc84x_capt_t *p_hw_capt)
{
    uint32_t temp_ctrl, save_ctrl, save_poll, n, k, s, divideby, rdelay, mult, waitclks, temp_count;
    uint32_t touch_threshold;
    uint32_t mean_notouch_baseline;
    uint32_t saved_low_cnt, saved_high_cnt;
    uint32_t ntch_high, ntch_low;
    uint32_t notouch_baseline[__NUM_SENSORS];
    uint32_t ntch_noise_margin;

    save_ctrl = p_hw_capt->ctrl;
    save_poll = p_hw_capt->poll_tcnt;

    divideby = 1 + ((save_ctrl & (0xF << 8)) >> 8);
    rdelay   = (save_poll & (0x3<<26)) >> 26;

    if (rdelay == 0) {
        mult = 0;
    } else {
        mult = 1 + (1 << rdelay);
    }

    waitclks = mult * divideby;

    temp_ctrl  = save_ctrl & ~((0x3 << 0));
    temp_ctrl |= KCAPT_POLL_NOW;

    mean_notouch_baseline = 0;
    saved_low_cnt = 0xFFF;
    saved_high_cnt = 0;

    for (s = 0; s != __NUM_SENSORS; s++) {

        notouch_baseline[s] = 0;

        temp_ctrl &= ~((uint32_t)0xFFFF << 16);
        temp_ctrl |= (1 << (s + __LOW_SENSOR)) << 16;

        for (n = 0; n != __NUM_CALIB_SAMPLES; n++) {

            amhw_lpc84x_capt_ctrl_write(p_hw_capt, temp_ctrl);

            while (!(amhw_lpc84x_capt_status_get(p_hw_capt) & KCAPT_POLLDONE_STATUS));

            temp_count = (amhw_lpc84x_capt_touch_get(p_hw_capt) & AMHW_LPC84X_CAPT_TOUCH_COUNT_MASK);

            if (temp_count > saved_high_cnt) {
                saved_high_cnt = temp_count;
            }
            if (temp_count < saved_low_cnt) {
                saved_low_cnt = temp_count;
            }

            notouch_baseline[s] += (amhw_lpc84x_capt_touch_get(p_hw_capt) & AMHW_LPC84X_CAPT_TOUCH_COUNT_MASK);

            if (waitclks == 0) {
                continue;
            }

            for (k = 0; k < waitclks / 7; k++);
        }

        notouch_baseline[s] /= __NUM_CALIB_SAMPLES;
        mean_notouch_baseline += notouch_baseline[s];
    }

    ntch_high = notouch_baseline[0];
    ntch_low = notouch_baseline[0];

    for (s = 0; s != __NUM_SENSORS - 1; s++) {

        if (notouch_baseline[s+1] > ntch_high) {
            ntch_high = notouch_baseline[s+1];
        }

        if (notouch_baseline[s+1] < ntch_low) {
            ntch_low = notouch_baseline[s+1];
        }
    }

#if __USE_AVERAGED == 1
    ntch_noise_margin = (ntch_high - ntch_low);
#else
    ntch_noise_margin = (saved_high_cnt - saved_low_cnt);
#endif

#if __TOUCH_TRIGGERS_LOWER == 1
#if __USE_AVERAGED == 1
    touch_threshold = ntch_low - (ntch_high - ntch_low + ntch_noise_margin);
#else
    touch_threshold = saved_low_cnt - (saved_high_cnt - saved_low_cnt + ntch_noise_margin);
#endif

#else
#if __USE_AVERAGED == 1
    touch_threshold = ntch_high + (ntch_high - ntch_low + ntch_noise_margin);
#else
    touch_threshold = saved_high_cnt + (saved_high_cnt - saved_low_cnt + noise_margin);
#endif

#endif

    mean_notouch_baseline /= __NUM_SENSORS;
    amhw_lpc84x_capt_ctrl_write(p_hw_capt, save_ctrl);

    return touch_threshold;
}

/**
 * \brief ACMP初始化
 */
static void lpc84x_acmp_init (amhw_lpc84x_acmp_t *p_hw_acmp)
{
    uint32_t flags;

    amhw_lpc84x_syscon_powerup(AMHW_LPC84X_SYSCON_PD_ACMP);

    amhw_lpc84x_clk_periph_enable(AMHW_LPC84X_CLK_ACMP);
    amhw_lpc84x_syscon_periph_reset(AMHW_LPC84X_RESET_ACMP);

    flags = AMHW_LPC84X_ACMP_CTRL_VP_BGAP   |   /* 基准电压正输入 */
            AMHW_LPC84X_ACMP_CTRL_VM_LADOUT |   /* 电压阶梯负输入 */
            AMHW_LPC84X_ACMP_CTRL_HYS_20MV  |   /* 迟滞电压为 20mV */
            (0 << 24)    ;                      /* 中断禁能 */

    amhw_lpc84x_acmp_config(p_hw_acmp, flags);

    amhw_lpc84x_acmp_ladsel_set(p_hw_acmp, 31);
    amhw_lpc84x_acmp_ladder_enable(p_hw_acmp);

}

/**
 * \brief CAPT初始化
 */
static void lpc84x_capt_init(amhw_lpc84x_capt_t  *p_hw_capt)
{
    uint32_t fro_clk;
    uint32_t touch_threshold;

    amhw_lpc84x_clk_periph_enable(AMHW_LPC84X_CLK_CAPT);
    amhw_lpc84x_syscon_periph_reset(AMHW_LPC84X_RESET_CAPT);

    LPC84X_SYSCON->captclksel = 0x0;

    fro_clk = am_clk_rate_get(CLK_FRO);

    amhw_lpc84x_capt_trigger_mode_set(p_hw_capt, KCAPT_COMPARATOR_TRIGGERMODE);
    amhw_lpc84x_capt_fdiv_set(p_hw_capt, (fro_clk / __LPX84X_CAPT_CLK) - 1);
    amhw_lpc84x_capt_xpin_mode(p_hw_capt, KCAPT_INACTIVE_XPINS_HIGHZ);

    /* Time out = 1<<TOUT = 2^TOUT divided FCLKs. 4096 divided FCLKs ~= 1 ms @ FCLK = 4 MHz */
    amhw_lpc84x_capt_timeout_set(p_hw_capt, 12);
    amhw_lpc84x_capt_poll_delay_set(p_hw_capt, 0);

    /* 设置低于阈值触发或高于阈值触发 */
#if (__TOUCH_TRIGGERS_LOWER == 1)
    amhw_lpc84x_capt_tchlow_er_set(p_hw_capt, 1);
#else
    amhw_lpc84x_capt_tchlow_er_set(p_hw_capt, 0);
#endif

    amhw_lpc84x_capt_reset_delay_set(p_hw_capt, KCAPT_RESET_WAIT8FCLKS);
    amhw_lpc84x_capt_meas_delay_set(p_hw_capt, KCAPT_MEASURE_WAIT9FCLKS);

    /* 计算触摸的触发阈值 */
    touch_threshold = __compute_notouch_baseline(p_hw_capt);
    amhw_lpc84x_capt_threshold_set(p_hw_capt, touch_threshold);

    /* 连接CAPT中断回调 */
    am_int_connect(INUM_ACMP_CAPT, __capt_irq_handler, (void *)p_hw_capt);

    /* 使能CAPT中断*/
    am_int_enable(INUM_ACMP_CAPT);

    /* 使能CAPT YESTOUCH 、NOTOUCH、TIMEOUT、OVERRUN中断*/
    amhw_lpc84x_capt_int_enable(p_hw_capt, KCAPT_YESTOUCH_INT |
                                           KCAPT_NOTOUCH_INT  |
                                           KCAPT_TIMEOUT_INT  |
                                           KCAPT_OVERRUN_INT);
    /* 设置为连续测量模式*/
    amhw_lpc84x_capt_pollmode_set(p_hw_capt, KCAPT_CONTINUOUS);

    /* 使能CAPT XPIN引脚*/
    amhw_lpc84x_capt_xpin_enable(p_hw_capt, KCAPT_X0PIN | KCAPT_X1PIN | KCAPT_X2PIN);
}

void demo_lpc84x_hw_capt_entry (amhw_lpc84x_capt_t *p_hw_capt,
                                amhw_lpc84x_acmp_t *p_hw_acmp)
{
    int                       x;

    /* 模拟比较初始化 */
    lpc84x_acmp_init(p_hw_acmp);

    /* CAPT初始化 */
    lpc84x_capt_init(p_hw_capt);

    while (1) {

        for (x = 0; x <= __NUM_SENSORS - 1; x++) {
          am_kprintf("X%d:%4d ", x, x_data[x]);
        }
        am_kprintf("timeout:%8d yestouch:%8d notouch:%8d\r\n",
                   timeout_cnt,
                   yestouch_cnt,
                   notouch_cnt);
    }
}

/* end of file */
