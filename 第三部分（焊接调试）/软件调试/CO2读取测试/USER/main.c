#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"


extern u8  TIM5CH3_CAPTURE_STA;		//���벶��״̬		    				
extern u32	TIM5CH3_CAPTURE_VAL;	//���벶��ֵ 

int main(void)
{
    long long temp=0;  
    HAL_Init();                     	//��ʼ��HAL��   
    Stm32_Clock_Init(360,25,2,8);   	//����ʱ��,180Mhz
    delay_init(180);               	 	//��ʼ����ʱ����
    uart_init(115200);             		//��ʼ��USART
    TIM5_CH3_Cap_Init(0XFFFFFFFF,90-1); //��1MHZ��Ƶ�ʼ���
    while(1)
    {
        delay_ms(10);
	      
        if(TIM5CH3_CAPTURE_STA&0X80)        //�ɹ�������һ�θߵ�ƽ
		{
			temp=TIM5CH3_CAPTURE_STA&0X3F; 
			temp*=0XFFFFFFFF;		 	    //���ʱ���ܺ�
			temp+=TIM5CH3_CAPTURE_VAL;      //�õ��ܵĸߵ�ƽʱ��
			printf("HIGH:%lld us\r\n",temp);//��ӡ�ܵĸߵ�ƽʱ��
			long long a=(temp*1.00/10)*20;
			printf("CO2:%lld ppm\r\n",a);//��ӡ�ܵĸߵ�ƽʱ��
			TIM5CH3_CAPTURE_STA=0;          //������һ�β���
		  
    }
	}
}

