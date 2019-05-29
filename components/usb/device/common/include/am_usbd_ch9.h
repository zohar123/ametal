/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief USBD CH9
 *
 * \internal
 * \par Modification history
 * - 1.00 16-9-27  bob, first implementation.
 * \endinternal
 */

#ifndef __AM_USBD_CH9_H
#define __AM_USBD_CH9_H

#include "ametal.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct  am_usb_request_funcs{
    uint8_t  (*pfn_status_get)   (void *p_drv, uint8_t **buf, uint16_t *p_length);
    uint8_t  (*pfn_feature_clr)  (void *p_drv);
    uint8_t  (*pfn_feature_set)  (void *p_drv);
    uint8_t  (*pfn_address_set)  (void *p_drv, uint8_t adr);
    uint8_t  (*pfn_config_get)   (void *p_drv, uint8_t **buf, uint16_t *p_length);
    uint8_t  (*pfn_config_set)   (void *p_drv);
    uint8_t  (*pfn_desc_get)     (void *p_drv, uint8_t **buf, uint16_t *p_length);
    uint8_t  (*pfn_interface_get)(void *p_drv, uint8_t **buf, uint16_t *p_length);
    uint8_t  (*pfn_interface_set)(void *p_drv);
}am_usb_request_funcs_t;


void am_usbd_ch9_std_request_init(void *p_arg);


#ifdef __cplusplus
}
#endif

#endif /* __AM_USBD_CH9_H */

/* end of file */
