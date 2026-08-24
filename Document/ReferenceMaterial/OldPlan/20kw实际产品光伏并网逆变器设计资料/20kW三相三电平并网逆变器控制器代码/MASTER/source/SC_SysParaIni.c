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
* FUNCION :  SystemInitial
* PURPOSE :  Global variables initializaion
* CALLED BY: main()
**********************************************************************/ 
void SystemInitial(void)
{
	g_SysCurrentState = WaitState;
	// AD Gain
	switch(g_InverterType)
	{
		case SolarLake10K:
			g_ADConvert.Defaultgain.f32IGridR = IGRID_MEASURE_GAIN_10K;
			g_ADConvert.Defaultgain.f32IGridS = IGRID_MEASURE_GAIN_10K;
			g_ADConvert.Defaultgain.f32IGridT = IGRID_MEASURE_GAIN_10K;
			g_ADConvert.Defaultgain.f32DciR = DCI_MEASURE_GAIN_10K;
			g_ADConvert.Defaultgain.f32DciS = DCI_MEASURE_GAIN_10K;                        
			g_ADConvert.Defaultgain.f32DciT = DCI_MEASURE_GAIN_10K;
			g_ADConvert.Defaultgain.f32IPV1 = IPV_MEASURE_GAIN_10K;
			g_ADConvert.Defaultgain.f32IPV2 = IPV_MEASURE_GAIN_10K;

			g_Rated.f32OutputCurrentRms = 15;
			g_Rated.f32OutputCurrentPeak = 22;
			g_Rated.f32HalfInputPower = 6750;
			g_Rated.f32InputPower = 10500;
			g_Rated.f32OutputPower = 10050;
			g_Rated.f32InputCurrent = 15.2;
			g_Rated.f32VinUnderDerated = 0.0428;//15/350;
			g_Rated.f32VinUnderVolt	= 350;
			g_Rated.i16TAmbDratingPoint = 72;
			g_Rated.i16TInvDratingPoint = 86;
			g_Rated.i16TNTCDratingPoint = 80;
		break;
		case SolarLake15K:
			g_ADConvert.Defaultgain.f32IGridR = IGRID_MEASURE_GAIN_15K;
			g_ADConvert.Defaultgain.f32IGridS = IGRID_MEASURE_GAIN_15K;
			g_ADConvert.Defaultgain.f32IGridT = IGRID_MEASURE_GAIN_15K;
			g_ADConvert.Defaultgain.f32DciR = DCI_MEASURE_GAIN_15K;
			g_ADConvert.Defaultgain.f32DciS = DCI_MEASURE_GAIN_15K;                        
			g_ADConvert.Defaultgain.f32DciT = DCI_MEASURE_GAIN_15K;
			g_ADConvert.Defaultgain.f32IPV1    = IPV_MEASURE_GAIN_15K;
			g_ADConvert.Defaultgain.f32IPV2    = IPV_MEASURE_GAIN_15K;

			g_Rated.f32OutputCurrentRms = 22;
			g_Rated.f32OutputCurrentPeak = 32;
			g_Rated.f32HalfInputPower = 10500;
			g_Rated.f32InputPower = 15700;
			g_Rated.f32OutputPower = 15050;
			g_Rated.f32InputCurrent = 21.2;
			g_Rated.f32VinUnderDerated = 0.0567;//21/370;
			g_Rated.f32VinUnderVolt	= 370;
			g_Rated.i16TAmbDratingPoint = 72;
			g_Rated.i16TInvDratingPoint = 86;
			g_Rated.i16TNTCDratingPoint = 75;
		break;
		case SolarLake17K:
			g_ADConvert.Defaultgain.f32IGridR = IGRID_MEASURE_GAIN_17K;
			g_ADConvert.Defaultgain.f32IGridS = IGRID_MEASURE_GAIN_17K;
			g_ADConvert.Defaultgain.f32IGridT = IGRID_MEASURE_GAIN_17K;
			g_ADConvert.Defaultgain.f32DciR = DCI_MEASURE_GAIN_17K;
			g_ADConvert.Defaultgain.f32DciS = DCI_MEASURE_GAIN_17K;                        
			g_ADConvert.Defaultgain.f32DciT = DCI_MEASURE_GAIN_17K;
			g_ADConvert.Defaultgain.f32IPV1    = IPV_MEASURE_GAIN_17K;
			g_ADConvert.Defaultgain.f32IPV2    = IPV_MEASURE_GAIN_17K;

			g_Rated.f32OutputCurrentRms = 25;
			g_Rated.f32OutputCurrentPeak = 36;
			g_Rated.f32HalfInputPower = 10500;
			g_Rated.f32InputPower = 17700;
			g_Rated.f32OutputPower = 17050;
			g_Rated.f32InputCurrent = 21.2;
			g_Rated.f32VinUnderDerated = 0.05;//21/420;
			g_Rated.f32VinUnderVolt	= 420;
			g_Rated.i16TAmbDratingPoint = 72;
			g_Rated.i16TInvDratingPoint = 86;
			g_Rated.i16TNTCDratingPoint = 72;
		break;
		case SolarLake20K:
			g_ADConvert.Defaultgain.f32IGridR = IGRID_MEASURE_GAIN_20K;
			g_ADConvert.Defaultgain.f32IGridS = IGRID_MEASURE_GAIN_20K;
			g_ADConvert.Defaultgain.f32IGridT = IGRID_MEASURE_GAIN_20K;
			g_ADConvert.Defaultgain.f32DciR = DCI_MEASURE_GAIN_20K;
			g_ADConvert.Defaultgain.f32DciS = DCI_MEASURE_GAIN_20K;                        
			g_ADConvert.Defaultgain.f32DciT = DCI_MEASURE_GAIN_20K;
			g_ADConvert.Defaultgain.f32IPV1  = IPV_MEASURE_GAIN_20K;
			g_ADConvert.Defaultgain.f32IPV2  = IPV_MEASURE_GAIN_20K;

			g_Rated.f32OutputCurrentRms = 29;
			g_Rated.f32OutputCurrentPeak = 42;
			g_Rated.f32HalfInputPower = 12000;
			g_Rated.f32InputPower = 21000;
			g_Rated.f32OutputPower = 20050;
			g_Rated.f32InputCurrent = 24.2;
			g_Rated.f32VinUnderDerated = 0.0557;//24/430;
			g_Rated.f32VinUnderVolt	= 430;
			g_Rated.i16TAmbDratingPoint = 74;
			g_Rated.i16TInvDratingPoint = 86;
			g_Rated.i16TNTCDratingPoint = 72;

		break;	
		default:
			g_ADConvert.Defaultgain.f32IGridR = IGRID_MEASURE_GAIN_20K;
			g_ADConvert.Defaultgain.f32IGridS = IGRID_MEASURE_GAIN_20K;
			g_ADConvert.Defaultgain.f32IGridT = IGRID_MEASURE_GAIN_20K;
			g_ADConvert.Defaultgain.f32DciR = DCI_MEASURE_GAIN_20K;
			g_ADConvert.Defaultgain.f32DciS = DCI_MEASURE_GAIN_20K;
			g_ADConvert.Defaultgain.f32DciT = DCI_MEASURE_GAIN_20K;
			g_ADConvert.Defaultgain.f32IPV1    = IPV_MEASURE_GAIN_20K;
			g_ADConvert.Defaultgain.f32IPV2    = IPV_MEASURE_GAIN_20K;

			g_Rated.f32OutputCurrentRms = 15;
			g_Rated.f32OutputCurrentPeak = 22;
			g_Rated.f32HalfInputPower = 6750;
			g_Rated.f32InputPower = 10500;
			g_Rated.f32OutputPower = 10050;
			g_Rated.f32InputCurrent = 15.2;
			g_Rated.f32VinUnderDerated = 0.0428;//15.1/350;
			g_Rated.f32VinUnderVolt	= 350;

			g_Rated.i16TAmbDratingPoint = 70;
			g_Rated.i16TInvDratingPoint = 86;
			g_Rated.i16TNTCDratingPoint = 70;


		break;
	}

	g_ADConvert.Defaultgain.f32VbusN = VBUS_MEASURE_GAIN;
	g_ADConvert.Defaultgain.f32VbusP = VBUS_MEASURE_GAIN;
	g_ADConvert.Defaultgain.f32VGridR = VGRID_MEASURE_GAIN;
	g_ADConvert.Defaultgain.f32VGridS = VGRID_MEASURE_GAIN;
	g_ADConvert.Defaultgain.f32VGridT = VGRID_MEASURE_GAIN;
	g_ADConvert.Defaultgain.f32VPV1 = VPV_MEASURE_GAIN;
	g_ADConvert.Defaultgain.f32VPV2 = VPV_MEASURE_GAIN;
	g_ADConvert.Defaultgain.f32GFCI = GFCI_MEASURE_GAIN;

	g_ADConvert.gain.f32IGridR = g_ADConvert.Defaultgain.f32IGridR;
	g_ADConvert.gain.f32IGridS = g_ADConvert.Defaultgain.f32IGridS;
	g_ADConvert.gain.f32IGridT = g_ADConvert.Defaultgain.f32IGridT;
	g_ADConvert.gain.f32DciR = g_ADConvert.Defaultgain.f32DciR;
	g_ADConvert.gain.f32DciS = g_ADConvert.Defaultgain.f32DciS;
	g_ADConvert.gain.f32DciT = g_ADConvert.Defaultgain.f32DciT;
	g_ADConvert.gain.f32IPV1 = g_ADConvert.Defaultgain.f32IPV1;
	g_ADConvert.gain.f32IPV2 = g_ADConvert.Defaultgain.f32IPV2;

	g_ADConvert.gain.f32VbusN = g_ADConvert.Defaultgain.f32VbusN;
	g_ADConvert.gain.f32VbusP = g_ADConvert.Defaultgain.f32VbusP;
	g_ADConvert.gain.f32VGridR = g_ADConvert.Defaultgain.f32VGridR;
	g_ADConvert.gain.f32VGridS = g_ADConvert.Defaultgain.f32VGridS;
	g_ADConvert.gain.f32VGridT = g_ADConvert.Defaultgain.f32VGridT;
	g_ADConvert.gain.f32VPV1 = g_ADConvert.Defaultgain.f32VPV1;
	g_ADConvert.gain.f32VPV2 = g_ADConvert.Defaultgain.f32VPV2;
	g_ADConvert.gain.f32GFCI = g_ADConvert.Defaultgain.f32GFCI;


	// AD Offset
	g_ADConvert.offset.f32DciR   = 0;
	g_ADConvert.offset.f32DciS   = 0;
	g_ADConvert.offset.f32DciT   = 0;
	g_ADConvert.offset.f32GFCI    = 0;
	g_ADConvert.offset.f32IGridR = 0;//0.33;
	g_ADConvert.offset.f32IGridS = 0;//0.77;
	g_ADConvert.offset.f32IGridT = 0;//0.81;
	g_ADConvert.offset.f32IPV1    = 0;//0.573;//0.483;//0.46;//0.629;//0.243;//0.042V--->0.04V	
	g_ADConvert.offset.f32IPV2    = 0;//0.643;//0.633;//0.63;//0.629;//0.243;
	g_ADConvert.offset.f32VbusN   = 0;
	g_ADConvert.offset.f32VbusP   = 0;
	g_ADConvert.offset.f32VGridR = 0;
	g_ADConvert.offset.f32VGridS = 0;
	g_ADConvert.offset.f32VGridT = 0;
	g_ADConvert.offset.f32VPV1    = 0;
	g_ADConvert.offset.f32VPV2    = 0;

	g_StateCheck.Word.byte0 = 0;
	g_StateCheck.Word.byte1 = 0;
	g_StateCheck.Word.byte2 = 0;
	g_StateCheck.Word.byte3 = 0;
	g_StateCheck.Word.byte4 = 0;
	g_StateCheck.Word.byte5 = 0;
	g_StateCheck.Word.byte6 = 0;
	g_StateCheck.Word.byte7 = 0;

	g_SlaveStateCheck.Word.byte0 = 0;

	g_SlaveSysFaultMessage.Word.byte0 = 0;
	g_SlaveSysFaultMessage.Word.byte1 = 0;
	g_SlaveSysFaultMessage.Word.byte2 = 0;
	g_SlaveSysFaultMessage.Word.byte3 = 0;
	g_SlaveSysFaultMessage.Word.byte4 = 0;
	g_SlaveSysFaultMessage.Word.byte5 = 0;
	g_SlaveSysFaultMessage.Word.byte6 = 0;
	g_SlaveSysFaultMessage.Word.byte7 = 0;
	g_SlaveSysFaultMessage.Word.unrecover0 = 0;
	g_SlaveSysFaultMessage.Word.unrecover1 = 0;

	g_SysFaultMessage.Word.byte0 = 0;
	g_SysFaultMessage.Word.byte1 = 0;
	g_SysFaultMessage.Word.byte2 = 0;
	g_SysFaultMessage.Word.byte3 = 0;
	g_SysFaultMessage.Word.byte4 = 0;
	g_SysFaultMessage.Word.byte5 = 0;
	g_SysFaultMessage.Word.byte6 = 0;
	g_SysFaultMessage.Word.byte7 = 0;
	g_SysFaultMessage.Word.unrecover0 = 0;
	g_SysFaultMessage.Word.unrecover1 = 0;

	g_SysFaultEnable.Word.byte0 = 0;
	g_SysFaultEnable.Word.byte1 = 0;
	g_SysFaultEnable.Word.byte2 = 0;
	g_SysFaultEnable.Word.byte3 = 0;
	g_SysFaultEnable.Word.byte4 = 0;
	g_SysFaultEnable.Word.byte5 = 0;
	g_SysFaultEnable.Word.byte6 = 0;
	g_SysFaultEnable.Word.byte7 = 0;
	g_SysFaultEnable.Word.unrecover0 = 1;
	g_SysFaultEnable.Word.unrecover1 = 1;

	//Protect enable

	g_SysFaultEnable.bit.BusOVP = 1;	// 
	g_SysFaultEnable.bit.BusUVP = 1;
	
	g_SysFaultEnable.bit.HwADFaultVGrid = 1;
	g_SysFaultEnable.bit.HwAcOCP = 1;
	g_SysFaultEnable.bit.HwADFaultIGrid = 1;
	g_SysFaultEnable.bit.HwADFaultDCI = 1;
	g_SysFaultEnable.bit.HwRChipFault = 1;
	g_SysFaultEnable.bit.HwAuxPowerFault = 1;
	g_SysFaultEnable.bit.HwBusOVP = 1;
	g_SysFaultEnable.bit.HwBoostOCP = 1;
	g_SysFaultEnable.bit.unrecoverHwAcOCP = 1;
	g_SysFaultEnable.bit.BusVoltZeroFault = 1;
	g_SysFaultEnable.bit.PvOVP = 1;
	g_SaftyConfig.bit.AIEnable =1;
	g_SaftyConfig.bit.GFCIEnable = 1;
	g_SysFaultEnable.bit.AcRmsOCP = 1;
	g_SysFaultEnable.bit.IacRmsUnbalance = 1;
	g_SysFaultEnable.bit.VbusUnbalance = 1;
	g_SysFaultEnable.bit.IpvUnbalance = 1;
	g_SysFaultEnable.bit.PvConfigSetWrong =1;

	g_SysCurrentState = WaitState;
	g_SysCurrentStateSlave = WaitState;
	
	g_Instantaneous.f32IGridR = 0;
	g_Instantaneous.f32IGridS = 0;
	g_Instantaneous.f32IGridT = 0;
	g_Instantaneous.f32VGridR = 0;
	g_Instantaneous.f32VGridS = 0;
	g_Instantaneous.f32VGridT = 0;

	g_Instantaneous.f32VPV1 = 0;
	g_Instantaneous.f32VPV2 = 0;
	g_Instantaneous.f32IPV1 = 0;
	g_Instantaneous.f32IPV2 = 0;
	g_Instantaneous.f32GFCI = 0;

	g_Instantaneous.f32VbusP = 0;
	g_Instantaneous.f32VbusN = 0;
	g_Instantaneous.f32VbusPN = 0;
	g_Instantaneous.f32VbusPNDelta = 0;
//----------------------------------------------------------------------------
	g_CalcResult.u16FullGridPeriodCnt = 0;

	g_CalcResult.Ave.f32VPV = 0;
	g_CalcResult.Ave.f32VPV1 = 0;
	g_CalcResult.Ave.f32VPV2 = 0;

	g_CalcResult.Ave.f32IPV1 = 0;
	g_CalcResult.Ave.f32IPV2 = 0;

	g_CalcResult.Ave.f32VbusP = 0;
	g_CalcResult.Ave.f32VbusN = 0;
	g_CalcResult.Ave.f32Vbus = 0;

	g_CalcResult.Ave.f32IGridR = 0;
	g_CalcResult.Ave.f32IGridS = 0;
	g_CalcResult.Ave.f32IGridT = 0;

	g_CalcResult.Ave.f32VGridR = 0;
	g_CalcResult.Ave.f32VGridS = 0;
	g_CalcResult.Ave.f32VGridT = 0;

	g_CalcResult.Ave.f32DciR = 0;
	g_CalcResult.Ave.f32DciS = 0;
	g_CalcResult.Ave.f32DciT = 0;

	g_CalcResult.Ave.f32GFCI = 0;

	g_CalcResult.Ave.f32FGrid = 0;
	g_CalcResult.Ave.f32FGridR = 0;
	g_CalcResult.Ave.f32FGridS = 0;
	g_CalcResult.Ave.f32FGridT = 0;

	g_CalcResult.Ave.f32BusVoltOffset = 0;
	g_CalcResult.Ave.VdqVbusForward= 0;

	g_CalcResult.Rms.f32IGridR = 0;
	g_CalcResult.Rms.f32IGridS = 0;
	g_CalcResult.Rms.f32IGridT = 0;

	g_CalcResult.Rms.f32VGridRS = 0;
	g_CalcResult.Rms.f32VGridST = 0;
	g_CalcResult.Rms.f32VGridTR = 0;

	g_CalcResult.Rms.f32VGridR = 0;
	g_CalcResult.Rms.f32VGridS = 0;
	g_CalcResult.Rms.f32VGridT = 0;

	g_CalcResult.Rms.f32GFCI = 0;

	g_CalcResult.Power.f32Input1Watt = 0;
	g_CalcResult.Power.f32Input2Watt = 0;
	g_CalcResult.Power.f32OutputWatt = 0;

	g_CalcResult.PowerDisplay.AccCalc.f32Input1Watt = 0;
	g_CalcResult.PowerDisplay.AccCalc.f32Input2Watt = 0;
	g_CalcResult.PowerDisplay.AccCalc.f32OutputWatt = 0;
	g_CalcResult.PowerDisplay.f32OutputEnergy = 0;
//----------------------------------------------------------------------------
	g_ADAcc.u16FullGridPeriodCnt = 0;
	
	g_ADAcc.Ave.f32VPV = 0;
	g_ADAcc.Ave.f32VPV1 = 0;
	g_ADAcc.Ave.f32VPV2 = 0;

	g_ADAcc.Ave.f32IPV1 = 0;
	g_ADAcc.Ave.f32IPV2 = 0;

	g_ADAcc.Ave.f32VbusP = 0;
	g_ADAcc.Ave.f32VbusN = 0;
	g_ADAcc.Ave.f32Vbus = 0;

	g_ADAcc.Ave.f32IGridR = 0;
	g_ADAcc.Ave.f32IGridS = 0;
	g_ADAcc.Ave.f32IGridT = 0;

	g_ADAcc.Ave.f32VGridR = 0;
	g_ADAcc.Ave.f32VGridS = 0;
	g_ADAcc.Ave.f32VGridT = 0;

	g_ADAcc.Ave.f32DciR = 0;
	g_ADAcc.Ave.f32DciS = 0;
	g_ADAcc.Ave.f32DciT = 0;

	g_ADAcc.Ave.f32GFCI = 0;

	g_ADAcc.Ave.f32FGrid = 0;
	g_ADAcc.Ave.f32FGridR = 0;
	g_ADAcc.Ave.f32FGridS = 0;
	g_ADAcc.Ave.f32FGridT = 0;

	g_ADAcc.Ave.f32BusVoltOffset = 0;
	g_ADAcc.Ave.VdqVbusForward= 0;

	g_ADAcc.Rms.f32IGridR = 0;
	g_ADAcc.Rms.f32IGridS = 0;
	g_ADAcc.Rms.f32IGridT = 0;

	g_ADAcc.Rms.f32VGridRS = 0;
	g_ADAcc.Rms.f32VGridST = 0;
	g_ADAcc.Rms.f32VGridTR = 0;

	g_ADAcc.Rms.f32VGridR = 0;
	g_ADAcc.Rms.f32VGridS = 0;
	g_ADAcc.Rms.f32VGridT = 0;

	g_ADAcc.Rms.f32GFCI = 0;

	g_ADAcc.Power.f32Input1Watt = 0;
	g_ADAcc.Power.f32Input2Watt = 0;
	g_ADAcc.Power.f32OutputWatt = 0;

	g_ADAcc.PowerDisplay.AccCalc.f32Input1Watt = 0;
	g_ADAcc.PowerDisplay.AccCalc.f32Input2Watt = 0;
	g_ADAcc.PowerDisplay.AccCalc.f32OutputWatt = 0;
	g_ADAcc.PowerDisplay.f32OutputEnergy = 0;
//----------------------------------------------------------------------------
	g_ADSum.u16FullGridPeriodCnt = 0;
	
	g_ADSum.Ave.f32VPV = 0;
	g_ADSum.Ave.f32VPV1 = 0;
	g_ADSum.Ave.f32VPV2 = 0;

	g_ADSum.Ave.f32IPV1 = 0;
	g_ADSum.Ave.f32IPV2 = 0;

	g_ADSum.Ave.f32VbusP = 0;
	g_ADSum.Ave.f32VbusN = 0;
	g_ADSum.Ave.f32Vbus = 0;

	g_ADSum.Ave.f32IGridR = 0;
	g_ADSum.Ave.f32IGridS = 0;
	g_ADSum.Ave.f32IGridT = 0;

	g_ADSum.Ave.f32VGridR = 0;
	g_ADSum.Ave.f32VGridS = 0;
	g_ADSum.Ave.f32VGridT = 0;

	g_ADSum.Ave.f32DciR = 0;
	g_ADSum.Ave.f32DciS = 0;
	g_ADSum.Ave.f32DciT = 0;

	g_ADSum.Ave.f32GFCI = 0;

	g_ADSum.Ave.f32FGrid = 0;
	g_ADSum.Ave.f32FGridR = 0;
	g_ADSum.Ave.f32FGridS = 0;
	g_ADSum.Ave.f32FGridT = 0;

	g_ADSum.Ave.f32BusVoltOffset = 0;
	g_ADSum.Ave.VdqVbusForward= 0;

	g_ADSum.Rms.f32IGridR = 0;
	g_ADSum.Rms.f32IGridS = 0;
	g_ADSum.Rms.f32IGridT = 0;

	g_ADSum.Rms.f32VGridRS = 0;
	g_ADSum.Rms.f32VGridST = 0;
	g_ADSum.Rms.f32VGridTR = 0;

	g_ADSum.Rms.f32VGridR = 0;
	g_ADSum.Rms.f32VGridS = 0;
	g_ADSum.Rms.f32VGridT = 0;

	g_ADSum.Rms.f32GFCI = 0;

	g_ADSum.Power.f32Input1Watt = 0;
	g_ADSum.Power.f32Input2Watt = 0;
	g_ADSum.Power.f32OutputWatt = 0;

	g_ADSum.PowerDisplay.AccCalc.f32Input1Watt = 0;
	g_ADSum.PowerDisplay.AccCalc.f32Input2Watt = 0;
	g_ADSum.PowerDisplay.AccCalc.f32OutputWatt = 0;
	g_ADSum.PowerDisplay.f32OutputEnergy = 0;
//----------------------------------------------------------------------------
	
	g_f32EnergyGain = ENERGY_MEASURE_GAIN;

	VGridMovAve_Init();

	g_u16SafetyCountDown = 30;	// 安规倒计时
	g_f32PowerLoadSpeed = 100;
	g_u16First4S = 4;
	g_u16RelayDelayCnt = 200;
//PLL initial 
	g_PLLCal.f32Valpha = 0;
	g_PLLCal.f32Vbeta = 0;
	g_PLLCal.f32Vd = 0;
	g_PLLCal.f32Vq = 0;

	g_PLLCal.f32VqErrOld = 0;
	g_PLLCal.f32VqErrNew = 0;
	g_PLLCal.f32ThetaStep = THETA_STEP_RATED;
	g_PLLCal.f32Theta = 0;
	g_PLLCal.f32ITheta = 0;
	g_PLLCal.SinTheta = 0;
	g_PLLCal.CosTheta = 0;
	g_PLLCal.ISinTheta = 0;
	g_PLLCal.ICosTheta = 0;

	g_BusRefCal.f32BusRef = 0;

	BoostLoopInit();
	

	PWMOutputsDisable();
	M_GRID_RELAY_OFF = 1;
	M_INVPWM_LOCK;
	M_INVPWM_ENABLEOCP;
//	g_Boost1PIDCalc.U16BoostState = BOOST_OFF;
	MpptReg.u16BoostOffFlag = 1;
	MpptReg1.u16BoostOffFlag = 1;
	MpptReg2.u16BoostOffFlag = 1;

	g_MPPTState.all = 0;
	g_PActiveLimit.f32Pinput1 = 10500;//10500;
	g_PActiveLimit.f32Pinput2 = 10500;//10500;
	g_PActiveLimit.f32PinputAll = 21000;//10500;
	g_MpptCalc.f32PowerK_alpha = 0.005;//0.005;//
	g_MpptCalc.f32PowerK_beta = 0.0046;//0.0048;//

	g_MPPTState.bit.FactoryMode = 0;
	g_MPPTState.bit.InputMode = INDEPENDENT_IN;
	g_MPPTState.bit.PV1ON = 0;
	g_MPPTState.bit.PV2ON = 0;

//INV initial 
	InverterStageInit();
//Relay
//	M_GRID_RELAY_ON = 1; //
//GridManagerInit
	GridManagerInit();
	g_GridManager.InvCon.f32PSpeed = 20;

	g_StateCheck.bit.PhaseSeqCheckOver = 0;	
//others
//	g_StateCheck.bit.Grid_Crossing_Flag = 0;

	g_BusPIDCal.f32IdRefMax = 1;//IPLimit;
	g_GridManager.f32PLimit500ms = 10000;
	g_PLLCal.AIDeltaTheta = 0;

	g_SafetyDCI.f32DciTestInjection = 0;
	g_SaftyConfig.bit.DCITestEnable = 0;

	g_SafetyReg.CoMMDate = 0;
	g_SaftyConfig.Word.VoltProCon = 0;
	g_SaftyConfig.Word.FreProCon = 0;
	g_SaftyConfig.Word.DCIProCon = 0;
	g_StateCheck.bit.BusUVP = 0;

	g_SafetyActive.RemoteOnOff = 0x55;
	g_StateCheck.bit.RemoteOff = 0;

}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/ 
void VGridMovAve_Init(void)
{
	Uint16 u16cnt;

	for(u16cnt=0; u16cnt<200; u16cnt++)
	{
		g_VGridMovAve.f32BufR[u16cnt] = 0.0;
		g_VGridMovAve.f32BufS[u16cnt] = 0.0;
		g_VGridMovAve.f32BufT[u16cnt] = 0.0;
	}
	g_VGridMovAve.f32R = 0.0;
	g_VGridMovAve.f32S = 0.0;
	g_VGridMovAve.f32T = 0.0;
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/ 
void BoostVLoopInit(struct BOOST_V_LOOP* p_boostVLoop)
{
	p_boostVLoop->f32DeltaVbus = 0;
	p_boostVLoop->f32DeltaPV = 0;
	p_boostVLoop->f32VPVErrOld = 0;
	p_boostVLoop->f32VPVErrNew =0;
	p_boostVLoop->f32VLoopOut= 0;
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/ 
void BoostILoopInit(struct BOOST_I_LOOP* p_boostILoop)
{
	p_boostILoop->f32IPVref= 0;
	p_boostILoop->f32IPVErrOld= 0;
	p_boostILoop->f32IPVErrNew=0;
	p_boostILoop->f32Duty= 0;
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void BoostLoopInit(void)
{
	BoostVLoopInit(&g_BoostVpvControl);
	BoostVLoopInit(&g_Boost1VpvControl);
	BoostILoopInit(&g_Boost1IpvControl);
	BoostVLoopInit(&g_Boost2VpvControl);
	BoostILoopInit(&g_Boost2IpvControl);
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/ 
void InverterStageInit(void)
{ 
	// Configure Current loop PID controller

	g_BusPIDCal.f32IdRef = 0;
	g_BusPIDCal.f32IdRef_DCSource = 0.05;
	g_BusPIDCal.f32IqRef = 0;

	g_CurPIDCal.f32IdErrOld = 0;
	g_CurPIDCal.f32IdErrNew = 0;
	g_CurPIDCal.f32DPI = 0;	//  cur Iq   PID

	g_CurPIDCal.f32IqErrOld = 0;
	g_CurPIDCal.f32IqErrNew = 0;
	g_CurPIDCal.f32QPI = 0;

	g_CurPIDCal.f32AlphaPI = 0;
	g_CurPIDCal.f32BetaPI = 0;

	g_CurPIDCal.PIa = 0;
	g_CurPIDCal.PIb = 0;
	g_CurPIDCal.PIc = 0;

	g_CurPIDCal.f32Duty3d = 0;

	//Duty
	g_CurPIDCal.f32DutyR = 0;
	g_CurPIDCal.f32DutyS = 0;
	g_CurPIDCal.f32DutyT = 0;
	
	g_CurPIDCal.f32DutyADeadTimeCom = 0;
	
	g_BusBanlanceCal.f32DeltaBusErrOld = 0;

	g_BusBanlanceCal.f32DeltaBusErrNew = 0;
	g_BusBanlanceCal.f32PIBusBanlance= 0;

	g_MPPTState.bit.InitFlag = 1;
	g_MpptCalc.u16OutputPowerLimitFlag = 0;
}
//===========================================================================
// No more.
//===========================================================================
