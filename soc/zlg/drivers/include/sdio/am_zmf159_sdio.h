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
 * \brief SDIO驱动，服务SDIO标准接口
 *
 * \internal
 * \par Modification History
 * - 1.01 17-12-28  pea, add a timeout mechanism, enable TX_ABRT and STOP_DET
 *                  interrupt
 * - 1.00 17-04-24  sdy, first implementation
 * \endinternal
 */

#ifndef __AM_ZLG_SDIO_H
#define __AM_ZLG_SDUI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_int.h"
#include "am_sdio.h"
#include "am_list.h"
#include "am_wait.h"
#include "am_softimer.h"

#include "am_vdebug.h"

/**
 * \addtogroup am_zlg_if_sdio
 * \copydoc am_zlg_sdio.h
 * @{
 */

/**
 * \brief SDIO设备信息参数结构体
 */
typedef struct am_zlg_sdio_devinfo {

    uint32_t              regbase;

    uint32_t              inum;

    /** \brief 时钟ID */
    int                   clk_num;

    uint8_t               width;

    uint32_t              speed;

    uint8_t               speed_mode;

    /** \brief 平台初始化函数 */
    void    (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void    (*pfn_plfm_deinit)(void);

} am_zlg_sdio_devinfo_t;

/**
 * \brief SDIO设备结构体
 */
typedef struct am_zlg_sdio_dev {

	am_sdio_serv_t                          sdio_serv;

    /** \brief SDIO控制器消息队列 */
    struct am_list_head                     msg_list;

//    /** \brief 指向SDIO传输结构体的指针,同一时间只能处理一个传输 */
//    am_sdio_transfer_t                      *p_cur_trans;

    /** \brief 是否完成本次消息 */
    volatile am_bool_t                       is_complete;

    /** \brief 用于数据接收/发送计数 */
    volatile uint32_t                        data_ptr;

    /** \brief 忙标识 */
    volatile am_bool_t                       busy;

    /** \brief 状态 */
    volatile uint8_t                         state;

    /** \brief 是否中断状态机 */
    volatile am_bool_t                       is_abort;

    /** \brief 指向SDIO设备信息的指针 */
    const am_zlg_sdio_devinfo_t             *p_devinfo;

    am_wait_t                                wait;

    uint16_t                                 int_status;

} am_zlg_sdio_dev_t;

/**
 * \brief SDIO初始化
 *
 * \param[in] p_dev     : 指向SDIO设备结构体的指针
 * \param[in] p_devinfo : 指向SDIO设备信息结构体的指针
 *
 * \return SDIO标准服务操作句柄
 */
am_sdio_handle_t am_zmf159_sdio_init (am_zlg_sdio_dev_t           *p_dev,
                                     const am_zlg_sdio_devinfo_t *p_devinfo);

/**
 * \brief 解除SDIO初始化
 *
 * \param[in] handle : 与从设备关联的SDIO标准服务操作句柄
 *
 * \return 无
 */
void am_zmf159_sdio_deinit (am_sdio_handle_t handle);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __AM_ZLG_SDIO_H */

/* end of file */
