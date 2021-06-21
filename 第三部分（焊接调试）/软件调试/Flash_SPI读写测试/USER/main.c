#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "w25qxx.h"


//要写入到W25Q16的字符串数组
const u8 TEXT_Buffer[]={"STM32 SPI TEST"};
#define SIZE sizeof(TEXT_Buffer)	 

int main(void)
{
    u8 key;
	u16 i=0;
	u8 datatemp[SIZE];
	u32 FLASH_SIZE;
    HAL_Init();                     //初始化HAL库   
    Stm32_Clock_Init(360,25,2,8);   //设置时钟,180Mhz
    delay_init(180);                //初始化延时函数
    uart_init(115200);              //初始化USART
    W25QXX_Init();				    //W25QXX初始化

	while(W25QXX_ReadID()!=W25Q80)								//检测不到W25Q256
	{
		printf("未检测到w25\r\n" );
	}

	FLASH_SIZE=1*1024*1024;	//FLASH 大小为32M字节
  	 
	while(1)
	{	
			W25QXX_Write((u8*)TEXT_Buffer,FLASH_SIZE-100,SIZE);		//从倒数第100个地址处开始,写入SIZE长度的数据
		
			W25QXX_Read(datatemp,FLASH_SIZE-100,SIZE);					//从倒数第100个地址处开始,读出SIZE个字节
	   printf("读到字符串：%s\r\n",datatemp );
	
		delay_ms(10);
	  
	}		    
}
