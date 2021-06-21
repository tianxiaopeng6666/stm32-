#include "usart.h"

/**
 ******************************************************************************
 * @file usart.c
 * @author T0102
 *
 * @brief  串口初始化和定义库函数
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
 
/*如果使用os,则包括下面的头文件即可.*/
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//os 使用	  
#endif

/*标准库需要的支持函数*/
#if 1
#pragma import(__use_no_semihosting)             
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;
/*定义_sys_exit()以避免使用半主机模式*/
void _sys_exit(int x) 
{ 
	x = x; 
} 

/*重定义fputc函数*/
int fputc(int ch, FILE *f)
{ 	
	while((UART7->SR&0X40)==0);/*循环发送,直到发送完毕*/
	UART7->DR = (u8) ch;      
	return ch;
}
#endif 

/*串口1中断服务程序*/
/*接收缓冲,最大USART_REC_LEN个字节.*/
u8 USART_RX_BUF[USART_REC_LEN];

/*接收状态
* bit15，	接收完成标志
* bit14，	接收到0x0d
* bit13~0,接收到的有效字节数目
*/
u16 USART_RX_STA=0;

/*串口发送缓存区*/
/*发送缓冲,最大USART3_MAX_SEND_LEN字节*/
__align(8) u8 USART3_TX_BUF[USART3_MAX_SEND_LEN];

/*串口接收缓存区*/
/*接收缓冲,最大USART3_MAX_RECV_LEN个字节.*/
u8 USART3_RX_BUF[USART3_MAX_RECV_LEN];


/*通过判断接收连续2个字符之间的时间差不大于100ms来决定是不是一次连续的数据.
* 如果2个字符接收间隔超过100ms,则认为不是1次连续数据.也就是超过100ms没有接收到
* 任何数据,则表示此次接收完毕.
* 接收到的数据状态
* [15]:0,没有接收到数据;1,接收到了一批数据.
* [14:0]:接收到的数据长度
*/
u16 USART3_RX_STA=0;

/*UART句柄*/
UART_HandleTypeDef UART7_Handler; 
UART_HandleTypeDef UART3_Handler;

/**
* @brief 初始化IO 串口1
*       
* @note bound:波特率
*/
void uart_init(u32 bound)
{	
	/*UART 初始化设置*/
	UART7_Handler.Instance=UART7;					            /*UART7*/
	UART7_Handler.Init.BaudRate=bound;				        /*波特率*/
	UART7_Handler.Init.WordLength=UART_WORDLENGTH_8B; /*字长为8位数据格式*/
	UART7_Handler.Init.StopBits=UART_STOPBITS_1;	    /*一个停止位*/
	UART7_Handler.Init.Parity=UART_PARITY_NONE;		    /*无奇偶校验位*/
	UART7_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE; /*无硬件流控*/
	UART7_Handler.Init.Mode=UART_MODE_TX_RX;		      /*收发模式*/
	HAL_UART_Init(&UART7_Handler);					          /*HAL_UART_Init()会使能UART7*/
}

/**
* @brief 初始化IO 串口3
*       
* @note bound:波特率
*/
void usart3_init(u32 bound)
{	
	/*UART 初始化设置*/
	UART3_Handler.Instance=USART3;					          /*UART7*/
	UART3_Handler.Init.BaudRate=bound;				        /*波特率*/
	UART3_Handler.Init.WordLength=UART_WORDLENGTH_8B; /*字长为8位数据格式*/
	UART3_Handler.Init.StopBits=UART_STOPBITS_1;	    /*一个停止位*/
	UART3_Handler.Init.Parity=UART_PARITY_NONE;		    /*无奇偶校验位*/
	UART3_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE; /*无硬件流控*/
	UART3_Handler.Init.Mode=UART_MODE_TX_RX;		      /*收发模式*/
	HAL_UART_Init(&UART3_Handler);					          /*HAL_UART_Init()会使能UART7*/
}

/**
* @brief UART底层初始化，时钟使能，引脚配置，中断配置
*       
* @note huart:串口句柄 且此函数会被HAL_UART_Init()调用
*/
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	if(huart==(&UART7_Handler))
	{
    /*GPIO端口设置*/
		GPIO_InitTypeDef GPIO_Initure;
	
		__HAL_RCC_GPIOE_CLK_ENABLE();			  /*使能GPIOA时钟*/
		__HAL_RCC_UART7_CLK_ENABLE();			  /*使能UART7时钟*/
	
		GPIO_Initure.Pin=GPIO_PIN_7;			  /*PE7*/
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;	/*复用推挽输出*/
		GPIO_Initure.Pull=GPIO_PULLUP;			/*上拉*/
		GPIO_Initure.Speed=GPIO_SPEED_FAST;	/*高速*/
		GPIO_Initure.Alternate=GPIO_AF8_UART7;	/*复用为UART7*/
		HAL_GPIO_Init(GPIOE,&GPIO_Initure);	   	/*初始化PA9*/

		GPIO_Initure.Pin=GPIO_PIN_8;			      /*PE8*/
		HAL_GPIO_Init(GPIOE,&GPIO_Initure);	   	/*初始化PA10*/
	
    __HAL_UART_DISABLE_IT(huart,UART_IT_TC);
#if EN_UART7_RX
		__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);/*开启接收中断*/
		HAL_NVIC_EnableIRQ(UART7_IRQn);				   /*使能UART7中断*/
		HAL_NVIC_SetPriority(UART7_IRQn,3,3);		 /*抢占优先级3，子优先级3*/
#endif	
	}
	if(huart==(&UART3_Handler))
	{
		/*GPIO端口设置*/
		GPIO_InitTypeDef GPIO_Initure;
	
		__HAL_RCC_GPIOD_CLK_ENABLE();			      /*使能GPIOB时钟*/
		__HAL_RCC_USART3_CLK_ENABLE();		    	/*使能USART3时钟*/
	
		GPIO_Initure.Pin=GPIO_PIN_8;			      /*PD8*/
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;	    /*复用推挽输出*/
		GPIO_Initure.Pull=GPIO_PULLUP;			    /*上拉*/
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		  /*高速*/
		GPIO_Initure.Alternate=GPIO_AF7_USART3;	/*复用为USART3*/
		HAL_GPIO_Init(GPIOD,&GPIO_Initure);	   	/*初始化PB10*/

		GPIO_Initure.Pin=GPIO_PIN_9;			      /*PD9*/
		HAL_GPIO_Init(GPIOD,&GPIO_Initure);	   	/*初始化PD9*/
	
//		__HAL_UART_DISABLE_IT(huart,UART_IT_TC);
		__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);	/*开启接收中断*/
		HAL_NVIC_EnableIRQ(USART3_IRQn);				  /*使能USART3中断*/
		HAL_NVIC_SetPriority(USART3_IRQn,2,3);		/*抢占优先级2，子优先级3*/
		TIM7_Int_Init(1000-1,9000-1);/*100ms中断*/
		USART3_RX_STA=0;		         /*清零*/
		TIM7->CR1&=~(1<<0);          /*关闭定时器7*/
	}
}

/**
* @brief 串口3,printf 函数
*       
* @note  确保一次发送数据不超过USART3_MAX_SEND_LEN字节
*/
void u3_printf(char* fmt,...)  
{  
	u16 i,j; 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)USART3_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART3_TX_BUF); /*此次发送数据的长度*/
	for(j=0;j<i;j++)							        /*循环发送数据*/
	{
		while((USART3->SR&0X40)==0);			  /*循环发送,直到发送完毕*/
		USART3->DR=USART3_TX_BUF[j];  
	} 
}

/**
* @brief 串口1中断服务程序
*/
void UART7_IRQHandler(void)                	
{ 
	u8 Res;
#if SYSTEM_SUPPORT_OS	 	/*使用OS*/
	OSIntEnter();    
#endif
	if((__HAL_UART_GET_FLAG(&UART7_Handler,UART_FLAG_RXNE)!=RESET))  /*接收中断(接收到的数据必须是0x0d 0x0a结尾)*/
	{
        HAL_UART_Receive(&UART7_Handler,&Res,1,1000); 
		if((USART_RX_STA&0x8000)==0)    /*接收未完成*/
		{
			if(USART_RX_STA&0x4000)       /*接收到了0x0d*/
			{
				if(Res!=0x0a)USART_RX_STA=0;/*接收错误,重新开始*/
				else USART_RX_STA|=0x8000;	/*接收完成了*/
			}
			else /*还没收到0X0D*/
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;/*接收数据错误,重新开始接收*/
				}		 
			}
		}   		 
	}
	HAL_UART_IRQHandler(&UART7_Handler);	
#if SYSTEM_SUPPORT_OS	 	/*使用OS*/
	OSIntExit();  											 
#endif
} 

/**
* @brief 串口中断服务程序
*/
void USART3_IRQHandler(void)
{
	u8 res;	      
	if(__HAL_UART_GET_FLAG(&UART3_Handler,UART_FLAG_RXNE)!=RESET)//接收到数据*/
	{	 
//		HAL_UART_Receive(&UART3_Handler,&res,1,1000);
		res=USART3->DR;
		/*接收完的一批数据,还没有被处理,则不再接收其他数据*/
		if((USART3_RX_STA&(1<<15))==0)
		{ 
			/*还可以接收数据*/
			if(USART3_RX_STA<USART3_MAX_RECV_LEN)
			{
//				__HAL_TIM_SetCounter(&TIM7_Handler,0);	
				TIM7->CNT=0;         				/*计数器清空	*/
				if(USART3_RX_STA==0) 				/*使能定时器7的中断 */
				{
//					__HAL_RCC_TIM7_CLK_ENABLE();            //使能TIM7时钟*/
					TIM7->CR1|=1<<0;     			/*使能定时器7*/
				}
				USART3_RX_BUF[USART3_RX_STA++]=res;	/*记录接收到的值*/
			}else 
			{
				USART3_RX_STA|=1<<15;				/*强制标记接收完成*/
			} 
		}
	}  				 											 
}   
 




