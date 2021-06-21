#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"



/*下面主函数是使用HAL库函数实现控制IO口输出*/
 
int main(void)
{
    HAL_Init();                     //初始化HAL库   
    Stm32_Clock_Init(360,25,2,8);   //设置时钟,180Mhz
    delay_init(180);                //初始化延时函数
    BeepJDQ_Init();                     //初始化LED  
    while(1)
    {
       
        Beep=0;JDQ=0;
        delay_ms(500);										//延时500ms
      
        Beep=1;JDQ=1;
        delay_ms(500);                                      //延时500ms 
    }
}
 











