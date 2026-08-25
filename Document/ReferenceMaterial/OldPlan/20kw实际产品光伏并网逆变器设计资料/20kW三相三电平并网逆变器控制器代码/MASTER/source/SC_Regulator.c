 /***********************************************************************
 *    FILENAME : SC_Regulator.c
 *
 *    PURPOSE  : 
 *    HISTORY  :
 *           DATE          VERSION      AUTHOR        NOTE
 ************************************************************************/
#include "SC_master.h"
#pragma CODE_SECTION(ADC_INT_Control, "ControlLoopInRAM");
#pragma CODE_SECTION(BoostControl, "ControlLoopInRAM");
#pragma CODE_SECTION(Boost1VLoop, "ControlLoopInRAM");
#pragma CODE_SECTION(Boost2VLoop, "ControlLoopInRAM");
#pragma CODE_SECTION(Boost1ILoop, "ControlLoopInRAM");
#pragma CODE_SECTION(Boost2ILoop, "ControlLoopInRAM");
#pragma CODE_SECTION(BoostParallelVLoop, "ControlLoopInRAM");
#pragma CODE_SECTION(BusPIDIdCal, "ControlLoopInRAM");
#pragma CODE_SECTION(CurrentPIDcontroller, "ControlLoopInRAM");
#pragma CODE_SECTION(dqPLLcontroller, "ControlLoopInRAM");
#pragma CODE_SECTION(BusBanlanceControl, "ControlLoopInRAM");
#pragma CODE_SECTION(InstantProtect, "ControlLoopInRAM");
#pragma CODE_SECTION(FeedForwardVolCalc, "ControlLoopInRAM");
#pragma CODE_SECTION(OCPONEBYONE, "ControlLoopInRAM");
#pragma CODE_SECTION(ADAccCalc, "ControlLoopInRAM");
#pragma CODE_SECTION(LCLActiveDamping, "ControlLoopInRAM");

/**********************************************************************
* FUNCION :  ADC_INT_Control
* PURPOSE :  
* CALLED BY: DSP/BIOS  kernel INT by 20KHz
**********************************************************************/
void ADC_INT_Control(void)
{	// start of SVPWM_Mode
	GpioDataRegs.GPBCLEAR.bit.GPIO40 = 1;
	Adc_Getvalue1();
	LCLActiveDamping();
	CurrentPIDcontroller();
//	GpioDataRegs.GPBSET.bit.GPIO40 = 1;//10us
	Adc_Getvalue2();
	OCPONEBYONE();//begin to Reset signal
	dqPLLcontroller();
	FeedForwardVolCalc();
	if((0 == g_SysFaultMessage.bit.HwAcOCP)&&(1 == AC_OCP_LEVEL)&&(0 == M_INVPWM_RESET_LEVEL))
	{
		M_INVPWM_ENABLEOCP;//Reset signal over
		if((NormalState == g_SysCurrentState)||(CheckState == g_SysCurrentState))
		{
			M_INVPWM_UNLOCK;
		}
	}
	M_INVPWM_ENABLEOCP;//Reset signal over
	DeadtimeCompensation();
	if(1 == g_StateCheck.bit.HwBusOVP)
	{
		MpptReg.u16BoostOffFlag = 1;
		MpptReg1.u16BoostOffFlag = 1;
		MpptReg2.u16BoostOffFlag = 1;
	}

	BoostControl();
	BusPIDIdCal();
	InstantProtect();
	BusBanlanceControl();

//	GpioDataRegs.GPBSET.bit.GPIO40 = 1;//30us
#ifdef	_IPOMS_
	IPOMS_SnatchGraph();
#endif

	ADAccCalc();
	GpioDataRegs.GPBSET.bit.GPIO40 = 1;//37.4us;43us;put program in ram time reduce to 24us
}

/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void BoostControl(void)
{
	if(INDEPENDENT_IN == g_MPPTState.bit.InputMode)
	{
		Boost1VLoop();
		Boost2VLoop();
		
		Boost1ILoop(g_Boost1VpvControl.f32VLoopOut);
		Boost2ILoop(g_Boost2VpvControl.f32VLoopOut);
	}
	else
	{
		BoostParallelVLoop();
	
		Boost1ILoop(g_BoostVpvControl.f32VLoopOut);
		Boost2ILoop(g_BoostVpvControl.f32VLoopOut);
	}
}

/**********************************************************************
* FUNCION :
* PURPOSE :  求出bus电压参考，确定Boost是否需要工作。
* CALLED BY: 在电网周期任务中执行，仅Normal状态下执行
**********************************************************************/
// void SetBoostState()
void BoostControlStrategy(void)
{
	float32 f32VPVMAX;
	float32 f32VPVMIN;

	f32VPVMAX = MAX(g_CalcResult.Ave.f32VPV1,g_CalcResult.Ave.f32VPV2);
	f32VPVMIN = MIN(g_CalcResult.Ave.f32VPV1,g_CalcResult.Ave.f32VPV2);
	if(NormalState != g_SysCurrentState)
	{
		return;
	}

	if(1 == g_MPPTState.bit.FactoryMode)	// DC Source Mode
	{
		g_BusRefCal.f32StandbusVolt = g_BusRefCal.f32GridPeakVolt + 40;	
		if(f32VPVMAX > (g_BusRefCal.f32StandbusVolt))
		{
			g_BusRefCal.f32BusRef = f32VPVMAX;//g_BusRefCal.f32StandbusVolt;
				
			MpptReg.u16BoostOffFlag = 1;
			MpptReg1.u16BoostOffFlag = 1;
			MpptReg2.u16BoostOffFlag = 1;
		}
		else if(f32VPVMIN < (g_BusRefCal.f32StandbusVolt))
		{
			g_BusRefCal.f32BusRef = g_BusRefCal.f32StandbusVolt;
//factory mode				
			MpptReg.u16BoostOffFlag = 0;
			if(1 == g_MPPTState.bit.PV1Test)
			{
				MpptReg1.u16BoostOffFlag = 0;
			}
			if(1 == g_MPPTState.bit.PV2Test)
			{
				MpptReg2.u16BoostOffFlag = 0;
			}
		}
		else
		{
		}
	}
	else
	{
		if(INDEPENDENT_IN == g_MPPTState.bit.InputMode)
		{
			if((g_Mppt1Calc.f32PvVoltRef > g_BusRefCal.f32StandbusVolt) 
				  &&(g_Mppt1Calc.f32PvVoltRef > g_Mppt2Calc.f32PvVoltRef))
			{
				g_BusRefCal.f32BusRef = g_Mppt1Calc.f32PvVoltRef - 1.5;
				g_BusRefCal.f32StandbusVolt = g_BusRefCal.f32GridPeakVolt + 20;
				MpptReg1.u16BoostOffFlag = 1;
				if(1 == g_MPPTState.bit.PV2ON)
				{
					MpptReg2.u16BoostOffFlag = 0;
				}
				else
				{
					MpptReg2.u16BoostOffFlag = 1;
				}
			}
			else if((g_Mppt2Calc.f32PvVoltRef > g_BusRefCal.f32StandbusVolt) 
					  &&(g_Mppt2Calc.f32PvVoltRef > g_Mppt1Calc.f32PvVoltRef))
			{
				g_BusRefCal.f32BusRef = g_Mppt2Calc.f32PvVoltRef - 1.5;
				g_BusRefCal.f32StandbusVolt = g_BusRefCal.f32GridPeakVolt + 20;
				MpptReg2.u16BoostOffFlag = 1;
				if(1 == g_MPPTState.bit.PV1ON)
				{
					MpptReg1.u16BoostOffFlag = 0;
				}
				else
				{
					MpptReg1.u16BoostOffFlag = 1;
				}
			}
			else 
			{
				g_BusRefCal.f32StandbusVolt = g_BusRefCal.f32GridPeakVolt + 40;	
				g_BusRefCal.f32BusRef = g_BusRefCal.f32StandbusVolt;

				if(1 == g_MPPTState.bit.PV1ON)
				{
					MpptReg1.u16BoostOffFlag = 0;
				}
				else
				{
					MpptReg1.u16BoostOffFlag = 1;
				}
				if(1 == g_MPPTState.bit.PV2ON)
				{
					MpptReg2.u16BoostOffFlag = 0;
				}
				else
				{
					MpptReg2.u16BoostOffFlag = 1;
				}
			}
		}

		else// if(PARALLEL_IN == g_MPPTState.bit.InputMode)	//  Parallel input
		{
			if(g_MpptCalc.f32PvVoltRef > g_BusRefCal.f32StandbusVolt)
			{
				g_BusRefCal.f32BusRef = g_MpptCalc.f32PvVoltRef - 1.5;	
				MpptReg.u16BoostOffFlag = 1;
				MpptReg1.u16BoostOffFlag = 1;
				MpptReg2.u16BoostOffFlag = 1;
				g_BusRefCal.f32StandbusVolt = g_BusRefCal.f32GridPeakVolt + 20;
			}
			else
			{
				g_BusRefCal.f32StandbusVolt = g_BusRefCal.f32GridPeakVolt + 40;	
				g_BusRefCal.f32BusRef = g_BusRefCal.f32StandbusVolt;
				MpptReg.u16BoostOffFlag = 0;
				MpptReg1.u16BoostOffFlag = 0;
				MpptReg2.u16BoostOffFlag = 0;
			}
		}
	}
	if(1 == g_StateCheck.bit.HwBusOVP)
	{
		MpptReg.u16BoostOffFlag = 1;
		MpptReg1.u16BoostOffFlag = 1;
		MpptReg2.u16BoostOffFlag = 1;
	}
}

/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void StandBusUpdate(void)
{
   	float32	f32MaxGridVolt = 0.0;

	f32MaxGridVolt = MAX(g_CalcResult.Rms.f32VGridR, g_CalcResult.Rms.f32VGridS);
	f32MaxGridVolt = MAX(g_CalcResult.Rms.f32VGridT, f32MaxGridVolt);

	g_BusRefCal.f32GridPeakVolt = f32MaxGridVolt * 2.45;
	
    if (g_BusRefCal.f32GridPeakVolt  <= 400)//VBUS_REF_MINLIMIT_500V)   
    {
        g_BusRefCal.f32GridPeakVolt  = 400;//VBUS_REF_MINLIMIT_500V;
    }
	else if(g_BusRefCal.f32GridPeakVolt  >= VBUS_REF_MAXLIMIT_730V)
	{
		g_BusRefCal.f32GridPeakVolt  = VBUS_REF_MAXLIMIT_730V;
	}
}

/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void BoostParallelVLoop(void)
{
// should first calculate BusCon_Reg.f32BusRef	
	float32 f32Vpv;

	if(NormalState == g_SysCurrentState)
	{
		if(1 == g_MPPTState.bit.FactoryMode)
		{
			g_BoostVpvControl.f32DeltaVbus = g_BusRefCal.f32BusRef- g_Instantaneous.f32VbusP - g_Instantaneous.f32VbusN;
			g_BoostVpvControl.f32DeltaPV = 0;
		}
		else
		{
			g_BoostVpvControl.f32DeltaVbus = g_BusRefCal.f32BusRef + 30.0 - g_Instantaneous.f32VbusP - g_Instantaneous.f32VbusN;
			if(g_BoostVpvControl.f32DeltaVbus > 0)
			{
				g_BoostVpvControl.f32DeltaVbus = 0;
			}
			else if(g_BoostVpvControl.f32DeltaVbus < -40)
			{
				g_BoostVpvControl.f32VLoopOut = g_BoostVpvControl.f32VLoopOut * 0.9;
			}
			f32Vpv = g_Instantaneous.f32VPV1;//
			g_BoostVpvControl.f32DeltaPV = f32Vpv - g_MpptCalc.f32PvVoltRef;
		}
	}
	else if(CheckState == g_SysCurrentState)
	{
		g_BoostVpvControl.f32DeltaVbus = g_BusRefCal.f32BusRef- g_Instantaneous.f32VbusP - g_Instantaneous.f32VbusN;
		g_BoostVpvControl.f32DeltaPV = 0;
	}
	

	g_BoostVpvControl.f32VPVErrOld = g_BoostVpvControl.f32VPVErrNew;
	g_BoostVpvControl.f32VPVErrNew = g_BoostVpvControl.f32DeltaVbus + g_BoostVpvControl.f32DeltaPV;
	
	g_BoostVpvControl.f32VLoopOut = g_BoostVpvControl.f32VLoopOut + KPI_BOOST_VPID * g_BoostVpvControl.f32VPVErrNew 
									 -KP_BOOST_VPID * g_BoostVpvControl.f32VPVErrOld;

    if ( g_BoostVpvControl.f32VLoopOut >=  g_Rated.f32InputCurrent)   
    {
        g_BoostVpvControl.f32VLoopOut = g_Rated.f32InputCurrent;  
    }
    else if ( g_BoostVpvControl.f32VLoopOut <= -0.1)  
    {
        g_BoostVpvControl.f32VLoopOut = -0.1;
    }

	if(CheckState == g_SysCurrentState)
	{
		if(g_BoostVpvControl.f32VLoopOut >=  0.5)
		{
			g_BoostVpvControl.f32VLoopOut = 0.5;
		}
	}
		
	if(1 == MpptReg.u16BoostOffFlag)
	{
		g_BoostVpvControl.f32VPVErrOld = 0;
		g_BoostVpvControl.f32VPVErrNew = 0;
		g_BoostVpvControl.f32VLoopOut = 0;
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void Boost1VLoop(void)
{
// should first calculate BusCon_Reg.f32BusRef	

	if(NormalState == g_SysCurrentState)
	{
		g_Boost1VpvControl.f32DeltaVbus = g_BusRefCal.f32BusRef + 30.0 - g_Instantaneous.f32VbusP - g_Instantaneous.f32VbusN;
		if(g_Boost1VpvControl.f32DeltaVbus > 0)
		{
			g_Boost1VpvControl.f32DeltaVbus = 0;
		}
		else if(g_Boost1VpvControl.f32DeltaVbus < -40)
		{
			g_Boost1VpvControl.f32VLoopOut = g_Boost1VpvControl.f32VLoopOut * 0.9;
		}
		g_Boost1VpvControl.f32DeltaPV = g_Instantaneous.f32VPV1 - g_Mppt1Calc.f32PvVoltRef;
	}
	else if(CheckState == g_SysCurrentState)
	{
		g_Boost1VpvControl.f32DeltaVbus = g_BusRefCal.f32BusRef- g_Instantaneous.f32VbusP - g_Instantaneous.f32VbusN;
		g_Boost1VpvControl.f32DeltaPV = 0;
	}

	g_Boost1VpvControl.f32VPVErrOld = g_Boost1VpvControl.f32VPVErrNew;
	g_Boost1VpvControl.f32VPVErrNew = g_Boost1VpvControl.f32DeltaVbus + g_Boost1VpvControl.f32DeltaPV;
	
	g_Boost1VpvControl.f32VLoopOut = g_Boost1VpvControl.f32VLoopOut + KPI_BOOST_VPID * g_Boost1VpvControl.f32VPVErrNew 
									 -KP_BOOST_VPID * g_Boost1VpvControl.f32VPVErrOld;

    if ( g_Boost1VpvControl.f32VLoopOut >=  g_Rated.f32InputCurrent)   
    {
        g_Boost1VpvControl.f32VLoopOut = g_Rated.f32InputCurrent;  
    }
    else if ( g_Boost1VpvControl.f32VLoopOut <= -0.1)  
    {
        g_Boost1VpvControl.f32VLoopOut = -0.1;
    }
	if(CheckState == g_SysCurrentState)
	{
		if(g_Boost1VpvControl.f32VLoopOut >=  0.5)
		{
			g_Boost1VpvControl.f32VLoopOut = 0.5;
		}
	}
	if(1 == MpptReg1.u16BoostOffFlag)	// 2￠áaê±MpptReg?￠MpptReg1?￠MpptReg2μ?u16BoostOffFlagí?ê±????í?ê±??á?
	{
		g_Boost1VpvControl.f32VPVErrOld = 0;
		g_Boost1VpvControl.f32VPVErrNew = 0;
		g_Boost1VpvControl.f32VLoopOut = 0;
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void Boost1ILoop(float32 f32IPVref)
{
	Uint16 u16tmp;
	
	g_Boost1IpvControl.f32IPVref = f32IPVref;
	
	g_Boost1IpvControl.f32IPVErrOld = g_Boost1IpvControl.f32IPVErrNew;
	g_Boost1IpvControl.f32IPVErrNew = g_Boost1IpvControl.f32IPVref - g_Instantaneous.f32IPV1;
	
	g_Boost1IpvControl.f32Duty = g_Boost1IpvControl.f32Duty + KPI_BOOST_IPID * g_Boost1IpvControl.f32IPVErrNew 
								-KP_BOOST_IPID * g_Boost1IpvControl.f32IPVErrOld;

    if ( g_Boost1IpvControl.f32Duty >=  BOOST_DUTY_MAX)   
    {
        g_Boost1IpvControl.f32Duty = BOOST_DUTY_MAX;  
    }
    else if ( g_Boost1IpvControl.f32Duty <= 0)  
    {
        g_Boost1IpvControl.f32Duty = 0;
	}
		
	if(1 == MpptReg1.u16BoostOffFlag)	// 2￠áaê±MpptReg?￠MpptReg1?￠MpptReg2μ?u16BoostOffFlagí?ê±????í?ê±??á?
	{
		g_Boost1IpvControl.f32IPVErrOld = 0;
		g_Boost1IpvControl.f32IPVErrNew = 0;
		g_Boost1IpvControl.f32Duty= 0;
		u16tmp = 0;
	}
	else
	{
		u16tmp = (int16)g_Boost1IpvControl.f32Duty;
	}

	ECap1Regs.CAP4 = u16tmp;
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void Boost2VLoop(void)
{
// should first calculate BusCon_Reg.f32BusRef	

	if(NormalState == g_SysCurrentState)
	{
		g_Boost2VpvControl.f32DeltaVbus = g_BusRefCal.f32BusRef + 30.0 - g_Instantaneous.f32VbusP - g_Instantaneous.f32VbusN;
		if(g_Boost2VpvControl.f32DeltaVbus > 0)
		{
			g_Boost2VpvControl.f32DeltaVbus = 0;
		}
		else if(g_Boost2VpvControl.f32DeltaVbus < -40)
		{
			g_Boost2VpvControl.f32VLoopOut = g_Boost2VpvControl.f32VLoopOut * 0.9;
		}
		g_Boost2VpvControl.f32DeltaPV = g_Instantaneous.f32VPV2 - g_Mppt2Calc.f32PvVoltRef;
	}
	else if(CheckState == g_SysCurrentState)
	{
		g_Boost2VpvControl.f32DeltaVbus = g_BusRefCal.f32BusRef- g_Instantaneous.f32VbusP - g_Instantaneous.f32VbusN;
		g_Boost2VpvControl.f32DeltaPV = 0;
	}

	g_Boost2VpvControl.f32VPVErrOld = g_Boost2VpvControl.f32VPVErrNew;
	g_Boost2VpvControl.f32VPVErrNew = g_Boost2VpvControl.f32DeltaVbus + g_Boost2VpvControl.f32DeltaPV;
	
	g_Boost2VpvControl.f32VLoopOut = g_Boost2VpvControl.f32VLoopOut + KPI_BOOST_VPID * g_Boost2VpvControl.f32VPVErrNew 
									 -KP_BOOST_VPID * g_Boost2VpvControl.f32VPVErrOld;

    if ( g_Boost2VpvControl.f32VLoopOut >=  g_Rated.f32InputCurrent)   
    {
        g_Boost2VpvControl.f32VLoopOut = g_Rated.f32InputCurrent;  
    }
    else if ( g_Boost2VpvControl.f32VLoopOut <= -0.1)  
    {
        g_Boost2VpvControl.f32VLoopOut = -0.1;
    }
	if(CheckState == g_SysCurrentState)
	{
		if(g_Boost2VpvControl.f32VLoopOut >=  0.5)
		{
			g_Boost2VpvControl.f32VLoopOut = 0.5;
		}
	}		
	if(1 == MpptReg2.u16BoostOffFlag)	// 2￠áaê±MpptReg?￠MpptReg1?￠MpptReg2μ?u16BoostOffFlagí?ê±????í?ê±??á?
	{
		g_Boost2VpvControl.f32VPVErrOld = 0;
		g_Boost2VpvControl.f32VPVErrNew = 0;
		g_Boost2VpvControl.f32VLoopOut = 0;
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void Boost2ILoop(float32 f32IPVref)
{
	Uint16 u16tmp;
	
	g_Boost2IpvControl.f32IPVref = f32IPVref;
	
	g_Boost2IpvControl.f32IPVErrOld = g_Boost2IpvControl.f32IPVErrNew;
	g_Boost2IpvControl.f32IPVErrNew = g_Boost2IpvControl.f32IPVref - g_Instantaneous.f32IPV2;
	
	g_Boost2IpvControl.f32Duty = g_Boost2IpvControl.f32Duty + KPI_BOOST_IPID * g_Boost2IpvControl.f32IPVErrNew 
								-KP_BOOST_IPID * g_Boost2IpvControl.f32IPVErrOld;

    if ( g_Boost2IpvControl.f32Duty >=  BOOST_DUTY_MAX)   
    {
        g_Boost2IpvControl.f32Duty = BOOST_DUTY_MAX;  
    }
    else if ( g_Boost2IpvControl.f32Duty <= 0)  
    {
        g_Boost2IpvControl.f32Duty = 0;
	}

	if(1 == MpptReg2.u16BoostOffFlag)	// 
	{
		g_Boost2IpvControl.f32IPVErrOld = 0;
		g_Boost2IpvControl.f32IPVErrNew = 0;
		g_Boost2IpvControl.f32Duty= 0;
		u16tmp = 0;
	}
	else
	{
		u16tmp = (int16)g_Boost2IpvControl.f32Duty;
	}

	ECap2Regs.CAP4 = u16tmp;
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void BusPIDIdCal(void)
{
	float32 f32Idreftemp =0;
	float32 f32Iqreftemp =0;

//BUS PI control loop
 //V_PI_BUS=I_qref   
	g_BusPIDCal.f32VBus = (g_Instantaneous.f32VbusP + g_Instantaneous.f32VbusN) * 0.2 + 0.8 * g_BusPIDCal.f32VBus ;
//   g_BusPIDCal.f32VBus = g_Instantaneous.f32VbusP + g_Instantaneous.f32VbusN;
	g_BusPIDCal.f32VBusErrOld = g_BusPIDCal.f32VBusErrNew;

	g_BusPIDCal.f32VBusErrNew = g_BusPIDCal.f32VBus - g_BusRefCal.f32BusRef;

	if(g_BusPIDCal.f32VBusErrNew > 30)
	{
		g_BusPIDCal.f32VBusErrNew = 30;
	}

	g_BusPIDCal.f32VBusPI = g_BusPIDCal.f32VBusPI + KPI_BUS_PID * g_BusPIDCal.f32VBusErrNew - KP_BUS_PID * g_BusPIDCal.f32VBusErrOld;//

	if (g_BusPIDCal.f32VBusPI <= 0.1)  
    {
        g_BusPIDCal.f32VBusPI = 0.1;
    }
  	else if ( g_BusPIDCal.f32VBusPI >=  g_BusPIDCal.f32IdRefMax)   
    {
       g_BusPIDCal.f32VBusPI = g_BusPIDCal.f32IdRefMax;  
    }
	if(NormalState  == g_SysCurrentState)
	{
//		g_GridManager.InvQ.f32Tanphi = 0;
		if(1 == g_MPPTState.bit.FactoryMode)
		{
			f32Idreftemp = g_BusPIDCal.f32IdRef_DCSource;
			f32Iqreftemp = 0.0038 * g_PLLCal.f32Vd;//0.0041
		}
		else
		{
			f32Idreftemp = g_BusPIDCal.f32VBusPI;
			f32Iqreftemp = f32Idreftemp * g_GridManager.InvQ.f32Tanphi + 0.0038 * g_PLLCal.f32Vd;//0.0038 * g_PLLCal.f32Vd;//
		}

		if(0x00 != g_GridManager.Lvrt.u16StartFlag)
		{
			f32Idreftemp = 0.3;
			f32Iqreftemp = g_GridManager.Lvrt.f32IqRef;
		}
		g_BusPIDCal.f32IdRef = f32Idreftemp;
		g_BusPIDCal.f32IqRef = f32Iqreftemp;
	}
	else
	{
		g_BusPIDCal.f32VBusErrOld = 0;
		g_BusPIDCal.f32VBusErrNew = 0;
		g_BusPIDCal.f32VBusPI = 0.1;
		g_BusPIDCal.f32IdRef =  0.1;
		g_BusPIDCal.f32IdRef_DCSource =0.1;
	}
}

/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void CurrentPIDcontroller(void)	
{
	float32 CircleLimitationTemp;
	float32 f32DutyDQtemp;

	static Uint16 InvSoftStartOK = 0;
	

	//abc->alpha,beta
	g_CurPIDCal.f32Ialpha = 0.66667 * g_Instantaneous.f32IGridR - 0.33333 * g_Instantaneous.f32IGridS - 0.33333 * g_Instantaneous.f32IGridT;
	g_CurPIDCal.f32Ibeta = (g_Instantaneous.f32IGridS - g_Instantaneous.f32IGridT) * 0.57735f;
	//alpha,beta->dq
	g_CurPIDCal.f32Id = g_CurPIDCal.f32Ialpha * g_PLLCal.ISinTheta - g_CurPIDCal.f32Ibeta * g_PLLCal.ICosTheta;// CurrConReg.f32Ialpha * PLLConReg.Sin_Theta - CurrConReg.f32Ibeta * PLLConReg.Cos_Theta;
	g_CurPIDCal.f32Iq = g_CurPIDCal.f32Ibeta * g_PLLCal.ISinTheta + g_CurPIDCal.f32Ialpha * g_PLLCal.ICosTheta;//CurrConReg.f32Ibeta * PLLConReg.Sin_Theta + CurrConReg.f32Ialpha * PLLConReg.Cos_Theta;

	//  cur Id   PID
	//BusPIDCal.f32IdRef = 0.5;
	g_CurPIDCal.f32IdErrOld = g_CurPIDCal.f32IdErrNew;
	g_CurPIDCal.f32IdErrNew = g_BusPIDCal.f32IdRef - g_CurPIDCal.f32Id;
	g_CurPIDCal.f32DPI = g_CurPIDCal.f32DPI + KPI_CURRENT_PID * g_CurPIDCal.f32IdErrNew - KP_CURRENT_PID * g_CurPIDCal.f32IdErrOld;
	
	//  cur Iq   PID
//	g_BusPIDCal.f32IqRef = 0;  //g_BusPIDCal.f32IqRef_SPIRX
	g_CurPIDCal.f32IqErrOld = g_CurPIDCal.f32IqErrNew;
	g_CurPIDCal.f32IqErrNew = g_BusPIDCal.f32IqRef - g_CurPIDCal.f32Iq;
	g_CurPIDCal.f32QPI = g_CurPIDCal.f32QPI + KPI_CURRENT_PID * g_CurPIDCal.f32IqErrNew - KP_CURRENT_PID * g_CurPIDCal.f32IqErrOld;
		
	// grid voltage feedforward	Got Voltage of axis_d after current PID regulator 
	if(NormalState == g_SysCurrentState)	
	{	
		g_CurPIDCal.f32DutyD = g_CurPIDCal.f32DPI + g_PLLCal.f32VdForward;    ///  <<<----curr .Id
		g_CurPIDCal.f32DutyQ = g_CurPIDCal.f32QPI + g_PLLCal.f32VqForward; /// <<<----curr .Iq
	}
	else if((1 == g_StateCheck.bit.InvSoftStartFalg)&&(CheckState == g_SysCurrentState))
	{	
		if(((g_CurPIDCal.f32DutyD + (0.00023 * g_PLLCal.f32VdForward)) < g_PLLCal.f32VdForward) &&(0 ==InvSoftStartOK))
		{
			g_CurPIDCal.f32IdErrOld = 0;
			g_CurPIDCal.f32IdErrNew = 0;
			g_CurPIDCal.f32IqErrOld = 0;
			g_CurPIDCal.f32IqErrNew = 0;
			g_CurPIDCal.f32DPI = 0;
			g_CurPIDCal.f32QPI = 0;	

			g_CurPIDCal.f32DutyD += 0.00023 * g_PLLCal.f32VdForward;
			g_CurPIDCal.f32DutyQ += 0.00023 * g_PLLCal.f32VqForward; 
		}
		else
		{
			InvSoftStartOK = 1;
			g_StateCheck.bit.InvSoftStartOver = 1;
			g_CurPIDCal.f32DutyD = g_CurPIDCal.f32DPI + g_PLLCal.f32VdForward;    ///  <<<----curr .Id
			g_CurPIDCal.f32DutyQ = g_CurPIDCal.f32QPI + g_PLLCal.f32VqForward; /// <<<----curr .Iq

			g_BusPIDCal.f32IdRef = 0.1;
			g_BusPIDCal.f32IqRef = 0.0031 * g_PLLCal.f32Vd;
		}
	}
	else
	{
		InvSoftStartOK = 0;
		g_StateCheck.bit.InvSoftStartFalg = 0;
		g_StateCheck.bit.InvSoftStartOver = 0;
		g_CurPIDCal.f32DutyD = 80;
		g_CurPIDCal.f32DutyQ = 0;
		g_CurPIDCal.f32IdErrOld = 0;
		g_CurPIDCal.f32IdErrNew = 0;
		g_CurPIDCal.f32IqErrOld = 0;
		g_CurPIDCal.f32IqErrNew = 0;
		g_CurPIDCal.f32DPI = 0;
		g_CurPIDCal.f32QPI = 0;
		g_BusPIDCal.f32IqRef = 0;
	}

	// Vector circle limitation
	f32DutyDQtemp =     	( g_CurPIDCal.f32DutyD
							* g_CurPIDCal.f32DutyD) 
							+ ( g_CurPIDCal.f32DutyQ
							* g_CurPIDCal.f32DutyQ);

	if ( f32DutyDQtemp > SVPWM_DUTY_LIMIT_2)
	{
		CircleLimitationTemp = isqrt(f32DutyDQtemp);
		g_CurPIDCal.f32DutyD = SVPWM_DUTY_LIMIT * g_CurPIDCal.f32DutyD * CircleLimitationTemp;
										
		g_CurPIDCal.f32DutyQ = SVPWM_DUTY_LIMIT * g_CurPIDCal.f32DutyQ * CircleLimitationTemp;
		//// Quit PI staturation status	
		g_CurPIDCal.f32DPI = g_CurPIDCal.f32DutyD - g_PLLCal.f32VdForward;
		g_CurPIDCal.f32QPI = g_CurPIDCal.f32DutyQ - g_PLLCal.f32VqForward;
	}

	//dq->alpha,beta
	g_CurPIDCal.f32AlphaPI = g_CurPIDCal.f32DutyD * g_PLLCal.ISinTheta + g_CurPIDCal.f32DutyQ * g_PLLCal.ICosTheta;//CurPIDCal.f32DutyD * PLLConReg.Sin_Theta + CurrConReg.f32DutyQ * PLLConReg.Cos_Theta;
	g_CurPIDCal.f32BetaPI = g_CurPIDCal.f32DutyQ * g_PLLCal.ISinTheta - g_CurPIDCal.f32DutyD * g_PLLCal.ICosTheta;//CurrConReg.f32DutyQ * PLLConReg.Sin_Theta - CurrConReg.f32DutyD * PLLConReg.Cos_Theta; 

	//alpha,beta->abc
	g_CurPIDCal.PIa = g_CurPIDCal.f32AlphaPI - 20 * g_CurPIDCal.f32CapHFCur_vinvR;//
	g_CurPIDCal.PIb = (0.866025 * g_CurPIDCal.f32BetaPI - 0.5 * g_CurPIDCal.f32AlphaPI) - 20 * g_CurPIDCal.f32CapHFCur_vinvS;//
	g_CurPIDCal.PIc = (-0.866025 * g_CurPIDCal.f32BetaPI - 0.5 * g_CurPIDCal.f32AlphaPI) - 20 * g_CurPIDCal.f32CapHFCur_vinvT;//

	//3d
	if (g_CurPIDCal.PIa >= g_CurPIDCal.PIb)
	{
		g_CurPIDCal.f32DutyMax = g_CurPIDCal.PIa;
		g_CurPIDCal.f32DutyMin = g_CurPIDCal.PIb;
	}
	else
	{
		g_CurPIDCal.f32DutyMax = g_CurPIDCal.PIb;
		g_CurPIDCal.f32DutyMin = g_CurPIDCal.PIa;		
	}
	if (g_CurPIDCal.PIc >= g_CurPIDCal.f32DutyMax )
	{
		g_CurPIDCal.f32DutyMax  = g_CurPIDCal.PIc;
	}
	else if(g_CurPIDCal.PIc < g_CurPIDCal.f32DutyMin)
	{
		g_CurPIDCal.f32DutyMin = g_CurPIDCal.PIc;
	}	
	g_CurPIDCal.f32Duty3d = 0.5 * (g_CurPIDCal.f32DutyMax + g_CurPIDCal.f32DutyMin);
//	CurPIDCal.f32Duty3d = 0;

	//Duty
	g_CurPIDCal.f32DutyR = g_CurPIDCal.PIa - g_CurPIDCal.f32Duty3d;// - BalanceDuty;
	g_CurPIDCal.f32DutyS = g_CurPIDCal.PIb - g_CurPIDCal.f32Duty3d;// - BalanceDuty;
	g_CurPIDCal.f32DutyT = g_CurPIDCal.PIc - g_CurPIDCal.f32Duty3d;// - BalanceDuty;
	//
	if(NormalState == g_SysCurrentState)	
	{
		g_CurPIDCal.f32DutyR += (g_CurPIDCal.f32DutyADeadTimeCom + g_BusBanlanceCal.f32PIBusBanlance);
		g_CurPIDCal.f32DutyS += (g_CurPIDCal.f32DutyBDeadTimeCom + g_BusBanlanceCal.f32PIBusBanlance);
		g_CurPIDCal.f32DutyT += (g_CurPIDCal.f32DutyCDeadTimeCom + g_BusBanlanceCal.f32PIBusBanlance);
	}
	else
	{
		g_BusBanlanceCal.f32PIBusBanlance = 0;
	}
	if(g_CurPIDCal.f32DutyR >= PWM_HALF_PERIOD)
	{
		g_CurPIDCal.f32DutyR = PWM_HALF_PERIOD - 1;
	}
	else if(g_CurPIDCal.f32DutyR < -PWM_HALF_PERIOD)
	{
		g_CurPIDCal.f32DutyR = 1 - PWM_HALF_PERIOD;	
	}
	if(g_CurPIDCal.f32DutyS >= PWM_HALF_PERIOD)
	{
		g_CurPIDCal.f32DutyS = PWM_HALF_PERIOD - 1;
	}
	else if(g_CurPIDCal.f32DutyS < -PWM_HALF_PERIOD)
	{
		g_CurPIDCal.f32DutyS = 1 - PWM_HALF_PERIOD;	
	}
	if(g_CurPIDCal.f32DutyT >= PWM_HALF_PERIOD)
	{
		g_CurPIDCal.f32DutyT = PWM_HALF_PERIOD - 1;
	}
	else if(g_CurPIDCal.f32DutyT < -PWM_HALF_PERIOD)
	{
		g_CurPIDCal.f32DutyT = 1 - PWM_HALF_PERIOD;	
	}
	
	if(g_CurPIDCal.f32DutyR >= 0)
	{
		EPwm1Regs.CMPA.half.CMPA = (int16)(g_CurPIDCal.f32DutyR);	
		EPwm1Regs.CMPB = (int16)(g_CurPIDCal.f32DutyR);	
		EPwm2Regs.CMPA.half.CMPA = 0;	
		EPwm2Regs.CMPB = 0;					
	}
	else
	{
		EPwm2Regs.CMPA.half.CMPA = (int16)(- g_CurPIDCal.f32DutyR);	
		EPwm2Regs.CMPB = (int16)(- g_CurPIDCal.f32DutyR);	
		EPwm1Regs.CMPA.half.CMPA = 0;	
		EPwm1Regs.CMPB = 0;					
	}
	if(g_CurPIDCal.f32DutyS >= 0)
	{
		EPwm3Regs.CMPA.half.CMPA = (int16)(g_CurPIDCal.f32DutyS);	
		EPwm3Regs.CMPB = (int16)(g_CurPIDCal.f32DutyS);	
		EPwm4Regs.CMPA.half.CMPA = 0;	
		EPwm4Regs.CMPB = 0;					
	}
	else
	{
		EPwm4Regs.CMPA.half.CMPA = (int16)(- g_CurPIDCal.f32DutyS);	
		EPwm4Regs.CMPB = (int16)(- g_CurPIDCal.f32DutyS);	
		EPwm3Regs.CMPA.half.CMPA = 0;	
		EPwm3Regs.CMPB = 0;					
	}
	if(g_CurPIDCal.f32DutyT >= 0)
	{
		EPwm5Regs.CMPA.half.CMPA = (int16)(g_CurPIDCal.f32DutyT);	
		EPwm5Regs.CMPB = (int16)(g_CurPIDCal.f32DutyT);	
		EPwm6Regs.CMPA.half.CMPA = 0;	
		EPwm6Regs.CMPB = 0;					
	}
	else
	{
		EPwm6Regs.CMPA.half.CMPA = (int16)(- g_CurPIDCal.f32DutyT);	
		EPwm6Regs.CMPB = (int16)(- g_CurPIDCal.f32DutyT);	
		EPwm5Regs.CMPA.half.CMPA = 0;	
		EPwm5Regs.CMPB = 0;					
	}
}

/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void dqPLLcontroller(void)
{

	static float32 f32PLL_Valpha_1 = 0;
	static float32 f32PLL_Valpha_2 = 0;
	static float32 f32PLL_Vbeta_1 = 0;
	static float32 f32PLL_Vbeta_2 = 0;
	static float32 f32PLL_Valpha_p = 0;
	static float32 f32PLL_Vbeta_p = 0;
	static float32 f32PLL_Valpha_n = 0;
	static float32 f32PLL_Vbeta_n = 0;
	float32 f32temp1;

	f32temp1 = (g_PLLCal.f32Valpha - f32PLL_Valpha_1) * 1.414 - f32PLL_Valpha_2;
	f32PLL_Valpha_1 +=(g_PLLCal.f32ThetaStep * f32temp1);//(314 * 0.0000625 * f32temp1);
	f32PLL_Valpha_2 +=(g_PLLCal.f32ThetaStep * f32PLL_Valpha_1);//(314 * 0.0000625 * f32PLL_Valpha_1);

	f32temp1 = (g_PLLCal.f32Vbeta - f32PLL_Vbeta_1) * 1.414 - f32PLL_Vbeta_2;
	f32PLL_Vbeta_1 += (g_PLLCal.f32ThetaStep * f32temp1);//(314 * 0.0000625 * f32temp1);
	f32PLL_Vbeta_2 += (g_PLLCal.f32ThetaStep * f32PLL_Vbeta_1);//(314 * 0.0000625 * f32PLL_Vbeta_1);

	f32PLL_Valpha_p = 0.5 * (f32PLL_Valpha_1 - f32PLL_Vbeta_2);
	f32PLL_Vbeta_p = 0.5 * (f32PLL_Vbeta_1 + f32PLL_Valpha_2);

	f32PLL_Valpha_n = 0.5 * (f32PLL_Valpha_1 + f32PLL_Vbeta_2);
	f32PLL_Vbeta_n = 0.5 * (f32PLL_Vbeta_1 - f32PLL_Valpha_2);

	g_PLLCal.f32Vd_Pos = f32PLL_Valpha_p * g_PLLCal.SinTheta - f32PLL_Vbeta_p * g_PLLCal.CosTheta;
	g_PLLCal.f32Vq_Pos = f32PLL_Vbeta_p * g_PLLCal.SinTheta + f32PLL_Valpha_p * g_PLLCal.CosTheta;

	g_PLLCal.f32Vd_Neg = f32PLL_Valpha_n * g_PLLCal.SinTheta + f32PLL_Vbeta_n * g_PLLCal.CosTheta;
	g_PLLCal.f32Vq_Neg = f32PLL_Vbeta_n * g_PLLCal.SinTheta - f32PLL_Valpha_n * g_PLLCal.CosTheta;

	if(g_PLLCal.f32Vq_Pos > 50)
	{
		g_PLLCal.f32Vq_Pos = 50;
	}
	else if(g_PLLCal.f32Vq_Pos < -50)
	{
		g_PLLCal.f32Vq_Pos = -50;
	}

	g_PLLCal.f32VNeg = (g_PLLCal.f32Vd_Neg * g_PLLCal.f32Vd_Neg + g_PLLCal.f32Vq_Neg * g_PLLCal.f32Vq_Neg);

	if(g_PLLCal.f32VNeg>10000)
	{
		g_PLLCal.f32VNeg = 10000;
	}

// dqPLL PID regulator process
	g_PLLCal.f32VqErrOld = g_PLLCal.f32VqErrNew;
	g_PLLCal.f32VqErrNew = g_PLLCal.f32Vq_Pos;
	g_PLLCal.f32PLLStep = g_PLLCal.f32PLLStep + g_PLLCal.f32VqErrNew * KPI_PLL - g_PLLCal.f32VqErrOld * KP_PLL ;//

// Up Limit Staturation Control 0.015708
	if(g_PLLCal.f32PLLStep >= THETA_STEP_MAX_LIMIT)//65Hz,0.02042035
	{
		g_PLLCal.f32PLLStep = THETA_STEP_MAX_LIMIT ;
	}
// Low Limit Staturation Control
	if(g_PLLCal.f32PLLStep <=  (-THETA_STEP_MIN_LIMIT) )//35Hz,0.01099557
	{
		g_PLLCal.f32PLLStep = (-THETA_STEP_MIN_LIMIT) ;
	}

//	g_PLLCal.f32ThetaStep = 0.019635;//0.015708;
	g_PLLCal.f32ThetaStep = g_PLLCal.f32PLLStep + THETA_STEP_RATED;
	g_PLLCal.f32Theta +=  g_PLLCal.f32ThetaStep;	

	if ( g_PLLCal.f32Theta > VALUE_2PI )
	{
		g_PLLCal.f32Theta = g_PLLCal.f32Theta - VALUE_2PI;
		g_StateCheck.bit.GridCrossingFlag = 1;

	}
	f32temp1 = g_PLLCal.f32Theta - 0.0321 + g_PLLCal.AIDeltaTheta;//0.0371;0.0471;//SC17k0.035

	if(f32temp1 > 0)//0.07854;
	{
		g_PLLCal.f32ITheta = f32temp1;//0.07854;
	}
	else
	{
		g_PLLCal.f32ITheta = VALUE_2PI + f32temp1;//0.07854;
	}
	sincos(g_PLLCal.f32ITheta, &(g_PLLCal.ISinTheta), &(g_PLLCal.ICosTheta));	
	sincos(g_PLLCal.f32Theta, &(g_PLLCal.SinTheta), &(g_PLLCal.CosTheta));
}

void LCLActiveDamping(void)
{
	float32 f32CapCur_valpha, f32CapCur_vbeta;
	float32 f32CapCur_difvinvR,f32CapCur_difvinvS,f32CapCur_difvinvT;
	float32 f32CapCur_vinvR,f32CapCur_vinvS,f32CapCur_vinvT;

	static float32 oldVinvR,oldVinvS,oldVinvT = 0;

//	g_PLLCal.f32Valpha = 0.66667 * g_Instantaneous.f32VGridR - 0.33333 * g_Instantaneous.f32VGridS - 0.33333 * g_Instantaneous.f32VGridT;
//	g_PLLCal.f32Vbeta = (g_Instantaneous.f32VGridS - g_Instantaneous.f32VGridT) * 0.57735f;

	f32CapCur_valpha = - 0.00314 * g_PLLCal.f32Vbeta;
	f32CapCur_vbeta = 0.00314 * g_PLLCal.f32Valpha;

	f32CapCur_difvinvR = 0.2 * (g_Instantaneous.f32VGridR - oldVinvR);
	f32CapCur_difvinvS = 0.2 * (g_Instantaneous.f32VGridS - oldVinvS);
	f32CapCur_difvinvT = 0.2 * (g_Instantaneous.f32VGridT - oldVinvT);

	f32CapCur_vinvR = f32CapCur_valpha;
	f32CapCur_vinvS = (0.866025 * f32CapCur_vbeta - 0.5 * f32CapCur_valpha);
	f32CapCur_vinvT = (-0.866025 * f32CapCur_vbeta - 0.5 * f32CapCur_valpha);

	g_CurPIDCal.f32CapHFCur_vinvR = f32CapCur_difvinvR - f32CapCur_vinvR;
	g_CurPIDCal.f32CapHFCur_vinvS = f32CapCur_difvinvS - f32CapCur_vinvS;
	g_CurPIDCal.f32CapHFCur_vinvT = f32CapCur_difvinvT - f32CapCur_vinvT;

	oldVinvR = g_Instantaneous.f32VGridR;
	oldVinvS = g_Instantaneous.f32VGridS;
	oldVinvT = g_Instantaneous.f32VGridT;

	if(g_CurPIDCal.f32CapHFCur_vinvR > 0.8)// PWM_HALF_PERIOD)
	{
		g_CurPIDCal.f32CapHFCur_vinvR = 0.8;// PWM_HALF_PERIOD;	
	}
	else if(g_CurPIDCal.f32CapHFCur_vinvR < -0.8)// PWM_HALF_PERIOD)
	{
		g_CurPIDCal.f32CapHFCur_vinvR = -0.8;// PWM_HALF_PERIOD;	
	}
	if(g_CurPIDCal.f32CapHFCur_vinvS > 0.8)// PWM_HALF_PERIOD)
	{
		g_CurPIDCal.f32CapHFCur_vinvS = 0.8;// PWM_HALF_PERIOD;	
	}
	else if(g_CurPIDCal.f32CapHFCur_vinvS < -0.8)// PWM_HALF_PERIOD)
	{
		g_CurPIDCal.f32CapHFCur_vinvS = -0.8;// PWM_HALF_PERIOD;	
	}
	if(g_CurPIDCal.f32CapHFCur_vinvT > 0.8)// PWM_HALF_PERIOD)
	{
		g_CurPIDCal.f32CapHFCur_vinvT = 0.8;// PWM_HALF_PERIOD;	
	}
	else if(g_CurPIDCal.f32CapHFCur_vinvT < -0.8)// PWM_HALF_PERIOD)
	{
		g_CurPIDCal.f32CapHFCur_vinvT = -0.8;// PWM_HALF_PERIOD;	
	}


} // end, dq frame Phase Lock Loop controller


/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void DeadtimeCompensation(void)
{
	float32 f32AlphaIref,f32BetaIref,f32IaRef,f32IbRef,f32IcRef;

	f32AlphaIref = g_BusPIDCal.f32IdRef * g_PLLCal.ISinTheta + g_BusPIDCal.f32IqRef * g_PLLCal.ICosTheta;//CurrConReg.f32DutyD * PLLConReg.Sin_Theta + CurrConReg.f32DutyQ * PLLConReg.Cos_Theta;
	f32BetaIref = g_BusPIDCal.f32IqRef * g_PLLCal.ISinTheta - g_BusPIDCal.f32IdRef * g_PLLCal.ICosTheta;//CurrConReg.f32DutyQ * PLLConReg.Sin_Theta - CurrConReg.f32DutyD * PLLConReg.Cos_Theta; 

	f32IaRef = f32AlphaIref;
	f32IbRef = (0.866025 * f32BetaIref - 0.5 * f32AlphaIref);
	f32IcRef = (-0.866025 * f32BetaIref - 0.5 * f32AlphaIref);
	if(f32IaRef > 0)
	{
		g_CurPIDCal.f32DutyADeadTimeCom = 35;
	}
	else
	{
		g_CurPIDCal.f32DutyADeadTimeCom = -35;
	}
	if(f32IbRef > 0)
	{
		g_CurPIDCal.f32DutyBDeadTimeCom = 35;
	}
	else
	{
		g_CurPIDCal.f32DutyBDeadTimeCom = -35;
	}
	if(f32IcRef > 0)
	{
		g_CurPIDCal.f32DutyCDeadTimeCom = 35;
	}
	else
	{
		g_CurPIDCal.f32DutyCDeadTimeCom = -35;
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void BusBanlanceControl(void)
{
	g_BusBanlanceCal.f32DeltaBusErrOld = g_BusBanlanceCal.f32DeltaBusErrNew;

	g_BusBanlanceCal.f32DeltaBusErrNew = g_Instantaneous.f32VbusP - g_Instantaneous.f32VbusN;
	g_BusBanlanceCal.f32PIBusBanlance= g_BusBanlanceCal.f32PIBusBanlance + g_BusBanlanceCal.f32DeltaBusErrNew * KPI_BUSBANLANCE_PID 
											- g_BusBanlanceCal.f32DeltaBusErrOld * KP_BUSBANLANCE_PID;	
																				
	if(g_BusBanlanceCal.f32PIBusBanlance > BUS_BANLANCE_MAX)
	{
		g_BusBanlanceCal.f32PIBusBanlance = BUS_BANLANCE_MAX;
	}
	else if(g_BusBanlanceCal.f32PIBusBanlance < (-BUS_BANLANCE_MAX))
	{
		g_BusBanlanceCal.f32PIBusBanlance = -BUS_BANLANCE_MAX;	
	}
	else
	{
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void OCPONEBYONE(void)
{
	static Uint16 s_u16CntEnter = 0;
	static Uint16 s_u16UnrecoverCntEnter = 0;
	if(0 == AC_OCP_LEVEL)
	{
		M_INVPWM_DISABLEOCP;
		M_INVPWM_LOCK;		
		s_u16CntEnter++;
		if(s_u16CntEnter >= 5)
		{
			s_u16CntEnter = 0;
			if(0 == g_SysFaultMessage.bit.HwAcOCP)
			{
				g_SysFaultMessage.bit.HwAcOCP = 1;
				s_u16UnrecoverCntEnter++;
				if(s_u16UnrecoverCntEnter >= 3)
				{
					s_u16UnrecoverCntEnter = 0;
					g_SysFaultMessage.bit.unrecoverHwAcOCP = 1;
				}
			}
		}
	}
	else
	{
		s_u16CntEnter = 0;
	}
}

/**********************************************************************
* FUNCION :  Adc_Getvalue1
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void FeedForwardVolCalc(void)
{

	g_PLLCal.f32Valpha = 0.66667 * g_Instantaneous.f32VGridR - 0.33333 * g_Instantaneous.f32VGridS - 0.33333 * g_Instantaneous.f32VGridT;
	g_PLLCal.f32Vbeta = (g_Instantaneous.f32VGridS - g_Instantaneous.f32VGridT) * 0.57735f;

	g_PLLCal.f32Vd = g_PLLCal.f32Valpha * g_PLLCal.SinTheta - g_PLLCal.f32Vbeta * g_PLLCal.CosTheta;
	g_PLLCal.f32Vq = g_PLLCal.f32Vbeta * g_PLLCal.SinTheta + g_PLLCal.f32Valpha * g_PLLCal.CosTheta;

	g_PLLCal.f32VdForward = g_PLLCal.f32Vd * g_CalcResult.Ave.VdqVbusForward;
	g_PLLCal.f32VqForward = g_PLLCal.f32Vq * g_CalcResult.Ave.VdqVbusForward;

}

/**********************************************************************
* FUNCION :  Adc_Getvalue1
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void InstantProtect(void)
{
	static Uint16 OVPTimecnt = 0;
	static Uint16 OCPTimecnt = 0;
	static Uint16 InputOCPTimecnt = 0;
	static Uint16 s_u16BusOVPCntEnter = 0;
	static Uint16 s_u16BusUnbanCntEnter = 0;
	static Uint16 s_u16SWOCPCntEnter = 0;
//	float32 f32OCPValue;
//	float32 f32ICPValue;

//	f32OCPValue = g_Rated.f32OutputCurrentPeak + 6;
//	f32ICPValue = g_Rated.f32InputCurrent + 5;

	if(0x01 == g_GridManager.Lvrt.u16Enable)
	{
		LvrtVdcalc();
	}
	else
	{
		g_GridManager.Lvrt.f32VdposFltered = 325;
		g_GridManager.Lvrt.u16StartFlag = 0;
	}

	if(	  (g_Instantaneous.f32IGridR > 50)||(g_Instantaneous.f32IGridR < -50)
		||(g_Instantaneous.f32IGridS > 50)||(g_Instantaneous.f32IGridS < -50)
		||(g_Instantaneous.f32IGridT > 50)||(g_Instantaneous.f32IGridT < -50))
	{
		OCPTimecnt++;
		if(OCPTimecnt>3)
		{
			OCPTimecnt = 0;	
			MpptReg.u16BoostOffFlag =1;
			MpptReg1.u16BoostOffFlag =1;
			MpptReg2.u16BoostOffFlag =1;
			PWMOutputsDisable();
			M_GRID_RELAY_OFF = 1;
			if(0 == g_SysFaultMessage.bit.SwOCPInstant)
			{
				g_SysFaultMessage.bit.SwOCPInstant = 1;
				s_u16SWOCPCntEnter++;
				if(s_u16SWOCPCntEnter >= 3)
				{
					s_u16SWOCPCntEnter = 0;
					g_SysFaultMessage.bit.unrecoverOCPInstant = 1;
				}
			}
		}
	}
	else
	{
		OCPTimecnt =0;
	}
	if(	  (g_Instantaneous.f32VGridR > 400)||(g_Instantaneous.f32VGridR < -400)
		||(g_Instantaneous.f32VGridS > 400)||(g_Instantaneous.f32VGridS < -400)
		||(g_Instantaneous.f32VGridT > 400)||(g_Instantaneous.f32VGridT < -400)	) 
	{
		OVPTimecnt++;
		if(OVPTimecnt>3)
		{
			OVPTimecnt = 0;	
			g_SysFaultMessage.bit.GridOVP = 1;
			MpptReg.u16BoostOffFlag =1;
			MpptReg1.u16BoostOffFlag =1;
			MpptReg2.u16BoostOffFlag =1;
			PWMOutputsDisable();
			M_GRID_RELAY_OFF = 1;
		}	
	}
	else
	{
		OVPTimecnt =0;
	}
	if(g_Instantaneous.f32VbusP > g_Instantaneous.f32VbusN)
	{
		g_Instantaneous.f32VbusPNDelta = g_Instantaneous.f32VbusP - g_Instantaneous.f32VbusN;
	}
	else
	{
		g_Instantaneous.f32VbusPNDelta = g_Instantaneous.f32VbusN - g_Instantaneous.f32VbusP;
	}

	if(g_Instantaneous.f32VbusPN > 980)
	{
		MpptReg.u16BoostOffFlag =1;
		MpptReg1.u16BoostOffFlag =1;
		MpptReg2.u16BoostOffFlag =1;
		PWMOutputsDisable();
		M_GRID_RELAY_OFF = 1;
		if(0 == g_SysFaultMessage.bit.BusOVP)
		{
			g_SysFaultMessage.bit.BusOVP = 1;
			s_u16BusOVPCntEnter++;
			if(s_u16BusOVPCntEnter >= 4)
			{
				s_u16BusOVPCntEnter = 0;
				g_SysFaultMessage.bit.unrecoverBusOVP = 1;
			}
		}
	}
	if(g_Instantaneous.f32VbusPNDelta > 100)
	{
		MpptReg.u16BoostOffFlag =1;
		MpptReg1.u16BoostOffFlag =1;
		MpptReg2.u16BoostOffFlag =1;
		PWMOutputsDisable();
		M_GRID_RELAY_OFF = 1;
		if(0 == g_SysFaultMessage.bit.VbusUnbalance)
		{
			g_SysFaultMessage.bit.VbusUnbalance = 1;
			s_u16BusUnbanCntEnter++;
			if(s_u16BusUnbanCntEnter >= 4)
			{
				s_u16BusUnbanCntEnter = 0;
				g_SysFaultMessage.bit.unrecoverVbusUnbalance = 1;
			}
		}
	}
	if((g_Instantaneous.f32IPV1 > 29)||(g_Instantaneous.f32IPV2 > 29))////40||(GetRealValue.f32IPV11>20)||(GetRealValue.f32IPV21>20))
	{
		InputOCPTimecnt++;
		if(InputOCPTimecnt>3)
		{
			g_SysFaultMessage.bit.unrecoverIPVInstant = 1;
			MpptReg.u16BoostOffFlag =1;
			MpptReg1.u16BoostOffFlag =1;
			MpptReg2.u16BoostOffFlag =1;
			PWMOutputsDisable();
			M_GRID_RELAY_OFF = 1;
		}
	}
	else
	{
		InputOCPTimecnt = 0;
	}

}
//===========================================================================
// No more.
//===========================================================================
