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
 * \brief ETH例程，通过HW层接口实现
 * - 操作步骤：
 *   1.使用本例程之前，需要下载移植LWIP协议栈(ametal/3rdparty)
 *   2.Demo已实现tcp_server & tcp_client，使用宏开关激活功能
 *   3.用户可修改配置文件，对管脚，IP地址, port端口, 速率，双工信息等进行配置
 *      (amhw_zlg_usr_conf.c), 推荐使用默认配置，驱动将自动识别PHY功能
 * - 实验现象
 *   1.使用tcp_server，使用网络测试工具创建客户端连接后，可进行loopback.
 *   2.使用tcp_client，使用网络测试工具创建服务器端连接后，可进行loopback.
 *
 *\note
 *   1.为标识ETH程序正在工作，点亮LED1，实际使用可以去掉
 *
 * \internal
 * \par Modification History
 * - 1.00 19-06-11  wyy, first implementation
 * \endinternal
 */

/***************************************************************************
 includes
 ***************************************************************************/
#include "demo_zlg_entries.h"

void demo_zmf159_core_hw_eth_entry(void)
{
    printf("demo_zmf159_core_hw_eth_entry!\r\n");
    demo_zlg_hw_eth_entry();
}

