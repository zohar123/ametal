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
 * - 1.00 15-11-17  win, first implementation.
 * \endinternal
 */
#ifndef __AM_UARTCMD_CMD_DEFINE_H
#define __AM_UARTCMD_CMD_DEFINE_H

#ifdef __cplusplus
extern "C" {
#endif

/** \brief 0 -- 从机模式； 其他 -- 主机模式 */
#define UARTCMD_CMD_MST_MODE            0

/** \brief 0 -- 命令帧中安全报文设置无效； 其他 -- 有效 */
#define UARTCMD_CMD_SM_ENABLE           0

#define UARTCMD_CMD_GENERAL_ADDR        0x00     /**< \brief 通用地址(同I2C地址) */
#define UARTCMD_CMD_DEFAULT_ADDR        0xB2     /**< \brief 默认地址0xB2 */
#define UARTCMD_CMD_ADDR_MASK           0xFE     /**< \brief 地址掩码(同I2C地址) */

/**
 * \brief 读写器命令类
 */
typedef enum uartcmd_cmd_class {
    UARTCMD_CMD_CLASS_MANUFACTURER    = 0x00,  /**< \brief 制造商命令（该类命令不对用户开放） */
    UARTCMD_CMD_CLASS_DEVICE          = 0x01,  /**< \brief 设备控制类命令 */
    UARTCMD_CMD_CLASS_MIFARE          = 0x02,  /**< \brief MIFARE 卡类命令(包括USI14443-3A) */
    UARTCMD_CMD_CLASS_ISO7816_3       = 0x05,  /**< \brief ISO7816-3 */
    UARTCMD_CMD_CLASS_ISO14443        = 0x06,  /**< \brief ISO14443（PICC）类命令 */
    UARTCMD_CMD_CLASS_PLUS_CPU        = 0x07,  /**< \brief PLUS CPU 卡类命令 */
    UARTCMD_CMD_CLASS_ISO15693        = 0x08,  /**< \brief ISO15693（VICC）类命令 */
    UARTCMD_CMD_CLASS_ISO18K6C        = 0x09,  /**< \brief ISO18000-6C */
    UARTCMD_CMD_CLASS_ISO18092        = 0x0A,  /**< \biref ISO18092(NFCIP-1) */
    UARTCMD_CMD_CLASS_SGIDC           = 0x0B,  /**< \brief 二代身份证类命令 */

    UARTCMD_CMD_CLASS_COMM            = 0xFF   /**< \brief 模块通信类 */
} uartcmd_cmd_class_t;

/**
 * \brief 制造商类命令代码
 */
typedef enum uartcmd_cmd_manu {
    UARTCMD_CMD_CODE_GET_DEVICE_BASIC_INFO   = ' ',  /**< \brief  0x20 读设备基本信息 */
    UARTCMD_CMD_CODE_GET_PRODUCTION_INFO     = '!',  /**< \brief 0x21 读生产信息 */
    UARTCMD_CMD_CODE_GET_TEST_RECORD         = '"',  /**< \brief 0x22 读测试记录 */
    UARTCMD_CMD_CODE_WRITE_PRODUCTION_INFO   = '#',  /**< \brief 0x23 写生产信息 */
    UARTCMD_CMD_CODE_WRITE_TEST_RECORD       = '$',  /**< \brief 0x24 写测试记录 */
    UARTCMD_CMD_CODE_ERASE_TEST_RECORD       = '%',  /**< \brief 0x25 擦除测试记录 */
    UARTCMD_CMD_CODE_MANUFACTURER_AUTH       = '*',  /**< \brief 0x2A 验证厂家密钥 */
    UARTCMD_CMD_CODE_UPDTAE                  = '+'   /**< \brief 0x2B 升级 */
} uartcmd_cmd_manu_t;

/**
 * \brief 设备控制类命令代码
 */
typedef enum uartcmd_cmd_code_dev_ctrl {

    /** \brief 0x41 获取设备信息 */
    UARTCMD_CMD_CODE_GET_DEVICE_INFO            = 'A',

    /** \brief 0x42 配置IC 卡接口 */
    UARTCMD_CMD_CODE_CONFIG_ICC_INTERFACE       = 'B',

    /** \brief 0x43 关闭IC 卡接口 */
    UARTCMD_CMD_CODE_CLOSE_ICC_INTERFACE        = 'C',

    /** \brief 0x44 设置IC 卡接口协议（工作模式） */
    UARTCMD_CMD_CODE_SET_ICC_INTERFACE_PROTOCOL = 'D',

    /** \brief 0x45 装载IC 卡密钥 */
    UARTCMD_CMD_CODE_LOAD_ICC_KEY               = 'E',

    /** \brief 0x46 设置IC 卡接口的寄存器值 */
    UARTCMD_CMD_CODE_SET_ICC_INTERFACE_REG      = 'F',

    /** \brief 0x47 获取IC 卡接口的寄存器值 */
    UARTCMD_CMD_CODE_GET_ICC_INTERFACE_REG      = 'G',

    /** \brief 0x48 设置波特率 */
    UARTCMD_CMD_CODE_SET_BAUD_RATE              = 'H',

    /** \brief 0x49 设置IC 卡接口的驱动方式输出(X1和Tx2的驱动方式) */
    UARTCMD_CMD_CODE_SET_ICC_INTERFACE_OUTPUT   = 'I',

    /** \brief 0x4A 设置I/O */
    UARTCMD_CMD_CODE_SET_IO                     = 'J',

    /** \brief 0x4B 设置新旧帧格式 */
    //UARTCMD_CMD_CODE_SET_FRAME_FORMAT           = 'K',

    /** \brief 0x4D 获取I/O 状态 */
    UARTCMD_CMD_CODE_GET_IO                     = 'M',

    /** \brief 0x4F 设置当前操作的IC卡接口 */
    UARTCMD_CMD_CODE_SET_CUR_ICC_INTERFACE      = 'O',

    /** \brief 0x50 获取当前操作的IC卡接口 */
    UARTCMD_CMD_CODE_GET_CUR_ICC_INTERFACE      = 'P',

    /** \brief 0x52 IC 卡接口复位 */
    UARTCMD_CMD_CODE_ICC_INTERFACE_RESET        = 'R',

    /** \brief 0x53 数据块交互 与卡（包括标签）间数据交互 */
    UARTCMD_CMD_CODE_EXCHANGE_BLOCK             = 'S',

    /** \brief 0x55 设置设备工作模式 */
    //UARTCMD_CMD_CODE_SET_DEVICE_MODE            = 'U',

    /** \brief 0x56 获取设备工作模式 */
    //UARTCMD_CMD_CODE_GET_DEVICE_MODE            = 'V',

    /** \brief 0x56 获取自动检测的回应(需要发命令) */
    UARTCMD_CMD_CODE_GET_AUTO_DECTECT_RESPONSE  = 'W',

    /** \brief 0x58 用户认证读卡器(分为明文验证和密文验证) */
    UARTCMD_CMD_CODE_USER_AUTH                  = 'Y',

    /** \brief 0x61 装载用户密钥    认证模块密钥或通讯加密密钥 */
    UARTCMD_CMD_CODE_LOAD_USER_KEY              = 'a',

    /** \brief 0x62 读EEPROM */
    UARTCMD_CMD_CODE_READ_E2                    = 'b',

    /** \brief 0x63 写EEPROM */
    UARTCMD_CMD_CODE_WRITE_E2                   = 'c',

    /** \brief 获取读卡芯片复位次数 */
    //UARTCMD_CMD_CODE_GET_RESET_COUNT            = 'd',
    
    /** \brief 选择天线 */
    UARTCMD_CMD_CODE_ANTE_SELECT                = 'e',

} uartcmd_cmd_code_dev_ctrl_t;

/**
 * \brief Mifare卡（MF0/1）类命令代码
 */
typedef enum uartcmd_cmd_code_mifare {
    UARTCMD_CMD_CODE_MF_REQUEST           = 'A', /**< \brief 0x41 请求 */
    UARTCMD_CMD_CODE_MF_ANTICOLL          = 'B', /**< \brief 0x42 防碰撞 */
    UARTCMD_CMD_CODE_MF_SELECT            = 'C', /**< \brief 0x43 选择 */
    UARTCMD_CMD_CODE_MF_HALT              = 'D', /**< \brief 0x44 卡挂起 */
    UARTCMD_CMD_CODE_MF_AUTH_E2           = 'E', /**< \brief 0x45 证实E2密钥验证 */
    UARTCMD_CMD_CODE_MF_AUTH_KEY          = 'F', /**< \brief 0x46 证实直接密钥验证 */
    UARTCMD_CMD_CODE_MF_READ              = 'G', /**< \brief 0x47 读 */
    UARTCMD_CMD_CODE_MF_WRITE             = 'H', /**< \brief 0x48 写 */
    UARTCMD_CMD_CODE_MF0_WRITE            = 'I', /**< \brief 0x49 写UltraLight */
    UARTCMD_CMD_CODE_MF_VALUE             = 'J', /**< \brief 0x4A 值操作 */
    UARTCMD_CMD_CODE_MF_RESET             = 'L', /**< \brief 0x4C 卡复位 */
    UARTCMD_CMD_CODE_MF_ACTIVE            = 'M', /**< \brief 0x4D 卡激活 */
    UARTCMD_CMD_CODE_MF_AUTO_DETECT       = 'N', /**< \brief 0x4E 自动检测 */
    UARTCMD_CMD_CODE_MF_GET_AUTO_DETECT   = 'O', /**< \brief 0x4F 读自动检测数据 */
    UARTCMD_CMD_CODE_MF_SET_VALUE         = 'P', /**< \brief 0x50 设置值块的值 */
    UARTCMD_CMD_CODE_MF_GET_VALUE         = 'Q', /**< \brief 0x51 获取值块的值 */
    UARTCMD_CMD_CODE_EXCHANGE_BLOCK1      = 'X', /**< \brief 0x58 数据交互命令 */
    UARTCMD_CMD_CODE_MF0_EXTEND           = 'S'  /**< \brief MF0卡片与PCD之间数据交互扩展命令 */
} uartcmd_cmd_code_mifare_t;

/**
 * \brief ISO7816-3 协议类命令代码
 */
typedef enum uartcmd_cmd_code_iso7816_3 {
    UARTCMD_CMD_CODE_CICC_RESET         = 'A', /**< \brief 0x41 接触式IC卡复位(自动处理PPS) */
    UARTCMD_CMD_CODE_CICC_TPDU          = 'B', /**< \brief 0x42 接触式IC卡传输协议（自动处理T=0和T=1协议）*/
    UARTCMD_CMD_CODE_CICC_COLD_RESET    = 'C', /**< \brief 0x43 接触式IC卡冷复位 */
    UARTCMD_CMD_CODE_CICC_WARM_RESET    = 'D', /**< \brief 0x44 接触式IC卡热复位 */
    UARTCMD_CMD_CODE_CICC_DEACTIVATION  = 'E', /**< \brief 0x45 接触式IC卡停活（关闭电源和时钟）*/
    UARTCMD_CMD_CODE_CICC_PPS           = 'F', /**< \brief 0x46 接触式IC卡PPS(传输协议协商) */
    UARTCMD_CMD_CODE_CICC_TP0           = 'G', /**< \brief 0x47 接触式IC卡T=0传输协议 */
    UARTCMD_CMD_CODE_CICC_TP1           = 'H', /**< \brief 0x48 接触式IC卡T=1传输协议 */
} uartcmd_cmd_code_iso7816_3_t;


/**
 * \brief ISO14443协议类命令代码
 */
typedef enum uartcmd_cmd_code_iso14443 {
    UARTCMD_CMD_CODE_PICCA_REQUEST          = 'A',  /**< \brief 0x41 A型卡请求 */
    UARTCMD_CMD_CODE_PICCA_ANTICOLL         = 'B',  /**< \brief 0x42 A型卡防碰撞 */
    UARTCMD_CMD_CODE_PICCA_SELECT           = 'C',  /**< \brief 0x43 A型卡选择 */
    UARTCMD_CMD_CODE_PICCA_HALT             = 'D',  /**< \brief 0x44 A型卡挂起 */
    UARTCMD_CMD_CODE_PICCA_RATS             = 'E',  /**< \brief 0x45 A型卡RATS */
    UARTCMD_CMD_CODE_PICCA_PPS              = 'F',  /**< \brief 0x46 A型卡PPS */
    UARTCMD_CMD_CODE_PICC_DESELECT          = 'G',  /**< \brief 0x47 DESELECT */
    UARTCMD_CMD_CODE_PICC_TPCL              = 'H',  /**< \brief 0x48 T=CL */
    UARTCMD_CMD_CODE_PICC_EXCHANGE_BLOCK2   = 'J',  /**< \brief 0x4B 数据交换 */

    /** \brief 0x4C A型卡复位（若支持ISO14443-4, 则自动执行RATS）*/
    UARTCMD_CMD_CODE_PICCA_RESET            = 'L',

    UARTCMD_CMD_CODE_PICCA_ACTIVE           = 'M',  /**< \brief 0x4D A型卡激活 */
    UARTCMD_CMD_CODE_PICCB_ACTIVE           = 'N',  /**< \brief 0x4E B型卡激活 */
    UARTCMD_CMD_CODE_PICCB_RESET            = 'O',  /**< \brief 0x4F B型卡复位（执行ATTRIB） */
    UARTCMD_CMD_CODE_PICCB_REQUEST          = 'P',  /**< \brief 0x50 B型卡请求 */
    UARTCMD_CMD_CODE_PICCB_SLOT_MARKER      = 'Q',  /**< \brief 0x51 B型卡防碰撞 */
    UARTCMD_CMD_CODE_PICCB_ATTRIB           = 'R',  /**< \brief 0x52 B型卡修改传输属性（选择卡） */
    UARTCMD_CMD_CODE_PICCB_HALT             = 'S',  /**< \brief 0x53 B型卡挂起 */
    UARTCMD_CMD_CODE_PICCB_GETUID           = 'T'   /**< \brief 0x54 获取二代证UID */
} uartcmd_cmd_code_iso14443_t;

/**
 * \brief ISO15693协议类命令代码
 */
typedef enum uartcmd_cmd_code_iso15693 {
    UARTCMD_CMD_CODE_VICC_INVENTORY             = 'A', /**< \brief 0x41 目录 */
    UARTCMD_CMD_CODE_VICC_STAY_QUIET            = 'B', /**< \brief 0x42 保持静默 */
    UARTCMD_CMD_CODE_VICC_READ_BLOCK            = 'C', /**< \brief 0x43 读数据块 */
    UARTCMD_CMD_CODE_VICC_WRITE_BLOCK           = 'D', /**< \brief 0x44 写数据块 */
    UARTCMD_CMD_CODE_VICC_LOCK_BLOCK            = 'E', /**< \brief 0x45 锁定块 */
    UARTCMD_CMD_CODE_VICC_SELECT                = 'F', /**< \brief 0x46 选择 */
    UARTCMD_CMD_CODE_VICC_RESET_READY           = 'H', /**< \brief 0x48 复位到准备好状态 */
    UARTCMD_CMD_CODE_VICC_WRITE_AFI             = 'I', /**< \brief 0x49 写AFI */
    UARTCMD_CMD_CODE_VICC_WRITE_DSFID           = 'K', /**< \brief 0x4B 写DSFID */
    UARTCMD_CMD_CODE_VICC_LOCK_DSFID            = 'L', /**< \brief 0x4C 锁定DSFID */
    UARTCMD_CMD_CODE_VICC_GET_SYS_INFO          = 'M', /**< \brief 0x4D 获取系统信息 */
    UARTCMD_CMD_CODE_VICC_GET_SECURITY_STATUS   = 'N'  /**< \brief 0x4E 获取安全状态 */
} uartcmd_cmd_code_iso15693_t;

/**
 * \brief 二代身份证类命令代码
 */
typedef enum uartcmd_cmd_code_2g_id_card {
    UARTCMD_CMD_CODE_SGIDC_RESET     = 'A',  /**< \brief 0x41   二代身份证复位 */
    UARTCMD_CMD_CODE_SGIDC_GET_UID   = 'B'   /**< \brief 0x42   读二代身份证ID号 */
} uartcmd_cmd_code_2g_id_card_t;

/**
 * \brief PLUS CPU 卡类命令代码（不包括和MF1部分的命令，以及激活等命令）
 */
typedef enum uartcmd_cmd_code_plus_cpu {

    /** \brief 0x42 SL0 个人化更新数据 */
    UARTCMD_CMD_CODE_SL0_WRITE_PERSO        = 'B',

    /** \brief 0x43 SL0 提交个人化 */
    UARTCMD_CMD_CODE_SL0_COMMIT_PERSO       = 'C',

    /** \brief 0x44 SL2 AES验证（直接密钥验证） */
    UARTCMD_CMD_CODE_SL2_AES_AUTH           = 'D',

    /** \brief 0x45 SL2 AES验证（E2密钥验证） */
    UARTCMD_CMD_CODE_SL2_AES_AUTH_E2        = 'E',

    /** \brief 0x46 SL2 CRYPTO 1验证（直接密钥验证） */
    UARTCMD_CMD_CODE_SL2_CRYPTO1_AUTH       = 'F',

    /** \brief 0x47 SL2 CRYPTO 1验证（E2密钥验证） */
    UARTCMD_CMD_CODE_SL2_CRYPTO1_AUTH_E2    = 'G',

    /** \brief 0x48 SL2 多块读 */
    UARTCMD_CMD_CODE_SL2_READ               = 'H',

    /** \brief 0x49 SL2 多块写 */
    UARTCMD_CMD_CODE_SL2_WRITE              = 'I',

    /** \brief 0x4A SL3 首次验证（直接密钥验证） */
    UARTCMD_CMD_CODE_SL3_FIRST_AUTH         = 'J',

    /** \brief 0x4B SL3 首次验证（E2密钥验证） */
    UARTCMD_CMD_CODE_SL3_FIRST_AUTH_E2      = 'K',

    /** \brief 0x4C SL3 跟随验证（直接密钥验证） */
    UARTCMD_CMD_CODE_SL3_FOLLOWING_AUTH     = 'L',

    /** \brief 0x4D SL3 跟随验证（E2密钥验证） */
    UARTCMD_CMD_CODE_SL3_FOLLOWING_AUTH_E2  = 'M',

    /** \brief 0x4E SL3 复位验证 */
    UARTCMD_CMD_CODE_SL3_RESET_AUTH         = 'N',

    /** \brief 0x4F SL3 读数据块 */
    UARTCMD_CMD_CODE_SL3_READ               = 'O',

    /** \brief 0x50 SL3 写数据块 */
    UARTCMD_CMD_CODE_SL3_WRITE              = 'P',

    /** \brief 0x51 SL3 写数据块 */
    UARTCMD_CMD_CODE_SL3_SET_VALUE          = 'Q',

    /** \brief 0x52 SL3 读数据块 */
    UARTCMD_CMD_CODE_SL3_GET_VALUE          = 'R',

    /** \brief 0x53 SL3 值块操作 */
    UARTCMD_CMD_CODE_SL3_VALUE              = 'S',

    /** \brief 0x54 SL3 中继攻击检测 */
    UARTCMD_CMD_CODE_SL3_PROX_CHECK         = 'T',

    /** \brief 0x55 SL3 虚拟卡支持 */
    UARTCMD_CMD_CODE_SL3_VCS                = 'U',

    /** \brief 0x56 SL3 虚拟卡支持(可获取UID) */
    UARTCMD_CMD_CODE_SL3_VCSL               = 'V',

    /** \brief 0x57 SL3 虚拟卡选择卡 */
    UARTCMD_CMD_CODE_SL3_SVC                = 'W',

    /** \brief 0x58 SL3 虚拟卡取消选择 */
    UARTCMD_CMD_CODE_SL3_DESVC              = 'X'

} uartcmd_cmd_code_plus_cpu_t;

/**
 * \brief ISO18000-6C类命令代码
 */
typedef enum uartcmd_cmd_code_iso18000_6c {

    /** \brief 0x41   设置查询（Query）参数 */
    UARTCMD_CMD_CODE_SET_QUERY_PARAMETER            = 'A',

    /** \brief 0x42   获取查询（Query）参数 */
    UARTCMD_CMD_CODE_GET_QUERY_PARAMETER            = 'B',

    /** \brief 0x43   设置盘存（Inventory）时的Singulation算法 */
    UARTCMD_CMD_CODE_SET_SINGULATION_ALGORITHM      = 'C',

    /** \brief 0x44   获取盘存（Inventory）时的Singulation算法 */
    UARTCMD_CMD_CODE_GET_SINGULATION_ALGORITHM      = 'D',

    /** \brief 0x45   设置Singulation算法参数 */
    UARTCMD_CMD_CODE_SET_SINGULATION_PARAMETER      = 'E',

    /** \brief 0x46   获取Singulation算法参数 */
    UARTCMD_CMD_CODE_GET_SINGULATION_PARAMETER      = 'F',

    /** \brief 0x48   获取盘存操作结果 */
    UARTCMD_CMD_CODE_RFID18K6C_GET_INVENTORY        = 'H',

    /** \brief 0x49   获取访问的回应 */
    UARTCMD_CMD_CODE_RFID18K6C_GET_ACCESS_RESPONSE  = 'I',

    /** \brief 0x4A   启动盘存操作 */
    UARTCMD_CMD_CODE_RFID18K6C_START_INVENTORY      = 'J',

    /** \brief 0x4B   启动读操作 */
    UARTCMD_CMD_CODE_RFID18K6C_START_READ           = 'K',

    /** \brief 0x4C   启动写操作 */
    UARTCMD_CMD_CODE_RFID18K6C_START_WRITE          = 'L',

    /** \brief 0x4D   启动灭活操作 */
    UARTCMD_CMD_CODE_RFID18K6C_START_KILL           = 'M',

    /** \brief 0x4E   启动锁操作 */
    UARTCMD_CMD_CODE_RFID18K6C_START_LOCK           = 'N',

    /** \brief 0x4F   启动擦除 */
    UARTCMD_CMD_CODE_RFID18K6C_START_ERASE          = 'O',

    /** \brief 0x50   终止当前的操作 */
    UARTCMD_CMD_CODE_RFID18K6C_ABORT                = 'P'

} uartcmd_cmd_code_iso18000_6c_t;

#ifdef __cplusplus
}
#endif

#endif /* __UARTCMD_CMD_DEFINE_H */

/* end of file */
