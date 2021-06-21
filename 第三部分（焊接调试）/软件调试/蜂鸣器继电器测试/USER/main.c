#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"



/*������������ʹ��HAL�⺯��ʵ�ֿ���IO�����*/
 
int main(void)
{
    HAL_Init();                     //��ʼ��HAL��   
    Stm32_Clock_Init(360,25,2,8);   //����ʱ��,180Mhz
    delay_init(180);                //��ʼ����ʱ����
    BeepJDQ_Init();                     //��ʼ��LED  
    while(1)
    {
       
        Beep=0;JDQ=0;
        delay_ms(500);										//��ʱ500ms
      
        Beep=1;JDQ=1;
        delay_ms(500);                                      //��ʱ500ms 
    }
}
 











