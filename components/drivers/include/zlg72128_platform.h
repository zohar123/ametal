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
 * \brief ZLG72128平台相关文件
 *
 *  应根据实际平台，修改完善该文件，完成相关类型、宏和函数的定义。
 *
 * \internal
 * \par Modification History
 * - 1.00 19-4-22  xgg, first implementation.
 * \endinternal
 */

#ifndef __ZLG72128_PLATFORM_H
#define __ZLG72128_PLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus  */

#include "ametal.h"
#include "am_wait.h"
#include "am_delay.h"
#include "am_gpio.h"
#include "am_i2c.h"
#include "am_types.h"

/**
 * \name 数据类型定义
 *
 * 应根据实际平台完成这些类型的定义。若平台已经有这些类型的定义，则可以直接包含
 * 相关类型的头文件，并删除下列类型的定义。
 *
 * @{
 */
typedef unsigned char  bool_t;

/** @} */

/**
 * \name 常量定义
 * @{
 */

/** \brief NULL 定义 */
#ifndef NULL
#define NULL ((void *)0)
#endif

/** @} */

/**
 * \name 工具函数
 * @{
 */

/**
 * \brief ms 级延时函数
 * \param[in] ms : 延时时间（单位：ms）
 * \return 无
 */
void zlg72128_plfm_delay_ms (uint32_t ms);

/** @} */


/**
 * \brief ZLG72128 平台相关的初始信息
 *
 * 在初始化ZLG72128时，可能需要一些与平台相关的信息，例如引脚信息、I2C总线信息等
 *
 * 在不同平台中，引脚等信息的表示方法可能不同，因而对应的类型可能不同。
 *
 * 典型的，其可以定义为一个结构体类型，其中包含了平台相关的信息，例如，系统使用
 * int类型的ID表示I2C总线标号，int类型的数值表示引脚编号，则该类型可以定义为：
 *
 * typedef struct {
 *     int  pin_rst;       // RST 引脚编号
 *     int  i2c_id;        // I2C总线编号
 * } zlg72128_plfm_init_info_t;
 *
 * 该类型完全由平台适配者定义，其决定了用户在该平台下使用ZLG72128时，需要传递的
 * 一些与平台相关的基础信息。
 *
 * 驱动在调用平台初始化函数 zlg72128_plfm_init() 时，原封不动的将平台相关的初
 * 始信息传递给平台初始化函数。
 *
 * 该类型中所有成员的赋值与使用，都由平台相关函数负责，ZLG72128驱动不会对其中的
 * 成员作任何操作。
 *
 * 若平台不使用该类型，可以默认为 void * 类型。
 */
typedef struct {

    /* 设备从机地址*/
    uint8_t    slv_addr;

    /** \brief 复位引脚 */
    int        rst_pin;

    /**
     * \brief 是否使用中断引脚，若为节省一个IO资源，则可不使用中断方式。
     * AM_TRUE  : 使用中断引脚(int_pin 参数指明引脚号)，在引脚中断中获取键值
     * AM_FALSE : 以一定的时间间隔（interval_ms参数指定）查询键值
     */
    am_bool_t  use_int_pin;

    /** \brief 中断引脚 */
    int        int_pin;

    /** \brief 仅当 use_int_pin 为 FALSE 时，该参数有效，指定查询的时间间隔 */
    uint32_t   interval_ms;

    /** \brief I2C句柄获取函数*/
    am_i2c_handle_t (*pfn_i2c_handle_get) (void);

    /** \brief I2C解初始化函数*/
    void (*pfn_i2c_deinit) (am_i2c_handle_t i2c_handle);

}zlg72128_plfm_init_info_t;

/**
 * \brief 平台函数参数类型
 *
 * 该类型的具体定义与平台相关，典型的，其可以定义为一个结构体类型，其中包含一些
 * 成员，用以保存平台函数中需要保存的状态。
 *
 * 例如，I2C异步传输可能分为几个阶段完成，则可以定义一个 state 成员，用以保存I2C
 * 异步传输当前的状态:
 *
 * typedef struct {
 *     int state;
 * } zlg72128_plfm_t;
 *
 * 在ZLG72128设备类型的定义中，将包含一个该类型的成员，在调用平台相关函数时，
 * 将该成员的地址作为第一个参数传递。
 *
 * 若平台不使用该类型，可以默认为 void * 类型。
 *
 * 该类型中所有成员的赋值与使用，都由平台相关函数负责，ZLG72128驱动不会对其中的
 * 成员作任何操作。通常情况下，其中的成员应该在 zlg72128_plfm_init()函数中初始化，
 * 在其他各平台函数中再使用。
 */
typedef struct {

    /* 读操作缓存区*/
    uint8_t            r_buff[4];

    /* 软件定时器 */
    am_softimer_t      timer;

    /* 写操作时需要的I2C设备*/
    am_i2c_device_t    i2c_dev;

    /* I2C读操作时需要的传输结构体*/
    am_i2c_transfer_t  trans[4];

    /* 用于存放I2C读操作的消息*/
    am_i2c_message_t   msg;

    /* I2C服务标准handle */
    am_i2c_handle_t    i2c_handle;

    /* 键值请求查询函数参数*/
    void              *p_key_arg;

    /* 键值请求查询函数*/
    void  (*pfn_keyval_report) (void *, uint8_t reg0_3[4]);

    /* 用于保存设备信息指针 */
    const zlg72128_plfm_init_info_t  *p_devinfo;

}zlg72128_plfm_t;

/**
 * \brief 平台初始化，该函数将会在 zlg72128_init()函数中，被首先调用
 *
 * 与平台相关的初始化，例如，初始化引脚、打开引脚中断等
 *
 * \param[in] p_plfm            : 平台自身结构体对象
 * \param[in] p_plfm_init_info  : 平台要求用户提供的一些初始化信息
 * \param[in] pfn_keyval_report : 平台需要伺机调用的“键值上报”函数
 * \param[in] p_key_arg         : 平台在调用“键值上报”函数时，传递给第一个参数的值
 *
 * \retval 0   : 初始化成功
 * \retval 非0 : 初始化失败
 *
 * \note 关于 pfn_keyval_report 和 p_key_arg 的进一步说明
 *
 *  pfn_keyval_report()，用于平台上报按键数据，平台可以自行选择合适的时机上报
 *  寄存器0x01 ~ 0x03 这3个寄存器中的当前值（读取出来上报即可，无需做任何额外
 *  的检查和处理，平台适配时，无需关心这3个寄存器的具体含义）。其调用形式如下：
 *
 *       uint8_t reg1_3[4];
 *       // 获取 寄存器1 ~ 寄存器3 这3个寄存器的值，存于 reg1_3 中。
 *       pfn_keyval_report(p_key_arg, reg0_3);
 *
 *  之所以让平台直接上报键值，而不是适配I2C读函数，主要有两个原因：
 *
 *  1. 这里需要读取的寄存器地址（0x01 ~ 0x03）是固定的，不需要适配出读取任意地址
 *     的“读函数”，因而不同平台可以伺机优化。
 *
 *  2. 在不同平台中，期望读取这些数据的时机可能不尽相同，例如：
 *
 *     - 裸机平台
 *           1. 可能在KEY_INT中断中直接获取；
 *           2. 可能在KEY_INT中设置一个标志，然后在主循环中查看检查该标志；
 *           3. 不使用KEY_INT引脚，直接在主循环中轮询；
 *           4. 不使用KEY_INT引脚，使用一个定时器轮询。

 *    - 具有操作系统的平台
 *           1. 可能在KEY_INT中断中直接获取；
 *           2. 可能在KEY_INT中断中释放一个信号量，然后在一个任务获取该信号量；
 *           3. 不使用KEY_INT引脚，直接在一个任务中轮询；
 *           4. 不使用KEY_INT引脚，使用一个定时器轮询。
 *
 *     在不同的调用时机，对I2C读取函数的要求也可能不同。例如，在中断中读取，由于
 *  I2C读取完4个数据需要花费ms级别的时间，因此，若在中断中等待I2C数据传输完成，则
 *  会极大的影响系统实时性。基于此，I2C读数据时，可能需要采用异步的方式，以便中断
 *  可以尽快结束（例如，在KEY_INT中断中只打开I2C中断，然后在中断中通知I2C硬件先
 *  读取reg0系统寄存器，若键值有效则一次再去读取reg1 ~ reg3），进而保证系统的实时性；
 *  而在主循环或普通任务中读取，则可以同步读取，I2C读取占用的时间并不会影响系统实时性。
 *
 *  综上，采用直接上报键值的方式，可以为平台保留极大的灵活性，对调用时机和I2C读
 *  数据的方式（同步或异步）都没有任何限制，平台可以根据需要选择最合适的方式。
 */
int zlg72128_plfm_init (zlg72128_plfm_t                  *p_plfm,
                        const zlg72128_plfm_init_info_t  *p_plfm_init_info,
                        void  (*pfn_keyval_report)  (void *, uint8_t reg1_3[3]),
                        void                             *p_key_arg);

/**
 * \brief 平台解初始化函数
 *
 * 释放平台相关的资源，例如部分平台需要释放引脚、关闭引脚中断、删除引脚回调函数
 * 或者释放i2c相关的资源
 * 若用户自身平台不需要释放任何资源，函数内部可以不做任何操作
 *
 * \param[in] p_plfm : 平台自身结构体对象，用于保存一些必要的状态数据
 *
 * \retval 0   : 解初始化成功
 * \retval 非0 : 解初始化失败
 */
int zlg72128_plfm_deinit (zlg72128_plfm_t *p_plfm);

/**
 * \brief I2C写函数
 *
 * 从指定器件中的某一寄存器开始，连续写入指定字节数的数据（该函数主要用于向
 * ZLG72128发送数码管显示控制命令）
 *
 * \param[in] p_plfm    : 平台自身结构体对象
 * \param[in] sub_addr  : 寄存器地址
 * \param[in] p_buf     : 数据缓存区
 * \param[in] nbytes    : 一次读写字节数
 *
 * \retval 0   : 写入成功
 * \retval 非0 : 写入失败（该值将原封不动的返回给上层应用）
 */
int zlg72128_plfm_i2c_write(zlg72128_plfm_t *p_plfm,
                            uint8_t          sub_addr,
                            uint8_t         *p_buf,
                            uint8_t          nbytes);
/**
 * @}
 */

#ifdef __cplusplus
}
#endif    /* __cplusplus */

#endif /* __ZLG72128_PLATFORM_H */

