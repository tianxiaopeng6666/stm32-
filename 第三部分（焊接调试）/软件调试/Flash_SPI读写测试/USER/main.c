#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "w25qxx.h"


//Ҫд�뵽W25Q16���ַ�������
const u8 TEXT_Buffer[]={"STM32 SPI TEST"};
#define SIZE sizeof(TEXT_Buffer)	 

int main(void)
{
    u8 key;
	u16 i=0;
	u8 datatemp[SIZE];
	u32 FLASH_SIZE;
    HAL_Init();                     //��ʼ��HAL��   
    Stm32_Clock_Init(360,25,2,8);   //����ʱ��,180Mhz
    delay_init(180);                //��ʼ����ʱ����
    uart_init(115200);              //��ʼ��USART
    W25QXX_Init();				    //W25QXX��ʼ��

	while(W25QXX_ReadID()!=W25Q80)								//��ⲻ��W25Q256
	{
		printf("δ��⵽w25\r\n" );
	}

	FLASH_SIZE=1*1024*1024;	//FLASH ��СΪ32M�ֽ�
  	 
	while(1)
	{	
			W25QXX_Write((u8*)TEXT_Buffer,FLASH_SIZE-100,SIZE);		//�ӵ�����100����ַ����ʼ,д��SIZE���ȵ�����
		
			W25QXX_Read(datatemp,FLASH_SIZE-100,SIZE);					//�ӵ�����100����ַ����ʼ,����SIZE���ֽ�
	   printf("�����ַ�����%s\r\n",datatemp );
	
		delay_ms(10);
	  
	}		    
}
