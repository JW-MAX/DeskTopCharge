#include "Port_60W.h"



/******************************************************
函数名：int Port_60W_main(void)
描述：车充18W充电器测试功能函数
参数：无
*******************************************************/
int Port_60W_main(void)
{
	
	 ProductModel_Address=1;
	 VoltageCondition=1;
	
	 while(1)
		{
			Drv_SysTimer();
			MeasureVlt_60W();
			SendDataProcess();
		}
}


/******************************************************
函数名：void MeasureVlt(void)
描述：电压值测量
参数：无
*******************************************************/
void MeasureVlt_60W(void)
{
	static u16 u16_Temp = 0;
	u16	u16_TempAx = 0,u16_TempQx = 0,u16_TempCx = 0;
	static u8 u8_Measure_CVlt_N_Cnt=0;
	
	
 if(VoltageCondition==1)  
 {
	u16_TempAx = Get_Adc_Average(0,5);
	u16_TempQx = Get_Adc_Average(8,5);
	u16_TempCx = Get_Adc_Average(14,5);
	 
	//全部测试完成后，准备下一个产品的单边测试
	//判断A口、Q口、C口是否都掉电来复位整个测试过程的声光信号以及断开放电负载
	if(((u16_TempAx <= 300)&&(u16_TempQx <= 300)&&(u16_TempCx <= 300))
   &&(m_st_MeasureVltVaule.bMeasureFinish == 1)
	  )
	{
		u16_Temp = 18;  //复位整个测试过程的声光信号
	}
	//判断C口是否掉电来复位C口单边测试过程的声光信号
	if((u16_TempCx <= 300)
   &&(m_st_MeasureVltVaule.bCOpMeasureFinish == 1)
	  )
	{
		u16_Temp = 20;  //复位C口单边测试过程的声光信号
	}	 
	//判断C口、A口、Q口是否为5V来确定是否开始这个产品的单边测试
  if(
		((u16_TempAx>= 777) && (u16_TempAx <= 878))
	&&((u16_TempQx>= 780) && (u16_TempQx <= 878))
	&&((u16_TempCx>= 780) && (u16_TempCx <= 878))
	&&(m_st_MeasureVltVaule.bCOpMeasureStart == 1)
		)
	{
		LED_PASS_C = 1;
		LED_FAULT_C = 0;
		BEEP=1;
		u16_Temp = 19;  //清除整个测试过程的故障标志位
		m_st_MeasureVltVaule.bCOpMeasureStart = 0;
	}	
	//判断C口、A口、Q口是否为5V 来确定是否开始这个产品的测试
	if(
		((u16_TempAx>= 777) && (u16_TempAx <= 878))
	&&((u16_TempQx>= 780) && (u16_TempQx <= 878))
	&&((u16_TempCx>= 780) && (u16_TempCx <= 878))
	&&(m_st_MeasureVltVaule.bMeasureStart == 1)
		)
	{
		LED_PASS_A = 1;
		LED_FAULT_A = 0;
		LED_PASS_Q = 1;
		LED_FAULT_Q = 0;
		LED_PASS_C = 1;
		LED_FAULT_C = 0;
		BEEP=1;
		u16_Temp = 21;  //清除C口单边测试过程的故障标志位
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
		case 0: //C口 单边升压测量
		if(DelayClear==0)
			{
		   VoltageCondition=0;
			 u8_Measure_CVlt_N_Cnt=0;
			 TempPer10ms_1=0;
			 TempPer10ms_2=0;
       DelayClear=1;				
			}	
		if(TempPer10ms_1>=30)  //延时100ms
		 {	
		  LED_PASS_C = 0;
			LED_FAULT_C =0;
      BEEP=0;
      if(u8_Measure_CVlt_N_Cnt==0)		//C口5V空载测量	
      {
			 m_st_MeasureVltVaule.u16_CVlt_5_N =Get_Adc_Average(14,5);
				
			 if(m_st_MeasureVltVaule.u16_CVlt_5_N >= 780
			 && m_st_MeasureVltVaule.u16_CVlt_5_N <= 867) 
			 {
				m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C5_N = 1;
			 }
			 else
			 {
			  m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C5_N = 2;
			 }
			 u8_Measure_CVlt_N_Cnt++;
			 TempPer10ms_2=0;
			}
			if(u8_Measure_CVlt_N_Cnt==1)   //C口9V空载测量	
			{
			 VOLTAGE_RELAY_OUT_C = 1;
		   if(TempPer10ms_2>=Port_60W_VOLTAGE_RELAY_DELAYTIME)
			 {
			   VOLTAGE_RELAY_OUT_C = 0;
				 if(TempPer10ms_2>=(Port_60W_VOLTAGE_RELAY_DELAYTIME+Port_60W_AD_DELAYTIME))
				 {
					 m_st_MeasureVltVaule.u16_CVlt_9_N =Get_Adc_Average(14,5);
					 
					 if(m_st_MeasureVltVaule.u16_CVlt_9_N >= 1404
			     && m_st_MeasureVltVaule.u16_CVlt_9_N <= 1552) 
			     {
				    m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C9_N = 1;
				   } 
					 else
					 {
					  m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C9_N = 2;
					 }
			    u8_Measure_CVlt_N_Cnt++;
          TempPer10ms_2=0;			     
			   }
			  }
			 }
			if(u8_Measure_CVlt_N_Cnt==2)  //C口12V空载测量	
			{
       VOLTAGE_RELAY_OUT_C = 1;
		   if(TempPer10ms_2>=Port_60W_VOLTAGE_RELAY_DELAYTIME)
			 {
			   VOLTAGE_RELAY_OUT_C = 0;
				 if(TempPer10ms_2>=(Port_60W_VOLTAGE_RELAY_DELAYTIME+Port_60W_AD_DELAYTIME))
				 {
					 m_st_MeasureVltVaule.u16_CVlt_12_N =Get_Adc_Average(14,5);
					 
					 if(m_st_MeasureVltVaule.u16_CVlt_12_N >= 1869
			     && m_st_MeasureVltVaule.u16_CVlt_12_N <= 2066) 
			     {
				    m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C12_N = 1;
				   } 
					 else
					 {
					  m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C12_N = 2;
					 }
			    u8_Measure_CVlt_N_Cnt++;
          TempPer10ms_2=0;			     
			   }
			  }
			 }
		  if(u8_Measure_CVlt_N_Cnt==3)  //C口15V空载测量	
			{
			 VOLTAGE_RELAY_OUT_C = 1;
		   if(TempPer10ms_2>=Port_60W_VOLTAGE_RELAY_DELAYTIME)
			 {
			   VOLTAGE_RELAY_OUT_C = 0;
				 if(TempPer10ms_2>=(Port_60W_VOLTAGE_RELAY_DELAYTIME+Port_60W_AD_DELAYTIME))
				 {
					 m_st_MeasureVltVaule.u16_CVlt_15_N =Get_Adc_Average(14,5);
					 
					 if(m_st_MeasureVltVaule.u16_CVlt_15_N >= 2341
			     && m_st_MeasureVltVaule.u16_CVlt_15_N <= 2587) 
			     {
				    m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C15_N = 1;
				   } 
					 else
					 {
					  m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C15_N = 2;
					 }
			    u8_Measure_CVlt_N_Cnt++;
          TempPer10ms_2=0;			     
			   }
			  }
			 }
			if(u8_Measure_CVlt_N_Cnt==4)  //C口20V空载测量	
			{
			 VOLTAGE_RELAY_OUT_C = 1;				
		   if(TempPer10ms_2>=Port_60W_VOLTAGE_RELAY_DELAYTIME)
			 {
			   VOLTAGE_RELAY_OUT_C = 0;
				 if(TempPer10ms_2>=(Port_60W_VOLTAGE_RELAY_DELAYTIME+Port_60W_AD_DELAYTIME))
				 {
					 m_st_MeasureVltVaule.u16_CVlt_20_N =Get_Adc_Average(14,5);
					 
					 if(m_st_MeasureVltVaule.u16_CVlt_20_N >= 3121
			     && m_st_MeasureVltVaule.u16_CVlt_20_N <= 3499) 
			     {
				    m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C20_N = 1;
				   } 
					 else
					 {
					  m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C20_N = 2;
					 }
			     u8_Measure_CVlt_N_Cnt=0;
           VoltageCondition=1;
					 u16_Temp=29;
           m_st_MeasureVltVaule.bCOpMeasureFinish=1;					 
			   }
			  }
			 }
		 }
		break;
			 
		case 1:  //A口 5V空载 满载测量
	  if(DelayClear==0)
			{
		   VoltageCondition=0;
			 TempPer10ms_1=0;
			 TempPer10ms_2=0;
       DelayClear=1;				
			}	
		if(TempPer10ms_1>=20)  //延时100ms
		{	
		  LED_PASS_A = 0;
			LED_FAULT_A = 0;
			LED_PASS_Q = 0;
		  LED_FAULT_Q = 0;
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
				 && m_st_MeasureVltVaule.u16_AVlt_5_N <= 867) 
			{
				m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_A5_N = 1;
				LOAD_RELAY_OUT_A5 = 1;
				if(TempPer10ms_2>=Port_60W_LOAD_RELAY_DELAYTIME)
				{
				m_st_MeasureVltVaule.u16_AVlt_5_F = Get_Adc_Average(0,5);
				LOAD_RELAY_OUT_A5 = 0;
				
				if(m_st_MeasureVltVaule.u16_AVlt_5_F >= 598
				   && m_st_MeasureVltVaule.u16_AVlt_5_F <= 845) //±5%
				{
					m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_A5_F= 1;
					
				}
				else//报故障
				{
					m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_A5_F = 2;
				}
				DelayCondition=0;
        DelayClear=0;				
				u16_Temp = 2;
			 }
			}
			else //报故障
			{
				m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_A5_N = 2;
				DelayCondition=0;
        DelayClear=0;				
				u16_Temp = 2;
			}
   }			
		break;
		
		case 2://Q口 5V空载 满载测量
		if(DelayClear==0)
		{
		 TempPer10ms_1=0;
		 TempPer10ms_2=0;
		 TempPer10ms_3=0;
		 DelayClear=1;				
		}
		if(DelayCondition==0)
		{
			m_st_MeasureVltVaule.u16_QVlt_5_N = Get_Adc_Average(8,5);//A口 5V空载采样
			DelayCondition=1;
			TempPer10ms_2=0;
		}	
		//满足5V±5% 后，吸合负载,进行带载测试 
		if(m_st_MeasureVltVaule.u16_QVlt_5_N >= 777
		&& m_st_MeasureVltVaule.u16_QVlt_5_N <= 867) 
		{
			m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_Q5_N = 1;
			LOAD_RELAY_OUT_Q5 = 1;
			if(TempPer10ms_2>=Port_60W_LOAD_RELAY_DELAYTIME)//延时1000ms
			{
			m_st_MeasureVltVaule.u16_QVlt_5_F = Get_Adc_Average(8,5);
			LOAD_RELAY_OUT_Q5 = 0;
			
			if(m_st_MeasureVltVaule.u16_QVlt_5_F >= 598
			&& m_st_MeasureVltVaule.u16_QVlt_5_F <= 845) //±5%
			{
				m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_Q5_F= 1;
				//SEGGER_RTT_printf(0,"bFault_Q5_F = %d\n",m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_Q5_F);
			}
			else//报故障
			{				
				m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_Q5_F = 2;
			}
			DelayCondition=0;
			DelayClear=0;				
			u16_Temp = 3;
		 }
		}
		else //报故障
		{				
			m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_Q5_N = 2;
			DelayCondition=0;
			DelayClear=0;				
			u16_Temp = 3;
		}  			
	break;
		
	case 3://Q口 9V空载 满载测量
			if(DelayClear==0)
			{
			 TempPer10ms_1=0;
			 TempPer10ms_2=0;
			 TempPer10ms_3=0;
       DelayClear=1;	
		   VOLTAGE_RELAY_OUT_Q = 1;				
			}
		  if(TempPer10ms_1>=Port_60W_VOLTAGE_RELAY_DELAYTIME)
			{
			VOLTAGE_RELAY_OUT_Q = 0;
		
			if(TempPer10ms_1>=(Port_60W_VOLTAGE_RELAY_DELAYTIME+Port_60W_AD_DELAYTIME))
		  {
			 if(DelayCondition==0)
			 {
				 m_st_MeasureVltVaule.u16_QVlt_9_N =Get_Adc_Average(8,5);
				 DelayCondition=1;
				 TempPer10ms_2=0;
				}
			
			//满足9V±5% 后，吸合负载,进行带载测试 
			if(m_st_MeasureVltVaule.u16_QVlt_9_N >= 1404
			&& m_st_MeasureVltVaule.u16_QVlt_9_N <= 1552) 
			{
				m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_Q9_N = 1;
				LOAD_RELAY_OUT_Q9 = 1;
				if(TempPer10ms_2>=Port_60W_LOAD_RELAY_DELAYTIME)
			  {
				m_st_MeasureVltVaule.u16_QVlt_9_F = Get_Adc_Average(8,5);
				//SEGGER_RTT_printf(0,"u16_CVlt_9_F = %d\n",m_st_MeasureVltVaule.u16_CVlt_9_F);
				LOAD_RELAY_OUT_Q9 = 0;
				
				if(m_st_MeasureVltVaule.u16_QVlt_9_F >= 1234
				&& m_st_MeasureVltVaule.u16_QVlt_9_F <= 1500) //±5%
				{					
					m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_Q9_F = 1;
					
				}
				else//报故障
				{				
          m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_Q9_F = 2;
				}
				DelayCondition=0;
				DelayClear=0;
				u16_Temp = 4;
			 }
			}
				
			else //报故障
			{				
				m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_Q9_N = 2;
				DelayCondition=0;
        DelayClear=0;				
				u16_Temp = 4;
			}
		}
  }			
	break;	
	
	case 4://Q口 12V空载 满载测量
			if(DelayClear==0)
			{
			 TempPer10ms_1=0;
			 TempPer10ms_2=0;
       DelayClear=1;
       VOLTAGE_RELAY_OUT_Q = 1;
		 }
		  if(TempPer10ms_1>=Port_60W_VOLTAGE_RELAY_DELAYTIME)
			{
			VOLTAGE_RELAY_OUT_Q = 0;
		
			if(TempPer10ms_1>=(Port_60W_VOLTAGE_RELAY_DELAYTIME+Port_60W_AD_DELAYTIME))
		  {
			 if(DelayCondition==0)
			 {
				 m_st_MeasureVltVaule.u16_QVlt_12_N =Get_Adc_Average(8,5);
				 DelayCondition=1;
				 TempPer10ms_2=0;
				}
			
			//满足12V±5% 后，吸合负载,进行带载测试 
			if(m_st_MeasureVltVaule.u16_QVlt_12_N >= 1869
			&& m_st_MeasureVltVaule.u16_QVlt_12_N <= 2066) 
			{
				m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_Q12_N = 1;
				LOAD_RELAY_OUT_Q12 = 1;
				if(TempPer10ms_2>=(Port_60W_LOAD_RELAY_DELAYTIME))
			  {
				m_st_MeasureVltVaule.u16_QVlt_12_F = Get_Adc_Average(8,5);
				LOAD_RELAY_OUT_Q12 = 0;
				
				if(m_st_MeasureVltVaule.u16_QVlt_12_F >= 1783
				&& m_st_MeasureVltVaule.u16_QVlt_12_F <= 1971) //±5%
				{					
					m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_Q12_F = 1;
					
				}
				else//报故障
				{					
          m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_Q12_F = 2;
				}
				DelayCondition=0;
				DelayClear=0;
				u16_Temp = 5;
			 }
			}
				
			else //报故障
			{				
				m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_Q12_N = 2;
				DelayCondition=0;
        DelayClear=0;				
				u16_Temp = 5;
			}
		}
 }			
			break;	
			
		case 5: //C口 5V空载 满载测量
			if(DelayClear==0)
			{
			 TempPer10ms_1=0;
			 TempPer10ms_2=0;
       DelayClear=1;				
			}
		  if(TempPer10ms_1>=(Port_60W_RestartTime*2))
			{					
		  if(DelayCondition==0)
			{
			 m_st_MeasureVltVaule.u16_CVlt_5_N =Get_Adc_Average(14,5);
			 DelayCondition=1;
			 TempPer10ms_2=0;
			}
			//满足5V±5% 后，吸合负载,进行带载测试
      
			if(m_st_MeasureVltVaule.u16_CVlt_5_N >= 780
			&& m_st_MeasureVltVaule.u16_CVlt_5_N <= 867) 
			{
				m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C5_N = 1;
				LOAD_RELAY_OUT_C5 = 1;
				if(TempPer10ms_2>=(Port_60W_LOAD_RELAY_DELAYTIME))
				{
				m_st_MeasureVltVaule.u16_CVlt_5_F = Get_Adc_Average(14,5);
				LOAD_RELAY_OUT_C5 = 0;
				
				if(m_st_MeasureVltVaule.u16_CVlt_5_F >= 598
				&& m_st_MeasureVltVaule.u16_CVlt_5_F <= 845) //±5%
				{

					m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C5_F = 1;
					
				}
				
				
				else//报故障
				{
					m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C5_F = 2;
				}
				DelayCondition=0;
				DelayClear=0;
				u16_Temp = 6;
			 }
			}
			else //报故障
			{				
				m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C5_N = 2;
				DelayCondition=0;
        DelayClear=0;				
				u16_Temp = 6;
			}
  }    
			break;
			
		case 6://C口 9V空载 满载测量
			if(DelayClear==0)
			{
			 TempPer10ms_1=0;
			 TempPer10ms_2=0;
       DelayClear=1;
       VOLTAGE_RELAY_OUT_C = 1;
       VOLTAGE_RELAY_OUT_Q = 1;			//重置Q口	
			}
		  if(TempPer10ms_1>=Port_60W_VOLTAGE_RELAY_DELAYTIME)
			{
			VOLTAGE_RELAY_OUT_C = 0;
		  VOLTAGE_RELAY_OUT_Q = 0;      //重置Q口	
			if(TempPer10ms_1>=(Port_60W_VOLTAGE_RELAY_DELAYTIME+Port_60W_AD_DELAYTIME))
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
				if(TempPer10ms_2>=Port_60W_LOAD_RELAY_DELAYTIME)
			  {
				m_st_MeasureVltVaule.u16_CVlt_9_F = Get_Adc_Average(14,5);
				LOAD_RELAY_OUT_C9 = 0;
				
				if(m_st_MeasureVltVaule.u16_CVlt_9_F >= 1296
				&& m_st_MeasureVltVaule.u16_CVlt_9_F <= 1500) //±5%
				{					
					m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C9_F = 1;
					
				}
				else//报故障
				{					
          m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C9_F = 2;
				}
				DelayCondition=0;
				DelayClear=0;
				u16_Temp = 7;
			 }
			}
				
			else //报故障
			{
				m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C9_N = 2;
				DelayCondition=0;
        DelayClear=0;				
				u16_Temp = 7;
			}
		}
 }			
			break;
			
		case 7://C口 12V空载 满载测量
			if(DelayClear==0)
			{
			 TempPer10ms_1=0;
			 TempPer10ms_2=0;			
       DelayClear=1;
       VOLTAGE_RELAY_OUT_C = 1;	
       VOLTAGE_RELAY_OUT_Q = 1;			//重置Q口					
			}
		  if(TempPer10ms_1>=Port_60W_VOLTAGE_RELAY_DELAYTIME)
			{ 
				VOLTAGE_RELAY_OUT_C = 0;
				VOLTAGE_RELAY_OUT_Q = 0;			//重置Q口	
			
		  if(TempPer10ms_1>=(Port_60W_VOLTAGE_RELAY_DELAYTIME+Port_60W_AD_DELAYTIME))
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
				if(TempPer10ms_2>=(Port_60W_LOAD_RELAY_DELAYTIME))
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
				u16_Temp = 8;
				
			 }
			}
			else //报故障
			{
				m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C12_N = 2;
				DelayCondition=0;
        DelayClear=0;				
				u16_Temp = 8;
				
			}
    }
 }			
			break;

    case 8://C口 15V空载 满载测量
			if(DelayClear==0)
			{
			 TempPer10ms_1=0;
			 TempPer10ms_2=0;
       DelayClear=1;	
       VOLTAGE_RELAY_OUT_C = 1;				
			}
		if(TempPer10ms_1>=Port_60W_VOLTAGE_RELAY_DELAYTIME)
		{
			VOLTAGE_RELAY_OUT_C = 0;
		
			if(TempPer10ms_1>=(Port_60W_VOLTAGE_RELAY_DELAYTIME+Port_60W_AD_DELAYTIME))
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
				if(TempPer10ms_2>=Port_60W_LOAD_RELAY_DELAYTIME)
			  {
				m_st_MeasureVltVaule.u16_CVlt_15_F = Get_Adc_Average(14,5);
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
				u16_Temp = 9;
			 }
			}
			else //报故障
			{
				m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C15_N = 2;
				DelayCondition=0;
        DelayClear=0;				
				u16_Temp = 9;
			}
		}	
	}  
			break;
			
		case 9://C口 20V空载 满载测量
			if(DelayClear==0)
			{
			 TempPer10ms_1=0;
			 TempPer10ms_2=0;
       DelayClear=1;	
		   VOLTAGE_RELAY_OUT_C = 1;				
			}
		
		if(TempPer10ms_1>=Port_60W_VOLTAGE_RELAY_DELAYTIME)
		{
			VOLTAGE_RELAY_OUT_C = 0;
		
			if(TempPer10ms_1>=(Port_60W_VOLTAGE_RELAY_DELAYTIME+Port_60W_AD_DELAYTIME))
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
				if(TempPer10ms_2>=Port_60W_LOAD_RELAY_DELAYTIME)
			  {
				m_st_MeasureVltVaule.u16_CVlt_20_F = Get_Adc_Average(14,5);
				//LOAD_RELAY_OUT_C20 = 0;
				
				if(m_st_MeasureVltVaule.u16_CVlt_20_F >= 3023
				&& m_st_MeasureVltVaule.u16_CVlt_20_F <= 3342) //±5%
				{
					m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C20_F = 1;
				}
				else//报故障
				{
					m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C20_F = 2;
				}
				DelayCondition=0;
				DelayClear=0;		
				u16_Temp = 10;
			 }	
			}
			else //报故障
			{
				m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C20_N = 2;
				DelayCondition=0;
				DelayClear=0;		
				u16_Temp = 10;
			}
	  }
	}			
			break;

	  case 10:  //C口与A口功率分配测试
			if(DelayCondition==0)
			{
				m_st_MeasureVltVaule.u16_CVltDis_20_Before = Get_Adc_Average(14,5); 
			  LOAD_RELAY_OUT_A5=1;  //吸合A口5V负载
				DelayCondition=1;
				TempPer10ms_1=0;
			}
			if(TempPer10ms_1>=Port_60W_RestartTime)
			 {
				if(VltControlCnt<Port_60W_VOLTAGE_RELAY_DELAYTIME_Cnt)
				{
					if(TempPer10ms_2>=Port_60W_VOLTAGE_RELAY_DELAYTIME)
					{
						if(VltControlTimeCnt<2)
						{
						VOLTAGE_RELAY_OUT_C =!VOLTAGE_RELAY_OUT_C;
						VltControlCnt++;
						}
						VltControlTimeCnt++;
						TempPer10ms_2=0;
						if(VltControlTimeCnt>Port_60W_VltControlTime_Cnt)
						{
						 VltControlTimeCnt=0;
						}
					}
				}
			 if(TempPer10ms_2>=(Port_60W_AD_DELAYTIME*5))
			 {
				m_st_MeasureVltVaule.u16_CVltDisA_20_F = Get_Adc_Average(14,5);
				 if(
					  (m_st_MeasureVltVaule.u16_CVltDis_20_Before>=3023)
				  &&(m_st_MeasureVltVaule.u16_CVltDis_20_Before<=3342)
				  &&(m_st_MeasureVltVaule.u16_CVltDisA_20_F>=2532)
				  &&(m_st_MeasureVltVaule.u16_CVltDisA_20_F<=3095)
				   )
					 {
						m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_C_DisA = 1;
					 }
					 else
					 {
					  m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_C_DisA = 2;
					 }
				VltControlCnt=0;
				TempPer10ms_1=0;
				TempPer10ms_2=0;
				u16_Temp=11;
			 }
		  }
			break;
		case 11:  //C口与A口、Q口功率分配测试
			LOAD_RELAY_OUT_Q9=1;  //吸合Q口9V负载
		  if(TempPer10ms_1>=(Port_60W_RestartTime*2))
			 {
				if(VltControlCnt<Port_60W_VOLTAGE_RELAY_DELAYTIME_Cnt)
				{
					if(TempPer10ms_2>=Port_60W_VOLTAGE_RELAY_DELAYTIME)
					{
						if(VltControlTimeCnt<2)
						{
						VOLTAGE_RELAY_OUT_C =!VOLTAGE_RELAY_OUT_C;					
						VltControlCnt++;
						}
						VltControlTimeCnt++;
						TempPer10ms_2=0;
						if(VltControlTimeCnt>Port_60W_VltControlTime_Cnt)
						{
						 VltControlTimeCnt=0;
						}
					}
				}
			if(TempPer10ms_2>=(Port_60W_AD_DELAYTIME*5))
			 {
				m_st_MeasureVltVaule.u16_CVltDisAQ_20_F = Get_Adc_Average(14,5);
				 if(
				    (m_st_MeasureVltVaule.u16_CVltDisAQ_20_F>=1903)
				  &&(m_st_MeasureVltVaule.u16_CVltDisAQ_20_F<=2768)
				   )
					 {
						m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_C_DisAQ = 1;
					 }
					 else
					 {
					  m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_C_DisAQ = 2;
					 }
				VltControlCnt=0;
				TempPer10ms_1=0;
				TempPer10ms_2=0;
				u16_Temp=12;
			 }
			}
			break;
		case 12:  //C口与Q口功率分配测试
      if(DelayClear==0)
			{	
		   LOAD_RELAY_OUT_A5=0;  //断开A口5V负载
			 DelayClear=1;
			}		  
			if(TempPer10ms_1>=(Port_60W_RestartTime*20))
			 {
				if(VltControlCnt<Port_60W_VOLTAGE_RELAY_DELAYTIME_Cnt)
				{
					if(TempPer10ms_2>=Port_60W_VOLTAGE_RELAY_DELAYTIME)
					{
						if(VltControlTimeCnt<2)
						{
						VOLTAGE_RELAY_OUT_C =!VOLTAGE_RELAY_OUT_C;
						VltControlCnt++;
						}
						VltControlTimeCnt++;
						TempPer10ms_2=0;
						if(VltControlTimeCnt>Port_60W_VltControlTime_Cnt)
						{
						 VltControlTimeCnt=0;
						}
					}
				}
			if(TempPer10ms_2>=(Port_60W_AD_DELAYTIME*5))
			 {
				m_st_MeasureVltVaule.u16_CVltDisQ_20_F = Get_Adc_Average(14,5);
				 if(
				    (m_st_MeasureVltVaule.u16_CVltDisQ_20_F>=2532)
				  &&(m_st_MeasureVltVaule.u16_CVltDisQ_20_F<=3095)
				   )
					 {
						m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_C_DisQ = 1;
						VltControlCnt=0;
						TempPer10ms_1=0;
						TempPer10ms_2=0;
						u16_Temp=13;
					 }
					 else
					 {
						LOAD_RELAY_OUT_A5=1;
						if(TempPer10ms_2>=Port_60W_RestartTime)
						{	
						 VltControlCnt=0;
						 TempPer10ms_1=0;
						 TempPer10ms_2=0;
						 u16_Temp=31;
						}	
					  //m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_C_DisQ = 2;
					 }
				
			  }
			}
			break;
			
			case 31:  //重测C口与Q口功率分配测试
			LOAD_RELAY_OUT_A5=0;  //断开A口5V负载
		  if(TempPer10ms_1>=(Port_60W_RestartTime*6))
			 {
				if(VltControlCnt<Port_60W_VOLTAGE_RELAY_DELAYTIME_Cnt)
				{
					if(TempPer10ms_2>=Port_60W_VOLTAGE_RELAY_DELAYTIME)
					{
						if(VltControlTimeCnt<2)
						{
						VOLTAGE_RELAY_OUT_C =!VOLTAGE_RELAY_OUT_C;
						VltControlCnt++;
						}
						VltControlTimeCnt++;
						TempPer10ms_2=0;
						if(VltControlTimeCnt>Port_60W_VltControlTime_Cnt)
						{
						 VltControlTimeCnt=0;
						}
					}
				}
			if(TempPer10ms_2>=(Port_60W_AD_DELAYTIME*5))
			 {
				m_st_MeasureVltVaule.u16_CVltDisQ_20_F = Get_Adc_Average(14,5);
				 if(
				    (m_st_MeasureVltVaule.u16_CVltDisQ_20_F>=2532)
				  &&(m_st_MeasureVltVaule.u16_CVltDisQ_20_F<=3095)
				   )
					 {
						m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_C_DisQ = 1;
					 }
					 else
					 {
					  m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_C_DisQ = 2;
					 }
				VltControlCnt=0;
				TempPer10ms_1=0;
				TempPer10ms_2=0;
				u16_Temp=13;
			 }
			}
			break;
		case 13:  //C口恢复测试
			LOAD_RELAY_OUT_Q9=0;  //断开Q口9V负载
		  if(TempPer10ms_1>=(Port_60W_RestartTime*2))
			 {
				if(VltControlCnt<Port_60W_VOLTAGE_RELAY_DELAYTIME_Cnt)
				{
					if(TempPer10ms_2>=Port_60W_VOLTAGE_RELAY_DELAYTIME)
					{
						if(VltControlTimeCnt<2)
						{
						VOLTAGE_RELAY_OUT_C =!VOLTAGE_RELAY_OUT_C;
						VltControlCnt++;
						}
						VltControlTimeCnt++;
						TempPer10ms_2=0;
						if(VltControlTimeCnt>Port_60W_VltControlTime_Cnt)
						{
						 VltControlTimeCnt=0;
						}
					 }
				 }
			if(TempPer10ms_2>=Port_60W_AD_DELAYTIME)
			 {
				m_st_MeasureVltVaule.u16_CVltCover_20_F = Get_Adc_Average(14,5);
				 if(
				    (m_st_MeasureVltVaule.u16_CVltCover_20_F>=3023)
				  &&(m_st_MeasureVltVaule.u16_CVltCover_20_F<=3342)
				   )
					 {
						m_st_MeasureVltFault.un16_Alarm_4.Bits.bFault_C_Cover = 1;
					 }
					 else
					 {
					  m_st_MeasureVltFault.un16_Alarm_4.Bits.bFault_C_Cover = 2;
					 }
				LOAD_RELAY_OUT_C20=0;  //断开C口20V负载
				VltControlCnt=0;
				DelayClear=0;
				DelayCondition=0;
				u16_Temp=14;
			 }
			}
			break;
		case 14:  //A口过流测试
			if(DelayClear==0)
			{
		   TempPer10ms_1=0;
			 TempPer10ms_2=0;
       DelayClear=1;
			 LOAD_RELAY_OUT_A20=1;				
			}	
		  if(TempPer10ms_1>=Port_60W_LOAD_RELAY_DELAYTIME)
			 {
				 if(DelayCondition==0)
				 {
				  m_st_MeasureVltVaule.u16_AVltOverC_5_Middle = Get_Adc_Average(0,5);
					LOAD_RELAY_OUT_A20=0;
					TempPer10ms_2=0;
					DelayCondition=1;
				 }				
				 if(TempPer10ms_2>=Port_60W_RestartTime)
				 {
				  m_st_MeasureVltVaule.u16_AVltOverC_5_Behind = Get_Adc_Average(0,5);
				 if(
				    (m_st_MeasureVltVaule.u16_AVltOverC_5_Middle<=Port_60W_OverCurrent_5V)
				  &&(m_st_MeasureVltVaule.u16_AVltOverC_5_Behind>=777)
				  &&(m_st_MeasureVltVaule.u16_AVltOverC_5_Behind<=867)
				   )
					 {
						m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_A_OverC = 1;
						DelayClear=0;
				    DelayCondition=0;
						u16_Temp=15;
					 }
					 else
					 {
					  m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_A_OverC = 2;
						DelayClear=0;
				    DelayCondition=0;
						u16_Temp=15;
					 }
          }					 
			 }
			break;
		case 15:  //Q口过流测试
			if(DelayClear==0)
			{
		   TempPer10ms_1=0;
			 TempPer10ms_2=0;
       DelayClear=1;
			 LOAD_RELAY_OUT_Q15=1;
			}	
		  if(TempPer10ms_1>=Port_60W_LOAD_RELAY_DELAYTIME)
			 {
				 if(DelayCondition==0)
				 {
					m_st_MeasureVltVaule.u16_QVltOverC_5_Middle = Get_Adc_Average(8,5);
					LOAD_RELAY_OUT_Q15=0;
					TempPer10ms_2=0;
					DelayCondition=1;
				 }				
				 if(TempPer10ms_2>=Port_60W_RestartTime)
				 {
					m_st_MeasureVltVaule.u16_QVltOverC_5_Behind = Get_Adc_Average(8,5);
					 
				 if(
				    (m_st_MeasureVltVaule.u16_QVltOverC_5_Middle<=Port_60W_OverCurrent_5V)
				  &&(m_st_MeasureVltVaule.u16_QVltOverC_5_Behind>=777)
				  &&(m_st_MeasureVltVaule.u16_QVltOverC_5_Behind<=867)
				   )
					 {
						m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_Q_OverC = 1;
						DelayClear=0;
				    DelayCondition=0;
						u16_Temp=16;
					 }
					 else
					 {
					  m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_Q_OverC = 2;
						DelayClear=0;
				    DelayCondition=0;
						u16_Temp=16;
					 }
          }					 
			 }
			break;
		case 16:  //C口过流测试
			if(DelayClear==0)
			{
		   TempPer10ms_1=0;
			 TempPer10ms_2=0;
       DelayClear=1;
			 LOAD_RELAY_OUT_C3=1;
			}
		  if(TempPer10ms_1>=Port_60W_LOAD_RELAY_DELAYTIME)
			 {
				 if(DelayCondition==0)
				 {
					m_st_MeasureVltVaule.u16_CVltOverC_5_Middle = Get_Adc_Average(14,5);
					LOAD_RELAY_OUT_C3=0;
					TempPer10ms_2=0;
					DelayCondition=1;
				 }				
				 if(TempPer10ms_2>=Port_60W_RestartTime)
				 {
				  m_st_MeasureVltVaule.u16_CVltOverC_5_Behind = Get_Adc_Average(14,5);
				 if(
				    (m_st_MeasureVltVaule.u16_CVltOverC_5_Middle<=Port_60W_OverCurrent_5V)
				  &&(m_st_MeasureVltVaule.u16_CVltOverC_5_Behind>=780)
				  &&(m_st_MeasureVltVaule.u16_CVltOverC_5_Behind<=867)
				   )
					 {
						m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_C_OverC = 1;
						DelayClear=0;
				    DelayCondition=0;
						m_st_MeasureVltVaule.bMeasureFinish = 1;
				    VoltageCondition=1;
						u16_Temp=30;
					 }
					 else
					 {
					  m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_C_OverC = 2;
						DelayClear=0;
				    DelayCondition=0;
						m_st_MeasureVltVaule.bMeasureFinish = 1;
				    VoltageCondition=1;
						u16_Temp=30;
					 }
          }					 
			 }
			break;
		case 18: //复位 清除故障 状态标志			
			m_st_MeasureVltVaule.bMeasureFinish = 0;
			BEEP = 0;
			LED_FAULT_A = 0;
			LED_FAULT_Q = 0;
		  LED_FAULT_C = 0;
			LED_PASS_A = 0;
		  LED_PASS_Q = 0;
			LED_PASS_C = 0;
		  LOAD_RELAY_OUT_A5=0; //断开放电负载
			LOAD_RELAY_OUT_Q12=0;//断开放电负载
			LOAD_RELAY_OUT_C20=0;//断开放电负载
		  DelayCondition=0;
			DelayClear=0;
		  m_st_MeasureVltVaule.bCOpMeasureStart = 1;
		  
			break;
		
		case 19://清标志
			m_st_MeasureVltFault.un16_Alarm_1.All = 0x0;
			m_st_MeasureVltFault.un16_Alarm_2.All = 0x0;
			m_st_MeasureVltFault.un16_Alarm_3.All = 0x0;
		  m_st_MeasureVltFault.un16_Alarm_4.All = 0x0;
		
		  m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_A5_N =0;
			m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_A5_F =0;
			m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_Q5_N =0;
			m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_Q5_F =0;
			m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_Q9_N =0;
			m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_Q9_F =0;
			m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_Q12_N =0;
			m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_Q12_F =0;
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
			m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_A_OverC =0;
			m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_Q_OverC =0;
			m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_C_OverC =0;
			m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_C_DisA =0;
			m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_C_DisAQ =0;
			m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_C_DisQ =0;
			m_st_MeasureVltFault.un16_Alarm_4.Bits.bFault_C_Cover =0;
			m_st_MeasureVltFault.un16_Alarm_4.Bits.bFault_C_Opposite =0;

		  DelayCondition=0;
		  BeepCnt=0;
			u16_Temp=0;
			
			break;
			
		case 20:
			m_st_MeasureVltVaule.bCOpMeasureFinish = 0;
			BEEP = 0;
		  LED_FAULT_C = 0;
			LED_PASS_C = 0;
		  DelayCondition=0;
			DelayClear=0;
		  m_st_MeasureVltVaule.bMeasureStart = 1;
		
			break;
		
		case 21:
			m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C5_N =0;
			m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C9_N =0;
			m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C12_N =0;
			m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C15_N =0;
			m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C20_N =0;
		
		  DelayCondition=0;
		  BeepCnt=0;
		  u16_Temp=1;
		
			break;
		
		default:
			break;
	}
	//C口单边测试完成后故障处理
	if(m_st_MeasureVltVaule.bCOpMeasureFinish == 1)
	{
		//置C口单边测试结果标志和声光信号控制
		if(m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C5_N == 2
		|| m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C9_N == 2
		|| m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C12_N== 2
		|| m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C15_N == 2
		|| m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C20_N == 2)
		{
			m_st_MeasureVltFault.un16_Alarm_4.Bits.bFault_C_Opposite=2;
			BEEP = 1;				
			LED_FAULT_C =1;   
      LED_PASS_C = 0;			 
		}
		else
		{
			m_st_MeasureVltFault.un16_Alarm_4.Bits.bFault_C_Opposite=1;
			LED_PASS_C = 1;
			LED_FAULT_C =0;
			if(BeepCnt<4)
			{
			if(TempPer10ms_1>=7)
			{
				BEEP =!BEEP;
				TempPer10ms_1=0;
				BeepCnt++;
			}
		 }
	 }
	}
	//整个测试完成后故障处理
	if(m_st_MeasureVltVaule.bMeasureFinish == 1)
	{
		//蜂鸣器控制
		if(
			 m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_A5_N == 2
		|| m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_A5_F == 2
		|| m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_Q5_N == 2
		|| m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_Q5_F == 2
		|| m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_Q9_N == 2
		|| m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_Q9_F == 2
		|| m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_Q12_N == 2
		|| m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_Q12_F == 2
		|| m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C5_N == 2
		|| m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C5_F == 2
		|| m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C9_N == 2
		|| m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C9_F== 2
		|| m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C12_N== 2
		|| m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C12_F == 2
		|| m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C15_N == 2
		|| m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C15_F == 2
		|| m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C20_N == 2
		|| m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C20_F == 2
		|| m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_A_OverC == 2
		|| m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_Q_OverC == 2
		|| m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_C_OverC == 2
		|| m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_C_DisA == 2
		|| m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_C_DisAQ == 2
		|| m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_C_DisQ == 2
		|| m_st_MeasureVltFault.un16_Alarm_4.Bits.bFault_C_Cover == 2
		|| m_st_MeasureVltFault.un16_Alarm_4.Bits.bFault_C_Opposite==2
		  )
		{
			BEEP = 1;
			LOAD_RELAY_OUT_A5=1; //测试完放电
			LOAD_RELAY_OUT_Q12=1;//测试完放电
			LOAD_RELAY_OUT_C20=1;//测试完放电
		if(
			 m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_A5_N == 2
		|| m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_A5_F == 2
		|| m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_A_OverC == 2
		  )
			{
				LED_FAULT_A = 1;
				LED_PASS_A = 0;
			}
			else
			{
				LED_FAULT_A = 0;
				LED_PASS_A = 1;
			}
		if(
			 m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_Q5_N == 2
		|| m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_Q5_F == 2
		|| m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_Q9_N == 2
		|| m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_Q9_F == 2
		|| m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_Q12_N == 2
		|| m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_Q12_F == 2
		|| m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_Q_OverC == 2
		  )
			{
				LED_FAULT_Q = 1;
				LED_PASS_Q = 0;
			}
			else
			{
				LED_FAULT_Q = 0;
				LED_PASS_Q = 1;
			}
			
			if( 
				   m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C5_N == 2
				|| m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C5_F == 2
				|| m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C9_N == 2
				|| m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C9_F== 2
				|| m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C12_N== 2
				|| m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C12_F == 2
				|| m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C15_N == 2
				|| m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C15_F == 2
				|| m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C20_N == 2
				|| m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C20_F == 2
			  || m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_C_OverC == 2
				|| m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_C_DisA == 2
				|| m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_C_DisAQ == 2
				|| m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_C_DisQ == 2
				|| m_st_MeasureVltFault.un16_Alarm_4.Bits.bFault_C_Cover == 2
				|| m_st_MeasureVltFault.un16_Alarm_4.Bits.bFault_C_Opposite==2
			  )
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
			LED_PASS_Q = 1;
			LED_FAULT_Q = 0;
			LED_PASS_C = 1;
			LED_FAULT_C =0;
			LOAD_RELAY_OUT_A5=1; //测试完放电
			LOAD_RELAY_OUT_Q12=1;//测试完放电
			LOAD_RELAY_OUT_C20=1;//测试完放电
			if(BeepCnt<4)
			{
				if(TempPer10ms_1>=7)
				{
					BEEP =!BEEP;
					TempPer10ms_1=0;
					BeepCnt++;
				}
		  }
	 }
	}
 }

 
 
