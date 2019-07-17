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
 * - 1.00 16-01-04  win, first implementation.
 * \endinternal
 */
#ifndef __AM_IC_ISO14443_H
#define __AM_IC_ISO14443_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_common.h"

/**
 * \addtogroup if_iso14443
 * \copydoc iso14443.h
 * @{
 */

/**
 * \name ISO14443命令
 * @{
 */

#define AM_IC_A_REQUEST_EN       1   /**< \brief 请求命令 */
#define AM_IC_A_ANTICOLL_EN      1   /**< \brief 防碰撞命令 */
#define AM_IC_A_SELECT_EN        1   /**< \brief 选择卡命令 */
#define AM_IC_A_HALT_EN          1   /**< \brief 挂起命令 */
#define AM_IC_A_ACTIVATE_EN      1   /**< \brief 激活 */

#define AM_IC_B_REQUEST_EN       1   /**< \brief ISO14443-3B卡请求 */
#define AM_IC_B_SLOT_MARKER_EN   0   /**< \brief ISO14443-3B卡时隙防碰撞 */
#define AM_IC_B_ATTRIB_EN        1   /**< \brief ISO14443-3B卡选择 */
#define AM_IC_B_HALT_EN          1   /**< \brief ISO14443-3B卡挂起 */
#define AM_IC_B_ACTIVATE_EN      1   /**< \brief ISO14443-3B卡激活 */

#define AM_IC_A_RATS_EN          1   /**< \brief RATS           (ISO14443A) */
#define AM_IC_A_PPS_EN           1   /**< \brief PPS            (ISO14443A) */
#define AM_IC_A_DESELECT_EN      1   /**< \brief DESELECT       (ISO14443A) */
#define AM_IC_B_PROT_PARSE_EN    1   /**< \brief B型卡参数解析       (ISO14443B) */
#define AM_IC_TPCL_EN            1   /**< \brief TPCL           (ISO14443)  */

/** @} */

/**
 * \name ISO14443协议命令码 -- A型卡命令代码
 * @{
 */

#define AM_IC_ISO14443_3A_REQALL            0x52    /**< \brief 请求所有的卡 */
#define AM_IC_ISO14443_3A_REQIDL            0x26    /**< \brief 请求空闲的卡 */
#define AM_IC_ISO14443_3A_SELECT_LEVEL_1    0x93    /**< \brief 一级防碰撞/选择 */
#define AM_IC_ISO14443_3A_SELECT_LEVEL_2    0x95    /**< \brief 二级防碰撞/选择 */
#define AM_IC_ISO14443_3A_SELECT_LEVEL_3    0x97    /**< \brief 三级防碰撞/选择 */

/** @} */

/**
 * \name ISO14443协议命令码 -- B型卡命令代码
 * @{
 */

#define AM_IC_ISO14443_3B_REQIDL            0x00    /**< \brief IDLE */
#define AM_IC_ISO14443_3B_REQALL            0x08    /**< \brief ALL */

/** @} */

/**
 * \brief ISO14443子协议类型
 */
typedef enum am_ic_iso14443_subprot_type {
    AM_IC_ISO14443A_106       = 0x00,     /**< \brief ISO14443A */
    AM_IC_ISO14443A_212       = 0x01,     /**< \brief ISO14443A */
    AM_IC_ISO14443A_424       = 0x02,     /**< \brief ISO14443A */
    AM_IC_ISO14443A_847       = 0x03,     /**< \brief ISO14443A */
    AM_IC_ISO14443B_106       = 0x04,     /**< \brief ISO14443B */
    AM_IC_ISO14443B_212       = 0x05,     /**< \brief ISO14443B */
    AM_IC_ISO14443B_424       = 0x06,     /**< \brief ISO14443B */
    AM_IC_ISO14443B_847       = 0x07,     /**< \brief ISO14443B */
} am_ic_iso14443_subprot_type_t;

/**
 * \brief ISO14443-3A卡激活的回应信息(不包括RATS)
 *        ISO14443协议卡片特有数据结构体定义
 */
typedef struct am_ic_a_reset_info {
    uint8_t     atq[2];         /**< \brief 请求应答 */
    uint8_t     sak;            /**< \brief 选择回应 */
    uint8_t     n_uid_len;      /**< \brief UID(卡唯一序列号)长度，4、7、10 */
    uint8_t     uid[10];        /**< \brief UID */
} am_ic_a_reset_info_t;

/**
 * \brief ISO14443-3B卡激活的回应信息
 *        ISO14443协议卡片特有数据结构体定义
 */
typedef struct am_ic_b_reset_info {
    uint8_t     pupi[4];        /**< \brief 伪唯一PICC 标识符 */
    uint8_t     app_data[4];    /**< \brief 应用数据 */
    uint8_t     prot_info[4];   /**< \brief 参数信息,基本为3字节,扩展的为4字节 */
} am_ic_b_reset_info_t;

/**
 * \brief ISO14443驱动函数结构体
 */
struct am_ic_iso14443_drv_funcs {

    /** \brief ISO14443-3 A卡请求 */
    int (*pfn_ic_a_request) (void     *p_drv,
                             uint8_t   req_code,
                             uint8_t  *p_atq,
                             uint32_t *p_nbytes);

    /** \brief ISO14443-3 A卡位方式防碰撞 */
    int (*pfn_ic_a_anticoll) (void     *p_drv,
                              uint8_t   mode,
                              uint8_t   level,
                              uint8_t  *p_uid,
                              uint8_t   bits,
                              uint32_t *p_nbytes);

    /** \brief ISO14443-3 A卡选择卡片 */
    int (*pfn_ic_a_select) (void          *p_drv,
                            uint8_t        sel_code,
                            const uint8_t *p_uid,
                            uint32_t       nbytes,
                            uint8_t       *p_sak);

    /** \brief ISO14443-3 A卡置为HALT状态 */
    int (*pfn_ic_a_halt) (void *p_drv);

    /** \brief ISO14443-3 A卡激活 */
    int (*pfn_ic_a_activate) (void                 *p_drv,
                              uint8_t               mode,
                              uint8_t               req_code,
                              am_ic_a_reset_info_t *p_reset_info);

    /** \brief ISO14443-3 B卡请求 */
    int (*pfn_ic_b_request) (void                 *p_drv,
                             uint8_t               req_code,
                             uint8_t               afi,
                             uint8_t               n,
                             am_ic_b_reset_info_t *p_reset_info);

    /** \brief ISO14443-3 B卡时隙标记防碰撞 */
    int (*pfn_ic_b_slot_mark) (void                 *p_drv,
                               uint8_t               n,
                               am_ic_b_reset_info_t *p_reset_info);

    /** \brief ISO14443-3 B卡修改属性并选择 */
    int (*pfn_ic_b_attrib_set) (void       *p_drv,
                                const void *p_pupi,
                                uint32_t    nbytes,
                                uint8_t     cid,
                                uint8_t     pro_type);

    /** \brief ISO14443-3 B卡置为HALT状态 */
    int (*pfn_ic_b_halt) (void *p_drv, uint8_t *p_pupi, uint32_t nbytes);

    /** \brief ISO14443-3 B卡激活 */
    int (*pfn_ic_b_activate) (void                 *p_drv,
                              uint8_t               req_code,
                              uint8_t               afi,
                              am_ic_b_reset_info_t *p_reset_info);

    /** \brief ISO14443-4 A卡请求应答以响应*/
    int (*pfn_ic_a_rats_get) (void     *p_drv,
                              uint8_t   cid,
                              void     *p_rats,
                              uint32_t  buf_size,
                              uint32_t *p_nbytes);

    /** \brief ISO14443-4 A卡协议和参数选择请求 */
    int (*pfn_ic_a_pps_set) (void *p_drv, uint8_t flags);

    /** \brief ISO14443-4 A卡取消选择 */
    int (*pfn_ic_a_deselect) (void *p_drv);

    /** \brief ISO14443 传输（T=CL）*/
    int (*pfn_ic_transfer) (void       *p_drv,
                            const void *p_txbuf,
                            uint32_t    n_tx,
                            void       *p_rxbuf,
                            uint32_t    buf_size,
                            uint32_t   *p_n_rx);
                           
};

/**
 * \brief ISO14443 服务结构体
 */
typedef struct am_ic_iso14443_serv {

    /** \brief ISO14443驱动函数结构体指针 */
    struct am_ic_iso14443_drv_funcs *p_funcs;

    /** \brief 用于驱动函数的第一个参数 */
    void                            *p_drv;
} am_ic_iso14443_serv_t;

/** \brief 声明ISO14443命令服务句柄 */
typedef am_ic_iso14443_serv_t *am_ic_iso14443_handle_t;

/**
 * \brief ISO14443-3 A卡请求
 *
 * \param[in]  handle   : ISO14443服务句柄
 * \param[in]  req_code : 请求代码
 *                       - AM_IC_ISO14443_3A_REQIDL 请求空闲卡
 *                       - AM_IC_ISO14443_3A_REQALL 请求所有卡
 * \param[out] p_atq    : 请求返回信息
 * \param[out] p_nbytes : 请求返回信息的字节数
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_ic_iso14443_a_request (am_ic_iso14443_handle_t  handle,
                              uint8_t                  req_code,
                              uint8_t                 *p_atq,
                              uint32_t                *p_nbytes)
{
    return handle->p_funcs->pfn_ic_a_request(handle->p_drv,
                                             req_code,
                                             p_atq,
                                             p_nbytes);
}

/**
 * \brief ISO14443-3 A卡位方式防碰撞
 *
 * \param[in] handle : ISO14443服务句柄
 * \param[in] mode   : 防碰撞模式
 *                       - 0x00 -- 执行防碰撞循环，
 *                       - 0x01 -- 只执行一次防碰撞
 * \param[in] level  : 防碰撞等级
 *                       - 0x93：第1级
 *                       - 0x95：第2级
 *                       - 0x97：第3级
 * \param[in, out] p_uid  : 输入已知的UID，返回完整UID
 * \param[in] bits        : 已知UID的位数
 * \param[out] p_nbytes   : 完整UID的字节数
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_ic_iso14443_a_anticoll (am_ic_iso14443_handle_t  handle,
                               uint8_t                  mode,
                               uint8_t                  level,
                               uint8_t                 *p_uid,
                               uint8_t                  bits,
                               uint32_t                *p_nbytes)
{
    return handle->p_funcs->pfn_ic_a_anticoll(handle->p_drv,
                                              mode,
                                              level,
                                              p_uid,
                                              bits,
                                              p_nbytes);
}

/**
 * \brief ISO14443-3 A卡选择卡片
 *
 * \param[in] handle    : ISO14443服务句柄
 * \param[in] level     : 防碰撞等级
 *                         - 0x93：第1级
 *                         - 0x95：第2级
 *                         - 0x97：第3级
 * \param[in]  p_uid    : 完整UID
 * \param[out] nbytes   : UID的字节数
 * \param[out] p_sak    : 选择应答(1字节)
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_ic_iso14443_a_select (am_ic_iso14443_handle_t  handle,
                             uint8_t                  level,
                             const uint8_t           *p_uid,
                             uint32_t                 nbytes,
                             uint8_t                 *p_sak)
{
    return handle->p_funcs->pfn_ic_a_select(handle->p_drv,
                                            level,
                                            p_uid,
                                            nbytes,
                                            p_sak);
}

/**
 * \brief ISO14443-3 A卡置为HALT状态
 *
 * \param[in] handle : ISO14443服务句柄
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_ic_iso14443_a_halt (am_ic_iso14443_handle_t handle)
{
    return handle->p_funcs->pfn_ic_a_halt(handle->p_drv);
}

/**
 * \brief ISO14443-3 A卡激活
 *
 * \param[in] handle        : ISO14443服务句柄
 * \param[in] mode          : 防碰撞模式
 *                             - 0x00 -- 执行防碰撞循环，
 *                             - 0x01 -- 只执行一次防碰撞
 * \param[in] req_code      : 请求代码
 *                             - 0x26 IDLE
 *                             - 0x52 ALL
 * \param[out] p_reset_info : ISO14443-3 A卡激活回应信息
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_ic_iso14443_a_activate (am_ic_iso14443_handle_t  handle,
                               uint8_t                  mode,
                               uint8_t                  req_code,
                               am_ic_a_reset_info_t    *p_reset_info)
{
    return handle->p_funcs->pfn_ic_a_activate(handle->p_drv,
                                              mode,
                                              req_code,
                                              p_reset_info);
}

/**
 * \brief ISO14443-3 B卡请求
 *
 * \param[in] handle        : ISO14443服务句柄
 * \param[in] req_code      : 请求代码
 *                             - 0x26 IDLE
 *                             - 0x52 ALL
 * \param[in] afi           : 应用标识符，0x00：全选
 * \param[in] n             : 时隙总数,取值范围0--4
 * \param[out] p_reset_info : ISO14443-3 B卡激活回应信息
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_ic_iso14443_b_request (am_ic_iso14443_handle_t  handle,
                              uint8_t                  req_code,
                              uint8_t                  afi,
                              uint8_t                  n,
                              am_ic_b_reset_info_t    *p_reset_info)
{
    return handle->p_funcs->pfn_ic_b_request(handle->p_drv,
                                             req_code,
                                             afi,
                                             n,
                                             p_reset_info);
}

/**
 * \brief ISO14443-3 B卡时隙标记防碰撞
 *
 * \param[in] handle       : ISO14443服务句柄
 * \param[in] n            : 时隙标记，取值范围2-16
 * \param[in] p_reset_info : ISO14443-3 B卡激活回应信息
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_ic_iso14443_b_slot_mark (am_ic_iso14443_handle_t  handle,
                                uint8_t                  n,
                                am_ic_b_reset_info_t    *p_reset_info)
{
    return handle->p_funcs->pfn_ic_b_slot_mark(handle->p_drv,
                                               n,
                                               p_reset_info);
}

/**
 * \brief ISO14443-3 B卡修改属性并选择
 *
 * \param[in] handle   : ISO14443服务句柄
 * \param[in] p_pupi   : IC唯一标识符
 * \param[in] nbytes   : 标识符字节数
 * \param[in] cid      : 取值范围为 0 - 14, 若不支持CID，则设置为0
 * \param[in] pro_type : 支持的协议，由请求回应中的ProtocolType指定
 *                        - pro_type.3      -- PCD 与PICC 是否继续通信
 *                            - 1              -- PCD 中止与PICC 继续通信
 *                            - 0              -- PCD 与PICC 继续通信
 *                        - pro_type.2:1    -- PICC EOF 和PCD SOF间的最小延迟
 *                            - 11             -- 10 etu + 512 / fs
 *                            - 10             -- 10 etu + 256 / fs
 *                            - 01             -- 10 etu + 128 / fs
 *                            - 00             -- 10 etu + 32 / fs
 *                        - pro_type.0      -- 是否遵循ISO14443-4
 *                            - 1              -- 遵循ISO14443-4;
 *                            - 0              -- 不遵循ISO14443-4. (二代证必须为1)
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_ic_iso14443_b_attrib_set (am_ic_iso14443_handle_t  handle,
                                 const void              *p_pupi,
                                 uint32_t                 nbytes,
                                 uint8_t                  cid,
                                 uint8_t                  pro_type)
{
    return handle->p_funcs->pfn_ic_b_attrib_set(handle->p_drv,
                                                p_pupi,
                                                nbytes,
                                                cid,
                                                pro_type);
}

/**
 * \brief ISO14443-3 B卡置为HALT状态
 *
 * \param[in] handle : ISO14443服务句柄
 * \param[in] p_pupi : IC唯一标识符
 * \param[in] nbytes : 标识符字节数
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_ic_iso14443_b_halt (am_ic_iso14443_handle_t  handle,
                           uint8_t                 *p_pupi,
                           uint32_t                 nbytes)
{
    return handle->p_funcs->pfn_ic_b_halt(handle->p_drv,
                                          p_pupi,
                                          nbytes);
}

/**
 * \brief ISO14443-3 B卡激活
 *
 * \param[in] handle        : ISO14443服务句柄
 * \param[in] req_code      : 请求代码
 *                           - 0x00 -- 空闲的卡
 *                           - 0x08 -- 所有的卡
 * \param[in] afi           : 应用标识符，0x00：全选
 * \param[out] p_reset_info : ISO14443-3 B卡激活回应信息
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_ic_iso14443_b_activate (am_ic_iso14443_handle_t  handle,
                               uint8_t                  req_code,
                               uint8_t                  afi,
                               am_ic_b_reset_info_t    *p_reset_info)
{
    return handle->p_funcs->pfn_ic_b_activate(handle->p_drv,
                                              req_code,
                                              afi,
                                              p_reset_info);
}

/**
 * \brief ISO14443-4 A卡请求应答以响应
 *
 * \param[in]  handle   : ISO14443服务句柄
 * \param[in]  cid      : 卡识别号，取值范围0--14
 * \param[out] p_rats   : 请求应答以响应信息
 * \param[out] p_nbytes : 请求应答以响应信息的字节数
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_ic_iso14443_a_rats_get (am_ic_iso14443_handle_t  handle,
                               uint8_t                  cid,
                               void                    *p_rats,
                               uint32_t                 buf_size,
                               uint32_t                *p_nbytes)
{
    return handle->p_funcs->pfn_ic_a_rats_get(handle->p_drv,
                                              cid,
                                              p_rats,
                                              buf_size,
                                              p_nbytes);
}

/**
 * \brief ISO14443-4 A卡协议和参数选择请求
 *
 * \param[in] handle : ISO14443服务句柄
 * \param[in] flags  : 位率编码
 *                      - b3:2 -- PCD  -> PICC的位率编码
 *                      - b1:0 -- PICC -> PCD 的位率编码
 *                          - (00)b -> 106Kb
 *                          - (01)b -> 212Kb
 *                          - (10)b -> 424Kb
 *                          - (11)b -> 847Kb
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_ic_iso14443_a_pps_set (am_ic_iso14443_handle_t handle,
                              uint8_t                 flags)
{
    return handle->p_funcs->pfn_ic_a_pps_set(handle->p_drv, flags);
}

/**
 * \brief ISO14443-4 A卡取消选择
 *
 * \param[in] handle : ISO14443服务句柄
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_ic_iso14443_a_deselect (am_ic_iso14443_handle_t handle)
{
    return handle->p_funcs->pfn_ic_a_deselect(handle->p_drv);
}

/**
 * \brief ISO14443 传输（T=CL）
 *
 * \param[in] handle  : ISO14443服务句柄
 * \param[in] p_txbuf : 传输的数据缓冲区
 * \param[in] n_tx    : 需要传输的数据字节数
 * \param[in] p_rxbuf : 接收数据的缓冲区
 * \param[in] n_rx    : 需要接收的字节数
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
int am_ic_iso14443_transfer (am_ic_iso14443_handle_t  handle,
                             const void              *p_txbuf,
                             uint32_t                 n_tx,
                             void                    *p_rxbuf,
                             uint32_t                 buf_size,
                             uint32_t                *p_n_rx)
{
    return handle->p_funcs->pfn_ic_transfer(handle,
                                            p_txbuf,
                                            n_tx,
                                            p_rxbuf,
                                            buf_size,
                                            p_n_rx);
}

/**
 * @} if_iso14443
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_IC_ISO14443_H */

/* end of file */
