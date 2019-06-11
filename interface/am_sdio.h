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
 * - 1.01 15-08-17  tee, modified some interface.
 * - 1.00 14-11-01  jon, first implementation.
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
 * \name SD 命令
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

#define AM_SD_VOLTAGE_WINDOW_SD            ((uint32_t)0x80100000)
#define AM_SD_HIGH_CAPACITY                ((uint32_t)0x40000000)
#define AM_SD_STD_CAPACITY                 ((uint32_t)0x00000000)
#define AM_SD_CHECK_PATTERN                ((uint32_t)0x000001AA)

#define AM_SDIO_M_WR            0x0000u    /**< \brief 写操作           */
#define AM_SDIO_M_RD            0x0002u    /**< \brief 读操作           */
//#define AM_SDIO_M_NOSTART       0x0004u    /**< \brief 无需重新启动     */
//#define AM_SDIO_M_REV_DIR_ADDR  0x0008u    /**< \brief 读写标志位反转   */
//#define AM_SDIO_M_RECV_LEN      0x0010u    /**< \brief 暂不支持         */

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

/** \brief SDIO总线宽度 */
typedef enum am_sdio_bus_width {
    AM_SDIO_BUS_WIDTH_1B  = 1, /**< \brief 1位数据线 */
    AM_SDIO_BUS_WIDTH_4B  = 2, /**< \brief 4位数据线 */
    AM_SDIO_BUS_WIDTH_8B  = 3, /**< \brief 8位数据线 */
} am_sdio_bus_width_t;

/** \brief command */
typedef struct am_sdio_trans{

    struct am_list_head  trans_node;

    uint32_t             cmd;        /**< \brief SDIO 命令*/
    uint32_t             arg;        /**< \brief 命令参数 */
    uint8_t              opt;        /**< \brief 传输操作 */
#define AM_SDIO_OW       0           /**< \brief 只写 */
#define AM_SDIO_WR       1           /**< \brief 先写后读 */

    void                *p_data;     /**< \brief 数据缓冲区 */
    uint32_t             blk_size;   /**< \brief 传输块大小 */
    uint32_t             nblock;     /**< \brief 传输块数量 */
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
    uint8_t              retries;    /**< \brief 超时重发次数 */
} am_sdio_trans_t;

/** \brief message */
typedef struct am_sdio_msg {
    struct am_list_head  msg_node;
    struct am_list_head  trans_list;
    int                  status;            /**< \brief 完成状态  */
    void                *p_arg;             /**< \brief 回调函数参数 */
    void (*pfn_complete) (void *p_arg);     /**< \brief 命令完成回调函数*/
} am_sdio_msg_t;

/**
 * \brief sdio驱动函数结构体
 */
struct am_sdio_drv_funcs {

    /** \brief 启动SDIO消息传输，完成后调用回调函数  */
    int (*pfn_sdio_msg_start)(void                  *p_drv,
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

/**< \brief 响应类型 */
#define AM_SDIO_RESPONSE_NO                    0
#define AM_SDIO_RESPONSE_SHORT                 1
#define AM_SDIO_RESPONSE_LONG                  2

/**
 * \brief SDIO 传输的命令结构体
 */
typedef struct am_sdio_cmd {
    uint32_t             cmd;        /**< \brief SDIO 命令*/
    uint32_t             arg;        /**< \brief 命令参数 */
    uint8_t              rsp_type;   /**< \brief 响应类型 */
    uint32_t             p_rsp[4];   /**< \brief 响应数据 */
} am_sdio_cmd_t;

/** \brief SDIO timeout obj */
typedef struct am_sdio_timeout_obj {
    am_tick_t   ticks;
    am_tick_t   timeout;
} am_sdio_timeout_obj_t;

am_static_inline
void am_adio_timeout_set (am_sdio_timeout_obj_t *p_t,
                          uint32_t              ms)
{
    p_t->timeout = am_ms_to_ticks(ms);
    p_t->ticks   = am_sys_tick_get();
}

/**
 * \brief check if timeout
 * \param[in] t     timeout obj
 */
am_static_inline
am_bool_t am_sdio_timeout (am_sdio_timeout_obj_t *p_t)
{
    return ((am_sys_tick_diff(p_t->ticks, am_sys_tick_get())) >= p_t->timeout);
}

/**
 * \brief SDIO传输命令结构体信息参数设置
 *
 * \param[in] p_cmd : 指向传输的命令结构体指针
 * \param[in] cmd   : SDIO 命令
 * \param[in] arg   : 命令参数
 *
 * \retval  AM_OK     : 传输结构体参数设置完成
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
void am_sdio_mkcmd (am_sdio_cmd_t *p_cmd,
                    uint32_t       cmd,
                    uint32_t       arg,
                    uint8_t        rsp_type)
{
    p_cmd->cmd = cmd;
    p_cmd->arg = arg;
    p_cmd->rsp_type = rsp_type;
    p_cmd->p_rsp[0] = 0;
    p_cmd->p_rsp[1] = 0;
    p_cmd->p_rsp[2] = 0;
    p_cmd->p_rsp[3] = 0;
}

/**
 * \brief SDIO传输命令结构体信息参数设置
 *
 * \param[in] p_trans : 指向传输结构体指针
 * \param[in] cmd       CMD 命令
 * \param[in] cmd_arg   命令参数
 * \param[in] opt       传输方式
 * \param[in] p_data    传输数据缓存
 * \param[in] blk_size  传输块大小
 * \param[in] nblk      传输块数量
 * \param[in] rsp_type  响应类型
 * \param[in] rsp_type  响应数据缓存
 * \param[in] retries   超时重发次数
 *
 * \retval  AM_OK     : 传输结构体参数设置完成
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
void am_sdio_mktrans (am_sdio_trans_t *p_trans,
                      uint8_t          cmd,
                      uint32_t         cmd_arg,
                      uint8_t          opt,
                      void            *p_data,
                      uint32_t         blk_size,
                      uint32_t         nblk,
                      uint8_t          rsp_type,
                      void            *p_rsp,
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

am_static_inline
void am_sdio_msg_init (am_sdio_msg_t *p_msg)
{
    memset(p_msg, 0, sizeof(*p_msg));

    AM_INIT_LIST_HEAD(&(p_msg->trans_list));

    p_msg->status = -AM_ENOTCONN;
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
 * \brief 开始处理一个消息
 *
 *     以异步的方式处理消息，若当前SDIO控制器空闲，则该消息会得到立即执行，
 * 若处理器不空闲，则会将该消息加入一个队列中排队等待，SDIO控制器将顺序处理
 * 队列中的消息。消息的处理状态和结果反映在\a p_msg->status。
 * 消息处理结束(成功、超时或出错)时，将会调用\a p_msg->pfn_complete 并传递
 * 参数\a p_msg->p_arg。
 *
 * \param[in]     handle : SDIO标准服务操作句柄
 * \param[in,out] p_msg  : 要处理的消息
 *
 * \retval  AM_OK     : 传输正常进行处理
 * \retval -AM_EINVAL : 输入参数错误
 *  传输过程以及结果状态，通过传输回调函数获取状态位
 *    \li  AM_OK  : 传输完成
 *    \li -AM_EIO : 传输错误
 */
am_static_inline
int am_sdio_msg_start (am_sdio_handle_t  handle,
                       am_sdio_msg_t    *p_msg)
{
    return handle->p_funcs->pfn_sdio_msg_start(handle->p_drv,
                                               p_msg);
}

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_SDIO_H */

/*end of file */
