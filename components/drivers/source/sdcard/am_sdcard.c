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

#define __SDIO_SPI_SWAB32(x) ((uint32_t)(\
            (((uint32_t)(x) & (uint32_t)0x000000ffUL) << 24) | \
            (((uint32_t)(x) & (uint32_t)0x0000ff00UL) <<  8) | \
            (((uint32_t)(x) & (uint32_t)0x00ff0000UL) >>  8) | \
            (((uint32_t)(x) & (uint32_t)0xff000000UL) >> 24)))

/* SD table */
static const uint32_t __gp_tran_exp[] = {
    10000, 100000, 1000000, 10000000, 0, 0, 0, 0
};

static const uint8_t __gp_tran_mant[] = {
    0,  10, 12, 13, 15, 20, 25, 30,
    35, 40, 45, 50, 55, 60, 70, 80,
};

static uint32_t __get_bits_str (uint32_t *p_str,
                                uint32_t  start,
                                uint8_t   len)
{
    uint32_t  mask;
    uint32_t  index;
    uint8_t   shift;
    uint32_t  value;

    mask  = (int)((len < 32) ? (1 << len) : 0) - 1;
    index = start / 32;
    shift = start & 31;
    value = p_str[index] >> shift;

    if ((len + shift) > 32) {
        value |= p_str[index + 1] << (32 - shift);
    }
    value &= mask;
    return value;
}

static int __sdcard_decode_cid (am_sdcard_mem_info_t *p_mem_info,
                                uint32_t             *p_cid)
{
    if (NULL == p_mem_info || NULL == p_cid) {
        return -AM_EINVAL;
    }
    memset(&(p_mem_info->cid), 0, sizeof(am_sdcard_cid_t));
    if (p_mem_info->attribute & AM_SDCARD_MMC) {
        switch (p_mem_info->csd.mmca_vsn) {
        case 0: /* MMC v1.0 - v1.2 */
        case 1: /* MMC v1.4 */
            p_mem_info->cid.mid    = __get_bits_str(p_cid, 104, 24);
            p_mem_info->cid.pnm[0] = __get_bits_str(p_cid, 96, 8);
            p_mem_info->cid.pnm[1] = __get_bits_str(p_cid, 88, 8);
            p_mem_info->cid.pnm[2] = __get_bits_str(p_cid, 80, 8);
            p_mem_info->cid.pnm[3] = __get_bits_str(p_cid, 72, 8);
            p_mem_info->cid.pnm[4] = __get_bits_str(p_cid, 64, 8);
            p_mem_info->cid.pnm[5] = __get_bits_str(p_cid, 56, 8);
            p_mem_info->cid.pnm[6] = __get_bits_str(p_cid, 48, 8);
            p_mem_info->cid.hwrev  = __get_bits_str(p_cid, 44, 4);
            p_mem_info->cid.fwrev  = __get_bits_str(p_cid, 40, 4);
            p_mem_info->cid.psn    = __get_bits_str(p_cid, 16, 24);
            p_mem_info->cid.month  = __get_bits_str(p_cid, 12, 4);
            p_mem_info->cid.year   = __get_bits_str(p_cid, 8, 4) + 1997;
            break;

        case 2: /* MMC v2.0 - v2.2 */
        case 3: /* MMC v3.1 - v3.3 */
        case 4: /* MMC v4 */
            p_mem_info->cid.mid    = __get_bits_str(p_cid, 120, 8);
            p_mem_info->cid.oid    = __get_bits_str(p_cid, 104, 16);
            p_mem_info->cid.pnm[0] = __get_bits_str(p_cid, 96, 8);
            p_mem_info->cid.pnm[1] = __get_bits_str(p_cid, 88, 8);
            p_mem_info->cid.pnm[2] = __get_bits_str(p_cid, 80, 8);
            p_mem_info->cid.pnm[3] = __get_bits_str(p_cid, 72, 8);
            p_mem_info->cid.pnm[4] = __get_bits_str(p_cid, 64, 8);
            p_mem_info->cid.pnm[5] = __get_bits_str(p_cid, 56, 8);
            p_mem_info->cid.prv    = __get_bits_str(p_cid, 48, 8);
            p_mem_info->cid.psn    = __get_bits_str(p_cid, 16, 32);
            p_mem_info->cid.month  = __get_bits_str(p_cid, 12, 4);
            p_mem_info->cid.year   = __get_bits_str(p_cid, 8, 4) + 1997;
            break;

        default:
            return -AM_EINVAL;
        }

    } else {
        p_mem_info->cid.mid    = __get_bits_str(p_cid, 120, 8);
        p_mem_info->cid.oid    = __get_bits_str(p_cid, 104, 16);
        p_mem_info->cid.pnm[0] = __get_bits_str(p_cid, 96, 8);
        p_mem_info->cid.pnm[1] = __get_bits_str(p_cid, 88, 8);
        p_mem_info->cid.pnm[2] = __get_bits_str(p_cid, 80, 8);
        p_mem_info->cid.pnm[3] = __get_bits_str(p_cid, 72, 8);
        p_mem_info->cid.pnm[4] = __get_bits_str(p_cid, 64, 8);
        p_mem_info->cid.prv    = __get_bits_str(p_cid, 56, 8);
        p_mem_info->cid.psn    = __get_bits_str(p_cid, 24, 32);
        p_mem_info->cid.year   = __get_bits_str(p_cid, 12, 8) + 2000;
        p_mem_info->cid.month  = __get_bits_str(p_cid, 8, 4);
    }
    return AM_OK;
}


static int __sdcard_decode_csd (am_sdcard_mem_info_t *p_mem_info,
                                uint32_t             *p_csd)
{
    uint32_t e, m, r;
    uint8_t  structure;

    if (NULL == p_mem_info   ||
        NULL == p_csd) {
        return -AM_EINVAL;
    }
    structure = __get_bits_str(p_csd, 126, 2);
    if (p_mem_info->attribute & AM_SDCARD_MMC) {
        if (structure == 0) {
            /* 该版本无法识别 */
            return -AM_EINVAL;
        }
        p_mem_info->csd.mmca_vsn   = __get_bits_str(p_csd, 122, 4);

        m = __get_bits_str(p_csd, 99, 4);
        e = __get_bits_str(p_csd, 96, 3);
        p_mem_info->csd.max_tr_speed  = __gp_tran_exp[e] * __gp_tran_mant[m];
        p_mem_info->csd.cmd_class     = __get_bits_str(p_csd, 84, 12);

        p_mem_info->csd.sector_size = 512;
        m = __get_bits_str(p_csd, 62, 12);
        e = __get_bits_str(p_csd, 47, 3);
        r = __get_bits_str(p_csd, 80, 4);
        p_mem_info->csd.sector_cnt = ((1 + m) << (e + r - 7));

        e = __get_bits_str(p_csd, 22, 4);

        if (e >= 9) {
            m = __get_bits_str(p_csd, 42, 5);
            r = __get_bits_str(p_csd, 37, 5);
            p_mem_info->csd.block_size = (m + 1) * (r + 1);
            p_mem_info->csd.block_size <<= e - 9;
        }

    } else {
        switch (structure) {
            case 0:
                m = __get_bits_str(p_csd, 99, 4);
                e = __get_bits_str(p_csd, 96, 3);
                p_mem_info->csd.max_tr_speed = __gp_tran_exp[e] * __gp_tran_mant[m];
                p_mem_info->csd.cmd_class = __get_bits_str(p_csd, 84, 12);

                p_mem_info->csd.sector_size = 512;
                m = __get_bits_str(p_csd, 62, 12);
                e = __get_bits_str(p_csd, 47, 3);
                r = __get_bits_str(p_csd, 80, 4);
                p_mem_info->csd.sector_cnt = ((1 + m) << (e + r - 7));


                if (__get_bits_str(p_csd, 46, 1)) {
                    p_mem_info->csd.block_size = 1;
                } else {
                    p_mem_info->csd.block_size = __get_bits_str(p_csd, 39, 7)+1;
                    p_mem_info->csd.block_size <<= (__get_bits_str(p_csd, 22, 4)-9);
                }

                break;

            case 1:
                m = __get_bits_str(p_csd, 99, 4);
                e = __get_bits_str(p_csd, 96, 3);
                p_mem_info->csd.max_tr_speed = __gp_tran_exp[e] * __gp_tran_mant[m];
                p_mem_info->csd.cmd_class    = __get_bits_str(p_csd, 84, 12);

                p_mem_info->csd.sector_size = 512;
                m = __get_bits_str(p_csd, 48, 22);
                p_mem_info->csd.sector_cnt = (1 + m) << 10;
                p_mem_info->csd.block_size = 1;
                break;
            default:
                return -AM_EINVAL;
        }
    }
    return AM_OK;
}

static int __sdcard_hard_init (am_sdcard_dev_t *p_dev)
{
    int                   ret;
    uint32_t              ocr;
    uint32_t              current_status = 0;
    uint32_t              rsp[4]         = {0};
    uint32_t              cid[4]         = {0};
    am_sdio_timeout_obj_t timeout;

    ret = am_sdcard_rest(p_dev);
    if (ret != AM_OK) {
        return ret;
    }

    ocr = p_dev->p_devinfo->ocr_valid;
    if (am_sdcard_send_if_cond(p_dev, ocr) == AM_OK) {
        p_dev->sdcard_info.attribute |= AM_SDCARD_SDV2X;
        ocr |= AM_SD_OCR_CCS;
    } else {
        p_dev->sdcard_info.attribute |= AM_SDCARD_SDV1X;
    }

    am_sdio_timeout_set(&timeout, 200);
    do {
        if (am_sdio_timeout(&timeout)) {
            return -AM_ETIME;
        }

        ret = am_sdcard_send_app_op_cond(p_dev,
                                         0,
                                         ocr,
                                         &ocr);

    } while(ret != AM_OK);

    if (ocr & AM_SD_OCR_CCS) {
        p_dev->sdcard_info.attribute |= AM_SDCARD_SDHC;
        AM_DBG_INFO("sd card is High Capacity SD Memory Card\n");
    } else {
        p_dev->sdcard_info.attribute |= AM_SDCARD_SDSC;
    }

    ret = am_sdcard_all_cid_get(p_dev, cid);

    if (ret != AM_OK) {
        return ret;
    }

    p_dev->sdcard_info.ocr = ocr;

    ret = am_sdio_relative_addr_get(p_dev, &(p_dev->sdcard_info.rca));
    if (ret != AM_OK) {
        return ret;
    }

    ret = am_sdcard_csd_get(p_dev, p_dev->sdcard_info.rca, rsp);

    if (ret != AM_OK) {
        return ret;
    }

    ret = __sdcard_decode_cid(&p_dev->sdcard_info, cid);
    if (ret != AM_OK) {
        return ret;
    }

    ret = __sdcard_decode_csd(&p_dev->sdcard_info, rsp);
    if (ret != AM_OK) {
        return ret;
    }

    am_sdio_timeout_set(&timeout, 10);
    do {

        if (am_sdio_timeout(&timeout)) {
            return -AM_ETIME;
        }

        ret = am_sdcard_status_get(p_dev,
                                       p_dev->sdcard_info.rca,
                                       &current_status);

        if (ret == AM_OK) {
            current_status = (current_status >> 9)  & 0xf;
        }

    } while(current_status != 3);

    /* 设置频率 */
    ret = am_sdio_cmd_write(&p_dev->sdio_dev,
                            AM_SDIO_CMD4,
                            0x04040000,
                            AM_SDIO_RESPONSE_SHORT,
                            rsp);
    if (ret != AM_OK) {
        return ret;
    }

    /* 选择卡 */
    ret = am_sdcard_card_select(p_dev, p_dev->sdcard_info.rca, 1);
    if (ret != AM_OK) {
        return ret;
    }

    am_sdio_timeout_set(&timeout, 10);
    do {

        if (am_sdio_timeout(&timeout)) {
            return -AM_ETIME;
        }

        ret = am_sdcard_status_get(p_dev,
                                       p_dev->sdcard_info.rca,
                                       &current_status);

        if (ret == AM_OK) {
            current_status = (current_status >> 9)  & 0xf;
        }

    } while(current_status != 4);


    /* 设置bus宽度 */
    ret = am_sdcard_bus_width_set(p_dev,
                                  p_dev->sdcard_info.rca,
                                  p_dev->sdio_dev.mode);
    if (ret != AM_OK) {
        return ret;
    }

    ret = am_sdcard_block_size_set(p_dev,
                                   p_dev->sdcard_info.csd.sector_size);
    if (ret != AM_OK) {
        return ret;
    }

    AM_DBG_INFO("sd card block size %d bytes\r\n",
                p_dev->sdcard_info.csd.sector_size);
    AM_DBG_INFO("sd card capacity %d MB\r\n",
                p_dev->sdcard_info.csd.sector_cnt / 2 / 1024);

    return AM_OK;
}

/**
 * \brief 应用程序初始化(CMD8)
 */
int am_sdcard_send_if_cond (am_sdcard_handle_t handle,
                            uint32_t           ocr)
{
    uint32_t          rsp[4];
    uint32_t          arg;
    uint8_t           pattern;
    int               ret;

    if (handle == NULL) {
        return AM_ERROR;
    }

    arg = ((ocr & 0xFF8000) != 0) << 8 | 0xAA;

    ret = am_sdio_cmd_write(&handle->sdio_dev,
                            AM_SDIO_CMD8,
                            arg,
                            AM_SDIO_RESPONSE_SHORT,
                            rsp);
    if (ret != AM_OK) {
        return ret;
    }

    if (handle->sdio_dev.mode == AM_SDIO_SPI_M) {
        if ((rsp[0] & (AM_SDIO_SPI_R1_ALL_ERROR)) != 0) {
            return -AM_EIO;
        }
    }

    pattern = (handle->sdio_dev.mode == AM_SDIO_SPI_M) ? rsp[1] : rsp[0];

    if (pattern != 0xAA) {
        return -AM_EIO;
    }
    return AM_OK;
}

/**
 * \brief 应用程序初始化(CMD55 + ACMD41)
 */
int am_sdcard_send_app_op_cond (am_sdcard_handle_t handle,
                                uint32_t           rca,
                                uint32_t           ocr,
                                uint32_t          *p_rocr)
{
    uint32_t          rsp1[4];
    uint32_t          rsp2[4];
    uint32_t          arg;
    int               ret;
    uint8_t           rsp_type;

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    arg = (handle->sdio_dev.mode != AM_SDIO_SPI_M) ? (rca << 16) : 0;

    ret = am_sdio_cmd_write(&handle->sdio_dev,
                            AM_SDIO_CMD55,
                            arg,
                            AM_SDIO_RESPONSE_SHORT,
                            rsp1);
    if (ret != AM_OK) {
        return ret;
    }

    arg = (handle->sdio_dev.mode == AM_SDIO_SPI_M) ? (ocr & (1 << 30)) : ocr;

    //rsp_type = (handle->p_devinfo->mode == AM_SDIO_SPI_M) ? AM_SDIO_RSP_R1 : AM_SDIO_RSP_R3;

    rsp_type = AM_SDIO_RESPONSE_SHORT;

    ret = am_sdio_cmd_write(&handle->sdio_dev,
                            AM_SDIO_CMD41,
                            arg,
                            rsp_type,
                            rsp2);
    if (ret != AM_OK) {
        return ret;
    }

    /* check if enter APP CMD mode */
    if ((handle->sdio_dev.mode != AM_SDIO_SPI_M) &&
        !(rsp1[0] & AM_SDIO_R1_APP_CMD)) {
        return -AM_EIO;
    }

    if (handle->sdio_dev.mode == AM_SDIO_SPI_M) {

        if (rsp2[0] & AM_SDIO_SPI_R1_IN_IDLE_STATE) {

            if (rsp2[0] & AM_SDIO_SPI_R1_ALL_ERROR) {
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
    uint32_t          rsp[4];
    int               ret;

    if (NULL == handle || NULL == p_cid) {
        return -AM_EINVAL;
    }

    /* just SPI mode */
    if (handle->sdio_dev.mode == AM_SDIO_SPI_M) {
        return -AM_EINVAL;
    }

    ret = am_sdio_cmd_write(&handle->sdio_dev,
                            AM_SDIO_CMD2,
                            0,
                            AM_SDIO_RESPONSE_LONG,
                            rsp);
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
                               uint32_t          *p_rca)
{
    uint32_t          rsp[4] = {0};
    int               ret;

    if (NULL == handle || NULL == p_rca) {
        return -AM_EINVAL;
    }

    /* just SPI mode */
    if (handle->sdio_dev.mode == AM_SDIO_SPI_M) {
        return -AM_EINVAL;
    }

    ret = am_sdio_cmd_write(&handle->sdio_dev,
                            AM_SDIO_CMD3,
                            0,
                            AM_SDIO_RESPONSE_SHORT,
                            rsp);
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
    uint32_t          rsp[4] = {0};
    int               ret;
    uint32_t          arg;
    uint8_t           rsp_type;

    if (NULL == handle) {
        return -AM_EINVAL;
    }

    if (handle->sdio_dev.mode == AM_SDIO_SPI_M) {
        /* just SPI */
        return -AM_EINVAL;
    }

    arg = select ? (rca << 16) : 0;

    rsp_type = select ? AM_SDIO_RESPONSE_SHORT : AM_SDIO_RESPONSE_NO;

    ret = am_sdio_cmd_write(&handle->sdio_dev,
                            AM_SDIO_CMD7,
                            arg,
                            rsp_type,
                            rsp);
    return ret;
}

/**
 * \brief 获取存储卡寄存器CSD值(CMD9)
 */
int am_sdcard_csd_get (am_sdcard_handle_t handle,
                       uint32_t           rca,
                       uint32_t          *p_csd)
{
    uint32_t          rsp[4];
    int               ret;
    uint32_t          arg;

    if (NULL == handle || NULL == p_csd) {
        return -AM_EINVAL;
    }

    if (handle->sdio_dev.mode == AM_SDIO_SPI_M) {
        arg = 0;
    } else {
        arg = rca << 16;
    }

    if (handle->sdio_dev.mode == AM_SDIO_SPI_M) {
        am_sdio_write_then_read(&handle->sdio_dev,
                                AM_SDIO_CMD9,
                                arg,
                                AM_SDIO_RESPONSE_SHORT,
                                rsp,
                                p_csd,
                                16,
                                1);
    } else {
        ret = am_sdio_cmd_write(&handle->sdio_dev,
                                AM_SDIO_CMD9,
                                arg,
                                AM_SDIO_RESPONSE_LONG,
                                rsp);
    }

    if (ret != AM_OK) {
        return ret;
    }

    if (handle->sdio_dev.mode == AM_SDIO_SPI_M) {
        if ((rsp[0] & AM_SDIO_SPI_R1_ALL_ERROR) != 0) {
            return -AM_EIO;
        } else {
            uint8_t  i;
            uint32_t temp;
            for (i = 0; i < 2; i++) {
                temp       = __SDIO_SPI_SWAB32(p_csd[i]);
                p_csd[i]   = __SDIO_SPI_SWAB32(p_csd[3-i]);
                p_csd[3-i] = temp;
            }
        }
    } else {
        memcpy(p_csd, rsp, 16);
    }

    return AM_OK;
}

/**
 * \brief 获取存储卡寄存器SCR值(CMD55 + ACMD51)
 */
int am_sdcard_scr_get (am_sdcard_handle_t handle,
                       uint32_t           rca,
                       uint32_t          *p_scr)
{
    uint32_t             rsp1[4];
    uint32_t             rsp2[4];
    int                  ret;
    uint32_t             arg;
    uint32_t             temp;

    if (NULL == handle || NULL == p_scr) {
        return -AM_EINVAL;
    }

    arg = (handle->sdio_dev.mode != AM_SDIO_SPI_M) ? (rca << 16) : 0;

    ret = am_sdio_cmd_write(&handle->sdio_dev,
                            AM_SDIO_CMD55,
                            arg,
                            AM_SDIO_RESPONSE_SHORT,
                            rsp1);
    if (ret != AM_OK) {
        return ret;
    }

    ret = am_sdio_write_then_read(&handle->sdio_dev,
                                  AM_SDIO_CMD51,
                                  0,
                                  AM_SDIO_RESPONSE_SHORT,
                                  rsp2,
                                  p_scr,
                                  8,
                                  1);
    if (ret != AM_OK) {
        return ret;
    }

    if ((handle->sdio_dev.mode != AM_SDIO_SPI_M) &&
        !(rsp1[0] & AM_SDIO_R1_APP_CMD)) {
        return -AM_EIO;
    }

    temp     = __SDIO_SPI_SWAB32(p_scr[0]);
    p_scr[0] = __SDIO_SPI_SWAB32(p_scr[1]);
    p_scr[1] = temp;

    return AM_OK;
}

/**
 * \brief 设置存储卡总线宽度(CMD55 + ACMD6)
 */
int am_sdcard_bus_width_set (am_sdcard_handle_t handle,
                             uint32_t           rca,
                             uint8_t            width)
{
    uint32_t          rsp1[4];
    uint32_t          rsp2[4];
    uint32_t          arg;
    int               ret;

    if (NULL == handle) {
        return -AM_EINVAL;
    }

    arg = (handle->sdio_dev.mode != AM_SDIO_SPI_M) ? (rca << 16) : 0;

    ret = am_sdio_cmd_write(&handle->sdio_dev,
                            AM_SDIO_CMD55,
                            arg,
                            AM_SDIO_RESPONSE_SHORT,
                            rsp1);
    if (ret != AM_OK) {
        return ret;
    }

    if ((handle->sdio_dev.mode == AM_SDIO_SD_1B_M) &&
        (width == AM_SDIO_SD_1B_M)) {
        arg = 0;
    } else if ((handle->sdio_dev.mode == AM_SDIO_SD_4B_M) &&
               (width == AM_SDIO_SD_4B_M)) {
        arg = 2;
    } else {
        return -AM_EINVAL;
    }

    ret = am_sdio_cmd_write(&handle->sdio_dev,
                            AM_SDIO_ACMD6,
                            arg,
                            AM_SDIO_RESPONSE_SHORT,
                            rsp2);
    if (ret != AM_OK) {
        return ret;
    }

    if ((handle->sdio_dev.mode != AM_SDIO_SPI_M) &&
        !(rsp1[0] & AM_SDIO_R1_APP_CMD)) {
        return -AM_EIO;
    }

    return AM_OK;
}

/**
 * \brief SD Card 复位
 */
int am_sdcard_rest (am_sdcard_handle_t handle)
{
    int               ret;

    if (handle == NULL) {
        return AM_ERROR;
    }

    ret = am_sdio_cmd_write(&handle->sdio_dev,
                            AM_SDIO_CMD0,
                            0,
                            AM_SDIO_RESPONSE_NO,
                            NULL);
    return ret;
}

/**
 * \brief SD Card 设置块大小
 */
int am_sdcard_block_size_set(am_sdcard_handle_t handle,
                             uint32_t           blk_size)
{
    uint32_t          rsp[4];
    int               ret;

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    ret = am_sdio_cmd_write(&handle->sdio_dev,
                            AM_SDIO_CMD16,
                            blk_size,
                            AM_SDIO_RESPONSE_SHORT,
                            rsp);
    if (ret == AM_OK) {
        handle->blk_size = blk_size;
    }
    return ret;
}

/**
 * \brief 同步从存储卡中读取数据块
 */
int am_sdcard_blocks_read (am_sdcard_handle_t handle,
                           uint8_t           *p_buf,
                           uint32_t           blk_start,
                           uint32_t           blk_num)
{
    int               ret;
    uint32_t          addr;
    uint8_t           cmd;
    uint32_t          rsp[4];

    if (handle == NULL || p_buf == NULL) {
        return -AM_EINVAL;
    }

    if ((handle->sdcard_info.attribute & AM_SDCARD_SDHC) ||
        (handle->sdcard_info.attribute & AM_SDCARD_SDXC))
    {
        if (handle->blk_size != 512) {
            return -AM_ENOTSUP;
        }
        addr             = blk_start;
    } else {
        addr = blk_start * handle->blk_size;
    }

    cmd = (blk_num > 1) ? AM_SDIO_CMD18 : AM_SDIO_CMD17;
    ret = am_sdio_write_then_read(&handle->sdio_dev,
                                  cmd,
                                  addr,
                                  AM_SDIO_RESPONSE_SHORT,
                                  rsp,
                                  p_buf,
                                  handle->blk_size,
                                  blk_num);
    if (ret != AM_OK) {
        return ret;
    }

    if (handle->p_devinfo->mode == AM_SDIO_SPI_M) {
        if ((rsp[0] & (AM_SDIO_SPI_R1_IN_IDLE_STATE |
                        AM_SDIO_SPI_R1_ALL_ERROR)) != 0) {
            return -AM_EIO;
        }
    } else {
        if ((rsp[0] & 0xFFFF0000) != 0) {
            return -AM_EIO;
        }
    }

    if (blk_num > 1) {
        am_sdcard_transfer_stop(handle);
    }

    return ret;
}

/**
 * \brief 同步从存储卡中写入数据块
 */
int am_sdcard_blocks_write (am_sdcard_handle_t  handle,
                            uint8_t            *p_buf,
                            uint32_t            blk_start,
                            uint32_t            blk_num)
{
    int                   ret;
    uint32_t              status;
    uint32_t              addr;
    uint8_t               cmd;
    uint32_t              rsp[4];
    am_sdio_timeout_obj_t timeout;

    if (handle == NULL || p_buf == NULL) {
        return -AM_EINVAL;
    }

    if ((handle->sdcard_info.attribute & AM_SDCARD_SDHC) ||
        (handle->sdcard_info.attribute & AM_SDCARD_SDXC))
    {
        if (handle->blk_size != 512) {
            return -AM_ENOTSUP;
        }
        addr             = blk_start;
    } else {
        addr = blk_start * handle->blk_size;
    }

    cmd = (blk_num > 1) ? AM_SDIO_CMD25 : AM_SDIO_CMD24;
    ret = am_sdio_write_then_write(&handle->sdio_dev,
                                   cmd,
                                   addr,
                                   AM_SDIO_RESPONSE_SHORT,
                                   rsp,
                                   p_buf,
                                   handle->blk_size,
                                   blk_num);

    if (ret != AM_OK) {
        return ret;
    }

    if (handle->p_devinfo->mode == AM_SDIO_SPI_M) {
        if ((rsp[0] & (AM_SDIO_SPI_R1_IN_IDLE_STATE |
                        AM_SDIO_SPI_R1_ALL_ERROR)) != 0) {
            return -AM_EIO;
        }
    } else {
        if ((rsp[0] & 0xFFFF0000) != 0) {
            return -AM_EIO;
        }
    }

    if (blk_num > 1) {
        am_sdcard_transfer_stop(handle);
    }

    am_sdio_timeout_set(&timeout, 500);
    while (1) {
        if (am_sdio_timeout(&timeout)) {
            return -AM_ETIME;
        }

        /* get status of the card */
        ret = am_sdcard_status_get(handle,
                                       handle->sdcard_info.rca,
                                       &status);
        if (ret != AM_OK){
            return ret;
        }

        if (handle->sdio_dev.mode == AM_SDIO_SPI_M) {
            if ((status & (AM_SDIO_SPI_R2_ERROR | AM_SDIO_SPI_R2_CC_ERROR))) {
                ret = -AM_EIO;
            }
            break;
        } else {
            if ((status & AM_SDIO_R1_READY_FOR_DATA) &&
                (AM_SDIO_R1_CURRENT_STATE(status) != AM_SDIO_R1_STATE_PRG)) {
                break;
            }
        }
    }

    return ret;
}

/**
 * \brief 擦除块(CMD38)
 */
int am_sdcard_blocks_erase (am_sdcard_handle_t handle,
                            uint32_t           blk_start,
                            uint32_t           blk_num)
{
    uint32_t         rsp[4];
    uint32_t         addr_end;
    uint32_t         current_status;
    uint32_t         temp;
    int              ret;
    uint32_t         addr_start;
    uint16_t         blk_size   = handle->blk_size;

    am_sdio_timeout_obj_t timeout;

    if (handle == NULL || blk_num <= 0) {
        return -AM_EINVAL;
    }

    if ((handle->sdcard_info.attribute & AM_SDCARD_SDHC) ||
        (handle->sdcard_info.attribute & AM_SDCARD_SDXC))
    {
        addr_start = blk_start;
        addr_end   = addr_start + blk_num - 1;
    } else {
        addr_start = blk_start * blk_size;
        addr_end   = addr_start + blk_num * blk_size - 1;
    }

    ret = am_sdio_cmd_write(&handle->sdio_dev,
                            AM_SDIO_CMD32,
                            addr_start,
                            AM_SDIO_RESPONSE_SHORT,
                            rsp);
    if (ret != AM_OK) {
        return ret;
    }

    ret = am_sdio_cmd_write(&handle->sdio_dev,
                            AM_SDIO_CMD33,
                            addr_end,
                            AM_SDIO_RESPONSE_SHORT,
                            rsp);
    if (ret != AM_OK) {
        return ret;
    }

    ret = am_sdio_cmd_write(&handle->sdio_dev,
                            AM_SDIO_CMD38,
                            0,
                            AM_SDIO_RESPONSE_SHORT,
                            rsp);
    if (ret != AM_OK) {
        return ret;
    }

    am_sdio_timeout_set(&timeout, 500);
    do {

        if (am_sdio_timeout(&timeout)) {
            return -AM_ETIME;
        }

        am_sdcard_status_get(handle,
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
    uint32_t        rsp[4];
    int             ret;

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    ret = am_sdio_cmd_write(&handle->sdio_dev,
                            AM_SDIO_CMD12,
                            0,
                            AM_SDIO_RESPONSE_SHORT,
                            rsp);
    return ret;
}

/**
 * \brief 获取卡状态(CMD13)
 */
int am_sdcard_status_get (am_sdcard_handle_t handle,
                              uint32_t           rca,
                              uint32_t          *p_status)
{
    uint32_t        rsp[4];
    uint32_t        arg;
    uint8_t         rsp_type;
    int             ret;

    if (NULL == handle || NULL == p_status) {
        return -AM_EINVAL;
    }

    arg = (handle->sdio_dev.mode != AM_SDIO_SPI_M) ? (rca << 16) : 0;

    rsp_type = (handle->sdio_dev.mode == AM_SDIO_SPI_M) ? AM_SDIO_RSP_R2 : AM_SDIO_RSP_R1;

    ret = am_sdio_cmd_write(&handle->sdio_dev,
                            AM_SDIO_CMD13,
                            arg,
                            rsp_type,
                            rsp);

    if (handle->sdio_dev.mode != AM_SDIO_SPI_M) {
        *p_status = rsp[0];
    } else {
        *p_status = rsp[0] >> 16;
    }

    return ret;
}

am_sdcard_handle_t am_sdcard_init(am_sdcard_dev_t           *p_dev,
                                  const am_sdcard_devinfo_t *p_devinfo,
                                  am_sdio_handle_t           sdio_handle)
{
    int ret;

    if ((p_dev == NULL) || (p_devinfo == NULL) || (sdio_handle== NULL)) {
        return NULL;
    }

    p_dev->p_devinfo   = p_devinfo;

    am_sdio_mkdev(&p_dev->sdio_dev,
                  sdio_handle,
                  p_dev->p_devinfo->mode,
                  p_dev->p_devinfo->speed,
                  p_dev->p_devinfo->sdio_crc_en);

    am_sdio_setup(&p_dev->sdio_dev);

    am_wait_init(&p_dev->wait);

    ret = __sdcard_hard_init(p_dev);

    p_dev->blk_size = p_dev->sdcard_info.csd.sector_size;

    if (ret != AM_OK) {
        return NULL;
    }

    return p_dev;
}

/******************************************************************************/
int am_sdcard_deinit (am_sdcard_handle_t handle)
{
    if (handle == NULL) {
        return -AM_EINVAL;
    }

    handle->p_devinfo            = NULL;

    return AM_OK;
}

/* end of file */
