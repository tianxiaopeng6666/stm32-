#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"

//PIR PE13

int main(void)
{
    u8 key;
    HAL_Init();                     //初始化HAL库   
    Stm32_Clock_Init(360,25,2,8);   //设置时钟,180Mhz
    delay_init(180);                //初始化延时函数
    uart_init(115200);              //初始化USART
    LED_Init();                     //初始化LED 
    PIR_Init();                     //初始化按键
	
    while(1)
    {
        key=PIR_Scan();            //按键扫描
		    if (key) printf("检测到人\r\n");
			  else printf("没检测到人\r\n");
        delay_ms(1000);
    }
}

