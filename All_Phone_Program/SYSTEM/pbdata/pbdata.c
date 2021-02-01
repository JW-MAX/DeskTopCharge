#include"pbdata.h"

void RCC_Configuration(void)
{
  SystemInit();
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);	//使能USART2
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	//RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//12M  最大14M
}
