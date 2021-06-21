#ifndef        _DS3231_H_
#define        _DS3231_H_

#include "sys.h"
#include "delay.h"

///*端口设置*/
//#define IIC_SCL         PBout(6)       //SCLK  时钟   
//#define IIC_SDA         PBout(7)       //SDA   写数据  
//#define        READ_SDA        PBin(7)                                 //SDA   读数据

////IO方向设置
//#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;/*GPIOB->ODR|=1<<7;*/}                        //设置SDA口为上拉输入
//#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}                                                        //设置SDA口为推挽输出


/*端口设置*/
#define IIC_SCL         PBout(6)       //SCLK  时钟   
#define IIC_SDA         PBout(7)       //SDA   写数据  
#define        READ_SDA        PBin(7)                                 //SDA   读数据

//IO方向设置


#define SDA_IN()  {GPIOB->MODER&=~(3<<(7*2));GPIOB->MODER|=0<<7*2;}	//PH5输入模式
#define SDA_OUT() {GPIOB->MODER&=~(3<<(7*2));GPIOB->MODER|=1<<7*2;} //PH5输出模式                       //设置SDA口为上拉输入
 

//DS3231初始宏设置
#define DS3231_WriteAddress 0xD0    //器件写地址 
#define DS3231_ReadAddress  0xD1    //器件读地址
#define DS3231_SECOND       0x00    //秒
#define DS3231_MINUTE       0x01    //分
#define DS3231_HOUR         0x02    //时
#define DS3231_WEEK         0x03    //星期
#define DS3231_DAY          0x04    //日
#define DS3231_MONTH        0x05    //月
#define DS3231_YEAR         0x06    //年
//闹铃1            
#define DS3231_SALARM1ECOND 0x07    //秒
#define DS3231_ALARM1MINUTE 0x08    //分
#define DS3231_ALARM1HOUR   0x09    //时
#define DS3231_ALARM1WEEK   0x0A    //星期/日
//闹铃2
#define DS3231_ALARM2MINUTE 0x0b    //分
#define DS3231_ALARM2HOUR   0x0c    //时
#define DS3231_ALARM2WEEK   0x0d    //星期/日
#define DS3231_CONTROL      0x0e    //控制寄存器
#define DS3231_STATUS       0x0f    //状态寄存器
#define BSY                 2       //忙
#define OSF                 7       //振荡器停止标志
#define DS3231_XTAL         0x10    //晶体老化寄存器
#define DS3231_TEMPERATUREH 0x11    //温度寄存器高字节(8位)
#define DS3231_TEMPERATUREL 0x12    //温度寄存器低字节(高2位) 


//IIC所有操作函数
void         IIC_Init(void);                        //初始化IIC的IO口                                 
void         IIC_Start(void);                                //发送IIC开始信号
void         IIC_Stop(void);                                          //发送IIC停止信号
void         IIC_Send_Byte(u8 txd);                        //IIC发送一个字节
u8                 IIC_Read_Byte(u8);                                //IIC读取一个字节
u8                 IIC_Wait_Ack(void);                         //IIC等待ACK信号
void         IIC_Ack(void);                                        //IIC发送ACK信号
void         IIC_NAck(void);                                        //IIC不发送ACK信号


u8                 BCD2_Hex(u8);
u8                 HEX2_Bcd(u8);
u8         DS3231_WriteByte(u8,u8);
u8                 DS3231_ReadCurrent(void);
u8                 DS3231_ReadRandom(u8);
u8         DS3231_ModifyTime(u8,u8,u8,u8,u8,u8);
u8         DS3231_ReadByte(u8 ReadAddr,u8* Receive);
void       gettime(void);

typedef struct 
{
	u8 hour;
	u8 min;
	u8 sec;			
	u8 w_year;
	u8  w_month;
	u8  w_date;
	u8  week;		 
}_calendar_obj;					 
extern _calendar_obj calendar;	//日历结构体

extern u8 const mon_table[12];	//月份日期数据表

void DS3231_Init(void);
void get_show_time(void);  
void DS3231_Set(u8 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);//设置时间

#endif


