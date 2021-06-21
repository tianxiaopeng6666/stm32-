#include "usart7.h"
#include "delay.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��os,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//os ʹ��	  
#endif
  
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
//#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)	
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((UART7->SR&0X40)==0);//ѭ������,ֱ���������   
	UART7->DR = (u8) ch;      
	return ch;
}
#endif 

#if EN_UART7_RX   //���ʹ���˽���
//����7�жϷ������
//ע��,��ȡUARTx->SR�ܱ���Ī������Ĵ���   	
u8 UART_RX_BUF[UART_REC_LEN];     //���ջ���,���UART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 UART_RX_STA=0;       //����״̬���	

u8 aRxBuffer[RXBUFFERSIZE];//HAL��ʹ�õĴ��ڽ��ջ���
UART_HandleTypeDef UART7_Handler; //UART���

//��ʼ��IO ����7
//bound:������
void uart_init(u32 bound)
{	
	//UART ��ʼ������
	UART7_Handler.Instance=UART7;					    //UART7
	UART7_Handler.Init.BaudRate=bound;				    //������
	UART7_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	UART7_Handler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	UART7_Handler.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	UART7_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
	UART7_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	HAL_UART_Init(&UART7_Handler);					    //HAL_UART_Init()��ʹ��UART7
	
	HAL_UART_Receive_IT(&UART7_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE);//�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������
  
}

//UART�ײ��ʼ����ʱ��ʹ�ܣ��������ã��ж�����
//�˺����ᱻHAL_UART_Init()����
//huart:���ھ��

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIO�˿�����
	GPIO_InitTypeDef GPIO_Initure;
	
	if(huart->Instance==UART7)//����Ǵ���7�����д���7 MSP��ʼ��
	{
		__HAL_RCC_GPIOE_CLK_ENABLE();			//ʹ��GPIOEʱ��
		__HAL_RCC_UART7_CLK_ENABLE();			//ʹ��UART7ʱ��
	
		GPIO_Initure.Pin=GPIO_PIN_7;			//PE7
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
		GPIO_Initure.Pull=GPIO_PULLUP;			//����
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//����
		GPIO_Initure.Alternate=GPIO_AF8_UART7;	//����ΪUART7
		HAL_GPIO_Init(GPIOE,&GPIO_Initure);	   	//��ʼ��PE7

		GPIO_Initure.Pin=GPIO_PIN_8;			//PE10
		HAL_GPIO_Init(GPIOE,&GPIO_Initure);	   	//��ʼ��PE10
		
#if EN_UART7_RX
		HAL_NVIC_EnableIRQ(UART7_IRQn);		//ʹ��UART7�ж�ͨ��
		HAL_NVIC_SetPriority(UART7_IRQn,3,3);	//��ռ���ȼ�3�������ȼ�3
#endif	
	}

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==UART7)//����Ǵ���7
	{
		if((UART_RX_STA&0x8000)==0)//����δ���
		{
			if(UART_RX_STA&0x4000)//���յ���0x0d
			{
				if(aRxBuffer[0]!=0x0a)UART_RX_STA=0;//���մ���,���¿�ʼ
				else UART_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(aRxBuffer[0]==0x0d)UART_RX_STA|=0x4000;
				else
				{
					UART_RX_BUF[UART_RX_STA&0X3FFF]=aRxBuffer[0] ;
					UART_RX_STA++;
					if(UART_RX_STA>(UART_REC_LEN-1))UART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}

	}
}
 
//����7�жϷ������
void UART7_IRQHandler(void)                	
{ 
	u32 timeout=0;
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntEnter();    
#endif
	
	HAL_UART_IRQHandler(&UART7_Handler);	//����HAL���жϴ����ú���
	
	timeout=0;
    while (HAL_UART_GetState(&UART7_Handler) != HAL_UART_STATE_READY)//�ȴ�����
	{
	 timeout++;////��ʱ����
     if(timeout>HAL_MAX_DELAY) break;		
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&UART7_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)//һ�δ������֮�����¿����жϲ�����RxXferCountΪ1
	{
	 timeout++; //��ʱ����
	 if(timeout>HAL_MAX_DELAY) break;	
	}
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntExit();  											 
#endif
} 
#endif	


/*�����������ֱ�Ӱ��жϿ����߼�д���жϷ������ڲ���
 ˵��������HAL�⴦���߼���Ч�ʲ��ߡ�*/
/*


//����1�жϷ������
void UART7_IRQHandler(void)                	
{ 
	u8 Res;
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntEnter();    
#endif
	if((__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_RXNE)!=RESET))  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
        HAL_UART_Receive(&UART1_Handler,&Res,1,1000); 
		if((USART_RX_STA&0x8000)==0)//����δ���
		{
			if(USART_RX_STA&0x4000)//���յ���0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}   		 
	}
	HAL_UART_IRQHandler(&UART1_Handler);	
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntExit();  											 
#endif
} 
#endif	
*/
 




