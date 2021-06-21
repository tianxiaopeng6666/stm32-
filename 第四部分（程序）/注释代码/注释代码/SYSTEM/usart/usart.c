#include "usart.h"

/**
 ******************************************************************************
 * @file usart.c
 * @author T0102
 *
 * @brief  ���ڳ�ʼ���Ͷ���⺯��
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
 
/*���ʹ��os,����������ͷ�ļ�����.*/
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//os ʹ��	  
#endif

/*��׼����Ҫ��֧�ֺ���*/
#if 1
#pragma import(__use_no_semihosting)             
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;
/*����_sys_exit()�Ա���ʹ�ð�����ģʽ*/
void _sys_exit(int x) 
{ 
	x = x; 
} 

/*�ض���fputc����*/
int fputc(int ch, FILE *f)
{ 	
	while((UART7->SR&0X40)==0);/*ѭ������,ֱ���������*/
	UART7->DR = (u8) ch;      
	return ch;
}
#endif 

/*����1�жϷ������*/
/*���ջ���,���USART_REC_LEN���ֽ�.*/
u8 USART_RX_BUF[USART_REC_LEN];

/*����״̬
* bit15��	������ɱ�־
* bit14��	���յ�0x0d
* bit13~0,���յ�����Ч�ֽ���Ŀ
*/
u16 USART_RX_STA=0;

/*���ڷ��ͻ�����*/
/*���ͻ���,���USART3_MAX_SEND_LEN�ֽ�*/
__align(8) u8 USART3_TX_BUF[USART3_MAX_SEND_LEN];

/*���ڽ��ջ�����*/
/*���ջ���,���USART3_MAX_RECV_LEN���ֽ�.*/
u8 USART3_RX_BUF[USART3_MAX_RECV_LEN];


/*ͨ���жϽ�������2���ַ�֮���ʱ������100ms�������ǲ���һ������������.
* ���2���ַ����ռ������100ms,����Ϊ����1����������.Ҳ���ǳ���100msû�н��յ�
* �κ�����,���ʾ�˴ν������.
* ���յ�������״̬
* [15]:0,û�н��յ�����;1,���յ���һ������.
* [14:0]:���յ������ݳ���
*/
u16 USART3_RX_STA=0;

/*UART���*/
UART_HandleTypeDef UART7_Handler; 
UART_HandleTypeDef UART3_Handler;

/**
* @brief ��ʼ��IO ����1
*       
* @note bound:������
*/
void uart_init(u32 bound)
{	
	/*UART ��ʼ������*/
	UART7_Handler.Instance=UART7;					            /*UART7*/
	UART7_Handler.Init.BaudRate=bound;				        /*������*/
	UART7_Handler.Init.WordLength=UART_WORDLENGTH_8B; /*�ֳ�Ϊ8λ���ݸ�ʽ*/
	UART7_Handler.Init.StopBits=UART_STOPBITS_1;	    /*һ��ֹͣλ*/
	UART7_Handler.Init.Parity=UART_PARITY_NONE;		    /*����żУ��λ*/
	UART7_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE; /*��Ӳ������*/
	UART7_Handler.Init.Mode=UART_MODE_TX_RX;		      /*�շ�ģʽ*/
	HAL_UART_Init(&UART7_Handler);					          /*HAL_UART_Init()��ʹ��UART7*/
}

/**
* @brief ��ʼ��IO ����3
*       
* @note bound:������
*/
void usart3_init(u32 bound)
{	
	/*UART ��ʼ������*/
	UART3_Handler.Instance=USART3;					          /*UART7*/
	UART3_Handler.Init.BaudRate=bound;				        /*������*/
	UART3_Handler.Init.WordLength=UART_WORDLENGTH_8B; /*�ֳ�Ϊ8λ���ݸ�ʽ*/
	UART3_Handler.Init.StopBits=UART_STOPBITS_1;	    /*һ��ֹͣλ*/
	UART3_Handler.Init.Parity=UART_PARITY_NONE;		    /*����żУ��λ*/
	UART3_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE; /*��Ӳ������*/
	UART3_Handler.Init.Mode=UART_MODE_TX_RX;		      /*�շ�ģʽ*/
	HAL_UART_Init(&UART3_Handler);					          /*HAL_UART_Init()��ʹ��UART7*/
}

/**
* @brief UART�ײ��ʼ����ʱ��ʹ�ܣ��������ã��ж�����
*       
* @note huart:���ھ�� �Ҵ˺����ᱻHAL_UART_Init()����
*/
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	if(huart==(&UART7_Handler))
	{
    /*GPIO�˿�����*/
		GPIO_InitTypeDef GPIO_Initure;
	
		__HAL_RCC_GPIOE_CLK_ENABLE();			  /*ʹ��GPIOAʱ��*/
		__HAL_RCC_UART7_CLK_ENABLE();			  /*ʹ��UART7ʱ��*/
	
		GPIO_Initure.Pin=GPIO_PIN_7;			  /*PE7*/
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;	/*�����������*/
		GPIO_Initure.Pull=GPIO_PULLUP;			/*����*/
		GPIO_Initure.Speed=GPIO_SPEED_FAST;	/*����*/
		GPIO_Initure.Alternate=GPIO_AF8_UART7;	/*����ΪUART7*/
		HAL_GPIO_Init(GPIOE,&GPIO_Initure);	   	/*��ʼ��PA9*/

		GPIO_Initure.Pin=GPIO_PIN_8;			      /*PE8*/
		HAL_GPIO_Init(GPIOE,&GPIO_Initure);	   	/*��ʼ��PA10*/
	
    __HAL_UART_DISABLE_IT(huart,UART_IT_TC);
#if EN_UART7_RX
		__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);/*���������ж�*/
		HAL_NVIC_EnableIRQ(UART7_IRQn);				   /*ʹ��UART7�ж�*/
		HAL_NVIC_SetPriority(UART7_IRQn,3,3);		 /*��ռ���ȼ�3�������ȼ�3*/
#endif	
	}
	if(huart==(&UART3_Handler))
	{
		/*GPIO�˿�����*/
		GPIO_InitTypeDef GPIO_Initure;
	
		__HAL_RCC_GPIOD_CLK_ENABLE();			      /*ʹ��GPIOBʱ��*/
		__HAL_RCC_USART3_CLK_ENABLE();		    	/*ʹ��USART3ʱ��*/
	
		GPIO_Initure.Pin=GPIO_PIN_8;			      /*PD8*/
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;	    /*�����������*/
		GPIO_Initure.Pull=GPIO_PULLUP;			    /*����*/
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		  /*����*/
		GPIO_Initure.Alternate=GPIO_AF7_USART3;	/*����ΪUSART3*/
		HAL_GPIO_Init(GPIOD,&GPIO_Initure);	   	/*��ʼ��PB10*/

		GPIO_Initure.Pin=GPIO_PIN_9;			      /*PD9*/
		HAL_GPIO_Init(GPIOD,&GPIO_Initure);	   	/*��ʼ��PD9*/
	
//		__HAL_UART_DISABLE_IT(huart,UART_IT_TC);
		__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);	/*���������ж�*/
		HAL_NVIC_EnableIRQ(USART3_IRQn);				  /*ʹ��USART3�ж�*/
		HAL_NVIC_SetPriority(USART3_IRQn,2,3);		/*��ռ���ȼ�2�������ȼ�3*/
		TIM7_Int_Init(1000-1,9000-1);/*100ms�ж�*/
		USART3_RX_STA=0;		         /*����*/
		TIM7->CR1&=~(1<<0);          /*�رն�ʱ��7*/
	}
}

/**
* @brief ����3,printf ����
*       
* @note  ȷ��һ�η������ݲ�����USART3_MAX_SEND_LEN�ֽ�
*/
void u3_printf(char* fmt,...)  
{  
	u16 i,j; 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)USART3_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART3_TX_BUF); /*�˴η������ݵĳ���*/
	for(j=0;j<i;j++)							        /*ѭ����������*/
	{
		while((USART3->SR&0X40)==0);			  /*ѭ������,ֱ���������*/
		USART3->DR=USART3_TX_BUF[j];  
	} 
}

/**
* @brief ����1�жϷ������
*/
void UART7_IRQHandler(void)                	
{ 
	u8 Res;
#if SYSTEM_SUPPORT_OS	 	/*ʹ��OS*/
	OSIntEnter();    
#endif
	if((__HAL_UART_GET_FLAG(&UART7_Handler,UART_FLAG_RXNE)!=RESET))  /*�����ж�(���յ������ݱ�����0x0d 0x0a��β)*/
	{
        HAL_UART_Receive(&UART7_Handler,&Res,1,1000); 
		if((USART_RX_STA&0x8000)==0)    /*����δ���*/
		{
			if(USART_RX_STA&0x4000)       /*���յ���0x0d*/
			{
				if(Res!=0x0a)USART_RX_STA=0;/*���մ���,���¿�ʼ*/
				else USART_RX_STA|=0x8000;	/*���������*/
			}
			else /*��û�յ�0X0D*/
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;/*�������ݴ���,���¿�ʼ����*/
				}		 
			}
		}   		 
	}
	HAL_UART_IRQHandler(&UART7_Handler);	
#if SYSTEM_SUPPORT_OS	 	/*ʹ��OS*/
	OSIntExit();  											 
#endif
} 

/**
* @brief �����жϷ������
*/
void USART3_IRQHandler(void)
{
	u8 res;	      
	if(__HAL_UART_GET_FLAG(&UART3_Handler,UART_FLAG_RXNE)!=RESET)//���յ�����*/
	{	 
//		HAL_UART_Receive(&UART3_Handler,&res,1,1000);
		res=USART3->DR;
		/*�������һ������,��û�б�����,���ٽ�����������*/
		if((USART3_RX_STA&(1<<15))==0)
		{ 
			/*�����Խ�������*/
			if(USART3_RX_STA<USART3_MAX_RECV_LEN)
			{
//				__HAL_TIM_SetCounter(&TIM7_Handler,0);	
				TIM7->CNT=0;         				/*���������	*/
				if(USART3_RX_STA==0) 				/*ʹ�ܶ�ʱ��7���ж� */
				{
//					__HAL_RCC_TIM7_CLK_ENABLE();            //ʹ��TIM7ʱ��*/
					TIM7->CR1|=1<<0;     			/*ʹ�ܶ�ʱ��7*/
				}
				USART3_RX_BUF[USART3_RX_STA++]=res;	/*��¼���յ���ֵ*/
			}else 
			{
				USART3_RX_STA|=1<<15;				/*ǿ�Ʊ�ǽ������*/
			} 
		}
	}  				 											 
}   
 




