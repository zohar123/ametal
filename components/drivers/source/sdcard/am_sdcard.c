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
 * \brief LM75温度传感器驱动
 *
 * \internal
 * \par Modification history
 * - 1.00 17-06-19  tee, first implementation.
 * \endinternal
 */

#include "am_sdcard.h"
#include "am_sdio.h"
#include "zmf159_regbase.h"
#include "hw/amhw_zmf159_sdio.h"

/*******************************************************************************
  Local functions
*******************************************************************************/

static int __sdcard_rest (void *p_drv);

static int __sdcard_read (void *p_drv, uint8_t *p_buf, uint32_t addr, uint16_t block_size);

static int __sdcard_write (void *p_drv, uint8_t *p_buf, uint32_t addr, uint16_t block_size);

static int __sdcard_erase (void *p_drv, uint32_t addr_start, uint32_t addr_end);

static int __sdcard_get_sdinfo (void *p_drv, am_sd_cardinfo_t *p_sdinfo);

static int __sdcard_stop (void *p_drv);


/**
 * \brief SDIO 驱动函数定义
 */
static am_const struct am_sdcard_drv_funcs __g_sdio_drv_funcs = {
        __sdcard_stop,
        __sdcard_rest,
        __sdcard_read,
        __sdcard_write,
        __sdcard_erase,
        __sdcard_get_sdinfo

};

static int __sdcard_rest (void *p_drv)
{
    am_sdcard_dev_t *p_dev    = (am_sdcard_dev_t *)p_drv;

    am_sdio_cmd_t      cmd = { 0, 0, SDIO_RESPONSE_NO, {0}};

    return am_sdio_send_cmd(p_dev->sdio_handle, &cmd);

}

static int __sdcard_read (void *p_drv, uint8_t *p_buf, uint32_t addr, uint16_t block_size)
{
    int current_status ;
    int error_status   ;
    am_sdcard_dev_t *p_dev    = (am_sdcard_dev_t *)p_drv;
    am_sdio_cmd_t                cmd;
    uint16_t BlockSize;

//    am_sdio_msg_start(p_dev->sdio_handle);

    amhw_zlg_sdio_t  *p_hw_sdio = (amhw_zlg_sdio_t *)ZMF159_SDIO_BASE;

    p_hw_sdio->mmc_io = 0x2;
    p_hw_sdio->buf_ctll  = 0x20 << 2;
    p_hw_sdio->mmc_io |= 0x01;

    struct am_sdio_message  p_msg;

    if (p_dev->sdcard_info.card_type == SDIO_HIGH_CAPACITY_SD_CARD)
      {
        BlockSize = 512;
        addr     /= 512;
      }

    /* Set Block Size for Card */
    am_sdio_mkcmd (&cmd, 16, BlockSize, SDIO_RESPONSE_SHORT);
    am_sdio_send_cmd(p_dev->sdio_handle, &cmd);


    am_sdio_mkcmd (&cmd, 17, addr, SDIO_RESPONSE_SHORT);
    am_sdio_send_cmd(p_dev->sdio_handle, &cmd);
    am_kprintf("RESPONSE1 = %x.\r\nRESPONSE2 = %x.\r\n", cmd.p_rsp[0], cmd.p_rsp[1]);

    am_mdelay(2);
    am_sdio_mkcmd (&cmd, 12, 0, SDIO_RESPONSE_SHORT);
    am_sdio_send_cmd(p_dev->sdio_handle, &cmd);
    am_kprintf("RESPONSE1 = %x.\r\nRESPONSE2 = %x.\r\n", cmd.p_rsp[0], cmd.p_rsp[1]);

    while(1) {
        am_sdio_mkcmd (&cmd, 13, (p_dev->sdcard_info.rca) << 16, SDIO_RESPONSE_SHORT);
        am_sdio_send_cmd(p_dev->sdio_handle, &cmd);
        am_kprintf("RESPONSE1 = %x.\r\nRESPONSE2 = %x.\r\n", cmd.p_rsp[0], cmd.p_rsp[1]);

        current_status = (cmd.p_rsp[0] >> 9) & 0xf;
        error_status   = (cmd.p_rsp[0] >> 19) & 0x1;

        if(current_status == 4) {
            break;
        }

    }
    p_msg.p_data   = p_buf;
    p_msg.blk_size = block_size;

    return am_sdio_msg_recv(p_dev->sdio_handle, &p_msg, 512);
}

static int __sdcard_write (void *p_drv , uint8_t *p_buf, uint32_t addr, uint16_t block_size)
{
    int               ret       = 0;
    am_sdcard_dev_t  *p_dev     = (am_sdcard_dev_t *)p_drv;
    am_sdio_cmd_t     cmd       = { 0, 0, SDIO_RESPONSE_NO, {0}};
    uint32_t         *p_data    = (uint32_t *)p_buf;
    amhw_zlg_sdio_t  *p_hw_sdio = (amhw_zlg_sdio_t *)ZMF159_SDIO_BASE;
    uint16_t BlockSize = 512;
    int current_status,n;

    struct am_sdio_message  p_msg;

    if (p_dev->sdcard_info.card_type == SDIO_HIGH_CAPACITY_SD_CARD)
      {
        BlockSize = 512;
        addr     /= 512;
      }

      /* Set Block Size for Card */
      am_sdio_mkcmd (&cmd, 16, BlockSize, SDIO_RESPONSE_SHORT);
      ret = am_sdio_send_cmd(p_dev->sdio_handle, &cmd);
      if (ret != AM_OK) {
          return ret;
      }

      /*!< Send CMD24 WRITE_SINGLE_BLOCK */
      am_sdio_mkcmd (&cmd, 24, addr, SDIO_RESPONSE_SHORT);
      ret = am_sdio_send_cmd(p_dev->sdio_handle, &cmd);
      if (ret != AM_OK) {
          return ret;
      }

      /*!< In case of single data block transfer no need of stop command at all */
      p_msg.p_data   = p_data;
      p_msg.blk_size = block_size;

//      am_sdio_msg_send(p_dev->sdio_handle, &p_msg, 512);

      p_hw_sdio->buf_ctll = 0x820;
      p_hw_sdio->mmc_io = 0x01;

      am_sdio_msg_send(p_dev->sdio_handle, &p_msg, 512);

      while(p_hw_sdio->mmc_io & 0x01);

//        while(1) {
//            if(p_hw_sdio->clr_mmc_int & 0x2) {                               //judge which interrupt generation
//                p_hw_sdio->clr_mmc_int = 0x2;                                //write 1 clear interrup
//                break;
//            }
//        }

      am_sdio_mkcmd (&cmd, 12, 0, SDIO_RESPONSE_SHORT);
      am_sdio_send_cmd(p_dev->sdio_handle, &cmd);
      am_kprintf("RESPONSE1 = %x.\r\nRESPONSE2 = %x.\r\n", cmd.p_rsp[0], cmd.p_rsp[1]);

    while(1) {
        am_sdio_mkcmd (&cmd, 13, (p_dev->sdcard_info.rca) << 16, SDIO_RESPONSE_SHORT);
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
    }

}

static int __sdcard_erase (void    *p_drv,
                           uint32_t addr_start,
                           uint32_t addr_end)
{
    am_sdcard_dev_t *p_dev    = (am_sdcard_dev_t *)p_drv;
    am_sdio_cmd_t    cmd      = { 0, 0, SDIO_RESPONSE_NO, {0}};

    int current_status;

      if (p_dev->sdcard_info.card_type == SDIO_HIGH_CAPACITY_SD_CARD)
      {
          addr_start /= 512;
          addr_end /= 512;
      }

      /*!< According to sd-card spec 1.0 ERASE_GROUP_START (CMD32) and erase_group_end(CMD33) */
      if ((SDIO_STD_CAPACITY_SD_CARD_V1_1 == p_dev->sdcard_info.card_type) ||
         (SDIO_STD_CAPACITY_SD_CARD_V2_0  == p_dev->sdcard_info.card_type) ||
         (SDIO_HIGH_CAPACITY_SD_CARD      == p_dev->sdcard_info.card_type)) {
        /*!< Send CMD32 SD_ERASE_GRP_START with argument as addr  */

          am_sdio_mkcmd (&cmd, 32, addr_start, SDIO_RESPONSE_SHORT);
          am_sdio_send_cmd(p_dev->sdio_handle, &cmd);

          if (cmd.p_rsp[1] != 32) {
              return -AM_SD_ERASE_SEQ_ERR;
          }

          /*!< Send CMD33 SD_ERASE_GRP_END with argument as addr  */
          am_sdio_mkcmd (&cmd, 33, addr_end, SDIO_RESPONSE_SHORT);
          am_sdio_send_cmd(p_dev->sdio_handle, &cmd);

          if (cmd.p_rsp[1] != 33) {
              return -AM_SD_ERASE_SEQ_ERR;
          }
      }

      /*!< Send CMD38 ERASE */
      /*!< Send CMD33 SD_ERASE_GRP_END with argument as addr  */
      am_sdio_mkcmd (&cmd, 38, 0, SDIO_RESPONSE_SHORT);
      am_sdio_send_cmd(p_dev->sdio_handle, &cmd);

      if (cmd.p_rsp[1] != 38) {
          return -AM_SD_ERASE_SEQ_ERR;
      }

    while(1) {
            am_sdio_mkcmd (&cmd, 13, (p_dev->sdcard_info.rca) << 16, SDIO_RESPONSE_SHORT);
            am_sdio_send_cmd(p_dev->sdio_handle, &cmd);
            am_kprintf("RESPONSE1 = %x.\r\nRESPONSE2 = %x.\r\n", cmd.p_rsp[0], cmd.p_rsp[1]);

            current_status = (cmd.p_rsp[0] >> 9) & 0xf;
            am_kprintf("current_status = %x.\r\n", current_status);

            if ((AM_SD_OCR_ERRORBITS & cmd.p_rsp[0]) == 0) {
                if(current_status == AM_SD_CARD_PROGRAMMING ||
                   current_status == AM_SD_CARD_RECEIVING) {
                    break;
                }
            }

        }

}

static int __sdcard_get_sdinfo (void *p_drv , am_sd_cardinfo_t *p_sdinfo)
{
    am_sdcard_dev_t *p_dev    = (am_sdcard_dev_t *)p_drv;
    am_sdio_cmd_t      cmd = { 0, 0, SDIO_RESPONSE_NO, {0}};

    return am_sdio_send_cmd(p_dev->sdio_handle, &cmd);
}

static int __sdcard_stop (void *p_drv )
{
    am_sdcard_dev_t *p_dev    = (am_sdcard_dev_t *)p_drv;
    am_sdio_cmd_t      cmd = { 0, 0, SDIO_RESPONSE_NO, {0}};

    return am_sdio_send_cmd(p_dev->sdio_handle, &cmd);
}

void __get_sdcard_csdinfo(am_sd_cardinfo_t *p_sd_card_info, uint32_t *p_response)
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

void __get_sdcard_cidinfo(am_sd_cardinfo_t *p_sd_card_info, uint32_t *p_response)
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

static int __sdcard_hard_init (am_sdcard_dev_t *sdcard_dev)
{
    int      ret;
    int      current_status;
    int      error_status;
    int      rca;

    uint32_t sd_type  = AM_SD_STD_CAPACITY;

    am_sdio_cmd_t cmd = { 0, 0, SDIO_RESPONSE_NO, {0}};

    ret = am_sdio_send_cmd(sdcard_dev->sdio_handle, &cmd);
    if (ret != AM_OK) {
        return ret;
    }

    am_sdio_mkcmd (&cmd, 8, 0x1AA, SDIO_RESPONSE_SHORT);
    ret = am_sdio_send_cmd(sdcard_dev->sdio_handle, &cmd);
    if (ret != AM_OK) {
        return ret;
    }

    //sd_type = AM_SD_HIGH_CAPACITY;

    while (1) {

        am_sdio_mkcmd (&cmd, 55, 0, SDIO_RESPONSE_SHORT);
        ret = am_sdio_send_cmd(sdcard_dev->sdio_handle, &cmd);
        if (ret != AM_OK) {
            return ret;
        }

        am_sdio_mkcmd (&cmd, 41, 0xC0100000, SDIO_RESPONSE_SHORT);
        ret = am_sdio_send_cmd(sdcard_dev->sdio_handle, &cmd);
        if (ret != AM_OK) {
            return ret;
        }

        if((cmd.p_rsp[0] >> 31 & 0x1)) {
            break;
        }
    }

    if((cmd.p_rsp[0] >> 30) & 0x1) {
        sdcard_dev->sdcard_info.card_type = SDIO_HIGH_CAPACITY_SD_CARD;
        am_kprintf("sd card is High Capacity SD Memory Card\n");
    } else {
        am_kprintf("CCS = 0\n");
    }

    am_kprintf("set SD card HCS VDD IS OK \r\n");

    if (SDIO_SECURE_DIGITAL_IO_CARD != sdcard_dev->sdcard_info.card_type) {

        am_sdio_mkcmd (&cmd, 2, 0, SDIO_RESPONSE_LONG);
        ret = am_sdio_send_cmd(sdcard_dev->sdio_handle, &cmd);
        if (ret != AM_OK) {
            return ret;
        }

        __get_sdcard_cidinfo(&sdcard_dev->sdcard_info, cmd.p_rsp);

        am_kprintf("get sd card cid ... \r\n");
    }

    if ((SDIO_STD_CAPACITY_SD_CARD_V1_1    == sdcard_dev->sdcard_info.card_type) ||
        (SDIO_STD_CAPACITY_SD_CARD_V2_0    == sdcard_dev->sdcard_info.card_type) ||
        (SDIO_SECURE_DIGITAL_IO_COMBO_CARD == sdcard_dev->sdcard_info.card_type) ||
        (SDIO_HIGH_CAPACITY_SD_CARD        == sdcard_dev->sdcard_info.card_type)) {

        am_sdio_mkcmd (&cmd, 3, 0, SDIO_RESPONSE_SHORT);
        ret = am_sdio_send_cmd(sdcard_dev->sdio_handle, &cmd);
        if (ret != AM_OK) {
            return ret;
        }

        sdcard_dev->sdcard_info.rca = cmd.p_rsp[0] >> 16;
        am_kprintf("get a new rca... \r\n");
    }

    if (SDIO_SECURE_DIGITAL_IO_CARD != sdcard_dev->sdcard_info.card_type) {

        am_sdio_mkcmd (&cmd, 9, (sdcard_dev->sdcard_info.rca) << 16, SDIO_RESPONSE_LONG);
        ret = am_sdio_send_cmd(sdcard_dev->sdio_handle, &cmd);
        if (ret != AM_OK) {
            return ret;
        }

        __get_sdcard_csdinfo(&sdcard_dev->sdcard_info, cmd.p_rsp);
        am_kprintf("get sd card csd ... \r\n");
    }

    while(1) {

        am_sdio_mkcmd (&cmd, 13, (sdcard_dev->sdcard_info.rca) << 16, SDIO_RESPONSE_SHORT);
        ret = am_sdio_send_cmd(sdcard_dev->sdio_handle, &cmd);
        if (ret != AM_OK) {
            return ret;
        }

        current_status = (cmd.p_rsp[0] >> 9)  & 0xf;
        error_status   = (cmd.p_rsp[0] >> 19) & 0x1;

        if(current_status == 3) {
            break;
        }
    }

//======================================================
//cmd 4 设置频率
//======================================================

    am_sdio_mkcmd (&cmd, 4, 0x04040000, SDIO_RESPONSE_SHORT);
    ret = am_sdio_send_cmd(sdcard_dev->sdio_handle, &cmd);
    if (ret != AM_OK) {
        return ret;
    }

    am_kprintf("RESPONSE1 = %x.\r\nRESPONSE2 = %x.\r\n", cmd.p_rsp[0], cmd.p_rsp[1]);

    /* 选择 SD Card RCA地址 */
    am_sdio_mkcmd (&cmd, 7, (sdcard_dev->sdcard_info.rca) << 16, SDIO_RESPONSE_SHORT);
    ret = am_sdio_send_cmd(sdcard_dev->sdio_handle, &cmd);
    if (ret != AM_OK) {
        return ret;
    }

    am_kprintf("RESPONSE1 = %x.\r\nRESPONSE2 = %x.\r\n", cmd.p_rsp[0], cmd.p_rsp[1]);

    while(1) {
        am_sdio_mkcmd (&cmd, 13, (sdcard_dev->sdcard_info.rca) << 16, SDIO_RESPONSE_SHORT);
        ret = am_sdio_send_cmd(sdcard_dev->sdio_handle, &cmd);
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

//
//======================================================
//cmd 6  设置bus宽度
//======================================================

    am_sdio_mkcmd (&cmd, 55, sdcard_dev->sdcard_info.rca << 16, SDIO_RESPONSE_SHORT);
    ret = am_sdio_send_cmd(sdcard_dev->sdio_handle, &cmd);
    if (ret != AM_OK) {
        return ret;
    }

    am_sdio_mkcmd (&cmd, 6, sdcard_dev->dev_info->mode, SDIO_RESPONSE_SHORT);
    ret = am_sdio_send_cmd(sdcard_dev->sdio_handle, &cmd);
    if (ret != AM_OK) {
        return ret;
    }

    am_kprintf(" sd card card block size is %d \r\n", sdcard_dev->sdcard_info.card_block_size);
    am_kprintf(" sd card init is ok ... \r\n");

    return AM_OK;
}

am_sdcard_handle_t am_sdcard_sdio_init(am_sdcard_dev_t      *sdcard_dev,
                                       am_sd_card_devinfo_t *sdcard_info,
                                       am_sdio_handle_t      sdio_handle)
{
    int ret;

    if ((sdcard_dev == NULL) || (sdcard_info == NULL) || (sdio_handle== NULL)) {
        return NULL;
    }

    sdcard_dev->sdio_handle           = sdio_handle;
    sdcard_dev->dev_info              = sdcard_info;
    sdcard_dev->sdcard_serv.p_funcs   = &__g_sdio_drv_funcs;
    sdcard_dev->sdcard_serv.p_drv     = sdcard_dev;

    am_wait_init(&sdcard_dev->wait);

    ret = __sdcard_hard_init (sdcard_dev);

    if (ret != AM_OK) {
        return NULL;
    }

    return &sdcard_dev->sdcard_serv;
}

/******************************************************************************/
int am_sdcard_sdio_deinit (am_sdcard_dev_t *sdcard_dev)
{
    if (sdcard_dev == NULL) {
        return -AM_EINVAL;
    }

    sdcard_dev->dev_info            = NULL;
    sdcard_dev->sdcard_serv.p_funcs = NULL;
    sdcard_dev->sdcard_serv.p_drv   = NULL;

    return AM_OK;
}



/* end of file */
