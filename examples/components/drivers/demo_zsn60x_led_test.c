#include "zsn60x.h"
#include "ametal.h"
#include "am_vdebug.h"
/* ZSN60x  LED≤‚ ‘≥Ã–Ú */
void demo_zsn60x_led_test_entry (zsn60x_handle_t handle)
{
    unsigned char ret = 0;
    ret = zsn60x_control_led (handle,  ZSN60X_CONTROL_LED_ON);
    if(ret == 0){
        am_kprintf("led on !\r\n");
    }else{
        am_kprintf("led control beacuse error 0x%02x", ret);
    }
    ret = zsn60x_control_led (handle,  ZSN60X_CONTROL_LED_OFF);
    if(ret == 0){
        am_kprintf("led off !\r\n");
    }else{
        am_kprintf("led control beacuse error 0x%02x", ret);
    }
}
