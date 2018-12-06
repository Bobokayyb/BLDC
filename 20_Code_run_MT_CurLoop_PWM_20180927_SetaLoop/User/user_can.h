#ifndef USER_CAN_H
#define USER_CAN_H
#include"stm32f10x.h"
#include"stm32f10x_conf.h"

#include"user_foc.h"
#include"user_position.h"


extern uint8_t Can_Data_Receive[8];
extern uint8_t Can_Data_Transmit[8];

typedef struct 
{
	u16 Run;              //ÔËĞĞÃüÁî
	u16 Speed_Ref;       //×ªËÙÖ¸Á
	u16 Position_Ref;       //×ªËÙÖ¸Áîî
	u16 Resv;             //±£Áô
}Can_Receive_Cmd;



extern Can_Receive_Cmd Can_Receive_Cmd_t;


extern void CAN_Configuration(void);
extern u8 Can_Send_Msg_13(void);
extern u8 Can_Send_Msg_14(void);
extern u8 Can_Receive_Msg(void);



#endif

