#ifndef MATH_H
#define MATH_H

#include"stm32f10x.h"

#define GLOBAL_Q 20

typedef s32 _iq;

#define _IQ(A)			(s32)((float)(A)*(float)(1<<GLOBAL_Q))
#define _IQint(A)		(s32)((A)>>GLOBAL_Q)

#define _IQsat(A,Pos,Neg)	((A>Pos)?Pos:((A<Neg)?Neg:(A)))
#define abs(A)			((A>0)?(A):(-A))
#define SIGN(A)			((A>0)?1:(-1))
#define MIN(A,B,C)		((A>B)?((B>C)?C:(B)):((A>C)?C:(A)))
#define MAX(A,B,C)	((A<B)?((B<C)?C:(B)):((A<C)?C:(A)))

typedef struct
{
	_iq In;
	_iq Out;
	_iq Kflt;
}LPFirst_t;
#define LPFDefault {0,0,_IQ(0.5)}

typedef struct
{
	_iq	Ref;
	_iq	Fdb;
	_iq	Err;
	_iq	Kp;
	_iq	Up;
	_iq	Ui;
	_iq	Max;
	_iq	Min;
	_iq	Out;
	_iq	SatErr;
	_iq	Ki;
	_iq	Kc;
}PIReg_t;
#define PIDefault {0,0,0,0,0,0,0,0,0,0,0,0}

extern void LPFirstCalc(LPFirst_t *p);
extern void VctPIRegCalcNewAnti(PIReg_t *v);

extern s32 _IQmpy(s32 x,s32 y);

#endif
