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
#ifndef __AM_UARTCMD_FLASH_H
#define __AM_UARTCMD_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_uartcmd_comm.h"
#include "am_uartcmd_flash_define.h"

#pragma pack(1)
typedef struct uartcmd_auto_detest_info {

    /**
     * \brief 卡片类型
     *        - RC_CARD_TYPE_MF1       -- Mifare S50/70卡
     *        - RC_CARD_TYPE_PLUS      -- PLUS CPU卡
     *        - RC_CARD_TYPE_FM1208    -- FM1208 CPU卡
     *        - RC_CARD_TYPE_FM12AG08  -- FM12AG08 国密CPU卡
     *        - RC_CARD_TYPE_ISO14443A -- ISO14443 TypeA
     *        - RC_CARD_TYPE_ISO14443B -- ISO14443 TypeB
     *        - RC_CARD_TYPE_ISO115693 -- ISO115693
     *        - RC_CARD_TYPE_ISO7816_3 -- ISO7816_3
     */
    uint8_t card_type;

    /**
     * \brief 自动检测模式
     *        - nMode.4:7    -- 保留，请勿写1
     *        - nMode.3      -- 是否执行Halt()命令
     *        - 0            -- 无动作
     *        - 1            -- 执行Halt()
     *        - nMode.2      -- 输出后是否继续检测
     *        - 0            -- 不继续检测
     *        - 1            -- 继续检测
     *        - nMode.1      -- 检测到卡时，是否输出中断信号
     *        - 0            -- 不产生中断
     *        - 1            -- 产生中断(数据发送结束后消失;I2C收到本机地址后消失)
     *        - nMode.0      -- 数据发送模式
     *        - 0            -- 串口不发送
     *        - 1            -- 串口主动发送
     */
    uint8_t mode;

    /**
     * \brief 天线模式
     *        - nTxMode.2:7   -- 保留，请勿写1
     *        - nTxMode.0:1   -- 天线驱动模式
     *        - 00            -- TX1、TX2 交替驱动
     *        - 01            -- 仅TX1 驱动
     *        - 10            -- 仅TX2 驱动
     *        - 11            -- TX1、TX2 同时驱动
     * */
    uint8_t tx_mode;

    /**
     * \brief 请求代码
     *         - 0x26      -- IDLE(TypeA)
     *         - 0x52      -- ALL(TypeA)
     *         - 0x00      -- IDLE(TypeB)
     *         - 0x08      -- ALL(TypeB)
     */
    uint8_t req_code;

    /**
     * \brief 外部认证（卡片获取访问权限）模式
     *        - AuthMode.7              -- 密钥来源
     *        - 0                       -- 外部输入密钥
     *        - 1                       -- 内部密钥
     *        - AuthMode.0:6            -- 密钥类型或算法标识
     *        - RC_ALGORITHM_CRYPTO1_A  -- Mifaer Crypto1 密钥A
     *        - RC_ALGORITHM_CRYPTO1_B  -- Mifaer Crypto1 密钥B
     *        - RC_ALGORITHM_3_DES      -- 3-DES
     *        - RC_ALGORITHM_1_DES      -- 1-DES
     *        - RC_ALGORITHM_SM1        -- SM1
     *        - RC_ALGORITHM_NO         -- 不验证
     *        - 其他                                         -- 保留将来使用
     * */
    uint8_t auth_mode;

    /**
     * \brief 外部认证时对应卡片的KID（仅用于CPU卡）
     */
    uint8_t ext_auth_kid;

    /**
     * \brief 内部认证（验证是否是本系统的卡）模式，见nExAuthMode
     */
    uint8_t inter_auth_mode;

    /**
     * \brief 外部认证时对应卡片的KID（仅用于CPU卡）
     */
    uint8_t inter_auth_kid;

    /**
     * \brief 外部认证密钥(内部E2/PSAM密钥，第1字节为密钥存放扇区)
     */
    uint8_t ext_auth_key[16];

    /**
     * \brief 内部认证密钥(内部E2/PSAM密钥，第1字节为密钥存放扇区)
     */
    uint8_t inter_auth_key[16];

    /** \brief 应用的FID（仅用于CPU卡）*/
    uint16_t app_fid;

    /** \brief 控制模式（保留）*/
    uint8_t ctrl_mode;

    /** \brief 用户卡配置文件的SFI（仅用于CPU卡） */
    uint8_t sfi_cfg_file;

    /** \brief 应用数据存放文件的SFI 或者是数据块的块号 */
    uint8_t app_sfi;

    /** \brief 应用数据存放的偏移量 */
    uint8_t app_offset;

    /** \brief 应用数据的长度 */
    uint8_t app_bytes;

    /**
     * \brief 数据输出模式
     *        - nDataOutMode.7                   -- 大小端模式
     *           - RC_LITTLE_ENDIAN              -- 小端模式
     *           - RC_BIG_ENDIAN                 -- 大端模式
     *        - nDataOutMode.4:6                 -- 数据处理模式
     *           - DATA_OUTPUT_PROCESS_NO        -- 不处理
     *           - DATA_OUTPUT_PROCESS_SEGMENT   -- 分段
     *           - DATA_OUTPUT_PROCESS_CROSS     -- 交叉
     *        - nDataOutMode.0:3                 -- 数据加密模式
     *           - DATA_OUTPUT_ENCRYPT_NO        -- 不加密
     *           - DATA_OUTPUT_ENCRYPT_1_DES     -- 1-DES
     *           - DATA_OUTPUT_ENCRYPT_3_DES     -- 3-DES
     *           - DATA_OUTPUT_ENCRYPT_SM1       -- SM1
     *           - DATA_OUTPUT_ENCRYPT_ROLLCODE  -- 滚码
     *           - DATA_OUTPUT_ENCRYPT_RC5       -- RC5
     *           - DATA_OUTPUT_ENCRYPT_AES       -- AES 加密
     */
    uint8_t data_output_mode;

} uartcmd_auto_detest_info_t;

typedef struct uartcmd_config_info {

    /** \brief 基本信息校验和 */
    uint16_t    check_sum;

    /** \brief 记录的起始地址 */
    uint16_t    record_addr;

    /** \brief 产品固件版本 */
    char        version[UARTCMD_FLASH_VERSION_SIZE];

    /** \brief 序列号 */
    char        serial_num[UARTCMD_FLASH_SN_SIZE];

    /** \brief 模块地址（与I2C地址相同）*/
    uint8_t     module_addr;

    /**
     * \brief 波特率编号
     *        - RC_BAUD_230400    -- 230400bps
     *        - RC_BAUD_172800    -- 172800bps
     *        - RC_BAUD_115200    -- 115200bps
     *        - RC_BAUD_57600     -- 57600bps
     *        - RC_BAUD_38400     -- 38400bps
     *        - RC_BAUD_28800     -- 28800bps
     *        - RC_BAUD_19200     -- 19200bps
     *        - RC_BAUD_9600      -- 9600bps
     */
    uint8_t     baud_num;

    /**
     * \brief 工作模式
     *        - workMode.4:7                    -- 工作模式
     *          - WORK_MODE_SLAVE               -- 从机模式
     *          - WORK_MODE_AUTO_DETEST         -- 自动检测模式
     *        - workMode.0:3                    -- 通信模式
     *          - COMM_AUTO_DETEST              -- 自动检测模式
     *          - COMM_IIC                      -- IIC通信
     *          - COMM_UART                     -- UART通信
     *          - COMM_UART_DEFAULT_BAUD_RATE   -- UART通信(波特率固定为19200)
     *          - COMM_UART_LAST_BAUD_RATE      -- UART通信(波特率固定为上次设置的波特率)
     *          - COMM_WG                       -- WG通信
     */
    uint8_t     work_mode;

    /**
     * \brief 帧格式
     *        - RC_FRAME_FORMAT_OLD       -- 旧帧格式
     *        - RC_FRAME_FORMAT_NEW       -- 新帧格式
     */
    uint8_t     frame_format;

    /** \brief 韦根位数 */
    uint16_t    wiegang_bits;

    /** \brief 韦根周期 */
    uint8_t     wiegang_period;

    /** \brief 韦根脉冲宽度 */
    uint8_t     wiegang_pulse_width;

    /** \brief 自动检测配置信息 */
    uartcmd_auto_detest_info_t auto_cfg;

    /** \brief 测试记录 */
    uint8_t     test_record[UARTCMD_RECORD_SIZE];

    /** \brief 用户密钥 */
    uint8_t     user_key[2][16];

} uartcmd_config_info_t;

/**
 * \brief UARTCMD 固件信息
 */
typedef struct uartcmd_flash_devinfo {
    uartcmd_config_info_t info;
    uint8_t               user_data[UARTCMD_FLASH_USER_MEM_SIZE];
    uint8_t               ic_card_key[16][32];
} uartcmd_flash_devinfo_t;

#pragma pack()

#ifdef __cplusplus
}
#endif

#endif /* __UARTCMD_FLASH_H */
