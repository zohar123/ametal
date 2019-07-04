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
 * \brief ZLG116 引脚及配置
 * 
 * \note 1. 可转移功能可以配置所有I/O引脚；
 *          GPIO功能和特殊功能属于固定功能，只能搭配对应的引脚使用 \n
 *          示例：PIO0_0引脚功能配置： \n 
 *          - 配置为GPIO功能使用宏：      PIO0_0_GPIO \n
 *          - 配置为比较器输入功能使用宏：PIO0_0_ACMP_I1 \n
 *          - 配置串口发送引脚功能使用宏：PIO_FUNC_U0_TXD
 *
 * \note 2. 复用功能方向由自身决定，PIOx_x_GPIO_INPUT或PIOx_x_GPIO_OUTPUT
 *          会将引脚配置为普通IO输入输出功能 
 *
 * \internal
 * \par Modification History
 * - 1.00 19-06-20  zjr, modified.

 * \endinternal
 */

#ifndef __ZLG116_PIN_SET_H
#define __ZLG116_PIN_SET_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ZLG116_pin.h"

/** 
 * \name UART1引脚配置
 * @{
 */
#define __UART1_RX            PIOA_10    //PIOA_10 PIOA_9  PIOB_7
#define __UART1_TX            PIOA_9     //PIOA_9  PIOA_10 PIOB_6
/** @} */

/** 
 * \name UART2引脚配置
 * @{
 */
#define __UART2_RX            PIOA_15    //PIOA_3  PIOA_15
#define __UART2_TX            PIOA_14    //PIOA_2  PIOA_14
/** @} */

/** 
 * \name SPI1引脚配置
 * @{
 */
#define __SPI1_SCK            PIOA_5     //PIOA_5  PIOB_3  PIOB_13
#define __SPI1_MISO           PIOA_6     //PIOA_6  PIOB_4  PIOB_14
#define __SPI1_MOSI           PIOA_7     //PIOA_7  PIOB_5  PIOB_15
/** @} */

/** 
 * \name ADC引脚配置
 * @{
 */
#define __ADC_IN0_GPIO        PIOA_0
#define __ADC_IN1_GPIO        PIOA_1
#define __ADC_IN2_GPIO        PIOA_2
#define __ADC_IN3_GPIO        PIOA_3
#define __ADC_IN4_GPIO        PIOA_4
#define __ADC_IN5_GPIO        PIOA_5
#define __ADC_IN6_GPIO        PIOA_6
#define __ADC_IN7_GPIO        PIOA_7
#define __ADC_IN8_GPIO        PIOB_0
#define __ADC_IN9_GPIO        PIOB_1
/** @} */

/** 
 * \name I2C引脚配置
 * @{
 */          
#define __I2C_SCL             PIOB_6     //PIOA_9  PIOA_11  PIOB_6  PIOB_8  PIOB_10  PIOB_13  PIOD_1
#define __I2C_SDA             PIOB_7     //PIOA_10 PIOA_12  PIOB_7  PIOB_9  PIOB_11  PIOB_14  PIOD_0
/** @} */

/** 
 * \name LED引脚配置
 * @{
 */
#define __LED0_GPIO           PIOB_1
#define __LED1_GPIO           PIOB_2
/** @} */

/** 
 * \name KEY引脚配置
 * @{
 */
#define __KEY_KP0_GPIO        PIOA_8
/** @} */

/** 
 * \name TIM1引脚配置
 * @{
 */
#define __TIM1_BKIN_GPIO      PIOA_6     //PIOA_6  PIOB_12
#define __TIM1_CH1N_GPIO      PIOA_7     //PIOA_7  PIOB_13
#define __TIM1_CH1_GPIO       PIOA_8 
#define __TIM1_CH2_GPIO       PIOA_9 
#define __TIM1_CH3_GPIO       PIOA_10    //PIOA_10 PIOB_15
#define __TIM1_CH3N_GPIO      PIOB_1 
#define __TIM1_CH4_GPIO       PIOA_11
#define __TIM1_ETR_GPIO       PIOA_12
#define __TIM1_CH2N_GPIO      PIOB_0     //PIOB_0  PIOB_14 
/** @} */

/** 
 * \name TIM2引脚配置
 * @{
 */
#define __TIM2_CH2_GPIO       PIOA_1     //PIOA_1  PIOB_3 
#define __TIM2_CH3_GPIO       PIOA_2     //PIOA_2  PIOB_10
#define __TIM2_CH4_GPIO       PIOA_3     //PIOA_3  PIOB_11
#define __TIM2_CH1_ETR_GPIO   PIOA_5     //PIOA_0  PIOA_5   PIOA_15
/** @} */

/** 
 * \name TIM3引脚配置
 * @{
 */
#define __TIM3_CH1_GPIO       PIOA_6     //PIOA_6  PIOB_4
#define __TIM3_CH2_GPIO       PIOA_7     //PIOA_7  PIOB_5
#define __TIM3_CH3_GPIO       PIOB_0
#define __TIM3_CH4_GPIO       PIOB_1
/** @} */

/** 
 * \name TIM14引脚配置
 * @{
 */
#define __TIM14_CH1_GPIO      PIOA_4     //PIOA_4 PIOA_7 PIOB_1
/** @} */

/** 
 * \name TIM16引脚配置
 * @{
 */
#define __TIM16_CH1_GPIO      PIOA_6     //PIOA_6  PIOB_8
#define __TIM16_CH1N_GPIO     PIOB_6
#define __TIM16_BKIN_GPIO     PIOB_5

#define __BUZZER_CHAN 1
/** @} */

/** 
 * \name TIM17引脚配置
 * @{
 */
#define __TIM17_CH1_GPIO      PIOA_7     //PIOA_7   PIOB_9
#define __TIM17_CH1N_GPIO     PIOB_7
#define __TIM17_BKIN_GPIO     PIOA_10    //PIOA_10  PIOB_4
/** @} */



#define __PIO_GPIO AM_ZLG116_GPIO_MODE_CODE(0x1)
#define __PIO_GPIO_TIM AM_ZLG116_GPIO_MODE_CODE(0xF) | AM_ZLG116_GPIO_MODE_CODE(0x1)



#if __UART1_RX == PIOA_10
    #define __GPIO_UART1_RX AM_ZLG116_GPIO_FUNC_CODE(0x1) | AM_ZLG116_GPIO_MODE_CODE(0x1)
#elif __UART1_RX == PIOA_9
    #define __GPIO_UART1_RX AM_ZLG116_GPIO_FUNC_CODE(0x3) | AM_ZLG116_GPIO_MODE_CODE(0x1)
#else
    #define __GPIO_UART1_RX AM_ZLG116_GPIO_FUNC_CODE(0x0) | AM_ZLG116_GPIO_MODE_CODE(0x1)
#endif
#if __UART1_TX == PIOA_9
    #define __GPIO_UART1_TX AM_ZLG116_GPIO_FUNC_CODE(0x1) | AM_ZLG116_GPIO_MODE_CODE(0x6)
#elif __UART1_TX == PIOA_10
    #define __GPIO_UART1_TX AM_ZLG116_GPIO_FUNC_CODE(0x3) | AM_ZLG116_GPIO_MODE_CODE(0x6)
#else
    #define __GPIO_UART1_TX AM_ZLG116_GPIO_FUNC_CODE(0x0) | AM_ZLG116_GPIO_MODE_CODE(0x6)
#endif



#define __GPIO_UART2_RX AM_ZLG116_GPIO_FUNC_CODE(0x1) | AM_ZLG116_GPIO_MODE_CODE(0x1)
#define __GPIO_UART2_TX AM_ZLG116_GPIO_FUNC_CODE(0x1) | AM_ZLG116_GPIO_MODE_CODE(0x6)



#define __GPIO_SPI1_SCK  AM_ZLG116_GPIO_FUNC_CODE(0x0) | AM_ZLG116_GPIO_MODE_CODE(0x6)
#define __GPIO_SPI1_MISO AM_ZLG116_GPIO_FUNC_CODE(0x0) | AM_ZLG116_GPIO_MODE_CODE(0x1)
#define __GPIO_SPI1_MOSI AM_ZLG116_GPIO_FUNC_CODE(0x0) | AM_ZLG116_GPIO_MODE_CODE(0x6)



#define __GPIO__ADC_IN  AM_ZLG116_GPIO_FUNC_CODE(0x0F) | AM_ZLG116_GPIO_MODE_CODE(0x0)



#if __I2C_SCL == PIOA_9
    #define __GPIO_I2C_SCL  AM_ZLG116_GPIO_FUNC_CODE(0x4) | AM_ZLG116_GPIO_MODE_CODE(0x7) | AM_ZLG116_GPIO_OUTRES_RATE_CODE(0x02)
#elif (__I2C_SCL == PIOA_11 || __I2C_SCL == PIOB_13)
    #define __GPIO_I2C_SCL  AM_ZLG116_GPIO_FUNC_CODE(0x5) | AM_ZLG116_GPIO_MODE_CODE(0x7) | AM_ZLG116_GPIO_OUTRES_RATE_CODE(0x02)
#else
    #define __GPIO_I2C_SCL  AM_ZLG116_GPIO_FUNC_CODE(0x1) | AM_ZLG116_GPIO_MODE_CODE(0x7) | AM_ZLG116_GPIO_OUTRES_RATE_CODE(0x02)
#endif

#if __I2C_SDA == PIOA_10
    #define __GPIO_I2C_SDA  AM_ZLG116_GPIO_FUNC_CODE(0x4) | AM_ZLG116_GPIO_MODE_CODE(0x7) | AM_ZLG116_GPIO_OUTRES_RATE_CODE(0x02)
#elif (__I2C_SDA == PIOA_12 || __I2C_SDA == PIOB_14)
    #define __GPIO_I2C_SDA  AM_ZLG116_GPIO_FUNC_CODE(0x5) | AM_ZLG116_GPIO_MODE_CODE(0x7) | AM_ZLG116_GPIO_OUTRES_RATE_CODE(0x02)
#else
    #define __GPIO_I2C_SDA  AM_ZLG116_GPIO_FUNC_CODE(0x1) | AM_ZLG116_GPIO_MODE_CODE(0x7) | AM_ZLG116_GPIO_OUTRES_RATE_CODE(0x02)
#endif




#define __GPIO_TIM1  AM_ZLG116_GPIO_FUNC_CODE(0x2) | AM_ZLG116_GPIO_MODE_CODE(0x6)

#define __GPIO_TIM2  AM_ZLG116_GPIO_FUNC_CODE(0x2) | AM_ZLG116_GPIO_MODE_CODE(0x6)

#define __GPIO_TIM3  AM_ZLG116_GPIO_FUNC_CODE(0x1) | AM_ZLG116_GPIO_MODE_CODE(0x6)

#if __TIM14_CH1_GPIO == PIOB_1
    #define __GPIO_TIM14  AM_ZLG116_GPIO_FUNC_CODE(0x0) | AM_ZLG116_GPIO_MODE_CODE(0x6)
#else
    #define __GPIO_TIM14  AM_ZLG116_GPIO_FUNC_CODE(0x4) | AM_ZLG116_GPIO_MODE_CODE(0x6)
#endif

#if __TIM16_CH1_GPIO == PIOB_8
    #define __GPIO_TIM16_CH1   AM_ZLG116_GPIO_FUNC_CODE(0x2) | AM_ZLG116_GPIO_MODE_CODE(0x6)
#else
    #define __GPIO_TIM16_CH1   AM_ZLG116_GPIO_FUNC_CODE(0x6) | AM_ZLG116_GPIO_MODE_CODE(0x6)
#endif

#define __GPIO_TIM16_CH1N  AM_ZLG116_GPIO_FUNC_CODE(0x2) | AM_ZLG116_GPIO_MODE_CODE(0x6)

#define __GPIO_TIM16_BKIN  AM_ZLG116_GPIO_FUNC_CODE(0x2) | AM_ZLG116_GPIO_MODE_CODE(0x6)

#if __TIM17_CH1_GPIO == PIOA_7
    #define __GPIO_TIM17_CH1   AM_ZLG116_GPIO_FUNC_CODE(0x5)
#else
    #define __GPIO_TIM17_CH1   AM_ZLG116_GPIO_FUNC_CODE(0x2)
#endif

#if __TIM17_BKIN_GPIO == PIOA_10
    #define __GPIO_TIM17_BKIN  AM_ZLG116_GPIO_FUNC_CODE(0x0)
#else
    #define __GPIO_TIM17_BKIN  AM_ZLG116_GPIO_FUNC_CODE(0x5)
#endif

#define __GPIO_TIM17_CH1N  AM_ZLG116_GPIO_FUNC_CODE(0x2)



#endif /* __ZLG116_PIN_SET_H */

/* end of file */
