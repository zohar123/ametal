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
 * \brief SDCard接口
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
 * \name SD卡版本
 * @{
 */
#define AM_SDCARD_VERSION        0x00E0
#define AM_SDCARD_SDV2X          0x0020
#define AM_SDCARD_SDV1X          0x0040
#define AM_SDCARD_MMC            0x0080
/** @} */

/**
 * \name SD卡容量分类
 * @{
 */
#define AM_SDCARD_CAPACITY_VER   0x0300
#define AM_SDCARD_SDSC           0x0100        /** <\brief 标准容量SD储存卡（<2GB） */
#define AM_SDCARD_SDHC           0x0200        /** <\brief 大容量SD储存卡（2GB~32GB） */
#define AM_SDCARD_SDXC           0x0400        /** <\brief 超大容量SD储存卡（32GB~2TB）*/
/** @} */

#define AM_SD_OCR_ERRORBITS      0xFDFFE008

/**
 * \name SD卡传输速率分类
 * @{
 */
#define AN_SDCARD_SPEED_CLASS    0x001F
#define AN_SDCARD_CLASS_0        0x0001        /** <\brief 这类卡片不指定性能 */
#define AN_SDCARD_CLASS_2        0x0002        /** <\brief 不低于2MB/S */
#define AN_SDCARD_CLASS_4        0x0004        /** <\brief 不低于4MB/S */
#define AN_SDCARD_CLASS_6        0x0008        /** <\brief 不低于6MB/S */
#define AN_SDCARD_CLASS_10       0x0010        /** <\brief 不低于10MB/S */
/** @} */

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

/** \brief CID , only in memory card */
typedef struct am_sdcard_cid {
    uint32_t    mid;
    uint16_t    oid;
    char        pnm[8];
    uint8_t     prv;
    uint32_t    psn;
    uint16_t    year;
    uint8_t     month;
    uint8_t     hwrev;
    uint8_t     fwrev;
}am_sdcard_cid_t;

/** \brief SCR register, only in memory card */
typedef struct am_sdcard_scr {
    uint8_t             structure;          /**< \brief structure version */
    uint8_t             sda_vsn;            /**< \brief SDA version */
    uint8_t             sda_spec3;
    uint8_t             bus_widths;         /**< \brief bus width */
#define AM_SD_SCR_BUS_WIDTH_1  (1<<0)
#define AM_SD_SCR_BUS_WIDTH_4  (1<<2)
    uint8_t             cmds;               /**< \brief CMD support */
#define AM_SD_SCR_CMD20_SUPPORT   (1<<0)
#define AM_SD_SCR_CMD23_SUPPORT   (1<<1)
    uint8_t             erase_value;        /**< \brief erase_value */
}am_sdcard_scr_t;

/** \brief SSR register, only in memory card */
typedef struct am_sdcard_ssr {
    uint32_t            au_size;            /**< \brief In sectors */
    uint32_t            erase_timeout;      /**< \brief In milliseconds */
    uint32_t            erase_offset;       /**< \brief In milliseconds */
}am_sdcard_ssr_t;

/** \brief CSD register, only in memory card */
typedef struct awbl_sdcard_csd {
    uint8_t     mmca_vsn;                   /**< \brief MMC version */
    uint32_t    max_tr_speed;               /**< \brief MAX transfer speed */
    uint16_t    cmd_class;                  /**< \brief command class  */
    uint16_t    sector_size;                /**< \brief sector size */
    uint32_t    sector_cnt;                 /**< \brief sector count */
    uint16_t    block_size;                 /**< \brief block size */
}am_sdcard_csd_t;

/**
 * \brief SD Card information
 */
typedef struct am_sdcard_mem_info{
    am_sdcard_csd_t csd;                    /**< \brief CSD寄存器 */
    am_sdcard_cid_t cid;                    /**< \brief CID寄存器 */
    am_sdcard_ssr_t ssr;                    /**< \brief SSR寄存器 */
    am_sdcard_scr_t scr;                    /**< \brief SCR寄存器 */

    uint32_t        ocr;                    /**< \brief OCR寄存器 */
    uint32_t        rca;                    /**< \brief RCA寄存器 */
    uint16_t        attribute;              /**< \brief SD Card属性描述*/
} am_sdcard_mem_info_t;

/**
 * \brief SDCARD 设备信息结构体
 */
typedef struct am_sdcard_devinfo{
    uint16_t         mode;                  /**< \brief 设备的SDIO模式标志，请参考“SDIO模式标志”*/
    uint32_t         speed;                 /**< \brief SDIO速率*/
    am_bool_t        sdio_crc_en;           /**< \brief SDIO CRC使能标志*/
    uint32_t         ocr_valid;             /**< \brief SD Card支持的ORC（电压范围） */
}am_sdcard_devinfo_t;

/**
 * \brief SDCARD 设备结构体
 */
typedef struct am_sdcard_dev{
    am_sdio_device_t           sdio_dev;    /**< \brief SDIO设备*/
    const am_sdcard_devinfo_t *p_devinfo;   /**< \brief 指向SD Card设备信息实例的指针*/
    uint32_t                   blk_size;    /**< \brief SD Card 块大小*/
    am_sdcard_mem_info_t       sdcard_info; /**< \brief SD Card 的属性信息*/
    am_wait_t                  wait;        /**< \brief wait 等待*/
}am_sdcard_dev_t;

/** \brief SDIO 标准服务操作句柄定义 */
typedef am_sdcard_dev_t *am_sdcard_handle_t;

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
                               uint32_t          *p_rca);

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
int am_sdcard_csd_get (am_sdcard_handle_t handle,
                       uint32_t           rca,
                       uint32_t          *p_csd);

/**
 * \brief 获取存储卡寄存器SCR值(CMD55 + ACMD51)
 *
 * \param[in]   p_card      : SDIO设备
 * \param[in]   rca         : 存储卡相对地址
 * \param[out]  p_scr       : 寄存器scr值
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
int am_sdcard_scr_get (am_sdcard_handle_t handle,
                       uint32_t           rca,
                       uint32_t          *p_scr);

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
                             uint8_t             width);

/**
 * \brief SD Card 设置块大小
 *
 * \param[in]  handle       : SD Card 句柄
 * \param[in]  blk_size     : 需要设置的块大小（单位：字节）
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
int am_sdcard_status_get (am_sdcard_handle_t handle,
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
am_sdcard_handle_t am_sdcard_init(am_sdcard_dev_t           *p_dev,
                                  const am_sdcard_devinfo_t *p_devinfo,
                                  am_sdio_handle_t           sdio_handle);

/**
 * \brief SDCard 设备解初始化
 *
 * \param[in]  handle : SDCard设备操作句柄
 *
 * \retval AM_OK操作成功，否则失败
 */
int am_sdcard_deinit (am_sdcard_handle_t handle);

#ifdef __cplusplus
}
#endif

#endif /* __AM_SDCARD_H */

/*end of file */

