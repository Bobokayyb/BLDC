#ifndef USER_RCC_C
#define USER_RCC_C

#include"user_rcc.h"

void RCC_Configuration(void);

void RCC_Configuration(void)
{
	ErrorStatus HSEStartUpStatus;
	
	// RCC system reset(for debug purpose)
		RCC_DeInit();
	
		// Enable HSE
		RCC_HSEConfig(RCC_HSE_ON);
	
		// Wait till HSE is ready
		HSEStartUpStatus = RCC_WaitForHSEStartUp();
	
		if(HSEStartUpStatus == SUCCESS)
		{
			// Enable Prefetch Buffer启用预取缓存区
			FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
	
			// Flash 2 wait state2个等待状态
			FLASH_SetLatency(FLASH_Latency_2);
	
			// HCLK = SYSCLK
			//AHB=0,不分频
			RCC_HCLKConfig(RCC_SYSCLK_Div1);
	
			// PCLK2 = HCLK
			//APB2不分频72MHz
			RCC_PCLK2Config(RCC_HCLK_Div1);
	
			// PCLK1 = HCLK/2
			//APB1 36MHz
			RCC_PCLK1Config(RCC_HCLK_Div2);
	
			// PLLCLK = 8MHz * 9 = 72 MHz
			RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
	
			// Enable PLL
			RCC_PLLCmd(ENABLE);
	
			// Wait till PLL is ready
			while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
			{
			}
	
			// Select PLL as system clock source
			RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	
			// Wait till PLL is used as system clock source
			while(RCC_GetSYSCLKSource() != 0x08)
			{
			}
		}
		else
		{
			// If HSE fails to start-up, the application will have wrong clock configuration.
			// User can add here some code to deal with this error
	
			// Go to infinite loop
			while (1)
			{
			}
		}

	
}

#endif

