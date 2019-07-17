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
 * \brief RFID  串口命令主机程序
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-13  win, first implementation.
 * \endinternal
 */ 

#include <am_uartcmd.h>
#include "string.h"
#include "am_kl26.h"
#include "am_gpio.h"
#include "am_int.h"
#include "am_cap.h"
#include "am_pwm.h"
#include "am_wdt.h"
#include "am_wait.h"
#include "am_vdebug.h"
#include "am_uart.h"
#include "am_spi.h"
#include "am_timer.h"
#include "kl26_pin.h"
#include "am_kl26_clk.h"
#include "am_softimer.h"
#include "hw/amhw_fsl_tpm.h"
#include "hw/amhw_kl26_sim.h"
#include "am_fm175xx.h"
#include "am_kl26_inst_init.h"
#include "kl26_periph_map.h"
#include "am_fsl_ftfa.h"
#include "am_reader_card.h"
#include "am_board.h"
#include "am_prj_config.h"
#include "am_uartcmd_cmd_dev_ctrl.h"
#include "am_uartcmd_cmd_iso14443.h"
#include "am_uartcmd_cmd_mifare.h"
#include "am_uartcmd_dr_ic_cpu_plus.h"
#include "am_uartcmd_dr_ic_iso14443.h"
#include "am_uartcmd_dr_ic_mifare.h"
#include "am_uartcmd_frame_new.h"
#include "am_uartcmd_reader_card.h"
#include "am_uartcmd_serial_uart.h"
#include "am_uartcmd_cd4051.h"
/*******************************************************************************
  宏定义
*******************************************************************************/


#define __FLASH_ADDR_START    0           /**< \brief 定义flash的起始地址 */
#define __FLASH_SIZE          0x1FC00     /**< \brief 定义flash的大小，最后1k保留 */

/*******************************************************************************
  struct
*******************************************************************************/

/* 定义 fm175xx 实例信息 */
static const am_fm175xx_devinfo_t __g_fm175xx_devinfo = {
    PIOE_1,
    PIOE_16,
    PIOC_1,
    AM_FM175XX_PROT_TYPE_ISO14443A_106
};

/**
 * \brief 存放所需的全局变量
 */
typedef struct uartcmd_variable {
    am_timer_handle_t   timer_handle;        /**< \brief 定时器句柄，用于辅助判断命令是否超时 */

    am_uart_handle_t    uart_handle;         /**< \brief 模块使用的串口句柄 */

    uartcmd_serial_t    serial_uart;         /**< \brief 串行设备串口结构体 */

    /** \brief 用于串口超时的定时器信息 */
    uartcmd_serial_uart_timer_t serial_uart_timer;

    am_softimer_t       uart_softtimer;      /**< \brief 串口软件定时器结构体 */

    am_uartcmd_dev_t    uartcmd_dev;         /**< \brief UARTCMD 设备结构体 */
    am_uartcmd_handle_t uartcmd_handle;      /**< \brief UARTCMD 服务句柄 */
    
    am_wdt_handle_t            wdt_handle;      /**< \brief 看门狗服务句柄 */
    
    am_ic_iso14443_handle_t    iso14443_handle; /** \brief ISO14443服务句柄 */
    
    amdr_ic_iso14443_dev_t     iso14443_dev;    /** \brief ISO14443设备结构体 */
        
    am_ic_cpu_plus_handle_t    plus_handle;     /** \brief CPU PLUS服务句柄 */
    
    am_fm175xx_dev_t           fm175xx_dev;     /** \brief FM17550S设备结构体 */
    
    am_antenna_info_t          p_antenna_info;      /** \brief CD4051设备结构体 */

    /** \brief 读卡器芯片设备控制类通用操作服务句柄 */
    am_reader_card_handle_t    reader_card_handle;
    
    /** \brief 读卡器芯片设备控制类通用操作设备结构体 */
    amdr_reader_card_dev_t     reader_card_dev;
    
    /** \brief Mifare类命令全局变量 */
    uartcmd_cmd_mifare_var_t mifare_var;

    am_ic_mifare_handle_t    mifare_handle;    /** \brief Mifare标准服务句柄*/

    amdr_ic_mifare_dev_t     mifare_dev;       /** \brief Mifare标准服务设备结构体 */

    am_spi_device_t     spi_dev;          /**< \brief SPI设备结构体 */
    am_spi_handle_t     spi_handle;       /**< \brief SPI服务句柄   */

    am_wait_t           wait;             /**< \brief 等待标志 */

    uartcmd_frame_t     frame_new;        /**< \brief 新帧类型 */

    uartcmd_cmd_t       dev_ctrl;         /**< \brief 设备控制类命令 */
    uartcmd_cmd_t       mifare;           /**< \brief mifare 类控制命令 */
    uartcmd_cmd_t       iso14443;         /**< \brief ISO14443设备控制类命令 */

    uint8_t             work_mode;        /**< \brief 工作模式 */
    uint8_t             frame_type;       /**< \brief 帧类型 */
    uint8_t             comm_mode;        /**< \brief 通信模式 */

    uint32_t            cap_count;        /**< \brief 捕获值 */
    uint8_t             baud_num;         /**< \brief 串口波特率 */

    am_bool_t           timeout;          /**< \brief 用于自动检测模式，超时标志 */
    
    am_bool_t           door;             /**< \brief 后门功能标志 */

} uartcmd_variable_t;

/*******************************************************************************
  函数声明
*******************************************************************************/

/** \brief IAP拷贝函数 */
int uartcmd_iap_copy(uint32_t dst_addr,
                     uint32_t src_addr,
                     uint32_t nbytes);

/** \brief IAP拷贝函数 */
int uartcmd_iap_compare(uint32_t dst_addr,
                        uint32_t src_addr,
                        uint32_t nbytes);

/*******************************************************************************
  全局变量
*******************************************************************************/

uint8_t g_frame_buf[282];  /**< \brief 存放帧的缓冲区 */

/** \brief IAP操作缓冲区 4字节对齐*/
uint8_t g_iap_buf[1024] __attribute__((aligned(4)));    

/** \brief 外部定义的配置信息结构体类型的变量 */
extern const uartcmd_flash_devinfo_t flash_data1;

/** \brief 外部定义的配置信息结构体类型的变量 */
extern const uartcmd_flash_devinfo_t flash_data2;

/**
 * \brief UARTCMD 配置信息
 */
const struct am_uartcmd_devinfo g_uartcmd_devinfo = {
    PIOC_2,       /* INT 引脚 */
    PIOC_3,       /* adr1 引脚 */
    PIOC_4,       /* adr0 引脚 */
    PIOA_4,       /* LED 灯引脚 */
    282,          /* 帧的最大长度 */
    g_frame_buf,  /* 存放帧的缓冲区 */
    &flash_data2, /* 出厂配置信息 */
    &flash_data1, /* 用户配置信息 */
    g_iap_buf,    /* IAP 操作缓冲区 */
    1024,         /* IAP 缓冲区大小 */
    2,            /* 当发送帧的信息不大于该值时，退出自动检测模式 */
};

/**
 * \brief fm17xx天线配置信息
 */
static  am_antenna_info_t  __g_cd4051_dev = {
    {
        PIOB_19,
        PIOB_16,
        PIOB_17,
        PIOB_18
    },
    AM_FM175XX_ANTENNA_EIGHT
};

/**
 * \brief 串口波特率
 */
uint32_t uart_baudrate[8] = {
     9600,
     19200,
     28800,
     38400,
     57600,
     115200,
     172800,
     230400,
};

uartcmd_variable_t g_var; /**< \brief 声明全局变量结构体的变量 */

/**
 * \brief 设备控制类指令操作FLASH的函数
 */
const struct uartcmd_cmd_dev_ctrl_funcs g_iap_funcs = {
    uartcmd_iap_copy,
    uartcmd_iap_compare
};

/******************************************************************************/

/**
 * \brief 捕获回调函数
 *
 * \param[in] p_arg   : 回调函数入口参数
 * \param[in] cap_val : 捕获值
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
static void cap_callback (void *p_arg, uint32_t cap_val)
{
    uartcmd_variable_t *p_var = (uartcmd_variable_t *)p_arg;
    
    /* 结束等待序列 */
    am_wait_done(&p_var->wait);

    p_var->cap_count = cap_val;
}

/**
 * \brief IAP拷贝函数
 *
 * \param[in] dst_addr : FLASH地址
 * \param[in] src_addr : 内存或FLASH地址
 * \param[in] nbytes   : 拷贝的字节数，最少为64B
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
int uartcmd_iap_copy(uint32_t dst_addr,
                     uint32_t src_addr,
                     uint32_t nbytes)
{
    uint32_t key    = 0;
    int32_t  status = -AM_EPERM;

    /* 擦除指定地址的扇区 */
    key = am_int_cpu_lock();
    status = am_fsl_ftfa_sector_erase(KL26_FTFA, dst_addr);
    am_int_cpu_unlock (key);

    /* 擦除成功则写入 */
    if (AM_OK == status) {
        key = am_int_cpu_lock();
        status = am_fsl_ftfa_flash_program(KL26_FTFA,
                                           dst_addr,
                                           (uint32_t *)src_addr,
                                           nbytes / 4);
        am_int_cpu_unlock (key);
        
        if (status == 0x100)
            return AM_OK;
    }

    return status;
}

/**
 * \brief IAP比较函数，用于比较内存和FLASH中的数据是否一致
 *
 * \param[in] dst_addr : FLASH地址
 * \param[in] src_addr : 内存或FLASH地址
 * \param[in] nbytes   : 拷贝的字节数，最少为256B
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
int uartcmd_iap_compare(uint32_t dst_addr,
                        uint32_t src_addr,
                        uint32_t nbytes)
{
    uint32_t *p_src = NULL;
    uint32_t *p_dst = NULL;

    while (nbytes--) {
        if (*p_src++ != *p_dst++) {
            return -AM_EPERM;
        }
    }

    return AM_OK;
}

/**
 * \brief 获取串口波特率
 *
 * \param[in]  p_var     : 存放全局变量的指针
 * \param[out] p_devinfo : UARTCMD配置信息结构体指针
 *
 * \retval AM_OK      : 成功
 * \retavl -AM_EINVAL : 参数错误
 */
int uartcmd_uart_auto_buadrate_get (uartcmd_variable_t              *p_var,
                                    const struct am_uartcmd_devinfo *p_devinfo)
{
    am_cap_handle_t   tpm0_cap_handle;
    const int         cap_chan = 2;         /* 定义CAP捕获通道 */
    uint32_t          t1, t2, t3;

    uint8_t i;

#if AM_CFG_ARG_CHK_EN
    if (p_var == NULL || p_devinfo == NULL) {
        return -AM_EINVAL;
    }
#endif
    
    /* 初始化TPM0捕获 */
    tpm0_cap_handle = am_kl26_tpm0_cap_inst_init();

    /* TPM0通道2使用PIOC_3，串口1的接收引脚也应配置为该引脚。在 am_hwconfig_tpm0_cap.c 中配置 */
    i = am_cap_config(tpm0_cap_handle,
                  cap_chan,
                  AM_CAP_TRIGGER_FALL,
                  (am_cap_callback_t)cap_callback,
                  (void *)p_var);

    /* 使能捕获通道 */
    am_cap_enable(tpm0_cap_handle, cap_chan);
    
    am_wait_init(&p_var->wait);

    while (1) {
        if (am_wait_on_timeout(&p_var->wait, 100) == -AM_ETIME) {
            am_wdt_feed(p_var->wdt_handle);
            continue;
        }

        t1 = p_var->cap_count;

        am_wait_init(&p_var->wait);
 
        if (am_wait_on_timeout(&p_var->wait, 100) == -AM_ETIME) {
            am_cap_reset(tpm0_cap_handle, cap_chan);
            am_wait_init(&p_var->wait);
            continue;
        }    

        /* 如果t2小于t1，说明定时器溢出了 */
        t2 = p_var->cap_count;
        if (t2 < t1)
            t2 += 65536;
        
        /* 这句要放在给t2赋值之后，因为执行该函数期间会触发一次tpm中断，会更新
           p_var->cap_count的值 */
        am_cap_disable(tpm0_cap_handle, cap_chan);

        am_cap_count_to_time(tpm0_cap_handle,
                             cap_chan,
                             t1,
                             t2,
                            (unsigned int *)&t3);
        t3 = (1000000000 / t3) * 7;

        for (i = 0; i < AM_NELEMENTS(uart_baudrate); i++) {
            if (t3 > uart_baudrate[i] * 4 / 5 && t3 < uart_baudrate[i] * 6 / 5) {
                break;
            }
        }

        if (i < AM_NELEMENTS(uart_baudrate)) {
            p_var->baud_num = i;
            if (i == 0x00) {
                am_udelay(120);
            }
            break;
        }

        am_cap_enable(tpm0_cap_handle, cap_chan);
        am_cap_reset(tpm0_cap_handle, cap_chan);
        am_wait_init(&p_var->wait);
    }

    return AM_OK;
}

/**
 * \brief 串口波特率设置
 *
 * \param[in]  p_var     : 存放全局变量的指针
 * \param[out] p_devinfo : UARTCMD配置信息结构体指针
 *
 * \retval AM_OK      : 成功
 * \retavl -AM_EINVAL : 参数错误
 */
int uartcmd_comm_uart_baudrate_set (uartcmd_variable_t              *p_var,
                                    const struct am_uartcmd_devinfo *p_devinfo)
{
    uint16_t                       check_sum;
    const uartcmd_flash_devinfo_t *p_flash;
    uint8_t                       *p_buf;
    int                            ret = AM_OK;
    uint32_t                       key;
    
    p_flash = p_devinfo->p_flash_usr_devinfo;
    p_buf   = (uint8_t *)p_devinfo->p_iap_buf;
    
    if (p_var->baud_num != p_devinfo->p_flash_usr_devinfo->info.baud_num) {
        
        memcpy(p_buf, p_flash, sizeof(uartcmd_flash_devinfo_t));
        
        ((uartcmd_flash_devinfo_t *)p_buf)->info.baud_num = p_var->baud_num;
        check_sum = ~uartcmd_cmd_byte_sum_get((uint8_t *)&(((uartcmd_flash_devinfo_t *)p_buf)->info) + 2, 
                                              sizeof(uartcmd_flash_devinfo_t) - 2);
        
        ((uartcmd_flash_devinfo_t *)p_buf)->info.check_sum = check_sum;
        key = am_int_cpu_lock();
        ret = uartcmd_iap_copy((uint32_t)p_flash,
                               (uint32_t)p_buf,
                                1024);
        am_int_cpu_unlock(key);
        
        if (ret == AM_OK) {
            ret = uartcmd_iap_compare((uint32_t)p_flash,
                                      (uint32_t)p_buf,
                                       1024);
        }
    }
    
    return ret;
}

/**
 * \brief UART receive data(poll mode)
 *
 * \param[in] p_usart : Pointer to USART register block
 * \param[in] p_rxbuf : The receive buffer
 * \param[in] nbytes  : The number bytes to receive
 * 
 * \return The number bytes received successful
 */
static int __uart_poll_receive (am_uart_handle_t  handle,
                                char             *p_rxbuf,
                                uint32_t          nbytes)
{
    uint32_t len = nbytes; 
    
    while (len){
        if (am_uart_poll_getchar(handle, (char *)p_rxbuf) != AM_OK) {
             continue;
        }
        
        len--;
        p_rxbuf++;
    }
    
    return nbytes;
}


/**
 * \brief 自动侦测模式
 *
 * \param[in]  p_var     : 存放全局变量的指针
 * \param[out] p_devinfo : UARTCMD配置信息结构体指针
 *
 * \retval AM_OK      : 成功
 * \retavl -AM_EINVAL : 参数错误
 */
int uartcmd_comm_mode_auto_detect (uartcmd_variable_t              *p_var,
                                   const struct am_uartcmd_devinfo *p_devinfo)
{
    char temp;  /* 从串口中接收的字符 */
    int  ret;
    
#if AM_CFG_ARG_CHK_EN
    if (p_var == NULL || p_devinfo == NULL) {
        return -AM_EINVAL;
    }
#endif

    while (1) {
        am_uart_poll_putchar(p_var->uart_handle, 0x00);
        if (uartcmd_uart_auto_buadrate_get(p_var, p_devinfo) == AM_OK) {

            /* 将捕获引脚恢复为串口接收引脚 */
            am_gpio_pin_cfg(p_devinfo->rxd_pin, PIOC_3_UART1_RX);
            
            am_uart_ioctl(p_var->uart_handle,
                          AM_UART_BAUD_SET,
                         (void *)uart_baudrate[p_var->baud_num]);

            /* 等待接收一个字符（死等） */
            __uart_poll_receive(p_var->uart_handle, &temp, 1);
            am_uart_poll_putchar(p_var->uart_handle, 0x11);
            if (temp != 0x20) {
                continue;
            }
            
            ret = uartcmd_comm_uart_baudrate_set(p_var, p_devinfo);
            if (ret != AM_OK) {
                continue;
            }

            /* 避免回复0x06过快，导致用户接收不到 */
            am_udelay(3000);
            
            temp = 0x06;
            am_uart_poll_putchar(p_var->uart_handle, temp);
            
            p_var->comm_mode = UARTCMD_FLASH_COMM_UART;
        }
        return AM_OK;
    }
}

/**
 * \brief 获取通信模式
 *
 * \param[in]  p_var     : 存放全局变量的指针
 * \param[out] p_devinfo : UARTCMD配置信息结构体指针
 *
 * \retval AM_OK      : 成功
 * \retavl -AM_EINVAL : 参数错误
 */
int uartcmd_comm_mode_get (uartcmd_variable_t              *p_var,
                           const struct am_uartcmd_devinfo *p_devinfo)
{
    const uartcmd_flash_devinfo_t *p_flash;
    uint8_t                       *p_buf;
    int                            ret;
    uint32_t                       key;
    uint16_t                       check_sum;
    am_bool_t                      auto_detect = AM_TRUE;

#if AM_CFG_ARG_CHK_EN
    if (p_var == NULL || p_devinfo == NULL) {
        return -AM_EINVAL;
    }
#endif
    
    /* 
     * 先判断内部配置信息是否错误（错误原因一般是由于写入配置信息时，发生掉电，导致配置信息写入失败）
     */
    check_sum = ~uartcmd_cmd_byte_sum_get((uint8_t *)&(p_devinfo->p_flash_usr_devinfo->info) + 2,
                                          sizeof(uartcmd_flash_devinfo_t) - 2);
    if ((check_sum != p_devinfo->p_flash_usr_devinfo->info.check_sum) ||
        (p_var->door == AM_TRUE)) {

        /* 基本校验和出错，还原配置 */
        p_flash = p_devinfo->p_flash_usr_devinfo;
        p_buf   = (uint8_t *)p_devinfo->p_iap_buf;

        memcpy(p_buf, p_devinfo->p_flash_devinfo, sizeof(uartcmd_flash_devinfo_t));

        key = am_int_cpu_lock();
        ret = uartcmd_iap_copy((uint32_t)p_flash,
                               (uint32_t)p_buf,
                                1024);
        am_int_cpu_unlock(key);

        if (ret == AM_OK) {
            ret = uartcmd_iap_compare((uint32_t)p_flash,
                                      (uint32_t)p_buf,
                                       1024);
        }

        while (ret) {
            /* 点亮LED灯 */
            am_gpio_set(g_uartcmd_devinfo.led_num, AM_GPIO_LEVEL_LOW);
            am_mdelay(100);

            /* 熄灭LED灯 */
            am_gpio_set(g_uartcmd_devinfo.led_num, AM_GPIO_LEVEL_HIGH);
            am_mdelay(100);
        }
    }
           
    p_var->baud_num = p_devinfo->p_flash_usr_devinfo->info.baud_num;
    
    /* 软件配置 */
    if (p_devinfo->p_flash_usr_devinfo->info.work_mode != (UARTCMD_FLASH_MODE_DEFAULT & 0x0F)) {
        p_var->comm_mode = p_devinfo->p_flash_usr_devinfo->info.work_mode & 0x0F;
        if (p_var->comm_mode == UARTCMD_FLASH_COMM_UART) {
            p_var->baud_num = p_devinfo->p_flash_usr_devinfo->info.baud_num;
            auto_detect     = AM_FALSE;
        }        
        goto __COMM_MODE_INIT;
    }
__COMM_MODE_INIT:
   
    /* UART初始化 */
    p_var->uart_handle = am_kl26_uart1_inst_init();

    am_uart_ioctl(p_var->uart_handle,
                  AM_UART_BAUD_SET,
                 (void *)uart_baudrate[p_var->baud_num]);
       
    if (!auto_detect) {
        goto __COMM_MODE_DEINIT;
    }
    
    uartcmd_comm_mode_auto_detect(p_var, p_devinfo);
    
__COMM_MODE_DEINIT:
    
    if (p_var->comm_mode == UARTCMD_FLASH_COMM_UART) {
        
        /* UARTCMD 模块中的串口初始化 */
        uartcmd_serial_uart_mkdev(&p_var->serial_uart,
                                  &p_var->serial_uart_timer,
                                   p_var->uart_handle,
                                  &p_var->uart_softtimer,
                                   30,
                                   UARTCMD_FLASH_COMM_UART);

        uartcmd_serial_uart_init(p_var->uartcmd_handle, &p_var->serial_uart);
    }
    
    return AM_OK;
}

/**
 * \brief 主函数初始化
 *
 * \param[in] p_var     : uartcmd_variable_t 结构体指针
 * \param[in] p_devinfo : UARTCMD 配置信息结构体指针
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
int uartcmd_main_init (uartcmd_variable_t              *p_var,
                       const struct am_uartcmd_devinfo *p_devinfo)
{
#if AM_CFG_ARG_CHK_EN
    if (p_var == NULL || p_devinfo == NULL) {
        return -AM_EINVAL;
    }
#endif




    /* 其他管脚配置为上拉，输入 */
    am_gpio_pin_cfg(p_devinfo->txd_pin, AM_GPIO_INPUT | AM_GPIO_PULLUP);
    am_gpio_pin_cfg(p_devinfo->rxd_pin, AM_GPIO_INPUT | AM_GPIO_PULLUP);

    p_var->comm_mode = UARTCMD_FLASH_COMM_UART;
    /* 工作为从机模式 */
    p_var->work_mode  = UARTCMD_FLASH_WORK_MODE_SLAVE;
    p_var->frame_type = p_devinfo->p_flash_usr_devinfo->info.frame_format;

    p_var->wdt_handle = am_kl26_wdt_inst_init();
    am_wdt_enable(p_var->wdt_handle, 2000);

    /*　FTFA初始化 */
    uint32_t key = am_int_cpu_lock();
    am_fsl_ftfa_init();
    am_mdelay(5);
    am_int_cpu_unlock (key);

    return AM_OK;
}

/**
 * \brief 命令类型初始化
 *
 * \param[in] handle : UARTCMD 服务句柄
 * \param[in] p_var  : uartcmd_variable_t 结构体指针
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
int uartcmd_cmd_init(am_uartcmd_handle_t handle, uartcmd_variable_t *p_var)
{
#if AM_CFG_ARG_CHK_EN
    if (handle == NULL || p_var == NULL) {
        return -AM_EINVAL;
    }
#endif
    
    p_var->spi_handle   = am_kl26_spi0_poll_inst_init();
 
    /* fm175xx初始化,获得fm175xx设备 */
    am_fm175xx_init(&p_var->fm175xx_dev,
                     p_var->spi_handle,
                    &__g_fm175xx_devinfo);

    p_var->p_antenna_info = __g_cd4051_dev;
    
    p_var->reader_card_handle = amdr_reader_card_init(&p_var->reader_card_dev,
                                                      &p_var->fm175xx_dev,
                                                      &p_var->p_antenna_info);

    p_var->iso14443_handle = amdr_ic_iso14443_init(&p_var->iso14443_dev,
                                                   &p_var->fm175xx_dev);

    /* 设备控制类命令初始化 */
    p_var->dev_ctrl.p_arg        = (void *)&g_iap_funcs;
    p_var->dev_ctrl.cmd_class    = UARTCMD_CMD_CLASS_DEVICE;
    p_var->dev_ctrl.p_iso_handle = (void *)p_var->reader_card_handle;
    uartcmd_cmd_dev_ctrl_init(handle, &p_var->dev_ctrl);
    
    /**************************************************************************/
    
    p_var->mifare_handle = amdr_ic_mifare_init(&p_var->mifare_dev,
                                                p_var->iso14443_handle,
                                               &p_var->fm175xx_dev);

    /* Mifare类命令初始化 */
    uartcmd_cmd_mifare_init(handle,
                           &p_var->mifare,
                           &p_var->mifare_var,
                            p_var->mifare_handle,
                            p_var->iso14443_handle,
                            p_var->plus_handle,
                            p_var->reader_card_handle,
                            UARTCMD_CMD_CLASS_MIFARE);
    
    /**************************************************************************/
    
    uartcmd_cmd_iso14443_init(handle,
                             &p_var->iso14443,
                              p_var->iso14443_handle,
                              p_var->reader_card_handle,
                              UARTCMD_CMD_CLASS_ISO14443);
                                                   
    /**************************************************************************/
                             
    return AM_OK;
}


/**
 * \brief 主函数入口
 */
void demo_amks16rfid8_uartcmd_handle_entry (void)

{
    /* 点亮LED灯 */
    am_gpio_pin_cfg(g_uartcmd_devinfo.led_num, AM_GPIO_OUTPUT_INIT_LOW);

    /* 主函数初始化 */
    uartcmd_main_init(&g_var, &g_uartcmd_devinfo);

    /* UARTCMD 初始化函数 */
    g_var.uartcmd_handle = am_uartcmd_init(&g_var.uartcmd_dev,
                                           &g_uartcmd_devinfo,
                                            g_var.comm_mode,
                                            g_var.work_mode,
                                            g_var.frame_type);

    /* 新帧类型注册 */
    uartcmd_frame_new_init(g_var.uartcmd_handle,
                          &g_var.frame_new,
                           UARTCMD_FLASH_FRAME_FORMAT_NEW);
    
    /* 命令类型初始化 */
    uartcmd_cmd_init(g_var.uartcmd_handle, &g_var);
    
    /* 熄灭LED灯 */
    am_gpio_set(g_uartcmd_devinfo.led_num, AM_GPIO_LEVEL_HIGH);

    /* 获取通信模式 */
    uartcmd_comm_mode_get(&g_var, &g_uartcmd_devinfo);
    
    if(__g_cd4051_dev.antenna_num == AM_FM175XX_ANTENNA_EIGHT){
        uartcmd_cd4051_pin_init(&__g_cd4051_dev);
    }
    while (1) {
        am_wdt_feed(g_var.wdt_handle);
        if (am_fm175xx_check(&g_var.fm175xx_dev)) {
            am_gpio_set(g_uartcmd_devinfo.led_num, AM_GPIO_LEVEL_LOW);
        } else {
            am_gpio_set(g_uartcmd_devinfo.led_num, AM_GPIO_LEVEL_HIGH);
        }
        am_uartcmd_execute(g_var.uartcmd_handle);
    }
}

/* end of file */

/** [src_kl26_std_tpm_cap] */

/* end of file */

