/***********************************************************************
 *
 *    PURPOSE  : 
 *    HISTORY  :
 *           DATE          VERSION      AUTHOR        NOTE
 ************************************************************************/
#ifdef  GLOBALS
#define PARA_EXT
#else
#define	PARA_EXT	extern
#endif
/************************************************************************************/
//Spi Buffer
PARA_EXT Uint16 g_SpiTxBuf[35];
PARA_EXT Uint16 g_SpiRxBuf[35];
// System Status definition for StateMachine
enum   SysState                             
{
	WaitState,		// 0
	CheckState,		// 1
	NormalState,	// 2
	FaultState,		// 3
	PermanentState,	// 4
	FlashState		// 5
}; 
PARA_EXT enum SysState	s_SysCurrentState, s_SysCurrentState_Master;
/************************************************************************************/
/************************************************************************************/
// State Check flags for process control
union  STATE_CHECK 
{
    struct
    {
        Uint16  byte0:8;
        Uint16  byte1:8;
        Uint16  byte2:8;
        Uint16  byte3:8;
		Uint16  byte4:8;
        Uint16  byte5:8;
		Uint16  byte6:8;
        Uint16  byte7:8;
    }Word;

    struct
    {
		//byte 0
        Uint16  AD_initial:1;    			// B0
        Uint16  Zero_Crossing_Flag:1;		// B1 电网过零点，半个电网周期
        Uint16  Offset_Fault_flag:1;					// B2
        Uint16  DC_Fan_State:1;		// B3
        Uint16  HWADFault_VGRID:1;		// B4
        Uint16  HWADFault_VINV:1;		// B5
        Uint16  HWADFault_DCI:1;		// B6
        Uint16  HWADFault_GFCI:1;				// B7

		//byte 1
		Uint16  :1;					// B0
		Uint16  :1;			// B1
		Uint16  PLLOver:1;			// B2
		Uint16  Grid_Pos_Flag:1;			// B3
		Uint16  Grid_Neg_Flag:1;			// B4
		Uint16  HandShakeOver:1;				// B5
		Uint16  :1;		// B6
		Uint16  :1;		// B7

		//byte 2,send to M DSP
		Uint16  PVIsoCheckOver:1;				// B0
		Uint16  AcRlyCheckOver:1;			// B1
		Uint16  FanCheckOver:1;			// B2
		Uint16  :1;			// B3
		Uint16  :1;	    	// B4
		Uint16  :1;		// B6 PV Input Mode: 0--Independent, 1--Parallel
		Uint16  :1;		// B6 PV Input Mode: 0--Independent, 1--Parallel
		Uint16  SlaveRelayON:1;		    // B7

		//byte 3,recieve from M DSP
		Uint16  MasterRelayOFF:1;		// B0
		Uint16  :1;		// B1
		Uint16  :1;		// B2
		Uint16  :1;		// B3
		Uint16  BusOverRef:1;		// B4
		Uint16  :1;	// B5
		Uint16  :1;			// B6
		Uint16  GFCICheckOVER:1;  	// B7 

		//byte 4
		Uint16  SafetySTART_Updateflag:1;
		Uint16  GridProtectV_Updateflag:1;
		Uint16  GridProtectFreqValue_Updateflag:1;
		Uint16  GridProtectDCI_Updateflag:1;
		Uint16  RemotPowerControlValue_Updateflag:1;
		Uint16  FreqPowerControlValue_Updateflag:1;
		Uint16  ReactiveValue_Updateflag1:1;
		Uint16  ReactiveValue_Updateflag2:1;

		//byte 5
		Uint16  LVRTValue_Updateflag1:1;
		Uint16  LVRTValue_Updateflag2:1;
		Uint16  SafetyIsoIslandandSoOn_Updateflag:1;
		Uint16  FactoryMode_Updateflag:1;
		Uint16  CalibrationPara_Updateflag1:1;
		Uint16  CalibrationPara_Updateflag2:1;
		Uint16  :1;
		Uint16  :1;
			
		//byte 6,recieve from M DSP
		Uint16  SafetySTART_UpdateOKflag:1;
		Uint16  GridProtectV_UpdateOKflag:1;
		Uint16  GridProtectFreqValue_UpdateOKflag:1;
		Uint16  GridProtectDCI_UpdateOKflag:1;
		Uint16  RemotPowerControlValue_UpdateOKflag:1;
		Uint16  FreqPowerControlValue_UpdateOKflag:1;
		Uint16  ReactiveValue_UpdateOKflag1:1;
		Uint16  ReactiveValue_UpdateOKflag2:1;

		//byte 7,recieve from M DSP
		Uint16  LVRTValue_UpdateOKflag1:1;
		Uint16  LVRTValue_UpdateOKflag2:1;
		Uint16  SafetyIsoIslandandSoOn_UpdateOKflag:1;
		Uint16  FactoryMode_UpdateOkflag:1;
		Uint16  CalibrationPara_UpdateOkflag1:1;
		Uint16  CalibrationPara_UpdateOkflag2:1;
		Uint16  :1;
		Uint16  :1;
    }bit;
}; 
PARA_EXT union STATE_CHECK  s_StateCheck/*,s_StateCheck_Master*/; 

// ADC data structure definition
struct	AD_SAMPLE
{	
	int16 i16Counter;
	int16 i16VGRID_R;
	int16 i16VGRID_S;
	int16 i16VGRID_T;	
	int16 i16VINV_R;
	int16 i16VINV_S;
	int16 i16VINV_T;	
	int16 i16DCI_R;
	int16 i16DCI_S;
	int16 i16DCI_T;	
	int16 i16VPV1;
	int16 i16VPV2;
	int16 i16VISO1;
	int16 i16VISO1_test0;
	int16 i16VISO1_test00;

	int16 i16VISO1_test1;
	int16 i16VISO1_test11;
	int16 i16VISO1_test2;
	int16 i16VISO1_test22;
	int16 i16VISO1_test3;
	int16 i16VISO1_test33;
	int16 i16VISO1_testoutput1;
//	int16 i16VISO1_testoutput11;
	int16 i16VISO1_testoutput2;
//	int16 i16VISO1_testoutput22;
	int16 i16VISO1_testoutput3;
//	int16 i16VISO1_testoutput33;
	int16 i16lost;
	int16 i16VISO2;	
	int16 i16GFCI;  	
	int16 i16TempAmb;
	int16 i16TempINVR;			
	int16 i16TempBoost2;
	int16 i16TempINVT;			
	int16 i16TempBoost1;
	int16 i16TempINVS;
	int16 i16TempINVMax;
	int16 i16TempINV;
	int16 i16TempBoost;
	
	int16 i16GridFreq;
	int16 i16GridRFreq;
	int16 i16GridSFreq;
	int16 i16GridTFreq;

	int32 i32ZeroCAPCNT;
	
	int16 i16IPV1;
	int16 i16IPV2;

	int16 i16VbusP;
	int16 i16VbusN;
	int16 i16Vbus;
		
	int16 i16IGridR;
	int16 i16IGridS;
	int16 i16IGridT;
	
	int16 i16VGrid_RSrms;
	int16 i16VGrid_TRrms;
	int16 i16VGrid_STrms;
	int16 i16VInv_RSrms;
	int16 i16VInv_STrms;
	int16 i16VInv_TRrms;
	
	int16 i16Input1Watt;		//PV1 Power hight
	int16 i16Input2Watt;		//PV2 Power hight
	int16 i16OutputWatt;		//Out Power hight
//	int32 i32VPV1;			//PV1 VoltageAvg
//	int32 i32IPV1;			//PV1 CurrentAvg
//	int32 i32VPV2;			//PV2 VoltageAvg
//	int32 i32IPV2;			//PV2 CurrentAvg
//	int32 i32IGridR;			//R-inverter CurrentRms
//	int32 i32IGridS;			//S-inverter CurrentRms
//	int32 i32IGridT;			//T-inverter CurrentRms
	Uint16 i16OutputEnergy;
	Uint16 i16EnergyTodayNew;
};
PARA_EXT struct	AD_SAMPLE	 M_CalcResultRms,CalcResultRms,ADChannelOffset,CalcResultAve, ADGain, GetRealValue;
	  

struct	AD_CALCULATE_RMS
{
	int16 i16Counter;
	int16 u16Counter;
	
	int32 i32VGRID_R;
	int32 i32VGRID_S;
	int32 i32VGRID_T;	
	int32 i32VINV_R;
	int32 i32VINV_S;
	int32 i32VINV_T;
		
	int32 i32DCI_R;
	int32 i32DCI_S;
	int32 i32DCI_T;	
	int32 i32VPV1;
	int32 i32VPV2;
	int32 i32VISO1;
	int32 i32VISO2;	
	int32 i32GFCI;  	
	int32 i32TempAmb;
	int32 i32TempINVR;			
	int32 i32TempBoost2;
	int32 i32TempINVT;			
	int32 i32TempBoost1;
	int32 i32TempINVS;
	
	int32 i32VGrid_RSrms;
	int32 i32VGrid_STrms;
	int32 i32VGrid_TRrms;

	int32 i32VInv_RSrms;
	int32 i32VInv_STrms;
	int32 i32VInv_TRrms;
		
};
PARA_EXT struct	AD_CALCULATE_RMS	 ADAccRms, ADSumRms , ADAccAve, ADSumAve;

struct ADOFFSET_SAMPLE
{
	int32 i32VGRID_R;
	int32 i32VGRID_S;
	int32 i32VGRID_T;

	int32 i32VINV_R;
	int32 i32VINV_S;
	int32 i32VINV_T;

	int32 i32DCI_R;
	int32 i32DCI_S;
	int32 i32DCI_T;

	int16 i16VPV1;
	int16 i16VPV2;

	int32 i32GFCI;
};
PARA_EXT struct	ADOFFSET_SAMPLE	 ADOffsetSum;

// System fault message
union  SYS_FAULT_MESSAGE   
{
    struct
    {
        Uint16  byte0:8;
        Uint16  byte1:8;
        Uint16  byte2:8;
        Uint16  byte3:8;
        Uint16  byte4:8;
        Uint16  byte5:8;
		Uint16  byte6:8;
        Uint16  byte7:8;
        Uint16  unrecover0:8;
        Uint16  unrecover1:8;
    }Word;

    struct
    {	// byte0


        Uint16  :1;			// B0
        Uint16  :1;			// B1
        Uint16  :1;			// B2
        Uint16  :1;		    // B3
        Uint16  :1;	    	// B4
        Uint16  :1;			// B5
        Uint16  :1;			// B6
        Uint16  :1;	    	// B7

        // byte1
        Uint16  :1;		// B0
        Uint16  :1;		// B1
        Uint16  :1;		// B2
        Uint16  :1;		// B3
        Uint16  :1;		// B4
        Uint16  :1;		// B5
        Uint16  :1;		// B6
        Uint16  :1;		// B7

		// byte 2
        Uint16  HWADFault_VINV:1;		// B0,not enable
        Uint16  HWADFault_DCI:1;		// B1,not enable
        Uint16  HWADFault_VGRID:1;		// B2,not enable
        Uint16  HWADFault_GFCI:1;		// B3,not enable
        Uint16  MChip_Fault:1;		// B4
		Uint16  :1;		// B5
        Uint16  :1;		// B6
        Uint16  :1;		// B7


		// byte 3
        Uint16  :1;		// B0
        Uint16  :1;		// B1
        Uint16  :1;		// B2
        Uint16  :1;		// B3
        Uint16  :1;		// B4
        Uint16  :1;		// B5
        Uint16  :1;		// B6
        Uint16  :1;		// B7

        // byte4
        Uint16  :1;		// B0
        Uint16  :1;		// B1
        Uint16  :1;		// B2
        Uint16  :1;		// B3
        Uint16  :1;		// B4
        Uint16  :1;		// B5
        Uint16  :1;		// B6
        Uint16  :1;		// B7,FAN fault is detected at COMM CPU

		// byte5
        Uint16  :1;		// B0
        Uint16  :1;		// B1
        Uint16  :1;		// B2
        Uint16  :1;		// B3
        Uint16  :1;		// B4
        Uint16  :1;		// B5
        Uint16  :1;		// B6
        Uint16  :1;		// B7

		// byte6
        // unrecover0
        Uint16  ConsistentFault_VGrid:1;		// B0
        Uint16  ConsistentFault_FGrid:1;		// B1
        Uint16  ConsistentFault_DCI:1;			// B2
        Uint16  ConsistentFault_GFCI:1;			// B3    
        Uint16  SpiCommLose:1;			// B4
        Uint16  SciCommLose:1;			// B5
        Uint16  RecoverRelayFail:1;		// B6
        Uint16  PvIsoFault:1;		// B7

		// byte7
        Uint16  OverTempFault_Inv:1;		// B0
        Uint16  OverTempFault_Boost:1;		// B1
        Uint16  OverTempFault_Env:1;		// B2
        Uint16  :1;		// B3
        Uint16  :1;			// B4
        Uint16  :1;			// B5
        Uint16  :1;		// B6
        Uint16  :1;		// B7

        // unrecover0
        Uint16  :1;		// B0
        Uint16  :1;		// B1
        Uint16  :1;			// B2
        Uint16  :1;			// B3    
        Uint16  :1;			// B4
        Uint16  :1;		// B5
        Uint16  :1;			// B6
        Uint16  :1;			// B7

		// unrecover1
        Uint16  :1;		// B0
        Uint16  :1;		// B1
        Uint16  unrecoverEEPROM_W:1;		// B2
        Uint16  unrecoverEEPROM_R:1;		// B3
        Uint16  unrecoverRelayFail:1;		// B4 
		Uint16  :1;		// B5
		Uint16  :1;		// B6
		Uint16  :1;		// B7

     }bit;
};
PARA_EXT union SYS_FAULT_MESSAGE s_SysFaultMessage,s_SysFaultMessage_Enable,s_SysFaultMessage_Master;

typedef struct
{
	Uint16	u16SafeCountry;
	int16 i16ConnectTime;					// 启动时间
	int16 i16ReconnectionTime;				// 重连时间
	int16 i16CountdownTime;				    // 倒计时

	int16 i16VGrid_Max1Limit;				// 电网电压上限1
	int16 i16VGrid_Max2Limit;				// 电网电压上限2
	int16 i16VGrid_Min1Limit;				// 电网电压下限1
	int16 i16VGrid_Min2Limit;				// 电网电压下限2
	int16 i16VGrid_MaxLimitBack;			// 启动/亓缤缪股舷?
	int16 i16VGrid_MinLimitBack;			// 启动/重连电网电压下限

	int16 i16VGrid_Max1_ProtectTime;		// 电网电压上限1保护时间
	int16 i16VGrid_Max2_ProtectTime;		// 电网电压上限2保护时?
	int16 i16VGrid_Min1_ProtectTime;		// 电网电压下限1保护时间
	int16 i16VGrid_Min2_ProtectTime;		// 电网电压下限2保护时间

	int16 i16FGrid_Max1Limit;				// 电网频率上限1
	int16 i16FGrid_Max2Limit;				// 电网频率上限2
	int16 i16FGrid_Min1Limit;				// 电网频率下限1
	int16 i16FGrid_Min2Limit;				// 电网频率下限2
	int16 i16FGrid_MaxLimitBack;			// 启动/重连电网频率上限
	int16 i16FGrid_MinLimitBack;			// 启动/重连电网频率下限

	int16 i16FGrid_Max1_ProtectTime;		// 电网频率上限1保护时间
	int16 i16FGrid_Max2_ProtectTime;		// 电网频率上限2保护时间
	int16 i16FGrid_Min1_ProtectTime;		// 电网频率下限1保护时间
	int16 i16FGrid_Min2_ProtectTime;		// 电网频率下限2保护时间

	int16 i16DCI_Max1Limit;					// DCI上限1
	int16 i16DCI_Max2Limit;  				// DCI上限2   
	int16 i16DCI_Max1_ProtectTime;			// DCI上限1保护时间
	int16 i16DCI_Max2_ProtectTime;			// DCI上限2保护时间
}SAFETY_PARAMETER_REG;

PARA_EXT SAFETY_PARAMETER_REG	SafetyReg, SafetyReg_default, SafetyReg_Comm;

typedef struct
{
	int16 i16Valpha;
	int16 i16Vbeta;			
	int16 i16Vd;			
	int16 i16Vq;			

	int16 i16VqErrOld;				
	int16 i16VqErrNew;			
	int16 i16KPI_PLL;			
	int16 i16KP_PLL;		
		
	int32 i32ThetaStep;			
	Uint32 u32Theta;

	Uint16 u16ThetaSin;
	int16 i16ThetaSinOld;
	int16 i16ThetaCos;

	int32 i32ITheta;
	int16 i16SinTheta;
	int16 i16SinThetaOld;
	int16 i16CosTheta;

}PLL_CAL;

PARA_EXT PLL_CAL	PLLCal;

typedef  struct
{
	Uint8 u8ReactiveMode1;
	Uint8 u8ReactiveMode2;
	Uint8 u8ReactiveMode3;
	Uint8 u8ReactiveMode4;
	Uint8 u8ReactiveMode5;
	int32 i32ReactiveCos;
	int32 i32ReactivefixedCos;
	Uint8 u8ReactivedynamicValue1;
	int32 i32ReactivedynamicCos1;
	Uint8 u8ReactivedynamicValue2;
	int32 i32ReactivedynamicCos2;
	Uint8 u8ReactivedynamicValue3;
	int32 i32ReactivedynamicCos3;
	Uint8 u8ReactivedynamicValue4;
	int32 i32ReactivedynamicCos4; 
}ReactiveValue_Reg1;

PARA_EXT   ReactiveValue_Reg1 ReactiveValue;

typedef  struct
{
	Uint16 i16ISOCheckMode_Input;
	Uint32 i16IsoCheckcount;
	Uint16 i16IsoCheckStep;
	Uint16 i16ISO_N;
	Uint16 i16ISO_P;
	Uint16 i16InvPowerMode;
	int16 i16TAmbOTP;
	int16 i16TInvOTP;
	int16 i16TNTCOTP;

	int16 i16ReConnetionTime;
	Uint16 g_VgridZero_Cnt_flag;
}ProcessVariable1;

PARA_EXT   ProcessVariable1  ProcessVariable;



//===========================================================================
// No more.
//===========================================================================

