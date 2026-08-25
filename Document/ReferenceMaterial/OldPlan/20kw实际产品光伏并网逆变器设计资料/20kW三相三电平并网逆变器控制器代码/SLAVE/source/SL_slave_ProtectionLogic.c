/***********************************************************************
 *
 *    PURPOSE  : 
 *    HISTORY  :
 *           DATE          VERSION      AUTHOR        NOTE
 *
 ************************************************************************/
#include "SL_slave.h"
/**********************************************************************
* FUNCION :  ConsistentCheck
* PURPOSE :  
*                   
* INPUT :
*        void
* RETURN :
*        void
* CALLS:
*        void
*
* CALLED BY: 
* 
**********************************************************************/
void ConsistentCheck(void)
{
	if((1 == s_SysFaultMessage_Enable.bit.ConsistentFault_VGrid)&&(0 == s_SysFaultMessage_Master.bit.HWADFault_VGRID))
	{
		ConsistentVGridCheck();
	}
	else
	{
		s_SysFaultMessage.bit.ConsistentFault_VGrid = 0;
	}

	if(1 == s_SysFaultMessage_Enable.bit.ConsistentFault_FGrid)
	{
		ConsistentGridFeqCheck();
	}
	else
	{
		s_SysFaultMessage.bit.ConsistentFault_FGrid = 0;
	}

	if((1 == s_SysFaultMessage_Enable.bit.ConsistentFault_DCI)&&(0 == s_SysFaultMessage_Master.bit.HWADFault_DCI))
	{
 		ConsistentGridDciCheck();
	}
	else
	{
		s_SysFaultMessage.bit.ConsistentFault_DCI = 0;
	}

	if((1 == s_SysFaultMessage_Enable.bit.ConsistentFault_GFCI)&&(0 == s_SysFaultMessage_Master.bit.HWADFault_GFCI))
	{
		ConsistentGFCICheck();
	}
	else
	{
		s_SysFaultMessage.bit.ConsistentFault_GFCI = 0;
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void ConsistentVGridCheck(void)
{
	static Uint8 VGridDeltaFaultFilter = 0;
	static Uint8 VGridDeltaBack = 0;
	int16 Delta_R = 0;
	int16 Delta_S = 0;
	int16 Delta_T = 0;

	if(CalcResultRms.i16VGRID_R > M_CalcResultRms.i16VGRID_R)
	{
		Delta_R =  CalcResultRms.i16VGRID_R - M_CalcResultRms.i16VGRID_R;
	}
	else
	{
		Delta_R = M_CalcResultRms.i16VGRID_R - CalcResultRms.i16VGRID_R;
	}

	if(CalcResultRms.i16VGRID_S > M_CalcResultRms.i16VGRID_S)
	{
		Delta_S =  CalcResultRms.i16VGRID_S - M_CalcResultRms.i16VGRID_S;
	}
	else
	{
		Delta_S = M_CalcResultRms.i16VGRID_S - CalcResultRms.i16VGRID_S;
	}

	if(CalcResultRms.i16VGRID_T > M_CalcResultRms.i16VGRID_T)
	{
		Delta_T =  CalcResultRms.i16VGRID_T - M_CalcResultRms.i16VGRID_T;
	}
	else
	{
		Delta_T = M_CalcResultRms.i16VGRID_T - CalcResultRms.i16VGRID_T;
	}


	if(0 == s_SysFaultMessage.bit.ConsistentFault_VGrid)
	{
		if((Delta_R > Delta15V)||(Delta_S > Delta15V)||(Delta_T > Delta15V))
		{
			VGridDeltaFaultFilter++;
			if(VGridDeltaFaultFilter > 250)
			{
				VGridDeltaFaultFilter = 0;
				s_SysFaultMessage.bit.ConsistentFault_VGrid = 1; 
			}	
		}
		else
		{
			VGridDeltaFaultFilter = 0;
		}	
	}
	else
	{
		VGridDeltaFaultFilter = 0;
		if((Delta_R < Delta15V)&&(Delta_S < Delta15V)&&(Delta_T < Delta15V))
		{
			VGridDeltaBack++;
			if(VGridDeltaBack > 250)
			{
				VGridDeltaBack = 0;
				s_SysFaultMessage.bit.ConsistentFault_VGrid = 0;
			}
		}
		else
		{
			VGridDeltaBack = 0;
		}
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void ConsistentGridFeqCheck(void)
{
	static Uint8 GridFeqDeltaFaultFilter = 0;
	static Uint8 GridFeqDeltaBack = 0;
	Uint16 Delta_T = 0;

	if(CalcResultRms.i16GridTFreq > M_CalcResultRms.i16GridTFreq)
	{
		Delta_T =  CalcResultRms.i16GridTFreq - M_CalcResultRms.i16GridTFreq;
	}
	else
	{
		Delta_T = M_CalcResultRms.i16GridTFreq - CalcResultRms.i16GridTFreq;
	}


	if(0 == s_SysFaultMessage.bit.ConsistentFault_VGrid)
	{
		if(Delta_T > Delta05HZ)
		{
			GridFeqDeltaFaultFilter++;
			if(GridFeqDeltaFaultFilter > 250)
			{
				GridFeqDeltaFaultFilter = 0;
				s_SysFaultMessage.bit.ConsistentFault_FGrid = 1; 
			}	
		}
		else
		{
			GridFeqDeltaFaultFilter = 0;
		}	
	}
	else
	{
		if(Delta_T < Delta05HZ)
		{
			GridFeqDeltaBack++;
			if(GridFeqDeltaBack > 250)
			{
				GridFeqDeltaBack = 0;
				s_SysFaultMessage.bit.ConsistentFault_FGrid = 0;
			}
		}
		else
		{
			GridFeqDeltaBack = 0;
		}
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void ConsistentGridDciCheck(void)
{
	static Uint8 GridDCIDeltaFaultFilter = 0;
	static Uint8 GridDCIDeltaBack = 0;
	Uint16 Delta_R = 0;
	Uint16 Delta_S = 0;
	Uint16 Delta_T = 0;

	if(CalcResultAve.i16DCI_R + 1000 > M_CalcResultRms.i16DCI_R)
	{
		Delta_R =  CalcResultAve.i16DCI_R + 1000 - M_CalcResultRms.i16DCI_R;
	}
	else
	{
		Delta_R = M_CalcResultRms.i16DCI_R - CalcResultAve.i16DCI_R - 1000;
	}

	if(CalcResultAve.i16DCI_S + 1000 > M_CalcResultRms.i16DCI_S)
	{
		Delta_S =  CalcResultAve.i16DCI_S + 1000 - M_CalcResultRms.i16DCI_S;
	}
	else
	{
		Delta_S = M_CalcResultRms.i16DCI_S - CalcResultAve.i16DCI_S - 1000;
	}

	if(CalcResultAve.i16DCI_T + 1000 > M_CalcResultRms.i16DCI_T)
	{
		Delta_T =  CalcResultAve.i16DCI_T + 1000 - M_CalcResultRms.i16DCI_T;
	}
	else
	{
		Delta_T = M_CalcResultRms.i16DCI_T - CalcResultAve.i16DCI_T - 1000;
	}


	if(0 == s_SysFaultMessage.bit.ConsistentFault_DCI)
	{
		if((Delta_R > 500)||(Delta_S > 500)||(Delta_T > 500))
		{
			GridDCIDeltaFaultFilter++;
			if(GridDCIDeltaFaultFilter > 250)
			{
				GridDCIDeltaFaultFilter = 0;
				s_SysFaultMessage.bit.ConsistentFault_DCI = 1; 
			}	
		}
		else
		{
			GridDCIDeltaFaultFilter = 0;
		}	
	}
	else
	{
		if((Delta_R < 300)&&(Delta_S < 300)&&(Delta_T < 300))
		{
			GridDCIDeltaBack++;
			if(GridDCIDeltaBack > 250)
			{
				GridDCIDeltaBack = 0;
				s_SysFaultMessage.bit.ConsistentFault_DCI = 0;
			}
		}
		else
		{
			GridDCIDeltaBack = 0;
		}
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void ConsistentGFCICheck(void)
{
	static Uint8 GFCIDeltaFaultFilter = 0;
	static Uint8 GFCIDCIDeltaBack = 0;
	Uint16 Delta = 0;

	if(CalcResultRms.i16GFCI > M_CalcResultRms.i16GFCI)
	{
		Delta =  CalcResultRms.i16GFCI - M_CalcResultRms.i16GFCI;
	}
	else
	{
		Delta = M_CalcResultRms.i16GFCI - CalcResultRms.i16GFCI;
	}

	if(0 == s_SysFaultMessage.bit.ConsistentFault_GFCI)
	{
		if(Delta > Delta100mA)
		{
			GFCIDeltaFaultFilter++;
			if(GFCIDeltaFaultFilter > 250)
			{
				GFCIDeltaFaultFilter = 0;
				s_SysFaultMessage.bit.ConsistentFault_GFCI = 1; 
			}	
		}
		else
		{
			GFCIDeltaFaultFilter = 0;
		}	
	}
	else
	{
		if(Delta < Delta100mA)
		{
			GFCIDCIDeltaBack++;
			if(GFCIDCIDeltaBack > 250)
			{
				GFCIDCIDeltaBack = 0;
				s_SysFaultMessage.bit.ConsistentFault_GFCI = 0;
			}
		}
		else
		{
			GFCIDCIDeltaBack = 0;
		}
	}
}
			
/**********************************************************************
* FUNCION :  ConsistentVGridCheck
* PURPOSE :  
*                   
* INPUT :
*        void
* RETURN :
*        void
* CALLS:
*        void
*
* CALLED BY: 
* 
**********************************************************************/

void TemperatureCheck(void)
{
	if(1 == s_SysFaultMessage_Enable.bit.OverTempFault_Inv)
	{
		InvTempCheck();
	}
	else
	{
		s_SysFaultMessage.bit.OverTempFault_Inv = 0;
	}

	if(1 == s_SysFaultMessage_Enable.bit.OverTempFault_Boost)
	{    
		BoostTempCheck();
	}
	else
	{
		s_SysFaultMessage.bit.OverTempFault_Boost = 0;
	}

	if(1 == s_SysFaultMessage_Enable.bit.OverTempFault_Env)
	{
		EnvTempCheck();
	}
	else
	{
		s_SysFaultMessage.bit.OverTempFault_Env = 0;
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void InvTempCheck(void)
{
    static  Uint16  s_u16Cnt_INV_OTP_Fault= 0;
    static  Uint16  s_u16Cnt_INV_OTP_FaultBack= 0;
	int16 tempT;
	tempT = ProcessVariable.i16TInvOTP;
	if(0 == s_SysFaultMessage.bit.OverTempFault_Inv)
	{ 
		if ((CalcResultAve.i16TempINVR >= tempT)|| (CalcResultAve.i16TempINVS >= tempT)|| (CalcResultAve.i16TempINVT >= tempT)) 
	    {
	        s_u16Cnt_INV_OTP_Fault++;
	        if(s_u16Cnt_INV_OTP_Fault >= 25)     
	        {
	            s_u16Cnt_INV_OTP_Fault = 0;
	            s_SysFaultMessage.bit.OverTempFault_Inv = 1;
	        }
	    }
	    else
	    {
	        s_u16Cnt_INV_OTP_Fault = 0;
	    }
	}
	else
	{
		if ((CalcResultAve.i16TempINVR < 70)&&(CalcResultAve.i16TempINVS < 70)&&(CalcResultAve.i16TempINVT < 70)) 
	    {
	        s_u16Cnt_INV_OTP_FaultBack++;
	        if(s_u16Cnt_INV_OTP_FaultBack >= 250)    
	        {
	            s_u16Cnt_INV_OTP_FaultBack = 0;
	            s_SysFaultMessage.bit.OverTempFault_Inv = 0;
	        }
	    }
	    else
	    {
	        s_u16Cnt_INV_OTP_FaultBack = 0;
	    }
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void BoostTempCheck(void)
{
    static  Uint16  s_u16Cnt_Boost_OTP_Fault= 0;
    static  Uint16  s_u16Cnt_Boost_OTP_FaultBack= 0;

	if(0 == s_SysFaultMessage.bit.OverTempFault_Boost)
	{    
		if (CalcResultAve.i16TempBoost1 > ProcessVariable.i16TNTCOTP)
	    {
	        s_u16Cnt_Boost_OTP_Fault++;
	        if(s_u16Cnt_Boost_OTP_Fault >= 25)     
	        {
	            s_u16Cnt_Boost_OTP_Fault = 0;
	            s_SysFaultMessage.bit.OverTempFault_Boost = 1;
	        }
	    }
	    else
	    {
	        s_u16Cnt_Boost_OTP_Fault = 0;
	    }
	}
	else
	{
		if(CalcResultAve.i16TempBoost1 < 70)
	    {
	        s_u16Cnt_Boost_OTP_FaultBack++;
	        if(s_u16Cnt_Boost_OTP_FaultBack >= 250)    
	        {
	            s_u16Cnt_Boost_OTP_FaultBack = 0;
	            s_SysFaultMessage.bit.OverTempFault_Boost = 0;
	        }
	    }
	    else
	    {
	        s_u16Cnt_Boost_OTP_FaultBack = 0;
	    }
	}
}

/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void EnvTempCheck(void)
{
    static  Uint16  s_u16Cnt_Env_OTP_Fault = 0;
    static  Uint16  s_u16Cnt_Env_OTP_FaultBack = 0;

	if(0 == s_SysFaultMessage.bit.OverTempFault_Env)
	{    
		if (CalcResultAve.i16TempAmb > ProcessVariable.i16TAmbOTP)
	    {
	        s_u16Cnt_Env_OTP_Fault++;
	        if(s_u16Cnt_Env_OTP_Fault >= 25)     
	        {
	            s_u16Cnt_Env_OTP_Fault = 0;
	            s_SysFaultMessage.bit.OverTempFault_Env = 1;
	        }
	    }
	    else
	    {
	        s_u16Cnt_Env_OTP_Fault = 0;
	    }
	}
	else
	{
		if (CalcResultAve.i16TempAmb < 72)
	    {
	        s_u16Cnt_Env_OTP_FaultBack++;
	        if(s_u16Cnt_Env_OTP_FaultBack >= 250)    
	        {
	            s_u16Cnt_Env_OTP_FaultBack = 0;
	            s_SysFaultMessage.bit.OverTempFault_Env = 0;
	        }
	    }
	    else
	    {
	        s_u16Cnt_Env_OTP_FaultBack = 0;
	    }
	}
}
/**********************************************************************
* FUNCION :  ConsistentVGridCheck
* PURPOSE :  
*                   
* INPUT :
*        void
* RETURN :
*        void
* CALLS:
*        void
*
* CALLED BY: 
* 
**********************************************************************/

void DigitalIOCheck()
{
	if((1 == s_SysFaultMessage_Enable.bit.MChip_Fault)&&(1 == s_StateCheck.bit.HandShakeOver))
	{
		MChipWDDetection();
	}
	else
	{
		s_SysFaultMessage.bit.MChip_Fault = 0;
	}
/*
	if(1 == s_SysFaultMessage_Enable.bit.DcFan1Fault)
	{
		DcFan1SpeedSense();
	}
	else
	{
		s_StateCheck.bit.DcFan1Fault = 0;
	}

	if(1 == s_SysFaultMessage_Enable.bit.DcFan2Fault)
	{
		DcFan2SpeedSense();
	}
	else
	{
		s_StateCheck.bit.DcFan2Fault = 0;
	}*/
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/	
void MChipWDDetection (void)
{   
    static Uint16 s_u16Cnt_MChip_Fault = 0 ; 
    static Uint16 s_u16Cnt_MChip_Fault_Back = 0 ;

	if(0 == s_SysFaultMessage.bit.MChip_Fault)
	{
		if (0 == MChip_WD_LEVEL)
	    {
	        s_u16Cnt_MChip_Fault ++;
	        if (s_u16Cnt_MChip_Fault > 10)	 
	        {
	            s_u16Cnt_MChip_Fault = 0;
	            s_SysFaultMessage.bit.MChip_Fault = 1;	
	        }
	    }
	    else
	    {
	        s_u16Cnt_MChip_Fault = 0;
	    }
	}
	else
	{
		if (1 == MChip_WD_LEVEL)
	    {
	        s_u16Cnt_MChip_Fault_Back ++;
	        if (s_u16Cnt_MChip_Fault_Back > 2500)	 
	        {
	            s_u16Cnt_MChip_Fault_Back = 0;
	            s_SysFaultMessage.bit.MChip_Fault = 0;	
	        }
	    }
	    else
	    {
	        s_u16Cnt_MChip_Fault_Back = 0;
	    }
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
/*
void DcFan1SpeedSense (void)
{
	static Uint16 s_u16Cnt_DcFan1_Hi_Level = 0 ;
	static Uint16  s_u16Cnt_DcFan1_Low_Level = 0;

    if(1 == s_StateCheck.bit.DC_Fan_State)
    {
        if (1 == DC_FAN1_FB_Level)                 // state  1:high lever  ,0:low lever
        {
            s_u16Cnt_DcFan1_Hi_Level++;
	        if (s_u16Cnt_DcFan1_Hi_Level > 1000)
	        {
				s_StateCheck.bit.DcFan1Fault = 1; 
	        }
			else
			{
				s_StateCheck.bit.DcFan1Fault = 0;
			}
            s_u16Cnt_DcFan1_Low_Level = 0;
        }
        else                                                // state  1:high lever  ,0:low lever
        {
            s_u16Cnt_DcFan1_Low_Level++;
	        if (s_u16Cnt_DcFan1_Low_Level > 1000)
	       	{
				s_StateCheck.bit.DcFan1Fault = 1;
	        }
			else
			{
				s_StateCheck.bit.DcFan1Fault = 0;
			}
			s_u16Cnt_DcFan1_Hi_Level = 0;
        } 
 	}
 	else
	{
		s_u16Cnt_DcFan1_Hi_Level = 0 ;
		s_u16Cnt_DcFan1_Low_Level = 0;
	}  
}*/	
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
/*
void DcFan2SpeedSense (void)
{
	static Uint16 s_u16Cnt_DcFan2_Hi_Level = 0;
	static Uint16  s_u16Cnt_DcFan2_Low_Level = 0;

    if(1 == s_StateCheck.bit.DC_Fan_State)
    {
        if (1 == DC_FAN2_FB_Level)   
        {
            s_u16Cnt_DcFan2_Hi_Level++;

            if (s_u16Cnt_DcFan2_Hi_Level > 1000)
            {
				s_StateCheck.bit.DcFan2Fault = 1;
            }
			else
			{
				s_StateCheck.bit.DcFan2Fault = 0;
			}
           	s_u16Cnt_DcFan2_Low_Level = 0;
        }
        else                                 
        {
            s_u16Cnt_DcFan2_Low_Level++;
            if (s_u16Cnt_DcFan2_Low_Level > 1000)
            {
				s_StateCheck.bit.DcFan2Fault = 1;
            }
			else
			{
				s_StateCheck.bit.DcFan2Fault = 0;
			}
			s_u16Cnt_DcFan2_Hi_Level = 0;
        }        
	}
	else
	{
		s_u16Cnt_DcFan2_Hi_Level = 0;
		s_u16Cnt_DcFan2_Low_Level = 0;
     }  
}		
*/	
/**********************************************************************
* FUNCION :  ConsistentVGridCheck
* PURPOSE :  
*                   
* INPUT :
*        void
* RETURN :
*        void
* CALLS:
*        void
*
* CALLED BY: 
* 
**********************************************************************/
void ADOffsetCheck(void)
{
	if(1 == s_SysFaultMessage_Enable.bit.HWADFault_VGRID)
	{
		ADOffsetVGRIDCheck();
	}
	else
	{
		s_SysFaultMessage.bit.HWADFault_VGRID = 0;
		s_StateCheck.bit.HWADFault_VGRID = 0;
	}

	if(1 == s_SysFaultMessage_Enable.bit.HWADFault_VINV)
	{
		ADOffsetVINVCheck();
	}
	else
	{
		s_SysFaultMessage.bit.HWADFault_VINV = 0;
		s_StateCheck.bit.HWADFault_VINV = 0;
	}

	if(1 == s_SysFaultMessage_Enable.bit.HWADFault_DCI)
	{
		ADOffsetDCICheck();
	}
	else
	{
		s_SysFaultMessage.bit.HWADFault_DCI = 0;
		s_StateCheck.bit.HWADFault_DCI = 0;
	}

	if(1 == s_SysFaultMessage_Enable.bit.HWADFault_GFCI)
	{
		ADOffsetGFCICheck();
	}
	else
	{
		s_SysFaultMessage.bit.HWADFault_GFCI = 0;
		s_StateCheck.bit.HWADFault_GFCI = 0;
	}

	if((0 == s_SysFaultMessage.bit.HWADFault_VGRID)&&(0==s_SysFaultMessage.bit.HWADFault_VINV)&&(0 == s_SysFaultMessage.bit.HWADFault_DCI)&&(0 == s_SysFaultMessage.bit.HWADFault_GFCI))
	{
		s_StateCheck.bit.AD_initial = 0;

		ADChannelOffset.i16VGRID_R = ADOffsetSum.i32VGRID_R >> 7;//
		ADChannelOffset.i16VGRID_S = ADOffsetSum.i32VGRID_S >> 7;
		ADChannelOffset.i16VGRID_T = ADOffsetSum.i32VGRID_T >> 7;

		ADChannelOffset.i16VINV_R = ADOffsetSum.i32VINV_R >> 7;
		ADChannelOffset.i16VINV_S = ADOffsetSum.i32VINV_S >> 7;
		ADChannelOffset.i16VINV_T = ADOffsetSum.i32VINV_T >> 7;

		ADChannelOffset.i16DCI_R = ADOffsetSum.i32DCI_R >> 5;
		ADChannelOffset.i16DCI_S = ADOffsetSum.i32DCI_S >> 5;
		ADChannelOffset.i16DCI_T = ADOffsetSum.i32DCI_T >> 5;

		ADChannelOffset.i16GFCI = ADOffsetSum.i32GFCI >> 5;
	}
	else
	{
		//s_StateCheck.bit.AD_initial = 1;
	}
}
/**********************************************************************
* FUNCION :  ConsistentVGridCheck
* PURPOSE :  
*                   
* INPUT :
*        void
* RETURN :
*        void
* CALLS:
*        void
*
* CALLED BY: 
* 
**********************************************************************/
void ADOffsetVGRIDCheck(void)
{
	static int16 Fault_Count_VGRID = 0;

	if((abs(ADOffsetSum.i32VGRID_R >> 5) > AD_Channel_Offset_VGRID_Limit)||(abs(ADOffsetSum.i32VGRID_S >> 5) > AD_Channel_Offset_VGRID_Limit)||(abs(ADOffsetSum.i32VGRID_T >> 5) > AD_Channel_Offset_VGRID_Limit))
	{
		Fault_Count_VGRID++;
		s_StateCheck.bit.HWADFault_VGRID = 1;
		if(Fault_Count_VGRID > 2)
		{
			s_SysFaultMessage.bit.HWADFault_VGRID = 1;
			Fault_Count_VGRID = 0;
		}
	}
	else
	{
		s_StateCheck.bit.HWADFault_VGRID = 0;
		s_SysFaultMessage.bit.HWADFault_VGRID = 0;
		Fault_Count_VGRID = 0;
	}
}
/**********************************************************************
* FUNCION :  ConsistentVGridCheck
* PURPOSE :  
*                   
* INPUT :
*        void
* RETURN :
*        void
* CALLS:
*        void
*
* CALLED BY: 
* 
**********************************************************************/
void ADOffsetVINVCheck(void)
{
	static int16 Fault_Count_VINV = 0;

	if((abs(ADOffsetSum.i32VINV_R >> 5) > AD_Channel_Offset_VINV_Limit)||(abs(ADOffsetSum.i32VINV_S >> 5) > AD_Channel_Offset_VINV_Limit)||(abs(ADOffsetSum.i32VINV_T >> 5) > AD_Channel_Offset_VINV_Limit))
	{
		Fault_Count_VINV++;
		s_StateCheck.bit.HWADFault_VINV = 1;
		if(Fault_Count_VINV > 2)
		{
			s_SysFaultMessage.bit.HWADFault_VINV = 1;
			Fault_Count_VINV = 0;
		}
	}
	else
	{
		s_StateCheck.bit.HWADFault_VINV = 0;
		s_SysFaultMessage.bit.HWADFault_VINV = 0;
		Fault_Count_VINV = 0;
	}
}
/**********************************************************************
* FUNCION :  ConsistentVGridCheck
* PURPOSE :  
*                   
* INPUT :
*        void
* RETURN :
*        void
* CALLS:
*        void
*
* CALLED BY: 
* 
**********************************************************************/
void ADOffsetDCICheck(void)
{
	static int16 Fault_Count_DCI = 0;

	if((abs(ADOffsetSum.i32DCI_R >> 5) > AD_Channel_Offset_DCI_Limit)||(abs(ADOffsetSum.i32DCI_S >> 5) > AD_Channel_Offset_DCI_Limit)||(abs(ADOffsetSum.i32DCI_T >> 5) > AD_Channel_Offset_GFCI_Limit))
	{
		Fault_Count_DCI++;
		s_StateCheck.bit.HWADFault_DCI = 1;
		if(Fault_Count_DCI > 2)
		{
			s_SysFaultMessage.bit.HWADFault_DCI = 1;
		}
	}
	else
	{
		s_SysFaultMessage.bit.HWADFault_DCI = 0;
		s_StateCheck.bit.HWADFault_DCI = 0;
	}
}
/**********************************************************************
* FUNCION :  ConsistentVGridCheck
* PURPOSE :  
*                   
* INPUT :
*        void
* RETURN :
*        void
* CALLS:
*        void
*
* CALLED BY: 
* 
**********************************************************************/
void ADOffsetGFCICheck(void)
{
	static int16 Fault_Count_GFCI = 0;

	if(abs(ADOffsetSum.i32GFCI >> 5) > AD_Channel_Offset_GFCI_Limit)
	{
		Fault_Count_GFCI++;
		s_StateCheck.bit.HWADFault_GFCI = 1;
		if(Fault_Count_GFCI > 2)
		{
			s_SysFaultMessage.bit.HWADFault_GFCI = 1;
		}
	}
	else
	{
		s_SysFaultMessage.bit.HWADFault_GFCI = 0;
		s_StateCheck.bit.HWADFault_GFCI = 0;
	}
}


//===========================================================================
// No more.
//===========================================================================


