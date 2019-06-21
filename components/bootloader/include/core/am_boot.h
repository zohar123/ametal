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
 * \brief bootloader 通用基本标准接口
 *
 * \internal
 * \par Modification History
 * - 1.00 18-11-13  yrh, first implementation.
 * \endinternal
 */
 
 #ifndef __AM_BOOT_H
 #define __AM_BOOT_H
 
#ifdef __cplusplus
extern "C" {
#endif
#include "ametal.h"
#include "am_boot_memory.h"

struct am_boot_drv_funcs{
    /**< \brief 检查应用程序是否有效 */
    am_bool_t (*pfn_app_is_ready)(void);
    /**< \brief 系统重启 */
    void      (*pfn_reset)(void);
    /**< \brief 跳转进入应用程序 */
    int       (*pfn_go_application)(void);
    /**< \brief 获取应用程序内存存放的起始地址 */
    int       (*pfn_get_app_sta_addr)(void);
};

typedef struct am_boot_serv {
    struct am_boot_drv_funcs *p_funcs;
    void                     *drv;
}am_boot_serv_t;



/**
 * \brief 判断应用代码是否有效
 * 
 * \retval AM_TRUE : 有效
 * \retval AM_TRUE : 无效
 */ 
am_bool_t am_boot_app_is_ready(void);

/**
 * \brief 跳转到应用代码代码
 *
 * \retval AM_ERROR 跳转出错或跳转目的地址有误
 */
int am_boot_go_application(void);

/**
 * \brief 系统软件重启
 *
 * \retval 无
 */
void am_boot_reset(void);

/**
 * \brief 获取应用代码存放的起始地址
 *
 * \retval > 0 返回应用代码存放的起始地址
 *         < 0 无法获取地址
 */
int am_boot_get_app_addr(void);


void am_boot_init(am_boot_serv_t *p_dev);

#ifdef __cplusplus
}
#endif

 #endif /* __AM_BOOT_H */
 
 /* end of file */
