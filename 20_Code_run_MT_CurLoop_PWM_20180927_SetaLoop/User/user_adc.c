#ifndef USER_ADC_C
#define USER_ADC_C

#include"user_adc.h"

s16 yyb_test_adc_1 = 0;
s16 yyb_test_adc_2 = 0;
s16 yyb_test_adc_3 = 0;
LPFirst_t Cur_DC_Filter = LPFDefault;

//DMA
#define  Channel_Number 3// ͨ�����ĺ궨��
s16  AD_Digital_Value[Channel_Number];					//AD����ֵ
ADC_Value Adc_Value_t;

void ADC_user_Init(void)
{
	Adc_Value_t.Volt_low_Range = 7260;  //100��
	Adc_Value_t.cur_Range = 3000;  //100��

	/*     k = 2*pi*f / fs   = 2*3.14*19.1/1000=0.12               
	f: ��ֹƵ��
	fs: ����Ƶ��
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
 
	 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);  //ʹ��DMAʱ��
	 
	 DMA_DeInit(DMA1_Channel1);	//��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
 
	 //DMA1_MEM_LEN=cndtr;//����ͨ�����ݳ���
	 DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;  //DMA�������ַ
	 DMA_InitStructure.DMA_MemoryBaseAddr = (u32)AD_Digital_Value;  //DMA�ڴ����ַ
	 DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //���ݴ��䷽�򣬴����跢�͵��ڴ�  DMA_CCRXλ4
	 DMA_InitStructure.DMA_BufferSize = Channel_Number;	//DMAͨ����DMA����Ĵ�С
	 DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
	 DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
	 DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //�������ݿ��Ϊ16λ
	 DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //�ڴ����ݿ��Ϊ16λ
	 DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  //������ѭ������ģʽ
	 DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMAͨ�� xӵ�и����ȼ� 
	 DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	 DMA_Init(DMA1_Channel1, &DMA_InitStructure);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���
		
}



void ADC_Configuration(void)
{
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M
                      
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	

	ADC_DeInit(ADC1);  //��λADC1,������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ;  //ɨ��ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//����������ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = Channel_Number;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   


	#define  I_A		ADC_Channel_0	
	#define  I_B		ADC_Channel_1	
	#define  I_C		ADC_Channel_2
	#define  U_C		ADC_Channel_3	
	#define  U_B		ADC_Channel_4	
	#define  U_A		ADC_Channel_5
	#define  TEMP_MOTOR		ADC_Channel_6

//��������������������˳����в���
//����ʱ��=  (7.5(����ʱ����)+12.5(����������))/12MHz = 1.667us
	ADC_RegularChannelConfig(ADC1, I_A, 	1, ADC_SampleTime_7Cycles5 );
	ADC_RegularChannelConfig(ADC1, I_B, 	2, ADC_SampleTime_7Cycles5 );
	ADC_RegularChannelConfig(ADC1, I_C, 	3, ADC_SampleTime_7Cycles5 );
//	ADC_RegularChannelConfig(ADC1, U_C, 	4, ADC_SampleTime_7Cycles5 );
//	ADC_RegularChannelConfig(ADC1, U_B, 	5, ADC_SampleTime_7Cycles5 );
//	ADC_RegularChannelConfig(ADC1, U_A, 	6, ADC_SampleTime_7Cycles5 );
//	ADC_RegularChannelConfig(ADC1, TEMP_MOTOR, 	7, ADC_SampleTime_7Cycles5 );	//���ò���ʱ��Ϊ239.5����
		
	ADC_DMACmd(ADC1, ENABLE); //ʹ��ADC1��DMA���� ��ԭ��ADC��û�еģ�ע�����
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
	
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼   
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1);	 //����ADУ׼
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
}


void Adc_ZeroCalc(void)
{
	static u16 Adc_Zero_Cnt = 10;

	while(Adc_Zero_Cnt > 0)
	{
		Adc_Zero_Cnt -= 1;
		
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);//�������ADת��
		//DMA_SetCurrDataCounter(DMA1_Channel1,8);      //DMA��normalģʽ��������
		DMA_Cmd(DMA1_Channel1, ENABLE);//����DMAͨ��
		while(!DMA_GetFlagStatus(DMA1_FLAG_TC1));

		DMA_ClearFlag(DMA1_FLAG_TC1);
		DMA_Cmd(DMA1_Channel1, DISABLE);//ֹͣDMAͨ��

//���������˾�������
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

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//�������ADת��
	//DMA_SetCurrDataCounter(DMA1_Channel1,8);      //DMA��normalģʽ��������
	DMA_Cmd(DMA1_Channel1, ENABLE);//����DMAͨ��
	while(!DMA_GetFlagStatus(DMA1_FLAG_TC1));

	DMA_ClearFlag(DMA1_FLAG_TC1);
	DMA_Cmd(DMA1_Channel1, DISABLE);//ֹͣDMAͨ��
	
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

