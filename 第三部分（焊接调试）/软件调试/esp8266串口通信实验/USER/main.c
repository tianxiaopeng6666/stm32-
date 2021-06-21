#include "sys.h"
#include "delay.h"
#include "usart.h"

//ESP8266串口通讯实验
//8266使用串口三 PD8 PD9  stm32接收串口三的数据
//stm32通过串口七 PE7 PE8 将数据转发 
//串口调试助手通过串口7可看到 esp8266通过串口三发送的数据


int main(void)
{

    HAL_Init();                     //初始化HAL库   
    Stm32_Clock_Init(360,25,2,8);   //设置时钟,180Mhz
    delay_init(180);                //初始化延时函数
    uart_init(115200);              //初始化USART1
		usart3_init(115200);  						//初始化串口3波特率为115200
   // LED_Init();                     //初始化LED 
   // KEY_Init();                     //初始化按键
printf("666");	//发送到串口
    while(1)
    {
			if(USART3_RX_STA&0X8000)		//接收到一次数据了
	{ 
		//USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//添加结束符
		printf("%s",USART3_RX_BUF);	//发送到串口
		USART3_RX_STA=0;
	} 

    } 
}

