#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
/************************************************

************************************************/


int main(void)
{
    u8 len;	
	  u16 times=0; 
    HAL_Init();                     //初始化HAL库   
    Stm32_Clock_Init(360,25,2,8);   //设置时钟,180Mhz
    delay_init(180);                //初始化延时函数
    uart_init(115200);              //初始化USART
    LED_Init();                     //初始化LED 
    KEY_Init();                     //初始化按键

    while(1)
    {
			
			printf("T0102串口调试\r\n"); 
			
    }
	}
