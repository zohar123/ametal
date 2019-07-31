/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2019 Guangzhou ZHIYUAN Electronics Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
*******************************************************************************/

/**
 * \file
 * \brief 平台适配函数的实现
 *
 *  zlg72128_platform.h 文件中声明的各个函数实现
 *
 * \internal
 * \par Modification History
 * - 1.00 19-4-22  xgg, first implementation.
 * \endinternal
 */
#include "zlg72128_platform.h"

/* 系统寄存器地址*/
static const uint8_t __g_sys_addr = 0x00;
/* 键值寄存器地址*/
static const uint8_t __g_key_addr = 0x01;

/* 键值上报函数*/
static void __zlg72128_key_report(zlg72128_plfm_t *p_plfm);
/* 检测键值是否有效函数*/
static void __zlg72128_key_vaild(zlg72128_plfm_t *p_plfm);

/* ms 级别延时函数实现 */
void zlg72128_plfm_delay_ms (uint32_t ms)
{
    am_mdelay(ms);

}

/* 检查系统寄存器是否有效函数*/
static void __zlg72128_key_vaild(zlg72128_plfm_t *p_plfm)
{
    /* 如果系统寄存器不为0，则依次读取其他寄存器*/
    if (p_plfm->r_buff[0]) {

        am_i2c_mktrans(&p_plfm->trans[2],
                        p_plfm->p_devinfo->slv_addr,
                        AM_I2C_ADDR_7BIT | AM_I2C_M_WR,
                        (uint8_t *)&__g_key_addr,
                        1);

        /* 读取有效键值寄存器 */
        am_i2c_mktrans(&p_plfm->trans[3],
                        p_plfm->p_devinfo->slv_addr,
                        AM_I2C_M_RD,
                        &p_plfm->r_buff[1],
                        3);
        /*构建消息*/
        am_i2c_mkmsg(&(p_plfm->msg),
                     &(p_plfm->trans[2]),
                     2,
                     (am_pfnvoid_t)__zlg72128_key_report,
                     (void *)p_plfm);

        /* 查询键值寄存器值*/
        am_i2c_msg_start(p_plfm->i2c_handle, &p_plfm->msg);
    } else {

        if (p_plfm->p_devinfo->use_int_pin) {
            /* 重新打开触发中断*/
            am_gpio_trigger_on(p_plfm->p_devinfo->int_pin);
        } else {
            /* 启动软件定时器*/
            am_softimer_start(&(p_plfm->timer),
                               p_plfm->p_devinfo->interval_ms);
        }
    }
}

/* 上报有效键值函数*/
static void __zlg72128_key_report(zlg72128_plfm_t *p_plfm)
{
    /* 上报有效键值*/
    p_plfm->pfn_keyval_report(p_plfm->p_key_arg, &p_plfm->r_buff[1]);

    if (p_plfm->p_devinfo->use_int_pin) {
        /* 重新打开触发中断*/
        am_gpio_trigger_on(p_plfm->p_devinfo->int_pin);
    } else {
        /* 启动软件定时器*/
        am_softimer_start(&(p_plfm->timer),
                           p_plfm->p_devinfo->interval_ms);
    }
}

/* 引脚中断处理函数*/
static void __int_keyval_i2c_read(zlg72128_plfm_t *p_plfm)
{
    /* 关闭引脚中断*/
    if (am_gpio_trigger_off(p_plfm->p_devinfo->int_pin)) {
         return;
    }

    am_i2c_mktrans(&p_plfm->trans[0],
                    p_plfm->p_devinfo->slv_addr,
                    AM_I2C_ADDR_7BIT | AM_I2C_M_WR,
                    (uint8_t *)&__g_sys_addr,
                    1);

    /* 读取系统寄存器 */
    am_i2c_mktrans(&p_plfm->trans[1],
                    p_plfm->p_devinfo->slv_addr,
                    AM_I2C_M_RD,
                    p_plfm->r_buff,
                    1);

    /*构建消息*/
    am_i2c_mkmsg(&(p_plfm->msg),
                 &(p_plfm->trans[0]),
                 2,
                 (am_pfnvoid_t)__zlg72128_key_vaild,
                 (void *)p_plfm);

    /* 查询系统寄存器，检测键值是否有效*/
    am_i2c_msg_start(p_plfm->i2c_handle, &p_plfm->msg);
}

/* 定时器轮询处理函数*/
static void __timer_keyval_i2c_read(zlg72128_plfm_t *p_plfm)
{
    if (!p_plfm->p_devinfo->use_int_pin) {
        /* 关闭软件定时器*/
        am_softimer_stop(&(p_plfm->timer));
    }

    am_i2c_mktrans(&p_plfm->trans[0],
                    p_plfm->p_devinfo->slv_addr,
                    AM_I2C_ADDR_7BIT | AM_I2C_M_WR,
                    (uint8_t *)&__g_sys_addr,
                    1);

    /* 读取系统寄存器 */
    am_i2c_mktrans(&p_plfm->trans[1],
                    p_plfm->p_devinfo->slv_addr,
                    AM_I2C_M_RD,
                    p_plfm->r_buff,
                    1);

    /*构建消息*/
    am_i2c_mkmsg(&(p_plfm->msg),
                 &(p_plfm->trans[0]),
                 2,
                 (am_pfnvoid_t)__zlg72128_key_vaild,
                 (void *)p_plfm);

    /* 查询系统寄存器，检测键值是否有效*/
    am_i2c_msg_start(p_plfm->i2c_handle, &p_plfm->msg);
}

/**
 * \brief 平台初始化，该函数将会在 zlg72128_init()函数中，被首先调用
 *
 * 与平台相关的初始化，例如，初始化GPIO、I2C等、打开引脚中断等
 */
int zlg72128_plfm_init (zlg72128_plfm_t                  *p_plfm,
                        const zlg72128_plfm_init_info_t  *p_plfm_init_info,
                        void  (*pfn_keyval_report) (void *, uint8_t reg1_3[3]),
                        void                             *p_key_arg)
{
    int ret = 0;

    if (p_plfm == NULL || p_plfm_init_info == NULL) {

        return -AM_EINVAL;
    }

    p_plfm->p_devinfo         = p_plfm_init_info;
    p_plfm->p_key_arg         = p_key_arg;
    p_plfm->i2c_handle        = p_plfm_init_info->pfn_i2c_handle_get();
    p_plfm->pfn_keyval_report = pfn_keyval_report;

    /* 制作从机设备用于I2C读写操作*/
    am_i2c_mkdev(&p_plfm->i2c_dev,
                  p_plfm->i2c_handle,
                  p_plfm->p_devinfo->slv_addr,
                  AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE);

    /* 复位引脚配置*/
    if (p_plfm->p_devinfo->rst_pin != -1) {
        /* 配置复位引脚输出状态为低电平*/
        if (am_gpio_pin_cfg(p_plfm->p_devinfo->rst_pin,
                            AM_GPIO_OUTPUT_INIT_LOW)) {

            return -AM_ENOTSUP;
        }

        am_mdelay(1);
        /* 拉高复位引脚*/
        if (am_gpio_set(p_plfm->p_devinfo->rst_pin, 1)) {

            return -AM_ENXIO;
        }
        am_mdelay(5);
    }

    /* 中断引脚配置*/
    if (p_plfm->p_devinfo->use_int_pin) {

        if (am_gpio_pin_cfg(p_plfm->p_devinfo->int_pin,
                            AM_GPIO_INPUT | AM_GPIO_PULLUP)) {
            return -AM_ENOTSUP;
        }

        /* 连接GPIO引脚中断 */
        if (am_gpio_trigger_connect(p_plfm_init_info->int_pin,
                                    (am_pfnvoid_t)__int_keyval_i2c_read,
                                    (void *)p_plfm)) {
            return -AM_EPERM;
        }

        /* 设置为低电平触发，若失败则设置为下降沿触发*/
        if(am_gpio_trigger_cfg(p_plfm_init_info->int_pin,
                               AM_GPIO_TRIGGER_LOW)) {

            if(am_gpio_trigger_cfg(p_plfm_init_info->int_pin,
                                   AM_GPIO_TRIGGER_FALL)) {
                /* 不支持当前触发方式*/
                return -AM_EPERM;
            }

        }
        if (am_gpio_trigger_on(p_plfm_init_info->int_pin)) {

            /* 使能引脚中断失败*/
            return -AM_EPERM;
        }
    } else {
        /* 使用软件定时器以一定的时间间隔扫描 */
        ret = am_softimer_init(&(p_plfm->timer),
                               (am_pfnvoid_t)__timer_keyval_i2c_read,
                               (void *)p_plfm);

        if (ret) {
            return ret;
        }

        am_softimer_start(&(p_plfm->timer),
                          p_plfm->p_devinfo->interval_ms);
    }

    return AM_OK;
}

/**
 * \brief 平台解初始化
 *
 * 释放一些与平台相关的资源，例如部分平台需要关闭引脚中断、释放引脚
 */
int zlg72128_plfm_deinit(zlg72128_plfm_t *p_plfm)
{
    int ret = 0;

    if(p_plfm == NULL) {
        return -AM_EINVAL;
    }

    p_plfm->p_devinfo->pfn_i2c_deinit(p_plfm->i2c_handle);

    if (p_plfm->p_devinfo->use_int_pin) {
        /* 关闭引脚中断*/
        if (am_gpio_trigger_off(p_plfm->p_devinfo->int_pin)) {
            return -AM_EPERM;
        }
        /* 删除引脚回调函数*/
        ret = am_gpio_trigger_disconnect(p_plfm->p_devinfo->int_pin,
                                         (am_pfnvoid_t)__int_keyval_i2c_read,
                                         (void *)p_plfm);
        if (ret) {
            return ret;
        }
    } else {

        am_softimer_stop(&(p_plfm->timer));
    }

    return AM_OK;
}

/* I2C写函数函数实现*/
int zlg72128_plfm_i2c_write (zlg72128_plfm_t *p_plfm,
                             uint8_t          sub_addr,
                             uint8_t         *p_buf,
                             uint8_t          nbytes)
{
    if (p_plfm == NULL) {

        return -AM_EINVAL;
    }

    return am_i2c_write(&p_plfm->i2c_dev,
                         sub_addr,
                         p_buf,
                         nbytes);
}


/* end of file */
