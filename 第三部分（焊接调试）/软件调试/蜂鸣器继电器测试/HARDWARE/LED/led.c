#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
  
////////////////////////////////////////////////////////////////////////////////// 	

//初始化PE12 15为输出.并使能时钟	    
//蜂鸣器 继电器 IO初始化
void BeepJDQ_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOE_CLK_ENABLE();           //开启GPIOE时钟
	
    GPIO_Initure.Pin=GPIO_PIN_15|GPIO_PIN_12; 
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);
	
    HAL_GPIO_WritePin(GPIOE,GPIO_PIN_15|GPIO_PIN_12,GPIO_PIN_RESET);	//PE12置0 蜂鸣器 继电器默认关闭
 
}
