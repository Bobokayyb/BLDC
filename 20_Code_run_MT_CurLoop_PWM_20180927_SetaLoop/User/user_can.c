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

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能PORTA时钟																 

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//使能CAN1时钟	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽
	GPIO_Init(GPIOA, &GPIO_InitStructure);			//初始化IO

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);			//初始化IO

	//CAN单元设置
	CAN_InitStructure.CAN_TTCM=DISABLE; 		//非时间触发通信模式  
	CAN_InitStructure.CAN_ABOM=DISABLE; 		//软件自动离线管理	 
	CAN_InitStructure.CAN_AWUM=DISABLE; 		//睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
	CAN_InitStructure.CAN_NART=ENABLE;			//禁止报文自动传送 
	CAN_InitStructure.CAN_RFLM=DISABLE; 		//报文不锁定,新的覆盖旧的  
	CAN_InitStructure.CAN_TXFP=DISABLE; 		//优先级由报文标识符决定 
	CAN_InitStructure.CAN_Mode= CAN_Mode_Normal;			//模式设置： mode:0,普通模式;1,回环模式; 
	//设置波特率
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq; 			//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位	CAN_SJW_1tq  CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
	CAN_InitStructure.CAN_BS1=CAN_BS1_10tq; 			//Tbs1=tbs1+1个时间单位CAN_BS1_1tq ~CAN_BS1_16tq
	CAN_InitStructure.CAN_BS2=CAN_BS2_7tq; 			//Tbs2=tbs2+1个时间单位CAN_BS2_1tq ~	CAN_BS2_8tq
	CAN_InitStructure.CAN_Prescaler=4;		//分频系数(Fdiv)为brp+1 
	CAN_Init(CAN1, &CAN_InitStructure); 		//初始化CAN1 

	CAN_FilterInitStructure.CAN_FilterNumber=0; //过滤器0
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	//屏蔽位模式
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;	//32位宽 
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;	//32位ID
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32位MASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//过滤器0关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;//激活过滤器0

	CAN_FilterInit(&CAN_FilterInitStructure);			//滤波器初始化
}

u8 Can_Send_Msg_13(void)
{	
	u8 mbox;
	u16 i=0;

	TxMessage_13.StdId=0x13;			// 标准标识符 
	TxMessage_13.ExtId=0x13;			// 设置扩展标示符 
	TxMessage_13.IDE=CAN_Id_Standard; 	// 标准帧
	TxMessage_13.RTR=CAN_RTR_Data;		// 数据帧
	TxMessage_13.DLC=8;				// 要发送的数据长度

	
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
	while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//等待发送结束
	if(i>=0XFFF)return 1;
	return 0;	 
}


u8 Can_Send_Msg_14(void)
{	
	u8 mbox;
	u16 i=0;

	TxMessage_14.StdId=0x14;			// 标准标识符 
	TxMessage_14.ExtId=0x14;			// 设置扩展标示符 
	TxMessage_14.IDE=CAN_Id_Standard; 	// 标准帧
	TxMessage_14.RTR=CAN_RTR_Data;		// 数据帧
	TxMessage_14.DLC=8;				// 要发送的数据长度

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
	while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//等待发送结束
	if(i>=0XFFF)return 1;
	return 0;	 
}




u8 Can_Receive_Msg(void)
{
 	u32 i;
	
    if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;		//没有接收到数据,直接退出 
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//读取数据	
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
	FOC_Data_t.s32_Position_Set_Q = _IQ((float)Can_Receive_Cmd_t.Position_Ref/3600.0);  //放大10倍
	
	return RxMessage.DLC;	
}
#endif

