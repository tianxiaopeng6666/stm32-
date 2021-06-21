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
    HAL_Init();                     //��ʼ��HAL��   
    Stm32_Clock_Init(360,25,2,8);   //����ʱ��,180Mhz
    delay_init(180);                //��ʼ����ʱ����
    uart_init(115200);              //��ʼ��USART
    DS3231_Init();
    DS3231_Set(21,05,25,14,50,25);  
//	�״�����ʱ�䣬������ɺ�ע�͵�����������һ�γ��򼴿ɵ��籣��
             //��,�£��գ�ʱ���֣���
	
    while(1)
    {
					get_show_time();

			if(t!=calendar.sec)
		{
			t=calendar.sec;
			printf("������:");
			printf("%ld,%d,%d\r\n",calendar.w_year+2000,calendar.w_month,calendar.w_date);
			printf("ʱ����:");
			printf("%d,%d,%d\r\n",calendar.hour,calendar.min,calendar.sec);
		}
				
    }
}

