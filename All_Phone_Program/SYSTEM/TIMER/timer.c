#include "timer.h"
#include "delay.h"


u16 TimeMS;
u16 Timer10ms,Timer40ms;

int DelayCondition=0,DelayClear=0;
u16 BeepCnt=0;
u16 VltControlCnt=0;
u16 VltControlTimeCnt=0;
u16 Temp,TempPer10ms,TempPer10ms_1,TempPer10ms_2,TempPer10ms_3;


//通用定时器中断初始化
void TIM2_Int_Init(void)
{
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//开启TIM3时钟 

	TIM_TimeBaseInitStructure.TIM_Prescaler=20;   //分频值  10ms
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;	   //计数模式
	TIM_TimeBaseInitStructure.TIM_Period=35999;		   //自动重装数值
	TIM_TimeBaseInitStructure.TIM_ClockDivision=0;  //设置时钟分割
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//允许更新中断

	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM2,ENABLE);		  //使能TIM3
}

void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//开启TIM3时钟 

	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;   //分频值
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;	   //计数模式
	TIM_TimeBaseInitStructure.TIM_Period=arr;		   //自动重装数值
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;  //设置时钟分割
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//允许更新中断

	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM3,ENABLE);		  //使能TIM3
}

void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)
	{	
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);		
		/* 全局运行时间每1ms增1 */
		TimeMS++;
		if(TimeMS >= 6000)
		{
			TimeMS = 0;
		}
	}
}

void Drv_SysTimer(void)
{
	u16 Temp,Temp2;
	static u16 Temp1=0;
	static u16 Temp3=0;
	Timer40ms = 0;
	Timer10ms = 0;
	Temp = TimeMS/10;
	Temp2 = TimeMS/40;
	if(Temp != Temp1)
	{
		Temp1 =Temp;
		Timer10ms = 1;
	}
	if(Temp2 != Temp3)
	{
		Temp3 = Temp2;
		Timer40ms = 1;
	}
}
/*
void TIM4_IRQHandler(void)
{
  

}	
*/






