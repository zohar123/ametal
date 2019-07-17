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
 * \brief UARTCMD底层驱动程序
 * 
 * \internal
 * \par Modification history
 * - 1.01 16-08-03  sky, modified.
 * - 1.00 15-09-03  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_UARTCMD_SEND_H
#define __AM_UARTCMD_SEND_H
 
#include "am_types.h"
#include "am_uart_rngbuf.h"

/**
 * @addtogroup am_if_uartcmd
 * @copydoc am_uartcmd.h
 * @{
 */
 
/** 
 * \name UARTCMD的通信模式与主从模式
 * @{
 */

#define AM_UARTCMD_MODE_AUTO_CHECK         0         /**< \brief 自动检测模式  */
#define AM_UARTCMD_MODE_UART               2         /**< \brief 串口模式      */
#define AM_UARTCMD_MODE_MASTER       (1 << 4)        /**< \brief 主机模式（保留功能） */
#define AM_UARTCMD_MODE_SLAVE        (0 << 4)        /**< \brief 从机模式（默认） */

/** @}*/

/**
 * \name UARTCMD的连接方式
 * @{
 */

#define AM_UARTCMD_CMNT_UART        0            /**< \brief UART连接 */
#define AM_UARTCMD_CMNT_I2C         1            /**< \brief I2C连接 */

/** @} */

 /** 
 * \name UARTCMD的帧格式
 * @{
 */

#define AM_UARTCMD_FRAME_FMT_OLD  0             /**< \brief  旧帧格式      */
#define AM_UARTCMD_FRAME_FMT_NEW  1             /**< \brief  新帧格式      */

 /** @} */
 
/**
 * \name 支持的波特率
 * @{
 */

#define  AM_UARTCMD_BAUDRATE_9600      9600     /**< \brief 波特率 9600   */
#define  AM_UARTCMD_BAUDRATE_19200     19200    /**< \brief 波特率 19200  */
#define  AM_UARTCMD_BAUDRATE_28800     28800    /**< \brief 波特率 28800  */
#define  AM_UARTCMD_BAUDRATE_38400     38400    /**< \brief 波特率 38400  */
#define  AM_UARTCMD_BAUDRATE_57600     57600    /**< \brief 波特率 57600  */
#define  AM_UARTCMD_BAUDRATE_115200    115200   /**< \brief 波特率 115200 */
#define  AM_UARTCMD_BAUDRATE_172800    172800   /**< \brief 波特率 172800 */
#define  AM_UARTCMD_BAUDRATE_230400    230400   /**< \brief 波特率 230400 */

/** @}*/

/**
 * \brief UARTCMD自动检测信息结构体
 */
typedef struct am_uartcmd_auto_detect_info {

    /**
     * \brief 检测模式，配置检测动作，是否产生中断、继续检测、完成动作等 #AM_UARTCMD_MIFARE_CARD_AD_HALT
     */
    uint8_t        ad_mode;

    /**
     * \brief 天线驱动模式，交替驱动、仅其中一种驱动、同时驱动 #AM_UARTCMD_MIFARE_CARD_TX_TX1
     */
    uint8_t        tx_mode;

    /**
     * \brief 请求模式，请求空闲卡或所有卡 #AM_UARTCMD_MIFARE_CARD_REQ_IDLE
     */
    uint8_t        req_mode;

    /**
     * \brief 验证模式，E2秘钥验证、直接验证或不验证 #AM_UARTCMD_MIFARE_CARD_AUTH_E2
     */
    uint8_t        auth_mode;           /**< \brief 验证模式 */

    /**
     * \brief 密钥类型，#AM_UARTCMD_IC_KEY_TYPE_A 或 #AM_UARTCMD_IC_KEY_TYPE_B
     *        如果auth_mode选择的是AM_UARTCMD_MIFARE_CARD_AUTH_NO，
     *        则key_type可以为任意值
     */
    uint8_t        key_type;

    /** \brief 密钥 */
    uint8_t        key[16];

    /** \brief 密钥长度 */
    uint8_t        key_len;

    /**
     * \brief 验证块号
     *          S50（ 0～63）
     *          S70（ 0～255）
     *          PLUS CPU 2K（ 0～127）
     *          PLUS CPU 4K（ 0～255）
     */
    uint8_t        nblock;

} am_uartcmd_auto_detect_info_t;

/**
 * \brief 标准的UARTCMD设备服务结构体定义
 */
typedef struct am_uartcmd_serv {

    void          *p_cookie;            /**< \brief 参数，仅供内部使用 */

    uint32_t       timeout_ms;          /**< \brief 命令等待时间(毫秒) */

    uint8_t        frame_seq;           /**< \brief 记录包号 */
    uint8_t        work_mode;           /**< \brief 当前使用的工作模式(主从模式) */
    uint8_t        cur_addr;            /**< \brief 当前传输的地址(7bit) */

    uint8_t        auto_detect;         /**< \brief 自动检测模式 */
    uint8_t        auto_detect_read;    /**< \brief 自动检测读标志 */
    am_pfnvoid_t   pfn_callback;        /**< \brief 回调函数 */
    void          *p_arg;               /**< \brief 回调函数的参数 */

    am_pfnvoid_t                  pfn_cmd_timeout_set;          /**< \brief 指令超时时间设置 */
    am_pfnvoid_t                  pfn_frame_proc;               /**< \brief 帧处理函数 */
    am_pfnvoid_t                  pfn_auto_detect_trigger_set;  /**< \brief 自动检测触发设置 */
    am_uartcmd_auto_detect_info_t *p_auto_detect_info;           /**< \brief 自动检测信息 */

} am_uartcmd_serv_t;

/**
 * \brief UARTCMD标准服务handle定义
 */
typedef am_uartcmd_serv_t *am_uartcmd_send_handle_t;

/** 
 * \brief UARTCMD（串口通信）设备定义
 */
typedef struct am_uartcmd_uart_dev {

    /** \brief UARTCMD 标准服务 */
    am_uartcmd_serv_t         uartcmd_serv;

    /** \brief 串口（带缓冲区）设备           */
    am_uart_rngbuf_dev_t     uart_ringbuf_dev;
    
    /** \brief 串口（带缓冲区）标准服务handle */
    am_uart_rngbuf_handle_t  uart_ringbuf_handle;

    /** \brief 帧处理函数 */
    am_pfnvoid_t   pfn_frame_proc;

} am_uartcmd_uart_dev_t;

/** 
 * \brief UARTCMD (UART通信模式) 信息定义
 */
typedef struct am_uartcmd_uart_dev_info {

    /** \brief 用于串口接收的缓冲区，建议大小在64以上     */
    uint8_t     *p_uart_rxbuf;

    /** \brief 用于串口发送的缓冲区，建议大小在64以上     */
    uint8_t     *p_uart_txbuf;

    /** \brief 用于串口接收的缓冲区大小 */
    uint16_t     rxbuf_size;

    /** \brief 用于串口发送的缓冲区大小 */
    uint16_t     txbuf_size;

    /** \brief 与UARTCMD通信的波特率(UART模式下使用) */
    uint32_t     baudrate;

    /** \brief 初始化时使用的模式 #AM_UARTCMD_MODE_AUTO_CHECK */
    uint8_t      now_mode;

    /** \brief UARTCMD的从机地址(7bit) */
    uint8_t      local_addr;

} am_uartcmd_uart_dev_info_t;

/**
 * \brief 初始化UARTCMD（使用UART新帧通信模式），获取到操作UARTCMD的标准服务handle
 *
 * 务必根据实际硬件使用何种通信连接方式选择，如使用UART，则使用该函数初始化
 * 以获得handle，否则，请使用am_uartcmd_i2c_init()函数获得handle。
 *
 * \param[in] p_dev        : 指向UARTCMD设备的指针
 * \param[in] uart_handle  : 与UARTCMD关联的UART，后续使用该handle进行UART通信
 * \param[in] p_uart_info  : UART通信模式相关信息
 *
 * \return 操作 UARTCMD 的操作句柄，如果初始化失败，则返回NULL
 */
am_uartcmd_send_handle_t am_uartcmd_uart_new_init (am_uartcmd_uart_dev_t            *p_dev,
                                            am_uart_handle_t                 uart_handle,
                                            const am_uartcmd_uart_dev_info_t *p_uart_info);



/**
 * \brief 读UARTCMD的产品、版本信息，如：“UARTCMDSP/T V1.00”
 *
 * \param[in] handle     : UARTCMD的操作句柄
 * \param[in] p_info     : 用于装载获取信息的缓冲区，大小应为 20 字节
 *
 * \retval AM_OK      : 读取信息成功
 * \retval -AM_EINVAL : 读取信息失败，参数错误
 * \retval -AM_EIO    : 读取信息失败，数据通信出错
 */ 
int am_uartcmd_info_get (am_uartcmd_send_handle_t handle,
                        uint8_t           *p_info);

/**
 * \brief 配置IC卡接口，执行成功后，启动IC卡接口，启动后默认支持TypeA卡
 *
 * \param[in] handle     : UARTCMD的操作句柄
 *
 * \retval AM_OK      : 配置成功
 * \retval -AM_EINVAL : 配置失败，参数错误
 * \retval -AM_EIO    : 配置失败，数据通信出错
 */ 
int am_uartcmd_ic_port_config (am_uartcmd_send_handle_t handle);

/**
 * \brief 关闭IC卡接口
 *
 * \param[in] handle     : UARTCMD的操作句柄
 *
 * \retval AM_OK      : 关闭成功
 * \retval -AM_EINVAL : 关闭失败，参数错误
 * \retval -AM_EIO    : 关闭失败，数据通信出错
 */ 
int am_uartcmd_ic_port_close (am_uartcmd_send_handle_t handle);
                            
/**
 * \name IC卡接口的协议（工作模式）
 * @{
 */
#define  AM_UARTCMD_IC_ISOTYPE_A      0x00    /**< \brief ISO14443-3A */
#define  AM_UARTCMD_IC_ISOTYPE_B      0x04    /**< \brief ISO14443-3B */
/** @}*/

/**
 * \brief 设置IC卡接口协议（工作模式）TypeA 或者 TypeB
 *
 * \param[in] handle     : UARTCMD的操作句柄
 * \param[in] isotype    : IC卡接口的协议类型，使用下列宏 ：
 *                          - #AM_UARTCMD_IC_ISOTYPE_A
 *                          - #AM_UARTCMD_IC_ISOTYPE_B
 *
 * \retval AM_OK      : 设置成功
 * \retval -AM_EINVAL : 设置失败，参数错误
 * \retval -AM_EIO    : 设置失败，数据通信出错
 */ 
int am_uartcmd_ic_isotype_set (am_uartcmd_send_handle_t handle,
                              uint8_t            isotype);
                             
/**
 * \name IC卡密匙类型
 * @{
 */

#define  AM_UARTCMD_IC_KEY_TYPE_A      0x60    /**< \brief 类型 A  */
#define  AM_UARTCMD_IC_KEY_TYPE_B      0x61    /**< \brief 类型 B  */

/** @}*/

/**
 * \brief 装载IC卡密钥
 *
 * 将输入的密钥保存在模块内部，模块掉电后该密钥不丢失，UARTCMDSP/T模块共能保存
 * A密钥16组、B密钥16组。
 *
 * \param[in] handle     : UARTCMD的操作句柄
 * \param[in] key_type   : 密钥类型，可以使用下列值：
 *                          - #AM_UARTCMD_IC_KEY_TYPE_A
 *                          - #AM_UARTCMD_IC_KEY_TYPE_B
 * \param[in] key_sec    : 密钥保存的区号，共可保存16组密钥，区号：0 ~ 15
 * \param[in] p_key      : 密钥缓冲区
 * \param[in] key_length : 密钥的长度，只能为6（6字节密钥）或16（16字节密钥）
 *
 * \retval AM_OK      : 装载密钥成功
 * \retval -AM_EINVAL : 装载失败，参数错误
 * \retval -AM_EIO    : 装载失败，数据通信出错
 */ 
int am_uartcmd_ic_key_load (am_uartcmd_send_handle_t  handle,
                           uint8_t             key_type,
                           uint8_t             key_sec,
                           uint8_t            *p_key,
                           uint8_t             key_length);

/**
 * \brief 设置IC卡接口的寄存器值
 *
 * 用于设置模块上读写卡芯片内部的寄存器值，通过该命令，可以实现很多现有命令
 * 不能完成的工作。
 *
 * \param[in] handle     : UARTCMD的操作句柄
 * \param[in] reg_addr   : 寄存器地址，范围：0x00 ~ 0x3F
 * \param[in] reg_val    : 设置的寄存器值
 *
 * \retval AM_OK      : 设置成功
 * \retval -AM_EINVAL : 设置失败，参数错误
 * \retval -AM_EIO    : 设置失败，数据通信出错
 */ 
int am_uartcmd_ic_reg_set (am_uartcmd_send_handle_t handle,
                          uint8_t            reg_addr,
                          uint8_t            reg_val);

/**
 * \brief 获取IC卡接口的寄存器值
 *
 * 该命令用于设置模块上读写卡芯片内部的寄存器值，
 * 通过该命令，可以实现很多现有命令不能完成的工作。
 *
 * \param[in]  handle     : UARTCMD的操作句柄
 * \param[in]  reg_addr   : 寄存器地址，范围：0x00 ~ 0x3F
 * \param[out] p_val      : 获取寄存器值的指针
 *
 * \retval AM_OK      : 获取成功
 * \retval -AM_EINVAL : 获取失败，参数错误
 * \retval -AM_EIO    : 获取失败，数据通信出错
 */ 
int am_uartcmd_ic_reg_get (am_uartcmd_send_handle_t  handle,
                          uint8_t             reg_addr,
                          uint8_t            *p_val);

/**
 * \brief 设置波特率
 *
 * 用于在UART通信过程中改变通信的波特率，掉电后该设置值保留。
 *
 * \param[in] handle        : UARTCMD的操作句柄
 * \param[in] baudrate_flag : 波特率标志，并不支持所有的波特率，应使用宏
 *                            AM_UARTCMD_BAUDRATE_* ，如设置波特率为115200，
 *                            则使用宏 #AM_UARTCMD_BAUDRATE_115200
 *
 * \retval AM_OK      : 设置成功
 * \retval -AM_EINVAL : 设置失败，参数错误
 * \retval -AM_EIO    : 设置失败，数据通信出错
 */ 
int am_uartcmd_baudrate_set (am_uartcmd_send_handle_t handle,
                            uint32_t           baudrate_flag);

/**
 * \name 天线驱动方式
 * @{
 */

#define  AM_UARTCMD_ANT_TX1_ONLY      1    /**< \brief 仅TX1驱动天线         */
#define  AM_UARTCMD_ANT_TX2_ONLY      2    /**< \brief 仅TX2驱动天线         */
#define  AM_UARTCMD_ANT_TX1_AND_TX2   3    /**< \brief TX1、TX2同时驱动天线  */
#define  AM_UARTCMD_ANT_NONE          0    /**< \brief 同时关闭TX1、TX2      */

/** @}*/
      
/**
 * \brief 设置天线驱动方式，可以任意打开、关闭某个天线驱动引脚
 *
 * \param[in] handle       : UARTCMD的操作句柄
 * \param[in] antmode_flag : 使用宏 AM_UARTCMD_ANT_*，如#AM_UARTCMD_ANT_TX1_ONLY
 *
 * \retval AM_OK      : 设置成功
 * \retval -AM_EINVAL : 设置失败，参数错误
 * \retval -AM_EIO    : 设置失败，数据通信出错
 */ 
int am_uartcmd_ant_mode_set (am_uartcmd_send_handle_t handle,
                            uint8_t            antmode_flag);

/**
 * \brief 设置帧格式，用于切换新、旧帧格式，设置成功后掉电不丢失
 *
 * \param[in] handle     : UARTCMD的操作句柄
 * \param[in] flag       : 帧格式宏标志，可使用下列宏：
 *                           - #AM_UARTCMD_FRAME_FMT_NEW
 *                           - #AM_UARTCMD_FRAME_FMT_OLD
 *
 * \retval AM_OK      : 设置成功
 * \retval -AM_EINVAL : 设置失败，参数错误
 * \retval -AM_EIO    : 设置失败，数据通信出错
 *
 * \note 帧切换成功后需要重新调用目标帧格式的初始化函数。
 */ 
int am_uartcmd_frame_fmt_set (am_uartcmd_send_handle_t handle,
                             uint8_t            flag);

/**
 * \brief 设置设备工作模式和从机地址
 *
 * \param[in] handle     : UARTCMD的操作句柄
 * \param[in] mode       : 模块的工作模式，可以使用下列宏值：
 *                         - #AM_UARTCMD_MODE_MASTER      主机模式
 *                         - #AM_UARTCMD_MODE_SLAVE       从机模式 （默认）
 *                         - #AM_UARTCMD_MODE_AUTO_CHECK  自动侦测模式
 *                         - #AM_UARTCMD_MODE_I2C         I2C模式
 *                         - #AM_UARTCMD_MODE_UART        UART模式
 *
 * \param[in] new_addr   : 设置的UARTCMD从机地址，最后一位为0，有效值0x02 ~ 0xFE
 *
 * \retval AM_OK      : 设置成功,设置成功后，后续应该使用新的地址通信
 * \retval -AM_EINVAL : 设置失败，参数错误
 * \retval -AM_EIO    : 设置失败，数据通信出错
 */ 
int am_uartcmd_mode_addr_set (am_uartcmd_send_handle_t handle,
                             uint8_t            mode,
                             uint8_t            new_addr);
      
/**
 * \brief 获取设备工作模式和从机地址
 *
 * \param[in]  handle     : UARTCMD的操作句柄
 * \param[out] p_mode     : 用于获取模块工作模式的指针，获取的值各位含义如下：
 *                          - bit7 ~ 4：0,从机模式; 1,自动检测卡片模式（主机模式）
 *                          - bit3 ~ 0：0,自动侦测模式; 1,I2C通信模式; 2,UART模式 
 * \param[out] p_addr     : 用于获取当前的UARTCMD的从机地址的指针
 *
 * \retval AM_OK      : 获取成功
 * \retval -AM_EINVAL : 获取失败，参数错误
 * \retval -AM_EIO    : 获取失败，数据通信出错
 */ 
int am_uartcmd_mode_addr_get (am_uartcmd_send_handle_t  handle,
                             uint8_t            *p_mode,
                             uint8_t            *p_addr);
                             
/**
 * \brief 装载用户密钥，模块里面提供了2个16字节的存储空间用于保存用户密钥。
 *
 * \param[in] handle     : UARTCMD的操作句柄
 * \param[in] key_sec    : 密钥保存的区号，共可保存2组用户密钥，区号：0 ~ 1
 * \param[in] p_key      : 密钥缓冲区，密钥的长度固定为16字节
 *
 * \retval AM_OK      : 装载密钥成功
 * \retval -AM_EINVAL : 装载失败，参数错误
 * \retval -AM_EIO    : 装载失败，数据通信出错
 */ 
int am_uartcmd_user_key_load (am_uartcmd_send_handle_t handle,
                             uint8_t            key_sec,
                             uint8_t           *p_key);
                              
/**
 * \brief 写EEPROM，模块内部拥有一个256Byte的EEPROM
 *
 * \param[in] handle      : UARTCMD的操作句柄
 * \param[in] eeprom_addr : EEPROM的片内地址，0 ~ 255
 * \param[in] p_buf       : 写入数据的缓冲区
 * \param[in] nbytes      : 写入数据的字节数，最大247字节
 *
 * \retval AM_OK      : 写入数据成功
 * \retval -AM_EINVAL : 写入失败，参数错误
 * \retval -AM_EIO    : 写入失败，数据通信出错
 */ 
int am_uartcmd_eeprom_write (am_uartcmd_send_handle_t handle,
                            uint8_t            eeprom_addr,
                            uint8_t           *p_buf,
                            uint8_t            nbytes);

/**
 * \brief 读EEPROM，模块内部拥有一个256Byte的EEPROM
 *
 * \param[in]  handle      : UARTCMD的操作句柄
 * \param[in]  eeprom_addr : EEPROM的片内地址，0 ~ 255
 * \param[out] p_buf       : 读出数据的缓冲区
 * \param[in]  nbytes      : 读出数据的字节数，最大249字节
 *
 * \retval AM_OK      : 读取数据成功
 * \retval -AM_EINVAL : 读取失败，参数错误
 * \retval -AM_EIO    : 读取失败，数据通信出错
 */ 
int am_uartcmd_eeprom_read (am_uartcmd_send_handle_t handle,
                           uint8_t            eeprom_addr,
                           uint8_t           *p_buf,
                           uint8_t            nbytes);

/**
 * \brief 切换天线
 *
 * \param[in]  handle      : UARTCMD的操作句柄
 * \param[in]  ante_num    : 天线编号
 *
 * \retval AM_OK      : 读取数据成功
 * \retval -AM_EINVAL : 读取失败，参数错误
 */
int am_uartcmd_ante_select (am_uartcmd_send_handle_t  handle,
                           uint8_t             ante_num);

/**
 * \name Mifare卡请求模式
 * @{
 */

#define AM_UARTCMD_MIFARE_CARD_REQ_IDLE 0x26 /**< \brief IDLE模式,请求空闲的卡 */
#define AM_UARTCMD_MIFARE_CARD_REQ_ALL  0x52 /**< \brief ALL模式,请求所有的卡  */

/** @}*/ 

/**
 * \brief Mifare卡请求操作
 *
 *  该函数作为卡的请求操作，只要符合ISO14443A标准的卡都应能发出响应。
 *
 * \param[in] handle      : UARTCMD的操作句柄
 * \param[in] req_mode    : 请求模式，可使用下列宏：
 *                           - #AM_UARTCMD_MIFARE_CARD_REQ_IDLE
 *                           - #AM_UARTCMD_MIFARE_CARD_REQ_ALL
 * \param[in] p_atq       : 获取请求应答信息（ATQ）的指针，其值为16位，不同卡
 *                          类型返回的信息不同,常见类型返回的ATQ如下：
 *
 *            Mifare1 S50    | Mifare1 S70 | Mifare1 Light | Mifare0 UltraLight 
 *       --------------------|-------------|---------------|-------------------
 *              0x0004       |    0x0002   |    0x0010     |      0x0044 
 *       ----------------------------------------------------------------------
 *          Mifare3 DESFire  |   SHC1101   |    SHC1102    |      11RF32
 *       --------------------|-------------|---------------|-------------------
 *               0x0344      |    0x0004   |    0x3300     |      0x0004
 *
 * \retval AM_OK      : 请求成功
 * \retval -AM_EINVAL : 请求失败，参数错误
 * \retval -AM_EIO    : 请求失败，数据通信出错
 *
 * \note 卡进入天线后，从射频场中获取能量，从而得电复位，复位后卡处于IDLE模式，
 * 用两种请求模式的任一种请求时，此时的卡均能响应；若对某一张卡成功进行了挂起
 * 操作（Halt命令或DeSelect命令），则进入了Halt模式，此时的卡只响应ALL（0x52）
 * 模式的请求，除非将卡离开天线感应区后再进入。
 */ 
int am_uartcmd_mifare_card_request (am_uartcmd_send_handle_t handle,
                                   uint8_t            req_mode,
                                   uint16_t          *p_atq);

/**
 * \name Mifare卡防碰撞等级
 * @{
 */
#define AM_UARTCMD_MIFARE_CARD_ANTICOLL_1  0x93  /**< \brief 第一级防碰撞  */
#define AM_UARTCMD_MIFARE_CARD_ANTICOLL_2  0x95  /**< \brief 第二级防碰撞  */
#define AM_UARTCMD_MIFARE_CARD_ANTICOLL_3  0x97  /**< \brief 第三级防碰撞  */
/** @}*/ 

/**
 * \brief Mifare卡的防碰撞操作
 *
 *  需要成功执行一次请求命令，并返回请求成功，才能进行防碰撞操作，否则返回错误。
 *
 * \param[in] handle         : UARTCMD的操作句柄
 * \param[in] anticoll_level : 防碰撞等级，可使用下列宏：
 *                             - #AM_UARTCMD_MIFARE_CARD_ANTICOLL_1
 *                             - #AM_UARTCMD_MIFARE_CARD_ANTICOLL_2
 *                             - #AM_UARTCMD_MIFARE_CARD_ANTICOLL_3
 *
 * \param[in] know_uid : 已知的卡序列号
 * \param[in] nbit_cnt : 位计数，表明已知的序列号uid的位数。其值小于32。
 *                       - 若nbit_cnt值为0,则表明没有已知序列号，序列号的所有位
 *                         都要从本函数获得。
 *                       - 若nbit_cnt不为0，则序列号中有已知的序列号的值，表示
 *                         序列号的前nbit_cnt位为已知序列号中所示前nbit_cnt位的
 *                         值，其余位需要本函数获得。
 * \param[in] *p_uid   : 防碰撞后获取到的卡序列号
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 失败，参数错误
 * \retval -AM_EIO    : 失败，数据通信出错
 *
 * \note  符合ISO14443A标准卡的序列号都是全球唯一的，正是这种唯一性，才能实现防
 * 碰撞的算法逻辑，若有若干张卡同时在天线感应区内则这个函数能够找到一张序列号较
 * 大的卡来操作。
 */ 
int am_uartcmd_mifare_card_anticoll (am_uartcmd_send_handle_t handle,
                                    uint8_t            anticoll_level,
                                    uint32_t           know_uid,
                                    uint8_t            nbit_cnt,
                                    uint32_t          *p_uid);
                       
/**
 * \brief Mifare卡的选择操作
 *
 *  需要成功执行一次防碰撞命令，并返回成功，才能进行卡选择操作，否则返回错误。
 *
 * \param[in] handle         : UARTCMD的操作句柄
 * \param[in] anticoll_level : 防碰撞等级，可使用下列宏：
 *                             - #AM_UARTCMD_MIFARE_CARD_ANTICOLL_1
 *                             - #AM_UARTCMD_MIFARE_CARD_ANTICOLL_2
 *                             - #AM_UARTCMD_MIFARE_CARD_ANTICOLL_3
 * \param[in] uid   : 前一个防碰撞函数获取的UID
 * \param[in] p_sak : 返回的信息，若bit2为1，则表明UID不完整
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 失败，参数错误
 * \retval -AM_EIO    : 失败，数据通信出错
 *
 * \note  卡的序列号长度有三种：4字节、7字节和10字节。 4字节的只要用一级选择即可
 * 得到完整的序列号，如Mifare1 S50/S70等；7字节的要用二级选择才能得到完整的序列
 * 号，前一级所得到的序列号的最低字节为级联标志0x88，在序列号内只有后3字节可用，
 * 后一级选择能得到4字节序列号，两者按顺序连接即为7字节序列号，如UltraLight
 * 和DesFire等；10字节的以此类推，但至今还未发现此类卡。
 */ 
int am_uartcmd_mifare_card_select (am_uartcmd_send_handle_t handle,
                                  uint8_t            anticoll_level,
                                  uint32_t           uid,
                                  uint8_t           *p_sak);

/**
 * \brief Mifare卡的挂起操作，使所选择的卡进入HALT状态
 *
 *  在HALT状态下，卡将不响应读卡器发出的IDLE模式的请求，除非将卡复位或离开天线感
 *  应区后再进入。但它会响应读卡器发出的ALL请求。
 *
 * \param[in] handle         : UARTCMD的操作句柄
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 失败，参数错误
 * \retval -AM_EIO    : 失败，数据通信出错
 */ 
int am_uartcmd_mifare_card_halt (am_uartcmd_send_handle_t handle);

                               
/**
 * \brief Mifare卡E2密钥验证
 *
 *     用模块内部已存入的密钥与卡的密钥进行验证，使用该命令前应先用“装载IC卡密钥”
 * 函数把密钥成功载入模块内。另外，需要验证的卡的扇区号不必与模块内密钥区号相等。
 *
 * \param[in] handle     : UARTCMD的操作句柄
 * \param[in] key_type   : 密钥类型，可以使用下列值：
 *                          - #AM_UARTCMD_IC_KEY_TYPE_A
 *                          - #AM_UARTCMD_IC_KEY_TYPE_B
 * \param[in] p_uid      : 卡序列号，4字节
 * \param[in] key_sec    : 密钥区号：0 ~ 7
 * \param[in] nblock     : 需要验证的卡块号，取值范围与卡类型有关，
 *                         - S50：0 ~ 63
 *                         - S70：0 ~ 255
 *                         - PLUS CPU 2K：0 ~ 127
 *                         - PLUS CPU 4K：0 ~ 255
 *
 * \retval AM_OK      : 验证成功
 * \retval -AM_EINVAL : 验证失败，参数错误
 * \retval -AM_EIO    : 验证失败，数据通信出错
 * 
 * \note PLUS CPU系列的卡的卡号有4字节和7字节之分，对于7字节卡号的卡，
 *       只需要将卡号的高4字节（等级2防碰撞得到的卡号）作为验证的卡号即可。
 */
int am_uartcmd_mifare_card_e2_authent (am_uartcmd_send_handle_t  handle,
                                      uint8_t             key_type,
                                      uint8_t            *p_uid,
                                      uint8_t             key_sec,
                                      uint8_t             nblock);

/**
 * \brief Mifare卡直接密钥验证
 *
 *    该命令将密码作为参数传递，因此在此之前不需用“装载IC卡密钥”命令。若当前卡
 * 为PLUS CPU卡的等级2或等级3，且输入的密码只有6字节，则模块自动将输入的密码
 * 复制2次，取前16字节作为当前验证密钥。
 *
 * \param[in] handle     : UARTCMD的操作句柄
 * \param[in] key_type   : 密钥类型，可以使用下列值：
 *                          - #AM_UARTCMD_IC_KEY_TYPE_A
 *                          - #AM_UARTCMD_IC_KEY_TYPE_B
 * \param[in] p_uid      : 卡序列号，4字节
 * \param[in] p_key      : 密钥缓冲区
 * \param[in] key_len    : 密钥的长度，只能为6（6字节密钥）或16（16字节密钥）
 * \param[in] nblock     : 需要验证的卡块号，取值范围与卡类型有关，
 *                         - S50：0 ~ 63
 *                         - S70：0 ~ 255
 *                         - PLUS CPU 2K：0 ~ 127
 *                         - PLUS CPU 4K：0 ~ 255
 *
 * \retval AM_OK      : 验证成功
 * \retval -AM_EINVAL : 验证失败，参数错误
 * \retval -AM_EIO    : 验证失败，数据通信出错
 * 
 * \note PLUS CPU系列的卡的卡号有4字节和7字节之分，对于7字节卡号的卡，
 *       只需要将卡号的高4字节（等级2防碰撞得到的卡号）作为验证的卡号即可。
 */
int am_uartcmd_mifare_card_direct_authent (am_uartcmd_send_handle_t  handle,
                                          uint8_t             key_type,
                                          uint8_t            *p_uid,
                                          uint8_t            *p_key,
                                          uint8_t             key_len,
                                          uint8_t             nblock);
          
/**
 * \brief Mifare卡数据读取
 *
 *     在验证成功之后，才能读相应的块数据，所验证的块号与读块号必须在同一个扇区内，
 * Mifare1卡从块号0开始按顺序每4个块1个扇区，若要对一张卡中的多个扇区进行操作，在
 * 对某一扇区操作完毕后，必须进行一条读命令才能对另一个扇区直接进行验证命令，否则
 * 必须从请求开始操作。对于PLUS CPU卡，若下一个读扇区的密钥和当前扇区的密钥相同，
 * 则不需要再次验证密钥，直接读即可。
 *
 * \param[in]  handle     : UARTCMD的操作句柄
 * \param[in]  nblock     : 读取数据的块号
 *                          - S50：0 ~ 63
 *                          - S70：0 ~ 255
 *                          - PLUS CPU 2K：0 ~ 127
 *                          - PLUS CPU 4K：0 ~ 255
 * \param[out] p_buf      : 存放读取的数据，大小为 16
 *
 * \retval AM_OK      : 读取成功
 * \retval -AM_EINVAL : 读取失败，参数错误
 * \retval -AM_EIO    : 读取失败，数据通信出错
 */
int am_uartcmd_mifare_card_read (am_uartcmd_send_handle_t  handle,
                                uint8_t             nblock,
                                uint8_t            *p_buf);

/**
 * \brief Mifare卡写数据,写之前必需成功进行密钥验证。
 *
 *      对卡内某一块进行验证成功后，即可对同一扇区的各个进行写操作（只要访问条件允许），
 *  其中包括位于扇区尾的密码块，这是更改密码的唯一方法。对于PLUS CPU卡等级2、3的AES密
 *  钥则是在其他位置修改密钥。
 *
 * \param[in] handle     : UARTCMD的操作句柄
 * \param[in] nblock     : 读取数据的块号
 *                         - S50：0 ~ 63
 *                         - S70：0 ~ 255
 *                         - PLUS CPU 2K：0 ~ 127
 *                         - PLUS CPU 4K：0 ~ 255
 * \param[in] p_buf      : 写入数据缓冲区，大小必须为 16
 *
 * \retval AM_OK      : 写入成功
 * \retval -AM_EINVAL : 写入失败，参数错误
 * \retval -AM_EIO    : 写入失败，数据通信出错
 */
int am_uartcmd_mifare_card_write (am_uartcmd_send_handle_t  handle,
                                 uint8_t             nblock,
                                 uint8_t            *p_buf);
                                
/**
 * \brief Mifare卡写数据
 *
 * \param[in] handle     : UARTCMD的操作句柄
 * \param[in] nblock     : 读取数据的块号, 1 ~ 15
 * \param[in] p_buf      : 写入数据缓冲区，大小必须为 4
 *
 * \retval AM_OK      : 写入成功
 * \retval -AM_EINVAL : 写入失败，参数错误
 * \retval -AM_EIO    : 写入失败，数据通信出错
 *
 * 此命令只对UltraLight卡有效，对UltraLight卡进行读操作与Mifare1卡一样。
 */
int am_uartcmd_ultralight_card_write (am_uartcmd_send_handle_t  handle,
                                     uint8_t             nblock,
                                     uint8_t            *p_buf);

/**
 * \name Mifare卡值操作模式
 * @{
 */

#define AM_UARTCMD_MIFARE_CARD_VALUE_ADD  0xC1  /**< \brief 加  */
#define AM_UARTCMD_MIFARE_CARD_VALUE_SUB  0xC0  /**< \brief 减  */

/** @}*/

/**
 * \brief Mifare值操作，对Mifare卡的值块进行加减操作
 *
 * \param[in] handle     : UARTCMD的操作句柄
 * \param[in] mode       : 值操作的模式，可以是加或减，使用下列宏：
 *                          - AM_UARTCMD_MIFARE_CARD_VALUE_ADD
 *                          - AM_UARTCMD_MIFARE_CARD_VALUE_SUB
 * \param[in] nblock     : 进行值操作的块号
 *                           - S50：0 ~ 63
 *                            - S70：0 ~ 255
 *                            - PLUS CPU 2K：0 ~ 127
 *                            - PLUS CPU 4K：0 ~ 255
 * \param[in] ntransblk  : 传输块号，计算结果值存放的块号
 * \param[in] value      : 4字节有符号数
 *
 * \retval AM_OK      : 写入成功
 * \retval -AM_EINVAL : 写入失败，参数错误
 * \retval -AM_EIO    : 写入失败，数据通信出错
 *
 * \note 要进行此类操作，块数据必须要有值块的格式，可参考NXP的相关文档。若卡块
 * 号与传输块号相同，则将操作后的结果写入原来的块内；若卡块号与传输块号不相同，
 * 则将操作后的结果写入传输块内，结果传输块内的数据被覆盖，原块内的值不变。
 * 处于等级2的PLUS CPU卡不支持值块操作，等级1、3支持。
 */
int am_uartcmd_mifare_card_val_operate (am_uartcmd_send_handle_t  handle,
                                       uint8_t             mode,
                                       uint8_t             nblock,
                                       uint8_t             ntransblk,
                                       int32_t             value);

/**
 * \brief Mifare卡复位，通过将载波信号关闭指定的时间，再开启来实现卡片复位。
 * 
 * \param[in] handle     : UARTCMD的操作句柄
 * \param[in] time_ms    : 关闭的时间（单位:ms），0为一直关闭
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 失败，参数错误
 * \retval -AM_EIO    : 失败，数据通信出错
 *
 * \note 该函数将天线信号关闭数毫秒，若一直关闭，则等到执行一个请求命令时打开。
 */
int am_uartcmd_mifare_card_reset (am_uartcmd_send_handle_t  handle,
                                 uint8_t             time_ms);

/**
 * \brief Mifare卡激活，该函数用于激活卡片，是请求、防碰撞和选择三条命令的组合。
 *
 * \param[in] handle     : UARTCMD的操作句柄
 * \param[in] req_mode   : 请求模式，可使用下列宏：
 *                           - #AM_UARTCMD_MIFARE_CARD_REQ_IDLE
 *                           - #AM_UARTCMD_MIFARE_CARD_REQ_ALL
 * \param[in] p_atq      : 请求应答
 * \param[in] p_saq      : 最后一级选择应答SAK
 * \param[in] p_len      : 序列号的实际长度，4字节、7字节或10字节
 * \param[in] p_uid      : 序列号缓冲区，长度应该与序列号长度保持一致，
 *                         如不确定，建议先使用最大长度10。
 * \param[in]  uid_len   ： p_uid 所指缓冲区的长度
 *
 * \retval AM_OK      : 写入成功
 * \retval -AM_EINVAL : 写入失败，参数错误
 * \retval -AM_EIO    : 写入失败，数据通信出错
 */
int uartcmd_card_active (am_uartcmd_send_handle_t  handle,
                                  uint8_t             req_mode,
                                  uint16_t           *p_atq,
                                  uint8_t            *p_saq,
                                  uint8_t            *p_len,
                                  uint8_t            *p_uid,
                                  uint8_t             uid_len);

/**
 * \brief Mifare值操作，设置值块的值
 *
 * \param[in] handle     : UARTCMD的操作句柄
 * \param[in] nblock     : 读取数据的值块地址
 * \param[in] value      : 设置的值
 *
 * \retval AM_OK      : 设置成功
 * \retval -AM_EINVAL : 设置失败，参数错误
 * \retval -AM_EIO    : 设置失败，数据通信出错
 */
int am_uartcmd_mifare_card_val_set (am_uartcmd_send_handle_t  handle,
                                   uint8_t             nblock,
                                   int32_t             value);
               
/**
 * \brief Mifare值操作，获取值块的值
 *
 * \param[in] handle     : UARTCMD的操作句柄
 * \param[in] nblock     : 读取数据的值块地址
 * \param[in] p_value    : 获取值的指针
 *
 * \retval AM_OK      : 读取成功
 * \retval -AM_EINVAL : 读取失败，参数错误
 * \retval -AM_EIO    : 读取失败，数据通信出错
 */
int am_uartcmd_mifare_card_val_get (am_uartcmd_send_handle_t  handle,
                                   uint8_t             nblock,
                                   int32_t            *p_value);

/**
 * \brief 命令传输
 *
 *     该函数属于模块扩展功能，用于模块向卡片发送任意长度组合的数据串，例如针对
 * NXP新推出的NTAG213F是属于Ultralight C系列卡片，但是该卡片又新添加了扇区数据
 * 读写密钥保护功能。而这个密钥验证命令即可利用此命名传输函数来实现。
 *
 * \param[in] handle     : UARTCMD的操作句柄
 * \param[in] p_tx_buf   : 发送数据的缓冲区
 * \param[in] tx_nbytes  : 发送字节数
 * \param[in] p_rx_buf   : 接收数据的缓冲区
 * \param[in] rx_nbytes  : 接收字节数
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 失败，参数错误
 * \retval -AM_EIO    : 失败，数据通信出错
 */
int am_uartcmd_mifare_card_cmd_trans (am_uartcmd_send_handle_t  handle,
                                     uint8_t            *p_tx_buf,
                                     uint8_t             tx_nbytes,
                                     uint8_t            *p_rx_buf,
                                     uint8_t             rx_nbytes);

/**
 * \brief 数据交互
 *
 * 该命令用读写器与卡片的数据交互，通过该函数可以实现读写卡器的所有功能。
 *
 * \param[in]  handle     : UARTCMD的操作句柄
 * \param[in]  p_data_buf : 需要交互发送的数据
 * \param[in]  len        : 交互数据的长度
 * \param[in]  wtxm_crc   : b7~b2,wtxm；b1,RFU(0); b0,0,CRC禁止能，1,CRC使能。
 * \param[in]  fwi        : 超时等待时间编码, 超时时间=（（0x01 << fwi*302us）
 * \param[out] p_rx_buf   : 数据交互回应帧信息
 * \param[in]  buf_size   : 接收回应信息的缓冲区大小
 * \param[in]  p_real_len : 用于获取实际接收的信息长度，如果缓冲区大小小于实际信
 *                         息长度，则多余部分将丢弃
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 失败，参数错误
 * \retval -AM_EIO    : 失败，数据通信出错
 */
int am_uartcmd_mifare_card_data_exchange (am_uartcmd_send_handle_t  handle,
                                         uint8_t            *p_data_buf,
                                         uint8_t             len,
                                         uint8_t             wtxm_crc,
                                         uint8_t             fwi,
                                         uint8_t            *p_rx_buf,
                                         uint8_t             buf_size,
                                         uint16_t           *p_real_len);

/**
 * \name Mifare卡检测模式
 * @{
 */

/** \brief 执行完一次自动检测后执行Halt命令  */
#define AM_UARTCMD_MIFARE_CARD_AD_HALT  0x08

/** @}*/

/**
 * \name Mifare卡天线驱动模式
 * @{
 */

#define AM_UARTCMD_MIFARE_CARD_TX_IN_TURN   0x00 /**< \brief TX1、TX2交替运行 */
#define AM_UARTCMD_MIFARE_CARD_TX_TX1       0x01 /**< \brief 仅TX1驱动 */
#define AM_UARTCMD_MIFARE_CARD_TX_TX2       0x02 /**< \brief 仅TX2驱动 */
#define AM_UARTCMD_MIFARE_CARD_TX_ALL       0x03 /**< \brief TX1和TX2同时驱动 */

/** @}*/

/**
 * \name Mifare卡秘钥验证模式
 * @{
 */

#define AM_UARTCMD_MIFARE_CARD_AUTH_E2      0x45 /**< \brief E2密钥验证(E) */
#define AM_UARTCMD_MIFARE_CARD_AUTH_DIRECT  0x46 /**< \brief 直接密钥验证(F) */
#define AM_UARTCMD_MIFARE_CARD_AUTH_NO      0x00 /**< \brief 不验证 */

/** @}*/

/**
 * \brief 启动Mifare卡自动检测
 * \note 在自动检测期间，若主机发送任何除读自动检测数据外的，且数据长度小于3
 *       的命令，将退出自动检测模式，如请求am_uartcmd_mifare_card_active()
 *       命令，在此期间，模块将不接收数据长度大于2的命令
 *
 * \param[in] handle    : UARTCMD的操作句柄
 * \param[in] p_adinfo  : 自动检测信息
 *
 * \retval AM_OK      : 写入成功
 * \retval -AM_EINVAL : 写入失败，参数错误
 * \retval -AM_EIO    : 写入失败，数据通信出错
 */
int am_uartcmd_mifare_card_auto_detect_start (am_uartcmd_send_handle_t                  handle,
                                             const am_uartcmd_auto_detect_info_t *p_adinfo);

/**
 * \brief 关闭Mifare卡自动检测
 *
 * \param[in] handle  : UARTCMD的操作句柄
 *
 * \retval AM_OK      : 写入成功
 * \retval -AM_EINVAL : 写入失败，参数错误
 * \retval -AM_EIO    : 写入失败，数据通信出错
 */
int am_uartcmd_mifare_card_auto_detect_stop (am_uartcmd_send_handle_t handle);

/**
 * \brief Mifare卡自动检测回调函数设置
 *
 * \param[in] handle       : UARTCMD的操作句柄
 * \param[in] pfn_callback : 回调函数指针
 * \param[in] p_arg        : 回调函数入口参数
 *
 * \retval AM_OK      : 写入成功
 * \retval -AM_EINVAL : 写入失败，参数错误
 * \retval -AM_EIO    : 写入失败，数据通信出错
 */
int am_uartcmd_mifare_card_auto_detect_cb_set (am_uartcmd_send_handle_t  handle,
                                              am_pfnvoid_t        pfn_callback,
                                              void               *p_arg);

/**
 * \name Mifare卡读自动检测标识
 * @{
 */

#define AM_UARTCMD_MIFARE_CARD_AD_READ_CANCEL    0x00  /**< \brief 取消检测 */
#define AM_UARTCMD_MIFARE_CARD_AD_READ_CONTINUE  0x01  /**< \brief 继续检测 */

/** @} */

/**
 * \brief Mifare卡片信息
 */
typedef struct am_uartcmd_mifare_card_info {
    uint8_t     tx_mode;        /**< \brief 天线驱动模式 */
    uint8_t     uid_len;        /**< \brief UID(卡唯一序列号)长度，4、7、10 */
    uint8_t     uid[10];        /**< \brief UID */
    uint8_t     card_data[16];  /**< \brief 卡片数据 */
} am_uartcmd_mifare_card_info_t;

/**
 * \brief Mifare卡读取自动检测数据命令
 *
 * \param[in]  handle      : UARTCMD的操作句柄
 * \param[out] p_card_info : 指向存放卡片数据的指针
 *
 * \retval AM_OK      : 写入成功
 * \retval -AM_EINVAL : 写入失败，参数错误
 * \retval -AM_EIO    : 写入失败，数据通信出错
 */
int am_uartcmd_mifare_card_auto_detect_read (am_uartcmd_send_handle_t            handle,
                                            am_uartcmd_mifare_card_info_t *p_card_info);




/**
 * \name PICCA卡请求模式
 * @{
 */
#define AM_UARTCMD_PICCA_REQ_IDLE 0x26 /**< \brief IDLE模式,请求空闲的卡 */
#define AM_UARTCMD_PICCA_REQ_ALL  0x52 /**< \brief ALL模式,请求所有的卡  */
/** @}*/ 

/**
 * \brief PICCA卡请求操作
 *
 *  该函数作为卡的请求操作，只要符合ISO14443A标准的卡都应能发出响应。
 *
 * \param[in] handle      : UARTCMD的操作句柄
 * \param[in] req_mode    : 请求模式，可使用下列宏：
 *                           - #AM_UARTCMD_PICCA_REQ_IDLE
 *                           - #AM_UARTCMD_PICCA_REQ_ALL
 * \param[in] p_atq       : 获取请求应答信息（ATQ）的指针，其值为16位，不同卡
 *                          类型返回的信息不同,常见类型返回的ATQ如下：
 *
 *            Mifare1 S50    | Mifare1 S70 | Mifare1 Light | Mifare0 UltraLight 
 *       --------------------|-------------|---------------|-------------------
 *              0x0004       |    0x0002   |    0x0010     |      0x0044 
 *       ----------------------------------------------------------------------
 *          Mifare3 DESFire  |   SHC1101   |    SHC1102    |      11RF32
 *       --------------------|-------------|---------------|-------------------
 *               0x0344      |    0x0004   |    0x3300     |      0x0004
 *
 * \retval AM_OK      : 请求成功
 * \retval -AM_EINVAL : 请求失败，参数错误
 * \retval -AM_EIO    : 请求失败，数据通信出错
 *
 * \note 卡进入天线后，从射频场中获取能量，从而得电复位，复位后卡处于IDLE模式，
 * 用两种请求模式的任一种请求时，此时的卡均能响应；若对某一张卡成功进行了挂起
 * 操作（Halt命令或DeSelect命令），则进入了Halt模式，此时的卡只响应ALL（0x52）
 * 模式的请求，除非将卡离开天线感应区后再进入。
 */ 
int am_uartcmd_picca_request (am_uartcmd_send_handle_t handle,
                             uint8_t            req_mode,
                             uint16_t          *p_atq);

/**
 * \name PICCA卡防碰撞等级
 * @{
 */
#define AM_UARTCMD_PICCA_ANTICOLL_1  0x93  /**< \brief 第一级防碰撞  */
#define AM_UARTCMD_PICCA_ANTICOLL_2  0x95  /**< \brief 第二级防碰撞  */
#define AM_UARTCMD_PICCA_ANTICOLL_3  0x97  /**< \brief 第三级防碰撞  */
/** @}*/ 

/**
 * \brief PICCA卡的防碰撞操作
 *
 *  需要成功执行一次请求命令，并返回请求成功，才能进行防碰撞操作，否则返回错误。
 *
 * \param[in] handle         : UARTCMD的操作句柄
 * \param[in] anticoll_level : 防碰撞等级，可使用下列宏：
 *                             - #AM_UARTCMD_MIFARE_CARD_ANTICOLL_1
 *                             - #AM_UARTCMD_MIFARE_CARD_ANTICOLL_2
 *                             - #AM_UARTCMD_MIFARE_CARD_ANTICOLL_3
 *
 * \param[in] know_uid : 已知的卡序列号
 * \param[in] nbit_cnt : 位计数，表明已知的序列号uid的位数。其值小于32。
 *                       - 若nbit_cnt值为0,则表明没有已知序列号，序列号的所有位
 *                         都要从本函数获得。
 *                       - 若nbit_cnt不为0，则序列号中有已知的序列号的值，表示
 *                         序列号的前nbit_cnt位为已知序列号中所示前nbit_cnt位的
 *                         值，其余位需要本函数获得。
 * \param[in] *p_uid   : 防碰撞后获取到的卡序列号
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 失败，参数错误
 * \retval -AM_EIO    : 失败，数据通信出错
 *
 * \note  符合ISO14443A标准卡的序列号都是全球唯一的，正是这种唯一性，才能实现防
 * 碰撞的算法逻辑，若有若干张卡同时在天线感应区内则这个函数能够找到一张序列号较
 * 大的卡来操作。
 */ 
int am_uartcmd_picca_anticoll (am_uartcmd_send_handle_t handle,
                              uint8_t            anticoll_level,
                              uint32_t           know_uid,
                              uint8_t            nbit_cnt,
                              uint32_t          *p_uid);
                       
/**
 * \brief PICC A卡的选择操作
 *
 *  需要成功执行一次防碰撞命令，并返回成功，才能进行卡选择操作，否则返回错误。
 *
 * \param[in] handle         : UARTCMD的操作句柄
 * \param[in] anticoll_level : 防碰撞等级，可使用下列宏：
 *                             - #AM_UARTCMD_MIFARE_CARD_ANTICOLL_1
 *                             - #AM_UARTCMD_MIFARE_CARD_ANTICOLL_2
 *                             - #AM_UARTCMD_MIFARE_CARD_ANTICOLL_3
 * \param[in] uid   : 前一个防碰撞函数获取的UID
 * \param[in] p_sak : 返回的信息，若bit2为1，则表明UID不完整
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 失败，参数错误
 * \retval -AM_EIO    : 失败，数据通信出错
 *
 * \note  卡的序列号长度有三种：4字节、7字节和10字节。 4字节的只要用一级选择即可
 * 得到完整的序列号，如Mifare1 S50/S70等；7字节的要用二级选择才能得到完整的序列
 * 号，前一级所得到的序列号的最低字节为级联标志0x88，在序列号内只有后3字节可用，
 * 后一级选择能得到4字节序列号，两者按顺序连接即为7字节序列号，如UltraLight
 * 和DesFire等；10字节的以此类推，但至今还未发现此类卡。
 */ 
int am_uartcmd_picca_select (am_uartcmd_send_handle_t handle,
                            uint8_t            anticoll_level,
                            uint32_t           uid,
                            uint8_t           *p_sak);

/**
 * \brief PICCA卡的挂起操作，使所选择的卡进入HALT状态
 *
 *  在HALT状态下，卡将不响应读卡器发出的IDLE模式的请求，除非将卡复位或离开天线感
 *  应区后再进入。但它会响应读卡器发出的ALL请求。
 *
 * \param[in] handle         : UARTCMD的操作句柄
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 失败，参数错误
 * \retval -AM_EIO    : 失败，数据通信出错
 */ 
int am_uartcmd_picca_halt (am_uartcmd_send_handle_t handle);

/**
 * \brief PICC TypeA卡RATS（request for answer to select）
 *
 *     RATS（request for answer to select）是ISO14443-4协议的命令，模块发送RATS，
 * 卡片发出ATS（answer to select）作为RATS的应答，在执行该命令前，必需先进行一次
 * 卡选择操作，且执行过一次RATS命令后，想再次执行RATS命令，必需先解除激活。
 *
 * \param[in] handle     : UARTCMD的操作句柄
 * \param[in] cid        : 卡标识符（card IDentifier，取值范围0x00～0x0E）
 * \param[in] p_ats_buf  : 用于接收回应的信息，不同卡，字节数会有不同
 * \param[in] bufsize    : 缓冲区大小
 * \param[in] p_rx_len   : 用于获取实际接收的信息长度，如果缓冲区大小小于实际信
 *                         息长度，则多余部分将丢弃
 *
 * \retval AM_OK      : 写入成功
 * \retval -AM_EINVAL : 写入失败，参数错误
 * \retval -AM_EIO    : 写入失败，数据通信出错
 */
int am_uartcmd_picca_rats (am_uartcmd_send_handle_t  handle,
                          uint8_t             cid,
                          uint8_t            *p_ats_buf,
                          uint8_t             bufsize,
                          uint16_t           *p_rx_len);

/**
 * \brief PICC TypeA卡PPS（request for answer to select）
 *
 *     PPS（protocal and parameter selection）是ISO14443-4协议的命令，用于改变有
 * 关的专用协议参数，该命令不是必需的，命令只支持默认参数，即该命令的参数设置为
 * 0即可。在执行该命令前，必需先成功执行一次RATS命令。
 *
 * \param[in] handle     : UARTCMD的操作句柄
 * \param[in] dsi_dri    : 模块与卡通信波特率,设置为0（默认）即可
 *
 * \retval AM_OK      : 写入成功
 * \retval -AM_EINVAL : 写入失败，参数错误
 * \retval -AM_EIO    : 写入失败，数据通信出错
 */
int am_uartcmd_picca_pps (am_uartcmd_send_handle_t  handle,
                         uint8_t             dsi_dri);
                         
/**
 * \brief PICC TypeA卡解除激活
 *
 *   该命令是ISO14443-4协议的命令，用于将卡片置为挂起（HALT）状态，处于挂起
 *（HALT）状态的卡可以用“请求”命令（请求代码为ALL）来重新激活卡，只有执行“RATS”
 * 命令的卡才用该命令。
 *
 * \param[in] handle     : UARTCMD的操作句柄
 *
 * \retval AM_OK      : 写入成功
 * \retval -AM_EINVAL : 写入失败，参数错误
 * \retval -AM_EIO    : 写入失败，数据通信出错
 */
int am_uartcmd_picca_deselect (am_uartcmd_send_handle_t  handle);
                             
/**
 * \brief PICC卡T=CL
 *
 *     T=CL是半双工分组传输协议，ISO14443-4协议命令，用于读写器与卡片之间的数据
 * 交互，一般符合ISO14443协议的CPU卡均用该协议与读写器通信。调用该命令时只需要
 * 将CPU卡COS命令的数据作为输入即可，其他的如分组类型、卡标识符CID、帧等待时间
 * FWT、等待时间扩展倍增因子WTXM（waiting time extensionmultiplier），等等由该
 * 命令自动完成。
 *
 * \param[in] handle      : UARTCMD的操作句柄
 * \param[in] p_cos_buf   : COS命令
 * \param[in] cos_bufsize ：COS命令的长度
 * \param[in] p_res_buf   : 用于接收回应的信息，不同COS命令，字节数会有不同
 * \param[in] res_bufsize : 接收回应信息的缓冲区大小
 * \param[in] p_rx_len    : 用于获取实际接收的信息长度，如果缓冲区大小小于实际信
 *                          息长度，则多余部分将丢弃
 *
 * \retval AM_OK      : 写入成功
 * \retval -AM_EINVAL : 写入失败，参数错误
 * \retval -AM_EIO    : 写入失败，数据通信出错
 */
int am_uartcmd_picc_tpcl (am_uartcmd_send_handle_t  handle,
                         uint8_t            *p_cos_buf,
                         uint8_t             cos_bufsize,
                         uint8_t            *p_res_buf,
                         uint8_t             res_bufsize,
                         uint16_t           *p_rx_len);

/**
 * \brief 数据交换
 *
 * 该命令用读写器与卡片的数据交互，通过该函数可以实现读写卡器的所有功能。
 *
 * \param[in] handle     : UARTCMD的操作句柄
 * \param[in] p_data_buf : 需要交互发送的数据（其内容与实际使用的卡有关）
 * \param[in] len        : 交互数据的长度
 * \param[in] wtxm_crc   : b7~b2,wtxm；b1,RFU(0); b0,0-CRC禁止能，1-CRC使能。
 * \param[in] fwi        : 超时等待时间编码, 超时时间=（（0x01 << fwi*302us）
 * \param[in] p_rx_buf   : 数据交互回应帧信息
 * \param[in] bufsize    : 接收回应信息的缓冲区大小
 * \param[in] p_rx_len   : 用于获取实际接收的信息长度，如果缓冲区大小小于实际信
 *                         息长度，则多余部分将丢弃
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 失败，参数错误
 * \retval -AM_EIO    : 失败，数据通信出错
 */
int am_uartcmd_picc_data_exchange (am_uartcmd_send_handle_t  handle,
                                  uint8_t            *p_data_buf,
                                  uint8_t             len,
                                  uint8_t             wtxm_crc,
                                  uint8_t             fwi,
                                  uint8_t            *p_rx_buf,
                                  uint8_t             bufsize,
                                  uint16_t           *p_rx_len);

/**
 * \brief PICC A卡复位，通过将载波信号关闭指定的时间，再开启来实现卡片复位。
 * 
 * \param[in] handle     : UARTCMD的操作句柄
 * \param[in] time_ms    : 关闭的时间（单位:ms），0为一直关闭
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 失败，参数错误
 * \retval -AM_EIO    : 失败，数据通信出错
 *
 * \note 该函数将天线信号关闭数毫秒，若一直关闭，则等到执行一个请求命令时打开。
 */
int am_uartcmd_picca_reset(am_uartcmd_send_handle_t  handle,
                          uint8_t             time_ms);
                          
/**
 * \brief PICC A卡激活，该函数用于激活卡片，是请求、防碰撞和选择三条命令的组合。
 *
 * \param[in] handle     : UARTCMD的操作句柄
 * \param[in] req_mode   : 请求模式，可使用下列宏：
 *                           - #AM_UARTCMD_PICCA_REQ_IDLE
 *                           - #AM_UARTCMD_PICCA_REQ_ALL
 * \param[in] p_atq      : 请求应答
 * \param[in] p_saq      : 最后一级选择应答SAK
 * \param[in] p_len      : 序列号长度，4字节或7字节
 * \param[in] p_uid      : 序列号缓冲区，长度应该与序列号长度保持一致
 * \param[in] uid_len    : p_uid 所指缓冲区的长度
 *
 * \retval AM_OK      : 写入成功
 * \retval -AM_EINVAL : 写入失败，参数错误
 * \retval -AM_EIO    : 写入失败，数据通信出错
 */
int am_uartcmd_picca_active (am_uartcmd_send_handle_t  handle,
                            uint8_t             req_mode,
                            uint16_t           *p_atq,
                            uint8_t            *p_saq,
                            uint8_t            *p_len,
                            uint8_t            *p_uid,
                            uint8_t             uid_len);

/**
 * \name PICCB卡请求模式
 * @{
 */

#define AM_UARTCMD_PICCB_REQ_IDLE 0x00 /**< \brief IDLE模式,请求空闲的卡 */
#define AM_UARTCMD_PICCB_REQ_ALL  0x08 /**< \brief ALL模式,请求所有的卡  */

/** @}*/ 

/**
 * \brief PICC B卡激活
 *
 *    在调用该函数前，需要确保IC工作模式已经配置为TypeB模式。
 *
 * \param[in] handle     : UARTCMD的操作句柄
 * \param[in] req_mode   : 请求模式，可使用下列宏：
 *                           - #AM_UARTCMD_PICCB_REQ_IDLE
 *                           - #AM_UARTCMD_PICCB_REQ_ALL
 * \param[in] p_uid      : UID相关信息，长度为12
 *
 * \retval AM_OK      : 写入成功
 * \retval -AM_EINVAL : 写入失败，参数错误
 * \retval -AM_EIO    : 写入失败，数据通信出错
 */
int am_uartcmd_piccb_active (am_uartcmd_send_handle_t  handle,
                            uint8_t             req_mode,
                            uint8_t            *p_uid);

/**
 * \brief PICC B卡复位，通过将载波信号关闭指定的时间，再开启来实现卡片复位。
 * 
 * \param[in] handle     : UARTCMD的操作句柄
 * \param[in] time_ms    : 关闭的时间（单位:ms），0为一直关闭
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 失败，参数错误
 * \retval -AM_EIO    : 失败，数据通信出错
 *
 * \note 该函数将天线信号关闭数毫秒，若一直关闭，则等到执行一个请求命令时打开。
 */
int am_uartcmd_piccb_reset (am_uartcmd_send_handle_t  handle,
                           uint8_t             time_ms);

/**
 * \brief PICCA卡请求操作
 *
 *  该函数作为卡的请求操作，只要符合ISO14443A标准的卡都应能发出响应。
 *
 * \param[in] handle      : UARTCMD的操作句柄
 * \param[in] req_mode    : 请求模式，可使用下列宏：
 *                           - #AM_UARTCMD_PICCB_REQ_IDLE
 *                           - #AM_UARTCMD_PICCB_REQ_ALL
 * \param[in] slot_time   : 时隙总数，0 ~ 4
 * \param[in] p_uid       : 返回的UID相关信息，长度为12
 *
 * \retval AM_OK      : 请求成功
 * \retval -AM_EINVAL : 请求失败，参数错误
 * \retval -AM_EIO    : 请求失败，数据通信出错
 */ 
int am_uartcmd_piccb_request (am_uartcmd_send_handle_t  handle,
                             uint8_t             req_mode,
                             uint8_t             slot_time,
                             uint8_t            *p_uid);

/**
 * \brief PICC B 卡的防碰撞操作
 *
 * \param[in] handle      : UARTCMD的操作句柄
 * \param[in] slot_flag   : 时隙标记，2 ~ 16，该参数值与请求命令的时隙总数有关
 *                          系，假如请求命令的时隙总数为n，侧该时隙标记N < 2^n
 * \param[in] p_uid       : 返回的UID相关信息，长度为12
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 失败，参数错误
 * \retval -AM_EIO    : 失败，数据通信出错
 */
int am_uartcmd_piccb_anticoll (am_uartcmd_send_handle_t  handle,
                              uint8_t             slot_flag,
                              uint8_t            *p_uid);
                             
/**
 * \brief PICC B卡的挂起操作，使所选择的卡进入HALT状态
 *
 *     该函数用于B型卡挂起，在执行挂起命令前，必需先执行成功过一次请求命令。执行
 * 挂起命令成功后，卡片处于挂起状态，模块必需通过ALL方式请求卡片，而不能用IDLE方
 * 式请求。
 *
 * \param[in] handle     : UARTCMD的操作句柄
 * \param[in] p_pupi     : PUPI,大小为4字节的标识符
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 失败，参数错误
 * \retval -AM_EIO    : 失败，数据通信出错
 */ 
int am_uartcmd_piccb_halt (am_uartcmd_send_handle_t handle,
                          uint8_t            *p_pupi);
 
/**
 * \brief 修改B卡的传输属性
 *
 *     该函数用于B型卡挂起，在执行挂起命令前，必需先执行成功过一次请求命令。执行
 * 挂起命令成功后，卡片处于挂起状态，模块必需通过ALL方式请求卡片，而不能用IDLE方
 * 式请求。
 *
 * \param[in] handle     : UARTCMD的操作句柄
 * \param[in] p_pupi     : PUPI,大小为4字节的标识符
 * \param[in] cid        : 取值范围为 0 - 14, 若不支持CID，则设置为0
 * \param[in] protype    : 支持的协议
 *                          - bit3 : 1-PCD中止与PICC继续通信,0-PCD与PICC继续通信
 *                          - bit2 ~ bit1 : 
 *                              - 11 : 10 etu + 512 / fs
 *                              - 10 : 10 etu + 256 / fs
 *                              - 01 : 10 etu + 128 / fs
 *                              - 00 : 10 etu + 32 / fs
 *                          - bit0 : 1-遵循ISO14443-4,0-不遵循ISO14443-4
 *                                   (二代身份证必须该位必须为1)
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 失败，参数错误
 * \retval -AM_EIO    : 失败，数据通信出错
 */ 
int am_uartcmd_piccb_attrib_set (am_uartcmd_send_handle_t handle,
                                uint8_t           *p_pupi,
                                uint8_t            cid,
                                uint8_t            protype);

/**
 * \brief 读取身份证信息
 *
 * \param[in]  handle   : UARTCMD的操作句柄
 * \param[in]  req_mode : 请求模式，可使用下列宏：
 *                           - #AM_UARTCMD_PICCB_REQ_IDLE
 *                           - #AM_UARTCMD_PICCB_REQ_ALL
 * \param[out] p_info : 身份证信息缓冲区（大小应为8字节）
 *
 * \retval AM_OK      : 写入成功
 * \retval -AM_EINVAL : 写入失败，参数错误
 * \retval -AM_EIO    : 写入失败，数据通信出错
 *
 * \note 要进行读身份证信息操作，需先调用 am_uartcmd_ic_isotype_set 接口将卡类型设置为B卡,
 *       读取完成之后应该视情况调用 am_uartcmd_ic_isotype_set 将卡类型还原或不还原。
 */
int am_uartcmd_id_card_info_get (am_uartcmd_send_handle_t handle, uint8_t req_mode, uint8_t *p_info);

/**
 * \brief 指令超时时间设置
 *
 * \param[in]  handle     : UARTCMD的操作句柄
 * \param[in]  timeout_ms : 指令超时时间（毫秒）
 *
 * \retval AM_OK : 设置成功
 * \retval 其他     : 设置失败
 */
int am_uartcmd_cmd_timeout_set (am_uartcmd_send_handle_t handle, uint32_t timeout_ms);

/** 
 * @}
 */
 
#endif /* __AM_UARTCMD_H */

/* end of file */
