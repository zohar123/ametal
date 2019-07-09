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
 * \brief tcp_echoclient.c
 *
 * \internal
 * \par Modification History
 * - 1.00 19-06-11  wyy, first implementation
 * \endinternal
 */

/***************************************************************************
 includes
 ***************************************************************************/

#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"
#include "memp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "eth_conf.h"
#include "am_zlg_phy.h"
#include "am_vdebug.h"

#if LWIP_TCP

u8_t recev_buf[50];
__IO uint32_t message_count = 0;
u8_t data[100];

struct tcp_pcb *echoclient_pcb;

enum echoclient_states {
    ES_NOT_CONNECTED = 0, 
    ES_CONNECTED, 
    ES_RECEIVED, 
    ES_CLOSING,
};

struct echoclient {
    enum   echoclient_states state; 
    struct tcp_pcb          *pcb;   
    struct pbuf             *p_tx;  
};

struct echoclient * echoclient_es;

static err_t eth_tcp_echoclient_recv(void *arg, struct tcp_pcb *tpcb,
        struct pbuf *p, err_t err);
static void eth_tcp_echoclient_connection_close(struct tcp_pcb *tpcb,
        struct echoclient * es);
static err_t eth_tcp_echoclient_poll(void *arg, struct tcp_pcb *tpcb);
static err_t eth_tcp_echoclient_sent(void *arg, struct tcp_pcb *tpcb,
        u16_t len);
static void eth_tcp_echoclient_send(struct tcp_pcb *tpcb,
        struct echoclient * es);
static err_t eth_tcp_echoclient_connected(void *arg, struct tcp_pcb *tpcb,
        err_t err);


/**
 * \brief  Connects to the TCP echo server
 * \param  None
 * \retval None
 */
void am_zlg_eth_tcp_client_init(void)
{
    struct ip_addr dest_ip_addr;

    echoclient_pcb = tcp_new();

    if (echoclient_pcb != NULL) {
        IP4_ADDR(&dest_ip_addr, g_eth_dest_ip[0], g_eth_dest_ip[1],
                g_eth_dest_ip[2], g_eth_dest_ip[3]);

        tcp_connect(echoclient_pcb, &dest_ip_addr, g_eth_dest_port,
                eth_tcp_echoclient_connected);
    } else {
        memp_free(MEMP_TCP_PCB, echoclient_pcb);
#ifdef SERIAL_DEBUG
        printf("\n\r can not create tcp pcb");
#endif 
    }
}

/**
 * \brief  Disconnects to the TCP echo server
 * \param  None
 * \retval None
 */
void am_zlg_eth_tcp_client_close(void)
{
    eth_tcp_echoclient_connection_close(echoclient_pcb, echoclient_es);
#ifdef SERIAL_DEBUG
    printf("\n\r close TCP connection");
#endif 
}

/**
 * \brief Function called when TCP connection established
 * \param tpcb: pointer on the connection contol block
 * \param err: when connection correctly established err should be ERR_OK
 * \retval err_t: returned error
 */
static err_t eth_tcp_echoclient_connected(void *arg, struct tcp_pcb *tpcb,
        err_t err)
{
    struct echoclient *es = NULL;

    if (err == ERR_OK) {
        es = (struct echoclient *) mem_malloc(sizeof(struct echoclient));
        echoclient_es = es;
        if (es != NULL) {
            es->state = ES_CONNECTED;
            es->pcb = tpcb;

            sprintf((char*) data, "sending tcp client message %u",
                    (unsigned int) message_count);

            es->p_tx = pbuf_alloc(PBUF_TRANSPORT, strlen((char*) data),
                    PBUF_POOL);

            if (es->p_tx) {
                pbuf_take(es->p_tx, (char*) data, strlen((char*) data));

                tcp_arg(tpcb, es);

                tcp_recv(tpcb, eth_tcp_echoclient_recv);

                tcp_sent(tpcb, eth_tcp_echoclient_sent);

                tcp_poll(tpcb, eth_tcp_echoclient_poll, 1);

                eth_tcp_echoclient_send(tpcb, es);

                return ERR_OK;
            }
        } else {
            eth_tcp_echoclient_connection_close(tpcb, es);

            return ERR_MEM;
        }
    } else {
        eth_tcp_echoclient_connection_close(tpcb, es);
    }
    return err;
}

/**
 * \brief tcp_receiv callback
 * \param arg: argument to be passed to receive callback
 * \param tpcb: tcp connection control block
 * \param err: receive error code
 * \retval err_t: retuned error
 */
static err_t eth_tcp_echoclient_recv(void *arg, struct tcp_pcb *tpcb,
        struct pbuf *p, err_t err)
{
    char *recdata = 0;

    struct echoclient *es;
    err_t ret_err;

    LWIP_ASSERT("arg != NULL", arg != NULL);

    es = (struct echoclient *) arg;

    if (p == NULL) {
        es->state = ES_CLOSING;
        if (es->p_tx == NULL) {
            eth_tcp_echoclient_connection_close(tpcb, es);
        } else {
            eth_tcp_echoclient_send(tpcb, es);
        }
        ret_err = ERR_OK;
    }
    else if (err != ERR_OK) {
        pbuf_free(p);

        ret_err = err;
    } else if (es->state == ES_CONNECTED) {
        message_count++;

        tcp_recved(tpcb, p->tot_len);
        recdata = (char *) malloc(p->len * sizeof(char));
        if (recdata != NULL) {
            memcpy(recdata, p->payload, p->len);
            recdata[p->len] = '\0';
        }

        es->p_tx = pbuf_alloc(PBUF_TRANSPORT, p->len * sizeof(char), PBUF_POOL);
        if (es->p_tx != NULL) {
            pbuf_take(es->p_tx, (char*) recdata, p->len * sizeof(char));
            eth_tcp_echoclient_send(tpcb, es);
        }
        free(recdata);

        pbuf_free(p);
        ret_err = ERR_OK;
    }
    else {
        tcp_recved(tpcb, p->tot_len);

        pbuf_free(p);
        ret_err = ERR_OK;
    }
    return ret_err;
}

/**
 * \brief function used to send data
 * \param  tpcb: tcp control block
 * \param  es: pointer on structure of type echoclient containing info on data
 *             to be sent
 * \retval None
 */
static void eth_tcp_echoclient_send(struct tcp_pcb *tpcb,
        struct echoclient * es)
{
    struct pbuf *ptr;
    err_t wr_err = ERR_OK;

    while ((wr_err == ERR_OK) && (es->p_tx != NULL)
            && (es->p_tx->len <= tcp_sndbuf(tpcb))) {

        ptr = es->p_tx;

        wr_err = tcp_write(tpcb, ptr->payload, ptr->len, 1);

        if (wr_err == ERR_OK) {
            es->p_tx = ptr->next;

            if (es->p_tx != NULL) {
                pbuf_ref(es->p_tx);
            }

            pbuf_free(ptr);
        } else if (wr_err == ERR_MEM) {
            es->p_tx = ptr;
        } else {
            /* other problem ?? */
        }
    }
}

/**
 * \brief  This function implements the tcp_poll callback function
 * \param  arg: pointer on argument passed to callback
 * \param  tpcb: tcp connection control block
 * \retval err_t: error code
 */
static err_t eth_tcp_echoclient_poll(void *arg, struct tcp_pcb *tpcb)
{
    err_t ret_err;
    struct echoclient *es;

    es = (struct echoclient*) arg;
    if (es != NULL) {
        if (es->p_tx != NULL) {
            eth_tcp_echoclient_send(tpcb, es);
        } else {
            if (es->state == ES_CLOSING) {
                eth_tcp_echoclient_connection_close(tpcb, es);
            }
        }
        ret_err = ERR_OK;
    } else {
        tcp_abort(tpcb);
        ret_err = ERR_ABRT;
    }
    return ret_err;
}

/**
 * \brief  This function implements the tcp_sent LwIP callback (called when ACK
 *         is received from remote host for sent data)
 * \param  arg: pointer on argument passed to callback
 * \param  tcp_pcb: tcp connection control block
 * \param  len: length of data sent
 * \retval err_t: returned error code
 */
static err_t eth_tcp_echoclient_sent(void *arg, struct tcp_pcb *tpcb,
        u16_t len)
{
    struct echoclient *es;

    LWIP_UNUSED_ARG(len);

    es = (struct echoclient *) arg;

    if (es->p_tx != NULL) {
        eth_tcp_echoclient_send(tpcb, es);
    }

    return ERR_OK;
}

/**
 * \brief This function is used to close the tcp connection with server
 * \param tpcb: tcp connection control block
 * \param es: pointer on echoclient structure
 * \retval None
 */
static void eth_tcp_echoclient_connection_close(struct tcp_pcb *tpcb,
        struct echoclient * es)
{
    tcp_recv(tpcb, NULL);
    tcp_sent(tpcb, NULL);
    tcp_poll(tpcb, NULL, 0);

    if (es != NULL) {
        mem_free(es);
    }

    tcp_close(tpcb);

}

#endif
