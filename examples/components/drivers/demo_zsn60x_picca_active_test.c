#include "zsn60x.h"
#include "ametal.h"
/* ZSN60x  A类卡激活测试程序 */
void demo_zsn60x_picca_active_test_entry (zsn60x_handle_t handle)
{
    unsigned char  atq[2] = {0};
    unsigned char  saq = 0;
    unsigned char  len = 0;
    unsigned char  uid[10] = {0};
    unsigned char  ret = 0;
    //A卡请求接口函数  请求模式为 0x26  IDLE
    ret = zsn60x_picca_active(handle, 0x26, atq, &saq, &len, uid);
    if(ret == 0){
        unsigned char i = 0;
        am_kprintf("ATQ  is : %02x  %02x\r\n", atq[0], atq[1]);
        am_kprintf("SAQ  is : %02x \r\n", saq);
        am_kprintf("UID is :");
        for(i = 0; i < len ; i ++ ){
            am_kprintf("%02x ",  uid[i]);
        }
        am_kprintf("\r\n");
    }else{
        am_kprintf("active fail beacuse error 0x%02x", ret);
    }
}
