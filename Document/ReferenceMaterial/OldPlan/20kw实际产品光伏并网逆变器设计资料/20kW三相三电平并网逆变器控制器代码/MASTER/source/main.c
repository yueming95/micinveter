/***********************************************************************
 *    FILENAME : main.c
 *
 *    PURPOSE  : This document  include  initializtion of CPU system , 
 *               ad sampling, three phase pwm output,Peripheral Interrupt Expansion equal,
 *               system default parameter  defined , state change funtion,
 *               sofeware period  interrupt
 *    HISTORY  :
 *           DATE          VERSION      AUTHOR        NOTE
 *		
 * 	Software Development Evironment: 
 *	-- CCS 3.3.83.19
 * 	-- BIOS 5.41.02.14
 *	-- Code Generation Tool v5.2.5
 *
 ************************************************************************/
#include "SC_master.h"

/**********************************************************************
* FUNCION : 
* PURPOSE : 
* INPUT :  
* RETURN : void
* CALLS: void
* CALLED BY:
**********************************************************************/
void MemCopy(Uint16 *SourceAddr, Uint16* SourceEndAddr, Uint16* DestAddr)
{
    while(SourceAddr < SourceEndAddr)
    { 
       *DestAddr++ = *SourceAddr++;
    }
    return;
}
/**********************************************************************
* FUNCION : main loop
* PURPOSE : initializtion of CPU system , ad sampling, three phase pwm output, 
*           Peripheral Interrupt Expansion equal,and  system default parameter defined 
* INPUT : NA
* RETURN : void
* CALLS: void
* CALLED BY:
**********************************************************************/
void main(void)
{
	InitSysCtrl();
    InitPieCtrl();		 				
	DINT;
	InitGpio();	// AC Relay has been disabled (GPIO72 = 0)

	memcpy(	&g_u16InitFlash_InRAM_runstart,
			&g_u16InitFlash_InRAM_loadstart,
			&g_u16InitFlash_InRAM_loadend - &g_u16InitFlash_InRAM_loadstart);

	InitFlash();
	
	memcpy(	&g_u16ControlLoopInRAM_runstart,
			&g_u16ControlLoopInRAM_loadstart,
			&g_u16ControlLoopInRAM_loadend - &g_u16ControlLoopInRAM_loadstart);
	
	MemCopy(&Flash28_API_LoadStart, &Flash28_API_LoadEnd, &Flash28_API_RunStart);
    // We must also copy required user interface functions to RAM. 
    MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);

	InitAdc();
	DelayUs(5000);
	InitEPwm();
    InitECap();
	ChoiceKindsMachine();
	SystemInitial();
	InitSpi();
	InitSci();

 	DelayUs(15000);

// Enable global Interrupts and higher priority real-time debug events:
	EINT;   // Enable Global interrupt INTM
	ERTM;   // Enable Global realtime interrupt DBGM 
} //end of Main 

/**********************************************************************
* FUNCION : GridPeriodCalcTsk
* PURPOSE : Calculate average values every grid period.
*			Calculate RMS values every grid period, including VGrid_rms,IGrid_rms,GFCI_rms,DCi,
*           VGridCheck,GFCICheck,DciCheck,GFCICheck.
*			Active MPPT Task every 25 grid periods.
* HISTORY :  DATE             AUTHOR        NOTE
**********************************************************************/
void Tsk_F_GridPeriodCalc(void)
{
	static Uint16 s_u16MPPTcnt = 0;
	static 	float32 s_f32WATT_sum1, s_f32WATT_sum2;
	float32 f32CntReci;

	while(1)
	{
		SEM_pend(&SEM_TskGridPeriodCalcReady, SYS_FOREVER);	// Trip every grid period
		
		if(0 == g_ADSum.u16FullGridPeriodCnt)
		{
			continue;
		}

		// Average and RMS calculate every grid period
		f32CntReci = 1 / (float32)g_ADSum.u16FullGridPeriodCnt;
		
		VPV_ave_Calc(f32CntReci);
		IPV_ave_Calc(f32CntReci);
		VBUS_ave_Calc(f32CntReci);
		VPVCheck();
		IPVCheck();
		VBUSCheck(); 
		BusVoltZeroCheck();
		SWOCPRecover();
		VGrid_ave_Calc(f32CntReci);
		IGrid_ave_Calc(f32CntReci);
		GFCI_ave_Calc(f32CntReci);
		DCI_ave_Calc(f32CntReci);

		if(1 == g_StateCheck.bit.PLLOver)
		{
			ADOffsetCalibration();
			GridVoltADOffsetCheck();
			GridCurrentADOffsetCheck();
			DCIADOffsetCheck();
			GFCISelfCheck(); 
		}

		AntiIslandingCalc();
		VGrid_rms_Calc(f32CntReci);
		VGridLine_rms_Calc(f32CntReci);
		IGrid_rms_Calc(f32CntReci);
		GFCI_rms_Calc(f32CntReci);
		GridFreqCalc(f32CntReci);

//		if(0 == g_GridManager.Lvrt.u16StartFlag)
//		{
//			GridVoltCheck();
//			GridFreqCheck();
//			GFCICheck();
//			DCICheck();
//		}
		GridCurrentCheck();

		StandBusUpdate();
		if(0 == g_MPPTState.bit.FactoryMode)
		{
			if(INDEPENDENT_IN == g_MPPTState.bit.InputMode)
			{
				if(1 == g_MPPTState.bit.PV1ON)
				{
					Mppt1Initial();
				}
				if(1 == g_MPPTState.bit.PV2ON)
				{
					Mppt2Initial();
				}
			}
			else
			{
				MpptInitial();
			}
		}
		PhaseSeqCheck();
		BoostControlStrategy();

		Power_Calc(f32CntReci);
		OPLoadLimit20ms();
		OILoadLimit20ms();
		OutputPowerLimitMPPT();				
		// Active MPPT task
		if(NormalState == g_SysCurrentState)
		{
//			OPLoadLimit20ms();
//			OILoadLimit20ms();
			s_u16MPPTcnt++;
			if(s_u16MPPTcnt >= 6)		// Trip every 25 grid period
			{
				s_f32WATT_sum1 += g_CalcResult.Power.f32Input1Watt; 
				s_f32WATT_sum2 += g_CalcResult.Power.f32Input2Watt; 
				if(s_u16MPPTcnt >= 25)		//25 Trip every 25 grid period
				{
				//	g_MpptCalc.f32PVWattCurrent = (s_f32WATT_sum1 + s_f32WATT_sum2) * 0.05;//
					g_Mppt1Calc.f32PVWattCurrent = s_f32WATT_sum1 * 0.05;//0.05
					g_Mppt2Calc.f32PVWattCurrent = s_f32WATT_sum2 * 0.05;//
					g_MpptCalc.f32PVWattCurrent = g_Mppt1Calc.f32PVWattCurrent + g_Mppt2Calc.f32PVWattCurrent;
					
					s_f32WATT_sum1 = 0; 
					s_f32WATT_sum2 = 0;
					s_u16MPPTcnt = 0;
					SEM_post(&SEM_TskMPPTReady); 
				}
			}													
		}
		else
		{
			s_u16MPPTcnt = 0;
			s_f32WATT_sum1 = 0;
			s_f32WATT_sum2 = 0;
			g_Mppt1Calc.f32PVWattCurrent = 0;
			g_Mppt2Calc.f32PVWattCurrent = 0;
			g_MpptCalc.f32PVWattCurrent = 0;
		}
	}
}

/**********************************************************************
* FUNCION :  Tsk_F_SpiComm
* PURPOSE : 
* CALLED BY: DSP/BIOS  kernel task  every 20ms 
**********************************************************************/
void Tsk_F_SpiComm(void)
{	
	static Uint16 ExitSpiFaultCnt = 0;
	while(1)
	{
		SEM_pend(&SEM_TskSpiCommReady, SYS_FOREVER);
		if(0 == g_GridManager.Lvrt.u16StartFlag)
		{
			GridVoltCheck();
			GridFreqCheck();
			GFCICheck();
			DCICheck();
		}			
		SetMasterValue();
		SpiChange(g_u16SpiTxBuf, g_u16SpiRxBuf, SPI_COM_LENGTH);
		SpiMasterReceiveData();

		if(0 == g_SysFaultMessage.bit.SpiCommLose)
		{
			if(g_u16SpiFaultCnt > 60)
			{
				g_SysFaultMessage.bit.SpiCommLose = 1;
			}
		}
		else
		{
			if(g_u16SpiFaultCnt < 1)
			{
				ExitSpiFaultCnt	++;
				if(ExitSpiFaultCnt > 250)
				{
					g_SysFaultMessage.bit.SpiCommLose = 0;
					ExitSpiFaultCnt = 0;
				}		
			}
		}
		g_u16SpiFaultCnt++;
	}
}

/**********************************************************************
* FUNCION :  Tsk_F_MPPT
* PURPOSE :  
* CALLED BY:  
**********************************************************************/
void Tsk_F_MPPT(void)
{
	while(1)
	{
		if (TRUE == SEM_pend(&SEM_TskMPPTReady, SYS_FOREVER) )
		{
			if(0 == g_MPPTState.bit.FactoryMode)
			{
				if(INDEPENDENT_IN == g_MPPTState.bit.InputMode)
				{
					if(1 == g_MPPTState.bit.PV1ON)
					{
					//	Mppt1Initial();
						if(0 == g_MpptCalc.u16OutputPowerLimitFlag)
						{
							MPPT1Track();
						}
					}
					if(1 == g_MPPTState.bit.PV2ON)
					{
					//	Mppt2Initial();
						if(0 == g_MpptCalc.u16OutputPowerLimitFlag)
						{
							MPPT2Track();
						}
					}
				}
				else
				{
					//MpptInitial();
					if(0 == g_MpptCalc.u16OutputPowerLimitFlag)
					{
						MPPTTrack();
					}
				}
			}
		}
	}	
}

/**********************************************************************
* FUNCION :  Tsk_F_PowerCalc
* PURPOSE : 
* CALLED BY: DSP/BIOS  kernel task  every 500ms 
**********************************************************************/	
void Tsk_F_PowerCalc(void)
{
	static Uint16 s_u16_3SecondCnt = 0;
	static Uint16 s_u16_1SecondCnt = 0;
#ifdef _IPOMS_ 
	extern Uint8	u8IPOMS_CommandBuffer0[50];
	extern Uint8	*pIPOMS_CommandIn0;
   
    pIPOMS_CommandIn0 = u8IPOMS_CommandBuffer0;	// for software oscilloscope
#endif

	while(1)
    {
        SEM_pend(&SEM_TskPowerCalcReady, SYS_FOREVER);

    	// power calculation
		Power_Display_Calc();

		if(s_u16_1SecondCnt < 1)
		{	
			s_u16_1SecondCnt++;
		}
		else
		{	// executed every 1S
			s_u16_1SecondCnt = 0;
			
			// 电网电压滑动平均值的计算和检查
			s_u16_3SecondCnt++;
			if(3 == s_u16_3SecondCnt)
			{
				s_u16_3SecondCnt = 0;
				VGrid10minMovingAve_Calc();
				if(1 == g_SaftyConfig.bit.OVPMovEnable)
				{
					VGrid10minMovingAveCheck();
				}
			}

			// 头3秒
			if(g_u16First4S > 0)
			{
				g_u16First4S--;
			}
			else
			{
				g_StateCheck.bit.PLLOver = 1;
			}
			if(1 == g_MPPTState.bit.FactoryMode)
			{
				g_u16SafetyCountDown = 1;
				g_f32PowerLoadSpeed = 150;
			}
			// 安规倒计时
			if((g_u16SafetyCountDown > 0) && (g_SysCurrentState == WaitState) && (g_CalcResult.Ave.f32Vbus > 200))
			{
				if((1 == g_StateCheck.bit.SafetySetOK)&&(((1 == g_MPPTState.bit.PV1ON)||(1 == g_MPPTState.bit.PV2ON))))
				{
					g_u16SafetyCountDown--;
				}
			}
		}
		
#ifdef _IPOMS_    
		SCIa_IPOMS();	// software oscilloscope
#endif
    }
} 

/**********************************************************************
* FUNCION :  Tsk_F_SCIb_Update
* PURPOSE :  Software OSC for debug
* CALLED BY:
**********************************************************************/
void Tsk_F_SCIb_Update(void)
{
	while(1)
	{
		SEM_pend(&SEM_TskSCIbUpdateReady, SYS_FOREVER);
		// Judge if command is "FDSPM"
		// If is, change update flag in Flash
		// Reboot by watchdog
		Scib_Communication();
	}
}

/**********************************************************************
* FUNCION :  SEM post  when periodic   timerBase of 2ms   is ready
* PURPOSE :  SEM post  for  state changed
* CALLED BY:  DSP/BIOS kernel 2ms periodic  call 
**********************************************************************/ 
void TimeBase2msPRD(void)
{
   SEM_post(&SEM_TskMasterStatesReady);
}

/**********************************************************************
* FUNCION :  SEM post  when periodic   timerBase of 20ms   is ready
* PURPOSE :  SEM post  for  spi communication with slave CPU
* CALLED BY:  DSP/BIOS kernel 20ms periodic  call 
**********************************************************************/ 
void TimeBase20msPRD(void)
{
	SEM_post(&SEM_TskSpiCommReady);
}

/**********************************************************************
* FUNCION :  SEM post  when periodic   timerBase of 500ms   is ready
* PURPOSE :  SEM post  for  power calculation and software debug
* CALLED BY:  DSP/BIOS kernel 500ms periodic  call 
**********************************************************************/ 
void TimeBase500msPRD(void)
{
	SEM_post(&SEM_TskPowerCalcReady);
}

/**********************************************************************
* FUNCION : User Init
* PURPOSE : This is the user initialization file to be specified in the DSP/BIOS configuration file, System - Global Settings.
* INPUT :
*        void
* RETURN : 
*        void
* CALLS:
*        void
*
* CALLED BY: DSP/BIOS configuration file-Global Settings 
* 
*********************************************************************/
void UserInit(void)
{
// Section .trcdata is generated by DSP/BIOS.
// It must be copied from its load to its run address BEFORE main().
// It should be removed in release version if not necessary.
	memcpy(	&trcdata_runstart, 
			&trcdata_loadstart, 
			&trcdata_loadend - &trcdata_loadstart);
} //end of UserInit

//===========================================================================
// No more.
//===========================================================================
