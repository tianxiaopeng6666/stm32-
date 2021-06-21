#include "led.h"

   
//LED IO初始化
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOC_CLK_ENABLE();           	//开启GPIOC时钟
	
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|
	                 GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;//PC0~7
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          	//上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;    	 	//高速
	
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|
	GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7,GPIO_PIN_SET);	//PC0~7置1，默认初始化后灯灭
	
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
	
}


