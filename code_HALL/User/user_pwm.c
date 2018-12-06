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
	
	//复位对应的外设
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);

	// timer1 _ PWM 上管 A8 A9 A10
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10); 
	// timer1 _ PWM 下管 A7
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出，下管直接通过GPIO控制，所以要设置成
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_7); 	
	// 下管  B0 B1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1);
	//这里TIME1部分复用
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1, ENABLE); //Timer1部分重映射  
	GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE); //USART1部分重映射 


//72000000/1/1800/2 = 20000Hz      对称三角波+ 只触发下溢中断
	TIM_TimeBaseStructure.TIM_Period = (1799); //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =(0); //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1;//TIM_CounterMode_Down;  //TIM向下计数模式
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 1;  //只触发下溢中断
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 	//高有效

	//下面几个参数是高级定时器才会用到，通用定时器不用配置        
	TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCNPolarity_Low;//  TIM_OCNPolarity_High;       //设置互补端输出极性       
	TIM_OCInitStructure.TIM_OutputNState=TIM_OutputNState_Enable;	//使能互补端输出         
	TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset;     	//死区后输出状态         
	TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCNIdleState_Reset;	//死区后互补端输出状态  

	TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Disable;//运行模式下输出
	TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Disable;//空闲模式下输出选择          
	TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;         //锁定设置        
	TIM_BDTRInitStructure.TIM_DeadTime = 0x00;// 0x90;                       //死区时间设置         
	TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;             //刹车功能使能         
	TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;//刹车输入极性        
	TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;//自动输出使能          
	TIM_BDTRConfig(TIM1,&TIM_BDTRInitStructure);
	//TIM_SelectOutputTrigger(TIM1,TIM_TRGOSource_Update);

	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIMx在CCR1上的预装载寄存器
	
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器

	TIM_OC3Init(TIM1, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIMx在CCR3上的预装载寄存器

//	TIM_OC4Init(TIM1, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
//	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIMx在CCR4上的预装载寄存器
	
	TIM_ARRPreloadConfig(TIM1, ENABLE); //使能TIMx在ARR上的预装载寄存器

	//PWM中断
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

 	TIM_ClearFlag(TIM1,TIM_IT_Update);
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断


	
	TIM_Cmd(TIM1, ENABLE);  //使能TIMx外设
	//下面这句是高级定时器才有的，输出pwm必须打开         
	TIM_CtrlPWMOutputs(TIM1, ENABLE); //pwm输出使能，一定要记得打开
	
	//TIM_ForcedOC1Config(TIM1,PWM_U_UP_OFF); //关闭通道1输出 
	//TIM_ForcedOC2Config(TIM1,PWM_V_UP_OFF); //关闭通道2输出 
	//TIM_ForcedOC3Config(TIM1,PWM_W_UP_OFF); //关闭通道3输出 
	
	
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

