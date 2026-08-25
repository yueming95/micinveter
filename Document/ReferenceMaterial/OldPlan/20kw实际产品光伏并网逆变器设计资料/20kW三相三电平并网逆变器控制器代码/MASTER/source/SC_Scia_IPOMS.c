/***********************************************************************
 *    FILENAME : SC_Scia_IPOMS.c
 *
 *    PURPOSE  : Interface of software oscilloscope, using SCI-A of 28335
 *               and IPOMS.
 *    HISTORY  :
 *           DATE          VERSION      AUTHOR        NOTE
 ************************************************************************/
#include "SC_master.h"                                    

#ifdef _IPOMS_

#pragma CODE_SECTION(IPOMS_SnatchGraph, "ControlLoopInRAM") 

typedef int (*pFunc1)(void);

/******************************variable definition******************************/
Uint8	u8IPOMS_UserDataBuf0[250];
Uint8	u8IPOMS_CommandBuffer0[50];
Uint8	*pIPOMS_CommandIn0;
Uint16	u16IPOMS_CommandLength = 0;
extern float32 f32Temp_iq;
extern float32 f32tanphi_calc;

extern int16 temp5_1;
extern int16 temp5_2;
extern int16 temp5_3;
extern int16 temp5_4;
extern int16 temp5_5;
extern int16 temp5_6;
extern int16 temp5_7;
extern int16 temp5_8;
extern int16 temp5_9;
extern int16 temp5_10;
extern int16 temp5_11;
extern int16 temp5_12;

extern int16 temp6_1;
extern int16 temp6_2;
extern int16 temp6_3;
extern int16 temp6_4;
extern int16 temp6_5;
extern int16 temp6_6;
extern int16 temp6_7;
extern int16 temp6_8;
extern int16 temp6_9;
extern int16 temp6_10;
extern int16 temp6_11;
extern int16 temp6_12;

extern int16 temp7_1;
extern int16 temp7_2;
extern int16 temp7_3;
extern int16 temp7_4;
extern int16 temp7_5;
extern int16 temp7_6;
extern int16 temp7_7;
extern int16 temp7_8;
extern int16 temp7_9;
extern int16 temp7_10;
extern int16 temp7_11;
extern int16 temp7_12;

extern int16 temp8_1;
extern int16 temp8_2;
extern int16 temp8_3;
extern int16 temp8_4;
extern int16 temp8_5;
extern int16 temp8_6;
extern int16 temp8_7;
extern int16 temp8_8;
extern int16 temp8_9;
extern int16 temp8_10;
extern int16 temp8_11;
extern int16 temp8_12;

extern int16 temp9_1;
extern int16 temp9_2;
extern int16 temp9_3;
extern int16 temp9_4;
extern int16 temp9_5;
extern int16 temp9_6;
extern int16 temp9_7;
extern int16 temp9_8;
extern int16 temp9_9;
extern int16 temp9_10;
extern int16 temp9_11;
extern int16 temp9_12;

extern int16 temp10_1;
extern int16 temp10_2;
extern int16 temp10_3;
extern int16 temp10_4;
extern int16 temp10_5;
extern int16 temp10_6;
extern int16 temp10_7;
extern int16 temp10_8;
extern int16 temp10_9;
extern int16 temp10_10;
extern int16 temp10_11;
extern int16 temp10_12;

extern int16 temp11_1;
extern int16 temp11_2;
extern int16 temp11_3;
extern int16 temp11_4;
extern int16 temp11_5;
extern int16 temp11_6;
extern int16 temp11_7;
extern int16 temp11_8;
extern int16 temp11_9;
extern int16 temp11_10;
extern int16 temp11_11;
extern int16 temp11_12;

extern int16 temp12_1;
extern int16 temp12_2;
extern int16 temp12_3;
extern int16 temp12_4;
extern int16 temp12_5;
extern int16 temp12_6;
extern int16 temp12_7;
extern int16 temp12_8;
extern int16 temp12_9;
extern int16 temp12_10;
extern int16 temp12_11;
extern int16 temp12_12;

extern int16 temp13_1;
extern int16 temp13_2;
extern int16 temp13_3;
extern int16 temp13_4;
extern int16 temp13_5;
extern int16 temp13_6;
extern int16 temp13_7;
extern int16 temp13_8;
extern int16 temp13_9;
extern int16 temp13_10;
extern int16 temp13_11;
extern int16 temp13_12;

extern int16 temp14_1;
extern int16 temp14_2;
extern int16 temp14_3;
extern int16 temp14_4;
extern int16 temp14_5;
extern int16 temp14_6;
extern int16 temp14_7;
extern int16 temp14_8;
extern int16 temp14_9;
extern int16 temp14_10;
extern int16 temp14_11;
extern int16 temp14_12;

extern int16 temp15_1;
extern int16 temp15_2;
extern int16 temp15_3;
extern int16 temp15_4;
extern int16 temp15_5;
extern int16 temp15_6;
extern int16 temp15_7;
extern int16 temp15_8;
extern int16 temp15_9;
extern int16 temp15_10;
extern int16 temp15_11;
extern int16 temp15_12;

extern int16 temp16_1;
extern int16 temp16_2;
extern int16 temp16_3;
extern int16 temp16_4;
extern int16 temp16_5;
extern int16 temp16_6;
extern int16 temp16_7;
extern int16 temp16_8;
extern int16 temp16_9;
extern int16 temp16_10;
extern int16 temp16_11;
extern int16 temp16_12;
// snatch graph part
Uint8	u8IPOMS_SnatchGraphEnable = 1;
Uint16	u16IPOMS_Interval = 0;
Uint16	u16IPOMS_Interval1 = 0;
Uint8	u8IPOMS_wTrigger = 0;
Uint8	u8IPOMS_wTriggerSource = 0;

Uint16	u16IPOMS_SnatchDataCnt = 0;
Uint16	u16IPOMS_SaveDataCnt = 0;
Uint16	u16IPOMS_TransmitCnt = 0;

Uint16	u16IPOMS_CompareVal = 0;
Uint8	u8IPOMS_Sign = 0;

Uint8	u8IPOMS_DataKind[4] = {0,0,0,0};

Uint16	u16IPOMS_GraphDataBuff[4][500];
Uint16	u16IPOMS_TransmitDataBuff[500];

/******************************variable definition******************************/

/******************************fuciton list******************************/
void IPOMS_Parsing(void);
void IPOMS_Q1Command(void);
void IPOMS_Q3Command(void);
void IPOMS_QDCommand(void);

int16 IPOMS_GraphicView_0(void);
int16 IPOMS_GraphicView_1(void);
int16 IPOMS_GraphicView_2(void);
int16 IPOMS_GraphicView_3(void);
int16 IPOMS_GraphicView_4(void);
int16 IPOMS_GraphicView_5(void);
int16 IPOMS_GraphicView_6(void);
int16 IPOMS_GraphicView_7(void);
int16 IPOMS_GraphicView_8(void);
int16 IPOMS_GraphicView_9(void);
int16 IPOMS_GraphicView_10(void);
int16 IPOMS_GraphicView_11(void);
int16 IPOMS_GraphicView_12(void);
int16 IPOMS_GraphicView_13(void);
int16 IPOMS_GraphicView_14(void);
int16 IPOMS_GraphicView_15(void);
int16 IPOMS_GraphicView_16(void);
int16 IPOMS_GraphicView_17(void);
int16 IPOMS_GraphicView_18(void);
int16 IPOMS_GraphicView_19(void);
int16 IPOMS_GraphicView_20(void);
int16 IPOMS_GraphicView_21(void);
int16 IPOMS_GraphicView_22(void);
int16 IPOMS_GraphicView_23(void);
int16 IPOMS_GraphicView_24(void);
int16 IPOMS_GraphicView_25(void);

Uint8 sbNumToAscii(Uint16 u16Number, int8 i8Exponent, Uint8 *pbBuffer);
void Sci_IPOMS_WriteBinary(Uint16 *pstart, Uint16 u16Length);

/******************************fuciton list******************************/

pFunc1 GetDataSubArray[30] =
{
	IPOMS_GraphicView_0,	// Note: Don't set "Channel x ID" to 0 in "Graphic View" in IPOMS. 0 is invalid.

	IPOMS_GraphicView_1,	// 1- is the number set to "Channelx ID"
	IPOMS_GraphicView_2,	// 2
	IPOMS_GraphicView_3,	// 3
	IPOMS_GraphicView_4,	// 4
	IPOMS_GraphicView_5,	// 5
	IPOMS_GraphicView_6,	// 6
	IPOMS_GraphicView_7,	// 7
	IPOMS_GraphicView_8,	// 8
	IPOMS_GraphicView_9,	// 9
	IPOMS_GraphicView_10,	// 10
	IPOMS_GraphicView_11,	// 11
	IPOMS_GraphicView_12,	// 11
	IPOMS_GraphicView_13,	// 11
	IPOMS_GraphicView_14,	// 11
	IPOMS_GraphicView_15,	// 11
	IPOMS_GraphicView_16,	// 11
	IPOMS_GraphicView_17,	// 11
	IPOMS_GraphicView_18,	// 11
	IPOMS_GraphicView_19,	// 11
	IPOMS_GraphicView_20,	// 11
	IPOMS_GraphicView_21,	// 11
	IPOMS_GraphicView_22,	// 11
	IPOMS_GraphicView_23,	// 11
	IPOMS_GraphicView_24,	// 11
	IPOMS_GraphicView_25,	// 11

	// U can add more functions like "IPOMS_GraphicView_x", (x < 30)
} ;

/**********************************************************************
* FUNCION :  TSK_F_SCIa_IPOMS
* PURPOSE :  Software OSC for debug
* CALLED BY:
**********************************************************************/
void SCIa_IPOMS(void)
{ 
	Uint8 u8IPOMS_temp;

	while(1)
	{
        u8IPOMS_temp = SciRead(ID_SCI_IPOMS, pIPOMS_CommandIn0);

        if(SCI_RX_EMPTY == u8IPOMS_temp)
		{
			break;
		}
		if(u16IPOMS_CommandLength >= IPOMS_MAX_COMMAND_LENGTH)
		{
			pIPOMS_CommandIn0 = u8IPOMS_CommandBuffer0;
			u16IPOMS_CommandLength = 0;
		}
		else if(0x5A == (*pIPOMS_CommandIn0))
		{
		//	FlashProgramming_TSK();
		}
		else if((IPOMS_CHAR_ENTER == (*pIPOMS_CommandIn0)) || (0x0A == (*pIPOMS_CommandIn0)))  //Deal with command with 0x0a
		{
		    IPOMS_Parsing();
			pIPOMS_CommandIn0 = u8IPOMS_CommandBuffer0;
			u16IPOMS_CommandLength = 0;
		}
		else
		{
			u16IPOMS_CommandLength++;
			pIPOMS_CommandIn0++;
      	} 
	}
}

/*=============================================================================*
 * FUNCTION: IPOMS_Parsing(void)
 * PURPOSE : IPOMS Command Parse
 * INPUT: 
 *     void  // TRUE:disable the dog£»FALSE:enable the dog
 *
 * RETURN: 
 *     void
 *
 * CALLS: 
 *     //IPOMS_Q1Command();
 *     //IPOMS_Q3Command();
 *     //IPOMS_QDCommand();
 *     
 *
 * CALLED BY: 
 *     
 * 
 *============================================================================*/                
void IPOMS_Parsing(void)
{
	switch(u8IPOMS_CommandBuffer0[0])
	{
    case 'Q':
        {   
            if('1' == u8IPOMS_CommandBuffer0[1])
            {
                IPOMS_Q1Command();
            }
            if('3' == u8IPOMS_CommandBuffer0[1])
            {
                IPOMS_Q3Command();
            }
            if('D' == u8IPOMS_CommandBuffer0[1])
            {
                IPOMS_QDCommand();
            }
        }
        break;

    default:

        break;
	}
	
}
/*=============================================================================*
 * FUNCTION: IPOMS_Q1Command(void)
 * PURPOSE : Send Data to IPOMS For Q1 Command
 * INPUT: 
 *     void  // TRUE:disable the dog£»FALSE:enable the dog
 *
 * RETURN: 
 *     void
 *
 * CALLS:
 *     
 *
 * CALLED BY: 
 *   //  INT32 IPOMS_Parsing() 
 * 
 *============================================================================*/
void IPOMS_Q1Command(void)                
{
	Uint8	bStrLen;
	Uint8	bStrLen1;
	Uint8   *pDataBuf;
	
	bStrLen1 = 0;
	bStrLen = 0;

	bStrLen = sbNumToAscii(g_InverterType/*variable you want to watch*/, 0, u8IPOMS_UserDataBuf0); //0   g_Sys_Current_State
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_Rated.f32OutputPower, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_u16SafetyCountDown, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SafetyConnect.u16ConnectTime, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SafetyConnect.u16ReconnectTime, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SafetyReg.u16SafeCountry/*variable you want to watch*/, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_MPPTState.bit.InputMode/*variable you want to watch*/, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SafetyReg.CoMMDate/*variable you want to watch*/, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SysCurrentState/*variable you want to watch*/, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SysCurrentStateSlave/*variable you want to watch*/, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_BusRefCal.f32BusRef * 10/*variable you want to watch*/, -1, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_Mppt1Calc.f32PVWattCurrent/*variable you want to watch*/, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_Mppt1Calc.f32PvVoltRef * 10/*variable you want to watch*/, -1, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalcResult.Ave.f32VPV1 * 10, -1, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_Mppt2Calc.f32PVWattCurrent/*variable you want to watch*/, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_Mppt2Calc.f32PvVoltRef * 10/*variable you want to watch*/, -1, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalcResult.Ave.f32VPV2 * 10, -1, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalcResult.Ave.f32FGrid * 100, -2, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalcResult.Rms.f32VGridR * 10, -1, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalcResult.Rms.f32VGridS * 10, -1, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalcResult.Rms.f32VGridT * 10, -1, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalcResult.Ave.f32VbusP * 10, -1, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalcResult.Ave.f32VbusN * 10, -1, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalcResult.Ave.f32Vbus * 10, -1, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii((g_CalcResult.Ave.f32IPV1 * 100+1000), -2, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii((g_CalcResult.Ave.f32IPV2 * 100+1000), -2, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii((g_CalcResult.Ave.f32DciR + 1000), 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii((g_CalcResult.Ave.f32DciS + 1000), 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii((g_CalcResult.Ave.f32DciT + 1000), 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii((g_CalcResult.Ave.f32GFCI + 1000), 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii((g_CalcResult.Rms.f32GFCI), 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii((g_CalcResult.Rms.f32IGridR * 100), -2, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii((g_CalcResult.Rms.f32IGridS * 100), -2, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii((g_CalcResult.Rms.f32IGridT * 100), -2, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii((g_BusPIDCal.f32IdRef * 100), -2, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii((g_BusPIDCal.f32IdRefMax * 100), -2, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalcResult.Power.f32Input1Watt, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalcResult.Power.f32Input2Watt, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalcResult.Power.f32OutputWatt, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalcResult.Ave.f32IGridR * 1000 + 1000, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalcResult.Ave.f32IGridS * 1000 + 1000, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalcResult.Ave.f32IGridT * 1000 + 1000, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalcResult.PowerDisplay.f32OutputEnergy, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_MpptCalc.f32PVVoltLimitPoint, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_MpptCalc.f32PowerShiftVolt, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_MpptCalc.f32PvVoltRef, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_MpptCalc.u16OutputPowerLimitFlag, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(111, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SysFaultMessage.Word.byte0, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SysFaultMessage.Word.byte1, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SysFaultMessage.Word.byte2, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;
	
	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SysFaultMessage.Word.byte3, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SysFaultMessage.Word.byte4, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SysFaultMessage.Word.byte5, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;
	
	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SysFaultMessage.Word.byte6, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SysFaultMessage.Word.byte7, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SysFaultMessage.Word.unrecover0, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;
	
	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SysFaultMessage.Word.unrecover1, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SlaveSysFaultMessage.Word.byte0, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SlaveSysFaultMessage.Word.byte1, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SlaveSysFaultMessage.Word.byte2, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;
	
	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SlaveSysFaultMessage.Word.byte3, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SlaveSysFaultMessage.Word.byte4, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SlaveSysFaultMessage.Word.byte5, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;
	
	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SlaveSysFaultMessage.Word.byte6, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SlaveSysFaultMessage.Word.byte7, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SlaveSysFaultMessage.Word.unrecover0, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;
	
	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SlaveSysFaultMessage.Word.unrecover1, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;
	
	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(222, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_StateCheck.Word.byte0, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_StateCheck.Word.byte1, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_StateCheck.Word.byte2, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_StateCheck.Word.byte3, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_StateCheck.Word.byte4, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_StateCheck.Word.byte5, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;	

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_StateCheck.Word.byte6, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_StateCheck.Word.byte7, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;	


	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SlaveStateCheck.Word.byte0, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(333, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_i16_Temperature_Boost, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_i16_Temperature_Amb, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;


	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_i16_Temperature_Inv, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(temp6_1, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(temp6_2, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(temp6_3, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(temp6_4, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(temp6_5, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(444, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SafetyConnect.f32VGridMaxLimitBack * 10, -1, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SafetyConnect.f32VGridMinLimitBack * 10, -1, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SafetyConnect.f32FGridMaxLimitBack * 100, -2, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SafetyConnect.f32FGridMinLimitBack * 100, -2, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SafetyConnect.f32PupSlop * 100, -2, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(555, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SaftyConfig.Word.VoltProCon, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SafetyVolt.f32VGridMax1Limit * 10, -1, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SafetyVolt.u16VGridMax1ProtectTime, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SafetyVolt.f32VGridMin1Limit * 10, -1, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;


	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SafetyVolt.u16VGridMin1ProtectTime, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SafetyVolt.f32VGridMax2Limit * 10, -1, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SafetyVolt.u16VGridMax2ProtectTime, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SafetyVolt.f32VGridMin2Limit * 10, -1, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;


	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SafetyVolt.u16VGridMin2ProtectTime, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(666, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SaftyConfig.Word.FreProCon, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SafetyFreq.f32FGridMax1Limit * 100, -2, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SafetyFreq.u16FGridMax1ProtectTime, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SafetyFreq.f32FGridMin1Limit * 100, -2, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SafetyFreq.u16FGridMin1ProtectTime, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SafetyFreq.f32FGridMax2Limit * 100, -2, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SafetyFreq.u16FGridMax2ProtectTime, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SafetyFreq.f32FGridMin2Limit * 100, -2, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SafetyFreq.u16FGridMin2ProtectTime, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;


	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(111, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;
/*
	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_PActiveLimit.f32SmaxVo, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_PActiveLimit.f32SmaxTemperature, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_PActiveLimit.f32SMaxLimit, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_PActiveLimit.f32PReactiveCommand, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_PActiveLimit.f32PActiveFreq, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_PActiveLimit.f32PActiveSoftStart, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_PActiveLimit.f32PActiveCommand, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_PActiveLimit.f32PActiveMaxLimit, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_PActiveLimit.f32Pinput1, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_PActiveLimit.f32Pinput2, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_PActiveLimit.f32PinputAll, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_BusPIDCal.f32IdRefMax * 10, -1, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_MPPTState.Word.byte2, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SafetyReg.FactoryCurrentLimit * 100, -2, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;
*/
	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(temp16_2, 0, pDataBuf); //GridR
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(temp16_3, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(temp16_4, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(temp16_5, 0, pDataBuf); //InvR
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(temp16_6, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(temp16_7, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;
/*
	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(temp16_8, 0, pDataBuf); //DCIR
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(temp16_9, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(temp16_10, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(temp16_11, 0, pDataBuf); //GFCI_Slave
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(temp16_12, 0, pDataBuf); //Fre_Slave
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;


	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(777, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SaftyConfig.Word.DCIProCon, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SafetyDCI.f32DciMax1Limit, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SafetyDCI.u16DciMax1ProtectTime , 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SafetyDCI.f32DciMax2Limit, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SafetyDCI.u16DciMax2ProtectTime , 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SafetyDCI.f32DciTestInjection * 1000, -3, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalcResult.Ave.f32IGridR * 1000, -3, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalcResult.Ave.f32IGridS * 1000, -3, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalcResult.Ave.f32IGridT * 1000, -3, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;


	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(888, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SaftyConfig.Word.PowerRemoteCon, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SafetyActive.f32PLimitCommand * 1000, -3, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SafetyActive.RemoteOnOff , 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(999, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;
*/
	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SaftyConfig.Word.PowerFreCon, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_GridManager.PFreq.f32FreqPoint * 100, -2, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_GridManager.PFreq.f32Slop * 100, -2, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_GridManager.PFreq.f32FOback * 100, -2, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_GridManager.PFreq.f32FUback * 100, -2, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32; 

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_GridManager.PFreq.u16WaitTime, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_GridManager.PFreq.f32BackSpeed * 100, -2, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32; 

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_GridManager.PFreq.u16ReloadFlag, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32; 

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_GridManager.PFreq.u16PowerFixedPointFlag, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32; 

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_GridManager.PFreq.u16WaitReloadFlag, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32; 

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_GridManager.PFreq.u16FPLimit, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_GridManager.PFreq.u16PowerM, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32; 


	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(111, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SaftyConfig.Word.ReactiveCon, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;


	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_PActiveLimit.f32SMaxLimit, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_PActiveLimit.f32PReactiveCommand, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii((1 + g_GridManager.InvQ.f32Cosphi) * 1000, -3, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii((1000 * (5 + g_GridManager.InvQ.f32Tanphi)), -3, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii((10000+g_GridManager.InvQ.f32Qvar), 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii((1 + g_GridManager.InvQ.f32Cosphi1) * 1000, -3, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_GridManager.InvQ.f32Pwatt1, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii((1 + g_GridManager.InvQ.f32Cosphi2) * 1000, -3, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;	

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_GridManager.InvQ.f32Pwatt2, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii((1 + g_GridManager.InvQ.f32Cosphi3) * 1000, -3, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_GridManager.InvQ.f32Pwatt3, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;	

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii((1 + g_GridManager.InvQ.f32Cosphi4) * 1000, -3, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_GridManager.InvQ.f32Pwatt4, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;	

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_GridManager.InvQ.f32LockinV, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;	

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_GridManager.InvQ.f32LockoutV, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_GridManager.InvQ.f32U1i, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;			

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_GridManager.InvQ.f32U1s, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;	

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_GridManager.InvQ.f32U2i, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_GridManager.InvQ.f32U2s, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_GridManager.InvQ.f32LockinP, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;	

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_GridManager.InvQ.f32LockoutP, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_GridManager.InvQ.f32QvarMax, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_GridManager.InvQ.f32QTime, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;
/*
	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(222, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SaftyConfig.Word.LVRTCon, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_GridManager.Lvrt.f32VLvrt, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_GridManager.Lvrt.u16Vpoint1, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_GridManager.Lvrt.u16Tpoint1, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_GridManager.Lvrt.u16Vpoint2, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_GridManager.Lvrt.u16Tpoint2, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_GridManager.Lvrt.u16Vpoint3, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_GridManager.Lvrt.u16Tpoint3, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_GridManager.Lvrt.u16Vpoint4, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_GridManager.Lvrt.u16Tpoint4, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_GridManager.Lvrt.u16K, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_GridManager.Lvrt.u16Tback, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_GridManager.Lvrt.u16PLvrtback, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;				

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(333, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SaftyConfig.Word.AICon, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SaftyConfig.Word.GFCICon, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SaftyConfig.Word.ISOCon, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;				

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_SafetyReg.u16Riso, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(444, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalibrationPara.f32KVPV1 * 1000, -3, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalibrationPara.f32OFFSETVPV1 * 10, -1, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalibrationPara.f32KIPV1 * 1000, -3, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;				

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalibrationPara.f32OFFSETIPV1 * 100, -2, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalibrationPara.f32KPowerPV1 * 1000, -3, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;	

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalibrationPara.f32OFFSETPowerPV1, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalibrationPara.f32KVPV2 * 1000, -3, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalibrationPara.f32OFFSETVPV2 * 10, -1, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalibrationPara.f32KIPV2 * 1000, -3, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;				

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalibrationPara.f32OFFSETIPV2 * 100, -2, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalibrationPara.f32KPowerPV2 * 1000, -3, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;	

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalibrationPara.f32OFFSETPowerPV2, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalibrationPara.f32KVBUS * 1000, -3, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;	

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalibrationPara.f32OFFSETVBUS * 10, -1, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalibrationPara.f32KPowerOutput * 1000, -3, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;	

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalibrationPara.f32OFFSETPowerOutput, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(555, 0, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalibrationPara.f32KGridVR * 1000, -3, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalibrationPara.f32OFFSETGridVR * 10, -1, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalibrationPara.f32KGridIR * 1000, -3, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalibrationPara.f32OFFSETGridIR * 100, -2, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;				

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalibrationPara.f32KGridVS * 1000, -3, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalibrationPara.f32OFFSETGridVS * 10, -1, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;	

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalibrationPara.f32KGridIS * 1000, -3, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalibrationPara.f32OFFSETGridIS * 100, -2, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalibrationPara.f32KGridVT * 1000, -3, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalibrationPara.f32OFFSETGridVT * 10, -1, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;				

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalibrationPara.f32KGridIT * 1000, -3, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalibrationPara.f32OFFSETGridIT * 100, -2, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;	

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalibrationPara.f32SlaveKVPV1 * 1000, -3, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalibrationPara.f32SlaveOFFSETVPV1 * 10, -1, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;	

	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalibrationPara.f32SlaveKVPV2 * 1000, -3, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;
*/
	pDataBuf = &u8IPOMS_UserDataBuf0[bStrLen];
	bStrLen1 = sbNumToAscii(g_CalibrationPara.f32SlaveOFFSETVPV2 * 10, -1, pDataBuf); 
	bStrLen += bStrLen1;
	u8IPOMS_UserDataBuf0[bStrLen++] = 32;	

	SciWrite(ID_SCI_IPOMS, u8IPOMS_UserDataBuf0, bStrLen);
}              
/*=============================================================================*
 * FUNCTION: IPOMS_Q3Command(void)
 * PURPOSE : Pick up parameter for Snatch Graph fuction
 * INPUT: 
 *     void  // TRUE:disable the dog£»FALSE:enable the dog
 *
 * RETURN: 
 *     void
 *
 * CALLS:
 *    //Scia_Write
 *  
 * CALLED BY: 
 * //    INT32 IPOMS_Parsing() 
 * 
 *============================================================================*/
void IPOMS_Q3Command(void)
{
	Uint16 u16SnatchDataCntTemp, u16IntervalTemp;
	Uint8 u8DataKindTemp1, u8DataKindTemp2, u8DataKindTemp3, u8DataKindTemp4;
	Uint8 u8TriggerSourceTemp, u8TriggerTemp, u8SignTemp;
	Uint16 i, u16LengthTemp, u16CompareValTemp;
	
	u16LengthTemp = u16IPOMS_CommandLength;
	
	for(i = 0;i < u16LengthTemp;i++)
	{
		u8IPOMS_UserDataBuf0[i] = u8IPOMS_CommandBuffer0[i];		
	}

	SciWrite(ID_SCI_IPOMS, u8IPOMS_UserDataBuf0, u16LengthTemp);
	
	//aaa
	u16SnatchDataCntTemp = (u8IPOMS_CommandBuffer0[2] - 48) * 100 + (u8IPOMS_CommandBuffer0[3] - 48) * 10 + u8IPOMS_CommandBuffer0[4] - 48;
	//bbb
	u16IntervalTemp = (u8IPOMS_CommandBuffer0[6] - 48) * 100 + (u8IPOMS_CommandBuffer0[7] - 48) * 10 + u8IPOMS_CommandBuffer0[8] - 48;
	//cc
	u8DataKindTemp1 = (u8IPOMS_CommandBuffer0[10] - 48) * 10 + u8IPOMS_CommandBuffer0[11] - 48;
	//dd
	u8DataKindTemp2 = (u8IPOMS_CommandBuffer0[13] - 48) * 10 + u8IPOMS_CommandBuffer0[14] - 48;
	//ee
	u8DataKindTemp3 = (u8IPOMS_CommandBuffer0[16] - 48) * 10 + u8IPOMS_CommandBuffer0[17] - 48;
	//ff
	u8DataKindTemp4 = (u8IPOMS_CommandBuffer0[19] - 48) * 10 + u8IPOMS_CommandBuffer0[20] - 48;
	//gg
	u8TriggerSourceTemp = (u8IPOMS_CommandBuffer0[22] - 48) * 10 + u8IPOMS_CommandBuffer0[23] - 48;
	//h
	u8TriggerTemp = u8IPOMS_CommandBuffer0[25] - 48;
	//+/-
	u8SignTemp = u8IPOMS_CommandBuffer0[27] - 48;
	//iiiii
	u16CompareValTemp = (u8IPOMS_CommandBuffer0[29] - 48) * 10000 + (u8IPOMS_CommandBuffer0[30] - 48) * 1000 \
	 + (u8IPOMS_CommandBuffer0[31] - 48) * 100 + (u8IPOMS_CommandBuffer0[32] - 48) * 10 + u8IPOMS_CommandBuffer0[33] - 48;
	
	if((u16SnatchDataCntTemp > 500) ||(u16IntervalTemp > 500) ||(u8TriggerTemp > 4)) 
	    
	{
        return;
	}
	
	u16IPOMS_SaveDataCnt = 0;
	u16IPOMS_SnatchDataCnt = u16SnatchDataCntTemp;
	u16IPOMS_Interval = u16IntervalTemp;
	u16IPOMS_Interval1 = u16IPOMS_Interval;
	
	u8IPOMS_DataKind[0] = u8DataKindTemp1;
	u8IPOMS_DataKind[1] = u8DataKindTemp2;
	u8IPOMS_DataKind[2] = u8DataKindTemp3;
	u8IPOMS_DataKind[3] = u8DataKindTemp4;
	u8IPOMS_wTriggerSource = u8TriggerSourceTemp;
	u8IPOMS_wTrigger = u8TriggerTemp;
	u8IPOMS_Sign = u8SignTemp;
	
	u16IPOMS_CompareVal = u16CompareValTemp;
	u16IPOMS_TransmitCnt = 0;
}

 /*=============================================================================*
 * FUNCTION: IPOMS_QDCommand(void)
 * PURPOSE :  Send Data to IPOMS For QD Command
 * INPUT: 
 *     void  // TRUE:disable the dog£»FALSE:enable the dog
 *
 * RETURN: 
 *     void
 *
 * CALLS:
 *    
 *  //Sci_IPOMS_WriteBinary()
 *     
 *     
 *     
 *
 * CALLED BY: 
 *     //INT32 IPOMS_Parsing() 
 * 
 *============================================================================*/
void IPOMS_QDCommand(void)
{
	Uint8 u8Temp;
	int16 i;
	int32 i32CheckSum;
	
	u8Temp = u8IPOMS_CommandBuffer0[2] - 48;
	
	if((u8Temp >=4 ) || (0 == u16IPOMS_TransmitCnt))	
	{
	    return;
    }
	u16IPOMS_TransmitDataBuff[0] = 0x01;		//SOH
	u16IPOMS_TransmitDataBuff[1] = u16IPOMS_TransmitCnt;	//length
	
	i32CheckSum =(int32)(0x01 + u16IPOMS_TransmitCnt);
	for(i = 0;i < u16IPOMS_TransmitCnt;i++)
	{
		u16IPOMS_TransmitDataBuff[2 + i] = u16IPOMS_GraphDataBuff[u8Temp][i];
		i32CheckSum += u16IPOMS_GraphDataBuff[u8Temp][i];
	}	
	
	u16IPOMS_TransmitDataBuff[2 + u16IPOMS_TransmitCnt] = (Uint16)(i32CheckSum & 0x0000FFFF);

	Sci_IPOMS_WriteBinary(u16IPOMS_TransmitDataBuff, u16IPOMS_TransmitCnt + 3);
}
 /*=============================================================================*
 * FUNCTION: IPOMS_SnatchGraph(void)
 * PURPOSE :  SnatchGraph Data in the switching interrupt
 * INPUT: 
 *     void  // TRUE:disable the dog£»FALSE:enable the dog
 *
 * RETURN: 
 *     void
 *
 * CALLS:
 * 
 * CALLED BY: 
 * 
 *============================================================================*/
Uint8 IPOMS_SnatchGraph(void)
{
	int16 i,j;
	
	//trigger condition check
	if(0 == u8IPOMS_SnatchGraphEnable)
	{ 
	  return(TRUE);
	}

	if(0 == u8IPOMS_wTrigger)
	{
	  return(FALSE);
	}
	if(u16IPOMS_Interval1 > 0)
	{
		if(0 == (--u16IPOMS_Interval1))
		{
			u16IPOMS_Interval1 = u16IPOMS_Interval;
		}
		else
		{
			return(FALSE);
		}
	}
	
	for(i = 0;i < 4;i++)
	{   
	    j=u8IPOMS_DataKind[i];
		if(0 == j)
		{		
		    continue;
		}
		u16IPOMS_GraphDataBuff[i][u16IPOMS_SaveDataCnt] = (Uint16)GetDataSubArray[j]();
	}
    ++u16IPOMS_SaveDataCnt;
	if(u16IPOMS_SaveDataCnt == u16IPOMS_SnatchDataCnt)
	{
		u16IPOMS_TransmitCnt = u16IPOMS_SnatchDataCnt;
		u16IPOMS_SnatchDataCnt = 0;
		u16IPOMS_SaveDataCnt = 0;
		u8IPOMS_wTrigger = 0;
		u8IPOMS_wTriggerSource = 0;
		u8IPOMS_SnatchGraphEnable = 1;
		return(TRUE);	
	}
	return(FALSE);
}

/*=============================================================================*
 * FUNCTION: IPOMS_GraphicView_x(void)
 * PURPOSE : Assign variable shown in Graphic View screen of IPOMS 
 *============================================================================*/
int16 IPOMS_GraphicView_0(void)	// 0
{// Note: Don't set "Channel x ID" to 0 in "Graphic View" in IPOMS. 0 is invalid.
	return(0);
}	

int16 IPOMS_GraphicView_1(void)	// 1
{
	return(g_CurPIDCal.f32Duty3d/*CurrConReg.f32Bus_Error_k*/);
}

int16 IPOMS_GraphicView_2(void)	// 2
{
	return(g_Instantaneous.f32IGridR * 100);
}

int16 IPOMS_GraphicView_3(void)	// 3
{
    return(g_Instantaneous.f32IGridS * 100);
}

int16 IPOMS_GraphicView_4(void)	// 4
{
	return(g_Instantaneous.f32IGridT * 100);
}

int16 IPOMS_GraphicView_5(void)	// 5
{
	return(g_Instantaneous.f32VGridR * 10);
}

int16 IPOMS_GraphicView_6(void)	// 6
{
	return(g_Instantaneous.f32VGridS * 10);
}

int16 IPOMS_GraphicView_7(void)	// 7
{
	return(g_Instantaneous.f32VGridT * 10);
}

int16 IPOMS_GraphicView_8(void)	// 8
{
	return(g_Instantaneous.f32VPV1 * 10);
}

int16 IPOMS_GraphicView_9(void)	// 9
{
	return(g_Instantaneous.f32VPV2 * 10);
}

int16 IPOMS_GraphicView_10(void)	// 10
{
	return(g_Instantaneous.f32IPV1 * 100);
}

int16 IPOMS_GraphicView_11(void)	// 11
{
	return(g_Instantaneous.f32IPV2 * 100);
}

int16 IPOMS_GraphicView_12(void)	// 11
{
	return(g_CurPIDCal.f32CapHFCur_vinvR * 1000);
}

int16 IPOMS_GraphicView_13(void)	// 11
{
	return(g_CurPIDCal.f32CapHFCur_vinvS * 1000);
}

int16 IPOMS_GraphicView_14(void)	// 11
{
	return(g_CurPIDCal.f32CapHFCur_vinvT * 1000);
}

int16 IPOMS_GraphicView_15(void)	// 11
{
	return(100);
}

int16 IPOMS_GraphicView_16(void)	// 11
{
	return(g_Instantaneous.f32VbusP * 10);
}

int16 IPOMS_GraphicView_17(void)	// 11
{
	return(g_Instantaneous.f32VbusN * 10);
}

int16 IPOMS_GraphicView_18(void)	// 11
{
	return(g_Instantaneous.f32VbusPN * 10);
}

int16 IPOMS_GraphicView_19(void)	// 11
{
	return(g_PLLCal.f32Theta * 1000);
}

int16 IPOMS_GraphicView_20(void)	// 11
{
	return(g_Instantaneous.f32DciR * 100 + 1000);
}

int16 IPOMS_GraphicView_21(void)	// 11
{
	return(g_Instantaneous.f32DciS * 100 + 1000);
}

int16 IPOMS_GraphicView_22(void)	// 11
{
	return(g_Instantaneous.f32DciT * 100 + 1000);
}

int16 IPOMS_GraphicView_23(void)	// 11
{
	return(g_PLLCal.f32Vd * 10);
}

int16 IPOMS_GraphicView_24(void)	// 11
{
	return(0);
}

int16 IPOMS_GraphicView_25(void)	// 11
{
	return(0);
}

/*=============================================================================*
 * FUNCTION: sbNumToAscii
 * PURPOSE :  Convert input u16Number into max. 8 digital numbers including
 *			  decimal represented byASCII code.
 * INPUT: 
 *     void  // TRUE:disable the dog£»FALSE:enable the dog
 *
 * RETURN: 
 *     void
 *
 * CALLS: 
 *     void
 *     
 *   
 *     
 *
 * CALLED BY:   
 *     IPOMS_Q1Command   
 *============================================================================*/
Uint8 sbNumToAscii(Uint16 u16Number, int8 i8Exponent, Uint8 *pbBuffer)
{
	Uint8 u8No;
	int8 i,j;
    Uint8 bArrayTemp[8] = {0,0,0,0,0,0,0,0};
    Uint32 u32NumberTemp;
	j = 0;

	if((0 == u16Number) || ((u16Number != 0) && (i8Exponent < -6)))
	{
		*pbBuffer = '0';
		u8No = 1;
	}
	else
	{
		u32NumberTemp = (Uint32)u16Number;
		for(i = 0;i < i8Exponent;i++)
		{
			if(u32NumberTemp <= 9999999)
			{
			    u32NumberTemp = u32NumberTemp * 10;
			}
			else 
			{
				u32NumberTemp = 99999999;
			}
		}
		while ((u32NumberTemp > 0) || (i8Exponent < 0))
		{
			i = u32NumberTemp % 10;
			u32NumberTemp = u32NumberTemp / 10;
			bArrayTemp[j] = i + 0x30;
			j++;

			i8Exponent++;
			if(0 == i8Exponent)
			{
				bArrayTemp[j] = '.'; 
				j++;
				if(0 == u32NumberTemp)
				{   
					bArrayTemp[j] = '0'; 
					j++;
				}
			}
		}
		u8No = 0;
		j--;
		while (j >= 0)
		{
			*(pbBuffer + u8No) = bArrayTemp[j];
			u8No++;
			j--;
		}
	}/*	end of number !=0 */
	return(u8No);/* char length*/	   
}

/*=============================================================================*
 * FUNCTION: Sci_IPOMS_WriteBinary
 * PURPOSE :  Write a 16bit data to  Scia Tx Port
 * INPUT: 
 *     void  // TRUE:disable the dog£»FALSE:enable the dog
 *
 * RETURN: 
 *     void
 *
 * CALLS: 
 *    // void sSplit()
 *     
 *   
 *     
 *
 * CALLED BY: 
 *     IPOMS_QDCommand()
 *    
 *    
 * 
 *============================================================================*/
void Sci_IPOMS_WriteBinary(Uint16 *pstart, Uint16 u16Length)
{
    int8 i, j;
	Uint16	*pData;
	Uint8	u8Data;
	Uint8	u8IPOMS_SendHighHalfByte;
	
	u8IPOMS_SendHighHalfByte = 1;	
    pData = pstart;
	for(i = 0;i < u16Length;i++)
	{
	    for(j = 0;j < 2;j++)
	    {   
	        if(1 == u8IPOMS_SendHighHalfByte)
            {   //write high 8bit
				u8Data = GET_HBYTE_OF_WORD(*pData);
				while(SciWrite(ID_SCI_IPOMS, &u8Data, 1));
                u8IPOMS_SendHighHalfByte = 0;    
            }
            else
	        {   //write low 8bit
	            u8Data = GET_LBYTE_OF_WORD(*pData);
				while(SciWrite(ID_SCI_IPOMS, &u8Data, 1));
                u8IPOMS_SendHighHalfByte = 1; 
     	    }
	    }

        ++pData;
    }
}

#endif // end of "#ifdef _IPOMS_"
//===========================================================================
// End of file.
//===========================================================================

