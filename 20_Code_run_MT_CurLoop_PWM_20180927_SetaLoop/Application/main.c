/*
*Copyright (c) 2018
*File : main.c
*2018-08-09
*/

#include"main.h"

u16 gu16_time_1ms_Flag = 0;
u16 gu16_time_bngCnt = 0;
u16 gu16_InitOK_Flag = 0;

u16 test_yyb_main = 0;
u16 test_yyb_main1 = 0;
u16 test_yyb_main_ISR = 0;
u16 test_yyb_main1_ISR = 0;


int main(void);
void Task_1ms(void);
void Task_10ms(void);
void Task_100ms(void);


int main(void)
{

	//RCC_Configuration();
	//时钟初始化芯片会自动进行
	//调用SystemInit()
	delay_init();
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	//NVIC_Configuration();
	
	//GPIO_Configuration();
	
	PWM_Configuration();   //PWM初始化，使用TIM1
	TIME_Configuration();  //后台1ms时钟初始化，用来跑后台程序
	POSITION_Configuration();  //HALL角度采样初始化
	
	DMA_Configuration();  //DMA直接采样ADC初始化
	ADC_Configuration();
	delay_ms(10);

	CAN_Configuration();
	DRV8305_Init();
	PWM_Enable();
	RS485_Init(115200);
	fault_word.bit.Initial_fault = DRV8305_Set();

    //控制变量初始化
	ADC_user_Init();
	Foc_Init();
	POSITION_INIT();

	delay_ms(1000);
	Adc_ZeroCalc();



	while(1)
	{
		if(gu16_time_1ms_Flag == 1)
		{
			gu16_time_1ms_Flag = 0;
			if(++gu16_time_bngCnt >= 100)
			{
				gu16_time_bngCnt = 0;
			}
			Task_1ms();
			Task_10ms();
			Task_100ms();
		}
	}
}

void Task_1ms(void)
{
	FaultDetect_1ms();
	Speed_Filt();
	ModeControl();
	Position_Calc();
	
	if(test_yyb_main < 1000)
		{test_yyb_main++;}
	else
		{test_yyb_main = 0; test_yyb_main1++;}
}
void Task_10ms(void)
{
	switch(gu16_time_bngCnt%10)
	{
	case 0:
		if(0==Can_Receive_Msg())
		{}
		break;
	case 1:
		if(0==Can_Send_Msg_14())
		{}
		break;
	case 3:
		if(0==Can_Send_Msg_13())
		{}
		break;
	default:
		break;
	}
}

void Task_100ms(void)
{
	static u16 time_1s_cnt = 0;
	switch(gu16_time_bngCnt)
	{
	case 8:
		//fault_word.bit.Initial_fault = DRV8305_Set();			
		break;
	case 19:
		break;
	case 27:
		if(time_1s_cnt < 10)
		{
			time_1s_cnt++;
		}
		else
		{
			time_1s_cnt = 0;
			//Speed_Calc();   //  1秒计算一次
			//test();
			//RS485_Send_Data();
		}
		break;
	default:
		break;
		
	}
}




//PWM中断函数
void TIM1_UP_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		test_yyb_main_ISR = TIM4->CNT;
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  //清除TIMx更新中断标志 

		if(gu16_InitOK_Flag == 1)
		{
			Get_Adc();
			HALL_Position_Deal();
			if(fault_word.byte || (Mode_en == Mode_Stop))
			{
				BCDL_ABC_OFF;
			}
			else
			{
				HALL_PWM_Deal();
			}
		}
//测试中断执行时间，不能超时
		test_yyb_main1_ISR = TIM4->CNT - test_yyb_main_ISR;

	}
}


