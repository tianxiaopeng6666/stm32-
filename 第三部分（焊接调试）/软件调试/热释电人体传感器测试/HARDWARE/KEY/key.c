#include "key.h"

//PE13

//PIR��ʼ������
void PIR_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOE_CLK_ENABLE();           //����GPIOEʱ��
   
    
    GPIO_Initure.Pin=GPIO_PIN_13;            //PE0
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //����
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);
    

}


u8 PIR_Scan(void)
{
   
        if(PIR==1)       return 1;
        else if(PIR==0) return 0;          
   
   
}
