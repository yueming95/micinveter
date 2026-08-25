 /***********************************************************************
 *    FILENAME : SL_MPPT.c
 *
 *    PURPOSE  : 
 *    HISTORY  :
 *           DATE          VERSION      AUTHOR        NOTE
 ************************************************************************/
#include "SC_master.h"
/************************************************************************/
void MPPTTrack(void)
{
	float32 f32Temp0;	
	static int16 Counter = 0;
	static int16 test_dynamic_Flag = 0;
	static int16 CounterPVPos = 0;
	static int16 CounterPVNeg = 0;
	static float32 Delta_MPPTStep = 4;

	g_MpptCalc.f32DeltaPowerDC = g_MpptCalc.f32PVWattCurrent * 0.001f;
	if(g_MpptCalc.f32DeltaPowerDC < 3)
	{
		g_MpptCalc.f32DeltaPowerDC = 3;
	}
	else if(g_MpptCalc.f32DeltaPowerDC > 5)
	{
		g_MpptCalc.f32DeltaPowerDC = 5;
	}
	else
	{
	}
	if(g_MpptCalc.f32PVWattCurrent < 2000)
	{
		Delta_MPPTStep = DeltaMPPTV;
	}
	else if(g_MpptCalc.f32PVWattCurrent > 4000)
	{
		Delta_MPPTStep = 2;
	}
	else
	{
	}


	f32Temp0 = MAX(g_Mppt1Calc.f32PVWattCurrent, g_Mppt2Calc.f32PVWattCurrent);
		
	if((g_MpptCalc.f32PVWattCurrent > g_PActiveLimit.f32PinputAll)||(f32Temp0 > g_PActiveLimit.f32Pinput1))
	{
		g_MpptCalc.f32PvRefOldOld = g_MpptCalc.f32PvRefOld;
		g_MpptCalc.f32PvRefOld = g_MpptCalc.f32PvVoltRef;
		g_MpptCalc.f32PVWattOld = g_MpptCalc.f32PVWattCurrent;
		g_MpptCalc.f32PvVoltRef +=	1;

		return;
	}

	if(g_MpptCalc.f32PvVoltRef > g_CalcResult.Ave.f32VPV1 + PV15V)  
	{
		CounterPVPos++;
		if(CounterPVPos>2)
		{
			g_MpptCalc.f32PvVoltRef  = g_CalcResult.Ave.f32VPV1  - Delta_MPPTStep;
			CounterPVPos = 0;
		}
	}
	else if(g_MpptCalc.f32PvVoltRef < g_CalcResult.Ave.f32VPV1 - PV15V)  
	{
		CounterPVNeg++;
		if(CounterPVNeg>2)
		{
			g_MpptCalc.f32PvVoltRef  = g_CalcResult.Ave.f32VPV1  - Delta_MPPTStep;
			CounterPVNeg = 0;
		}
	}

	else
 	{
		CounterPVNeg = 0;
		CounterPVPos = 0;
		if(1 == g_MpptCalc.u16FastSearch)
		{
			if(g_MpptCalc.f32PvVoltRef  > (g_MpptCalc.f32PvOpenVolt * 0.83))//+7 is still ok?
			{
				g_MpptCalc.f32PvVoltRef  = g_MpptCalc.f32PvVoltRef - g_MpptCalc.f32PvOpenVolt * 0.01;				
			}
			else
			{
				g_MpptCalc.f32PvVoltRef  = g_MpptCalc.f32PvVoltRef  + Delta_MPPTStep;     
				g_MpptCalc.u16FastSearch = 0;
				test_dynamic_Flag = 0;
				Counter = 0;
			}
			g_MpptCalc.f32PvRefOldOld = g_MpptCalc.f32PvRefOld;
			g_MpptCalc.f32PvRefOld = g_MpptCalc.f32PvVoltRef;
			g_MpptCalc.f32PVWattOld = g_MpptCalc.f32PVWattCurrent;
		}
		else
		{
			if(g_MpptCalc.f32PVWattOld + g_MpptCalc.f32DeltaPowerDC <= g_MpptCalc.f32PVWattCurrent)             
			{
				Counter++;
				if(Counter > 3)
				{
					test_dynamic_Flag=1;
					Counter = 0;
				}
				if(test_dynamic_Flag==1)
				{
					if (g_MpptCalc.f32PvVoltRef >= g_MpptCalc.f32PvRefOld)
					{
						g_MpptCalc.f32PvRefOldOld = g_MpptCalc.f32PvRefOld;
						g_MpptCalc.f32PvRefOld = g_MpptCalc.f32PvVoltRef;
						g_MpptCalc.f32PvVoltRef = g_MpptCalc.f32PvRefOld - 0.1;
					}
					else if(g_MpptCalc.f32PvVoltRef < g_MpptCalc.f32PvRefOld)
					{
						g_MpptCalc.f32PvRefOldOld = g_MpptCalc.f32PvRefOld;
						g_MpptCalc.f32PvRefOld = g_MpptCalc.f32PvVoltRef;
						g_MpptCalc.f32PvVoltRef = g_MpptCalc.f32PvRefOld + 0.3;//0.2
					}
					else
					{
					}
				}
				else
				{	
					if(g_MpptCalc.f32PvVoltRef  >= g_MpptCalc.f32PvRefOld)
					{
						g_MpptCalc.f32PvRefOldOld = g_MpptCalc.f32PvRefOld;
						g_MpptCalc.f32PvRefOld = g_MpptCalc.f32PvVoltRef;
						g_MpptCalc.f32PvVoltRef += Delta_MPPTStep;
					}
					else
					{
						g_MpptCalc.f32PvRefOldOld = g_MpptCalc.f32PvRefOld;
						g_MpptCalc.f32PvRefOld = g_MpptCalc.f32PvVoltRef;
						g_MpptCalc.f32PvVoltRef  -= Delta_MPPTStep;
					}
				}
				g_MpptCalc.f32PVWattOld = g_MpptCalc.f32PVWattCurrent;
			}
			else if(g_MpptCalc.f32PVWattOld > g_MpptCalc.f32PVWattCurrent + g_MpptCalc.f32DeltaPowerDC)
			{
				test_dynamic_Flag = 0;
				Counter = 0;
				if(g_MpptCalc.f32PvVoltRef  >= g_MpptCalc.f32PvRefOld)
				{
					g_MpptCalc.f32PvRefOldOld = g_MpptCalc.f32PvRefOld;
					g_MpptCalc.f32PvRefOld = g_MpptCalc.f32PvVoltRef;
					g_MpptCalc.f32PvVoltRef  -= Delta_MPPTStep;
				}
				else
				{
					g_MpptCalc.f32PvRefOldOld = g_MpptCalc.f32PvRefOld;
					g_MpptCalc.f32PvRefOld = g_MpptCalc.f32PvVoltRef;
					g_MpptCalc.f32PvVoltRef  += Delta_MPPTStep;
				}
				g_MpptCalc.f32PVWattOld = g_MpptCalc.f32PVWattCurrent;
			}
			else
			{
				test_dynamic_Flag = 0;
				Counter = 0;
				if(g_MpptCalc.f32PvVoltRef  >= g_MpptCalc.f32PvRefOld)
				{
					g_MpptCalc.f32PvRefOldOld = g_MpptCalc.f32PvRefOld;
					g_MpptCalc.f32PvRefOld = g_MpptCalc.f32PvVoltRef ;
					g_MpptCalc.f32PvVoltRef  += (Delta_MPPTStep * 0.4);//1.5v
				}
				else
				{
					g_MpptCalc.f32PvRefOldOld = g_MpptCalc.f32PvRefOld;
					g_MpptCalc.f32PvRefOld = g_MpptCalc.f32PvVoltRef ;
					g_MpptCalc.f32PvVoltRef  -= (Delta_MPPTStep * 0.4);
				}               
			}
		}
	}


	if(g_MpptCalc.f32PvVoltRef  < PV250V + 5)  
	{
		g_MpptCalc.f32PvVoltRef  = PV250V + 5;
	}
}
/************************************************************************/
void MPPT1Track(void)
{
	static int16 Counter1= 0;
	static int16 test_dynamic_Flag1 = 0;
	static float32 Delta_MPPT1Step = 4;

	g_Mppt1Calc.f32DeltaPowerDC = g_Mppt1Calc.f32PVWattCurrent * 0.001f;
	if(g_Mppt1Calc.f32DeltaPowerDC < 3)
	{
		g_Mppt1Calc.f32DeltaPowerDC = 3;
	}
	else if(g_Mppt1Calc.f32DeltaPowerDC > 5)
	{
		g_Mppt1Calc.f32DeltaPowerDC = 5;
	}
	else
	{
	}

	if(g_Mppt1Calc.f32PVWattCurrent < 1500)
	{
		Delta_MPPT1Step = DeltaMPPTV;
	}
	else if(g_Mppt1Calc.f32PVWattCurrent > 3000)
	{
		Delta_MPPT1Step = 2;
	}
	else
	{
	}


	if(g_Mppt1Calc.f32PVWattCurrent > g_PActiveLimit.f32Pinput1)
	{
		g_Mppt1Calc.f32PvRefOldOld = g_Mppt1Calc.f32PvRefOld;
		g_Mppt1Calc.f32PvRefOld = g_Mppt1Calc.f32PvVoltRef ;
		g_Mppt1Calc.f32PVWattOld = g_Mppt1Calc.f32PVWattCurrent;

		g_Mppt1Calc.f32PvVoltRef +=	1;

		return;
	}

	if(g_Mppt1Calc.f32PvVoltRef > g_CalcResult.Ave.f32VPV1 + PV15V)  
	{
		g_Mppt1Calc.f32PvVoltRef  = g_CalcResult.Ave.f32VPV1 - Delta_MPPT1Step;
	}
	else if(g_Mppt1Calc.f32PvVoltRef < g_CalcResult.Ave.f32VPV1 - PV15V)  
	{
		g_Mppt1Calc.f32PvVoltRef  = g_CalcResult.Ave.f32VPV1 - Delta_MPPT1Step;
	}
	else
 	{
		if(1 == g_Mppt1Calc.u16FastSearch)
		{
			if(g_Mppt1Calc.f32PvVoltRef  > (g_Mppt1Calc.f32PvOpenVolt * 0.83))//+7 is still ok?
			{
				g_Mppt1Calc.f32PvVoltRef  = g_Mppt1Calc.f32PvVoltRef - g_Mppt1Calc.f32PvOpenVolt * 0.01;				
			}
			else
			{
				g_Mppt1Calc.f32PvVoltRef  = g_Mppt1Calc.f32PvVoltRef  + Delta_MPPT1Step;     
				g_Mppt1Calc.u16FastSearch = 0;
				test_dynamic_Flag1 = 0;
				Counter1 = 0;
			}

			g_Mppt1Calc.f32PvRefOldOld = g_Mppt1Calc.f32PvRefOld;
			g_Mppt1Calc.f32PvRefOld = g_Mppt1Calc.f32PvVoltRef ;
			g_Mppt1Calc.f32PVWattOld = g_Mppt1Calc.f32PVWattCurrent;
		}
		else
		{
			if(g_Mppt1Calc.f32PVWattOld + g_Mppt1Calc.f32DeltaPowerDC <= g_Mppt1Calc.f32PVWattCurrent)             
			{
				Counter1++;
				if(Counter1 > 3)
				{
					test_dynamic_Flag1 = 1;
					Counter1 = 0;
				}
				if(test_dynamic_Flag1 == 1)
				{
					if (g_Mppt1Calc.f32PvVoltRef >= g_Mppt1Calc.f32PvRefOld)
					{
						g_Mppt1Calc.f32PvRefOldOld = g_Mppt1Calc.f32PvRefOld;
						g_Mppt1Calc.f32PvRefOld = g_Mppt1Calc.f32PvVoltRef ;
						g_Mppt1Calc.f32PvVoltRef = g_Mppt1Calc.f32PvRefOld - 0.1;
					}
					else if(g_Mppt1Calc.f32PvVoltRef < g_Mppt1Calc.f32PvRefOld)
					{
						g_Mppt1Calc.f32PvRefOldOld = g_Mppt1Calc.f32PvRefOld;
						g_Mppt1Calc.f32PvRefOld = g_Mppt1Calc.f32PvVoltRef ;
						g_Mppt1Calc.f32PvVoltRef = g_Mppt1Calc.f32PvRefOld + 0.3;//0.2
					}
					else
					{
					}
				}
				else
				{	
					if(g_Mppt1Calc.f32PvVoltRef  >= g_Mppt1Calc.f32PvRefOld)
					{
						g_Mppt1Calc.f32PvRefOldOld = g_Mppt1Calc.f32PvRefOld;
						g_Mppt1Calc.f32PvRefOld = g_Mppt1Calc.f32PvVoltRef ;
						g_Mppt1Calc.f32PvVoltRef += Delta_MPPT1Step;
					}
					else
					{
						g_Mppt1Calc.f32PvRefOldOld = g_Mppt1Calc.f32PvRefOld;
						g_Mppt1Calc.f32PvRefOld = g_Mppt1Calc.f32PvVoltRef ;
						g_Mppt1Calc.f32PvVoltRef  -= Delta_MPPT1Step;
					}
				}
				g_Mppt1Calc.f32PVWattOld = g_Mppt1Calc.f32PVWattCurrent;
			}
			else if(g_Mppt1Calc.f32PVWattOld > g_Mppt1Calc.f32PVWattCurrent + g_Mppt1Calc.f32DeltaPowerDC)
			{
				test_dynamic_Flag1 = 0;
				Counter1 = 0;
				if(g_Mppt1Calc.f32PvVoltRef  >= g_Mppt1Calc.f32PvRefOld)
				{
					g_Mppt1Calc.f32PvRefOldOld = g_Mppt1Calc.f32PvRefOld;
					g_Mppt1Calc.f32PvRefOld = g_Mppt1Calc.f32PvVoltRef ;
					g_Mppt1Calc.f32PvVoltRef  -= Delta_MPPT1Step;
				}
				else
				{
					g_Mppt1Calc.f32PvRefOldOld = g_Mppt1Calc.f32PvRefOld;
					g_Mppt1Calc.f32PvRefOld = g_Mppt1Calc.f32PvVoltRef ;
					g_Mppt1Calc.f32PvVoltRef  += Delta_MPPT1Step;
				}
				g_Mppt1Calc.f32PVWattOld = g_Mppt1Calc.f32PVWattCurrent;
			}
			else
			{
				test_dynamic_Flag1 = 0;
				Counter1 = 0;
				if(g_Mppt1Calc.f32PvVoltRef  >= g_Mppt1Calc.f32PvRefOld)
				{
					g_Mppt1Calc.f32PvRefOldOld = g_Mppt1Calc.f32PvRefOld;
					g_Mppt1Calc.f32PvRefOld = g_Mppt1Calc.f32PvVoltRef;
					g_Mppt1Calc.f32PvVoltRef  += (Delta_MPPT1Step * 0.4);//1.5v
				}
				else
				{
					g_Mppt1Calc.f32PvRefOldOld = g_Mppt1Calc.f32PvRefOld;
					g_Mppt1Calc.f32PvRefOld = g_Mppt1Calc.f32PvVoltRef ;
					g_Mppt1Calc.f32PvVoltRef  -= (Delta_MPPT1Step * 0.4);
				}               
			}
		}
	}

	if(g_Mppt1Calc.f32PvVoltRef  < PV250V + 5)  
	{
		g_Mppt1Calc.f32PvVoltRef  = PV250V + 5;
	}
}

/************************************************************************/
void MPPT2Track(void)
{
	static int16 Counter2= 0;
	static int16 test_dynamic_Flag2 = 0;
	static float32 Delta_MPPT2Step = 4;

	g_Mppt2Calc.f32DeltaPowerDC = g_Mppt2Calc.f32PVWattCurrent * 0.001f;
	if(g_Mppt2Calc.f32DeltaPowerDC < 3)
	{
		g_Mppt2Calc.f32DeltaPowerDC = 3;
	}
	else if(g_Mppt2Calc.f32DeltaPowerDC > 5)
	{
		g_Mppt2Calc.f32DeltaPowerDC = 5;
	}
	else
	{
	}

	if(g_Mppt2Calc.f32PVWattCurrent < 1500)
	{
		Delta_MPPT2Step = DeltaMPPTV;
	}
	else if(g_Mppt2Calc.f32PVWattCurrent > 3000)
	{
		Delta_MPPT2Step = 2;
	}
	else
	{
	}

	if(g_Mppt2Calc.f32PVWattCurrent > g_PActiveLimit.f32Pinput2)
	{
		g_Mppt2Calc.f32PvRefOldOld = g_Mppt2Calc.f32PvRefOld;
		g_Mppt2Calc.f32PvRefOld = g_Mppt2Calc.f32PvVoltRef ;
		g_Mppt2Calc.f32PVWattOld = g_Mppt2Calc.f32PVWattCurrent;
		g_Mppt2Calc.f32PvVoltRef +=	1;
		return;
	}

	if(g_Mppt2Calc.f32PvVoltRef > g_CalcResult.Ave.f32VPV2  + PV15V)  
	{
		g_Mppt2Calc.f32PvVoltRef  = g_CalcResult.Ave.f32VPV2  - Delta_MPPT2Step;
	}
	else if(g_Mppt2Calc.f32PvVoltRef < g_CalcResult.Ave.f32VPV2 - PV15V)  
	{
		g_Mppt2Calc.f32PvVoltRef  = g_CalcResult.Ave.f32VPV2  - Delta_MPPT2Step;
	}
	else
 	{
		if(1 == g_Mppt2Calc.u16FastSearch)
		{
			if(g_Mppt2Calc.f32PvVoltRef  > (g_Mppt2Calc.f32PvOpenVolt * 0.83))//+7 is still ok?
			{
				g_Mppt2Calc.f32PvVoltRef  = g_Mppt2Calc.f32PvVoltRef - g_Mppt2Calc.f32PvOpenVolt * 0.01;				
			}
			else
			{
				g_Mppt2Calc.f32PvVoltRef  = g_Mppt2Calc.f32PvVoltRef  + Delta_MPPT2Step;     
				g_Mppt2Calc.u16FastSearch = 0;
				test_dynamic_Flag2 = 0;
				Counter2 = 0;
			}
			g_Mppt2Calc.f32PvRefOldOld = g_Mppt2Calc.f32PvRefOld;
			g_Mppt2Calc.f32PvRefOld = g_Mppt2Calc.f32PvVoltRef ;
			g_Mppt2Calc.f32PVWattOld = g_Mppt2Calc.f32PVWattCurrent;
		}
		else
		{
			if(g_Mppt2Calc.f32PVWattOld + g_Mppt2Calc.f32DeltaPowerDC <= g_Mppt2Calc.f32PVWattCurrent)             
			{
				Counter2++;
				if(Counter2 > 3)
				{
					test_dynamic_Flag2 = 1;
					Counter2 = 0;
				}
				if(test_dynamic_Flag2 == 1)
				{
					if (g_Mppt2Calc.f32PvVoltRef >= g_Mppt2Calc.f32PvRefOld)
					{
						g_Mppt2Calc.f32PvRefOldOld = g_Mppt2Calc.f32PvRefOld;
						g_Mppt2Calc.f32PvRefOld = g_Mppt2Calc.f32PvVoltRef;
						g_Mppt2Calc.f32PvVoltRef = g_Mppt2Calc.f32PvRefOld - 0.1;
					}
					else if(g_Mppt2Calc.f32PvVoltRef < g_Mppt2Calc.f32PvRefOld)
					{
						g_Mppt2Calc.f32PvRefOldOld = g_Mppt2Calc.f32PvRefOld;
						g_Mppt2Calc.f32PvRefOld = g_Mppt2Calc.f32PvVoltRef;
						g_Mppt2Calc.f32PvVoltRef = g_Mppt2Calc.f32PvRefOld + 0.3;//0.2
					}
					else
					{
					}
				}
				else
				{	
					if(g_Mppt2Calc.f32PvVoltRef  >= g_Mppt2Calc.f32PvRefOld)
					{
						g_Mppt2Calc.f32PvRefOldOld = g_Mppt2Calc.f32PvRefOld;
						g_Mppt2Calc.f32PvRefOld = g_Mppt2Calc.f32PvVoltRef;
						g_Mppt2Calc.f32PvVoltRef += Delta_MPPT2Step;
					}
					else
					{
						g_Mppt2Calc.f32PvRefOldOld = g_Mppt2Calc.f32PvRefOld;
						g_Mppt2Calc.f32PvRefOld = g_Mppt2Calc.f32PvVoltRef;
						g_Mppt2Calc.f32PvVoltRef  -= Delta_MPPT2Step;
					}
				}
				g_Mppt2Calc.f32PVWattOld = g_Mppt2Calc.f32PVWattCurrent;
			}
			else if(g_Mppt2Calc.f32PVWattOld > g_Mppt2Calc.f32PVWattCurrent + g_Mppt2Calc.f32DeltaPowerDC)
			{
				test_dynamic_Flag2 = 0;
				Counter2 = 0;
				if(g_Mppt2Calc.f32PvVoltRef  >= g_Mppt2Calc.f32PvRefOld)
				{
					g_Mppt2Calc.f32PvRefOldOld = g_Mppt2Calc.f32PvRefOld;
					g_Mppt2Calc.f32PvRefOld = g_Mppt2Calc.f32PvVoltRef;
					g_Mppt2Calc.f32PvVoltRef  -= Delta_MPPT2Step;
				}
				else
				{
					g_Mppt2Calc.f32PvRefOldOld = g_Mppt2Calc.f32PvRefOld;
					g_Mppt2Calc.f32PvRefOld = g_Mppt2Calc.f32PvVoltRef;
					g_Mppt2Calc.f32PvVoltRef  += Delta_MPPT2Step;
				}
				g_Mppt2Calc.f32PVWattOld = g_Mppt2Calc.f32PVWattCurrent;
			}
			else
			{
				test_dynamic_Flag2 = 0;
				Counter2 = 0;
				if(g_Mppt2Calc.f32PvVoltRef  >= g_Mppt2Calc.f32PvRefOld)
				{
					g_Mppt2Calc.f32PvRefOldOld = g_Mppt2Calc.f32PvRefOld;
					g_Mppt2Calc.f32PvRefOld = g_Mppt2Calc.f32PvVoltRef;
					g_Mppt2Calc.f32PvVoltRef += (Delta_MPPT2Step * 0.4);//1.5v
				}
				else
				{
					g_Mppt2Calc.f32PvRefOldOld = g_Mppt2Calc.f32PvRefOld;
					g_Mppt2Calc.f32PvRefOld = g_Mppt2Calc.f32PvVoltRef;
					g_Mppt2Calc.f32PvVoltRef -= (Delta_MPPT2Step * 0.4);
				}               
			}
		}
	}

	if(g_Mppt2Calc.f32PvVoltRef  < PV250V + 5)  
	{
		g_Mppt2Calc.f32PvVoltRef  = PV250V + 5;
	}
}

/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void MpptInitial(void)
{
	if(1 == g_MPPTState.bit.InitFlag)
	{
		g_MPPTState.bit.InitFlag = 0;
		g_MpptCalc.u16FastSearch = 1;
		g_MpptCalc.f32PvOpenVolt = g_CalcResult.Ave.f32VPV1;
		//g_MpptCalc.f32MPPTLimit = g_Rated.f32OutputPower;
		g_MpptCalc.f32PvVoltRef = g_MpptCalc.f32PvOpenVolt * 0.99;		
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void Mppt1Initial(void)
{
	if(1 == g_MPPTState.bit.InitFlag1)
	{
		g_MPPTState.bit.InitFlag1 = 0;
		g_Mppt1Calc.u16FastSearch = 1;
		g_Mppt1Calc.f32PvOpenVolt = g_CalcResult.Ave.f32VPV1;
//		g_Mppt1Calc.f32MPPT1Limit = g_Rated.f32HalfInputPower;
		g_Mppt1Calc.f32PvVoltRef = g_Mppt1Calc.f32PvOpenVolt * 0.99;
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void Mppt2Initial(void)
{
	if(1 == g_MPPTState.bit.InitFlag2)
	{
		g_MPPTState.bit.InitFlag2 = 0;
		g_Mppt2Calc.u16FastSearch = 1;
		g_Mppt2Calc.f32PvOpenVolt = g_CalcResult.Ave.f32VPV2;
		//g_Mppt2Calc.f32MPPT2Limit = g_Rated.f32HalfInputPower;
		g_Mppt2Calc.f32PvVoltRef = g_Mppt2Calc.f32PvOpenVolt * 0.99;
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void PowerShiftPVref(void)
{

	g_MpptCalc.f32PowerErrOld = g_MpptCalc.f32PowerErrNew;
	g_MpptCalc.f32PowerErrNew = g_CalcResult.Power.f32OutputWatt - g_PActiveLimit.f32PActiveMaxLimit;

	if(g_MpptCalc.f32PowerErrNew > 500)
	{
		g_MpptCalc.f32PowerErrNew = 500;
	}
	g_MpptCalc.f32PowerShiftVolt = g_MpptCalc.f32PowerShiftVolt + g_MpptCalc.f32PowerErrNew * g_MpptCalc.f32PowerK_alpha - g_MpptCalc.f32PowerErrOld * g_MpptCalc.f32PowerK_beta;
					
	if(g_MpptCalc.f32PowerShiftVolt > 500)    //180V
	{
		g_MpptCalc.f32PowerShiftVolt = 500;
	}

	if(g_MpptCalc.f32PowerShiftVolt < -10)    //10v
	{
		g_MpptCalc.f32PowerShiftVolt = -10;
		g_MpptCalc.u16OutputPowerLimitFlag = 0;
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void OverPowerJudge(void)
{
	if(1 == g_MPPTState.bit.FactoryMode)
	{
		g_MpptCalc.u16OutputPowerLimitFlag= 0;
	}
	else
	{
		if(g_CalcResult.Power.f32OutputWatt > g_PActiveLimit.f32PActiveMaxLimit + 100)//300
		{
			if(0 == g_MpptCalc.u16OutputPowerLimitFlag)
			{
				if(PARALLEL_IN == g_MPPTState.bit.InputMode)
				{	
					g_MpptCalc.f32PVVoltLimitPoint = g_CalcResult.Ave.f32VPV1;
					g_MpptCalc.f32PowerShiftVolt = 0;
					g_MpptCalc.f32PowerErrNew = 0;
				}
				else
				{
					if(g_MPPTState.bit.PV1ON)
					{
						g_Mppt1Calc.f32PVVoltLimitPoint = g_CalcResult.Ave.f32VPV1;
						g_MpptCalc.f32PowerShiftVolt = 0;
						g_MpptCalc.f32PowerErrNew = 0;
					}
					if(g_MPPTState.bit.PV2ON)
					{
						g_Mppt2Calc.f32PVVoltLimitPoint = g_CalcResult.Ave.f32VPV2;
						g_MpptCalc.f32PowerShiftVolt = 0;
						g_MpptCalc.f32PowerErrNew = 0;
					}
				}
				g_MpptCalc.u16OutputPowerLimitFlag = 1;
			}				
		}
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void OutputPowerLimitMPPT(void)
{
	float32 f32TempPowerMax;

	OverPowerJudge();
	if(1 == g_MpptCalc.u16OutputPowerLimitFlag)
	{
		PowerShiftPVref();

		if (PARALLEL_IN == g_MPPTState.bit.InputMode)
		{
			if(g_CalcResult.Power.f32Input2Watt > g_CalcResult.Power.f32Input1Watt)
			{
				f32TempPowerMax = g_CalcResult.Power.f32Input2Watt;
			}
			else
			{
				f32TempPowerMax = g_CalcResult.Power.f32Input1Watt;
			}

			if(f32TempPowerMax > g_PActiveLimit.f32Pinput1)
			{
				g_MpptCalc.f32PvVoltRef  =  g_MpptCalc.f32PvVoltRef  + DeltaMPPTV * 0.05;				
				g_MpptCalc.f32PVVoltLimitPoint = g_MpptCalc.f32PvVoltRef - g_MpptCalc.f32PowerShiftVolt;
			}
			else
			{
				g_MpptCalc.f32PvVoltRef = g_MpptCalc.f32PVVoltLimitPoint + g_MpptCalc.f32PowerShiftVolt;
			}

			g_MpptCalc.f32PvRefOld = g_MpptCalc.f32PvVoltRef;
		}
		else //if(INDEPENDENT_IN == g_MPPTState.bit.InputMode)
		{
			if(1 == g_MPPTState.bit.PV1ON)
			{
				if(g_CalcResult.Power.f32Input1Watt > g_PActiveLimit.f32Pinput1)
				{
					g_Mppt1Calc.f32PvVoltRef  =  g_Mppt1Calc.f32PvVoltRef  + DeltaMPPTV * 0.05;				
					g_Mppt1Calc.f32PVVoltLimitPoint = g_Mppt1Calc.f32PvVoltRef - g_MpptCalc.f32PowerShiftVolt;
				}
				else
				{
					g_Mppt1Calc.f32PvVoltRef = g_Mppt1Calc.f32PVVoltLimitPoint + g_MpptCalc.f32PowerShiftVolt;
				}
				g_Mppt1Calc.f32PvRefOld = g_Mppt1Calc.f32PvVoltRef;
			}
			if(1 == g_MPPTState.bit.PV2ON)
			{
				if(g_CalcResult.Power.f32Input2Watt > g_PActiveLimit.f32Pinput2)
				{
					g_Mppt2Calc.f32PvVoltRef  =  g_Mppt2Calc.f32PvVoltRef  + DeltaMPPTV * 0.05;				
					g_Mppt2Calc.f32PVVoltLimitPoint = g_Mppt2Calc.f32PvVoltRef - g_MpptCalc.f32PowerShiftVolt;
				}
				else
				{
					g_Mppt2Calc.f32PvVoltRef = g_Mppt2Calc.f32PVVoltLimitPoint + g_MpptCalc.f32PowerShiftVolt;
				}
				g_Mppt2Calc.f32PvRefOld = g_Mppt2Calc.f32PvVoltRef;
			}
		}
	}													
}

//===========================================================================
// No more.
//===========================================================================
