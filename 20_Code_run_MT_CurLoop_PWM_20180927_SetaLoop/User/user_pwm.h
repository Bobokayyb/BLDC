#ifndef USER_PWM_H
#define USER_PWM_H

#include"stm32f10x.h"
#include"stm32f10x_conf.h"
#include"sys.h"



#define PWM_freq 20000
#define PWM_Prescaler 2
#define PWM_Period 72000000/PWM_freq/PWM_Prescaler

extern void PWM_Configuration(void);
extern void PWM_Enable(void);
extern void PWM_Disable(void);


#endif

