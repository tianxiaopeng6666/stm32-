//  功能描述   : 
//              说明: 
//              ----------------------------------------------------------------
//              GND    电源地
//              VCC    接5V或3.3v电源
//              SCL    接PB10（SCL）
//              SDA    接PB11（SDA）
//              RES    PE14
//              ----------------------------------------------------------------

#include "main.h"
#include "stm32f4xx_hal.h"
#include "oled.h"
#include "bmp.h"
void SystemClock_Config(void);
int main()
{
	u8 t=' ';
	HAL_Init();
	SystemClock_Config();
	OLED_Init();
	LED_ON;
	while(1)
	{
		OLED_ShowPicture(0,0,128,64,BMP1,1);
		OLED_Refresh();
		HAL_Delay(500);
		OLED_Clear();
		OLED_ShowChinese(0,0,0,16,1);//中
		OLED_ShowChinese(18,0,1,16,1);//景
		OLED_ShowChinese(36,0,2,16,1);//园
		OLED_ShowChinese(54,0,3,16,1);//电
		OLED_ShowChinese(72,0,4,16,1);//子
		OLED_ShowChinese(90,0,5,16,1);//技
		OLED_ShowChinese(108,0,6,16,1);//术
		OLED_ShowString(8,16,"ZHONGJINGYUAN",16,1);
		OLED_ShowString(20,32,"2014/05/01",16,1);
		OLED_ShowString(0,48,"ASCII:",16,1);  
		OLED_ShowString(63,48,"CODE:",16,1);
		OLED_ShowChar(48,48,t,16,1);//显示ASCII字符	   
		t++;
		if(t>'~')t=' ';
		OLED_ShowNum(103,48,t,3,16,1);
		OLED_Refresh();
		HAL_Delay(500);
		OLED_Clear();
		OLED_ShowChinese(0,0,0,16,1);  //16*16 中
	  OLED_ShowChinese(16,0,0,24,1); //24*24 中
		OLED_ShowChinese(24,20,0,32,1);//32*32 中
	  OLED_ShowChinese(64,0,0,64,1); //64*64 中
		OLED_Refresh();
	  HAL_Delay(500);
  	OLED_Clear();
		OLED_ShowString(0,0,"ABC",8,1);//6*8 “ABC”
		OLED_ShowString(0,8,"ABC",12,1);//6*12 “ABC”
	  OLED_ShowString(0,20,"ABC",16,1);//8*16 “ABC”
		OLED_ShowString(0,36,"ABC",24,1);//12*24 “ABC”
	  OLED_Refresh();
		HAL_Delay(500);
		OLED_ScrollDisplay(11,4,1);
	}
}

void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 20;
//  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
 // RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
		
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {

  }

    /**Configure the main internal regulator output voltage 
    */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {

  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}











