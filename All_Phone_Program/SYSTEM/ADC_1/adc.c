#include"adc.h"


void ADC_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_3;//PA_V//PA_C//PB_C
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;//PB_V//PC_C
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;//PB_V
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode=DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode=DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel=6;

	ADC_Init(ADC1,&ADC_InitStructure);

/*
  ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_239Cycles5);//PA_V  ADC1通道0，PA0
	ADC_RegularChannelConfig(ADC1,ADC_Channel_2,2,ADC_SampleTime_239Cycles5);//PA_C  ADC1通道2，PA2
	ADC_RegularChannelConfig(ADC1,ADC_Channel_14,1,ADC_SampleTime_239Cycles5);//PB_V ADC1通道14，PC4
	ADC_RegularChannelConfig(ADC1,ADC_Channel_3,2,ADC_SampleTime_239Cycles5);//PB_C  ADC1通道3，PA3
	ADC_RegularChannelConfig(ADC1,ADC_Channel_8,1,ADC_SampleTime_239Cycles5);//PC_V ADC1通道8，PB0
	ADC_RegularChannelConfig(ADC1,ADC_Channel_9,2,ADC_SampleTime_239Cycles5);//PC_C  ADC1通道9，PB1

	ADC_Cmd(ADC1,ENABLE);

	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));

	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));

	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
*/
}
