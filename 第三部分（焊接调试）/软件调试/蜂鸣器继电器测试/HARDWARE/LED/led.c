#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
  
////////////////////////////////////////////////////////////////////////////////// 	

//��ʼ��PE12 15Ϊ���.��ʹ��ʱ��	    
//������ �̵��� IO��ʼ��
void BeepJDQ_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOE_CLK_ENABLE();           //����GPIOEʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_15|GPIO_PIN_12; 
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);
	
    HAL_GPIO_WritePin(GPIOE,GPIO_PIN_15|GPIO_PIN_12,GPIO_PIN_RESET);	//PE12��0 ������ �̵���Ĭ�Ϲر�
 
}
