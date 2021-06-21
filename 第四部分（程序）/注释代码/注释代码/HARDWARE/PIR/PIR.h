#ifndef _KEY_H
#define _KEY_H
#include "sys.h"



//下面的方式是通过直接操作HAL库函数方式读取IO
#define PIR        HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_13)  //PE13


void PIR_Init(void);
u8 PIR_Scan(void);
#endif
