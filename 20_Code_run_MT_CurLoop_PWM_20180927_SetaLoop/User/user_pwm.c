#ifndef USER_PWM_C
#define USER_PWM_C

#include"user_pwm.h"

void PWM_Configuration(void);

void PWM_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//��λ��Ӧ������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);

	// timer1 _ PWM �Ϲ� A8 A9 A10
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10); 
	// timer1 _ PWM �¹� A7
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //����������¹�ֱ��ͨ��GPIO���ƣ�����Ҫ���ó�
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_7); 	
	// �¹�  B0 B1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1);
	//����TIME1���ָ���
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1, ENABLE); //Timer1������ӳ��  
	GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE); //USART1������ӳ�� 


//72000000/1/1800/2 = 20000Hz      �Գ����ǲ�+ ֻ���������ж�
	TIM_TimeBaseStructure.TIM_Period = (1799); //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =(0); //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1;//TIM_CounterMode_Down;  //TIM���¼���ģʽ
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 1;  //ֻ���������ж�
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 	//����Ч

	//���漸�������Ǹ߼���ʱ���Ż��õ���ͨ�ö�ʱ����������        
	TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCNPolarity_Low;//  TIM_OCNPolarity_High;       //���û������������       
	TIM_OCInitStructure.TIM_OutputNState=TIM_OutputNState_Enable;	//ʹ�ܻ��������         
	TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset;     	//���������״̬         
	TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCNIdleState_Reset;	//�����󻥲������״̬  

	TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Disable;//����ģʽ�����
	TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Disable;//����ģʽ�����ѡ��          
	TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;         //��������        
	TIM_BDTRInitStructure.TIM_DeadTime = 0x00;// 0x90;                       //����ʱ������         
	TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;             //ɲ������ʹ��         
	TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;//ɲ�����뼫��        
	TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;//�Զ����ʹ��          
	TIM_BDTRConfig(TIM1,&TIM_BDTRInitStructure);
	//TIM_SelectOutputTrigger(TIM1,TIM_TRGOSource_Update);

	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR1�ϵ�Ԥװ�ؼĴ���
	
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���

	TIM_OC3Init(TIM1, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR3�ϵ�Ԥװ�ؼĴ���

//	TIM_OC4Init(TIM1, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
//	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR4�ϵ�Ԥװ�ؼĴ���
	
	TIM_ARRPreloadConfig(TIM1, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���

	//PWM�ж�
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

 	TIM_ClearFlag(TIM1,TIM_IT_Update);
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�


	
	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIMx����
	//��������Ǹ߼���ʱ�����еģ����pwm�����         
	TIM_CtrlPWMOutputs(TIM1, ENABLE); //pwm���ʹ�ܣ�һ��Ҫ�ǵô�
	
	//TIM_ForcedOC1Config(TIM1,PWM_U_UP_OFF); //�ر�ͨ��1��� 
	//TIM_ForcedOC2Config(TIM1,PWM_V_UP_OFF); //�ر�ͨ��2��� 
	//TIM_ForcedOC3Config(TIM1,PWM_W_UP_OFF); //�ر�ͨ��3��� 
	
	
}

void PWM_Enable(void)
{
	PCout(13)=1;
}
void PWM_Disable(void)
{
	PCout(13)=0;
}



#endif

