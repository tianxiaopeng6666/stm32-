#ifndef _KEY_H
#define _KEY_H
#include "sys.h"



//����ķ�ʽ��ͨ��ֱ�Ӳ���HAL�⺯����ʽ��ȡIO
#define PIR        HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_13)  //PE13


void PIR_Init(void);
u8 PIR_Scan(void);
#endif
