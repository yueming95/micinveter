/***********************************************************************
 *    FILENAME : SC_MasterStates.c
 *
 *    PURPOSE  : 
 *    HISTORY  :
 *           DATE          VERSION      AUTHOR        NOTE
 ************************************************************************/
#include "SC_master.h"

/**********************************************************************
* FUNCION :  Tsk_F_MasterStates
* PURPOSE : 
* CALLED BY: DSP/BIOS  kernel task  every 2ms 
**********************************************************************/
void Tsk_F_MasterStates (void)
{
	while (1) // start of task main loop 
	{
		SEM_pend(&SEM_TskMasterStatesReady, SYS_FOREVER);    
		
		DigitalIOCheck();

		switch (g_SysCurrentState)
		{
			case WaitState:
				MasterWaitState();
			break;  
			case CheckState:
				MasterCheckState();
			break;
			case NormalState:   
				MasterNormalState();
			break;   
			case FaultState:    
				MasterFaultState();
			break;
			case PermanentState:        
				MasterPermanentState();
			break; 
			case FlashState:
            	MasterFlashState();
			break;
			default:
				MasterPermanentState(); 
			break;	  
		}//end of switch

		LvrtManager();

	} // end of task main loop 	
}
		
/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void MasterWaitState(void)
{
	if(ReadyToEnterFlashState())
	{
		EnterFlashState();
	}
	else if(ReadyToEnterPermanentState())
    {
        EnterPermanentState();
	}
	else if(ReadyToEnterFaultState())
    {
        EnterFaultState();
	}
	else if(ReadyToEnterCheckState())
	{
		EnterCheckState();
	}
	else
	{
		if(   0 == g_StateCheck.bit.SafetyCountDownSetOver
		   && 1 == g_StateCheck.bit.SafetySetOK				)
		{
			//g_u16SafetyCountDown = g_SafetyConnect.u16ReconnectTime;
			g_StateCheck.bit.SafetyCountDownSetOver = 1;
		}

		MpptReg.u16BoostOffFlag = 1;
		MpptReg1.u16BoostOffFlag = 1;
		MpptReg2.u16BoostOffFlag = 1;

		BoostLoopInit();
	}
}

/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void MasterCheckState(void)
{
	if(ReadyToEnterFlashState())
	{
		EnterFlashState();
	}
	else if(ReadyToEnterPermanentState())
    {
        EnterPermanentState();
	}
	else if(ReadyToEnterFaultState())
    {
        EnterFaultState();
	}
	else if(ReadyToEnterWaitState())
	{
//		g_u16SafetyCountDown = 300;
		EnterWaitState();
	}
	else if(ReadyToEnterNormalState())
	{
		EnterNormalState();
	}
	else
	{		
		g_u16SafetyCountDown = g_SafetyConnect.u16ConnectTime;		
		VbusSoftStart();
		if(1 == g_SlaveStateCheck.bit.RelayCheckOver)
		{
			M_GRID_RELAY_OFF = 1;
			g_StateCheck.bit.MasterRelayOFF = 1;
		}

		if(1 == g_SlaveStateCheck.bit.SlaveRelayON)
		{
			if((g_PLLCal.f32Theta>4.1)&&(g_PLLCal.f32Theta<4.8))
			{
				if(0 == g_StateCheck.bit.InvSoftStartFalg)
				{	
					HWI_disable();
					EPwm1Regs.AQCSFRC.bit.CSFB = 0x0;
					EPwm2Regs.AQCSFRC.bit.CSFB = 0x0;
					EPwm3Regs.AQCSFRC.bit.CSFB = 0x0;
					EPwm4Regs.AQCSFRC.bit.CSFB = 0x0;
					EPwm5Regs.AQCSFRC.bit.CSFB = 0x0;
					EPwm6Regs.AQCSFRC.bit.CSFB = 0x0;
					DelayUs(5);		
					EPwm1Regs.AQCSFRC.bit.CSFA = 0x0;
					EPwm2Regs.AQCSFRC.bit.CSFA = 0x0;
					EPwm3Regs.AQCSFRC.bit.CSFA = 0x0;
					EPwm4Regs.AQCSFRC.bit.CSFA = 0x0;
					EPwm5Regs.AQCSFRC.bit.CSFA = 0x0;
					EPwm6Regs.AQCSFRC.bit.CSFA = 0x0;
					
					g_StateCheck.bit.InvSoftStartFalg = 1;
					HWI_enable();	
				}
			}
		}

		if(1 == g_StateCheck.bit.InvSoftStartOver)
		{
			M_GRID_RELAY_ON = 1;
			g_StateCheck.bit.MasterRelayOFF = 0;
			if(g_u16RelayDelayCnt > 0)
			{
				g_u16RelayDelayCnt--;
			}
			else
			{
				g_u16RelayDelayCnt = 0;
			}
		}
	}
}		
/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void MasterNormalState(void)
{	
	if(ReadyToEnterFlashState())
	{
		EnterFlashState();
	}
	else if(ReadyToEnterPermanentState())
    {
        EnterPermanentState();
	}
	else if(ReadyToEnterFaultState())
    {
        EnterFaultState();
	}
	else if(ReadyToEnterWaitState())
	{
//		g_u16SafetyCountDown = 300;
		EnterWaitState();
	}
	else
	{
		if(1 == g_MPPTState.bit.FactoryMode)
		{
			if((g_BusPIDCal.f32IdRef_DCSource + 0.002)< g_BusPIDCal.f32IdRefMax)
			{
				g_BusPIDCal.f32IdRef_DCSource += 0.002;  
			}
			else if((g_BusPIDCal.f32IdRef_DCSource - 0.002)> g_BusPIDCal.f32IdRefMax)
			{
				g_BusPIDCal.f32IdRef_DCSource -=0.002;	
			}
			else
			{
				g_BusPIDCal.f32IdRef_DCSource = g_BusPIDCal.f32IdRefMax;
			}
		}

		g_GridManager.Lvrt.u16LvrtNormalDelay ++;
		if(g_GridManager.Lvrt.u16LvrtNormalDelay > 5000)
		{
			g_GridManager.Lvrt.u16LvrtNormalDelay = 5000;	
		}
	}
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  1-BusRef缓升
*            2-Check状态确定Boost是否工作
* CALLED BY: 仅应在Check状态执行
**********************************************************************/
void VbusSoftStart(void)
{
	static Uint16 StartDelay =0;

	if(0 == g_StateCheck.bit.BusOverRef)
	{	
		StartDelay ++;
		if(StartDelay >10)
		{
			StartDelay =0;
			if(g_BusRefCal.f32BusRef < g_BusRefCal.f32GridPeakVolt+40)
			{
				g_BusRefCal.f32BusRef += 0.5;
			
				if(1 == g_MPPTState.bit.FactoryMode)	// DC Source Mode or Parallel input
				{
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
				else if(PARALLEL_IN == g_MPPTState.bit.InputMode)
				{
					MpptReg.u16BoostOffFlag = 0;
					MpptReg1.u16BoostOffFlag = 0;
					MpptReg2.u16BoostOffFlag = 0;
				}
				else //(INDEPENDENT_IN == g_MPPTState.bit.InputMode)
				{
					if(1 == g_MPPTState.bit.PV1ON)
					{
						MpptReg1.u16BoostOffFlag = 0;
					}
					if(1 == g_MPPTState.bit.PV2ON)
					{
						if(1 == g_MPPTState.bit.PV1ON)
						{
							MpptReg2.u16BoostOffFlag = 1;
						}
						else
						{
							MpptReg2.u16BoostOffFlag = 0;
						}
					}
				}
			}
			else
			{
				if(g_CalcResult.Ave.f32Vbus > g_BusRefCal.f32GridPeakVolt+30)
				{
					g_StateCheck.bit.BusOverRef = 1;
					M_GRID_RELAY_ON = 1;
				}
			}
		}
	}
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void MasterFaultState(void) 
{ 
	if(ReadyToEnterFlashState())
	{
		EnterFlashState();
	}
	else if(ReadyToEnterPermanentState())
    {
        EnterPermanentState();
	}
	else
    {
		if(FaultBackCheck())
        {
//			g_u16SafetyCountDown = 10;
            EnterWaitState();
		}
		else
		{
			MpptReg.u16BoostOffFlag = 1;
			MpptReg1.u16BoostOffFlag = 1;
			MpptReg2.u16BoostOffFlag = 1;
			BoostLoopInit();

			HWI_disable();
			PWMOutputsDisable();
			HWI_enable();
		}
	}
}

/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void MasterPermanentState(void)
{
	if(ReadyToEnterFlashState())
	{
		EnterFlashState();
	}
	else
	{
		MpptReg.u16BoostOffFlag = 1;
		MpptReg1.u16BoostOffFlag = 1;
		MpptReg2.u16BoostOffFlag = 1;
		BoostLoopInit();
		HWI_disable();
		PWMOutputsDisable();
		HWI_enable();
	}
}

/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void MasterFlashState(void)
{
	Uint16 Status;
	FLASH_ST FlashStatus;
	 
//	PWMOFFandRelayDisconnect();

    HWI_disable();
	SWI_disable();

	Status = InitPLLForFlashAPI();
	if(Status != STATUS_SUCCESS) 
	{
	   return;
	}
 
 	Status = CsmUnlock();
	if(Status != 0x01) 
	{
	   return;
	}
	Flash_CPUScaleFactor = SCALE_FACTOR;
	  	   
	Status = Flash_Erase(SECTORB, &FlashStatus);	  	 
	if(STATUS_SUCCESS==Status)//0 is success?
    {
	   EnableDog();
       REST_SYS();
    }     
}

/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
Uint16 ReadyToEnterWaitState(void)
{
	// 如果PV电压过，就进入wait
	if(((0 == g_MPPTState.bit.PV1ON)&&(0 == g_MPPTState.bit.PV2ON))||(1 == g_StateCheck.bit.BusUVP)||(0x66 == g_SafetyActive.RemoteOnOff))
	{
		if(0x66 == g_SafetyActive.RemoteOnOff)
		{
			g_StateCheck.bit.RemoteOff = 1;
		}
		return 1;
	}
	else
	{
		return 0;
	}
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
Uint16 ReadyToEnterCheckState(void)
{
	if(   1 == g_StateCheck.bit.SafetySetOK
	   && 0x55 == g_SafetyActive.RemoteOnOff
	   && 1 == g_StateCheck.bit.SafetyCountDownSetOver
	   && 1 == g_StateCheck.bit.ADOffsetCalibrateOver
	   && 1 == g_StateCheck.bit.GFCI50mACheckOK
	   && 1 == g_StateCheck.bit.PhaseSeqCheckOver
	   && 1 == g_SlaveStateCheck.bit.PVIsoCheckOver
	   && 0 == g_StateCheck.bit.BusUVP
	   && 0 == g_u16SafetyCountDown
	   && ((1 == g_MPPTState.bit.PV1ON)||(1 == g_MPPTState.bit.PV2ON))//
	  )
	{
		g_StateCheck.bit.RemoteOff = 0;
		return 1;
	}
	else
	{
		return 0;
	}
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
Uint16 ReadyToEnterNormalState(void)
{
	if(   1 == g_StateCheck.bit.BusOverRef
	   && 1 == g_SlaveStateCheck.bit.RelayCheckOver
	   && 1 == g_StateCheck.bit.InvSoftStartFalg
	   && 0 == g_u16RelayDelayCnt		)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
Uint16 ReadyToEnterFaultState(void)
{
	if(FaultCheck())
		return 1;
	else
		return 0;
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
Uint16 ReadyToEnterPermanentState(void)
{
//	if((g_SysCurrentStateSlave == PermanentState) || PermanentFaultCheck())
	if(PermanentFaultCheck())
		return 1;
	else
		return 0;
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
Uint16 ReadyToEnterFlashState(void)
{
	if(g_StateCheck.bit.FlashFlag == 1)
		return 1;
	else
		return 0;
}

/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void EnterWaitState(void)
{
	g_SysCurrentState = WaitState; 
	PWMOFFandRelayDisconnect();

	g_StateCheck.bit.ADOffsetCalibrateOver = 0;
	g_StateCheck.bit.SafetyCountDownSetOver = 0;
	g_StateCheck.bit.GFCI50mACheckOK = 0;
	g_StateCheck.bit.PhaseSeqCheckOver = 0;
	g_ADConvert.offset.f32IPV1 = 0;
	g_ADConvert.offset.f32IPV2 = 0;
}

/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void EnterCheckState(void)
{
	g_SysCurrentState = CheckState;
	GridManagerInit();
	BoostLoopInit();
	M_INVPWM_UNLOCK;
	g_StateCheck.bit.BusOverRef = 0;	// bus电压未升
	g_StateCheck.bit.InvSoftStartOver =0;
	g_StateCheck.bit.MasterRelayOFF = 0;

	g_u16RelayDelayCnt = 200;

	g_BusRefCal.f32BusRef = g_CalcResult.Ave.f32Vbus ;
	g_Mppt1Calc.f32PvOpenVolt = g_CalcResult.Ave.f32VPV1;
	g_Mppt1Calc.f32PvVoltRef = g_Mppt1Calc.f32PvOpenVolt * 0.995;	
	g_Mppt2Calc.f32PvOpenVolt = g_CalcResult.Ave.f32VPV2;
	g_Mppt2Calc.f32PvVoltRef = g_Mppt2Calc.f32PvOpenVolt * 0.995;

	g_MpptCalc.f32PvOpenVolt = g_CalcResult.Ave.f32VPV1;
	g_MpptCalc.f32PvVoltRef = g_MpptCalc.f32PvOpenVolt * 0.99;	

	g_ADConvert.offset.f32IPV1 = g_CalcResult.Ave.f32IPV1;
	g_ADConvert.offset.f32IPV2 = g_CalcResult.Ave.f32IPV2;
	g_SafetyDCI.f32DciTestInjection = 0;
	g_SaftyConfig.bit.DCITestEnable = 0;

}

/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void EnterNormalState(void)
{
	g_SysCurrentState = NormalState;
//	PWMOutputsEnable();
	g_GridManager.InvQ.f32Tanphi = 0;
//	BoostLoopInit();
//	g_GridManager.PFreq.u16StartFlag = 0;	
//	InverterStageInit();
	g_Mppt1Calc.f32PvOpenVolt = g_CalcResult.Ave.f32VPV1;
	g_Mppt1Calc.f32PvVoltRef = g_Mppt1Calc.f32PvOpenVolt * 0.99;	

	g_Mppt2Calc.f32PvOpenVolt = g_CalcResult.Ave.f32VPV2;
	g_Mppt2Calc.f32PvVoltRef = g_Mppt2Calc.f32PvOpenVolt * 0.99;
		
	g_MpptCalc.f32PvOpenVolt = g_CalcResult.Ave.f32VPV1;
	g_MpptCalc.f32PvVoltRef = g_MpptCalc.f32PvOpenVolt * 0.99;	

	g_MPPTState.bit.InitFlag1 = 1;
	g_MPPTState.bit.InitFlag2 = 1;
	g_MPPTState.bit.InitFlag = 1;
	g_BusRefCal.f32BusRef = g_CalcResult.Ave.f32Vbus;
	g_BusRefCal.f32StandbusVolt = g_BusRefCal.f32GridPeakVolt + 40;

	g_MpptCalc.f32MPPT1Limit = 10500;
	g_MpptCalc.f32MPPT2Limit = 10500;
//	g_MpptCalc.f32MPPTLimit = 20000;
	g_MpptCalc.f32PowerErrOld = 0;
	g_MpptCalc.f32PowerErrNew = 0;
	g_MpptCalc.f32PowerShiftVolt = 0;
	g_MpptCalc.u16OutputPowerLimitFlag = 0;
}

/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void EnterFaultState(void)
{
	g_SysCurrentState = FaultState;
	PWMOFFandRelayDisconnect();
}

/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void EnterPermanentState(void)
{
	g_SysCurrentState = PermanentState;
	PWMOFFandRelayDisconnect();
}

/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void EnterFlashState(void)
{
	g_SysCurrentState = FlashState;
	PWMOFFandRelayDisconnect();
	DelayUs(12000);
}

/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void PWMOFFandRelayDisconnect(void)
{
	HWI_disable();
	PWMOutputsDisable();
	HWI_enable();
	M_GRID_RELAY_OFF = 1;
	M_INVPWM_LOCK;
//	g_Boost1PIDCalc.U16BoostState = BOOST_OFF;
	MpptReg.u16BoostOffFlag = 1;
	MpptReg1.u16BoostOffFlag = 1;
	MpptReg2.u16BoostOffFlag = 1;
	ECap1Regs.CAP4 = 0;
	ECap2Regs.CAP4 = 0;
	BoostLoopInit();
}

/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
Uint16 PermanentFaultCheck(void)
{
	if(   0 == g_SysFaultMessage.Word.unrecover0
	   && 0 == g_SysFaultMessage.Word.unrecover1
	   && 0 == g_SlaveSysFaultMessage.Word.unrecover0
	   && 0 == g_SlaveSysFaultMessage.Word.unrecover1)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
Uint16 FaultCheck(void)
{
	if(   0 == g_SysFaultMessage.Word.byte0
	   && 0 == g_SysFaultMessage.Word.byte1	
	   && 0 == g_SysFaultMessage.Word.byte2	
	   && 0 == g_SysFaultMessage.Word.byte3	
	   && 0 == g_SysFaultMessage.Word.byte4	
	   && 0 == g_SysFaultMessage.Word.byte5	
	   && 0 == g_SysFaultMessage.Word.byte6	
	   && 0 == g_SysFaultMessage.Word.byte7
	   && 0 == g_SlaveSysFaultMessage.Word.byte0
	   && 0 == g_SlaveSysFaultMessage.Word.byte1	
	   && 0 == g_SlaveSysFaultMessage.Word.byte2	
	   && 0 == g_SlaveSysFaultMessage.Word.byte3	
	   && 0 == g_SlaveSysFaultMessage.Word.byte4	
	   && 0 == g_SlaveSysFaultMessage.Word.byte5	
	   && 0 == g_SlaveSysFaultMessage.Word.byte6	   
	   && 0 == g_SlaveSysFaultMessage.Word.byte7)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
Uint16 FaultBackCheck(void)
{
	return !(FaultCheck());
}

//===========================================================================
// No more.
//===========================================================================

