#ifndef USER_POSITION_C
#define USER_POSITION_C

#include"user_position.h"

u16 yyb_test_positon = 0;
u16 yyb_test_1 = 0;
u16 yyb_test_2 = 0;
u16 yyb_test_3 = 0;


LPFirst_t LPF_speed = LPFDefault;


// 3-2-6-4-5-1 //读数组值后，得到123456
const u8 bldc_hall_tab[8]={0x7F,6,2,1,4,5,3,0xFF};


HALL_POSITION Hall_Position_t = Hall_PositionDefault;

void POSITION_INIT(void)
{
	Hall_Position_t.MT_cnt_delta_limit = 50000;
	Hall_Position_t.u16_Motor_Speed_MT_limit = 20;//初始限制变化率

	
	/*     k = 2*pi*f / fs   = 2*3.14*19.1/1000=0.12               
	f: 截止频率
	fs: 调用频率
	*/
	LPF_speed.Kflt = _IQ(0.12);

	Hall_Position_t.Speed_Dir = 1;

	
}



void POSITION_Configuration(void);



void POSITION_Configuration(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
/*	
	//GPIOB6	  中断线以及中断初始化配置
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource6);
   	EXTI_InitStructure.EXTI_Line=EXTI_Line6;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	//GPIOB7	  中断线以及中断初始化配置
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource7);
   	EXTI_InitStructure.EXTI_Line=EXTI_Line7;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	//GPIOB8	  中断线以及中断初始化配置
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource8);
   	EXTI_InitStructure.EXTI_Line=EXTI_Line8;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn ;	//使能按键所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//子优先级1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
*/	
}

void HALL_Position_Deal(void)
{
	
	u8 hall_val=0;
	if(HALL_A==1)
		hall_val|=4;
	if(HALL_B==1)
		hall_val|=1;
	if(HALL_C==1)
		hall_val|=2;
	Hall_Position_t.step_HALL = bldc_hall_tab[hall_val]; 
	if(FOC_Data_t.motor_Dir == Dir_plus) //正转
	{
		Hall_Position_t.step = Hall_Position_t.step_HALL + 1;
	}
	else  
	{
		Hall_Position_t.step = Hall_Position_t.step_HALL - 1;
		if(Hall_Position_t.step < 1)
		{
			Hall_Position_t.step = 6;
		}
		 //减两次。。才是反转hall位置
		Hall_Position_t.step -= 1;
		if(Hall_Position_t.step < 1)
		{
			Hall_Position_t.step = 6;
		}
	}  
	if(Hall_Position_t.step > 6)
	{
		Hall_Position_t.step = 1;
	}
	else if(Hall_Position_t.step < 1)
	{
		Hall_Position_t.step = 6;
	}

	if(Mode_en == Mode_Start)
	{
		Hall_Position_t.start_cnt++;
		if(Hall_Position_t.start_cnt > 3)
		{
			Mode_en = Mode_Run;
			Hall_Position_t.start_cnt = 0;
		}
	}

	if(Hall_Position_t.step_HALL == Hall_Position_t.step_old)
	{
		if(Hall_Position_t.Speed_cnt_clear < 11000)
		{
			Hall_Position_t.Speed_cnt_clear++;
		}
	}
	else
	{
		Hall_Position_t.Speed_cnt_clear = 0;
		Hall_Position_t.Speed_Cnt_temp ++;
		Hall_Position_t.Speed_Cnt ++;  //每发送step变化，计数一次


//根据旋转方向来判断转速的正负
		if(((Hall_Position_t.step_HALL > 2)&&(Hall_Position_t.step_HALL < 5))&&((Hall_Position_t.step_old > 2)&&(Hall_Position_t.step_old < 5)))
		{
			if(Hall_Position_t.step_HALL > Hall_Position_t.step_old)
			{
				Hall_Position_t.Speed_Dir = -1;
			}
			else if(Hall_Position_t.step_HALL < Hall_Position_t.step_old)
			{
				Hall_Position_t.Speed_Dir = 1;
			}
		}
	}
	
	if(Hall_Position_t.Speed_Cnt_temp > 4)
	{
		Hall_Position_t.Speed_Cnt_temp = 0;
		
		Hall_Position_t.MT_cnt_delta = TIM4->CNT - Hall_Position_t.MT_cnt_old;
		Hall_Position_t.MT_cnt_old = TIM4->CNT;
		/*
		if(Hall_Position_t.MT_cnt_delta < Hall_Position_t.MT_cnt_delta_limit)
		{
			Hall_Position_t.MT_cnt_delta_limit = Hall_Position_t.MT_cnt_delta_limit<<1;
		}
		else
		{
			Hall_Position_t.MT_cnt_delta_limit = Hall_Position_t.MT_cnt_delta_limit>>1;
		}
		*/

		
		// time4的计数频率100kHz       
		// 5*60*100000/6/2=2500000
		//5是speed_cnt_temp
		//60是min转换成秒
		//100000是time4计数频率
		//6是step数
		//2是极对数
		Hall_Position_t.u16_Motor_Speed_MT = (u16)((u32)2500000/Hall_Position_t.MT_cnt_delta);
		if(abs(Hall_Position_t.u16_Motor_Speed_MT - Hall_Position_t.u16_Motor_Speed_MT_old) > Hall_Position_t.u16_Motor_Speed_MT_limit)
		{
			Hall_Position_t.u16_Motor_Speed_MT = Hall_Position_t.u16_Motor_Speed_MT_old + Hall_Position_t.u16_Motor_Speed_MT_limit;
			if(Hall_Position_t.u16_Motor_Speed_MT_limit < 200)
			{
				Hall_Position_t.u16_Motor_Speed_MT_limit = Hall_Position_t.u16_Motor_Speed_MT_limit<<1;
			}
		}
		else
		{
			if(Hall_Position_t.u16_Motor_Speed_MT_limit > 20)
			{
				Hall_Position_t.u16_Motor_Speed_MT_limit = Hall_Position_t.u16_Motor_Speed_MT_limit>>1;
			}
		}
		Hall_Position_t.u16_Motor_Speed_MT_old = Hall_Position_t.u16_Motor_Speed_MT;
		
	}
//在判断500ms内都没有发生转动，那么转速慢慢减到0
	if(Hall_Position_t.Speed_cnt_clear > 5000)
	{
		if(Hall_Position_t.u16_Motor_Speed_MT > 0)
		Hall_Position_t.u16_Motor_Speed_MT--;
	}


//将转速乘以方向，得到最终带符号的转速
	Hall_Position_t.s16_Motor_Speed_MT_Real = (s16)(Hall_Position_t.Speed_Dir*(s16)Hall_Position_t.u16_Motor_Speed_MT);

	Hall_Position_t.step_old_old = Hall_Position_t.step_old;
	Hall_Position_t.step_old = Hall_Position_t.step_HALL;
}

void HALL_PWM_Deal(void)
{
	switch(Hall_Position_t.step)
	{
		case 1:  {BCDL_AB_ON(FOC_Data_t.pwm_val);yyb_test_positon=1;break;}
		case 2:  {BCDL_AC_ON(FOC_Data_t.pwm_val);yyb_test_positon=2;break;}
		case 3:  {BCDL_BC_ON(FOC_Data_t.pwm_val);yyb_test_positon=3;break;}
		case 4:  {BCDL_BA_ON(FOC_Data_t.pwm_val);yyb_test_positon=4;break;}
		case 5:  {BCDL_CA_ON(FOC_Data_t.pwm_val);yyb_test_positon=5;break;}
		case 6:  {BCDL_CB_ON(FOC_Data_t.pwm_val);yyb_test_positon=6;break;}
		default: {BCDL_ABC_OFF;yyb_test_positon=7;break;}
	}	
}

void test(void)
{
	if(yyb_test_2)
	{
		if(yyb_test_1 < 6)
		{
			yyb_test_1++;
		}
		else
		{
			yyb_test_1=1;
		}
		Hall_Position_t.step=yyb_test_1;
		if(FOC_Data_t.pwm_val< yyb_test_3)
			{FOC_Data_t.pwm_val+=1;}
		
		PWM_Enable();
		HALL_PWM_Deal();
		Can_Data_Transmit[6] = (u8)(yyb_test_1);
	}
	else
	{
		yyb_test_1=0;
		FOC_Data_t.pwm_val=0;
		PWM_Disable();
		BCDL_ABC_OFF;
	}
}



//转速计算放在1ms定时器中进行
void Speed_Calc(void)
{
	Hall_Position_t.u16_Motor_Speed = (u16)(Hall_Position_t.Speed_Cnt*M_1MIN_X);
	Hall_Position_t.Speed_Cnt = 0;  //计算完转速后，要将计数值清零
}

void Speed_Filt(void)
{
	LPF_speed.In = _IQ(Hall_Position_t.s16_Motor_Speed_MT_Real/Motor_Speed_Base);
	LPFirstCalc(&LPF_speed);
	Hall_Position_t.s16_Motor_Speed_Filter = (s16)_IQmpy(LPF_speed.Out,Motor_Speed_Base);
}


void Position_Calc(void)
{
	//speed/200/60/1000 *360*1000
	//200表示转速比
	//60代表1min=60s
	//1000是 该积分放在1ms里面
	//360代表360度
	//1000代表精度
	Hall_Position_t.s32_Motor_Position += (Hall_Position_t.s16_Motor_Speed_Filter*3/100);
	if(Hall_Position_t.s32_Motor_Position < 0)
	{
		Hall_Position_t.s32_Motor_Position = 0;
	}
	else if(Hall_Position_t.s32_Motor_Position > 360000)
	{
		Hall_Position_t.s32_Motor_Position = 360000;
	}
	Hall_Position_t.s32_Motor_Position_Q = _IQ((float)Hall_Position_t.s32_Motor_Position/360000.0);
}


#endif

