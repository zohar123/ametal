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
 * \brief ISO14443服务标准接口
 *
 * \internal
 * \par Modification History
 * - 1.00 16-01-04  hetaigang, first implementation.
 * \endinternal
 */
#ifndef __AM_DR_IC_ISO14443_H
#define __AM_DR_IC_ISO14443_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_uartcmd_ic_iso14443.h"
#include "am_uartcmd_includes.h"

/**
 * \brief tpcl_prot_para T=CL协议参数
 */
typedef struct tpcl_prot_para {
    uint8_t pcb_num;                /**< \brief 分包标识 */
    uint8_t cid;                    /**< \brief CID */
    uint8_t fsdi;                   /**< \brief PCD  能接收的最大帧 */
    uint8_t fsci;                   /**< \brief PICC 能接收的最大帧 */
    uint8_t ta1;                    /**< \brief 含DR、DS */
    uint8_t bit_rate;               /**< \brief 位速率(同TA1, 含DR、DS) */
    uint8_t fwi;                    /**< \brief 帧等待时间因子 */
    uint8_t sfgi;                   /**< \brief 帧保护时间因子 */
    uint8_t tc1;                    /**< \brief 含是否支持CID、NAD */
    uint8_t fo;                     /**< \brief 帧选择是否支持CID、NAD (同TC1) */
} tpcl_prot_para_t;

/**
 * \brief ISO14443设备结构体
 */
typedef struct amdr_ic_iso14443_dev {

    /** \brief ISO14443服务结构体 */
    am_ic_iso14443_serv_t serv;

    /** \brief 全局变量，用于保存A型卡UID */
    uint8_t               uid_back[4];

    /** \brief T=CL通信协议参数 */
    tpcl_prot_para_t      cur_prot_para;

    /** \brief 读卡芯片驱动结构体 */
    am_fm175xx_dev_t     *p_fm17550s_dev;

} amdr_ic_iso14443_dev_t;

/**
 * \brief ISO14443驱动层初始化
 *
 * \param[in] p_dev : ISO14443设备结构体指针
 *
 * \return ISO14443服务句柄
 */
am_ic_iso14443_handle_t amdr_ic_iso14443_init (amdr_ic_iso14443_dev_t *p_dev,
                                               am_fm175xx_dev_t       *p_fm17550s_dev);

/**
 * \brief ISO14443驱动层解除初始化
 *
 * \param[in] p_dev : ISO14443设备结构体指针
 *
 * \return 无
 */
void amdr_ic_iso14443_deinit (amdr_ic_iso14443_dev_t *p_dev);

#ifdef __cplusplus
}
#endif

#endif /* __AMDR_IC_ISO14443_H */

/* end of file */
