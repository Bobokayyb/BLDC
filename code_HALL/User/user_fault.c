#ifndef USER_FAULT_C
#define USER_FAULT_C

#include"user_fault.h"

union fault_def fault_word = {0};

s16 yyb_test_fault_1 = 0;
s16 yyb_test_fault_2 = 0;
s16 yyb_test_fault_3 = 0;
u16 yyb_test_fault_4 = 0;

s16 yyb_test_fault_5 = 0;
s16 yyb_test_fault_6 = 0;
s16 yyb_test_fault_7 = 0;




void FaultDetect_1ms(void)
{
	if((Adc_Value_t.cur_A > over_cur_max)||(Adc_Value_t.cur_B > over_cur_max)||(Adc_Value_t.cur_C > over_cur_max))
	//if((AD_Digital_Value[0] > 280)||(AD_Digital_Value[1] > 280)||(AD_Digital_Value[2] > 280))
	{
		fault_word.bit.over_cur = 1;
	}
}

#endif
