#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
  
////////////////////////////////////////////////////////////////////////////////// 	

//��ʼ��PB1Ϊ���.��ʹ��ʱ��	    
//LED IO��ʼ��
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOD_CLK_ENABLE();           //����GPIODʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_13; 
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);
	
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_SET);	//PD15��1��Ĭ�ϳ�ʼ�������
 
}
