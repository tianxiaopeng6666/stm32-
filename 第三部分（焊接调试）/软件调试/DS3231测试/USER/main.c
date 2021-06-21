#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "ds3231.h"

u8 i=0;
//PIR PE13

int main(void)
{
//    u8 key;
		u8 t=0;
    HAL_Init();                     //初始化HAL库   
    Stm32_Clock_Init(360,25,2,8);   //设置时钟,180Mhz
    delay_init(180);                //初始化延时函数
    uart_init(115200);              //初始化USART
    DS3231_Init();
    DS3231_Set(21,05,25,14,50,25);  
//	首次设置时间，设置完成后注释掉，重新下载一次程序即可掉电保存
             //年,月，日，时，分，秒
	
    while(1)
    {
					get_show_time();

			if(t!=calendar.sec)
		{
			t=calendar.sec;
			printf("年月日:");
			printf("%ld,%d,%d\r\n",calendar.w_year+2000,calendar.w_month,calendar.w_date);
			printf("时分秒:");
			printf("%d,%d,%d\r\n",calendar.hour,calendar.min,calendar.sec);
		}
				
    }
}

