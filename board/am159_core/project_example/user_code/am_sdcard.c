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
#include "am_delay.h"
#include "string.h"

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
    int                   ret;
    uint32_t              ocr;
    uint32_t              current_status = 0;
    uint32_t              rsp[4]         = {0};
    am_bool_t             timeout_flg    = 0;
    am_sdio_timeout_obj_t timeout;

    uint32_t sd_type  = AM_SD_STD_CAPACITY;

    ret = am_sdcard_rest(p_dev);
    if (ret != AM_OK) {
        return ret;
    }

    if (am_sdcard_send_if_cond(p_dev, p_dev->p_devinfo->ocr_valid) == AM_OK) {
        p_dev->sdcard_info.type = AM_SDIO_STD_CAPACITY_SD_CARD_V2_0;
    } else {
        p_dev->sdcard_info.type = AM_SDIO_STD_CAPACITY_SD_CARD_V1_1;
    }

    sd_type = AM_SD_HIGH_CAPACITY;

    timeout_flg = 0;
    am_adio_timeout_set(&timeout, 10);
    do {
        timeout_flg = am_sdio_timeout(&timeout);

        ret = am_sdcard_send_app_op_cond(p_dev,
                                         0,
                                         AM_SD_VOLTAGE_WINDOW_SD | sd_type,
                                         &ocr);

    } while((ret != AM_OK) && (!timeout_flg));

    if (timeout_flg) {
        return -AM_ETIME;
    }

    if (ocr & AM_SD_HIGH_CAPACITY) {
        p_dev->sdcard_info.type = AM_SDIO_HIGH_CAPACITY_SD_CARD;
        AM_DBG_INFO("sd card is High Capacity SD Memory Card\n");
    }

    if (AM_SDIO_SECURE_DIGITAL_IO_CARD != p_dev->sdcard_info.type) {

        ret = am_sdcard_all_cid_get(p_dev, rsp);

        if (ret != AM_OK) {
            return ret;
        }

        __sdcard_cidinfo_get(&p_dev->sdcard_info, rsp);
    }

    if ((AM_SDIO_STD_CAPACITY_SD_CARD_V1_1    == p_dev->sdcard_info.type) ||
        (AM_SDIO_STD_CAPACITY_SD_CARD_V2_0    == p_dev->sdcard_info.type) ||
        (AM_SDIO_SECURE_DIGITAL_IO_COMBO_CARD == p_dev->sdcard_info.type) ||
        (AM_SDIO_HIGH_CAPACITY_SD_CARD        == p_dev->sdcard_info.type)) {

        ret = am_sdio_relative_addr_get(p_dev, &(p_dev->sdcard_info.rca));
        if (ret != AM_OK) {
            return ret;
        }
    }

    if (AM_SDIO_SECURE_DIGITAL_IO_CARD != p_dev->sdcard_info.type) {

        ret = aw_sdcard_csd_get(p_dev, p_dev->sdcard_info.rca, rsp);

        if (ret != AM_OK) {
            return ret;
        }

        __sdcard_csdinfo_get(&p_dev->sdcard_info, rsp);
    }

    timeout_flg = 0;
    am_adio_timeout_set(&timeout, 10);
    do {

        timeout_flg = am_sdio_timeout(&timeout);

        ret = am_sdcard_status_reg_get(p_dev,
                                       p_dev->sdcard_info.rca,
                                       &current_status);

        if (ret == AM_OK) {
            current_status = (current_status >> 9)  & 0xf;
        }

    } while((current_status != 3) && (!timeout_flg));

    if (timeout_flg) {
        return -AM_ETIME;
    }

    /* 设置频率 */
    am_sdio_msg_t     msg;
    am_sdio_trans_t   trans;

    am_sdio_msg_init(&msg);

    am_sdio_mktrans(&trans,
                       AM_SDIO_CMD4,
                       0x04040000,
                       AM_SDIO_OW,
                       NULL,
                       0,
                       0,
                       AM_SDIO_RESPONSE_SHORT,
                       rsp,
                       0);

    am_sdio_trans_add_tail(&msg, &trans);

    ret = am_sdio_msg_start(p_dev->sdio_handle, &msg);
    if (ret != AM_OK) {
        return ret;
    }

    /* 选择卡 */
    ret = am_sdcard_card_select(p_dev, p_dev->sdcard_info.rca, 1);
    if (ret != AM_OK) {
        return ret;
    }

    timeout_flg = 0;
    am_adio_timeout_set(&timeout, 10);
    do {

        timeout_flg = am_sdio_timeout(&timeout);

        ret = am_sdcard_status_reg_get(p_dev,
                                       p_dev->sdcard_info.rca,
                                       &current_status);

        if (ret == AM_OK) {
            current_status = (current_status >> 9)  & 0xf;
        }

    } while((current_status != 4) && (!timeout_flg));

    if (timeout_flg) {
        return -AM_ETIME;
    }

    /* 设置bus宽度 */
    ret = am_sdcard_bus_width_set(p_dev,
                            p_dev->sdcard_info.rca,
                            p_dev->p_devinfo->mode);
    if (ret != AM_OK) {
        return ret;
    }

    AM_DBG_INFO("sd card init successful block size 0x%x \r\n",
                p_dev->sdcard_info.block_size);

    return AM_OK;
}

/**
 * \brief 应用程序初始化(CMD8)
 */
int am_sdcard_send_if_cond (am_sdcard_handle_t handle,
                            uint32_t           ocr)
{
    am_sdio_msg_t     msg;
    am_sdio_trans_t   trans;
    uint32_t          rsp[4];
    uint32_t          arg;
    uint8_t           pattern;
    int               ret;

    if (handle == NULL) {
        return AM_ERROR;
    }

    arg = ((ocr & 0xFF8000) != 0) << 8 | 0xAA;

    am_sdio_msg_init(&msg);

    am_sdio_mktrans(&trans,
                    AM_SDIO_CMD8,
                    arg,
                    AM_SDIO_OW,
                    NULL,
                    0,
                    0,
                    AM_SDIO_RESPONSE_SHORT,
                    rsp,
                    0);

    am_sdio_trans_add_tail(&msg, &trans);

    ret = am_sdio_msg_start(handle->sdio_handle, &msg);

    if (ret != AM_OK) {
        return ret;
    }

    if (handle->p_devinfo->mode == AM_SDIO_SPI_M) {
        if ((rsp[0] & (AM_SDIO_SPI_R1_ALL_ERROR)) != 0) {
            return -AM_EIO;
        }
    }

    pattern = (handle->p_devinfo->mode == AM_SDIO_SPI_M) ? rsp[1] : rsp[0];

    if (pattern != 0xAA) {
        return -AM_EIO;
    }
    return AM_OK;
}

int am_sdcard_send_app_op_cond (am_sdcard_handle_t handle,
                                uint32_t           rca,
                                uint32_t           ocr,
                                uint32_t          *p_rocr)
{
    am_sdio_msg_t     msg;
    am_sdio_trans_t   trans[2];
    uint32_t          rsp1[4];
    uint32_t          rsp2[4];
    uint32_t          arg;
    int               ret;
    uint8_t           rsp_type;

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    arg = (handle->p_devinfo->mode != AM_SDIO_SPI_M) ? (rca << 16) : 0;

    am_sdio_msg_init(&msg);

    am_sdio_mktrans(&trans[0],
                    AM_SDIO_CMD55,
                    arg,
                    AM_SDIO_OW,
                    NULL,
                    0,
                    0,
                    AM_SDIO_RESPONSE_SHORT,
                    rsp1,
                    0);

    am_sdio_trans_add_tail(&msg, &trans[0]);

    arg = (handle->p_devinfo->mode == AM_SDIO_SPI_M) ? (ocr & (1 << 30)) : ocr;

    //rsp_type = (handle->p_devinfo->mode == AM_SDIO_SPI_M) ? AM_SDIO_RSP_R1 : AM_SDIO_RSP_R3;

    rsp_type = AM_SDIO_RESPONSE_SHORT;

    am_sdio_mktrans(&trans[1],
                    AM_SDIO_CMD41,
                    arg,
                    AM_SDIO_OW,
                    NULL,
                    0,
                    0,
                    rsp_type,
                    rsp2,
                    0);

    am_sdio_trans_add_tail(&msg, &trans[1]);

    ret = am_sdio_msg_start(handle->sdio_handle, &msg);

    if (ret != AM_OK) {
        return ret;
    }

    /* check if enter APP CMD mode */
    if ((handle->p_devinfo->mode != AM_SDIO_SPI_M) &&
        !(rsp1[0] & AM_SD_R1_APP_CMD)) {
        return -AM_EIO;
    }

    if (handle->p_devinfo->mode == AM_SDIO_SPI_M) {

        if (rsp2[0] & AM_SD_SPI_R1_IN_IDLE_STATE) {

            if (rsp2[0] & AM_SD_SPI_R1_ALL_ERROR) {
                return -AM_EIO;
            } else {
                ret = ocr ? -AM_EBUSY : AM_OK;
                return ret;
            }
        }
    } else {
        if ((!(rsp2[0] & 0x80000000)) && ocr) {
            return -AM_EBUSY;
        }
        if (p_rocr != NULL) {
            *p_rocr = rsp2[0];
        }
    }

    return AM_OK;
}

/**
 * \brief 获取存储卡所有的CID(CMD2)
 */
int am_sdcard_all_cid_get (am_sdcard_handle_t handle,
                           uint32_t          *p_cid)
{
    am_sdio_msg_t     msg;
    am_sdio_trans_t   trans;
    uint32_t          rsp[4];
    int               ret;

    if (NULL == handle || NULL == p_cid) {
        return -AM_EINVAL;
    }

    /* just SPI mode */
    if (handle->p_devinfo->mode == AM_SDIO_SPI_M) {
        return -AM_EINVAL;
    }

    am_sdio_msg_init(&msg);

    am_sdio_mktrans(&trans,
                    AM_SDIO_CMD2,
                    0,
                    AM_SDIO_OW,
                    NULL,
                    0,
                    0,
                    AM_SDIO_RESPONSE_LONG,
                    rsp,
                    0);

    am_sdio_trans_add_tail(&msg, &trans);

    ret = am_sdio_msg_start(handle->sdio_handle, &msg);
    if (ret != AM_OK) {
        return ret;
    }

    memcpy(p_cid, rsp, 16);

    return AM_OK;
}

/**
 * \brief 获取存储卡相对地址(CMD3)
 */
int am_sdio_relative_addr_get (am_sdcard_handle_t handle,
                               uint16_t          *p_rca)
{
    am_sdio_msg_t     msg;
    am_sdio_trans_t   trans;
    uint32_t          rsp[4] = {0};
    int               ret;

    if (NULL == handle || NULL == p_rca) {
        return -AM_EINVAL;
    }

    /* just SPI mode */
    if (handle->p_devinfo->mode == AM_SDIO_SPI_M) {
        return -AM_EINVAL;
    }

    am_sdio_msg_init(&msg);

    am_sdio_mktrans(&trans,
                    AM_SDIO_CMD3,
                    0,
                    AM_SDIO_OW,
                    NULL,
                    0,
                    0,
                    AM_SDIO_RESPONSE_SHORT,
                    rsp,
                    0);

    am_sdio_trans_add_tail(&msg, &trans);

    ret = am_sdio_msg_start(handle->sdio_handle, &msg);
    if (ret != AM_OK) {
        return ret;
    }

    *p_rca = (uint16_t)(rsp[0]>>16);
    return AM_OK;
}

/**
 * \brief 选定指定相对地址的卡(CMD7)
 */
int am_sdcard_card_select (am_sdcard_handle_t handle,
                           uint32_t           rca,
                           uint8_t            select)
{
    am_sdio_msg_t     msg;
    am_sdio_trans_t   trans;
    uint32_t          rsp[4] = {0};
    int               ret;
    uint32_t          arg;
    uint8_t           rsp_type;

    if (NULL == handle) {
        return -AM_EINVAL;
    }

    if (handle->p_devinfo->mode == AM_SDIO_SPI_M) {
        /* just SPI */
        return -AM_EINVAL;
    }

    am_sdio_msg_init(&msg);

    arg = select ? (rca << 16) : 0;

    rsp_type = select ? AM_SDIO_RESPONSE_SHORT : AM_SDIO_RESPONSE_NO;

    am_sdio_mktrans(&trans,
                       AM_SDIO_CMD7,
                       arg,
                       AM_SDIO_OW,
                       NULL,
                       0,
                       0,
                       rsp_type,
                       rsp,
                       0);

    am_sdio_trans_add_tail(&msg, &trans);

    ret = am_sdio_msg_start(handle->sdio_handle, &msg);
    return ret;
}

/**
 * \brief 获取存储卡寄存器CSD值(CMD9)
 */
int aw_sdcard_csd_get (am_sdcard_handle_t handle,
                       uint32_t           rca,
                       uint32_t          *p_csd)
{
    am_sdio_msg_t     msg;
    am_sdio_trans_t   trans;
    uint32_t          rsp[4];
    int               ret;

    if (NULL == handle || NULL == p_csd) {
        return -AM_EINVAL;
    }

    am_sdio_msg_init(&msg);
    if (handle->p_devinfo->mode == AM_SDIO_SPI_M) {
        am_sdio_mktrans(&trans,
                           AM_SDIO_CMD9,
                           0,
                           AM_SDIO_OW,
                           p_csd,
                           16,
                           1,
                           AM_SDIO_RESPONSE_LONG,
                           rsp,
                           0);

    } else {
        am_sdio_mktrans(&trans,
                           AM_SDIO_CMD9,
                           (rca << 16),
                           AM_SDIO_OW,
                           NULL,
                           0,
                           0,
                           AM_SDIO_RESPONSE_LONG,
                           rsp,
                           0);
    }
    am_sdio_trans_add_tail(&msg, &trans);

    ret = am_sdio_msg_start(handle->sdio_handle, &msg);
    if (ret != AM_OK) {
        return ret;
    }

    if (handle->p_devinfo->mode == AM_SDIO_SPI_M) {
        if ((rsp[0] & AM_SD_SPI_R1_ALL_ERROR) != 0) {
            return -AM_EIO;
        } else {
//            uint8_t  i;
//            uint32_t temp;
//            for (i = 0; i < 2; i++) {
//                temp       = __SDIO_SPI_SWAB32(p_csd[i]);
//                p_csd[i]   = __SDIO_SPI_SWAB32(p_csd[3-i]);
//                p_csd[3-i] = temp;
//            }
        }
    } else {
        memcpy(p_csd, rsp, 16);
    }

    return AM_OK;
}

/**
 * \brief 设置存储卡总线宽度(CMD55 + ACMD6)
 */
int am_sdcard_bus_width_set (am_sdcard_handle_t handle,
                             uint32_t           rca,
                             uint8_t            width)
{
    am_sdio_msg_t     msg;
    am_sdio_trans_t   trans[2];
    uint32_t          rsp1[4];
    uint32_t          rsp2[4];
    uint32_t          arg;
    int               ret;

    if (NULL == handle) {
        return -AM_EINVAL;
    }

    am_sdio_msg_init(&msg);

    arg = (handle->p_devinfo->mode != AM_SDIO_SPI_M) ? (rca << 16) : 0;

    am_sdio_mktrans(&trans[0],
                       AM_SDIO_CMD55,
                       arg,
                       AM_SDIO_OW,
                       NULL,
                       0,
                       0,
                       AM_SDIO_RESPONSE_SHORT,
                       rsp1,
                       0);

    am_sdio_trans_add_tail(&msg, &trans[0]);

    if ((handle->p_devinfo->mode == AM_SDIO_SD_1B_M) &&
        (width == AM_SDIO_BUS_WIDTH_1B)) {
        arg = 0;
    } else if ((handle->p_devinfo->mode == AM_SDIO_SD_4B_M) &&
               (width == AM_SDIO_BUS_WIDTH_4B)) {
        arg = 2;
    } else {
        return -AM_EINVAL;
    }

    am_sdio_mktrans(&trans[1],
                       AM_SDIO_ACMD6,
                       arg,
                       AM_SDIO_OW,
                       NULL,
                       0,
                       0,
                       AM_SDIO_RESPONSE_SHORT,
                       rsp2,
                       0);


    am_sdio_trans_add_tail(&msg, &trans[1]);

    ret = am_sdio_msg_start(handle->sdio_handle, &msg);
    if (ret != AM_OK) {
        return ret;
    }

    if ((handle->p_devinfo->mode != AM_SDIO_SPI_M) &&
        !(rsp1[0] & AM_SD_R1_APP_CMD)) {
        return -AM_EIO;
    }

    return AM_OK;
}

/**
 * \brief SD Card 复位
 */
int am_sdcard_rest (am_sdcard_handle_t handle)
{
    am_sdio_msg_t     msg;
    am_sdio_trans_t   trans;
    int               ret;

    if (handle == NULL) {
        return AM_ERROR;
    }

    am_sdio_msg_init(&msg);

    am_sdio_mktrans(&trans,
                    AM_SDIO_CMD0,
                    0,
                    AM_SDIO_OW,
                    NULL,
                    0,
                    0,
                    AM_SDIO_RESPONSE_NO,
                    NULL,
                    0);

    am_sdio_trans_add_tail(&msg, &trans);

    ret = am_sdio_msg_start(handle->sdio_handle, &msg);

    return ret;
}

/**
 * \brief SD Card 设置块大小
 */
int am_sdcard_block_size_set(am_sdcard_handle_t handle,
                             uint32_t           blk_size)
{
    am_sdio_msg_t     msg;
    uint32_t          rsp[4];
    am_sdio_trans_t   trans;
    int               ret;

    if (handle == NULL) {
        return AM_ERROR;
    }

    am_sdio_msg_init(&msg);

    am_sdio_mktrans(&trans,
                    AM_SDIO_CMD16,
                    blk_size,
                    AM_SDIO_OW,
                    NULL,
                    0,
                    0,
                    AM_SDIO_RESPONSE_SHORT,
                    rsp,
                    0);

    am_sdio_trans_add_tail(&msg, &trans);

    ret = am_sdio_msg_start(handle->sdio_handle, &msg);

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
    am_sdio_msg_t     msg;
    int               ret;
    uint32_t          rsp[4];
    am_sdio_trans_t   trans[2];
    uint32_t          addr = blk_size * blk;

    if (handle == NULL || p_buf == NULL) {
        return AM_ERROR;
    }

    if (handle->sdcard_info.type == AM_SDIO_HIGH_CAPACITY_SD_CARD)
      {
        blk_size  = 512;
        addr     /= 512;
      }

    am_sdio_msg_init(&msg);

    am_sdio_mktrans(&trans[0],
                    AM_SDIO_CMD16,
                    blk_size,
                    AM_SDIO_OW,
                    NULL,
                    0,
                    0,
                    AM_SDIO_RESPONSE_SHORT,
                    rsp,
                    0);

    am_sdio_trans_add_tail(&msg, &trans[0]);

    am_sdio_mktrans(&trans[1],
                    AM_SDIO_CMD17,
                    addr,
                    AM_SDIO_WR,
                    p_buf,
                    blk_size,
                    1,
                    AM_SDIO_RESPONSE_SHORT,
                    rsp,
                    0);

    am_sdio_trans_add_tail(&msg, &trans[1]);

    ret = am_sdio_msg_start(handle->sdio_handle, &msg);
    if (ret != AM_OK) {
        return ret;
    }

    if (handle->p_devinfo->mode == AM_SDIO_SPI_M) {
        if ((rsp[0] & (AM_SD_SPI_R1_IN_IDLE_STATE |
                        AM_SD_SPI_R1_ALL_ERROR)) != 0) {
            return -AM_EIO;
        }
    } else {
        if ((rsp[0] & 0xFFFF0000) != 0) {
            return -AM_EIO;
        }
    }

    return AM_OK;
}

/**
 * \brief 同步从存储卡中读取N个数据块(CMD18 + (CMD12))
 */
int am_sdio_multiple_blocks_read (am_sdcard_handle_t handle,
                                  uint8_t           *p_buf,
                                  uint32_t           blk_start,
                                  uint32_t           blk_num,
                                  uint16_t           blk_size)
{
    int               ret;
    am_sdio_msg_t     msg;
    uint32_t          rsp1[4];
    uint32_t          rsp2[4];
    am_sdio_trans_t   trans[3];
    uint32_t          addr      = blk_start * blk_size;

    if (handle == NULL || p_buf == NULL) {
        return -AM_EINVAL;
    }

    if (handle->sdcard_info.type == AM_SDIO_HIGH_CAPACITY_SD_CARD)
    {
      blk_size  = 512;
      addr     /= 512;
    }

    am_sdio_msg_init(&msg);

    am_sdio_mktrans(&trans[0],
                    AM_SDIO_CMD16,
                    blk_size,
                    AM_SDIO_OW,
                    NULL,
                    0,
                    0,
                    AM_SDIO_RESPONSE_SHORT,
                    NULL,
                    0);

    am_sdio_trans_add_tail(&msg, &trans[0]);

    am_sdio_mktrans(&trans[1],
                    AM_SDIO_CMD18,
                    addr,
                    AM_SDIO_WR,
                    p_buf,
                    blk_size,
                    blk_num,
                    AM_SDIO_RESPONSE_SHORT,
                    rsp1,
                    0);

    am_sdio_trans_add_tail(&msg, &trans[1]);

    am_sdio_mktrans(&trans[2],
                    AM_SDIO_CMD12,
                    0,
                    AM_SDIO_OW,
                    NULL,
                    0,
                    0,
                    AM_SDIO_RESPONSE_SHORT,
                    rsp2,
                    0);

    am_sdio_trans_add_tail(&msg, &trans[2]);

    ret = am_sdio_msg_start(handle->sdio_handle, &msg);
    if (ret != AM_OK) {
        return ret;
    }

    if (handle->p_devinfo->mode == AM_SDIO_SPI_M) {
        if ((rsp1[0] & (AM_SD_SPI_R1_IN_IDLE_STATE |
                        AM_SD_SPI_R1_ALL_ERROR)) != 0) {
            return -AM_EIO;
        }
    } else {
        if ((rsp1[0] & 0xFFFF0000) != 0) {
            return -AM_EIO;
        }
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
    int               ret;
    am_sdio_msg_t     msg;
    uint32_t          rsp[4];
    am_sdio_trans_t   trans[2];
    uint32_t          addr      = blk * blk_size;

    if (handle == NULL || p_buf == NULL) {
        return AM_ERROR;
    }

    if (handle->sdcard_info.type == AM_SDIO_HIGH_CAPACITY_SD_CARD)
    {
      blk_size  = 512;
      addr     /= 512;
    }

    am_sdio_msg_init(&msg);

    am_sdio_mktrans(&trans[0],
                    AM_SDIO_CMD16,
                    blk_size,
                    AM_SDIO_OW,
                    NULL,
                    0,
                    0,
                    AM_SDIO_RESPONSE_SHORT,
                    rsp,
                    0);

    am_sdio_trans_add_tail(&msg, &trans[0]);

    am_sdio_mktrans(&trans[1],
                    AM_SDIO_CMD24,
                    addr,
                    AM_SDIO_OW,
                    p_buf,
                    blk_size,
                    1,
                    AM_SDIO_RESPONSE_SHORT,
                    rsp,
                    0);

    am_sdio_trans_add_tail(&msg, &trans[1]);

    ret = am_sdio_msg_start(handle->sdio_handle, &msg);
    if (ret != AM_OK) {
        return ret;
    }

    if (handle->p_devinfo->mode == AM_SDIO_SPI_M) {
        if ((rsp[0] & (AM_SD_SPI_R1_IN_IDLE_STATE |
                        AM_SD_SPI_R1_ALL_ERROR)) != 0) {
            return -AM_EIO;
        }
    } else {
        if ((rsp[0] & 0xFFFF0000) != 0) {
            return -AM_EIO;
        }
    }

    return AM_OK;
}

/**
 * \brief 写多个块(CMD25 + (CMD12))
 */
int am_sdcard_multiple_blocks_write (am_sdcard_handle_t  handle,
                                     uint8_t            *p_buf,
                                     uint32_t            blk_start,
                                     uint32_t            blk_num,
                                     uint16_t            blk_size)
{
    int               ret;
    am_sdio_msg_t     msg;
    uint32_t          rsp1[4];
    uint32_t          rsp2[4];
    am_sdio_trans_t   trans[3];
    uint32_t          addr      = blk_start * blk_size;

    if (handle == NULL || p_buf == NULL) {
        return AM_ERROR;
    }

    if (handle->sdcard_info.type == AM_SDIO_HIGH_CAPACITY_SD_CARD)
    {
      blk_size  = 512;
      addr     /= 512;
    }

    am_sdio_msg_init(&msg);

    am_sdio_mktrans(&trans[0],
                    AM_SDIO_CMD16,
                    blk_size,
                    AM_SDIO_OW,
                    NULL,
                    0,
                    0,
                    AM_SDIO_RESPONSE_SHORT,
                    NULL,
                    0);

    am_sdio_trans_add_tail(&msg, &trans[0]);

    am_sdio_mktrans(&trans[1],
                    AM_SDIO_CMD25,
                    addr,
                    AM_SDIO_OW,
                    p_buf,
                    blk_size,
                    blk_num,
                    AM_SDIO_RESPONSE_SHORT,
                    rsp1,
                    0);

    am_sdio_trans_add_tail(&msg, &trans[1]);

    am_sdio_mktrans(&trans[2],
                    AM_SDIO_CMD12,
                    0,
                    AM_SDIO_OW,
                    NULL,
                    0,
                    0,
                    AM_SDIO_RESPONSE_SHORT,
                    rsp2,
                    0);

    am_sdio_trans_add_tail(&msg, &trans[2]);

    ret = am_sdio_msg_start(handle->sdio_handle, &msg);
    if (ret != AM_OK) {
        return ret;
    }

    if (handle->p_devinfo->mode == AM_SDIO_SPI_M) {
        if ((rsp1[0] & (AM_SD_SPI_R1_IN_IDLE_STATE |
                        AM_SD_SPI_R1_ALL_ERROR)) != 0) {
            return -AM_EIO;
        }
    } else {
        if ((rsp1[0] & 0xFFFF0000) != 0) {
            return -AM_EIO;
        }
    }

    return AM_OK;
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
    am_sdio_msg_t    msg;
    uint32_t         rsp[4];
    am_sdio_trans_t  trans[3];
    uint32_t         addr_end;
    uint32_t         current_status;
    uint32_t         temp;
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

    am_sdio_msg_init(&msg);

    if ((AM_SDIO_STD_CAPACITY_SD_CARD_V1_1 == handle->sdcard_info.type) ||
       (AM_SDIO_STD_CAPACITY_SD_CARD_V2_0  == handle->sdcard_info.type) ||
       (AM_SDIO_HIGH_CAPACITY_SD_CARD      == handle->sdcard_info.type)) {

        am_sdio_mktrans(&trans[0],
                        AM_SDIO_CMD32,
                        addr_start,
                        AM_SDIO_OW,
                        NULL,
                        0,
                        0,
                        AM_SDIO_RESPONSE_SHORT,
                        rsp,
                        0);

        am_sdio_trans_add_tail(&msg, &trans[0]);

        am_sdio_mktrans(&trans[1],
                        AM_SDIO_CMD33,
                        addr_end,
                        AM_SDIO_OW,
                        NULL,
                        0,
                        0,
                        AM_SDIO_RESPONSE_SHORT,
                        rsp,
                        0);

        am_sdio_trans_add_tail(&msg, &trans[1]);

    }

    am_sdio_mktrans(&trans[2],
                    AM_SDIO_CMD38,
                    0,
                    AM_SDIO_OW,
                    NULL,
                    0,
                    0,
                    AM_SDIO_RESPONSE_SHORT,
                    rsp,
                    0);

    am_sdio_trans_add_tail(&msg, &trans[2]);

    am_sdio_msg_start(handle->sdio_handle, &msg);

    do {
        am_sdcard_status_reg_get(handle,
                                 handle->sdcard_info.rca,
                                 &current_status);

        temp = (current_status >> 9) & 0xf;

    } while(((AM_SD_OCR_ERRORBITS & current_status) == 0) &&
            ((temp == AM_SD_CARD_PROGRAMMING) || (temp == AM_SD_CARD_RECEIVING)));

    return AM_OK;
}

/**
 * \brief 停止传输(CMD12)
 */
int am_sdcard_transfer_stop (am_sdcard_handle_t handle)
{
    am_sdio_msg_t   msg;
    am_sdio_trans_t trans;
    uint32_t        rsp[4];

    if (handle == NULL) {
        return AM_ERROR;
    }

    am_sdio_msg_init(&msg);

    am_sdio_mktrans(&trans,
                    AM_SDIO_CMD12,
                    0,
                    AM_SDIO_OW,
                    NULL,
                    0,
                    0,
                    AM_SDIO_RESPONSE_SHORT,
                    rsp,
                    0);

    am_sdio_trans_add_tail(&msg, &trans);

    am_sdio_msg_start(handle->sdio_handle, &msg);

    return AM_OK;
}

int am_sdcard_sdinfo_get (am_sdcard_handle_t handle,
                          am_sdcard_info_t  *p_sdinfo)
{
//    am_sdio_cmd_t      cmd = { 0, 0, AM_SDIO_RESPONSE_NO, {0}};
//
//    if (handle == NULL || p_sdinfo == NULL) {
//        return AM_ERROR;
//    }
//
//    return am_sdio_send_cmd(handle->sdio_handle, &cmd);
    return AM_OK;
}

int am_sdcard_status_reg_get (am_sdcard_handle_t handle,
                              uint32_t           rca,
                              uint32_t          *p_status)
{
    am_sdio_msg_t   msg;
    am_sdio_trans_t trans;
    uint32_t        rsp[4];
    uint32_t        arg;
    uint8_t         rsp_type;

    if (NULL == handle || NULL == p_status) {
        return -AM_EINVAL;
    }

    am_sdio_msg_init(&msg);

    arg = (handle->p_devinfo->mode != AM_SDIO_SPI_M) ? (rca << 16) : 0;

    rsp_type = (handle->p_devinfo->mode == AM_SDIO_SPI_M) ? AM_SDIO_RSP_R2 : AM_SDIO_RSP_R1;

    am_sdio_mktrans(&trans,
                    AM_SDIO_CMD13,
                    arg,
                    AM_SDIO_OW,
                    NULL,
                    0,
                    0,
                    rsp_type,
                    rsp,
                    0);

    am_sdio_trans_add_tail(&msg, &trans);

    am_sdio_msg_start(handle->sdio_handle, &msg);

    if (handle->p_devinfo->mode != AM_SDIO_SPI_M) {
        *p_status = rsp[0];
    } else {
        *p_status = rsp[0] >> 16;
    }

    return AM_OK;
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
