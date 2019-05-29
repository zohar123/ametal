


#include "am_int.h"
#include "zmf159_pin.h"
#include "zmf159_inum.h"
#include "am_zmf159_usbd.h"
#include "zmf159_periph_map.h"
#include "am_usbd_ch9.h"
#include "string.h"

#define Recipient (p_dev->isa.setup_data.bm_request_type &  \
                  (AM_USB_REQ_TYPE_MASK | AM_USB_REQ_TYPE_RECIPIENT_MASK))



/* __interface_get */
static uint8_t __interface_get(uint8_t interface, uint8_t alt)
{
    if (alt > 0) {
        return AM_USB_STATUS_NOT_SUPPORTED;
    } else if (interface > 1) {
        return AM_USB_STATUS_NOT_SUPPORTED;
    }

    return AM_USB_STATUS_SUCCESS;
}


/* the device endpoint init */
static am_usb_status_t __ep_init(am_zmf159_device_t       *p_dev,
                                 am_usbd_endpoint_init_t  *epinit)
{
    int i       = 0;
    uint8_t  ep = (epinit->endpoint_address & AM_USBD_ENDPOINT_NUMBER_MASK);

    amhw_zmf159_usb_bdt_t  *pUSB_OTG_BDT = p_dev->p_usb_bdt + ep;
    uint32_t                dataBufAdd   = (uint32_t)pUSB_OTG_BDT+0x200+0x100*ep;

    for (i = 0; i < AM_USBD_MAX_EP_CNT; i++) {
        /* 端点描述符中必须指定了这个端点 */
        if (p_dev->isa.endpoint_info[i].inuse == AM_TRUE &&
            p_dev->isa.endpoint_info[i].ep_address == epinit->endpoint_address) {
            break;
        }
    }

    if (i == AM_USBD_MAX_EP_CNT)
        return AM_USB_STATUS_INVALID_REQUEST;

    if (ep >= AM_USBD_MAX_EP_CNT)
        return AM_USB_STATUS_INVALID_PARAMETER;
    if (epinit->transfer_type > AM_USB_ENDPOINT_INTERRUPT)
        return AM_USB_STATUS_INVALID_PARAMETER;

    /* 设置端点能传输的最大字节个数 */
    if (epinit->max_packet_size > AM_USBD_MAX_EP_DATA_CNT)
        epinit->max_packet_size = AM_USBD_MAX_EP_DATA_CNT;

    pUSB_OTG_BDT->rx_buf[0].format = 0;
    pUSB_OTG_BDT->rx_buf[0].adress = dataBufAdd;
    pUSB_OTG_BDT->rx_buf[0].format |= 0x40<<16;//长度64byte
    pUSB_OTG_BDT->rx_buf[0].format |= 1<<7;

    pUSB_OTG_BDT->rx_buf[1].format = 0;
    pUSB_OTG_BDT->rx_buf[1].format |= 0x40<<16;//长度64byte
    pUSB_OTG_BDT->rx_buf[1].adress = pUSB_OTG_BDT->rx_buf[0].adress+0x40;
    pUSB_OTG_BDT->rx_buf[1].format |= 1<<6;
    pUSB_OTG_BDT->rx_buf[1].format |= 1<<7;

    pUSB_OTG_BDT->tx_buf[0].format = 0x00;
    pUSB_OTG_BDT->tx_buf[0].adress = pUSB_OTG_BDT->rx_buf[0].adress+0x80;

    pUSB_OTG_BDT->tx_buf[1].format = 0x00;
    pUSB_OTG_BDT->tx_buf[1].adress = pUSB_OTG_BDT->rx_buf[0].adress+0xC0;

    for(i = 0; i < 64; i++) {
        *(uint8_t*)(pUSB_OTG_BDT->tx_buf[0].adress + i) = 0;
        *(uint8_t*)(pUSB_OTG_BDT->tx_buf[1].adress + i) = 0;
        *(uint8_t*)(pUSB_OTG_BDT->rx_buf[0].adress + i) = 0;
        *(uint8_t*)(pUSB_OTG_BDT->rx_buf[1].adress + i) = 0;
    }

    if (ep == 0) {
        amhw_zmf159_ep_ctrl_set(ZMF159_USB, 0, 0x0d);
    } else if (epinit->endpoint_address >> 7) {
        amhw_zmf159_ep_ctrl_set(ZMF159_USB, ep, 0x05);
    } else {
        amhw_zmf159_ep_ctrl_set(ZMF159_USB, ep, 0x09);
    }

    p_dev->isa.endpoint_info[ep].stalled         = 0;
    p_dev->isa.endpoint_info[ep].max_packet_size = epinit->max_packet_size;
    p_dev->isa.endpoint_info[ep].transfer_type   = epinit->transfer_type;

    return AM_USB_STATUS_SUCCESS;
}

/* the device init */
static am_usb_status_t __zmf159_init(void *p_arg)
{
    uint8_t i = 0;
    am_usbd_endpoint_init_t endpoint;
    am_zmf159_device_t    *p_dev        = (am_zmf159_device_t *)p_arg;
    p_dev->isa.cur_config        = 0;

    if (p_dev == NULL) {
        return AM_USB_STATUS_INVALID_HANDLE;
    }

    amhw_zmf159_bdt_page_set(ZMF159_USB, 0x20007000);

    amhw_zmf159_addr_set(ZMF159_USB, 0);
    amhw_zmf159_ctrl_set(ZMF159_USB, ZMF159_USB_ODD_RST);
    amhw_zmf159_ctrl_reset(ZMF159_USB, ZMF159_USB_ODD_RST);

    p_dev->p_usb_bdt = (amhw_zmf159_usb_bdt_t *)amhw_zmf159_bdt_page_get(ZMF159_USB);

    /* 清除SETUP数据 */
    p_dev->isa.setup_data.bm_request_type = 0;
    p_dev->isa.setup_data.b_request       = 0;
    p_dev->isa.setup_data.w_value         = 0;
    p_dev->isa.setup_data.w_index         = 0;
    p_dev->isa.setup_data.w_length        = 0;

    /**< \brief 初始化端点 */
    for (i = 0; i < AM_USBD_MAX_EP_CNT; i++) {
        if (p_dev->isa.endpoint_info[i].inuse == 1) {
            endpoint.endpoint_address = p_dev->isa.endpoint_info[i].ep_address;
            endpoint.max_packet_size  = p_dev->isa.endpoint_info[i].max_packet_size;
            endpoint.transfer_type    = p_dev->isa.endpoint_info[i].transfer_type;
            __ep_init(p_dev, &endpoint);
        }
    }

    p_dev->tx_odd[0] = 0;

    amhw_zmf159_ctrl_set(ZMF159_USB, ZMF159_USB_EN);

    amhw_zmf159_usb_int_enable(ZMF159_USB,
                               ZMF159_USB_INT_STAT_RST | ZMF159_USB_INT_STAT_TOK_DNE);

    return AM_USB_STATUS_SUCCESS;
}

/* usbd deinit */
static am_usb_status_t __zmf159_deinit(void *p_arg)
{
    am_zmf159_device_t *p_dev  = (am_zmf159_device_t *)p_arg;

    if (p_dev == NULL) {
        return AM_USB_STATUS_INVALID_HANDLE;
    }

    amhw_zmf159_ctrl_reset(ZMF159_USB, ZMF159_USB_EN);
    amhw_zmf159_usb_int_disable(ZMF159_USB,
                                ZMF159_USB_INT_STAT_RST | ZMF159_USB_INT_STAT_TOK_DNE);
    amhw_zmf159_addr_set(ZMF159_USB, 0);

    if (p_dev->p_info->pfn_plfm_deinit)
        p_dev->p_info->pfn_plfm_deinit();

    return AM_USB_STATUS_SUCCESS;
}

/* reset */
static void __zmf159_reset(void *p_arg)
{
    uint8_t i = 0;
    am_usbd_endpoint_init_t endpoint;
    am_zmf159_device_t     *p_dev = (am_zmf159_device_t *)p_arg;

    p_dev->isa.cur_config = 0;

    /* Current Feature initialization */
    p_dev->isa.cur_feature = p_dev->p_info->p_devinfo->p_descriptor[1].p_desc[7];

    p_dev->isa.cur_interface = 0;

    p_dev->p_usb_bdt = (amhw_zmf159_usb_bdt_t *)amhw_zmf159_bdt_page_get(ZMF159_USB);

    /* 清除SETUP数据 */
    p_dev->isa.setup_data.bm_request_type = 0;
    p_dev->isa.setup_data.b_request       = 0;
    p_dev->isa.setup_data.w_value         = 0;
    p_dev->isa.setup_data.w_index         = 0;
    p_dev->isa.setup_data.w_length        = 0;

    /**< \brief 初始化端点 */
    for (i = 0; i < AM_USBD_MAX_EP_CNT; i++) {
        if (p_dev->isa.endpoint_info[i].inuse == 1) {
            endpoint.endpoint_address = p_dev->isa.endpoint_info[i].ep_address;
            endpoint.max_packet_size  = p_dev->isa.endpoint_info[i].max_packet_size;
            endpoint.transfer_type    = p_dev->isa.endpoint_info[i].transfer_type;
            __ep_init(p_dev, &endpoint);
        }
    }

    amhw_zmf159_ep_ctrl_set(ZMF159_USB, 0, 0x0d);
    amhw_zmf159_ctrl_set(ZMF159_USB, ZMF159_USB_ODD_RST);
    amhw_zmf159_ctrl_reset(ZMF159_USB, ZMF159_USB_ODD_RST);

    p_dev->tx_odd[0] = 0;
    amhw_zmf159_addr_set(ZMF159_USB, 0);
}

/* usbd send data function */
static am_usb_status_t __zmf159_usbd_send(void *p_arg, uint8_t ep, uint8_t *p_buf, uint32_t length)
{
    uint32_t i;
    uint8_t *pBuf;
    am_zmf159_device_t    *p_dev = (am_zmf159_device_t *)p_arg;
    amhw_zmf159_usb_bdt_t *pUSB_OTG_BDT = p_dev->p_usb_bdt;

    if (p_dev == NULL) {
        return AM_USB_STATUS_INVALID_PARAMETER;
    }

    if (ep >= AM_USBD_MAX_EP_CNT) {
        return AM_USB_STATUS_INVALID_PARAMETER;
    }

    pBuf = (uint8_t*)((pUSB_OTG_BDT + ep)->tx_buf[p_dev->tx_odd[ep]].adress) ;

    (pUSB_OTG_BDT + ep)->tx_buf[p_dev->tx_odd[ep]].format &= 0x00ff00;

    for (i = 0;i < length; i++) {
        pBuf[i] = *p_buf;
        p_buf++;
    }

    pBuf[i] = '\0';  // 截断数据，防止脏数据

    (pUSB_OTG_BDT+ep)->tx_buf[p_dev->tx_odd[ep]].format |= (length<<16)|(p_dev->ep_indata_num[ep]<<6);
    p_dev->ep_indata_num[ep] = !p_dev->ep_indata_num[ep];

    (pUSB_OTG_BDT+ep)->tx_buf[p_dev->tx_odd[ep]].format |= 1<<7;
    p_dev->tx_odd[ep] = !p_dev->tx_odd[ep];

    return AM_USB_STATUS_SUCCESS;
}

/* recv function*/
static uint8_t __zmf159_usbd_recv(void *p_arg, uint8_t ep, uint8_t *p_buf, uint32_t length)
{
    uint8_t  i         = 0;
    uint8_t  len       = 0;
    uint8_t *p_buffer  = NULL;
    uint8_t  val_len   = 0;

    am_zmf159_device_t    *p_dev = (am_zmf159_device_t *)p_arg;
    amhw_zmf159_usb_bdt_t *p_bdt = p_dev->p_usb_bdt;

    if (p_dev == NULL || ep >= AM_USBD_MAX_EP_CNT) {
        return AM_USB_STATUS_INVALID_PARAMETER;
    }

    p_buffer = (uint8_t*)((p_bdt + ep)->rx_buf[p_dev->rx_odd[ep]].adress) ;

    if(ep == 0){
        val_len = 0x40;
    }else{
        val_len = ((p_bdt + ep)->rx_buf[p_dev->rx_odd[ep]].format >> 16) & 0x3ff;
    }

    p_dev->isa.endpoint_info[ep].val_length = (val_len & 0x1FF);

    if (val_len == 0){
        return 0;
    }else if (val_len < length){
        len = val_len;
    }else if(val_len >= length){
        len = length;
    }

    for (i = 0;i < len; i++) {
        *p_buf = p_buffer[i];
         p_buf++;
    }

    (p_bdt + ep)->rx_buf[p_dev->rx_odd[ep]].format &= 0xff00;//清除计数,清除stall
    (p_bdt + ep)->rx_buf[p_dev->rx_odd[ep]].format |= (64 << 16);//设置最大接收字节
    (p_bdt + ep)->rx_buf[p_dev->rx_odd[ep]].format |= 1 << 7;

    return len;
}

/**
 * \brief 终止某个端点正在进行的传输
 */
static am_usb_status_t __zmf159_usbd_cancel (am_usbd_handle_t handle,
                                            uint8_t           endpoint_addr)
{
    return AM_USB_STATUS_ERROR;
}


/* usbd ctrl */
static am_usb_status_t __zmf159_usbd_control(am_usbd_handle_t         handle,
                                            am_usbd_control_type_t    type,
                                            void                     *param)
{
    uint8_t *p_temp8;
    uint8_t  endpoint = 0;
    am_zmf159_device_t  *p_dev = (am_zmf159_device_t *)handle;
    am_usb_status_t      error = AM_USB_STATUS_ERROR;
    am_usbd_ep_status_t *p_endpoint_status = NULL;
    if (!handle)
        return AM_USB_STATUS_INVALID_HANDLE;

    switch ((int)type) {
        case AM_USBD_CONTROL_RUN:
            error = AM_USB_STATUS_SUCCESS;
            break;

        case AM_USBD_CONTROL_STOP:
            error = AM_USB_STATUS_SUCCESS;
            break;

        case AM_USBD_CONTROL_ENDPOINT_INIT:     /* 对某个端点初始化 */
            if (param) {
                error = __ep_init(p_dev, (am_usbd_endpoint_init_t *)param);
            }
            break;

        case AM_USBD_CONTROL_ENDPOINT_DEINIT:   /* 对某个端点去初始化 */
            if (param) {
                p_temp8 = (uint8_t *)param;
                //todo
//                error = __usb_device_endpoint_deinit(p_dev, *temp8);
            }
            break;

        case AM_USBD_CONTROL_ENDPOINT_STALL:    /* 控制端点阻塞 */
            if (param) {
                p_temp8 = (uint8_t *)param;
                //todo
//                error = __usb_device_endpoint_stall(p_dev, *temp8);
            }
            break;

        case AM_USBD_CONTROL_ENDPOINT_UNSTALL:  /* 控制端点不阻塞 */
            if (param) {
                p_temp8 = (uint8_t *)param;
                //todo
//                error = __usb_device_endpoint_unstall(p_dev, *temp8);
            }
            break;

        case AM_USBD_CONTROL_GET_DEVICE_STATUS: /* 获取设备状态 */

            break;

        case AM_USBD_CONTROL_GET_ENDPOINT_STATUS:   /* 通过端点地址获取端点状态 */
            if (param) {
                p_endpoint_status = (am_usbd_ep_status_t *)param;

                endpoint = (p_endpoint_status->endpoint_address) & AM_USBD_ENDPOINT_NUMBER_MASK;

                if (endpoint < AM_USBD_MAX_EP_CNT) {
                    p_endpoint_status->endpoint_status =
                            amhw_zmf159_ep_stat_get(ZMF159_USB, endpoint);
                    error = AM_USB_STATUS_SUCCESS;
                } else {
                    error = AM_USB_STATUS_INVALID_PARAMETER;
                }
            }
            break;

        case AM_USBD_CONTROL_SET_ENDPOINT_STATUS :
            if (param) {
                p_endpoint_status = (am_usbd_ep_status_t *)param;

                endpoint = (p_endpoint_status->endpoint_address) & AM_USBD_ENDPOINT_NUMBER_MASK;

                if (endpoint < AM_USBD_MAX_EP_CNT) {

                    amhw_zmf159_ep_stat_set(ZMF159_USB,
                                            endpoint,
                                            p_endpoint_status->endpoint_status);

                    error = AM_USB_STATUS_SUCCESS;
                } else {
                    error = AM_USB_STATUS_INVALID_PARAMETER;
                }
            }
            break;

        case AM_USBD_CONTROL_SET_DEVICE_ADDRESS:    /* 设置USB设备地址 */
            if (param) {
                p_temp8 = (uint8_t *)param;
                amhw_zmf159_addr_set(ZMF159_USB, *p_temp8);
                error = AM_USB_STATUS_SUCCESS;
            }
            break;

        case AM_USBD_CONTROL_GET_SYNCH_FRAME:
            break;

        case AM_USBD_CONTROL_RESUME:
//            amhw_zlg217_usbd_wakeup();
            break;

            /* 设置默认状态 */
        case AM_USBD_CONTROL_SET_DEFAULT_STATUS:
//            __usb_device_setdefault_state(p_dev);
            error = AM_USB_STATUS_SUCCESS;
            break;

        case AM_USBD_CONTROL_GET_SPEED:
            if (param) {
                p_temp8 = (uint8_t *)param;
                *p_temp8 = AM_USB_SPEED_FULL;
                error = AM_USB_STATUS_SUCCESS;
            }
            break;

        case AM_USBD_CONTROL_GET_OTG_STATUS:
            break;
        case AM_USBD_CONTROL_SET_OTG_STATUS:
            break;
        case AM_USBD_CONTROL_SET_TEST_MODE:
            break;
        default:
            break;
    }

    return error;
}

/* the controlor interface function */
static const am_usbd_interface_t __g_usb_device_interface = {
    __zmf159_init,
    __zmf159_deinit,
    __zmf159_usbd_send,
    __zmf159_usbd_recv,
    __zmf159_usbd_cancel,
    __zmf159_usbd_control,
};


/* data output state */
static void __data_stage_out(am_zmf159_device_t *p_dev)
{
    uint16_t   len       = 0;
    am_data_info_t *p_ctrl_info = &p_dev->isa.ctrl_info;

    if (p_ctrl_info->p_buf && p_ctrl_info->length) {

        len = p_ctrl_info->packet_size;

        if (len > p_ctrl_info->length) {
            len = p_ctrl_info->length;
        }

        len = __zmf159_usbd_recv(p_dev, 0, p_ctrl_info->p_buf + p_ctrl_info->offset, len);

        p_ctrl_info->length -= len;
        p_ctrl_info->offset += len;
    }

    /* Set the next State*/
    if (p_ctrl_info->length >= p_ctrl_info->packet_size) {
          p_dev->state = OUT_DATA;
    } else {
        if (p_ctrl_info->length > 0) {

            p_dev->state = LAST_OUT_DATA;
        } else if (p_ctrl_info->length == 0) {
            /* 接收完成之后发送空包进行响应 */
            p_dev->state = WAIT_STATUS_IN;
            p_dev->ep_indata_num[0] = 1;
            __zmf159_usbd_send(p_dev, 0, NULL, 0);
        }
    }
}

/* data entry state */
static void __data_stage_in(am_zmf159_device_t *p_dev)
{
    am_data_info_t *p_ctrl_info = &p_dev->isa.ctrl_info;

    uint16_t send_len  = p_ctrl_info->length;
    uint32_t status    = p_dev->state;

    /* 如果待发送的字节长度为 0 则发送空包 */
    if ((send_len == 0) && (status == LAST_IN_DATA)) {
        if(p_dev->data_mul == AM_TRUE) {
            /* No more data to send and empty packet */
            __zmf159_usbd_send(p_dev, 0, NULL, 0);
            status = LAST_IN_DATA;
            p_dev->data_mul = AM_FALSE;
        } else {
            /* No more data to send so STALL the TX Status*/
            status = WAIT_STATUS_OUT;
        }
        p_dev->state = status;
        return ;
    }

        /* 判断发送数据是否大于包最大字节数  */
    if(send_len <= p_ctrl_info->packet_size){
        status = LAST_IN_DATA;
    }else{
        send_len = p_ctrl_info->packet_size;
        status = IN_DATA;
    }

    __zmf159_usbd_send(p_dev, 0, p_ctrl_info->p_buf + p_ctrl_info->offset, send_len);

    p_ctrl_info->length -= send_len;
    p_ctrl_info->offset += send_len;

    p_dev->state = status;
}

/* the device nodata setup */
static void __nodata_setup0(am_zmf159_device_t *p_dev)
{
    am_usbd_dev_t  *p_usb_dev = &p_dev->isa;
    uint8_t  ret  = AM_USB_STATUS_NOT_SUPPORTED;
    uint8_t  code = p_dev->isa.setup_data.b_request;
    uint8_t  type = p_dev->isa.setup_data.bm_request_type;
    uint32_t status = SETTING_UP;

    switch (type & (AM_USB_REQ_TYPE_MASK | AM_USB_REQ_TYPE_RECIPIENT_MASK)){

    /* 标准设备请求 */
    case (AM_USB_REQ_TYPE_STANDARD | AM_USB_REQ_TYPE_RECIPIENT_DEVICE):
        switch (code) {
        case AM_USB_REQ_STANDARD_SET_CONFIGURATION :
            ret = p_usb_dev->p_funcs->pfn_config_set(p_usb_dev);
            break;

        case AM_USB_REQ_STANDARD_SET_ADDRESS:
            if ((p_dev->isa.setup_data.w_value > 127)
               || (p_dev->isa.setup_data.w_index != 0)
               || (p_dev->isa.cur_config != 0)) {
                status = STALLED;
                p_dev->state = status;
                return;
            } else {
                ret = AM_USB_STATUS_SUCCESS;
            }
            break;

        case AM_USB_REQ_STANDARD_SET_FEATURE :
            if (((p_dev->isa.setup_data.w_value & (0xff)) == AM_USB_REQ_STA_FEATURE_DEVICE_REMOTE_WAKEUP)
                 && (p_dev->isa.setup_data.w_index == 0) && (AM_BIT_ISSET(p_dev->isa.cur_feature, 5))) {
                ret = p_usb_dev->p_funcs->pfn_feature_set(p_usb_dev);
            } else {
                ret = AM_USB_STATUS_NOT_SUPPORTED;
            }

            break;

        case AM_USB_REQ_STANDARD_CLEAR_FEATURE :
            if ((p_dev->isa.setup_data.w_value & 0xff) ==
                 AM_USB_REQ_STA_FEATURE_DEVICE_REMOTE_WAKEUP &&
                 p_dev->isa.setup_data.w_index  == 0 &&
                 AM_BIT_ISSET(p_dev->isa.cur_feature, 5)) {

                ret = p_usb_dev->p_funcs->pfn_feature_set(p_usb_dev);
            } else {
                ret = AM_USB_STATUS_NOT_SUPPORTED;
            }
            break;
        default :
            break;
        }
        break;

    /* 标准接口请求 */
    case (AM_USB_REQ_TYPE_STANDARD | AM_USB_REQ_TYPE_RECIPIENT_INTERFACE):
        if (code == AM_USB_REQ_STANDARD_SET_INTERFACE) {
            ret = p_usb_dev->p_funcs->pfn_interface_set(p_usb_dev);
        }
        break;

    /* 标准端点类请求 */
    case (AM_USB_REQ_TYPE_STANDARD | AM_USB_REQ_TYPE_RECIPIENT_ENDPOINT):
        /*CLEAR FEATURE for EndPoint*/
        if (code == AM_USB_REQ_STANDARD_CLEAR_FEATURE) {
            ret = p_usb_dev->p_funcs->pfn_feature_clr(p_usb_dev);
        }else if (code == AM_USB_REQ_STANDARD_SET_FEATURE) {
            ret = p_usb_dev->p_funcs->pfn_interface_set(p_usb_dev);
        } else {
            ret = AM_USB_STATUS_NOT_SUPPORTED;
        }
        break;
    default :
        break;
    }

    // 类请求
    if (ret != AM_USB_STATUS_SUCCESS) {
        ret = p_dev->isa.class_req.pfn_class(p_dev->isa.class_req.p_arg, code);
    }

    if (ret != AM_USB_STATUS_SUCCESS) {
        p_dev->state = status;
        return;
   }

    status = WAIT_STATUS_IN;/* After no data stage SETUP */

    // 发送空包
    p_dev->ep_indata_num[0] = 1;
    __zmf159_usbd_send(p_dev, 0, NULL, 0);

    p_dev->state = status;
    return;
}

/* the device has data stages */
void __data_setup0(am_zmf159_device_t *p_dev)
{
    uint8_t  ret = AM_USB_STATUS_NOT_SUPPORTED;
    uint32_t req_num = p_dev->isa.setup_data.b_request;
    uint32_t ep, Reserved;
    uint32_t status;
    am_usbd_dev_t  *p_usb_dev = &p_dev->isa;

    am_data_info_t *p_data_info = &(p_usb_dev->ctrl_info);

    p_data_info->offset   = 0;
    p_data_info->length   = 0xFFFF;

    switch (req_num) {
    case AM_USB_REQ_STANDARD_GET_DESCRIPTOR:
        ret = p_usb_dev->p_funcs->pfn_desc_get(
                p_usb_dev, &p_data_info->p_buf, &p_data_info->length);
        break;

    case AM_USB_REQ_STANDARD_GET_STATUS:
    {
        switch (Recipient) {
        case (AM_USB_REQ_TYPE_STANDARD | AM_USB_REQ_TYPE_RECIPIENT_DEVICE):
            ret = p_usb_dev->p_funcs->pfn_status_get(
                    p_usb_dev, &p_data_info->p_buf, &p_data_info->length);
            break;

        case (AM_USB_REQ_TYPE_STANDARD | AM_USB_REQ_TYPE_RECIPIENT_INTERFACE):
            if ((__interface_get(p_usb_dev->setup_data.w_index, 0) == AM_USB_STATUS_SUCCESS)
                && (p_usb_dev->cur_config != 0)) {
                ret = p_usb_dev->p_funcs->pfn_status_get(
                        p_usb_dev, &p_data_info->p_buf, &p_data_info->length);
            }
            break;

        case (AM_USB_REQ_TYPE_STANDARD | AM_USB_REQ_TYPE_RECIPIENT_ENDPOINT):
            ep = (p_usb_dev->setup_data.w_index & 0x0f);
            Reserved = p_usb_dev->setup_data.w_index & 0x70;
            status = amhw_zmf159_ep_stat_get(ZMF159_USB, ep);
            if ((ep < p_dev->ep_num) && (Reserved == 0) && (status != 0)) {
                ret = p_usb_dev->p_funcs->pfn_status_get(
                        p_usb_dev, &p_data_info->p_buf, &p_data_info->length);
            }
            break;
        default :
            break;
        }
        break;
    }

    case AM_USB_REQ_STANDARD_GET_CONFIGURATION:
        if (Recipient == (AM_USB_REQ_TYPE_STANDARD | AM_USB_REQ_TYPE_RECIPIENT_DEVICE)) {
            ret = p_usb_dev->p_funcs->pfn_config_get(
                    p_usb_dev, &p_data_info->p_buf, &p_data_info->length);
        }
        break;

    case AM_USB_REQ_STANDARD_GET_INTERFACE:
        if ((Recipient == (AM_USB_REQ_TYPE_STANDARD | AM_USB_REQ_TYPE_RECIPIENT_INTERFACE))
            && (__interface_get(p_usb_dev->setup_data.w_index, 0) == AM_USB_STATUS_SUCCESS)) {
            ret = p_usb_dev->p_funcs->pfn_interface_get(
                    p_usb_dev, &p_data_info->p_buf, &p_data_info->length);
        }
        break;

    default :
        break;
    }

    if(ret == AM_USB_STATUS_NOT_SUPPORTED){
        // 类请求
        ret = p_usb_dev->class_req.pfn_class(p_dev->isa.class_req.p_arg,
                                             p_dev->isa.setup_data.b_request);
        if (ret == AM_USB_STATUS_RETRY) {
            p_dev->state = PAUSE;
            return;
        }
    }

    if (p_data_info->length == 0xFFFF) {
        /* Data is not ready, wait it */
        p_dev->state = PAUSE;
        return;
    }

    if ((ret == AM_USB_STATUS_NOT_SUPPORTED) || (p_data_info->length == 0)) {
        /* Unsupported request */
        p_dev->state = STALLED;
        return;
    }

    if (AM_BIT_ISSET(p_usb_dev->setup_data.bm_request_type, 7)) {

        volatile uint32_t req_len = p_usb_dev->setup_data.w_length;

        /* 判断需要发送数据的总长度
         * 如果请求数据长度小于待发送信息数据长度，则待发送数据长度为请求数据长度
         * 如果请求数据长度大于待发送信息数据长度，则待发送数据长度为待发送信息总长度
         */
        if (p_data_info->length > req_len) {

            p_data_info->length = req_len;

        } else if (p_data_info->length < p_usb_dev->setup_data.w_length) {

            if (p_data_info->length < p_dev->max_packsizee) {
                p_dev->data_mul = AM_FALSE;
            } else if ((p_data_info->length % p_dev->max_packsizee) == 0) {
                p_dev->data_mul = AM_TRUE;
            }

        }
        p_data_info->packet_size = p_dev->max_packsizee;
        __data_stage_in(p_dev);
    } else {
        /* 待获取的数据的长度  */
        p_data_info->length = p_dev->isa.setup_data.w_length;
        p_dev->state = OUT_DATA;
    }
    return;
}

/* setup token packets interrupt processing function*/
static void __setup0_process(am_zmf159_device_t *p_dev)
{
    if (p_dev->state != PAUSE) {
        __zmf159_usbd_recv(p_dev, 0, (uint8_t *)(&p_dev->isa.setup_data), 8);
        //AM_DBG_INFO("\r\n\r\nsetup_data->type = %02x", p_dev->isa.setup_data.bm_request_type);
        //AM_DBG_INFO("\r\nsetup_data->code = %02x", p_dev->isa.setup_data.b_request);
        //AM_DBG_INFO("\r\nsetup_data->data = %04x", p_dev->isa.setup_data.w_value);
        //AM_DBG_INFO("\r\nsetup_data->index = %04x", p_dev->isa.setup_data.w_index);
        //AM_DBG_INFO("\r\nsetup_data->length = %04x\r\n\r\n\r\n", p_dev->isa.setup_data.w_length);

        if(p_dev->isa.setup_data.bm_request_type == 0xa1){
            AM_DBG_INFO("1111");
        }
    }
    p_dev->state = SETTING_UP;
    if (p_dev->isa.setup_data.w_length == 0) {
        __nodata_setup0(p_dev);
    } else {
        __data_setup0(p_dev);
    }
}

/* input token packets interrupt processing function*/
static void __in0_process(am_zmf159_device_t *p_dev)
{
    uint32_t ControlState = p_dev->state;

    if ((ControlState == IN_DATA) || (ControlState == LAST_IN_DATA)) {
        __data_stage_in(p_dev);
        ControlState = p_dev->state;
    } else if (ControlState == WAIT_STATUS_IN) {

        if ((p_dev->isa.setup_data.b_request == AM_USB_REQ_STANDARD_SET_ADDRESS) &&
            (Recipient == (AM_USB_REQ_TYPE_STANDARD | AM_USB_REQ_TYPE_RECIPIENT_DEVICE))) {
            /* 设置设备地址  接收到设置地址指令后  设备已经发送相应空包成功 */
            p_dev->isa.p_funcs->pfn_address_set(&(p_dev->isa), p_dev->isa.setup_data.w_value);
        }
    } else {
        ControlState = STALLED;
    }

    p_dev->state = ControlState;
}

/* output token packets interrupt processing function */
static void __out0_process(am_zmf159_device_t *p_dev)
{
    uint32_t ControlState = p_dev->state;

    if ((ControlState == OUT_DATA) || (ControlState == LAST_OUT_DATA)) {

        __data_stage_out(p_dev);

        ControlState = p_dev->state; /* may be changed outside the function */

    } else if (ControlState == WAIT_STATUS_OUT) {

        __zmf159_usbd_recv(p_dev, 0, NULL, 0);
        ControlState = STALLED;

    }else {
        ControlState = STALLED;
    }

    p_dev->state = ControlState;
}


/* token interrupt function */
static void __token_isr(am_zmf159_device_t *p_dev)
{
    uint32_t format, tok_pid = 0;
    uint32_t status;
    uint8_t  tx_oddtemp;
    uint8_t  ep, trans_mode;

    /* 获取USB状态寄存器   */
    status = amhw_zmf159_usb_stat_get(ZMF159_USB);
    amhw_zmf159_usb_int_clear(ZMF159_USB, ZMF159_USB_INT_STAT_TOK_DNE);

    /*  获取中断端点号 */
    ep = (status >> 4) & 0xf;
    trans_mode = (status >> 3) & 0x01;

    if(ep == 0) {
        if(trans_mode == 0) {
            p_dev->rx_odd[0] = ( status >> 2 ) & 0x1;
            format = p_dev->p_usb_bdt->rx_buf[ p_dev->rx_odd[0] ].format;
        } else {
            tx_oddtemp = ( status >> 2) & 0x1;
            format = p_dev->p_usb_bdt->tx_buf[tx_oddtemp].format;
            p_dev->p_usb_bdt->tx_buf[tx_oddtemp].format = 0;
        }

        /* 获取端点0输入令牌:  0x01-OUT令牌,  0x09-IN令牌,  0x0d SETUP令牌   */
        tok_pid = (format >> 2) & 0xf;

        if(tok_pid == 0x0D) {
            amhw_zmf159_ctrl_reset(ZMF159_USB, ZMF159_USB_TXD_SUSPEND_TOKBSY);
            p_dev->ep_indata_num[0] = 1;
            __setup0_process(p_dev);
            return;
        }else if(tok_pid == 0x01) {
            __out0_process(p_dev);
            return;
        }else if(tok_pid == 0x9) {
            __in0_process(p_dev);
            return;
        }

    } else if(ep != 0) {

        if(trans_mode == 0) {
            p_dev->rx_odd[ep] = (status >> 2) & 0x1;
            format = (p_dev->p_usb_bdt + ep)->rx_buf[p_dev->rx_odd[ep]].format;
        } else {
            tx_oddtemp = (status >> 2) & 0x1;
            format = (p_dev->p_usb_bdt + ep)->rx_buf[tx_oddtemp].format;
           (p_dev->p_usb_bdt + ep)->rx_buf[tx_oddtemp].format = 0;
        }

        /* 端点回调函数  */
        if (p_dev->isa.endpoint_info[ep].pfn_callback != NULL) {
            (p_dev->isa.endpoint_info[ep].pfn_callback)(p_dev->isa.endpoint_info[ep].p_arg);
        }
    }
}

/* 中断回调函数*/
static void __zmf159_usbd_istr(void *p_arg)
{
    volatile uint16_t status = 0;
    am_zmf159_device_t  *p_dev = (am_zmf159_device_t *)p_arg;

    status = amhw_zmf159_usb_int_get(ZMF159_USB);

    if(status & ZMF159_USB_INT_STAT_RST) {
        amhw_zmf159_usb_int_clear(ZMF159_USB, ZMF159_USB_INT_STAT_RST);
        __zmf159_reset(p_dev);
    }

    if(status & ZMF159_USB_INT_STAT_TOK_DNE) {
        __token_isr(p_dev);
    }
} /* USB_Istr */


/**
 * \brief 通过wValue的值寻找指定的描述符
 *
 * \retval 成功返回描述符指针，失败返回NULL
 *
 * \note 配置描述符的下级描述符不能通过此法获得
 */
static uint8_t * __find_desc_by_wValue1 (am_zmf159_device_t    *p_dev,
                                         uint16_t               w_value)
{
    int i = 0;

    for (i = 0; i < p_dev->p_info->p_devinfo->descriptor_num; i++) {
        if (w_value == p_dev->p_info->p_devinfo->p_descriptor[i].desc_wValue) {
            return (uint8_t *)(p_dev->p_info->p_devinfo->p_descriptor[i].p_desc);
        }
    }
    return NULL;
}


/**
 * \brief （通过配置描述符）初始化端点信息（端点个数，输入输出属性，支持的最大包大小）
 *
 * \retval 成功返回该使用的端点个数，失败返回-1
 */
static am_err_t __init_ep_info (am_zmf159_device_t *p_dev)
{
    uint8_t  ret     = 0;       /* 定义返回值 */
    uint8_t *p_tmp   = NULL;    /* 数据缓存指针 */
    uint8_t  ep_num  = 0;       /* 端点号 */
    uint8_t  tmp     = 0;       /* 临时变量*/
    uint8_t  offset  = 0;       /* 偏移量*/
    uint8_t  len     = 0;       /* 描述符总长度 */
    am_usb_descriptor_endpoint_t  *p_desc_ep   = NULL;

    /* 获取配置描述符 */
    p_tmp = __find_desc_by_wValue1(p_dev, (AM_USB_DESC_TYPE_CONFIGURE << 8) | 0);
    if (p_tmp == NULL)
        return AM_ERROR;

    /*配置描述符及其下属描述符的总长度*/
    len = (uint8_t)( ( *(p_tmp + 2) ) | ( ( *(p_tmp + 3)) >> 8 ) );
    /* 找端点描述符*/
    while (tmp < len) {
        offset  = *(p_tmp);   /* 该描述符的字节数,第一次为配置描述符*/
        p_tmp  +=   offset;   /* 做偏移 */
        tmp    +=   offset;

        /* 如果是端点描述符*/
        if ((*(p_tmp + 1) == AM_USB_DESC_TYPE_ENDPOINT) && (*p_tmp == AM_USB_DESC_LENGTH_ENDPOINT)) {
            p_desc_ep = (am_usb_descriptor_endpoint_t *)p_tmp;
            // 获取端点号
            ep_num = (p_desc_ep->b_endpoint_address & AM_USB_DESC_ENDPOINT_ADDR_NUMBER_MASK);
            p_dev->isa.endpoint_info[ep_num].inuse           = 1;  // 表示端点被使用
            p_dev->isa.endpoint_info[ep_num].ep_address      = p_desc_ep->b_endpoint_address;
            p_dev->isa.endpoint_info[ep_num].max_packet_size = (p_desc_ep->wmax_packet_size[1] << 8) |
                                                                   p_desc_ep->wmax_packet_size[0];
            p_dev->isa.endpoint_info[ep_num].transfer_type   = p_desc_ep->bm_attributes;
            p_dev->isa.endpoint_info[ep_num].stalled = 0;
            ret++;
        }
    }
    return ret;
}

/* usbd init entry */
am_usbd_dev_t *am_zmf159_usbd_init(am_zmf159_device_t              *p_dev,
                                   const am_zmf159_usbd_devinfo_t  *p_info)
{
    uint8_t i = 0;
    p_dev->isa.ctrl_handle = p_dev;
    p_dev->p_info          = p_info;
    p_dev->isa.p_info      = p_info->p_devinfo;
    p_dev->state           = WAIT_SETUP;
    p_dev->isa.p_interface = &__g_usb_device_interface;
    p_dev->data_mul        = AM_FALSE;

    /* Initialize devices one by one */
    p_dev->ep_num        = EP_NUM;
    p_dev->config_num    = 1;
    p_dev->rx_buf        = NULL;
    p_dev->max_packsizee = 64;

    /* 设置端点0的默认配置 */
    p_dev->isa.endpoint_info[0].stalled          = 0;
    p_dev->isa.endpoint_info[0].ep_address       = 0;
    p_dev->isa.endpoint_info[0].max_packet_size  = AM_USBD_MAX_EP_DATA_CNT;
    p_dev->isa.endpoint_info[0].transfer_type    = AM_USB_ENDPOINT_CONTROL;
    p_dev->isa.endpoint_info[0].inuse            = 1;
    p_dev->isa.endpoint_info[0].val_length       = 0;

    /* 设置除端点0外其他端点为默认配置 */
    for (i = 1 ;i < AM_USBD_MAX_EP_CNT; i++) {
        p_dev->isa.endpoint_info[i].stalled          = 0;
        p_dev->isa.endpoint_info[i].ep_address       = 0;
        p_dev->isa.endpoint_info[i].max_packet_size  = 0;
        p_dev->isa.endpoint_info[i].transfer_type    = 0;
        p_dev->isa.endpoint_info[i].inuse            = 0;
        p_dev->isa.endpoint_info[i].val_length       = 0;
    }

    /* 根据描述符对其他端点进行相应配置 */
    if (__init_ep_info(p_dev) == -1) {
        am_kprintf("fail to init endpoint\n");
    }

    if (p_dev->p_info->pfn_plfm_init) {
        p_dev->p_info->pfn_plfm_init();
    }

    /* 标准请求函数初始化 */
    am_usbd_ch9_std_request_init(&p_dev->isa);

    p_dev->isa.p_interface->pfn_device_init(p_dev);

    am_int_connect(p_dev->p_info->inum, __zmf159_usbd_istr, p_dev);
    am_int_enable(p_dev->p_info->inum);

    return &(p_dev->isa);
}

void am_zmf159_usbd_deinit( const am_zmf159_usbd_devinfo_t  *p_info)
{
    if (p_info->pfn_plfm_deinit != NULL) {
        p_info->pfn_plfm_deinit();
    }
}

