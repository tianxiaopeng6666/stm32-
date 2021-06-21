#include "sys.h"
#include "delay.h"
#include "usart.h"




int main(void)
{
	u8 len;	
	u16 times=0; 
	
    HAL_Init();                    	//��ʼ��HAL��    
    Stm32_Clock_Init(96,4,2,4);     //����ʱ��,96Mhz
    delay_init(96);                 //��ʼ����ʱ����
    
	uart_init(115200);				//��ʼ������115200
	
	while(1)
	{
        		
	    if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			printf("\r\n�����͵���ϢΪ:\r\n");
			HAL_UART_Transmit(&UART1_Handler,(uint8_t*)USART_RX_BUF,len,1000);	//���ͽ��յ�������
			while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);		//�ȴ����ͽ���
			printf("\r\n\r\n");//���뻻��
			USART_RX_STA=0;
		}else
		{
			times++;
			if(times%5000==0)
			{
				printf("\r\nALIENTEK NANO STM32������ ����ʵ��\r\n");
				printf("����@ALIENTEK\r\n\r\n\r\n");
			}
			if(times%200==0)printf("����������,�Իس�������\r\n");  
			delay_ms(10);   
		} 
		
	}
}

