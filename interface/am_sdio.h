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
#include "am_list.h"

/**
 * \addtogroup am_if_sdio
 * \copydoc am_sdio.h
 * @{
 */

/**
 * \name SDIO模式标志，用于am_sdio_mkdev()函数的mode参数
 * @{
 */

/**
  * @brief SDIO Commands  Index
  */
#define AM_SD_CMD_GO_IDLE_STATE           ((uint8_t)0)
#define AM_SD_CMD_SEND_OP_COND            ((uint8_t)1)
#define AM_SD_CMD_ALL_SEND_CID            ((uint8_t)2)
#define AM_SD_CMD_SET_REL_ADDR            ((uint8_t)3) /*!< SDIO_SEND_REL_ADDR for SD Card */
#define AM_SD_CMD_SET_DSR                 ((uint8_t)4)
#define AM_SD_CMD_SDIO_SEN_OP_COND        ((uint8_t)5)
#define AM_SD_CMD_HS_SWITCH               ((uint8_t)6)
#define AM_SD_CMD_SEL_DESEL_CARD          ((uint8_t)7)
#define AM_SD_CMD_HS_SEND_EXT_CSD         ((uint8_t)8)
#define AM_SD_CMD_SEND_CSD                ((uint8_t)9)
#define AM_SD_CMD_SEND_CID                ((uint8_t)10)
#define AM_SD_CMD_READ_DAT_UNTIL_STOP     ((uint8_t)11) /*!< SD Card doesn't support it */
#define AM_SD_CMD_STOP_TRANSMISSION       ((uint8_t)12)
#define AM_SD_CMD_SEND_STATUS             ((uint8_t)13)
#define AM_SD_CMD_HS_BUSTEST_READ         ((uint8_t)14)
#define AM_SD_CMD_GO_INACTIVE_STATE       ((uint8_t)15)
#define AM_SD_CMD_SET_BLOCKLEN            ((uint8_t)16)
#define AM_SD_CMD_READ_SINGLE_BLOCK       ((uint8_t)17)
#define AM_SD_CMD_READ_MULT_BLOCK         ((uint8_t)18)
#define AM_SD_CMD_HS_BUSTEST_WRITE        ((uint8_t)19)
#define AM_SD_CMD_WRITE_DAT_UNTIL_STOP    ((uint8_t)20) /*!< SD Card doesn't support it */
#define AM_SD_CMD_SET_BLOCK_COUNT         ((uint8_t)23) /*!< SD Card doesn't support it */
#define AM_SD_CMD_WRITE_SINGLE_BLOCK      ((uint8_t)24)
#define AM_SD_CMD_WRITE_MULT_BLOCK        ((uint8_t)25)
#define AM_SD_CMD_PROG_CID                ((uint8_t)26) /*!< reserved for manufacturers */
#define AM_SD_CMD_PROG_CSD                ((uint8_t)27)
#define AM_SD_CMD_SET_WRITE_PROT          ((uint8_t)28)
#define AM_SD_CMD_CLR_WRITE_PROT          ((uint8_t)29)
#define AM_SD_CMD_SEND_WRITE_PROT         ((uint8_t)30)
#define AM_SD_CMD_SD_ERASE_GRP_START      ((uint8_t)32) /*!< To set the address of the first write
                                                      block to be erased. (For SD card only) */
#define AM_SD_CMD_SD_ERASE_GRP_END        ((uint8_t)33) /*!< To set the address of the last write block of the
                                                      continuous range to be erased. (For SD card only) */
#define AM_SD_CMD_ERASE_GRP_START         ((uint8_t)35) /*!< To set the address of the first write block to be erased.
                                                      (For MMC card only spec 3.31) */

#define AM_SD_CMD_ERASE_GRP_END           ((uint8_t)36) /*!< To set the address of the last write block of the
                                                      continuous range to be erased. (For MMC card only spec 3.31) */

#define AM_SD_CMD_ERASE                   ((uint8_t)38)
#define AM_SD_CMD_FAST_IO                 ((uint8_t)39) /*!< SD Card doesn't support it */
#define AM_SD_CMD_GO_IRQ_STATE            ((uint8_t)40) /*!< SD Card doesn't support it */
#define AM_SD_CMD_LOCK_UNLOCK             ((uint8_t)42)
#define AM_SD_CMD_APP_CMD                 ((uint8_t)55)
#define AM_SD_CMD_GEN_CMD                 ((uint8_t)56)
#define AM_SD_CMD_NO_CMD                  ((uint8_t)64)


#define AM_SD_VOLTAGE_WINDOW_SD            ((uint32_t)0x80100000)
#define AM_SD_HIGH_CAPACITY                ((uint32_t)0x40000000)
#define AM_SD_STD_CAPACITY                 ((uint32_t)0x00000000)
#define AM_SD_CHECK_PATTERN                ((uint32_t)0x000001AA)

/**
 * \brief SDIO 模式设置
 */
#define AM_SDIO_SDIO_M              0       /**< \brief SDIO SDIO模式 */
#define AM_SDIO_SD_1B_M             1       /**< \brief SDIO SD 1线模式 */
#define AM_SDIO_SD_4B_M             2       /**< \brief SDIO SD 4线模式 */
#define AM_SDIO_SD_8B_M             3       /**< \brief SDIO SD 8线模式 */


/**
 * \brief 命令响应类型
 */
#define AWBL_SDIO_RSP_NONE      0
#define AWBL_SDIO_RSP_R1        1
#define AWBL_SDIO_RSP_R1B       2
#define AWBL_SDIO_RSP_R2        3
#define AWBL_SDIO_RSP_R3        4
#define AWBL_SDIO_RSP_R4        5
#define AWBL_SDIO_RSP_R5        6
#define AWBL_SDIO_RSP_R6        7
#define AWBL_SDIO_RSP_R7        8

/**
 * \brief 读写方向标志
 */
#define AWBL_SDIO_NO_DATA       0
#define AWBL_SDIO_RD_DATA       1
#define AWBL_SDIO_WR_DATA       2


#define AM_SDIO_M_WR            0x0000u    /**< \brief 写操作           */
#define AM_SDIO_M_RD            0x0002u    /**< \brief 读操作           */
//#define AM_SDIO_M_NOSTART       0x0004u    /**< \brief 无需重新启动     */
//#define AM_SDIO_M_REV_DIR_ADDR  0x0008u    /**< \brief 读写标志位反转   */
//#define AM_SDIO_M_RECV_LEN      0x0010u    /**< \brief 暂不支持         */

/**
 * \brief SDIO控制器信息结构体
 */

struct am_sdio_transfer;      /**< \brief 声明SDIO传输的结构体类型     */
struct am_sdio_message;       /**< \brief 声明SDIO消息的结构体类型     */
struct am_sdio_cmd;           /**< \brief 声明SDIO消息的结构体类型     */

/**
 * \brief sdio驱动函数结构体
 */
struct am_sdio_drv_funcs {

//    /** \brief 获取SDIO控制器信息 */
    int (*pfn_sdio_msg_send) (void *p_drv, struct am_sdio_message *p_buf, uint16_t len);

    //    /** \brief 获取SDIO控制器信息 */
    int (*pfn_sdio_msg_recv) (void *p_drv, struct am_sdio_message *p_msg, uint16_t len);

    /** \brief 设置SDIO从机设备   */
    int (*pfn_sdio_send_cmd) (void *p_drv,  struct am_sdio_cmd *p_cmd);

    /** \brief 启动SDIO消息传输，完成后调用回调函数  */
    int (*pfn_sdio_msg_start)(void                   *p_drv);
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

/** \brief SDIO从设备描述结构 */
typedef struct am_sdio_device {

    am_sdio_handle_t  handle;       /**< \brief 该设备关联的SDIO标准服务handle */

    /** \brief 从设备的SDIO模式标志，请参考“SDIO模式标志”*/
    uint16_t         mode;

    /* \brief 设备支持的最高速度 */
    uint32_t         max_speed_hz;

    /** \brief 从机设备特性*/
    uint16_t         dev_flags;
} am_sdio_device_t;

/**< \brief 响应类型 */
#define SDIO_RESPONSE_NO                    0
#define SDIO_RESPONSE_SHORT                 1
#define SDIO_RESPONSE_LONG                  2

/**
 * \brief SDIO 传输的命令结构体
 */
typedef struct am_sdio_cmd {
    uint32_t             cmd;        /**< \brief SDIO 命令*/
    uint32_t             arg;        /**< \brief 命令参数 */
    uint8_t              rsp_type;   /**< \brief 响应类型 */
    uint32_t             p_rsp[4];   /**< \brief 响应数据 */
} am_sdio_cmd_t;

/**
 * \brief SDIO 传输结构体 (推荐使用 am_sdio_mktrans() 设置本数据结构)
 */
typedef struct am_sdio_message {
//    am_sdio_cmd_t       *p_cmd;    /**< \brief 命令的组成区 */
    uint8_t             *p_data;     /**< \brief 数据缓冲区 */
    uint32_t             blk_size;   /**< \brief 传输块大小 */
    uint32_t             nblock;     /**< \brief 传输块数量 */
//    uint8_t              opt;      /**< \brief 传输操作 */
} am_sdio_message_t;

///**
// * \brief SDIO 消息 (推荐使用 am_sdio_mkmsg() 设置本数据结构)
// */
//typedef struct am_sdio_message{
//    am_sdio_transfer_t *p_transfers;  /**< \brief 组成消息的传输              */
//    uint16_t            trans_num;    /**< \brief 请求处理的传输个数          */
//    uint16_t            done_num;     /**< \brief 成功处理的传输个数          */
//    am_pfnvoid_t        pfn_complete; /**< \brief 传输完成回调函数            */
//    void               *p_arg;        /**< \brief 传递给 pfn_complete 的参数  */
//    int                 status;       /**< \brief 消息的状态                  */
//    void               *ctlrdata[2];  /**< \brief 控制器使用                  */
//}am_sdio_message_t;


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
}

/**
 * \brief SDIO从机设备描述结构体参数设置
 *
 * \param[in] p_dev       : 指向从机设备描述结构体的指针
 * \param[in] handle      : 与从设备关联的I2C标准服务操作句柄
 * \param[in] mode        : SDIO模式标志
 * \param[in] max_speed_hz: 设备支持的最大速度
 * \param[in] dev_flags   : 从机设备特性，见“SDIO从设备属性标志”
 *
 * \return 无
 */
am_static_inline
void am_sdio_mkdev (am_sdio_device_t *p_dev,
                    am_sdio_handle_t  handle,
                    uint16_t          mode,
                    uint32_t          max_speed_hz,
                    uint16_t          dev_flags)
{
    p_dev->handle        = handle;
    p_dev->mode          = mode;
    p_dev->max_speed_hz  = max_speed_hz;
    p_dev->dev_flags     = dev_flags;
}

/**
 * \brief SDIO传输结构体信息参数设置
 *
 * \param[in] p_trans  : 指向传输结构体的指针
 * \param[in] p_data   : 数据缓冲区
 * \param[in] p_cmd    : 传输的命令结构体指针
 * \param[in] blk_size : 传输块的大小
 * \param[in] nblock   : 传输块的个数
 * \param[in] opt      : 传输的类型
 *
 * \retval  AM_OK     : 传输结构体参数设置完成
 * \retval -AM_EINVAL : 参数错误
 */

//am_static_inline
//void am_sdio_mktrans (am_sdio_transfer_t *p_trans,
//                      uint8_t            *p_data,
//                      am_sdio_cmd_t      *p_cmd,
//                      uint32_t            blk_size,
//                      uint32_t            nblock,
//                      uint8_t             opt)
//{
//    p_trans->p_data   = p_data;
//    p_trans->blk_size = blk_size;
//    p_trans->nblock   = nblock;
//    p_trans->opt      = opt;
//    p_trans->p_cmd    = p_cmd;
//}

/**
 * \brief 初始化消息
 *
 * \param[in] p_msg        : SDIO消息描述符指针
 * \param[in] p_transfers  : 组成消息的传输
 * \param[in] trans_num    : 请求处理的传输个数
 * \param[in] pfn_complete : 传输完成回调函数
 * \param[in] p_arg        : 传递给回调函数的参数
 *
 * \return 无
 */
//am_static_inline
//void am_sdio_mkmsg (am_sdio_message_t  *p_msg,
//                   am_sdio_transfer_t  *p_transfers,
//                   uint16_t             trans_num,
//                   am_pfnvoid_t         pfn_complete,
//                   void                *p_arg)
//{
//
//    p_msg->p_transfers  = p_transfers;  /**< \brief 组成消息的传输              */
//
//    p_msg->trans_num    = trans_num;    /**< \brief 请求处理的传输个数          */
//    p_msg->done_num     = 0;            /**< \brief 成功处理的传输个数          */
//    p_msg->pfn_complete = pfn_complete; /**< \brief 传输完成回调函数            */
//    p_msg->p_arg        = p_arg;        /**< \brief 传递给 pfn_complete 的参数  */
//    p_msg->status       = 0;            /**< \brief 消息的状态                  */
//}

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
int am_sdio_msg_start (am_sdio_handle_t    handle)
{
    return handle->p_funcs->pfn_sdio_msg_start(handle->p_drv);
}

int (*pfn_sdio_send_cmd) (void *p_drv,  am_sdio_cmd_t *p_cmd);

am_static_inline
int am_sdio_send_cmd (am_sdio_handle_t    handle,
		              am_sdio_cmd_t      *p_cmd)
{
    return handle->p_funcs->pfn_sdio_send_cmd(handle->p_drv,
    		                                  p_cmd);
}

am_static_inline
int am_sdio_msg_send (am_sdio_handle_t handle,  struct am_sdio_message  *p_msg, uint16_t len)
{
	 return handle->p_funcs->pfn_sdio_msg_send(handle->p_drv, p_msg, len);
}

am_static_inline
int am_sdio_msg_recv (am_sdio_handle_t handle,   struct am_sdio_message  *p_msg, uint16_t len)
{
	return handle->p_funcs->pfn_sdio_msg_recv(handle->p_drv, p_msg, len);
}



/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_SDIO_H */

/*end of file */
