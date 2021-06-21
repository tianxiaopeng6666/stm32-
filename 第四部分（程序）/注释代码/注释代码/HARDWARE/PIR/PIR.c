#include "PIR.h"

/**
 ******************************************************************************
 * @file PIR.c
 * @author T0102
 *
 * @brief  热释电传感器初始化
 ******************************************************************************
 *@note
 *
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 ZJUT.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ZJUT under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. 
 ******************************************************************************
 */

/**
 * @brief PIR初始化函数
 *       
 * @note 配置GPIO_PIN_13
 */
void PIR_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOE_CLK_ENABLE();           /*开启GPIOE时钟*/
   
    
    GPIO_Initure.Pin=GPIO_PIN_13;           /*PE0*/
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      /*输入*/
    GPIO_Initure.Pull=GPIO_PULLDOWN;        /*下拉*/
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     /*高速*/
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);
    

}

/**
 * @brief 返回主函数值
 *       
 * @note return 热释电值
 */
u8 PIR_Scan(void)
{
  	if(PIR==1)       return 1;
    else if(PIR==0)  return 0;          
		return 0;
}



