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
 * \brief CPU PLUS卡标准服务接口
 *
 * \internal
 * \par Modification History
 * - 1.00 16-01-05  win, first implementation.
 * \endinternal
 */
#ifndef __AM_IC_CPU_PLUS_H
#define __AM_IC_CPU_PLUS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_common.h"

/**
 * \name SL3读卡模式
 * @{
 */

/** \brief 读卡模式0; 命令有MAC; 数据密文; 回应无MAC */
#define AM_IC_CPU_PLUS_SL3_READ_MODE0           0x30

/** \brief 读卡模式1; 命令有MAC; 数据密文; 回应有MAC */
#define AM_IC_CPU_PLUS_SL3_READ_MODE1           0x31

/** \brief 读卡模式2; 命令有MAC; 数据明文; 回应无MAC */
#define AM_IC_CPU_PLUS_SL3_READ_MODE2           0x32

/** \brief 读卡模式3; 命令有MAC; 数据明文; 回应有MAC */
#define AM_IC_CPU_PLUS_SL3_READ_MODE3           0x33

/** \brief 读卡模式4; 命令无MAC; 数据密文; 回应无MAC */
#define AM_IC_CPU_PLUS_SL3_READ_MODE4           0x34

/** \brief 读卡模式5; 命令无MAC; 数据密文; 回应有MAC */
#define AM_IC_CPU_PLUS_SL3_READ_MODE5           0x35

/** \brief 读卡模式6; 命令无MAC; 数据明文; 回应无MAC */
#define AM_IC_CPU_PLUS_SL3_READ_MODE6           0x36

/** \brief 读卡模式7; 命令无MAC; 数据明文; 回应有MAC */
#define AM_IC_CPU_PLUS_SL3_READ_MODE7           0x37

/** @} */

/**
 * \name SL3写卡模式
 * @{
 */

/** \brief 写卡模式0; 命令有MAC; 数据密文; 回应无MAC */
#define AM_IC_CPU_PLUS_SL3_WRITE_MODE0          0xA0

/** \brief 写卡模式1; 命令有MAC; 数据密文; 回应有MAC */
#define AM_IC_CPU_PLUS_SL3_WRITE_MODE1          0xA1

/** \brief 写卡模式2; 命令有MAC; 数据明文; 回应无MAC */
#define AM_IC_CPU_PLUS_SL3_WRITE_MODE2          0xA2

/** \brief 写卡模式3; 命令有MAC; 数据明文; 回应有MAC */
#define AM_IC_CPU_PLUS_SL3_WRITE_MODE3          0xA3

/** @} */

/**
 * \name SL3传输模式（带加/减操作）
 * @{
 */

/** \brief 加值传输模式0; 回应无MAC */
#define AM_IC_CPU_PLUS_SL3_INC_TRAN_MODE0       0xB6

/** \brief 加值传输模式1; 回应有MAC */
#define AM_IC_CPU_PLUS_SL3_INC_TRAN_MODE1       0xB7

/** \brief 减值传输模式0; 回应无MAC */
#define AM_IC_CPU_PLUS_SL3_DEC_TRAN_MODE0       0xB8

/** \brief 减值传输模式1; 回应有MAC */
#define AM_IC_CPU_PLUS_SL3_DEC_TRAN_MODE1       0xB9

/** @} */

/**
 * \name SL3复位模式
 * @{
 */

/** \brief 恢复模式0; 回应无MAC */
#define AM_IC_CPU_PLUS_SL3_RESTORE_MODE0        0xC2

/** \brief 恢复模式1; 回应有MAC */
#define AM_IC_CPU_PLUS_SL3_RESTORE_MODE1        0xC3

/** @} */

/**
 * \name SL3传输模式（不带加/减操作）
 * @{
 */

/** \brief 传输模式0; 回应无MAC */
#define AM_IC_CPU_PLUS_SL3_TRANSFER_MODE0        0xB4

/** \brief 传输模式1; 回应有MAC */
#define AM_IC_CPU_PLUS_SL3_TRANSFER_MODE1        0xB5

/** @} */

/**
 * \brief CPU PLUS卡标准服务驱动函数
 */
struct am_ic_cpu_plus_drv_funcs {

    /** \brief 卡片个人化时写入数据或者密钥(SL0专用命令) */
    int (*pfn_ic_personal_data_write) (void     *p_drv,
                                       uint32_t  addr,
                                       uint8_t  *p_buf);

    /** \brief 提交个人化(跃迁到SL1,SL0专用命令) */
    int (*pfn_ic_personal_commit) (void *p_drv);

    /** \brief 首次验证密钥 */
    int (*pfn_ic_first_key_auth) (void          *p_drv,
                                  uint32_t       key_addr,
                                  const uint8_t *p_key,
                                  uint32_t       nbytes);

    /** \brief 跟随验证密钥 */
    int (*pfn_ic_follow_key_auth) (void     *p_drv,
                                   uint32_t  key_addr,
                                   uint8_t  *p_key,
                                   uint32_t  nbytes);

    /** \brief 复位验证 */
    int (*pfn_ic_reset_auth) (void *p_drv);

    /** \brief SL3读数据命令 */
    int (*pfn_ic_data_read) (void     *p_drv,
                             uint8_t   mode,
                             uint8_t  *p_buf,
                             uint32_t  block_addr,
                             uint8_t   block_num);

    /** \brief SL3写数据命令 */
    int (*pfn_ic_data_write) (void          *p_drv,
                              uint8_t        mode,
                              const uint8_t *p_buf,
                              uint32_t       block_addr,
                              uint8_t        block_num);

    /** \brief 值块写数据(SL3) */
    int (*pfn_ic_value_block_write) (void    *p_drv,
                                     uint8_t  mode,
                                     uint32_t addr,
                                     int32_t  value);

    /** \brief 获取值块的数据（SL3）*/
    int (*pfn_ic_value_block_read) (void     *p_drv,
                                    uint8_t   mode,
                                    uint32_t  addr,
                                    int32_t  *p_value);

    /** \brief 值块操作(加/减，自带传输模式，SL3) */
    int (*pfn_ic_value_operation) (void     *p_drv,
                                   uint8_t   mode,
                                   uint32_t  src_addr,
                                   uint32_t  dst_addr,
                                   int32_t   value);

    /** \brief 值块恢复，将指定地址的值复制到缓冲区中 */
    int (*pfn_ic_restore) (void     *p_drv,
                           uint8_t   mode,
                           uint32_t  src_addr);

    /** \brief 值块传输，将值块操作结果(缓冲区)传输到指定的块 */
    int (*pfn_ic_transfer) (void     *p_drv,
                            uint8_t   mode,
                            uint32_t  dst_addr);
};

/**
 * \brief CPU PLUS卡标准服务结构体
 */
typedef struct am_ic_cpu_plus_serv {

    /** CPU PLUS卡标准服务驱动函数结构体指针 */
    struct am_ic_cpu_plus_drv_funcs *p_funcs;

    /* \brief 用于驱动函数的第一个参数 */
    void                            *p_drv;
} am_ic_cpu_plus_serv_t;

typedef am_ic_cpu_plus_serv_t *am_ic_cpu_plus_handle_t;

/**
 * \brief 卡片个人化时写入数据或者密钥(SL0专用命令)
 *
 * \param[in] handle : CPU PLUS卡标准服务句柄
 * \param[in] addr   : 数据块或密钥地址
 * \param[in] p_buf  : 16字节数据或密钥
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_ic_cpu_plus_personal_data_write (am_ic_cpu_plus_handle_t  handle,
                                        uint32_t                 addr,
                                        uint8_t                 *p_buf)
{
    return handle->p_funcs->pfn_ic_personal_data_write(handle->p_drv,
                                                       addr,
                                                       p_buf);
}

/**
 * \brief 提交个人化(跃迁到SL1,SL0专用命令)
 *
 * \param[in] handle : CPU PLUS卡标准服务句柄
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_ic_cpu_plus_personal_commit (am_ic_cpu_plus_handle_t handle)
{
    return handle->p_funcs->pfn_ic_personal_commit(handle->p_drv);
}

/**
 * \brief 首次验证密钥
 *
 * \param[in] handle   : CPU PLUS卡标准服务句柄
 * \param[in] key_addr : 密钥地址
 * \param[in] p_key    : 16字节密钥
 * \param[in] nbytes   : 密钥长度
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_ic_cpu_plus_first_key_auth (am_ic_cpu_plus_handle_t  handle,
                                   uint32_t                 key_addr,
                                   const uint8_t           *p_key,
                                   uint32_t                 nbytes)
{
    return handle->p_funcs->pfn_ic_first_key_auth(handle->p_drv,
                                                  key_addr,
                                                  p_key,
                                                  nbytes);
}

/**
 * \brief 跟随验证密钥
 *
 * \param[in] handle   : CPU PLUS卡标准服务句柄
 * \param[in] key_addr : 密钥地址
 * \param[in] p_key    : 16字节密钥
 * \param[in] nbytes   : 密钥长度
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_ic_cpu_plus_follow_key_auth (am_ic_cpu_plus_handle_t  handle,
                                    uint32_t                 key_addr,
                                    uint8_t                 *p_key,
                                    uint32_t                 nbytes)
{
    return handle->p_funcs->pfn_ic_follow_key_auth(handle->p_drv,
                                                   key_addr,
                                                   p_key,
                                                   nbytes);
}

/**
 * \brief 复位验证
 *
 * \param[in] handle : CPU PLUS卡标准服务句柄
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_ic_cpu_plus_reset_auth (am_ic_cpu_plus_handle_t handle)
{
    return handle->p_funcs->pfn_ic_reset_auth(handle->p_drv);
}

/**
 * \brief SL3读数据命令
 *
 * \param[in] handle    : CPU PLUS卡标准服务句柄
 * \param[in] mode      : 默认为SL3_READ_MODE3  PLUS S卡只支持模式SL3读卡模式
 *                         - AM_IC_CPU_PLUS_SL3_READ_MODE0  -- 读卡模式0
 *                         - AM_IC_CPU_PLUS_SL3_READ_MODE1  -- 读卡模式1
 *                         - AM_IC_CPU_PLUS_SL3_READ_MODE2  -- 读卡模式2
 *                         - AM_IC_CPU_PLUS_SL3_READ_MODE3  -- 读卡模式3
 *                         - AM_IC_CPU_PLUS_SL3_READ_MODE4  -- 读卡模式4
 *                         - AM_IC_CPU_PLUS_SL3_READ_MODE5  -- 读卡模式5
 *                         - AM_IC_CPU_PLUS_SL3_READ_MODE6  -- 读卡模式6
 *                         - AM_IC_CPU_PLUS_SL3_READ_MODE7  -- 读卡模式7
 * \param[out] p_buf     : 读出的16 * block_num数据
 * \param[in] block_addr : 读的起始块地址
 * \param[in] block_num  : 读的块数（取值为1～216）
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_ic_cpu_plus_data_read (am_ic_cpu_plus_handle_t  handle,
                              uint8_t                  mode,
                              uint8_t                 *p_buf,
                              uint32_t                 block_addr,
                              uint8_t                  block_num)
{
    return handle->p_funcs->pfn_ic_data_read(handle->p_drv,
                                             mode,
                                             p_buf,
                                             block_addr,
                                             block_num);
}

/**
 * \brief SL3写数据命令
 *
 * \param[in] handle    : CPU PLUS卡标准服务句柄
 * \param[in] mode      : 参考说明，写模式定义
 *                           AM_IC_CPU_PLUS_SL3_WRITE_MODE0  写卡模式0
 *                           AM_IC_CPU_PLUS_SL3_WRITE_MODE1  写卡模式1
 *                           AM_IC_CPU_PLUS_SL3_WRITE_MODE2  写卡模式2
 *                           AM_IC_CPU_PLUS_SL3_WRITE_MODE3  写卡模式3
 *                           默认为 SL3_WRITE_MODE3
 * \param[in] p_buf      : 写入的16 * block_num字节数据
 * \param[in] block_addr : 写的起始块地址
 * \param[in] block_num  : 写的块数（取值为1～3）
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_ic_cpu_plus_data_write (am_ic_cpu_plus_handle_t  handle,
                               uint8_t                  mode,
                               const uint8_t           *p_buf,
                               uint32_t                 block_addr,
                               uint8_t                  block_num)
{
    return handle->p_funcs->pfn_ic_data_write(handle->p_drv,
                                              mode,
                                              p_buf,
                                              block_addr,
                                              block_num);
}

/**
 * \brief 值块写数据(SL3)
 *
 * \param[in] handle : CPU PLUS卡标准服务句柄
 * \param[in] mode   : 写入的模式(和am_ic_cpu_plus_data_write()相同)
 * \param[in] addr   : 值存放地址
 * \param[in] value  : 写入值
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_ic_cpu_plus_value_block_write (am_ic_cpu_plus_handle_t handle,
                                      uint8_t                 mode,
                                      uint32_t                addr,
                                      int32_t                 value)
{
    return handle->p_funcs->pfn_ic_value_block_write(handle->p_drv,
                                                     mode,
                                                     addr,
                                                     value);
}

/**
 * \brief 获取值块的数据（SL3）
 *
 * \param[in] handle    : CPU PLUS卡标准服务句柄
 * \param[in] mode      : 读入模式（和am_ic_cpu_plus_data_read()相同）
 * \param[in] addr      : 值存放地址
 * \param[out] P_value  : 返回数据
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_ic_cpu_plus_value_block_read (am_ic_cpu_plus_handle_t  handle,
                                     uint8_t                  mode,
                                     uint32_t                 addr,
                                     int32_t                 *p_value)
{
    return handle->p_funcs->pfn_ic_value_block_read(handle->p_drv,
                                                    mode,
                                                    addr,
                                                    p_value);
}

/**
 * \brief 值块操作(加/减，自带传输模式，SL3)
 *
 * \param[in] handle   : CPU PLUS卡标准服务句柄
 * \param[in] mode     : 值块操作的模式, 只支持以下模式
 *                        AM_IC_CPU_PLUS_SL3_INC_TRAN_MODE0 加值传输模式0
 *                        AM_IC_CPU_PLUS_SL3_INC_TRAN_MODE1 加值传输模式1
 *                        AM_IC_CPU_PLUS_SL3_DEC_TRAN_MODE0 减值传输模式0
 *                        AM_IC_CPU_PLUS_SL3_DEC_TRAN_MODE1 减值传输模式1
 * \param[in] src_addr : 值存放源地址
 * \param[in] dst_addr : 操作后值存放的地址
 * \param[in] value    : 值
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_ic_cpu_plus_value_operation (am_ic_cpu_plus_handle_t handle,
                                    uint8_t                 mode,
                                    uint32_t                src_addr,
                                    uint32_t                dst_addr,
                                    int32_t                 value)
{
    return handle->p_funcs->pfn_ic_value_operation(handle->p_drv,
                                                   mode,
                                                   src_addr,
                                                   dst_addr,
                                                   value);
}

/**
 * \brief 值块恢复，将指定地址的值复制到缓冲区中
 *
 * \param[in] handle   : CPU PLUS卡标准服务句柄
 * \param[in] mode     : 值块操作的模式, 只支持以下模式
 *                       AM_IC_CPU_PLUS_SL3_RESTORE_MODE0  恢复模式0
 *                       AM_IC_CPU_PLUS_SL3_RESTORE_MODE1  恢复模式1
 * \param[in] src_addr : 值操作的源地址
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_ic_cpu_plus_restore (am_ic_cpu_plus_handle_t handle,
                            uint8_t                 mode,
                            uint32_t                src_addr)
{
    return handle->p_funcs->pfn_ic_restore(handle->p_drv,
                                           mode,
                                           src_addr);
}

/**
 * \brief 值块传输，将值块操作结果(缓冲区)传输到指定的块
 *
 * \param[in] handle   : CPU PLUS卡标准服务句柄
 * \param[in] mode     : 值块操作的模式, 只支持以下模式
 *                       AM_IC_CPU_PLUS_SL3_TRANSFER_MODE0 传输模式0
 *                       AM_IC_CPU_PLUS_SL3_TRANSFER_MODE1 传输模式1
 * \param[in] dst_addr : 值操作目的地址
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_ic_cpu_plus_transfer (am_ic_cpu_plus_handle_t handle,
                             uint8_t                 mode,
                             uint32_t                dst_addr)
{
    return handle->p_funcs->pfn_ic_transfer(handle->p_drv,
                                            mode,
                                            dst_addr);
}

#ifdef __cplusplus
}
#endif

#endif /* __AM_IC_CPU_PLUS_H */

/* end of file */

