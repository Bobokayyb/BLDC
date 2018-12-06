#ifndef USER_FOC_H
#define USER_FOC_H

#include"stm32f10x.h"
#include"stm32f10x_conf.h"
#include"math.h"
#include"user_position.h"
#include"user_fault.h"
#include"user_pwm.h"
#include"user_can.h"

#define Dir_plus 0
#define Dir_minus 1
#define PWM_Min 30
#define PWM_Max 400


typedef struct
{
	u16 u16_Position_Set;
	_iq s32_Position_Set_Q;
	_iq s32_Position_Err_Q;
	s16 s16_Speed_Set;
	s16 s16_Speed_Set_Ramp_Before;
	s16 s16_Speed_Set_Ramp;
	u16 motor_Dir;
	s16 s16_pwm_val;
	u16 pwm_val;
}FOC_DATA;
#define FOC_DataDefault {0,0,0,0,0,0,0}
extern FOC_DATA FOC_Data_t;

typedef enum
{
	Mode_Stop = 0,
	Mode_Start,
	Mode_Run,
	Mode_Fault,
}MODE;
extern MODE Mode_en;

typedef enum
{
	Speed_Dir_Plus = 0,
	Speed_Dir_Minus,
}MODE_SPEED;
extern MODE_SPEED Mode_Speed_en;


extern void Foc_Init(void);
extern void ModeControl(void);
extern void SpeedControl(void);
extern void Position_Loop(void);

#endif

