#ifndef DRV8305_H
#define DRV8305_H

#include"stm32f10x.h"
#include"stm32f10x_conf.h"
#include"sys.h"

#define SPI2_CS PBout(12)

#define Write 0x0000
#define Read 0x8000
#define Address_1 0x0800
#define Address_2 0x1000
#define Address_3 0x1800
#define Address_4 0x2000
#define Address_5 0x2800
#define Address_6 0x3000
#define Address_7 0x3800
#define Address_8 0x4000
#define Address_9 0x4800
#define Address_A 0x5000
#define Address_B 0x5800
#define Address_C 0x6000

#define Gain_10 0x0000
#define Gain_20 0x0015



extern void DRV8305_Init(void);
extern u16 DRV8305_Set(void);


#endif

