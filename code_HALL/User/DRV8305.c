#ifndef DRV8305_C
#define DRV8305_C

#include"DRV8305.h"

u16 SPI2_ReceiveData_yyb = 0;
u16 SPI2_SendData_yyb = Address_A;
u16 test_yyb1_SPI2 = 0;
u16 test_yyb2_SPI2 = 0;
u16 test_yyb3_SPI2 = 0;
u16 test_yyb4_SPI2 = 0;



void DRV8305_Init(void);
u16 DRV8305_Set(void);



u16 DRV8305_Set(void)
{
	u16 retry;
	//test_yyb_SPI2 = SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE);

	SPI2_CS=0;

//等待发送区为空
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==0)
	{
		//retry++;
		//if(retry>200)
			//return 0;
			test_yyb1_SPI2 += 2;
	}

	test_yyb2_SPI2 += 1;

	SPI_I2S_SendData(SPI2,(u16)(Write|Address_A|Gain_10));
	//SPI_I2S_SendData(SPI2,(u16)(Read|SPI2_SendData_yyb));

//等待发送完成，读接收区为非空
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==0)
	{
		//retry++;
		//if(retry>400)
			//return 0;
			test_yyb3_SPI2 += 2;
	}

	test_yyb4_SPI2 += 1;

	SPI2_ReceiveData_yyb = SPI_I2S_ReceiveData(SPI2);

	SPI2_CS=1;

	return 0;
}


void DRV8305_Init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;  //??SPI???
    GPIO_InitTypeDef GPIO_InitStructure; //??GPIO???
	//??SPI1???GPIOA?GPIOC????????????
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|
            RCC_APB2Periph_AFIO,
            ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);

	/* ??PA4?????,??MAX5415???? */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
	SPI2_CS=1;

//GPIOC_13是DRV8305的使能信号
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//??????
    GPIO_Init(GPIOC, &GPIO_InitStructure);

  	/* ??SPI??: NSS,SCK,MISO and MOSI */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//??????
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	//GPIO_ResetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//??????
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /**********SPI????****************/
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//SPI?2????
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;					  //SPI???
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;				  //SPI????8????
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;						  //??????0
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;					  //???????????
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;						  //??NSS???????
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;//????????32
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				   //?????????
	SPI_InitStructure.SPI_CRCPolynomial = 7;						   //CRC??????????7?
	SPI_Init(SPI2, &SPI_InitStructure);//?????????SPI???

	SPI_Cmd(SPI2, ENABLE);	//??SPI1

	DRV8305_Set();
	//SPI_I2S_ClearFlag(SPI2,SPI_I2S_FLAG_TXE);
	//SPI2->SR |= (SPI_I2S_FLAG_TXE);
	
	//SPI_I2S_SendData(SPI2,(u16)(Write|Address_A|Gain_10));
	
}



#endif

