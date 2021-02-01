#ifndef __RELAY_H
#define __RELAY_H 

#include "pbdata.h"

#define LOAD_RELAY_OUT_A5    PEout(5)     //A口5v负载控制继电器信号、J20
#define LOAD_RELAY_OUT_A9    PEout(3)     //A口9v负载控制继电器信号、J22
#define LOAD_RELAY_OUT_A12   PEout(8)     //A口12v负载控制继电器信号、J33
#define LOAD_RELAY_OUT_A15   PEout(10)    //A口15v负载控制继电器信号、J21
#define LOAD_RELAY_OUT_A20   PEout(11)    //A口20v负载控制继电器信号、J19;A口过流测试负载
#define LOAD_RELAY_OUT_Q5    PEout(13)    //Q口5v负载控制继电器信号、J30
#define LOAD_RELAY_OUT_Q9    PEout(15)    //Q口9v负载控制继电器信号、J32
#define LOAD_RELAY_OUT_Q12   PEout(14)    //Q口12v负载控制继电器信号、J34
#define LOAD_RELAY_OUT_Q15   PBout(15)    //Q口15v负载控制继电器信号、J31;Q口过流测试负载
#define LOAD_RELAY_OUT_Q20   PEout(1)		  //Q口20v负载控制继电器信号、J29;连接C_port
#define LOAD_RELAY_OUT_C5    PEout(12)		//C口5v负载控制继电器信号、J24
#define LOAD_RELAY_OUT_C9    PDout(10)		//C口9v负载控制继电器信号、J26
#define LOAD_RELAY_OUT_C12   PDout(11)		//C口12v负载控制继电器信号、J28
#define LOAD_RELAY_OUT_C15   PEout(6)		  //C口15v负载控制继电器信号、J27
#define LOAD_RELAY_OUT_C20   PEout(4)		  //C口20v负载控制继电器信号、J25
#define LOAD_RELAY_OUT_C1    PEout(2)		  //C口1号负载控制继电器信号、J23
#define LOAD_RELAY_OUT_C2    PEout(7)		  //C口2号负载控制继电器信号、J18
#define LOAD_RELAY_OUT_C3    PEout(9)		  //C口3号负载控制继电器信号、J17;C口过流测试负载

#define VOLTAGE_RELAY_OUT_A   PBout(11)		//A口电压控制继电器信号
#define VOLTAGE_RELAY_OUT_Q   PBout(10)		//Q口电压控制继电器信号、连接A_Port
#define VOLTAGE_RELAY_OUT_C   PBout(14)		//C口电压控制继电器信号

/**********Port_30W相应延时宏定义*********/
#define Port_30W_VOLTAGE_RELAY_DELAYTIME 	4 //诱骗器下降沿、上升沿时间间隔100ms
#define Port_30W_AD_DELAYTIME  						4
#define Port_30W_LOAD_RELAY_DELAYTIME  		20 //带载延200ms
#define Port_30W_RestartTime  		        30 //不同口带载时，其他口重启时间

/**********UsbCar_18W相应延时宏定义*********/
#define UsbCar_18W_VOLTAGE_RELAY_DELAYTIME 	10 //诱骗器下降沿、上升沿时间间隔100ms
#define UsbCar_18W_AD_DELAYTIME  						30
#define UsbCar_18W_LOAD_RELAY_DELAYTIME  		200 //带载延200ms
#define UsbCar_18W_RestartTime  		        600 //不同口带载时，其他口重启时间

/**********Port_60W相应延时宏定义*********/
#define Port_60W_VOLTAGE_RELAY_DELAYTIME 	10 //诱骗器下降沿、上升沿时间间隔100ms
#define Port_60W_AD_DELAYTIME  						30
#define Port_60W_LOAD_RELAY_DELAYTIME  		100 //带载延200ms
#define Port_60W_RestartTime  		        250 //不同口带载时，其他口重启时间
#define Port_60W_VOLTAGE_RELAY_DELAYTIME_Cnt 	8 //诱骗器下降沿次数计数器判断条件
#define Port_60W_VltControlTime_Cnt       4  //电压分配中电压等级间隔延时计数器判断条件
#define Port_60W_OverCurrent_5V         497  //过流过程中电压卡控
void GPIO_Configuration_Relay(void);


	 


#endif
