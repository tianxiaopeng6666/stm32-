#ifndef _USART_H
#define _USART_H
#include "sys.h"
#include "stdio.h"	

#define UART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_UART7_RX 			1		//使能（1）/禁止（0）串口7接收
	  	
extern u8  UART_RX_BUF[UART_REC_LEN]; //接收缓冲,最大UART_REC_LEN个字节.末字节为换行符 
extern u16 UART_RX_STA;         		//接收状态标记	
extern UART_HandleTypeDef UART7_Handler; //UART句柄

#define RXBUFFERSIZE   1 //缓存大小
extern u8 aRxBuffer[RXBUFFERSIZE];//HAL库USART接收Buffer

//如果想串口中断接收，请不要注释以下宏定义
void uart_init(u32 bound);


#endif
