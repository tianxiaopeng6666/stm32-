#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"


int main(void)
{
	
    HAL_Init();                    	//初始化HAL库    
    Stm32_Clock_Init(96,4,2,4);     //设置时钟,96Mhz
    delay_init(96);                 //初始化延时函数
    LED_Init();                     //初始化LED 
	while(1)
	{
        		
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET);//LED0亮
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET);//LED1灭
		delay_ms(500);                                  //延时500ms
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_SET);//LED0灭
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);//LED1亮
		delay_ms(500);                                   //延时500ms
		
	}
}


