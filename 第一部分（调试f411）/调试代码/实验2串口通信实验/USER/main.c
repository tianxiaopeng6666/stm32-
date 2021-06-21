#include "sys.h"
#include "delay.h"
#include "usart.h"




int main(void)
{
	u8 len;	
	u16 times=0; 
	
    HAL_Init();                    	//初始化HAL库    
    Stm32_Clock_Init(96,4,2,4);     //设置时钟,96Mhz
    delay_init(96);                 //初始化延时函数
    
	uart_init(115200);				//初始化串口115200
	
	while(1)
	{
        		
	    if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			printf("\r\n您发送的消息为:\r\n");
			HAL_UART_Transmit(&UART1_Handler,(uint8_t*)USART_RX_BUF,len,1000);	//发送接收到的数据
			while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);		//等待发送结束
			printf("\r\n\r\n");//插入换行
			USART_RX_STA=0;
		}else
		{
			times++;
			if(times%5000==0)
			{
				printf("\r\nALIENTEK NANO STM32开发板 串口实验\r\n");
				printf("哈哈@ALIENTEK\r\n\r\n\r\n");
			}
			if(times%200==0)printf("请输入数据,以回车键结束\r\n");  
			delay_ms(10);   
		} 
		
	}
}

