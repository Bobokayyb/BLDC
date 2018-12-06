#ifndef USER_FAULT_H
#define USER_FAULT_H

#include"user_adc.h"


#define over_cur_max 400  //·Å´ó100±¶
#define over_volt_max 3600
#define under_volt_max 1300


struct fault_bit
{
	char over_cur          :1;
	char over_vol          :1;
	char under_vol         :1;
	char bldc_power_err    :1;
	char HALL_err          :1;
	char Motor_UVW_err     :1;
	char Start_err         :1;
	char Initial_fault     :1;
};
union fault_def
{
	struct fault_bit bit;
	char byte;
};

extern union fault_def fault_word;

extern void FaultDetect_1ms(void);


#endif
