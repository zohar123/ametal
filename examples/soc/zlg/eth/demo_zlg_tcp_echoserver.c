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
 * \brief tcp_echoserver.c
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
#include "eth_conf.h"
#include <stdlib.h>
#include <string.h>

#if LWIP_TCP

static struct tcp_pcb *tcp_echoserver_pcb;

enum tcp_echoserver_states {
    ES_NONE = 0, ES_ACCEPTED, ES_RECEIVED, ES_CLOSING
};

struct tcp_echoserver_struct {
    u8_t state; /* current connection state */
    struct tcp_pcb *pcb; /* pointer on the current tcp_pcb */
    struct pbuf *p; /* pointer on the received/to be transmitted pbuf */
};

struct tcp_echoserver_struct *tcp_echoserver_es;

static err_t am_eth_tcp_echoserver_accept(void *arg, struct tcp_pcb *newpcb,
        err_t err);
static err_t am_eth_tcp_echoserver_recv(void *arg, struct tcp_pcb *tpcb,
        struct pbuf *p, err_t err);
static void am_eth_tcp_echoserver_error(void *arg, err_t err);
static err_t am_eth_tcp_echoserver_poll(void *arg, struct tcp_pcb *tpcb);
static err_t am_eth_tcp_echoserver_sent(void *arg, struct tcp_pcb *tpcb,
        u16_t len);
static void am_eth_tcp_echoserver_send(struct tcp_pcb *tpcb,
        struct tcp_echoserver_struct *es);
static void am_eth_tcp_echoserver_connection_close(struct tcp_pcb *tpcb,
        struct tcp_echoserver_struct *es);

/**
 * @brief  Initializes the tcp echo server
 * @param  None
 * @retval None
 */
void am_eth_tcp_server_init(void)
{
    tcp_echoserver_pcb = tcp_new();

    if (tcp_echoserver_pcb != NULL) {
        err_t err;
        err = tcp_bind(tcp_echoserver_pcb, IP_ADDR_ANY,
                g_eth_local_port);

        if (err == ERR_OK) {
            tcp_echoserver_pcb = tcp_listen(tcp_echoserver_pcb);
            tcp_accept(tcp_echoserver_pcb, am_eth_tcp_echoserver_accept);
            printf("wait for client............\n");
            printf("...........................\n");
        } else {
            memp_free(MEMP_TCP_PCB, tcp_echoserver_pcb);
            printf("Can not bind pcb\n");
        }
    } else {
        printf("Can not create new pcb\n");
    }
}

/**
 * @brief  This functions closes the tcp connection
 * @param  None
 * @retval None
 */
void am_eth_tcp_server_close(void)
{
    am_eth_tcp_echoserver_connection_close(tcp_echoserver_pcb,
            tcp_echoserver_es);
#ifdef SERIAL_DEBUG
    printf("closes the tcp server\n");
#endif
}

/**
 * @brief  This function is the implementation of tcp_accept LwIP callback
 * @param  arg: not used
 * @param  newpcb: pointer on tcp_pcb struct for the newly created tcp connection
 * @param  err: not used
 * @retval err_t: error status
 */
static err_t am_eth_tcp_echoserver_accept(void *arg, struct tcp_pcb *newpcb,
        err_t err)
{
    err_t ret_err;
    struct tcp_echoserver_struct *es;
    printf("client already connect............\n");

    LWIP_UNUSED_ARG(arg);
    LWIP_UNUSED_ARG(err);
    tcp_setprio(newpcb, TCP_PRIO_MIN);
    es = (struct tcp_echoserver_struct *) mem_malloc(
            sizeof(struct tcp_echoserver_struct));

    tcp_echoserver_es = es;

    if (es != NULL) {
        es->state = ES_ACCEPTED;
        es->pcb = newpcb;
        es->p = NULL;
        tcp_arg(newpcb, es);
        tcp_recv(newpcb, am_eth_tcp_echoserver_recv);
        tcp_err(newpcb, am_eth_tcp_echoserver_error);
        tcp_poll(newpcb, am_eth_tcp_echoserver_poll, 1);
        ret_err = ERR_OK;
    } else {
        am_eth_tcp_echoserver_connection_close(newpcb, es);
        ret_err = ERR_MEM;
    }
    return ret_err;
}

/**
 * @brief  This function is the implementation for tcp_recv LwIP callback
 * @param  arg: pointer on a argument for the tcp_pcb connection
 * @param  tpcb: pointer on the tcp_pcb connection
 * @param  pbuf: pointer on the received pbuf
 * @param  err: error information regarding the reveived pbuf
 * @retval err_t: error code
 */
static err_t am_eth_tcp_echoserver_recv(void *arg, struct tcp_pcb *tpcb,
        struct pbuf *p, err_t err)
{
#ifdef SERIAL_DEBUG
    char *recdata=0;
#endif

    struct tcp_echoserver_struct *es;
    err_t ret_err;

    LWIP_ASSERT("arg != NULL", arg != NULL);

    es = (struct tcp_echoserver_struct *) arg;
    if (p == NULL) {
        es->state = ES_CLOSING;
        if (es->p == NULL) {
            am_eth_tcp_echoserver_connection_close(tpcb, es);
        } else {
            tcp_sent(tpcb, am_eth_tcp_echoserver_sent);
            am_eth_tcp_echoserver_send(tpcb, es);
        }
        ret_err = ERR_OK;
    } else if (err != ERR_OK) {
        es->p = NULL;
        pbuf_free(p);
        ret_err = err;
    } else if (es->state == ES_ACCEPTED) {
        es->state = ES_RECEIVED;
        es->p = p;
        tcp_sent(tpcb, am_eth_tcp_echoserver_sent);

#ifdef SERIAL_DEBUG
        recdata=(char *)malloc(p->len*sizeof(char));
        if(recdata!=NULL)
        {
            memcpy(recdata,p->payload,p->len);
            printf("upd_rec:%s",recdata);
        }
        free(recdata);
#endif    
        am_eth_tcp_echoserver_send(tpcb, es);

        ret_err = ERR_OK;
    } else if (es->state == ES_RECEIVED) {
        if (es->p == NULL) {
            es->p = p;

#ifdef SERIAL_DEBUG
            recdata=(char *)malloc(p->len*sizeof(char));
            if(recdata!=NULL)
            {
                memcpy(recdata,p->payload,p->len);
                printf("upd_rec:%s",recdata);
            }
            free(recdata);
#endif
            am_eth_tcp_echoserver_send(tpcb, es);
        } else {
            struct pbuf *ptr;
            ptr = es->p;
            pbuf_chain(ptr, p);
        }
        ret_err = ERR_OK;
    } else {
        tcp_recved(tpcb, p->tot_len);
        es->p = NULL;
        pbuf_free(p);
        ret_err = ERR_OK;
    }
    return ret_err;
}

/**
 * @brief  This function implements the tcp_err callback function (called
 *         when a fatal tcp_connection error occurs.
 * @param  arg: pointer on argument parameter
 * @param  err: not used
 * @retval None
 */
static void am_eth_tcp_echoserver_error(void *arg, err_t err)
{
    struct tcp_echoserver_struct *es;

    LWIP_UNUSED_ARG(err);

    es = (struct tcp_echoserver_struct *) arg;
    if (es != NULL) {
        mem_free(es);
    }
}

/**
 * @brief  This function implements the tcp_poll LwIP callback function
 * @param  arg: pointer on argument passed to callback
 * @param  tpcb: pointer on the tcp_pcb for the current tcp connection
 * @retval err_t: error code
 */
static err_t am_eth_tcp_echoserver_poll(void *arg, struct tcp_pcb *tpcb)
{
    err_t ret_err;
    struct tcp_echoserver_struct *es;

    es = (struct tcp_echoserver_struct *) arg;
    if (es != NULL) {
        if (es->p != NULL) {
            am_eth_tcp_echoserver_send(tpcb, es);
        } else {
            if (es->state == ES_CLOSING) {
                am_eth_tcp_echoserver_connection_close(tpcb, es);
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
 * @brief  This function implements the tcp_sent LwIP callback (called when ACK
 *         is received from remote host for sent data)
 * @param  None
 * @retval None
 */
static err_t am_eth_tcp_echoserver_sent(void *arg, struct tcp_pcb *tpcb,
        u16_t len)
{
    struct tcp_echoserver_struct *es;

    LWIP_UNUSED_ARG(len);

    es = (struct tcp_echoserver_struct *) arg;

    if (es->p != NULL) {
        am_eth_tcp_echoserver_send(tpcb, es);
    } else {
        if (es->state == ES_CLOSING)
            am_eth_tcp_echoserver_connection_close(tpcb, es);
    }
    return ERR_OK;
}

/**
 * @brief  This function is used to send data for tcp connection
 * @param  tpcb: pointer on the tcp_pcb connection
 * @param  es: pointer on echo_state structure
 * @retval None
 */
static void am_eth_tcp_echoserver_send(struct tcp_pcb *tpcb,
        struct tcp_echoserver_struct *es)
{
    struct pbuf *ptr;
    err_t wr_err = ERR_OK;

    while ((wr_err == ERR_OK) && (es->p != NULL)
            && (es->p->len <= tcp_sndbuf(tpcb))) {
        ptr = es->p;
        wr_err = tcp_write(tpcb, ptr->payload, ptr->len, 1);

        if (wr_err == ERR_OK) {
            u16_t plen;
            plen = ptr->len;
            es->p = ptr->next;
            if (es->p != NULL) {
                pbuf_ref(es->p);
            }
            pbuf_free(ptr);
            tcp_recved(tpcb, plen);
        } else if (wr_err == ERR_MEM) {
            es->p = ptr;
        } else {
            /* other problem ?? */
        }
    }
}

/**
 * @brief  This functions closes the tcp connection
 * @param  tcp_pcb: pointer on the tcp connection
 * @param  es: pointer on echo_state structure
 * @retval None
 */
static void am_eth_tcp_echoserver_connection_close(struct tcp_pcb *tpcb,
        struct tcp_echoserver_struct *es)
{
    tcp_arg(tpcb, NULL);
    tcp_sent(tpcb, NULL);
    tcp_recv(tpcb, NULL);
    tcp_err(tpcb, NULL);
    tcp_poll(tpcb, NULL, 0);
    if (es != NULL) {
        mem_free(es);
    }
    tcp_close(tpcb);
}

#endif
