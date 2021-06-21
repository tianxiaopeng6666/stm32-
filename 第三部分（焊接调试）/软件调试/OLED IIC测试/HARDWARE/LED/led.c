#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
  
////////////////////////////////////////////////////////////////////////////////// 	

//初始化PB1为输出.并使能时钟	    
//LED IO初始化
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOD_CLK_ENABLE();           //开启GPIOD时钟
	
    GPIO_Initure.Pin=GPIO_PIN_13; 
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);
	
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_SET);	//PD15置1，默认初始化后灯灭
 
}
