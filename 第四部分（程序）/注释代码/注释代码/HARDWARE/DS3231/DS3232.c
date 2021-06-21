#include "ds3231.h"
#include "delay.h"
#include "usart.h"
#include "string.h"

/**
 ******************************************************************************
 * @file DS3232.c
 * @author T0102
 *
 * @brief  DS3232时钟芯片的初始化
 ******************************************************************************
 *@note
 *
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 ZJUT.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ZJUT under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. 
 ******************************************************************************
 */
 
 /*I2C读入的信号值 作为全局变量*/
_calendar_obj calendar;

/**
* @brief BCD二进制转HEX16进制
 */
u8 BCD2HEX(u8 val)
{
    u8 i;
    i= val&0x0f;
    val >>= 4;
    val &= 0x0f;
    val *= 10;
    i += val;
    
    return i;
}

/**
* @brief 字节位数变化
*       
* @note 8位转16位
*/
u16 B_BCD(u8 val)
{
  u8 i,j,k;
  i=val/10;
  j=val%10;
  k=j+(i<<4);
  return k;
}


/**
* @brief 初始化IIC
*/
void IIC_Init(void)
{                                             
		GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOB_CLK_ENABLE();           /*开启GPIOA时钟  */ 
           
    GPIO_Initure.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  /*推挽输出*/
	  GPIO_Initure.Pull=GPIO_PULLUP;          /*上拉*/
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     /*高速*/
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	    
    IIC_SCL=1;
    IIC_SDA=1;
}

/**
* @brief 产生IIC起始信号
*/
void IIC_Start(void)
{
    SDA_OUT();   /*sda线输出*/
	  delay_us(1);	 
    IIC_SDA=1;                    
    IIC_SCL=1;
    delay_us(5);
    IIC_SDA=0;   /*START:当CLK从低电平转高电平*/
    delay_us(5);
    IIC_SCL=0;   /*钳住I2C总线，准备发送或接收数据 */
	  delay_us(2);
}          

/**
* @brief 产生IIC停止信号
*/
void IIC_Stop(void)
{
    SDA_OUT();/*sda线输出*/
    IIC_SCL=0;
    IIC_SDA=0;/*STOP:当CLK从低电平转高电平*/
    delay_us(4);
    IIC_SCL=1; 
    delay_us(5);
    IIC_SDA=1;/*发送I2C总线结束信号*/
    delay_us(4);                                                                   
}

/**
* @brief 等待应答信号到来
*       
* @note 返回值：1，接收应答失败   0，接收应答成功
*/
u8 IIC_Wait_Ack(void)
{
    u8 ucErrTime=0;
    SDA_IN();      //SDA设置为输入  
    IIC_SDA=1;delay_us(1);           
    IIC_SCL=1;delay_us(1);         
    while(READ_SDA)
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            IIC_Stop();
            return 1;
        }
     }
     IIC_SCL=0;//时钟输出0            
     return 0;  
} 

/**
* @brief 产生ACK应答
*/
void IIC_Ack(void)
{
    IIC_SCL=0;
    SDA_OUT();
    IIC_SDA=0;
    delay_us(2);
    IIC_SCL=1;
    delay_us(2);
    IIC_SCL=0;
}

/**
* @brief 不产生ACK应答
*/void IIC_NAck(void)
{
    IIC_SCL=0;
    SDA_OUT();
    IIC_SDA=1;
    delay_us(2);
    IIC_SCL=1;
    delay_us(2);
    IIC_SCL=0;
}                                                                              
 
/**
* @brief IIC发送一个字节
*       
* @note 返回从机有无应答  
*       1，有应答
*       0，无应答
*/
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
    SDA_OUT();             
    IIC_SCL=0;/*拉低时钟开始数据传输*/
    for(t=0;t<8;t++)
    {    
        IIC_SCL=0;
        delay_us(5);
        if((txd&0x80)>>7)
            IIC_SDA=1;
        else
            IIC_SDA=0;
				txd<<=1;
				delay_us(2);
        IIC_SCL=1;
        delay_us(2);
        IIC_SCL=0;
				delay_us(2);
        
    }         
}

/**
* @brief 根据读入字节 返回8位数据
*       
* @note 读1个字节，ack=1时，发送ACK，ack=0，发送nACK  
*/
u8 IIC_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
    SDA_IN();/*SDA设置为输入*/
    for(i=0;i<8;i++ )
    {
        IIC_SCL=0; 
        delay_us(2);
        IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)
	          receive++;   
        delay_us(1); 
    }                                         
    if (!ack)
        IIC_NAck();/*发送nACK*/
    else
        IIC_Ack(); /*发送ACK */  
    return receive;
}

/**
* @brief 指定地址写数据/命令 
*/
void I2cByteWrite(u8 addr,u8 bytedata)
{
  IIC_Start();
  delay_us(5);
  IIC_Send_Byte(DS3231_WriteAddress);/*IIC发送字节*/
  IIC_Wait_Ack();
  delay_us(5);
  IIC_Send_Byte(addr);
  IIC_Wait_Ack();
  delay_us(5);
  IIC_Send_Byte(bytedata);
  IIC_Wait_Ack();
  delay_us(5);
  IIC_Stop();
}	

/**
* @brief 读取一个8为地址
*/
u8 I2cByteRead(u8 ReadAddr)
{
	uint8_t data = 0;
	
	IIC_Start();													/*产生起始位*/
	IIC_Send_Byte(DS3231_WriteAddress); 	/*发送从机地址（写模式）*/
	IIC_Wait_Ack();										    /*等待响应*/
	delay_us(5);
	IIC_Send_Byte(ReadAddr);							/*发送寄存器地址*/
	IIC_Wait_Ack();										    /*等待响应*/
	delay_us(5);
	IIC_Start();													/*重复起始位*/
	IIC_Send_Byte(DS3231_ReadAddress);		/*发送从机地址（读模式）*/
	IIC_Wait_Ack();									      /*等待响应*/
	delay_us(5);
	data = IIC_Read_Byte(0);							/*读取数据，参数设为0 --- NACK*/
//	*Receive = data;										/*将结果赋值给接收位*/
	IIC_Stop();
	return data;
}

/**
* @brief 使能时钟
*/
void DS3231_Init(void)
{
	IIC_Init();
	I2cByteWrite(0x0e,0); /*I2C写入*/
	delay_ms(2); 
  I2cByteWrite(0x0f,0x0);
	delay_ms(2); 
}

/**
* @brief 读取时钟信号
*/
void DS3231_Get(void)
{
  calendar.w_year=I2cByteRead(0x06);  
  calendar.w_month=I2cByteRead(0x05);  
  calendar.w_date=I2cByteRead(0x04); 
  calendar.hour=I2cByteRead(0x02);  
  calendar.min=I2cByteRead(0x01);
  calendar.sec=I2cByteRead(0x00);
}

/**
* @brief 通过写入数据 配置时钟芯片
*/
void DS3231_Set(u8 yea,u8 mon,u8 da,u8 hou,u8 min,u8 sec)
{
  u8 temp=0;
  
  temp=B_BCD(yea);
  I2cByteWrite(0x06,temp);
  
  temp=B_BCD(mon);
  I2cByteWrite(0x05,temp);
   
  temp=B_BCD(da);
  I2cByteWrite(0x04,temp);
  
  temp=B_BCD(hou);
  I2cByteWrite(0x02,temp);
  
  temp=B_BCD(min);
  I2cByteWrite(0x01,temp);
  
  temp=B_BCD(sec);
  I2cByteWrite(0x00,temp);
}
				
/**
* @brief 读入时钟信号  并存入全局变量中
*/
void get_show_time(void)
{
		/*读取年*/
    calendar.w_year=I2cByteRead(0x06);  
    calendar.w_year=BCD2HEX(calendar.w_year);

    /*读取月*/
    calendar.w_month=I2cByteRead(0x05); 
    calendar.w_month=BCD2HEX(calendar.w_month);

    /*读取日*/
    calendar.w_date=I2cByteRead(0x04);  
    calendar.w_date=BCD2HEX(calendar.w_date);
 
    /*读取时*/
    calendar.hour=I2cByteRead(0x02); 
    calendar.hour&=0x3f;                   
    calendar.hour=BCD2HEX(calendar.hour);

    /*读取分*/
    calendar.min=I2cByteRead(0x01);
    calendar.min=BCD2HEX(calendar.min);
    
		/*读取秒*/
    calendar.sec=I2cByteRead(0x00);
    calendar.sec=BCD2HEX(calendar.sec);
}			







