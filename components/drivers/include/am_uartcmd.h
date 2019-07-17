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
 * - 1.00 15-11-12  win, first implementation.
 * \endinternal
 */
 
#ifndef __AM_UARTCMD_H
#define __AM_UARTCMD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_list.h"
#include "am_common.h"
#include "am_uartcmd_flash.h"

struct uartcmd_cmd_mifare_var;

/**
 * \brief 新帧格式结构体
 */
struct am_uartcmd_frame {
    uint8_t   local_addr;   /**< \brief 本机地址，同I2C */
    uint8_t   slot_index;   /**< \brief IC卡槽索引编号 */
    uint8_t   smc_seq;      /**< \brief SMC序列号 */
    uint8_t   cmd_class;    /**< \brief 命令类型 */
};

/**
 * \brief UARTCMD 设备信息结构体
 */
typedef struct am_uartcmd_devinfo {
    uint32_t  int_pin;         /**< \brief 中断引脚号 */
//    uint32_t  sbr_pin;         /**< \brief SBR引脚号  */
//    uint32_t  dbr_pin;         /**< \brief DBR引脚号  */
    uint32_t  rxd_pin;        /**< \brief ADR1引脚号 */
    uint32_t  txd_pin;        /**< \brief ADR0引脚号 */
//    uint32_t  sadr_pin;        /**< \brief SADR引脚号 */
//    uint32_t  restore1;        /**< \brief 回复出厂设置控制引脚1 */
//    uint32_t  restore2;        /**< \brief 回复出厂设置控制引脚2 */                         
    uint32_t  led_num;         /**< \brief LED引脚号  */

    uint32_t  frame_max;       /**< \brief 帧的最大长度 */
    void     *p_frame_buf;     /**< \brief 存放帧的缓冲区（用户传入） */

    /** \brief 存放在FLASH中的固件配置信息，用于还原出厂配置 */
    const uartcmd_flash_devinfo_t *p_flash_devinfo;

    /** \brief 存放用户修改后配置 */
    const uartcmd_flash_devinfo_t *p_flash_usr_devinfo;

    void     *p_iap_buf;       /**< \brief 模块内部IAP操作使用的缓冲区，大小为512B */
    uint32_t  iap_buf_size;    /**< \brief 模块内部IAP操作缓冲区大小 */

    /** \brief 退出自动检测模式的命令，其信息长度的限制值，最大不能超过该值 */
    uint32_t  info_len_limit;

} am_uartcmd_devinfo_t;

/**
 * \brief UARTCMD 设备结构体
 */
typedef struct am_uartcmd_dev {
    
    /** \brief UARTCMD 设备信息结构体 */
    const am_uartcmd_devinfo_t *p_devinfo;
    
    uint8_t               status;           /**< \brief 当前UARTCMD 工作状态 */
    uint8_t               frame_type;       /**< \brief 标明当前帧的类型 */
    uint8_t               antenna;          /**< \brief 天线驱动模式 */
    uint8_t               comm_mode;        /**< \brief 当前通讯模式 */
    uint8_t               work_mode;        /**< \brief 工作模式 */
    int                   result;           /**< \brief 命令执行结果 */
    am_bool_t             wait_send;        /**< \brief 自动检测到了数据，等待用户读取 */
    am_bool_t             update_addr;      /**< \breif 是否更新地址 */
    am_bool_t             update_speed;     /**< \brief 是否更新速率 */
   
    uint32_t              rx_count;         /**< \brief 接收计数 */
    uint32_t              tx_count;         /**< \brief 发送计数 */
    uint32_t              temp_count;       /**< \brief 用于配合发送计数 */

    struct am_list_head   frame_head;       /**< \brief 帧类型的头结点 */
    struct am_list_head   cmd_head;         /**< \brief 命令类型的头结点 */
    struct am_list_head   serial_head;      /**< \brief 串行设备的头结点 */

    struct uartcmd_cmd_mifare_var *p_var;
    struct am_uartcmd_frame        frame;

} am_uartcmd_dev_t;

/** \brief 声明串口命令服务句柄 */
typedef am_uartcmd_dev_t *am_uartcmd_handle_t;

/**
 * \brief UARTCMD 初始化函数
 *
 * \param[in] p_dev      : UARTCMD 设备结构体
 * \param[in] p_devinfo  : UARTCMD 设备信息结构体 
 * \param[in] comm_mode  : 通信模式
 * \param[in] work_mode  : 工作模式
 * \param[in] frame_type : 帧类型
 *
 * \return UARTCMD服务句柄，如果为NULL，则初始化失败
 */
am_uartcmd_handle_t am_uartcmd_init (am_uartcmd_dev_t           *p_dev,
                                     const am_uartcmd_devinfo_t *p_devinfo,
                                     uint8_t                     comm_mode,
                                     uint8_t                     work_mode,
                                     uint8_t                     frame_type);
                                     
/**
 * \brief 从UARTCMD 缓冲区中读数据
 * 
 * \param[in] handle   : UARTCMD 服务句柄
 * \param[in] p_rx_buf : 读出的数据存放的缓冲区
 * \param[in] nbytes   : 读出的字节数
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误  
 */
int am_uartcmd_buf_read (am_uartcmd_handle_t handle, void *p_rx_buf, uint32_t nbytes);

/**
 * \brief 往UARTCMD 缓冲区中写数据
 * 
 * \param[in] handle   : UARTCMD 服务句柄
 * \param[in] p_tx_buf : 写入的数据存放的缓冲区
 * \param[in] nbytes   : 写入的字节数
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误  
 */
int am_uartcmd_buf_write (am_uartcmd_handle_t handle, void *p_tx_buf, uint32_t nbytes);

/**
 * \brief 通知 UARTCMD 模块数据已全部写入缓冲区
 *
 * \param[in] handle : UARTCMD服务句柄
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
int am_uartcmd_recv_finished (am_uartcmd_handle_t handle);

/**
 * \brief UARTCMD 执行函数
 *        UARTCMD 模块的逻辑由该函数实现
 *
 * \param[in] handle : UARTCMD 服务句柄
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误 
 */
int am_uartcmd_execute (am_uartcmd_handle_t handle);

#ifdef __cplusplus
}
#endif

#endif /* __AM_UARTCMD_H */

/* end of file */
