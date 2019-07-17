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
#ifndef __AM_READER_CARD_H
#define __AM_READER_CARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_common.h"

/**
 * \name 读卡芯片支持的ISO协议类型
 * @{
 */

/** \brief ISO14443 Type A */
#define AM_READER_CARD_ISO14443A          0x00

/** \brief ISO14443 Type B */
#define AM_READER_CARD_ISO14443B          0x01

/** \brief ISO15693 快速模式 */
#define AM_READER_CARD_ISO15693_FAST      0x02

/** \brief ISO15693 标准模式 */
#define AM_READER_CARD_ISO15693_STANDARD  0x03

/** @} */

/**
 * \name 天线的驱动模式 (TX1,TX2驱动方式)
 * @{
 */

/** \brief 同时关闭TX1和TX2 */
#define AM_READER_CARD_ALL_OFF            0x00

/** \brief 仅TX1驱动天线 */
#define AM_READER_CARD_TX1_ON             0x01

/** \brief 仅TX2驱动天线 */
#define AM_READER_CARD_TX2_ON             0x02

/** \brief TX1和TX2同时驱动天线 */
#define AM_READER_CARD_ALL_ON             0x03

/** @} */

/**
 * \brief 读卡芯片驱动函数结构体
 */
struct am_reader_card_drv_funcs {

    /** \brief 关闭芯片 */
    int (*pfn_cd_close) (void *p_drv);

    /** \brief 复位芯片 */
    int (*pfn_cd_reset) (void *p_drv);

    /** \brief 配置芯片 */
    int (*pfn_cd_config) (void *p_drv);

    /** \brief 设置芯片ISO协议类型 */
    int (*pfn_cd_iso_type_set) (void *p_drv, uint8_t type);
	
	  /** \brief 设置Tx通道号*/
    int (*pfn_cd_channal_set) (void *p_drv, uint8_t channal_num);

    /** \brief 设置芯片寄存器的值 */
    int (*pfn_cd_reg_write) (void     *p_drv,
                             uint32_t  reg_addr,
                             void     *p_tx_buf,
                             uint32_t  nbytes);

    /** \brief 获取芯片寄存器的值 */
    int (*pfn_cd_reg_read) (void     *p_drv,
                            uint32_t  reg_addr,
                            uint32_t *p_rx_buf,
                            uint32_t  nbytes);

    /** \brief 设置天线的驱动模式 */
    int (*pfn_cd_antenna_mode_set) (void *p_drv, uint32_t mode);

    /** \brief 获取天线的驱动模式 */
    int (*pfn_cd_antenna_mode_get) (void *p_drv, uint32_t *p_mode);

    /** \brief 数据交换 */
    int (*pfn_cd_block_exchange) (void     *p_drv,
                                  uint8_t  *p_tx_buf,
                                  uint32_t  tx_nbytes,
                                  uint8_t  *p_rx_buf,
                                  uint32_t  rx_buf_size,
                                  uint32_t *p_rx_nbytes,
                                  uint8_t   nwtxm_crc,
                                  uint8_t   nfwi);

    /** \brief 载波暂停 */
    int (*pfn_cd_wave_pause) (void     *p_drv,
                              uint32_t  pause_ms,
                              uint32_t  wait_ms);

    /** \brief 获取身份证ID */
    int (*pfn_cd_sid_get) (void     *p_drv,
                           uint8_t  *p_uid,
                           uint32_t *p_rx_nbytes);

    /** \brief 获取复位次数 */
    int (*pfn_cd_reset_count_get) (void *p_drv, uint32_t *p_count);

};

/**
 * \brief 读卡芯片服务结构体
 */
typedef struct am_reader_card_serv {

    /** \brief 读卡芯片驱动函数结构体指针 */
    struct am_reader_card_drv_funcs *p_funcs;

    /** \brief 用于驱动函数的第一个参数 */
    void                            *p_drv;

} am_reader_card_serv_t;

/** \brief 定义读卡芯片服务句柄 */
typedef am_reader_card_serv_t *am_reader_card_handle_t;

/**
 * \brief 关闭读卡芯片
 *
 * \param[in] handle : 读卡芯片服务句柄
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_reader_card_close (am_reader_card_handle_t handle)
{
    return handle->p_funcs->pfn_cd_close(handle->p_drv);
}

/**
 * \brief 复位芯片
 *
 * \param[in] handle : 读卡芯片服务句柄
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_reader_card_reset (am_reader_card_handle_t handle)
{
    return handle->p_funcs->pfn_cd_reset(handle->p_drv);
}

/**
 * \brief 配置芯片
 *
 * \param[in] handle : 读卡芯片服务句柄
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_reader_card_config (am_reader_card_handle_t handle)
{
    return handle->p_funcs->pfn_cd_config(handle->p_drv);
}

/**
 * \brief 设置芯片ISO协议类型
 *
 * \param[in] handle : 读卡芯片服务句柄
 * \param[in] type   : ISO协议类型
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_reader_card_iso_type_set (am_reader_card_handle_t handle,
                                 uint8_t                 type)
{
    return handle->p_funcs->pfn_cd_iso_type_set(handle->p_drv,
                                                type);
}

/**
 * \brief 设置芯片Tx通道号
 *
 * \param[in] handle : 读卡芯片服务句柄
 * \param[in] type   : 通道号
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_reader_card_tx_channal_set (am_reader_card_handle_t handle,
                                   uint8_t                 num)
{
    return handle->p_funcs->pfn_cd_channal_set(handle->p_drv, num);
}

/**
 * \brief 设置芯片寄存器的值
 * \note  寄存器地址自动增加
 *
 * \param[in] handle   : 读卡芯片服务句柄
 * \param[in] reg_addr : 芯片内部寄存器的地址
 * \param[in] p_tx_buf : 设置的值，存放缓冲区
 * \param[in] nbytes   : 发送的字节数
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_reader_card_reg_write (am_reader_card_handle_t  handle,
                              uint32_t                 reg_addr,
                              void                    *p_tx_buf,
                              uint32_t                 nbytes)
{
    return handle->p_funcs->pfn_cd_reg_write(handle->p_drv,
                                            reg_addr,
                                            p_tx_buf,
                                            nbytes);
}

/**
 * \brief 获取芯片寄存器的值
 * \note  寄存器地址自动增加
 *
 * \param[in] handle   : 读卡芯片服务句柄
 * \param[in] reg_addr : 芯片内部寄存器的地址
 * \param[in] p_tx_buf : 获取的值，存放的缓冲区
 * \param[in] nbytes   : 接收的字节数
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_reader_card_reg_read (am_reader_card_handle_t  handle,
                             uint32_t                 reg_addr,
                             void                    *p_rx_buf,
                             uint32_t                 nbytes)
{
    return handle->p_funcs->pfn_cd_reg_read(handle->p_drv,
                                            reg_addr,
                                            p_rx_buf,
                                            nbytes);
}

/**
 * \brief 设置天线的驱动模式
 *
 * \param[in] handle : 读卡芯片服务句柄
 * \param[in] mode   : 天线模式
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_reader_card_antenna_mode_set (am_reader_card_handle_t  handle,
                                     uint32_t                 mode)
{
    return handle->p_funcs->pfn_cd_antenna_mode_set(handle->p_drv, mode);
}

/**
 * \brief 获取天线的驱动模式
 *
 * \param[in] handle : 读卡芯片服务句柄
 * \param[in] p_mode : 天线模式
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_reader_card_antenna_mode_get (am_reader_card_handle_t   handle,
                                     uint32_t                 *p_mode)
{
    return handle->p_funcs->pfn_cd_antenna_mode_get(handle->p_drv, p_mode);
}

/**
 * \brief 数据交换
 *
 * \param[in] handle      : 读卡芯片服务句柄
 * \param[in] p_tx_buf    : 待发送的数据
 * \param[in] tx_nbytes   : 发送数据的长度
 * \param[in] p_rx_buf    : 待接收的数据
 * \param[in] rx_buf_size : 接收缓冲区大小
 * \param[in] p_rx_nbytes : 共接收到多少字节
 * \param[in] nwtxm_crc   : crc校验参数
 * \param[in] nfwi        : 延时
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_reader_card_block_exchange (am_reader_card_handle_t  handle,
                                   uint8_t                 *p_tx_buf,
                                   uint32_t                 tx_nbytes,
                                   uint8_t                 *p_rx_buf,
                                   uint32_t                 rx_buf_size,
                                   uint32_t                *p_rx_nbytes,
                                   uint8_t                  nwtxm_crc,
                                   uint8_t                  nfwi)
{
    return handle->p_funcs->pfn_cd_block_exchange(handle->p_drv,
                                                  p_tx_buf,
                                                  tx_nbytes,
                                                  p_rx_buf,
                                                  rx_buf_size,
                                                  p_rx_nbytes,
                                                  nwtxm_crc,
                                                  nfwi);
}

/**
 * \brief 载波暂停
 *
 * \param[in] handle   : 读卡芯片服务句柄
 * \param[in] pause_ms : 暂停时间，关闭RF场该时间后重新打开，若为0则不重新打开
 * \param[in] wait_ms  : RF场重新打开后持续等待该时间，若为0则不等待
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_reader_card_wave_pause (am_reader_card_handle_t handle,
                               uint32_t                pause_ms,
                               uint32_t                wait_ms)
{
    return handle->p_funcs->pfn_cd_wave_pause(handle->p_drv,
                                              pause_ms,
                                              wait_ms);
}

/**
 * \brief 获取身份证ID
 *
 * \param[in] handle      : 读卡芯片服务句柄
 * \param[in] p_uid       : 返回的身份证ID
 * \param[in] p_rx_nbytes : 身份证ID的字节数
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_reader_card_sid_get (am_reader_card_handle_t  handle,
                            uint8_t                 *p_uid,
                            uint32_t                *p_rx_nbytes)
{
    return handle->p_funcs->pfn_cd_sid_get(handle->p_drv,
                                           p_uid,
                                           p_rx_nbytes);
}

/**
 * \brief 获取读卡芯片复位次数
 *
 * \param[in]  handle  : 读卡芯片服务句柄
 * \param[out] p_count : 返回复位次数
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_reader_card_reset_count_get (am_reader_card_handle_t  handle,
                                    uint32_t                *p_count)
{
    return handle->p_funcs->pfn_cd_reset_count_get(handle->p_drv, p_count);
}

#ifdef __cplusplus
}
#endif

#endif /* __AM_READER_CARD_H */

/* end of file */
