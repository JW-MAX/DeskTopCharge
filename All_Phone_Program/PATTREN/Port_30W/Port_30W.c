#include "Port_30W.h"


/******************************************************
函数名：int Port_30W_main(void)
描述：30W充电器测试功能函数
参数：无
*******************************************************/
int Port_30W_main(void)
{
	ProductModel_Address=0;
	VoltageCondition=1;
	
 while(1)
	{
		Drv_SysTimer();
		MeasureVlt_30W();
		SendDataProcess();
	}
}

/******************************************************
函数名：void MeasureVlt(void)
描述：电压值测量
参数：无
*******************************************************/
void MeasureVlt_30W(void)
{
	static u16 u16_Temp = 0;
	u16	u16_TempAx = 0,u16_TempBx = 0;
	
	u16_TempAx = Get_Adc_Average(0,5);
	u16_TempBx = Get_Adc_Average(14,5);
	
	//判断C口和A口是否掉电 来复位声光信号
	if(VoltageCondition==1)
  {
	if(((u16_TempAx <= 300)||(u16_TempBx <= 300))
   	 &&(m_st_MeasureVltVaule.bMeasureFinish == 1)
	  )
	{
		u16_Temp = 8;
	}
		 
	//判断C口和A口是否都为5V 来确定是否开始测量		 
	if(((u16_TempAx>= 777) && (u16_TempAx <= 858))
			&&((u16_TempBx>= 780) && (u16_TempBx <= 862))
			&&(m_st_MeasureVltVaule.bMeasureStart == 1)
		)
	{
		LED_PASS_A = 1;
		LED_FAULT_A = 0;
		LED_PASS_C = 1;
		LED_FAULT_C = 0;
		BEEP=1;
		u16_Temp = 9;
		m_st_MeasureVltVaule.bMeasureStart = 0;
	}
 }
	if(Timer10ms == 1)
		{
			TempPer10ms_1++;
			TempPer10ms++;
			TempPer10ms_2 ++;
		}
			
		if(TempPer10ms>=2)
		{
			TempPer10ms = 0;
		}	
   
	switch(u16_Temp)
	{
		case 0: //A口 空载 满载测量
		if(DelayClear==0)
			{
			 VoltageCondition=0;
			 TempPer10ms_1=0;
			 TempPer10ms_2=0;
       DelayClear=1;				
			}	
		if(TempPer10ms_1>=3)  //延时100ms
		{
		  LED_PASS_A = 0;
			LED_FAULT_A = 0;
		  LED_PASS_C = 0;
			LED_FAULT_C =0;
      BEEP=0;	
			if(DelayCondition==0)
			{
				m_st_MeasureVltVaule.u16_AVlt_5_N = Get_Adc_Average(0,5);//A口 5V空载采样
				DelayCondition=1;
				TempPer10ms_2=0;
			}	
			//满足5V±5% 后，吸合负载,进行带载测试 
			if(m_st_MeasureVltVaule.u16_AVlt_5_N >= 777
				 && m_st_MeasureVltVaule.u16_AVlt_5_N <= 858) 
			{
				m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_A5_N = 1;
				LOAD_RELAY_OUT_A5 = 1;
				if(TempPer10ms_2>=Port_30W_LOAD_RELAY_DELAYTIME)//延时1000ms
				{
				m_st_MeasureVltVaule.u16_AVlt_5_F = Get_Adc_Average(0,5);
				LOAD_RELAY_OUT_A5 = 0;
				
				if(m_st_MeasureVltVaule.u16_AVlt_5_F >= 610
				   && m_st_MeasureVltVaule.u16_AVlt_5_F <= 719) //±5%
				{
					m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_A5_F= 1;
				}
				else//报故障
				{
					m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_A5_F = 2;
				}
				DelayCondition=0;
        DelayClear=0;				
				u16_Temp = 1;
			 }
			}
			else //报故障
			{
				m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_A5_N = 2;
				DelayCondition=0;
        DelayClear=0;				
				u16_Temp = 1;
			}
	   
   }			
			break;
			
		case 1: //C口 5V空载 满载测量
			if(DelayClear==0)
			{
			 TempPer10ms_1=0;
			 TempPer10ms_2=0;
       DelayClear=1;				
			}
		  if(TempPer10ms_1>=Port_30W_RestartTime)
			{					 
			if(DelayCondition==0)
			{
			 m_st_MeasureVltVaule.u16_CVlt_5_N =Get_Adc_Average(14,5);
			 DelayCondition=1;
			 TempPer10ms_2=0;
			}
			//SEGGER_RTT_printf(0,"u16_CVlt_5_N = %d\n",m_st_MeasureVltVaule.u16_CVlt_5_N);
			//满足5V±5% 后，吸合负载,进行带载测试
      
			if(m_st_MeasureVltVaule.u16_CVlt_5_N >= 780
				&& m_st_MeasureVltVaule.u16_CVlt_5_N <= 862) 
			{
				m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C5_N = 1;
				LOAD_RELAY_OUT_C5 = 1;
				if(TempPer10ms_2>=(Port_30W_LOAD_RELAY_DELAYTIME))
				{
				m_st_MeasureVltVaule.u16_CVlt_5_F = Get_Adc_Average(14,5);
				LOAD_RELAY_OUT_C5 = 0;
				
				if(m_st_MeasureVltVaule.u16_CVlt_5_F >= 696
				&& m_st_MeasureVltVaule.u16_CVlt_5_F <= 770) //±5%
				{
					m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C5_F = 1;
				}
				
				
				else//报故障
				{
					m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C5_F = 2;
				}
				DelayCondition=0;
				DelayClear=0;
				u16_Temp = 2;
			 }
			}
			else //报故障
			{
				m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C5_N = 2;
				DelayCondition=0;
        DelayClear=0;				
				u16_Temp = 2;
			}
  }    
			break;
			
		case 2://C口 9V空载 满载测量
			if(DelayClear==0)
			{
			 TempPer10ms_1=0;
			 TempPer10ms_2=0;
       DelayClear=1;
       VOLTAGE_RELAY_OUT_C = 1;				
			}
		  if(TempPer10ms_1>=Port_30W_VOLTAGE_RELAY_DELAYTIME)
			{
		  VOLTAGE_RELAY_OUT_C = 0;
		
			if(TempPer10ms_1>=(Port_30W_VOLTAGE_RELAY_DELAYTIME+Port_30W_AD_DELAYTIME))
		  {
			 if(DelayCondition==0)
			 {
				 m_st_MeasureVltVaule.u16_CVlt_9_N =Get_Adc_Average(14,5);
				 DelayCondition=1;
				 TempPer10ms_2=0;
				}
			//满足9V±5% 后，吸合负载,进行带载测试 
			if(m_st_MeasureVltVaule.u16_CVlt_9_N >= 1404
				&& m_st_MeasureVltVaule.u16_CVlt_9_N <= 1552) 
			{
				m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C9_N = 1;
				LOAD_RELAY_OUT_C9 = 1;
				if(TempPer10ms_2>=(Port_30W_LOAD_RELAY_DELAYTIME))
			  {
				m_st_MeasureVltVaule.u16_CVlt_9_F = Get_Adc_Average(14,5);
				LOAD_RELAY_OUT_C9 = 0;
				
				if(m_st_MeasureVltVaule.u16_CVlt_9_F >= 1296
				&& m_st_MeasureVltVaule.u16_CVlt_9_F <= 1432) //±5%
				{
					m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C9_F = 1;
				}
				else//报故障
				{
					m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C9_F = 2;
				}
				DelayCondition=0;
				DelayClear=0;
				u16_Temp = 3;
			 }
			}
				
			else //报故障
			{
				m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C9_N = 2;
				DelayCondition=0;
        DelayClear=0;				
				u16_Temp = 3;
			}
		}
	}		

			break;
			
		case 3://C口 12V空载 满载测量
		if(DelayClear==0)
		{
		 TempPer10ms_1=0;
		 TempPer10ms_2=0;
		 DelayClear=1;
		 VOLTAGE_RELAY_OUT_C = 1;				
		}
		if(TempPer10ms_1>=Port_30W_VOLTAGE_RELAY_DELAYTIME)
		{ 
		  VOLTAGE_RELAY_OUT_C = 0;
		
		 if(TempPer10ms_1>=(Port_30W_VOLTAGE_RELAY_DELAYTIME+Port_30W_AD_DELAYTIME))
			{	
			if(DelayCondition==0)
				{
				 m_st_MeasureVltVaule.u16_CVlt_12_N =Get_Adc_Average(14,5);
				 DelayCondition=1;
				 TempPer10ms_2=0;
				}			
				//满足12V±5% 后，吸合负载,进行带载测试 
			if(m_st_MeasureVltVaule.u16_CVlt_12_N >= 1869
				&& m_st_MeasureVltVaule.u16_CVlt_12_N <= 2066) 
			  {
				m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C12_N = 1;
				LOAD_RELAY_OUT_C12 = 1;
				if(TempPer10ms_2>=(Port_30W_LOAD_RELAY_DELAYTIME))
			  {
				m_st_MeasureVltVaule.u16_CVlt_12_F = Get_Adc_Average(14,5);
				LOAD_RELAY_OUT_C12 = 0;
				
				if(m_st_MeasureVltVaule.u16_CVlt_12_F >= 1783
				&& m_st_MeasureVltVaule.u16_CVlt_12_F <= 1971) //±5%
				{
					m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C12_F = 1;
			  }
				else//报故障
				{
					m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C12_F = 2;
				}
				DelayCondition=0;
				DelayClear=0;
				u16_Temp = 4;
			 }
			}
			else //报故障
			{
				m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C12_N = 2;
				DelayCondition=0;
        DelayClear=0;				
				u16_Temp = 4;
			}
		}
	}			
			break;
			
		case 4://C口 15V空载 满载测量
			if(DelayClear==0)
			{
			 TempPer10ms_1=0;
			 TempPer10ms_2=0;
       DelayClear=1;	
       VOLTAGE_RELAY_OUT_C = 1;							
			}
			if(TempPer10ms_1>=Port_30W_VOLTAGE_RELAY_DELAYTIME)
		  {
		  VOLTAGE_RELAY_OUT_C = 0;
		
			if(TempPer10ms_1>=(Port_30W_VOLTAGE_RELAY_DELAYTIME+Port_30W_AD_DELAYTIME))
			{
			 if(DelayCondition==0)
			{
			 m_st_MeasureVltVaule.u16_CVlt_15_N =Get_Adc_Average(14,5);
				 DelayCondition=1;
				 TempPer10ms_2=0;
			}
			//满足15V±5% 后，吸合负载,进行带载测试 
			if(m_st_MeasureVltVaule.u16_CVlt_15_N >= 2341
				&& m_st_MeasureVltVaule.u16_CVlt_15_N <= 2587) 
			{
				m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C15_N = 1;
				LOAD_RELAY_OUT_C15 = 1;
				if(TempPer10ms_2>=Port_30W_LOAD_RELAY_DELAYTIME)
			  {
				m_st_MeasureVltVaule.u16_CVlt_15_F = Get_Adc_Average(14,5);
				SEGGER_RTT_printf(0,"u16_CVlt_15_F = %d\n",m_st_MeasureVltVaule.u16_CVlt_15_F);
				LOAD_RELAY_OUT_C15 = 0;
				
				if(m_st_MeasureVltVaule.u16_CVlt_15_F >= 2242
				&& m_st_MeasureVltVaule.u16_CVlt_15_F <= 2478) //±5%
				{
					m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C15_F = 1;
				}
				else//报故障
				{
					m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C15_F = 2;
				}
				DelayCondition=0;
				DelayClear=0;
				u16_Temp = 5;
			 }
			}
			else //报故障
			{
				m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C15_N = 2;
				DelayCondition=0;
        DelayClear=0;				
				u16_Temp = 5;
			}
		}	
	}  
			break;
			
		case 5://C口 20V空载 满载测量
			if(DelayClear==0)
			{
			 TempPer10ms_1=0;
			 TempPer10ms_2=0;
       DelayClear=1;	
       VOLTAGE_RELAY_OUT_C = 1;				
			}
			if(TempPer10ms_1>=Port_30W_VOLTAGE_RELAY_DELAYTIME)
		  {
		  VOLTAGE_RELAY_OUT_C = 0;
		  if(TempPer10ms_1>=(Port_30W_VOLTAGE_RELAY_DELAYTIME+Port_30W_AD_DELAYTIME))
			{
			 if(DelayCondition==0)
			{
			   m_st_MeasureVltVaule.u16_CVlt_20_N =Get_Adc_Average(14,5);
				 DelayCondition=1;
				 TempPer10ms_2=0;
			}
			//满足20V±5% 后，吸合负载,进行带载测试 
			if(m_st_MeasureVltVaule.u16_CVlt_20_N >= 3121
				&& m_st_MeasureVltVaule.u16_CVlt_20_N <= 3499) 
			{
				m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C20_N = 1;
				LOAD_RELAY_OUT_C20 = 1;
				if(TempPer10ms_2>=Port_30W_LOAD_RELAY_DELAYTIME)
			  {
				m_st_MeasureVltVaule.u16_CVlt_20_F = Get_Adc_Average(14,5);
				LOAD_RELAY_OUT_C20 = 0;
				
				if(m_st_MeasureVltVaule.u16_CVlt_20_F >= 3023
				&& m_st_MeasureVltVaule.u16_CVlt_20_F <= 3342) //±5%
				{
					m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C20_F = 1;
				}
				else//报故障
				{
					m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C20_F = 2;
				}
				u16_Temp =6;
				VoltageCondition=1;
				m_st_MeasureVltVaule.bMeasureFinish = 1;
			 }	
			}
			else //报故障
			{
				m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C20_N = 2;
				u16_Temp = 6;
				VoltageCondition=1;
				m_st_MeasureVltVaule.bMeasureFinish = 1;
			}
		 }
	}			
			break;
	
		case 8: //复位 清除故障 状态标志
			m_st_MeasureVltVaule.bMeasureFinish = 0;
			BEEP = 0;
			LED_FAULT_A = 0;
			LED_FAULT_C = 0;
			LED_PASS_A = 0;
			LED_PASS_C = 0;
		  DelayCondition=0;
			DelayClear=0;
		  m_st_MeasureVltVaule.bMeasureStart = 1;
		
			break;
		case 9://清标志
			m_st_MeasureVltFault.un16_Alarm_1.All = 0x0;
			m_st_MeasureVltFault.un16_Alarm_2.All = 0x0;
			m_st_MeasureVltFault.un16_Alarm_3.All = 0x0;
		
			m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_A5_N =0;
			m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_A5_F =0;
			m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C5_N =0;
			m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C5_F =0;
			m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C9_N =0;
			m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C9_F =0;
			m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C12_N =0;
			m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C12_F =0;
			m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C15_N =0;
			m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C15_F =0;
			m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C20_N =0;
			m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C20_F =0;
		  DelayCondition=0;
		  BeepCnt=0;
		  u16_Temp = 0;
		
		
			break;
		default:
			break;
	}
	//测试完成后 处理
	if(m_st_MeasureVltVaule.bMeasureFinish == 1)
	{
		//蜂鸣器控制
		if(m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_A5_N == 2
		|| m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_A5_F == 2
		|| m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C5_N == 2
		|| m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C5_F == 2
		|| m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C9_N == 2
		|| m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C9_F== 2
		|| m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C12_N== 2
		|| m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C12_F == 2
		|| m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C15_N == 2
		|| m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C15_F == 2
		|| m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C20_N == 2
		|| m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C20_F == 2)
		{
			BEEP = 1;
			if(m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_A5_N == 2
			|| m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_A5_F == 2)
			{
				LED_FAULT_A = 1;
				LED_PASS_A = 0;
			}
			else
			{
				LED_FAULT_A = 0;
				LED_PASS_A = 1;
			}
			
			if( m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C5_N == 2
				|| m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C5_F == 2
				|| m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C9_N == 2
				|| m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C9_F== 2
				|| m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C12_N== 2
				|| m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C12_F == 2
				|| m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C15_N == 2
				|| m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C15_F == 2
				|| m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C20_N == 2
				|| m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C20_F == 2)
				{
					LED_FAULT_C =1;   
          LED_PASS_C = 0;
			  }
				else
				{
					LED_FAULT_C =0;   
          LED_PASS_C = 1;
				} 
			}
		else
		{
			LED_PASS_A = 1;
			LED_FAULT_A = 0;
			LED_PASS_C = 1;
			LED_FAULT_C =0;
			if(BeepCnt<4)
			{
			if(TempPer10ms_1>=10)
			{
				BEEP =!BEEP;
				TempPer10ms_1=0;
				BeepCnt++;
			}
		 }
	 }
	}
}






