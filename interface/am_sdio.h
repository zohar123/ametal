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
 * \brief SDIO标准接口
 *
 * \internal
 * \par Modification history
 * - 1.00 19-06-12  ipk, first implementation.
 * \endinternal
 */

#ifndef __AM_SDIO_H
#define __AM_SDIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_common.h"
#include "string.h"
#include "am_list.h"

/**
 * \addtogroup am_if_sdio
 * \copydoc am_sdio.h
 * @{
 */

/**
 * \name SDIO 命令
 * @{
 */
#define AM_SDIO_CMD0               0
#define AM_SDIO_CMD1               1
#define AM_SDIO_CMD2               2
#define AM_SDIO_CMD3               3
#define AM_SDIO_CMD4               4
#define AM_SDIO_CMD5               5
#define AM_SDIO_CMD6               6
#define AM_SDIO_CMD7               7
#define AM_SDIO_CMD8               8
#define AM_SDIO_CMD9               9
#define AM_SDIO_CMD10              10
#define AM_SDIO_CMD11              11
#define AM_SDIO_CMD12              12
#define AM_SDIO_CMD13              13
#define AM_SDIO_CMD14              14
#define AM_SDIO_CMD15              15
#define AM_SDIO_CMD16              16
#define AM_SDIO_CMD17              17
#define AM_SDIO_CMD18              18
#define AM_SDIO_CMD19              19
#define AM_SDIO_CMD20              20
#define AM_SDIO_CMD21              21
#define AM_SDIO_CMD22              22
#define AM_SDIO_CMD23              23
#define AM_SDIO_CMD24              24
#define AM_SDIO_CMD25              25
#define AM_SDIO_CMD26              26
#define AM_SDIO_CMD27              27
#define AM_SDIO_CMD28              28
#define AM_SDIO_CMD29              29
#define AM_SDIO_CMD31              31
#define AM_SDIO_CMD32              32
#define AM_SDIO_CMD33              33
#define AM_SDIO_CMD34              34
#define AM_SDIO_CMD35              35
#define AM_SDIO_CMD36              36
#define AM_SDIO_CMD37              37
#define AM_SDIO_CMD38              38
#define AM_SDIO_CMD39              39
#define AM_SDIO_CMD40              40
#define AM_SDIO_CMD41              41
#define AM_SDIO_CMD42              42
#define AM_SDIO_CMD43              43
#define AM_SDIO_CMD44              44
#define AM_SDIO_CMD45              45
#define AM_SDIO_CMD46              46
#define AM_SDIO_CMD47              47
#define AM_SDIO_CMD48              48
#define AM_SDIO_CMD49              49
#define AM_SDIO_CMD50              50
#define AM_SDIO_CMD51              51
#define AM_SDIO_CMD52              52
#define AM_SDIO_CMD53              53
#define AM_SDIO_CMD54              54
#define AM_SDIO_CMD55              55
#define AM_SDIO_CMD56              56
#define AM_SDIO_CMD57              57
#define AM_SDIO_CMD57              57
#define AM_SDIO_CMD58              58
#define AM_SDIO_CMD59              59

#define AM_SDIO_ACMD6              6
#define AM_SDIO_ACMD13             13
#define AM_SDIO_ACMD14             14
#define AM_SDIO_ACMD15             15
#define AM_SDIO_ACMD16             16
#define AM_SDIO_ACMD22             22
#define AM_SDIO_ACMD23             23
#define AM_SDIO_ACMD28             28
#define AM_SDIO_ACMD41             41
#define AM_SDIO_ACMD42             42
#define AM_SDIO_ACMD51             51
/** @} */

/**
 * \name SD mode R1 response
 * \anchor grp_awbl_sd_r1_rsp
 * @{
 */

/** \brief command argument out of range */
#define AM_SDIO_R1_OUT_OF_RANGE          (1 << 31)

/** \brief address error */
#define AM_SDIO_R1_ADDRESS_ERROR         (1 << 30)

/** \brief not support block length or transmit mismatch to the block length */
#define AM_SDIO_R1_BLOCK_LEN_ERROR       (1 << 29)

/** \brief erase sequence error */
#define AM_SDIO_R1_ERASE_SEQ_ERROR       (1 << 28)

/** \brief the block write or erase is invalid */
#define AM_SDIO_R1_ERASE_PARAM           (1 << 27)

/** \brief the block write is protected */
#define AM_SDIO_R1_WP_VIOLATION          (1 << 26)

/** \brief indicate whether the card is locked */
#define AM_SDIO_R1_CARD_IS_LOCKED        (1 << 25)

/** \brief indicate in the lock/unlock command，password verify error */
#define AM_SDIO_R1_LOCK_UNLOCK_FAILED    (1 << 24)

/** \brief CRC error */
#define AM_SDIO_R1_COM_CRC_ERROR         (1 << 23)

/** \brief ECC failed */
#define AM_SDIO_R1_CARD_ECC_FAILED       (1 << 21)

/** \brief card host error */
#define AM_SDIO_R1_CC_ERROR              (1 << 20)

/** \brief unknown error */
#define AM_SDIO_R1_ERROR                 (1 << 19)

#define AM_SDIO_R1_UNDERRUN              (1 << 18)
#define AM_SDIO_R1_OVERRUN               (1 << 17)

/** \brief CID/CSD overwrite */
#define AM_SDIO_R1_CID_CSD_OVERWRITE     (1 << 16)

/** \brief skip write protected area */
#define AM_SDIO_R1_WP_ERASE_SKIP         (1 << 15)

/** \brief indicate whether card ECC is forbid */
#define AM_SDIO_R1_CARD_ECC_DISABLED     (1 << 14)

#define AM_SDIO_R1_ERASE_RESET           (1 << 13)
#define AM_SDIO_R1_STATUS(x)             (x & 0xFFFFE000)

/** \brief current state */
#define AM_SDIO_R1_CURRENT_STATE(x)      ((x & 0x00001E00) >> 9)

/** \brief bus can transmit data */
#define AM_SDIO_R1_READY_FOR_DATA        (1 << 8)
#define AM_SDIO_R1_SWITCH_ERROR          (1 << 7)
#define AM_SDIO_R1_EXCEPTION_EVENT       (1 << 6)
/** \brief indicate whether the card into APP CMD mode */
#define AM_SDIO_R1_APP_CMD               (1 << 5)
#define AM_SDIO_R1_ALL_ERROR             0xF9F90000
/** @} */

/**
 * \name card status of R1 on SD mode
 * \anchor grp_awbl_sd_r1_state
 * @{
 */
#define AM_SDIO_R1_STATE_IDLE   0
#define AM_SDIO_R1_STATE_READY  1
#define AM_SDIO_R1_STATE_IDENT  2
#define AM_SDIO_R1_STATE_STBY   3
#define AM_SDIO_R1_STATE_TRAN   4
#define AM_SDIO_R1_STATE_DATA   5
#define AM_SDIO_R1_STATE_RCV    6
#define AM_SDIO_R1_STATE_PRG    7
#define AM_SDIO_R1_STATE_DIS    8
/** @} */

/**
 * \name card status of R1 on SPI mode
 * \anchor grp_awbl_spi_r1_state
 * @{
 */

/** \brief 指示卡是否处于空闲状态 */
#define AM_SDIO_SPI_R1_IN_IDLE_STATE       0x01

/** \brief 操作被复位 */
#define AM_SDIO_SPI_R1_ERASE_RESET         0x02

/** \brief 对于当前卡状态为非法命令  */
#define AM_SDIO_SPI_R1_ILLEGAL_CMD         0x04

/** \brief CRC校验失败 */
#define AM_SDIO_SPI_R1_COM_CRC_ERROR       0x08

/** \brief 擦除错误 */
#define AM_SDIO_SPI_R1_ERASE_SEQ_ERROR     0x10

/** \brief 地址错误，非对齐或数据长度不匹配 */
#define AM_SDIO_SPI_R1_ADDRESS_ERROR       0x20

/** \brief 参数错误，命令参数段不合法  */
#define AM_SDIO_SPI_R1_PARA_ERROR          0x40
#define AM_SDIO_SPI_R1_ALL_ERROR           0x7E
#define AM_SDIO_SPI_R1_MASK                0x7F
/** @} */

/**
 * \name SPI模式 R2状态
 * \anchor grp_awbl_spi_r2_state
 * @{
 */

/** \brief 卡被锁定 */
#define AM_SDIO_SPI_R2_CARD_LOCKED         0x01

/** \brief 忽略写保护,擦除 */
#define AM_SDIO_SPI_R2_WP_ERASE_SKIP       0x02

/** \brief LOCK、UNLOCK命令失败 */
#define AM_SDIO_SPI_R2_LOCK_UNLOCK_FAIL    0x02

/** \brief 为止错误 */
#define AM_SDIO_SPI_R2_ERROR               0x04

/** \brief 卡内部控制器错误 */
#define AM_SDIO_SPI_R2_CC_ERROR            0x08

/** \brief ECC错误 */
#define AM_SDIO_SPI_R2_CARD_ECC_ERROR      0x10

/** \brief 对写保护区域进行了写操作 */
#define AM_SDIO_SPI_R2_WP_VIOLATION        0x20

/** \brief 擦除参数无效，对无效区域擦除 */
#define AM_SDIO_SPI_R2_ERASE_PARAM         0x40

/** \brief    */
#define AM_SDIO_SPI_R2_OUT_OF_RANGE        0x80
#define AM_SDIO_SPI_R2_CSD_OVERWRITE       0x80
/** @} */

/** \brief command */
typedef struct am_sdio_trans{

    struct am_list_head  trans_node; /**< \brief 传输链表节点 */

    uint8_t              opt;        /**< \brief 传输操作 */
#define AM_SDIO_OW           0       /**< \brief 只写 */
#define AM_SDIO_WR           1       /**< \brief 先写后读 */

    uint32_t             cmd;        /**< \brief SDIO 命令*/
    uint32_t             arg;        /**< \brief 命令参数 */
    uint8_t              rsp_type;   /**< \brief 命令响应类型 */
#define AM_SDIO_RSP_NONE      0
#define AM_SDIO_RSP_R1        1
#define AM_SDIO_RSP_R1B       2
#define AM_SDIO_RSP_R2        3
#define AM_SDIO_RSP_R3        4
#define AM_SDIO_RSP_R4        5
#define AM_SDIO_RSP_R5        6
#define AM_SDIO_RSP_R6        7
#define AM_SDIO_RSP_R7        8

    uint32_t            *p_rsp;      /**< \brief 响应数据 */

    void                *p_data;     /**< \brief 数据缓冲区 */
    uint32_t             blk_size;   /**< \brief 传输块大小 */
    uint32_t             nblock;     /**< \brief 传输块数量 */
    uint8_t              retries;    /**< \brief 超时重发次数 */
} am_sdio_trans_t;

/** \brief message */
typedef struct am_sdio_msg {
    struct am_list_head  trans_list;        /**< \brief 消息传输链表 */
    int                  status;            /**< \brief 完成状态  */
    void                *p_arg;             /**< \brief 回调函数参数 */
    void (*pfn_complete) (void *p_arg);     /**< \brief 命令完成回调函数*/
} am_sdio_msg_t;

struct am_sdio_device;

/**
 * \brief SDIO驱动函数结构体
 */
struct am_sdio_drv_funcs {

    /** \brief 设置SDIO设备   */
    int (*pfn_sdio_setup) (void *p_drv, struct am_sdio_device *p_dev);

    /** \brief 启动SDIO消息传输，完成后调用回调函数  */
    int (*pfn_sdio_msg_start)(void                  *p_drv,
                              struct am_sdio_device *p_dev,
                              struct am_sdio_msg    *p_msg);
};

/**
 * \brief SDIO 标准服务结构体
 */
typedef struct am_sdio_serv {
    struct am_sdio_drv_funcs   *p_funcs;    /**< \brief SDIO  sdio驱动函数     */
    void                       *p_drv;      /**< \brief SDIO  sdio驱动函数参数 */
} am_sdio_serv_t;

/** \brief SDIO 标准服务操作句柄定义 */
typedef am_sdio_serv_t *am_sdio_handle_t;

/** \brief SDIO host 设备  */
typedef struct am_sdio_device {

    am_sdio_handle_t        handle;
    uint8_t                 mode;           /**< \brief 工作模式  */
#define AM_SDIO_SPI_M               0       /**< \brief SDIO SPI模式 */
#define AM_SDIO_SD_1B_M             1       /**< \brief SDIO SD 1线模式 */
#define AM_SDIO_SD_4B_M             2       /**< \brief SDIO SD 4线模式 */
#define AM_SDIO_SD_8B_M             3       /**< \brief SDIO SD 8线模式 */

    uint32_t                speed;          /**< \brief SDIO速率  */
    am_bool_t               crc_enable;     /**< \brief CRC使能标志  */

} am_sdio_device_t;

/**< \brief 响应类型 */
#define AM_SDIO_RESPONSE_NO         0
#define AM_SDIO_RESPONSE_SHORT      1
#define AM_SDIO_RESPONSE_LONG       2

/** \brief SDIO timeout obj */
typedef struct am_sdio_timeout_obj {
    am_tick_t   ticks;
    am_tick_t   timeout;
} am_sdio_timeout_obj_t;

/**
 * \brief      超时时间设置
 *
 * \param[in]  p_t   :  timeout obj
 * \param[in]  ms    :  超时时间（单位：ms）
 *
 * \return 无
 */
am_static_inline
void am_sdio_timeout_set (am_sdio_timeout_obj_t *p_t,
                          uint32_t              ms)
{
    p_t->timeout = am_ms_to_ticks(ms);
    p_t->ticks   = am_sys_tick_get();
}

/**
 * \brief 检查是否超时
 *
 * \param[in] t     timeout obj
 *
 * \retval 返回值为1则超时，否则未超时
 */
am_static_inline
am_bool_t am_sdio_timeout (am_sdio_timeout_obj_t *p_t)
{
    return ((am_sys_tick_diff(p_t->ticks, am_sys_tick_get())) >= p_t->timeout);
}

/**
 * \brief 设置SDIO设备结构体参数
 *
 * \param[in] p_dev         : SDIO从机设备描述符指针
 * \param[in] handle        : SDIO标准服务操作句柄
 * \param[in] mode          : 设备模式标志，见“SDIO模式标志”
 * \param[in] speed         : SDIO设备的速度
 * \param[in] crc_enable    : CRC使能标志
 *
 * \return 无
 *
 * \par 范例
 * 见 am_sdio_setup()
 */
am_static_inline
void am_sdio_mkdev (am_sdio_device_t *p_dev,
                    am_sdio_handle_t  handle,
                    uint8_t           mode,
                    uint32_t          speed,
                    am_bool_t         crc_enable)
{
    p_dev->handle       = handle;
    p_dev->mode         = mode;
    p_dev->speed        = speed;
    p_dev->crc_enable   = crc_enable;
}

/**
 * \brief 设置SDIO HOST设备
 *
 * \attention 在SDIO HOST设备被使用前，必须先调用本函数进行设置，且如本函数返回
 *            错误，则一定不能再使用此SDIO HOST设备
 *
 * \param[in] p_dev : SDIO HOST设备
 *
 * \retval   AM_OK           : 设置成功
 * \retval  -AM_ENOTSUP      : 配置失败，不支持的位宽、模式等
 *
 * \par 示例
 * \code
 * am_sdio_device_t sdio_dev;     // 设备描述结构
 *
 * // 初始化设备描述结构
 * am_sdio_mkdev(&sdio_dev,
 *               handle,          // SDIO标准服务handle
 *               AM_SDIO_SD_4B_M, // SDIO四线模式
 *               1000000,         // 速度 1000000 Hz
 *               AM_TRUE),        // CRC使能
 *
 * // 设置设备
 * am_sdio_setup(&sdio_dev);
 *
 * \endcode
 */
am_static_inline
int am_sdio_setup (am_sdio_device_t *p_dev)
{
    return p_dev->handle->p_funcs->pfn_sdio_setup(p_dev->handle->p_drv, p_dev);
}

/**
 * \brief SDIO传输命令结构体信息参数设置
 *
 * \param[in] p_trans : 指向传输结构体指针
 * \param[in] opt       传输方式
 * \param[in] cmd       CMD 命令
 * \param[in] cmd_arg   命令参数
 * \param[in] rsp_type  响应类型
 * \param[in] rsp_type  响应数据缓存
 * \param[in] p_data    传输数据缓存
 * \param[in] blk_size  传输块大小
 * \param[in] nblk      传输块数量
 * \param[in] retries   超时重发次数
 *
 * \retval  AM_OK     : 传输结构体参数设置完成
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
void am_sdio_mktrans (am_sdio_trans_t *p_trans,
                      uint8_t          opt,
                      uint8_t          cmd,
                      uint32_t         cmd_arg,
                      uint8_t          rsp_type,
                      void            *p_rsp,
                      void            *p_data,
                      uint32_t         blk_size,
                      uint32_t         nblk,
                      uint8_t          retries)
{
    p_trans->cmd      = cmd;
    p_trans->arg      = cmd_arg;
    p_trans->opt      = opt;
    p_trans->p_data   = p_data;
    p_trans->blk_size = blk_size;
    p_trans->nblock   = nblk;
    p_trans->rsp_type = rsp_type;
    p_trans->p_rsp    = p_rsp;
    p_trans->retries  = retries;
}

/**
 * \brief 初始化消息
 *
 * \param[in] p_msg        : SDIO消息描述符指针
 * \param[in] pfn_complete : 传输完成回调函数
 * \param[in] p_arg        : 传递给回调函数的参数
 *
 * \return 无
 */
am_static_inline
void am_sdio_msg_init (am_sdio_msg_t *p_msg,
                       am_pfnvoid_t   pfn_complete,
                       void          *p_arg)
{
    memset(p_msg, 0, sizeof(*p_msg));

    AM_INIT_LIST_HEAD(&(p_msg->trans_list));

    p_msg->status        = -AM_ENOTCONN;
    p_msg->pfn_complete  = pfn_complete;
    p_msg->p_arg         = p_arg;
}

/**
 * \brief 将传输添加到消息末尾
 * \param p_msg     message object
 * \param p_cmd     command to be added
 */
am_static_inline
void am_sdio_trans_add_tail (am_sdio_msg_t   *p_msg,
                             am_sdio_trans_t *p_trans)
{
    am_list_add_tail(&p_trans->trans_node, &p_msg->trans_list);
}

/**
 * \brief 将传输从消息中删除
 * \param[in] p_trans : 待加入删除的传输
 * \return 无
 * \note 删除前，务必确保该传输确实已经加入了消息中
 */
am_static_inline
void am_sdio_trans_del (am_sdio_trans_t *p_trans)
{
    am_list_del(&p_trans->trans_node);
}

/**
 * \brief 从message列表表头取出一条 transfer
 * \attention 调用此函数必须锁定控制器
 *
 * \retval  p_msg
 */
am_static_inline
am_sdio_trans_t *am_sdio_msg_out (am_sdio_msg_t *p_msg)
{
    am_sdio_trans_t *p_trans = NULL;

    if (!(am_list_empty(&p_msg->trans_list))) {
        p_trans = am_list_entry(p_msg->trans_list.next,
                                am_sdio_trans_t,
                                trans_node);
        am_list_del(p_msg->trans_list.next);
    }

    return p_trans;
}

/**
 * \brief SDIO传输函数
 *
 * \param[in] p_dev   : SDIO HOST设备
 * \param[in] p_msg   : 传输的消息
 *
 * \retval AM_OK            : 传输成功
 * \retval -AM_ENOMEM       : 内存空间不足
 * \retval -AM_ECANCELED    : 操作被取消
 * \retval -AM_EIO          : SDIO总线I/O错误
 * \retval -AM_EBUSY        : SDIO HOST忙，正在处理其他msg
 * \retval -AM_EINVAL       : 参数错误
 * \retval -AM_ETIME        : HOST处理超时
 * \retval -AM_ENOTSUP      : HOST不支持该功能
 */
am_static_inline
int am_sdio_msg_start (am_sdio_device_t *p_dev,
                       am_sdio_msg_t    *p_msg)
{
    return p_dev->handle->p_funcs->pfn_sdio_msg_start(p_dev->handle->p_drv,
                                                      p_dev,
                                                      p_msg);
}

/**
 * \brief SDIO写命令
 *
 * \param[in]  p_dev        : SDIO设备
 * \param[in]  cmd          : 需要写入的命令
 * \param[in]  cmd_arg      : 命令参数
 * \param[in]  rsp_type     : 响应类型
 * \param[in]  p_rsp        : 响应数据缓存，不需要时可传入NULL
 *
 * \retval AM_OK            : 传输成功
 * \retval -AM_ENOMEM       : 内存空间不足
 * \retval -AM_ECANCELED    : 操作被取消
 * \retval -AM_EIO          : SDIO总线I/O错误
 * \retval -AM_EBUSY        : SDIO HOST忙，正在处理其他msg
 * \retval -AM_EINVAL       : 参数错误
 * \retval -AM_ETIME        : HOST处理超时
 * \retval -AM_ENOTSUP      : HOST不支持该功能
 */
int am_sdio_cmd_write (am_sdio_device_t *p_dev,
                       uint8_t           cmd,
                       uint32_t          cmd_arg,
                       uint8_t           rsp_type,
                       void             *p_rsp);

/**
 * \brief SDIO先写命令再读数据
 *
 * \param[in]  p_dev        : SDIO设备
 * \param[in]  cmd          : 需要写的命令
 * \param[in]  cmd_arg      : 命令参数
 * \param[in]  rsp_type     : 命令响应类型
 * \param[in]  p_rsp        : 响应缓存
 * \param[in]  p_data       : 读取的数据缓存
 * \param[in]  blk_size     : 需要读取数据的块大小
 * \param[in]  nblks        : 需要读取数据的块个数
 *
 * \retval AM_OK            : 传输成功
 * \retval -AM_ENOMEM       : 内存空间不足
 * \retval -AM_ECANCELED    : 操作被取消
 * \retval -AM_EIO          : SDIO总线I/O错误
 * \retval -AM_EBUSY        : SDIO HOST忙，正在处理其他msg
 * \retval -AM_EINVAL       : 参数错误
 * \retval -AM_ETIME        : HOST处理超时
 * \retval -AM_ENOTSUP      : HOST不支持该功能
 */
int am_sdio_write_then_read (am_sdio_device_t *p_dev,
                             uint8_t           cmd,
                             uint32_t          cmd_arg,
                             uint8_t           rsp_type,
                             void             *p_rsp,
                             void             *p_data,
                             uint32_t          blk_size,
                             uint32_t          nblks);

/**
 * \brief SDIO先写命令再写数据
 *
 * \param[in]  p_dev        : SDIO设备
 * \param[in]  cmd          : 需要写的命令
 * \param[in]  cmd_arg      : 命令参数
 * \param[in]  rsp_type     : 命令响应类型
 * \param[in]  p_rsp        : 响应缓存
 * \param[in]  p_data       : 写入的数据缓存
 * \param[in]  blk_size     : 需要写入数据的块大小
 * \param[in]  nblks        : 需要写入数据的块个数
 *
 * \retval AM_OK            : 传输成功
 * \retval -AM_ENOMEM       : 内存空间不足
 * \retval -AM_ECANCELED    : 操作被取消
 * \retval -AM_EIO          : SDIO总线I/O错误
 * \retval -AM_EBUSY        : SDIO HOST忙，正在处理其他msg
 * \retval -AM_EINVAL       : 参数错误
 * \retval -AM_ETIME        : HOST处理超时
 * \retval -AM_ENOTSUP      : HOST不支持该功能
 */
int am_sdio_write_then_write (am_sdio_device_t *p_dev,
                              uint8_t           cmd,
                              uint32_t          cmd_arg,
                              uint8_t           rsp_type,
                              uint32_t         *p_rsp,
                              uint8_t          *p_data,
                              uint32_t          blk_size,
                              uint32_t          nblks);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_SDIO_H */

/*end of file */
