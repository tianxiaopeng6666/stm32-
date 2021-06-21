#include "timer.h"
#include "led.h"

/**
 ******************************************************************************
 * @file timer.c
 * @author T0102
 *
 * @brief  ��ʼ����ʱ���Ͳ���PWM��Ϣ
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
 
/* ��ʱ��3PWM��� */
TIM_HandleTypeDef TIM3_Handler;

/* ��ʱ��3ͨ��4��� */
TIM_OC_InitTypeDef TIM3_CH4Handler;

/* ��ʱ����� */
TIM_HandleTypeDef TIM7_Handler;

extern vu16 USART3_RX_STA;

/**
* @brief TIM3 PWM���ֳ�ʼ��
*       
* @note  arr���Զ���װֵ
*        psc��ʱ��Ԥ��Ƶ��
*/
void TIM3_PWM_Init(u16 arr,u16 psc)
{ 
    TIM3_Handler.Instance=TIM3;                                             /*��ʱ��3*/
    TIM3_Handler.Init.Prescaler=psc;                                        /*��ʱ����Ƶ*/
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;                       /*���ϼ���ģʽ*/
    TIM3_Handler.Init.Period=arr;                                           /*�Զ���װ��ֵ*/
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM3_Handler);                                        /*��ʼ��PWM*/
    
    TIM3_CH4Handler.OCMode=TIM_OCMODE_PWM1;                                 /*ģʽѡ��PWM1*/
    TIM3_CH4Handler.Pulse=arr/2;                                            /*���ñȽ�ֵ,��ֵ����ȷ��ռ�ձ�*/
                                                                            /*Ĭ�ϱȽ�ֵΪ�Զ���װ��ֵ��һ��,��ռ�ձ�Ϊ50%*/
    TIM3_CH4Handler.OCPolarity=TIM_OCPOLARITY_LOW;                          /*����Ƚϼ���Ϊ�� */
    HAL_TIM_PWM_ConfigChannel(&TIM3_Handler,&TIM3_CH4Handler,TIM_CHANNEL_4);/*����TIM3ͨ��4*/
    HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_4);                         /*����PWMͨ��4*/
}

/**
* @brief TIM7 PWM���ֳ�ʼ��
*       
* @note  arr���Զ���װֵ
*        psc��ʱ��Ԥ��Ƶ��
*/
void TIM7_Int_Init(u16 arr,u16 psc)
{
		TIM7_Handler.Instance=TIM7;                            /*ͨ�ö�ʱ��3 */
    TIM7_Handler.Init.Prescaler=psc;                       /*��Ƶϵ��    */
    TIM7_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;      /*���ϼ�����  */
    TIM7_Handler.Init.Period=arr;                          /*�Զ�װ��ֵ  */
    TIM7_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;/*ʱ�ӷ�Ƶ����*/
    HAL_TIM_Base_Init(&TIM7_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM7_Handler);   /*ʹ�ܶ�ʱ��3�Ͷ�ʱ��3�����жϣ�TIM_IT_UPDATE*/
}
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM7)
    {
    		__HAL_RCC_TIM7_CLK_ENABLE();            /*ʹ��TIM7ʱ��*/
	    	HAL_NVIC_SetPriority(TIM7_IRQn,0,1);    /*�����ж����ȼ�����ռ���ȼ�0�������ȼ�1*/
	    	HAL_NVIC_EnableIRQ(TIM7_IRQn);          /*����ITM7�ж�*/
	  }
}
void TIM7_IRQHandler(void)
{ 	    		    
		USART3_RX_STA|=1<<15;	/*��ǽ������*/
		__HAL_TIM_CLEAR_FLAG(&TIM7_Handler,TIM_EventSource_Update );/*���TIM7�����жϱ�־*/
		TIM7->CR1&=~(1<<0);   /*�رն�ʱ��7*/
} 

/**
* @brief ��ʱ���ײ�������ʱ��ʹ�ܣ���������
*       
* @note  htim:��ʱ�����  �˺����ᱻHAL_TIM_PWM_Init()����
*/
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;
		
	  __HAL_RCC_TIM3_CLK_ENABLE();		     	/*ʹ�ܶ�ʱ��3*/
    __HAL_RCC_GPIOB_CLK_ENABLE();			    /*����GPIOBʱ��*/
	
    GPIO_Initure.Pin=GPIO_PIN_1;          /*PB1*/
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;    /*�����������*/
    GPIO_Initure.Pull=GPIO_PULLUP;        /*����*/
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;   /*����*/
  	GPIO_Initure.Alternate= GPIO_AF2_TIM3;/*PB1����ΪTIM3_CH4*/
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
}

/**
* @brief ����TIMͨ��4��ռ�ձ�
*       
* @note  compare:�Ƚ�ֵ
*/
void TIM_SetTIM3Compare4(u32 compare)
{
	TIM3->CCR4=compare;
}

/**
* @brief ��ȡTIM����/�ȽϼĴ���ֵ
*/
u32 TIM_GetTIM3Capture4(void)
{
	return  HAL_TIM_ReadCapturedValue(&TIM3_Handler,TIM_CHANNEL_4);
}


/*���������벶�����Դ��ʵ����غ���Դ��*/

/*��ʱ��5���*/
TIM_HandleTypeDef TIM5_Handler;

/**
* @brief ��ʱ��5ͨ��1���벶������
*       
* @note  arr���Զ���װֵ(TIM2,TIM5��32λ��!!)
*        psc��ʱ��Ԥ��Ƶ��
*/
void TIM5_CH3_Cap_Init(u32 arr,u16 psc)
{  
    TIM_IC_InitTypeDef TIM5_CH3Config;  
    
    TIM5_Handler.Instance=TIM5;                            /*ͨ�ö�ʱ��5*/
    TIM5_Handler.Init.Prescaler=psc;                       /*��Ƶϵ��*/
    TIM5_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;      /*���ϼ�����*/
    TIM5_Handler.Init.Period=arr;                          /*�Զ�װ��ֵ*/
    TIM5_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;/*ʱ�ӷ�Ƶ����*/
    HAL_TIM_IC_Init(&TIM5_Handler);                        /*��ʼ�����벶��ʱ������*/
    
    TIM5_CH3Config.ICPolarity=TIM_ICPOLARITY_RISING;       /*�����ز���*/
    TIM5_CH3Config.ICSelection=TIM_ICSELECTION_DIRECTTI;   /*ӳ�䵽TI1��*/
    TIM5_CH3Config.ICPrescaler=TIM_ICPSC_DIV1;             /*���������Ƶ������Ƶ*/
    TIM5_CH3Config.ICFilter=0;                             /*���������˲��������˲�*/
    HAL_TIM_IC_ConfigChannel(&TIM5_Handler,&TIM5_CH3Config,TIM_CHANNEL_3);/*����TIM5ͨ��1*/
	
    HAL_TIM_IC_Start_IT(&TIM5_Handler,TIM_CHANNEL_3);      /*����TIM5�Ĳ���ͨ��1�����ҿ��������ж�*/
    __HAL_TIM_ENABLE_IT(&TIM5_Handler,TIM_IT_UPDATE);      /*ʹ�ܸ����ж�*/
}

/**
* @brief ��ʱ��5�ײ�������ʱ��ʹ�ܣ���������
*       
* @note  htim:��ʱ��5���  �˺����ᱻHAL_TIM_IC_Init()����
*/
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_TIM5_CLK_ENABLE();            /*ʹ��TIM5ʱ��*/
    __HAL_RCC_GPIOA_CLK_ENABLE();			      /*����GPIOAʱ��*/
	
    GPIO_Initure.Pin=GPIO_PIN_2;            /*PA0*/
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;      /*�����������*/
    GPIO_Initure.Pull=GPIO_PULLDOWN;        /*����*/
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     /*����*/
    GPIO_Initure.Alternate=GPIO_AF2_TIM5;   /*PA0����ΪTIM5ͨ��1*/
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);

    HAL_NVIC_SetPriority(TIM5_IRQn,2,0);    /*�����ж����ȼ�����ռ���ȼ�2�������ȼ�0*/
    HAL_NVIC_EnableIRQ(TIM5_IRQn);          /*����ITM5�ж�ͨ��*/
}

/*����״̬*/
/*[7]:0,û�гɹ��Ĳ���;1,�ɹ�����һ��.*/
/*[6]:0,��û���񵽵͵�ƽ;1,�Ѿ����񵽵͵�ƽ��.*/
/*[5:0]:����͵�ƽ������Ĵ���(����32λ��ʱ����˵,1us��������1,���ʱ��:4294��)*/
/*���벶��״̬*/
u8  TIM5CH3_CAPTURE_STA=0;

/*���벶��ֵ(TIM2/TIM5��32λ)*/
u32	TIM5CH3_CAPTURE_VAL;

/**
* @brief ��ʱ��5�жϷ�����
*/
void TIM5_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM5_Handler);/*��ʱ�����ô�����*/
}
 

/**
* @brief ��ʱ�������жϣ�����������жϴ���ص�����
*       
* @note  �����жϣ����������ʱִ�У��Ҹú�����HAL_TIM_IRQHandler�лᱻ����
*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//
{
	
	if((TIM5CH3_CAPTURE_STA&0X80)==0)         /*��δ�ɹ�����*/
	{
			if(TIM5CH3_CAPTURE_STA&0X40)          /*�Ѿ����񵽸ߵ�ƽ��*/
			{
				if((TIM5CH3_CAPTURE_STA&0X3F)==0X3F)/*�ߵ�ƽ̫����*/
				{
					TIM5CH3_CAPTURE_STA|=0X80;		    /*��ǳɹ�������һ��*/
					TIM5CH3_CAPTURE_VAL=0XFFFFFFFF;
				}else TIM5CH3_CAPTURE_STA++;
			}	 
	}		
}

/**
* @brief ��ʱ�����벶���жϴ���ص�����
*       
* @note  �����жϷ���ʱִ�У��Ҹú�����HAL_TIM_IRQHandler�лᱻ����
*/
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if((TIM5CH3_CAPTURE_STA&0X80)==0)   /*��δ�ɹ�����*/
	{
		if(TIM5CH3_CAPTURE_STA&0X40)		  /*����һ���½���*/
		{	  			
		    TIM5CH3_CAPTURE_STA|=0X80;		/*��ǳɹ�����һ�θߵ�ƽ����*/
        TIM5CH3_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM5_Handler,TIM_CHANNEL_3);/*��ȡ��ǰ�Ĳ���ֵ.*/
        TIM_RESET_CAPTUREPOLARITY(&TIM5_Handler,TIM_CHANNEL_3);                    /*���ԭ��������*/
        TIM_SET_CAPTUREPOLARITY(&TIM5_Handler,TIM_CHANNEL_3,TIM_ICPOLARITY_RISING);/*����TIM5ͨ��1�����ز���*/
		}else  								          /*��δ��ʼ,��һ�β���������*/
		{
				TIM5CH3_CAPTURE_STA=0;			/*���*/
				TIM5CH3_CAPTURE_VAL=0;
				TIM5CH3_CAPTURE_STA|=0X40;	/*��ǲ�����������*/
				__HAL_TIM_DISABLE(&TIM5_Handler);        /*�رն�ʱ��5*/
				__HAL_TIM_SET_COUNTER(&TIM5_Handler,0);
				TIM_RESET_CAPTUREPOLARITY(&TIM5_Handler,TIM_CHANNEL_3);   /*һ��Ҫ�����ԭ�������ã���*/
				TIM_SET_CAPTUREPOLARITY(&TIM5_Handler,TIM_CHANNEL_3,TIM_ICPOLARITY_FALLING);/*��ʱ��5ͨ��1����Ϊ�½��ز���*/
				__HAL_TIM_ENABLE(&TIM5_Handler);/*ʹ�ܶ�ʱ��5*/
			}		    
	}		
	
}

