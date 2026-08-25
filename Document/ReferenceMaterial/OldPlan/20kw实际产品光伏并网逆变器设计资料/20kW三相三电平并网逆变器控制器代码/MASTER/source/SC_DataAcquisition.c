/***********************************************************************

 *    FILENAME : SC_DataAcquisition.c
 *
 *    PURPOSE  : 
 *    HISTORY  :
 *           DATE          VERSION      AUTHOR        NOTE
 ************************************************************************/
#include "SC_master.h"

#pragma CODE_SECTION(Adc_Getvalue1, "ControlLoopInRAM");
#pragma CODE_SECTION(Adc_Getvalue2, "ControlLoopInRAM");
#pragma CODE_SECTION(Adc_Calc, "ControlLoopInRAM");
/**********************************************************************
* FUNCION :  Adc_Getvalue1
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void Adc_Getvalue1(void)
{
	static float32 f32DCIInjectionR = 0; 

	if((NormalState == g_SysCurrentState)&&(1 == g_SaftyConfig.bit.DCITestEnable))
	{
		if(f32DCIInjectionR > g_SafetyDCI.f32DciTestInjection)
		{
			f32DCIInjectionR = g_SafetyDCI.f32DciTestInjection - 0.01;
		}
		else if(f32DCIInjectionR < g_SafetyDCI.f32DciTestInjection)
		{
			f32DCIInjectionR = g_SafetyDCI.f32DciTestInjection + 0.01;
		}
		else
		{

		}
	}
	else
	{
		f32DCIInjectionR = 0;
	}

	g_GeneralADbuffer.f32IGridS = (float32)ADC_RESULT_IGRID_S - AD_DEFAULT_AC_OFFSET;
	g_GeneralADbuffer.f32IGridT = (float32)ADC_RESULT_IGRID_T - AD_DEFAULT_AC_OFFSET;
	g_GeneralADbuffer.f32IGridR = (float32)ADC_RESULT_IGRID_R - AD_DEFAULT_AC_OFFSET;

	g_GeneralADbuffer.f32VGridR = (float32)ADC_RESULT_VGRID_R - AD_DEFAULT_AC_OFFSET;
	g_GeneralADbuffer.f32VGridS = (float32)ADC_RESULT_VGRID_S - AD_DEFAULT_AC_OFFSET;				                      	                             
	g_GeneralADbuffer.f32VGridT = (float32)ADC_RESULT_VGRID_T - AD_DEFAULT_AC_OFFSET;   	

	g_Instantaneous.f32IGridR = g_GeneralADbuffer.f32IGridR * g_ADConvert.gain.f32IGridR - g_ADConvert.offset.f32IGridR + f32DCIInjectionR;
	g_Instantaneous.f32IGridS = g_GeneralADbuffer.f32IGridS * g_ADConvert.gain.f32IGridS - g_ADConvert.offset.f32IGridS;
	g_Instantaneous.f32IGridT = g_GeneralADbuffer.f32IGridT * g_ADConvert.gain.f32IGridT - g_ADConvert.offset.f32IGridT;	
	
	g_Instantaneous.f32VGridR = g_GeneralADbuffer.f32VGridR * g_ADConvert.gain.f32VGridR - g_ADConvert.offset.f32VGridR;//300 * PLLConReg.Sin_AITheta;//
	g_Instantaneous.f32VGridS = g_GeneralADbuffer.f32VGridS * g_ADConvert.gain.f32VGridS - g_ADConvert.offset.f32VGridS;//311 * PLLConReg.Sin_Theta_120;//	
	g_Instantaneous.f32VGridT = g_GeneralADbuffer.f32VGridT * g_ADConvert.gain.f32VGridT - g_ADConvert.offset.f32VGridT;//311 * PLLConReg.Sin_Theta120;//
}

/**********************************************************************
* FUNCION :  Adc_Getvalue2
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void Adc_Getvalue2(void)
{
	static Uint16 i16ADSelectCounter =0; 

	g_GeneralADbuffer.f32VPV1 = (float32)ADC_RESULT_VPV1;
	g_GeneralADbuffer.f32VPV2 = (float32)ADC_RESULT_VPV2;			
	g_GeneralADbuffer.f32IPV1 = (float32)ADC_RESULT_IPV1 - 68.5;		 
	g_GeneralADbuffer.f32IPV2 = (float32)ADC_RESULT_IPV2 - 68.5;
	
	g_GeneralADbuffer.f32VbusP = (float32)ADC_RESULT_VBUSP;
	g_GeneralADbuffer.f32VbusN = (float32)ADC_RESULT_VBUSN;
		
	g_GeneralADbuffer.f32GFCI = (float32)ADC_RESULT_GFCI - AD_DEFAULT_AC_OFFSET;
		
	g_Instantaneous.f32VPV1 = g_GeneralADbuffer.f32VPV1 * g_ADConvert.gain.f32VPV1 - g_ADConvert.offset.f32VPV1;
	g_Instantaneous.f32VPV2 = g_GeneralADbuffer.f32VPV2 * g_ADConvert.gain.f32VPV2 - g_ADConvert.offset.f32VPV2;
	g_Instantaneous.f32IPV1 = g_GeneralADbuffer.f32IPV1 * g_ADConvert.gain.f32IPV1 - g_ADConvert.offset.f32IPV1;
	g_Instantaneous.f32IPV2 = g_GeneralADbuffer.f32IPV2 * g_ADConvert.gain.f32IPV2 - g_ADConvert.offset.f32IPV2;


	g_Instantaneous.f32GFCI = g_GeneralADbuffer.f32GFCI * g_ADConvert.gain.f32GFCI - g_ADConvert.offset.f32GFCI;    

	g_Instantaneous.f32VbusP = g_GeneralADbuffer.f32VbusP * g_ADConvert.gain.f32VbusP - g_ADConvert.offset.f32VbusP;//350;//
	g_Instantaneous.f32VbusN = g_GeneralADbuffer.f32VbusN * g_ADConvert.gain.f32VbusN - g_ADConvert.offset.f32VbusN;//350;//

//	BusOverCheck_Instant(g_Instantaneous.f32VbusP, g_Instantaneous.f32VbusN);	// 
	
	g_Instantaneous.f32VbusPN = g_Instantaneous.f32VbusP + g_Instantaneous.f32VbusN;

	switch(i16ADSelectCounter)
	{
		case 0:	// 0----> DCI-R
			g_GeneralADbuffer.f32Mux = (float32)ADC_RESULT_IDCI;
			g_Instantaneous.f32DciR = (g_GeneralADbuffer.f32Mux - AD_DEFAULT_AC_OFFSET) * g_ADConvert.gain.f32DciR - g_ADConvert.offset.f32DciR;
			i16ADSelectCounter = 1;
			M_SELECT_DCI_S;
		break;
		case 1:	// 1----> DCI-S
			g_GeneralADbuffer.f32Mux = (float32)ADC_RESULT_IDCI;
			g_Instantaneous.f32DciS = (g_GeneralADbuffer.f32Mux - AD_DEFAULT_AC_OFFSET) * g_ADConvert.gain.f32DciS - g_ADConvert.offset.f32DciS;
			i16ADSelectCounter = 2;
			M_SELECT_DCI_T;
		break;
		case 2:	// 2----> DCI-T
			g_GeneralADbuffer.f32Mux = (float32)ADC_RESULT_IDCI;
			g_Instantaneous.f32DciT = (g_GeneralADbuffer.f32Mux - AD_DEFAULT_AC_OFFSET) * g_ADConvert.gain.f32DciT - g_ADConvert.offset.f32DciT;
			i16ADSelectCounter = 0;
			M_SELECT_DCI_R;
		break;
		default:
			i16ADSelectCounter = 0;
			M_SELECT_DCI_R;
		break;	
	}
}

/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void VGrid_rms_Calc(float32 f32CntReci)
{
	g_CalcResult.Rms.f32VGridR = sqrt(g_ADSum.Rms.f32VGridR * f32CntReci);
	g_CalcResult.Rms.f32VGridS = sqrt(g_ADSum.Rms.f32VGridS * f32CntReci);
	g_CalcResult.Rms.f32VGridT = sqrt(g_ADSum.Rms.f32VGridT * f32CntReci);
}

/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void VGridLine_rms_Calc(float32 f32CntReci)	
{
	g_CalcResult.Rms.f32VGridRS = sqrt(g_ADSum.Rms.f32VGridRS * f32CntReci);
	g_CalcResult.Rms.f32VGridST = sqrt(g_ADSum.Rms.f32VGridST * f32CntReci);
	g_CalcResult.Rms.f32VGridTR = sqrt(g_ADSum.Rms.f32VGridTR * f32CntReci);
}
/**********************************************************************
* FUNCTION : Executed every 3S
* PURPOSE  :  
* CALLED BY: 
**********************************************************************/
void VGrid10minMovingAve_Calc(void)
{
	static float32 s_f32SumR = 0;
	static float32 s_f32SumS = 0;
	static float32 s_f32SumT = 0;
	float32 f32TempMov = 0;
	static Uint16 s_u16First200Cnt = 0;
	static Uint16 s_u16MovCnt = 0;

	if(s_u16First200Cnt < 200)
	{
		g_VGridMovAve.f32BufR[s_u16First200Cnt] = g_CalcResult.Rms.f32VGridR;
		s_f32SumR += g_CalcResult.Rms.f32VGridR;
		g_VGridMovAve.f32BufS[s_u16First200Cnt] = g_CalcResult.Rms.f32VGridS;
		s_f32SumS += g_CalcResult.Rms.f32VGridS;
		g_VGridMovAve.f32BufT[s_u16First200Cnt] = g_CalcResult.Rms.f32VGridT;
		s_f32SumT += g_CalcResult.Rms.f32VGridT;

		s_u16First200Cnt++;
		
		f32TempMov = s_f32SumR / s_u16First200Cnt;
		g_VGridMovAve.f32R= f32TempMov;
		f32TempMov = s_f32SumS / s_u16First200Cnt;
		g_VGridMovAve.f32S= f32TempMov;
		f32TempMov = s_f32SumT / s_u16First200Cnt;
		g_VGridMovAve.f32T= f32TempMov;
	}
	else
	{
		s_f32SumR -= g_VGridMovAve.f32BufR[s_u16MovCnt];
		g_VGridMovAve.f32BufR[s_u16MovCnt] = g_CalcResult.Rms.f32VGridR;
		s_f32SumR += g_CalcResult.Rms.f32VGridR;
		
		s_f32SumS -= g_VGridMovAve.f32BufS[s_u16MovCnt];
		g_VGridMovAve.f32BufS[s_u16MovCnt] = g_CalcResult.Rms.f32VGridS;
		s_f32SumS += g_CalcResult.Rms.f32VGridS;
		
		s_f32SumT -= g_VGridMovAve.f32BufT[s_u16MovCnt];
		g_VGridMovAve.f32BufT[s_u16MovCnt] = g_CalcResult.Rms.f32VGridT;
		s_f32SumT += g_CalcResult.Rms.f32VGridT;
		
		if(s_u16MovCnt < 199)
			s_u16MovCnt++;
		else
			s_u16MovCnt = 0;
		
		f32TempMov = s_f32SumR * 0.005;
		g_VGridMovAve.f32R = f32TempMov;
		f32TempMov = s_f32SumS * 0.005;
		g_VGridMovAve.f32S = f32TempMov;
		f32TempMov = s_f32SumT * 0.005;
		g_VGridMovAve.f32T = f32TempMov;
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void IGrid_rms_Calc(float32 f32CntReci)
{
	g_CalcResult.Rms.f32IGridR = sqrt(g_ADSum.Rms.f32IGridR * f32CntReci);
	g_CalcResult.Rms.f32IGridS = sqrt(g_ADSum.Rms.f32IGridS * f32CntReci);
	g_CalcResult.Rms.f32IGridT = sqrt(g_ADSum.Rms.f32IGridT * f32CntReci);
}
						
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void GFCI_rms_Calc(float32 f32CntReci)
{
	g_CalcResult.Rms.f32GFCI = sqrt(g_ADSum.Rms.f32GFCI * f32CntReci);
}
			
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void GFCI_ave_Calc(float32 f32CntReci)
{
	float32 f32TempAve;
	f32TempAve = g_ADSum.Ave.f32GFCI * f32CntReci;
	g_CalcResult.Ave.f32GFCI = f32TempAve;
}
			
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void DCI_ave_Calc(float32 f32CntReci)
{
	g_CalcResult.Ave.f32DciR = g_ADSum.Ave.f32DciR * f32CntReci;
	g_CalcResult.Ave.f32DciS = g_ADSum.Ave.f32DciS * f32CntReci;
	g_CalcResult.Ave.f32DciT = g_ADSum.Ave.f32DciT * f32CntReci;
}

/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void GridFreqCalc(float32 f32CntReci)
{

	//PLL
	g_CalcResult.Ave.f32FGrid = g_ADSum.Ave.f32FGrid * f32CntReci;



}

/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void VPV_ave_Calc(float32 f32CntReci)
{
	g_CalcResult.Ave.f32VPV1 = g_ADSum.Ave.f32VPV1 * f32CntReci;
	g_CalcResult.Ave.f32VPV2 = g_ADSum.Ave.f32VPV2 * f32CntReci;
}

/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void IPV_ave_Calc(float32 f32CntReci)
{
	g_CalcResult.Ave.f32IPV1 = g_ADSum.Ave.f32IPV1 * f32CntReci;
	g_CalcResult.Ave.f32IPV2 = g_ADSum.Ave.f32IPV2 * f32CntReci;
}

/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void VBUS_ave_Calc(float32 f32CntReci)
{
	g_CalcResult.Ave.f32VbusP = g_ADSum.Ave.f32VbusP * f32CntReci;
	g_CalcResult.Ave.f32VbusN = g_ADSum.Ave.f32VbusN * f32CntReci;

	g_CalcResult.Ave.f32Vbus = g_CalcResult.Ave.f32VbusP + g_CalcResult.Ave.f32VbusN;

	if(g_CalcResult.Ave.f32Vbus > 100)
	{
		g_CalcResult.Ave.VdqVbusForward = 1.98 * PWM_HALF_PERIOD / g_CalcResult.Ave.f32Vbus;// 1.95,//17k:1.9
	}
	else
	{
		g_CalcResult.Ave.VdqVbusForward = 0;
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void VGrid_ave_Calc(float32 f32CntReci)
{
	g_CalcResult.Ave.f32VGridR = g_ADSum.Ave.f32VGridR * f32CntReci;
	g_CalcResult.Ave.f32VGridS = g_ADSum.Ave.f32VGridS * f32CntReci;
	g_CalcResult.Ave.f32VGridT = g_ADSum.Ave.f32VGridT * f32CntReci;
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void IGrid_ave_Calc(float32 f32CntReci)
{
	g_CalcResult.Ave.f32IGridR = g_ADSum.Ave.f32IGridR * f32CntReci;
	g_CalcResult.Ave.f32IGridS = g_ADSum.Ave.f32IGridS * f32CntReci;
	g_CalcResult.Ave.f32IGridT = g_ADSum.Ave.f32IGridT * f32CntReci;



}

/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void Power_Calc(float32 f32CntReci)
{
	g_CalcResult.Power.f32Input1Watt = g_ADSum.Power.f32Input1Watt * f32CntReci;
	g_CalcResult.Power.f32Input2Watt = g_ADSum.Power.f32Input2Watt * f32CntReci;

	g_CalcResult.Power.f32OutputWatt = g_ADSum.Power.f32OutputWatt * f32CntReci;
	g_CalcResult.Power.f32InvQ = g_ADSum.Power.f32InvQ * f32CntReci;
}

/**********************************************************************
* FUNCION :
* PURPOSE :  Calculate every 500mS
* CALLED BY: 
**********************************************************************/
void Power_Display_Calc(void)
{
	g_CalcResult.PowerDisplay.AccCalc.f32Input1Watt = g_ADSum.PowerDisplay.AccCalc.f32Input1Watt * 2 / (float32)PWM_FREQ;
	g_CalcResult.PowerDisplay.AccCalc.f32Input2Watt = g_ADSum.PowerDisplay.AccCalc.f32Input2Watt * 2 / (float32)PWM_FREQ;

	g_CalcResult.PowerDisplay.AccCalc.f32OutputWatt = g_ADSum.PowerDisplay.AccCalc.f32OutputWatt * 2 / (float32)PWM_FREQ;

	g_CalcResult.PowerDisplay.f32OutputEnergy += g_CalcResult.PowerDisplay.AccCalc.f32OutputWatt * g_f32EnergyGain;
}

/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void ADAccCalc(void)
{
	static Uint16 s_u16PowerAveCnt=0;

	g_ADAcc.u16FullGridPeriodCnt++;

//--Could calculte average every 0.5 grid period-----------------------------------
	g_ADAcc.Ave.f32VPV1 += g_Instantaneous.f32VPV1;
	g_ADAcc.Ave.f32VPV2 += g_Instantaneous.f32VPV2;
	g_ADAcc.Ave.f32IPV1 += g_Instantaneous.f32IPV1;
	g_ADAcc.Ave.f32IPV2 += g_Instantaneous.f32IPV2;

	g_ADAcc.Ave.f32VbusP += g_Instantaneous.f32VbusP;
	g_ADAcc.Ave.f32VbusN += g_Instantaneous.f32VbusN;

//--Must calculate average every grid period---------------------------------------
	if (0 == g_StateCheck.bit.ADOffsetCalibrateOver)
	{
		g_ADAcc.Ave.f32VGridR += g_Instantaneous.f32VGridR;
		g_ADAcc.Ave.f32VGridS += g_Instantaneous.f32VGridS;
		g_ADAcc.Ave.f32VGridT += g_Instantaneous.f32VGridT;
	}

	g_ADAcc.Ave.f32IGridR += g_Instantaneous.f32IGridR;
	g_ADAcc.Ave.f32IGridS += g_Instantaneous.f32IGridS;
	g_ADAcc.Ave.f32IGridT += g_Instantaneous.f32IGridT;

	g_ADAcc.Ave.f32DciR += g_Instantaneous.f32DciR;
	g_ADAcc.Ave.f32DciS += g_Instantaneous.f32DciS;
	g_ADAcc.Ave.f32DciT += g_Instantaneous.f32DciT;

	g_ADAcc.Ave.f32GFCI += g_Instantaneous.f32GFCI;
	g_ADAcc.Ave.f32FGrid += g_PLLCal.f32ThetaStep * CoffStepToFre;

//-----RMS--------------------------------------------------------------------
	g_ADAcc.Rms.f32IGridR +=	g_Instantaneous.f32IGridR * g_Instantaneous.f32IGridR;
	g_ADAcc.Rms.f32IGridS +=	g_Instantaneous.f32IGridS * g_Instantaneous.f32IGridS;
	g_ADAcc.Rms.f32IGridT +=	g_Instantaneous.f32IGridT * g_Instantaneous.f32IGridT;

	g_ADAcc.Rms.f32VGridR +=	g_Instantaneous.f32VGridR * g_Instantaneous.f32VGridR;
	g_ADAcc.Rms.f32VGridS +=	g_Instantaneous.f32VGridS * g_Instantaneous.f32VGridS;
	g_ADAcc.Rms.f32VGridT +=	g_Instantaneous.f32VGridT * g_Instantaneous.f32VGridT;

	g_ADAcc.Rms.f32VGridRS += (g_Instantaneous.f32VGridR-g_Instantaneous.f32VGridS) * (g_Instantaneous.f32VGridR-g_Instantaneous.f32VGridS);
	g_ADAcc.Rms.f32VGridST += (g_Instantaneous.f32VGridS-g_Instantaneous.f32VGridT) * (g_Instantaneous.f32VGridS-g_Instantaneous.f32VGridT);
	g_ADAcc.Rms.f32VGridTR += (g_Instantaneous.f32VGridT-g_Instantaneous.f32VGridR) * (g_Instantaneous.f32VGridT-g_Instantaneous.f32VGridR);

	g_ADAcc.Rms.f32GFCI += g_Instantaneous.f32GFCI * g_Instantaneous.f32GFCI;

//-----Power and Energy-------------------------------------------------------
	g_ADAcc.Power.f32Input1Watt += g_Instantaneous.f32IPV1 * g_Instantaneous.f32VPV1;
	g_ADAcc.Power.f32Input2Watt += g_Instantaneous.f32IPV2 * g_Instantaneous.f32VPV2;
	g_ADAcc.Power.f32OutputWatt += g_Instantaneous.f32IGridR * (g_Instantaneous.f32VGridR - g_Instantaneous.f32VGridT)
						       + g_Instantaneous.f32IGridS * (g_Instantaneous.f32VGridS - g_Instantaneous.f32VGridT);
	g_ADAcc.Power.f32InvQ += 0.57735 * ( (g_Instantaneous.f32VGridS - g_Instantaneous.f32VGridT) * g_Instantaneous.f32IGridR
										+ (g_Instantaneous.f32VGridT - g_Instantaneous.f32VGridR) * g_Instantaneous.f32IGridS
										 + (g_Instantaneous.f32VGridR - g_Instantaneous.f32VGridS) * g_Instantaneous.f32IGridT);
	g_ADAcc.PowerDisplay.AccCalc.f32Input1Watt += g_Instantaneous.f32IPV1 * g_Instantaneous.f32VPV1;
	g_ADAcc.PowerDisplay.AccCalc.f32Input2Watt += g_Instantaneous.f32IPV2 * g_Instantaneous.f32VPV2;
	g_ADAcc.PowerDisplay.AccCalc.f32OutputWatt += g_Instantaneous.f32IGridR * (g_Instantaneous.f32VGridR - g_Instantaneous.f32VGridT)
						    	               + g_Instantaneous.f32IGridS * (g_Instantaneous.f32VGridS - g_Instantaneous.f32VGridT);
//----------------------------------------------------------------------------
	if(1 == g_StateCheck.bit.GridCrossingFlag)
	{
		g_StateCheck.bit.GridCrossingFlag = 0;

		g_ADSum.u16FullGridPeriodCnt = g_ADAcc.u16FullGridPeriodCnt;
//----------------------------------------------------------------------------
		g_ADSum.Ave.f32VPV1 = g_ADAcc.Ave.f32VPV1;
		g_ADSum.Ave.f32VPV2 = g_ADAcc.Ave.f32VPV2;
		g_ADSum.Ave.f32IPV1 = g_ADAcc.Ave.f32IPV1;
		g_ADSum.Ave.f32IPV2 = g_ADAcc.Ave.f32IPV2;

		g_ADSum.Ave.f32VbusP = g_ADAcc.Ave.f32VbusP;
		g_ADSum.Ave.f32VbusN = g_ADAcc.Ave.f32VbusN;

//--Must calculate average every grid period---------------------------------------
		
		g_ADSum.Ave.f32VGridR = g_ADAcc.Ave.f32VGridR;
		g_ADSum.Ave.f32VGridS = g_ADAcc.Ave.f32VGridS;
		g_ADSum.Ave.f32VGridT = g_ADAcc.Ave.f32VGridT;
		

		g_ADSum.Ave.f32IGridR = g_ADAcc.Ave.f32IGridR;
		g_ADSum.Ave.f32IGridS = g_ADAcc.Ave.f32IGridS;
		g_ADSum.Ave.f32IGridT = g_ADAcc.Ave.f32IGridT;

		g_ADSum.Ave.f32DciR = g_ADAcc.Ave.f32DciR;
		g_ADSum.Ave.f32DciS = g_ADAcc.Ave.f32DciS;
		g_ADSum.Ave.f32DciT = g_ADAcc.Ave.f32DciT;

		g_ADSum.Ave.f32GFCI = g_ADAcc.Ave.f32GFCI;

		g_ADSum.Ave.f32FGrid = g_ADAcc.Ave.f32FGrid;
		g_ADSum.Ave.f32FGridR = g_ADAcc.Ave.f32FGridR;
		g_ADSum.Ave.f32FGridS = g_ADAcc.Ave.f32FGridS;
		g_ADSum.Ave.f32FGridT = g_ADAcc.Ave.f32FGridT;

		g_ADAcc.Ave.f32VPV1 =0;
		g_ADAcc.Ave.f32VPV2 =0;
		g_ADAcc.Ave.f32IPV1 =0;
		g_ADAcc.Ave.f32IPV2 =0;

		g_ADAcc.Ave.f32VbusP =0;
		g_ADAcc.Ave.f32VbusN =0;

		g_ADAcc.Ave.f32VGridR =0;
		g_ADAcc.Ave.f32VGridS =0;
		g_ADAcc.Ave.f32VGridT =0;		
		
		g_ADAcc.Ave.f32IGridR =0;
		g_ADAcc.Ave.f32IGridS =0;
		g_ADAcc.Ave.f32IGridT =0;

		g_ADAcc.Ave.f32DciR =0;
		g_ADAcc.Ave.f32DciS =0;
		g_ADAcc.Ave.f32DciT =0;

		g_ADAcc.Ave.f32GFCI =0;

		g_ADAcc.Ave.f32FGrid =0;
		g_ADAcc.Ave.f32FGridR = 0;
		g_ADAcc.Ave.f32FGridS = 0;
		g_ADAcc.Ave.f32FGridT = 0;

//-----RMS--------------------------------------------------------------------
		g_ADSum.Rms.f32IGridR = g_ADAcc.Rms.f32IGridR;
		g_ADSum.Rms.f32IGridS = g_ADAcc.Rms.f32IGridS;
		g_ADSum.Rms.f32IGridT = g_ADAcc.Rms.f32IGridT;

		g_ADSum.Rms.f32VGridR = g_ADAcc.Rms.f32VGridR;
		g_ADSum.Rms.f32VGridS = g_ADAcc.Rms.f32VGridS;
		g_ADSum.Rms.f32VGridT = g_ADAcc.Rms.f32VGridT;

		g_ADSum.Rms.f32VGridRS = g_ADAcc.Rms.f32VGridRS;
		g_ADSum.Rms.f32VGridST = g_ADAcc.Rms.f32VGridST;
		g_ADSum.Rms.f32VGridTR = g_ADAcc.Rms.f32VGridTR;

		g_ADSum.Rms.f32GFCI = g_ADAcc.Rms.f32GFCI;

		g_ADAcc.Rms.f32IGridR =0;
		g_ADAcc.Rms.f32IGridS =0;
		g_ADAcc.Rms.f32IGridT =0;

		g_ADAcc.Rms.f32VGridR =0;
		g_ADAcc.Rms.f32VGridS =0;
		g_ADAcc.Rms.f32VGridT =0;

		g_ADAcc.Rms.f32VGridRS =0;
		g_ADAcc.Rms.f32VGridST =0;
		g_ADAcc.Rms.f32VGridTR =0;

		g_ADAcc.Rms.f32GFCI =0;

//		g_ADSum.Rms = g_ADAcc.Rms;
//		AD_CALCULATE_RMS_Clear(&g_ADAcc.Rms);
		g_ADSum.Power.f32Input1Watt = g_ADAcc.Power.f32Input1Watt;
		g_ADSum.Power.f32Input2Watt = g_ADAcc.Power.f32Input2Watt;
		g_ADSum.Power.f32OutputWatt = g_ADAcc.Power.f32OutputWatt;
		g_ADSum.Power.f32InvQ = g_ADAcc.Power.f32InvQ;

		g_ADAcc.Power.f32Input1Watt =0;
		g_ADAcc.Power.f32Input2Watt =0;
		g_ADAcc.Power.f32OutputWatt =0;
		g_ADAcc.Power.f32InvQ = 0;

//		g_ADSum.Power = g_ADAcc.Power;
//		AD_CALCULATE_POWER_AVE_Clear(&g_ADAcc.Power);

		g_ADAcc.u16FullGridPeriodCnt = 0;
		
		SEM_post(&SEM_TskGridPeriodCalcReady);
	}

	s_u16PowerAveCnt++;
	if(s_u16PowerAveCnt >= PWM_FREQ*0.5)	// 2=1S/500mS, calculate every 500mS
	{
		s_u16PowerAveCnt = 0;
		
		g_ADSum.PowerDisplay.AccCalc.f32Input1Watt = g_ADAcc.PowerDisplay.AccCalc.f32Input1Watt;
		g_ADSum.PowerDisplay.AccCalc.f32Input2Watt = g_ADAcc.PowerDisplay.AccCalc.f32Input2Watt;
		g_ADSum.PowerDisplay.AccCalc.f32OutputWatt = g_ADAcc.PowerDisplay.AccCalc.f32OutputWatt;
		
		g_ADAcc.PowerDisplay.AccCalc.f32Input1Watt = 0;
		g_ADAcc.PowerDisplay.AccCalc.f32Input2Watt = 0;
		g_ADAcc.PowerDisplay.AccCalc.f32OutputWatt = 0;
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
* NOTE :     Executed every grid period.
**********************************************************************/
void ADOffsetCalibration(void)
{
	static Uint16 s_cnt = 0;
	struct	ADOFFSET_SAMPLE
	{
		float32	f32VGridR;
		float32	f32VGridS;
		float32	f32VGridT;
		float32	f32IGridR;
		float32	f32IGridS;
		float32	f32IGridT;
		float32	f32GFCI;
		float32	f32DciR;
		float32	f32DciS;
		float32	f32DciT;
	};
	static struct ADOFFSET_SAMPLE ADOffsetSum={0,0,0,0,0,0,0,0,0,0};
	if(1 == g_StateCheck.bit.ADOffsetCalibrateOver)
	{
		ADOffsetSum.f32VGridR = 0;
		ADOffsetSum.f32VGridS = 0;
		ADOffsetSum.f32VGridT = 0;
		ADOffsetSum.f32IGridR = 0;
		ADOffsetSum.f32IGridS = 0;
		ADOffsetSum.f32IGridT = 0;
		ADOffsetSum.f32GFCI = 0;
		ADOffsetSum.f32DciR = 0;
		ADOffsetSum.f32DciS = 0;
		ADOffsetSum.f32DciT = 0;

		s_cnt = 0;
		return;
	}
	else
	{
		g_ADConvert.offset.f32VGridR = 0;
		g_ADConvert.offset.f32VGridS = 0;
		g_ADConvert.offset.f32VGridT = 0;
		g_ADConvert.offset.f32IGridR = 0;
		g_ADConvert.offset.f32IGridS = 0;
		g_ADConvert.offset.f32IGridT = 0;
		g_ADConvert.offset.f32GFCI = 0;
		g_ADConvert.offset.f32DciR = 0;
		g_ADConvert.offset.f32DciS = 0;
		g_ADConvert.offset.f32DciT = 0;
	}
	if(s_cnt <= 32)	// 32个电网周期
	{
		if(s_cnt > 0)	// 从锁相后第2个周期开始累加，确保完整周期
		{
			ADOffsetSum.f32VGridR += g_CalcResult.Ave.f32VGridR;
			ADOffsetSum.f32VGridS += g_CalcResult.Ave.f32VGridS;
			ADOffsetSum.f32VGridT += g_CalcResult.Ave.f32VGridT;
			ADOffsetSum.f32IGridR += g_CalcResult.Ave.f32IGridR;
			ADOffsetSum.f32IGridS += g_CalcResult.Ave.f32IGridS;
			ADOffsetSum.f32IGridT += g_CalcResult.Ave.f32IGridT;
			ADOffsetSum.f32GFCI += g_CalcResult.Ave.f32GFCI;
			ADOffsetSum.f32DciR += g_CalcResult.Ave.f32DciR;
			ADOffsetSum.f32DciS += g_CalcResult.Ave.f32DciS;
			ADOffsetSum.f32DciT += g_CalcResult.Ave.f32DciT;
		}

		s_cnt++;
	}
	else
	{
		ADOffsetSum.f32VGridR = ADOffsetSum.f32VGridR * 0.03125;	// 0.03125 = 1/32
		ADOffsetSum.f32VGridS = ADOffsetSum.f32VGridS * 0.03125;
		ADOffsetSum.f32VGridT = ADOffsetSum.f32VGridT * 0.03125;
		ADOffsetSum.f32IGridR = ADOffsetSum.f32IGridR * 0.03125;
		ADOffsetSum.f32IGridS = ADOffsetSum.f32IGridS * 0.03125;
		ADOffsetSum.f32IGridT = ADOffsetSum.f32IGridT * 0.03125;
		ADOffsetSum.f32GFCI = ADOffsetSum.f32GFCI * 0.03125;
		ADOffsetSum.f32DciR = ADOffsetSum.f32DciR * 0.03125;
		ADOffsetSum.f32DciS = ADOffsetSum.f32DciS * 0.03125;
		ADOffsetSum.f32DciT = ADOffsetSum.f32DciT * 0.03125;

		g_ADConvert.offset.f32VGridR = ADOffsetSum.f32VGridR;
		g_ADConvert.offset.f32VGridS = ADOffsetSum.f32VGridS;
		g_ADConvert.offset.f32VGridT = ADOffsetSum.f32VGridT;
		g_ADConvert.offset.f32IGridR = ADOffsetSum.f32IGridR;
		g_ADConvert.offset.f32IGridS = ADOffsetSum.f32IGridS;
		g_ADConvert.offset.f32IGridT = ADOffsetSum.f32IGridT;
		g_ADConvert.offset.f32GFCI = ADOffsetSum.f32GFCI;
		g_ADConvert.offset.f32DciR = ADOffsetSum.f32DciR;
		g_ADConvert.offset.f32DciS = ADOffsetSum.f32DciS;
		g_ADConvert.offset.f32DciT = ADOffsetSum.f32DciT;

		ADOffsetSum.f32VGridR = 0;
		ADOffsetSum.f32VGridS = 0;
		ADOffsetSum.f32VGridT = 0;
		ADOffsetSum.f32IGridR = 0;
		ADOffsetSum.f32IGridS = 0;
		ADOffsetSum.f32IGridT = 0;
		ADOffsetSum.f32GFCI = 0;
		ADOffsetSum.f32DciR = 0;
		ADOffsetSum.f32DciS = 0;
		ADOffsetSum.f32DciT = 0;

		s_cnt = 0;

		g_StateCheck.bit.ADOffsetCalibrateOver = 1;
	}
}

/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void ChoiceKindsMachine(void)
{
	Uint16 u16type;
	
	u16type = (Uint16)((GpioDataRegs.GPCDAT.all & 0x0000F000)>>12);
	switch(u16type)
	{
		case 0:	
			g_InverterType = SolarLake5K;
		break;
		case 1:	
			g_InverterType = SolarLake6K;
		break;
		case 2:
			g_InverterType = SolarLake8K;
		break;
		case 3:
			g_InverterType = SolarLake10K;
		break;
		case 4:
			g_InverterType = SolarLake12K;
		break;
		case 5:
			g_InverterType = SolarLake15K;
		break;
		case 6:
			g_InverterType = SolarLake17K;
		break;
		case 7:
			g_InverterType = SolarLake20K;
		break;
		case 8:
			g_InverterType = SolarLake25K;
		break;	
		case 9:
			g_InverterType = SolarLake30K;
		break;
		default:
			g_InverterType = InvalidType;
		break;
	}
}
//===========================================================================
// No more.
//===========================================================================

