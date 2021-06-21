#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "beep.h" 



int main(void)
{
	
    HAL_Init();                    	//初始化HAL库    
    Stm32_Clock_Init(96,4,2,4);     //设置时钟,96Mhz
    delay_init(96);                 //初始化延时函数
    LED_Init();                     //初始化LED 
	  BEEP_Init();                    //初始化蜂鸣器端口
	while(1)
	{
        		
	      HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET); //LED0=0;
        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);   //BEEP=1;
        delay_ms(300);										//延时500ms
        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET);   //LED0=1;
        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET); //BEEP=0;
        delay_ms(300);       
		
	}
}

