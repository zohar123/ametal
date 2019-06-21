#include "zsn60x.h"
#include "ametal.h"
/* ZSN60x  B类卡激活测试程序 */
void demo_zsn60x_piccb_active_test_entry (zsn60x_handle_t handle)
{
    unsigned char  info[12] = {0};
    unsigned char  ret = 0;
    /* 在使用B卡相关的函数前，需设置协议为B类卡 */
    ret = zsn60x_set_ios_type (handle, ZSN60X_ICC_ISO_TYPE_B);
    if(ret  !=  0 ){
        am_kprintf("ios set fail beacuse %0x2", ret);
        return;
    }
    //B卡请求接口函数  请求模式为 0x00  IDLE
    ret = zsn60x_piccb_active(handle, 0x00, info);
    if(ret == 0){
        unsigned char i = 0;
        am_kprintf("CARD INFO  is :");
        for(i = 0; i < 12 ; i ++ ){
            am_kprintf("%02x ",  info[i]);
       }
        am_kprintf("\r\n");
    }else{
        am_kprintf("active fail beacuse error 0x%02x", ret);
    }
}
