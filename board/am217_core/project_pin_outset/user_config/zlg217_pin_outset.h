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
 * \brief ZLG217 引脚及配置
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

#ifndef __ZLG217_PIN_SET_H
#define __ZLG217_PIN_SET_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ZLG217_pin.h"

/** 
 * \name UART1引脚配置
 * @{
 */
#define __UART1_RX            PIOA_10     //PIOA_10  PIOB_7
#define __UART1_TX            PIOA_9      //PIOA_9   PIOB_6

#if __UART1_RX == PIOA_10
    #define __GPIO_UART1_RX   AM_ZLG217_GPIO_REMAP_CODE(0x02, 0) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#else
    #define __GPIO_UART1_RX   AM_ZLG217_GPIO_REMAP_CODE(0x02, 1) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#endif
#if __UART1_TX == PIOA_9
    #define __GPIO_UART1_TX   AM_ZLG217_GPIO_REMAP_CODE(0x02, 0) | AM_ZLG217_GPIO_MODE_CODE(0x6)
#else
    #define __GPIO_UART1_TX   AM_ZLG217_GPIO_REMAP_CODE(0x02, 1) | AM_ZLG217_GPIO_MODE_CODE(0x6)
#endif
/** @} */

/** 
 * \name UART2引脚配置
 * @{
 */
#define __UART2_RX            PIOA_3
#define __UART2_TX            PIOA_2

#define __GPIO_UART2_RX       AM_ZLG217_GPIO_REMAP_CODE(0x11, 0) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __GPIO_UART2_TX       AM_ZLG217_GPIO_REMAP_CODE(0x11, 0) | AM_ZLG217_GPIO_MODE_CODE(0x6) | AM_ZLG217_GPIO_OUTRES_RATE_CODE(0x02)
/** @} */

/** 
 * \name UART3引脚配置
 * @{
 */
#define __UART3_RX             PIOB_11       //PIOB_11  PIOC_11
#define __UART3_TX             PIOB_10       //PIOB_10  PIOC_10 


#if __UART3_RX == PIOB_11
    #define __GPIO_UART3_RX       AM_ZLG217_GPIO_REMAP_CODE(0x04, 0) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#else
    #define __GPIO_UART3_RX       AM_ZLG217_GPIO_REMAP_CODE(0x04, 1) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#endif
#if __UART3_TX == PIOB_10
    #define __GPIO_UART3_TX       AM_ZLG217_GPIO_REMAP_CODE(0x04, 0) | AM_ZLG217_GPIO_MODE_CODE(0x6) | AM_ZLG217_GPIO_OUTRES_RATE_CODE(0x02)
#else
    #define __GPIO_UART3_TX       AM_ZLG217_GPIO_REMAP_CODE(0x04, 1) | AM_ZLG217_GPIO_MODE_CODE(0x6) | AM_ZLG217_GPIO_OUTRES_RATE_CODE(0x02)
#endif
/** @} */

/** 
 * \name SPI1引脚配置
 * @{
 */
#define __SPI1_NSS             PIOA_4     //PIOA_4    PIOA_15
#define __SPI1_SCK             PIOA_5     //PIOA_5    PIOB_3
#define __SPI1_MISO            PIOA_6     //PIOA_6    PIOB_4
#define __SPI1_MOSI            PIOA_7     //PIOA_7    PIOB_5

#define __GPIO_SPI1_NSS        AM_ZLG217_GPIO_REMAP_CODE(0x11, 0) | AM_ZLG217_GPIO_MODE_CODE(0x6)
#if __SPI1_SCK == PIOA_5
    #define __GPIO_SPI1_SCK        AM_ZLG217_GPIO_REMAP_CODE(0x00, 0) | AM_ZLG217_GPIO_MODE_CODE(0x6)
#else
    #define __GPIO_SPI1_SCK        AM_ZLG217_GPIO_REMAP_CODE(0x00, 1) | AM_ZLG217_GPIO_MODE_CODE(0x6)
#endif
#if __SPI1_MISO == PIOA_6
    #define __GPIO_SPI1_MISO       AM_ZLG217_GPIO_REMAP_CODE(0x00, 0) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#else
    #define __GPIO_SPI1_MISO       AM_ZLG217_GPIO_REMAP_CODE(0x00, 1) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#endif
#if __SPI1_MOSI == PIOA_7
    #define __GPIO_SPI1_MOSI       AM_ZLG217_GPIO_REMAP_CODE(0x00, 0) | AM_ZLG217_GPIO_MODE_CODE(0x6)
#else
    #define __GPIO_SPI1_MOSI       AM_ZLG217_GPIO_REMAP_CODE(0x00, 1) | AM_ZLG217_GPIO_MODE_CODE(0x6)
#endif





/** @} */

/** 
 * \name SPI2引脚配置
 * @{
 */
#define __SPI2_NSS             PIOB_12
#define __SPI2_SCK             PIOB_13
#define __SPI2_MISO            PIOB_14
#define __SPI2_MOSI            PIOB_15

#define __GPIO_SPI2_NSS        AM_ZLG217_GPIO_REMAP_CODE(0x11, 0) | AM_ZLG217_GPIO_MODE_CODE(0x6)
#define __GPIO_SPI2_SCK        AM_ZLG217_GPIO_REMAP_CODE(0x11, 0) | AM_ZLG217_GPIO_MODE_CODE(0x6)
#define __GPIO_SPI2_MISO       AM_ZLG217_GPIO_REMAP_CODE(0x11, 0) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __GPIO_SPI2_MOSI       AM_ZLG217_GPIO_REMAP_CODE(0x11, 0) | AM_ZLG217_GPIO_MODE_CODE(0x6)
/** @} */

/** 
 * \name I2C1引脚配置
 * @{
 */          
#define __I2C1_SCL             PIOB_6     //PIOB_6  PIOB_8  
#define __I2C1_SDA             PIOB_7     //PIOB_7  PIOB_9

#if __I2C1_SCL == PIOB_6
    #define __GPIO_I2C1_SCL   AM_ZLG217_GPIO_REMAP_CODE(0x01, 0) | AM_ZLG217_GPIO_MODE_CODE(0x7)
#else
    #define __GPIO_I2C1_SCL   AM_ZLG217_GPIO_REMAP_CODE(0x01, 1) | AM_ZLG217_GPIO_MODE_CODE(0x7)
#endif
#if __I2C1_SDA == PIOB_7
    #define __GPIO_I2C1_SDA   AM_ZLG217_GPIO_REMAP_CODE(0x01, 0) | AM_ZLG217_GPIO_MODE_CODE(0x7)
#else
    #define __GPIO_I2C1_SDA   AM_ZLG217_GPIO_REMAP_CODE(0x01, 1) | AM_ZLG217_GPIO_MODE_CODE(0x7)
#endif
/** @} */

/** 
 * \name I2C2引脚配置
 * @{
 */          
#define __I2C2_SCL             PIOB_10
#define __I2C2_SDA             PIOB_11

#define __GPIO_I2C2_SCL   AM_ZLG217_GPIO_REMAP_CODE(0x11, 0) | AM_ZLG217_GPIO_MODE_CODE(0x7) | AM_ZLG217_GPIO_OUTRES_RATE_CODE(0x02)
#define __GPIO_I2C2_SDA   AM_ZLG217_GPIO_REMAP_CODE(0x11, 0) | AM_ZLG217_GPIO_MODE_CODE(0x7) | AM_ZLG217_GPIO_OUTRES_RATE_CODE(0x02)
/** @} */

/** 
 * \name DAC引脚配置
 * @{
 */
#define __DAC1_OUT             PIOA_4

#define __GPIO_DAC1_OUT   AM_ZLG217_GPIO_REMAP_CODE(0x11, 0)
/** @} */

/** 
 * \name ADC引脚配置
 * @{
 */
#define __ADC1_IN0             PIOA_0
#define __ADC1_IN1             PIOA_1
#define __ADC1_IN2             PIOA_2
#define __ADC1_IN3             PIOA_3
#define __ADC1_IN4             PIOA_4
#define __ADC1_IN5             PIOA_5
#define __ADC1_IN6             PIOA_6
#define __ADC1_IN7             PIOA_7

#define __GPIO_ADC1_IN   AM_ZLG217_GPIO_REMAP_CODE(0x11, 0) | AM_ZLG217_GPIO_MODE_CODE(0x0)
/** @} */

/** 
 * \name LED引脚配置
 * @{
 */
#define __LED0_GPIO           PIOC_9
#define __LED1_GPIO           PIOA_8
/** @} */

/** 
 * \name KEY引脚配置
 * @{
 */
#define __KEY_KP0_GPIO        PIOC_7
/** @} */

/** 
 * \name 蜂鸣器通道配置
 * @{
 */
#define __BUZZER_CHAN 2
/** @} */

/** 
 * \name TIM引脚配置
 * @{
 */
#define __TIM1_BAKIN_REMAP0     PIOB_12,AM_ZLG217_GPIO_REMAP_CODE(0x06, 0) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __TIM1_BAKIN_REMAP1     PIOA_6, AM_ZLG217_GPIO_REMAP_CODE(0x06, 1) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __TIM1_CH1N_REMAP0      PIOB_13,AM_ZLG217_GPIO_REMAP_CODE(0x06, 0) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __TIM1_CH1N_REMAP1      PIOA_7, AM_ZLG217_GPIO_REMAP_CODE(0x06, 1) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __TIM1_CH2N_REMAP0      PIOB_14,AM_ZLG217_GPIO_REMAP_CODE(0x06, 0) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __TIM1_CH2N_REMAP1      PIOB_0, AM_ZLG217_GPIO_REMAP_CODE(0x06, 1) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __TIM1_CH3N_REMAP0      PIOB_15,AM_ZLG217_GPIO_REMAP_CODE(0x06, 0) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __TIM1_CH3N_REMAP1      PIOB_1, AM_ZLG217_GPIO_REMAP_CODE(0x06, 1) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __TIM1_CH1_REMAP0       PIOA_8, AM_ZLG217_GPIO_REMAP_CODE(0x06, 0) | AM_ZLG217_GPIO_MODE_CODE(0x1)     //PIOA_8,PIOA_12
#define __TIM1_CH1_REMAP1       PIOA_8, AM_ZLG217_GPIO_REMAP_CODE(0x06, 1) | AM_ZLG217_GPIO_MODE_CODE(0x1)     //PIOA_8,PIOA_12
#define __TIM1_CH2_REMAP0       PIOA_9, AM_ZLG217_GPIO_REMAP_CODE(0x06, 0) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __TIM1_CH2_REMAP1       PIOA_9, AM_ZLG217_GPIO_REMAP_CODE(0x06, 1) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __TIM1_CH3_REMAP0       PIOA_10,AM_ZLG217_GPIO_REMAP_CODE(0x06, 0) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __TIM1_CH3_REMAP1       PIOA_10,AM_ZLG217_GPIO_REMAP_CODE(0x06, 1) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __TIM1_CH4_REMAP0       PIOA_11,AM_ZLG217_GPIO_REMAP_CODE(0X06, 0) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __TIM1_CH4_REMAP1       PIOA_11,AM_ZLG217_GPIO_REMAP_CODE(0X06, 1) | AM_ZLG217_GPIO_MODE_CODE(0x1)


#define __TIM2_CH1_ETR_REMAP0   PIOA_0, AM_ZLG217_GPIO_REMAP_CODE(0x08, 0) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __TIM2_CH1_ETR_REMAP1   PIOA_15,AM_ZLG217_GPIO_REMAP_CODE(0x08, 1) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __TIM2_CH1_ETR_REMAP2   PIOA_0, AM_ZLG217_GPIO_REMAP_CODE(0x08, 2) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __TIM2_CH1_ETR_REMAP3   PIOA_15,AM_ZLG217_GPIO_REMAP_CODE(0x08, 3) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __TIM2_CH2_REMAP0       PIOA_1, AM_ZLG217_GPIO_REMAP_CODE(0x08, 0) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __TIM2_CH2_REMAP1       PIOB_3, AM_ZLG217_GPIO_REMAP_CODE(0x08, 1) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __TIM2_CH2_REMAP2       PIOA_1, AM_ZLG217_GPIO_REMAP_CODE(0x08, 2) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __TIM2_CH2_REMAP3       PIOB_3, AM_ZLG217_GPIO_REMAP_CODE(0x08, 3) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __TIM2_CH3_REMAP0       PIOA_2, AM_ZLG217_GPIO_REMAP_CODE(0x08, 0) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __TIM2_CH3_REMAP1       PIOA_2, AM_ZLG217_GPIO_REMAP_CODE(0x08, 1) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __TIM2_CH3_REMAP2       PIOB_10,AM_ZLG217_GPIO_REMAP_CODE(0x08, 2) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __TIM2_CH3_REMAP3       PIOB_10,AM_ZLG217_GPIO_REMAP_CODE(0x08, 3) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __TIM2_CH4_REMAP0       PIOA_3, AM_ZLG217_GPIO_REMAP_CODE(0x08, 0) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __TIM2_CH4_REMAP1       PIOA_3, AM_ZLG217_GPIO_REMAP_CODE(0x08, 1) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __TIM2_CH4_REMAP2       PIOB_11,AM_ZLG217_GPIO_REMAP_CODE(0x08, 2) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __TIM2_CH4_REMAP3       PIOB_11,AM_ZLG217_GPIO_REMAP_CODE(0x08, 3) | AM_ZLG217_GPIO_MODE_CODE(0x1)


#define __TIM3_CH1_REMAP0       PIOA_6, AM_ZLG217_GPIO_REMAP_CODE(0x0A, 0) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __TIM3_CH1_REMAP2       PIOB_4, AM_ZLG217_GPIO_REMAP_CODE(0x0A, 2) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __TIM3_CH1_REMAP3       PIOC_6, AM_ZLG217_GPIO_REMAP_CODE(0X0A, 3) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __TIM3_CH2_REMAP0       PIOA_7, AM_ZLG217_GPIO_REMAP_CODE(0x0A, 0) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __TIM3_CH2_REMAP2       PIOB_5, AM_ZLG217_GPIO_REMAP_CODE(0x0A, 2) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __TIM3_CH2_REMAP3       PIOC_7, AM_ZLG217_GPIO_REMAP_CODE(0x0A, 3) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __TIM3_CH3_REMAP0       PIOB_0, AM_ZLG217_GPIO_REMAP_CODE(0x0A, 0) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __TIM3_CH3_REMAP2       PIOB_0, AM_ZLG217_GPIO_REMAP_CODE(0x0A, 2) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __TIM3_CH3_REMAP3       PIOC_8, AM_ZLG217_GPIO_REMAP_CODE(0x0A, 3) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __TIM3_CH4_REMAP0       PIOB_1, AM_ZLG217_GPIO_REMAP_CODE(0x0A, 0) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __TIM3_CH4_REMAP1       PIOB_1, AM_ZLG217_GPIO_REMAP_CODE(0x0A, 1) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __TIM3_CH4_REMAP3       PIOC_9, AM_ZLG217_GPIO_REMAP_CODE(0x0A, 3) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __TIM3_ETR              PIOD_2, AM_ZLG217_GPIO_REMAP_CODE(0x11, 0) | AM_ZLG217_GPIO_MODE_CODE(0x1)


#define __TIM4_CH1              PIOB_6, AM_ZLG217_GPIO_REMAP_CODE(0x11, 0) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __TIM4_CH2              PIOB_7, AM_ZLG217_GPIO_REMAP_CODE(0x11, 0) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __TIM4_CH3              PIOB_8, AM_ZLG217_GPIO_REMAP_CODE(0x11, 0) | AM_ZLG217_GPIO_MODE_CODE(0x1)
#define __TIM4_CH4              PIOB_9, AM_ZLG217_GPIO_REMAP_CODE(0x11, 0) | AM_ZLG217_GPIO_MODE_CODE(0x1)
/** @} */



#define __PIO_GPIO AM_ZLG217_GPIO_REMAP_CODE(0x11, 0) | AM_ZLG217_GPIO_MODE_CODE(0x1)




#endif /* __ZLG217_PIN_SET_H */

/* end of file */
