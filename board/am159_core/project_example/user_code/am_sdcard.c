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
 * \brief SD卡驱动
 *
 * \internal
 * \par Modification history
 * - 1.00 19-06-04  ipk, first implementation.
 * \endinternal
 */

#include "am_sdcard.h"
#include "am_sdio.h"
#include "am_vdebug.h"
#include "string.h"

static int __sdcard_sdio_cmd_send (am_sdcard_dev_t *p_dev,
                                   uint32_t         cmd,
                                   uint32_t         cmd_arg,
                                   uint8_t          rsp_type,
                                   uint32_t        *p_rsp)
{
    am_sdio_cmd_t sdio_cmd;
    int           ret;

    am_sdio_mkcmd (&sdio_cmd, cmd, cmd_arg, rsp_type);
    ret = am_sdio_send_cmd(p_dev->sdio_handle, &sdio_cmd);

    if (p_rsp) {
        memcpy(p_rsp, sdio_cmd.p_rsp, sizeof(sdio_cmd.p_rsp));
    }

    return ret;
}

static int __sdcard_sdio_data_send (am_sdcard_dev_t *p_dev,
                                    uint8_t         *p_buf,
                                    uint32_t         buf_size)
{
    return am_sdio_msg_send(p_dev->sdio_handle, p_buf, buf_size);
}

static int __sdcard_sdio_msg_receive (am_sdcard_dev_t *p_dev,
                                      uint8_t         *p_buf,
                                      uint32_t         buf_size)
{
    return am_sdio_msg_recv(p_dev->sdio_handle, p_buf, buf_size);
}

static void __sdcard_csdinfo_get(am_sdcard_info_t *p_sd_card_info, uint32_t *p_response)
{
    uint8_t tmp = 0;

    /*!< Byte 0 */
    tmp = (uint8_t)((p_response[0] & 0xFF000000) >> 24);
    p_sd_card_info->csd.csd_struct  = (tmp & 0xC0) >> 6;
    p_sd_card_info->csd.sys_specversion = (tmp & 0x3C) >> 2;
    p_sd_card_info->csd.Reserved1 = tmp & 0x03;

    /*!< Byte 1 */
    tmp = (uint8_t)((p_response[0] & 0x00FF0000) >> 16);
    p_sd_card_info->csd.taac = tmp;

    /*!< Byte 2 */
    tmp = (uint8_t)((p_response[0] & 0x0000FF00) >> 8);
    p_sd_card_info->csd.nsac = tmp;

    /*!< Byte 3 */
    tmp = (uint8_t)(p_response[0] & 0x000000FF);
    p_sd_card_info->csd.max_bus_clkfrec = tmp;

    /*!< Byte 4 */
    tmp = (uint8_t)((p_response[1] & 0xFF000000) >> 24);
    p_sd_card_info->csd.card_comd_classes = tmp << 4;

    /*!< Byte 5 */
    tmp = (uint8_t)((p_response[1] & 0x00FF0000) >> 16);
    p_sd_card_info->csd.card_comd_classes |= (tmp & 0xF0) >> 4;
    p_sd_card_info->csd.rdblocklen = tmp & 0x0F;

    /*!< Byte 6 */
    tmp = (uint8_t)((p_response[1] & 0x0000FF00) >> 8);
    p_sd_card_info->csd.part_block_read = (tmp & 0x80) >> 7;
    p_sd_card_info->csd.wrblock_misalign = (tmp & 0x40) >> 6;
    p_sd_card_info->csd.rdblock_misalign = (tmp & 0x20) >> 5;
    p_sd_card_info->csd.dsrimpl = (tmp & 0x10) >> 4;
    p_sd_card_info->csd.Reserved2 = 0; /*!< Reserved */

    if ((p_sd_card_info->type == AM_SDIO_STD_CAPACITY_SD_CARD_V1_1) ||
        (p_sd_card_info->type == AM_SDIO_STD_CAPACITY_SD_CARD_V2_0))
    {
      p_sd_card_info->csd.device_size = (tmp & 0x03) << 10;

      /*!< Byte 7 */
      tmp = (uint8_t)(p_response[1] & 0x000000FF);
      p_sd_card_info->csd.device_size |= (tmp) << 2;

      /*!< Byte 8 */
      tmp = (uint8_t)((p_response[2] & 0xFF000000) >> 24);
      p_sd_card_info->csd.device_size |= (tmp & 0xC0) >> 6;

      p_sd_card_info->csd.max_rd_current_vddmin = (tmp & 0x38) >> 3;
      p_sd_card_info->csd.max_rd_current_vddmax = (tmp & 0x07);

      /*!< Byte 9 */
      tmp = (uint8_t)((p_response[2] & 0x00FF0000) >> 16);
      p_sd_card_info->csd.max_wr_current_vddmin = (tmp & 0xE0) >> 5;
      p_sd_card_info->csd.max_wr_current_vddmax = (tmp & 0x1C) >> 2;
      p_sd_card_info->csd.device_size_mul = (tmp & 0x03) << 1;
      /*!< Byte 10 */
      tmp = (uint8_t)((p_response[2] & 0x0000FF00) >> 8);
      p_sd_card_info->csd.device_size_mul |= (tmp & 0x80) >> 7;

      p_sd_card_info->capacity = (p_sd_card_info->csd.device_size + 1) ;
      p_sd_card_info->capacity *= (1 << (p_sd_card_info->csd.device_size_mul + 2));
      p_sd_card_info->block_size = 1 << (p_sd_card_info->csd.rdblocklen);
      p_sd_card_info->capacity *= p_sd_card_info->block_size;
    }
    else if (p_sd_card_info->type == AM_SDIO_HIGH_CAPACITY_SD_CARD)
    {
      /*!< Byte 7 */
      tmp = (uint8_t)(p_response[1] & 0x000000FF);
      p_sd_card_info->csd.device_size = (tmp & 0x3F) << 16;

      /*!< Byte 8 */
      tmp = (uint8_t)((p_response[2] & 0xFF000000) >> 24);

      p_sd_card_info->csd.device_size |= (tmp << 8);

      /*!< Byte 9 */
      tmp = (uint8_t)((p_response[2] & 0x00FF0000) >> 16);

      p_sd_card_info->csd.device_size |= (tmp);

      /*!< Byte 10 */
      tmp = (uint8_t)((p_response[2] & 0x0000FF00) >> 8);

      p_sd_card_info->capacity = ((uint64_t)p_sd_card_info->csd.device_size + 1) * 512 * 1024;
      p_sd_card_info->block_size = 512;
    }

    p_sd_card_info->csd.erase_grsize = (tmp & 0x40) >> 6;
    p_sd_card_info->csd.erase_grmul = (tmp & 0x3F) << 1;

    /*!< Byte 11 */
    tmp = (uint8_t)(p_response[2] & 0x000000FF);
    p_sd_card_info->csd.erase_grmul |= (tmp & 0x80) >> 7;
    p_sd_card_info->csd.wrprotect_grsize = (tmp & 0x7F);

    /*!< Byte 12 */
    tmp = (uint8_t)((p_response[3] & 0xFF000000) >> 24);
    p_sd_card_info->csd.wrprotect_grenable = (tmp & 0x80) >> 7;
    p_sd_card_info->csd.man_deflecc = (tmp & 0x60) >> 5;
    p_sd_card_info->csd.wrspeed_fact = (tmp & 0x1C) >> 2;
    p_sd_card_info->csd.max_wrblocklen = (tmp & 0x03) << 2;

    /*!< Byte 13 */
    tmp = (uint8_t)((p_response[3] & 0x00FF0000) >> 16);
    p_sd_card_info->csd.max_wrblocklen |= (tmp & 0xC0) >> 6;
    p_sd_card_info->csd.write_block_pa_partial = (tmp & 0x20) >> 5;
    p_sd_card_info->csd.reserved3 = 0;
    p_sd_card_info->csd.content_protect_appli = (tmp & 0x01);

    /*!< Byte 14 */
    tmp = (uint8_t)((p_response[3] & 0x0000FF00) >> 8);
    p_sd_card_info->csd.file_format_grouop = (tmp & 0x80) >> 7;
    p_sd_card_info->csd.copy_flag = (tmp & 0x40) >> 6;
    p_sd_card_info->csd.perm_wr_protect = (tmp & 0x20) >> 5;
    p_sd_card_info->csd.temp_wr_protect = (tmp & 0x10) >> 4;
    p_sd_card_info->csd.file_format = (tmp & 0x0C) >> 2;
    p_sd_card_info->csd.ecc = (tmp & 0x03);

    /*!< Byte 15 */
    tmp = (uint8_t)(p_response[3] & 0x000000FF);
    p_sd_card_info->csd.csd_crc = (tmp & 0xFE) >> 1;
    p_sd_card_info->csd.reserved4 = 1;

}

static void __sdcard_cidinfo_get(am_sdcard_info_t *p_sd_card_info, uint32_t *p_response)
{
    uint8_t tmp = 0;
    /*!< Byte 0 */
    tmp = (uint8_t)((p_response[0] & 0xFF000000) >> 24);
    p_sd_card_info->cid.manu_facturer_id = tmp;

    /*!< Byte 1 */
    tmp = (uint8_t)((p_response[0] & 0x00FF0000) >> 16);
    p_sd_card_info->cid.oem_appli_id = tmp << 8;

    /*!< Byte 2 */
    tmp = (uint8_t)((p_response[0] & 0x000000FF00) >> 8);
    p_sd_card_info->cid.oem_appli_id |= tmp;

    /*!< Byte 3 */
    tmp = (uint8_t)(p_response[0] & 0x000000FF);
    p_sd_card_info->cid.prod_name1 = tmp << 24;

    /*!< Byte 4 */
    tmp = (uint8_t)((p_response[1] & 0xFF000000) >> 24);
    p_sd_card_info->cid.prod_name1 |= tmp << 16;

    /*!< Byte 5 */
    tmp = (uint8_t)((p_response[1] & 0x00FF0000) >> 16);
    p_sd_card_info->cid.prod_name1 |= tmp << 8;

    /*!< Byte 6 */
    tmp = (uint8_t)((p_response[1] & 0x0000FF00) >> 8);
    p_sd_card_info->cid.prod_name1 |= tmp;

    /*!< Byte 7 */
    tmp = (uint8_t)(p_response[1] & 0x000000FF);
    p_sd_card_info->cid.prod_name2 = tmp;

    /*!< Byte 8 */
    tmp = (uint8_t)((p_response[2] & 0xFF000000) >> 24);
    p_sd_card_info->cid.prod_rev = tmp;

    /*!< Byte 9 */
    tmp = (uint8_t)((p_response[2] & 0x00FF0000) >> 16);
    p_sd_card_info->cid.prod_sn = tmp << 24;

    /*!< Byte 10 */
    tmp = (uint8_t)((p_response[2] & 0x0000FF00) >> 8);
    p_sd_card_info->cid.prod_sn |= tmp << 16;

    /*!< Byte 11 */
    tmp = (uint8_t)(p_response[2] & 0x000000FF);
    p_sd_card_info->cid.prod_sn |= tmp << 8;

    /*!< Byte 12 */
    tmp = (uint8_t)((p_response[3] & 0xFF000000) >> 24);
    p_sd_card_info->cid.prod_sn |= tmp;

    /*!< Byte 13 */
    tmp = (uint8_t)((p_response[3] & 0x00FF0000) >> 16);
    p_sd_card_info->cid.reserved1 |= (tmp & 0xF0) >> 4;
    p_sd_card_info->cid.manufact_date = (tmp & 0x0F) << 8;

    /*!< Byte 14 */
    tmp = (uint8_t)((p_response[3] & 0x0000FF00) >> 8);
    p_sd_card_info->cid.manufact_date |= tmp;

    /*!< Byte 15 */
    tmp = (uint8_t)(p_response[3] & 0x000000FF);
    p_sd_card_info->cid.cid_crc = (tmp & 0xFE) >> 1;
    p_sd_card_info->cid.reserved2 = 1;
}

static int __sdcard_hard_init (am_sdcard_dev_t *p_dev)
{
    int           ret;
    int           current_status;
    uint32_t      rsp[4];
    am_bool_t     timeout_flg;
    am_sdio_timeout_obj_t timeout;

    uint32_t sd_type  = AM_SD_STD_CAPACITY;

    ret = am_sdcard_rest(p_dev);
    if (ret != AM_OK) {
        return ret;
    }

    ret = __sdcard_sdio_cmd_send(p_dev,
                                 AM_SD_CMD8,
                                 0x1AA,
                                 AM_SDIO_RESPONSE_SHORT,
                                 rsp);
    if (ret != AM_OK) {
        return ret;
    }

    sd_type = AM_SD_HIGH_CAPACITY;

    timeout_flg = 0;
    am_adio_timeout_set(&timeout, 10);
    do {
        timeout_flg = am_sdio_timeout(&timeout);

        ret = __sdcard_sdio_cmd_send(p_dev,
                                     AM_SD_CMD55,
                                     0,
                                     AM_SDIO_RESPONSE_SHORT,
                                     rsp);
        if (ret != AM_OK) {
            return ret;
        }

        ret = __sdcard_sdio_cmd_send(p_dev,
                                     AM_SD_CMD41,
                                     AM_SD_VOLTAGE_WINDOW_SD | sd_type,
                                     AM_SDIO_RESPONSE_SHORT,
                                     rsp);
        if (ret != AM_OK) {
            return ret;
        }

        if((rsp[0] >> 31) & 0x1) {
            break;
        }
    } while(!timeout_flg);

    if (timeout_flg) {
        return -AM_ETIME;
    }

    if (rsp[0] & AM_SD_HIGH_CAPACITY) {
        p_dev->sdcard_info.type = AM_SDIO_HIGH_CAPACITY_SD_CARD;
        AM_DBG_INFO("sd card is High Capacity SD Memory Card\n");
    }

    if (AM_SDIO_SECURE_DIGITAL_IO_CARD != p_dev->sdcard_info.type) {

        ret = __sdcard_sdio_cmd_send(p_dev,
                                     AM_SD_CMD2,
                                     0,
                                     AM_SDIO_RESPONSE_LONG,
                                     rsp);
        if (ret != AM_OK) {
            return ret;
        }

        __sdcard_cidinfo_get(&p_dev->sdcard_info, rsp);
    }

    if ((AM_SDIO_STD_CAPACITY_SD_CARD_V1_1    == p_dev->sdcard_info.type) ||
        (AM_SDIO_STD_CAPACITY_SD_CARD_V2_0    == p_dev->sdcard_info.type) ||
        (AM_SDIO_SECURE_DIGITAL_IO_COMBO_CARD == p_dev->sdcard_info.type) ||
        (AM_SDIO_HIGH_CAPACITY_SD_CARD        == p_dev->sdcard_info.type)) {

        ret = __sdcard_sdio_cmd_send(p_dev,
                                     AM_SD_CMD3,
                                     0,
                                     AM_SDIO_RESPONSE_SHORT,
                                     rsp);
        if (ret != AM_OK) {
            return ret;
        }

        p_dev->sdcard_info.rca = rsp[0] >> 16;
    }

    if (AM_SDIO_SECURE_DIGITAL_IO_CARD != p_dev->sdcard_info.type) {

        ret = __sdcard_sdio_cmd_send(p_dev,
                                     AM_SD_CMD9,
                                     (p_dev->sdcard_info.rca << 16),
                                     AM_SDIO_RESPONSE_LONG,
                                     rsp);
        if (ret != AM_OK) {
            return ret;
        }

        __sdcard_csdinfo_get(&p_dev->sdcard_info, rsp);
    }

    timeout_flg = 0;
    am_adio_timeout_set(&timeout, 10);
    do {

        timeout_flg = am_sdio_timeout(&timeout);

        ret = __sdcard_sdio_cmd_send(p_dev,
                                     AM_SD_CMD13,
                                     (p_dev->sdcard_info.rca << 16),
                                     AM_SDIO_RESPONSE_SHORT,
                                     rsp);
        if (ret != AM_OK) {
            return ret;
        }

        current_status = (rsp[0] >> 9)  & 0xf;

        if(current_status == 3) {
            break;
        }

    } while(!timeout_flg);

    if (timeout_flg) {
        return -AM_ETIME;
    }

//======================================================
//cmd 4 设置频率
//======================================================

    ret = __sdcard_sdio_cmd_send(p_dev,
                                 AM_SD_CMD4,
                                 0x04040000,
                                 AM_SDIO_RESPONSE_SHORT,
                                 rsp);
    if (ret != AM_OK) {
        return ret;
    }

    /* 选择 SD Card RCA地址 */
    ret = __sdcard_sdio_cmd_send(p_dev,
                                 AM_SD_CMD7,
                                 (p_dev->sdcard_info.rca << 16),
                                 AM_SDIO_RESPONSE_SHORT,
                                 rsp);
    if (ret != AM_OK) {
        return ret;
    }

    timeout_flg = 0;
    am_adio_timeout_set(&timeout, 10);
    do {

        timeout_flg = am_sdio_timeout(&timeout);

        ret = __sdcard_sdio_cmd_send(p_dev,
                                     AM_SD_CMD13,
                                     (p_dev->sdcard_info.rca << 16),
                                     AM_SDIO_RESPONSE_SHORT,
                                     rsp);
        if (ret != AM_OK) {
            return ret;
        }

        current_status = (rsp[0] >> 9) & 0xf;

        if(current_status == 4) {
            break;
        }

    } while(!timeout_flg);

    if (timeout_flg) {
        return -AM_ETIME;
    }

//
//======================================================
//cmd 6  设置bus宽度
//======================================================

    ret = __sdcard_sdio_cmd_send(p_dev,
                                 AM_SD_CMD55,
                                 (p_dev->sdcard_info.rca << 16),
                                 AM_SDIO_RESPONSE_SHORT,
                                 rsp);
    if (ret != AM_OK) {
        return ret;
    }

    ret = __sdcard_sdio_cmd_send(p_dev,
                                 AM_SD_CMD6,
                                 p_dev->p_devinfo->mode,
                                 AM_SDIO_RESPONSE_SHORT,
                                 rsp);
    if (ret != AM_OK) {
        return ret;
    }

    AM_DBG_INFO("sd card init successful block size 0x%x \r\n",
                p_dev->sdcard_info.block_size);

    return AM_OK;
}

/**
 * \brief SD Card 复位
 */
int am_sdcard_rest (am_sdcard_handle_t handle)
{
    int ret;

    if (handle == NULL) {
        return AM_ERROR;
    }

    ret = __sdcard_sdio_cmd_send(handle,
                                 AM_SD_CMD0,
                                 0,
                                 AM_SDIO_RESPONSE_SHORT,
                                 NULL);

    return ret;
}

/**
 * \brief 读单个块(CMD17)
 *
 */
int am_sdcard_single_block_read (am_sdcard_handle_t handle,
                                 uint8_t           *p_buf,
                                 uint32_t           blk,
                                 uint16_t           blk_size)
{
    int                     ret;
    uint32_t                rsp[4];
    uint32_t                len;
    int                     current_status ;
    uint32_t                addr = blk_size * blk;

    if (handle == NULL || p_buf == NULL) {
        return AM_ERROR;
    }

    if (handle->sdcard_info.type == AM_SDIO_HIGH_CAPACITY_SD_CARD)
      {
        blk_size  = 512;
        addr     /= 512;
      }

    /* Set Block Size for Card */
    ret = __sdcard_sdio_cmd_send(handle,
                                 AM_SD_CMD16,
                                 blk_size,
                                 AM_SDIO_RESPONSE_SHORT,
                                 rsp);
    if (ret != AM_OK) {
        return ret;
    }

    ret = __sdcard_sdio_cmd_send(handle,
                                 AM_SD_CMD17,
                                 addr,
                                 AM_SDIO_RESPONSE_SHORT,
                                 rsp);
    if (ret != AM_OK) {
        return ret;
    }

    len = __sdcard_sdio_msg_receive(handle, p_buf, blk_size);

    ret = __sdcard_sdio_cmd_send(handle,
                                 AM_SD_CMD12,
                                 0,
                                 AM_SDIO_RESPONSE_SHORT,
                                 rsp);

    while(1) {
        ret = __sdcard_sdio_cmd_send(handle,
                                     AM_SD_CMD13,
                                     (handle->sdcard_info.rca << 16),
                                     AM_SDIO_RESPONSE_SHORT,
                                     rsp);

        current_status = (rsp[0] >> 9) & 0xf;

        if(current_status == 4) {
            break;
        }
    }

    if (len != blk_size) {
        return AM_ERROR;
    }

    return AM_OK;
}

/**
 * \brief 写单个块(CMD24)
 *
 */
int am_sdcard_single_block_write (am_sdcard_handle_t  handle,
                                  uint8_t            *p_buf,
                                  uint32_t            blk,
                                  uint16_t            blk_size)
{
    int               ret       = 0;
    uint32_t          rsp[4];
    int               current_status;
    uint32_t          len;
    uint32_t          addr      = blk * blk_size;

    if (handle == NULL || p_buf == NULL) {
        return AM_ERROR;
    }

    if (handle->sdcard_info.type == AM_SDIO_HIGH_CAPACITY_SD_CARD)
    {
      blk_size  = 512;
      addr     /= 512;
    }

    /* Set Block Size for Card */
    ret = __sdcard_sdio_cmd_send(handle,
                                 AM_SD_CMD16,
                                 blk_size,
                                 AM_SDIO_RESPONSE_SHORT,
                                 rsp);
    if (ret != AM_OK) {
        return ret;
    }

    /*!< Send CMD24 WRITE_SINGLE_BLOCK */
    ret = __sdcard_sdio_cmd_send(handle,
                                 AM_SD_CMD24,
                                 addr,
                                 AM_SDIO_RESPONSE_SHORT,
                                 rsp);
    if (ret != AM_OK) {
        return ret;
    }

    len = __sdcard_sdio_data_send(handle, p_buf, blk_size);

    ret = __sdcard_sdio_cmd_send(handle,
                                 AM_SD_CMD12,
                                 0,
                                 AM_SDIO_RESPONSE_SHORT,
                                 rsp);
    if (ret != AM_OK) {
        return ret;
    }

    while(1) {

        ret = __sdcard_sdio_cmd_send(handle,
                                     AM_SD_CMD13,
                                     (handle->sdcard_info.rca << 16),
                                     AM_SDIO_RESPONSE_SHORT,
                                     rsp);
        if (ret != AM_OK) {
            return ret;
        }

        current_status = (rsp[0] >> 9) & 0xf;

        if(current_status == 4) {
            break;
        }
    }

    if (len != blk_size) {
        return AM_ERROR;
    }

    return AM_OK;
}

int am_sdcard_multiple_blocks_write (am_sdcard_handle_t  handle,
                                     uint8_t            *p_buf,
                                     uint32_t            blk_start,
                                     uint32_t            blk_num,
                                     uint16_t            blk_size)
{

}

/**
 * \brief 擦除块(CMD38)
 *
 */
int am_sdcard_block_erase (am_sdcard_handle_t handle,
                           uint32_t           blk_start,
                           uint32_t           blk_num,
                           uint16_t           blk_size)
{
    uint32_t         rsp[4];
    uint32_t         addr_end;
    int              current_status;
    uint32_t         addr_start = blk_start * blk_size;

    if (handle == NULL) {
        return AM_ERROR;
    }

    addr_end = addr_start + blk_size * blk_num - 1;

      if (handle->sdcard_info.type == AM_SDIO_HIGH_CAPACITY_SD_CARD)
      {
          addr_start /= 512;
          addr_end /= 512;
      }

      /*!< According to sd-card spec 1.0 ERASE_GROUP_START (CMD32) and erase_group_end(CMD33) */
      if ((AM_SDIO_STD_CAPACITY_SD_CARD_V1_1 == handle->sdcard_info.type) ||
         (AM_SDIO_STD_CAPACITY_SD_CARD_V2_0  == handle->sdcard_info.type) ||
         (AM_SDIO_HIGH_CAPACITY_SD_CARD      == handle->sdcard_info.type)) {

        /*!< Send CMD32 SD_ERASE_GRP_START with argument as addr  */
          __sdcard_sdio_cmd_send(handle,
                                 AM_SD_CMD32,
                                 addr_start,
                                 AM_SDIO_RESPONSE_SHORT,
                                 rsp);
          if (rsp[1] != AM_SD_CMD32) {
              return -AM_SD_ERASE_SEQ_ERR;
          }

          /*!< Send CMD33 SD_ERASE_GRP_END with argument as addr  */
          __sdcard_sdio_cmd_send(handle,
                                 AM_SD_CMD33,
                                 addr_end,
                                 AM_SDIO_RESPONSE_SHORT,
                                 rsp);
          if (rsp[1] != AM_SD_CMD33) {
              return -AM_SD_ERASE_SEQ_ERR;
          }
      }

      /*!< Send CMD38 ERASE */
      /*!< Send CMD33 SD_ERASE_GRP_END with argument as addr  */
      __sdcard_sdio_cmd_send(handle,
                             AM_SD_CMD38,
                             0,
                             AM_SDIO_RESPONSE_SHORT,
                             rsp);
      if (rsp[1] != AM_SD_CMD38) {
          return -AM_SD_ERASE_SEQ_ERR;
      }

    do {
        __sdcard_sdio_cmd_send(handle,
                               AM_SD_CMD13,
                               (handle->sdcard_info.rca << 16),
                               AM_SDIO_RESPONSE_SHORT,
                               rsp);

            current_status = (rsp[0] >> 9) & 0xf;

        } while(((AM_SD_OCR_ERRORBITS & rsp[0]) == 0) &&
                ((current_status == AM_SD_CARD_PROGRAMMING) || (current_status == AM_SD_CARD_RECEIVING)));

    return AM_OK;
}

/**
 * \brief 停止传输(CMD12)
 */
int am_sdcard_transmission_stop (am_sdcard_handle_t handle)
{
    int      ret;
    uint32_t rsp[4];

    if (handle == NULL) {
        return AM_ERROR;
    }

    ret = __sdcard_sdio_cmd_send(handle,
                                 AM_SD_CMD12,
                                 0,
                                 AM_SDIO_RESPONSE_SHORT,
                                 rsp);
    return ret;
}

int am_sdcard_sdinfo_get (am_sdcard_handle_t handle,
                          am_sdcard_info_t  *p_sdinfo)
{
    am_sdio_cmd_t      cmd = { 0, 0, AM_SDIO_RESPONSE_NO, {0}};

    if (handle == NULL || p_sdinfo == NULL) {
        return AM_ERROR;
    }

    return am_sdio_send_cmd(handle->sdio_handle, &cmd);
}

am_sdcard_handle_t am_sdcard_sdio_init(am_sdcard_dev_t     *p_dev,
                                       am_sdcard_devinfo_t *p_devinfo,
                                       am_sdio_handle_t     sdio_handle)
{
    int ret;

    if ((p_dev == NULL) || (p_devinfo == NULL) || (sdio_handle== NULL)) {
        return NULL;
    }

    p_dev->sdio_handle          = sdio_handle;
    p_dev->p_devinfo            = p_devinfo;

    am_wait_init(&p_dev->wait);

    ret = __sdcard_hard_init(p_dev);

    if (ret != AM_OK) {
        return NULL;
    }

    return p_dev;
}

/******************************************************************************/
int am_sdcard_sdio_deinit (am_sdcard_handle_t handle)
{
    if (handle == NULL) {
        return -AM_EINVAL;
    }

    handle->p_devinfo            = NULL;

    return AM_OK;
}



/* end of file */
