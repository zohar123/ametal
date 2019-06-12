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
 * \brief SDCard标准接口
 *
 * \internal
 * \par Modification history
 * - 1.00 19-06-04  ipk, first implementation.
 * \endinternal
 */

#ifndef __AM_SDCARD_H
#define __AM_SDCARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_sdio.h"
#include "am_wait.h"

/**
 * \brief Supported SD Memory Cards
 */
#define AM_SDIO_STD_CAPACITY_SD_CARD_V1_1          0x00
#define AM_SDIO_STD_CAPACITY_SD_CARD_V2_0          0x01
#define AM_SDIO_HIGH_CAPACITY_SD_CARD              0x02
#define AM_SDIO_MULTIMEDIA_CARD                    0x03
#define AM_SDIO_SECURE_DIGITAL_IO_CARD             0x04
#define AM_SDIO_HIGH_SPEED_MULTIMEDIA_CARD         0x05
#define AM_SDIO_SECURE_DIGITAL_IO_COMBO_CARD       0x06
#define AM_SDIO_HIGH_CAPACITY_MMC_CARD             0x07

#define AM_SD_OCR_ERRORBITS                        0xFDFFE008

/**
 * \name OCR寄存器
 * \anchor grp_awbl_sd_ocr_range
 * @{
 */

/** \brief VDD 电压 1.65 - 1.95 */
#define AM_SD_OCR_VDD_165_195    0x00000080

/** \brief VDD 电压 2.0 ~ 2.1 */
#define AM_SD_OCR_VDD_20_21      0x00000100

/** \brief VDD 电压 2.1 ~ 2.2 */
#define AM_SD_OCR_VDD_21_22      0x00000200

/** \brief VDD 电压 2.2 ~ 2.3 */
#define AM_SD_OCR_VDD_22_23      0x00000400

/** \brief VDD 电压 2.3 ~ 2.4 */
#define AM_SD_OCR_VDD_23_24      0x00000800

/** \brief VDD 电压 2.4 ~ 2.5 */
#define AM_SD_OCR_VDD_24_25      0x00001000

/** \brief VDD 电压 2.5 ~ 2.6 */
#define AM_SD_OCR_VDD_25_26      0x00002000

/** \brief VDD 电压 2.6 ~ 2.7 */
#define AM_SD_OCR_VDD_26_27      0x00004000

/** \brief VDD 电压 2.7 ~ 2.8 */
#define AM_SD_OCR_VDD_27_28      0x00008000

/** \brief VDD 电压 2.8 ~ 2.9 */
#define AM_SD_OCR_VDD_28_29      0x00010000

/** \brief VDD 电压 2.9 ~ 3.0 */
#define AM_SD_OCR_VDD_29_30      0x00020000

/** \brief VDD 电压 3.0 ~ 3.1 */
#define AM_SD_OCR_VDD_30_31      0x00040000

/** \brief VDD 电压 3.1 ~ 3.2 */
#define AM_SD_OCR_VDD_31_32      0x00080000

/** \brief VDD 电压 3.2 ~ 3.3 */
#define AM_SD_OCR_VDD_32_33      0x00100000

/** \brief VDD 电压 3.3 ~ 3.4 */
#define AM_SD_OCR_VDD_33_34      0x00200000

/** \brief VDD 电压 3.4 ~ 3.5 */
#define AM_SD_OCR_VDD_34_35      0x00400000

/** \brief VDD 电压 3.5 ~ 3.6 */
#define AM_SD_OCR_VDD_35_36      0x00800000

/** \brief 1.8V switching request */
#define AM_SD_OCR_S18R           0x01000000

/** \brief SDXC power control */
#define AM_SD_OCR_XPC            0x10000000

/** \brief Card Capacity Status (CCS)1 */
#define AM_SD_OCR_CCS            0x40000000
/** @} */

/**
 * \brief  SD Card States
 */
typedef enum {
  AM_SD_CARD_READY                  = 0x01,
  AM_SD_CARD_IDENTIFICATION         = 0x02,
  AM_SD_CARD_STANDBY                = 0x03,
  AM_SD_CARD_TRANSFER               = 0x04,
  AM_SD_CARD_SENDING                = 0x05,
  AM_SD_CARD_RECEIVING              = 0x06,
  AM_SD_CARD_PROGRAMMING            = 0x07,
  AM_SD_CARD_DISCONNECTED           = 0x08,
  AM_SD_CARD_ERROR                  = 0xFF
}am_sd_card_state;

/**
 * \brief  Card Specific Data: CSD Register
 */
typedef struct am_sdcard_csd {
   uint8_t  csd_struct;             /**< \brief CSD structure */
   uint8_t  sys_specversion;        /**< \brief System specification version */
   uint8_t  Reserved1;              /**< \brief Reserved */
   uint8_t  taac;                   /**< \brief Data read access-time 1 */
   uint8_t  nsac;                   /**< \brief Data read access-time 2 in CLK cycles */
   uint8_t  max_bus_clkfrec;        /**< \brief Max. bus clock frequency */
   uint16_t card_comd_classes;      /**< \brief Card command classes */
   uint8_t  rdblocklen;             /**< \brief Max. read data block length */
   uint8_t  part_block_read;        /**< \brief Partial blocks for read allowed */
   uint8_t  wrblock_misalign;       /**< \brief Write block misalignment */
   uint8_t  rdblock_misalign;       /**< \brief Read block misalignment */
   uint8_t  dsrimpl;                /**< \brief DSR implemented */
   uint8_t  Reserved2;              /**< \brief Reserved */
   uint32_t device_size;            /**< \brief Device Size */
   uint8_t  max_rd_current_vddmin;  /**< \brief Max. read current @ VDD min */
   uint8_t  max_rd_current_vddmax;  /**< \brief Max. read current @ VDD max */
   uint8_t  max_wr_current_vddmin;  /**< \brief Max. write current @ VDD min */
   uint8_t  max_wr_current_vddmax;  /**< \brief Max. write current @ VDD max */
   uint8_t  device_size_mul;        /**< \brief Device size multiplier */
   uint8_t  erase_grsize;           /**< \brief Erase group size */
   uint8_t  erase_grmul;            /**< \brief Erase group size multiplier */
   uint8_t  wrprotect_grsize;       /**< \brief Write protect group size */
   uint8_t  wrprotect_grenable;     /**< \brief Write protect group enable */
   uint8_t  man_deflecc;            /**< \brief Manufacturer default ECC */
   uint8_t  wrspeed_fact;           /**< \brief Write speed factor */
   uint8_t  max_wrblocklen;         /**< \brief Max. write data block length */
   uint8_t  write_block_pa_partial; /**< \brief Partial blocks for write allowed */
   uint8_t  reserved3;              /**< \brief Reserded */
   uint8_t  content_protect_appli;  /**< \brief Content protection application */
   uint8_t  file_format_grouop;     /**< \brief File format group */
   uint8_t  copy_flag;              /**< \brief Copy flag (OTP) */
   uint8_t  perm_wr_protect;        /**< \brief Permanent write protection */
   uint8_t  temp_wr_protect;        /**< \brief Temporary write protection */
   uint8_t  file_format;            /**< \brief File Format */
   uint8_t  ecc;                    /**< \brief ECC code */
   uint8_t  csd_crc;                /**< \brief CSD CRC */
   uint8_t  reserved4;              /**< \brief always 1*/
} am_sdcard_csd_t;

/**
 * \brief  Card Identification Data: CID Register
 */
typedef struct am_sdcard_cid {
   uint8_t  manu_facturer_id;      /**< \brief ManufacturerID */
   uint16_t oem_appli_id;          /**< \brief OEM/Application ID */
   uint32_t prod_name1;            /**< \brief Product Name part1 */
   uint8_t  prod_name2;            /**< \brief Product Name part2*/
   uint8_t  prod_rev;              /**< \brief Product Revision */
   uint32_t prod_sn;               /**< \brief Product Serial Number */
   uint8_t  reserved1;             /**< \brief Reserved1 */
   uint16_t manufact_date;         /**< \brief Manufacturing Date */
   uint8_t  cid_crc;               /**< \brief CID CRC */
   uint8_t  reserved2;             /**< \brief always 1 */
} am_sdcard_cid_t;

/**
 * \brief SD Card information
 */
typedef struct {
    am_sdcard_csd_t csd;
    am_sdcard_cid_t cid;
    uint64_t        capacity;     /**< \brief Card Capacity */
    uint32_t        block_size;   /**< \brief Card Block Size */
    uint16_t        rca;
    uint32_t        type;
} am_sdcard_info_t;

/**
 * \brief SDCARD 设备信息结构体
 */
typedef struct am_sdcard_devinfo{

    /** \brief 从设备的SDIO模式标志，请参考“SDIO模式标志”*/
    uint16_t         mode;

    uint16_t         blk_size;

    uint32_t         ocr_valid;        /**< \brief 支持的ORC（电压范围） */

}am_sdcard_devinfo_t;

/**
 * \brief SDCARD 设备结构体
 */
typedef struct am_sdcard_dev{
    am_sdio_handle_t        sdio_handle;
    am_sdio_device_t        sdio_dev;
    am_sdcard_devinfo_t    *p_devinfo;
    am_sdcard_info_t        sdcard_info;
    am_wait_t               wait;
}am_sdcard_dev_t;

/** \brief SDIO 标准服务操作句柄定义 */
typedef am_sdcard_dev_t *am_sdcard_handle_t;

/**
 * \brief SD Card 复位
 *
 * \param[in]  handle       : SD Card 句柄
 *
 * \retval AM_OK            : 复位成功
 */
int am_sdcard_rest (am_sdcard_handle_t handle);

/**
 * \brief 应用程序初始化(CMD8)
 *
 * \param[in]   handle      : SDIO设备
 * \param[in]   ocr         : SDIO HOST支持的OCR范围
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
int am_sdcard_send_if_cond (am_sdcard_handle_t handle,
                            uint32_t           ocr);

/**
 * \brief 应用程序初始化(CMD55 + ACMD41)
 *
 * \param[in]   handle      : SDIO设备
 * \param[in]   rca         : 相对地址
 * \param[in]   ocr         : SDIO HOST支持的OCR范围
 * \param[in]   p_rocr      : 获取到SDIO设备支持的OCR范围
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
int am_sdcard_send_app_op_cond (am_sdcard_handle_t handle,
                                uint32_t           rca,
                                uint32_t           ocr,
                                uint32_t          *p_rocr);

/**
 * \brief 获取存储卡所有的CID(CMD2)
 *
 * \param[in]   handle      : SDIO设备
 * \param[in]   p_cid       : 寄存器cid所有数据
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
int am_sdcard_all_cid_get (am_sdcard_handle_t handle,
                           uint32_t          *p_cid);

/**
 * \brief 获取存储卡相对地址(CMD3)
 *
 * \param[in]   handle      : SDIO设备
 * \param[out]  p_rca       : 存储卡返回的rca值
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
int am_sdio_relative_addr_get (am_sdcard_handle_t handle,
                               uint16_t          *p_rca);

/**
 * \brief 选定指定相对地址的卡(CMD7)
 *
 * \param[in]   p_card      : SDIO设备
 * \param[in]   rca         : 存储卡相对地址
 * \param[out]  select      : 选择卡（1） / 取消卡（ 0）
 *
 * \retval AW_OK            : 传输成功
 * \retval -AW_ENOMEM       : 内存空间不足
 * \retval -AW_ECANCELED    : 操作被取消
 * \retval -AW_EIO          : SDIO总线I/O错误
 * \retval -AW_EBUSY        : SDIO HOST忙，正在处理其他msg
 * \retval -AW_EINVAL       : 参数错误
 * \retval -AW_ETIME        : HOST处理超时
 * \retval -AW_ENOTSUP      : HOST不支持该功能
 */
int am_sdcard_card_select (am_sdcard_handle_t handle,
                           uint32_t           rca,
                           uint8_t            select);

/**
 * \brief 获取存储卡寄存器CSD值(CMD9)
 *
 * \param[in]   handle      : SDIO设备
 * \param[in]   rca         : 存储卡相对地址
 * \param[out]  p_csd       : 寄存器csd值
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
int aw_sdcard_csd_get (am_sdcard_handle_t handle,
                       uint32_t           rca,
                       uint32_t          *p_csd);

/**
 * \brief 设置存储卡总线宽度(CMD55 + ACMD6)
 *
 * \param[in]  handle       : SDIO设备
 * \param[in]  rca          : 存储卡相对地址
 * \param[out] width        : 总线宽度
 *
 * \retval AW_OK            : 传输成功
 * \retval -AW_ENOMEM       : 内存空间不足
 * \retval -AW_ECANCELED    : 操作被取消
 * \retval -AW_EIO          : SDIO总线I/O错误
 * \retval -AW_EBUSY        : SDIO HOST忙，正在处理其他msg
 * \retval -AW_EINVAL       : 参数错误
 * \retval -AW_ETIME        : HOST处理超时
 * \retval -AW_ENOTSUP      : HOST不支持该功能
 */
int am_sdcard_bus_width_set (am_sdcard_handle_t  handle,
                             uint32_t            rca,
                             am_sdio_bus_width_t width);

/**
 * \brief SD Card 设置块大小
 *
 * \param[in]  handle       : SD Card 句柄
 * \param[in]  blk_size     : 需要设置的块大小
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
int am_sdcard_block_size_set(am_sdcard_handle_t handle,
                             uint32_t           blk_size);

/**
 * \brief 同步从存储卡中读取数据块
 *
 * \param[in]  handle       : SDIO设备
 * \param[in]  p_buf        : 数据缓冲区
 * \param[in]  blk_start    : 起始块号
 * \param[in]  blk_num      : 块个数
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
int am_sdcard_blocks_read (am_sdcard_handle_t handle,
                           uint8_t           *p_buf,
                           uint32_t           blk_start,
                           uint32_t           blk_num);

/**
 * \brief 同步从存储卡中写入数据块
 *
 * \param[in]  handle       : SD Card 句柄
 * \param[in]  p_buf        : 数据缓冲区
 * \param[in]  blk_start    : 起始块号
 * \param[in]  blk          : 需要写入的块数量
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
int am_sdcard_blocks_write (am_sdcard_handle_t  handle,
                            uint8_t            *p_buf,
                            uint32_t            blk_start,
                            uint32_t            blk_num);

/**
 * \brief 擦除块(CMD38)
 *
 * \param[in]  handle       : SD Card 句柄
 * \param[in]  blk_start    : 需要擦除的起始块号
 * \param[in]  blk_num      : 需要擦除的块数量
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
int am_sdcard_blocks_erase (am_sdcard_handle_t handle,
                            uint32_t           blk_start,
                            uint32_t           blk_num);

/**
 * \brief 停止传输(CMD12)
 *
 * \param[in]  handle       : SD Card 句柄
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
int am_sdcard_transfer_stop (am_sdcard_handle_t handle);


int am_sdcard_sdinfo_get (am_sdcard_handle_t handle,
                          am_sdcard_info_t  *p_sdinfo);

/**
 * \brief 获取卡状态(CMD13)
 *
 * \param[in]  handle       : SDIO设备
 * \param[in]  rca          : 存储卡相对地址
 * \param[out] p_status     : 存储卡状态
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
int am_sdcard_status_reg_get (am_sdcard_handle_t handle,
                              uint32_t           rca,
                              uint32_t          *p_status);

/**
 * \brief SDCard 设备初始化
 *
 * \param[in]  p_dev       : SDCard设备
 * \param[in]  p_devinfo   : SDCard设备信息结构体指针
 * \param[out] sdio_handle : SDIO标准句柄
 *
 * \return 成功则返回SDCard设备操作句柄，失败返回NULL
 */
am_sdcard_handle_t am_sdcard_sdio_init(am_sdcard_dev_t      *p_dev,
                                       am_sdcard_devinfo_t  *p_devinfo,
                                       am_sdio_handle_t      sdio_handle);

/**
 * \brief SDCard 设备解初始化
 *
 * \param[in]  handle : SDCard设备操作句柄
 *
 * \retval AM_OK操作成功，否则失败
 */
int am_sdcard_sdio_deinit (am_sdcard_handle_t handle);

#ifdef __cplusplus
}
#endif

#endif /* __AM_SDIO_H */

/*end of file */




