/***********************************************************************
 *    FILENAME : SC_ProtectionLogic.c
 *
 *    PURPOSE  : 
 *    HISTORY  :
 *           DATE          VERSION      AUTHOR        NOTE
 *
 ************************************************************************/
#include "SC_master.h"


/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void GridVoltCheck(void)
{
//	PhaseSeqCheck();
	static Uint16 s_u16UVPCntExit = 0;
	static Uint16 s_u16OVPCntExit = 0;
	static Uint16 s_u16UVPUnrecoverCnt = 0;

	VGrid_PreConnectionCheck();	
	//----VGrid Over Check----
	if(1 == g_SaftyConfig.bit.OVP2Enable)
	{
		VGrid_Over2Check();
	}
	if(1 == g_SaftyConfig.bit.OVP1Enable)
	{
		VGrid_Over1Check();
	}
		
	//----VGrid Under Check----
	if(1 == g_SaftyConfig.bit.UVP2Enable)
	{
		VGrid_Under2Check();
	}
	if(1 == g_SaftyConfig.bit.UVP1Enable)
	{
		VGrid_Under1Check();
	}

	if(1 == g_SysFaultMessage.bit.GridUVP)
	{
		if(   g_CalcResult.Rms.f32VGridR > g_SafetyConnect.f32VGridMinLimitBack
		   && g_CalcResult.Rms.f32VGridS > g_SafetyConnect.f32VGridMinLimitBack
		   && g_CalcResult.Rms.f32VGridT > g_SafetyConnect.f32VGridMinLimitBack	)
		{
			s_u16UVPCntExit++;
			if(s_u16UVPCntExit >= 250)
			{
				s_u16UVPCntExit = 0;
//				if(0 == s_u16UVPUnrecoverCnt)
//				{
					g_SysFaultMessage.bit.GridUVP = 0;
//				}
				s_u16UVPUnrecoverCnt = 3100;
				g_u16SafetyCountDown = g_SafetyConnect.u16ReconnectTime;
				g_f32PowerLoadSpeed = g_SafetyConnect.f32ReconnectPupSlop;
			}
		}
		else
		{
			s_u16UVPCntExit = 0;
		}
	}
	else
	{
		if(s_u16UVPUnrecoverCnt>0)
		{
			s_u16UVPUnrecoverCnt--;
		}
	}

	if(1 == g_SysFaultMessage.bit.GridOVP)
	{
		if(   g_CalcResult.Rms.f32VGridR < g_SafetyConnect.f32VGridMaxLimitBack
		   && g_CalcResult.Rms.f32VGridS < g_SafetyConnect.f32VGridMaxLimitBack
		   && g_CalcResult.Rms.f32VGridT < g_SafetyConnect.f32VGridMaxLimitBack	)
		{
			s_u16OVPCntExit++;
			if(s_u16OVPCntExit >= 250)
			{
				s_u16OVPCntExit = 0;
				g_SysFaultMessage.bit.GridOVP = 0;
				g_u16SafetyCountDown = g_SafetyConnect.u16ReconnectTime;
				g_f32PowerLoadSpeed = g_SafetyConnect.f32ReconnectPupSlop;
			}
		}
		else
		{
			s_u16OVPCntExit = 0;
		}
	}
}


/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void VGrid_PreConnectionCheck(void)
{
	static Uint16 s_u16UVPCntEnter = 0;
	static Uint16 s_u16OVPCntEnter = 0;
	if((WaitState == g_SysCurrentState)&& (1 == g_StateCheck.bit.SafetySetOK))
	{
		if(   g_CalcResult.Rms.f32VGridR > g_SafetyConnect.f32VGridMaxLimitBack
		   || g_CalcResult.Rms.f32VGridS > g_SafetyConnect.f32VGridMaxLimitBack
		   || g_CalcResult.Rms.f32VGridT > g_SafetyConnect.f32VGridMaxLimitBack	)
		{
			s_u16OVPCntEnter++;
			if(s_u16OVPCntEnter >= 25)//g_SafetyReg.u16VGridMax1ProtectTime)
			{
				s_u16OVPCntEnter = 0;
				g_SysFaultMessage.bit.GridOVP = 1;
			}
		}
		else
		{
			s_u16OVPCntEnter = 0;
		}

		if(   g_CalcResult.Rms.f32VGridR < g_SafetyConnect.f32VGridMinLimitBack
		   || g_CalcResult.Rms.f32VGridS < g_SafetyConnect.f32VGridMinLimitBack
		   || g_CalcResult.Rms.f32VGridT < g_SafetyConnect.f32VGridMinLimitBack	)
		{
			s_u16UVPCntEnter++;
			if(s_u16UVPCntEnter >= 25)//g_SafetyReg.u16VGridMin1ProtectTime)
			{
				s_u16UVPCntEnter = 0;
				g_SysFaultMessage.bit.GridUVP = 1;
			}
		}
		else
		{
			s_u16UVPCntEnter = 0;
		}
	}
}


/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void VGrid_Over1Check(void)
{
	static Uint16 s_u16CntEnter = 0;

	if(0 == g_SysFaultMessage.bit.GridOVP)
	{
		if(   g_CalcResult.Rms.f32VGridR > g_SafetyVolt.f32VGridMax1Limit
		   || g_CalcResult.Rms.f32VGridS > g_SafetyVolt.f32VGridMax1Limit
		   || g_CalcResult.Rms.f32VGridT > g_SafetyVolt.f32VGridMax1Limit	)
		{
			s_u16CntEnter++;
			if(s_u16CntEnter >= g_SafetyVolt.u16VGridMax1ProtectTime)
			{
				s_u16CntEnter = 0;
				g_SysFaultMessage.bit.GridOVP = 1;
			}
		}
		else
		{
			s_u16CntEnter = 0;
		}
	}

}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void VGrid_Over2Check(void)
{
	static Uint16 s_u16CntEnter = 0;

	if(0 == g_SysFaultMessage.bit.GridOVP)
	{
		if(   g_CalcResult.Rms.f32VGridR > g_SafetyVolt.f32VGridMax2Limit
		   || g_CalcResult.Rms.f32VGridS > g_SafetyVolt.f32VGridMax2Limit
		   || g_CalcResult.Rms.f32VGridT > g_SafetyVolt.f32VGridMax2Limit	)
		{
			s_u16CntEnter++;
			if(s_u16CntEnter >= g_SafetyVolt.u16VGridMax2ProtectTime)
			{
				s_u16CntEnter = 0;
				g_SysFaultMessage.bit.GridOVP = 1;
			}
		}
		else
		{
			s_u16CntEnter = 0;
		}
	}
}

/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void VGrid_Under1Check(void)
{
	static Uint16 s_u16CntEnter = 0;

	if(0 == g_SysFaultMessage.bit.GridUVP)
	{
		if(   g_CalcResult.Rms.f32VGridR < g_SafetyVolt.f32VGridMin1Limit
		   || g_CalcResult.Rms.f32VGridS < g_SafetyVolt.f32VGridMin1Limit
		   || g_CalcResult.Rms.f32VGridT < g_SafetyVolt.f32VGridMin1Limit	)
		{
			s_u16CntEnter++;
			if(s_u16CntEnter >= g_SafetyVolt.u16VGridMin1ProtectTime)
			{
				s_u16CntEnter = 0;
				g_SysFaultMessage.bit.GridUVP = 1;
			}
		}
		else
		{
			s_u16CntEnter = 0;
		}
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void VGrid_Under2Check(void)
{
	static Uint16 s_u16CntEnter = 0;

	if(0 == g_SysFaultMessage.bit.GridUVP)
	{
		if(   g_CalcResult.Rms.f32VGridR < g_SafetyVolt.f32VGridMin2Limit
		   || g_CalcResult.Rms.f32VGridS < g_SafetyVolt.f32VGridMin2Limit
		   || g_CalcResult.Rms.f32VGridT < g_SafetyVolt.f32VGridMin2Limit	)
		{
			s_u16CntEnter++;
			if(s_u16CntEnter >= g_SafetyVolt.u16VGridMin2ProtectTime)
			{
				s_u16CntEnter = 0;
				g_SysFaultMessage.bit.GridUVP = 1;
			}
		}
		else
		{
			s_u16CntEnter = 0;
		}
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void VGrid10minMovingAveCheck(void)
{	
	if(0 == g_SysFaultMessage.bit.GridOVP)
	{
		if(g_VGridMovAve.f32R > g_SafetyVolt.f32VGridTenMinutesLimit)
		{
			g_SysFaultMessage.bit.GridOVP = 1;
		}

		if(g_VGridMovAve.f32S > g_SafetyVolt.f32VGridTenMinutesLimit)
		{
			g_SysFaultMessage.bit.GridOVP = 1;
		}

		if(g_VGridMovAve.f32T > g_SafetyVolt.f32VGridTenMinutesLimit)
		{
			g_SysFaultMessage.bit.GridOVP = 1;
		}
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void GridVoltADOffsetCheck(void)
{
	if(1 == g_SysFaultEnable.bit.HwADFaultVGrid)
	{
		VGrid_ADOffsetCheck();
	}
	else
	{
		g_SysFaultMessage.bit.HwADFaultVGrid = 0;
	}
}

/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void VGrid_ADOffsetCheck(void)
{
	static Uint16 s_u16CntExit = 0;
	float32 f32VGridAveMax;

	f32VGridAveMax = MAX(FABS(g_ADConvert.offset.f32VGridR), FABS(g_ADConvert.offset.f32VGridS));
	f32VGridAveMax = MAX(FABS(g_ADConvert.offset.f32VGridT), f32VGridAveMax); 
	
	if(0 == g_SysFaultMessage.bit.HwADFaultVGrid)
	{
		if(f32VGridAveMax > AD_OFFSET_VGRID_LIMIT) //
		{
			g_SysFaultMessage.bit.HwADFaultVGrid = 1;
		}
	}
	else
	{
		s_u16CntExit++;
		if(s_u16CntExit >= 250)
		{
			s_u16CntExit = 0;
			g_SysFaultMessage.bit.HwADFaultVGrid = 0;
		}
	}
}

/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void GridCurrentADOffsetCheck(void)
{
	if(1 == g_SysFaultEnable.bit.HwADFaultIGrid)
	{
		IGrid_ADOffsetCheck();
	}
	else
	{
		g_SysFaultMessage.bit.HwADFaultIGrid = 0;
	}
}

/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void IGrid_ADOffsetCheck(void)
{
	static Uint16 s_u16CntExit = 0;
	float32 f32IGridAveMax;

	f32IGridAveMax = MAX(FABS(g_ADConvert.offset.f32IGridR), FABS(g_ADConvert.offset.f32IGridS));
	f32IGridAveMax = MAX(FABS(g_ADConvert.offset.f32IGridT), f32IGridAveMax); 
	
	if(0 == g_SysFaultMessage.bit.HwADFaultIGrid)
	{
		if(f32IGridAveMax > AD_OFFSET_IGRID_LIMIT)//
		{
			g_SysFaultMessage.bit.HwADFaultIGrid = 1;
		}
	}
	else
	{
		s_u16CntExit++;
		if(s_u16CntExit >= 250)
		{
			s_u16CntExit = 0;
			g_SysFaultMessage.bit.HwADFaultIGrid = 0;
		}
	}
}

/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void DCIADOffsetCheck(void)
{
	if(1 == g_SysFaultEnable.bit.HwADFaultDCI)
	{
		IGridDCI_ADOffsetCheck();
	}
	else
	{
		g_SysFaultMessage.bit.HwADFaultDCI = 0;
	}
}

/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void IGridDCI_ADOffsetCheck(void)
{
	static Uint16 s_u16CntExit = 0;
	float32 f32DCIAveMax;

	f32DCIAveMax = MAX(FABS(g_ADConvert.offset.f32DciR), FABS(g_ADConvert.offset.f32DciS));
	f32DCIAveMax = MAX(FABS(g_ADConvert.offset.f32DciT), f32DCIAveMax); 
	
	if(0 == g_SysFaultMessage.bit.HwADFaultDCI)
	{
		if(f32DCIAveMax > AD_OFFSET_DCI_LIMIT)
		{
			g_SysFaultMessage.bit.HwADFaultDCI = 1;
		}
	}
	else
	{
		s_u16CntExit++;
		if(s_u16CntExit >= 250)
		{
			s_u16CntExit = 0;
			g_SysFaultMessage.bit.HwADFaultDCI = 0;
		}
	}
}

/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void GridFreqCheck(void)
{
	static Uint16 s_u16OFPCntExit = 0;
	static Uint16 s_u16UFPCntExit = 0;
	// Over Check

	FGrid_PreConnectionCheck();	
	if(1 == g_SaftyConfig.bit.OFP2Enable)
	{	
		GridFreqOver2Check();
	}
	if(1 == g_SaftyConfig.bit.OFP1Enable)
	{	
		GridFreqOver1Check();
	}
	// Under Check
	if(1 == g_SaftyConfig.bit.UFP2Enable)
	{	
		GridFreqUnder2Check();
	}
	if(1 == g_SaftyConfig.bit.UFP1Enable)
	{	
		GridFreqUnder1Check();
	}

	if(1 == g_SysFaultMessage.bit.GridOFP)
	{
		if(g_CalcResult.Ave.f32FGrid < g_SafetyConnect.f32FGridMaxLimitBack)
		{
			s_u16OFPCntExit++;
			if(s_u16OFPCntExit >= 250)
			{
				s_u16OFPCntExit = 0;
				g_SysFaultMessage.bit.GridOFP = 0;
				g_u16SafetyCountDown = g_SafetyConnect.u16ReconnectTime;
				g_f32PowerLoadSpeed = g_SafetyConnect.f32ReconnectPupSlop;
			}
		}
		else
		{
			s_u16OFPCntExit = 0;
		}
	}

	if(1 == g_SysFaultMessage.bit.GridUFP)
	{
		if(g_CalcResult.Ave.f32FGrid > g_SafetyConnect.f32FGridMinLimitBack)
		{
			s_u16UFPCntExit++;
			if(s_u16UFPCntExit >= 250)
			{
				s_u16UFPCntExit = 0;
				g_SysFaultMessage.bit.GridUFP = 0;
				g_u16SafetyCountDown = g_SafetyConnect.u16ReconnectTime;
				g_f32PowerLoadSpeed = g_SafetyConnect.f32ReconnectPupSlop;
			}
		}
		else
		{
			s_u16UFPCntExit = 0;
		}
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void FGrid_PreConnectionCheck(void)
{
	static Uint16 s_u16UFPCntEnter = 0;
	static Uint16 s_u16OFPCntEnter = 0;
	if((WaitState == g_SysCurrentState)&& (1 == g_StateCheck.bit.SafetySetOK))
	{
		if(g_CalcResult.Ave.f32FGrid < g_SafetyConnect.f32FGridMinLimitBack)
		{
			s_u16UFPCntEnter++;
			if(s_u16UFPCntEnter >= 25)//g_SafetyReg.u16VGridMax1ProtectTime)
			{
				s_u16UFPCntEnter = 0;
				g_SysFaultMessage.bit.GridUFP = 1;
			}
		}
		else
		{
			s_u16UFPCntEnter = 0;
		}

		if(g_CalcResult.Ave.f32FGrid > g_SafetyConnect.f32FGridMaxLimitBack)
		{
			s_u16OFPCntEnter++;
			if(s_u16OFPCntEnter >= 25)//g_SafetyReg.u16VGridMin1ProtectTime)
			{
				s_u16OFPCntEnter = 0;
				g_SysFaultMessage.bit.GridOFP = 1;
			}
		}
		else
		{
			s_u16OFPCntEnter = 0;
		}
	}
}


/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void GridFreqOver1Check(void)
{
	static Uint16 s_u16CntEnter = 0;

	if(0 == g_SysFaultMessage.bit.GridOFP)
	{
		if(g_CalcResult.Ave.f32FGrid > g_SafetyFreq.f32FGridMax1Limit)
		{
			s_u16CntEnter++;
			if(s_u16CntEnter >= g_SafetyFreq.u16FGridMax1ProtectTime)
			{
				s_u16CntEnter = 0;
				g_SysFaultMessage.bit.GridOFP = 1;
			}
		}
		else
		{
			s_u16CntEnter = 0;
		}
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void GridFreqOver2Check(void)
{
	static Uint16 s_u16CntEnter = 0;

	if(0 == g_SysFaultMessage.bit.GridOFP)
	{
		if(g_CalcResult.Ave.f32FGrid > g_SafetyFreq.f32FGridMax2Limit)
		{
			s_u16CntEnter++;
			if(s_u16CntEnter >= g_SafetyFreq.u16FGridMax2ProtectTime)
			{
				s_u16CntEnter = 0;
				g_SysFaultMessage.bit.GridOFP = 1;
			}
		}
		else
		{
			s_u16CntEnter = 0;
		}
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void GridFreqUnder1Check(void)
{
	static Uint16 s_u16CntEnter = 0;

	if(0 == g_SysFaultMessage.bit.GridUFP)
	{
		if(g_CalcResult.Ave.f32FGrid < g_SafetyFreq.f32FGridMin1Limit)
		{
			s_u16CntEnter++;
			if(s_u16CntEnter >= g_SafetyFreq.u16FGridMin1ProtectTime)
			{
				s_u16CntEnter = 0;
				g_SysFaultMessage.bit.GridUFP = 1;
			}
		}
		else
		{
			s_u16CntEnter = 0;
		}
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void GridFreqUnder2Check(void)
{
	static Uint16 s_u16CntEnter = 0;

	if(0 == g_SysFaultMessage.bit.GridUFP)
	{
		if(g_CalcResult.Ave.f32FGrid < g_SafetyFreq.f32FGridMin2Limit)
		{
			s_u16CntEnter++;
			if(s_u16CntEnter >= g_SafetyFreq.u16FGridMin2ProtectTime)
			{
				s_u16CntEnter = 0;
				g_SysFaultMessage.bit.GridUFP = 1;
			}
		}
		else
		{
			s_u16CntEnter = 0;
		}
	}
}

/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void GridCurrentCheck(void)
{
//	Uint16	u16OldState_IAC_Unbalance;
	float32 f32IGridDiffMax;

	if(1 == g_SysFaultEnable.bit.AcRmsOCP)
	{
		IGrid_OverCheck();
	}
	else
	{
		g_SysFaultMessage.bit.AcRmsOCP = 0;
	}

	if(1 == g_SysFaultEnable.bit.IacRmsUnbalance)
	{
		f32IGridDiffMax = MAX(FABS(g_CalcResult.Rms.f32IGridR-g_CalcResult.Rms.f32IGridS), FABS(g_CalcResult.Rms.f32IGridS-g_CalcResult.Rms.f32IGridT));
		f32IGridDiffMax = MAX(FABS(g_CalcResult.Rms.f32IGridT-g_CalcResult.Rms.f32IGridR), f32IGridDiffMax);
		IGridUnbalanceCheck(f32IGridDiffMax);//LVRT???
	}
	else
	{
		g_SysFaultMessage.bit.IacRmsUnbalance = 0;
	}

}

/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void IGrid_OverCheck(void)
{
	static Uint16 s_u16CntEnter = 0;
	static Uint16 s_u16CntExit = 0;

	if(0 == g_SysFaultMessage.bit.AcRmsOCP)
	{
		if(   g_CalcResult.Rms.f32IGridR > g_Rated.f32OutputCurrentRms+2.0
		   || g_CalcResult.Rms.f32IGridS > g_Rated.f32OutputCurrentRms+2.0
		   || g_CalcResult.Rms.f32IGridT > g_Rated.f32OutputCurrentRms+2.0	)
		{
			s_u16CntEnter++;
			if(s_u16CntEnter >= 10)
			{
				s_u16CntEnter = 0;
				g_SysFaultMessage.bit.AcRmsOCP = 1;
			}
		}
		else
		{
			s_u16CntEnter = 0;
		}
	}
	else
	{
		if(   g_CalcResult.Rms.f32IGridR < g_Rated.f32OutputCurrentRms
		   && g_CalcResult.Rms.f32IGridS < g_Rated.f32OutputCurrentRms
		   && g_CalcResult.Rms.f32IGridT < g_Rated.f32OutputCurrentRms	)
		{
			s_u16CntExit++;
			if(s_u16CntExit >= 250)
			{
				s_u16CntExit = 0;
				g_SysFaultMessage.bit.AcRmsOCP = 0;
			}
		}
		else
		{
			s_u16CntExit = 0;
		}
	}
}

/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void IGridUnbalanceCheck(float32 f32IGridDiffMax)
{
	static Uint16 s_u16CntEnter = 0;
	static Uint16 s_u16CntExit = 0;
	static Uint16 s_u16UnrecoverCntEnter = 0;	
	
	if(0 == g_SysFaultMessage.bit.IacRmsUnbalance)
	{
		if(f32IGridDiffMax > IAC_UNBALANCE_LIMIT)
		{
			s_u16CntEnter++;
			if(s_u16CntEnter >= 30)
			{
				s_u16CntEnter = 0;
				g_SysFaultMessage.bit.IacRmsUnbalance = 1;
				s_u16UnrecoverCntEnter++;
				if(s_u16UnrecoverCntEnter>3)
				{
					g_SysFaultMessage.bit.unrecoverIacRmsUnbalance = 1;
				}
			}
		}
		else
		{
			s_u16CntEnter = 0;
		}
	}
	else
	{
		if(f32IGridDiffMax < 1.5)
		{
			s_u16CntExit++;
			if(s_u16CntExit >= 250)
			{
				s_u16CntExit = 0;
				g_SysFaultMessage.bit.IacRmsUnbalance = 0;
			}
		}
		else
		{
			s_u16CntExit = 0;
		}
	}
}

/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void GFCICheck(void)
{
	if((1 == g_SaftyConfig.bit.GFCIEnable)&&(1 == g_StateCheck.bit.SafetySetOK))//g_SaftyConfig.Word.GFCICon
	{
		GFCIProtectCheck();
	}
	else
	{
		g_StateCheck.bit.GFCI50mACheckOK = 1;
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void GFCIProtectCheck(void)
{
	static Uint16 s_u16GFCICheckCnt = 0;

	if(1 == g_StateCheck.bit.ADOffsetCalibrateOver)
	{
		if(s_u16GFCICheckCnt <100)
		{
			s_u16GFCICheckCnt ++ ;

			if(s_u16GFCICheckCnt <= 35)
			{
				GFCI_50mA_ON;
			}
			else if(s_u16GFCICheckCnt >= 40)
			{
				GFCI50mASelfCheck();
			}
		}
		else
		{
			s_u16GFCICheckCnt = 100;
			GFCI_50mA_OFF;
			GFCIProtect();
			g_StateCheck.bit.GFCI50mACheckOK = 1;
		}
	}
	else
	{
		s_u16GFCICheckCnt = 0;
	}

}		
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void GFCISelfCheck(void)
{
    static Uint16 s_u16GFCISelfExitCnt = 0;

	if(1 == g_SaftyConfig.bit.GFCIEnable)
	{		
		if(0 == g_SysFaultMessage.bit.GFCIDeviceFault)
		{
		    if((g_ADConvert.offset.f32GFCI > 50)||(g_ADConvert.offset.f32GFCI < -50))
		    {
	            g_SysFaultMessage.bit.GFCIDeviceFault = 1;
		    }
		}
		else
		{
			s_u16GFCISelfExitCnt++;
			if(s_u16GFCISelfExitCnt >= 250)
			{
				s_u16GFCISelfExitCnt = 0;
				g_SysFaultMessage.bit.GFCIDeviceFault = 0;
			}
		}
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void GFCI50mASelfCheck(void)
{
    static Uint16  s_u16GFCI50mACnt = 0;

    if((g_CalcResult.Rms.f32GFCI > 80) || (g_CalcResult.Rms.f32GFCI < 20))
    {
        s_u16GFCI50mACnt++;
        if(s_u16GFCI50mACnt > 5)
        {
            g_SysFaultMessage.bit.GFCIDeviceFault = 1;
            s_u16GFCI50mACnt = 0;
        }
    }
    else
    {      
        s_u16GFCI50mACnt = 0;
    }
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void GFCIProtect(void)
{
	float32 f32DeltaGFCIRms = 0;
	float32 f32DiffGFCIRms = 0;
	float32 f32DiffGFCIAve = 0;

	static float32 s_f32GFCIRmsBuf[3] = {0,0,0};	
	static float32 s_f32GFCIAveBuf[3] = {0,0,0};

	static float32 s_f32GFCIBeforeJumpAve = 0;
	static float32 s_f32GFCIBeforeJumpRms = 0;

	static Uint16 s_u16GFCINormalDelayCnt = 0;
	static Uint16 s_u16LittleJumpFlag = 0;//小值跳变标志
	static Uint16 s_u16LittleJumpRecordFlag = 0;
	static Uint16 s_u16GFCIJumpCnt = 0;
	static Uint16 s_u16FaultBackCnt = 0;

	static Uint16 s_u16GFCI30mAFaultCnt = 0;
	static Uint16 s_u16GFCI60mAFaultCnt = 0;
	static Uint16 s_u16GFCI300mAFaultCnt = 0;
	 
	s_f32GFCIRmsBuf[0] = s_f32GFCIRmsBuf[1]; 
	s_f32GFCIRmsBuf[1] = s_f32GFCIRmsBuf[2]; 
	s_f32GFCIRmsBuf[2] = g_CalcResult.Rms.f32GFCI;

	s_f32GFCIAveBuf[0] = s_f32GFCIAveBuf[1];
	s_f32GFCIAveBuf[1] = s_f32GFCIAveBuf[2];
	s_f32GFCIAveBuf[2] = g_CalcResult.Ave.f32GFCI;

	f32DeltaGFCIRms = FABS(s_f32GFCIRmsBuf[2] - s_f32GFCIRmsBuf[0]); 

	if(NormalState == g_SysCurrentState)
	{
		s_u16GFCINormalDelayCnt ++;
	}
	else
	{
		s_u16GFCINormalDelayCnt = 0;
	}

	if(s_u16GFCINormalDelayCnt >= 150)  //  3s 连续值检测
	{
		s_u16GFCINormalDelayCnt = 150;

		if(g_CalcResult.Rms.f32GFCI > 250)//g_Safety.GFCI.f32GFCI300mALimit)     //250mA
		{
			s_u16GFCI300mAFaultCnt++;
			if(s_u16GFCI300mAFaultCnt > 11)//g_SafetyReg.f32GFCI300mAProtectionTime) 220ms
			{
				g_SysFaultMessage.bit.GFCIFault = 1;  
				s_u16GFCI300mAFaultCnt = 0;
			}
		}
		else
		{
			s_u16GFCI300mAFaultCnt = 0;
		}  

		if(0 == s_u16LittleJumpFlag && f32DeltaGFCIRms > 0 ) //>1)   突变值检测  无突变
		{
			f32DiffGFCIRms = sqrt(FABS(s_f32GFCIRmsBuf[2] * s_f32GFCIRmsBuf[2] - s_f32GFCIRmsBuf[0] * s_f32GFCIRmsBuf[0]));				
			f32DiffGFCIAve = FABS(s_f32GFCIAveBuf[2] - s_f32GFCIAveBuf[0]);
		}
		else if(1 == s_u16LittleJumpFlag)  //小值突变
		{
			f32DiffGFCIRms = sqrt(FABS(s_f32GFCIRmsBuf[2] * s_f32GFCIRmsBuf[2] - s_f32GFCIBeforeJumpRms * s_f32GFCIBeforeJumpRms));				 
			f32DiffGFCIAve = FABS(s_f32GFCIAveBuf[2] - s_f32GFCIBeforeJumpAve);	
		}

		if((f32DiffGFCIRms >= 150)||(f32DiffGFCIAve >= 100))//g_Safety.GFCI.f32GFCI150mALimit) //150mA 0.04s
		{
			s_u16GFCI30mAFaultCnt = 0;
			s_u16GFCI60mAFaultCnt = 0;		
			s_u16LittleJumpFlag = 0;
			s_u16LittleJumpRecordFlag= 0;
			s_f32GFCIBeforeJumpRms = 0;		
			s_f32GFCIBeforeJumpAve = 0;	 
			g_SysFaultMessage.bit.GFCIFault = 1;	
		}
		else if(f32DiffGFCIAve >= 45)//g_Safety.GFCI.f32GFCI60mALimit)   //60mA   0.15s    
		{
			s_u16GFCI30mAFaultCnt = 0;
			s_u16LittleJumpFlag = 1;
			s_u16GFCI60mAFaultCnt++;	
			if(0 == (s_u16LittleJumpRecordFlag&0x02)) 	
			{
				s_f32GFCIBeforeJumpRms = s_f32GFCIRmsBuf[0];  
				s_f32GFCIBeforeJumpAve = s_f32GFCIAveBuf[0];
				s_u16LittleJumpRecordFlag = 2;			  
			}
			if(s_u16GFCI60mAFaultCnt >= 5)//g_Safety.GFCI.f32GFCI60mAProtectionTime)   //150ms(110ms)
			{			
				s_u16GFCI60mAFaultCnt = 0;
				s_u16LittleJumpFlag = 0;
				s_u16LittleJumpRecordFlag= 0;
				s_f32GFCIBeforeJumpRms = 0;	
				s_f32GFCIBeforeJumpAve = 0;		
				g_SysFaultMessage.bit.GFCIFault = 1;		
			}				
		}
		else if(f32DiffGFCIAve >= 25)//g_Safety.GFCI.f32GFCI30mALimit)   // 30mA  0.3s
		{
			s_u16GFCI30mAFaultCnt++;
			s_u16GFCI60mAFaultCnt = 0;	
			s_u16LittleJumpFlag = 1;
			if(0 == (s_u16LittleJumpRecordFlag&0x01)) 	
			{
				s_f32GFCIBeforeJumpRms = s_f32GFCIRmsBuf[0];  
				s_f32GFCIBeforeJumpAve = s_f32GFCIAveBuf[0];
				s_u16LittleJumpRecordFlag = 1;			  
			}					   
			if(s_u16GFCI30mAFaultCnt >= 10)//g_Safety.GFCI.f32GFCI30mAProtectionTime)          // 300ms(160ms)
			{
				s_u16GFCI30mAFaultCnt = 0;
				s_f32GFCIBeforeJumpRms = 0;	
				s_f32GFCIBeforeJumpAve = 0;
				s_u16LittleJumpRecordFlag= 0;	  	    	
				g_SysFaultMessage.bit.GFCIFault = 1;		
				s_u16LittleJumpFlag = 0;
			}		  
		}
		else
		{ 
			s_u16GFCI30mAFaultCnt = 0;
			s_u16GFCI60mAFaultCnt = 0;	
			s_u16GFCIJumpCnt++;
			if(s_u16GFCIJumpCnt > 25) 
			{
				s_u16GFCIJumpCnt = 0;
				s_u16LittleJumpFlag = 0;  
				s_u16LittleJumpRecordFlag = 0;			 
				s_f32GFCIBeforeJumpRms = 0;	
				s_f32GFCIBeforeJumpAve = 0;		 
			}  
		}

	}

	if(1 == g_SysFaultMessage.bit.GFCIFault)
	{
		if(g_CalcResult.Rms.f32GFCI < 30)
		{
			s_u16FaultBackCnt++;
			if(s_u16FaultBackCnt > 275)
			{
				g_SysFaultMessage.bit.GFCIFault = 0;
			}
		} 
	}
	else
	{
		s_u16FaultBackCnt = 0;
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void DCICheck(void)
{
	static Uint16 s_u16CntExit = 0;
	if(NormalState == g_SysCurrentState)
	{
		if(1 == g_SaftyConfig.bit.DCI2Enable)//
		{
			DCI_Over2Check();
		}
		
		if(1 == g_SaftyConfig.bit.DCI1Enable)
		{
			DCI_Over1Check();
		}
	}

	if(1 == g_SysFaultMessage.bit.DciOCP)
	{
		s_u16CntExit++;
		if(s_u16CntExit >= 250)
		{
			s_u16CntExit = 0;
			g_SysFaultMessage.bit.DciOCP = 0;
		}
	}
	else
	{
		s_u16CntExit = 0;
	}
}

/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void DCI_Over1Check(void)
{
	static Uint16 s_u16CntEnter = 0;
	float32 f32DCIAveMax;

	f32DCIAveMax = MAX(FABS(g_CalcResult.Ave.f32DciR), FABS(g_CalcResult.Ave.f32DciS));
	f32DCIAveMax = MAX(FABS(g_CalcResult.Ave.f32DciT), f32DCIAveMax); 
	
	if(0 == g_SysFaultMessage.bit.DciOCP)
	{
		if(f32DCIAveMax > g_SafetyDCI.f32DciMax1Limit)
		{
			s_u16CntEnter++;
			if(s_u16CntEnter >= g_SafetyDCI.u16DciMax1ProtectTime)
			{
				s_u16CntEnter = 0;
				g_SysFaultMessage.bit.DciOCP = 1;
			}
		}
		else
		{
			s_u16CntEnter = 0;
		}
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void DCI_Over2Check(void)
{
	static Uint16 s_u16CntEnter = 0;
	float32 f32DCIAveMax;

	f32DCIAveMax = MAX(FABS(g_CalcResult.Ave.f32DciR), FABS(g_CalcResult.Ave.f32DciS));
	f32DCIAveMax = MAX(FABS(g_CalcResult.Ave.f32DciT), f32DCIAveMax); 

	if(0 == g_SysFaultMessage.bit.DciOCP)
	{
		if(f32DCIAveMax > g_SafetyDCI.f32DciMax2Limit)
		{
			s_u16CntEnter++;
			if(s_u16CntEnter >= g_SafetyDCI.u16DciMax2ProtectTime)
			{
				s_u16CntEnter = 0;
				g_SysFaultMessage.bit.DciOCP = 1;
			}
		}
		else
		{
			s_u16CntEnter = 0;
		}
	}
}
			
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void VPVCheck(void)
{
	if(1 == g_SysFaultEnable.bit.PvOVP)
	{
		VPVOverLimitCheck();
	}
	else
	{
		g_SysFaultMessage.bit.PvOVP = 0;
	}

//	VPVUnderLimitCheck();

	if(1 == g_SysFaultEnable.bit.PvConfigSetWrong)
	{
		VPVConfigCheck();
	}
	else
	{
		g_SysFaultMessage.bit.PvConfigSetWrong = 0;
	}
	
	VPV1OnOffCheck();
	VPV2OnOffCheck();
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void VPVOverLimitCheck(void)
{
	float32 f32VpvMax;
	static Uint16 s_u16CntEnter = 0;
	static Uint16 s_u16CntExit = 0;

	f32VpvMax = MAX(g_CalcResult.Ave.f32VPV1, g_CalcResult.Ave.f32VPV2);
	if(0 == g_SysFaultMessage.bit.PvOVP)
	{
		if(f32VpvMax > PV_OVER_VOLT_LIMIT)
		{
			s_u16CntEnter++;
			if(s_u16CntEnter >= 3)
			{
				s_u16CntEnter = 0;
				g_SysFaultMessage.bit.PvOVP = 1;
			}
		}
		else
		{
			s_u16CntEnter = 0;
		}
	}
	else
	{
		if(f32VpvMax < PV_OVER_VOLT_LIMIT-5)
		{
			s_u16CntExit++;
			if(s_u16CntExit >= 250)
			{
				s_u16CntExit = 0;
				g_SysFaultMessage.bit.PvOVP = 0;
			}
		}
		else
		{
			s_u16CntExit = 0;
		}
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
* TBD: PV配置错误的报错必须由用户更改PV配置后才能解除
**********************************************************************/
void VPVConfigCheck(void)
{
	float32 f32VpvDiff;
	static Uint16 s_u16CntEnter = 0;
	static Uint16 s_u16CntExit = 0;

	if((INDEPENDENT_IN == g_MPPTState.bit.InputMode)||(0 == g_StateCheck.bit.SafetySetOK))	// 如果是独立输入，则PV电压相差过大不会引起unrecoverPvConfigSet_Wrong
	{
		g_SysFaultMessage.bit.PvConfigSetWrong = 0;
		return;
	}

	f32VpvDiff = FABS(g_CalcResult.Ave.f32VPV1 - g_CalcResult.Ave.f32VPV2);
	if(0 == g_SysFaultMessage.bit.PvConfigSetWrong)
	{
		if(f32VpvDiff > 20)
		{
			s_u16CntEnter++;
			if(s_u16CntEnter >= 300)
			{
				s_u16CntEnter = 0;
				g_SysFaultMessage.bit.PvConfigSetWrong = 1;
			}
		}
		else
		{
			s_u16CntEnter = 0;
		}
	}
	else
	{
		if(f32VpvDiff < 20)
		{
			s_u16CntExit++;
			if(s_u16CntExit >= 300)
			{
				s_u16CntExit = 0;
				g_SysFaultMessage.bit.PvConfigSetWrong = 0;
			}
		}
		else
		{
			s_u16CntExit = 0;
		}
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void VPV1OnOffCheck(void)
{
	static Uint16 s_u16CntEnter = 0;
	static Uint16 s_u16CntExit = 0;
	int16	TempPVON;
	if(NormalState == g_SysCurrentState)
	{
		TempPVON = PV_UNDER_VOLT_LIMIT;
	}
	else
	{
		TempPVON = PV_UNDER_VOLT_LIMIT + 100;
	}
		
	if (1 == g_MPPTState.bit.PV1ON)
	{
//		if(g_CalcResult.Ave.f32VPV1 < 200)
//		{
//			g_MPPTState.bit.PV1ON = 0;
//		}
//		else 
		if(g_CalcResult.Ave.f32VPV1 < TempPVON)
		{
			s_u16CntEnter++;
			if (s_u16CntEnter >= 250)	// 5S
			{
				s_u16CntEnter = 0;
				g_MPPTState.bit.PV1ON = 0;
			}
		}
		else
		{
			s_u16CntEnter = 0;
		}
	}
	else
	{
		if (g_CalcResult.Ave.f32VPV1 >= PV_UNDER_VOLT_LIMIT+100)
		{
			s_u16CntExit++;
			if (s_u16CntExit >= 250)	// 5S
			{
				s_u16CntExit = 0;
				g_MPPTState.bit.PV1ON = 1;
				g_MPPTState.bit.InitFlag1 = 1;

				g_Mppt1Calc.f32PvOpenVolt = g_CalcResult.Ave.f32VPV1;
				g_Mppt1Calc.f32PvVoltRef = g_Mppt1Calc.f32PvOpenVolt * 0.99;
				//COM
//				g_MpptCalc.f32PvOpenVolt = g_CalcResult.Ave.f32VPV1;
//				g_MpptCalc.f32PvVoltRef = g_MpptCalc.f32PvOpenVolt * 0.99;
			}
		}
		else
		{
			s_u16CntExit = 0;
		}
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void VPV2OnOffCheck(void)
{
	static Uint16 s_u16CntEnter = 0;
	static Uint16 s_u16CntExit = 0;
	int16	TempPVON;
	if(NormalState == g_SysCurrentState)
	{
		TempPVON = PV_UNDER_VOLT_LIMIT;
	}
	else
	{
		TempPVON = PV_UNDER_VOLT_LIMIT + 100;
	}	
	if (1 == g_MPPTState.bit.PV2ON)
	{
//		if(g_CalcResult.Ave.f32VPV2 < 200)
//		{
//			g_MPPTState.bit.PV2ON = 0;
//		}
//		else 
		if(g_CalcResult.Ave.f32VPV2 < TempPVON)
		{
			s_u16CntEnter++;
			if (s_u16CntEnter >= 250)	// 5s
			{
				s_u16CntEnter = 0;
				g_MPPTState.bit.PV2ON = 0;
			}
		}
		else
		{
			s_u16CntEnter = 0;
		}
	}
	else
	{
		if (g_CalcResult.Ave.f32VPV2 >= PV_UNDER_VOLT_LIMIT+100)
		{
			s_u16CntExit++;
			if (s_u16CntExit >= 250)	// 5s
			{
				s_u16CntExit = 0;
				g_MPPTState.bit.PV2ON = 1;
				g_MPPTState.bit.InitFlag2 = 1;
				g_Mppt2Calc.f32PvOpenVolt = g_CalcResult.Ave.f32VPV2;
				g_Mppt2Calc.f32PvVoltRef = g_Mppt2Calc.f32PvOpenVolt * 0.99;
			}
		}
		else
		{
			s_u16CntExit = 0;
		}
	}
}

/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void IPVCheck(void)
{
	float32 f32IpvDiff;

	if(1 == g_SysFaultEnable.bit.IpvUnbalance)
	{
		f32IpvDiff = FABS(g_CalcResult.Ave.f32IPV1-g_CalcResult.Ave.f32IPV2);
		IPVUnbalanceCheck(f32IpvDiff);
	}
	else
	{
		g_SysFaultMessage.bit.IpvUnbalance = 0;
	}
}

/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void IPVUnbalanceCheck(float32 f32IpvDiff)
{
	static Uint16 s_u16CntEnter = 0;
	static Uint16 s_u16CntExit = 0;
	static Uint16 s_u16UnrecoverCnt = 0;
	//factory mode , possible only one boost work. 
	//If the every boost power is limited when parralel, is the check necessary?

	if((1 == g_MPPTState.bit.FactoryMode)||(INDEPENDENT_IN == g_MPPTState.bit.InputMode))	// TBD 如果是独立输入，则PV电流相差过大不会引起IPV_Unbalance
	{
		g_SysFaultMessage.bit.IpvUnbalance = 0;
		return;
	}

	if(0 == g_SysFaultMessage.bit.IpvUnbalance)
	{
		if(f32IpvDiff > 6)
		{
			s_u16CntEnter++;
			if(s_u16CntEnter >= 30)
			{
				s_u16CntEnter = 0;
				g_SysFaultMessage.bit.IpvUnbalance = 1;
				s_u16UnrecoverCnt++;
				if(s_u16UnrecoverCnt >= 5)
				{
					s_u16UnrecoverCnt = 0;
					g_SysFaultMessage.bit.unrecoverIpvUnbalance = 1;
				}
			}
		}
		else
		{
			s_u16CntEnter = 0;
		}
	}
	else
	{
		if(f32IpvDiff <= 2)
		{
			s_u16CntExit++;
			if(s_u16CntExit >= 250)
			{
				s_u16CntExit = 0;
				g_SysFaultMessage.bit.IpvUnbalance = 0;
			}
		}
		else
		{
			s_u16CntExit = 0;
		}
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void VBUSCheck(void)
{
	static Uint16 s_u16CntExit = 0;

	if(1 == g_SysFaultEnable.bit.VbusUnbalance)
	{
		BusUnbalanceCheck();
	}
	else
	{
		g_SysFaultMessage.bit.VbusUnbalance = 0;
	}


	if(1 == g_SysFaultEnable.bit.BusUVP)
	{
		if(   (CheckState == g_SysCurrentState && 1 == g_StateCheck.bit.BusOverRef)
		   || NormalState == g_SysCurrentState	)
		{
			BusUnderCheck();
			s_u16CntExit = 0;
		}
		else
		{
			if(1 == g_StateCheck.bit.BusUVP)
			{
				s_u16CntExit++;
				if(s_u16CntExit >= 250)
				{
					s_u16CntExit = 0;
					g_StateCheck.bit.BusUVP = 0;
				}
			}
		}
	}
	else
	{
		g_StateCheck.bit.BusUVP = 0;
	}

	if(1 == g_SysFaultEnable.bit.BusOVP)
	{
		BusOverCheck();
	}
	else
	{
		g_SysFaultMessage.bit.BusOVP = 0;
	}
}

/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void BusUnbalanceCheck(void)
{
	static Uint16 s_u16CntEnter = 0;
	static Uint16 s_u16UnrecoverCntEnter = 0;
	static Uint16 s_u16CntExit = 0;
	float32 f32VbusDiff;

	f32VbusDiff = FABS(g_CalcResult.Ave.f32VbusP-g_CalcResult.Ave.f32VbusN);
	
	if(0 == g_SysFaultMessage.bit.VbusUnbalance)
	{
		if(f32VbusDiff > BUS_UNBALANCE_LIMIT)
		{
			s_u16CntEnter++;
			if(s_u16CntEnter >= 10)
			{
				s_u16CntEnter = 0;
				g_SysFaultMessage.bit.VbusUnbalance = 1;
				s_u16UnrecoverCntEnter++;
				if(s_u16UnrecoverCntEnter >= 5)
				{
					s_u16UnrecoverCntEnter = 0;
					g_SysFaultMessage.bit.unrecoverVbusUnbalance = 1;
				}
			}
		}
		else
		{
			s_u16CntEnter = 0;
		}
	}
	else
	{
		if(f32VbusDiff <= BUS_UNBALANCE_LIMIT-30)
		{
			s_u16CntExit++;
			if(s_u16CntExit >= 250)
			{
				s_u16CntExit = 0;
				g_SysFaultMessage.bit.VbusUnbalance = 0;
			}
		}
		else
		{
			s_u16CntExit = 0;
		}
	}
}

/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void BusVoltZeroCheck(void)
{
	if(g_CalcResult.Ave.f32Vbus < 150)
	{
		g_SysFaultMessage.bit.BusVoltZeroFault = 1;
	}
	else if (g_CalcResult.Ave.f32Vbus > 240)
	{
		g_SysFaultMessage.bit.BusVoltZeroFault = 0;
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 最好取消报错
**********************************************************************/
void BusUnderCheck(void)
{
	static Uint16 s_u16CntEnter = 0;
//	static Uint16 s_u16CntExit = 0;

	if(0 == g_StateCheck.bit.BusUVP)
	{
		if(g_CalcResult.Ave.f32Vbus < g_BusRefCal.f32GridPeakVolt + 6)
		{
			s_u16CntEnter++;
			if(s_u16CntEnter >= 200)
			{
				s_u16CntEnter = 0;
				g_StateCheck.bit.BusUVP = 1;
				g_u16SafetyCountDown = 300;
			}
		}
		else
		{
			s_u16CntEnter = 0;
		}
	}

}

/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void BusOverCheck(void)
{
	static Uint16 s_u16CntEnter = 0;
	static Uint16 s_u16CntExit = 0;
	float32 f32VpvMax;

	if(0 == g_SysFaultMessage.bit.BusOVP)
	{
		f32VpvMax = MAX(g_CalcResult.Ave.f32VPV1, g_CalcResult.Ave.f32VPV2);
		if(g_CalcResult.Ave.f32Vbus > BUS_OVER_VOLT_LIMIT && g_CalcResult.Ave.f32Vbus > f32VpvMax+20)
		{
			s_u16CntEnter++;
			if(s_u16CntEnter >= 5)
			{
				s_u16CntEnter = 0;
				g_SysFaultMessage.bit.BusOVP = 1;
			}
		}
		else
		{
			s_u16CntEnter = 0;
		}
	}
	else
	{
		if(g_CalcResult.Ave.f32Vbus < BUS_OVER_VOLT_LIMIT-3)
		{
			s_u16CntExit++;
			if(s_u16CntExit >= 250)
			{
				s_u16CntExit = 0;
				g_SysFaultMessage.bit.BusOVP = 0;
			}
		}
		else
		{
			s_u16CntExit = 0;
		}
	}
}

/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void SWOCPRecover(void)
{
	static Uint16 s_u16CntExit = 0;
	if(1 == g_SysFaultMessage.bit.SwOCPInstant)
	{
		s_u16CntExit++;
		if(s_u16CntExit >= 250)
		{
			s_u16CntExit = 0;
			g_SysFaultMessage.bit.SwOCPInstant = 0;
		}
	}
	else
	{
		s_u16CntExit = 0;
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void BusOverCheck_Instant(float32 f32VbusP, float32 f32VbusN)
{
	float32 VbusPNMax;
//	Uint16 u16OldState_BusVoltOverFault;

	// g_SysFaultMessage.bit.BusVoltOverFault也可能在电网周期执行腂usOverCheck中被置位或清零。

	if(1 == g_SysFaultEnable.bit.BusOVP)
	{
		if(0 == g_SysFaultMessage.bit.BusOVP)
		{
			VbusPNMax = MAX(f32VbusP, f32VbusN);

			if(           VbusPNMax > BUS_SINGLE_INSTANT_OVER_VOLT_LIMIT
			   || f32VbusP+f32VbusN > BUS_INSTANT_OVER_VOLT_LIMIT		)
			{
				g_SysFaultMessage.bit.BusOVP = 1;
			}
		}
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/

void PhaseSeqCheck(void)
{
	static Uint16 s_SeqFaultCnt = 0;
	static Uint16 s_SeqCheckCnt = 0;

//	static Uint16 s_NegCnt = 0;
//	static Uint16 s_PosCnt = 0;
//	static Uint16 s_ZeroCrossFlag = 0;

	if(0 == g_StateCheck.bit.PhaseSeqCheckOver)
	{
/*		if(g_Instantaneous.f32VGridR > 0)
		{
			if(0 == s_ZeroCrossFlag)
			{
				s_PosCnt++;
				if(s_PosCnt>8)
				{
					if(8 == s_NegCnt)
					{
						s_ZeroCrossFlag = 1;
					}
					s_PosCnt = 8;
					s_NegCnt = 0;
				}
			}
			else
			{
				s_NegCnt = 0;
			}			
		}
		else
		{
			s_PosCnt = 0;
			s_NegCnt++;
			if(s_NegCnt>8)
			{
				s_NegCnt = 8;
			}
		}
		if(1 == s_ZeroCrossFlag)			
*/
		if(1 == g_StateCheck.bit.PLLOver)
		{
//			s_ZeroCrossFlag = 0;
			if((g_CalcResult.Rms.f32VGridR > 120)&&(g_CalcResult.Rms.f32VGridS > 120)&&(g_CalcResult.Rms.f32VGridT > 120))			
			{
				s_SeqCheckCnt++;

				if(g_PLLCal.f32Vd_Pos < 120)
				{
					s_SeqFaultCnt++;
					if(s_SeqFaultCnt > 25)
					{
						g_SysFaultMessage.bit.PhaseSequenceFault = 1;
						s_SeqFaultCnt = 0;
					}
				}
				else
				{
					s_SeqFaultCnt = 0;
				}
				
				if(s_SeqCheckCnt > 50)
				{
					g_StateCheck.bit.PhaseSeqCheckOver = 1;
					s_SeqCheckCnt = 0;
				}
			}
			else
			{
				s_SeqFaultCnt = 0;
				s_SeqCheckCnt = 0;
			}
		}
	}
	else
	{
		s_SeqFaultCnt = 0;
		s_SeqCheckCnt = 0;
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void DigitalIOCheck(void)
{
	if(1 == g_SysFaultEnable.bit.HwAcOCP)
	{
		DIO_AcOCPCheck();
	}
	else
	{
		g_SysFaultMessage.bit.HwAcOCP = 0;
	}

//	if(1 == g_SysFaultEnable.bit.unrecoverHwAcOCP)
//	{
//		DIO_unrecoverAcOCPCheck();
//	}
//	else
//	{
//		g_SysFaultMessage.bit.unrecoverHwAcOCP = 0;
//	}

	if(1 == g_SysFaultEnable.bit.HwBusOVP)
	{
		DIO_BusOVPCheck();
	}

	if(1 == g_SysFaultEnable.bit.HwBoostOCP)
	{
		DIO_BoostOCPCheck();
	}
	else
	{
		g_SysFaultMessage.bit.HwBoostOCP = 0;
	}

	if(1 == g_SysFaultEnable.bit.HwAuxPowerFault)
	{
		DIO_AuxPowerCheck();
	}
	else
	{
		g_SysFaultMessage.bit.HwAuxPowerFault = 0;
	}

	if((1 == g_SysFaultEnable.bit.HwRChipFault)&&(1 == g_StateCheck.bit.SafetySetOK))
	{
		DIO_RChipWDCheck();
	}
	else
	{
		g_SysFaultMessage.bit.HwRChipFault = 0;
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void DIO_AcOCPCheck(void)
{
	static Uint16 s_u16CntExit = 0;

	if(1 == g_SysFaultMessage.bit.HwAcOCP)
	{
		if(1 == AC_OCP_LEVEL)
		{
			s_u16CntExit++;
			if(s_u16CntExit >= 2500)
			{
				s_u16CntExit = 0;
				g_SysFaultMessage.bit.HwAcOCP = 0;
			}
		}
		else
		{
			s_u16CntExit = 0;
		}
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
// TBD:需要根据17K HwBusOVPCheck()进行修改
void DIO_BusOVPCheck(void)
{
	static Uint16 s_u16CntEnter = 0;
	static Uint16 s_u16CntExit = 0;

	if(0 == g_StateCheck.bit.HwBusOVP)
	{
		if(0 == BUS_OVP_LEVEL)
		{
			s_u16CntEnter++;
			if(s_u16CntEnter >= 1)	// 只要过压就报错，无持续时间判断
			{
				s_u16CntEnter = 0;
				g_StateCheck.bit.HwBusOVP = 1;
				// TBD 停止发Boost PWM
			}
		}
		else
		{
			s_u16CntEnter = 0;
		}
	}
	else
	{	
		if(1 == BUS_OVP_LEVEL)
		{
			s_u16CntExit++;
			if(s_u16CntExit >= 2500)
			{
				s_u16CntExit = 0;
				g_StateCheck.bit.HwBusOVP = 0;
			}
		}
		else
		{
			s_u16CntExit = 0;
		}
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void DIO_BoostOCPCheck(void)
{
	static Uint16 s_u16CntEnter = 0;
	static Uint16 s_u16CntExit = 0;

	if(0 == g_SysFaultMessage.bit.HwBoostOCP)
	{
		if(0==PV1_OCP_LEVEL || 0==PV2_OCP_LEVEL)
		{
			s_u16CntEnter++;
			if(s_u16CntEnter >= 1)	// 只要过流就报错，无持续时间判断
			{
				s_u16CntEnter = 0;
				g_SysFaultMessage.bit.HwBoostOCP = 1;
			}
		}
		else
		{
			s_u16CntEnter = 0;
		}
	}
	else
	{
		if(1==PV1_OCP_LEVEL && 1==PV2_OCP_LEVEL)
		{
			s_u16CntExit++;
			if(s_u16CntExit >= 2500)
			{
				s_u16CntExit = 0;
				g_SysFaultMessage.bit.HwBoostOCP = 0;
			}
		}
		else
		{
			s_u16CntExit = 0;
		}
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void DIO_AuxPowerCheck(void)
{
	static Uint16 s_u16CntEnter = 0;
	static Uint16 s_u16CntExit = 0;

	if(0 == g_SysFaultMessage.bit.HwAuxPowerFault)
	{
		if(0 == AUX_OK_LEVEL)
		{
			s_u16CntEnter++;
			if(s_u16CntEnter >= 3)
			{
				s_u16CntEnter = 0;
				g_SysFaultMessage.bit.HwAuxPowerFault = 1;
			}
		}
		else
		{
			s_u16CntEnter = 0;
		}
	}
	else
	{
		if(1 == AUX_OK_LEVEL)
		{
			s_u16CntExit++;
			if(s_u16CntExit >= 2500)
			{
				s_u16CntExit = 0;
				g_SysFaultMessage.bit.HwAuxPowerFault = 0;
			}
		}
		else
		{
			s_u16CntExit = 0;
		}
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void DIO_RChipWDCheck(void)
{
	static Uint16 s_u16CntEnter = 0;
	static Uint16 s_u16CntExit = 0;

	if(0 == g_SysFaultMessage.bit.HwRChipFault)
	{
		if(0 == RCHIP_WD_LEVEL)
		{
			s_u16CntEnter++;
			if(s_u16CntEnter >= 10)
			{
				s_u16CntEnter = 0;
				g_SysFaultMessage.bit.HwRChipFault = 1;
			}
		}
		else
		{
			s_u16CntEnter = 0;
		}
	}
	else
	{
		if(1 == RCHIP_WD_LEVEL)
		{
			s_u16CntExit++;
			if(s_u16CntExit >= 2500)
			{
				s_u16CntExit = 0;
				g_SysFaultMessage.bit.HwRChipFault = 0;
			}
		}
		else
		{
			s_u16CntExit = 0;
		}
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void AntiIslandingCalc(void)   //island 
{
	/**********************************************************************
	HF AI
	**********************************************************************/
	unsigned char i;

	float32 f32Perioddiff = 0;
	float32 f32Perioddiff1 = 0;
	float32 f32AvePeriod = 0;

	static	float32 f32Mov8Period = 0;
	static	float32 f32Mov8PeriodOld = 0;

	static unsigned char bflag = 0;
	static unsigned char Deltflag = 1;
	static unsigned char bgridoff = 0;

	static float32 f32MovPeriod8Buf[8]= {50,50,50,50,50,50,50,50};
	static Uint8 u8Period8index = 0;
	static Uint8 u8islandingFlag = 0;
	float32 f32MovPeriod8Sum = 0;
	static float32 f32ReactivePowerDelta = 0;
    static Uint16 s_u16Cnt_GridFre_Low_Fault = 0;  

	f32MovPeriod8Buf[u8Period8index] = g_CalcResult.Ave.f32FGrid;
	u8Period8index++;

	if(u8Period8index >= 8)
	{
		u8Period8index = 0;
		f32MovPeriod8Sum = 0;

		for(i = 0; i < 8; i++)
		{
			f32MovPeriod8Sum = f32MovPeriod8Sum + f32MovPeriod8Buf[i];
		}
		f32Mov8PeriodOld = f32Mov8Period;
		f32Mov8Period = f32MovPeriod8Sum * 0.125;

		if(f32Mov8PeriodOld >= f32Mov8Period)
		{
			f32Perioddiff1 = (f32Mov8PeriodOld - f32Mov8Period);
		}
		else
		{
			f32Perioddiff1 = (f32Mov8Period - f32Mov8PeriodOld);
		}
          
		if(f32Perioddiff1 > 0.4)
		{
			s_u16Cnt_GridFre_Low_Fault++;
			if(s_u16Cnt_GridFre_Low_Fault >= 5)
            {  
	            if((g_SysCurrentState == NormalState)&&(1 == g_SaftyConfig.bit.AIEnable))
				{					
					g_SysFaultMessage.bit.GridUFP = 1;
					s_u16Cnt_GridFre_Low_Fault = 0;	
				}	
            }     
		}
		else
		{
			s_u16Cnt_GridFre_Low_Fault = 0;
		}

		if(u8islandingFlag == 0)
		{
			if(Deltflag == 1)
			{
				Deltflag = 0;
				f32ReactivePowerDelta = 0.0053;//0.3degree can trigger 0.065Hz jump;
			}
			else
			{
				Deltflag = 1;
				f32ReactivePowerDelta = 0;
			}
		}
		else
		{
			if(Deltflag == 1)
			{
				Deltflag = 0;
				f32ReactivePowerDelta = 0.0872;//5degree can trigger 1Hz jump;
			}
			else
			{
				Deltflag = 1;
				f32ReactivePowerDelta = 0;
			}
		}
	}

	if(u8islandingFlag == 0)//detect weather islanding may occur or not
	{
		bflag++;
		f32AvePeriod = f32Mov8Period;
		if((g_CalcResult.Ave.f32FGrid > (f32AvePeriod + 0.1))||((g_CalcResult.Ave.f32FGrid + 0.1) < f32AvePeriod))//||((u32GridFreqNew + c5us) < u32AvePeriod))
		{
			bgridoff++;
		}
		if(bflag == 8)
		{
			if(bgridoff >= 3)
			{
				u8islandingFlag = 1;
				Deltflag = 1;
			}
			bflag = 0;
			bgridoff = 0;
		}
	}
	else //detect weather islanding may disappear or not
	{
		bflag++;
		if(bflag == 8)
		{
			bflag = 0;
					
			if(f32Mov8PeriodOld >= f32Mov8Period)
			{
				f32Perioddiff = (f32Mov8PeriodOld - f32Mov8Period);
			}
			else
			{
				f32Perioddiff = (f32Mov8Period - f32Mov8PeriodOld);
			}
              
			if(f32Perioddiff < 0.025)
			{
				u8islandingFlag = 0;     
			}
		}
	}			

	if((g_SysCurrentState == NormalState)&&(1 == g_SaftyConfig.bit.AIEnable))
	{
		g_PLLCal.AIDeltaTheta = f32ReactivePowerDelta;
	}
	else
	{
		g_PLLCal.AIDeltaTheta = 0;
	}		 
}

//===========================================================================
// No more.
//===========================================================================

