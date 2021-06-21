#include "key.h"

//PE13

//PIR初始化函数
void PIR_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOE_CLK_ENABLE();           //开启GPIOE时钟
   
    
    GPIO_Initure.Pin=GPIO_PIN_13;            //PE0
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);
    

}


u8 PIR_Scan(void)
{
   
        if(PIR==1)       return 1;
        else if(PIR==0) return 0;          
   
   
}
