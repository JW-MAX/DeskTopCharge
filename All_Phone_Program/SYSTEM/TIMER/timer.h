#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"


extern u16 TimeMS;
extern u16 Timer10ms,Timer40ms;

extern int DelayCondition;//延时状态
extern int DelayClear;//延时标志清楚
extern u16 BeepCnt;    //蜂鸣器鸣响次数计时器
extern u16 VltControlCnt; //电压控制信号发送次数计数器
extern u16 VltControlTimeCnt;//电压分配中电压等级间隔延时计数器
extern u16 Temp,TempPer10ms,TempPer10ms_1,TempPer10ms_2,TempPer10ms_3;


void TIM2_Int_Init(void);
void TIM3_Int_Init(u16 arr,u16 psc);
void Drv_SysTimer(void);
//void TIM4_Configuration(void);

#endif
