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
 * \brief 支持设备控制类指令的标准接口
 *
 * \internal
 * \par Modification History
 * - 1.00 15-11-27  win, first implementation.
 * \endinternal
 */
#ifndef __AM_DR_READER_CARD_H
#define __AM_DR_READER_CARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_reader_card.h" 
#include "am_uartcmd_includes.h"
#include "am_uartcmd_cd4051.h"
/**
 * \brief 读卡芯片设备结构体
 */
typedef struct amdr_reader_card_dev {
    
    /** \brief 读卡芯片服务结构体 */
    am_reader_card_serv_t  serv;
    
    /** \brief 读卡芯片设备结构体指针 */
    am_fm175xx_dev_t      *p_fm17550s_dev;

    /*  \brief 芯片天线设备结构体指针*/
    am_antenna_info_t     *p_antenna_info;

} amdr_reader_card_dev_t;

/**
 * \brief 读卡芯片驱动层初始化函数 
 *
 * \param[in] p_dev : 读卡芯片设备结构体指针
 *
 * \retval AM_OK : 成功
 * \retval -AM_EIINVAL : 参数错误
 */
am_reader_card_handle_t amdr_reader_card_init (amdr_reader_card_dev_t   *p_dev,
                                               am_fm175xx_dev_t         *p_fm17550s_dev,
                                               am_antenna_info_t       *p_cd4051_dev);

/**
 * \brief 读卡芯片驱动层去初始化函数 
 *
 * \param[in] p_dev : 读卡芯片设备结构体指针
 *
 * \retval AM_OK : 成功
 * \retval -AM_EIINVAL : 参数错误
 */
int amdr_reader_card_deinit (amdr_reader_card_dev_t *p_dev);
    
#ifdef __cplusplus
}
#endif

#endif /* __AMDR_READER_CARD_H */

/* end of file */
