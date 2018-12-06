#ifndef MAIN_H
#define MAIN_H

#include"head.h"

extern u16 gu16_time_1ms_Flag;
extern u16 gu16_InitOK_Flag;

// IO Define
//LED
#define LED1 		PBout(5)	
#define LED2 		PBout(4)
#define LED3 		PBout(3)
#define LED4 		PDout(2)

//TM1624
#define DIO_OUT 	PAout(11)	
#define CLK_OUT 	PAout(12)
#define STB_OUT 	PAout(15)

//KEY
#define KEY_DAT		PCin(9) // 
#define KEY_EN		PBin(2) // 

//bit_byte
struct bit_def {
    char  b0:1;
    char  b1:1;
    char  b2:1;
    char  b3:1;
    char  b4:1;
    char  b5:1;
    char  b6:1;
    char  b7:1;
};
union byte_def{
  struct bit_def bit0;
  char  byte;
};


extern union byte_def flag3;
#define f_tch_free			flag3.bit0.b4
#define f_tch_complete		flag3.bit0.b5
#define f_tch_start			flag3.bit0.b6
#define f_tch_high			flag3.bit0.b7


//key
extern u16 tch_H_cnt;
extern u8 tch_L_cnt;
extern u8 tch_cnt;
extern u8 tch_dat;
extern u8 tch_dat1; //按键数据
extern u8 tch_dat2; //按键数据
extern u8 tch_code1;//按键接收码
extern u8 tch_code2;//按键接收码
//extern u16 len_key_cnt_10ms;


#define MAX_TX3_NUM	4
extern u8 Tx3_Buf[MAX_TX3_NUM];
extern u8 Tx3_Cnt;
extern u8 Tx3_Sum;
extern u8 Tx3_Len;


#define MAX_RX3_NUM	4
extern u8 Rx3_Buf[MAX_RX3_NUM];
extern u8 Rx3_Cnt;

#endif
