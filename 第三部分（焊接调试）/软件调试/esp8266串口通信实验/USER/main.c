#include "sys.h"
#include "delay.h"
#include "usart.h"

//ESP8266����ͨѶʵ��
//8266ʹ�ô����� PD8 PD9  stm32���մ�����������
//stm32ͨ�������� PE7 PE8 ������ת�� 
//���ڵ�������ͨ������7�ɿ��� esp8266ͨ�����������͵�����


int main(void)
{

    HAL_Init();                     //��ʼ��HAL��   
    Stm32_Clock_Init(360,25,2,8);   //����ʱ��,180Mhz
    delay_init(180);                //��ʼ����ʱ����
    uart_init(115200);              //��ʼ��USART1
		usart3_init(115200);  						//��ʼ������3������Ϊ115200
   // LED_Init();                     //��ʼ��LED 
   // KEY_Init();                     //��ʼ������
printf("666");	//���͵�����
    while(1)
    {
			if(USART3_RX_STA&0X8000)		//���յ�һ��������
	{ 
		//USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//��ӽ�����
		printf("%s",USART3_RX_BUF);	//���͵�����
		USART3_RX_STA=0;
	} 

    } 
}

