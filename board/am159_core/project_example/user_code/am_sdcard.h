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
 * \brief SDCARD标准接口
 *
 * \internal
 * \par Modification history
 * - 1.01 15-08-17  tee, modified some interface.
 * - 1.00 14-11-01  jon, first implementation.
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
  * @brief  SDIO Transfer state
  */
typedef enum {
  AM_SD_TRANSFER_OK  = 0,
  AM_SD_TRANSFER_BUSY = 1,
  AM_SD_TRANSFER_ERROR
} am_sd_transfer_state;

/**
  * @brief Supported SD Memory Cards
  */
#define AM_SDIO_STD_CAPACITY_SD_CARD_V1_1             ((uint32_t)0x00000000)
#define AM_SDIO_STD_CAPACITY_SD_CARD_V2_0             ((uint32_t)0x00000001)
#define AM_SDIO_HIGH_CAPACITY_SD_CARD                 ((uint32_t)0x00000002)
#define AM_SDIO_MULTIMEDIA_CARD                       ((uint32_t)0x00000003)
#define AM_SDIO_SECURE_DIGITAL_IO_CARD                ((uint32_t)0x00000004)
#define AM_SDIO_HIGH_SPEED_MULTIMEDIA_CARD            ((uint32_t)0x00000005)
#define AM_SDIO_SECURE_DIGITAL_IO_COMBO_CARD          ((uint32_t)0x00000006)
#define AM_SDIO_HIGH_CAPACITY_MMC_CARD                ((uint32_t)0x00000007)

#define AM_SD_OCR_ERRORBITS                        ((uint32_t)0xFDFFE008)


/**
  * @brief  Masks for R6 Response
  */
#define AM_SD_R6_GENERAL_UNKNOWN_ERROR     ((uint32_t)0x00002000)
#define AM_SD_R6_ILLEGAL_CMD               ((uint32_t)0x00004000)
#define AM_SD_R6_COM_CRC_FAILED            ((uint32_t)0x00008000)

#define AM_SD_VOLTAGE_WINDOW_SD            ((uint32_t)0x80100000)
#define AM_SD_HIGH_CAPACITY                ((uint32_t)0x40000000)
#define AM_SD_STD_CAPACITY                 ((uint32_t)0x00000000)
#define AM_SD_CHECK_PATTERN                ((uint32_t)0x000001AA)

#define AM_SD_MAX_VOLT_TRIAL               ((uint32_t)0x0000FFFF)
#define AM_SD_ALLZERO                      ((uint32_t)0x00000000)

#define AM_SD_WIDE_BUS_SUPPORT             ((uint32_t)0x00040000)
#define AM_SD_SINGLE_BUS_SUPPORT           ((uint32_t)0x00010000)
#define AM_SD_CARD_LOCKED                  ((uint32_t)0x02000000)

#define AM_SD_DATATIMEOUT                  ((uint32_t)0xFFFFFFFF)
#define AM_SD_0TO7BITS                     ((uint32_t)0x000000FF)
#define AM_SD_8TO15BITS                    ((uint32_t)0x0000FF00)
#define AM_SD_16TO23BITS                   ((uint32_t)0x00FF0000)
#define AM_SD_24TO31BITS                   ((uint32_t)0xFF000000)
#define AM_SD_MAX_DATA_LENGTH              ((uint32_t)0x01FFFFFF)

#define AM_SD_HALFFIFO                     ((uint32_t)0x00000008)
#define AM_SD_HALFFIFOBYTES                ((uint32_t)0x00000020)

/**
 * \name SD mode R1 response
 * \anchor grp_awbl_sd_r1_rsp
 * @{
 */


/** \brief command argument out of range */
#define AM_SD_R1_OUT_OF_RANGE          (1 << 31)

/** \brief address error */
#define AM_SD_R1_ADDRESS_ERROR         (1 << 30)

/** \brief not support block length or transmit mismatch to the block length */
#define AM_SD_R1_BLOCK_LEN_ERROR       (1 << 29)

/** \brief erase sequence error */
#define AM_SD_R1_ERASE_SEQ_ERROR       (1 << 28)

/** \brief the block write or erase is invalid */
#define AM_SD_R1_ERASE_PARAM           (1 << 27)

/** \brief the block write is protected */
#define AM_SD_R1_WP_VIOLATION          (1 << 26)

/** \brief indicate whether the card is locked */
#define AM_SD_R1_CARD_IS_LOCKED        (1 << 25)

/** \brief indicate in the lock/unlock command，password verify error */
#define AM_SD_R1_LOCK_UNLOCK_FAILED    (1 << 24)

/** \brief CRC error */
#define AM_SD_R1_COM_CRC_ERROR         (1 << 23)

/** \brief ECC failed */
#define AM_SD_R1_CARD_ECC_FAILED       (1 << 21)

/** \brief card host error */
#define AM_SD_R1_CC_ERROR              (1 << 20)

/** \brief unknown error */
#define AM_SD_R1_ERROR                 (1 << 19)

#define AM_SD_R1_UNDERRUN              (1 << 18)
#define AM_SD_R1_OVERRUN               (1 << 17)

/** \brief CID/CSD overwrite */
#define AM_SD_R1_CID_CSD_OVERWRITE     (1 << 16)

/** \brief skip write protected area */
#define AM_SD_R1_WP_ERASE_SKIP         (1 << 15)

/** \brief indicate whether card ECC is forbid */
#define AM_SD_R1_CARD_ECC_DISABLED     (1 << 14)

#define AM_SD_R1_ERASE_RESET           (1 << 13)
#define AM_SD_R1_STATUS(x)             (x & 0xFFFFE000)

/** \brief current state */
#define AM_SD_R1_CURRENT_STATE(x)      ((x & 0x00001E00) >> 9)

/** \brief bus can transmit data */
#define AM_SD_R1_READY_FOR_DATA        (1 << 8)
#define AM_SD_R1_SWITCH_ERROR          (1 << 7)
#define AM_SD_R1_EXCEPTION_EVENT       (1 << 6)
/** \brief indicate whether the card into APP CMD mode */
#define AM_SD_R1_APP_CMD               (1 << 5)
#define AM_SD_R1_ALL_ERROR             0xF9F90000

/** @} */

/**
 * \name card status of R1 on SPI mode
 * \anchor grp_awbl_spi_r1_state
 * @{
 */

/** \brief 指示卡是否处于空闲状态 */
#define AM_SD_SPI_R1_IN_IDLE_STATE       0x01

/** \brief 操作被复位 */
#define AM_SD_SPI_R1_ERASE_RESET         0x02

/** \brief 对于当前卡状态为非法命令  */
#define AM_SD_SPI_R1_ILLEGAL_CMD         0x04

/** \brief CRC校验失败 */
#define AM_SD_SPI_R1_COM_CRC_ERROR       0x08

/** \brief 擦除错误 */
#define AM_SD_SPI_R1_ERASE_SEQ_ERROR     0x10

/** \brief 地址错误，非对齐或数据长度不匹配 */
#define AM_SD_SPI_R1_ADDRESS_ERROR       0x20

/** \brief 参数错误，命令参数段不合法  */
#define AM_SD_SPI_R1_PARA_ERROR          0x40
#define AM_SD_SPI_R1_ALL_ERROR           0x7E
#define AM_SD_SPI_R1_MASK                0x7F

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
  * @brief  SD Card States
  */
typedef enum {
  AM_SD_CARD_READY                  = (0x00000001),
  AM_SD_CARD_IDENTIFICATION         = (0x00000002),
  AM_SD_CARD_STANDBY                = (0x00000003),
  AM_SD_CARD_TRANSFER               = (0x00000004),
  AM_SD_CARD_SENDING                = (0x00000005),
  AM_SD_CARD_RECEIVING              = (0x00000006),
  AM_SD_CARD_PROGRAMMING            = (0x00000007),
  AM_SD_CARD_DISCONNECTED           = (0x00000008),
  AM_SD_CARD_ERROR                  = (0x000000FF)
}am_sd_card_state;

/**
  * @brief  Card Specific Data: CSD Register
  */
typedef struct am_sdcard_csd {
   uint8_t  csd_struct;             /*!< CSD structure */
   uint8_t  sys_specversion;        /*!< System specification version */
   uint8_t  Reserved1;              /*!< Reserved */
   uint8_t  taac;                   /*!< Data read access-time 1 */
   uint8_t  nsac;                   /*!< Data read access-time 2 in CLK cycles */
   uint8_t  max_bus_clkfrec;        /*!< Max. bus clock frequency */
   uint16_t card_comd_classes;      /*!< Card command classes */
   uint8_t  rdblocklen;             /*!< Max. read data block length */
   uint8_t  part_block_read;        /*!< Partial blocks for read allowed */
   uint8_t  wrblock_misalign;       /*!< Write block misalignment */
   uint8_t  rdblock_misalign;       /*!< Read block misalignment */
   uint8_t  dsrimpl;                /*!< DSR implemented */
   uint8_t  Reserved2;              /*!< Reserved */
   uint32_t device_size;            /*!< Device Size */
   uint8_t  max_rd_current_vddmin;  /*!< Max. read current @ VDD min */
   uint8_t  max_rd_current_vddmax;  /*!< Max. read current @ VDD max */
   uint8_t  max_wr_current_vddmin;  /*!< Max. write current @ VDD min */
   uint8_t  max_wr_current_vddmax;  /*!< Max. write current @ VDD max */
   uint8_t  device_size_mul;        /*!< Device size multiplier */
   uint8_t  erase_grsize;           /*!< Erase group size */
   uint8_t  erase_grmul;            /*!< Erase group size multiplier */
   uint8_t  wrprotect_grsize;       /*!< Write protect group size */
   uint8_t  wrprotect_grenable;     /*!< Write protect group enable */
   uint8_t  man_deflecc;            /*!< Manufacturer default ECC */
   uint8_t  wrspeed_fact;           /*!< Write speed factor */
   uint8_t  max_wrblocklen;         /*!< Max. write data block length */
   uint8_t  write_block_pa_partial; /*!< Partial blocks for write allowed */
   uint8_t  reserved3;              /*!< Reserded */
   uint8_t  content_protect_appli;  /*!< Content protection application */
   uint8_t  file_format_grouop;     /*!< File format group */
   uint8_t  copy_flag;              /*!< Copy flag (OTP) */
   uint8_t  perm_wr_protect;        /*!< Permanent write protection */
   uint8_t  temp_wr_protect;        /*!< Temporary write protection */
   uint8_t  file_format;            /*!< File Format */
   uint8_t  ecc;                    /*!< ECC code */
   uint8_t  csd_crc;                /*!< CSD CRC */
   uint8_t  reserved4;              /*!< always 1*/
} am_sdcard_csd_t;

/**
  * @brief  Card Identification Data: CID Register
  */
typedef struct am_sdcard_cid {
   uint8_t  manu_facturer_id;      /*!< ManufacturerID */
   uint16_t oem_appli_id;          /*!< OEM/Application ID */
   uint32_t prod_name1;            /*!< Product Name part1 */
   uint8_t  prod_name2;            /*!< Product Name part2*/
   uint8_t  prod_rev;              /*!< Product Revision */
   uint32_t prod_sn;               /*!< Product Serial Number */
   uint8_t  reserved1;             /*!< Reserved1 */
   uint16_t manufact_date;         /*!< Manufacturing Date */
   uint8_t  cid_crc;               /*!< CID CRC */
   uint8_t  reserved2;             /*!< always 1 */
} am_sdcard_cid_t;

/**
  * @brief SD Card information
  */
typedef struct {
    am_sdcard_csd_t csd;
    am_sdcard_cid_t cid;
    uint64_t        capacity;     /*!< Card Capacity */
    uint32_t        block_size;   /*!< Card Block Size */
    uint16_t        rca;
    uint32_t        type;
} am_sdcard_info_t;

/**
 * \brief sdio驱动函数结构体
 */
struct am_sdcard_drv_funcs {

    /** \brief 获取SDIO控制器信息 */
    int (*pfn_sdcard_stop) (void *p_drv);

    /** \brief 获取SDIO控制器信息 */
    int (*pfn_sdcard_rest) (void *p_drv);

    /** \brief 获取SDIO控制器信息 */
    int (*pfn_sdcard_read) (void *p_drv, uint8_t *p_buf, uint32_t addr, uint16_t block_size);

    /** \brief 获取SDIO控制器信息 */
    int (*pfn_sdcard_write) (void *p_drv, uint8_t *p_buf, uint32_t addr, uint16_t block_size);

    /** \brief 设置SDIO从机设备   */
    int (*pfn_sdcard_erase) (void *p_drv, uint32_t addr_start, uint32_t addr_end);

    int (*pfn_sdcard_get_sdinfo) (void *p_drv,  am_sdcard_info_t *p_sdinfo);

};

/**
 * \brief SDCARD 设备信息结构体
 */
typedef struct am_sdcard_devinfo{

    /** \brief 从设备的SDIO模式标志，请参考“SDIO模式标志”*/
    uint16_t         mode;

    uint32_t         ocr_valid;        /**< \brief 支持的ORC（电压范围） */

}am_sdcard_devinfo_t;

/** \brief SDIO host 信息  */
typedef struct am_sdio_host_info {
    uint8_t                 bus_index;        /**< \brief 总线ID */
    uint8_t                 mode;             /**< \brief 工作模式  1线模式 4线模式 SD 8线模式*/
#define AM_SDIO_SPI_M               0       /**< \brief SDIO SPI模式 */
#define AM_SDIO_SD_1B_M             1       /**< \brief SDIO SD 1线模式 */
#define AM_SDIO_SD_4B_M             2       /**< \brief SDIO SD 4线模式 */
#define AM_SDIO_SD_8B_M             3       /**< \brief SDIO SD 8线模式 */

    uint8_t                 connect_type;
    am_bool_t               crc_en;           /**< \brief CRC校验使能 */
    uint32_t                ocr_valid;        /**< \brief 支持的ORC（电压范围） */
} am_sdio_host_info_t;

/**
 * \brief SDCARD 设备结构体
 */
typedef struct am_sdcard_dev{
    am_sdio_handle_t        sdio_handle;
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
 * \brief 读单个块(CMD17)
 *
 * \param[in]  handle       : SD Card 句柄
 * \param[out] p_buf        : 数据缓冲区
 * \param[in]  blk          : 块号
 * \param[in]  blk_size     : 块大小
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
int am_sdcard_single_block_read (am_sdcard_handle_t handle,
                                 uint8_t           *p_buf,
                                 uint32_t           blk,
                                 uint16_t           blk_size);

/**
 * \brief 同步从存储卡中读取N个数据块(CMD18 + (CMD12))
 *
 * \param[in]  handle       : SDIO设备
 * \param[in]  p_buf        : 数据缓冲区
 * \param[in]  blk_start    : 起始块号
 * \param[in]  blk_num      : 块个数
 * \param[in]  blk_size     : 块大小
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
int am_sdio_multiple_blocks_read (am_sdcard_handle_t handle,
                                  uint8_t           *p_buf,
                                  uint32_t           blk_start,
                                  uint32_t           blk_num,
                                  uint16_t           blk_size);

/**
 * \brief 写单个块(CMD24)
 *
 * \param[in]  handle       : SD Card 句柄
 * \param[in]  p_buf        : 数据缓冲区
 * \param[in]  blk          : 块号
 * \param[in]  blk_size     : 块大小
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
int am_sdcard_single_block_write (am_sdcard_handle_t  handle,
                                  uint8_t            *p_buf,
                                  uint32_t            blk,
                                  uint16_t            blk_size);

/**
 * \brief 写多个块(CMD25 + (CMD12))
 *
 * \param[in]  handle       : SD Card 句柄
 * \param[in]  p_buf        : 数据缓冲区
 * \param[in]  blk_start    : 起始块号
 * \param[in]  blk          : 需要写入的块数量
 * \param[in]  blk_size     : 块大小
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
int am_sdcard_multiple_blocks_write (am_sdcard_handle_t  handle,
                                     uint8_t            *p_buf,
                                     uint32_t            blk_start,
                                     uint32_t            blk_num,
                                     uint16_t            blk_size);

/**
 * \brief 擦除块(CMD38)
 *
 * \param[in]  handle       : SD Card 句柄
 * \param[in]  blk_start    : 需要擦除的起始块号
 * \param[in]  blk_num      : 需要擦除的块数量
 * \param[in]  blk_size     : 块大小（单位：字节）
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
int am_sdcard_block_erase (am_sdcard_handle_t handle,
                           uint32_t           blk_start,
                           uint32_t           blk_num,
                           uint16_t           blk_size);

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




