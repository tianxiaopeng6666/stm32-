/** @mainpage 智慧大棚
*   @section 项目详细描述
*   通过智慧大棚系统的部署，可以实现自动化(或手动控制)大棚内部CO2浓度的平衡，并且在出现异常状态时报警
*		@section 功能描述
*   智能大棚是基于stm32F4开发
*   本功能基采用二氧化碳传感器，热释电传感器、继电器、蜂鸣器、排风扇等传感器执行器。
*/
/**
 ******************************************************************************
 * @file main.c
 * @author T0102
 * @brief  
   本主程序主要初始化各个传感器，并将得到的传感器值进行逻辑判断，实现根据CO2传感器和热释电传感器的输入值来控制执行器继电器的开关状态，也可以通过阿里云进行手动控制继电器开关状态。
**/
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"
#include "oled.h"
#include "BeepJdq.h"
#include "PIR.h"
#include "ds3231.h"

extern u8  TIM5CH3_CAPTURE_STA;		//输入捕获状态		    				
extern u32	TIM5CH3_CAPTURE_VAL;	//输入捕获值 

int main(void)
{
		u8 t=0;/*DS3231计时*/
	  u8 j =0;/*阿里云从串口3输入的继电器开关状态*/
	  u8 mode=0;/*阿里云从串口3输入的控制模式*/
    long long temp=0;/*CO2传感器上升沿占空比*/
	  long long a=0;/*二氧化碳含量*/
		int flag = 0x0F;/*作为一个4位的标志位，对0-1000，1000-1500，1500-的三个范围限定蜂鸣器1s的蜂鸣*/
	  u16 xx;/*收到由串口得到手动控制的继电器开关值*/
	
		/*初始化HAL库*/
    HAL_Init();
	
		/*设置时钟,180Mhz*/
		Stm32_Clock_Init(360,25,2,8);
	
		/*初始化延时函数*/
		delay_init(180);
	
		/*初始化USART*/
    uart_init(115200);
	
		/*初始化串口3波特率为115200*/
		usart3_init(115200);
		
		/*初始化OLED*/
		OLED_Init();
		
		/*初始化继电器蜂鸣器*/
		BeepJDQ_Init();
		
		/*初始化热释电传感器	*/
		PIR_Init();
		
		/*初始化DS3231,首次设置时间，设置完成后注释掉，重新下载一次程序即可掉电保存
		* DS3231_Set(21,05,31,13,16,20);
		*/
    DS3231_Init();
		
		/*以1MHZ的频率计数*/
		TIM5_CH3_Cap_Init(0XFFFFFFFF,90-1);
    while(1)
    {
				  get_show_time();
					if(t!=calendar.sec)
					{
						t=calendar.sec;
						
						/*打印("年月日:");
						*	打印("%ld,%d,%d\r\n",calendar.w_year+2000,calendar.w_month,calendar.w_date);
						*	打印("时分秒:");
						* 打印("%d,%d,%d\r\n",calendar.hour,calendar.min,calendar.sec);*/
						OLED_ShowNum(0,27,calendar.hour,2,16,1);
						OLED_ShowString(17,27,":",16,1);//
						OLED_ShowNum(25,27,calendar.min,2,16,1);
						OLED_ShowNum(0,48,calendar.w_year+2000,4,16,1);
						OLED_ShowChinese(30,48,0,16,1);  //16*16 年
						OLED_ShowNum(46,48,calendar.w_month,2,16,1);
						OLED_ShowChinese(61,48,1,16,1);  //16*16 月
						OLED_ShowNum(77,48,calendar.w_date,2,16,1);
						OLED_ShowChinese(93,48,2,16,1);  //16*16 日
						OLED_Refresh();
					}
					
					/*串口接受一次数据*/
					if(USART3_RX_STA&0X8000)
					{ 
						USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//添加结束符
						printf("%s",USART3_RX_BUF);	//发送到串口
						USART3_RX_STA=0;

						/*xx作为标志位，得到从阿里云传入的功能码 48 49 50 51 即为功能码0 1 2 3，表示为继电器开关状态0 1，模式：自动模式0 手动模式1 */
						xx =(int)USART3_RX_BUF[0];
						if (xx==50)
							mode=0; 
						else if(xx==51)
							mode=1;
						if (xx==48)
							j=0; 
						else if(xx==49)
							j=1;
					}
					
					/*在OLED上显示CO2含量*/
					OLED_ShowString(0,0,"CO2:",16,1);//
					OLED_ShowNum(30,0,a,4,16,1);
					OLED_ShowString(64,0,"ppm",16,1);//
					OLED_Refresh();
					u3_printf("%lld\r\n",a);	//发送命令
					delay_ms(500);
					
					/*CO2的GPIO_PIN_9成功捕获到了一次高电平，接受一次数据
					* 计算溢出时间总和
					* 得到总的高电平时间
					* 开启下一次捕获
					*/
					if(TIM5CH3_CAPTURE_STA&0X80)
					{
						temp=TIM5CH3_CAPTURE_STA&0X3F; 
						temp*=0XFFFFFFFF;
						temp+=TIM5CH3_CAPTURE_VAL;
						a=(temp*1.00/10)*20;
						TIM5CH3_CAPTURE_STA=0;
					}
					
					/*开启自动模式*/
					if (mode==0)
					{
						OLED_ShowString(93,27,"auto",16,1);
						
						/*当CO2含量a小于1000并且进入标志位为1，蜂鸣器响1s，继电器开，并复位进入标志位*/
						if (a<1000&&(flag&0x01)==1)
						{
							Beep=1;	
							JDQ=1;
							flag=0x0E;
							delay_ms(1000);
							Beep=0;	
						}
				
						/*当CO2含量a大于1500并且进入标志位为1，蜂鸣器响1s，继电器关，并复位进入标志位*/
						else if (a>1500&&(flag&0x02)==2)
						{
							Beep=1;	
							JDQ=0;
							flag = 0x0D;
							delay_ms(1000);
							Beep=0;	
						}
				
						/*当CO2含量a大于1000并小于1500，继电器开，如果检测到有人，蜂鸣器报警0.1s，OLED打印有人信息，继电器关闭
						* 如果检测到没人,OLED打印没人信息，继电器继续开启
						*/
						if (a<1500&&a>1000)
						{
							JDQ=1;
							if (PIR_Scan()==1)
							{
								Beep=1;
								
								/*OLED打印16*16的 有 人*/
								OLED_ShowChinese(61,27,6,16,1);
								OLED_ShowChinese(77,27,8,16,1);
								JDQ=0;
								delay_ms(100);
								Beep=0;			
							}
							else if (PIR_Scan()==0)
							{
								/*OLED打印16*16的 没 人*/
							  OLED_ShowChinese(61,27,7,16,1);
								OLED_ShowChinese(77,27,8,16,1);
								JDQ=1;

							}	
						}
						
						/*在整个自动模式中都会检测有没有人，OLED打印相关信息*/
						if (PIR_Scan()==1)
						{
							OLED_ShowChinese(61,27,6,16,1);
							OLED_ShowChinese(77,27,8,16,1);
						}
						else if (PIR_Scan()==0)
						{
						  OLED_ShowChinese(61,27,7,16,1);
							OLED_ShowChinese(77,27,8,16,1);
						}	
					}
					
					/*手动模式 得到继电器输入值，并在OLED上面打印*/
					else if (mode==1)
					{
					   OLED_ShowString(93,27,"man",16,1);//
				     if(j==0)JDQ=0;
					   else if(j==1)JDQ=1;
					}
		}
}


