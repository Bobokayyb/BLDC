#ifndef USER_GPIO_C
#define USER_GPIO_C

#include"user_gpio.h"

void GPIO_Configuration(void);

void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//开发板特殊，所以要重映射
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3部分重映射  TIM3_CH2->PB5    

	
	//设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形 GPIOB.5
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //TIM_CH2
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO

}

void Key_TxRx_Init(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}



#endif

