/***********************************************************************
 *    FILENAME : SC_Spi.c
 *
 *    PURPOSE  : 
 *    HISTORY  :
 *           DATE          VERSION      AUTHOR        NOTE
 ************************************************************************/
#include "SC_master.h"
void PackageMasterData();
float32 f32Temp_iq = 0;
float32 f32tanphi_calc = 0;
Uint16 SpiChecksum(Uint16* pData,Uint8 len);

int16 temp5_1 = 0;
int16 temp5_2 = 0;
int16 temp5_3 = 0;
int16 temp5_4 = 0;
int16 temp5_5 = 0;
int16 temp5_6 = 0;
int16 temp5_7 = 0;
int16 temp5_8 = 0;
int16 temp5_9 = 0;
int16 temp5_10 = 0;
int16 temp5_11 = 0;
int16 temp5_12 = 0;

int16 temp6_1 = 0;
int16 temp6_2 = 0;
int16 temp6_3 = 0;
int16 temp6_4 = 0;
int16 temp6_5 = 0;
int16 temp6_6 = 0;
int16 temp6_7 = 0;
int16 temp6_8 = 0;
int16 temp6_9 = 0;
int16 temp6_10 = 0;
int16 temp6_11 = 0;
int16 temp6_12 = 0;

int16 temp7_1 = 0;
int16 temp7_2 = 0;
int16 temp7_3 = 0;
int16 temp7_4 = 0;
int16 temp7_5 = 0;
int16 temp7_6 = 0;
int16 temp7_7 = 0;
int16 temp7_8 = 0;
int16 temp7_9 = 0;
int16 temp7_10 = 0;
int16 temp7_11 = 0;
int16 temp7_12 = 0;

int16 temp8_1 = 0;
int16 temp8_2 = 0;
int16 temp8_3 = 0;
int16 temp8_4 = 0;
int16 temp8_5 = 0;
int16 temp8_6 = 0;
int16 temp8_7 = 0;
int16 temp8_8 = 0;
int16 temp8_9 = 0;
int16 temp8_10 = 0;
int16 temp8_11 = 0;
int16 temp8_12 = 0;

int16 temp9_1 = 0;
int16 temp9_2 = 0;
int16 temp9_3 = 0;
int16 temp9_4 = 0;
int16 temp9_5 = 0;
int16 temp9_6 = 0;
int16 temp9_7 = 0;
int16 temp9_8 = 0;
int16 temp9_9 = 0;
int16 temp9_10 = 0;
int16 temp9_11 = 0;
int16 temp9_12 = 0;

int16 temp10_1 = 0;
int16 temp10_2 = 0;
int16 temp10_3 = 0;
int16 temp10_4 = 0;
int16 temp10_5 = 0;
int16 temp10_6 = 0;
int16 temp10_7 = 0;
int16 temp10_8 = 0;
int16 temp10_9 = 0;
int16 temp10_10 = 0;
int16 temp10_11 = 0;
int16 temp10_12 = 0;

int16 temp11_1 = 0;
int16 temp11_2 = 0;
int16 temp11_3 = 0;
int16 temp11_4 = 0;
int16 temp11_5 = 0;
int16 temp11_6 = 0;
int16 temp11_7 = 0;
int16 temp11_8 = 0;
int16 temp11_9 = 0;
int16 temp11_10 = 0;
int16 temp11_11 = 0;
int16 temp11_12 = 0;

int16 temp12_1 = 0;
int16 temp12_2 = 0;
int16 temp12_3 = 0;
int16 temp12_4 = 0;
int16 temp12_5 = 0;
int16 temp12_6 = 0;
int16 temp12_7 = 0;
int16 temp12_8 = 0;
int16 temp12_9 = 0;
int16 temp12_10 = 0;
int16 temp12_11 = 0;
int16 temp12_12 = 0;

int16 temp13_1 = 0;
int16 temp13_2 = 0;
int16 temp13_3 = 0;
int16 temp13_4 = 0;
int16 temp13_5 = 0;
int16 temp13_6 = 0;
int16 temp13_7 = 0;
int16 temp13_8 = 0;
int16 temp13_9 = 0;
int16 temp13_10 = 0;
int16 temp13_11 = 0;
int16 temp13_12 = 0;

int16 temp14_1 = 0;
int16 temp14_2 = 0;
int16 temp14_3 = 0;
int16 temp14_4 = 0;
int16 temp14_5 = 0;
int16 temp14_6 = 0;
int16 temp14_7 = 0;
int16 temp14_8 = 0;
int16 temp14_9 = 0;
int16 temp14_10 = 0;
int16 temp14_11 = 0;
int16 temp14_12 = 0;

int16 temp15_1 = 0;
int16 temp15_2 = 0;
int16 temp15_3 = 0;
int16 temp15_4 = 0;
int16 temp15_5 = 0;
int16 temp15_6 = 0;
int16 temp15_7 = 0;
int16 temp15_8 = 0;
int16 temp15_9 = 0;
int16 temp15_10 = 0;
int16 temp15_11 = 0;
int16 temp15_12 = 0;

int16 temp16_1 = 0;
int16 temp16_2 = 0;
int16 temp16_3 = 0;
int16 temp16_4 = 0;
int16 temp16_5 = 0;
int16 temp16_6 = 0;
int16 temp16_7 = 0;
int16 temp16_8 = 0;
int16 temp16_9 = 0;
int16 temp16_10 = 0;
int16 temp16_11 = 0;
int16 temp16_12 = 0;




/**********************************************************************
* FUNCION :  InitSpi
* PURPOSE :  This function initializes the SPI-A to a known state.
* CALLED BY: 
**********************************************************************/
void InitSpi(void)
{
	SpiaRegs.SPICCR.bit.SPISWRESET    = 0;		// Initializes the SPI operating flags to the reset condition.	
	SpiaRegs.SPICCR.bit.CLKPOLARITY   = 1;		// Data is output on falling edge and input on rising edge.
	SpiaRegs.SPICCR.bit.SPILBK        = 0;		// Disable loop back mode.
	SpiaRegs.SPICCR.bit.SPICHAR       = 0xF;	// Character lenth is 16 bit. 

	SpiaRegs.SPICTL.bit.OVERRUNINTENA = 0;		// Disable receiver overrun flag bit interrupts.
	SpiaRegs.SPICTL.bit.CLK_PHASE     = 0;		// Normal phase, no delay clock.
	SpiaRegs.SPICTL.bit.MASTER_SLAVE  = 1;		// Master.
	SpiaRegs.SPICTL.bit.TALK          = 1;		// Enable transmission.
	SpiaRegs.SPICTL.bit.SPIINTENA     = 0;		// Disable interrupt.

	SpiaRegs.SPIBRR                   = 124;	// Rate is 300000bps = (150000000/4) / (BRR+1).
	SpiaRegs.SPIPRI.bit.FREE          = 1;		// Free fun, continue SPI operation regardless of suspend

	SpiaRegs.SPIFFTX.bit.SPIRST       = 0;		// Reset FIFO
	SpiaRegs.SPIFFTX.bit.SPIFFENA     = 1;		// Enable FIFO
	SpiaRegs.SPIFFTX.bit.TXFFIENA     = 0;		// Disable TXFIFO interrupt
	SpiaRegs.SPIFFRX.bit.RXFFIENA     = 1;		// Enable  RXFIFO interrupt
	SpiaRegs.SPIFFRX.bit.RXFFIL       = SPI_FIFO_LENTH;		// Initializes RX interrupt level
	SpiaRegs.SPIFFTX.bit.SPIRST       = 1;		// Resume FIFO transmit and receive
	SpiaRegs.SPIFFCT.bit.TXDLY        = 15;		// delay 16 SPI clocks(TXDLY+1)

	SpiaRegs.SPICCR.bit.SPISWRESET    = 1;		// Relinquish SPI from Reset 

	PieCtrlRegs.PIEIER6.bit.INTx1 = 1;			// Enable INT 6.1 in the PIE, SPIRxINTA is INT6.1
   	IER |= M_INT6; 								// Enable CPU Interrupt 6 

	DelayUs(5000L); 

}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void SpiChange(Uint16 *pTX_Buf,Uint16 *pRX_Buf,Uint8 item_Num)
{
	int i;
	Uint16 u16_remain;
	Uint16 u16_level;	// 每次发送和接收的字数

	u16_remain = item_Num;
	while(u16_remain > 0)
	{	// 确定每次发送的字数
		if(u16_remain >= SPI_FIFO_LENTH)
		{
			u16_level   = SPI_FIFO_LENTH;
			u16_remain -= SPI_FIFO_LENTH;
		}
		else
		{
			u16_level  = u16_remain;
			u16_remain = 0;
		}
		// 设置FIFO接收中断触发级数
		SpiaRegs.SPIFFRX.bit.RXFFIL = u16_level;
		// 向TXFIFO填入要发送的字
		for(i=0; i<u16_level; i++)
		{
			SpiaRegs.SPITXBUF = *pTX_Buf;
			pTX_Buf++;
		}
		// 等待收到与发送字数相同的字数
		SEM_pend(&SEM_SPIa, SYS_FOREVER);	// TBD 应加入超时机制
		// 检查收到的数据量是否符合预期
		if(SpiaRegs.SPIFFRX.bit.RXFFST == u16_level)
		{	// 收到的数据量符合预期，把数据读出来
			for(i=0; i<u16_level; i++)
			{
				*pRX_Buf = SpiaRegs.SPIRXBUF;
				pRX_Buf++;
			}
			SpiaRegs.SPIFFRX.bit.RXFFINTCLR = 1;	// 清除FIFO接收中断
		}
		else
		{	// TBD 收到的数据量不符合预期，报错
			asm ("      ESTOP0");	// for test
  			for(;;);
		}
	}
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void SetMasterValue(void)
{
	g_u16SpiTxBuf[0] =                               ((Uint16)'M' <<8) | 0x00AA;
	g_u16SpiTxBuf[1] =                ((Uint16)g_SysCurrentState  <<8) | 0x00FF;	//Hight Byte:master state 	Low Byte:reserved
	g_u16SpiTxBuf[2] = (Uint16)g_StateCheck.Word.byte4;
	g_u16SpiTxBuf[3] = ((Uint16)g_SysFaultMessage.Word.byte0 <<8) | (Uint16)g_SysFaultMessage.Word.byte1;
	g_u16SpiTxBuf[4] = ((Uint16)g_SysFaultMessage.Word.byte2 <<8) | (Uint16)g_SysFaultMessage.Word.byte3;
	g_u16SpiTxBuf[5] = ((Uint16)g_SysFaultMessage.Word.byte4 <<8) | (Uint16)g_SysFaultMessage.Word.byte5;
	g_u16SpiTxBuf[6] = ((Uint16)g_SysFaultMessage.Word.byte6 <<8) | (Uint16)g_SysFaultMessage.Word.byte7;
	g_u16SpiTxBuf[7] = ((Uint16)g_SysFaultMessage.Word.unrecover0 <<8) | (Uint16)g_SysFaultMessage.Word.unrecover1; 
	PackageMasterData();
	g_u16SpiTxBuf[19] = (Uint16)(g_CalcResult.Rms.f32VGridR * 10);//
	g_u16SpiTxBuf[20] = (Uint16)(g_CalcResult.Rms.f32VGridS * 10);//
	g_u16SpiTxBuf[21] = (Uint16)(g_CalcResult.Rms.f32VGridT * 10);//
	g_u16SpiTxBuf[22] = ((Uint16)g_StateCheck.Word.byte6<<8)|(Uint16)g_StateCheck.Word.byte7;//;//
	g_u16SpiTxBuf[23] = (int16)(g_CalcResult.Rms.f32GFCI);//
	g_u16SpiTxBuf[24] = (int16)(g_CalcResult.Ave.f32DciR + 1000);//
	g_u16SpiTxBuf[25] = (int16)(g_CalcResult.Ave.f32DciS + 1000);//
	g_u16SpiTxBuf[26] = (int16)(g_CalcResult.Ave.f32DciT + 1000);
	g_u16SpiTxBuf[27] = 0;
	g_u16SpiTxBuf[28] = 0;
	g_u16SpiTxBuf[29] = 0;
	g_u16SpiTxBuf[30] = 0;
	g_u16SpiTxBuf[31] = SpiChecksum(g_u16SpiTxBuf,31);

	g_StateCheck.Word.byte6 = 0;
	g_StateCheck.Word.byte7 = 0;
/*
	Uint16 i;
	g_u16SpiTxBuf[0] = (((Uint16)'M'<<8)|0xAA);
	for(i=1; i<31; i++)
		g_u16SpiTxBuf[i] = i;
	g_u16SpiTxBuf[31] = SpiChecksum(g_u16SpiTxBuf,31);
*/
}
/************************************************************************
Faction name:	PackageSlaveData
*************************************************************************/
void PackageMasterData()
{
	static Uint8 b_SpiSendCnt = 0;

	
	g_u16SpiTxBuf[8] = b_SpiSendCnt;
	switch(b_SpiSendCnt)
	{
		/***Data***/
		case 0x00:
			g_u16SpiTxBuf[9]  = MASTER_MAIN_VERSION;			//Master DSP SW
			g_u16SpiTxBuf[10] = MASTER_MAIN_VERSION0;			//Master DSP SW
			g_u16SpiTxBuf[11] = MASTER_SECONDARY_VERSION1;			//Master DSP SW
			g_u16SpiTxBuf[12] = MASTER_SECONDARY_VERSION2;
			g_u16SpiTxBuf[13] = g_u16SafetyCountDown;
			g_u16SpiTxBuf[14] = g_StateCheck.Word.byte0;
			g_u16SpiTxBuf[15] = g_InverterType;	
			g_u16SpiTxBuf[16] = 0;
			g_u16SpiTxBuf[17] = 0;
			g_u16SpiTxBuf[18] = 0;
		break;
		case 0x01:
			g_u16SpiTxBuf[9] = (Uint16)(g_CalcResult.PowerDisplay.AccCalc.f32Input1Watt * 0.1);			//PV1 Power low
			g_u16SpiTxBuf[10] = (Uint16)(g_CalcResult.PowerDisplay.AccCalc.f32Input2Watt * 0.1);			//PV2 power low
			g_u16SpiTxBuf[11] = (Uint16)(g_CalcResult.PowerDisplay.AccCalc.f32OutputWatt * 0.1);		//Out Power hight
			g_u16SpiTxBuf[12] = (Uint16)(g_CalcResult.PowerDisplay.f32OutputEnergy * 0.1);
			g_u16SpiTxBuf[13] = (Uint16)(g_CalcResult.Ave.f32VPV1 * 10);			//PV1 VoltageAvg
			g_u16SpiTxBuf[14] = (Uint16)(g_CalcResult.Ave.f32IPV1 * 100);			//PV1 CurrentAvg
			g_u16SpiTxBuf[15] = (Uint16)(g_CalcResult.Ave.f32VPV2 * 10);			//PV2 VoltageAvg
			g_u16SpiTxBuf[16] = (Uint16)(g_CalcResult.Ave.f32IPV2 * 100);			//PV2 CurrentAvg
			g_u16SpiTxBuf[17] = (Uint16)(g_CalcResult.Ave.f32Vbus * 10);			//PV2 CurrentAvg
			g_u16SpiTxBuf[18] = g_SafetyReg.CoMMDate;			//PV2 CurrentAvg
		break;
		case 0x02:
			g_u16SpiTxBuf[9]  = (Uint16)(g_CalcResult.Rms.f32IGridR * 100);			//R-inverter CurrentRms
			g_u16SpiTxBuf[10] = (Uint16)(g_CalcResult.Rms.f32IGridS * 100);			//S-inverter CurrentRms
			g_u16SpiTxBuf[11] = (Uint16)(g_CalcResult.Rms.f32IGridT * 100);			//T-inverter CurrentRms
			g_u16SpiTxBuf[12] = (Uint16)(g_CalcResult.Ave.f32FGrid * 100);
			g_u16SpiTxBuf[13] = (Uint16)(g_CalcResult.Power.f32InvQ * 0.1);
			g_u16SpiTxBuf[16] = 0;
			g_u16SpiTxBuf[17] = 0;
		break;
		default:
		break;
	}
	
	b_SpiSendCnt++;
	if(b_SpiSendCnt > 2)
	{
		b_SpiSendCnt = 0;
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
/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void SpiMasterReceiveData(void)
{
	Uint16 u16Temp;
	float32 f32Temp_1;
	Uint16 u16Temp_1;
	Uint8 u8Temp_1;
//	float f32Temp_iq;
	static int16 SafetyUpdataFinishFlag = 0; 
#ifdef SC_LOG_PRINTF
	Uint16 i;
#endif

	u16Temp = SpiChecksum(g_u16SpiRxBuf,31);	// checksum
	if(u16Temp != g_u16SpiRxBuf[31]) 
	{
#ifdef SC_LOG_PRINTF
		LOG_printf(&trace, "SPI fail----Checksum Error! M-%d, S-%d",u16Temp, g_u16SpiRxBuf[31]);
		for(i=0; i<SPI_COM_LENGTH; i++)
		{
			if((((Uint16)'S'<<8)|0xAA) == g_u16SpiRxBuf[i])
				break;
		}
		LOG_printf(&trace, "==== Head is in %d", i);
#endif
	}
	else if(g_u16SpiRxBuf[0] != (((Uint16)'S'<<8)|0xAA))
	{
#ifdef SC_LOG_PRINTF
		LOG_printf(&trace, "SPI fail----First char Error!");
#endif
	}
	else
	{
		g_u16SpiFaultCnt = 0;		

		g_SlaveStateCheck.Word.byte0 = g_u16SpiRxBuf[2];
		
		g_SlaveSysFaultMessage.Word.byte0 = (Uint8)((g_u16SpiRxBuf[3]&0xFF00)>>8);
		g_SlaveSysFaultMessage.Word.byte1 = (Uint8) (g_u16SpiRxBuf[3]&0x00FF);
		g_SlaveSysFaultMessage.Word.byte2 = (Uint8)((g_u16SpiRxBuf[4]&0xFF00)>>8);
		g_SlaveSysFaultMessage.Word.byte3 = (Uint8) (g_u16SpiRxBuf[4]&0x00FF);
		g_SlaveSysFaultMessage.Word.byte4 = (Uint8)((g_u16SpiRxBuf[5]&0xFF00)>>8);
		g_SlaveSysFaultMessage.Word.byte5 = (Uint8)( g_u16SpiRxBuf[5]&0x00FF);
		g_SlaveSysFaultMessage.Word.byte6 = (Uint8)((g_u16SpiRxBuf[6]&0xFF00)>>8);
		g_SlaveSysFaultMessage.Word.byte7 = (Uint8)( g_u16SpiRxBuf[6]&0x00FF);
		g_SlaveSysFaultMessage.Word.unrecover0 = (Uint8)((g_u16SpiRxBuf[7]&0xFF00)>>8);	
		g_SlaveSysFaultMessage.Word.unrecover1 = (Uint8)( g_u16SpiRxBuf[7]&0x00FF);

		u16Temp = g_u16SpiRxBuf[8];
		switch(u16Temp)
		{
			case 0x00:
				g_SafetyReg.u16SafeCountry = g_u16SpiRxBuf[9];
				g_MPPTState.bit.InputMode = g_u16SpiRxBuf[10];//parrelel,independent

				temp6_1	= g_u16SpiRxBuf[11];
				temp6_2 = g_u16SpiRxBuf[12];
				temp6_3 = g_u16SpiRxBuf[13];
				u16Temp_1 = g_u16SpiRxBuf[14];
				if(u16Temp_1 != g_SafetyReg.CoMMDate)
				{
					g_SafetyReg.CoMMDate = g_u16SpiRxBuf[14];
					g_CalcResult.PowerDisplay.f32OutputEnergy = 0;
				}
				temp6_4	= g_u16SpiRxBuf[15];
				temp6_5 = g_u16SpiRxBuf[16];
			break;
			case 0x01:
				temp16_2 = g_u16SpiRxBuf[9];//i16VGRID_R
				temp16_3 = g_u16SpiRxBuf[10];//i16VGRID_S
				temp16_4 = g_u16SpiRxBuf[11];//i16VGRID_T
				temp16_5 = g_u16SpiRxBuf[12];//i16VINV_R
				temp16_6 = g_u16SpiRxBuf[13];//i16VINV_S
				temp16_7 = g_u16SpiRxBuf[14];//i16VINV_T
				temp16_8 = g_u16SpiRxBuf[15] + 1000;//i16DCI_R
				temp16_9 = g_u16SpiRxBuf[16] + 1000;//i16DCI_S
				temp16_10 = g_u16SpiRxBuf[17] + 1000;//i16DCI_T
				temp16_11 = g_u16SpiRxBuf[18];//i16GFCI
				temp16_12 = g_u16SpiRxBuf[19];//Freq
			break;
			case 0x02:
//				temp16_1 = g_u16SpiRxBuf[9];
//				temp16_2 = g_u16SpiRxBuf[10];
//				temp16_3 = g_u16SpiRxBuf[11];
//				temp16_4 = g_u16SpiRxBuf[12];
//				temp16_5 = g_u16SpiRxBuf[13];
//				temp16_6 = g_u16SpiRxBuf[14];
//				temp16_7 = g_u16SpiRxBuf[15];
//				temp16_8 = g_u16SpiRxBuf[16];
//				temp16_9 = g_u16SpiRxBuf[17];
//				temp16_10 = g_u16SpiRxBuf[18];
//				temp16_11 = g_u16SpiRxBuf[19];
//				temp16_12 = g_u16SpiRxBuf[20];
			break;

			case 0x30:
//Tconnection PowerUpSpeed	Tback PowerbackUpSpeed	VGridBack>	VGridBack<	  FGridBack> FGridBack<
//        {60,	20,			60,		20,				2060,			2660,		5449,		4510},

				g_SafetyConnect.u16ConnectTime	= g_u16SpiRxBuf[9];
				g_u16SafetyCountDown = g_SafetyConnect.u16ConnectTime;
//				f32Temp_1 = g_u16SpiRxBuf[10] * g_Rated.f32OutputPower * 3.3333e-6f;
				f32Temp_1 = g_u16SpiRxBuf[10] * g_Rated.f32OutputPower * 3.3333e-6f;
				g_SafetyConnect.f32PupSlop = f32Temp_1;
				g_f32PowerLoadSpeed = g_SafetyConnect.f32PupSlop;
				g_SafetyConnect.u16ReconnectTime = g_u16SpiRxBuf[11];
				f32Temp_1 = g_u16SpiRxBuf[12] * g_Rated.f32OutputPower * 3.3333e-6f;
				g_SafetyConnect.f32ReconnectPupSlop = f32Temp_1;

				f32Temp_1 = g_u16SpiRxBuf[13] * 0.1;				
				g_SafetyConnect.f32VGridMaxLimitBack = f32Temp_1;
				f32Temp_1 = g_u16SpiRxBuf[14] * 0.1;
				g_SafetyConnect.f32VGridMinLimitBack = f32Temp_1;
				f32Temp_1 = g_u16SpiRxBuf[15] * 0.01;
				g_SafetyConnect.f32FGridMaxLimitBack = f32Temp_1;
				f32Temp_1 = g_u16SpiRxBuf[16] * 0.01;
				g_SafetyConnect.f32FGridMinLimitBack = f32Temp_1;
//				temp6_8 = g_u16SpiRxBuf[16];
//				temp6_9 = g_u16SpiRxBuf[17];
//				temp6_10 = g_u16SpiRxBuf[18];
//				temp6_11 = g_u16SpiRxBuf[19];
//				temp6_12 = g_u16SpiRxBuf[20];
				g_StateCheck.bit.SafetySTART_UpdateOKflag = 1;
				SafetyUpdataFinishFlag = (SafetyUpdataFinishFlag|1);
			break;

			case 0x31:
//EABLE,VHi1, Time, VLow1,  Time,   VHi2, Time2,	VLow2,	Time  10MINV,
//{0,   2660,  20,  2030,	200,	2800,	60,		1750,	60   2530,},
				g_SaftyConfig.Word.VoltProCon = GET_LBYTE_OF_WORD(g_u16SpiRxBuf[9]);


				f32Temp_1 = g_u16SpiRxBuf[10] * 0.1;
				g_SafetyVolt.f32VGridMax1Limit = f32Temp_1;
				u16Temp_1 = g_u16SpiRxBuf[11]>>1;
				g_SafetyVolt.u16VGridMax1ProtectTime = u16Temp_1;
				f32Temp_1 = g_u16SpiRxBuf[12] * 0.1;
				g_SafetyVolt.f32VGridMax2Limit = f32Temp_1;
				u16Temp_1 = g_u16SpiRxBuf[13]>>1;
				g_SafetyVolt.u16VGridMax2ProtectTime= u16Temp_1;
				f32Temp_1 = g_u16SpiRxBuf[14] * 0.1;
				g_SafetyVolt.f32VGridMin1Limit = f32Temp_1;
				u16Temp_1 = g_u16SpiRxBuf[15]>>1;
				g_SafetyVolt.u16VGridMin1ProtectTime = u16Temp_1;
				f32Temp_1 = g_u16SpiRxBuf[16] * 0.1;
				g_SafetyVolt.f32VGridMin2Limit = f32Temp_1;
				u16Temp_1 = g_u16SpiRxBuf[17]>>1;
				g_SafetyVolt.u16VGridMin2ProtectTime = u16Temp_1;
				f32Temp_1 = g_u16SpiRxBuf[18] * 0.1;
				g_SafetyVolt.f32VGridTenMinutesLimit = f32Temp_1;
//				temp7_11 = g_u16SpiRxBuf[19];
//				temp7_12 = g_u16SpiRxBuf[20];
				g_StateCheck.bit.GridProtectV_UpdateOKflag = 1;
				SafetyUpdataFinishFlag = (SafetyUpdataFinishFlag|1<<1);
			break;

			case 0x32:
//EABLE,  FHi1, Time10,	FLow1, Time10,  FHi2,  Time,   FLow2,  Time,
//	{0,   5490,   20,	4510,	20,		5600,	100,	4400,	100},
				g_SaftyConfig.Word.FreProCon = GET_LBYTE_OF_WORD(g_u16SpiRxBuf[9]);
				f32Temp_1 = g_u16SpiRxBuf[10] * 0.01;
				g_SafetyFreq.f32FGridMax1Limit = f32Temp_1;
				u16Temp_1 = g_u16SpiRxBuf[11]>>1;
				g_SafetyFreq.u16FGridMax1ProtectTime = u16Temp_1;
				f32Temp_1 = g_u16SpiRxBuf[12] * 0.01;
				g_SafetyFreq.f32FGridMax2Limit = f32Temp_1;
				u16Temp_1 = g_u16SpiRxBuf[13]>>1;
				g_SafetyFreq.u16FGridMax2ProtectTime = u16Temp_1;
				f32Temp_1 = g_u16SpiRxBuf[14] * 0.01;
				g_SafetyFreq.f32FGridMin1Limit = f32Temp_1;
				u16Temp_1 = g_u16SpiRxBuf[15]>>1;
				g_SafetyFreq.u16FGridMin1ProtectTime = u16Temp_1;
				f32Temp_1 = g_u16SpiRxBuf[16] * 0.01;
				g_SafetyFreq.f32FGridMin2Limit = f32Temp_1;
				u16Temp_1 = g_u16SpiRxBuf[17]>>1;
				g_SafetyFreq.u16FGridMin2ProtectTime = u16Temp_1;
//				temp8_10 = g_u16SpiRxBuf[18];
//				temp8_11 = g_u16SpiRxBuf[19];
//				temp8_12 = g_u16SpiRxBuf[20];
				g_StateCheck.bit.GridProtectFreqValue_UpdateOKflag = 1;
				SafetyUpdataFinishFlag = (SafetyUpdataFinishFlag|1<<2);
			break;

			case 0x33:
				g_SaftyConfig.Word.DCIProCon = GET_LBYTE_OF_WORD(g_u16SpiRxBuf[9]);
				f32Temp_1 = g_u16SpiRxBuf[10];
				g_SafetyDCI.f32DciMax1Limit = f32Temp_1;
				u16Temp_1 = g_u16SpiRxBuf[11]>>1;
				g_SafetyDCI.u16DciMax1ProtectTime = u16Temp_1;
				f32Temp_1 = g_u16SpiRxBuf[12];
				g_SafetyDCI.f32DciMax2Limit = f32Temp_1;
				u16Temp_1 = g_u16SpiRxBuf[13]>>1;
				g_SafetyDCI.u16DciMax2ProtectTime = u16Temp_1;
				f32Temp_1 = g_u16SpiRxBuf[14]* 0.001;
				g_SafetyDCI.f32DciTestInjection = f32Temp_1;
				temp9_7 = g_u16SpiRxBuf[15];
				temp9_8 = g_u16SpiRxBuf[16];
				temp9_9 = g_u16SpiRxBuf[17];
				temp9_10 = g_u16SpiRxBuf[18];
				temp9_11 = g_u16SpiRxBuf[19];
				temp9_12 = g_u16SpiRxBuf[20];
				g_StateCheck.bit.GridProtectDCI_UpdateOKflag = 1;
				SafetyUpdataFinishFlag = (SafetyUpdataFinishFlag|1<<3);
			break;

			case 0x34:
				g_SaftyConfig.Word.PowerRemoteCon = GET_LBYTE_OF_WORD(g_u16SpiRxBuf[9]);
				if(1 == g_SaftyConfig.bit.RemoteONOFFEnable)
				{
					g_SafetyActive.RemoteOnOff = g_u16SpiRxBuf[11];
				}
				else
				{
					g_SafetyActive.RemoteOnOff = 0x55;
				}
				f32Temp_1 = g_u16SpiRxBuf[10]* 0.001;
				g_SafetyActive.f32PLimitCommand = f32Temp_1;
//				g_SafetyActive.RemoteOnOff = g_u16SpiRxBuf[11];
				temp10_4 = g_u16SpiRxBuf[12];
				temp10_5 = g_u16SpiRxBuf[13];
				temp10_6 = g_u16SpiRxBuf[14];
				temp10_7 = g_u16SpiRxBuf[15];
				temp10_8 = g_u16SpiRxBuf[16];
				temp10_9 = g_u16SpiRxBuf[17];
				temp10_10 = g_u16SpiRxBuf[18];
				temp10_11 = g_u16SpiRxBuf[19];
				temp10_12 = g_u16SpiRxBuf[20];
				g_StateCheck.bit.RemotPowerControlValue_UpdateOKflag = 1;
				SafetyUpdataFinishFlag = (SafetyUpdataFinishFlag|1<<4);
			break;

			case 0x35:
				g_SaftyConfig.Word.PowerFreCon = GET_LBYTE_OF_WORD(g_u16SpiRxBuf[9]);
				f32Temp_1 = g_u16SpiRxBuf[10] * 0.01;
				g_GridManager.PFreq.f32FreqPoint = f32Temp_1;
				f32Temp_1 = g_u16SpiRxBuf[11] * 0.01;
				g_GridManager.PFreq.f32Slop = f32Temp_1;
				f32Temp_1 = g_u16SpiRxBuf[12] * 0.01;
				g_GridManager.PFreq.f32FOback = f32Temp_1;
				f32Temp_1 = g_u16SpiRxBuf[13] * 0.01;
				g_GridManager.PFreq.f32FUback = f32Temp_1;
				u16Temp_1 = g_u16SpiRxBuf[14] * 50;
				g_GridManager.PFreq.u16WaitTime = u16Temp_1;
				f32Temp_1 = g_u16SpiRxBuf[15] * 0.01;
				g_GridManager.PFreq.f32BackSpeed = f32Temp_1;
				temp11_8 = g_u16SpiRxBuf[16];
				temp11_9 = g_u16SpiRxBuf[17];
				temp11_10 = g_u16SpiRxBuf[18];
				temp11_11 = g_u16SpiRxBuf[19];
				temp11_12 = g_u16SpiRxBuf[20];
				g_StateCheck.bit.FreqPowerControlValue_UpdateOKflag = 1;
				SafetyUpdataFinishFlag = (SafetyUpdataFinishFlag|1<<5);
			break;

			case 0x36:
				g_SaftyConfig.Word.ReactiveCon = GET_LBYTE_OF_WORD(g_u16SpiRxBuf[9]);

				f32Temp_1 = 0.01 * (int16)g_u16SpiRxBuf[10];
				g_GridManager.InvQ.f32Cosphi = f32Temp_1;
				f32Temp_1 = 0.0001 * (int16)g_u16SpiRxBuf[11];
				g_GridManager.InvQ.f32Qvar = f32Temp_1 * g_Rated.f32OutputPower;
				f32Temp_1 = 0.01 * (int16)g_u16SpiRxBuf[12];
				g_GridManager.InvQ.f32Cosphi1 = f32Temp_1;
				f32Temp_1 = 0.01 * (int16)g_u16SpiRxBuf[13];
				g_GridManager.InvQ.f32Pwatt1 = f32Temp_1 * g_Rated.f32OutputPower;
				f32Temp_1 = 0.01 * (int16)g_u16SpiRxBuf[14];
				g_GridManager.InvQ.f32Cosphi2 = f32Temp_1;

				f32Temp_1 = 0.01 * (int16)g_u16SpiRxBuf[15];
				g_GridManager.InvQ.f32Pwatt2 = f32Temp_1 * g_Rated.f32OutputPower;

				f32Temp_1 = 0.01 * (int16)g_u16SpiRxBuf[16];
				g_GridManager.InvQ.f32Cosphi3 = f32Temp_1;

				f32Temp_1 = 0.01 * (int16)g_u16SpiRxBuf[17];
				g_GridManager.InvQ.f32Pwatt3 = f32Temp_1 * g_Rated.f32OutputPower;

				f32Temp_1 = 0.01 * (int16)g_u16SpiRxBuf[18];
				g_GridManager.InvQ.f32Cosphi4 = f32Temp_1;

				f32Temp_1 = 0.01 * (int16)g_u16SpiRxBuf[19];
				g_GridManager.InvQ.f32Pwatt4 = f32Temp_1 * g_Rated.f32OutputPower;


				f32Temp_1 = 0.01 * (int16)g_u16SpiRxBuf[20];
				g_GridManager.InvQ.f32LockinV = f32Temp_1;

				f32Temp_1 = 0.01 * (int16)g_u16SpiRxBuf[21];
				g_GridManager.InvQ.f32LockoutV = f32Temp_1;

				f32Temp_1 = 0.01 * (int16)g_u16SpiRxBuf[22];
				g_GridManager.InvQ.f32U1i = f32Temp_1;

				f32Temp_1 = 0.01 * (int16)g_u16SpiRxBuf[23];
				g_GridManager.InvQ.f32U1s = f32Temp_1;

				f32Temp_1 = 0.01 * (int16)g_u16SpiRxBuf[24];
				g_GridManager.InvQ.f32U2i = f32Temp_1;


				g_StateCheck.bit.ReactiveValue_UpdateOKflag1 = 1;
				SafetyUpdataFinishFlag = (SafetyUpdataFinishFlag|1<<6);
			break;

			case 0x37:
				f32Temp_1 = 0.01 * (int16)g_u16SpiRxBuf[9];
				g_GridManager.InvQ.f32U2s = f32Temp_1;

				f32Temp_1 = 0.01 * (int16)g_u16SpiRxBuf[10];
				g_GridManager.InvQ.f32LockinP = f32Temp_1 * g_Rated.f32OutputPower;

				f32Temp_1 = 0.01 * (int16)g_u16SpiRxBuf[11];
				g_GridManager.InvQ.f32LockoutP = f32Temp_1 * g_Rated.f32OutputPower;

				f32Temp_1 = 0.01 * (int16)g_u16SpiRxBuf[12];
				g_GridManager.InvQ.f32QvarMax = f32Temp_1 * g_Rated.f32OutputPower;

				g_GridManager.InvQ.f32QTime= g_u16SpiRxBuf[13] * 50;
				temp13_6 = g_u16SpiRxBuf[14];
				temp13_7 = g_u16SpiRxBuf[15];
				temp13_8 = g_u16SpiRxBuf[16];
				temp13_9 = g_u16SpiRxBuf[17];
				temp13_10 = g_u16SpiRxBuf[18];
				temp13_11 = g_u16SpiRxBuf[19];
				temp13_12 = g_u16SpiRxBuf[20];
				g_StateCheck.bit.ReactiveValue_UpdateOKflag2 = 1;
				SafetyUpdataFinishFlag = (SafetyUpdataFinishFlag|1<<7);
			break;

			case 0x38:
				g_SaftyConfig.Word.LVRTCon = GET_LBYTE_OF_WORD(g_u16SpiRxBuf[9]);
				g_GridManager.Lvrt.f32VLvrt = g_u16SpiRxBuf[10];
				g_GridManager.Lvrt.u16Vpoint1 = g_u16SpiRxBuf[11];
				g_GridManager.Lvrt.u16Tpoint1 = g_u16SpiRxBuf[12];
				g_GridManager.Lvrt.u16Vpoint2 = g_u16SpiRxBuf[13];
				g_GridManager.Lvrt.u16Tpoint2 = g_u16SpiRxBuf[14];
				g_GridManager.Lvrt.u16Vpoint3 = g_u16SpiRxBuf[15];
				g_GridManager.Lvrt.u16Tpoint3 = g_u16SpiRxBuf[16];
				g_GridManager.Lvrt.u16Vpoint4 = g_u16SpiRxBuf[17];
				g_GridManager.Lvrt.u16Tpoint4 = g_u16SpiRxBuf[18];
				g_GridManager.Lvrt.u16K = g_u16SpiRxBuf[19];
				g_GridManager.Lvrt.u16Tback= g_u16SpiRxBuf[20];
				g_GridManager.Lvrt.u16PLvrtback = g_u16SpiRxBuf[21];

				g_StateCheck.bit.LVRTValue_UpdateOKflag1 = 1;
				SafetyUpdataFinishFlag = (SafetyUpdataFinishFlag|1<<8);
			break;

			case 0x39:
				temp15_1 = g_u16SpiRxBuf[9];
				temp15_2 = g_u16SpiRxBuf[10];
				temp15_3 = g_u16SpiRxBuf[11];
				temp15_4 = g_u16SpiRxBuf[12];
				temp15_5 = g_u16SpiRxBuf[13];
				temp15_6 = g_u16SpiRxBuf[14];
				temp15_7 = g_u16SpiRxBuf[15];
				temp15_8 = g_u16SpiRxBuf[16];
				temp15_9 = g_u16SpiRxBuf[17];
				temp15_10 = g_u16SpiRxBuf[18];
				temp15_11 = g_u16SpiRxBuf[19];
				temp15_12 = g_u16SpiRxBuf[20];
				g_StateCheck.bit.LVRTValue_UpdateOKflag2 = 1;
				SafetyUpdataFinishFlag = (SafetyUpdataFinishFlag|1<<9);
			break;

			case 0x3A:
				g_SaftyConfig.Word.AICon = GET_LBYTE_OF_WORD(g_u16SpiRxBuf[9]);
				g_SaftyConfig.Word.GFCICon = GET_LBYTE_OF_WORD(g_u16SpiRxBuf[10]);
				g_SaftyConfig.Word.ISOCon = GET_LBYTE_OF_WORD(g_u16SpiRxBuf[11]);
				g_SafetyReg.u16Riso = g_u16SpiRxBuf[12];
				temp16_5 = g_u16SpiRxBuf[13];
				temp16_6 = g_u16SpiRxBuf[14];
				temp16_7 = g_u16SpiRxBuf[15];
				temp16_8 = g_u16SpiRxBuf[16];
				temp16_9 = g_u16SpiRxBuf[17];
				temp16_10 = g_u16SpiRxBuf[18];
				temp16_11 = g_u16SpiRxBuf[19];
				temp16_12 = g_u16SpiRxBuf[20];
				g_StateCheck.bit.SafetyIsoIslandandSoOn_UpdateOKflag = 1;
				SafetyUpdataFinishFlag = (SafetyUpdataFinishFlag|1<<10);
			break;
			case 0x3B:
				u8Temp_1 = GET_LBYTE_OF_WORD(g_u16SpiRxBuf[9]);
				g_MPPTState.Word.byte2 = u8Temp_1;//factory mode
				f32Temp_1 = GET_HBYTE_OF_WORD(g_u16SpiRxBuf[9]);
				f32Temp_1 = f32Temp_1 * 0.01 * g_Rated.f32OutputCurrentPeak;								
				g_SafetyReg.FactoryCurrentLimit = f32Temp_1;
				temp16_2 = g_u16SpiRxBuf[10];
				temp16_3 = g_u16SpiRxBuf[11];
				temp16_4 = g_u16SpiRxBuf[12];
				temp16_5 = g_u16SpiRxBuf[13];
				temp16_6 = g_u16SpiRxBuf[14];
				temp16_7 = g_u16SpiRxBuf[15];
				temp16_8 = g_u16SpiRxBuf[16];
				temp16_9 = g_u16SpiRxBuf[17];
				temp16_10 = g_u16SpiRxBuf[18];
				temp16_11 = g_u16SpiRxBuf[19];
				temp16_12 = g_u16SpiRxBuf[20];
				g_StateCheck.bit.FactoryMode_UpdateOkflag = 1;
				SafetyUpdataFinishFlag = (SafetyUpdataFinishFlag|1<<11);
			break;
			case 0x3C:
				f32Temp_1 = 0.001 * g_u16SpiRxBuf[9];
				g_CalibrationPara.f32KVPV1 = f32Temp_1;
				f32Temp_1 = g_ADConvert.Defaultgain.f32VPV1 * g_CalibrationPara.f32KVPV1;
				if((g_CalibrationPara.f32KVPV1>0.95)&&(g_CalibrationPara.f32KVPV1<1.05))
				{
					g_ADConvert.gain.f32VPV1 = f32Temp_1;
				}
				f32Temp_1 = 0.1 * (int16)g_u16SpiRxBuf[10];
				g_CalibrationPara.f32OFFSETVPV1 = f32Temp_1;
				if((g_CalibrationPara.f32OFFSETVPV1>-15)&&(g_CalibrationPara.f32OFFSETVPV1<15))
				{
					g_ADConvert.offset.f32VPV1 = f32Temp_1;
				}
				f32Temp_1 = 0.001 * g_u16SpiRxBuf[11];
				g_CalibrationPara.f32KIPV1 = f32Temp_1;
				f32Temp_1 = g_ADConvert.Defaultgain.f32IPV1 * g_CalibrationPara.f32KIPV1;
				if((g_CalibrationPara.f32KIPV1>0.95)&&(g_CalibrationPara.f32KIPV1<1.05))
				{
					g_ADConvert.gain.f32IPV1 = f32Temp_1;
				}
				f32Temp_1 = 0.01 * (int16)g_u16SpiRxBuf[12];
				g_CalibrationPara.f32OFFSETIPV1 = f32Temp_1;
				f32Temp_1 = 0.001 * g_u16SpiRxBuf[13];
				g_CalibrationPara.f32KPowerPV1 = f32Temp_1;
				g_CalibrationPara.f32OFFSETPowerPV1 = g_u16SpiRxBuf[14];

				f32Temp_1 = 0.001 * g_u16SpiRxBuf[15];
				g_CalibrationPara.f32KVPV2 = f32Temp_1;
				f32Temp_1 = g_ADConvert.Defaultgain.f32VPV2 * g_CalibrationPara.f32KVPV2;
				if((g_CalibrationPara.f32KVPV2>0.95)&&(g_CalibrationPara.f32KVPV2<1.05))
				{
					g_ADConvert.gain.f32VPV2 = f32Temp_1;
				}
				f32Temp_1 = 0.1 * (int16)g_u16SpiRxBuf[16];
				g_CalibrationPara.f32OFFSETVPV2 = f32Temp_1;
				if((g_CalibrationPara.f32OFFSETVPV2>-15)&&(g_CalibrationPara.f32OFFSETVPV2<15))
				{
					g_ADConvert.offset.f32VPV2 = f32Temp_1;
				}

				f32Temp_1 = 0.001 * g_u16SpiRxBuf[17];
				g_CalibrationPara.f32KIPV2 = f32Temp_1;
				f32Temp_1 = g_ADConvert.Defaultgain.f32IPV2 * g_CalibrationPara.f32KIPV2;
				if((g_CalibrationPara.f32KIPV2>0.95)&&(g_CalibrationPara.f32KIPV2<1.05))
				{
					g_ADConvert.gain.f32IPV2 = f32Temp_1;
				}
				f32Temp_1 = 0.01 * (int16)g_u16SpiRxBuf[18];
				g_CalibrationPara.f32OFFSETIPV2 = f32Temp_1;
				f32Temp_1 = 0.001 * g_u16SpiRxBuf[19];
				g_CalibrationPara.f32KPowerPV2 = f32Temp_1;
				g_CalibrationPara.f32OFFSETPowerPV2 = g_u16SpiRxBuf[20];

				f32Temp_1 = 0.001 * g_u16SpiRxBuf[21];
				g_CalibrationPara.f32KVBUS = f32Temp_1;
				f32Temp_1 = g_ADConvert.Defaultgain.f32VbusP * g_CalibrationPara.f32KVBUS;
				if((g_CalibrationPara.f32KVBUS>0.95)&&(g_CalibrationPara.f32KVBUS<1.05))
				{
					g_ADConvert.gain.f32VbusP = f32Temp_1;
					g_ADConvert.gain.f32VbusN = f32Temp_1;
				}

				f32Temp_1 = 0.05 * (int16)g_u16SpiRxBuf[22];
				g_CalibrationPara.f32OFFSETVBUS = f32Temp_1;
				if((g_CalibrationPara.f32OFFSETVBUS>-7)&&(g_CalibrationPara.f32OFFSETVBUS<7))
				{
					g_ADConvert.offset.f32VbusP = f32Temp_1;
					g_ADConvert.offset.f32VbusN = f32Temp_1;
				}

				f32Temp_1 = 0.001 * g_u16SpiRxBuf[23];
				g_CalibrationPara.f32KPowerOutput = f32Temp_1;
				g_CalibrationPara.f32OFFSETPowerOutput = (int16)g_u16SpiRxBuf[24];

				g_StateCheck.bit.CalibrationPara_UpdateOkflag1 = 1;
				SafetyUpdataFinishFlag = (SafetyUpdataFinishFlag|1<<12);
			break;
			case 0x3D:

				f32Temp_1 = 0.001 * g_u16SpiRxBuf[9];
				g_CalibrationPara.f32KGridVR = f32Temp_1;
				f32Temp_1 = g_ADConvert.Defaultgain.f32VGridR * g_CalibrationPara.f32KGridVR;
				if((g_CalibrationPara.f32KGridVR>0.95)&&(g_CalibrationPara.f32KGridVR<1.05))
				{
					g_ADConvert.gain.f32VGridR = f32Temp_1;
				}

				f32Temp_1 = 0.1 * (int16)g_u16SpiRxBuf[10];
				g_CalibrationPara.f32OFFSETGridVR = f32Temp_1;
				f32Temp_1 = 0.001 * g_u16SpiRxBuf[11];
				g_CalibrationPara.f32KGridIR = f32Temp_1;
				f32Temp_1 = g_ADConvert.Defaultgain.f32IGridR * g_CalibrationPara.f32KGridIR;
				if((g_CalibrationPara.f32KGridIR>0.95)&&(g_CalibrationPara.f32KGridIR<1.05))
				{
					g_ADConvert.gain.f32IGridR = f32Temp_1;
				}

				f32Temp_1 = 0.1 * (int16)g_u16SpiRxBuf[12];
				g_CalibrationPara.f32OFFSETGridIR = f32Temp_1;

				f32Temp_1 = 0.001 * g_u16SpiRxBuf[13];
				g_CalibrationPara.f32KGridVS = f32Temp_1;
				f32Temp_1 = g_ADConvert.Defaultgain.f32VGridS * g_CalibrationPara.f32KGridVS;
				if((g_CalibrationPara.f32KGridVS>0.95)&&(g_CalibrationPara.f32KGridVS<1.05))
				{
					g_ADConvert.gain.f32VGridS = f32Temp_1;
				}

				f32Temp_1 = 0.1 * (int16)g_u16SpiRxBuf[14];
				g_CalibrationPara.f32OFFSETGridVS = f32Temp_1;
				f32Temp_1 = 0.001 * g_u16SpiRxBuf[15];
				g_CalibrationPara.f32KGridIS = f32Temp_1;
				f32Temp_1 = g_ADConvert.Defaultgain.f32IGridS * g_CalibrationPara.f32KGridIS;
				if((g_CalibrationPara.f32KGridIS>0.95)&&(g_CalibrationPara.f32KGridIS<1.05))
				{
					g_ADConvert.gain.f32IGridS = f32Temp_1;
				}

				f32Temp_1 = 0.1 * (int16)g_u16SpiRxBuf[16];
				g_CalibrationPara.f32OFFSETGridIS = f32Temp_1;

				f32Temp_1 = 0.001 * g_u16SpiRxBuf[17];
				g_CalibrationPara.f32KGridVT = f32Temp_1;
				f32Temp_1 = g_ADConvert.Defaultgain.f32VGridT * g_CalibrationPara.f32KGridVT;
				if((g_CalibrationPara.f32KGridVT>0.95)&&(g_CalibrationPara.f32KGridVT<1.05))
				{
					g_ADConvert.gain.f32VGridT = f32Temp_1;
				}

				f32Temp_1 = 0.1 * (int16)g_u16SpiRxBuf[18];
				g_CalibrationPara.f32OFFSETGridVT = f32Temp_1;
				f32Temp_1 = 0.001 * g_u16SpiRxBuf[19];
				g_CalibrationPara.f32KGridIT = f32Temp_1;
				f32Temp_1 = g_ADConvert.Defaultgain.f32IGridT * g_CalibrationPara.f32KGridIT;
				if((g_CalibrationPara.f32KGridIT>0.95)&&(g_CalibrationPara.f32KGridIT<1.05))
				{
					g_ADConvert.gain.f32IGridT = f32Temp_1;
				}

				f32Temp_1 = 0.1 * (int16)g_u16SpiRxBuf[20];
				g_CalibrationPara.f32OFFSETGridIT = f32Temp_1;

				f32Temp_1 = 0.001 * g_u16SpiRxBuf[21];
				g_CalibrationPara.f32SlaveKVPV1 = f32Temp_1;
				f32Temp_1 = 0.1 * (int16)g_u16SpiRxBuf[22];
				g_CalibrationPara.f32SlaveOFFSETVPV1 = f32Temp_1;
				f32Temp_1 = 0.001 * g_u16SpiRxBuf[23];
				g_CalibrationPara.f32SlaveKVPV2 = f32Temp_1;
				f32Temp_1 = 0.1 * (int16)g_u16SpiRxBuf[24];
				g_CalibrationPara.f32SlaveOFFSETVPV2 = f32Temp_1;

				g_StateCheck.bit.CalibrationPara_UpdateOkflag2 = 1;
				SafetyUpdataFinishFlag = (SafetyUpdataFinishFlag|1<<13);
			break;
			default:
			break;
		}
		if(0x3FFF==SafetyUpdataFinishFlag)
		{
			g_StateCheck.bit.SafetySetOK = 1;
		}
		g_SysCurrentStateSlave = (enum SysState)g_u16SpiRxBuf[25];
//		g_u16SlaveStateCheckByte3 = g_u16SpiRxBuf[26];
//		g_u16SlaveMessageFaultByte7 = g_u16SpiRxBuf[27];
		g_i16_Temperature_Inv = g_u16SpiRxBuf[28];
		g_i16_Temperature_Amb = g_u16SpiRxBuf[29];
		g_i16_Temperature_Boost = g_u16SpiRxBuf[30];
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
/*void ConnectPapaCheck(void)
{	
	int16 temp = 0;
	if(g_SafetyConnect.f32VGridMaxLimitBack > 300)
	{
		temp++;
	}
	if(g_SafetyConnect.f32VGridMinLimitBack > 300)
	{
		temp++;
	}
	if(g_SafetyConnect.f32VGridMaxLimitBack < 100)
	{
		temp++;
	}
	if(g_SafetyConnect.f32VGridMinLimitBack < 100)
	{
		temp++;
	}
	if(g_SafetyConnect.f32FGridMaxLimitBack > 70)
	{
		temp++;
	}
	if(g_SafetyConnect.f32FGridMinLimitBack > 70)
	{
		temp++;
	}
	if(g_SafetyConnect.f32FGridMaxLimitBack < 40)
	{
		temp++;
	}
	if(g_SafetyConnect.f32FGridMinLimitBack < 40)
	{
		temp++;
	}
	if(0 == temp)
	{
		g_StateCheck.bit.SafetySTART_UpdateOKflag = 1;
		SafetyUpdataFinishFlag = (SafetyUpdataFinishFlag|1);
	}
}*/
//===========================================================================
// End of file.
//===========================================================================

