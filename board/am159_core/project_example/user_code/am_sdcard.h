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
 * \name SD 命令
 * @{
 */
#define AM_SD_CMD0               0
#define AM_SD_CMD1               1
#define AM_SD_CMD2               2
#define AM_SD_CMD3               3
#define AM_SD_CMD4               4
#define AM_SD_CMD5               5
#define AM_SD_CMD6               6
#define AM_SD_CMD7               7
#define AM_SD_CMD8               8
#define AM_SD_CMD9               9
#define AM_SD_CMD10              10
#define AM_SD_CMD11              11
#define AM_SD_CMD12              12
#define AM_SD_CMD13              13
#define AM_SD_CMD14              14
#define AM_SD_CMD15              15
#define AM_SD_CMD16              16
#define AM_SD_CMD17              17
#define AM_SD_CMD18              18
#define AM_SD_CMD19              19
#define AM_SD_CMD20              20
#define AM_SD_CMD21              21
#define AM_SD_CMD22              22
#define AM_SD_CMD23              23
#define AM_SD_CMD24              24
#define AM_SD_CMD25              25
#define AM_SD_CMD26              26
#define AM_SD_CMD27              27
#define AM_SD_CMD28              28
#define AM_SD_CMD29              29
#define AM_SD_CMD31              31
#define AM_SD_CMD32              32
#define AM_SD_CMD33              33
#define AM_SD_CMD34              34
#define AM_SD_CMD35              35
#define AM_SD_CMD36              36
#define AM_SD_CMD37              37
#define AM_SD_CMD38              38
#define AM_SD_CMD39              39
#define AM_SD_CMD40              40
#define AM_SD_CMD41              41
#define AM_SD_CMD42              42
#define AM_SD_CMD43              43
#define AM_SD_CMD44              44
#define AM_SD_CMD45              45
#define AM_SD_CMD46              46
#define AM_SD_CMD47              47
#define AM_SD_CMD48              48
#define AM_SD_CMD49              49
#define AM_SD_CMD50              50
#define AM_SD_CMD51              51
#define AM_SD_CMD52              52
#define AM_SD_CMD53              53
#define AM_SD_CMD54              54
#define AM_SD_CMD55              55
#define AM_SD_CMD56              56
#define AM_SD_CMD57              57
#define AM_SD_CMD57              57
#define AM_SD_CMD58              58
#define AM_SD_CMD59              59

#define AM_SD_ACMD6              6
#define AM_SD_ACMD13             13
#define AM_SD_ACMD14             14
#define AM_SD_ACMD15             15
#define AM_SD_ACMD16             16
#define AM_SD_ACMD22             22
#define AM_SD_ACMD23             23
#define AM_SD_ACMD28             28
#define AM_SD_ACMD41             41
#define AM_SD_ACMD42             42
#define AM_SD_ACMD51             51
/** @} */

/** @defgroup EVAL_SDIO_SD_Exported_Types
  * @{
  */
typedef enum {

    /**
      * @brief  SDIO specific error defines
      */
    AM_SD_CMD_CRC_FAIL                    = (1), /*!< Command response received (but CRC check failed) */
    AM_SD_DATA_CRC_FAIL                   = (2), /*!< Data bock sent/received (CRC check Failed) */
    AM_SD_CMD_RSP_TIMEOUT                 = (3), /*!< Command response timeout */
    AM_SD_DATA_TIMEOUT                    = (4), /*!< Data time out */
    AM_SD_TX_UNDERRUN                     = (5), /*!< Transmit FIFO under-run */
    AM_SD_RX_OVERRUN                      = (6), /*!< Receive FIFO over-run */
    AM_SD_START_BIT_ERR                   = (7), /*!< Start bit not detected on all data signals in widE bus mode */
    AM_SD_CMD_OUT_OF_RANGE                = (8), /*!< CMD's argument was out of range.*/
    AM_SD_ADDR_MISALIGNED                 = (9), /*!< Misaligned address */
    AM_SD_BLOCK_LEN_ERR                   = (10), /*!< Transferred block length is not allowed for the card or the number of transferred bytes does not match the block length */
    AM_SD_ERASE_SEQ_ERR                   = (11), /*!< An error in the sequence of erase command occurs.*/
    AM_SD_BAD_ERASE_PARAM                 = (12), /*!< An Invalid selection for erase groups */
    AM_SD_WRITE_PROT_VIOLATION            = (13), /*!< Attempt to program a write protect block */
    AM_SD_LOCK_UNLOCK_FAILED              = (14), /*!< Sequence or password error has been detected in unlock command or if there was an attempt to access a locked card */
    AM_SD_COM_CRC_FAILED                  = (15), /*!< CRC check of the previous command failed */
    AM_SD_ILLEGAL_CMD                     = (16), /*!< Command is not legal for the card state */
    AM_SD_CARD_ECC_FAILED                 = (17), /*!< Card internal ECC was applied but failed to correct the data */
    AM_SD_CC_ERROR                        = (18), /*!< Internal card controller error */
    AM_SD_GENERAL_UNKNOWN_ERROR           = (19), /*!< General or Unknown error */
    AM_SD_STREAM_READ_UNDERRUN            = (20), /*!< The card could not sustain data transfer in stream read operation. */
    AM_SD_STREAM_WRITE_OVERRUN            = (21), /*!< The card could not sustain data programming in stream mode */
    AM_SD_CID_CSD_OVERWRITE               = (22), /*!< CID/CSD overwrite error */
    AM_SD_WP_ERASE_SKIP                   = (23), /*!< only partial address space was erased */
    AM_SD_CARD_ECC_DISABLED               = (24), /*!< Command has been executed without using internal ECC */
    AM_SD_ERASE_RESET                     = (25), /*!< Erase sequence was cleared before executing because an out of erase sequence command was received */
    AM_SD_AKE_SEQ_ERROR                   = (26), /*!< Error in sequence of authentication. */
    AM_SD_INVALID_VOLTRANGE               = (27),
    AM_SD_ADDR_OUT_OF_RANGE               = (28),
    AM_SD_SWITCH_ERROR                    = (29),
    AM_SD_SDIO_DISABLED                   = (30),
    AM_SD_SDIO_FUNCTION_BUSY              = (31),
    AM_SD_SDIO_FUNCTION_FAILED            = (32),

    /**
     * @brief  SDIO specific error defines
     */
    AM_SD_SDIO_UNKNOWN_FUNCTION           = (33),

    /**
     * @brief  Standard error defines
     */
    AM_SD_INTERNAL_ERROR,
    AM_SD_NOT_CONFIGURED,
    AM_SD_REQUEST_PENDING,
    AM_SD_REQUEST_NOT_APPLICABLE,
    AM_SD_INVALID_PARAMETER,
    AM_SD_UNSUPPORTED_FEATURE,
    AM_SD_UNSUPPORTED_HW,
    AM_SD_ERROR,
    AM_SD_OK = 0
} am_sd_error;

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

/** \brief SDIO host 信息  */
typedef struct awbl_sdio_host_info {
    uint8_t                 bus_index;        /**< \brief 总线ID */
    uint8_t                 mode;             /**< \brief 工作模式  1线模式 4线模式 SD 8线模式*/
    uint8_t                 connect_type;
    am_bool_t               crc_en;           /**< \brief CRC校验使能 */
    uint32_t                ocr_valid;        /**< \brief 支持的ORC（电压范围） */
}awbl_sdio_host_info_t;

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

    uint16_t         block_size;

}am_sdcard_devinfo_t;

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
 * \brief 读单个块(CMD17)
 *
 * \param[in]  handle       : SD Card 句柄
 * \param[out] p_buf        : 数据缓冲区
 * \param[in]  blk          : 块号
 * \param[in]  blk_size     : 块大小
 *
 * \retval AM_OK            : 传输成功
 */
int am_sdcard_single_block_read (am_sdcard_handle_t handle,
                                 uint8_t           *p_buf,
                                 uint32_t           blk,
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
 * \param[in]  blk          : 块号
 * \param[in]  blk_size     : 块大小
 *
 * \retval AM_OK            : 传输成功
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
 * \retval AM_OK            : 擦除成功
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
 * \retval AM_OK            : 成功
 */
int am_sdcard_transmission_stop (am_sdcard_handle_t handle);

int am_sdcard_sdinfo_get (am_sdcard_handle_t handle,
                          am_sdcard_info_t  *p_sdinfo);

am_sd_card_state am_sd_getstate(am_sdcard_handle_t  handle);

am_sd_error am_sd_init_cards(am_sdcard_handle_t  handle);

am_sd_error am_sd_get_card_info(am_sdcard_handle_t  handle,
                                am_sdcard_info_t      *p_cardinfo);

am_sd_error am_sd_read_block(am_sdcard_handle_t  handle,
                             uint8_t            *p_buff,
                             uint64_t            addr,
                             uint16_t            block_size);

am_sd_error am_sd_read_multi_blocks(am_sdcard_handle_t  handle,
                                    uint8_t            *p_buff,
                                    uint64_t            addr,
                                    uint16_t            block_size,
                                    uint32_t            number_blocks);

am_sd_error am_sd_write_block(am_sdcard_handle_t  handle,
                              uint8_t            *p_buff,
                              uint64_t            addr,
                              uint16_t            blocksize);

am_sd_error am_sd_write_multi_blocks(am_sdcard_handle_t  handle,
                                     uint8_t            *writebuff,
                                     uint64_t            addr,
                                     uint16_t            block_size,
                                     uint32_t            number_blocks);

am_sd_transfer_state  am_sd_get_transfer_state(am_sdcard_handle_t  handle);

am_sd_error am_sd_stop_transfer(am_sdcard_handle_t  handle);

am_sd_error am_sd_erase(am_sdcard_handle_t  handle,
                        uint64_t            startaddr,
                        uint64_t            endaddr);

am_sd_error am_sd_send_status(uint32_t *p_cardstatus);

am_sd_error am_sd_send_sdstatus(uint32_t *p_sdstatus);

am_sdcard_handle_t am_sdcard_sdio_init(am_sdcard_dev_t      *p_dev,
                                       am_sdcard_devinfo_t  *p_devinfo,
                                       am_sdio_handle_t      sdio_handle);

int am_sdcard_sdio_deinit (am_sdcard_handle_t handle);

#ifdef __cplusplus
}
#endif

#endif /* __AM_SDIO_H */

/*end of file */




