//###########################################################################
//
// FILE:	SL_slave_Sci.c
//
// TITLE:	DSP2803x SCI Initialization & Support Functions.
//
//###########################################################################
#include "SL_slave.h"
#include "DSP2803x_Flash2803x_API.h"

//#define SCI_TEST_MODE   //测试模式

volatile Uint8 SciTxBuff[80];
volatile Uint8 SciRxBuff[80];
volatile Uint8 SciTxPoint=0;
volatile Uint8 SciReceivePonit=0;
int16 RelayTemp11 = 0;
int16 RelayTemp12 = 0;
int16 RelayTemp13 = 0;
Uint16 EnergyTodayOld = 0;

extern Uint16 temp_zero = 0;
extern Uint16 Todaydate;
extern Uint16 MPPTMode;
//extern Uint8 Updataflg;
extern Uint16 ILrmsMPPTlimit;
Uint16 SafetyCountry;
Uint16 InputMode = 1;
Uint16 PowerLevelType;
Uint16 MainDSPWarningByte;
extern Uint16 MASTER_MAIN_VERSION;
extern Uint16 MASTER_MAIN_VERSION0;
extern Uint16 MASTER_SECONDARY_VERSION2;
extern Uint16 MASTER_SECONDARY_VERSION1;

extern Uint32 MyCallbackCounter;

Uint8 RealValue = 0;
Uint8 SafetyOrig = 0;
Uint8 shake_hand_finish = 0;
//Uint8 Resetflg = 0;
Uint8 g_Sys_Current_State = 0;
Uint8 s_EepromWriteorRead_Flag = 0; 
Uint16 Orig_epromflg = 0xffff;
Uint16 g_SCICommCnt = 0;
Uint16 g_SPICommCnt = 0;

const Uint16 SafetyDefaultSTARTTable[COUNTRYNUM][8] = 
{ 			//Tconnection PowerUpSpeed	Tback PowerbackUpSpeed	VGridBack>	VGridBack<	  FGridBack> FGridBack<
/*0Germany4105*/  	{60,	100,		60,		100,			2640,		1955,			5055,		4750},
/*1ItalyCEI0-21*/ 	{30,	100,		300,	100,			2640,		1955,			5050,		4990},
/*2Australia*/  	{60,	100,		60,		100,			2670,		2030,			5449,		4551},
/*3Spanish*/    	{180,	100,		30,		100,			2530,		1990,			5055,		4900},
/*4Czechia*/  		{30,	100,		60,		100,			2640,		1990,			5050,		4950},
/*5GreeceIslands*/  {180,	100,		180,	100,			2640,		1870,			5100,		4750},
/*6GreeceContinent*/{180,	100,		180,	100,			2640,		1870,			5050,		4950},
/*7Netherland*/		{30,	100,		30,		100,			2530,		1870,			5100,		4800}, 
/*8Belgium*/		{30,	100,		30,		100,			2530,		2000,			5050,		4750},
/*9England*/		{180,	100,		180,	100,			2610,		2150,			5150,		4750},
/*10China*/			{30,	100,		30,		100,			2530,		1870,			5019,		4951},
/*11France*/		{30,	100,		30,		100,			2600,		1900,			5020,		4750},
/*12Bulgarian*/		{30,	100,		30,		100,			2600,		1900,			5020,		4750},
/*13Germany-BDEW*/	{60,	100,		60,		100,			2730,		2165,			5055,		4750},
/*14Germany-0126*/	{60,	100,		60,		100,			2730,		2165,			5055,		4750},
/*15Italy-CEI0-16*/	{30,	100,		300,	100,			2645,		1955,			5020,		4990}};

const Uint16 SafetyDefaultVTable[COUNTRYNUM][10] = 
{ 
				   //EABLE,10MINV,EABLE,VHi1, Time, VLow1,  Time,   VHi2, Time2,	VLow2,	Time
/*0Germany*/   		{15,  2640,  10,  2800,	6,		1840,	10,		1750,	60 ,    2640},
/*1Italy0-21*/ 		{15,  2645,  30,  2800,	10,		1955,	10,		920,	100,    2645},
/*2Australia*/  	{15,  2700,  190, 2800,	6,		2000,	190,	1750,	6 ,    2700},
/*3Spanish*/    	{15,  2530,  140, 2645,	16,		1955,	1400,	1750,	160,    2530},
/*4Czechia*/  		{15,  2640,  10,  2800,	6,		1960,	100,	1750,	60 ,    2530},
/*5GreeceIslands*/  {15,  2640,  20,  2800,	6,		1840,	200,	1750,	60 ,    2530},
/*6GreeceContinent*/{15,  2640,  20,  2800,	6,		1840,	200,	1750,	60 ,    2530},
/*7Netherland*/		{15,  2530,  20,  2800,	6,		1840,	200,	1750,	60 ,    2530}, 
/*8Belgium*/		{15,  2530,  80,  2800,	6,		1960,	80,		1750,	60 ,    2530},
/*9England*/		{15,  2610,  92,  2730,	42,		2090,	2420,	1920,	42 ,    2530},
/*10China*/			{15,  2530,  190, 2820,	4,		1870,	190,	1100,	6,    	2530},
/*11France*/		{15,  2600,  10,  2800,	60,		1870,	100,	1750,	60 ,    2530},	
/*12Bulgarian*/		{15,  2600,  10,  2800,	60,		1870,	100,	1750,	60 ,    2530},
/*13Germany-BDEW*/	{15,  2730,  10,  3000,	20,		1840,	2400,	1035,	600,    2530},
/*14Germany-0126*/	{15,  2730,  10,  3000,	20,		1840,	2400,	1035,	600,    2530},
/*15Italy-CEI0-16*/	{15,  2645,  30,  2800,	10,		1940,	100,	920,	10,   2530}};	

const Uint16 SafetyDefaultFreTable[COUNTRYNUM][9] = 
{ 				//EABLE,  FHi1, Time10,	FLow1, Time10,  FHi2,  Time,   FLow2,  Time,
/*0Germany*/   		{15,   5150,   10,	5200,	60,		4750,	10,		4500,	60 },
/*1Italy0-21*/   	{15,   5150,   90,	5200,	200,	4750,	390,	4700,	200},
/*2Australia*/  	{15,   5450,   190,	5500,	10,	    4550,	190,	4500,	10},
/*3Spanish*/    	{15,   5050,   40,	5200,	60,		4800,	18000,	4500,	60},
/*4Czechia*/  		{15,   5050,   10,	5200,	60,		4950,	10,		4500,	60},
/*5GreeceIslands*/  {15,   5100,   20,	5200,	60,		4750,	20,		4500,	60},
/*6GreeceContinent*/{15,   5050,   20,	5200,	60,		4950,	20,		4700,	60},
/*7Netherland*/		{15,   5100,   20,	5200,	60,		4800,	20,		4700,	60}, 
/*8Belgium*/		{15,   5050,   10,	5200,	60,		4750,	10,		4500,	60},
/*9England*/		{15,   5150,   9000, 5200,	400,	4750,	2000,	4700,	400},
/*10China*/			{15,   5020,   12900,5050,	16,		4950,	61000,	4800,	16},
/*11France*/		{15,   5020,   10,	5200,	60,		4750,	10,		4500,	60},
/*12Bulgarian*/		{15,   5020,   10,	5200,	60,		4750,	10,		4500,	60},
/*13Germany-BDEW*/	{15,   5150,   10,	5200,	100,	4750,	10,		4500,	100},
/*14Germany-0126*/	{15,   5150,   10,	5200,	100,	4750,	10,		4500,	100},
/*15Italy-CEI0-16*/	{15,   5150,   90,	5200,	200,	4750,	390,	4700,	200}};

const Uint16 SafetyDefaultDCITable[COUNTRYNUM][6] = 
{ 					//EABLE,Limit1,Time,   Limit2,  Time
/*0Germany*/   		{3, 	150, 	80, 	500, 	16,	0},
/*1Italy0-21*/   	{3, 	150, 	80, 	500, 	16,	0},
/*2Australia*/  	{3, 	150, 	80, 	500, 	16,	0},
/*3Spanish*/    	{3, 	150, 	80, 	500, 	16,	0},
/*4Czechia*/  		{3, 	150, 	80, 	500, 	16,	0},
/*5GreeceIslands*/  {3, 	150, 	80, 	500, 	16,	0},
/*6GreeceContinent*/{3, 	150, 	80, 	500, 	16,	0},
/*7Netherland*/		{3, 	150, 	80, 	500, 	16,	0}, 
/*8Belgium*/		{3, 	150, 	80, 	500, 	16,	0},
/*9England*/		{3, 	150, 	80, 	500, 	16,	0},
/*10China*/			{3, 	150, 	80, 	500, 	16,	0},
/*11France*/		{3, 	150, 	80, 	500, 	16,	0},
/*12Bulgarian*/		{3, 	150, 	80, 	500, 	16,	0},
/*13Germany-BDEW*/	{3, 	150, 	80, 	500, 	16,	0},
/*14Germany-0126*/	{3, 	150, 	80, 	500, 	16,	0},
/*15Italy-CEI0-16*/	{3, 	150, 	80, 	500, 	16,	0}};

 
const Uint16 SafetyDefaultRemotPowerControlTable[COUNTRYNUM][3] = 
{ 				   //EABLE,Remote
/*0Germany*/   		{0,   100,	85},
/*1Italy0-21*/     	{0,   100,	85},
/*2Australia*/  	{0,   100,	85},
/*3Spanish*/    	{0,   100,	85},
/*4Czechia*/  		{0,   100,	85},
/*5GreeceIslands*/  {0,   100,	85},
/*6GreeceContinent*/{0,   100,	85},
/*7Netherland*/		{0,   100,	85}, 
/*8Belgium*/		{0,   100,	85},
/*9England*/		{0,   100,	85},
/*10China*/			{0,   100,	85},
/*11France*/		{0,   100,	85},	
/*12Bulgarian*/		{0,   100,	85},
/*13Germany-BDEW*/	{0,   100,	85},
/*14Germany-0126*/	{0,   100,	85},
/*15Italy-CEI0-16*/	{0,   100,	85}};	

const Uint16 SafetyDefaultFreqPowerControlTable[COUNTRYNUM][7] = 
{ 				   //EABLE, STATR,  K,   BACKUP,  BACKDOWN,  WTAITTIME, MODE,  SPEED
/*0Germany4105*/ 	{0,     5020,   50,  5150,     4750,      0,            	240},
/*1Italy0-21*/  	{0,     5030,   24,  5010,     4990,      300,          	20 },
/*2Australia*/  	{0,     5030,   24,  5010,     4990,      300,          	20 },
/*3Spanish*/    	{0,     5030,   24,  5010,     4990,      300,          	20 },
/*4Czechia*/  		{0,     5030,   24,  5010,     4990,      300,          	20 },
/*5GreeceIslands*/  {0,     5030,   24,  5010,     4990,      300,          	20 },
/*6GreeceContinent*/{0,     5030,   24,  5010,     4990,      300,          	20 },
/*7Netherland*/		{0,     5030,   24,  5010,     4990,      300,          	20 }, 
/*8Belgium*/		{0,     5030,   24,  5010,     4990,      300,          	20 },
/*9England*/		{0,     5030,   24,  5010,     4990,      300,          	20 },
/*10China*/			{0,     5030,   24,  5010,     4990,      300,          	20 },
/*11France*/		{0,     5030,   24,  5010,     4990,      300,          	20 },	
/*12Bulgarian*/		{0,     5030,   24,  5010,     4990,      300,          	20 },
/*13Germany-BDEW*/	{0,     5020,   50,  5015,     4750,      0,            	10 },
/*14Germany-0126*/	{0,     5020,   50,  5015,     4750,      0,            	10 },
/*15Italy-CEI0-16*/	{0,     5030,   24,  5010,     4990,      300,          	20 }};	

const Uint16 SafetyDefaultReactiveTable[COUNTRYNUM][21] = //cosphi =100
{ 				   //Qoe	cosphi	Q	cosphi1		P1	cosphi2		P2		cosphi3	 	P3		cosphi4	P4	 	LockinV	 LockoutV	U1s		U2s		U1i		U2i		LockinP	LockoutP	Qmax	Time
/*0Germany410*/ 	{0,		100,	0,		100,	50,		100,	50,		100,		50,		95,		100,	 81,	  80,		0,		0,		0,		0,		0,		0,			 0,		0  },
/*1Italy0-21*/  	{0, 	100,	0,		100,	50,		100,	50,		100,		50,		95,		100,	 105,	  100,		108,	110,	92,		90,		20,		5,			 4845,	2  },
/*2Australia*/  	{0, 	100,	0,		100,	50,		100,	50,		100,		50,		95,		100,	 105,	  100,		108,	110,	92,		90,		20,		5,			 4845,	2  },
/*3Spanish*/    	{0, 	100,	0,		100,	50,		100,	50,		100,		50,		95,		100,	 105,	  100,		108,	110,	92,		90,		20,		5,			 4845,	2  },
/*4Czechia*/  		{0, 	100,	0,		100,	50,		100,	50,		100,		50,		95,		100,	 105,	  100,		108,	110,	92,		90,		20,		5,			 4845,	2  },
/*5GreeceIslands*/  {0, 	100,	0,		100,	50,		100,	50,		100,		50,		95,		100,	 105,	  100,		108,	110,	92,		90,		20,		5,			 4845,	2  },
/*6GreeceContinent*/{0, 	100,	0,		100,	50,		100,	50,		100,		50,		95,		100,	 105,	  100,		108,	110,	92,		90,		20,		5,			 4845,	2  },
/*7Netherland*/		{0, 	100,	0,		100,	50,		100,	50,		100,		50,		95,		100,	 105,	  100,		108,	110,	92,		90,		20,		5,			 4845,	2  },
/*8Belgium*/		{0, 	100,	0,		100,	50,		100,	50,		100,		50,		95,		100,	 105,	  100,		108,	110,	92,		90,		20,		5,			 4845,	2  },
/*9England*/		{0, 	100,	0,		100,	50,		100,	50,		100,		50,		95,		100,	 105,	  100,		108,	110,	92,		90,		20,		5,			 4845,	2  },
/*10China*/			{0, 	100,	0,		100,	50,		100,	50,		100,		50,		95,		100,	 105,	  100,		108,	110,	92,		90,		20,		5,			 4845,	2  },
/*11France*/		{0, 	100,	0,		100,	50,		100,	50,		100,		50,		95,		100,	 105,	  100,		108,	110,	92,		90,		20,		5,			 4845,	2  },	
/*12Bulgarian*/		{0, 	100,	0,		100,	50,		100,	50,		100,		50,		95,		100,	 105,	  100,		108,	110,	92,		90,		20,		5,			 4845,	2  },
/*13Germany-BDEW*/	{0, 	100,	0,		100,	50,		100,	50,		100,		50,		95,		100,	 81,	  80,		108,	110,	92,		90,		20,		5,			 4845,	2  },
/*14Germany-0126*/	{0, 	100,	0,		100,	50,		100,	50,		100,		50,		95,		100,	 81,	  80,		108,	110,	92,		90,		20,		5,			 4845,	2  },
/*15Italy-CEI0-16*/	{0, 	100,	0,		100,	50,		100,	50,		100,		50,		95,		100,	 81,	  80,		108,	110,	92,		90,		20,		5,			 4845,	2  }};


const Uint16 SafetyDefaultLVRTTable[COUNTRYNUM][13] = 
{ 				   //ENABLE	VLVRT	VPoint1	Tpoint1	VPoint2	Tpoint2	VPoint3	Tpoint3	VPoint4	Tpoint4	K	Tback	PLVRTBack
/*0Germany4105*/   	{0,		85,		0,		200,	40,		0,		40,		300,	85,		0,		2,	0,		200	},
/*1Italy0-21*/     	{0, 	85,		0,		200	,	40,		0,		40,		300	,	85,		0,		2,	0,		200	},
/*2Australia*/  	{0, 	85,		0,		200	,	40,		0,		40,		300	,	85,		0,		2,	0,		200	},
/*3Spanish*/    	{0, 	85,		0,		200	,	40,		0,		40,		300	,	85,		0,		2,	0,		200	},
/*4Czechia*/  		{0, 	85,		0,		200	,	40,		0,		40,		300	,	85,		0,		2,	0,		200	},
/*5GreeceIslands*/  {0, 	85,		0,		200	,	40,		0,		40,		300	,	85,		0,		2,	0,		200	},
/*6GreeceContinent*/{0, 	85,		0,		200	,	40,		0,		40,		300	,	85,		0,		2,	0,		200	},
/*7Netherland*/		{0, 	85,		0,		200	,	40,		0,		40,		300	,	85,		0,		2,	0,		200	}, 
/*8Belgium*/		{0, 	85,		0,		200	,	40,		0,		40,		300	,	85,		0,		2,	0,		200	},
/*9England*/		{0, 	85,		0,		200	,	40,		0,		40,		300	,	85,		0,		2,	0,		200	},
/*10China*/			{0, 	85,		0,		200	,	40,		0,		40,		300	,	85,		0,		2,	0,		200	},
/*11France*/		{0, 	85,		0,		200	,	40,		0,		40,		300	,	85,		0,		2,	0,		200	},
/*12Bulgarian*/		{0, 	85,		0,		200	,	40,		0,		40,		300	,	85,		0,		2,	0,		200	},
/*13Germany-BDEW*/	{0, 	90, 	0, 		300,	30,		0,		30,		1000,	90,		0,		2,	1,		0	},
/*14Germany-0126*/	{0, 	90, 	0, 		300,	30,		0,		30,		1000,	90,		0,		2,	1,		0	},
/*15Italy-CEI0-16*/	{0, 	85, 	0, 		200, 	40, 	0, 		40, 	300, 	85, 	0, 		2, 	0, 		200	}};
	


const Uint16 SafetyDefaultIsoIslandandSoOnTable[COUNTRYNUM][4] = 
{ 		//		   ISLAND GFCI  ISOENABLE  ISO     
/*0Germany4105*/  	{1,    1,   	1,    1000 },
/*1Italy0-21*/   	{1,    1,   	1,    1000 },
/*2Australia*/  	{1,    1,   	1,    300 },
/*3Spanish*/    	{1,    1,   	1,    1000 },
/*4Czechia*/  		{1,    1,   	1,    1000 },
/*5GreeceIslands*/  {1,    1,   	1,    1000 },
/*6GreeceContinent*/{1,    1,   	1,    1000 },
/*7Netherland*/		{1,    1,   	1,    1000 }, 
/*8Belgium*/		{1,    1,   	1,    1000 },
/*9England*/		{1,    1,   	1,    1000 },
/*10China*/			{1,    1,   	1,    300 },
/*11France*/		{1,    1,   	1,    1000 },
/*12Bulgarian*/		{1,    1,   	1,    1000 },
/*13Germany-BDEW*/	{1,    1,   	1,    1000 },
/*14Germany-0126*/	{1,    1,   	1,    1000 },
/*15Italy-CEI0-16*/	{1,    1,   	1,    1000 }};

const Uint16 FactoryModeDefaultValue = 5120;//0x1400

const Uint16 CalibrationDefaultTable[32] = {1000,0,1000,0,1000,0,1000,0,1000,0,1000,0,1000,0,1000,0,1000,0,
									 1000,0,1000,0,1000,0,1000,0,1000,0,1000,0,1000,0};


Uint16 SafetySTARTTable[8] = {0};
Uint16 SafetyVTable[10] = {0};
Uint16 SafetyFreTable[9] = {0};
Uint16 SafetyDCITable[6] = {0};
Uint16 SafetyRemotPowerControlTable[3] = {0};
Uint16 SafetyFreqPowerControlTable[7] = {0};
Uint16 SafetyReactiveTable[21] = {0};
Uint16 SafetyLVRTTable[13] = {0};	
Uint16 SafetyIsoIslandandSoOnTable[4] = {0};
Uint16 FactoryMode = 0;
Uint16 CalibrationTable[32] = {0};
					
/**********************************************************************
* FUNCION :  TSK_F_SCIa
* PURPOSE :  
**********************************************************************/
void TSK_F_SCIa(void)
{
	//push up control state line
	while(1)
	{
		SEM_pend(&SEM_SCIa, SYS_FOREVER);
		Sci_ReceiveData();	
	}
}
/**********************************************************************
* FUNCION :  InitSci
* PURPOSE :  
**********************************************************************/
void InitSci(void)
{
	// Initialize SCI-A:
/*	SciaRegs.SCICCR.all =0x0007;   // 1 stop bit,  No loopback
                                   // No parity,8 char bits,
                                   // async mode, idle-line protocol
	SciaRegs.SCICTL1.all =0x0001;  // disable TX, enable RX, internal SCICLK,  0x0003 enable TX
                                   // Disable RX ERR, SLEEP, TXWAKE
	//
	SciaRegs.SCICTL1.bit.TXENA = 1;
	SciaRegs.SCICTL1.bit.RXENA = 1;	
	SciaRegs.SCICTL2.bit.TXINTENA = 1;
	SciaRegs.SCICTL2.bit.RXBKINTENA = 1;
	//
	SciaRegs.SCIHBAUD = 0x00;
    SciaRegs.SCILBAUD = 0xC2;

	SciaRegs.SCIPRI.bit.FREE = 1; 
	SciaRegs.SCICTL1.bit.SWRESET = 1;

	PieCtrlRegs.PIEIER9.bit.INTx1 = 1;//RX_ISR
	PieCtrlRegs.PIEIER9.bit.INTx2 = 1;//TX_ISR
	IER |= 0x0100;*/
	SciaRegs.SCICCR.all =0x0007;   // 1 stop bit,  No loopback
                                   // No parity,8 char bits,
                                   // async mode, idle-line protocol
	SciaRegs.SCICTL1.all =0x0001;  // disable TX, enable RX, internal SCICLK,  0x0003 enable TX
                                   // Disable RX ERR, SLEEP, TXWAKE
	SciaRegs.SCICTL2.bit.TXINTENA =0;
	SciaRegs.SCICTL2.bit.RXBKINTENA =1;

	SciaRegs.SCIHBAUD = 0x00;
    SciaRegs.SCILBAUD = 0xC2;

	SciaRegs.SCIPRI.bit.FREE = 1; 
	SciaRegs.SCICTL1.bit.SWRESET = 1;


	PieCtrlRegs.PIEIER9.bit.INTx1 = 1;	//RX_ISR
	PieCtrlRegs.PIEIER9.bit.INTx2 = 1;	//TX_ISR
	IER |= 0x0100;		
}
/**********************************************************************
* FUNCION :  Sci_ReceiveData
* PURPOSE :  
**********************************************************************/
void Sci_ReceiveData(void)
{
	g_SCICommCnt = 0;	
	switch(SciRxBuff[3])  
    {
		case 0x10:
			ShakeHandsCheck(); //上电握手，EEPROM无数据，写入日期，清发电量；
									//如果第一次上电，或安规国家有变化，写入默认值
			Sci_SendData();
		break;

    	case 0x20:
			SetSciValueForDataread();//发送实时数据信息
			Sci_SendData();	
		break;

	    case 0x30:	       					 //恢复出厂设置，擦除EEPROM，
            GoBacktoOrig();
			Sci_SendData();
		break;

		case 0x40:   						//恢复当前国安规参数为默认值
			ResetToDefaultsafetyvalue();
			Sci_SendData();
		break;

		case 0x50:
		    ClearTodayOutputPower();	    //清除当天发电量
			Sci_SendData();
		break;

		case 0x70:
			SetSofteVersionToLCD();  		 //通讯板发送版本信息，机?
			Sci_SendData();
		break;

		case 0x90:
		     if((SciRxBuff[4]=='F')&&(SciRxBuff[5]=='D')&&(SciRxBuff[6]=='S')
		        &&(SciRxBuff[7]=='P')&&(SciRxBuff[8]=='S'))
		        SoftwareOnlineUpgrade();
				Sci_SendData();
        break;
		case 0x80:
			GetONOFFCommand();  		 //通讯板发送版本信息，机型。
			Sci_SendData();
		break;
		case 0x81:
			GetDeratingCommand();  		 //通讯板发送版本信息，机型。
			Sci_SendData();
		break;
		case 0x82:
			GetPowerFactorCommand();  		 //通讯板发送版本信息，机型。
			Sci_SendData();
		break;
		case 0x83:
			GetReactivePowerCommand();  		 //通讯板发送版本信息，机型。
			Sci_SendData();
		break;

		case 0x60:   										//读安规参数
			switch(SciRxBuff[4])
			{
				case 0x30:     
					SetSTARTValueToLCD();    				//向通讯板发送开始数据信息					
					Sci_SendData();
				break;

				case 0x31:     
					SetGridProtectVValueToLCD();      		//向通栋宸⑺偷缤；な菪畔?				
					Sci_SendData();
				break;

				case 0x32:     
					SetGridProtectFreqValueToLCD();      	//向通讯板发送电网保护数据信息					
					Sci_SendData();
				break;

				case 0x33:     
					SetGridProtectDCIValueToLCD();      	//向通讯板发送电网保护数据信息					
					Sci_SendData();
				break;

				case 0x34:     
					SetRemotPowerControlValueToLCD();    		//向通讯板发送远程有功功率调度数据信息					
					Sci_SendData();
				break;
					
				case 0x35:     
					SetFreqPowerControlValueToLCD();    		//向通讯板发送随频率降载有功功率调度数据信息					
					Sci_SendData();
				break;

				case 0x36:     
					SetReactiveValueToLCD();       				//向通讯板发送无功控制电流数据信息					
					Sci_SendData();
				break;

				case 0x37:     
					SetLVRTValueToLCD();    					//向通讯板发送低电压穿越数据信息					
					Sci_SendData();
				break;

				case 0x38:     
					SetIsoIslandandSoOnValueToLCD();    	//向通讯板发送孤岛，绝缘阻抗，漏电流、开关机等					
					Sci_SendData();
				break;

				case 0x39:     
					SetFactoryModeValueToLCD();    					//向通讯板发送低电压穿越数据信息					
					Sci_SendData();
				break;

				case 0x3A:     
					SetCalibrationValueToLCD();    	//向通讯板发送孤岛，绝缘阻抗，漏电流、开关机等					
					Sci_SendData();
				break;


				default:
				break;
			}
		break;

		case 0x06: 
			switch(SciRxBuff[4])
			{
				case 0x30:     
					GetSTARTValueFromLCD();   				//从通讯板获得并保存低电压穿越数据信息					
					Sci_SendData();
				break;

				case 0x31:     
					GetGridProtectVValueFromLCD();     		//从通讯板获得并保存电网保护数据信息					
					Sci_SendData();
				break;

				case 0x32:     
					GetGridProtectFreqValueFromLCD();     	//从通讯板获得并保存电网保护数据信息					
					Sci_SendData();
				break;

				case 0x33:     
					GetGridProtectDCIValueFromLCD();     	//从通讯板获得并保存电网保护数据信息					
					Sci_SendData();
				break;

				case 0x34:     
					GetRemotPowerControlValueFromLCD();  	 //从通讯板获得并保存远程有功功率调度数据信息					
					Sci_SendData();
				break;
					
				case 0x35:     
					GetFreqPowerControlValueFromLCD();  	 //从通讯板获得并保存随频率降载有功功率调度数据信息					
					Sci_SendData();
				break;

				case 0x36:     
					GetReactiveValueFromLCD();      		//从通讯板获得并保存无功控制电流数据信息					
					Sci_SendData();
				break;

				case 0x37:     
					GetLVRTValueFromLCD();   				//从通讯板获得并保存低电压穿越数据信息					
					Sci_SendData();
				break;

				case 0x38:     
					GetIsoIslandandSoOnValueFromLCD();   	//从通讯板获得并保存孤岛，绝缘阻抗，漏电流、开?				
					Sci_SendData();
				break;

				case 0x39:     
					GetFactoryModeValueFromLCD();   				//从通讯板获得并保存低电压穿越数据信息					
					Sci_SendData();
				break;

				case 0x3A:     
					GetCalibrationValueFromLCD();   	//从通讯板获得并保存孤岛，绝缘阻抗，漏电流、开?				
					Sci_SendData();
				break;

				default:
				break;					
			}
		break;
		default:
		break;
    }
}
/**********************************************************************
* FUNCION :  Sci_SendData
* PURPOSE :  
**********************************************************************/
void Sci_SendData(void)
{
	SciaRegs.SCICTL1.bit.TXENA = 1;
	SciaRegs.SCICTL1.bit.RXENA = 0;	
	SciaRegs.SCICTL2.bit.TXINTENA = 1;
	SciaRegs.SCICTL2.bit.RXBKINTENA = 0;
	SciTxPoint = 0;
	SciaRegs.SCITXBUF = SciTxBuff[SciTxPoint];
	SciTxPoint++;
}
//---------------------------------------------------------------------------
// Example: InitSciGpio:
//---------------------------------------------------------------------------
// This function initializes GPIO pins to function as SCI pins
//
// Each GPIO pin can be configured as a GPIO pin or up to 3 different
// peripheral functional pins. By default all pins come up as GPIO
// inputs after reset.
//
// Caution:
// Only one GPIO pin should be enabled for SCITXDA/B operation.
// Only one GPIO pin shoudl be enabled for SCIRXDA/B operation.
// Comment out other unwanted lines.
/**********************************************************************
* FUNCION :  Sci_SendData
* PURPOSE :  
**********************************************************************/
void InitSciGpio()
{
	InitSciaGpio();
}
/**********************************************************************
* FUNCION :  InitSciaGpio
* PURPOSE :  
**********************************************************************/
void InitSciaGpio()
{
    EALLOW;

	GpioCtrlRegs.GPAPUD.bit.GPIO28 = 0;    // Enable pull-up for GPIO28 (SCIRXDA)
	GpioCtrlRegs.GPAPUD.bit.GPIO29 = 0;	   // Enable pull-up for GPIO29 (SCITXDA)
	GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 3;  // Asynch input GPIO28 (SCIRXDA)
	GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 1;   // Configure GPIO28 for SCIRXDA operation
	GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 1;   // Configure GPIO29 for SCITXDA operation

    EDIS;
}
/**********************************************************************
* FUNCION :  SetSciValueForDataread
* PURPOSE :  
**********************************************************************/
void SetSciValueForDataread(void)
{
	Uint16 point = 0;
	Uint8 statetemp2;
	Uint16 checksumtemp = 0;
	Uint16 i;
//*******state------bFirstTime--------mean
//        0            0               WAIT
//        0            1               CHECKING
//        0            2               RECONNECT   //THER IS  NO < RECONNECT> IN HF
//		  1            X               NORMAL
//		  2            X               FAULT
//        3            X               PERMAN
//_____________________________________________
	if(s_SysCurrentState_Master == WaitState)
	{
	 	statetemp2 = 0;
	}
	else if(s_SysCurrentState_Master == CheckState)
	{
		statetemp2 = 1;
	}
	else if(s_SysCurrentState_Master == NormalState)
	{
		statetemp2 = 2;
	}
	else if(s_SysCurrentState_Master == FaultState)
	{
		statetemp2 = 3;
	}
	else 
	{
		statetemp2 = 4;
	}

	if(Todaydate != SciRxBuff[2])                        // 01  03  XX  10   xx
	{
		Todaydate = SciRxBuff[2];
		temp_zero = 0;
		EnergyTodayOld = 0;

		if(I2C_Write_InWord((ENERGY_OUTPUT_ADDR + 2*(Todaydate-1)),&temp_zero,1))
		{
			s_SysFaultMessage.bit.unrecoverEEPROM_W = 1;
		}

		if( I2C_Write_InWord(ENERGY_DATE_ADDR,&Todaydate,1))
		{
			s_SysFaultMessage.bit.unrecoverEEPROM_W = 1;
		}
		else
		{
		}
	}

	SciTxBuff[point++] = 0x01; // 副CPU 地址
	SciTxBuff[point++] = SciTxBuffNum; //数据长度
	SciTxBuff[point++] = 0x20; //0 功能码
	

    SciTxBuff[point++] = 0;       //1 逆变器状态state
	SciTxBuff[point++] = statetemp2;       //2 开机状态（waiting、checking、reconnecting）
	SciTxBuff[point++] = (s_SysFaultMessage.Word.byte1|s_SysFaultMessage_Master.Word.byte1); //4
	SciTxBuff[point++] = (s_SysFaultMessage.Word.byte0|s_SysFaultMessage_Master.Word.byte0); //3
	SciTxBuff[point++] = (s_SysFaultMessage.Word.byte3|s_SysFaultMessage_Master.Word.byte3); //6
	SciTxBuff[point++] = (s_SysFaultMessage.Word.byte2|s_SysFaultMessage_Master.Word.byte2); //5
	SciTxBuff[point++] = (s_SysFaultMessage.Word.byte5|s_SysFaultMessage_Master.Word.byte5); //8
	SciTxBuff[point++] = (s_SysFaultMessage.Word.byte4|s_SysFaultMessage_Master.Word.byte4); //7
	SciTxBuff[point++] = (s_SysFaultMessage.Word.byte7|s_SysFaultMessage_Master.Word.byte7); //10
	SciTxBuff[point++] = (s_SysFaultMessage.Word.byte6|s_SysFaultMessage_Master.Word.byte6); //9
	SciTxBuff[point++] = (s_SysFaultMessage.Word.unrecover1|s_SysFaultMessage_Master.Word.unrecover1);//12
	SciTxBuff[point++] = (s_SysFaultMessage.Word.unrecover0|s_SysFaultMessage_Master.Word.unrecover0);//11
	
	SciTxBuff[point++] = GET_HBYTE_OF_WORD(M_CalcResultRms.i16VPV1);   //13第一路PV电压
	SciTxBuff[point++] = GET_LBYTE_OF_WORD(M_CalcResultRms.i16VPV1);   //14第一路PV电压
	SciTxBuff[point++] = GET_HBYTE_OF_WORD(M_CalcResultRms.i16IPV1);   //15第一路pv电流 
	SciTxBuff[point++] = GET_LBYTE_OF_WORD(M_CalcResultRms.i16IPV1);   //16第一路PV缌?
	SciTxBuff[point++] = GET_HBYTE_OF_WORD(M_CalcResultRms.i16VPV2);  //17第二路PV电压
	SciTxBuff[point++] = GET_LBYTE_OF_WORD(M_CalcResultRms.i16VPV2);  //18第二路PV电压
	SciTxBuff[point++] = GET_HBYTE_OF_WORD(M_CalcResultRms.i16IPV2);  //19第二路PV电流
	SciTxBuff[point++] = GET_LBYTE_OF_WORD(M_CalcResultRms.i16IPV2);  //20第二路PV电流
	SciTxBuff[point++] = GET_HBYTE_OF_WORD(M_CalcResultRms.i16Input1Watt);  //21第一路输入功率高字节
	SciTxBuff[point++] = GET_LBYTE_OF_WORD(M_CalcResultRms.i16Input1Watt);  //22第一路淙牍β实妥纸?
	SciTxBuff[point++] = GET_HBYTE_OF_WORD(M_CalcResultRms.i16Input2Watt);  //23第二路输入功率高字节
	SciTxBuff[point++] = GET_LBYTE_OF_WORD(M_CalcResultRms.i16Input2Watt);  //24第二路输入功实妥纸?

	SciTxBuff[point++] = GET_HBYTE_OF_WORD(M_CalcResultRms.i16OutputWatt);   //25输出功率高字节
	SciTxBuff[point++] = GET_LBYTE_OF_WORD(M_CalcResultRms.i16OutputWatt);   //26输出功率低字节

	SciTxBuff[point++] = GET_HBYTE_OF_WORD(M_CalcResultRms.i16OutputWatt);   //27输出功率高字节
	SciTxBuff[point++] = GET_LBYTE_OF_WORD(M_CalcResultRms.i16OutputWatt);   //28输出功率低字节

	SciTxBuff[point++] = GET_HBYTE_OF_WORD(M_CalcResultRms.i16GridFreq);   //29电网频率高字节
	SciTxBuff[point++] = GET_LBYTE_OF_WORD(M_CalcResultRms.i16GridFreq);   //30电网频率低字节CalcResultRms.i16GridTFreq;

	SciTxBuff[point++] = GET_HBYTE_OF_WORD(M_CalcResultRms.i16VGRID_R);     //31 电网R相电压VGird
	SciTxBuff[point++] = GET_LBYTE_OF_WORD(M_CalcResultRms.i16VGRID_R);     //32 电网R相电压VGird
	SciTxBuff[point++] = GET_HBYTE_OF_WORD(M_CalcResultRms.i16IGridR);     //33 电网R相电流IGrid
	SciTxBuff[point++] = GET_LBYTE_OF_WORD(M_CalcResultRms.i16IGridR);     //34 电网R相电流IGrid
	SciTxBuff[point++] = GET_HBYTE_OF_WORD(M_CalcResultRms.i16VGRID_S);     //35 电网S相电压VGrid
	SciTxBuff[point++] = GET_LBYTE_OF_WORD(M_CalcResultRms.i16VGRID_S);     //36 电网S相电压VGrid
	SciTxBuff[point++] = GET_HBYTE_OF_WORD(M_CalcResultRms.i16IGridS);     //37 电网S相电流IGrid
	SciTxBuff[point++] = GET_LBYTE_OF_WORD(M_CalcResultRms.i16IGridS);     //38 电网S相电流IGrid
	SciTxBuff[point++] = GET_HBYTE_OF_WORD(M_CalcResultRms.i16VGRID_T);     //39 电网T相电筕Grid
	SciTxBuff[point++] = GET_LBYTE_OF_WORD(M_CalcResultRms.i16VGRID_T);     //40 电网T相电压VGrid
	SciTxBuff[point++] = GET_HBYTE_OF_WORD(M_CalcResultRms.i16IGridT);     //41 电网T相电流IGrid
	SciTxBuff[point++] = GET_LBYTE_OF_WORD(M_CalcResultRms.i16IGridT);     //42电网T相电流IGrid

	SciTxBuff[point++] = GET_HBYTE_OF_WORD(M_CalcResultRms.i16EnergyTodayNew);   //43当日输出电量高字节
	SciTxBuff[point++] = GET_LBYTE_OF_WORD(M_CalcResultRms.i16EnergyTodayNew);   //44当日输出电量高字节

	SciTxBuff[point++] = 0;   //45当日发电时间高字节
	SciTxBuff[point++] = 0;   //46当日发电时间低字节

	SciTxBuff[point++] = GET_HBYTE_OF_WORD(CalcResultAve.i16TempINVMax);   //47Inv温度
	SciTxBuff[point++] = GET_LBYTE_OF_WORD(CalcResultAve.i16TempINVMax);   //48Inv温度


	SciTxBuff[point++] = GET_HBYTE_OF_WORD(CalcResultAve.i16TempAmb);   //49Inv温度
	SciTxBuff[point++] = GET_LBYTE_OF_WORD(CalcResultAve.i16TempAmb);   //50Inv温度

	SciTxBuff[point++] = GET_HBYTE_OF_WORD(M_CalcResultRms.i16Vbus);     //51 电网T相电流IGrid
	SciTxBuff[point++] = GET_LBYTE_OF_WORD(M_CalcResultRms.i16Vbus);     //52 电网T相电流IGrid

	SciTxBuff[point++] = GET_HBYTE_OF_WORD(CalcResultAve.i16VPV1);   //53第一路PV电压
	SciTxBuff[point++] = GET_LBYTE_OF_WORD(CalcResultAve.i16VPV1);   //54第一路PV电压

	SciTxBuff[point++] = GET_HBYTE_OF_WORD(CalcResultAve.i16VPV2);   //55第一路PV电压
	SciTxBuff[point++] = GET_LBYTE_OF_WORD(CalcResultAve.i16VPV2);   //56第一路PV电压

	SciTxBuff[point++] = GET_HBYTE_OF_WORD(ProcessVariable.i16ReConnetionTime);   //(GetRealValue.i16TempAmb);//,CalcResultAve.i16TempBoost1);//
	SciTxBuff[point++] = GET_LBYTE_OF_WORD(ProcessVariable.i16ReConnetionTime);   //CalcResultAve.i16TempBoost1);//
	SciTxBuff[point++] = 0x00;   //59warning information
	SciTxBuff[point++] = GET_LBYTE_OF_WORD(MainDSPWarningByte);   //60warning information

	SciTxBuff[point++] = GET_HBYTE_OF_WORD(CalcResultAve.i16VISO1_testoutput1);  //61 Hight byte of PV1 ISO
	SciTxBuff[point++] = GET_LBYTE_OF_WORD(CalcResultAve.i16VISO1_testoutput1);  //62 Low byte of PV1 ISO 

	SciTxBuff[point++] = GET_HBYTE_OF_WORD(CalcResultAve.i16VISO1_testoutput2);  //63 Hight byte of PV2 ISO
	SciTxBuff[point++] = GET_LBYTE_OF_WORD(CalcResultAve.i16VISO1_testoutput2);  //64 Low byte of PV2 ISO

	SciTxBuff[point++] = GET_HBYTE_OF_WORD(CalcResultAve.i16VISO1_testoutput3);  //65 Hight byte of PN ISO
	SciTxBuff[point++] = GET_LBYTE_OF_WORD(CalcResultAve.i16VISO1_testoutput3);  //66 Low byte of PN ISO

	for(i=0;i<=SciTxBuff[1] + 1;i++)
	{
		checksumtemp = checksumtemp + ((Uint16)SciTxBuff[i] & 0x00FF);
	}

    SciTxBuff[point++] = (Uint8)(checksumtemp & 0x00FF);//62
    SciTxBuff[point] = (Uint8)(checksumtemp >>8);   //61
}

/**********************************************************************
* FUNCION :  PrepareForWait
* PURPOSE :  
**********************************************************************/
void PrepareForWait(void)
{
	PWMOFFandRelayDisconnect();
		     
	s_StateCheck.bit.AcRlyCheckOver = 0;
	s_StateCheck.bit.PVIsoCheckOver = 0;

	s_StateCheck.bit.AD_initial = 1;
}
/**********************************************************************
* FUNCION :  Sci_SendData
* PURPOSE :  
**********************************************************************/

void GoBacktoOrig(void)
{
	ClearEEPROM();
	if(writedefaultsafety(0))
	{
		SetSciValueForAnswerLCD(0x66, 0x30);
		s_SysFaultMessage.bit.unrecoverEEPROM_W = 1;
	}
	else
	{
		Orig_epromflg = 0xffff;		
		s_SysCurrentState_Master = WaitState;
		PrepareForWait();  	
		SetSciValueForAnswerLCD(0x55, 0x30);			
	}
}


/**********************************************************************
* FUNCION :  Sci_SendData
* PURPOSE :  
**********************************************************************/

void GetONOFFCommand(void)
{
	Uint16 temp1;

	temp1 = ((Uint16)(SciRxBuff[5]<<8) | SciRxBuff[6]);
	if(temp1 == SafetyRemotPowerControlTable[2])
	{
		SetSciValueForAnswerLCD(0x55, 0x80);
	}
	else
	{
		SafetyRemotPowerControlTable[2] = temp1;

		if(CheckRemotPowerControlValue())
		{
			if(I2C_Write_InWord(SAFETY_RemotPowerControlValue_ADDR,&SafetyRemotPowerControlTable[2],1))
			{
			    s_SysFaultMessage.bit.unrecoverEEPROM_W = 1;
			    SetSciValueForAnswerLCD(0x66, 0x80);
			}
			else
			{	
				s_EepromWriteorRead_Flag = 1;
				SetSciValueForAnswerLCD(0x55, 0x80);
				s_StateCheck.bit.RemotPowerControlValue_Updateflag = 1;
			}
		}
		else
		{
			s_SysFaultMessage.bit.unrecoverEEPROM_W = 1;
		    SetSciValueForAnswerLCD(0x66, 0x80);
		}
	}
}

/**********************************************************************
* FUNCION :  Sci_SendData
* PURPOSE :  
**********************************************************************/

void GetDeratingCommand(void)
{
	Uint16 temp1;

	temp1 = ((Uint16)(SciRxBuff[5]<<8) | SciRxBuff[6]);
	if(temp1 == SafetyRemotPowerControlTable[1])
	{
		SetSciValueForAnswerLCD(0x55, 0x81);
	}
	else
	{
		SafetyRemotPowerControlTable[1] = temp1;

		if(CheckRemotPowerControlValue())
		{
			if(I2C_Write_InWord(SAFETY_RemotPowerControlValue_ADDR,&SafetyRemotPowerControlTable[1],1))
			{
			    s_SysFaultMessage.bit.unrecoverEEPROM_W = 1;
			    SetSciValueForAnswerLCD(0x66, 0x81);
			}
			else
			{	
				s_EepromWriteorRead_Flag = 1;
				SetSciValueForAnswerLCD(0x55, 0x81);
				s_StateCheck.bit.RemotPowerControlValue_Updateflag = 1;
			}
		}
		else
		{
			s_SysFaultMessage.bit.unrecoverEEPROM_W = 1;
		    SetSciValueForAnswerLCD(0x66, 0x81);
		}
	}
}
/**********************************************************************
* FUNCION :  Sci_SendData
* PURPOSE :  			  		 //通讯板发送版本信息，机型。
**********************************************************************/

void GetPowerFactorCommand(void)
{
	Uint16 temp1;

	temp1 = ((Uint16)(SciRxBuff[5]<<8) | SciRxBuff[6]);
	if(temp1 == SafetyLVRTTable[1])
	{
		SetSciValueForAnswerLCD(0x55, 0x82);
	}
	else
	{
		SafetyLVRTTable[1] = temp1;

		if(CheckReactiveValue())
		{
			if(I2C_Write_InWord(SAFETY_ReactiveValue_ADDR,&SafetyReactiveTable[1],1))
			{
			    s_SysFaultMessage.bit.unrecoverEEPROM_W = 1;
			    SetSciValueForAnswerLCD(0x66, 0x82);
			}
			else
			{	
				s_EepromWriteorRead_Flag = 1;
				SetSciValueForAnswerLCD(0x55, 0x82);
				s_StateCheck.bit.ReactiveValue_Updateflag1 = 1;
			}
		}
		else
		{
			s_SysFaultMessage.bit.unrecoverEEPROM_W = 1;
		    SetSciValueForAnswerLCD(0x66, 0x82);
		}
	}
}
/**********************************************************************
* FUNCION :  Sci_SendData
* PURPOSE :  			  		 //通讯板发送版本信息，机型。
**********************************************************************/

void GetReactivePowerCommand(void)
{
	Uint16 temp1;

	temp1 = ((Uint16)(SciRxBuff[5]<<8) | SciRxBuff[6]);
	if(SafetyLVRTTable[2] == temp1)
	{
		SetSciValueForAnswerLCD(0x55, 0x83);		
	}
	else
	{
		SafetyLVRTTable[2] = temp1;
		if(CheckReactiveValue())
		{
			if(I2C_Write_InWord(SAFETY_ReactiveValue_ADDR,&SafetyReactiveTable[2],1))
			{
			    s_SysFaultMessage.bit.unrecoverEEPROM_W = 1;
			    SetSciValueForAnswerLCD(0x66, 0x83);
			}
			else
			{	
				s_EepromWriteorRead_Flag = 1;
				SetSciValueForAnswerLCD(0x55, 0x83);
				s_StateCheck.bit.ReactiveValue_Updateflag1 = 1;
			}
		}
		else
		{
			s_SysFaultMessage.bit.unrecoverEEPROM_W = 1;
		    SetSciValueForAnswerLCD(0x66, 0x83);
		}
	}
}

/**********************************************************************
* FUNCION :  Sci_SendData
* PURPOSE :  
**********************************************************************/

void ResetToDefaultsafetyvalue(void)
{
	Uint16 point1=0;
	Uint16 point2=0;

	if(writedefaultsafety(SafetyCountry))
	{
		s_SysFaultMessage.bit.unrecoverEEPROM_W = 1;
		SetSciValueForAnswerLCD(0x66, 0x40);
	}
	else
	{
		SafetySTARTTable[point1++] = SafetyDefaultSTARTTable[SafetyCountry][point2++];
		SafetySTARTTable[point1++] = SafetyDefaultSTARTTable[SafetyCountry][point2++];
		SafetySTARTTable[point1++] = SafetyDefaultSTARTTable[SafetyCountry][point2++];
		SafetySTARTTable[point1++] = SafetyDefaultSTARTTable[SafetyCountry][point2++];
		SafetySTARTTable[point1++] = SafetyDefaultSTARTTable[SafetyCountry][point2++];
		SafetySTARTTable[point1++] = SafetyDefaultSTARTTable[SafetyCountry][point2++];
		SafetySTARTTable[point1++] = SafetyDefaultSTARTTable[SafetyCountry][point2++];
		SafetySTARTTable[point1++] = SafetyDefaultSTARTTable[SafetyCountry][point2++];
		point1 = 0;
		point2 = 0;

		SafetyVTable[point1++] = SafetyDefaultVTable[SafetyCountry][point2++];
		SafetyVTable[point1++] = SafetyDefaultVTable[SafetyCountry][point2++];
		SafetyVTable[point1++] = SafetyDefaultVTable[SafetyCountry][point2++];
		SafetyVTable[point1++] = SafetyDefaultVTable[SafetyCountry][point2++];
		SafetyVTable[point1++] = SafetyDefaultVTable[SafetyCountry][point2++];
		SafetyVTable[point1++] = SafetyDefaultVTable[SafetyCountry][point2++];
		SafetyVTable[point1++] = SafetyDefaultVTable[SafetyCountry][point2++];
		SafetyVTable[point1++] = SafetyDefaultVTable[SafetyCountry][point2++];
		SafetyVTable[point1++] = SafetyDefaultVTable[SafetyCountry][point2++];
		SafetyVTable[point1++] = SafetyDefaultVTable[SafetyCountry][point2++];
		point1 = 0;
		point2 = 0;

		SafetyFreTable[point1++] = SafetyDefaultFreTable[SafetyCountry][point2++];
		SafetyFreTable[point1++] = SafetyDefaultFreTable[SafetyCountry][point2++];
		SafetyFreTable[point1++] = SafetyDefaultFreTable[SafetyCountry][point2++];
		SafetyFreTable[point1++] = SafetyDefaultFreTable[SafetyCountry][point2++];
		SafetyFreTable[point1++] = SafetyDefaultFreTable[SafetyCountry][point2++];
		SafetyFreTable[point1++] = SafetyDefaultFreTable[SafetyCountry][point2++];
		SafetyFreTable[point1++] = SafetyDefaultFreTable[SafetyCountry][point2++];
		SafetyFreTable[point1++] = SafetyDefaultFreTable[SafetyCountry][point2++];
		SafetyFreTable[point1++] = SafetyDefaultFreTable[SafetyCountry][point2++];
		point1 = 0;
		point2 = 0;		

		SafetyDCITable[point1++] =  SafetyDefaultDCITable[SafetyCountry][point2++];
		SafetyDCITable[point1++] =  SafetyDefaultDCITable[SafetyCountry][point2++];
		SafetyDCITable[point1++] =  SafetyDefaultDCITable[SafetyCountry][point2++];
		SafetyDCITable[point1++] =  SafetyDefaultDCITable[SafetyCountry][point2++];
		SafetyDCITable[point1++] =  SafetyDefaultDCITable[SafetyCountry][point2++];
		SafetyDCITable[point1++] =  SafetyDefaultDCITable[SafetyCountry][point2++];
		point1 = 0;
		point2 = 0;	
		
		SafetyRemotPowerControlTable[point1++] =  SafetyDefaultRemotPowerControlTable[SafetyCountry][point2++];
		SafetyRemotPowerControlTable[point1++] =  SafetyDefaultRemotPowerControlTable[SafetyCountry][point2++];
		SafetyRemotPowerControlTable[point1++] =  SafetyDefaultRemotPowerControlTable[SafetyCountry][point2++];

		point1 = 0;
		point2 = 0;	
		
		SafetyFreqPowerControlTable[point1++] =  SafetyDefaultFreqPowerControlTable[SafetyCountry][point2++];
		SafetyFreqPowerControlTable[point1++] =  SafetyDefaultFreqPowerControlTable[SafetyCountry][point2++];
		SafetyFreqPowerControlTable[point1++] =  SafetyDefaultFreqPowerControlTable[SafetyCountry][point2++];
		SafetyFreqPowerControlTable[point1++] =  SafetyDefaultFreqPowerControlTable[SafetyCountry][point2++];
		SafetyFreqPowerControlTable[point1++] =  SafetyDefaultFreqPowerControlTable[SafetyCountry][point2++];
		SafetyFreqPowerControlTable[point1++] =  SafetyDefaultFreqPowerControlTable[SafetyCountry][point2++];
		SafetyFreqPowerControlTable[point1++] =  SafetyDefaultFreqPowerControlTable[SafetyCountry][point2++];
		point1 = 0;
		point2 = 0;	

		SafetyReactiveTable[point1++] =  SafetyDefaultReactiveTable[SafetyCountry][point2++];
		SafetyReactiveTable[point1++] =  SafetyDefaultReactiveTable[SafetyCountry][point2++];
		SafetyReactiveTable[point1++] =  SafetyDefaultReactiveTable[SafetyCountry][point2++];
		SafetyReactiveTable[point1++] =  SafetyDefaultReactiveTable[SafetyCountry][point2++];
		SafetyReactiveTable[point1++] =  SafetyDefaultReactiveTable[SafetyCountry][point2++];
		SafetyReactiveTable[point1++] =  SafetyDefaultReactiveTable[SafetyCountry][point2++];
		SafetyReactiveTable[point1++] =  SafetyDefaultReactiveTable[SafetyCountry][point2++];	
		SafetyReactiveTable[point1++] =  SafetyDefaultReactiveTable[SafetyCountry][point2++];
		SafetyReactiveTable[point1++] =  SafetyDefaultReactiveTable[SafetyCountry][point2++];
		SafetyReactiveTable[point1++] =  SafetyDefaultReactiveTable[SafetyCountry][point2++];
		SafetyReactiveTable[point1++] =  SafetyDefaultReactiveTable[SafetyCountry][point2++];
		SafetyReactiveTable[point1++] =  SafetyDefaultReactiveTable[SafetyCountry][point2++];
		SafetyReactiveTable[point1++] =  SafetyDefaultReactiveTable[SafetyCountry][point2++];
		SafetyReactiveTable[point1++] =  SafetyDefaultReactiveTable[SafetyCountry][point2++];	
		SafetyReactiveTable[point1++] =  SafetyDefaultReactiveTable[SafetyCountry][point2++];	
		SafetyReactiveTable[point1++] =  SafetyDefaultReactiveTable[SafetyCountry][point2++];
		SafetyReactiveTable[point1++] =  SafetyDefaultReactiveTable[SafetyCountry][point2++];
		SafetyReactiveTable[point1++] =  SafetyDefaultReactiveTable[SafetyCountry][point2++];	
		SafetyReactiveTable[point1++] =  SafetyDefaultReactiveTable[SafetyCountry][point2++];
		SafetyReactiveTable[point1++] =  SafetyDefaultReactiveTable[SafetyCountry][point2++];	
		SafetyReactiveTable[point1++] =  SafetyDefaultReactiveTable[SafetyCountry][point2++];
		point1 = 0;
		point2 = 0;	

		SafetyLVRTTable[point1++] = SafetyDefaultLVRTTable[SafetyCountry][point2++];
		SafetyLVRTTable[point1++] = SafetyDefaultLVRTTable[SafetyCountry][point2++];
		SafetyLVRTTable[point1++] = SafetyDefaultLVRTTable[SafetyCountry][point2++];
		SafetyLVRTTable[point1++] = SafetyDefaultLVRTTable[SafetyCountry][point2++];
		SafetyLVRTTable[point1++] = SafetyDefaultLVRTTable[SafetyCountry][point2++];
		SafetyLVRTTable[point1++] = SafetyDefaultLVRTTable[SafetyCountry][point2++];
		SafetyLVRTTable[point1++] = SafetyDefaultLVRTTable[SafetyCountry][point2++];
		SafetyLVRTTable[point1++] = SafetyDefaultLVRTTable[SafetyCountry][point2++];
		SafetyLVRTTable[point1++] = SafetyDefaultLVRTTable[SafetyCountry][point2++];
		SafetyLVRTTable[point1++] = SafetyDefaultLVRTTable[SafetyCountry][point2++];
		SafetyLVRTTable[point1++] = SafetyDefaultLVRTTable[SafetyCountry][point2++];
		SafetyLVRTTable[point1++] = SafetyDefaultLVRTTable[SafetyCountry][point2++];
		SafetyLVRTTable[point1++] = SafetyDefaultLVRTTable[SafetyCountry][point2++];
		point1 = 0;
		point2 = 0;		
									
		SafetyIsoIslandandSoOnTable[point1++] = SafetyDefaultIsoIslandandSoOnTable[SafetyCountry][point2++];
		SafetyIsoIslandandSoOnTable[point1++] = SafetyDefaultIsoIslandandSoOnTable[SafetyCountry][point2++];
		SafetyIsoIslandandSoOnTable[point1++] = SafetyDefaultIsoIslandandSoOnTable[SafetyCountry][point2++];
		SafetyIsoIslandandSoOnTable[point1++] = SafetyDefaultIsoIslandandSoOnTable[SafetyCountry][point2++];
		point1 = 0;
		point2 = 0; 	
									
		FactoryMode = FactoryModeDefaultValue;
/*
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];			
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];			
		point1 = 0;
		point2 = 0; 
*/
								
		PrepareForWait();
		SetSciValueForAnswerLCD(0x55, 0x40);
		s_StateCheck.Word.byte4 = 0xFF;
		s_StateCheck.Word.byte5 = 0x3F;
	}
}

/**********************************************************************
* FUNCION :  Sci_SendData
* PURPOSE :  
**********************************************************************/
void ClearTodayOutputPower(void)
{        				               	    		
	temp_zero = 0;
	EnergyTodayOld = 0;
	if(I2C_Write_InWord((ENERGY_OUTPUT_ADDR+2*(Todaydate-1)),&temp_zero,1))
	{
		s_SysFaultMessage.bit.unrecoverEEPROM_W = 1;
		SetSciValueForAnswerLCD(0x66, 0x50);
	}
	else
	{
		SetSciValueForAnswerLCD(0x55, 0x50);
	}
}
/**********************************************************************
* FUNCION :  Sci_SendData
* PURPOSE :  
**********************************************************************/
void SetSofteVersionToLCD(void)
{
	Uint16 point_0X70 = 0;
	Uint16 count_0X70 = 0;
	Uint16 checksum_0X70 = 0;	
//	Uint16 powerleveltemp = 0;

	SciTxBuff[point_0X70++] = 1;	
	SciTxBuff[point_0X70++] = 10;
	SciTxBuff[point_0X70++] = 0x70;

	SciTxBuff[point_0X70++] = PowerLevelType;

	SciTxBuff[point_0X70++] = MASTER_MAIN_VERSION;
	SciTxBuff[point_0X70++] = MASTER_MAIN_VERSION0;
	SciTxBuff[point_0X70++] = MASTER_SECONDARY_VERSION1;
	SciTxBuff[point_0X70++] = MASTER_SECONDARY_VERSION2;

	SciTxBuff[point_0X70++] = SLAVE_MAIN_VERSION;
	SciTxBuff[point_0X70++] = SLAVE_MAIN_VERSION0;
	SciTxBuff[point_0X70++] = SLAVE_SECONDARY_VERSION1;
	SciTxBuff[point_0X70++] = SLAVE_SECONDARY_VERSION2;


	for(count_0X70 = 0;	count_0X70<=SciTxBuff[1] + 1; count_0X70++)  //计算校验码时包含了SciTxBuff[0]
	{
		checksum_0X70 = checksum_0X70 + ((Uint16)SciTxBuff[count_0X70] & 0x00ff);
	}
	
    SciTxBuff[point_0X70++] = (Uint8)(checksum_0X70 & 0xff);
    SciTxBuff[point_0X70] =  (Uint8)(checksum_0X70>> 8);
}
/**********************************************************************
* FUNCION :  Sci_SendData
* PURPOSE :  
**********************************************************************/
void SetSTARTValueToLCD(void)    	//向通讯板发送电网保护数据信息
{
	Uint16 i;
	Uint16 Safety_point = 0;
//	Uint16 SafetyTable_point = 0;
	Uint16 checksumtemp = 0;

	SciTxBuff[Safety_point++] = 0x01; // 副CPU 地址
	SciTxBuff[Safety_point++] = 34; //数据长度
	SciTxBuff[Safety_point++] = 0x60; //0 功能码
	SciTxBuff[Safety_point++] = 0x30; //0 子功能码

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetySTARTTable[0]);//3
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetySTARTTable[0]);//4
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetySTARTTable[1]);//5
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetySTARTTable[1]);//6
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetySTARTTable[2]);//7
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetySTARTTable[2]);//8
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetySTARTTable[3]);//9
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetySTARTTable[3]);//10	

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetySTARTTable[4]);//3
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetySTARTTable[4]);//4
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetySTARTTable[5]);//5
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetySTARTTable[5]);//6
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetySTARTTable[6]);//7
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetySTARTTable[6]);//8
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetySTARTTable[7]);//9
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetySTARTTable[7]);//10
	for(;Safety_point<=SciTxBuff[1]+1;)
    {
       SciTxBuff[Safety_point++] = 0;//保留  
    }  

	for(i=0;i<=(SciTxBuff[1] + 1);i++)
	{
		checksumtemp = checksumtemp + ((Uint16)SciTxBuff[i] & 0x00FF);
	}

    SciTxBuff[Safety_point++] = (Uint8)(checksumtemp & 0x00FF);
	SciTxBuff[Safety_point] = (Uint8)(checksumtemp >>8);         
}
/**********************************************************************
* FUNCION :  Sci_SendData
* PURPOSE :  
**********************************************************************/
void SetGridProtectVValueToLCD(void)    	//向通讯板发送电网保护数据信息
{
	Uint16 i;
	Uint16 Safety_point = 0;
  //	Uint16 SafetyTable_point = 0;
	Uint16 checksumtemp = 0;

	SciTxBuff[Safety_point++] = 0x01; // 副CPU 地址
	SciTxBuff[Safety_point++] = 34; //数据长度
	SciTxBuff[Safety_point++] = 0x60; //0 功能码
	SciTxBuff[Safety_point++] = 0x31; //0 子功能码

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyVTable[0]);//3
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyVTable[0]);//4

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyVTable[1]);//5
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyVTable[1]);//6
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyVTable[2]);//7
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyVTable[2]);//8
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyVTable[3]);//9
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyVTable[3]);//10	

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyVTable[4]);//3
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyVTable[4]);//4
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyVTable[5]);//5
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyVTable[5]);//6
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyVTable[6]);//7
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyVTable[6]);//8
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyVTable[7]);//9
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyVTable[7]);//10	

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyVTable[8]);//3
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyVTable[8]);//4
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyVTable[9]);//5
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyVTable[9]);//6
	for(;Safety_point<=SciTxBuff[1]+1;)
    {
       SciTxBuff[Safety_point++] = 0;//保留  
    }   


	for(i=0;i<=(SciTxBuff[1] + 1);i++)
	{
		checksumtemp = checksumtemp + ((Uint16)SciTxBuff[i] & 0x00FF);
	}

    SciTxBuff[Safety_point++] = (Uint8)(checksumtemp & 0x00FF);
    SciTxBuff[Safety_point] = (Uint8)(checksumtemp >>8);         
}
/**********************************************************************
* FUNCION :  Sci_SendData
* PURPOSE :  
**********************************************************************/
void SetGridProtectFreqValueToLCD(void)    	//向通讯板发送电网保护数据信息
{
	Uint16 i;
	Uint16 Safety_point = 0;
	//Uint16 SafetyTable_point = 0;
	Uint16 checksumtemp = 0;

	SciTxBuff[Safety_point++] = 0x01; // 副CPU 地址
	SciTxBuff[Safety_point++] = 34; //数据长度
	SciTxBuff[Safety_point++] = 0x60; //0 功能码
	SciTxBuff[Safety_point++] = 0x32; //0 子功能码



	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyFreTable[0]);//3
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyFreTable[0]);//4

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyFreTable[1]);//5
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyFreTable[1]);//6
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyFreTable[2]);//7
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyFreTable[2]);//8
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyFreTable[3]);//9
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyFreTable[3]);//10	

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyFreTable[4]);//3
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyFreTable[4]);//4
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyFreTable[5]);//5
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyFreTable[5]);//6
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyFreTable[6]);//7
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyFreTable[6]);//8
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyFreTable[7]);//9
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyFreTable[7]);//10	

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyFreTable[8]);//3
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyFreTable[8]);//4    
    for(;Safety_point<=SciTxBuff[1]+1;)
    {
       SciTxBuff[Safety_point++] = 0;//保留  
    }  


	for(i=0;i<=(SciTxBuff[1] + 1);i++)
	{
		checksumtemp = checksumtemp + ((Uint16)SciTxBuff[i] & 0x00FF);
	}

    SciTxBuff[Safety_point++] = (Uint8)(checksumtemp & 0x00FF);
    SciTxBuff[Safety_point] = (Uint8)(checksumtemp >>8);         
}
/**********************************************************************
* FUNCION :  Sci_SendData
* PURPOSE :  
**********************************************************************/
void SetGridProtectDCIValueToLCD(void)    	//向通讯板发送电网保な菪畔?
{
	Uint16 i;
	Uint16 Safety_point = 0;
//	Uint16 SafetyTable_point = 0;
	Uint16 checksumtemp = 0;

	SciTxBuff[Safety_point++] = 0x01; // 副CPU 地址
	SciTxBuff[Safety_point++] = 34; //数据长度
	SciTxBuff[Safety_point++] = 0x60; //0 功能码
	SciTxBuff[Safety_point++] = 0x33; //0 子功能码

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyDCITable[0]);//3
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyDCITable[0]);//4

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyDCITable[1]);//5
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyDCITable[1]);//6
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyDCITable[2]);//7
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyDCITable[2]);//8
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyDCITable[3]);//9
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyDCITable[3]);//10	

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyDCITable[4]);//3
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyDCITable[4]);//4
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyDCITable[5]);//5
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyDCITable[5]);//6
	for(;Safety_point<=SciTxBuff[1]+1;)
    {
       SciTxBuff[Safety_point++] = 0;//保留  
    } 
	

	for(i=0;i<=(SciTxBuff[1] + 1);i++)
	{
		checksumtemp = checksumtemp + ((Uint16)SciTxBuff[i] & 0x00FF);
	}

    SciTxBuff[Safety_point++] = (Uint8)(checksumtemp & 0x00FF);
    SciTxBuff[Safety_point] = (Uint8)(checksumtemp >>8);         
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
**********************************************************************/					
void SetRemotPowerControlValueToLCD(void)  //向通讯宸⑺驮冻逃泄β实鞫仁菪畔?
{
	Uint16 i;
	Uint16 Safety_point = 0;
//	Uint16 SafetyTable_point = 0;
	Uint16 checksumtemp = 0;

	SciTxBuff[Safety_point++] = 0x01; // 副CPU 地址
	SciTxBuff[Safety_point++] = 34; //数据长度
	SciTxBuff[Safety_point++] = 0x60; //0 功能码
	SciTxBuff[Safety_point++] = 0x34; //0 子功能码

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyRemotPowerControlTable[0]);//3
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyRemotPowerControlTable[0]);//4

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyRemotPowerControlTable[1]);//5
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyRemotPowerControlTable[1]);//6
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyRemotPowerControlTable[2]);//7
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyRemotPowerControlTable[2]);//8
    for(;Safety_point<=SciTxBuff[1]+1;)
    {
       SciTxBuff[Safety_point++] = 0;//保留  
    } 
    
	for(i=0;i<=(SciTxBuff[1] + 1);i++)
	{
		checksumtemp = checksumtemp + ((Uint16)SciTxBuff[i] & 0x00FF);
	}

    SciTxBuff[Safety_point++] = (Uint8)(checksumtemp & 0x00FF);
    SciTxBuff[Safety_point] = (Uint8)(checksumtemp >>8);         
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
**********************************************************************/					
void SetFreqPowerControlValueToLCD(void)  //向通讯板发送远程有功功率调度数据信息
{
	Uint16 i;
	Uint16 Safety_point = 0;
  //	Uint16 SafetyTable_point = 0;
	Uint16 checksumtemp = 0;

	SciTxBuff[Safety_point++] = 0x01; // 副CPU 地址
	SciTxBuff[Safety_point++] = 34; //数据长度
	SciTxBuff[Safety_point++] = 0x60; //0 功能码
	SciTxBuff[Safety_point++] = 0x35; //0 子功能码

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyFreqPowerControlTable[0]);//3
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyFreqPowerControlTable[0]);//4

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyFreqPowerControlTable[1]);//5
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyFreqPowerControlTable[1]);//6
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyFreqPowerControlTable[2]);//7
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyFreqPowerControlTable[2]);//8
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyFreqPowerControlTable[3]);//9
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyFreqPowerControlTable[3]);//10	

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyFreqPowerControlTable[4]);//3
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyFreqPowerControlTable[4]);//4
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyFreqPowerControlTable[5]);//5
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyFreqPowerControlTable[5]);//6
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyFreqPowerControlTable[6]);//5
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyFreqPowerControlTable[6]);//6
    for(;Safety_point<=SciTxBuff[1]+1;)
    {
       SciTxBuff[Safety_point++] = 0;//保留  
    } 

	for(i=0;i<=(SciTxBuff[1] + 1);i++)
	{
		checksumtemp = checksumtemp + ((Uint16)SciTxBuff[i] & 0x00FF);
	}

    SciTxBuff[Safety_point++] = (Uint8)(checksumtemp & 0x00FF);
	SciTxBuff[Safety_point] = (Uint8)(checksumtemp >>8);         
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
**********************************************************************/						
void SetReactiveValueToLCD(void)      //向通讯板发送无功控制电流数据信息
{
	Uint16 i;
	Uint16 Safety_point = 0;
	//Uint16 SafetyTable_point = 0;
	Uint16 checksumtemp = 0;


	SciTxBuff[Safety_point++] = 0x01; // 副CPU 地址
	SciTxBuff[Safety_point++] = 66; //数据长度
	SciTxBuff[Safety_point++] = 0x60; //0 功能码
	SciTxBuff[Safety_point++] = 0x36; //0 子功能码

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyReactiveTable[0]);//3
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyReactiveTable[0]);//4

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyReactiveTable[1]);//5
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyReactiveTable[1]);//6
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyReactiveTable[2]);//7
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyReactiveTable[2]);//8
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyReactiveTable[3]);//9
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyReactiveTable[3]);//10	

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyReactiveTable[4]);//3
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyReactiveTable[4]);//4
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyReactiveTable[5]);//5
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyReactiveTable[5]);//6
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyReactiveTable[6]);//5
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyReactiveTable[6]);//6

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyReactiveTable[7]);//3
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyReactiveTable[7]);//4

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyReactiveTable[8]);//5
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyReactiveTable[8]);//6
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyReactiveTable[9]);//7
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyReactiveTable[9]);//8
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyReactiveTable[10]);//9
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyReactiveTable[10]);//10	

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyReactiveTable[11]);//3
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyReactiveTable[11]);//4
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyReactiveTable[12]);//5
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyReactiveTable[12]);//6
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyReactiveTable[13]);//5
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyReactiveTable[13]);//6

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyReactiveTable[14]);//5
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyReactiveTable[14]);//6
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyReactiveTable[15]);//5
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyReactiveTable[15]);//6

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyReactiveTable[16]);//3
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyReactiveTable[16]);//4

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyReactiveTable[17]);//5
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyReactiveTable[17]);//6
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyReactiveTable[18]);//7
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyReactiveTable[18]);//8
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyReactiveTable[19]);//9
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyReactiveTable[19]);//10	

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyReactiveTable[20]);//3
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyReactiveTable[20]);//4
    for(;Safety_point<=SciTxBuff[1]+1;)
    {
       SciTxBuff[Safety_point++] = 0;//保留  
    } 
    
	for(i=0;i<=(SciTxBuff[1] + 1);i++)
	{
		checksumtemp = checksumtemp + ((Uint16)SciTxBuff[i] & 0x00FF);
	}

    SciTxBuff[Safety_point++] = (Uint8)(checksumtemp & 0x00FF);
    SciTxBuff[Safety_point] = (Uint8)(checksumtemp >>8);   
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
**********************************************************************/						
void SetLVRTValueToLCD(void)  	//向通讯板发送低电压穿越数据信息
{
	Uint16 i;
	Uint16 Safety_point = 0;
	//Uint16 SafetyTable_point = 0;
	Uint16 checksumtemp = 0;


	SciTxBuff[Safety_point++] = 0x01; // 副CPU 地址
	SciTxBuff[Safety_point++] = 34; //数据长度
	SciTxBuff[Safety_point++] = 0x60; //0 功能码
	SciTxBuff[Safety_point++] = 0x37; //0 子功能码

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyLVRTTable[0]);//3
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyLVRTTable[0]);//4

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyLVRTTable[1]);//5
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyLVRTTable[1]);//6
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyLVRTTable[2]);//7
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyLVRTTable[2]);//8
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyLVRTTable[3]);//9
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyLVRTTable[3]);//10	

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyLVRTTable[4]);//3
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyLVRTTable[4]);//4
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyLVRTTable[5]);//5
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyLVRTTable[5]);//6
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyLVRTTable[6]);//5
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyLVRTTable[6]);//6

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyLVRTTable[7]);//3
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyLVRTTable[7]);//4

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyLVRTTable[8]);//5
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyLVRTTable[8]);//6
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyLVRTTable[9]);//7
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyLVRTTable[9]);//8
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyLVRTTable[10]);//9
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyLVRTTable[10]);//10	

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyLVRTTable[11]);//3
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyLVRTTable[11]);//4
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyLVRTTable[12]);//5
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyLVRTTable[12]);//6
    for(;Safety_point<=SciTxBuff[1]+1;)
    {
       SciTxBuff[Safety_point++] = 0;//保留  
    } 
	
	for(i=0;i<=(SciTxBuff[1] + 1);i++)
	{
		checksumtemp = checksumtemp + ((Uint16)SciTxBuff[i] & 0x00FF);
	}

	SciTxBuff[Safety_point++] = (Uint8)(checksumtemp & 0x00FF);
	SciTxBuff[Safety_point] = (Uint8)(checksumtemp >>8);   
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
**********************************************************************/						
void SetIsoIslandandSoOnValueToLCD(void)   	//向通讯板发送孤岛，绝缘阻抗，漏电流、开关机等
{
	Uint16 i;
	Uint16 Safety_point = 0;
	//Uint16 SafetyTable_point = 0;
	Uint16 checksumtemp = 0;

	SciTxBuff[Safety_point++] = 0x01; // 副CPU 地址
	SciTxBuff[Safety_point++] = 34; //数据长度
	SciTxBuff[Safety_point++] = 0x60; //0 功能码
	SciTxBuff[Safety_point++] = 0x38; //0 子功能码

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyIsoIslandandSoOnTable[0]);//3
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyIsoIslandandSoOnTable[0]);//4

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyIsoIslandandSoOnTable[1]);//5
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyIsoIslandandSoOnTable[1]);//6
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyIsoIslandandSoOnTable[2]);//7
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyIsoIslandandSoOnTable[2]);//8
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(SafetyIsoIslandandSoOnTable[3]);//9
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(SafetyIsoIslandandSoOnTable[3]);//10	
    for(;Safety_point<=SciTxBuff[1]+1;)
    {
       SciTxBuff[Safety_point++] = 0;//保留  
    } 
    
	for(i=0;i<=(SciTxBuff[1] + 1);i++)
	{
		checksumtemp = checksumtemp + ((Uint16)SciTxBuff[i] & 0x00FF);
	}

	SciTxBuff[Safety_point++] = (Uint8)(checksumtemp & 0x00FF); 
    SciTxBuff[Safety_point] = (Uint8)(checksumtemp >>8);   
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
**********************************************************************/						
void SetFactoryModeValueToLCD(void)   	//向通讯板发送孤岛，绝缘阻抗，漏电流、开关机等
{
	Uint16 i;
	Uint16 Safety_point = 0;
	//Uint16 SafetyTable_point = 0;
	Uint16 checksumtemp = 0;

	SciTxBuff[Safety_point++] = 0x01; // 副CPU 地址
	SciTxBuff[Safety_point++] = 34; //数据长度
	SciTxBuff[Safety_point++] = 0x60; //0 功能码
	SciTxBuff[Safety_point++] = 0x39; //0 子功能码

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(FactoryMode);//3
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(FactoryMode);//4
	for(;Safety_point<=SciTxBuff[1]+1;)
    {
       SciTxBuff[Safety_point++] = 0;//保留  
    } 

	for(i=0;i<=(SciTxBuff[1] + 1);i++)
	{
		checksumtemp = checksumtemp + ((Uint16)SciTxBuff[i] & 0x00FF);
	}

	SciTxBuff[Safety_point++] = (Uint8)(checksumtemp & 0x00FF); 
    SciTxBuff[Safety_point] = (Uint8)(checksumtemp >>8);   
}

/**********************************************************************
* FUNCION :  
* PURPOSE :  
**********************************************************************/						
void SetCalibrationValueToLCD(void)      //向通讯板发送校准参数信息
{
	Uint16 i;
	Uint16 Safety_point = 0;
	//Uint16 SafetyTable_point = 0;
	Uint16 checksumtemp = 0;


	SciTxBuff[Safety_point++] = 0x01; // 副CPU 地址
	SciTxBuff[Safety_point++] = 66; //数据长度
	SciTxBuff[Safety_point++] = 0x60; //0 功能码
	SciTxBuff[Safety_point++] = 0x3A; //0 子功能码

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(CalibrationTable[0]);//3
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(CalibrationTable[0]);//4

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(CalibrationTable[1]);//5
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(CalibrationTable[1]);//6
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(CalibrationTable[2]);//7
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(CalibrationTable[2]);//8
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(CalibrationTable[3]);//9
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(CalibrationTable[3]);//10	

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(CalibrationTable[4]);//3
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(CalibrationTable[4]);//4
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(CalibrationTable[5]);//5
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(CalibrationTable[5]);//6
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(CalibrationTable[6]);//5
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(CalibrationTable[6]);//6

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(CalibrationTable[7]);//3
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(CalibrationTable[7]);//4

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(CalibrationTable[8]);//5
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(CalibrationTable[8]);//6
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(CalibrationTable[9]);//7
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(CalibrationTable[9]);//8
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(CalibrationTable[10]);//9
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(CalibrationTable[10]);//10	

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(CalibrationTable[11]);//3
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(CalibrationTable[11]);//4
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(CalibrationTable[12]);//5
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(CalibrationTable[12]);//6
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(CalibrationTable[13]);//5
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(CalibrationTable[13]);//6

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(CalibrationTable[14]);//5
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(CalibrationTable[14]);//6
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(CalibrationTable[15]);//5
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(CalibrationTable[15]);//6

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(CalibrationTable[16]);//3
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(CalibrationTable[16]);//4

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(CalibrationTable[17]);//5
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(CalibrationTable[17]);//6
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(CalibrationTable[18]);//7
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(CalibrationTable[18]);//8
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(CalibrationTable[19]);//9
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(CalibrationTable[19]);//10	

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(CalibrationTable[20]);//3
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(CalibrationTable[20]);//4


	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(CalibrationTable[21]);//5
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(CalibrationTable[21]);//6
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(CalibrationTable[22]);//7
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(CalibrationTable[22]);//8
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(CalibrationTable[23]);//9
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(CalibrationTable[23]);//10	

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(CalibrationTable[24]);//3
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(CalibrationTable[24]);//4
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(CalibrationTable[25]);//5
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(CalibrationTable[25]);//6
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(CalibrationTable[26]);//5
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(CalibrationTable[26]);//6

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(CalibrationTable[27]);//5
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(CalibrationTable[27]);//6
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(CalibrationTable[28]);//5
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(CalibrationTable[28]);//6

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(CalibrationTable[29]);//3
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(CalibrationTable[29]);//4

	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(CalibrationTable[30]);//5
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(CalibrationTable[30]);//6
	SciTxBuff[Safety_point++] = GET_HBYTE_OF_WORD(CalibrationTable[31]);//7
	SciTxBuff[Safety_point++] = GET_LBYTE_OF_WORD(CalibrationTable[31]);//8


	for(i=0;i<=(SciTxBuff[1] + 1);i++)
	{
		checksumtemp = checksumtemp + ((Uint16)SciTxBuff[i] & 0x00FF);
	}

	SciTxBuff[Safety_point++] = (Uint8)(checksumtemp & 0x00FF);
	SciTxBuff[Safety_point] = (Uint8)(checksumtemp >>8);   
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
**********************************************************************/						
void GetSTARTValueFromLCD(void)     	//0x30,通讯板获得并保存电网保护数据信息,SafetySTARTTable[8]
{
	Uint16 Safety_point = 5;
	Uint16 SafetyTable_point = 0;

	SafetySTARTTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetySTARTTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetySTARTTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetySTARTTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetySTARTTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetySTARTTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetySTARTTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetySTARTTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);

	if(CheckSTARTValue())
	{
		if(I2C_Write_InWord(SAFETY_STARTValue_ADDR,SafetySTARTTable,SafetyTable_point))
		{
		    s_SysFaultMessage.bit.unrecoverEEPROM_W = 1;
		    SetSciValueForResetOrModifySafetyvalue(0x66, 0x30);
		}
		else
		{	
			s_EepromWriteorRead_Flag = 1;
			SetSciValueForResetOrModifySafetyvalue(0x55, 0x30);
			s_StateCheck.bit.SafetySTART_Updateflag = 1;
		}
	}
	else
	{
		s_SysFaultMessage.bit.unrecoverEEPROM_W = 1;
	    SetSciValueForResetOrModifySafetyvalue(0x66, 0x30);
	}
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
**********************************************************************/						
void GetGridProtectVValueFromLCD(void)     	//从通讯板获得并保存电网保护数据信息
{
	Uint16 Safety_point = 5;
	Uint16 SafetyTable_point = 0;


	SafetyVTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyVTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyVTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyVTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyVTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyVTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyVTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyVTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyVTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyVTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);

	if(CheckGridProtectVValue())
	{
		if(I2C_Write_InWord(SAFETY_GridProtectVValue_ADDR,SafetyVTable,SafetyTable_point))
		{
		    s_SysFaultMessage.bit.unrecoverEEPROM_W = 1;
		    SetSciValueForResetOrModifySafetyvalue(0x66, 0x31);
		}
		else
		{	
			s_EepromWriteorRead_Flag = 1;
			SetSciValueForResetOrModifySafetyvalue(0x55, 0x31);
			s_StateCheck.bit.GridProtectV_Updateflag = 1;
		}
	}
	else
	{
		s_SysFaultMessage.bit.unrecoverEEPROM_W = 1;
	    SetSciValueForResetOrModifySafetyvalue(0x66, 0x31);
	}
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
**********************************************************************/						
void GetGridProtectFreqValueFromLCD(void)     	//从通讯板获得并保存电网保护数据信息
{
	Uint16 Safety_point = 5;
	Uint16 SafetyTable_point = 0;


	SafetyFreTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyFreTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyFreTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyFreTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyFreTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyFreTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyFreTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyFreTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyFreTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);

	if(CheckGridProtectFreqValue())
	{
		if(I2C_Write_InWord(SAFETY_GridProtectFreqValue_ADDR,SafetyFreTable,SafetyTable_point))
		{
		    s_SysFaultMessage.bit.unrecoverEEPROM_W = 1;
		    SetSciValueForResetOrModifySafetyvalue(0x66, 0x32);
		}
		else
		{	
			s_EepromWriteorRead_Flag = 1;
			SetSciValueForResetOrModifySafetyvalue(0x55, 0x32);
			s_StateCheck.bit.GridProtectFreqValue_Updateflag = 1;
		}
	}
	else
	{
		s_SysFaultMessage.bit.unrecoverEEPROM_W = 1;
	    SetSciValueForResetOrModifySafetyvalue(0x66, 0x32);
	}
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
**********************************************************************/						
void GetGridProtectDCIValueFromLCD(void)     	//从通讯板获得并保存电网保护数据信息
{
	Uint16 Safety_point = 5;
	Uint16 SafetyTable_point = 0;

	SafetyDCITable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyDCITable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyDCITable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyDCITable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyDCITable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyDCITable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;

	if(CheckGridProtectDCIValue())
	{
		if(I2C_Write_InWord(SAFETY_GridProtectDCIValue_ADDR,SafetyDCITable,SafetyTable_point))
		{
		    s_SysFaultMessage.bit.unrecoverEEPROM_W = 1;
		    SetSciValueForResetOrModifySafetyvalue(0x66, 0x33);
		}
		else
		{	
			s_EepromWriteorRead_Flag = 1;
			SetSciValueForResetOrModifySafetyvalue(0x55, 0x33);
			s_StateCheck.bit.GridProtectDCI_Updateflag = 1;
		}
	}
	else
	{
		s_SysFaultMessage.bit.unrecoverEEPROM_W = 1;
	    SetSciValueForResetOrModifySafetyvalue(0x66, 0x33);
	}
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
**********************************************************************/						
void GetRemotPowerControlValueFromLCD(void)   //从通讯板获得并保存远程有功功率调度数菪畔?
{
	Uint16 Safety_point = 5;
	Uint16 SafetyTable_point = 0;

	SafetyRemotPowerControlTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyRemotPowerControlTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyRemotPowerControlTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;

	if(CheckRemotPowerControlValue())
	{
		if(I2C_Write_InWord(SAFETY_RemotPowerControlValue_ADDR,SafetyRemotPowerControlTable,SafetyTable_point))
		{
		    s_SysFaultMessage.bit.unrecoverEEPROM_W = 1;
		    SetSciValueForResetOrModifySafetyvalue(0x66, 0x34);
		}
		else
		{	
			s_EepromWriteorRead_Flag = 1;
			SetSciValueForResetOrModifySafetyvalue(0x55, 0x34);
			s_StateCheck.bit.RemotPowerControlValue_Updateflag = 1;
		}
	}
	else
	{
		s_SysFaultMessage.bit.unrecoverEEPROM_W = 1;
	    SetSciValueForResetOrModifySafetyvalue(0x66, 0x34);
	}
}

/**********************************************************************
* FUNCION :  
* PURPOSE :  
**********************************************************************/						
void GetFreqPowerControlValueFromLCD(void)   //从通讯板获得并保存远程有功功率调度数据信息
{
	Uint16 Safety_point = 5;
	Uint16 SafetyTable_point = 0;

	SafetyFreqPowerControlTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyFreqPowerControlTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyFreqPowerControlTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyFreqPowerControlTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyFreqPowerControlTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyFreqPowerControlTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyFreqPowerControlTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);


	if(CheckFreqPowerControlValue())
	{
		if(I2C_Write_InWord(SAFETY_FreqPowerControlValue_ADDR,SafetyFreqPowerControlTable,SafetyTable_point))
		{
		    s_SysFaultMessage.bit.unrecoverEEPROM_W = 1;
		    SetSciValueForResetOrModifySafetyvalue(0x66, 0x35);
		}
		else
		{	
			s_EepromWriteorRead_Flag = 1;
			SetSciValueForResetOrModifySafetyvalue(0x55, 0x35);
			s_StateCheck.bit.FreqPowerControlValue_Updateflag = 1;
		}
	}
	else
	{
		s_SysFaultMessage.bit.unrecoverEEPROM_W = 1;
	    SetSciValueForResetOrModifySafetyvalue(0x66, 0x35);
	}
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
**********************************************************************/						
void GetReactiveValueFromLCD(void)   	//从通讯板获得并保存随频率降载数据信息
{
	Uint16 Safety_point = 5;
	Uint16 SafetyTable_point = 0;

	SafetyReactiveTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyReactiveTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyReactiveTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyReactiveTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyReactiveTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyReactiveTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyReactiveTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyReactiveTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyReactiveTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyReactiveTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyReactiveTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyReactiveTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyReactiveTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyReactiveTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyReactiveTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyReactiveTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyReactiveTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyReactiveTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyReactiveTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyReactiveTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyReactiveTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);

	if(CheckReactiveValue())
	{
		if(I2C_Write_InWord(SAFETY_ReactiveValue_ADDR,SafetyReactiveTable,SafetyTable_point))
		{
		    s_SysFaultMessage.bit.unrecoverEEPROM_W = 1;
		    SetSciValueForResetOrModifySafetyvalue(0x66, 0x36);
		}
		else
		{	
			s_EepromWriteorRead_Flag = 1;
			SetSciValueForResetOrModifySafetyvalue(0x55, 0x36);
			s_StateCheck.bit.ReactiveValue_Updateflag1 = 1;
			s_StateCheck.bit.ReactiveValue_Updateflag2 = 1;
		}
	}
	else
	{
		s_SysFaultMessage.bit.unrecoverEEPROM_W = 1;
	    SetSciValueForResetOrModifySafetyvalue(0x66, 0x36);
	}

}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
**********************************************************************/						
void GetLVRTValueFromLCD(void)   	//从通讯板获得并保存低电压穿越数据信息	
{
	Uint16 Safety_point = 5;
	Uint16 SafetyTable_point = 0;

	SafetyLVRTTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyLVRTTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyLVRTTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyLVRTTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyLVRTTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyLVRTTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyLVRTTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyLVRTTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyLVRTTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyLVRTTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyLVRTTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyLVRTTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyLVRTTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
 	
	if(CheckLVRTValue())
	{
		if(I2C_Write_InWord(SAFETY_LVRTValue_ADDR,SafetyLVRTTable,SafetyTable_point))
		{
		    s_SysFaultMessage.bit.unrecoverEEPROM_W = 1;
		    SetSciValueForResetOrModifySafetyvalue(0x66, 0x37);
		}
		else
		{	
			s_EepromWriteorRead_Flag = 1;
			SetSciValueForResetOrModifySafetyvalue(0x55, 0x37);
			s_StateCheck.bit.LVRTValue_Updateflag1 = 1;
			s_StateCheck.bit.LVRTValue_Updateflag2 = 1;
		}
	}
	else
	{
		s_SysFaultMessage.bit.unrecoverEEPROM_W = 1;
	    SetSciValueForResetOrModifySafetyvalue(0x66, 0x37);
	}
	
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
**********************************************************************/						
void GetIsoIslandandSoOnValueFromLCD(void)   	//从通讯板获得并保存孤岛，绝缘阻抗，漏电流、开关机等
{
	Uint16 Safety_point = 5;
	Uint16 SafetyTable_point = 0;

	SafetyIsoIslandandSoOnTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyIsoIslandandSoOnTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyIsoIslandandSoOnTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	SafetyIsoIslandandSoOnTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);


	if(CheckIsoIslandandSoOnValue())
	{
		if(I2C_Write_InWord(SAFETY_IsoIslandandSoOn_ADDR,SafetyIsoIslandandSoOnTable,SafetyTable_point))
		{
		    s_SysFaultMessage.bit.unrecoverEEPROM_W = 1;
		    SetSciValueForResetOrModifySafetyvalue(0x66, 0x38);
		}
		else
		{	
			s_EepromWriteorRead_Flag = 1;
			SetSciValueForResetOrModifySafetyvalue(0x55, 0x38);
			s_StateCheck.bit.SafetyIsoIslandandSoOn_Updateflag = 1;
		}
	}
	else
	{
		s_SysFaultMessage.bit.unrecoverEEPROM_W = 1;
	    SetSciValueForResetOrModifySafetyvalue(0x66, 0x38);
	}
 
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
**********************************************************************/						
void GetFactoryModeValueFromLCD(void)   	//从通讯板获得并保存孤岛，绝缘阻抗，漏电流、开关机等
{
	Uint16 Safety_point = 5;

	FactoryMode = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);

	if(CheckFactoryModeValue())
	{
		if(I2C_Write_InWord(FactoryMode_ADDR, &FactoryMode,1))
		{
		    s_SysFaultMessage.bit.unrecoverEEPROM_W = 1;
		    SetSciValueForResetOrModifySafetyvalue(0x66, 0x39);
		}
		else
		{	
			s_EepromWriteorRead_Flag = 1;
			SetSciValueForResetOrModifySafetyvalue(0x55, 0x39);
			s_StateCheck.bit.FactoryMode_Updateflag = 1;
		}
	}
	else
	{
		s_SysFaultMessage.bit.unrecoverEEPROM_W = 1;
	    SetSciValueForResetOrModifySafetyvalue(0x66, 0x39);
	}
 
}

/**********************************************************************
* FUNCION :  
* PURPOSE :  
**********************************************************************/						
void GetCalibrationValueFromLCD(void)   	//从通讯板获得并保存孤岛，绝缘阻抗，漏电流、开关机等
{
	Uint16 Safety_point = 5;
	Uint16 SafetyTable_point = 0;
	int32 Temp1 = 0;
	int16 Temp2 = 0;

	CalibrationTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	CalibrationTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	CalibrationTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	CalibrationTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	CalibrationTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	CalibrationTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	CalibrationTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	CalibrationTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	CalibrationTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	CalibrationTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	CalibrationTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	CalibrationTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	CalibrationTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	CalibrationTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	CalibrationTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	CalibrationTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	CalibrationTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	CalibrationTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	CalibrationTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	CalibrationTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	CalibrationTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	CalibrationTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	CalibrationTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	CalibrationTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	CalibrationTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	CalibrationTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	CalibrationTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	CalibrationTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	CalibrationTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	CalibrationTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	CalibrationTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);
	Safety_point+=2;
	CalibrationTable[SafetyTable_point++] = ((Uint16)(SciRxBuff[Safety_point]<<8) | SciRxBuff[Safety_point+1]);



	if(CheckCalibrationValue())
	{
		if(I2C_Write_InWord(Calibration_ADDR,CalibrationTable,SafetyTable_point))
		{
		    s_SysFaultMessage.bit.unrecoverEEPROM_W = 1;
		    SetSciValueForResetOrModifySafetyvalue(0x66, 0x3A);
		}
		else
		{	
			s_EepromWriteorRead_Flag = 1;
			SetSciValueForResetOrModifySafetyvalue(0x55, 0x3A);
			s_StateCheck.bit.CalibrationPara_Updateflag1 = 1;
			s_StateCheck.bit.CalibrationPara_Updateflag2 = 1;

			Temp1 = 1536 * ((int32)CalibrationTable[28]);
			Temp2 = (int16)(Temp1>>12);
			ADGain.i16VPV1 = 10 * CalibrationTable[28] + Temp2;//10468;= 10 + 0.375;0.375* 4096 = 1536;
			ADOffsetSum.i16VPV1 = CalibrationTable[29];

			Temp1 = 1536 * ((int32)CalibrationTable[30]);
			Temp2 = (int16)(Temp1>>12);
			ADGain.i16VPV2 = 10 * CalibrationTable[30] + Temp2;//10468;
			ADOffsetSum.i16VPV2 = CalibrationTable[31];
		}
	}
	else
	{
		s_SysFaultMessage.bit.unrecoverEEPROM_W = 1;
	    SetSciValueForResetOrModifySafetyvalue(0x66, 0x3A);
	}
 
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
**********************************************************************/						
Uint16 CheckSTARTValue(void)     	//从通讯板获得并保存电网保护数据信息
{
	Uint16 SafetyTable_point = 0;

	if( (SafetySTARTTable[SafetyTable_point] > 0) && (SafetySTARTTable[SafetyTable_point++]<100)&&
		(SafetySTARTTable[SafetyTable_point] > 0) && (SafetySTARTTable[SafetyTable_point++]<100)&&
		(SafetySTARTTable[SafetyTable_point] > 0) && (SafetySTARTTable[SafetyTable_point++]<100)&&
		(SafetySTARTTable[SafetyTable_point] > 0) && (SafetySTARTTable[SafetyTable_point++]<100)&&
		(SafetySTARTTable[SafetyTable_point] > 0) && (SafetySTARTTable[SafetyTable_point++]<100)&&
		(SafetySTARTTable[SafetyTable_point] > 0) && (SafetySTARTTable[SafetyTable_point++]<100)&&
		(SafetySTARTTable[SafetyTable_point] > 0) && (SafetySTARTTable[SafetyTable_point++]<100)&&
		(SafetySTARTTable[SafetyTable_point] > 0) && (SafetySTARTTable[SafetyTable_point++]<100))
	{
		return 1;
		
	}
	else
	{
		return 1;//0;
	}
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
**********************************************************************/						
Uint16 CheckGridProtectVValue(void)     	//从通讯板获得并保存电网保护数据信息
{
	Uint16 SafetyTable_point = 0;

	if( (SafetyVTable[SafetyTable_point] > 0) && (SafetyVTable[SafetyTable_point++]<100)&&
		(SafetyVTable[SafetyTable_point] > 0) && (SafetyVTable[SafetyTable_point++]<100)&&
		(SafetyVTable[SafetyTable_point] > 0) && (SafetyVTable[SafetyTable_point++]<100)&&
		(SafetyVTable[SafetyTable_point] > 0) && (SafetyVTable[SafetyTable_point++]<100)&&
		(SafetyVTable[SafetyTable_point] > 0) && (SafetyVTable[SafetyTable_point++]<100)&&
		(SafetyVTable[SafetyTable_point] > 0) && (SafetyVTable[SafetyTable_point++]<100)&&
		(SafetyVTable[SafetyTable_point] > 0) && (SafetyVTable[SafetyTable_point++]<100)&&
		(SafetyVTable[SafetyTable_point] > 0) && (SafetyVTable[SafetyTable_point++]<100)&&
		(SafetyVTable[SafetyTable_point] > 0) && (SafetyVTable[SafetyTable_point++]<100)&&
		(SafetyVTable[SafetyTable_point] > 0) && (SafetyVTable[SafetyTable_point++]<100)&&
		(SafetyVTable[SafetyTable_point] > 0) && (SafetyVTable[SafetyTable_point++]<100))
	{
		return 1;
		
	}
	else
	{
		return 1;//0;
	}
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
**********************************************************************/						
Uint16 CheckGridProtectFreqValue(void)     	//从通讯板获得并４娴缤；な菪畔?
{
	Uint16 SafetyTable_point = 0;

	if( (SafetyFreTable[SafetyTable_point] > 0) && (SafetyFreTable[SafetyTable_point++]<100)&&
		(SafetyFreTable[SafetyTable_point] > 0) && (SafetyFreTable[SafetyTable_point++]<100)&&
		(SafetyFreTable[SafetyTable_point] > 0) && (SafetyFreTable[SafetyTable_point++]<100)&&
		(SafetyFreTable[SafetyTable_point] > 0) && (SafetyFreTable[SafetyTable_point++]<100)&&
		(SafetyFreTable[SafetyTable_point] > 0) && (SafetyFreTable[SafetyTable_point++]<100)&&
		(SafetyFreTable[SafetyTable_point] > 0) && (SafetyFreTable[SafetyTable_point++]<100)&&
		(SafetyFreTable[SafetyTable_point] > 0) && (SafetyFreTable[SafetyTable_point++]<100)&&
		(SafetyFreTable[SafetyTable_point] > 0) && (SafetyFreTable[SafetyTable_point++]<100)&&
		(SafetyFreTable[SafetyTable_point] > 0) && (SafetyFreTable[SafetyTable_point++]<100))
	{
		return 1;
		
	}
	else
	{
		return 1;//0;
	}
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
**********************************************************************/						
Uint16 CheckGridProtectDCIValue(void)     	//从通讯板获得并保存电网保护数据信息
{
	Uint16 SafetyTable_point = 0;

	if( (SafetyDCITable[SafetyTable_point] > 0) && (SafetyDCITable[SafetyTable_point++]<100)&&
		(SafetyDCITable[SafetyTable_point] > 0) && (SafetyDCITable[SafetyTable_point++]<100)&&
		(SafetyDCITable[SafetyTable_point] > 0) && (SafetyDCITable[SafetyTable_point++]<100)&&
		(SafetyDCITable[SafetyTable_point] > 0) && (SafetyDCITable[SafetyTable_point++]<100)&&
		(SafetyDCITable[SafetyTable_point] > 0) && (SafetyDCITable[SafetyTable_point++]<100))
	{
		return 1;
		
	}
	else
	{
		return 1;//0;
	}
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
**********************************************************************/						
Uint16 CheckRemotPowerControlValue(void)     	//从通讯板获得并保存电网保护数据信息
{
	Uint16 SafetyTable_point = 0;

	if( (SafetyRemotPowerControlTable[SafetyTable_point] > 0) && (SafetyRemotPowerControlTable[SafetyTable_point++]<100)&&
		(SafetyRemotPowerControlTable[SafetyTable_point] > 0) && (SafetyRemotPowerControlTable[SafetyTable_point++]<100))
	{
		return 1;
		
	}
	else
	{
		return 1;//0;
	}
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
**********************************************************************/						
Uint16 CheckFreqPowerControlValue(void)     	//从通讯板获得并保存电网保护数据信息
{
	Uint16 SafetyTable_point = 0;

	if( (SafetyFreqPowerControlTable[SafetyTable_point] > 0) && (SafetyFreqPowerControlTable[SafetyTable_point++]<100)&&
		(SafetyFreqPowerControlTable[SafetyTable_point] > 0) && (SafetyFreqPowerControlTable[SafetyTable_point++]<100)&&
		(SafetyFreqPowerControlTable[SafetyTable_point] > 0) && (SafetyFreqPowerControlTable[SafetyTable_point++]<100)&&
		(SafetyFreqPowerControlTable[SafetyTable_point] > 0) && (SafetyFreqPowerControlTable[SafetyTable_point++]<100)&&
		(SafetyFreqPowerControlTable[SafetyTable_point] > 0) && (SafetyFreqPowerControlTable[SafetyTable_point++]<100)&&
		(SafetyFreqPowerControlTable[SafetyTable_point] > 0) && (SafetyFreqPowerControlTable[SafetyTable_point++]<100)&&
		(SafetyFreqPowerControlTable[SafetyTable_point] > 0) && (SafetyFreqPowerControlTable[SafetyTable_point++]<100)&&
		(SafetyFreqPowerControlTable[SafetyTable_point] > 0) && (SafetyFreqPowerControlTable[SafetyTable_point++]<100))
	{
		return 1;
		
	}
	else
	{
		return 1;//0;
	}
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
**********************************************************************/						
Uint16 CheckReactiveValue(void)     	//从通讯板获得并保存电网保护数据信息
{
	Uint16 SafetyTable_point = 0;

	if( (SafetyReactiveTable[SafetyTable_point] > 0) && (SafetyReactiveTable[SafetyTable_point++]<100)&&
		(SafetyReactiveTable[SafetyTable_point] > 0) && (SafetyReactiveTable[SafetyTable_point++]<100)&&
		(SafetyReactiveTable[SafetyTable_point] > 0) && (SafetyReactiveTable[SafetyTable_point++]<100)&&
		(SafetyReactiveTable[SafetyTable_point] > 0) && (SafetyReactiveTable[SafetyTable_point++]<100)&&
		(SafetyReactiveTable[SafetyTable_point] > 0) && (SafetyReactiveTable[SafetyTable_point++]<100)&&
		(SafetyReactiveTable[SafetyTable_point] > 0) && (SafetyReactiveTable[SafetyTable_point++]<100)&&
		(SafetyReactiveTable[SafetyTable_point] > 0) && (SafetyReactiveTable[SafetyTable_point++]<100)&&
		(SafetyReactiveTable[SafetyTable_point] > 0) && (SafetyReactiveTable[SafetyTable_point++]<100)&&
		(SafetyReactiveTable[SafetyTable_point] > 0) && (SafetyReactiveTable[SafetyTable_point++]<100)&&
		(SafetyReactiveTable[SafetyTable_point] > 0) && (SafetyReactiveTable[SafetyTable_point++]<100)&&
		(SafetyReactiveTable[SafetyTable_point] > 0) && (SafetyReactiveTable[SafetyTable_point++]<100)&&
		(SafetyReactiveTable[SafetyTable_point] > 0) && (SafetyReactiveTable[SafetyTable_point++]<100)&&
		(SafetyReactiveTable[SafetyTable_point] > 0) && (SafetyReactiveTable[SafetyTable_point++]<100)&&
		(SafetyReactiveTable[SafetyTable_point] > 0) && (SafetyReactiveTable[SafetyTable_point++]<100)&&
		(SafetyReactiveTable[SafetyTable_point] > 0) && (SafetyReactiveTable[SafetyTable_point++]<100)&&
		(SafetyReactiveTable[SafetyTable_point] > 0) && (SafetyReactiveTable[SafetyTable_point++]<100)&&
		(SafetyReactiveTable[SafetyTable_point] > 0) && (SafetyReactiveTable[SafetyTable_point++]<100)&&
		(SafetyReactiveTable[SafetyTable_point] > 0) && (SafetyReactiveTable[SafetyTable_point++]<100)&&
		(SafetyReactiveTable[SafetyTable_point] > 0) && (SafetyReactiveTable[SafetyTable_point++]<100)&&
		(SafetyReactiveTable[SafetyTable_point] > 0) && (SafetyReactiveTable[SafetyTable_point++]<100)&&
		(SafetyReactiveTable[SafetyTable_point] > 0) && (SafetyReactiveTable[SafetyTable_point++]<100))
	{
		return 1;
		
	}
	else
	{
		return 1;//0;
	}
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
**********************************************************************/						
Uint16 CheckLVRTValue(void)     	//从通讯板获得并保存电网保护数信?
{
	Uint16 SafetyTable_point = 0;

	if( (SafetyLVRTTable[SafetyTable_point] > 0) && (SafetyLVRTTable[SafetyTable_point++]<100)&&
		(SafetyLVRTTable[SafetyTable_point] > 0) && (SafetyLVRTTable[SafetyTable_point++]<100)&&
		(SafetyLVRTTable[SafetyTable_point] > 0) && (SafetyLVRTTable[SafetyTable_point++]<100)&&
		(SafetyLVRTTable[SafetyTable_point] > 0) && (SafetyLVRTTable[SafetyTable_point++]<100)&&
		(SafetyLVRTTable[SafetyTable_point] > 0) && (SafetyLVRTTable[SafetyTable_point++]<100)&&
		(SafetyLVRTTable[SafetyTable_point] > 0) && (SafetyLVRTTable[SafetyTable_point++]<100)&&
		(SafetyLVRTTable[SafetyTable_point] > 0) && (SafetyLVRTTable[SafetyTable_point++]<100)&&
		(SafetyLVRTTable[SafetyTable_point] > 0) && (SafetyLVRTTable[SafetyTable_point++]<100)&&
		(SafetyLVRTTable[SafetyTable_point] > 0) && (SafetyLVRTTable[SafetyTable_point++]<100)&&
		(SafetyLVRTTable[SafetyTable_point] > 0) && (SafetyLVRTTable[SafetyTable_point++]<100)&&
		(SafetyLVRTTable[SafetyTable_point] > 0) && (SafetyLVRTTable[SafetyTable_point++]<100)&&
		(SafetyLVRTTable[SafetyTable_point] > 0) && (SafetyLVRTTable[SafetyTable_point++]<100)&&
		(SafetyLVRTTable[SafetyTable_point] > 0) && (SafetyLVRTTable[SafetyTable_point++]<100))
	{
		return 1;
		
	}
	else
	{
		return 1;//0;
	}
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
**********************************************************************/						
Uint16 CheckIsoIslandandSoOnValue(void)      //从通讯板获得并保存无功控制电流数据信息
{
	Uint16 SafetyTable_point = 0;

	if( (SafetyIsoIslandandSoOnTable[SafetyTable_point] > 0) && (SafetyIsoIslandandSoOnTable[SafetyTable_point++]<100)&&
		(SafetyIsoIslandandSoOnTable[SafetyTable_point] > 0) && (SafetyIsoIslandandSoOnTable[SafetyTable_point++]<100)&&
		(SafetyIsoIslandandSoOnTable[SafetyTable_point] > 0) && (SafetyIsoIslandandSoOnTable[SafetyTable_point++]<100)&&
		(SafetyIsoIslandandSoOnTable[SafetyTable_point] > 0) && (SafetyIsoIslandandSoOnTable[SafetyTable_point++]<100))
	{
		return 1;
	}
	else
	{
		return 1;//0;
	}
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
**********************************************************************/						
Uint16 CheckFactoryModeValue(void)      //从通讯板获得并保存无功控制电流数据信息
{
		return 1;//0;
}


/**********************************************************************
* FUNCION :  
* PURPOSE :  
**********************************************************************/						
Uint16 CheckCalibrationValue(void)      //从通讯板获得并保存无功控制电流数据信息
{
		return 1;//0;
}

/**********************************************************************
* FUNCION :  Sci_SendData
* PURPOSE :  
**********************************************************************/
void SetSciValueForAnswerLCD(Uint8 state, Uint8 FunCode)
{
	Uint16 i;
	Uint16 point = 0;
	Uint16 checksumtemp = 0;
	SciTxBuff[point++] = 1;//0   副CPU地址
	SciTxBuff[point++] = 2;//0   length 
	SciTxBuff[point++] = FunCode;//0   length 
	SciTxBuff[point++] = state;//0x33 success 0x44	failed  /  0x11 success 0x22 failed
	for(i=0;i<=(SciTxBuff[1] + 1);i++)
	{
		checksumtemp = checksumtemp + ((Uint16)SciTxBuff[i] & 0x00FF);
	}

    SciTxBuff[point++] = (Uint8)(checksumtemp & 0x00FF);
    SciTxBuff[point] = (Uint8)(checksumtemp >>8);
//	CalcChecksum();
}

/**********************************************************************
* FUNCION :  Sci_SendData
* PURPOSE :  
**********************************************************************/
void SetSciValueForResetOrModifySafetyvalue(Uint8 state,Uint8 FunCode)
{
	Uint16 i;
	Uint16 point = 0;
	Uint16 checksumtemp = 0;
	SciTxBuff[point++] = 1;//0   副CPU 地址
	SciTxBuff[point++] = 3;//0   length 
	SciTxBuff[point++] = 0x06;//0x33 success 0x44	failed  /  0x11 success 0x22 failed
	SciTxBuff[point++] = FunCode;//0x33 success 0x44	failed  /  0x11 success 0x22 failed
	SciTxBuff[point++] = state;//0x33 success 0x44	failed  /  0x11 success 0x22 failed

	for(i=0;i<=(SciTxBuff[1] + 1);i++)
	{
		checksumtemp = checksumtemp + ((Uint16)SciTxBuff[i] & 0x00FF);
	}

	SciTxBuff[point++] = (Uint8)(checksumtemp & 0x00FF);
    SciTxBuff[point] = (Uint8)(checksumtemp >>8);
//	CalcChecksum();
}
/**********************************************************************
* FUNCION :  ShakeHandsFirstTime
* PURPOSE :  更新发电量，日期，安规。写入默认国家
**********************************************************************/
void ShakeHandsCheck(void)
{
	Uint16 point1 = 0;
	Uint16 point2 = 0;
	int32 Temp1 = 0;
	int16 Temp2 = 0;
	int16 TempCountryWrongFlag = 0;
	int16 TempCountrySet = 0;
//	Uint8  state = 0;
//	Uint16 InputMode = 0;
//	static int16 TempShakeHandCnt =0;

	if(I2C_Read_InWord(ENERGY_DATE_ADDR,&Todaydate,1))
	{
		s_SysFaultMessage.bit.unrecoverEEPROM_R = 1;                         //从E2PROM读出《今天日期》
	}
	InputMode = SciRxBuff[5];
	TempCountrySet = SciRxBuff[4];
	if((Orig_epromflg != 0x1234)||(Todaydate != SciRxBuff[2])) 	     //its default （0xff）
	{
		temp_zero = 0;
		EnergyTodayOld = 0;
		//read date
		Todaydate = ((Uint16)SciRxBuff[2]) & 0x00FF;					
		//write date to EEPROM, clear energy;
		if( I2C_Write_InWord(ENERGY_DATE_ADDR,&Todaydate,1) ||I2C_Write_InWord((ENERGY_OUTPUT_ADDR + 2 * (Todaydate-1)),&temp_zero,1))
		{
			s_SysFaultMessage.bit.unrecoverEEPROM_W = 1;
		}
	}
	else
	{
		if(I2C_Read_InWord((Uint16)(ENERGY_OUTPUT_ADDR+2 * (Todaydate-1)),&EnergyTodayOld,1))
		{
			s_SysFaultMessage.bit.unrecoverEEPROM_R = 1;                     //从E2PROM读出《今天发电量》
		}
	}
	M_CalcResultRms.i16EnergyTodayNew = EnergyTodayOld;
	if(Orig_epromflg != 0x1234)
	{
		point1 = 0;
		point2 = 0; 
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];			
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];
		CalibrationTable[point1++] = CalibrationDefaultTable[point2++];			
		point1 = 0;
		point2 = 0; 
		if(I2C_Write_InWord(Calibration_ADDR,(Uint16*)&CalibrationDefaultTable,32))
		{
			s_SysFaultMessage.bit.unrecoverEEPROM_W = 1;
		}
	}
	else
	{
		if(I2C_Read_InWord(Calibration_ADDR,CalibrationTable,32))
		{
			s_SysFaultMessage.bit.unrecoverEEPROM_R = 1;
		}
	}
	
	Temp1 = 1536 * ((int32)CalibrationTable[28]);
	Temp2 = (int16)(Temp1>>12);
	ADGain.i16VPV1 = 10 * CalibrationTable[28] + Temp2;//10468;= 10 + 0.375;0.375* 4096 = 1536;
	ADOffsetSum.i16VPV1 = CalibrationTable[29];

	Temp1 = 1536 * ((int32)CalibrationTable[30]);
	Temp2 = (int16)(Temp1>>12);
	ADGain.i16VPV2 = 10 * CalibrationTable[30] + Temp2;//10468;
	ADOffsetSum.i16VPV2 = CalibrationTable[31];

	if(I2C_Read_InWord(SAFETY_COUNTRY_ADDR,&SafetyCountry,1))
	{
		s_SysFaultMessage.bit.unrecoverEEPROM_R = 1;                         //从E2PROM读出《今天日期》
	}

	if((SafetyCountry != TempCountrySet)||(Orig_epromflg != 0x1234))
	{
		if(TempCountrySet<COUNTRYNUM)
		{
			SafetyCountry = TempCountrySet;
			TempCountryWrongFlag = 0;
		}
		else
		{
			SafetyCountry = 0;
			TempCountryWrongFlag = 1;
		}
			point1 = 0;
			point2 = 0;
	
            SafetySTARTTable[point1++] = SafetyDefaultSTARTTable[SafetyCountry][point2++];
			SafetySTARTTable[point1++] = SafetyDefaultSTARTTable[SafetyCountry][point2++];
			SafetySTARTTable[point1++] = SafetyDefaultSTARTTable[SafetyCountry][point2++];
			SafetySTARTTable[point1++] = SafetyDefaultSTARTTable[SafetyCountry][point2++];
			SafetySTARTTable[point1++] = SafetyDefaultSTARTTable[SafetyCountry][point2++];
			SafetySTARTTable[point1++] = SafetyDefaultSTARTTable[SafetyCountry][point2++];
			SafetySTARTTable[point1++] = SafetyDefaultSTARTTable[SafetyCountry][point2++];
			SafetySTARTTable[point1++] = SafetyDefaultSTARTTable[SafetyCountry][point2++];
			point1 = 0;
			point2 = 0;

			SafetyVTable[point1++] = SafetyDefaultVTable[SafetyCountry][point2++];
			SafetyVTable[point1++] = SafetyDefaultVTable[SafetyCountry][point2++];
			SafetyVTable[point1++] = SafetyDefaultVTable[SafetyCountry][point2++];
			SafetyVTable[point1++] = SafetyDefaultVTable[SafetyCountry][point2++];
			SafetyVTable[point1++] = SafetyDefaultVTable[SafetyCountry][point2++];
			SafetyVTable[point1++] = SafetyDefaultVTable[SafetyCountry][point2++];
			SafetyVTable[point1++] = SafetyDefaultVTable[SafetyCountry][point2++];
			SafetyVTable[point1++] = SafetyDefaultVTable[SafetyCountry][point2++];
			SafetyVTable[point1++] = SafetyDefaultVTable[SafetyCountry][point2++];
			SafetyVTable[point1++] = SafetyDefaultVTable[SafetyCountry][point2++];
			point1 = 0;
			point2 = 0;

			SafetyFreTable[point1++] = SafetyDefaultFreTable[SafetyCountry][point2++];
			SafetyFreTable[point1++] = SafetyDefaultFreTable[SafetyCountry][point2++];
			SafetyFreTable[point1++] = SafetyDefaultFreTable[SafetyCountry][point2++];
			SafetyFreTable[point1++] = SafetyDefaultFreTable[SafetyCountry][point2++];
			SafetyFreTable[point1++] = SafetyDefaultFreTable[SafetyCountry][point2++];
			SafetyFreTable[point1++] = SafetyDefaultFreTable[SafetyCountry][point2++];
			SafetyFreTable[point1++] = SafetyDefaultFreTable[SafetyCountry][point2++];
			SafetyFreTable[point1++] = SafetyDefaultFreTable[SafetyCountry][point2++];
			SafetyFreTable[point1++] = SafetyDefaultFreTable[SafetyCountry][point2++];
			point1 = 0;
			point2 = 0;		

			SafetyDCITable[point1++] =  SafetyDefaultDCITable[SafetyCountry][point2++];
			SafetyDCITable[point1++] =  SafetyDefaultDCITable[SafetyCountry][point2++];
			SafetyDCITable[point1++] =  SafetyDefaultDCITable[SafetyCountry][point2++];
			SafetyDCITable[point1++] =  SafetyDefaultDCITable[SafetyCountry][point2++];
			SafetyDCITable[point1++] =  SafetyDefaultDCITable[SafetyCountry][point2++];
			SafetyDCITable[point1++] =  SafetyDefaultDCITable[SafetyCountry][point2++];
			point1 = 0;
			point2 = 0;	
			
			SafetyRemotPowerControlTable[point1++] =  SafetyDefaultRemotPowerControlTable[SafetyCountry][point2++];
			SafetyRemotPowerControlTable[point1++] =  SafetyDefaultRemotPowerControlTable[SafetyCountry][point2++];
			SafetyRemotPowerControlTable[point1++] =  SafetyDefaultRemotPowerControlTable[SafetyCountry][point2++];
			point1 = 0;
			point2 = 0;	
			
			SafetyFreqPowerControlTable[point1++] =  SafetyDefaultFreqPowerControlTable[SafetyCountry][point2++];
			SafetyFreqPowerControlTable[point1++] =  SafetyDefaultFreqPowerControlTable[SafetyCountry][point2++];
			SafetyFreqPowerControlTable[point1++] =  SafetyDefaultFreqPowerControlTable[SafetyCountry][point2++];
			SafetyFreqPowerControlTable[point1++] =  SafetyDefaultFreqPowerControlTable[SafetyCountry][point2++];
			SafetyFreqPowerControlTable[point1++] =  SafetyDefaultFreqPowerControlTable[SafetyCountry][point2++];
			SafetyFreqPowerControlTable[point1++] =  SafetyDefaultFreqPowerControlTable[SafetyCountry][point2++];
			SafetyFreqPowerControlTable[point1++] =  SafetyDefaultFreqPowerControlTable[SafetyCountry][point2++];
			point1 = 0;
			point2 = 0;	

			SafetyReactiveTable[point1++] =  SafetyDefaultReactiveTable[SafetyCountry][point2++];
			SafetyReactiveTable[point1++] =  SafetyDefaultReactiveTable[SafetyCountry][point2++];
			SafetyReactiveTable[point1++] =  SafetyDefaultReactiveTable[SafetyCountry][point2++];
			SafetyReactiveTable[point1++] =  SafetyDefaultReactiveTable[SafetyCountry][point2++];
			SafetyReactiveTable[point1++] =  SafetyDefaultReactiveTable[SafetyCountry][point2++];
			SafetyReactiveTable[point1++] =  SafetyDefaultReactiveTable[SafetyCountry][point2++];
			SafetyReactiveTable[point1++] =  SafetyDefaultReactiveTable[SafetyCountry][point2++];	
			SafetyReactiveTable[point1++] =  SafetyDefaultReactiveTable[SafetyCountry][point2++];
			SafetyReactiveTable[point1++] =  SafetyDefaultReactiveTable[SafetyCountry][point2++];
			SafetyReactiveTable[point1++] =  SafetyDefaultReactiveTable[SafetyCountry][point2++];
			SafetyReactiveTable[point1++] =  SafetyDefaultReactiveTable[SafetyCountry][point2++];
			SafetyReactiveTable[point1++] =  SafetyDefaultReactiveTable[SafetyCountry][point2++];
			SafetyReactiveTable[point1++] =  SafetyDefaultReactiveTable[SafetyCountry][point2++];
			SafetyReactiveTable[point1++] =  SafetyDefaultReactiveTable[SafetyCountry][point2++];	
			SafetyReactiveTable[point1++] =  SafetyDefaultReactiveTable[SafetyCountry][point2++];	
			SafetyReactiveTable[point1++] =  SafetyDefaultReactiveTable[SafetyCountry][point2++];
			SafetyReactiveTable[point1++] =  SafetyDefaultReactiveTable[SafetyCountry][point2++];
			SafetyReactiveTable[point1++] =  SafetyDefaultReactiveTable[SafetyCountry][point2++];	
			SafetyReactiveTable[point1++] =  SafetyDefaultReactiveTable[SafetyCountry][point2++];
			SafetyReactiveTable[point1++] =  SafetyDefaultReactiveTable[SafetyCountry][point2++];	
			SafetyReactiveTable[point1++] =  SafetyDefaultReactiveTable[SafetyCountry][point2++];
			point1 = 0;
			point2 = 0;	

			SafetyLVRTTable[point1++] = SafetyDefaultLVRTTable[SafetyCountry][point2++];
			SafetyLVRTTable[point1++] = SafetyDefaultLVRTTable[SafetyCountry][point2++];
			SafetyLVRTTable[point1++] = SafetyDefaultLVRTTable[SafetyCountry][point2++];
			SafetyLVRTTable[point1++] = SafetyDefaultLVRTTable[SafetyCountry][point2++];
			SafetyLVRTTable[point1++] = SafetyDefaultLVRTTable[SafetyCountry][point2++];
			SafetyLVRTTable[point1++] = SafetyDefaultLVRTTable[SafetyCountry][point2++];
			SafetyLVRTTable[point1++] = SafetyDefaultLVRTTable[SafetyCountry][point2++];
			SafetyLVRTTable[point1++] = SafetyDefaultLVRTTable[SafetyCountry][point2++];
			SafetyLVRTTable[point1++] = SafetyDefaultLVRTTable[SafetyCountry][point2++];
			SafetyLVRTTable[point1++] = SafetyDefaultLVRTTable[SafetyCountry][point2++];
			SafetyLVRTTable[point1++] = SafetyDefaultLVRTTable[SafetyCountry][point2++];
			SafetyLVRTTable[point1++] = SafetyDefaultLVRTTable[SafetyCountry][point2++];
			SafetyLVRTTable[point1++] = SafetyDefaultLVRTTable[SafetyCountry][point2++];
			point1 = 0;
			point2 = 0;		
										
			SafetyIsoIslandandSoOnTable[point1++] = SafetyDefaultIsoIslandandSoOnTable[SafetyCountry][point2++];
			SafetyIsoIslandandSoOnTable[point1++] = SafetyDefaultIsoIslandandSoOnTable[SafetyCountry][point2++];
			SafetyIsoIslandandSoOnTable[point1++] = SafetyDefaultIsoIslandandSoOnTable[SafetyCountry][point2++];
			SafetyIsoIslandandSoOnTable[point1++] = SafetyDefaultIsoIslandandSoOnTable[SafetyCountry][point2++];
			point1 = 0;
			point2 = 0; 
										
			FactoryMode = FactoryModeDefaultValue;


		if(writedefaultsafety(SafetyCountry))
		{
			s_SysFaultMessage.bit.unrecoverEEPROM_W = 1; 
		}
		else
		{
            if(I2C_Write_InWord(SAFETY_COUNTRY_ADDR, &SafetyCountry,1))
		    {
			    s_SysFaultMessage.bit.unrecoverEEPROM_W = 1;
		    }
		
		}
	}
	else
	{
		readsafetyvalue();
		TempCountryWrongFlag = 0;
	}

	if((0 == s_SysFaultMessage.bit.unrecoverEEPROM_R)&&(Orig_epromflg != 0x1234))
	{
		Orig_epromflg = 0x1234;    			//change to <not default>
		if(I2C_Write_InWord(BOOTSTATE_ADDR,&Orig_epromflg,1))//I2C_Read_InByte的主要作用是移动数据，但能返回数值。                                                   //以表示执行时出现的状况，
		{
			s_SysFaultMessage.bit.unrecoverEEPROM_R = 1;                  //返回常量I2C_W_R_OK==0则表示一切正常，否则说明有问题。
		}
	}

	if(0 == s_SysFaultMessage.bit.unrecoverEEPROM_R)
	{
		SetSciValueForCommissionEndCheck();//改为传递尾55表示成功，66表示失败
		if(0 == TempCountryWrongFlag)
		{
			s_StateCheck.Word.byte4 = 0xFF;
			s_StateCheck.Word.byte5 = 0x3F;
			s_StateCheck.bit.HandShakeOver = 1;
		}
	}
} 
/**********************************************************************
* FUNCION :  Sci_SendData
* PURPOSE :  
**********************************************************************/
void SetSciValueForCommissionEndCheck(void)
{
	Uint16 i;
	Uint16 point = 0;
	Uint16 checksumtemp = 0;
//	Uint16 powerleveltemp = 0;

	SciTxBuff[point++] = 0x01;             // 01 slave DSP 
	SciTxBuff[point++] = 11; // 数据个数 

	SciTxBuff[point++] = 0x10;//

	SciTxBuff[point++] = PowerLevelType;
	SciTxBuff[point++] = MASTER_MAIN_VERSION;
	SciTxBuff[point++] = MASTER_MAIN_VERSION0;
	SciTxBuff[point++] = MASTER_SECONDARY_VERSION1;
	SciTxBuff[point++] = MASTER_SECONDARY_VERSION2;

	SciTxBuff[point++] = SLAVE_MAIN_VERSION;
	SciTxBuff[point++] = SLAVE_MAIN_VERSION0;
	SciTxBuff[point++] = SLAVE_SECONDARY_VERSION1;
	SciTxBuff[point++] = SLAVE_SECONDARY_VERSION2;
	SciTxBuff[point++] = 0x55;



	for(i=0;i<=(SciTxBuff[1] + 1);i++)
	{
		checksumtemp = checksumtemp + ((Uint16)SciTxBuff[i] & 0x00FF);
	}
    
	SciTxBuff[point++] = (Uint8)(checksumtemp & 0x00FF);
    SciTxBuff[point]   = (Uint8)(checksumtemp >>8);	
}

void SoftwareOnlineUpgrade(void)
{
    Uint16 Status;
	Uint16 point_0X70 = 0;
	Uint16 count_0X70 = 0;
	Uint16 checksum_0X70 = 0;	
    //Resetflg = 0xff;
    PrepareForWait();
//	DELAY_US(40000);
    s_SysCurrentState = PermanentState; //副cpu 停机
    g_Sys_Current_State = PermanentState; //副cpu 停机	
    Status = CsmUnlock();
    if(Status != 1)
    {
       Example_Error(Status);
    }

    EALLOW;
       Flash_CPUScaleFactor = SCALE_FACTOR;
    EDIS;

    EALLOW;
        Flash_CallbackPtr = &MyCallbackFunction;
    EDIS;

    MyCallbackCounter = 0;  
    DELAY_US(40000);
    DINT;

    Example_CallFlashAPI(); //擦除B 区

	SciTxBuff[point_0X70++] = 1;	
	SciTxBuff[point_0X70++] = 10;
	SciTxBuff[point_0X70++] = 0x90;

	SciTxBuff[point_0X70++] = 1;

	SciTxBuff[point_0X70++] = 2;
	SciTxBuff[point_0X70++] = 3;
	SciTxBuff[point_0X70++] = 4;
	SciTxBuff[point_0X70++] = 5;

	SciTxBuff[point_0X70++] = 6;
	SciTxBuff[point_0X70++] = 7;
	SciTxBuff[point_0X70++] = 8;
	SciTxBuff[point_0X70++] = 9;


	for(count_0X70 = 0;	count_0X70<=SciTxBuff[1] + 1; count_0X70++)  //计算校验码时包含了SciTxBuff[0]
	{
		checksum_0X70 = checksum_0X70 + ((Uint16)SciTxBuff[count_0X70] & 0x00ff);
	}
	
    SciTxBuff[point_0X70++] = (Uint8)(checksum_0X70 & 0xff);
    SciTxBuff[point_0X70] =  (Uint8)(checksum_0X70>> 8);
}
//===========================================================================
// End of file. dwsed
//===========================================================================
