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
 * \brief tcp_echoclient.h
 *
 * \internal
 * \par Modification History
 * - 1.00 19-06-11  wyy, first implementation
 * \endinternal
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TCP_CLIENT_H__
#define __TCP_CLIENT_H__

/***************************************************************************
Exported functions
***************************************************************************/
void am_eth_tcp_client_init(void);
void am_eth_tcp_client_close(void);
#endif /* __TCP_CLIENT_H__ */
