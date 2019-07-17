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
 * - 1.00 15-09-  win, first implementation.
 * \endinternal
 */
#ifndef __AM_IC_MIFARE_H
#define __AM_IC_MIFARE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_common.h"
#include "am_uartcmd_ic_iso14443.h"

/**
 * \name Mifare卡密钥类型
 * @{
 */

#define AM_IC_MIFARE_KEY_TYPE_MASK    0x7F    /**< \brief 密钥类型掩码 */
#define AM_IC_MIFARE_KEY_TYPE_A       0x60    /**< \brief 密钥A验证 */
#define AM_IC_MIFARE_KEY_TYPE_B       0x61    /**< \brief 密钥B验证 */

#define AM_IC_MIFARE_KEY_SRC_MASK     0x80    /**< \brief 密钥输入源掩码 */
#define AM_IC_MIFARE_KEY_SRC_EXT      0x00    /**< \brief 使用外部输入的密钥验证 */
#define AM_IC_MIFARE_KEY_SRC_E2       0x80    /**< \brief 使用内部E2的密钥验证 */

/** @} */

/**
 * \brief Mifare 卡命令定义
 * @{
 */
#define AM_IC_MIFARE1_CMD_READ            0x30    /**< \brief 读命令(16字节) */
#define AM_IC_MIFARE1_CMD_WRITE           0xA0    /**< \brief 写命令(16字节) */
#define AM_IC_MIFARE1_CMD_TRANSFER        0xB0    /**< \brief 传输命令 */
#define AM_IC_MIFARE1_CMD_NO_COMMAND      0x00    /**< \brief 空命令 */
#define AM_IC_MIFARE1_CMD_INCREMENT       0xC1    /**< \brief 值增命令 */
#define AM_IC_MIFARE1_CMD_DECREMENT       0xC0    /**< \brief 值减命令 */
#define AM_IC_MIFARE1_CMD_RESTORE         0xC2    /**< \brief 复制命令 */
#define AM_IC_MIFARE0_CMD_WRITE           0xA2    /**< \brief UltraLight 写命令(4字节) */
#define AM_IC_MIFARE0_CMD_AUTH            0xA3    /**< \brief UltraLight 验证命令 */

/** @} */

/**
 * \brief Mifare卡标准服务驱动函数
 */
struct am_ic_mifare_drv_funcs {

    /** \brief 密钥验证 */
    int (*pfn_ic_key_auth) (void       *p_drv,
                            uint8_t     mode,
                            const void *p_key,
                            uint32_t    key_nbytes,
                            const void *p_uid,
                            uint32_t    uid_nbytes,
                            uint8_t     block);

    /** \brief Mifare卡扩展激活 */
    int (*pfn_ic_activate) (void                 *p_drv,
                            uint8_t               mode,
                            uint8_t               req_code,
                            am_ic_a_reset_info_t *p_reset_info);

    /** \brief Mifare卡读取多个块数据 */
    int (*pfn_ic_blocks_read) (void    *p_drv,
                               uint8_t  start_block,
                               uint8_t  block_num,
                               void    *p_buf);

    /** \brief Mifare卡写多个块 */
    int (*pfn_ic_blocks_write) (void       *p_drv,
                                uint8_t     start_block,
                                uint8_t     block_num,
                                const void *p_buf);

    /** \brief Mifare卡获取值块数据 */
    int (*pfn_ic_value_block_get) (void    *p_drv,
                                   uint8_t  block,
                                   int32_t *p_value);

    /** \brief Mifare卡设置值块的值 */
    int (*pfn_ic_value_block_set) (void    *p_drv,
                                   uint8_t  block,
                                   int32_t  value);

    /** \brief Mifare卡值块操作 */
    int (*pfn_ic_value_operation) (void    *p_drv,
                                   uint8_t  mode,
                                   uint8_t  src_block,
                                   int32_t  value,
                                   uint8_t  dst_block);

    /** \brief Mifare UltraLight卡读取多个块数据 */
    int (*pfn_ic_ultralight_blocks_read) (void    *p_drv,
                                          uint8_t  start_block,
                                          uint8_t  block_num,
                                          void    *p_buf);

    /** \brief Mifare UltraLight卡设置多个块 */
    int (*pfn_ic_ultralight_blocks_write) (void       *p_drv,
                                           uint8_t     start_block,
                                           uint8_t     block_num,
                                           const void *p_buf);

    /** \brief Mifare UltraLight卡传输 */
    int (*pfn_ic_ultralight_transfer) (void       *p_drv,
                                       const void *p_txbuf,
                                       void       *p_rxbuf,
                                       uint32_t   *p_rx_nbytes);
};

/**
 * \brief Mifare标准服务结构体
 */
typedef struct am_ic_mifare_serv {

    /** Mifare卡标准服务驱动函数结构体指针 */
    struct am_ic_mifare_drv_funcs *p_funcs;

    /* \brief 用于驱动函数的第一个参数 */
    void                          *p_drv;
} am_ic_mifare_serv_t;

/** \brief Mifare卡标准服务句柄 */
typedef am_ic_mifare_serv_t *am_ic_mifare_handle_t;

/**
 * \brief Mifare卡密钥验证
 *
 * \param[in] handle     : Mifare卡标准服务句柄
 * \param[in] mode       : 验证模式
 *                         - mode.7         -- 密钥来源
 *                           - AM_IC_MIFARE_KEY_SRC_EXT 使用外部输入的密钥
 *                           - AM_IC_MIFARE_KEY_SRC_E2  使用内部E2的密钥
 *                         - mode.6-0       -- 密钥类型
 *                           - AM_IC_MIFARE_KEY_TYPE_A  密钥A
 *                           - AM_IC_MIFARE_KEY_TYPE_B  密钥B
 * \param[in] p_key      : 密钥
 * \param[in] key_nbytes : 密钥长度
 * \param[in] p_uid      : UID
 * \param[in] uid_nbytes : UID长度
 * \param[in] block      : 块号
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_ic_mifare_key_auth (am_ic_mifare_handle_t  handle,
                           uint8_t                mode,
                           const void            *p_key,
                           uint32_t               key_nbytes,
                           const void            *p_uid,
                           uint32_t               uid_nbytes,
                           uint8_t                block)
{
    return handle->p_funcs->pfn_ic_key_auth(handle->p_drv,
                                            mode,
                                            p_key,
                                            key_nbytes,
                                            p_uid,
                                            uid_nbytes,
                                            block);
}

/**
 * \brief Mifare卡激活
 *
 * \param[in] handle       : Mifare卡标准服务句柄
 * \param[in] mode         : 防碰撞模式
 *                           - 0  执行防碰撞循环，
 *                           - 1  只执行一次防碰撞
 * \param[in] req_code     : 请求代码
 *                           - AM_IC_ISO14443_3A_REQIDL 请求空闲卡
 *                           - AM_IC_ISO14443_3A_REQALL 请求所有卡
 * \param[in] p_reset_info : ISO14443-3A卡激活回应信息
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_ic_mifare_activate (am_ic_mifare_handle_t  handle,
                           uint8_t                mode,
                           uint8_t                req_code,
                           am_ic_a_reset_info_t  *p_reset_info)
{
    return handle->p_funcs->pfn_ic_activate(handle->p_drv,
                                            mode,
                                            req_code,
                                            p_reset_info);
}

/**
 * \brief Mifare卡读取多个块数据
 *
 * \param[in] handle       : Mifare卡标准服务句柄
 * \param[in] start_block  : 读的起始块号
 * \param[in] block_num    : 读的块数（所有块必须在同一扇区）
 * \param[in] p_buf        : 读出的16 * block_num 字节数据
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_ic_mifare_blocks_read (am_ic_mifare_handle_t  handle,
                              uint8_t                start_block,
                              uint8_t                block_num,
                              void                  *p_buf)
{
    return handle->p_funcs->pfn_ic_blocks_read(handle->p_drv,
                                               start_block,
                                               block_num,
                                               p_buf);

}

/**
 * \brief Mifare卡写多个块
 *
 * \param[in] handle       : Mifare卡标准服务句柄
 * \param[in] start_block  : 写的起始块号
 * \param[in] block_num    : 写的块数（所有块必须在同一扇区）
 * \param[in] p_buf        : 写入的16 * block_num 字节数据
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_ic_mifare_blocks_write (am_ic_mifare_handle_t  handle,
                               uint8_t                start_block,
                               uint8_t                block_num,
                               const void            *p_buf)
{
    return handle->p_funcs->pfn_ic_blocks_write(handle->p_drv,
                                                start_block,
                                                block_num,
                                                p_buf);
}

/**
 * \brief Mifare卡获取值块数据
 *
 * \param[in] handle  : Mifare卡标准服务句柄
 * \param[in] block   : 块号
 * \param[in] p_value : 值(有符号数)
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_ic_mifare_value_block_get (am_ic_mifare_handle_t  handle,
                                  uint8_t                block,
                                  int32_t               *p_value)
{
    return handle->p_funcs->pfn_ic_value_block_get(handle->p_drv,
                                                   block,
                                                   p_value);
}

/**
 * \brief Mifare卡设置值块的值
 *
 * \param[in] handle : Mifare卡标准服务句柄
 * \param[in] block  : 块号
 * \param[in] value  : 值(有符号数)
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_ic_mifare_value_block_set (am_ic_mifare_handle_t handle,
                                  uint8_t               block,
                                  int32_t               value)
{
    return handle->p_funcs->pfn_ic_value_block_set(handle->p_drv,
                                                   block,
                                                   value);
}

/**
 * \brief Mifare卡值块操作
 *
 * \param[in] handle    : Mifare卡标准服务句柄
 * \param[in] mode      : 值操作模式
 *                        - AM_IC_MIFARE1_CMD_INCREMENT 将源块的值加上指定的值存放到目的块
 *                        - AM_IC_MIFARE1_CMD_DECREMENT 将源块的值减去指定的值存放到目的块
 *                        - AM_IC_MIFARE1_CMD_RESTORE   将源块的值复制到目的块
 * \param[in] src_block : 源块
 * \param[in] value     : 值(最高位无效, 即没有符号位)
 * \param[in] dst_block : 目的块
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_ic_mifare_value_operation (am_ic_mifare_handle_t handle,
                                  uint8_t               mode,
                                  uint8_t               src_block,
                                  int32_t               value,
                                  uint8_t               dst_block)
{
    return handle->p_funcs->pfn_ic_value_operation(handle->p_drv,
                                                   mode,
                                                   src_block,
                                                   value,
                                                   dst_block);
}

/**
 * \brief Mifare UltraLight卡读取多个块数据
 *
 * \param[in] handle      : Mifare卡标准服务句柄
 * \param[in] start_block : 读的起始块号
 * \param[in] block_num   : 读的块数
 * \param[in] p_buf       : 读出的4 * block_num字节数据
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_ic_mifare_ultralight_blocks_read (am_ic_mifare_handle_t   handle,
                                         uint8_t                 start_block,
                                         uint8_t                 block_num,
                                         void                   *p_buf)
{
    return handle->p_funcs->pfn_ic_ultralight_blocks_read(handle->p_drv,
                                                          start_block,
                                                          block_num,
                                                          p_buf);
}

/**
 * \brief Mifare UltraLight卡设置多个块
 *
 * \param[in] handle      : Mifare卡标准服务句柄
 * \param[in] start_block : 写的起始块号
 * \param[in] block_num   : 写的块数
 * \param[in] p_buf       : 写入的4 * block_num字节数据
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_ic_mifare_ultralight_blocks_write (am_ic_mifare_handle_t   handle,
                                          uint8_t                 start_block,
                                          uint8_t                 block_num,
                                          const void             *p_buf)
{
    return handle->p_funcs->pfn_ic_ultralight_blocks_write(handle->p_drv,
                                                           start_block,
                                                           block_num,
                                                           p_buf);
}

/**
 * \brief Mifare UltraLight卡传输
 *
 * \param[in] handle      : Mifare卡标准服务句柄
 * \param[in] p_txbuf     : 发送缓冲区，存放要发送给卡片的数据
 * \param[in] p_rxbuf     : 接收缓冲区，存放从卡片读取过来的数据
 * \param[in] p_rx_nbytes : 接受的数据长度（字节）
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_ic_mifare_ultralight_transfer (am_ic_mifare_handle_t   handle,
                                      const void             *p_txbuf,
                                      void                   *p_rxbuf,
                                      uint32_t               *p_rx_nbytes)
{
    return handle->p_funcs->pfn_ic_ultralight_transfer(handle->p_drv,
                                                       p_txbuf,
                                                       p_rxbuf,
                                                       p_rx_nbytes);
}

#ifdef __cplusplus
}
#endif

#endif /* __AM_IC_MIFARE_H */

/* end of file */
