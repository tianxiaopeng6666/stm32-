#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"

//PIR PE13

int main(void)
{
    u8 key;
    HAL_Init();                     //��ʼ��HAL��   
    Stm32_Clock_Init(360,25,2,8);   //����ʱ��,180Mhz
    delay_init(180);                //��ʼ����ʱ����
    uart_init(115200);              //��ʼ��USART
    LED_Init();                     //��ʼ��LED 
    PIR_Init();                     //��ʼ������
	
    while(1)
    {
        key=PIR_Scan();            //����ɨ��
		    if (key) printf("��⵽��\r\n");
			  else printf("û��⵽��\r\n");
        delay_ms(1000);
    }
}

