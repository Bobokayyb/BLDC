#ifndef PARAM_H
#define PARAM_H

#define Motor_Speed_Base (9000.0)
#define BASE_CUR (500.0)  //�Ŵ�100��


//�������                              
#define M_NS_NUM	2	//���������     //һ��2�Լ���4�Լ�
#define M_NUM		3	//����߲���     //һ��3�߲ۡ�6�߲�
#define M_SPEED		M_NS_NUM*M_NUM*2	//���һȦ�Ļ������
#define M_1MIN_X	60.0/((float)M_SPEED)//���1minת��ϵ��

#endif
