#ifndef USER_POSITION_H
#define USER_POSITION_H

#include"stm32f10x.h"
#include"stm32f10x_conf.h"
#include"sys.h"
#include"user_foc.h"
#include"motor_parameter.h"
#include"math.h"


//上管PWM，下管高低电平
//上管1开。0关
//下管1开，0关
//BLDC
#define PWM_A_DOWN	PAout(7)
#define PWM_B_DOWN	PBout(0)
#define PWM_C_DOWN	PBout(1)
#define HALL_A		PBin(6)
#define HALL_B		PBin(7)
#define HALL_C		PBin(8)



//下面这些定义用于PWM控制桥开断
#define DOWM_ON		1
#define DOWM_OFF	0

#define TIM1_A_UP(PWM_V)\
{TIM_SetCompare1(TIM1,PWM_V);TIM_SetCompare2(TIM1,0);TIM_SetCompare3(TIM1,0);}
#define TIM1_A_DOWN		\
{PWM_A_DOWN=DOWM_ON;PWM_B_DOWN=DOWM_OFF;PWM_C_DOWN=DOWM_OFF;}

#define TIM1_B_UP(PWM_V)\
{TIM_SetCompare1(TIM1,0);TIM_SetCompare2(TIM1,PWM_V);TIM_SetCompare3(TIM1,0);}
#define TIM1_B_DOWN		\
{PWM_A_DOWN=DOWM_OFF;PWM_B_DOWN=DOWM_ON;PWM_C_DOWN=DOWM_OFF;}

#define TIM1_C_UP(PWM_V)\
{TIM_SetCompare1(TIM1,0);TIM_SetCompare2(TIM1,0);TIM_SetCompare3(TIM1,PWM_V);}
#define TIM1_C_DOWN		\
{PWM_A_DOWN=DOWM_OFF;PWM_B_DOWN=DOWM_OFF;PWM_C_DOWN=DOWM_ON;}

#define TIM1_ABC_OFF	\
{PWM_A_DOWN=DOWM_OFF;PWM_B_DOWN=DOWM_OFF;PWM_C_DOWN=DOWM_OFF;}

#define BCDL_AB_ON(PWM_V)	 {TIM1_A_UP(PWM_V);TIM1_B_DOWN;}//只开A的上管，B的下管，其他无效
#define BCDL_AC_ON(PWM_V)	 {TIM1_A_UP(PWM_V);TIM1_C_DOWN;}
#define BCDL_BC_ON(PWM_V)	 {TIM1_B_UP(PWM_V);TIM1_C_DOWN;}
#define BCDL_BA_ON(PWM_V)	 {TIM1_B_UP(PWM_V);TIM1_A_DOWN;}
#define BCDL_CA_ON(PWM_V)	 {TIM1_C_UP(PWM_V);TIM1_A_DOWN;}
#define BCDL_CB_ON(PWM_V)	 {TIM1_C_UP(PWM_V);TIM1_B_DOWN;}
#define BCDL_ABC_OFF {TIM1_A_UP(0);TIM1_B_UP(0);TIM1_C_UP(0);TIM1_ABC_OFF;}

typedef struct
{
	u8 step_HALL;
	u8 step;  //电机角度旋转位置
	u8 step_old;
	u8 step_old_old;
	u16 start_cnt;   //启动计数值
	u16 MT_cnt_old;
	u16 MT_cnt_delta;
	u16 MT_cnt_delta_limit;
	u16 Speed_Cnt;   //记录电机旋转次数
	u16 Speed_Cnt_temp;
	u16 Speed_cnt_clear;
	s16 Speed_Dir;
	u16 u16_Motor_Speed;  //电机真实转速
	u16 u16_Motor_Speed_MT;  //电机真实转速
	u16 u16_Motor_Speed_MT_old;
	u16 u16_Motor_Speed_MT_limit;
	s16 s16_Motor_Speed_Filter;
	s16 s16_Motor_Speed_MT_Real;
	s32 s32_Motor_Position;
	_iq s32_Motor_Position_Q;
}HALL_POSITION;
#define Hall_PositionDefault {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}

extern HALL_POSITION Hall_Position_t;


extern void POSITION_INIT(void);
extern void POSITION_Configuration(void);
extern void HALL_Position_Deal(void);
extern void HALL_PWM_Deal(void);
extern void Speed_Calc(void);
extern void Speed_Filt(void);
extern void test(void);
extern void Position_Calc(void);

#endif

