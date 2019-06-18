#include "zsn60x.h"  
#include "ametal.h"
static int a = 0;

/* 检测到卡片回调函数 */
void __card_input(void *p_arg){
    a = 1;
}

/* ZSN60x  自动检测测试程序 */
void demo_zsn60x_auto_detect_test_entry(zsn60x_handle_t handle)
{
    uint8_t ret;
    /* 使用默认密钥 */
    uint8_t data[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    zsn60x_auto_detect_ctrl_t  auto_ctrl;
    zsn60x_auto_detect_data_t  auto_data;

    //自动检测模式配置
    auto_ctrl.ad_mode        = ZSN60X_AUTO_DETECT_CONTINUE | ZSN60X_AUTO_DETECT_INTERRUPT |
                               ZSN60X_AUTO_DETECT_SEND;
    auto_ctrl.tx_mode        = ZSN60X_ANT_MODE_TX12;
    auto_ctrl.req_code       = ZSN60X_MIFARE_REQUEST_IDLE;
    auto_ctrl.auth_mode      = ZSN60X_AUTO_DETECT_KEY_AUTH;
    auto_ctrl.key_type       = ZSN60X_ICC_KEY_TYPE_A;
    auto_ctrl.p_key          = data;
    auto_ctrl.key_len        = 6;
    auto_ctrl.block          = 4;
    auto_ctrl.pfn_card_input = __card_input;
    auto_ctrl.p_arg          = NULL;

    ret = zsn60x_auto_detect(handle, &auto_ctrl);
    if(ret == 0){
        am_kprintf("Entry auto detect card mode success!\r\n");
    }else{
        am_kprintf("Entry auto detect card mode fail beacuse error 0x%02x!\r\n", ret);
        return ;
    }

    /* 等待回调函数触发 */
    while(a == 0);
    ret = zsn60x_get_auto_detect(handle, 0, &auto_data);
    a = 0;

    if(ret == 0){
        am_kprintf("Auto detect card success!\r\n");
    }else{
        am_kprintf("Auto detect card fail beacuse error 0x%2x!\r\n", ret);
    }
}  
