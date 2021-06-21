#ifndef __PWM_H
#define __PWM_H
#include "sys.h"

extern TIM_HandleTypeDef TIM3_Handler;      //��ʱ��3PWM��� 
extern TIM_OC_InitTypeDef TIM3_CH4Handler;  //��ʱ��3ͨ��4���
extern TIM_HandleTypeDef TIM5_Handler;      //��ʱ��5���

void TIM3_Init(u16 arr,u16 psc);
void TIM3_PWM_Init(u16 arr,u16 psc);
void TIM_SetTIM3Compare4(u32 compare);
u32 TIM_GetTIM3Capture4(void);
void TIM5_CH3_Cap_Init(u32 arr,u16 psc);
#endif

