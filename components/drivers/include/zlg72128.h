/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2019 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
*******************************************************************************/

/**
 * \file
 * \brief ZLG72128通用驱动包
 *
 * \note
 * 根据ZLG72128的特性，组件将I2C的传输抽离出来，在zlg72128_platform.h
 * 中已经对I2C相关的传输函数进行了定义，用户在使用组件时，需要根据自身的平台来对
 * 其进行适配，若是运用ZLG72128模块做应用，则只需要注意组件内的接口函数即可。
 * zlg72128_platform.h中不仅仅包含了I2C传输函数，还有与平台相关的初始化
 * 函数及延时函数，用户也需要根据自身平台来进行适配，详情见zlg72128_platform.h
 * 文件。
 *
 * \internal
 * \par Modification History
 * - 1.00 18-11-13  xgg， first implementation.
 * \endinternal
 */

#ifndef __ZLG72128_H
#define __ZLG72128_H

#include "zlg72128_platform.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup if_zlg72128
 * @copydoc zlg72128.h
 * @{
 */

/**
 * \name 普通按键（第1行 ~ 第3行所有按键）键值宏定义
 *
 *  列举出了 zlg72128_key_cb_t 回调函数中的 key_val 可能的一系列值，可以
 *  使用key_val值与这一系列宏值作相应对比，以判断是哪一按键按下。宏的命名规
 *  则是ZLG72128_KEY_x_y ，其中，x代码哪一行（1 ~ 3），y表示哪一列（1 ~ 8)
 *
 * @{
 */

#define  ZLG72128_KEY_1_1     1     /**< \brief 第1排第1个按键  */
#define  ZLG72128_KEY_1_2     2     /**< \brief 第1排第2个按键  */
#define  ZLG72128_KEY_1_3     3     /**< \brief 第1排第3个按键  */
#define  ZLG72128_KEY_1_4     4     /**< \brief 第1排第4个按键  */
#define  ZLG72128_KEY_1_5     5     /**< \brief 第1排第5个按键  */
#define  ZLG72128_KEY_1_6     6     /**< \brief 第1排第6个按键  */
#define  ZLG72128_KEY_1_7     7     /**< \brief 第1排第7个按键  */
#define  ZLG72128_KEY_1_8     8     /**< \brief 第1排第8个按键  */

#define  ZLG72128_KEY_2_1     9     /**< \brief 第2排第1个按键  */
#define  ZLG72128_KEY_2_2     10    /**< \brief 第2排第2个按键  */
#define  ZLG72128_KEY_2_3     11    /**< \brief 第2排第3个按键  */
#define  ZLG72128_KEY_2_4     12    /**< \brief 第2排第4个按键  */
#define  ZLG72128_KEY_2_5     13    /**< \brief 第2排第5个按键  */
#define  ZLG72128_KEY_2_6     14    /**< \brief 第2排第6个按键  */
#define  ZLG72128_KEY_2_7     15    /**< \brief 第2排第7个按键  */
#define  ZLG72128_KEY_2_8     16    /**< \brief 第2排第8个按键  */


#define  ZLG72128_KEY_3_1     17    /**< \brief 第3排第1个按键  */
#define  ZLG72128_KEY_3_2     18    /**< \brief 第3排第2个按键  */
#define  ZLG72128_KEY_3_3     19    /**< \brief 第3排第3个按键  */
#define  ZLG72128_KEY_3_4     20    /**< \brief 第3排第4个按键  */
#define  ZLG72128_KEY_3_5     21    /**< \brief 第3排第5个按键  */
#define  ZLG72128_KEY_3_6     22    /**< \brief 第3排第6个按键  */
#define  ZLG72128_KEY_3_7     23    /**< \brief 第3排第7个按键  */
#define  ZLG72128_KEY_3_8     24    /**< \brief 第3排第8个按键  */

/** @} */

/**
 * \name 功能按键（第4行所有按键）编号宏定义
 *
 *   功能按键从第4行开始，从左往右，依次定义为 F0 ~ F7，其状态分别与按键回
 * 调函数中的 funkey_val 值的 bit0 ~ bit7 对应。0位按下，1为释放。
 * 由于funkey_val是8个功能键的组合值，可以直接使用这个8位数值实现较为复杂
 * 的按键控制。若只需要简单的检测其中某个功能键的状态，可以使用
 * #ZLG72128_FUNKEY_CHECK()宏检查某一功能键的状态。
 * #ZLG72128_FUNKEY_CHECK()宏的funkey_num参数为0 ~ 7，建议使
 * 用 此处定义的宏值：#ZLG72128_FUNKEY_0 ~ #ZLG72128_FUNKEY_7
 *
 * @{
 */

#define  ZLG72128_FUNKEY_0     0     /**< \brief 功能键1  */
#define  ZLG72128_FUNKEY_1     1     /**< \brief 功能键2  */
#define  ZLG72128_FUNKEY_2     2     /**< \brief 功能键3  */
#define  ZLG72128_FUNKEY_3     3     /**< \brief 功能键4  */
#define  ZLG72128_FUNKEY_4     4     /**< \brief 功能键4  */
#define  ZLG72128_FUNKEY_5     5     /**< \brief 功能键5  */
#define  ZLG72128_FUNKEY_6     6     /**< \brief 功能键6  */
#define  ZLG72128_FUNKEY_7     7     /**< \brief 功能键7  */

/** @} */

/**
 * \name 数码管显示移位的方向
 * 用于 \sa zlg72128_digitron_shift() 函数的 \a dir 参数。
 * @{
 */
#define  ZLG72128_DIGITRON_SHIFT_RIGHT  0   /**< \brief 右移  */
#define  ZLG72128_DIGITRON_SHIFT_LEFT   1   /**< \brief 左移  */

/** @} */

/**
 * \name 功能按键（第4行所有按键）状态检测宏
 *
 *   功能按键从第4行开始，从左往右，依次定义为 F0 ~ F7，其状态分别与按键回调函数
 * 中的 funkey_val 值的 bit0 ~ bit7 对应。0位按下，1为释放。由于funkey_val是8个
 * 功能键的组合值，可以直接使用这个8位数值实现较为复杂的按键控制。若只需要简单
 * 的检测其中某个功能键的状态，可以使用该宏检查某一功能键的状态。
 *
 * \param[in] funkey_val : 按键回调函数中获取到的功能键键值
 * \param[in] funkey_num : 功能键编号，有效值：0 ~ 7，分别对应 F0 ~ F7，建议使用
 *                         宏值#ZLG72128_FUNKEY_0 ~ #ZLG72128_FUNKEY_7
 *
 * \retval 1 : 对应功能键当前是按下状态
 * \retval 0 : 对应功能键当前是释放状态
 * @{
 */

#define  ZLG72128_FUNKEY_CHECK(funkey_val, funkey_num)  \
               (((funkey_val) & (1 << ((funkey_num) & 0x07))) ? 0 : 1)

/** @} */

/**
 * \name 数码管段
 * 用于 \sa zlg72128_digitron_seg_ctrl() 段控制函数的 \a seg 参数。
 * @{
 */

#define  ZLG72128_DIGITRON_SEG_A    0        /**< \brief a  段  */
#define  ZLG72128_DIGITRON_SEG_B    1        /**< \brief b  段  */
#define  ZLG72128_DIGITRON_SEG_C    2        /**< \brief c  段  */
#define  ZLG72128_DIGITRON_SEG_D    3        /**< \brief d  段  */
#define  ZLG72128_DIGITRON_SEG_E    4        /**< \brief e  段  */
#define  ZLG72128_DIGITRON_SEG_F    5        /**< \brief f  段  */
#define  ZLG72128_DIGITRON_SEG_G    6        /**< \brief g  段  */
#define  ZLG72128_DIGITRON_SEG_DP   7        /**< \brief dp 段  */

/** @} */

/** \brief 部分接口调用ZLG72128_TRUE或ZLG72128_FLASE控制数码管属性 */
#define  ZLG72128_TRUE              1
#define  ZLG72128_FALSE             0

/**
 * \brief ZLG72128从机地址（用户可检测A4的电平来确定当前ZLG72128模块的从机地址）
 *        若引脚A4为浮空或者高电平，则从机地址为0x30
 *        若引脚A4为低电平，则从机地址为0x20
 */
#define  ZLG72128_PIN_HIGH_SLV_ADDR  0x30
#define  ZLG72128_PIN_LOW_SLV_ADDR   0x20

/**
 * \brief 定义普通按键（第1行 ~ 第3行）回调函数类型
 *
 *    可以使用 zlg72128_key_cb_set() 函数设置该回调函数至系统之中，当有按键
 * 事件发生时，调用回调函数。
 *
 * \param[in] p_arg      : 设置回调函数时设定的用户自定义参数
 *
 * \param[in] key_val    : 键值，从第1行开始，从左至右顺序编号。第1行第1个按键
 *                         键值为1，第1行第2个按键键值为2，以此类推。建议使用
 *                         该值直接与 ZLG72128_KEY_*_*(#ZLG72128_KEY_1_1)
 *                         比较，以判断是哪一按键按下。特殊地，0表示无任何普通
 *                         按键按下。
 *
 * \param[in] repeat_cnt : 按键连击次数，普通按键支持连击，该值仅用于辅助修饰
 *                         key_val0对应的普通按键的连击次数，与funkey_val表征
 *                         的功能键没有关系。0 表示首次按键，未连击。其余值表
 *                         示识别到的连击次数。当按键按住时间超过2s后，开始连
 *                         续有效，连续有效的时间间隔约200ms，即每隔200ms，
 *                         repeat_cnt加1，并调用一次该函数，直到按键释放。该值
 *                         最大为255，若连击次数大于255后，则将进入持续连击阶段，
 *                         即以没有时间间隔来调用回调函数。
 *
 * \param[in] funkey_val : 功能键键值。共计8位，分别与第4行各个按键对应，最高位
 *                         与第4行第1个按键对应，次高位与第4行第2个按键对应，最
 *                         低位与第4行第8个按键对应。位值为0表明对应键按下，位
 *                         值为1表明对应键未按下。只要该值发生改变，就会调用设置
 *                         的回调函数。
 *
 * \note 基于功能键和普通键，很容易实现组合键应用，类似PC机上的Alt、Shift等按键。
 *
 *     如经常用到的一个功能，编辑代码时，TAB键是右移，如果按住Shift键，则TAB键
 * 变为左移。可以很容易实现类似的功能，如功能键0来当做Shift键，普通按键0当做TA
 * B键。
 */
typedef void (*zlg72128_key_cb_t) (void       *p_arg,
                                   uint8_t     key_val,
                                   uint8_t     repeat_cnt,
                                   uint8_t     funkey_val);

/**
 * \brief ZLG72128设备信息结构体定义
 */
typedef struct zlg72128_devinfo {

    /** \brief 平台信息结构体*/
    zlg72128_plfm_init_info_t  plfm_info;

}zlg72128_devinfo_t;

/**
 * \brief ZLG72128设备结构体定义
 *
 * \note 因为考虑到多个平台或者用户对引脚的定义不同，并且相对来说复
 * 位引脚和中断引脚的配置是相对简单的，所以在ZLG72128的组件中并没有
 * 引脚的配置函数。ZLG72128的复位信号是低电平有效，用户将复位引脚拉
 * 低5-10ms便可以达到复位效果。同理，对于中断引脚来说也是低电平有效
 * 用户可以将中断触发方式设置为下降沿或低电平触发。用户对引脚相关的
 * 操作可以在zlg72128_platform.h中定义出来的zlg72128_plfm_init函数中
 * 进行。
 *
 */
typedef struct zlg72128_dev {

    /** \brief 保存平台相关的设备*/
    zlg72128_plfm_t     plfm;

    /** \brief 保存普通键回调函数 */
    zlg72128_key_cb_t   pfn_key_cb;

    /** \brief 保存按键回调函数参数 */
    void               *pfn_key_arg;

} zlg72128_dev_t;


/** \brief 定义ZLG72128操作句柄  */
typedef zlg72128_dev_t   *zlg72128_handle_t;

/**
 * \brief ZLG72128初始化函数
 *  调用ZLG72128其他接口前，应该首先调用该初始化函数，以获取操作
 *  ZLG72128的handle。获取到的句柄在调用大部分接口时，需传入到接
 *  口的第一个参数handle中。初始化函数内部调用了zlg72128_platform.h
 *  中定义的zlg72128_plfm_init函数来对平台相关的信息进行初始化
 *
 * \param[in] p_dev     : 指向ZLG72128设备的指针
 * \param[in] p_devinfo : 设备信息结构体，信息结构体中保存了zlg72128_platform.h
 *                        中定义的zlg72128_plfm_init_info_t结构体，内部调用
 *                        zlg72128_plfm_init时会传到第二个参数p_plfm_init_info中。
 *
 * \return 若执行成功返回操作ZLG72128的handle，
 *         若返回NULL，则参数无效或平台初始化函数执行失败。
 *
 * \note 设备指针p_dev指向的设备只需要定义，不需要在调用本函数前初始化。
 */
zlg72128_handle_t zlg72128_init (zlg72128_dev_t           *p_dev,
                                 const zlg72128_devinfo_t *p_devinfo);

/**
 * \brief 普通按键（第1行 ~ 第3行所有按键）回调函数注册
 *
 * 当有按键事件发生时，将调用此处设置的回调函数。按键事件包括以下3种情况：
 *  1. 有普通按键按下
 *  2. 普通按键一直按下，处于连击状态
 *  3. 任一功能按键状态发生变化，释放->按下 或 按下->释放
 *
 * \param[in] handle      : ZLG72128的操作句柄
 * \param[in] pfn_key_cb  : 按键回调函数
 * \param[in] p_arg       : 回调函数的参数
 *
 * \retval -2 : 参数错误
 * \retval  0 : 执行成功
 */
int zlg72128_key_cb_set (zlg72128_handle_t   handle,
                         zlg72128_key_cb_t   pfn_key_cb,
                         void               *p_arg);

/**
 * \brief 设置数码管闪烁时间，即当设定某位闪烁时点亮持续的时间和熄灭持续的时间
 *
 *     出厂设置为：亮和灭的时间均为500ms。时间单位为ms，有效的时间为：
 *  150、200、250、……、800、850、900，即 150ms ~ 900ms，且以50ms为间距。
 *  若时间不是恰好为这些值，则会设置一个最接近的值。
 *
 * \param[in] handle : ZLG72128的操作句柄
 * \param[in] on_ms  : 点亮持续的时间，有效值为 150ms ~ 900ms，且以50ms为间距
 * \param[in] off_ms : 熄灭持续的时间，有效值为 150ms ~ 900ms，且以50ms为间距
 *
 * \retval -2     : 参数错误
 * \retval  0     : 执行成功
 * \retval 其它值 : I2C写操作失败
 *
 * \note 仅设置闪烁时间并不能立即看到闪烁，还必须打开某位的闪烁开关后方能看到
 * 闪烁现象。 \sa zlg72128_digitron_flash_ctrl()
 */
int zlg72128_digitron_flash_time_cfg (zlg72128_handle_t  handle,
                                      uint16_t           on_ms,
                                      uint16_t           off_ms);

/**
 * \brief 控制（打开或关闭）数码管闪烁，默认均不闪烁
 *
 * \param[in] handle   : ZLG72128的操作句柄
 * \param[in] ctrl_val : 控制值，位0 ~ 11分别对应从左至右的数码管。值为1时闪烁，
 *                       值为0时不闪烁。
 *
 * \retval -2     : 参数错误
 * \retval  0     : 执行成功
 * \retval 其它值 : I2C写操作失败
 */
int zlg72128_digitron_flash_ctrl (zlg72128_handle_t  handle,
                                  uint16_t           ctrl_val);

/**
 * \brief 控制数码管的显示属性（打开显示或关闭显示）
 *
 *    默认情况下，所有数码管均处于打开显示状态，则将按照12位数码管扫描，实际中，
 * 可能需要显示的位数并不足12位，此时，即可使用该函数关闭某些位的显示。
 *
 * \param[in] handle   : ZLG72128的操作句柄
 * \param[in] ctrl_val : 控制值，位0 ~ 位11分别对应从左至右的数码管。
 *                       值为1关闭显示，值为0打开显示
 *
 * \retval -2     : 参数错误
 * \retval  0     : 执行成功
 * \retval 其它值 : I2C写操作失败
 *
 * \note 这与设置段码为熄灭段码不同，设置段码为熄灭段码时，数码管同样被扫描，同
 * 样需要在每次扫描时填充相应的段码。使用该函数关闭某些位的显示后，是从根本上不
 * 再扫描该位。
 */
int zlg72128_digitron_disp_ctrl (zlg72128_handle_t  handle,
                                 uint16_t           ctrl_val);

/**
 * \brief 设置数码管显示的字符
 *
 *      ZLG72128已经提供了常见的10种数字和21种字母的直接显示。若需要显示不支持的
 *  图形，可以直接使用 \sa zlg72128_digitron_dispbuf_set() 函数直接设置段码。
 *
 * \param[in] handle     : ZLG72128的操作句柄
 * \param[in] pos        : 本次显示的位置，有效值 0 ~ 11
 * \param[in] ch         : 显示的字符，支持字符 0123456789AbCdEFGHiJLopqrtUychT
 *                         以及空格(无显示)
 * \param[in] is_dp_disp : 是否显示小数点
 *                        - ZLG72128_TRUE :显示
 *                        - ZLG72128_FALSE:不显示
 * \param[in] is_flash   : 该位是否闪烁
 *                        - ZLG72128_TRUE :闪烁
 *                        - ZLG72128_FALSE:不闪烁
 *
 * \retval -2     : 参数错误
 * \retval  0     : 执行成功
 * \retval 其它值 : I2C写操作失败
 */
int zlg72128_digitron_disp_char (zlg72128_handle_t  handle,
                                 uint8_t            pos,
                                 char               ch,
                                 bool_t             is_dp_disp,
                                 bool_t             is_flash);

/**
 * \brief 从指定位置开始显示一个字符串
 *
 * \param[in] handle    : ZLG72128的操作句柄
 * \param[in] start_pos : 字符串起始位置
 * \param[in] p_str     : 显示的字符串
 *
 * \retval -2     : 参数错误
 * \retval  0     : 执行成功
 * \retval 其它值 : I2C写操作失败
 *
 * \note 若遇到不支持的字符，该字符位将不显示
 */
int zlg72128_digitron_disp_str (zlg72128_handle_t  handle,
                                uint8_t            start_pos,
                                const char        *p_str);

/**
 * \brief 设置数码管显示的数字（0 ~ 9）
 *
 * \param[in] handle     : ZLG72128的操作句柄
 * \param[in] pos        : 本次显示的位置，有效值 0 ~ 11
 * \param[in] num        : 显示的数字，0 ~ 9
 * \param[in] is_dp_disp : 是否显示小数点
 *                        - ZLG72128_TRUE :显示
 *                        - ZLG72128_FALSE:不显示
 * \param[in] is_flash   : 该位是否闪烁
 *                        - ZLG72128_TRUE :闪烁
 *                        - ZLG72128_FALSE:不闪烁
 *
 * \retval -2     : 参数错误
 * \retval  0     : 执行成功
 * \retval 其它值 : I2C写操作失败
 */
int zlg72128_digitron_disp_num (zlg72128_handle_t  handle,
                                uint8_t            pos,
                                uint8_t            num,
                                bool_t             is_dp_disp,
                                bool_t             is_flash);

/**
 * \brief 直接设置数码管显示的内容（段码）
 *
 *      当用户需要显示一些自定义的特殊图形时，可以使用该函数，直接设置段码。一
 *  般来讲，ZLG72128已经提供了常见的10种数字和21种字母的直接显示，用户不必使用
 *  此函数直接设置段码，可以直接使用 \sa 函数显示数字或字母
 *      为方便快速设置多个数码管位显示的段码，本函数可以一次连续写入多个数码管
 *  显示的段码。
 *
 * \param[in] handle    : ZLG72128的操作句柄
 * \param[in] start_pos : 本次设置段码起始位置，有效值 0 ~ 11
 * \param[in] p_buf     : 段码存放的缓冲区
 * \param[in] num       : 本次设置的数码管段码的个数
 *
 * \retval -2     : 参数错误
 * \retval  0     : 执行成功
 * \retval 其它值 : I2C写操作失败
 *
 * \note 若 start_pos + num 的值超过 12 ，则多余的缓冲区内容被丢弃。
 */
int zlg72128_digitron_dispbuf_set (zlg72128_handle_t  handle,
                                   uint8_t            start_pos,
                                   uint8_t           *p_buf,
                                   uint8_t            num);

/**
 * \brief 直接控制段的点亮或熄灭
 *
 *   为了更加灵活的控制数码管显示，提供了直接控制某一段的属性（亮或灭）。
 *
 * \param[in] handle : ZLG72128的操作句柄
 * \param[in] pos    : 控制段所处的位，有效值 0 ~ 11
 * \param[in] seg    : 段，有效值 0 ~ 7，分别对应：a,b,c,d,e,f,g,dp
 *                     建议使用下列宏：
 *                       - ZLG72128_DIGITRON_SEG_A
 *                       - ZLG72128_DIGITRON_SEG_B
 *                       - ZLG72128_DIGITRON_SEG_C
 *                       - ZLG72128_DIGITRON_SEG_D
 *                       - ZLG72128_DIGITRON_SEG_E
 *                       - ZLG72128_DIGITRON_SEG_F
 *                       - ZLG72128_DIGITRON_SEG_G
 *                       - ZLG72128_DIGITRON_SEG_DP
 *
 * \param[in] is_on  : 是否点亮该段
 *                    - ZLG72128_TRUE :点亮
 *                    - ZLG72128_FALSE:熄灭
 *
 * \retval -2     : 参数错误
 * \retval  0     : 执行成功
 * \retval 其它值 : I2C写操作失败
 */
int zlg72128_digitron_seg_ctrl (zlg72128_handle_t  handle,
                                uint8_t            pos,
                                char               seg,
                                bool_t             is_on);

/**
 * \brief 显示移位命令
 *
 *      使当前所有数码管的显示左移或右移，并可指定是否是循环移动。如果不是循环
 *   移位，则移位后，右边空出的位（左移）或左边空出的位（右移）将不显示任何内容。
 *   若是循环移动，则空出的位将会显示被移除位的内容。
 *
 * \param[in] handle    : ZLG72128的操作句柄
 * \param[in] dir       : 移位方向
 *                       - ZLG72128_DIGITRON_SHIFT_LEFT   左移
 *                       - ZLG72128_DIGITRON_SHIFT_RIGHT  右移
 * \param[in] is_cyclic : 是否循环移位
 *                       - ZLG72128_TRUE :循环移位
 *                       - ZLG72128_FALSE:不循环移位
 * \param[in] num       : 本次移位的位数，有效值 0（不移动） ~ 11，其余值无效
 *
 * \retval -2     : 参数错误
 * \retval  0     : 执行成功
 * \retval 其它值 : I2C写操作失败
 *
 * \note 关于移位的说明
 *
 *   实际中，可能会发现移位方向相反，这是由于可能存在两种硬件设计：
 *   1、 最右边为0号数码管，从左至右为：11、10、9、8、7、6、5、4、3、2、 1、 0
 *   2、 最左边为0号数码管，从左至右为： 0、 1、2、3、4、5、6、7、8、9、10、11
 *   这主要取决于硬件设计时 COM0 ~ COM11 引脚所对应数码管所处的物理位置。
 *
 *       此处左移和右移的概念是以《数据手册》中典型应用电路为参考的，即最右边
 *   为0号数码管。那么左移和右移的概念分别为：
 *   左移：数码管0显示切换到1，数码管1显示切换到2，……，数码管10显示切换到11
 *   右移：数码管11显示切换到10，数码管1显示切换到2，……，数码管10显示切换到11
 *
 *   可见，若硬件电路设计数码管位置是相反的，则移位效果可能也恰恰是相反的，此处
 * 只需要稍微注意即可。
 */
int zlg72128_digitron_shift (zlg72128_handle_t  handle,
                             uint8_t            dir,
                             bool_t             is_cyclic,
                             uint8_t            num);

/**
 * \brief 数码管扫描位数设置命令，设置数码管扫描位数
 *
 * \param[in] handle : ZLG72128的操作句柄
 * \param[in] num    : 数码管显示位数(有效值为0~12)
 *
 * \retval -2     : 参数错误
 * \retval  0     : 执行成功
 * \retval 其它值 : I2C写操作失败
 */
int zlg72128_digitron_scan_set (zlg72128_handle_t  handle,
                                uint8_t            num);
/**
 * \brief 复位命令，将数码管的所有LED段熄灭
 *
 * \param[in] handle : ZLG72128的操作句柄
 *
 * \retval -2     : 参数错误
 * \retval  0     : 执行成功
 * \retval 其它值 : I2C写操作失败
 */
int zlg72128_digitron_disp_reset (zlg72128_handle_t handle);

/**
 * \brief 测试命令，所有LED段以0.5S的速率闪烁，用于测试数码管显示是否正常
 *
 * \param[in] handle : ZLG72128的操作句柄
 *
 * \retval -2     : 参数错误
 * \retval  0     : 执行成功
 * \retval 其它值 : I2C写操作失败
 */
int zlg72128_digitron_disp_test (zlg72128_handle_t handle);

/**
 * \brief ZLG72128解初始化函数
 *
 *  当不再使用指定的ZLG72128设备时，可以使用该函数解初始化该设备，以释放相关资源
 *
 * \param[in] handle : ZLG72128的操作句柄
 *
 * \retval -2     : 参数错误
 * \retval 其它值 : 平台解初始化失败
 */
int zlg72128_deinit(zlg72128_handle_t handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __ZLG72128_H */

/* end of file */
