#ifndef		SR_SLAVE_H
#define		SR_SLAVE_H

#include "DSP28x_Project.h"     	// Device Headerfile and Examples Include File
#include "F28035_BIOS_flashcfg.h"
#include "IQmathLib.h"
#include "Flash2803x_API_Library.h"

#include "SL_slave_variable.h"
#include "SL_slave_define.h"

#include <stdio.h> 
#include <math.h> 

#include "DSP2803x_Flash2803x_API.h"
#include "Flash2803x_API_Config.h"

void Tsk_F_StateSwitch(void);
void SlaveWaitState(void);
void SlaveCheckState(void);
void SlaveNormalState(void);
void SlaveFaultState(void);
void SlavePermanentState(void);
void SlaveFlashState(void);
void SysParamDefault(void);

void Adc_Getvalue1(void); 
void Adc_Getvalue2(void);
void ADAccCalc(void); 
void TSK_F_GridPeriodCalc(void);

void VPV_ave_Calc(void);
void TEMP_ave_Calc(void);
void TEMP_INVR_Calc(void);
void TEMP_INVS_Calc(void);
void TEMP_INVT_Calc(void);
void TEMP_Boost_Calc(void);
void TEMP_Amb_Calc(void);
void PrepareForWait(void);


void VGrid_ave_Calc(void);
void VInv_ave_Calc(void);	
void GFCI_ave_Calc(void);
void VGrid_rms_Calc(void);
void VInv_rms_Calc(void);	
void DCI_ave_Calc(void);
void DCI_rms_Calc(void);
void GFCI_rms_Calc(void);
void GridFreqCalc(void);
void ADOffsetCalibration(void);

void SlaveFlashState(void);
void dqPLLcontroller(void);

void EnterWaitState(void);
void EnterCheckState(void);
void EnterNormalState(void);
void EnterFaultState(void);
void EnterPermanentState(void);
void EnterFlashState(void);

void SlaveWaitState(void);
void SlaveCheckState(void);
void SlaveFaultState(void); 
void SlavePermanentState(void);

void PWMOFFandRelayDisconnect(void);

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

void PWMOutputsEnable(void);
void PWMOutputsDisable(void);
void PWMOFFandRelayDisconnect(void);
Uint16 PermanentFaultCheck();
Uint16 FaultCheck();
Uint16 FaultBackCheck();

void ConsistentCheck(void);
void ConsistentVGridCheck(void);
void ConsistentGridFeqCheck(void);
void ConsistentGridDciCheck(void);
void ConsistentGFCICheck(void);

void TemperatureCheck(void);
void InvTempCheck(void);
void BoostTempCheck(void);
void EnvTempCheck(void);
void LTempCheck(void);

void DigitalIOCheck(void);
void MChipWDDetection(void);
void DcFan1SpeedSense(void);
void DcFan2SpeedSense(void);

void ADOffsetCheck(void);
void ADOffsetVGRIDCheck(void);
void ADOffsetVINVCheck(void);
void ADOffsetDCICheck(void);
void ADOffsetGFCICheck(void);

void SlaveFanCheck(void);
void PvInsulationCheck(void);
void SlaveRelayShortCheck(void);
void SlaveRelayOpenCheck(void);
void ReadFromEeprom(void);
void SPICommCheck(void);

void ShakeHandsCheck(void);
void SetSciValueForDataread(void);
void GoBacktoOrig(void);
void GetONOFFCommand(void);
void GetDeratingCommand(void);
void GetPowerFactorCommand(void);
void GetReactivePowerCommand(void);

void SetSofteVersionToLCD(void);
void SetSciValueForCommissionEndCheck(void);
void ResetToDefaultsafetyvalue(void);
void ClearTodayOutputPower(void);

					
				
Uint16 CheckSTARTValue(void);					
Uint16 CheckGridProtectVValue(void);
Uint16 CheckGridProtectFreqValue(void);	
Uint16 CheckGridProtectDCIValue(void);
Uint16 CheckRemotPowerControlValue(void);
Uint16 CheckFreqPowerControlValue(void);
					
Uint16 CheckReactiveValue(void);					
Uint16 CheckLVRTValue(void);					
Uint16 CheckIsoIslandandSoOnValue(void);
Uint16 CheckFactoryModeValue(void);						
Uint16 CheckCalibrationValue(void);

void Sci_SendData(void);
void Sci_ReceiveData(void);


void SetSTARTValueToLCD(void);    				//向通讯板发送开始数据信息					
void SetGridProtectVValueToLCD(void);      		//向通讯板发送电网保护数据信息					
void SetGridProtectFreqValueToLCD(void);      	//向通讯板发送电网保护数据信息					
void SetGridProtectDCIValueToLCD(void);      	//向通讯板发送电网保护数据信息										
void SetRemotPowerControlValueToLCD(void);    		//向通讯板发送远程有功功率调度数据信息					    
void SetFreqPowerControlValueToLCD(void);    		//向通讯板发送远程有功功率调度数据信息
void SetReactiveValueToLCD(void);       		//向通讯板发送无功控制电流数据信息					
void SetLVRTValueToLCD(void);    				//向通讯板发送低电压穿越数据信息					
void SetIsoIslandandSoOnValueToLCD(void);    	//向通讯板发送孤岛，绝缘阻抗，漏电流、开关机等					
void SetFactoryModeValueToLCD(void);    				//向通讯板发送自动测试值					
void SetCalibrationValueToLCD(void);    	//向通讯板发送孤岛，绝缘阻抗，漏电流、开关机等					

void GetSTARTValueFromLCD(void);   				//从通讯板获得并保存低电压穿越数据信息					
void GetGridProtectVValueFromLCD(void);     	//从通讯板获得并保存电网保护数据信息					
void GetGridProtectFreqValueFromLCD(void);     	//从通讯板获得并保存电网保护数据信息					
void GetGridProtectDCIValueFromLCD(void);     	//从通讯板获得并保存电网保护数据信息										
void GetRemotPowerControlValueFromLCD(void);   //从通讯板获得并保存远程有功功率调度数据信息					    
void GetFreqPowerControlValueFromLCD(void);   //从通讯板获得并保存远程有功功率调度数据信息					   
void GetReactiveValueFromLCD(void);      		//从通讯板获得并保存无功控制电流数据信息					
void GetLVRTValueFromLCD(void);   				//从通讯板获得并保存低电压穿越数据信息					
void GetIsoIslandandSoOnValueFromLCD(void);   	//从通讯板获得并保存孤岛，绝缘阻抗，漏电流、开关机等					
void GetFactoryModeValueFromLCD(void);   			//从通讯板获得并保存自动测试值					
void GetCalibrationValueFromLCD(void);   	//从通讯板获得并保存孤岛，绝缘阻抗，漏电流、开关机等					
void SetSciValueForResetOrModifySafetyvalue(Uint8 state, Uint8 Funcode);
void SetSciValueForAnswerLCD(Uint8 state, Uint8 Funcode);

void SoftwareOnlineUpgrade(void);   //在线升级

#endif
//===========================================================================
// No more.
//===========================================================================
