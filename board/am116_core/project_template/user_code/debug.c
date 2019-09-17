/******************************************************************************
* @file debug.c
* @author Zohar_Lee
* @version
* @date:2019.06.13
* @brief
******************************************************************************/
#include "stdio.h"
#include "debug.h"
// #include "string.h"

/******************************************************************************
* 功能说明: 打印float类型的数，因为SEGGER_RTT_printf不支持float,故写此函数
* 输入: buf1:自定义的字符数组，如“a = ”
*       temp:要打印的float变量
* 输出: 无
* 返回：无
* 注意：无
******************************************************************************/
void debug_float(const char buf1[], float temp)
{
    char buf[20] = {0};

    sprintf((char *)buf, "%g", temp);
    debug("%s", buf1);
    debug("%s\r\n", buf);
}

/******************************************************************************
* 功能说明: 按16进制打印指定数组的指定长度
* 输入: 无
* 输出: 无
* 返回：无
* 注意：无
******************************************************************************/
void vPrintfBufInHex(uint8_t uc_buffer[], uint32_t ul_length_of_buffer)
{
    for (uint32_t i = 0; i < ul_length_of_buffer; i++)
        debug("%02x ", uc_buffer[i]);
    debug("\r\n");
}

void vPrintfBufInChar(uint8_t uc_buffer[], uint32_t ul_length_of_buffer)
{
    for (uint32_t i = 0; i < ul_length_of_buffer; i++)
        debug("%c", uc_buffer[i]);
    debug("\r\n");
}

void vPrintfBufInHexWithUart0(uint8_t uc_buffer[], uint32_t ul_length_of_buffer)
{
    for (uint32_t i = 0; i < ul_length_of_buffer; i++)
        printf("%02x ", uc_buffer[i]);
    printf("\r\n");
}
