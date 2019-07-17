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
#ifndef __AM_UARTCMD_FLASH_DEFINE_H
#define __AM_UARTCMD_FLASH_DEFINE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \name 工作模式
 * @{
 */

#define UARTCMD_FLASH_WORK_MODE_MASK      0xF0    /**< \brief 工作模式掩码 */
#define UARTCMD_FLASH_WORK_MODE_SLAVE     0x00    /**< \brief 从机模式 */
#define UARTCMD_FLASH_WORK_MODE_AUTO      0x10    /**< \brief 自动检测模式 */

#define UARTCMD_FLASH_COMM_MODE_MASK      0x0F    /**< \brief 通信模式掩码 */
#define UARTCMD_FLASH_COMM_AUTO           0x00    /**< \brief 通信自动检测模式 */
#define UARTCMD_FLASH_COMM_I2C            0x01    /**< \brief I2C通信 */
#define UARTCMD_FLASH_COMM_UART           0x02    /**< \brief UART通信 */
#define UARTCMD_FLASH_COMM_WG             0x08    /**< \brief WG通信 */

/** \brief 默认工作模式 */
#define UARTCMD_FLASH_MODE_DEFAULT       (UARTCMD_FLASH_WORK_MODE_SLAVE | \
                                          UARTCMD_FLASH_COMM_AUTO)

/** @} */

/**
 * \name 版本信息
 * @
 */

/** \brief 版本信息长度 */
#define UARTCMD_FLASH_VERSION_SIZE        20

/** \brief 序列号长度 */
#define UARTCMD_FLASH_SN_SIZE             16

/** @} */

/***
 * \name 帧格式
 * @{
 */

#define UARTCMD_FLASH_FRAME_FORMAT_OLD       0x00    /**< \brief 旧帧格式 */
#define UARTCMD_FLASH_FRAME_FORMAT_NEW       0x01    /**< \brief 新帧格式 */

/** @} */

/**
 * \name 波特率
 * @{
 */

#define UARTCMD_FLASH_BAUD_230400            7       /**< \brief 230400 */
#define UARTCMD_FLASH_BAUD_172800            6       /**< \brief 172800 */
#define UARTCMD_FLASH_BAUD_115200            5       /**< \brief 115200 */
#define UARTCMD_FLASH_BAUD_57600             4       /**< \brief 57600 */
#define UARTCMD_FLASH_BAUD_38400             3       /**< \brief 38400 */
#define UARTCMD_FLASH_BAUD_28800             2       /**< \brief 28800 */
#define UARTCMD_FLASH_BAUD_19200             1       /**< \brief 19200 */
#define UARTCMD_FLASH_BAUD_9600              0       /**< \brief 9600 */

/** @} */

//#define AUTO_DETEST_EN_MASK             0x80        //!< 自动检测使能掩码
//#define AUTO_DETEST_NOT_SENT_MASK       0x40        //!< 数据未发送掩码
//#define AUTO_DETEST_EXECUTIVE_HALT_MASK 0x08        //!< 最后执行Halt()掩码
//#define AUTO_DETEST_CONTINUE_MASK       0x04        //!< 输出后继续执行掩码
//#define AUTO_DETEST_INTERRUPT_MASK      0x02        //!< 产生中断掩码（UART）
//#define AUTO_DETEST_INITIATIVE_SEND_MASK    0x01    //!< 主动发送掩码(UART)

/**
 * \name 密钥类型
 * @{
 */

#define UARTCMD_FLASH_KEY_TYPE_A        0x60  /**< \brief 密钥类型A */
#define UARTCMD_FLASH_KEY_A_LEN         0x06  /**< \brief 类型A长度为8 */
#define UARTCMD_FLASH_KEY_TYPE_B        0x61  /**< \brief 密钥类型B */
#define UARTCMD_FLASH_KEY_B_LEN         0x10  /**< \brief 密钥长度为16 */

/**
 *  \brief 存放到FLASH中的密钥长度。不管是类型A还是类型B，存放到FLASH中都是16字节。
 *         需将类型A填充到16字节
 */
#define UARTCMD_FLASH_KEY_LEN           0x10

/** \brief 用户密钥长度 */
#define UARTCMD_FLASH_USER_KEY_LEN      0x10

/** @} */

/**
 * \name 用户数据
 * @{
 */

#define UARTCMD_FLASH_USER_MEM_SIZE     256  /**< \brief 用户数据大小 */

/** @} */

/**
 * \name I2C地址
 * @{
 */

#define UARTCMD_FLASH_MAX_I2C_ADDR         0xBF  /**< \brief 设备最大I2C地址 */
#define UARTCMD_FLASH_MIN_I2C_ADDR         0xB0  /**< \brief 设备最小I2C地址 */
#define UARTCMD_FLASH_DEFAULT_ADDR         0xB2  /**< \brief 设备默认地址 0xB2 */

/** @} */

/**
 * \name 协议类型
 * @{
 */

#define UARTCMD_FLASH_CRAD_TPYE_ISO14443A   0x3F  /**< \brief ISO14443A */
#define UARTCMD_FLASH_CARD_TYPE_ISO14443B   0x5F  /**< \brief ISO14443B */
#define UARTCMD_FLASH_CARD_TYPE_7816        0x9F  /**< \brief ISO7816_3 */

/** @} */

/**
 * \name 认证时算法标识
 * @{
 */

#define UARTCMD_FLASH_AUTH_ALGORITHM_ID_MASK   0x7F   /**< \brief 算法标识掩码 */
#define UARTCMD_FLASH_AUTH_ALGORITHM_3_DES     0x00   /**< \brief 3-DES */
#define UARTCMD_FLASH_AUTH_ALGORITHM_1_DES     0x01   /**< \brief 1-DES */
#define UARTCMD_FLASH_AUTH_ALGORITHM_SM1       0x03   /**< \brief SM1 */

/** \brief Mifaer Crypto1/PLUS CPU AES 密钥A */
#define UARTCMD_FLASH_AUTH_ALGORITHM_KEY_A     0x60

/** \brief Mifaer Crypto1/PLUS CPU AES 密钥B */
#define UARTCMD_FLASH_AUTH_ALGORITHM_KEY_B     0x61

#define UARTCMD_FLASH_AUTH_ALGORITHM_NO        0x7F   /**< \brief 不验证 */

/** @} */

/**
 * \name 自动检测模式
 * @{
 */

#define UARTCMD_FLASH_AUTO_DETEST_EN_MASK               0x80    /**< \brief 自动检测使能掩码 */
#define UARTCMD_FLASH_AUTO_DETEST_NO_SEND_MASK          0x40    /**< \brief 数据未发送掩码 */
#define UARTCMD_FLASH_AUTO_DETEST_EXECUTE_HALT_MASK     0x08    /**< \brief 最后执行Halt()掩码 */
#define UARTCMD_FLASH_AUTO_DETEST_CONTINUE_MASK         0x04    /**< \brief 输出后继续执行掩码 */
#define UARTCMD_FLASH_AUTO_DETEST_INTERRUPT_MASK        0x02    /**< \brief 产生中断掩码（UART） */
#define UARTCMD_FLASH_AUTO_DETEST_SEND_MASK             0x01    /**< \brief 主动发送掩码(UART) */

/** \brief UARTCMD自动检测模式掩码 */
#define UARTCMD_FLASH_AUTO_DETEST_MODE_MASK            (UARTCMD_FLASH_AUTO_DETEST_EXECUTE_HALT_MASK | \
                                                        UARTCMD_FLASH_AUTO_DETEST_CONTINUE_MASK     | \
                                                        UARTCMD_FLASH_AUTO_DETEST_INTERRUPT_MASK    | \
                                                        UARTCMD_FLASH_AUTO_DETEST_SEND_MASK)

#define UARTCMD_FLASH_AUTO_DETEST_EN                    0x80     /**< \brief 自动检测使能 */
#define UARTCMD_FLASH_AUTO_DETEST_NO_SEND               0x40     /**< \brief 数据未发送 */
#define UARTCMD_FLASH_AUTO_DETEST_EXECUTE_HALT          0x08     /**< \brief 最后执行Halt() */
#define UARTCMD_FLASH_AUTO_DETEST_CONTINUE              0x04     /**< \brief 输出后继续执行 */
#define UARTCMD_FLASH_AUTO_DETEST_INTERRUPT             0x02     /**< \brief 产生中断 */
#define UARTCMD_FLASH_AUTO_DETEST_SEND                  0x01     /**< \brief 主动发送 */

/** @} */

/**
 * \name 控制模式
 * @{
 */

#define UARTCMD_FLASH_CONTROL_BY_PSAM          0xAA  /**< \brief PSAM 配置文件控制模式 */
#define UARTCMD_FLASH_CONTROL_BY_USER_CARD     0x55  /**< \brief 用户卡配置文件控制模式 */
#define UARTCMD_FLASH_CONTROL_BY_READER        0x00  /**< \brief 读卡器控制模式 */

/** @} */

/**
 * \name 数据输出模式
 * @{
 */

/** \brief 数据输出大小端模式掩码 */
#define UARTCMD_FLASH_DATA_ENDIAN_MASK       0x80

#define UARTCMD_FLASH_DATA_ENDIAN_LITTLE     0x00  /**< \brief 小端模式(默认) */
#define UARTCMD_FLASH_DATA_ENDIAN_BIG        0x80  /**< \brief 大端模式 */

/** \brief 数据输出处理模式掩码 */
#define UARTCMD_FLASH_DATA_OUTPUT_MASK       0x70

#define UARTCMD_FLASH_DATA_OUTPUT_NO         0x00  /**< \brief 数据不处理 */
#define UARTCMD_FLASH_DATA_OUTPUT_SEGMENT    0x10  /**< \brief 数据分段 */
#define UARTCMD_FLASH_DATA_OUTPUT_CROSS      0x20  /**< \brief 数据交叉 */

/** \brief 数据输出加密模式掩码 */
#define UARTCMD_FLASH_DATA_ENCRYPT_MASK      0x0F

#define UARTCMD_DATA_ENCRYPT_NO              0x00  /**< \brief 不加密 */
#define UARTCMD_DATA_ENCRYPT_1_DES           0x01  /**< \brief 1-DES */
#define UARTCMD_DATA_ENCRYPT_3_DES           0x02  /**< \brief 3-DES */
#define UARTCMD_DATA_ENCRYPT_SM1             0x03  /**< \brief SM1 */
#define UARTCMD_DATA_ENCRYPT_ROLLCODE        0x08  /**< \brief 滚码加密 */
#define UARTCMD_DATA_ENCRYPT_RC5             0x09  /**< \brief RC5 加密 */
#define UARTCMD_DATA_ENCRYPT_AES             0x0A  /**< \brief AES 加密 */

/** @} */

/**
 * \name 卡的类型
 * @{
 */

#define UARTCMD_FLASH_CARD_TYPE_ISO14443A     0x3F  /**< \brief ISO14443A */
#define UARTCMD_FLASH_CARD_TYPE_ISO14443B     0x5F  /**< \brief ISO14443B */
#define UARTCMD_FLASH_CARD_TYPE_ISO115693     0x7F  /**< \brief ISO115693 */
#define UARTCMD_FLASH_CARD_TYPE_ISO7816_3     0x9F  /**< \brief ISO7816_3 */
#define UARTCMD_FLASH_CARD_TYPE_INVALID       0xFF  /**< \brief 无效卡/无效的SAK */

#define UARTCMD_FLASH_CARD_TYPE_MF0           0x00  /**< \brief MF0系列卡 */
#define UARTCMD_FLASH_CARD_TYPE_UL            0x01  /**< \brief MF0 UL系列卡 */
#define UARTCMD_FLASH_CARD_TYPE_ULC           0x02  /**< \brief MF0 ULC系列卡 */

#define UARTCMD_FLASH_CARD_TYPE_MF1           0x08  /**< \brief MF1系列卡 */
#define UARTCMD_FLASH_CARD_TYPE_MINI          0x09  /**< \brief MF1 mini系列卡 */
#define UARTCMD_FLASH_CARD_TYPE_S50           0x0A  /**< \brief MF1 s50系列卡 */
#define UARTCMD_FLASH_CARD_TYPE_S70           0x0B  /**< \brief MF1 s70系列卡 */

#define UARTCMD_FLASH_CARD_TYPE_MF3           0x10  /**< \brief MF3系列卡 */
#define UARTCMD_FLASH_CARD_TYPE_D20           0x11  /**< \brief MF3 D20卡 */
#define UARTCMD_FLASH_CARD_TYPE_D21           0x12  /**< \brief MF3 D21卡 */
#define UARTCMD_FLASH_CARD_TYPE_D40           0x13  /**< \brief MF3 D40卡 */
#define UARTCMD_FLASH_CARD_TYPE_D41           0x14  /**< \brief MF3 D41卡 */
#define UARTCMD_FLASH_CARD_TYPE_D80           0x15  /**< \brief MF3 D80卡 */
#define UARTCMD_FLASH_CARD_TYPE_D81           0x16  /**< \brief MF3 D81卡 */

#define UARTCMD_FLASH_CARD_TYPE_MFP           0x18  /**< \brief PLUS CPU 系列卡 */
#define UARTCMD_FLASH_CARD_TYPE_MFP2_SL2      0x19  /**< \brief PLUS CPU 2K(SL2) */
#define UARTCMD_FLASH_CARD_TYPE_MFP4_SL2      0x1A  /**< \brief PLUS CPU 4K(SL2) */
#define UARTCMD_FLASH_CARD_TYPE_MFPS_SL3      0x1B  /**< \brief PLUS S 2/4K(SL3) */
#define UARTCMD_FLASH_CARD_TYPE_MFPX_SL3      0x1C  /**< \brief PLUS X 2/4K(SL3) */

#define UARTCMD_FLASH_CARD_TYPE_ISO14443A_4   0x20  /**< \brief ISO14443-4 TypeA 系列卡 */
#define UARTCMD_FLASH_CARD_TYPE_FM1208        0x21  /**< \brief 复旦微电子FM1208CPU卡 */
#define UARTCMD_FLASH_CARD_TYPE_FM12AG08      0x22  /**< \brief 复旦微电子FM12AG08 国密CPU卡 */

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __UARTCMD_FLASH_DEFINE_H */
