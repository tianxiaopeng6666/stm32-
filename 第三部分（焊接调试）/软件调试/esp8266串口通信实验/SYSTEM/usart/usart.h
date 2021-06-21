#ifndef _USART_H
#define _USART_H
#include "sys.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	
#include "timer.h"

#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART7_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	
extern UART_HandleTypeDef UART7_Handler; //UART句柄
//如果想串口中断接收，请不要注释以下宏定义
void uart_init(u32 bound);

//串口3相关宏定义
#define USART3_MAX_RECV_LEN		800					//最大接收缓存字节数
#define USART3_MAX_SEND_LEN		800					//最大发送缓存字节数
#define USART3_RX_EN 			1					//0,不接收;1,接收.

extern u8  USART3_RX_BUF[USART3_MAX_RECV_LEN]; 		//接收缓冲,最大USART3_MAX_RECV_LEN字节
extern u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 		//发送缓冲,最大USART3_MAX_SEND_LEN字节
extern u16 USART3_RX_STA;   						//接收数据状态

void usart3_init(u32 bound);				//串口3初始化 
void TIM7_Int_Init(u16 arr,u16 psc);
void u3_printf(char* fmt, ...);
#endif
