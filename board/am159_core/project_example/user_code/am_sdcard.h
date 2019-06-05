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
#define SDIO_STD_CAPACITY_SD_CARD_V1_1             ((uint32_t)0x00000000)
#define SDIO_STD_CAPACITY_SD_CARD_V2_0             ((uint32_t)0x00000001)
#define SDIO_HIGH_CAPACITY_SD_CARD                 ((uint32_t)0x00000002)
#define SDIO_MULTIMEDIA_CARD                       ((uint32_t)0x00000003)
#define SDIO_SECURE_DIGITAL_IO_CARD                ((uint32_t)0x00000004)
#define SDIO_HIGH_SPEED_MULTIMEDIA_CARD            ((uint32_t)0x00000005)
#define SDIO_SECURE_DIGITAL_IO_COMBO_CARD          ((uint32_t)0x00000006)
#define SDIO_HIGH_CAPACITY_MMC_CARD                ((uint32_t)0x00000007)

#define AM_SD_OCR_ERRORBITS                        ((uint32_t)0xFDFFE008)

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
typedef struct am_sd_csd {
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
} am_sd_csd_t;

/**
  * @brief  Card Identification Data: CID Register
  */
typedef struct am_sd_cid {
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
} am_sd_cid_t;

/**
  * @brief SD Card information
  */
typedef struct {
    am_sd_csd_t sd_csd;
    am_sd_cid_t sd_cid;
    uint64_t    card_capacity;   /*!< Card Capacity */
    uint32_t    card_block_size; /*!< Card Block Size */
    uint16_t    rca;
    uint32_t    card_type;
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

/** \brief 获取SDIO控制器信息 */
int am_sdcard_rest (am_sdcard_handle_t handle);

/** \brief 获取SDIO控制器信息 */
int am_sdcard_read (am_sdcard_handle_t handle,
                    uint8_t           *p_buf,
                    uint32_t           addr,
                    uint16_t           block_size);

int am_sdcard_write (am_sdcard_handle_t  handle,
                     uint8_t            *p_buf,
                     uint32_t            addr,
                     uint16_t            block_size);

int am_sdcard_erase (am_sdcard_handle_t handle,
                     uint32_t           addr_start,
                     uint32_t           blocks);

int am_sdcard_sdinfo_get (am_sdcard_handle_t handle,
                          am_sdcard_info_t  *p_sdinfo);

int am_sdcard_stop (am_sdcard_handle_t handle);

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




