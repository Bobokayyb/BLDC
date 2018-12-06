#ifndef USER_CAN_C
#define USER_CAN_C

#include"user_can.h"

uint8_t Can_Data_Receive[8];
uint8_t Can_Data_Transmit[8];
CanTxMsg TxMessage_13;
CanTxMsg TxMessage_14;
CanRxMsg RxMessage;
Can_Receive_Cmd Can_Receive_Cmd_t;






void CAN_Configuration(void);
u8 Can_Send_Msg_13(void);
u8 Can_Send_Msg_14(void);

u8 Can_Receive_Msg(void);


void CAN_Configuration(void)
{
	
	GPIO_InitTypeDef		GPIO_InitStructure; 
	CAN_InitTypeDef 		CAN_InitStructure;
	CAN_FilterInitTypeDef	CAN_FilterInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��PORTAʱ��																 

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//ʹ��CAN1ʱ��	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);			//��ʼ��IO

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);			//��ʼ��IO

	//CAN��Ԫ����
	CAN_InitStructure.CAN_TTCM=DISABLE; 		//��ʱ�䴥��ͨ��ģʽ  
	CAN_InitStructure.CAN_ABOM=DISABLE; 		//����Զ����߹���	 
	CAN_InitStructure.CAN_AWUM=DISABLE; 		//˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
	CAN_InitStructure.CAN_NART=ENABLE;			//��ֹ�����Զ����� 
	CAN_InitStructure.CAN_RFLM=DISABLE; 		//���Ĳ�����,�µĸ��Ǿɵ�  
	CAN_InitStructure.CAN_TXFP=DISABLE; 		//���ȼ��ɱ��ı�ʶ������ 
	CAN_InitStructure.CAN_Mode= CAN_Mode_Normal;			//ģʽ���ã� mode:0,��ͨģʽ;1,�ػ�ģʽ; 
	//���ò�����
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq; 			//����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ	CAN_SJW_1tq  CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
	CAN_InitStructure.CAN_BS1=CAN_BS1_10tq; 			//Tbs1=tbs1+1��ʱ�䵥λCAN_BS1_1tq ~CAN_BS1_16tq
	CAN_InitStructure.CAN_BS2=CAN_BS2_7tq; 			//Tbs2=tbs2+1��ʱ�䵥λCAN_BS2_1tq ~	CAN_BS2_8tq
	CAN_InitStructure.CAN_Prescaler=4;		//��Ƶϵ��(Fdiv)Ϊbrp+1 
	CAN_Init(CAN1, &CAN_InitStructure); 		//��ʼ��CAN1 

	CAN_FilterInitStructure.CAN_FilterNumber=0; //������0
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	//����λģʽ
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;	//32λ�� 
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;	//32λID
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32λMASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//������0������FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;//���������0

	CAN_FilterInit(&CAN_FilterInitStructure);			//�˲�����ʼ��
}

u8 Can_Send_Msg_13(void)
{	
	u8 mbox;
	u16 i=0;

	TxMessage_13.StdId=0x13;			// ��׼��ʶ�� 
	TxMessage_13.ExtId=0x13;			// ������չ��ʾ�� 
	TxMessage_13.IDE=CAN_Id_Standard; 	// ��׼֡
	TxMessage_13.RTR=CAN_RTR_Data;		// ����֡
	TxMessage_13.DLC=8;				// Ҫ���͵����ݳ���

	
	Can_Data_Transmit[0] = (u8)(Mode_en&&0x00FF);
	Can_Data_Transmit[1] = (u8)((u16)(Hall_Position_t.s16_Motor_Speed_Filter+10000) & 0x00FF);
	Can_Data_Transmit[2] = (u8)(((u16)(Hall_Position_t.s16_Motor_Speed_Filter+10000) & 0xFF00)>>8);
	//Can_Data_Transmit[3] = (u8)(HALL_A);
	//Can_Data_Transmit[4] = (u8)(HALL_B);
	//Can_Data_Transmit[5] = (u8)(HALL_C);
	Can_Data_Transmit[3] = (u8)(Hall_Position_t.Speed_Dir+1);
	Can_Data_Transmit[3] |= (u8)(Mode_Speed_en<<4);
	Can_Data_Transmit[4] = (u8)((u16)(FOC_Data_t.s16_Speed_Set_Ramp_Before+10000) & 0x00FF);
	Can_Data_Transmit[5] = (u8)(((u16)(FOC_Data_t.s16_Speed_Set_Ramp_Before+10000) & 0xFF00)>>8);
	Can_Data_Transmit[6] = (u8)((u16)(FOC_Data_t.s16_Speed_Set_Ramp+10000) & 0x00FF);
	Can_Data_Transmit[7] = (u8)(((u16)(FOC_Data_t.s16_Speed_Set_Ramp+10000) & 0xFF00)>>8);

	for(i=0;i<8;i++)
	TxMessage_13.Data[i]=Can_Data_Transmit[i];		
	
	mbox= CAN_Transmit(CAN1, &TxMessage_13);   
	i=0; 
	while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//�ȴ����ͽ���
	if(i>=0XFFF)return 1;
	return 0;	 
}


u8 Can_Send_Msg_14(void)
{	
	u8 mbox;
	u16 i=0;

	TxMessage_14.StdId=0x14;			// ��׼��ʶ�� 
	TxMessage_14.ExtId=0x14;			// ������չ��ʾ�� 
	TxMessage_14.IDE=CAN_Id_Standard; 	// ��׼֡
	TxMessage_14.RTR=CAN_RTR_Data;		// ����֡
	TxMessage_14.DLC=8;				// Ҫ���͵����ݳ���

	Can_Data_Transmit[0] = (u8)((u16)(FOC_Data_t.s16_Speed_Set+10000) & 0x00FF);
	Can_Data_Transmit[1] = (u8)(((u16)(FOC_Data_t.s16_Speed_Set+10000) & 0xFF00)>>8);
	Can_Data_Transmit[2] = (u8)((u16)(Hall_Position_t.s16_Motor_Speed_Filter+10000) & 0x00FF);
	Can_Data_Transmit[3] = (u8)(((u16)(Hall_Position_t.s16_Motor_Speed_Filter+10000) & 0xFF00)>>8);
	Can_Data_Transmit[4] = (u8)((u16)(FOC_Data_t.s32_Position_Set_Q>>(GLOBAL_Q-12)) & 0x00FF);
	Can_Data_Transmit[5] = (u8)(((u16)(FOC_Data_t.s32_Position_Set_Q>>(GLOBAL_Q-12)) & 0xFF00)>>8);
	Can_Data_Transmit[6] = (u8)((u16)(Hall_Position_t.s32_Motor_Position_Q>>(GLOBAL_Q-12)) & 0x00FF);
	Can_Data_Transmit[7] = (u8)(((u16)(Hall_Position_t.s32_Motor_Position_Q>>(GLOBAL_Q-12)) & 0xFF00)>>8);

	for(i=0;i<8;i++)
	TxMessage_14.Data[i]=Can_Data_Transmit[i];		
	
	mbox= CAN_Transmit(CAN1, &TxMessage_14);   
	i=0; 
	while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//�ȴ����ͽ���
	if(i>=0XFFF)return 1;
	return 0;	 
}




u8 Can_Receive_Msg(void)
{
 	u32 i;
	
    if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;		//û�н��յ�����,ֱ���˳� 
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//��ȡ����	
    for(i=0;i<8;i++)
    {
    	Can_Data_Receive[i]=RxMessage.Data[i]; 
    }

	Can_Receive_Cmd_t.Run = Can_Data_Receive[0];
	Can_Receive_Cmd_t.Speed_Ref = (u16)(Can_Data_Receive[1]);
	Can_Receive_Cmd_t.Speed_Ref |= ((u16)Can_Data_Receive[2]<<8);
	Can_Receive_Cmd_t.Position_Ref = (u16)(Can_Data_Receive[3]);
	Can_Receive_Cmd_t.Position_Ref |= ((u16)Can_Data_Receive[4]<<8);
	
	//FOC_Data_t.s16_Speed_Set = (s16)(Can_Receive_Cmd_t.Speed_Ref - 10000);
	FOC_Data_t.s32_Position_Set_Q = _IQ((float)Can_Receive_Cmd_t.Position_Ref/3600.0);  //�Ŵ�10��
	
	return RxMessage.DLC;	
}
#endif

