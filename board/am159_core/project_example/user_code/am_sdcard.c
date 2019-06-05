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
#include "am_delay.h"
#include "am_vdebug.h"
#include "zmf159_regbase.h"
#include "hw/amhw_zmf159_sdio.h"


static void __sdcard_csdinfo_get(am_sdcard_info_t *p_sd_card_info, uint32_t *p_response)
{
    uint8_t tmp = 0;

    /*!< Byte 0 */
    tmp = (uint8_t)((p_response[0] & 0xFF000000) >> 24);
    p_sd_card_info->sd_csd.csd_struct  = (tmp & 0xC0) >> 6;
    p_sd_card_info->sd_csd.sys_specversion = (tmp & 0x3C) >> 2;
    p_sd_card_info->sd_csd.Reserved1 = tmp & 0x03;

    /*!< Byte 1 */
    tmp = (uint8_t)((p_response[0] & 0x00FF0000) >> 16);
    p_sd_card_info->sd_csd.taac = tmp;

    /*!< Byte 2 */
    tmp = (uint8_t)((p_response[0] & 0x0000FF00) >> 8);
    p_sd_card_info->sd_csd.nsac = tmp;

    /*!< Byte 3 */
    tmp = (uint8_t)(p_response[0] & 0x000000FF);
    p_sd_card_info->sd_csd.max_bus_clkfrec = tmp;

    /*!< Byte 4 */
    tmp = (uint8_t)((p_response[1] & 0xFF000000) >> 24);
    p_sd_card_info->sd_csd.card_comd_classes = tmp << 4;

    /*!< Byte 5 */
    tmp = (uint8_t)((p_response[1] & 0x00FF0000) >> 16);
    p_sd_card_info->sd_csd.card_comd_classes |= (tmp & 0xF0) >> 4;
    p_sd_card_info->sd_csd.rdblocklen = tmp & 0x0F;

    /*!< Byte 6 */
    tmp = (uint8_t)((p_response[1] & 0x0000FF00) >> 8);
    p_sd_card_info->sd_csd.part_block_read = (tmp & 0x80) >> 7;
    p_sd_card_info->sd_csd.wrblock_misalign = (tmp & 0x40) >> 6;
    p_sd_card_info->sd_csd.rdblock_misalign = (tmp & 0x20) >> 5;
    p_sd_card_info->sd_csd.dsrimpl = (tmp & 0x10) >> 4;
    p_sd_card_info->sd_csd.Reserved2 = 0; /*!< Reserved */

    if ((p_sd_card_info->card_type == SDIO_STD_CAPACITY_SD_CARD_V1_1) ||
        (p_sd_card_info->card_type == SDIO_STD_CAPACITY_SD_CARD_V2_0))
    {
      p_sd_card_info->sd_csd.device_size = (tmp & 0x03) << 10;

      /*!< Byte 7 */
      tmp = (uint8_t)(p_response[1] & 0x000000FF);
      p_sd_card_info->sd_csd.device_size |= (tmp) << 2;

      /*!< Byte 8 */
      tmp = (uint8_t)((p_response[2] & 0xFF000000) >> 24);
      p_sd_card_info->sd_csd.device_size |= (tmp & 0xC0) >> 6;

      p_sd_card_info->sd_csd.max_rd_current_vddmin = (tmp & 0x38) >> 3;
      p_sd_card_info->sd_csd.max_rd_current_vddmax = (tmp & 0x07);

      /*!< Byte 9 */
      tmp = (uint8_t)((p_response[2] & 0x00FF0000) >> 16);
      p_sd_card_info->sd_csd.max_wr_current_vddmin = (tmp & 0xE0) >> 5;
      p_sd_card_info->sd_csd.max_wr_current_vddmax = (tmp & 0x1C) >> 2;
      p_sd_card_info->sd_csd.device_size_mul = (tmp & 0x03) << 1;
      /*!< Byte 10 */
      tmp = (uint8_t)((p_response[2] & 0x0000FF00) >> 8);
      p_sd_card_info->sd_csd.device_size_mul |= (tmp & 0x80) >> 7;

      p_sd_card_info->card_capacity = (p_sd_card_info->sd_csd.device_size + 1) ;
      p_sd_card_info->card_capacity *= (1 << (p_sd_card_info->sd_csd.device_size_mul + 2));
      p_sd_card_info->card_block_size = 1 << (p_sd_card_info->sd_csd.rdblocklen);
      p_sd_card_info->card_capacity *= p_sd_card_info->card_block_size;
    }
    else if (p_sd_card_info->card_type == SDIO_HIGH_CAPACITY_SD_CARD)
    {
      /*!< Byte 7 */
      tmp = (uint8_t)(p_response[1] & 0x000000FF);
      p_sd_card_info->sd_csd.device_size = (tmp & 0x3F) << 16;

      /*!< Byte 8 */
      tmp = (uint8_t)((p_response[2] & 0xFF000000) >> 24);

      p_sd_card_info->sd_csd.device_size |= (tmp << 8);

      /*!< Byte 9 */
      tmp = (uint8_t)((p_response[2] & 0x00FF0000) >> 16);

      p_sd_card_info->sd_csd.device_size |= (tmp);

      /*!< Byte 10 */
      tmp = (uint8_t)((p_response[2] & 0x0000FF00) >> 8);

      p_sd_card_info->card_capacity = ((uint64_t)p_sd_card_info->sd_csd.device_size + 1) * 512 * 1024;
      p_sd_card_info->card_block_size = 512;
    }

    p_sd_card_info->sd_csd.erase_grsize = (tmp & 0x40) >> 6;
    p_sd_card_info->sd_csd.erase_grmul = (tmp & 0x3F) << 1;

    /*!< Byte 11 */
    tmp = (uint8_t)(p_response[2] & 0x000000FF);
    p_sd_card_info->sd_csd.erase_grmul |= (tmp & 0x80) >> 7;
    p_sd_card_info->sd_csd.wrprotect_grsize = (tmp & 0x7F);

    /*!< Byte 12 */
    tmp = (uint8_t)((p_response[3] & 0xFF000000) >> 24);
    p_sd_card_info->sd_csd.wrprotect_grenable = (tmp & 0x80) >> 7;
    p_sd_card_info->sd_csd.man_deflecc = (tmp & 0x60) >> 5;
    p_sd_card_info->sd_csd.wrspeed_fact = (tmp & 0x1C) >> 2;
    p_sd_card_info->sd_csd.max_wrblocklen = (tmp & 0x03) << 2;

    /*!< Byte 13 */
    tmp = (uint8_t)((p_response[3] & 0x00FF0000) >> 16);
    p_sd_card_info->sd_csd.max_wrblocklen |= (tmp & 0xC0) >> 6;
    p_sd_card_info->sd_csd.write_block_pa_partial = (tmp & 0x20) >> 5;
    p_sd_card_info->sd_csd.reserved3 = 0;
    p_sd_card_info->sd_csd.content_protect_appli = (tmp & 0x01);

    /*!< Byte 14 */
    tmp = (uint8_t)((p_response[3] & 0x0000FF00) >> 8);
    p_sd_card_info->sd_csd.file_format_grouop = (tmp & 0x80) >> 7;
    p_sd_card_info->sd_csd.copy_flag = (tmp & 0x40) >> 6;
    p_sd_card_info->sd_csd.perm_wr_protect = (tmp & 0x20) >> 5;
    p_sd_card_info->sd_csd.temp_wr_protect = (tmp & 0x10) >> 4;
    p_sd_card_info->sd_csd.file_format = (tmp & 0x0C) >> 2;
    p_sd_card_info->sd_csd.ecc = (tmp & 0x03);

    /*!< Byte 15 */
    tmp = (uint8_t)(p_response[3] & 0x000000FF);
    p_sd_card_info->sd_csd.csd_crc = (tmp & 0xFE) >> 1;
    p_sd_card_info->sd_csd.reserved4 = 1;

}

static void __sdcard_cidinfo_get(am_sdcard_info_t *p_sd_card_info, uint32_t *p_response)
{
    uint8_t tmp = 0;
    /*!< Byte 0 */
    tmp = (uint8_t)((p_response[0] & 0xFF000000) >> 24);
    p_sd_card_info->sd_cid.manu_facturer_id = tmp;

    /*!< Byte 1 */
    tmp = (uint8_t)((p_response[0] & 0x00FF0000) >> 16);
    p_sd_card_info->sd_cid.oem_appli_id = tmp << 8;

    /*!< Byte 2 */
    tmp = (uint8_t)((p_response[0] & 0x000000FF00) >> 8);
    p_sd_card_info->sd_cid.oem_appli_id |= tmp;

    /*!< Byte 3 */
    tmp = (uint8_t)(p_response[0] & 0x000000FF);
    p_sd_card_info->sd_cid.prod_name1 = tmp << 24;

    /*!< Byte 4 */
    tmp = (uint8_t)((p_response[1] & 0xFF000000) >> 24);
    p_sd_card_info->sd_cid.prod_name1 |= tmp << 16;

    /*!< Byte 5 */
    tmp = (uint8_t)((p_response[1] & 0x00FF0000) >> 16);
    p_sd_card_info->sd_cid.prod_name1 |= tmp << 8;

    /*!< Byte 6 */
    tmp = (uint8_t)((p_response[1] & 0x0000FF00) >> 8);
    p_sd_card_info->sd_cid.prod_name1 |= tmp;

    /*!< Byte 7 */
    tmp = (uint8_t)(p_response[1] & 0x000000FF);
    p_sd_card_info->sd_cid.prod_name2 = tmp;

    /*!< Byte 8 */
    tmp = (uint8_t)((p_response[2] & 0xFF000000) >> 24);
    p_sd_card_info->sd_cid.prod_rev = tmp;

    /*!< Byte 9 */
    tmp = (uint8_t)((p_response[2] & 0x00FF0000) >> 16);
    p_sd_card_info->sd_cid.prod_sn = tmp << 24;

    /*!< Byte 10 */
    tmp = (uint8_t)((p_response[2] & 0x0000FF00) >> 8);
    p_sd_card_info->sd_cid.prod_sn |= tmp << 16;

    /*!< Byte 11 */
    tmp = (uint8_t)(p_response[2] & 0x000000FF);
    p_sd_card_info->sd_cid.prod_sn |= tmp << 8;

    /*!< Byte 12 */
    tmp = (uint8_t)((p_response[3] & 0xFF000000) >> 24);
    p_sd_card_info->sd_cid.prod_sn |= tmp;

    /*!< Byte 13 */
    tmp = (uint8_t)((p_response[3] & 0x00FF0000) >> 16);
    p_sd_card_info->sd_cid.reserved1 |= (tmp & 0xF0) >> 4;
    p_sd_card_info->sd_cid.manufact_date = (tmp & 0x0F) << 8;

    /*!< Byte 14 */
    tmp = (uint8_t)((p_response[3] & 0x0000FF00) >> 8);
    p_sd_card_info->sd_cid.manufact_date |= tmp;

    /*!< Byte 15 */
    tmp = (uint8_t)(p_response[3] & 0x000000FF);
    p_sd_card_info->sd_cid.cid_crc = (tmp & 0xFE) >> 1;
    p_sd_card_info->sd_cid.reserved2 = 1;
}

static int __sdcard_hard_init (am_sdcard_dev_t *p_dev)
{
    int           ret;
    int           current_status;
    am_sdio_cmd_t cmd = {0};
    am_bool_t     timeout_flg;
    am_sdio_timeout_obj_t timeout;

    uint32_t sd_type  = AM_SD_STD_CAPACITY;

    ret = am_sdcard_rest(p_dev);
    if (ret != AM_OK) {
        return ret;
    }

    am_sdio_mkcmd (&cmd, 8, 0x1AA, AM_SDIO_RESPONSE_SHORT);
    ret = am_sdio_send_cmd(p_dev->sdio_handle, &cmd);
    if (ret != AM_OK) {
        return ret;
    }

    sd_type = AM_SD_HIGH_CAPACITY;

    timeout_flg = 0;
    am_adio_timeout_set(&timeout, 10);
    do {
        timeout_flg = am_sdio_timeout(&timeout);

        am_sdio_mkcmd (&cmd, 55, 0, AM_SDIO_RESPONSE_SHORT);
        ret = am_sdio_send_cmd(p_dev->sdio_handle, &cmd);
        if (ret != AM_OK) {
            return ret;
        }

        am_kprintf("cmd55 rsp0 %d  rsp1 %d\r\n", cmd.p_rsp[0], cmd.p_rsp[1]);

        am_sdio_mkcmd (&cmd,
                       41,
                       AM_SD_VOLTAGE_WINDOW_SD | sd_type,
                       AM_SDIO_RESPONSE_SHORT);
        ret = am_sdio_send_cmd(p_dev->sdio_handle, &cmd);
        if (ret != AM_OK) {
            return ret;
        }
        am_kprintf("cmd41 rsp0 %d  rsp1 %d\r\n", cmd.p_rsp[0], cmd.p_rsp[1]);
        if((cmd.p_rsp[0] >> 31 & 0x1)) {
            break;
        }
    } while(!timeout_flg);

    if (timeout_flg) {
        return -AM_ETIME;
    }

    if((cmd.p_rsp[0] >> 30) & 0x1) {
        p_dev->sdcard_info.card_type = SDIO_HIGH_CAPACITY_SD_CARD;
        am_kprintf("sd card is High Capacity SD Memory Card\n");
    } else {
        am_kprintf("CCS = 0\n");
    }

    am_kprintf("set SD card HCS VDD IS OK \r\n");

    if (SDIO_SECURE_DIGITAL_IO_CARD != p_dev->sdcard_info.card_type) {

        am_sdio_mkcmd (&cmd, 2, 0, AM_SDIO_RESPONSE_LONG);
        ret = am_sdio_send_cmd(p_dev->sdio_handle, &cmd);
        if (ret != AM_OK) {
            return ret;
        }

        __sdcard_cidinfo_get(&p_dev->sdcard_info, cmd.p_rsp);

        am_kprintf("get sd card cid ... \r\n");
    }

    if ((SDIO_STD_CAPACITY_SD_CARD_V1_1    == p_dev->sdcard_info.card_type) ||
        (SDIO_STD_CAPACITY_SD_CARD_V2_0    == p_dev->sdcard_info.card_type) ||
        (SDIO_SECURE_DIGITAL_IO_COMBO_CARD == p_dev->sdcard_info.card_type) ||
        (SDIO_HIGH_CAPACITY_SD_CARD        == p_dev->sdcard_info.card_type)) {

        am_sdio_mkcmd (&cmd, 3, 0, AM_SDIO_RESPONSE_SHORT);
        ret = am_sdio_send_cmd(p_dev->sdio_handle, &cmd);
        if (ret != AM_OK) {
            return ret;
        }

        p_dev->sdcard_info.rca = cmd.p_rsp[0] >> 16;
        am_kprintf("get a new rca... \r\n");
    }

    if (SDIO_SECURE_DIGITAL_IO_CARD != p_dev->sdcard_info.card_type) {

        am_sdio_mkcmd (&cmd, 9, (p_dev->sdcard_info.rca) << 16, AM_SDIO_RESPONSE_LONG);
        ret = am_sdio_send_cmd(p_dev->sdio_handle, &cmd);
        if (ret != AM_OK) {
            return ret;
        }

        __sdcard_csdinfo_get(&p_dev->sdcard_info, cmd.p_rsp);
        am_kprintf("get sd card csd ... \r\n");
    }

    timeout_flg = 0;
    am_adio_timeout_set(&timeout, 10);
    do {

        timeout_flg = am_sdio_timeout(&timeout);

        am_sdio_mkcmd (&cmd, 13, (p_dev->sdcard_info.rca) << 16, AM_SDIO_RESPONSE_SHORT);
        ret = am_sdio_send_cmd(p_dev->sdio_handle, &cmd);
        if (ret != AM_OK) {
            return ret;
        }

        current_status = (cmd.p_rsp[0] >> 9)  & 0xf;
//        error_status   = (cmd.p_rsp[0] >> 19) & 0x1;

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

    am_sdio_mkcmd (&cmd, 4, 0x04040000, AM_SDIO_RESPONSE_SHORT);
    ret = am_sdio_send_cmd(p_dev->sdio_handle, &cmd);
    if (ret != AM_OK) {
        return ret;
    }

    am_kprintf("RESPONSE1 = %x.\r\nRESPONSE2 = %x.\r\n", cmd.p_rsp[0], cmd.p_rsp[1]);

    /* 选择 SD Card RCA地址 */
    am_sdio_mkcmd (&cmd, 7, (p_dev->sdcard_info.rca) << 16, AM_SDIO_RESPONSE_SHORT);
    ret = am_sdio_send_cmd(p_dev->sdio_handle, &cmd);
    if (ret != AM_OK) {
        return ret;
    }

    am_kprintf("RESPONSE1 = %x.\r\nRESPONSE2 = %x.\r\n", cmd.p_rsp[0], cmd.p_rsp[1]);

    timeout_flg = 0;
    am_adio_timeout_set(&timeout, 10);
    do {

        timeout_flg = am_sdio_timeout(&timeout);

        am_sdio_mkcmd (&cmd, 13, (p_dev->sdcard_info.rca) << 16, AM_SDIO_RESPONSE_SHORT);
        ret = am_sdio_send_cmd(p_dev->sdio_handle, &cmd);
        if (ret != AM_OK) {
            return ret;
        }

        am_kprintf("RESPONSE1 = %x.\r\nRESPONSE2 = %x.\r\n", cmd.p_rsp[0], cmd.p_rsp[1]);

        current_status = (cmd.p_rsp[0] >> 9) & 0xf;
        am_kprintf("current_status = %x.\r\n", current_status);

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

    am_sdio_mkcmd (&cmd, 55, p_dev->sdcard_info.rca << 16, AM_SDIO_RESPONSE_SHORT);
    ret = am_sdio_send_cmd(p_dev->sdio_handle, &cmd);
    if (ret != AM_OK) {
        return ret;
    }

    am_sdio_mkcmd (&cmd, 6, p_dev->p_devinfo->mode, AM_SDIO_RESPONSE_SHORT);
    ret = am_sdio_send_cmd(p_dev->sdio_handle, &cmd);
    if (ret != AM_OK) {
        return ret;
    }

    am_kprintf(" sd card card block size is %d \r\n", p_dev->sdcard_info.card_block_size);
    am_kprintf(" sd card init is ok ... \r\n");

    return AM_OK;
}


int am_sdcard_rest (am_sdcard_handle_t handle)
{
    am_sdio_cmd_t cmd = { 0, 0, AM_SDIO_RESPONSE_NO, {0}};

    if (handle == NULL) {
        return AM_ERROR;
    }

    return am_sdio_send_cmd(handle->sdio_handle, &cmd);
}

int am_sdcard_read (am_sdcard_handle_t handle,
                    uint8_t           *p_buf,
                    uint32_t           addr,
                    uint16_t           block_size)
{
    int                     current_status ;
    am_sdio_cmd_t           cmd;
    struct am_sdio_message  p_msg;
    int                     ret;
    uint32_t                len;
    uint16_t                BlockSize;

    amhw_zlg_sdio_t  *p_hw_sdio = (amhw_zlg_sdio_t *)ZMF159_SDIO_BASE;

    if (handle == NULL || p_buf == NULL) {
        return AM_ERROR;
    }

//    am_sdio_msg_start(p_dev->sdio_handle);

//    amhw_zlg_sdio_transfdir_set(p_hw_sdio, AMHW_ZLG_SDIO_READ_MODE);
//    amhw_zlg_sdio_autodattr_enable(p_hw_sdio);

    if (handle->sdcard_info.card_type == SDIO_HIGH_CAPACITY_SD_CARD)
      {
        BlockSize = 512;
        addr     /= 512;
      }

    /* Set Block Size for Card */
    am_sdio_mkcmd (&cmd, 16, BlockSize, AM_SDIO_RESPONSE_SHORT);
    ret = am_sdio_send_cmd(handle->sdio_handle, &cmd);
    if (ret != AM_OK) {
        return ret;
    }

    am_sdio_mkcmd (&cmd, 17, addr, AM_SDIO_RESPONSE_SHORT);
    ret = am_sdio_send_cmd(handle->sdio_handle, &cmd);
    if (ret != AM_OK) {
        return ret;
    }

    p_msg.p_data   = p_buf;
    p_msg.blk_size = block_size;

    len = am_sdio_msg_recv(handle->sdio_handle, &p_msg, 512);

    //am_mdelay(4);
    am_sdio_mkcmd(&cmd, 12, 0, AM_SDIO_RESPONSE_SHORT);
    am_sdio_send_cmd(handle->sdio_handle, &cmd);
    am_kprintf("RESPONSE1 = %x.\r\nRESPONSE2 = %x.\r\n", cmd.p_rsp[0], cmd.p_rsp[1]);

    while(1) {
        am_sdio_mkcmd (&cmd, 13, (handle->sdcard_info.rca) << 16, AM_SDIO_RESPONSE_SHORT);
        am_sdio_send_cmd(handle->sdio_handle, &cmd);
        am_kprintf("RESPONSE1 = %x.\r\nRESPONSE2 = %x.\r\n", cmd.p_rsp[0], cmd.p_rsp[1]);

        current_status = (cmd.p_rsp[0] >> 9) & 0xf;
//        error_status   = (cmd.p_rsp[0] >> 19) & 0x1;

        if(current_status == 4) {
            break;
        }

    }

    return len;
}

int am_sdcard_write (am_sdcard_handle_t  handle,
                     uint8_t            *p_buf,
                     uint32_t            addr,
                     uint16_t            block_size)
{
    int               ret       = 0;
    am_sdio_cmd_t     cmd       = { 0, 0, AM_SDIO_RESPONSE_NO, {0}};
    uint16_t          BlockSize = 512;
    int               current_status;

    struct am_sdio_message  p_msg;

    if (handle == NULL || p_buf == NULL) {
        return AM_ERROR;
    }

    if (handle->sdcard_info.card_type == SDIO_HIGH_CAPACITY_SD_CARD)
      {
        BlockSize = 512;
        addr     /= 512;
      }

      /* Set Block Size for Card */
      am_sdio_mkcmd (&cmd, 16, BlockSize, AM_SDIO_RESPONSE_SHORT);
      ret = am_sdio_send_cmd(handle->sdio_handle, &cmd);
      if (ret != AM_OK) {
          return ret;
      }

      /*!< Send CMD24 WRITE_SINGLE_BLOCK */
      am_sdio_mkcmd (&cmd, 24, addr, AM_SDIO_RESPONSE_SHORT);
      ret = am_sdio_send_cmd(handle->sdio_handle, &cmd);
      if (ret != AM_OK) {
          return ret;
      }

      /*!< In case of single data block transfer no need of stop command at all */
      p_msg.p_data   = p_buf;
      p_msg.blk_size = block_size;

      am_sdio_msg_send(handle->sdio_handle, &p_msg, 512);

      //am_mdelay(4);
      am_sdio_mkcmd (&cmd, 12, 0, AM_SDIO_RESPONSE_SHORT);
      am_sdio_send_cmd(handle->sdio_handle, &cmd);
      am_kprintf("RESPONSE1 = %x.\r\nRESPONSE2 = %x.\r\n", cmd.p_rsp[0], cmd.p_rsp[1]);

    while(1) {
        am_sdio_mkcmd (&cmd, 13, (handle->sdcard_info.rca) << 16, AM_SDIO_RESPONSE_SHORT);
        ret = am_sdio_send_cmd(handle->sdio_handle, &cmd);
        if (ret != AM_OK) {
            return ret;
        }

        am_kprintf("RESPONSE1 = %x.\r\nRESPONSE2 = %x.\r\n", cmd.p_rsp[0], cmd.p_rsp[1]);

        current_status = (cmd.p_rsp[0] >> 9) & 0xf;
        am_kprintf("current_status = %x.\r\n", current_status);

        if(current_status == 4) {
            break;
        }
    }

    return AM_OK;
}

int am_sdcard_erase (am_sdcard_handle_t handle,
                     uint32_t           addr_start,
                     uint32_t           blocks)
{
    int              current_status;
    uint32_t         addr_end;
    am_sdio_cmd_t    cmd      = { 0, 0, AM_SDIO_RESPONSE_NO, {0}};

    if (handle == NULL) {
        return AM_ERROR;
    }

    addr_end = blocks * handle->sdcard_info.card_block_size + addr_start;

      if (handle->sdcard_info.card_type == SDIO_HIGH_CAPACITY_SD_CARD)
      {
          addr_start /= 512;
          addr_end /= 512;
      }

      /*!< According to sd-card spec 1.0 ERASE_GROUP_START (CMD32) and erase_group_end(CMD33) */
      if ((SDIO_STD_CAPACITY_SD_CARD_V1_1 == handle->sdcard_info.card_type) ||
         (SDIO_STD_CAPACITY_SD_CARD_V2_0  == handle->sdcard_info.card_type) ||
         (SDIO_HIGH_CAPACITY_SD_CARD      == handle->sdcard_info.card_type)) {
        /*!< Send CMD32 SD_ERASE_GRP_START with argument as addr  */

          am_sdio_mkcmd (&cmd, 32, addr_start, AM_SDIO_RESPONSE_SHORT);
          am_sdio_send_cmd(handle->sdio_handle, &cmd);

          if (cmd.p_rsp[1] != 32) {
              return -AM_SD_ERASE_SEQ_ERR;
          }

          /*!< Send CMD33 SD_ERASE_GRP_END with argument as addr  */
          am_sdio_mkcmd (&cmd, 33, addr_end, AM_SDIO_RESPONSE_SHORT);
          am_sdio_send_cmd(handle->sdio_handle, &cmd);

          if (cmd.p_rsp[1] != 33) {
              return -AM_SD_ERASE_SEQ_ERR;
          }
      }

      /*!< Send CMD38 ERASE */
      /*!< Send CMD33 SD_ERASE_GRP_END with argument as addr  */
      am_sdio_mkcmd (&cmd, 38, 0, AM_SDIO_RESPONSE_SHORT);
      am_sdio_send_cmd(handle->sdio_handle, &cmd);

      if (cmd.p_rsp[1] != 38) {
          return -AM_SD_ERASE_SEQ_ERR;
      }

      am_mdelay(10);

    while(1) {
            am_sdio_mkcmd (&cmd, 13, (handle->sdcard_info.rca) << 16, AM_SDIO_RESPONSE_SHORT);
            am_sdio_send_cmd(handle->sdio_handle, &cmd);
            am_kprintf("RESPONSE1 = %x.\r\nRESPONSE2 = %x.\r\n", cmd.p_rsp[0], cmd.p_rsp[1]);

            current_status = (cmd.p_rsp[0] >> 9) & 0xf;
            am_kprintf("current_status = %x.\r\n", current_status);

            if (((AM_SD_OCR_ERRORBITS & cmd.p_rsp[0]) != 0) ||
                (current_status != AM_SD_CARD_PROGRAMMING)  ||
                (current_status != AM_SD_CARD_RECEIVING)) {
                break;
            }
        }
      return AM_OK;
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

int am_sdcard_stop (am_sdcard_handle_t handle)
{
    am_sdio_cmd_t      cmd = { 0, 0, AM_SDIO_RESPONSE_NO, {0}};

    if (handle == NULL) {
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
