/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2017 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief LPCD模式  配合系统超低功耗停止模式  在超低功耗情况下实现卡片检测功能
 *      在此例程中，KS16将会进入VLPS模式，即超低功耗停止模式。
 *      在FM175xx进入LPCD模式之后，MUC进入VLPS模式，来降低MCU自身功耗
 *
 * - 操作步骤：
 *   1. 正确连接并配置好串口。
 *   2. 正确连接好天线，复位，即会打印相关提示信息，用户可以根据提示信息来判断是否进入LPCD模式成功
 *   3. 进入LPCD模式成功后，将会进行读卡操作，并打印卡片相关信息。
 *
 * - 实验现象：
 *   1. 串口打印出卡片类型号和卡号及卡片信息
 *
 * \par 源代码
 * \snippet demo_amks16rfid_dr_fm175xx_lpcd_vlps_read_id.c src_amks16rfid_dr_fm175xx_lpcd_vlps_read_id
 *
 * \internal
 * \par Modification history
 * - 1.00 19-07-012  htf, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_amks16rfid_core_dr_fm175xx_lpcd_vlps_read_id
 * \copydoc demo_amks16rfid_core_dr_fm175xx_lpcd_vlps_read_id.c
 */

/** [src_amks16rfid_dr_fm175xx_lpcd_vlps_read_id] */

#include "am_fm175xx.h"
#include "am_vdebug.h"
#include "am_fm175xx_reg.h"
#include "am_kl26_pmu.h"
#include "am_hwconf_fm175xx.h"
#include "demo_components_entries.h"
#include "demo_amks16rfid_entries.h"
/**
 * \brief 触发卡进场回调函数  用户可自己任意配置   本例程以读取ID为例
 */
static int8_t  picca_active_info(am_fm175xx_handle_t handle)
{
    uint8_t      tag_type[2]  = { 0 };      /* ATQA */
    uint8_t      uid[10]      = { 0 };      /* UID */
    uint8_t      uid_real_len =   0;        /* 接收到的UID的长度 */
    uint8_t      sak[3]       = { 0 };      /* SAK */
    uint8_t      i;

    /* 卡激活操作  */
    if (AM_FM175XX_STATUS_SUCCESS == am_fm175xx_picca_active\
                                              (handle,
                                               AM_FM175XX_PICCA_REQ_ALL,
                                               tag_type,
                                               uid,
                                               &uid_real_len,
                                               sak)) {
        am_kprintf("ATQA :%02x %02x\n", tag_type[0], tag_type[1]);

        /* 打印UID */
        am_kprintf("UID  :");
        for (i = 0; i < uid_real_len; i++) {
            am_kprintf("%02x ", uid[i]);
        }
        am_kprintf("\n");

        /* 打印SAK */
        am_kprintf("SAK  :");
        for (i = 4; i <= uid_real_len; i+=3) {
            am_kprintf("%02x ", sak[(i - 4) / 3]);
        }
        am_kprintf("\n\n");
        return AM_OK;
    }else{
        return AM_ERROR;
    }
}

/**
 * \brief 用户回调函数
 *
 *  触发中断之后将会自动退出LPCD模式  再次进入用户需调用am_fm175xx_lpcd_mode_entry
 *  重新进入LPCD模式
 */
static void  __fm175xx_lpcd_cb(void *p_arg)
{
    int *p_flag = (int *)p_arg;
    *p_flag =1;
}

/**
 * \brief A类LPCD模式 卡读卡类型和卡号例程
 */
void demo_amks16rfid_dr_fm175xx_lpcd_vlps_read_id (void)
{
    int int_flag = 0;
    am_fm175xx_handle_t handle = am_fm175xx_inst_init();


    am_kprintf("FM175xx LPCD mode test!\n");

    /* 设置回调函数及函数变量
     * 注意，若使用INT SPI 中断回调函数中，不能与FM175xx进行通信。
     * 否则将会发生同优先级中断嵌套，导致程序一直在中断中等待中断发生
     */
    am_fm175xx_lpcd_cb_set(handle, __fm175xx_lpcd_cb, &int_flag);

    /* 进入LPCD模式 */
    am_fm175xx_lpcd_mode_entry(handle);

    am_kprintf("Enter VLPS_MODE.\n");
    if(am_kl26_pmu_mode_into(AM_KL26_PMU_MODE_VLPS) != AM_OK) {
        am_kprintf("Enter VLPS_MODE Failed !\r\n");
    } else {
        am_kprintf("Wake Up from VLPS_MODE !\r\n");
    }
    while(1){
        uint8_t isr;

         /* 触发卡进场回调函数
          * 注意，LPCD模式只会触发两种中断，卡进场中断以及自动唤醒中断
          */
        if(int_flag == 1){

            int_flag = 0;

            isr = am_fm175xx_get_reg_ext(handle, AM_FM175XX_LPCD_IRQ);

            if((isr & AM_FM175XX_LPCD_CARD_IRQ) == AM_FM175XX_LPCD_CARD_IRQ){

                /* 退出LPCD模式需要对部分FM175xx配置重新进行设置 */
                am_fm175xx_exit_lpcd_config(handle);

                /*可在此进行相应的卡操作*/
                picca_active_info(handle);

                /* 触发卡定时自动唤醒中断 */
            }else if((isr & AM_FM175XX_LPCD_WUP_IRQ) == AM_FM175XX_LPCD_WUP_IRQ){
                am_fm175xx_lpcd_init(handle);
            }
            /* 再次进入LPCD模式 */
            am_fm175xx_lpcd_mode_entry(handle);

            am_kprintf("Enter VLPS_MODE.\n");
            if(am_kl26_pmu_mode_into(AM_KL26_PMU_MODE_VLPS) != AM_OK) {
                am_kprintf("Enter VLPS_MODE Failed !\r\n");
            } else {
                am_kprintf("Wake Up from VLPS_MODE !\r\n");
            }
        }
    }
}

/** [demo_amks16rfid_core_dr_fm175xx_lpcd_vlps_read_id] */

/* end of file */



