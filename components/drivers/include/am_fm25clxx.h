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
#include "am_nvram.h"

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

    /**< \brief SPI设备              */
    am_spi_device_t              spi_dev;

    /**< \brief fm25clxx设备提供的 NVRAM 标准服务 */
    am_nvram_dev_t               *p_serv;

    /**< \brief 实例信息             */
    const am_fm25clxx_devinfo_t  *p_devinfo;

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
 * \brief 初始化fm25clxx的NVRAM功能，向系统提供  NVRAM 标准服务
 * 
 *   该函数将fm25clxx设备作为标准的NVRAM设备注册到系统中
 * 
 * \param[in] handle      : ep24cxx操作句柄
 * \param[in] p_dev       : NVRAM 标准设备
 * \param[in] p_dev_name  : NVRAM 标准设备的设备名
 *
 * \return AM_OK, 提供NVRAM服务成功；其它值，提供NVRAM服务失败。
 *
 * \note 使用该函数后，即可使用NVRAM标准接口 am_nvram_set() 和 am_nvram_get() 访问
 * fm25clxx存储器，使用的段必须在 am_nvram_cfg.c文件中定义，存储段依赖的设备名即可
 * 填写为此处 p_name 指定的名字，如 p_name 为  "fm24c02", 这可以向g_nvram_segs[]存
 * 储段列表中增加如下5个存储段（仅作为示例）：
 *    {"ip",         0,  0,  4,   "fm24c02"},
 *    {"ip",         1,  4,  4,   "fm24c02"},
 *    {"temp_limit", 0,  8,  4,   "fm24c02"},
 *    {"system",     0,  12, 50,  "fm24c02"},
 *    {"test",       0,  62, 178, "fm24c02"},
 */
int am_fm25clxx_nvram_init (am_fm25clxx_handle_t   handle,
                            am_nvram_dev_t        *p_dev,
                            const char            *p_dev_name);
                                
/**
 * @}
 */

#ifdef __cplusplus
}
#endif
 
#endif /* __AM_FM25CLXX_H */

/* end of file */
