#ifndef MATH_C
#define MATH_C

#include"math.h"

s32 _IQmpy(s32 x,s32 y)
{
	return((s32)(((int64_t)x*(int64_t)y)>>GLOBAL_Q));
}

s32 _IQdiv(s32 x,s32 y)
{
	return((s32)(((int64_t)x<<GLOBAL_Q)/(int64_t)y));
}

void LPFirstCalc(LPFirst_t *p)
{
	p->Out = p->Out +_IQmpy((p->In - p->Out),p->Kflt);
}

void VctPIRegCalcNewAnti(PIReg_t *v)
{
	_iq TempPI;
	
	v->Err = v->Ref - v->Fdb;
	v->Up = _IQmpy(v->Kp,v->Err);
	TempPI = v->Up +v->Ui;

	if(TempPI > v->Max)
	{
		v->Out = v->Max;
		if(v->Err < 0)
		{
			v->Ui = v->Ui + _IQmpy(v->Ki,v->Err);
		}
	}
	else if(TempPI < v->Min)
	{
		v->Out = v->Min;
		if(v->Err > 0)
		{
			v->Ui = v->Ui + _IQmpy(v->Ki,v->Err);
		}
	}
	else
	{
		v->Ui = v->Ui + _IQmpy(v->Ki,v->Err);
		v->Out = v->Up + v->Ui;
	}
}




#endif
