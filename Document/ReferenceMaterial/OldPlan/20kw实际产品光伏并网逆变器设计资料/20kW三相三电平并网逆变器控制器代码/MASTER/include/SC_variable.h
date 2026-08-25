/***********************************************************************
 *    FILENAME : SC_variable.h
 *
 *    PURPOSE  : 
 *    HISTORY  :
 *           DATE          VERSION      AUTHOR        NOTE
 *           2012-xx-xx    V0.0         
 ************************************************************************/
#ifdef  SC_GLOBALS
#define PARA_EXT
#else
#define	PARA_EXT	extern
#endif
/************************************************************************************/
// Inverter type
enum	InvType
{
	SolarLake5K,	// 0
	SolarLake6K,	// 1
	SolarLake8K,	// 2
	SolarLake10K,	// 3
	SolarLake12K,   // 4
	SolarLake15K,   // 5
	SolarLake17K,   // 6
	SolarLake20K,   // 7
	SolarLake25K,   // 8
	SolarLake30K,   // 9

	InvalidType=16	// 16
};
PARA_EXT enum InvType g_InverterType;

/************************************************************************************/
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
PARA_EXT enum SysState	g_SysCurrentState, g_SysCurrentStateSlave;

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
		Uint16	byte4:8;
		Uint16	byte5:8;
		Uint16	byte6:8;
		Uint16	byte7:8;
    }Word;

    struct
    {	
		// byte 0
        Uint16  OverTempDerating:1;    	// B0
        Uint16  OverFreqDerating:1;		// B1
        Uint16  RemoteDerating:1;		// B2
        Uint16  RemoteOff:1;		// B3
        Uint16  :1;		// B4
        Uint16  :1;		// B5
        Uint16  :1;		// B6
        Uint16  :1;		// B7

		// byte 1
		Uint16  BusUVP:1;   // B0
        Uint16  GridCrossingFlag:1;	// B1
        Uint16  FlashFlag:1;    	// B0
        Uint16  HwBusOVP:1;		// B1
        Uint16  PLLOver:1;			// B2
        Uint16  PhaseSeqCheckOver:1;		// B3
        Uint16  InvSoftStartFalg:1;		// B6
        Uint16  InvSoftStartOver:1;		// B7

		// byte 2
        Uint16  SafetySetOK:1;   	// B0
        Uint16  SafetyCountDownSetOver:1;			// B1 电网过零点，半个电网周期
        Uint16  :1;			// B2 电网过零点，一个电网周期
        Uint16  :1;					// B3
        Uint16  :1;					// B4 PV电压过低
        Uint16  :1;			// B5
        Uint16  :1;			// B6
        Uint16  :1;				// B7
		
		// byte 3
		Uint16  :1;					// B0
		Uint16  :1;				// B1
		Uint16  :1;				// B2
		Uint16  :1;					// B3
		Uint16  :1;			// B4
		Uint16  :1;				// B5
		Uint16  :1;		// B6
		Uint16  :1;	// B7
		
		// byte 4 (byte4 and byte5 are send to Slave-CPU via SPI)
		Uint16  MasterRelayOFF:1;		// B0
		Uint16  ADOffsetCalibrateOver:1;		// B1
		Uint16  :1;		// B2
		Uint16  :1;		// B3
		Uint16  BusOverRef:1;				// B4
		Uint16  :1;		// B5
		Uint16  :1;		// B6
		Uint16  GFCI50mACheckOK:1;			// B7 
		
		// byte 5 (byte4 and byte5 are send to Slave-CPU via SPI)
		Uint16  :1;		// B0
		Uint16  :1;		// B1
		Uint16  :1;		// B2
		Uint16  :1;		// B3
		Uint16  :1;		// B4
		Uint16  :1;		// B5
		Uint16  :1;		// B6
		Uint16  :1;  			// B7 安规设置完成
		//byte 6
		Uint16  SafetySTART_UpdateOKflag:1;
		Uint16  GridProtectV_UpdateOKflag:1;
		Uint16  GridProtectFreqValue_UpdateOKflag:1;
		Uint16  GridProtectDCI_UpdateOKflag:1;
		Uint16  RemotPowerControlValue_UpdateOKflag:1;
		Uint16  FreqPowerControlValue_UpdateOKflag:1;
		Uint16  ReactiveValue_UpdateOKflag1:1;
		Uint16  ReactiveValue_UpdateOKflag2:1;

		//byte 7
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
PARA_EXT union STATE_CHECK  g_StateCheck; 

/************************************************************************************/
// State Check flags from Slave CPU
union  SLAVE_STATE_CHECK 
{
    struct
    {
        Uint16  byte0:8;
	}Word;

    struct
    {	
		// byte 0
		Uint16  PVIsoCheckOver:1;			// B0
		Uint16  RelayCheckOver:1;			// B1
		Uint16  :1;				// B2
		Uint16  :1;				// B3
		Uint16  :1;	    			// B4
		Uint16  :1;		// B5
		Uint16  :1;		// B6
		Uint16  SlaveRelayON:1;		// B7
	}bit;
}; 
PARA_EXT union SLAVE_STATE_CHECK  g_SlaveStateCheck;

/************************************************************************************/
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
    {	// byte0,warning

        Uint16  GridOVP:1;			// B0
        Uint16  GridUVP:1;			// B1
        Uint16  GridOFP:1;			// B2
        Uint16  GridUFP:1;			// B3
        Uint16  PVUVP:1;			// B4
        Uint16  VLvrtlow:1;		// B5
        Uint16  :1;			// B6
        Uint16  :1;		// B7

        // byte1
        Uint16  PvOVP:1;			// B5
        Uint16  IpvUnbalance:1;		// B6
        Uint16	PvConfigSetWrong:1;	// B7
        Uint16  GFCIFault:1;	// B1
        Uint16  PhaseSequenceFault:1;	// B0
        Uint16  HwBoostOCP:1;		// B2
        Uint16  HwAcOCP:1;			// B1   
        Uint16  AcRmsOCP:1;			// B0

		// byte2
        Uint16  HwADFaultIGrid:1;	// B2
		Uint16  HwADFaultDCI:1;		// B0
        Uint16  HwADFaultVGrid:1;	// B2
        Uint16  GFCIDeviceFault:1;	// B5
		Uint16  HwRChipFault:1;		// B6
        Uint16  HwAuxPowerFault:1;	// B7        
        Uint16  BusVoltZeroFault:1;		// B4只在check中起作用：若bus电压采不到或采样值不对，则不再打BOOST驱动
        Uint16  IacRmsUnbalance:1;	// B3

		// byte3
        Uint16  BusUVP:1;			// B4
        Uint16  BusOVP:1;			// B5
        Uint16  VbusUnbalance:1;	// B6
        Uint16  DciOCP:1;	// B0 
        Uint16  SwOCPInstant:1;	// B1
        Uint16  :1;			// B3
        Uint16  :1;		// B6
        Uint16  :1;		// B6
		// byte 4
        Uint16  :1;		// B0
        Uint16  :1;		// B1
        Uint16  :1;		// B2
        Uint16  :1;		// B3
        Uint16  :1;		// B4
        Uint16  :1;		// B5
        Uint16  :1;		// B6
        Uint16  :1;		// B7

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
        Uint16	:1;		// B0
        Uint16  :1;		// B1
        Uint16  :1;		// B2
        Uint16  :1;		// B3
        Uint16  SpiCommLose:1;		// B4
        Uint16  :1;		// B5
        Uint16  :1;		// B6
        Uint16  :1;		// B7

		// byte7
        Uint16  :1;		// B0
        Uint16  :1;		// B1
        Uint16  :1;		// B2
        Uint16  :1;		// B3
        Uint16  :1;		// B4
        Uint16  :1;		// B5
        Uint16  :1;		// B6
        Uint16  :1;		// B7

        // unrecover0
        Uint16  unrecoverHwAcOCP:1;				// B1
        Uint16  unrecoverBusOVP:1;				// B2   ,LVRT close INV duty 
        Uint16  unrecoverIacRmsUnbalance:1;		// B3
        Uint16  unrecoverIpvUnbalance:1;		// B5
        Uint16  unrecoverVbusUnbalance:1;		// B6
        Uint16  unrecoverOCPInstant:1;		// B4
 	    Uint16  :1;		// B7 
        Uint16  :1;		// B7

		// unrecover1
        Uint16  :1;		// B0
        Uint16  unrecoverIPVInstant:1;		// B1
		Uint16  :1;		// B2
		Uint16  :1;		// B3
		Uint16  :1;		// B4
        Uint16  :1;		// B5
        Uint16  :1;		// B6
        Uint16  :1;		// B7
     }bit;
};
PARA_EXT union SYS_FAULT_MESSAGE g_SysFaultMessage, g_SlaveSysFaultMessage;

/************************************************************************************/
// System Protection Enable and Disable
union  SYS_FAULT_Enable   
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
    {	// byte 0
        Uint16  :1;			// B0
        Uint16  :1;			// B1
        Uint16  :1;			// B2
        Uint16  :1;			// B3
        Uint16	GridMovAveOVP:1;	// B4
        Uint16  HwADFaultVGrid:1;	// B5
        Uint16  GridOFP1:1;			// B6
        Uint16  GridOFP2:1;			// B7
        

		// byte 1
		Uint16  GridUFP1:1;			// B0
        Uint16  GridUFP2:1;			// B1
        Uint16  PvOVP:1;			// B2
        Uint16  IpvUnbalance:1;		// B3
        Uint16	PvConfigSetWrong:1;	// B4
        Uint16  AcRmsOCP:1;			// B5
        Uint16  HwAcOCP:1;			// B6
        Uint16  HwADFaultIGrid:1;	// B7
        

		// byte 2
		Uint16  IacRmsUnbalance:1;	// B0
        Uint16  BusUVP:1;			// B1
        Uint16  BusOVP:1;			// B2
        Uint16  VbusUnbalance:1;	// B3
        Uint16  DciOCP1:1;			// B4
        Uint16  DciOCP2:1;			// B5
		Uint16  HwADFaultDCI:1;		// B6
        Uint16  GFCIJump1Fault:1;	// B7
		

		// byte 3
		Uint16  GFCIJump2Fault:1;	// B0
		Uint16  GFCIJump3Fault:1;	// B1
		Uint16  GFCIOver300mA:1;	// B2
        Uint16  GFCIDeviceFault:1;	// B3
		Uint16  HwRChipFault:1;		// B4
        Uint16  HwAuxPowerFault:1;	// B5
        Uint16  :1;		// B6
        Uint16  :1;		// B7

        // byte4
        Uint16  HwBusOVP:1;			// B0
        Uint16  HwBoostOCP:1;		// B1
        Uint16  BusVoltZeroFault:1;		// B2
        Uint16  :1;		// B3
        Uint16  :1;		// B4
        Uint16  :1;		// B5
        Uint16  :1;		// B6
        Uint16  :1;		// B7

		// byte5
        Uint16  AntiIsland:1;		// B0
        Uint16  GFCIProtec:1;		// B1
        Uint16  :1;		// B2
        Uint16  :1;		// B3
        Uint16  :1;		// B4
        Uint16  :1;		// B5
        Uint16  :1;		// B6
        Uint16  :1;		// B7

		// byte6
        Uint16	:1;		// B0
        Uint16  :1;		// B1
        Uint16  :1;		// B2
        Uint16  :1;		// B3
        Uint16  :1;		// B4
        Uint16  :1;		// B5
        Uint16  :1;		// B6
        Uint16  :1;		// B7

		// byte7
        Uint16  :1;		// B0
        Uint16  :1;		// B1
        Uint16  :1;		// B2
        Uint16  :1;		// B3
        Uint16  :1;		// B4
        Uint16  :1;		// B5
        Uint16  SPICommFail:1;		// B6
        Uint16  :1;		// B7

        // unrecover0
        Uint16  :1;	// B0
        Uint16  unrecoverHwAcOCP:1;				// B1
        Uint16  unrecoverBusOVP:1;				// B2    
        Uint16  unrecoverIacRmsUnbalance:1;		// B3
        Uint16  :1;		// B4
        Uint16  unrecoverIpvUnbalance:1;		// B5
        Uint16  unrecoverVbusUnbalance:1;		// B6
        Uint16  :1;		// B7

		// unrecover1
        Uint16  :1;		// B0
        Uint16  :1;		// B1
		Uint16  :1;		// B2
		Uint16  :1;		// B3
		Uint16  :1;		// B4
        Uint16  :1;		// B5
        Uint16  :1;		// B6
        Uint16  :1;		// B7
     }bit;
};
PARA_EXT union SYS_FAULT_Enable  g_SysFaultEnable;
/************************************************************************************/
// AD sample data structure definition
struct	AD_SAMPLE
{	
	float32	f32IGridR;
	float32	f32IGridS;
	float32	f32IGridT;

	float32	f32VGridR;
	float32	f32VGridS;
	float32	f32VGridT;

	float32	f32VPV1;
	float32	f32VPV2;
	float32	f32IPV1;
	float32	f32IPV2;

	float32	f32VbusP;
	float32	f32VbusN;

	float32	f32VbusPN;
	float32	f32VbusPNDelta;
	

	float32	f32GFCI;

	float32	f32Mux;
	float32	f32DciR;
	float32	f32DciS;
	float32	f32DciT;
};
PARA_EXT struct	AD_SAMPLE	g_GeneralADbuffer, g_Instantaneous;

// AD calibration data structure
struct	AD_CONVERT
{	
	struct AD_SAMPLE gain;
	struct AD_SAMPLE Defaultgain;
	struct AD_SAMPLE offset;
};
PARA_EXT struct	AD_CONVERT g_ADConvert;
/************************************************************************************/
// Average data structure
struct AD_CALCULATE_AVE
{
	float32	f32VPV1;
	float32	f32VPV2;
	float32	f32VPV;
	float32	f32IPV1;
	float32	f32IPV2;

	float32	f32VbusP;
	float32	f32VbusN;
	float32	f32Vbus;
	
	float32	f32IGridR;
	float32	f32IGridS;
	float32	f32IGridT;

	float32	f32VGridR;
	float32	f32VGridS;
	float32	f32VGridT;

	float32	f32GFCI;

	float32	f32DciR;        
	float32	f32DciS;
	float32	f32DciT;

	float32 f32FGrid;
	float32	f32FGridR;
	float32	f32FGridS;
	float32	f32FGridT;

	float32 f32BusVoltOffset;
	float32	Coff_Dforward;
	float32	VdqVbusForward;
};

// RMS data structure
struct	AD_CALCULATE_RMS
{
//--Sample by main DSP------------------------
	float32	f32IGridR;
	float32	f32IGridS;
	float32	f32IGridT;

	float32	f32VGridR;
	float32	f32VGridS;
	float32	f32VGridT;

	float32	f32VGridRS;
	float32	f32VGridST;
	float32	f32VGridTR;

	float32	f32GFCI;
};

// Power
struct	AD_CALCULATE_POWER_AVE
{
	float32	f32Input1Watt;
	float32	f32Input2Watt;
	float32	f32OutputWatt;
	float32 f32InvP;
	float32 f32InvQ;
};

struct DISPLAY_POWER_AVE
{
	struct	AD_CALCULATE_POWER_AVE AccCalc;
	float32	f32OutputEnergy;
};

// Total data structure of Average and RMS values
struct	AD_ACC_Reg
{
	Uint16 u16FullGridPeriodCnt;

	struct AD_CALCULATE_AVE Ave;
	struct AD_CALCULATE_RMS Rms;
	struct AD_CALCULATE_POWER_AVE Power;		// calculate every grid period
	struct DISPLAY_POWER_AVE PowerDisplay;		// calculate every 500mS
};
PARA_EXT struct	AD_ACC_Reg g_CalcResult, g_ADAcc, g_ADSum;
PARA_EXT float32 g_f32EnergyGain;

struct	MovingAverage10min
{
	float32 f32BufR[200];
	float32 f32BufS[200];
	float32 f32BufT[200];

	float32 f32R;
	float32 f32S;
	float32 f32T;
};
PARA_EXT struct	MovingAverage10min g_VGridMovAve;
/************************************************************************************/
struct MPPTCALCULATE
{
	float32   f32DeltaPowerDC;
	float32   f32PvVoltRef;
	float32   f32PvVolt;
	float32   f32PvRefOld;
	float32   f32PvRefOldOld;
	float32   f32PVWattOld;
	float32   f32PVWattCurrent;
	float32   f32PvOpenVolt;
	float32   f32MPPTStep;
	float32   f32MPPT1Limit;
	float32   f32MPPT2Limit;
	float32   f32MPPTLimit;
	float32	  f32PVVoltLimitPoint;
	float32   f32PowerShiftVolt;
	float32   f32PowerErrNew;
	float32   f32PowerErrOld;
	float32	  f32PowerK_alpha;
	float32	  f32PowerK_beta;

	Uint16    u16FastSearch;
	Uint16    u16OutputPowerLimitFlag;
//	Uint16	  u16BoostOffFlag;
};
PARA_EXT  struct MPPTCALCULATE  g_MpptCalc, g_Mppt1Calc, g_Mppt2Calc;
PARA_EXT float32 g_f32MpptVBusStand;
/************************************************************************************/

union  MPPT_STATE 
{
    Uint16 all:16;
    struct
    {
        Uint16  byte0:8;
        Uint16  byte1:8;
        Uint16  byte2:8;
    }Word;

    struct
    {
        Uint16  InputMode:1;		// B0 && B1
        // InputMode:
        //    0----INDEPENDENT_IN
        //    1----PARALLEL_IN
        //    2----DC_SOURCE_IN
        Uint16  :1;		// B0 && B1
        Uint16  :1;	// B2
        Uint16  :1;			// B3
        Uint16  :1;			// B4
        Uint16  :1;				// B5
        Uint16  :1;	// B6
        Uint16  :1;	// B7

        Uint16  InitFlag1:1;		// B0
        Uint16  InitFlag2:1;		// B1
        Uint16  InitFlag:1;			// B2
        Uint16  PV1ON:1;	// B3
        Uint16  PV2ON:1;	// B4
        Uint16  :1;	// B5
        Uint16  :1;	// B6
        Uint16  :1;//PVSleep:1;			// B7

        Uint16  PV1Test:1;		// B0
        Uint16  PV2Test:1;		// B1
        Uint16  :1;			// B2
        Uint16  :1;	// B3
        Uint16  :1;	// B4
        Uint16  :1;	// B5
        Uint16  :1;	// B6
        Uint16  FactoryMode:1;//PVSleep:1;			// B7
    }bit;
}; 
PARA_EXT union MPPT_STATE  g_MPPTState; 
/************************************************************************************/
struct BUSREFCALCULATE
{
	float32 f32StandbusVolt;
	float32 f32BusRef;
	float32 f32GridPeakVolt;
};
PARA_EXT  struct BUSREFCALCULATE  g_BusRefCal;
/************************************************************************************/
struct BOOST_V_LOOP
{
	float32	f32DeltaPV;
	float32	f32DeltaVbus;
	float32	f32VPVErrOld;
	float32	f32VPVErrNew;
	float32	f32VLoopOut;
};
PARA_EXT struct BOOST_V_LOOP g_BoostVpvControl, g_Boost1VpvControl, g_Boost2VpvControl;

struct BOOST_I_LOOP
{
	float32   f32IPVref;
	float32   f32IPVErrOld;
	float32   f32IPVErrNew;
	float32   f32Duty;
};
PARA_EXT struct BOOST_I_LOOP g_Boost1IpvControl, g_Boost2IpvControl;
/************************************************************************************/

struct MPPT_REG
{
/*    float32   f32InputWatt_New;    
    float32   f32InputWatt_Old;     
    float32   f32OutputWatt;
    float32   f32InputPowerLimit;
    float32   f32OutputPowerLimit;
	float32   f32OutputPowerLimit1;
	float32   f32OutputPowerLimit2;
	float32   f32OutputPowerLimitPre;
    float32   f32PV_Fdb;
    float32   f32PV_Fdb_old;
    float32   f32Id_Ref_Max;      // Id reference max
    float32   f32Id_Ref_Min;       // Id reference min
    float32   f32PV_Volt_Open;      // pv open voltage
	float32   f32PV_Volt_Ref;
*/
	Uint16	  u16BoostOffFlag;
/*
	float32   f32PV_Vol_LimitPoint;
	Uint16    u16PowerLimit_Flag;
	float32	  f32PowerErr_Old;
	float32	  f32PowerErr_New;
	float32	  f32Power_Shift_Volt;
	float32	  f32PowerK_alpha;
	float32	  f32PowerK_beta;
*/
};
PARA_EXT  struct MPPT_REG  MpptReg, MpptReg1, MpptReg2;
/************************************************************************************/
struct BUSPIDCALCULATE
{
	float32	f32VBus; 
	float32	f32VBusErrNew;       // current bus voltage error
	float32	f32VBusErrOld;        // before bus voltage error
	float32	f32VBusPI;
	float32	f32IdRef; 
	float32	f32IdRef_DCSource; 
	float32	f32IdRefMax; //
	float32 f32IqRef;
};
PARA_EXT  struct BUSPIDCALCULATE  g_BusPIDCal;
/************************************************************************************/
struct BUSBANLANCECALCULATE
{
	float32	f32DeltaBusErrNew;       // current bus voltage error
	float32	f32DeltaBusErrOld;        // before bus voltage error
	float32 f32PIBusBanlance;
};
PARA_EXT  struct BUSBANLANCECALCULATE  g_BusBanlanceCal;
/************************************************************************************/
// CurrentPIDcontroller with inner staturation, parameters definition
struct CURRENTPIDCALCULATE
{  
	float32  f32Ialpha;   		// Input: Reference input 
	float32  f32Ibeta;   		// Input: Feedback input 
	float32  f32Id;		// Variable: Error 
	float32  f32Iq;			// Parameter: Proportional gain
	//float32  f32RC_Dout;			// Variable: Proportional output 
	//float32  f32RC_Qout;			// Variable: Integral output 
	//int16  Cnt_RC_K;			// Variable: Derivative output	
	//int16  Cnt_RC_K1;	// Variable: Pre-saturated output
	//int16  Cnt_RC_K3;		// Parameter: Maximum output 
//	float32  DArray[360];		// Parameter: Minimum output
//	float32  QArray[360];   		// Output: PID output 
//	float32  DArray_Last;		// Variable: Saturated difference
//	float32  QArray_Last;
	float32  f32IdErrOld; 	// History: Previous proportional output		
	float32  f32IdErrNew;		
	float32  f32DPI;	
	float32  f32IqErrOld; 	// History: Previous proportional output	
	float32  f32IqErrNew;			// Parameter: Integral correction gain
	float32  f32QPI;		
	float32  f32DutyD;		// 
	float32  f32DutyQ;			// 
	float32  f32DQPI;			// 
//	float32  f32DutyDForward; 		//
//	float32  f32DutyQForward; 		// 
	float32  f32DQF;
	float32  f32DQFReci;
	float32  f32AlphaPI;			// Parameter: Integral correction gain
	float32  f32BetaPI; 		// Parameter: Derivative gain 
	float32  PIa;		// 
	float32  PIb; 		//
	float32  PIc; 		//
	float32  f32Duty3d;		// History: Previous proportional output
	float32  f32DutyMax;
	float32  f32DutyMin;
	float32  f32DutyR;			// Parameter: Integral correction gain
	float32  f32DutyS; 		// Parameter: Derivative gain 
	float32  f32DutyT;		// History: Previous proportional output
	float32  f32DutyADeadTimeCom;
	float32  f32DutyBDeadTimeCom;
	float32  f32DutyCDeadTimeCom;
	float32  f32CapHFCur_vinvR;
	float32  f32CapHFCur_vinvS;
	float32  f32CapHFCur_vinvT;
	//float32  f32Bus_Error_k_1;
	//float32  f32Bus_Error_k;
	//float32  f32Duty_Deltabus;
	//float32  BusBalance_Duty_Max;
};	            
PARA_EXT	struct	CURRENTPIDCALCULATE	g_CurPIDCal;

/************************************************************************************/
struct PLLCALCULATE
{  
	float32  f32Valpha;   		// Input: Reference input 
	float32  f32Vbeta;   		// Input: Feedback input 
	float32  f32Vd;		// 
	float32  f32Vq;			// Parameter: Proportional gain
	float32  f32VdForward;		// 
	float32  f32VqForward;			// Parameter: Proportional gain
	float32  f32Vd_Pos;
	float32  f32Vq_Pos;
	float32  f32Vd_Neg;
	float32  f32Vq_Neg;
	float32  f32VNeg;
	float32  f32VqErrOld;			// Variable: Error //Variable: Proportional output 
	float32  f32VqErrNew;			// Variable: Error //Variable: Integral output
	float32  f32PLLStep; 
	float32  f32ThetaStep;		// Parameter: Minimum output
	float32  f32Theta;   		// Output: PID output 
//	float32  f32AITheta;   		// Output: PID output 
//	float32  f32AITempTheta;   		// Output: PID output 
	float32  SinTheta;		// Variable: Saturated difference
	float32  CosTheta;			// Parameter: Integral gain
//	float32  Sin_AITheta;		// Variable: Saturated difference
//	float32  Cos_AITheta;			// Parameter: Integral gain
//	float32  f32Theta_120;   		// Output: PID output
//	float32  f32Theta120;   		// Output: PID output
//	float32  Sin_Theta_120;		// Variable: Saturated difference
//	float32  Cos_Theta_120;			// Parameter: Integral gain
//	float32  Sin_Theta120;		// Variable: Saturated difference
//	float32  Cos_Theta120;			// Parameter: Integral gain
//	float32  f32Fre_Max;
//	float32  f32Fre_Min;
//	float32  f32Fre_Delta_k;
//	float32  f32Fre_Delta_k_1;

	float32  f32ITheta;   		// Output: PID output 
	float32  ISinTheta;		// Variable: Saturated difference
	float32  ICosTheta;			// Parameter: Integral gain
	float32  AIDeltaTheta;			// Parameter: Integral gain
};	            
PARA_EXT	struct	PLLCALCULATE	g_PLLCal;
/************************************************************************************/
union  SAFTYCONFIG 
{
    Uint16 all:16;
    struct
    {
        Uint16  VoltProCon:8;
        Uint16  FreProCon:8;
        Uint16  DCIProCon:8;
        Uint16  PowerRemoteCon:8;
        Uint16  PowerFreCon:8;
        Uint16  ReactiveCon:8;
        Uint16  LVRTCon:8;
        Uint16  AICon:8;
        Uint16  GFCICon:8;
        Uint16  ISOCon:8;
    }Word;

    struct
    {
		//VoltProCon
        Uint16  OVP1Enable:1;		// B0 && B1
        Uint16  OVP2Enable:1;	// B2
        Uint16  UVP1Enable:1;			// B3
        Uint16  UVP2Enable:1;			// B4
        Uint16  OVPMovEnable:1;				// B5
        Uint16  :1;	// B6
        Uint16  :1;	// B7
        Uint16  :1;	// B7
		//FreProCon
        Uint16  OFP1Enable:1;		// B0
        Uint16  OFP2Enable:1;		// B1
        Uint16  UFP1Enable:1;			// B2
        Uint16  UFP2Enable:1;	// B3
        Uint16  :1;	// B4
        Uint16  :1;	// B5
        Uint16  :1;	// B6
        Uint16  :1;//PVSleep:1;			// B7
		//DCIProCon
        Uint16  DCI1Enable:1;		// B0
        Uint16  DCI2Enable:1;		// B1
        Uint16  DCITestEnable:1;			// B2
        Uint16  :1;	// B3
        Uint16  :1;	// B4
        Uint16  :1;	// B5
        Uint16  :1;	// B6
        Uint16  :1;//PVSleep:1;			// B7
		//PowerRemoteCon
        Uint16  RemoteDeratingEnable:1;		// B0
        Uint16  RemoteONOFFEnable:1;		// B1
        Uint16  :1;			// B2
        Uint16  :1;	// B3
        Uint16  :1;	// B4
        Uint16  :1;	// B5
        Uint16  :1;	// B6
        Uint16  :1;//PVSleep:1;			// B7
		//PowerFreCon
        Uint16  FreDeratingEnable:1;		// B0
        Uint16  :1;		// B1
        Uint16  :1;			// B2
        Uint16  :1;	// B3
        Uint16  :1;	// B4
        Uint16  :1;	// B5
        Uint16  :1;	// B6
        Uint16  :1;//PVSleep:1;			// B7
		//ReactiveCon
        Uint16  ReactiveEnable:1;		// B0
        Uint16  ReactiveMode:3;		// B1
        Uint16  :1;			// B2
        Uint16  :1;	// B3
        Uint16  :1;	// B4
        Uint16  :1;	// B5
		//LVRTCon
        Uint16  LVRTEnable:1;		// B0
        Uint16  :1;		// B1
        Uint16  :1;			// B2
        Uint16  :1;	// B3
        Uint16  :1;	// B4
        Uint16  :1;	// B5
        Uint16  :1;	// B6
        Uint16  :1;//PVSleep:1;			// B7
		//AICon
        Uint16  AIEnable:1;		// B0
        Uint16  :1;		// B1
        Uint16  :1;			// B2
        Uint16  :1;	// B3
        Uint16  :1;	// B4
        Uint16  :1;	// B5
        Uint16  :1;	// B6
        Uint16  :1;//PVSleep:1;			// B7
		//GFCICon
        Uint16  GFCIEnable:1;		// B0
        Uint16  :1;		// B1
        Uint16  :1;			// B2
        Uint16  :1;	// B3
        Uint16  :1;	// B4
        Uint16  :1;	// B5
        Uint16  :1;	// B6
        Uint16  :1;//PVSleep:1;			// B7
		//ISOCon
        Uint16  ISOEnable:1;		// B0
        Uint16  :1;		// B1
        Uint16  :1;			// B2
        Uint16  :1;	// B3
        Uint16  :1;	// B4
        Uint16  :1;	// B5
        Uint16  :1;	// B6
        Uint16  :1;//PVSleep:1;			// B7
    }bit;
}; 
PARA_EXT union SAFTYCONFIG  g_SaftyConfig; 
// Safety Parameters
typedef struct
{

	float32	f32KVPV1;				//
	float32	f32OFFSETVPV1;				// 
	float32	f32KIPV1;				// 
	float32	f32OFFSETIPV1;				// 
	float32	f32KPowerPV1;				// 电网电压下限1
	float32	f32OFFSETPowerPV1;				// 电网电压下限2

	float32	f32KVPV2;				// 电网电压下限1
	float32	f32OFFSETVPV2;				// 电网电压下限2
	float32	f32KIPV2;				// 
	float32	f32OFFSETIPV2;				// 
	float32	f32KPowerPV2;				// 电网电压下限1
	float32	f32OFFSETPowerPV2;				// 电网电压下限2

	float32	f32KVBUS;				//
	float32	f32OFFSETVBUS;				// 

	float32	f32KPowerOutput;				// 电网电压下限1
	float32	f32OFFSETPowerOutput;				// 电网电压下限2

	float32	f32KGridVR;				//
	float32	f32OFFSETGridVR;				// 
	float32	f32KGridIR;				// 
	float32	f32OFFSETGridIR;				// 

	float32	f32KGridVS;				//
	float32	f32OFFSETGridVS;				// 
	float32	f32KGridIS;				// 
	float32	f32OFFSETGridIS;				// 


	float32	f32KGridVT;				//
	float32	f32OFFSETGridVT;				// 
	float32	f32KGridIT;				// 
	float32	f32OFFSETGridIT;				// 

	float32	f32SlaveKVPV1;				//
	float32	f32SlaveOFFSETVPV1;				// 

	float32	f32SlaveKVPV2;				//
	float32	f32SlaveOFFSETVPV2;				// 


}CALIBRATION_PARAMETER_REG;

PARA_EXT CALIBRATION_PARAMETER_REG	g_CalibrationPara;


// Safety Parameters
typedef struct
{

	float32 f32VGridMaxLimitBack;			// 启动/重连电网电压上限
	float32 f32VGridMinLimitBack;			// 启动/重连电网电压下限

	float32 f32FGridMaxLimitBack;			// 启动/重连电网频率上限
	float32 f32FGridMinLimitBack;			// 启动/重连电网频率下限

	float32 f32PupSlop;			// 启动/重连电网电压上限
	float32 f32ReconnectPupSlop;			// 启动/重连电网电压上限

	Uint16	u16ConnectTime;					// 启动时间
	Uint16	u16ReconnectTime;				// 重连时间

}SAFETY_CONNECTPARAMETER;

PARA_EXT SAFETY_CONNECTPARAMETER	g_SafetyConnect;
// Safety Parameters
typedef struct
{
	float32	f32VGridMax1Limit;				// 电网电压上限1
	float32	f32VGridMax2Limit;				// 电网电压上限2
	float32	f32VGridMin1Limit;				// 电网电压下限1
	float32	f32VGridMin2Limit;				// 电网电压下限2

	float32	f32VGridTenMinutesLimit;				// 电网电压上限1

	Uint16	u16VGridMax1ProtectTime;		// 电网电压上限1保护时间
	Uint16  u16VGridMax2ProtectTime;		// 电网电压上限2保护时间
	Uint16  u16VGridMin1ProtectTime;		// 电网电压下限1保护时间
	Uint16  u16VGridMin2ProtectTime;		// 电网电压下限2保护时间

}SAFETY_VOLTPROTECT;

PARA_EXT SAFETY_VOLTPROTECT	g_SafetyVolt;
// Safety Parameters
typedef struct
{
	float32	f32FGridMax1Limit;				// 电网频率上限1
	float32	f32FGridMax2Limit;				// 电网频率上限2
	float32	f32FGridMin1Limit;				// 电网频率下限1
	float32	f32FGridMin2Limit;				// 电网频率下限2

	Uint16	u16FGridMax1ProtectTime;		// 电网频率上限1保护时间
	Uint16	u16FGridMax2ProtectTime;		// 电网频率上限2保护时间
	Uint16  u16FGridMin1ProtectTime;		// 电网频率下限1保护时间
	Uint16  u16FGridMin2ProtectTime;		// 电网频率下限2保护时间

}SAFETY_FREQPROTECT;

PARA_EXT SAFETY_FREQPROTECT	g_SafetyFreq;

//GridActivePowerLimit
typedef struct
{
	Uint16   u16Enable;
	float32  f32PLimitCommand;
	Uint16   RemoteOnOff;
	Uint16   u16PLimit;
}SAFETY_ACTIVECON;
PARA_EXT SAFETY_ACTIVECON	g_SafetyActive;

// Safety Parameters
typedef struct
{
	float32	f32DciMax1Limit;				// DCI上限1
	float32	f32DciMax2Limit;  				// DCI上限2   
	Uint16	u16DciMax1ProtectTime;			// DCI上限1保护时间
	Uint16	u16DciMax2ProtectTime;			// DCI上限2保护时间
	float32	f32DciTestInjection;			// DCI上限2保护时间

}SAFETY_DCIPROTECT;

PARA_EXT SAFETY_DCIPROTECT	g_SafetyDCI;

// Safety Parameters
typedef struct
{
	Uint16	u16SafeCountry;

	Uint16	u16Riso;			// DCI上限2保护时间
	Uint16	CoMMDate;					// 启动时间
	float32	FactoryCurrentLimit;					// 启动时间

}SAFETY_PARAMETER_REG;

PARA_EXT SAFETY_PARAMETER_REG	g_SafetyReg;

PARA_EXT Uint16 g_u16SafetyCountDown;	// 安规倒计时
PARA_EXT float32 g_f32PowerLoadSpeed;	// 安规倒计时
PARA_EXT Uint16 g_u16First4S;
PARA_EXT Uint16 g_u16RelayDelayCnt;
/************************************************************************************/
// Judge standards by Samilpower
typedef struct
{
	float32 f32OutputCurrentRms;
	float32 f32OutputCurrentPeak;
	float32 f32HalfInputPower;
	float32 f32InputPower;
	float32 f32OutputPower;
	float32 f32InputCurrent;
	float32 f32VinUnderDerated;
	float32 f32VinUnderVolt;
	int16	i16TAmbDratingPoint;
	int16	i16TInvDratingPoint;
	int16	i16TNTCDratingPoint;
}RATED;
PARA_EXT RATED g_Rated;

// Judge standards by Samilpower
typedef struct
{
	float32 f32PActiveCommand;
	float32 f32PActiveSoftStart;
	float32 f32PActiveFreq;
	float32 f32SmaxVo;
	float32 f32SmaxTemperature;
	float32 f32SMaxLimit;
	float32 f32PActiveMaxLimit;
	float32 f32PReactiveCommand;
	float32 f32FactoryCurrent;
	float32 f32Pinput1;
	float32 f32Pinput2;
	float32 f32PinputAll;
}PACTIVELIMIT;
PARA_EXT PACTIVELIMIT g_PActiveLimit;

/************************************************************************************/
//SCI
//Queue structure																	

typedef struct
{
	Uint8	*pu8In;
	Uint8	*pu8Out;
	Uint8	*pu8Start;
	Uint16	u16Length;
	Uint16 	u16Size;
}QUEUE;
			
//Sci structure																		
//Including  tranmit and receive queue structure and Tx,Rx threshold control variabls*

typedef struct
{
	Uint8	u8TxStatus;
	Uint16	u16TxLength;
	QUEUE	*pqRx;
}SciStruct;
PARA_EXT SciStruct	*g_pSciIndex[MAX_SCI_NO];

/************************************************************************************/
//GridManagerment
//GridReconnect
struct InvConnect
{
	Uint16   u16ConnectTime;
	float32  f32ConnectSpeed;
	Uint16   u16ReconnectTime;
	float32  f32ReconnnetSpeed;
	float32  f32VOback;
	float32  f32VUback;
	float32  f32FOback;
	float32  f32FUback;
	float32  f32PSpeed;
	Uint16   u16PLimit;
};
//PfLimit
struct PowerFreqLimit
{
//	Uint16   u16Enable;
	float32  f32FreqPoint;
	float32  f32Slop;
	float32  f32FOback;
	float32  f32FUback;
	Uint16   u16WaitTime;
//	Uint16   u16BackMode;
	float32  f32BackSpeed;
//	Uint16   u16StartFlag;
	float32   u16PowerM;
//	float32   u16PowerMback;
	float32   u16FPLimit; 
	Uint16   u16ReloadFlag;
	Uint16   u16WaitReloadFlag;
	Uint16   u16PowerFixedPointFlag;
};

struct InvQManager
{
	Uint16   u16Enable;
	float32  f32Cosphi;
	float32  f32Qvar;
	float32  f32Cosphi1;
	float32  f32Pwatt1;
	float32  f32Cosphi2;
	float32  f32Pwatt2;
	float32  f32Cosphi3;
	float32  f32Pwatt3;
	float32  f32Cosphi4;
	float32  f32Pwatt4;
	float32  f32LockinV;
	float32  f32LockoutV;
	float32  f32U1s;
	float32  f32U2s;
	float32  f32U1i;
	float32  f32U2i;
	float32  f32LockinP;
	float32  f32LockoutP;
	float32  f32QvarMax;
	float32  f32QTime; 
	float32  f32Tanphi;
	Uint16   u16LockFlag;
	Uint16   u16QULockFlag;
};

struct LVRTManager
{
	Uint16   u16Enable;
	float32  f32VLvrt;
	Uint16   u16Vpoint1;
	Uint16   u16Tpoint1;
	Uint16   u16Vpoint2;
	Uint16   u16Tpoint2;
	Uint16   u16Vpoint3;
	Uint16   u16Tpoint3;
	Uint16   u16Vpoint4;
	Uint16   u16Tpoint4;
	Uint16   u16K;
	Uint16   u16Tback;
	Uint16   u16PLvrtback;
	Uint16   u16StartFlag;
	Uint16   u16LvrtNormalDelay;
	float32  f32VdposFltered;
	float32  f32IqSave;
	float32  f32IqRef;
	float32  f32PLvrtLimit;
};

struct GridManager
{
	float32 f32IPMOSLooK;
	float32 f32IPMOSLooK2;
	float32 f32PLimit20ms;
	float32 f32PLimit500ms;
	struct InvConnect   InvCon;
	struct PowerFreqLimit  PFreq;
	struct InvQManager InvQ;
	struct LVRTManager Lvrt;
};
PARA_EXT struct GridManager g_GridManager;

PARA_EXT int16 g_i16_Temperature_Inv;	// for test
PARA_EXT int16 g_i16_Temperature_Boost;	// for test
PARA_EXT int16 g_i16_Temperature_Amb;	// for test
/************************************************************************************/
//Spi data Buffer
PARA_EXT Uint16 g_u16SpiTxBuf[35];
PARA_EXT Uint16 g_u16SpiRxBuf[35];
PARA_EXT Uint16 g_u16SpiFaultCnt;
//PARA_EXT Uint16 g_u16SlaveStateCheckByte3;		// for test
//PARA_EXT Uint16 g_u16SlaveMessageFaultByte7;	// for test

//PARA_EXT Uint16 g_u16SlaveMessageFaultByte6;	// for test
//PARA_EXT Uint16 g_u16SlaveMessageFaultUnrecover;	// for test

//PARA_EXT Uint16 g_u16SPIErrorCnt;	// for test
//PARA_EXT Uint16 g_u16InputModeChangCnt;	// for test
/************************************************************************************/
//PARA_EXT Uint32 g_u32VgridRFreqCnt;
//PARA_EXT Uint32 g_u32VgridSFreqCnt;


extern volatile Uint8 SciRxBuff[80];
extern volatile Uint8 SciReceivePonit;
extern Uint32 Flash_CPUScaleFactor;
//===========================================================================
// No more.
//===========================================================================

