#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"


extern u8  TIM5CH3_CAPTURE_STA;		//输入捕获状态		    				
extern u32	TIM5CH3_CAPTURE_VAL;	//输入捕获值 

int main(void)
{
    long long temp=0;  
    HAL_Init();                     	//初始化HAL库   
    Stm32_Clock_Init(360,25,2,8);   	//设置时钟,180Mhz
    delay_init(180);               	 	//初始化延时函数
    uart_init(115200);             		//初始化USART
    TIM5_CH3_Cap_Init(0XFFFFFFFF,90-1); //以1MHZ的频率计数
    while(1)
    {
        delay_ms(10);
	      
        if(TIM5CH3_CAPTURE_STA&0X80)        //成功捕获到了一次高电平
		{
			temp=TIM5CH3_CAPTURE_STA&0X3F; 
			temp*=0XFFFFFFFF;		 	    //溢出时间总和
			temp+=TIM5CH3_CAPTURE_VAL;      //得到总的高电平时间
			printf("HIGH:%lld us\r\n",temp);//打印总的高点平时间
			long long a=(temp*1.00/10)*20;
			printf("CO2:%lld ppm\r\n",a);//打印总的高点平时间
			TIM5CH3_CAPTURE_STA=0;          //开启下一次捕获
		  
    }
	}
}

