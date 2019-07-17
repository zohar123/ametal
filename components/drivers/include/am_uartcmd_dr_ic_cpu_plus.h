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
 * \brief CPU PLUS卡标准服务驱动层
 *
 * \internal
 * \par Modification History
 * - 1.00 16-01-06  hetaigang, first implementation.
 * \endinternal
 */
#ifndef __AM_DR_IC_CPU_PLUS_H
#define __AM_DR_IC_CPU_PLUS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_common.h"
#include "am_uartcmd_cpu_plus.h"
#include "am_uartcmd_ic_iso14443.h"

/**
 * \brief CPU PLUS卡设备结构体
 */
typedef struct amdr_ic_cpu_plus_dev {

    /** \brief CPU PLUS卡服务结构体 */
    am_ic_cpu_plus_serv_t   serv;

    /** \brief ISO14443服务句柄 */
    am_ic_iso14443_handle_t iso14443_handle;

    uint8_t  pcd_vector[16];   /** \brief PCD加密初始化向量 */
    uint8_t  picc_vector[16];  /** \brief PICC加密初始化向量 */
    uint8_t  random[16];       /** \brief 随机数 */

    /** \brief 交易随机数，由首次验证命令生成，在整个交易中使用 */
    uint8_t  ti[4];

    uint8_t  picc_cap2[6];     /** \brief PICC能力参数 */
    uint8_t  subkey1[16];      /** \brief CMAC计算子密钥1 */
    uint8_t  subkey2[16];      /** \brief CMAC计算子密钥2 */

    /** \brief  加密会话密钥 */
    uint8_t  session_key_enc[16];

    /** \brief MAC 会话密钥 */
    uint8_t  session_key_mac[16];

    am_bool_t   enckey_expand;    /** \brief 加密密钥扩展标志 */
    am_bool_t   mackey_expand;    /** \brief MAC密钥扩展标志 */
    am_bool_t   subkey_expand;    /** \brief CMAC计算子密钥扩展标志 */

    uint16_t read_count;       /** \brief 读计数器 */
    uint16_t write_count;      /** \brief 写计数器 */

} amdr_ic_cpu_plus_dev_t;


/**
 * \brief CPU PLUS卡驱动初始函数
 *
 * \param[in] p_dev : CPU PLUS卡设备结构体指针
 *
 * \return CPU PLUS标准服务句柄
 */
am_ic_cpu_plus_handle_t amdr_ic_cpu_plus_init (amdr_ic_cpu_plus_dev_t  *p_dev,
                                               am_ic_iso14443_handle_t  iso14443_handle);

/**
 * \brief CPU PLUS卡解除初始函数
 *
 * \param[in] p_dev : CPU PLUS卡设备结构体指针
 *
 * \return 无
 */
void amdr_ic_cpu_plus_deinit (amdr_ic_cpu_plus_dev_t *p_dev);

#ifdef __cplusplus
}
#endif

#endif /* __AMDR_IC_CPU_PLUS_H */

/* end of file */
