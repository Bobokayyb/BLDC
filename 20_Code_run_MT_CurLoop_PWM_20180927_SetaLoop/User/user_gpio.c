#ifndef USER_GPIO_C
#define USER_GPIO_C

#include"user_gpio.h"

void GPIO_Configuration(void);

void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//���������⣬����Ҫ��ӳ��
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3������ӳ��  TIM3_CH2->PB5    

	
	//���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨�� GPIOB.5
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //TIM_CH2
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO

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

