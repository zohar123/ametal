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
 * \brief KL26 引脚及配置
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
 * - 1.00 19-06-19  zjr, modified.

 * \endinternal
 */

#ifndef __KL26_PIN_SET_H
#define __KL26_PIN_SET_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../../../../soc/freescale/kl26/kl26_pin.h"

/** 
 * \name I2C0引脚配置
 * @{
 */
#define __I2C0_SCL PIOE_24    //PIOB_0 PIOB_2 PIOC_8 PIOE_19 PIOE_24
#define __I2C0_SDA PIOE_25    //PIOB_1 PIOB_3 PIOC_9 PIOE_18 PIOE_25
/** @} */

/** 
 * \name I2C1引脚配置
 * @{
 */
#define __I2C1_SCL PIOA_3    //PIOA_3  PIOC_1  PIOC_10  PIOE_1
#define __I2C1_SDA PIOA_4    //PIOA_4  PIOC_2  PIOC_11  PIOE_0
/** @} */

/** 
 * \name SPI0引脚配置
 * @{
 */
#define __SPI0_SCK  PIOC_5     //PIOC_5  PIOD_1  PIOE_17
#define __SPI0_MOSI PIOC_6     //PIOC_6  PIOC_7  PIOD_2   PIOD_3   PIOE_18  PIOE_19
#define __SPI0_MISO PIOC_7     //PIOC_7  PIOC_6  PIOD_2   PIOD_3   PIOE_18  PIOE_19
/** @} */

/** 
 * \name SPI1引脚配置
 * @{
 */
#define __SPI1_SCK  PIOD_5
#define __SPI1_MOSI PIOD_6     //PIOD_7  PIOE_1   PIOB_16  PIOB_17
#define __SPI1_MISO PIOD_7     //PIOD_6  PIOE_0   PIOE_1   PIOB_16  PIOB_17
/** @} */

/** 
 * \name UART0引脚配置
 * @{
 */
#define __UART0_RX PIOA_1    //PIOA_1  PIOB_16 PIOD_6 PIOE_21
#define __UART0_TX PIOA_2    //PIOA_2  PIOB_17 PIOD_7 PIOE_20
/** @} */

/** 
 * \name UART1引脚配置
 * @{
 */
#define __UART1_RX PIOA_18    //PIOC_3 PIOE_1 PIOA_18 
#define __UART1_TX PIOA_19    //PIOC_4 PIOE_0 PIOA_19 
/** @} */

/** 
 * \name UART2引脚配置
 * @{
 */
#define __UART2_RX PIOD_2    //PIOD_2  PIOD_4  PIOE_17  PIOE_23
#define __UART2_TX PIOD_3    //PIOD_3  PIOD_5  PIOE_16  PIOE_22
/** @} */

/** 
 * \name DAC引脚配置
 * @{
 */
#define __DAC0_OUT  PIOE_30 
#define __GPIO_DAC0_OUT AM_KL26_GPIO_FUNC_CODE(0x0)
/** @} */

/** 
 * \name TPM0引脚配置
 * @{
 */
#define __TPM0_CH0  PIOD_0  //PIOD_0  PIOA_3  PIOC_1  PIOE_24
#define __TPM0_CH1  PIOD_1  //PIOD_1  PIOA_4  PIOC_2  PIOE_25
#define __TPM0_CH2  PIOD_2  //PIOD_2  PIOA_5  PIOC_3  PIOE_29
#define __TPM0_CH3  PIOD_3  //PIOD_3  PIOC_4  PIOE_30 
#define __TPM0_CH4  PIOD_4  //PIOD_4  PIOC_8  PIOE_31
#define __TPM0_CH5  PIOD_5  //PIOD_5  PIOA_0  PIOC_9  

#define __BUZZER_CHAN 2
/** @} */

/** 
 * \name TPM1引脚配置
 * @{
 */
#define __TPM1_CH0  PIOE_20  //PIOA_12  PIOB_0  PIOE_20
#define __TPM1_CH1  PIOE_21  //PIOA_13  PIOB_1  PIOE_21
/** @} */

/** 
 * \name TPM2引脚配置
 * @{
 */
#define __TPM2_CH0  PIOE_22  //PIOA_1  PIOB_2  PIOB_18  PIOE_22
#define __TPM2_CH1  PIOE_23  //PIOA_2  PIOB_3  PIOB_19  PIOE_23
/** @} */

/** 
 * \name TSI0引脚配置
 * @{
 */
#define __TSI0_CH0  PIOB_0 
#define __TSI0_CH1  PIOA_0
#define __TSI0_CH2  PIOA_1
#define __TSI0_CH3  PIOA_2
#define __TSI0_CH4  PIOA_3
#define __TSI0_CH5  PIOA_4
#define __TSI0_CH6  PIOB_1
#define __TSI0_CH7  PIOB_2
#define __TSI0_CH8  PIOB_3
#define __TSI0_CH9  PIOB_16
#define __TSI0_CH10 PIOB_17
#define __TSI0_CH11 PIOB_18
#define __TSI0_CH12 PIOB_19
#define __TSI0_CH13 PIOC_0
#define __TSI0_CH14 PIOC_1
#define __TSI0_CH15 PIOC_2
/** @} */

/** 
 * \name LED引脚配置
 * @{
 */
#define __LED0_GPIO PIOC_4
#define __LED1_GPIO PIOC_8
/** @} */

/** 
 * \name KEY引脚配置
 * @{
 */
#define __KEY_KP0_GPIO PIOC_0
/** @} */

/** 
 * \name ADC引脚配置
 * @{
 */
#define __ADC_SE0_GPIO  PIOE_20
#define __ADC_SE1_GPIO  PIOE_16
#define __ADC_SE2_GPIO  PIOE_18
#define __ADC_SE3_GPIO  PIOE_22
#define __ADC_SE4A_GPIO PIOE_21
#define __ADC_SE4B_GPIO PIOE_29
#define __ADC_SE5A_GPIO PIOE_17
#define __ADC_SE5B_GPIO PIOD_1
#define __ADC_SE6A_GPIO PIOE_19
#define __ADC_SE6B_GPIO PIOD_5
#define __ADC_SE7A_GPIO PIOE_23
#define __ADC_SE7B_GPIO PIOD_6
#define __ADC_SE8_GPIO  PIOB_0
#define __ADC_SE9_GPIO  PIOB_1
#define __ADC_SE11_GPIO PIOC_2
#define __ADC_SE12_GPIO PIOB_2
#define __ADC_SE13_GPIO PIOB_3
#define __ADC_SE14_GPIO PIOC_0
#define __ADC_SE15_GPIO PIOC_1
#define __ADC_SE23_GPIO PIOE_30

#define __ADC_DP0_GPIO PIOE_20
#define __ADC_DP1_GPIO PIOE_16
#define __ADC_DP2_GPIO PIOE_18
#define __ADC_DP3_GPIO PIOE_22

#define __ADC_DM0_GPIO PIOE_21
#define __ADC_DM1_GPIO PIOE_17
#define __ADC_DM2_GPIO PIOE_19
#define __ADC_DM3_GPIO PIOE_23
/** @} */


#define __PIO_GPIO AM_KL26_GPIO_FUNC_CODE(0x1)



#if __I2C0_SCL == PIOE_24
    #define __GPIO_I2C0_SCL AM_KL26_GPIO_FUNC_CODE(0x5)
#elif __I2C0_SCL == PIOE_19
    #define __GPIO_I2C0_SCL AM_KL26_GPIO_FUNC_CODE(0x4)
#else
    #define __GPIO_I2C0_SCL AM_KL26_GPIO_FUNC_CODE(0x2)
#endif

#if __I2C0_SDA == PIOE_25
    #define __GPIO_I2C0_SDA AM_KL26_GPIO_FUNC_CODE(0x5)
#elif __I2C0_SDA == PIOE_18
    #define __GPIO_I2C0_SDA AM_KL26_GPIO_FUNC_CODE(0x4)
#else
    #define __GPIO_I2C0_SDA AM_KL26_GPIO_FUNC_CODE(0x2)
#endif


#if __I2C1_SCL == PIOE_1
    #define __GPIO_I2C1_SCL AM_KL26_GPIO_FUNC_CODE(0x6)
#else
    #define __GPIO_I2C1_SCL AM_KL26_GPIO_FUNC_CODE(0x2)
#endif

#if __I2C1_SDA == PIOE_0
    #define __GPIO_I2C1_SDA AM_KL26_GPIO_FUNC_CODE(0x6)
#else
    #define __GPIO_I2C1_SDA AM_KL26_GPIO_FUNC_CODE(0x2)
#endif

 
#define __GPIO_SPI0_SCK AM_KL26_GPIO_FUNC_CODE(0x2) | AM_GPIO_PULLUP

#if __SPI0_MOSI == PIOC_6
    #define __GPIO_SPI0_MOSI AM_KL26_GPIO_FUNC_CODE(0x2) | AM_GPIO_PULLUP
#elif __SPI0_MOSI == PIOC_7
    #define __GPIO_SPI0_MOSI AM_KL26_GPIO_FUNC_CODE(0x5) | AM_GPIO_PULLUP
#elif __SPI0_MOSI == PIOD_2
    #define __GPIO_SPI0_MOSI AM_KL26_GPIO_FUNC_CODE(0x2) | AM_GPIO_PULLUP
#elif __SPI0_MOSI == PIOD_3
    #define __GPIO_SPI0_MOSI AM_KL26_GPIO_FUNC_CODE(0x5) | AM_GPIO_PULLUP
#elif __SPI0_MOSI == PIOE_18
    #define __GPIO_SPI0_MOSI AM_KL26_GPIO_FUNC_CODE(0x2) | AM_GPIO_PULLUP
#elif __SPI0_MOSI == PIOE_19
    #define __GPIO_SPI0_MOSI AM_KL26_GPIO_FUNC_CODE(0x5) | AM_GPIO_PULLUP
#endif

#if __SPI0_MISO == PIOC_7
    #define __GPIO_SPI0_MISO AM_KL26_GPIO_FUNC_CODE(0x2) | AM_GPIO_PULLUP
#elif __SPI0_MISO == PIOC_6
    #define __GPIO_SPI0_MISO AM_KL26_GPIO_FUNC_CODE(0x5) | AM_GPIO_PULLUP
#elif __SPI0_MISO == PIOD_3
    #define __GPIO_SPI0_MISO AM_KL26_GPIO_FUNC_CODE(0x2) | AM_GPIO_PULLUP
#elif __SPI0_MISO == PIOD_2
    #define __GPIO_SPI0_MISO AM_KL26_GPIO_FUNC_CODE(0x5) | AM_GPIO_PULLUP
#elif __SPI0_MISO == PIOE_19
    #define __GPIO_SPI0_MISO AM_KL26_GPIO_FUNC_CODE(0x2) | AM_GPIO_PULLUP
#elif __SPI0_MISO == PIOE_18
    #define __GPIO_SPI0_MISO AM_KL26_GPIO_FUNC_CODE(0x5) | AM_GPIO_PULLUP
#endif


#define __GPIO_SPI1_SCK AM_KL26_GPIO_FUNC_CODE(0x2) | AM_GPIO_PULLUP

#if __SPI1_MOSI == PIOD_6
    #define __GPIO_SPI1_MOSI AM_KL26_GPIO_FUNC_CODE(0x2) | AM_GPIO_PULLUP
#elif __SPI1_MOSI == PIOD_7
    #define __GPIO_SPI1_MOSI AM_KL26_GPIO_FUNC_CODE(0x5) | AM_GPIO_PULLUP
#elif __SPI1_MOSI == PIOE_1
    #define __GPIO_SPI1_MOSI AM_KL26_GPIO_FUNC_CODE(0x2) | AM_GPIO_PULLUP
#elif __SPI1_MOSI == PIOB_16
    #define __GPIO_SPI1_MOSI AM_KL26_GPIO_FUNC_CODE(0x2) | AM_GPIO_PULLUP
#elif __SPI1_MOSI == PIOB_17
    #define __GPIO_SPI1_MOSI AM_KL26_GPIO_FUNC_CODE(0x5) | AM_GPIO_PULLUP
#endif

#if __SPI1_MISO == PIOD_7
    #define __GPIO_SPI1_MISO AM_KL26_GPIO_FUNC_CODE(0x2) | AM_GPIO_PULLUP
#elif __SPI1_MISO == PIOD_6
    #define __GPIO_SPI1_MISO AM_KL26_GPIO_FUNC_CODE(0x5) | AM_GPIO_PULLUP
#elif __SPI1_MISO == PIOE_0
    #define __GPIO_SPI1_MISO AM_KL26_GPIO_FUNC_CODE(0x2) | AM_GPIO_PULLUP
#elif __SPI1_MISO == PIOE_1
    #define __GPIO_SPI1_MISO AM_KL26_GPIO_FUNC_CODE(0x5) | AM_GPIO_PULLUP
#elif __SPI1_MISO == PIOB_17
    #define __GPIO_SPI1_MISO AM_KL26_GPIO_FUNC_CODE(0x2) | AM_GPIO_PULLUP
#elif __SPI1_MISO == PIOB_16
    #define __GPIO_SPI1_MISO AM_KL26_GPIO_FUNC_CODE(0x5) | AM_GPIO_PULLUP
#endif


#if __UART0_RX == PIOA_1
    #define __GPIO_UART0_RX AM_KL26_GPIO_FUNC_CODE(0x2)
#elif __UART0_RX == PIOB_16
    #define __GPIO_UART0_RX AM_KL26_GPIO_FUNC_CODE(0x3)
#elif __UART0_RX == PIOD_6
    #define __GPIO_UART0_RX AM_KL26_GPIO_FUNC_CODE(0x3)
#elif __UART0_RX == PIOE_21
    #define __GPIO_UART0_RX AM_KL26_GPIO_FUNC_CODE(0x4)
#endif

#if __UART0_TX == PIOA_2
    #define __GPIO_UART0_TX AM_KL26_GPIO_FUNC_CODE(0x2)
#elif __UART0_TX == PIOB_17
    #define __GPIO_UART0_TX AM_KL26_GPIO_FUNC_CODE(0x3)
#elif __UART0_TX == PIOD_7
    #define __GPIO_UART0_TX AM_KL26_GPIO_FUNC_CODE(0x3)
#elif __UART0_TX == PIOE_20
    #define __GPIO_UART0_TX AM_KL26_GPIO_FUNC_CODE(0x4)
#endif


#define __GPIO_UART1_RX AM_KL26_GPIO_FUNC_CODE(0x3)
#define __GPIO_UART1_TX AM_KL26_GPIO_FUNC_CODE(0x3)


#if __UART2_RX == PIOE_23
    #define __GPIO_UART2_RX AM_KL26_GPIO_FUNC_CODE(0x4)
#else 
    #define __GPIO_UART2_RX AM_KL26_GPIO_FUNC_CODE(0x3)
#endif
#if __UART2_TX == PIOE_22
    #define __GPIO_UART2_TX AM_KL26_GPIO_FUNC_CODE(0x4)
#else
    #define __GPIO_UART2_TX AM_KL26_GPIO_FUNC_CODE(0x3)
#endif


#if __TPM0_CH0 == PIOD_0
    #define __GPIO_TPM0_CH0 AM_KL26_GPIO_FUNC_CODE(0x4)
#elif __TPM0_CH0 == PIOA_3
    #define __GPIO_TPM0_CH0 AM_KL26_GPIO_FUNC_CODE(0x3)
#elif __TPM0_CH0 == PIOC_1
    #define __GPIO_TPM0_CH0 AM_KL26_GPIO_FUNC_CODE(0x4)
#elif __TPM0_CH0 == PIOE_24
    #define __GPIO_TPM0_CH0 AM_KL26_GPIO_FUNC_CODE(0x3)
#endif
#if __TPM0_CH1 == PIOD_1
    #define __GPIO_TPM0_CH1 AM_KL26_GPIO_FUNC_CODE(0x4)
#elif __TPM0_CH1 == PIOA_4
    #define __GPIO_TPM0_CH1 AM_KL26_GPIO_FUNC_CODE(0x3)
#elif __TPM0_CH1 == PIOC_2
    #define __GPIO_TPM0_CH1 AM_KL26_GPIO_FUNC_CODE(0x4)
#elif __TPM0_CH1 == PIOE_25
    #define __GPIO_TPM0_CH1 AM_KL26_GPIO_FUNC_CODE(0x3)
#endif
#if __TPM0_CH2 == PIOD_2
    #define __GPIO_TPM0_CH2 AM_KL26_GPIO_FUNC_CODE(0x4)
#elif __TPM0_CH2 == PIOA_5
    #define __GPIO_TPM0_CH2 AM_KL26_GPIO_FUNC_CODE(0x3)
#elif __TPM0_CH2 == PIOC_3
    #define __GPIO_TPM0_CH2 AM_KL26_GPIO_FUNC_CODE(0x4)
#elif __TPM0_CH2 == PIOE_29
    #define __GPIO_TPM0_CH2 AM_KL26_GPIO_FUNC_CODE(0x3)
#endif
#if __TPM0_CH3 == PIOD_3
    #define __GPIO_TPM0_CH3 AM_KL26_GPIO_FUNC_CODE(0x4)
#elif __TPM0_CH3 == PIOC_4
    #define __GPIO_TPM0_CH3 AM_KL26_GPIO_FUNC_CODE(0x4)
#elif __TPM0_CH3 == PIOE_30
    #define __GPIO_TPM0_CH3 AM_KL26_GPIO_FUNC_CODE(0x3)
#endif
#if __TPM0_CH4 == PIOD_4
    #define __GPIO_TPM0_CH4 AM_KL26_GPIO_FUNC_CODE(0x4)
#elif __TPM0_CH4 == PIOC_8
    #define __GPIO_TPM0_CH4 AM_KL26_GPIO_FUNC_CODE(0x3)
#elif __TPM0_CH4 == PIOE_31
    #define __GPIO_TPM0_CH4 AM_KL26_GPIO_FUNC_CODE(0x3)
#endif
#if __TPM0_CH5 == PIOD_5
    #define __GPIO_TPM0_CH5 AM_KL26_GPIO_FUNC_CODE(0x4)
#elif __TPM0_CH5 == PIOC_9
    #define __GPIO_TPM0_CH5 AM_KL26_GPIO_FUNC_CODE(0x3)
#elif __TPM0_CH5 == PIOA_0
    #define __GPIO_TPM0_CH5 AM_KL26_GPIO_FUNC_CODE(0x3)
#endif


#define __GPIO_TPM1_CH0 AM_KL26_GPIO_FUNC_CODE(0x3)
#define __GPIO_TPM1_CH1 AM_KL26_GPIO_FUNC_CODE(0x3)


#define __GPIO_TPM2_CH0 AM_KL26_GPIO_FUNC_CODE(0x3)
#define __GPIO_TPM2_CH1 AM_KL26_GPIO_FUNC_CODE(0x3)


#define __GPIO_TSI0 AM_KL26_GPIO_FUNC_CODE(0x0)


#define __GPIO_ADC0 AM_KL26_GPIO_FUNC_CODE(0x0)

#ifdef __cplusplus
}
#endif

#endif /* __KL26_PIN_SET_H */

/* end of file */
