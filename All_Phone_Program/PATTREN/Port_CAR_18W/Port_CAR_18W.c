#include "Port_CAR_18W.h"


/******************************************************
函数名：int Port_CAR_18W_main(void)
描述：车充18W充电器测试功能函数
参数：无
*******************************************************/
int Port_CAR_18W_main(void)
{
	 
	 ProductModel_Address=1;
	 VoltageCondition=1;
	 VOLTAGE_RELAY_OUT_Q = 1;//接通A1口
	 LOAD_RELAY_OUT_Q20 = 1;//接通A2口
	
	 while(1)
		{
			Drv_SysTimer();
			MeasureVlt_CAR_18W();
			SendDataProcess();
		}
}


/******************************************************
函数名：void MeasureVlt(void)
描述：电压值测量
参数：无
*******************************************************/
void MeasureVlt_CAR_18W(void)
{
	static u16 u16_Temp = 0;
	u16	u16_TempAx = 0,u16_TempBx = 0;
	
 if(VoltageCondition==1)
 {
	
	u16_TempAx = Get_Adc_Average(0,5);
	u16_TempBx = Get_Adc_Average(14,5);
	
	//判断A1口或A2口是否掉电来复位声光信号
	if(((u16_TempAx <= 300)||(u16_TempBx <= 300))
   	 &&(m_st_MeasureVltVaule.bMeasureFinish == 1)
	  )
	{
		u16_Temp = 9;
	}
		 
	//判断C口或A口是否为5V 来确定是否开始测量		 
	if(((u16_TempAx>= 788) && (u16_TempAx <= 871))
			&&((u16_TempBx>= 793) && (u16_TempBx <= 877))
			&&(m_st_MeasureVltVaule.bMeasureStart == 1)
		)
	{
		LED_PASS_A = 1;
		LED_FAULT_A = 0;
		LED_PASS_C = 1;
		LED_FAULT_C = 0;
		BEEP=1;
		u16_Temp = 10;
		m_st_MeasureVltVaule.bMeasureStart = 0;
	}
 }
	if(Timer10ms == 1)
		{
			TempPer10ms_1++;
			TempPer10ms++;
			TempPer10ms_2 ++;
			TempPer10ms_3++;
		}
			
		if(TempPer10ms>=2)
		{
			TempPer10ms = 0;
		}	
   
	switch(u16_Temp)
	{
		case 0: //A1口 5V空载 满载测量
		if(DelayClear==0)
			{
		   VoltageCondition=0;
			 LOAD_RELAY_OUT_Q20 = 0; //断开A2口
			 TempPer10ms_1=0;
			 TempPer10ms_2=0;
			 TempPer10ms_3=0;
       DelayClear=1;				
			}	
		if(TempPer10ms_1>=20)  //延时100ms
		{	
		  LED_PASS_A = 0;
			LED_FAULT_A = 0;
		  LED_PASS_C = 0;
			LED_FAULT_C =0;
      BEEP=0;	
			if(TempPer10ms_1>=UsbCar_18W_RestartTime)
			{
			if(DelayCondition==0)
			{
				m_st_MeasureVltVaule.u16_QVlt_5_N = Get_Adc_Average(0,5);//A口 5V空载采样
				DelayCondition=1;
				TempPer10ms_2=0;
			}	
			//满足5V±5% 后，吸合负载,进行带载测试 
			if(m_st_MeasureVltVaule.u16_QVlt_5_N >= 788
			&& m_st_MeasureVltVaule.u16_QVlt_5_N <= 871) 
			{
				m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_Q5_N = 1;
				LOAD_RELAY_OUT_A5 = 1;
				if(TempPer10ms_2>=UsbCar_18W_LOAD_RELAY_DELAYTIME)//延时1000ms
				{
				m_st_MeasureVltVaule.u16_QVlt_5_F = Get_Adc_Average(0,5);
				LOAD_RELAY_OUT_A5 = 0;
				
				if(m_st_MeasureVltVaule.u16_QVlt_5_F >= 579
			  && m_st_MeasureVltVaule.u16_QVlt_5_F <= 738) //±5%
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
				u16_Temp = 1;
			 }
			}
			else //报故障
			{				
				m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_Q5_N = 2;
				DelayCondition=0;
        DelayClear=0;				
				u16_Temp = 1;
			}
		}
   }			
			break;
		case 1://A1口 9V空载 满载测量
			if(DelayClear==0)
			{
			 TempPer10ms_1=0;
			 TempPer10ms_2=0;
			 TempPer10ms_3=0;
       DelayClear=1;
       VOLTAGE_RELAY_OUT_A = 1;				
			}
	    if(TempPer10ms_1>=UsbCar_18W_VOLTAGE_RELAY_DELAYTIME)
			{
			VOLTAGE_RELAY_OUT_A = 0;
		
			if(TempPer10ms_1>=(UsbCar_18W_VOLTAGE_RELAY_DELAYTIME+UsbCar_18W_AD_DELAYTIME))
		  {
			 if(DelayCondition==0)
			 {
				 m_st_MeasureVltVaule.u16_QVlt_9_N =Get_Adc_Average(0,5);
				 DelayCondition=1;
				 TempPer10ms_2=0;
				}
			
			//满足9V±5% 后，吸合负载,进行带载测试 
			if(m_st_MeasureVltVaule.u16_QVlt_9_N >= 1430
			&& m_st_MeasureVltVaule.u16_QVlt_9_N <= 1581) 
			{
				m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_Q9_N = 1;
				LOAD_RELAY_OUT_A9 = 1;
				if(TempPer10ms_2>=UsbCar_18W_LOAD_RELAY_DELAYTIME)
			  {
				m_st_MeasureVltVaule.u16_QVlt_9_F = Get_Adc_Average(0,5);
				//SEGGER_RTT_printf(0,"u16_CVlt_9_F = %d\n",m_st_MeasureVltVaule.u16_CVlt_9_F);
				LOAD_RELAY_OUT_A9 = 0;
				
				if(m_st_MeasureVltVaule.u16_QVlt_9_F >= 1300
				&& m_st_MeasureVltVaule.u16_QVlt_9_F <= 1537) //±5%
				{					
					m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_Q9_F = 1;
					
				}
				else//报故障
				{				
          m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_Q9_F = 2;
				}
				DelayCondition=0;
				DelayClear=0;
				
				u16_Temp = 2;
			 }
			}
				
			else //报故障
			{				
				m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_Q9_N = 2;
				DelayCondition=0;
        DelayClear=0;				
				u16_Temp = 2;
			}
		}
 }			
			break;	
	
	case 2://A1口 12V空载 满载测量
			if(DelayClear==0)
			{
			 TempPer10ms_1=0;
			 TempPer10ms_2=0;
       DelayClear=1;	
       VOLTAGE_RELAY_OUT_A = 1;				
			}
		  if(TempPer10ms_1>=UsbCar_18W_VOLTAGE_RELAY_DELAYTIME)
			{
			VOLTAGE_RELAY_OUT_A = 0;
		
			if(TempPer10ms_1>=(UsbCar_18W_VOLTAGE_RELAY_DELAYTIME+UsbCar_18W_AD_DELAYTIME))
		  {
			 if(DelayCondition==0)
			 {
				 m_st_MeasureVltVaule.u16_QVlt_12_N =Get_Adc_Average(0,5);
				 DelayCondition=1;
				 TempPer10ms_2=0;
				}
			
			//满足12V±5% 后，吸合负载,进行带载测试 
			if(m_st_MeasureVltVaule.u16_QVlt_12_N >= 1908
			&& m_st_MeasureVltVaule.u16_QVlt_12_N <= 2109) 
			{
				m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_Q12_N = 1;
				LOAD_RELAY_OUT_A12 = 1;
				if(TempPer10ms_2>=(UsbCar_18W_LOAD_RELAY_DELAYTIME))
			  {
				m_st_MeasureVltVaule.u16_QVlt_12_F = Get_Adc_Average(0,5);
				LOAD_RELAY_OUT_A12 = 0;
				
				if(m_st_MeasureVltVaule.u16_QVlt_12_F >= 1809
				&& m_st_MeasureVltVaule.u16_QVlt_12_F <= 2000) //±5%
				{					
					m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_Q12_F = 1;
					
				}
				else//报故障
				{					
          m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_Q12_F = 2;
				}
				DelayCondition=0;
				DelayClear=0;
				u16_Temp = 4;
			 }
			}
				
			else //报故障
			{				
				m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_Q12_N = 2;
				DelayCondition=0;
        DelayClear=0;				
				u16_Temp = 4;
			}
		}
 }			
			break;	
			
		case 4: //A2口 5V空载 满载测量
			if(DelayClear==0)
			{
			 TempPer10ms_1=0;
			 TempPer10ms_2=0;
			 VOLTAGE_RELAY_OUT_Q = 0;//断开A1口
       LOAD_RELAY_OUT_Q20 = 1;//接通A2口
       DelayClear=1;				
			}
		  if(TempPer10ms_1>=UsbCar_18W_RestartTime)
			{					
		  if(DelayCondition==0)
			{
			 m_st_MeasureVltVaule.u16_CVlt_5_N =Get_Adc_Average(14,5);
			 DelayCondition=1;
			 TempPer10ms_2=0;
			}
			//满足5V±5% 后，吸合负载,进行带载测试
      
			if(m_st_MeasureVltVaule.u16_CVlt_5_N >= 793
			&& m_st_MeasureVltVaule.u16_CVlt_5_N <= 877) 
			{
				m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C5_N = 1;
				LOAD_RELAY_OUT_C5 = 1;
				if(TempPer10ms_2>=(UsbCar_18W_LOAD_RELAY_DELAYTIME))
				{
				m_st_MeasureVltVaule.u16_CVlt_5_F = Get_Adc_Average(14,5);
				LOAD_RELAY_OUT_C5 = 0;
				
				if(m_st_MeasureVltVaule.u16_CVlt_5_F >= 579
				&& m_st_MeasureVltVaule.u16_CVlt_5_F <= 703) //±5%
				{

					m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C5_F = 1;
					
				}
				
				
				else//报故障
				{
					m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C5_F = 2;
				}
				DelayCondition=0;
				DelayClear=0;
				u16_Temp = 5;
			 }
			}
			else //报故障
			{				
				m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C5_N = 2;
				DelayCondition=0;
        DelayClear=0;				
				u16_Temp = 5;
			}
  }    
			break;
			
		case 5://A2口 9V空载 满载测量
			if(DelayClear==0)
			{
			 TempPer10ms_1=0;
			 TempPer10ms_2=0;
       DelayClear=1;	
       VOLTAGE_RELAY_OUT_C = 1;				
			}
		  if(TempPer10ms_1>=UsbCar_18W_VOLTAGE_RELAY_DELAYTIME)
			{
			VOLTAGE_RELAY_OUT_C = 0;
		
			if(TempPer10ms_1>=(UsbCar_18W_VOLTAGE_RELAY_DELAYTIME+UsbCar_18W_AD_DELAYTIME))
		  {
			 if(DelayCondition==0)
			 {
				 m_st_MeasureVltVaule.u16_CVlt_9_N =Get_Adc_Average(14,5);
				 DelayCondition=1;
				 TempPer10ms_2=0;
				}		
		
			//满足9V±5% 后，吸合负载,进行带载测试 
			if(m_st_MeasureVltVaule.u16_CVlt_9_N >= 1432
			&& m_st_MeasureVltVaule.u16_CVlt_9_N <= 1583) 
			{
				m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C9_N = 1;
				LOAD_RELAY_OUT_C9 = 1;
				if(TempPer10ms_2>=(UsbCar_18W_LOAD_RELAY_DELAYTIME))
			  {
				m_st_MeasureVltVaule.u16_CVlt_9_F = Get_Adc_Average(14,5);
				LOAD_RELAY_OUT_C9 = 0;
				
				if(m_st_MeasureVltVaule.u16_CVlt_9_F >= 1254
				&& m_st_MeasureVltVaule.u16_CVlt_9_F <= 1387) //±5%
				{					
					m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C9_F = 1;
					
				}
				else//报故障
				{					
          m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C9_F = 2;
				}
				DelayCondition=0;
				DelayClear=0;
				u16_Temp = 6;
			 }
			}
				
			else //报故障
			{
				m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C9_N = 2;
				DelayCondition=0;
        DelayClear=0;				
				u16_Temp = 6;
			}
		}
 }			
			break;
			
		case 6://A2口 12V空载 满载测量
			if(DelayClear==0)
			{
			 TempPer10ms_1=0;
			 TempPer10ms_2=0;			
       DelayClear=1;
       VOLTAGE_RELAY_OUT_C = 1;				
			}
			if(TempPer10ms_1>=UsbCar_18W_VOLTAGE_RELAY_DELAYTIME)
			{ 
				VOLTAGE_RELAY_OUT_C = 0;
			
		 if(TempPer10ms_1>=(UsbCar_18W_VOLTAGE_RELAY_DELAYTIME+UsbCar_18W_AD_DELAYTIME))
			{	
			if(DelayCondition==0)
				{				
				 m_st_MeasureVltVaule.u16_CVlt_12_N =Get_Adc_Average(14,5);
				 DelayCondition=1;
				 TempPer10ms_2=0;				
				}							
			//满足12V±5% 后，吸合负载,进行带载测试 
			if(m_st_MeasureVltVaule.u16_CVlt_12_N >= 1909
				&& m_st_MeasureVltVaule.u16_CVlt_12_N <= 2111) 
			  {
				m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C12_N = 1;
				LOAD_RELAY_OUT_C12 = 1;
				if(TempPer10ms_2>=(UsbCar_18W_LOAD_RELAY_DELAYTIME))
			  {				
				m_st_MeasureVltVaule.u16_CVlt_12_F = Get_Adc_Average(14,5);
				LOAD_RELAY_OUT_C12 = 0;
				
				if(m_st_MeasureVltVaule.u16_CVlt_12_F >= 1765
				&& m_st_MeasureVltVaule.u16_CVlt_12_F <= 1951) //±5%
				{					
					m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C12_F = 1;
					
				}
				else//报故障
				{
					m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C12_F = 2;
				}
				VOLTAGE_RELAY_OUT_Q = 1;//接通A1口 
				DelayCondition=0;
				DelayClear=0;
				u16_Temp = 8;
				
			 }
			}
			else //报故障
			{
				m_st_MeasureVltFault.un16_Alarm_1.Bits.bFault_C12_N = 2;
				VOLTAGE_RELAY_OUT_Q = 1;//接通A1口 
				DelayCondition=0;
        DelayClear=0;				
				u16_Temp = 8;
				
			}
    }
	//}
 }			
			break;

		case 8://A1、A2口同时带载时 5V空载 满载测量
			if(DelayClear==0)
			{
			 TempPer10ms_1=0;
			 TempPer10ms_2=0;
       DelayClear=1;				
			} 
      if(TempPer10ms_1>=(UsbCar_18W_RestartTime-500))
			{
		  if(DelayCondition==0)
			{
			 m_st_MeasureVltVaule.u16_CVlt_15_N =Get_Adc_Average(0,5);
			 m_st_MeasureVltVaule.u16_CVlt_20_N =Get_Adc_Average(14,5);
			 DelayCondition=1;
			 TempPer10ms_2=0;
			}
			//满足5V±5% 后，吸合负载,进行带载测试
      
			if((m_st_MeasureVltVaule.u16_CVlt_15_N  >= 792
			&&  m_st_MeasureVltVaule.u16_CVlt_15_N  <= 876)
      &&(m_st_MeasureVltVaule.u16_CVlt_20_N  >= 794
			&&  m_st_MeasureVltVaule.u16_CVlt_20_N  <= 878))			
			{
				m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C15_N = 1;
				m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C20_N = 1;
				LOAD_RELAY_OUT_A5 = 1;
				LOAD_RELAY_OUT_C5 = 1;
				if(TempPer10ms_2>=UsbCar_18W_LOAD_RELAY_DELAYTIME)
				{
				m_st_MeasureVltVaule.u16_CVlt_15_F = Get_Adc_Average(0,5);
				m_st_MeasureVltVaule.u16_CVlt_20_F = Get_Adc_Average(14,5);
				LOAD_RELAY_OUT_C5 = 0;
				LOAD_RELAY_OUT_A5 = 0;
				
				if((m_st_MeasureVltVaule.u16_CVlt_15_F >= 579
				&& m_st_MeasureVltVaule.u16_CVlt_15_F <= 738) //±5%
				&&(m_st_MeasureVltVaule.u16_CVlt_20_F >= 579
				&& m_st_MeasureVltVaule.u16_CVlt_20_F <= 738))
				{

					m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C15_F = 1;
					m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C20_F = 1;
				}
				
				
				else//报故障
				{
					m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C15_F = 2;
					m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C20_F = 2;
				}
				VoltageCondition=1;
				m_st_MeasureVltVaule.bMeasureFinish = 1;
				u16_Temp = 12;
			  
			 }
			}
		
			else //报故障
			{				
				m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C15_N = 2;
				m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C20_N = 2;
				VoltageCondition=1;
        m_st_MeasureVltVaule.bMeasureFinish = 1;
				u16_Temp = 12;
			}
		}
			break;
		case 9: //复位 清除故障 状态标志			
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
		
		case 10://清标志
			m_st_MeasureVltFault.un16_Alarm_1.All = 0x0;
			m_st_MeasureVltFault.un16_Alarm_2.All = 0x0;
			m_st_MeasureVltFault.un16_Alarm_3.All = 0x0;
		
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
		if(m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_Q5_N == 2
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
		|| m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_C20_F == 2)
		{
			BEEP = 1;
		if(m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_Q5_N == 2
		|| m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_Q5_F == 2
		|| m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_Q9_N == 2
		|| m_st_MeasureVltFault.un16_Alarm_2.Bits.bFault_Q9_F == 2
		|| m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_Q12_N == 2
		|| m_st_MeasureVltFault.un16_Alarm_3.Bits.bFault_Q12_F == 2)
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











