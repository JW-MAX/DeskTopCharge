#include "pbdata.h"
#include "switch.h"
#include "Port_30W.h"
#include "Port_CAR_18W.h"
#include "Port_60W.h"
#include "Port_AA_100W.h"
#include "Port_AC_100W.h"
#include "Port_AC_DC_100W.h"


void SwitchJudge(void);
int ID=0;
int m=0,n=0,p=0,q=0;

int main(void)
{
	RCC_Configuration();
	GPIO_Configuration_switch();
	SwitchJudge();
	Adc_Init();
	GPIO_Configuration_Relay();
	GPIO_Configuration_Led();
	TIM3_Int_Init(999,71);  //1ms =1/1KHz = 1/72M/(71+1)/(999+1)
	USART2_Init(9600);
	delay_init();
	MeasureVltInit();
	ID=m*8+n*4+p*2+q;
	switch(ID)
	 {
		 case 0:
			 Port_30W_main();//Port_30W
		 break;
		 
		 case 1:
			 Port_CAR_18W_main();//Port_CAR_18W
		 break;
		 
		 case 2:
			 Port_60W_main();//Port_60W
		 break;
		 
		 case 3:
			 //Port_AA_100W
		 break;
		 
		 case 4:
			 //Port_AC_100W
		 break;
		 
		 case 5:
			 //Port_AC_DC_100W
		 break;
		 
	 }
}


void SwitchJudge(void)
{
  if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12)!=Bit_RESET)
	{m=1;}
	else 
		{m=0;}
	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_13)!=Bit_RESET)
	{n=1;}
	else 
		{n=0;}
	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_14)!=Bit_RESET)
	{p=1;}
	else 
		{p=0;}
	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_15)!=Bit_RESET)
	{q=1;}
	else 
		{q=0;}
}



