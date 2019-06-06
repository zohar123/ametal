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
 * \brief 旺宏公司的MX25系列FM25CLXX芯片驱动 
 * 
 * 已知兼容芯片： 
 *   - MX25L1606E
 *   - MX25L8006E
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-09-14  tee, first implementation.
 * \endinternal
 */
 
#ifndef __AM_FM25CLXX_H
#define __AM_FM25CLXX_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "ametal.h"
#include "am_types.h"
#include "am_gpio.h"
#include "am_spi.h"
    
/*******************************************************************************
  宏定义
*******************************************************************************/

/**
 * \name SPI FLASH的各个命令
 * @{
 */

#define  __FM25CL64B_WREN  0x06  //设置写使能锁存器
#define  __FM25CL64B_WRDI  0x04  //写禁能
#define  __FM25CL64B_RDSR  0x05  //读状态寄存器
#define  __FM25CL64B_WRSR  0x01  //写状态寄存器
#define  __FM25CL64B_READ  0x03  //读取内存数据
#define  __FM25CL64B_WRITE 0x02  //写入内存数据
/** @} */


/**
 * \name 状态寄存器的值
 * @{
 * 寄存器说明：bit7:   WPEN  写保护使能位；
 *            bit3:2  BP1:0 块保护；
 *                          00--无
 *                          01--后1/4
 *                          10--后1/2
 *                          11--全部
 *            bit1    WEL   写使能；
 */
#define  __PROTECTED_ALL        0x8E //全部地址写保护:0000h-1FFFh
#define  __PROTECTED_HALF       0x8A //后半部分写保护:1000h-1FFFh
#define  __PROTECTED_HALF_HALF  0x86 //后1/4写保护   :1800h-1FFFh
#define  __PROTECTED_NO         0x82 //无写保护
/** @} */

/**
 * \addtogroup am_if_fm25clxx
 * \copydoc am_fm25clxx.h
 * @{
 */

 
/**
 * \brief FM25CLXX 实例信息
 */
typedef struct am_fm25clxx_devinfo {
 
    uint16_t          spi_mode;      /**< \brief 器件使用的SPI模式 */
    int               spi_cs_pin;    /**< \brief SPI片选引脚       */
    uint32_t          spi_speed;     /**< \brief 使用的SPI速率     */
} am_fm25clxx_devinfo_t;
     
/**
 * \brief FM25CLXX 实例
 */
typedef struct am_fm25clxx_dev {
    am_spi_device_t              spi_dev;        /**< \brief SPI设备              */
    const am_fm25clxx_devinfo_t *p_devinfo;      /**< \brief 实例信息            */
} am_fm25clxx_dev_t;

/** \brief 定义 FM25CLXX 的实例句柄类型 */
typedef struct am_fm25clxx_dev *am_fm25clxx_handle_t;


/** 
 * \brief 初始化 FM25CLXX，获取操作 FM25CLXX 的 handle
 * 
 * \param[in] p_dev      : 指向 FM25CLXX 实例的指针
 * \param[in] p_devinfo  : 指向 FM25CLXX 实例信息的指针
 * \param[in] handle     : SPI标准服务handle,使用该 handle 与 FM25CLXX 进行通信
 *
 * \return FM25CLXX 操作句柄，如果初始化失败，则返回值为NULL
 */
am_fm25clxx_handle_t am_fm25clxx_init(am_fm25clxx_dev_t            *p_dev,
                                      const am_fm25clxx_devinfo_t  *p_devinfo,
                                      am_spi_handle_t               spi_handle);

/**
 * \brief 读取数据
 *
 * \param[in] handle : FM25CLXX 操作句柄
 * \param[in] addr   : 读取数据的首地址
 * \param[in] p_buf  : 读取数据存放的缓冲区
 * \param[in] len    : 数据读取的长度
 *
 * \retval  AM_OK     : 读取数据成功
 * \retval -AM_EINVAL : 读取数据失败, 参数错误
 * \retval -AM_EIO    : 读取数据失败, SPI通信出错
 */
int am_fm25clxx_read(am_fm25clxx_handle_t  handle,
                     uint32_t              addr,
                     uint8_t              *p_buf,
                     uint32_t              len);


/** 
 * \brief 写入数据
 *
 * \param[in] handle : FM25CLXX 操作句柄
 * \param[in] addr   : 写入数据的首地址
 * \param[in] p_buf  : 写入数据存放的缓冲区
 * \param[in] len    : 数据读取的长度
 *
 * \retval  AM_OK     : 读取数据成功
 * \retval -AM_EINVAL : 读取数据失败, 参数错误
 * \retval -AM_EIO    : 读取数据失败, SPI通信出错
 */
int am_fm25clxx_write(am_fm25clxx_handle_t  handle,
                      uint32_t              addr,
                      uint8_t              *p_buf,
                      uint32_t              len);

                            
/** 
 * \brief 读取FM25CLXX状态寄存器的值
 * 
 * \param[in]  handle : FM25CLXX 操作句柄
 * \param[out] p_stat : 获取到的状态寄存器的值
 *
 * \retval  AM_OK     : 读取状态寄存器成功
 * \retval -AM_EINVAL : 读取失败, 参数错误
 * \retval -AM_EIO    : 读取失败, SPI通信出错
 */
int am_fm25clxx_status_read(am_fm25clxx_handle_t  handle,
                            uint8_t              *p_stat);

/** 
 * \brief 设置FM25CLXX状态寄存器的值
 * 
 * \param[in] handle : FM25CLXX 操作句柄
 * \param[in] val    : 写入状态寄存器的值
 *
 * \retval  AM_OK     : 写入状态寄存器成功
 * \retval -AM_EINVAL : 写入失败, 参数错误
 * \retval -AM_EIO    : 写入失败, SPI通信出错
 *
 */
int am_fm25clxx_status_write(am_fm25clxx_handle_t  handle,
                             uint8_t               val);

/** 
 * \brief FM25CLXX解初始化，释放相关资源
 * 
 *     解初始化后，FM25CLXX相关函数均不可使用，包括使用FM25CLXX获取
 * 到的NVRAM标准服务handle。
 * 
 * \param[in] p_dev : 指向 FM25CLXX 设备的指针
 *
 * \return 无
 */
void am_fm25clxx_deinit(am_fm25clxx_dev_t *p_dev);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
 
#endif /* __AM_FM25CLXX_H */

/* end of file */
