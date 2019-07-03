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
 * \brief LPC84x 引脚及配置参数定义
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
 * - 1.00 19-07-02  YRZ, modified.
 * \endinternal
 */
#ifndef __LPC84X_PIN_OUTSET_H
#define __LPC84X_PIN_OUTSET_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lpc84x_pin.h"

/** 
 * \name 按键引脚配置
 * @{
 */  
#define __KEY_KP0_PIN          PIO1_11      /**< \brief 所有引脚可选 */
/** @} */   
  
/** 
 * \name LED引脚配置
 * @{
 */  
#define __LED0_PIN             PIO1_8       /**< \brief 所有引脚可选 */
#define __LED1_PIN             PIO1_10      /**< \brief 所有引脚可选 */  
/** @} */ 
 
/** 
 * \name ADC引脚配置
 * @{
 */
#define __ADC_IN0_PIN          PIO0_7       /**< \brief 固定引脚不可更改 */
#define __ADC_IN1_PIN          PIO0_6       /**< \brief 固定引脚不可更改 */
#define __ADC_IN2_PIN          PIO0_14      /**< \brief 固定引脚不可更改 */
#define __ADC_IN3_PIN          PIO0_23      /**< \brief 固定引脚不可更改 */
#define __ADC_IN4_PIN          PIO0_22      /**< \brief 固定引脚不可更改 */
#define __ADC_IN5_PIN          PIO0_21      /**< \brief 固定引脚不可更改 */
#define __ADC_IN6_PIN          PIO0_20      /**< \brief 固定引脚不可更改 */
#define __ADC_IN7_PIN          PIO0_19      /**< \brief 固定引脚不可更改 */
#define __ADC_IN8_PIN          PIO0_18      /**< \brief 固定引脚不可更改 */
#define __ADC_IN9_PIN          PIO0_17      /**< \brief 固定引脚不可更改 */
#define __ADC_IN10_PIN         PIO0_13      /**< \brief 固定引脚不可更改 */
#define __ADC_IN11_PIN         PIO0_4       /**< \brief 固定引脚不可更改 */
/** @} */
  
/** 
 * \name DAC引脚配置
 * @{
 */
#define __DAC_OUT_PIN          PIO0_17      /**< \brief 固定引脚不可更改 */
/** @} */

/** 
 * \name I2C引脚配置
 * @{
 */
#define __I2C0_SCL_PIN         PIO0_10      /**< \brief 固定引脚不可更改 */
#define __I2C0_SDA_PIN         PIO0_11      /**< \brief 固定引脚不可更改 */
    
#define __I2C1_SCL_PIN         PIO0_18      /**< \brief 所有引脚可选 */
#define __I2C1_SDA_PIN         PIO0_19      /**< \brief 所有引脚可选 */
    
#define __I2C2_SCL_PIN         PIO0_9       /**< \brief 所有引脚可选 */
#define __I2C2_SDA_PIN         PIO0_8       /**< \brief 所有引脚可选 */
    
#define __I2C3_SCL_PIN         PIO0_6       /**< \brief 所有引脚可选 */
#define __I2C3_SDA_PIN         PIO0_7       /**< \brief 所有引脚可选 */
/** @} */

/** 
 * \name SCT CAP引脚配置
 * @{
 */
#define __SCT0_CAP_PIN0        PIO0_25      /**< \brief 所有引脚可选 */
#define __SCT0_CAP_PIN1        PIO0_26      /**< \brief 所有引脚可选 */
#define __SCT0_CAP_PIN2        PIO0_27      /**< \brief 所有引脚可选 */
#define __SCT0_CAP_PIN3        PIO0_28      /**< \brief 所有引脚可选 */
/** @} */

/** 
 * \name SCT PWM引脚配置
 * @{
 */
#define __SCT0_OUT0_PIN        PIO0_23      /**< \brief 所有引脚可选 */
#define __SCT0_OUT1_PIN        PIO0_24      /**< \brief 所有引脚可选 */
#define __SCT0_OUT2_PIN        PIO0_25      /**< \brief 所有引脚可选 */
#define __SCT0_OUT3_PIN        PIO0_26      /**< \brief 所有引脚可选 */
#define __SCT0_OUT4_PIN        PIO0_27      /**< \brief 所有引脚可选 */
#define __SCT0_OUT5_PIN        PIO0_15      /**< \brief 所有引脚可选 */
/** @} */

/** 
 * \name SPI0引脚配置
 * @{
 */
#define __SPI0_SCK_PIN         PIO0_17      /**< \brief 所有引脚可选 */
#define __SPI0_MOSI_PIN        PIO0_18      /**< \brief 所有引脚可选 */
#define __SPI0_MISO_PIN        PIO0_19      /**< \brief 所有引脚可选 */
/** @} */                                                            
                                                                     
/**                                                                  
 * \name SPI1引脚配置                                                
 * @{                                                                 
 */                                                                  
#define __SPI1_SCK_PIN         PIO0_11      /**< \brief 所有引脚可选 */
#define __SPI1_MOSI_PIN        PIO0_10      /**< \brief 所有引脚可选 */
#define __SPI1_MISO_PIN        PIO0_9       /**< \brief 所有引脚可选 */
/** @} */

/** 
 * \name USART0引脚配置
 * @{
 */
#define __U0_TXD_PIN           PIO1_0       /**< \brief 所有引脚可选 */
#define __U0_RXD_PIN           PIO1_2       /**< \brief 所有引脚可选 */
/** @} */                      
                               
/**                            
 * \name USART1引脚配置        
 * @{                          
 */                            
#define __U1_TXD_PIN           PIO0_23       /**< \brief 所有引脚可选 */
#define __U1_RXD_PIN           PIO0_14       /**< \brief 所有引脚可选 */
/** @} */                      
                               
/**                            
 * \name USART2引脚配置        
 * @{                          
 */                            
#define __U2_TXD_PIN           PIO0_27      /**< \brief 所有引脚可选 */
#define __U2_RXD_PIN           PIO0_26      /**< \brief 所有引脚可选 */
/** @} */       

/**                            
 * \name USART3引脚配置        
 * @{                          
 */                            
#define __U3_TXD_PIN           PIO0_27      /**< \brief 所有引脚可选 */
#define __U3_RXD_PIN           PIO0_26      /**< \brief 所有引脚可选 */
/** @} */     

/**                            
 * \name USART4引脚配置        
 * @{                          
 */                            
#define __U4_TXD_PIN           PIO0_27      /**< \brief 所有引脚可选 */
#define __U4_RXD_PIN           PIO0_26      /**< \brief 所有引脚可选 */
/** @} */  

#endif /* __LPC82X_PIN_OUTSET_H */

/* end of file */
