#ifndef PARAM_H
#define PARAM_H

#define Motor_Speed_Base (9000.0)
#define BASE_CUR (500.0)  //放大100倍


//电机参数                              
#define M_NS_NUM	2	//电机极对数     //一般2对极、4对极
#define M_NUM		3	//电机线槽数     //一般3线槽、6线槽
#define M_SPEED		M_NS_NUM*M_NUM*2	//电机一圈的换向次数
#define M_1MIN_X	60.0/((float)M_SPEED)//电机1min转速系数

#endif
