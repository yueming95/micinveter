//###########################################################################
//
// FILE:   SL_slave_Spi.c
//
// TITLE:  DSP2803x SPI Initialization & Support Functions.
//
//###########################################################################

#include "SL_slave.h"

void SetSlaveValue();
void PackageSlaveData();
Uint16 SpiChecksum(Uint16* pData,Uint8 len);
Uint16 SPI_Packet_point=0;
Uint16 MASTER_MAIN_VERSION;
Uint16 MASTER_MAIN_VERSION0;
Uint16 MASTER_SECONDARY_VERSION2;
Uint16 MASTER_SECONDARY_VERSION1;

extern Uint16 EnergyTodayOld;
extern Uint16 ILrmsMPPTlimit;
extern Uint16 Todaydate;
extern Uint16 Orig_epromflg;
//extern Uint16 SafetySTARTTable[8];
extern Uint16 SafetyVTable[10];
extern Uint16 SafetyFreTable[9];
extern Uint16 SafetyDCITable[6];
extern Uint16 SafetyRemotPowerControlTable[3];
extern Uint16 SafetyFreqPowerControlTable[7];
extern Uint16 SafetyReactiveTable[21];
extern Uint16 SafetyLVRTTable[13];	
extern Uint16 SafetyIsoIslandandSoOnTable[4];  
extern Uint16 CalibrationTable[32];
extern Uint16 FactoryMode;
extern Uint16 SafetyCountry;
extern Uint16 InputMode;
extern Uint16 PowerLevelType;
extern Uint16 MainDSPWarningByte;
extern Uint16 g_SPICommCnt;
//---------------------------------------------------------------------------
// InitSPI:
//---------------------------------------------------------------------------
// This function initializes the SPI(s) to a known state.
//
void InitSpi(void)
{
    // Initialize SPI-A
	// Initialize SPI-B of Slave DSP
	EALLOW;

    GpioCtrlRegs.GPAPUD.bit.GPIO24 = 0;   
    GpioCtrlRegs.GPAPUD.bit.GPIO13 = 0;  
    GpioCtrlRegs.GPAPUD.bit.GPIO14 = 0;   
    GpioCtrlRegs.GPAPUD.bit.GPIO15 = 0;   

	GpioCtrlRegs.GPAQSEL2.bit.GPIO24 = 3; // Asynch input GPIO24 (SPISIMOB)端口的采样与系统时钟
    GpioCtrlRegs.GPAQSEL1.bit.GPIO13 = 3; // Asynch input GPIO13 (SPISOMIB)
    GpioCtrlRegs.GPAQSEL1.bit.GPIO14 = 3; // Asynch input GPIO14 (SPICLKB)
    GpioCtrlRegs.GPAQSEL1.bit.GPIO15 = 3; // Asynch input GPIO15 (SPISTEB)

	GpioCtrlRegs.GPAMUX1.bit.GPIO15	= 3;	// pin 75, /SPISTEB, 与主CPU通讯SPI使能
    GpioCtrlRegs.GPAMUX1.bit.GPIO14	= 3;	// pin 77, SPICLKB,  与主CPU通讯SPI时钟
    GpioCtrlRegs.GPAMUX2.bit.GPIO24	= 3;	// pin 80, SPISIMOB, 与主CPU通讯SPI接收
    GpioCtrlRegs.GPAMUX1.bit.GPIO13	= 3;	// pin 76, SPISOMIB, 与主CPU通讯SPI发送

	EDIS;

	EALLOW;

	SpibRegs.SPICCR.bit.SPISWRESET    = 0;		// Initializes the SPI operating flags to the reset condition.
	SpibRegs.SPICCR.bit.CLKPOLARITY   = 1;		// Data is output on falling edge and input on rising edge.
	SpibRegs.SPICCR.bit.SPILBK        = 0;		// Disable loop back mode.
	SpibRegs.SPICCR.bit.SPICHAR       = 0xF;	// Character lenth is 16 bit.

	SpibRegs.SPICTL.bit.OVERRUNINTENA = 0;		// Disable receiver overrun flag bit interrupts.
	SpibRegs.SPICTL.bit.CLK_PHASE     = 0;		// Normal phase, no delay clock.
	SpibRegs.SPICTL.bit.MASTER_SLAVE  = 0;		// Slave.
	SpibRegs.SPICTL.bit.TALK          = 1;		// Enable transmission.
	SpibRegs.SPICTL.bit.SPIINTENA     = 1;		// Enable interrupt.
                                                 
	SpibRegs.SPIBRR                   = 49;		// Rate is 300000bps = (60000000/4) / 50.
	SpibRegs.SPIPRI.bit.FREE          = 1;		// Free fun, continue SPI operation regardless of suspend			
    SpibRegs.SPICCR.bit.SPISWRESET    = 1;		// Relinquish SPI from Reset 

	EDIS;

	PieCtrlRegs.PIECTRL.bit.ENPIE = 1; 
	PieCtrlRegs.PIEIER6.bit.INTx3 = 1;		// Enable INT 6.3 in the PIE, SPIRxINTB is INT6.3
   	IER |= M_INT6; 							// Enable CPU Interrupt 1 
	SetSlaveValue();
	SpibRegs.SPITXBUF = (((Uint16)'S'<<8)|0xAA);
}
/****************************************************************************
Fuction name:	SetSlaveValue:
*****************************************************************************/
void SetSlaveValue()
{
	g_SpiTxBuf[0] = ((Uint16)'S'<<8)|0x00AA;
	g_SpiTxBuf[1] = 0;//0xFF is reserved
	g_SpiTxBuf[2] = s_StateCheck.Word.byte2;
	g_SpiTxBuf[3] = ((((Uint16)s_SysFaultMessage.Word.byte0)<<8)|(Uint16)s_SysFaultMessage.Word.byte1);
	g_SpiTxBuf[4] = ((((Uint16)s_SysFaultMessage.Word.byte2)<<8)|(Uint16)s_SysFaultMessage.Word.byte3);
	g_SpiTxBuf[5] = ((((Uint16)s_SysFaultMessage.Word.byte4)<<8)|(Uint16)s_SysFaultMessage.Word.byte5);
	g_SpiTxBuf[6] = ((((Uint16)s_SysFaultMessage.Word.byte6)<<8)|(Uint16)s_SysFaultMessage.Word.byte7);
	g_SpiTxBuf[7] = ((((Uint16)s_SysFaultMessage.Word.unrecover0)<<8)|(Uint16)s_SysFaultMessage.Word.unrecover1); 

	PackageSlaveData();    	//g_SpiTxBuf[8] - g_SpiTxBuf[18]

//	g_SpiTxBuf[21] = ILrmsMPPTlimit;
//	g_SpiTxBuf[22] = CalcResultAve.i16VISO1_testoutput1;//(Uint16)ReactiveValue.u8ReactiveMode2;
//	ReactiveValue.i32ReactiveCos = ReactiveValue.i32ReactivefixedCos*100;
//	ReactiveValue.i32ReactiveCos = ReactiveValue.i32ReactiveCos>>24;
//	g_SpiTxBuf[23] = CalcResultAve.i16VISO1_testoutput2;//ReactiveValue.i32ReactiveCos;
//	g_SpiTxBuf[24] = CalcResultAve.i16VISO1_testoutput3;//0;
	g_SpiTxBuf[25] = s_SysCurrentState;
	g_SpiTxBuf[26] = s_StateCheck.Word.byte3;//test
	g_SpiTxBuf[27] = s_SysFaultMessage.Word.byte7;
	g_SpiTxBuf[28] = CalcResultAve.i16TempINVMax;
	g_SpiTxBuf[29] = CalcResultAve.i16TempAmb;
	g_SpiTxBuf[30] = CalcResultAve.i16TempBoost1;//CalcResultAve.i16TempINVT;
	g_SpiTxBuf[31] = SpiChecksum(g_SpiTxBuf,31);
} 
/************************************************************************
Faction name:	PackageSlaveData
*************************************************************************/
void PackageSlaveData()
{
	static Uint8 b_SpiSendCnt = 0;
//	static Uint16 b16_SpiSendCnt = 1234;
	g_SpiTxBuf[8] = b_SpiSendCnt;
	switch(b_SpiSendCnt)
	{
		case 0x00:
			g_SpiTxBuf[9]  = SafetyCountry;//SafetyReg.u16SafeCountry;
			g_SpiTxBuf[10] = InputMode;//SafetyReg.i16ReconnectionTime;
			g_SpiTxBuf[11] = CalcResultAve.i16VISO1_testoutput1;//;
			g_SpiTxBuf[12] = CalcResultAve.i16VISO1_testoutput2;//SafetyReg.i16VGrid_Min2Limit;
			g_SpiTxBuf[13] = CalcResultAve.i16VISO1_testoutput3;//SafetyReg.i16VGrid_Min1_ProtectTime;
			g_SpiTxBuf[14] = Todaydate;//SafetyReg.i16VGrid_Min2_ProtectTime;
			g_SpiTxBuf[15] = CalcResultAve.i16VPV1;//SafetyReg.i16VGrid_Max1Limit;	
			g_SpiTxBuf[16] = CalcResultAve.i16VPV2;//SafetyReg.i16VGrid_Max2Limit;
			g_SpiTxBuf[17] = 0;//SafetyReg.i16VGrid_Max1_ProtectTime;
			g_SpiTxBuf[18] = 0;//SafetyReg.i16VGrid_Max2_ProtectTime;
			g_SpiTxBuf[19] = 0;
			g_SpiTxBuf[20] = 0;
		break;
		case 0x01:
			g_SpiTxBuf[9] = CalcResultRms.i16VGRID_R;
			g_SpiTxBuf[10] = CalcResultRms.i16VGRID_S;	
			g_SpiTxBuf[11] = CalcResultRms.i16VGRID_T;
			g_SpiTxBuf[12] = CalcResultRms.i16VINV_R;
			g_SpiTxBuf[13] = CalcResultRms.i16VINV_S;
			g_SpiTxBuf[14] = CalcResultRms.i16VINV_T;
			g_SpiTxBuf[15] = CalcResultAve.i16DCI_R;
			g_SpiTxBuf[16] = CalcResultAve.i16DCI_S;
			g_SpiTxBuf[17] = CalcResultAve.i16DCI_T;	
			g_SpiTxBuf[18] = CalcResultRms.i16GFCI;
			g_SpiTxBuf[19] = (Uint16)(PLLCal.i32ThetaStep>>8);
			g_SpiTxBuf[20] = PLLCal.i16Vd;//

		break;
		case 0x02:
			g_SpiTxBuf[9]  = InputMode;//SafetyReg.i16DCI_Max1Limit;//
			g_SpiTxBuf[10] = 0;//SafetyReg.i16DCI_Max2Limit;//
			g_SpiTxBuf[11] = 0;//SafetyReg.i16DCI_Max1_ProtectTime;//
			g_SpiTxBuf[12] = 0;//SafetyReg.i16DCI_Max2_ProtectTime;	
			g_SpiTxBuf[13] = 0;//SafetyReg.i16ReconnectionTime;
			g_SpiTxBuf[14] = 0;
			g_SpiTxBuf[15] = 0;
			g_SpiTxBuf[16] = 0;

			g_SpiTxBuf[17] = 0;
			g_SpiTxBuf[18] = 0;
			g_SpiTxBuf[19] = 0;
			g_SpiTxBuf[20] = 0;
		break;

		case 0x30:
//Tconnection PowerUpSpeed	Tback PowerbackUpSpeed	VGridBack>	VGridBack<	  FGridBack> FGridBack<
//      {60,	20,			60,		20,				2060,			2660,		5449,		4510},
//SafetySTARTTable[8]
			g_SpiTxBuf[9] = SafetySTARTTable[0];//1
			g_SpiTxBuf[10] = SafetySTARTTable[1];//2
			
			g_SpiTxBuf[11] = SafetySTARTTable[2];//3
			g_SpiTxBuf[12] = SafetySTARTTable[3];//4

			g_SpiTxBuf[13] = SafetySTARTTable[4];//5
			g_SpiTxBuf[14] = SafetySTARTTable[5];//6

			g_SpiTxBuf[15] = SafetySTARTTable[6];//7
			g_SpiTxBuf[16] = SafetySTARTTable[7];//8

			g_SpiTxBuf[17] = 0;//9
			g_SpiTxBuf[18] = 0;//10
			
			g_SpiTxBuf[19] = 0;//11
			g_SpiTxBuf[20] = 0;//12

		break;

		case 0x31:	
//EABLE,VHi1, Time, VLow1,  Time,   VHi2, Time2,	VLow2,	Time  10MINV,
//{0,   2660,  20,  2030,	200,	2800,	60,		1750,	60   2530,},
//SafetyVTable[10]
			g_SpiTxBuf[9] = SafetyVTable[0];//1
			g_SpiTxBuf[10] = SafetyVTable[1];
			g_SpiTxBuf[11] = SafetyVTable[2];//4
			g_SpiTxBuf[12] = SafetyVTable[3];//5
			g_SpiTxBuf[13] = SafetyVTable[4];//6
			g_SpiTxBuf[14] = SafetyVTable[5];//7
			g_SpiTxBuf[15] = SafetyVTable[6];//8
			g_SpiTxBuf[16] = SafetyVTable[7];//9
			g_SpiTxBuf[17] = SafetyVTable[8];//10
			g_SpiTxBuf[18] = SafetyVTable[9];//SafetyVTable[1];//2
						
			g_SpiTxBuf[19] = 0;//11
			g_SpiTxBuf[20] = 0;//12
		break;

		case 0x32:			
//EABLE,  FHi1, Time10,	FLow1, Time10,  FHi2,  Time,   FLow2,  Time,
//	{0,   5490,   20,	4510,	20,		5600,	100,	4400,	100},
//SafetyFreTable[9]
			g_SpiTxBuf[9] = SafetyFreTable[0];//1
			g_SpiTxBuf[10] = SafetyFreTable[1];//2
			
			g_SpiTxBuf[11] = SafetyFreTable[2];//3
			g_SpiTxBuf[12] = SafetyFreTable[3];//4

			g_SpiTxBuf[13] = SafetyFreTable[4];//5
			g_SpiTxBuf[14] = SafetyFreTable[5];//6

			g_SpiTxBuf[15] = SafetyFreTable[6];//7
			g_SpiTxBuf[16] = SafetyFreTable[7];//8

			g_SpiTxBuf[17] = SafetyFreTable[8];//9
			g_SpiTxBuf[18] = 0;//10
			
			g_SpiTxBuf[19] = 0;//11
			g_SpiTxBuf[20] = 0;//12
		break;

		case 0x33:			
//SafetyDCITable[6]
//{0, 	500, 	400, 	500, 	400,	0},
			g_SpiTxBuf[9] = SafetyDCITable[0];//1
			g_SpiTxBuf[10] = SafetyDCITable[1];//2
			
			g_SpiTxBuf[11] = SafetyDCITable[2];//3
			g_SpiTxBuf[12] = SafetyDCITable[3];//4

			g_SpiTxBuf[13] = SafetyDCITable[4];//5
			g_SpiTxBuf[14] = SafetyDCITable[5];//6

			g_SpiTxBuf[15] = 0;//7
			g_SpiTxBuf[16] = 0;//8

			g_SpiTxBuf[17] = 0;//9
			g_SpiTxBuf[18] = 0;//10
			
			g_SpiTxBuf[19] = 0;//11
			g_SpiTxBuf[20] = 0;//12
		break;
		

		case 0x34:	
// 	{0,   100,	55},
//SafetyRemotPowerControlTable[3]
			g_SpiTxBuf[9] = SafetyRemotPowerControlTable[0];//1
			g_SpiTxBuf[10] = SafetyRemotPowerControlTable[1];//2
			
			g_SpiTxBuf[11] = SafetyRemotPowerControlTable[2];;//3
			g_SpiTxBuf[12] = 0;//4

			g_SpiTxBuf[13] = 0;//5
			g_SpiTxBuf[14] = 0;//6

			g_SpiTxBuf[15] = 0;//7
			g_SpiTxBuf[16] = 0;//8

			g_SpiTxBuf[17] = 0;//9
			g_SpiTxBuf[18] = 0;//10
			
			g_SpiTxBuf[19] = 0;//11
			g_SpiTxBuf[20] = 0;//12
		break;

		case 0x35:	
//SafetyFreqPowerControlTable[7]
//{0,     5030,   24,  5010,     4990,      300,          	20 },
			g_SpiTxBuf[9] = SafetyFreqPowerControlTable[0];//1
			g_SpiTxBuf[10] = SafetyFreqPowerControlTable[1];//2
			
			g_SpiTxBuf[11] = SafetyFreqPowerControlTable[2];//3
			g_SpiTxBuf[12] = SafetyFreqPowerControlTable[3];//4

			g_SpiTxBuf[13] = SafetyFreqPowerControlTable[4];//5
			g_SpiTxBuf[14] = SafetyFreqPowerControlTable[5];//6

			g_SpiTxBuf[15] = SafetyFreqPowerControlTable[6];//7
			g_SpiTxBuf[16] = 0;//8

			g_SpiTxBuf[17] = 0;//9
			g_SpiTxBuf[18] = 0;//10
			
			g_SpiTxBuf[19] = 0;//11
			g_SpiTxBuf[20] = 0;//12
		break;


		case 0x36:	
//{0, 100, 0, 100, 50, 100, 50, 100, 50, 95, 100, 105,  100, 108, 110, 92, 90, 20, 5, 4845, 2 },
//SafetyReactiveTable[21]
			g_SpiTxBuf[9] = SafetyReactiveTable[0];//1
			g_SpiTxBuf[10] = SafetyReactiveTable[1];//2
			
			g_SpiTxBuf[11] = SafetyReactiveTable[2];//3
			g_SpiTxBuf[12] = SafetyReactiveTable[3];//4

			g_SpiTxBuf[13] = SafetyReactiveTable[4];//5
			g_SpiTxBuf[14] = SafetyReactiveTable[5];//6

			g_SpiTxBuf[15] = SafetyReactiveTable[6];//7
			g_SpiTxBuf[16] = SafetyReactiveTable[7];//8

			g_SpiTxBuf[17] = SafetyReactiveTable[8];//9
			g_SpiTxBuf[18] = SafetyReactiveTable[9];//10
			
			g_SpiTxBuf[19] = SafetyReactiveTable[10];//11
			g_SpiTxBuf[20] = SafetyReactiveTable[11];//12
			g_SpiTxBuf[21] = SafetyReactiveTable[12];//11
			g_SpiTxBuf[22] = SafetyReactiveTable[13];//12
			g_SpiTxBuf[23] = SafetyReactiveTable[14];//11
			g_SpiTxBuf[24] = SafetyReactiveTable[15];//12
		break;

		case 0x37:	
//{0, 100, 0, 100, 50, 100, 50, 100, 50, 95, 100, 105,  100, 108, 110, 92, 90, 20, 5, 4845, 2 },
//SafetyReactiveTable[21]

			g_SpiTxBuf[9] = SafetyReactiveTable[16];//1
			g_SpiTxBuf[10] = SafetyReactiveTable[17];//2
			g_SpiTxBuf[11] = SafetyReactiveTable[18];//3
			g_SpiTxBuf[12] = SafetyReactiveTable[19];//4
			g_SpiTxBuf[13] = SafetyReactiveTable[20];//5

		break;

		case 0x38:	
//SafetyLVRTTable[13]
//{1,	85,	0,	200, 40, 0,	40,	300, 85, 0,	2, 0, 200},
			g_SpiTxBuf[9] = SafetyLVRTTable[0];//1
			g_SpiTxBuf[10] = SafetyLVRTTable[1];//2
			
			g_SpiTxBuf[11] = SafetyLVRTTable[2];//3
			g_SpiTxBuf[12] = SafetyLVRTTable[3];//4

			g_SpiTxBuf[13] = SafetyLVRTTable[4];//5
			g_SpiTxBuf[14] = SafetyLVRTTable[5];//6

			g_SpiTxBuf[15] = SafetyLVRTTable[6];//7
			g_SpiTxBuf[16] = SafetyLVRTTable[7];//8

			g_SpiTxBuf[17] = SafetyLVRTTable[8];//9
			g_SpiTxBuf[18] = SafetyLVRTTable[9];//10
			
			g_SpiTxBuf[19] = SafetyLVRTTable[10];//11
			g_SpiTxBuf[20] = SafetyLVRTTable[11];//12
			g_SpiTxBuf[21] = SafetyLVRTTable[12];//11
		break;

		case 0x39:	

			g_SpiTxBuf[9] = 0;//1
			g_SpiTxBuf[10] = 0;//2
			
			g_SpiTxBuf[11] = 0;//3
			g_SpiTxBuf[12] = 0;//4

			g_SpiTxBuf[13] = 0;//5
			g_SpiTxBuf[14] = 0;//6

			g_SpiTxBuf[15] = 0;//7
			g_SpiTxBuf[16] = 0;//8

			g_SpiTxBuf[17] = 0;//9
			g_SpiTxBuf[18] = 0;//10
			
			g_SpiTxBuf[19] = 0;//11
			g_SpiTxBuf[20] = 0;//12
		break;

		case 0x3A:	
//  {0, 0,  1000, 0 },
//SafetyIsoIslandandSoOnTable[4]
			g_SpiTxBuf[9] = SafetyIsoIslandandSoOnTable[0];//1
			g_SpiTxBuf[10] = SafetyIsoIslandandSoOnTable[1];//2
			
			g_SpiTxBuf[11] = SafetyIsoIslandandSoOnTable[2];//3
			g_SpiTxBuf[12] = SafetyIsoIslandandSoOnTable[3];//4

			g_SpiTxBuf[13] = 0;//5
			g_SpiTxBuf[14] =  0;//0;//6

			g_SpiTxBuf[15] = 0;//7
			g_SpiTxBuf[16] = 0;//0;//8

			g_SpiTxBuf[17] = 0;//9
			g_SpiTxBuf[18] = 0;//10
			
			g_SpiTxBuf[19] = 0;//11
			g_SpiTxBuf[20] = 0;//b16_SpiSendCnt++;//12
		break;
		case 0x3B:	
//  {0, 0,  1000, 0 },
//SafetyIsoIslandandSoOnTable[4]
			g_SpiTxBuf[9] = FactoryMode;//1
			g_SpiTxBuf[10] = 0;//2
			
			g_SpiTxBuf[11] = 0;//3
			g_SpiTxBuf[12] = 0;//4

			g_SpiTxBuf[13] = 0;//5
			g_SpiTxBuf[14] =  0;//0;//6

			g_SpiTxBuf[15] = 0;//7
			g_SpiTxBuf[16] = 0;//0;//8

			g_SpiTxBuf[17] = 0;//9
			g_SpiTxBuf[18] = 0;//10
			
			g_SpiTxBuf[19] = 0;//11
			g_SpiTxBuf[20] = 0;//b16_SpiSendCnt++;//12
		break;
		case 0x3C:	
//{1000,0,1000,0,1000,0,1000,0,1000,0,1000,0,1000,0,1000,0,1000,0,
// 1000,0,1000,0,1000,0,1000,0,1000,0,1000,0,1000,0};
//CalibrationTable[32]
			g_SpiTxBuf[9] = CalibrationTable[0];//1
			g_SpiTxBuf[10] = CalibrationTable[1];//2
			
			g_SpiTxBuf[11] = CalibrationTable[2];//3
			g_SpiTxBuf[12] = CalibrationTable[3];//4

			g_SpiTxBuf[13] = CalibrationTable[4];//5
			g_SpiTxBuf[14] = CalibrationTable[5];//0;//6

			g_SpiTxBuf[15] = CalibrationTable[6];//7
			g_SpiTxBuf[16] = CalibrationTable[7];//0;//8

			g_SpiTxBuf[17] = CalibrationTable[8];//9
			g_SpiTxBuf[18] = CalibrationTable[9];//10
			
			g_SpiTxBuf[19] = CalibrationTable[10];//11
			g_SpiTxBuf[20] = CalibrationTable[11];//b16_SpiSendCnt++;//12
			g_SpiTxBuf[21] = CalibrationTable[12];//11
			g_SpiTxBuf[22] = CalibrationTable[13];//b16_SpiSendCnt++;//12
			g_SpiTxBuf[23] = CalibrationTable[14];//11
			g_SpiTxBuf[24] = CalibrationTable[15];//b16_SpiSendCnt++;//12
			break;
		case 0x3D:	
//{1000,0,1000,0,1000,0,1000,0,1000,0,1000,0,1000,0,1000,0,1000,0,
// 1000,0,1000,0,1000,0,1000,0,1000,0,1000,0,1000,0};
//CalibrationTable[32]
			g_SpiTxBuf[9] = CalibrationTable[16];//1
			g_SpiTxBuf[10] = CalibrationTable[17];//2
			
			g_SpiTxBuf[11] = CalibrationTable[18];//3
			g_SpiTxBuf[12] = CalibrationTable[19];//4

			g_SpiTxBuf[13] = CalibrationTable[20];//5
			g_SpiTxBuf[14] = CalibrationTable[21];//0;//6

			g_SpiTxBuf[15] = CalibrationTable[22];//7
			g_SpiTxBuf[16] = CalibrationTable[23];//0;//8

			g_SpiTxBuf[17] = CalibrationTable[24];//9
			g_SpiTxBuf[18] = CalibrationTable[25];//10
			
			g_SpiTxBuf[19] = CalibrationTable[26];//11
			g_SpiTxBuf[20] = CalibrationTable[27];//b16_SpiSendCnt++;//12
			g_SpiTxBuf[21] = CalibrationTable[28];//11
			g_SpiTxBuf[22] = CalibrationTable[29];//b16_SpiSendCnt++;//12
			g_SpiTxBuf[23] = CalibrationTable[30];//11
			g_SpiTxBuf[24] = CalibrationTable[31];//b16_SpiSendCnt++;//12

		break;
		default:
		break;
	}

	if(1 == s_StateCheck.bit.SafetySTART_Updateflag)
	{
		b_SpiSendCnt = 0x30;
	}
	else if(1 == s_StateCheck.bit.GridProtectV_Updateflag)
	{
		b_SpiSendCnt = 0x31;
	}
	else if(1 == s_StateCheck.bit.GridProtectFreqValue_Updateflag)
	{
		b_SpiSendCnt = 0x32;
	}
	else if(1 == s_StateCheck.bit.GridProtectDCI_Updateflag)
	{
		b_SpiSendCnt = 0x33;
	}
	else if(1 == s_StateCheck.bit.RemotPowerControlValue_Updateflag)
	{
		b_SpiSendCnt = 0x34;
	}
	else if(1 == s_StateCheck.bit.FreqPowerControlValue_Updateflag)
	{
		b_SpiSendCnt = 0x35;
	}
	else if(1 == s_StateCheck.bit.ReactiveValue_Updateflag1)
	{
		b_SpiSendCnt = 0x36;
	}
	else if(1 == s_StateCheck.bit.ReactiveValue_Updateflag2)
	{
		b_SpiSendCnt = 0x37;
	}
	else if(1 == s_StateCheck.bit.LVRTValue_Updateflag1)
	{
		b_SpiSendCnt = 0x38;
	}
	else if(1 == s_StateCheck.bit.LVRTValue_Updateflag2)
	{
		b_SpiSendCnt = 0x39;
	}
	else if(1 == s_StateCheck.bit.SafetyIsoIslandandSoOn_Updateflag)
	{
		b_SpiSendCnt = 0x3A;
	}
	else if(1 == s_StateCheck.bit.FactoryMode_Updateflag)
	{
		b_SpiSendCnt = 0x3B;
	}
	else if(1 == s_StateCheck.bit.CalibrationPara_Updateflag1)
	{
		b_SpiSendCnt = 0x3C;
	}
	else if(1 == s_StateCheck.bit.CalibrationPara_Updateflag2)
	{
		b_SpiSendCnt = 0x3D;
	}

	else
	{
		if(b_SpiSendCnt > 2)
		{
			b_SpiSendCnt = 0;
		}
		else
		{
			b_SpiSendCnt++;
		}
	}				
}
/************************************************************************
Fuction name:SpiChecksum
*************************************************************************/
//data0 - data30
Uint16 SpiChecksum(Uint16* pData,Uint8 len)
{
	Uint8 i;
	Uint16 b_SumTemp = 0;

	for(i=0; i<len; i++)
	{
		b_SumTemp += *pData;
		pData++;
	}
	return b_SumTemp;	
}
/*************************************************************************
Fuction name:	SpiSlaveUpdateData
**************************************************************************/
void SpiSlaveUpdateData(void)
{
	Uint16 u16Temp;
	Uint16 u16Date;
	static Uint16 u16Cout = 0;
	//checksum
	u16Temp = SpiChecksum(g_SpiRxBuf,31);
	if((u16Temp == g_SpiRxBuf[31]) && (g_SpiRxBuf[0] == (((Uint16)'M'<<8)|0xAA)))
	{
		g_SPICommCnt = 0;
		s_SysCurrentState_Master = (enum SysState)((g_SpiRxBuf[1]>>8) & 0x00FF);
		
		s_StateCheck.Word.byte3 = (Uint8)(g_SpiRxBuf[2]&0x00FF);


		s_SysFaultMessage_Master.Word.byte0 = (Uint8)((g_SpiRxBuf[3]&0xFF00)>>8);
		s_SysFaultMessage_Master.Word.byte1 = (Uint8)(g_SpiRxBuf[3]&0x00FF);
		s_SysFaultMessage_Master.Word.byte2 = (Uint8)((g_SpiRxBuf[4]&0xFF00)>>8);
		s_SysFaultMessage_Master.Word.byte3 = (Uint8)(g_SpiRxBuf[4]&0x00FF);
		s_SysFaultMessage_Master.Word.byte4 = (Uint8)((g_SpiRxBuf[5]&0xFF00)>>8);
		s_SysFaultMessage_Master.Word.byte5 = (Uint8)(g_SpiRxBuf[5]&0x00FF);
		s_SysFaultMessage_Master.Word.byte6 = (Uint8)((g_SpiRxBuf[6]&0xFF00)>>8);
		s_SysFaultMessage_Master.Word.byte7 = (Uint8)(g_SpiRxBuf[6]&0x00FF);
		s_SysFaultMessage_Master.Word.unrecover0 = (Uint8)((g_SpiRxBuf[7]&0xFF00)>>8);	
		s_SysFaultMessage_Master.Word.unrecover1 = (Uint8)(g_SpiRxBuf[7]&0x00FF);

		M_CalcResultRms.i16VGRID_R = g_SpiRxBuf[19];
		M_CalcResultRms.i16VGRID_S = g_SpiRxBuf[20];
		M_CalcResultRms.i16VGRID_T = g_SpiRxBuf[21];
		M_CalcResultRms.i16GFCI= g_SpiRxBuf[23];
		M_CalcResultRms.i16DCI_R= g_SpiRxBuf[24];
		M_CalcResultRms.i16DCI_S = g_SpiRxBuf[25];
		M_CalcResultRms.i16DCI_T = g_SpiRxBuf[26];


		s_StateCheck.Word.byte6 = (Uint8)((g_SpiRxBuf[22]&0xFF00)>>8);
		s_StateCheck.Word.byte4 = ((s_StateCheck.Word.byte4)&(~s_StateCheck.Word.byte6));
		s_StateCheck.Word.byte7 = (Uint8)(g_SpiRxBuf[22]&0x00FF);
		s_StateCheck.Word.byte5 = ((s_StateCheck.Word.byte5)&(~s_StateCheck.Word.byte7));



		u16Temp = g_SpiRxBuf[8];
		switch(u16Temp)
		{
			case 0x00:
			    MASTER_MAIN_VERSION = GET_LBYTE_OF_WORD(g_SpiRxBuf[9]);
			    MASTER_MAIN_VERSION0 = GET_LBYTE_OF_WORD(g_SpiRxBuf[10]);
				MASTER_SECONDARY_VERSION1 = GET_LBYTE_OF_WORD(g_SpiRxBuf[11]);
				MASTER_SECONDARY_VERSION2 = GET_LBYTE_OF_WORD(g_SpiRxBuf[12]);
				ProcessVariable.i16ReConnetionTime = g_SpiRxBuf[13];
				MainDSPWarningByte = g_SpiRxBuf[14];
				PowerLevelType = g_SpiRxBuf[15];

				if(3 ==  PowerLevelType)//10k
				{
					ProcessVariable.i16TAmbOTP = 78;
					ProcessVariable.i16TInvOTP = 95;
					ProcessVariable.i16TNTCOTP = 86;
				}
				else if(5 ==  PowerLevelType)//15k
				{
					ProcessVariable.i16TAmbOTP = 78;
					ProcessVariable.i16TInvOTP = 95;
					ProcessVariable.i16TNTCOTP = 86;
				}
				else if(6 ==  PowerLevelType)//17k
				{
					ProcessVariable.i16TAmbOTP = 78;
					ProcessVariable.i16TInvOTP = 95;
					ProcessVariable.i16TNTCOTP = 80;
				}
				else if(7 ==  PowerLevelType)//20k
				{
					ProcessVariable.i16TAmbOTP = 81;
					ProcessVariable.i16TInvOTP = 95;
					ProcessVariable.i16TNTCOTP = 80;
				}
				else
				{
					ProcessVariable.i16TAmbOTP = 78;
					ProcessVariable.i16TInvOTP = 95;
					ProcessVariable.i16TNTCOTP = 80;
				}			
			break;
			case 0x01:
				M_CalcResultRms.i16Input1Watt = g_SpiRxBuf[9];		//PV1 Power hight
				M_CalcResultRms.i16Input2Watt = g_SpiRxBuf[10];		//PV1 Power hight
				M_CalcResultRms.i16OutputWatt = g_SpiRxBuf[11];		//Out Power hight
				M_CalcResultRms.i16OutputEnergy = g_SpiRxBuf[12];
				M_CalcResultRms.i16VPV1 = g_SpiRxBuf[13];			//PV1 VoltageAvg
				M_CalcResultRms.i16IPV1 = g_SpiRxBuf[14];			//PV1 CurrentAvg
				M_CalcResultRms.i16VPV2 = g_SpiRxBuf[15];			//PV2 VoltageAvg
				M_CalcResultRms.i16IPV2 = g_SpiRxBuf[16];			//PV2 CurrentAvg
				M_CalcResultRms.i16Vbus = g_SpiRxBuf[17];
				u16Date = g_SpiRxBuf[18];
				if(u16Date !=Todaydate)
				{
					M_CalcResultRms.i16OutputEnergy = 0;
				}

				M_CalcResultRms.i16EnergyTodayNew = EnergyTodayOld + M_CalcResultRms.i16OutputEnergy;
//				M_CalcResultRms.i16EnergyTodayNew = EnergyTodayOld;
			break;
			case 0x02:

				M_CalcResultRms.i16IGridR = g_SpiRxBuf[9];			//R-inverter CurrentRms
				M_CalcResultRms.i16IGridS = g_SpiRxBuf[10];			//S-inverter CurrentRms
				M_CalcResultRms.i16IGridT = g_SpiRxBuf[11];			//T-inverter CurrentRms
				M_CalcResultRms.i16GridFreq = g_SpiRxBuf[12];

			break;
			case 0x03:

			break;
			case 0x04:

			break;
			case 0x05:

			break;
			default:
			break;
		}
	}
	else
	{
		//Spi communication is fault
	}

	if(u16Cout > 4500)//90 seconds
	{
		u16Cout = 0;

		if(I2C_Write_InWord((ENERGY_OUTPUT_ADDR + 2 * (Todaydate-1)),&(M_CalcResultRms.i16EnergyTodayNew),1))
		{
			s_SysFaultMessage.bit.unrecoverEEPROM_W = 1;
		}
	}
	else
	{
		u16Cout++;
	}
}
//---------------------------------------------------------------------------
// Example: InitSpiGpio:
//---------------------------------------------------------------------------
// This function initializes GPIO pins to function as SPI pins
//
// Each GPIO pin can be configured as a GPIO pin or up to 3 different
// peripheral functional pins. By default all pins come up as GPIO
// inputs after reset.
//
// Caution:
// For each SPI peripheral
// Only one GPIO pin should be enabled for SPISOMO operation.
// Only one GPIO pin should be enabled for SPISOMI operation.
// Only one GPIO pin should be enabled for SPICLK  operation.
// Only one GPIO pin should be enabled for SPISTE  operation.
// Comment out other unwanted lines.

void InitSpiGpio()
{

   InitSpiaGpio();
 #if DSP28_SPIB
   InitSpibGpio();
 #endif // endif DSP28_SPIB
}

void InitSpiaGpio()
{

   EALLOW;

/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;   // Enable pull-up on GPIO16 (SPISIMOA)
//  GpioCtrlRegs.GPAPUD.bit.GPIO5 = 0;    // Enable pull-up on GPIO5 (SPISIMOA)
    GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;   // Enable pull-up on GPIO17 (SPISOMIA)
//  GpioCtrlRegs.GPAPUD.bit.GPIO3 = 0;    // Enable pull-up on GPIO3 (SPISOMIA)
//    GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;   // Enable pull-up on GPIO18 (SPICLKA)
    GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;   // Enable pull-up on GPIO19 (SPISTEA)

/* Set qualification for selected pins to asynch only */
// This will select asynch (no qualification) for the selected pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAQSEL2.bit.GPIO16 = 3; // Asynch input GPIO16 (SPISIMOA)
//  GpioCtrlRegs.GPAQSEL1.bit.GPIO5 = 3;  // Asynch input GPIO5 (SPISIMOA)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 3; // Asynch input GPIO17 (SPISOMIA)
//  GpioCtrlRegs.GPAQSEL1.bit.GPIO3 = 3;  // Asynch input GPIO3 (SPISOMIA)
//    GpioCtrlRegs.GPAQSEL2.bit.GPIO18 = 3; // Asynch input GPIO18 (SPICLKA)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3; // Asynch input GPIO19 (SPISTEA)

/* Configure SPI-A pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be SPI functional pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 1; // Configure GPIO16 as SPISIMOA
//  GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 2;  // Configure GPIO5 as SPISIMOA
    GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 1; // Configure GPIO17 as SPISOMIA
//  GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 2;  // Configure GPIO3 as SPISOMIA
 //   GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 1; // Configure GPIO18 as SPICLKA
    GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 1; // Configure GPIO19 as SPISTEA

    EDIS;
}

#if DSP28_SPIB
void InitSpibGpio()
{
   EALLOW;

/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAPUD.bit.GPIO12 = 0;     // Enable pull-up on GPIO12 (SPISIMOB)
//  GpioCtrlRegs.GPAPUD.bit.GPIO24 = 0;     // Enable pull-up on GPIO24 (SPISIMOB)

    GpioCtrlRegs.GPAPUD.bit.GPIO13 = 0;     // Enable pull-up on GPIO13 (SPISOMIB)
//  GpioCtrlRegs.GPAPUD.bit.GPIO25 = 0;     // Enable pull-up on GPIO25 (SPISOMIB)

    GpioCtrlRegs.GPAPUD.bit.GPIO14 = 0;     // Enable pull-up on GPIO14 (SPICLKB)
//  GpioCtrlRegs.GPAPUD.bit.GPIO26 = 0;     // Enable pull-up on GPIO26 (SPICLKB)

    GpioCtrlRegs.GPAPUD.bit.GPIO15 = 0;     // Enable pull-up on GPIO15 (SPISTEB)
//  GpioCtrlRegs.GPAPUD.bit.GPIO27 = 0;     // Enable pull-up on GPIO27 (SPISTEB)


/* Set qualification for selected pins to asynch only */
// This will select asynch (no qualification) for the selected pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAQSEL1.bit.GPIO12 = 3;   // Asynch input GPIO12 (SPISIMOB)
//  GpioCtrlRegs.GPAQSEL2.bit.GPIO24 = 3;   // Asynch input GPIO24 (SPISIMOB)

    GpioCtrlRegs.GPAQSEL1.bit.GPIO13 = 3;   // Asynch input GPIO13 (SPISOMIB)
//  GpioCtrlRegs.GPAQSEL2.bit.GPIO25 = 3;   // Asynch input GPIO25 (SPISOMIB)

    GpioCtrlRegs.GPAQSEL1.bit.GPIO14 = 3;   // Asynch input GPIO14 (SPICLKB)
//  GpioCtrlRegs.GPAQSEL2.bit.GPIO26 = 3;   // Asynch input GPIO26 (SPICLKB)

    GpioCtrlRegs.GPAQSEL1.bit.GPIO15 = 3;   // Asynch input GPIO15 (SPISTEB)
//  GpioCtrlRegs.GPAQSEL2.bit.GPIO27 = 3;   // Asynch input GPIO27 (SPISTEB)

/* Configure SPI-B pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be SPI functional pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 3;    // Configure GPIO12 as SPISIMOB
//  GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 3;    // Configure GPIO24 as SPISIMOB

    GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 3;    // Configure GPIO13 as SPISOMIB
//  GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 3;    // Configure GPIO25 as SPISOMIB

    GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 3;    // Configure GPIO14 as SPICLKB
//  GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 3;    // Configure GPIO26 as SPICLKB

    GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 3;    // Configure GPIO15 as SPISTEB
//  GpioCtrlRegs.GPAMUX2.bit.GPIO27 = 3;    // Configure GPIO27 as SPISTEB

    EDIS;
}
#endif //endif DSP28_SPIB

//===========================================================================
// End of file.
//===========================================================================
