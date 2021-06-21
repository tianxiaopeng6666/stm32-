#include "BeepJdq.h"

/**
 ******************************************************************************
 * @file BeepJdq.c
 * @author T0102
 *
 * @brief  �������ͼ̵�����ʹ�ܳ�ʼ��
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
* @brief �������ͼ̵�����ʹ�ܳ�ʼ��
*       
* @note  ��ʼ��PE12 15Ϊ���.��ʹ��ʱ��
*        ������ �̵��� IO��ʼ�� 
*/
void BeepJDQ_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOE_CLK_ENABLE();           /*����GPIOEʱ��*/
	
    GPIO_Initure.Pin=GPIO_PIN_15|GPIO_PIN_12; 
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  /*�������*/
    GPIO_Initure.Pull=GPIO_PULLUP;          /*����*/
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     /*����*/
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);     /*��ʼ��PE12 15Ϊ���*/  
	
    HAL_GPIO_WritePin(GPIOE,GPIO_PIN_15|GPIO_PIN_12,GPIO_PIN_RESET);	/*PE12��0 ������ �̵���Ĭ�Ϲر�*/
 
}
