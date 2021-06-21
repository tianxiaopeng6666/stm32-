#include "timer.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F429开发板
//定时器驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2016/1/6
//版本：V1.2
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
//修改说明
//V1.1 20160106
//新增TIM3_PWM_Init函数,用于PWM输出 
//V1.2 20160106
//新增TIM5_CH1_Cap_Init函数,用于输入捕获
////////////////////////////////////////////////////////////////////////////////// 

TIM_HandleTypeDef TIM3_Handler;         //定时器3PWM句柄 
TIM_OC_InitTypeDef TIM3_CH4Handler;	    //定时器3通道4句柄


/***************************************************************************
****************************************************************************
  下面是PWM输出实验相关函数源码
****************************************************************************
****************************************************************************/
 
//TIM3 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM3_PWM_Init(u16 arr,u16 psc)
{ 
    TIM3_Handler.Instance=TIM3;            //定时器3
    TIM3_Handler.Init.Prescaler=psc;       //定时器分频
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//向上计数模式
    TIM3_Handler.Init.Period=arr;          //自动重装载值
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM3_Handler);       //初始化PWM
    
    TIM3_CH4Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM3_CH4Handler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，
                                            //默认比较值为自动重装载值的一半,即占空比为50%
    TIM3_CH4Handler.OCPolarity=TIM_OCPOLARITY_LOW; //输出比较极性为低 
    HAL_TIM_PWM_ConfigChannel(&TIM3_Handler,&TIM3_CH4Handler,TIM_CHANNEL_4);//配置TIM3通道4
    HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_4);//开启PWM通道4
}

//定时器底层驱动，时钟使能，引脚配置
//此函数会被HAL_TIM_PWM_Init()调用
//htim:定时器句柄
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;
		
	__HAL_RCC_TIM3_CLK_ENABLE();			//使能定时器3
    __HAL_RCC_GPIOB_CLK_ENABLE();			//开启GPIOB时钟
	
    GPIO_Initure.Pin=GPIO_PIN_1;           	//PB1
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//复用推完输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
	GPIO_Initure.Alternate= GPIO_AF2_TIM3;	//PB1复用为TIM3_CH4
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
}


//设置TIM通道4的占空比
//compare:比较值
void TIM_SetTIM3Compare4(u32 compare)
{
	TIM3->CCR4=compare;
}

//获取TIM捕获/比较寄存器值
u32 TIM_GetTIM3Capture4(void)
{
	return  HAL_TIM_ReadCapturedValue(&TIM3_Handler,TIM_CHANNEL_4);
}


/***************************************************************************
****************************************************************************
  下面是输入捕获相关源码实验相关函数源码
****************************************************************************
****************************************************************************/


TIM_HandleTypeDef TIM5_Handler;         //定时器5句柄


//定时器5通道1输入捕获配置
//arr：自动重装值(TIM2,TIM5是32位的!!)
//psc：时钟预分频数
void TIM5_CH3_Cap_Init(u32 arr,u16 psc)
{  
    TIM_IC_InitTypeDef TIM5_CH3Config;  
    
    TIM5_Handler.Instance=TIM5;                          //通用定时器5
    TIM5_Handler.Init.Prescaler=psc;                     //分频系数
    TIM5_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM5_Handler.Init.Period=arr;                        //自动装载值
    TIM5_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频银子
    HAL_TIM_IC_Init(&TIM5_Handler);//初始化输入捕获时基参数
    
    TIM5_CH3Config.ICPolarity=TIM_ICPOLARITY_RISING;    //上升沿捕获
    TIM5_CH3Config.ICSelection=TIM_ICSELECTION_DIRECTTI;//映射到TI1上
    TIM5_CH3Config.ICPrescaler=TIM_ICPSC_DIV1;          //配置输入分频，不分频
    TIM5_CH3Config.ICFilter=0;                          //配置输入滤波器，不滤波
    HAL_TIM_IC_ConfigChannel(&TIM5_Handler,&TIM5_CH3Config,TIM_CHANNEL_3);//配置TIM5通道1
	
    HAL_TIM_IC_Start_IT(&TIM5_Handler,TIM_CHANNEL_3);   //开启TIM5的捕获通道1，并且开启捕获中断
    __HAL_TIM_ENABLE_IT(&TIM5_Handler,TIM_IT_UPDATE);   //使能更新中断
}


//定时器5底层驱动，时钟使能，引脚配置
//此函数会被HAL_TIM_IC_Init()调用
//htim:定时器5句柄
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_TIM5_CLK_ENABLE();            //使能TIM5时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();			//开启GPIOA时钟
	
    GPIO_Initure.Pin=GPIO_PIN_2;            //PA0
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;      //复用推挽输出
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    GPIO_Initure.Alternate=GPIO_AF2_TIM5;   //PA0复用为TIM5通道1
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);

    HAL_NVIC_SetPriority(TIM5_IRQn,2,0);    //设置中断优先级，抢占优先级2，子优先级0
    HAL_NVIC_EnableIRQ(TIM5_IRQn);          //开启ITM5中断通道  
}


//捕获状态
//[7]:0,没有成功的捕获;1,成功捕获到一次.
//[6]:0,还没捕获到低电平;1,已经捕获到低电平了.
//[5:0]:捕获低电平后溢出的次数(对于32位定时器来说,1us计数器加1,溢出时间:4294秒)
u8  TIM5CH3_CAPTURE_STA=0;	//输入捕获状态		    				
u32	TIM5CH3_CAPTURE_VAL;	//输入捕获值(TIM2/TIM5是32位)


//定时器5中断服务函数
void TIM5_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM5_Handler);//定时器共用处理函数
}
 

//定时器更新中断（计数溢出）中断处理回调函数， 该函数在HAL_TIM_IRQHandler中会被调用
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//更新中断（溢出）发生时执行
{
	
	if((TIM5CH3_CAPTURE_STA&0X80)==0)//还未成功捕获
	{
			if(TIM5CH3_CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((TIM5CH3_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
				{
					TIM5CH3_CAPTURE_STA|=0X80;		//标记成功捕获了一次
					TIM5CH3_CAPTURE_VAL=0XFFFFFFFF;
				}else TIM5CH3_CAPTURE_STA++;
			}	 
	}		
}


//定时器输入捕获中断处理回调函数，该函数在HAL_TIM_IRQHandler中会被调用
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)//捕获中断发生时执行
{
	if((TIM5CH3_CAPTURE_STA&0X80)==0)//还未成功捕获
	{
		if(TIM5CH3_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
			{	  			
				TIM5CH3_CAPTURE_STA|=0X80;		//标记成功捕获到一次高电平脉宽
                TIM5CH3_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM5_Handler,TIM_CHANNEL_3);//获取当前的捕获值.
                TIM_RESET_CAPTUREPOLARITY(&TIM5_Handler,TIM_CHANNEL_3);   //一定要先清除原来的设置！！
                TIM_SET_CAPTUREPOLARITY(&TIM5_Handler,TIM_CHANNEL_3,TIM_ICPOLARITY_RISING);//配置TIM5通道1上升沿捕获
			}else  								//还未开始,第一次捕获上升沿
			{
				TIM5CH3_CAPTURE_STA=0;			//清空
				TIM5CH3_CAPTURE_VAL=0;
				TIM5CH3_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
				__HAL_TIM_DISABLE(&TIM5_Handler);        //关闭定时器5
				__HAL_TIM_SET_COUNTER(&TIM5_Handler,0);
				TIM_RESET_CAPTUREPOLARITY(&TIM5_Handler,TIM_CHANNEL_3);   //一定要先清除原来的设置！！
				TIM_SET_CAPTUREPOLARITY(&TIM5_Handler,TIM_CHANNEL_3,TIM_ICPOLARITY_FALLING);//定时器5通道1设置为下降沿捕获
				__HAL_TIM_ENABLE(&TIM5_Handler);//使能定时器5
			}		    
	}		
	
}






