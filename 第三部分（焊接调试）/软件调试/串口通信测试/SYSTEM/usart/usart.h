#ifndef _USART_H
#define _USART_H
#include "sys.h"
#include "stdio.h"	


#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART7_RX 			1		//ʹ�ܣ�1��/��ֹ��0������7����
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	
extern UART_HandleTypeDef UART7_Handler; //UART���

#define RXBUFFERSIZE   1 //�����С
extern u8 aRxBuffer[RXBUFFERSIZE];//HAL��USART����Buffer

//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 bound);


#endif
