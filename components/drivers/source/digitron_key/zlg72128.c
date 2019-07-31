/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2019 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief ZLG72128 driver implement
 *
 * \internal
 * \par Modification History
 * - 1.00 19-4-25  xgg, first implementation.
 * \endinternal
 */
#include "zlg72128.h"

/*******************************************************************************
  Sub register defines
*******************************************************************************/
#define __ZLG72128_REGADDR_SYSTEM        0x00
#define __ZLG72128_REGADDR_KEY           0x01
#define __ZLG72128_REGADDR_REPEATCNT     0x02
#define __ZLG72128_REGADDR_FUNCTION_KEY  0x03

#define __ZLG72128_REGADDR_CMDBUF0       0x07
#define __ZLG72128_REGADDR_CMDBUF1       0x08

#define __ZLG72128_REGADDR_FLASH_TIME    0x0B

#define __ZLG72128_REGADDR_DISP_CTRL0    0x0C
#define __ZLG72128_REGADDR_DISP_CTRL1    0x0D

#define __ZLG72128_REGADDR_FLASH_CTRL0   0x0E
#define __ZLG72128_REGADDR_FLASH_CTRL1   0x0F

#define __ZLG72128_REGADDR_DISPBUF0      0x10
#define __ZLG72128_REGADDR_DISPBUF1      0x11
#define __ZLG72128_REGADDR_DISPBUF(n)   (0x10 + (n))

#define __ZLG72128_KEYVAL_VAILD          0x01
/*******************************************************************************
  cmd defines(for cmdbuf0(0x07))
*******************************************************************************/
/* seg off */
#define __ZLG72128_DIGITRON_CMD_SEGOFF           0x10

/* seg on  */
#define __ZLG72128_DIGITRON_CMD_SEGON            0x11

/* download the char and decode */
#define __ZLG72128_DIGITRON_CMD_DOWNLOAD(pos)   (0x20 + ((pos) & 0xF))

/* rest cmd */
#define __ZLG72128_DIGITRON_CMD_RESET            0x30

/* test cmd */
#define __ZLG72128_DIGITRON_CMD_TEST             0x40

/* shift cmd */
#define __ZLG72128_DIGITRON_CMD_SHIFT(is_left, is_cyclic, n) \
    (((is_left) ? 0x50 : 0x70) + ((is_cyclic) ? 0x10 : 0x00) + ((n) & 0x0F))

/* scanning cmd*/
#define __ZLG72128_DIGTRON_CMD_SCAN(num)        (0x90 + ((num) & 0x0F))

/*******************************************************************************
  fixed options defines
*******************************************************************************/

/* 数码管最大有效数*/
#define __ZLG72128_DIGITRON_NUM_MAX  12

/**
 * \name 传输缓存的状态
 * @{
 */

#define __ZLG72128_TRANS_BUF_NONE     0   /* 传输缓存中无待传输的数据 */
#define __ZLG72128_TRANS_BUF_FILLING  1   /* 传输信息填充中           */
#define __ZLG72128_TRANS_BUF_FILLED   2   /* 传输信息已填充，等待发送 */

/** @} */


/**
 * \brief   I2C 当前的状态
 * @{
 */

#define __ZLG72128_I2C_IDLE  0        /* I2C当前空闲   */
#define __ZLG72128_I2C_BUSY  1        /* I2C正在传输中 */

/** @} */

/*******************************************************************************
  Local Functions
*******************************************************************************/
static int __zlg72128_data_trans (zlg72128_dev_t  *p_dev,
                                  uint8_t          regaddr,
                                  uint8_t         *p_buf,
                                  uint8_t          len)
{
    return zlg72128_plfm_i2c_write(&p_dev->plfm,
                                    regaddr,
                                    p_buf,
                                    len);
}

/******************************************************************************/
static void __zlg72128_keyval_report (void *p_arg, uint8_t key[3])
{
    zlg72128_dev_t *p_dev = (zlg72128_dev_t *)p_arg;

    if (p_dev == NULL) {
        return;
    }

    p_dev->pfn_key_cb(p_dev->pfn_key_arg, key[0], key[1], key[2]);
}

/******************************************************************************/
zlg72128_handle_t zlg72128_init (zlg72128_dev_t           *p_dev,
                                 const zlg72128_devinfo_t *p_devinfo)
{
    int ret = 0;

    if (p_dev == NULL || p_devinfo == NULL) {
        return NULL;
    }

    ret = zlg72128_plfm_init(&p_dev->plfm,
                             &p_devinfo->plfm_info,
                              __zlg72128_keyval_report,
                              p_dev);

    if (ret) {
        return NULL;
    }

    return p_dev;
}

/******************************************************************************/
int zlg72128_key_cb_set (zlg72128_handle_t  handle,
                         zlg72128_key_cb_t  pfn_key_cb,
                         void              *p_arg)
{
    if ((pfn_key_cb != NULL) && (handle != NULL)) {

        handle->pfn_key_cb  = pfn_key_cb;
        handle->pfn_key_arg = p_arg;

        return 0;
    }

    return -2;
}

/******************************************************************************/
int zlg72128_digitron_flash_time_cfg (zlg72128_handle_t  handle,
                                      uint16_t           on_ms,
                                      uint16_t           off_ms)
{
    uint8_t ctrl;
    uint8_t on_n, off_n;

    if (handle == NULL) {
        return -2;
    }

    if (on_ms < 150) {
        on_n = 0;
    } else if (on_ms > 900) {
        on_n = 0x0F;
    } else {
        on_n = ((on_ms - 150) + 25) / 50;
    }

    if (off_ms < 150) {
        off_n = 0;
    } else if (off_ms > 900) {
        off_n = 0x0F;
    } else {
        off_n = ((off_ms - 150) + 25) / 50;
    }

    on_n  &= 0xF;
    off_n &= 0xF;

    ctrl = (on_n << 4) | off_n;

    return __zlg72128_data_trans(handle,
                                 __ZLG72128_REGADDR_FLASH_TIME,
                                &ctrl,
                                 1);
}

/******************************************************************************/
int zlg72128_digitron_flash_ctrl (zlg72128_handle_t  handle,
                                  uint16_t           ctrl_val)
{
    uint8_t ctrl[2];

    if (handle == NULL) {
        return -2;
    }

    /* 低字节控制的高8位，高字节控制的低8位 */
    ctrl[0] = (ctrl_val >> 8) & 0xF;      /**< \brief 高8位后发送 */
    ctrl[1] =  ctrl_val & 0xFF;           /**< \brief 低8位后发送 */

    return __zlg72128_data_trans(handle,
                                 __ZLG72128_REGADDR_FLASH_CTRL0,
                                 ctrl,
                                 2);
}

/******************************************************************************/
int zlg72128_digitron_disp_ctrl (zlg72128_handle_t  handle,
                                 uint16_t           ctrl_val)
{
    uint8_t ctrl[2];

    if (handle == NULL) {
        return -2;
    }

    /* 低字节控制的高8位，高字节控制的低8位 */
    ctrl[0] = (ctrl_val >> 8) & 0xF;  /**< \brief 高8位后发送 */
    ctrl[1] =  ctrl_val & 0xFF;       /**< \brief 低8位后发送 */

    return __zlg72128_data_trans(handle,
                                 __ZLG72128_REGADDR_DISP_CTRL0,
                                 ctrl,
                                 2);
}

/******************************************************************************/
int zlg72128_digitron_dispbuf_set (zlg72128_handle_t  handle,
                                   uint8_t            start_pos,
                                   uint8_t           *p_buf,
                                   uint8_t            num)
{
    if ((p_buf == NULL)  || \
        (handle == NULL) || \
        (start_pos >= __ZLG72128_DIGITRON_NUM_MAX) ) {

        return -2;
    }

    if ((start_pos + num) > 12) {
        num = 12 - start_pos;
    }

    return __zlg72128_data_trans(handle,
                                __ZLG72128_REGADDR_DISPBUF(start_pos),
                                p_buf,
                                num);
}

/******************************************************************************/
int zlg72128_digitron_disp_char (zlg72128_handle_t  handle,
                                 uint8_t            pos,
                                 char               ch,
                                 bool_t             is_dp_disp,
                                 bool_t             is_flash)

{
    static const char code[32] = {
        '0', '1', '2', '3', '4', '5', '6', '7',
        '8', '9', 'A', 'b', 'C', 'd', 'E', 'F',
        'G', 'H', 'i', 'J', 'L', 'o', 'p', 'q',
        'r', 't', 'U', 'y', 'c', 'h', 'T', ' ',
    };

    uint8_t i;
    uint8_t cmd[2];

    if ((handle == NULL) || (pos >= __ZLG72128_DIGITRON_NUM_MAX)) {
        return -2;
    }

    for (i = 0; i < sizeof(code); i++) {

        if (code[i] == ch) {
            break;
        }
    }

    if (i == sizeof(code)) {
        return -2;
    }

    cmd[0] = __ZLG72128_DIGITRON_CMD_DOWNLOAD(pos);
    cmd[1] = i;

    if (is_dp_disp) {
        cmd[1] |= (1 << 7);
    }

    if (is_flash) {
        cmd[1] |= (1 << 6);
    }

    return __zlg72128_data_trans(handle,
                                __ZLG72128_REGADDR_CMDBUF0,
                                cmd,
                                2);
}

/******************************************************************************/
int zlg72128_digitron_disp_str (zlg72128_handle_t  handle,
                                uint8_t            start_pos,
                                const char        *p_str)
{
    int ret = 0;

    if ((handle == NULL) || \
        (p_str == NULL)  || \
        start_pos >= __ZLG72128_DIGITRON_NUM_MAX) {

        return -2;
    }

    while (*p_str != '\0' && start_pos < __ZLG72128_DIGITRON_NUM_MAX) {

        /* independent '.' */
        if (*p_str == '.') {

            /* dsiplay '.' */
            ret = zlg72128_digitron_disp_char(handle,
                                              start_pos,
                                              ' ',
                                              1,
                                              0);
        } else {

            /* The next char is '.', just display at here */
            if (*(p_str + 1) == '.') {

                /* The '.' display at here */
                ret = zlg72128_digitron_disp_char(handle,
                                                  start_pos,
                                                 *p_str,
                                                  1,
                                                  0);

                /* skip the '.', the pos needn't increase */
                p_str++;

            } else {

                ret = zlg72128_digitron_disp_char(handle,
                                                  start_pos,
                                                 *p_str,
                                                  0,
                                                  0);
            }

        }

        if (ret != 0) {
            return ret;
        }

        start_pos++;
        p_str++;
    }

    return ret;
}

/******************************************************************************/
int zlg72128_digitron_disp_num (zlg72128_handle_t  handle,
                                uint8_t            pos,
                                uint8_t            num,
                                bool_t             is_dp_disp,
                                bool_t             is_flash)
{
    uint8_t cmd[2];

    if ((num >= 10) || \
        (handle == NULL) || \
        (pos >= __ZLG72128_DIGITRON_NUM_MAX)) {
        return -2;
    }

    cmd[0] = __ZLG72128_DIGITRON_CMD_DOWNLOAD(pos);
    cmd[1] = num;

    if (is_dp_disp) {
        cmd[1] |= (1 << 7);
    }

    if (is_flash) {
        cmd[1] |= (1 << 6);
    }

    return __zlg72128_data_trans(handle,
                                 __ZLG72128_REGADDR_CMDBUF0,
                                 cmd,
                                 2);
}

/******************************************************************************/
int zlg72128_digitron_seg_ctrl (zlg72128_handle_t  handle,
                                uint8_t            pos,
                                char               seg,
                                bool_t             is_on)
{
    uint8_t cmd[2];

    /* 参数有效化检测*/
    if ((handle == NULL) || \
        (seg >= 8)       || \
        (pos >= __ZLG72128_DIGITRON_NUM_MAX)) {
        return -2;
    }

    if (is_on) {
        cmd[0] = __ZLG72128_DIGITRON_CMD_SEGON;
    } else {
        cmd[0] = __ZLG72128_DIGITRON_CMD_SEGOFF;
    }

    cmd[1] = ((pos & 0x0F) << 4) | (seg);

    return __zlg72128_data_trans(handle,
                                 __ZLG72128_REGADDR_CMDBUF0,
                                 cmd,
                                 2);
}

/******************************************************************************/
int zlg72128_digitron_shift (zlg72128_handle_t  handle,
                             uint8_t            dir,
                             bool_t             is_cyclic,
                             uint8_t            num)
{
    uint8_t cmd;

    if ((handle == NULL) || \
        (num >= __ZLG72128_DIGITRON_NUM_MAX)) {
        return -2;
    }

    /* 低4位为移位的位数, dir (0-right，1-left) */
    cmd = __ZLG72128_DIGITRON_CMD_SHIFT(dir, is_cyclic, num);

    return __zlg72128_data_trans(handle,
                                 __ZLG72128_REGADDR_CMDBUF0,
                                 &cmd,
                                 1);

}

/******************************************************************************/
int zlg72128_digitron_scan_set (zlg72128_handle_t  handle,
                                uint8_t            num)
{
    uint8_t cmd;

    if (handle == NULL) {
        return -2;
    }

    if (num > __ZLG72128_DIGITRON_NUM_MAX) {
        num = 12;
    }

    /* 低4位为扫描的位数 */
    cmd = __ZLG72128_DIGTRON_CMD_SCAN(num);

    return __zlg72128_data_trans(handle,
                                 __ZLG72128_REGADDR_CMDBUF0,
                                 &cmd,
                                 1);
}

/******************************************************************************/
int zlg72128_digitron_disp_reset (zlg72128_handle_t  handle)
{
    uint8_t cmd ;

    if (handle == NULL) {
        return -2;
    }

    cmd = __ZLG72128_DIGITRON_CMD_RESET;

    return __zlg72128_data_trans(handle,
                                 __ZLG72128_REGADDR_CMDBUF0,
                                 &cmd,
                                 1);
}

/******************************************************************************/
int zlg72128_digitron_disp_test (zlg72128_handle_t handle)
{
    uint8_t cmd;

    if (handle == NULL) {
        return -2;
    }

    cmd = __ZLG72128_DIGITRON_CMD_TEST;

    return __zlg72128_data_trans(handle,
                                 __ZLG72128_REGADDR_CMDBUF0,
                                 &cmd,
                                 1);
}

/******************************************************************************/
int zlg72128_deinit(zlg72128_handle_t handle)
{
    if (handle == NULL) {
        return -2;
    }

    /* 平台解初始化*/
    return zlg72128_plfm_deinit(&handle->plfm);
}

/* end of file */
