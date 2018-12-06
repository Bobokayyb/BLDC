#ifndef USER_ADC_H
#define USER_ADC_H

#include"stm32f10x.h"
#include"stm32f10x_conf.h"
#include"user_foc.h"
#include"main.h"

extern s16 yyb_test_adc_1;
extern s16 yyb_test_adc_2;
extern s16 yyb_test_adc_3;


typedef struct
{
	s16 Volt_low_Range;
	s16 cur_Range;

	s16 Volt_low;
	s16 cur_A;
	s16 cur_B;
	s16 cur_C;

	s16 cur_A_zero;
	s16 cur_B_zero;
	s16 cur_C_zero;

	_iq cur_DC_Filter;
}ADC_Value;
extern ADC_Value Adc_Value_t;


extern void ADC_user_Init(void);
extern void DMA_Configuration(void);
extern void ADC_Configuration(void);
extern void Adc_ZeroCalc(void);
extern void Get_Adc(void);

#endif

