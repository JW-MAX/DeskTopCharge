#ifndef __SCREEN_H
#define __SCREEN_H 

#include "sys.h"

#include "timer.h"
#include"relay.h"
#include"adc.h"	 
#include"led.h"
#include "delay.h"
#include "usart2.h"
#include "SEGGER_RTT_Conf.h"
#include "SEGGER_RTT.h"
#include "adc.h"
#include "stdio.h"

#define cof  2475

struct structMeasureVltVaule
{
	u16 u16_AVlt_5_N;
	u16 u16_AVlt_5_F;
	u16 u16_AVlt_9_N;
	u16 u16_AVlt_9_F;
	u16 u16_AVlt_12_N;
	u16 u16_AVlt_12_F;
	u16 u16_CVlt_5_N;
	u16 u16_CVlt_5_F;
	u16 u16_CVlt_9_N;
	u16 u16_CVlt_9_F;
	u16 u16_CVlt_12_N;
	u16 u16_CVlt_12_F;
	u16 u16_CVlt_15_N;
	u16 u16_CVlt_15_F;
	u16 u16_CVlt_20_N;
	u16 u16_CVlt_20_F;
	u16 u16_QVlt_5_N;
	u16 u16_QVlt_5_F;
	u16 u16_QVlt_9_N;
	u16 u16_QVlt_9_F;
	u16 u16_QVlt_12_N;
	u16 u16_QVlt_12_F;
	
	u16 u16_CVltDis_20_Before;  //未测试电压分配，C口20V带载电压
	u16 u16_CVltDisA_20_F;      //C口20V带载与A口电压分配
	u16 u16_CVltDisAQ_20_F;     //C口20V带载与A口、Q口电压分配
  u16 u16_CVltDisQ_20_F;      //C口20V带载与Q口电压分配
	u16 u16_CVltCover_20_F;//测试电压分配后，C口20V带载电压
	
	u16 u16_AVltOverC_5_Before; //未测试各口过流，A口5V电压
	u16 u16_AVltOverC_5_Middle; //测试各口过流时，A口电压为0V
  u16 u16_AVltOverC_5_Behind; //测试各口过流后，A口5V电压
	u16 u16_QVltOverC_5_Before; //未测试各口过流，Q口5V电压
	u16 u16_QVltOverC_5_Middle; //测试各口过流时，Q口电压为0V
  u16 u16_QVltOverC_5_Behind; //测试各口过流后，Q口5V电压
	u16 u16_CVltOverC_5_Before; //未测试各口过流，C口5V电压
	u16 u16_CVltOverC_5_Middle; //测试各口过流时，C口电压为0V
  u16 u16_CVltOverC_5_Behind; //测试各口过流后，C口5V电压

	u16 bMeasureFinish:1;
	u16 bMeasureStart:1;
	u16 bStartSendData:1;
	u16 bRxDataFinish:1;
	u16 bCOpMeasureFinish:1;
	u16 bCOpMeasureStart:1;
	u16 bRev:10;
};
struct structMeasureVltFault
{
	union {
		u16 All;
		struct { 
		u16 bFault_A5_N:2;
		u16 bFault_A5_F:2;
		u16 bFault_C5_N:2;
		u16 bFault_C5_F:2;
		u16 bFault_C9_N:2;
		u16 bFault_C9_F:2;
		u16 bFault_C12_N:2;
		u16 bFault_C12_F:2;
		//u16 bRev:2;
		}Bits;
	} un16_Alarm_1;
	union {
		u16 All;
		struct{
		u16 bFault_C15_N:2;
		u16 bFault_C15_F:2;
		u16 bFault_C20_N:2;
		u16 bFault_C20_F:2;
		u16 bFault_Q5_N:2;
		u16 bFault_Q5_F:2;
		u16 bFault_Q9_N:2;
		u16 bFault_Q9_F:2;
		}Bits;
	} un16_Alarm_2;
	union {
		u16 All;
		struct{
		u16 bFault_Q12_N:2;
		u16 bFault_Q12_F:2;
		u16	bFault_A_OverC:2;
		u16	bFault_Q_OverC:2;
		u16	bFault_C_OverC:2;
		u16	bFault_C_DisA:2;
		u16	bFault_C_DisAQ:2;
		u16	bFault_C_DisQ:2;
		}Bits;
	} un16_Alarm_3;
	union {
		u16 All;
		struct{
		u16 bFault_C_Cover:2;
		u16 bFault_C_Opposite:2;
		u16 bRev:12;
		}Bits;
	} un16_Alarm_4;
};
extern struct structMeasureVltVaule m_st_MeasureVltVaule;
extern struct structMeasureVltFault m_st_MeasureVltFault;
extern u8 SendData[50];
extern u8 u8_senddataCnt;
extern u16 u16_StartMeasureConfCnt;

extern u16 u16_RxCount;
extern int *i16_pRxBuff;
extern int i16_RxBuff[50];
extern int ProductModel_Address;//显示屏对应产品显示地址
extern int VoltageCondition;//输入口掉电标志

void MeasureVltInit(void);
void SendDataProcess(void);
void DataLoad(void);

#endif



