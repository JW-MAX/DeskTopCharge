#ifndef __RELAY_H
#define __RELAY_H 

#include "pbdata.h"

#define LOAD_RELAY_OUT_A5    PEout(5)     //A��5v���ؿ��Ƽ̵����źš�J20
#define LOAD_RELAY_OUT_A9    PEout(3)     //A��9v���ؿ��Ƽ̵����źš�J22
#define LOAD_RELAY_OUT_A12   PEout(8)     //A��12v���ؿ��Ƽ̵����źš�J33
#define LOAD_RELAY_OUT_A15   PEout(10)    //A��15v���ؿ��Ƽ̵����źš�J21
#define LOAD_RELAY_OUT_A20   PEout(11)    //A��20v���ؿ��Ƽ̵����źš�J19;A�ڹ������Ը���
#define LOAD_RELAY_OUT_Q5    PEout(13)    //Q��5v���ؿ��Ƽ̵����źš�J30
#define LOAD_RELAY_OUT_Q9    PEout(15)    //Q��9v���ؿ��Ƽ̵����źš�J32
#define LOAD_RELAY_OUT_Q12   PEout(14)    //Q��12v���ؿ��Ƽ̵����źš�J34
#define LOAD_RELAY_OUT_Q15   PBout(15)    //Q��15v���ؿ��Ƽ̵����źš�J31;Q�ڹ������Ը���
#define LOAD_RELAY_OUT_Q20   PEout(1)		  //Q��20v���ؿ��Ƽ̵����źš�J29;����C_port
#define LOAD_RELAY_OUT_C5    PEout(12)		//C��5v���ؿ��Ƽ̵����źš�J24
#define LOAD_RELAY_OUT_C9    PDout(10)		//C��9v���ؿ��Ƽ̵����źš�J26
#define LOAD_RELAY_OUT_C12   PDout(11)		//C��12v���ؿ��Ƽ̵����źš�J28
#define LOAD_RELAY_OUT_C15   PEout(6)		  //C��15v���ؿ��Ƽ̵����źš�J27
#define LOAD_RELAY_OUT_C20   PEout(4)		  //C��20v���ؿ��Ƽ̵����źš�J25
#define LOAD_RELAY_OUT_C1    PEout(2)		  //C��1�Ÿ��ؿ��Ƽ̵����źš�J23
#define LOAD_RELAY_OUT_C2    PEout(7)		  //C��2�Ÿ��ؿ��Ƽ̵����źš�J18
#define LOAD_RELAY_OUT_C3    PEout(9)		  //C��3�Ÿ��ؿ��Ƽ̵����źš�J17;C�ڹ������Ը���

#define VOLTAGE_RELAY_OUT_A   PBout(11)		//A�ڵ�ѹ���Ƽ̵����ź�
#define VOLTAGE_RELAY_OUT_Q   PBout(10)		//Q�ڵ�ѹ���Ƽ̵����źš�����A_Port
#define VOLTAGE_RELAY_OUT_C   PBout(14)		//C�ڵ�ѹ���Ƽ̵����ź�

/**********Port_30W��Ӧ��ʱ�궨��*********/
#define Port_30W_VOLTAGE_RELAY_DELAYTIME 	4 //��ƭ���½��ء�������ʱ����100ms
#define Port_30W_AD_DELAYTIME  						4
#define Port_30W_LOAD_RELAY_DELAYTIME  		20 //������200ms
#define Port_30W_RestartTime  		        30 //��ͬ�ڴ���ʱ������������ʱ��

/**********UsbCar_18W��Ӧ��ʱ�궨��*********/
#define UsbCar_18W_VOLTAGE_RELAY_DELAYTIME 	10 //��ƭ���½��ء�������ʱ����100ms
#define UsbCar_18W_AD_DELAYTIME  						30
#define UsbCar_18W_LOAD_RELAY_DELAYTIME  		200 //������200ms
#define UsbCar_18W_RestartTime  		        600 //��ͬ�ڴ���ʱ������������ʱ��

/**********Port_60W��Ӧ��ʱ�궨��*********/
#define Port_60W_VOLTAGE_RELAY_DELAYTIME 	10 //��ƭ���½��ء�������ʱ����100ms
#define Port_60W_AD_DELAYTIME  						30
#define Port_60W_LOAD_RELAY_DELAYTIME  		100 //������200ms
#define Port_60W_RestartTime  		        250 //��ͬ�ڴ���ʱ������������ʱ��
#define Port_60W_VOLTAGE_RELAY_DELAYTIME_Cnt 	8 //��ƭ���½��ش����������ж�����
#define Port_60W_VltControlTime_Cnt       4  //��ѹ�����е�ѹ�ȼ������ʱ�������ж�����
#define Port_60W_OverCurrent_5V         497  //���������е�ѹ����
void GPIO_Configuration_Relay(void);


	 


#endif
