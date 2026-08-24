/***********************************************************************
 *    FILENAME : SC_master.h
 *
 *    PURPOSE  : 
 *    HISTORY  :
 *           DATE          VERSION      AUTHOR        NOTE
 *           2012-xx-xx    V0.0                       Created.
 ************************************************************************/
#ifndef SC_MASTER_H
#define SC_MASTER_H

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

#include "math.h"
#include "C28x_FPU_FastRTS.h"

#include "SC_sysctrl.h"
#include "SC_define.h"
#include "SC_variable.h"
#include "SC_mastercfg.h"

#include "Flash28335_API_Library.h"
// function declaration
void ADC_INT_Control(void);
void PvInModelChk(void);
void StartBoostControl(void);
void BoostControl(void);
void InitialBoostDuty();
void BoostControlStrategy(void);
void SetBoostState();
void StandBusUpdate(void);
void UpdateBoostDuty();
void BoostParallelVLoop(void);
void Boost1VLoop(void);
void Boost1ILoop(float32);
void Boost2VLoop(void);
void Boost2ILoop(float32);
void BusPIDIdCal(void);
void BusBanlanceControl(void);
void OCPONEBYONE(void);
void FeedForwardVolCalc(void);
void InstantProtect(void);
void CurrentPIDcontroller(void);
void dqPLLcontroller(void);
void DeadtimeCompensation(void);
void LCLActiveDamping(void);

void Adc_Getvalue1(void); 
void Adc_Getvalue2(void); 
void Adc_PowerOn(void);
void Adc_Calc(void);
void ADOffsetCalibration(void);
void ChoiceKindsMachine(void);

void Tsk_F_MasterStates (void);
void MasterWaitState(void);
void MasterCheckState(void);
void MasterNormalState(void);
void VbusSoftStart(void);
void MasterFaultState(void);
void MasterPermanentState(void);
void MasterFlashState(void);

Uint16 ReadyToEnterWaitState(void);
Uint16 ReadyToEnterCheckState(void);
Uint16 ReadyToEnterNormalState(void);
Uint16 ReadyToEnterFaultState(void);
Uint16 ReadyToEnterPermanentState(void);
Uint16 ReadyToEnterFlashState(void);

void EnterWaitState(void);
void EnterCheckState(void);
void EnterNormalState(void);
void EnterFaultState(void);
void EnterPermanentState(void);
void EnterFlashState(void);

void VPV_ave_Calc(float32 f32CntReci);
void IPV_ave_Calc(float32 f32CntReci);
void VBUS_ave_Calc(float32 f32CntReci);
void VGrid_ave_Calc(float32 f32CntReci);
void IGrid_ave_Calc(float32 f32CntReci);
//void GridManage_Iq_Calc(void);	
void ADAccCalc(void);	

void Tsk_F_SpiComm(void);

void Tsk_F_GridPeriodCalc(void);
void VGrid_rms_Calc(float32 f32CntReci);
void VGridLine_rms_Calc(float32 f32CntReci);
void VGrid10minMovingAve_Calc(void);
void IGrid_rms_Calc(float32 f32CntReci);
void GFCI_rms_Calc(float32 f32CntReci);
void GFCI_ave_Calc(float32 f32CntReci);
void DCI_ave_Calc(float32 f32CntReci);
void GridFreqCalc(float32 f32CntReci);

void GridVoltCheck(void);
void VGrid_Over1Check(void);
void VGrid_Over2Check(void);
void VGrid_PreConnectionCheck(void);
void VGrid_Under1Check(void);
void VGrid_Under2Check(void);

void VGrid10minMovingAveCheck(void);
void GridVoltADOffsetCheck(void);
void VGrid_ADOffsetCheck(void);
void GridCurrentADOffsetCheck(void);
void IGrid_ADOffsetCheck(void);
void DCIADOffsetCheck(void);
void IGridDCI_ADOffsetCheck(void);
void GridFreqCheck(void);
void FGrid_PreConnectionCheck(void);
void GridFreqOver1Check(void);
void GridFreqOver2Check(void);
void GridFreqUnder1Check(void);
void GridFreqUnder2Check(void);
void GridCurrentCheck(void);
void IGrid_OverCheck(void);
void IGridUnbalanceCheck(float32 f32IGridDiffMax);
//void unrecoverGridCurrentUnbalanceCheck(Uint16 u16OldState_IAC_Unbalance);
void VPVCheck(void);
void VPVOverLimitCheck(void);
//void VPVUnderLimitCheck(void);
void VPVConfigCheck(void);
void VPV1OnOffCheck(void);
void VPV2OnOffCheck(void);
void IPVCheck(void);
void IPVUnbalanceCheck(float32 f32IpvDiff);
void VBUSCheck(void);
void BusVoltZeroCheck(void);
void SWOCPRecover(void);
void BusUnbalanceCheck(void);
//void unrecoverBusUnbalanceCheck(Uint16 u16OldState_BusUnbalance);
void BusUnderCheck(void);
void BusOverCheck(void);
void BusOverCheck_Instant(float32 f32VbusP, float32 f32VbusN);
//void unrecoverBusOverCheck(Uint16 u16OldState_BusVoltOverFault);
void PhaseSeqCheck(void);
void DigitalIOCheck(void);
void DIO_AcOCPCheck(void);
//void DIO_unrecoverAcOCPCheck(void);
void DIO_BusOVPCheck(void);
void DIO_BoostOCPCheck(void);
void DIO_AuxPowerCheck(void);
void DIO_RChipWDCheck(void);
void DCICheck(void);
void DCI_Over1Check(void);
void DCI_Over2Check(void);
void GFCICheck(void);
void GFCIProtectCheck(void);
void GFCISelfCheck(void);
void GFCI50mASelfCheck(void);
void GFCIProtect(void);
void AntiIslandingCalc(void); 

void SystemInitial(void);
void BoostLoopInit(void);
void BoostVLoopInit(struct BOOST_V_LOOP* p_boostVLoop);
void BoostILoopInit(struct BOOST_I_LOOP* p_boostILoop);
void InverterStageInit(void);
void VGridMovAve_Init(void);
void Tsk_F_MPPT(void);
void MpptInitial();
void MPPTTrack();
void Mppt1Initial();
void MPPT1Track();
void Mppt2Initial();
void MPPT2Track();
void OutputPowerLimitMPPT(void);

void TemperatureCheck(void);
void Power_Calc(float32 f32CntReci);
void Power_Display_Calc(void);
//GridManagerment
void OPLoadLimit20ms(void);
void OILoadLimit20ms(void);
void OPLoadLimit500ms(void);
void GridManagerInit(void);
void LvrtVdcalc(void);
void LvrtManager(void);
//end GridManager
void DelayUs(Uint16);

Uint16 SciRead(Uint16 SciId, Uint8 *pBuf);
Uint16 SciWrite(Uint16 SciId, Uint8 *pBuf, Uint16 u16Length);
Uint16 QueDataIn(QUEUE *pQue, Uint16 u16QueData);

void InitScia(Uint32 u32BaudRate, Uint16 u16RxMode);
void InitScib(Uint32 u32BaudRate, Uint16 u16RxMode);
void InitSci(void);
void SCIa_IPOMS(void);
Uint8 IPOMS_SnatchGraph(void);

void PWMOutputsEnable(void);
void PWMOutputsDisable(void);
void PWMOFFandRelayDisconnect(void);
Uint16 PermanentFaultCheck(void);
Uint16 FaultCheck(void);
Uint16 FaultBackCheck(void);

// SPI
void SetMasterValue(void);
void SpiChange(Uint16 *pTX_Buf,Uint16 *pRX_Buf,Uint8 item_Num);
void SpiMasterReceiveData(void);

//SCIB
void Scib_Communication(void);
Uint16 InitPLLForFlashAPI(void);
#endif
//===========================================================================
// No more.
//===========================================================================

