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
 * \brief 热电偶相关头文件定义
 *
 * \internal
 * \par Modification history
 * - 1.00 19-8-28  htf, first implementation.
 * \endinternal
 */

#ifndef __AM_THERMOCOUPLE_H
#define __AM_THERMOCOUPLE_H

#include "am_spi.h"
#include "am_types.h"
#include "am_softimer.h"
#include <stdint.h>


/**< \brief 热电偶电压值转温度函数定义 */
typedef am_err_t (*pfn_vol_to_tem_t) (double, double *);
/**< \brief 热电偶温度转电压函数 */
typedef am_err_t (*pfn_tem_to_vol_t) (double, double *);

/**< \brief 热点偶温度电压转换funcs结构体 */
typedef struct  am_therconversion {
    pfn_vol_to_tem_t   pfn_v2t;
    pfn_tem_to_vol_t   pfn_t2v;
}am_ther_conversion_t;


typedef  am_ther_conversion_t*  am_ther_formula_t;

/*
 * \brief  J型热电偶计算电压转温度
 * \param[in]  p_dev  : 热电偶设备结构体
 */
am_ther_formula_t am_thermocouple_j_init(am_ther_conversion_t *p_dev);

/*
 * \brief  K型热电偶计算电压转温度
 * \param[in]  p_dev  : 热电偶设备结构体
 */
am_ther_formula_t am_thermocouple_k_init(am_ther_conversion_t *p_dev);

/*
 * \brief  T型热电偶计算电压转温度
 * \param[in]  p_dev  : 热电偶设备结构体
 */
am_ther_formula_t am_thermocouple_t_init(am_ther_conversion_t *p_dev);

/*
 * \brief  N型热电偶计算电压转温度
 * \param[in]  p_dev  : 热电偶设备结构体
 */
am_ther_formula_t am_thermocouple_n_init(am_ther_conversion_t *p_dev);

/*
 * \brief  R型热电偶计算电压转温度
 * \param[in]  p_dev  : 热电偶设备结构体
 */
am_ther_formula_t am_thermocouple_r_init(am_ther_conversion_t *p_dev);

/*
 * \brief  E型热电偶计算电压转温度
 * \param[in]  p_dev  : 热电偶设备结构体
 */
am_ther_formula_t am_thermocouple_e_init(am_ther_conversion_t *p_dev);

/*
 * \brief  热电偶温度转电压
 * \param[in]  handle       : 热电偶公式句柄
 * \param[in]  temperature  : 待转换的温度
 * \param[in]  p_voltage    : 保存转换后电压的指针
 */
am_err_t am_ther_tem_to_vol(am_ther_formula_t handle,
                            double            temperature,
                            double           *p_voltage);

/*
 * \brief  热电偶温度转电压
 * \param[in]  handle        : 热电偶公式句柄
 * \param[in]  voltage       : 待转换的电压
 * \param[in]  p_temperature : 保存转换后温度的指针
 */
am_err_t am_ther_vol_to_tem(am_ther_formula_t handle,
                            double            voltage,
                            double           *p_temperature);

#endif
