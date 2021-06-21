#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"


int main(void)
{
	
    HAL_Init();                    	//��ʼ��HAL��    
    Stm32_Clock_Init(96,4,2,4);     //����ʱ��,96Mhz
    delay_init(96);                 //��ʼ����ʱ����
    LED_Init();                     //��ʼ��LED 
	while(1)
	{
        		
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET);//LED0��
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET);//LED1��
		delay_ms(500);                                  //��ʱ500ms
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_SET);//LED0��
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);//LED1��
		delay_ms(500);                                   //��ʱ500ms
		
	}
}


