/*******************************************************************************
* Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* @file       cd_include.h
* @author     sky
* @date       2015/12/23
* @version    V1.00
* @brief      (cd,coupling device, 耦合设备)包含头文件，该文件和芯片相关
*                    - V1.00    2015/12/23 sky 创建文档(采用FM17550S的代码)
*******************************************************************************/

/**
 * \file
 * \brief coupling device 耦合设备头文件定义
 *
 * \internal
 * \par Modification history
 * - 1.00 15-12-23  sky, first implementation.
 * \endinternal
 */

#ifndef __AM_CD_INCLUDE_H
#define __AM_CD_INCLUDE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup if_cd_include
 * \copydoc cd_include.h
 * @{
 */
    
/**
 * \brief 计算数组元素个数的宏
 * \param[in] array : 数组
 * \return 数组元素个数
 */
#ifndef count_of
#define count_of(array) (sizeof(array) / sizeof(array[0]))
#endif

/**
 * \brief 耦合设备选择
 */
#include "am_fm175xx.h"
#include "am_fm175xx_reg.h"

/**
 * @} if_cd_include
 */

#ifdef __cplusplus
}
#endif

#endif /* __CD_INCLUDE_H */

/* end of file */
