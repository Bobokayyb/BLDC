#ifndef USER_ADC_C
#define USER_ADC_C

#include"user_adc.h"

s16 yyb_test_adc_1 = 0;
s16 yyb_test_adc_2 = 0;
s16 yyb_test_adc_3 = 0;
LPFirst_t Cur_DC_Filter = LPFDefault;

//DMA
#define  Channel_Number 3// 通道数的宏定义
s16  AD_Digital_Value[Channel_Number];					//AD采样值
ADC_Value Adc_Value_t;

void ADC_user_Init(void)
{
	Adc_Value_t.Volt_low_Range = 7260;  //100倍
	Adc_Value_t.cur_Range = 3000;  //100倍

	/*     k = 2*pi*f / fs   = 2*3.14*19.1/1000=0.12               
	f: 截止频率
	fs: 调用频率
	*/
	Cur_DC_Filter.Kflt = _IQ(0.12);

	Adc_Value_t.cur_A_zero = 0;
	Adc_Value_t.cur_B_zero = 0;
	Adc_Value_t.cur_C_zero = 0;
}


void ADC_Configuration(void);
void Adc_ZeroCalc(void);
void Get_Adc(void);


void DMA_Configuration(void)
{
	DMA_InitTypeDef DMA_InitStructure;
 
	 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);  //使能DMA时钟
	 
	 DMA_DeInit(DMA1_Channel1);	//将DMA的通道1寄存器重设为缺省值
 
	 //DMA1_MEM_LEN=cndtr;//保存通道数据长度
	 DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;  //DMA外设基地址
	 DMA_InitStructure.DMA_MemoryBaseAddr = (u32)AD_Digital_Value;  //DMA内存基地址
	 DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //数据传输方向，从外设发送到内存  DMA_CCRX位4
	 DMA_InitStructure.DMA_BufferSize = Channel_Number;	//DMA通道的DMA缓存的大小
	 DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
	 DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
	 DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //外设数据宽度为16位
	 DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //内存数据宽度为16位
	 DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  //工作在循环缓存模式
	 DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMA通道 x拥有高优先级 
	 DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
	 DMA_Init(DMA1_Channel1, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器
		
}



void ADC_Configuration(void)
{
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1, ENABLE );	  //使能ADC1通道时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M
                      
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	

	ADC_DeInit(ADC1);  //复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ;  //扫描通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//工作在连续模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = Channel_Number;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   


	#define  I_A		ADC_Channel_0	
	#define  I_B		ADC_Channel_1	
	#define  I_C		ADC_Channel_2
	#define  U_C		ADC_Channel_3	
	#define  U_B		ADC_Channel_4	
	#define  U_A		ADC_Channel_5
	#define  TEMP_MOTOR		ADC_Channel_6

//规则采样，按照下面这个顺序进行采样
//采样时间=  (7.5(采样时间数)+12.5(采样周期数))/12MHz = 1.667us
	ADC_RegularChannelConfig(ADC1, I_A, 	1, ADC_SampleTime_7Cycles5 );
	ADC_RegularChannelConfig(ADC1, I_B, 	2, ADC_SampleTime_7Cycles5 );
	ADC_RegularChannelConfig(ADC1, I_C, 	3, ADC_SampleTime_7Cycles5 );
//	ADC_RegularChannelConfig(ADC1, U_C, 	4, ADC_SampleTime_7Cycles5 );
//	ADC_RegularChannelConfig(ADC1, U_B, 	5, ADC_SampleTime_7Cycles5 );
//	ADC_RegularChannelConfig(ADC1, U_A, 	6, ADC_SampleTime_7Cycles5 );
//	ADC_RegularChannelConfig(ADC1, TEMP_MOTOR, 	7, ADC_SampleTime_7Cycles5 );	//配置采样时间为239.5周期
		
	ADC_DMACmd(ADC1, ENABLE); //使能ADC1的DMA传输 ，原本ADC上没有的，注意添加
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
	
	ADC_ResetCalibration(ADC1);	//使能复位校准   
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	
	ADC_StartCalibration(ADC1);	 //开启AD校准
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
}


void Adc_ZeroCalc(void)
{
	static u16 Adc_Zero_Cnt = 10;

	while(Adc_Zero_Cnt > 0)
	{
		Adc_Zero_Cnt -= 1;
		
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);//软件启动AD转换
		//DMA_SetCurrDataCounter(DMA1_Channel1,8);      //DMA的normal模式必须设置
		DMA_Cmd(DMA1_Channel1, ENABLE);//启动DMA通道
		while(!DMA_GetFlagStatus(DMA1_FLAG_TC1));

		DMA_ClearFlag(DMA1_FLAG_TC1);
		DMA_Cmd(DMA1_Channel1, DISABLE);//停止DMA通道

//除法带来了精度问题
		Adc_Value_t.cur_A_zero += AD_Digital_Value[0]/10;
		Adc_Value_t.cur_B_zero += AD_Digital_Value[1]/10;
		Adc_Value_t.cur_C_zero += AD_Digital_Value[2]/10;

		//Adc_Value_t.cur_A_zero = AD_Digital_Value[0];
		//Adc_Value_t.cur_B_zero = AD_Digital_Value[1];
		//Adc_Value_t.cur_C_zero = AD_Digital_Value[2];
	}

	gu16_InitOK_Flag = 1;
}

void Get_Adc(void)   
{
	u32 temp;

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//软件启动AD转换
	//DMA_SetCurrDataCounter(DMA1_Channel1,8);      //DMA的normal模式必须设置
	DMA_Cmd(DMA1_Channel1, ENABLE);//启动DMA通道
	while(!DMA_GetFlagStatus(DMA1_FLAG_TC1));

	DMA_ClearFlag(DMA1_FLAG_TC1);
	DMA_Cmd(DMA1_Channel1, DISABLE);//停止DMA通道
	
	temp = (u32)(AD_Digital_Value[0]-Adc_Value_t.cur_A_zero)<<(GLOBAL_Q - 12);
	Adc_Value_t.cur_A = _IQmpy(temp,Adc_Value_t.cur_Range);
	temp = (u32)(AD_Digital_Value[1]-Adc_Value_t.cur_B_zero)<<(GLOBAL_Q - 12);
	Adc_Value_t.cur_B = _IQmpy(temp,Adc_Value_t.cur_Range);
	temp = (u32)(AD_Digital_Value[2]-Adc_Value_t.cur_C_zero)<<(GLOBAL_Q - 12);
	Adc_Value_t.cur_C = _IQmpy(temp,Adc_Value_t.cur_Range);

	Cur_DC_Filter.In = _IQ((float)(Adc_Value_t.cur_A+Adc_Value_t.cur_B+Adc_Value_t.cur_C)/BASE_CUR);
	LPFirstCalc(&Cur_DC_Filter);
	Adc_Value_t.cur_DC_Filter = Cur_DC_Filter.Out;
}


#endif

