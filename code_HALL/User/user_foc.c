#ifndef USER_FOC_C
#define USER_FOC_C

#include"user_foc.h"

u16 AD_sref_val = 0;

u16 yyb_set = 0;
u16 yyb_test = 0;

FOC_DATA FOC_Data_t = FOC_DataDefault;

PIReg_t Speed_PI = PIDefault;
MODE Mode_en = Mode_Stop;
MODE_SPEED Mode_Speed_en = Speed_Dir_Plus;


void Foc_Init(void)
{
	FOC_Data_t.s16_Speed_Set = 0;
	FOC_Data_t.motor_Dir = Dir_plus;
//转速环初始化
	Speed_PI.Kp = _IQ(0.2);  //0.1
	Speed_PI.Ki = _IQ(0.004);  //0.002
	Speed_PI.Max = _IQ(0.6);   //720*0.2=144
	Speed_PI.Min = _IQ(-0.6);  //720*0.02=14
	Speed_PI.Ui = 0;
	Speed_PI.Out = 0;
}

void Foc_Reset(void)
{
	Speed_PI.Ui = 0;
	Speed_PI.Out = 0;
}


void Position_Loop(void)
{
	FOC_Data_t.s32_Position_Err_Q = (FOC_Data_t.s32_Position_Set_Q-Hall_Position_t.s32_Motor_Position_Q);

	if(abs(FOC_Data_t.s32_Position_Err_Q) < _IQ(0.2/360))
	{
		FOC_Data_t.s16_Speed_Set = 0;
	}
	else
	{
		FOC_Data_t.s16_Speed_Set = _IQmpy(_IQmpy(_IQ(2.0),(FOC_Data_t.s32_Position_Set_Q-Hall_Position_t.s32_Motor_Position_Q)),Motor_Speed_Base);
		FOC_Data_t.s16_Speed_Set = _IQsat(FOC_Data_t.s16_Speed_Set,6000,-6000);
	}

}


void SpeedControl(void)
{
#if 1  //采用状态机跳转，解决换向问题
	switch(Mode_Speed_en)
	{
	case Speed_Dir_Plus:

		Speed_PI.Max = _IQ(0.4);
		Speed_PI.Min = 0;  //正转时，限制最小占空比为0

		if(FOC_Data_t.s16_Speed_Set < 0)
		{
			if(Hall_Position_t.s16_Motor_Speed_MT_Real < 500)
			{
				Mode_Speed_en = Speed_Dir_Minus;
				Speed_PI.Ui = 0;
				Speed_PI.Out = 0;
			}
			FOC_Data_t.s16_Speed_Set_Ramp_Before = 0;
		}
		else
		{
			FOC_Data_t.s16_Speed_Set_Ramp_Before = FOC_Data_t.s16_Speed_Set;
		}

		break;
	case Speed_Dir_Minus:

		Speed_PI.Max = 0;
		Speed_PI.Min = _IQ(-0.4);
		
		if(FOC_Data_t.s16_Speed_Set > 0)
		{
			if(Hall_Position_t.s16_Motor_Speed_MT_Real > -500)
			{
				Mode_Speed_en = Speed_Dir_Plus;
				Speed_PI.Ui = 0;
				Speed_PI.Out = 0;
			}
			FOC_Data_t.s16_Speed_Set_Ramp_Before = 0;
		}
		else
		{
			FOC_Data_t.s16_Speed_Set_Ramp_Before = FOC_Data_t.s16_Speed_Set;
		}

		break;
	default:
		break;
	}

//增加了一个斜率
	FOC_Data_t.s16_Speed_Set_Ramp = _IQsat(FOC_Data_t.s16_Speed_Set_Ramp_Before,FOC_Data_t.s16_Speed_Set_Ramp+10,FOC_Data_t.s16_Speed_Set_Ramp-10);

	Speed_PI.Ref = _IQ((float)FOC_Data_t.s16_Speed_Set_Ramp_Before/Motor_Speed_Base);
	Speed_PI.Fdb = _IQ((float)Hall_Position_t.s16_Motor_Speed_Filter/Motor_Speed_Base);
	VctPIRegCalcNewAnti(&Speed_PI);
	
	FOC_Data_t.s16_pwm_val = _IQmpy(Speed_PI.Out,PWM_Period);

	if(FOC_Data_t.s16_pwm_val > 5)
	{
		FOC_Data_t.motor_Dir = Dir_plus;
		FOC_Data_t.pwm_val = (u16)FOC_Data_t.s16_pwm_val;
	}
	else if(FOC_Data_t.s16_pwm_val < -5)
	{
		FOC_Data_t.motor_Dir = Dir_minus;
		FOC_Data_t.pwm_val = (u16)(-FOC_Data_t.s16_pwm_val);
	}
	else
	{
		FOC_Data_t.pwm_val = (u16)0;
	}
#else

	Speed_PI.Ref = _IQ((float)FOC_Data_t.s16_Speed_Set/Motor_Speed_Base);
	Speed_PI.Fdb = _IQ((float)Hall_Position_t.s16_Motor_Speed_Filter/Motor_Speed_Base);
	VctPIRegCalcNewAnti(&Speed_PI);
	
	FOC_Data_t.s16_pwm_val = _IQmpy(Speed_PI.Out,PWM_Period);

	
	if(FOC_Data_t.s16_pwm_val > 5)
	{
		FOC_Data_t.motor_Dir = Dir_plus;
		FOC_Data_t.pwm_val = (u16)FOC_Data_t.s16_pwm_val;
	}
	else if(FOC_Data_t.s16_pwm_val < -5)
	{
		FOC_Data_t.motor_Dir = Dir_minus;
		FOC_Data_t.pwm_val = (u16)(-FOC_Data_t.s16_pwm_val);
	}
	else
	{
		FOC_Data_t.pwm_val = (u16)0;
	}
#endif
}

void ModeControl(void)
{
/*故障模式优先级最高*/
	if(fault_word.byte)
	{
		BCDL_ABC_OFF;
		Mode_en = Mode_Fault;
		Foc_Reset();//停止模式下，要对数据清零
		FOC_Data_t.pwm_val = 0;
		Hall_Position_t.start_cnt = 0;  //对启动计数清零
	}
	else
	{
		switch(Mode_en)
		{
			case Mode_Stop:
				BCDL_ABC_OFF;
				Foc_Reset();//停止模式下，要对数据清零
				FOC_Data_t.pwm_val = 0;
				Hall_Position_t.start_cnt = 0;  //对启动计数清零
	
				if(Can_Receive_Cmd_t.Run)
				//if(yyb_set>0)
				{
					Mode_en = Mode_Run;
				}
				break;
			case Mode_Start:

				break;
				
			case Mode_Run:
				//if(FOC_Data_t.pwm_val<50)
					//{FOC_Data_t.pwm_val+=1;}
				Position_Loop();
				SpeedControl();   // 转速环控制


				if(Can_Receive_Cmd_t.Run == 0)
				//if(yyb_set<1)
				{
					Mode_en = Mode_Stop;
					BCDL_ABC_OFF;
				}
				break;
			case Mode_Fault:
				BCDL_ABC_OFF;
				break;
			default:
				break;
		}
	}
}

#endif

