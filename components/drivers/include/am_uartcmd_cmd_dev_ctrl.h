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
 * \brief 
 *
 * \internal
 * \par Modification History
 * - 1.00 15-11-27  win, first implementation.
 * \endinternal
 */
#ifndef __AM_UARTCMD_CMD_DEV_CTRL_H
#define __AM_UARTCMD_CMD_DEV_CTRL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_reader_card.h"
#include "am_uartcmd_cmd.h"
#include "am_uartcmd_cmd_define.h"

/** \brief 声明IAP函数指针类型 */
typedef int (*pfn_iap_funcs_t)(uint32_t dst_addr,
                               uint32_t src_addr,
                               uint32_t nbytes);
/**
 * \brief 设备控制类指令操作FLASH函数
 *        这两个函数由用户传入
 */
struct uartcmd_cmd_dev_ctrl_funcs {

    /**
     * \brief 将内存中的数据写入FLASH中
     *        返回值为AM_OK表示成功，其他值表示失败
     */
    pfn_iap_funcs_t pfn_iap_copy;

    /**
     * \brief 比较内存中的数据和FLASH中的数据
     *        返回值为AM_OK表示成功，其他值表示失败
     */
    pfn_iap_funcs_t pfn_iap_compare;
};

/**
 * \brief 设置设备控制类命令结构体参数
 * 
 * \param[in] p_cmd       : 设备控制类命令结构体指针
 * \param[in] p_funcs     : 设备控制类指令操作FLASH函数结构体指针
 * \param[in] cd_handle   : 读卡器服务句柄
 * \param[in] pfn_copy    : IAP拷贝函数指针
 * \param[in] pfn_compare : IAP比较函数指针
 * \param[in] cmd_classs  : 命令类型
 * 
 * \retval AM_OK : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int uartcmd_cmd_dev_ctrl_mkdev (uartcmd_cmd_t                     *p_cmd,
                                struct uartcmd_cmd_dev_ctrl_funcs *p_funcs,
                                am_reader_card_handle_t            cd_handle,
                                pfn_iap_funcs_t                    pfn_copy,
                                pfn_iap_funcs_t                    pfn_compare,
                                uint8_t                            cmd_class)
{
    if (p_cmd       == NULL ||
        p_funcs     == NULL ||
        pfn_copy    == NULL ||
        pfn_compare == NULL) {
        return -AM_EINVAL;
    }

    p_funcs->pfn_iap_copy    = pfn_copy;
    p_funcs->pfn_iap_compare = pfn_compare;
    p_cmd->p_arg             = (void *)p_funcs;
    p_cmd->cmd_class         = cmd_class;
    p_cmd->p_iso_handle      = (void *)cd_handle;

    return AM_OK;
}

/**
 * \brief 字节累加和计算 
 * 
 * \param[in] p_buf  : 需要计算累加和的缓冲区
 * \param[in] nbytes : 缓冲区字节数
 *
 * \return 累加和
 */
uint32_t uartcmd_cmd_byte_sum_get (const void *p_buf, uint32_t nbytes);

/**
 * \brief 设备控制类命令初始化
 *
 * \param[in] handle : UARTCMD服务句柄
 * \param[in] p_cmd  : 设备控制类命令结构体指针
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
int uartcmd_cmd_dev_ctrl_init (am_uartcmd_handle_t  handle,
                               uartcmd_cmd_t       *p_cmd);

/**
 * \brief 设备控制类命令去除初始化
 *
 * \param[in] handle : UARTCMD服务句柄
 * \param[in] p_cmd  : 设备控制类命令结构体指针
 *
 * \return 无
 */
void uartcmd_cmd_dev_ctrl_deinit (am_uartcmd_handle_t handle, uartcmd_cmd_t *p_cmd);

#ifdef __cplusplus
}
#endif


#endif /* __UARTCMD_CMD_DEV_CTRL_H */

/* end of file */
