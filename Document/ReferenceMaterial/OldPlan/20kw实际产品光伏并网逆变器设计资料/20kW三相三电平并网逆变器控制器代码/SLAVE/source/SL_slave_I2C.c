//###########################################################################
//
// FILE:	SL_slave_I2C.c
//
// TITLE:	DSP2803x SCI Initialization & Support Functions.
//
//###########################################################################

#include "SL_slave.h"


Uint16 recoverdefaultsafety(Uint16 country);
//extern Uint16 EnergyTodayOld;
Uint16 MPPTMode = 0;
void  ClearEEPROM(void);

Uint16 I2Cerror = 0;

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

Uint16 Todaydate = 0;
Uint16 SafetyCOMMCountry = 0;
Uint16 ILrmsMPPTlimit = 10;
Uint16 g_StateCheck = 0;
//Uint8 Updataflg = 0x00;
Uint16 temp_ad = 0;
// Global variables
//I2cMsgOut is used in writing EEPROM
TypeI2cMsg I2cMsgOut =	{	I2C_MSGSTAT_INACTIVE,
							I2C_SLAVE_ADDR,
							I2C_NUMBYTES,
							I2C_EEPROM_HIGH_ADDR,
							I2C_EEPROM_LOW_ADDR,
						};
//I2cMsgIn is used in reading EEPROM
TypeI2cMsg I2cMsgIn = 	{	I2C_MSGSTAT_INACTIVE,
							I2C_SLAVE_ADDR,
							I2C_NUMBYTES,
							I2C_EEPROM_HIGH_ADDR,
							I2C_EEPROM_LOW_ADDR,
						};

TypeI2cMsg *CurrentMsgPtr;				// Used in interrupts
Uint16 u16IntSource;
volatile Uint16 u16I2cIsrError;	//used in ISR, record process result.

volatile Uint16 u16I2CErrorCode = 0;	// only for test
/*=============================================================================*
 * FUNCTION: 
 *		I2C_ConvAddr()
 *
 * PURPOSE : 
 *		Decompose EEPROM address to block address and byte address.
 *
 * INPUT: 
 *		u16I2cAddr----EEPROM address, from 0 to 0x7FE.
 *		pRtI2cAddr----pointer of a TypeI2cAddr variable, contains the result.
 *
 * RETURN: 
 *     void
 *
 * CALLS: 
 *     void
 *
 * CALLED BY: 
 *		I2C.c
 * 
 *============================================================================*/
void I2C_ConvAddr(Uint16 u16I2cAddr, TypeI2cAddr* pRtI2cAddr)
{
	pRtI2cAddr->u16ByteAddr  = u16I2cAddr & 0x00FF;
	pRtI2cAddr->u16BlockAddr = (u16I2cAddr & 0xFF00)>>8;
}

/*=============================================================================*
 * FUNCTION: 
 *		InitI2C()
 *
 * PURPOSE : 
 *		Initialize the I2C module of TMS320F28335:
 *		100KHz. Clock Pulse Width High: 4.7us, Clock Pulse Width Low: 5.3us
 *		Only enable SCD(Stop Condition Detected) and ARDY(Register Access Ready)
 *	interrupts.
 *		Take I2C out of reset.
 *		Enable FIFO mode.
 *		Enable TXFIFO, clear TXFFINT, disable TXFFINT.
 *		Enable RXFIFO, clear RXFFINT, disable RXFFINT.
 *
 * INPUT: 
 *     void
 *
 * RETURN: 
 *     void
 *
 * CALLS: 
 *     void
 *
 * CALLED BY: 
 *     Main.c  
 * 
 *============================================================================*/
void InitI2C(void)
{
	I2caRegs.I2CMDR.bit.IRS = 0;
	DELAY_US(1000);

	I2caRegs.I2CPSC.all = 5;  				// Prescaler - need 7-12 Mhz on module clk (60/6 = 10MHz)  

	I2caRegs.I2CCLKL = 48;					// 48+5=53, tcl=5.3us
	I2caRegs.I2CCLKH = 42;					// 42+5=47, tch=4.7us, and fscl=100KHz(M24128 MAX Fscl=400KHz)
	I2caRegs.I2CIER.all = 0x24;				// Enable SCD & ARDY interrupts
	
	I2caRegs.I2CFFTX.bit.TXFFRST = 0;
	I2caRegs.I2CFFTX.all = 0x6040;			// Enable FIFO mode and TXFIFO, clear TXFFINT, disable TXFFINT, set TX FIFO INT level to 0

	I2caRegs.I2CFFRX.bit.RXFFIL = 4;
	I2caRegs.I2CFFRX.bit.RXFFRST = 0;		// reset RXFIFO
	I2caRegs.I2CFFRX.all = 0x2044;			// Enable RXFIFO, clear RXFFINT, set RX FIFO INT Level to 0
	I2caRegs.I2CFFRX.bit.RXFFIENA = 1;		// Enable RXFFINT

	I2caRegs.I2CMDR.all = 0x0020;			// Take I2C out of reset
											// Stop I2C when suspended
	I2caRegs.I2CFFTX.bit.TXFFINTCLR = 1;
	I2caRegs.I2CFFRX.bit.RXFFINTCLR = 1;										
	
	u16IntSource = I2C_NO_ISRC;
	u16I2cIsrError = I2C_SUCCESS;

	PieCtrlRegs.PIEIER8.bit.INTx1 = 1;
	IER |= M_INT8;

	GpioDataRegs.GPBCLEAR.bit.GPIO34  = 1;	//
}
/*=============================================================================*
 * FUNCTION: 
 *		ReleaseI2CBusDeadLock()
 *
 * PURPOSE : 
 *		Take I2C bus out of dead lock.
 *
 * INPUT: 
 *     void
 *
 * RETURN: 
 *     I2C_ERROR------fail
 *	   I2C_SUCCESS----success!
 *
 * CALLS: 
 *     DelayUs()
 *
 * CALLED BY: 
 *     Main.c  
 * 
 *============================================================================*/
Uint16 ReleaseI2CBusDeadLock(void)
{
	Uint16 i;

	if(0 == GpioDataRegs.GPBDAT.bit.GPIO32)	// need to release I2C bus lock
	{
		/* switch SCL mode to GPIO output mode*/
		EALLOW;
		GpioDataRegs.GPBSET.bit.GPIO33 = 1;		// HIGH
		GpioCtrlRegs.GPBDIR.bit.GPIO33 = 1;		// output
		GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 0;	// GPIO
		EDIS;
		/* make 9 SCL clocks*/
		for(i=0; i<9; i++)
		{
			DELAY_US(3);	// delay for 4.6uS
			GpioDataRegs.GPBTOGGLE.bit.GPIO33 = 1;
			DELAY_US(3);	// delay for 4.6uS
			GpioDataRegs.GPBTOGGLE.bit.GPIO33 = 1;
		}
		/* return to SCL mode*/
		EALLOW;
		GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 1;	// SCLA
		EDIS;
	}
	else
		return I2C_SUCCESS;

	DELAY_US(20000);	// delay for about 30mS

	if(1 == GpioDataRegs.GPBDAT.bit.GPIO32)
		return I2C_SUCCESS;		// success! jump out
	else
		return I2C_ERROR;		// not success yet, try again!
}
/*=============================================================================*
 * FUNCTION: 
 *		I2C_Reset()
 *
 * PURPOSE : 
 *		Reset 28335 I2C Module.
 *		Then, if I2CSTR[BB] is 1 (not busy), initialize I2C module.
 *
 * INPUT: 
 *		void
 *
 * RETURN: 
 *		Uint16 type value:
 *		I2C_ERROR----after reset, I2CSTR[BB] remains 0 (i.e, still busy)
 *		I2C_SUCCESS--after reset, I2CSTR[BB] is 1 (i.e, not busy)
 *
 * CALLS: 
 *		InitI2C()
 *		DELAY_US()
 *
 * CALLED BY: 
 *     Main.c  
 * 
 *============================================================================*/
Uint16 I2C_Reset(void)
{
	I2caRegs.I2CMDR.bit.STP = 1;
	TSK_sleep(2);
	I2caRegs.I2CMDR.bit.IRS = 0;
	TSK_sleep(2);
	I2caRegs.I2CMDR.bit.IRS = 1;
	TSK_sleep(2);

	if(0 == I2caRegs.I2CSTR.bit.BB)
	{
		InitI2C();
		return I2C_SUCCESS;
	}
	else
	{
		return I2C_ERROR;
	}
}

/*=============================================================================*
 * FUNCTION: 
 *		I2C_PollACK()
 *
 * PURPOSE : 
 *		Check if M24128 EEPROM is ready for next operation.
 *		Minimize system delay by Polling On ACK.
 *
 * INPUT: 
 *		TypeI2cMsg *msg: Address of a TypeI2cMsg type structure.
 *		It should be address of I2cMsgIn.
 *
 * RETURN: 
 *		Uint16 type value:
 *		I2C_ERROR----FATAL ERROR, caused by I2C module cannot enter ISR, or
 *	I2C module can not ACK after 100 times retry. when return this, Inverter
 *	must be Power on again.
 *		I2C_SUCCESS--M24128 is ready for next operation now.
 *
 * CALLS: 
 *		SEM_pendBinary()
 *
 * CALLED BY: 
 *     Main.c  
 *
 * MORE INFORMATION:
 *		After "I2C_Write_Page()" ends, I2cMsgOut.MsgStatus is
 *	I2C_MSGSTAT_INACTIVE.
 *
 *		Test shows that when executed in FLASH, one most out loop consumes 
 *	200uS when ACK is lost. As the longest write time is 5ms(25*200us),
 *	25 loops is ok. For enough margin, 100 loops is applied.
 *============================================================================*/
Uint16 I2C_PollACK(TypeI2cMsg *msg)
{
	TypeI2cMsg *pCurMsgPtrTmp;
	Uint16 u16Error;
	Uint16 i;

	pCurMsgPtrTmp = CurrentMsgPtr;
	CurrentMsgPtr = msg;

	for(i=0; i<100; i++)
	{	//---- START+DEVICE SELECT
		msg->MsgStatus = I2C_MSGSTAT_SEND_NOSTOP_BUSY;
		I2caRegs.I2CSAR = msg->SlaveAddress;	
		I2caRegs.I2CCNT = 2;
		I2caRegs.I2CDXR = msg->MemoryHighAddr;
		I2caRegs.I2CDXR = msg->MemoryLowAddr;
		I2caRegs.I2CMDR.all = 0x2620;				//dummy WRITE

		// 1--- WAIT FOR ISR RETRUN
		u16Error = SEM_pendBinary(&SEM_I2C_isr, I2C_MAX_MS_SEMPEND);

		// 2_1--dummy WRITE ISR returns
		if(u16Error)				
		{	// 3_1--dummy WRITE---NACK
			if(I2C_NACK_ERROR == u16I2cIsrError)//STOP will be set in ISR			
			{	//wait for dummy WRITE---NACK----STOP ISR returns SCD
				u16Error = SEM_pendBinary(&SEM_I2C_isr, I2C_MAX_MS_SEMPEND);
				// 3_1--4_1--dummy WRITE---NACK----STOP ISR returns SCD
				if(u16Error)
				{
					if(I2caRegs.I2CFFTX.bit.TXFFST)	//clear TXFFST
					{	
						I2caRegs.I2CFFTX.bit.TXFFRST = 0;
						while(I2caRegs.I2CFFTX.bit.TXFFST);
						I2caRegs.I2CFFTX.bit.TXFFRST = 1;
					}
					continue;
				}
				// 3_1--4_2--dummy WRITE---NACK----STOP not enter ISR
				else
				{
					if(I2C_MSGSTAT_SEND_NOSTOP == CurrentMsgPtr->MsgStatus)
					{
						if(I2caRegs.I2CFFTX.bit.TXFFST)	//clear TXFFST
						{	
							I2caRegs.I2CFFTX.bit.TXFFRST = 0;
							while(I2caRegs.I2CFFTX.bit.TXFFST);
							I2caRegs.I2CFFTX.bit.TXFFRST = 1;
						}
						continue;
					}
					else
					{
						CurrentMsgPtr = pCurMsgPtrTmp;
						u16I2CErrorCode = 1;
						return I2C_ERROR;
					}
				}
			}
			// 3_2--dummy WRITE---OK
			else if(I2C_MSGSTAT_RESTART == msg->MsgStatus)
			{	
				msg->MsgStatus = I2C_MSGSTAT_READ_BUSY;
				I2caRegs.I2CSAR = msg->SlaveAddress;	
				I2caRegs.I2CCNT = 1;
				I2caRegs.I2CMDR.all = 0x2C20;		// RESTART
				
				u16Error = SEM_pendBinary(&SEM_I2C_isr, I2C_MAX_MS_SEMPEND);

				// 3_2--4_1--dummy WRITE---OK----RESTART ISR returns SCD
				if(u16Error)		
				{	// 3_2--4_1--dummy WRITE---OK----RESTART----OK
					if(I2C_MSGSTAT_INACTIVE == msg->MsgStatus)	
					{
						CurrentMsgPtr = pCurMsgPtrTmp;
						return I2C_SUCCESS;
					}
					// 3_2--4_1--dummy WRITE---OK----RESTART----OTHER
					else
					{
						continue;
					}
				}
				// 3_2--4_2--dummy WRITE---OK----RESTART not enter ISR
				else
				{
					CurrentMsgPtr = pCurMsgPtrTmp;
					u16I2CErrorCode = 2;
					return I2C_ERROR;
				}
			}
			// 3_3--dummy WRITE---OTHER
			else
			{
				continue;
			}
		}
		// 2_2--dummy WRITE not enter ISR, FATAL ERROR, must be power on again
		else
		{
			CurrentMsgPtr = pCurMsgPtrTmp;
			u16I2CErrorCode = 3;
			return I2C_ERROR;
		}
	}

	// no ACK after 100 times retry, FATAL ERROR, must be power on again
	CurrentMsgPtr = pCurMsgPtrTmp;
	u16I2CErrorCode = 4;
	return I2C_ERROR;
}

/*=============================================================================*
 * FUNCTION: 
 *		I2C_Write_InByte()
 *
 * PURPOSE : 
 *		Provide EEPROM write interface to other threads.
 *		Analyze EEPROM address and data amount to be wrote in, split write zone
 *	in page of M24128, then call "I2C_Write_Page()" to complete every row writing.
 *		
 * INPUT: 
 *		Uint16 u16StartAddr----EEPROM address to be wrote in,
 *		Uint16* pSrcAddr-------address of data to be wrote in, each Uint16-type
 * memory contains 1 byte data in 8 LSBs.
 *		Uint16 u16ByteNum------byte amount of data to be wrote in.
 *
 * RETURN: 
 *		Uint16 type value:
 *		I2C_W_R_OK
 *		I2C_W_R_EXCEED_LIMIT
 *		I2C_W_R_DATA_AMOUNT_IS_ZERO
 *		I2C_W_FAILED
 *
 * CALLS: 
 *		I2C_ConvAddr()
 *		I2C_Write_Page()
 *		I2C_PollACK()
 *
 * CALLED BY: 
 *		Main.c  
 * 
 * MORE INFORMATION:
 *		Internal I2C error check and retry is handled by I2C_Write_Page().
 *		Every time after I2C_Write_Page() ends, it calls I2C_PollACK() to wait
 *	for the completion of write.
 *		This function has all data written.
 *============================================================================*/
Uint16 I2C_Write_InByte(  Uint16 u16StartAddr, 
                          Uint16* pSrcAddr, 
                          Uint16 u16ByteNum    )
{
	Uint16 u16Addr_cur;
	Uint16 u16PageAddr_next;
	Uint16 u16ByteNum_remain;
	Uint16 u16EachNum;
	Uint16 u16Error;
	TypeI2cAddr i2cStartAddr;
	
	//check if u16StartAddr is a valid M24128 address
	if ((u16StartAddr + u16ByteNum) >= M24128_ADDR_LIMIT)
	{
		return I2C_W_R_EXCEED_LIMIT;	//Error: write area exceeds EEPROM limit
	}
	else if (0 == u16ByteNum)
	{
		return I2C_W_R_DATA_AMOUNT_IS_ZERO;	//Error: byte of data amount is 0.
	}

	LCK_pend(&LCK_EEPROM, SYS_FOREVER);
	//every while loop writes a PAGE (upto 64 bytes),
	//u16ByteNum_remain shows how many bytes are left. When it is 0, jump out.
	CurrentMsgPtr = &I2cMsgOut;
	u16Addr_cur = u16StartAddr;
	u16ByteNum_remain = u16ByteNum;
	I2cMsgOut.p_MsgBuffer = pSrcAddr;
	
	while(u16ByteNum_remain > 0)
	{
		u16PageAddr_next = \
			(u16Addr_cur & M24128_ADDR_PAGE_MASK) + M24128_ADDR_PAGE_LEN;
		u16EachNum = u16PageAddr_next - u16Addr_cur;
		
		if(u16ByteNum_remain < u16EachNum)
		{
			u16EachNum = u16ByteNum_remain;
			u16ByteNum_remain = 0;
		}
		else
		{
			u16ByteNum_remain -= u16EachNum;
		}
		
		I2C_ConvAddr(u16Addr_cur, &i2cStartAddr);
		I2cMsgOut.SlaveAddress = I2C_SLAVE_ADDR;
		I2cMsgOut.NumOfBytes = u16EachNum;
		I2cMsgOut.MemoryHighAddr= i2cStartAddr.u16BlockAddr;
		I2cMsgOut.MemoryLowAddr = i2cStartAddr.u16ByteAddr;
		
		u16Error = I2C_Write_Page(&I2cMsgOut);
		
		if(I2C_SUCCESS == u16Error)
		{	//wait for M24128 completing internal write.
			u16Error = I2C_PollACK(&I2cMsgIn);
			if(I2C_ERROR == u16Error)
			{
				LCK_post(&LCK_EEPROM);
				return I2C_W_FAILED;	//WRITE EEPROM FAILED
			}
			else
			{	
				if(u16ByteNum_remain > 0)
				{
					u16Addr_cur += u16EachNum;
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			LCK_post(&LCK_EEPROM);
			return I2C_W_FAILED;	//WRITE EEPROM FAILED
		}
	}
	LCK_post(&LCK_EEPROM);
	return I2C_W_R_OK;
}

/*=============================================================================*
 * FUNCTION: 
 *		I2C_Write_InWord()
 *
 * PURPOSE : 
 *		Provide EEPROM write interface to other threads.
 *		Analyze EEPROM address and data amount to be wrote in, split write zone
 *	in page of M24128, then call "I2C_Write_Page()" to complete every row writing.
 *		
 * INPUT: 
 *		Uint16 u16StartAddr----EEPROM address to be wrote in.
 *		Uint16* pSrcAddr-------address of data to be wrote in.
 *		Uint16 u16WordNum------word amount of data to be wrote in.
 *
 * RETURN: 
 *		Uint16 type value:
 *		I2C_W_R_OK
 *		I2C_W_R_EXCEED_LIMIT
 *		I2C_W_R_DATA_AMOUNT_IS_ZERO
 *		I2C_W_FAILED
 *
 * CALLS: 
 *		I2C_ConvAddr()
 *		I2C_Write_Page()
 *		I2C_PollACK()
 *
 * CALLED BY: 
 *		Main.c  
 * 
 * MORE INFORMATION:
 *		Internal I2C error check and retry is handled by I2C_Write_Page().
 *		Every time after I2C_Write_Page() ends, it calls I2C_PollACK() to wait
 *	for the completion of write.
 *		This function has all data written.
 *============================================================================*/
Uint16 I2C_Write_InWord(  Uint16 u16StartAddr, 
                          Uint16* pSrcAddr, 
                          Uint16 u16WordNum   )
{
	Uint16 i;
	Uint16 u16Addr_cur;
	Uint16 u16PageAddr_next;
	Uint16 u16WordNum_remain;
	Uint16 u16EachNum;
	Uint16 u16Error;
	TypeI2cAddr i2cStartAddr;
	Uint16 data[I2C_PAGE_SIZE];
	
	//check if u16StartAddr is a valid M24128 address
	if ((u16StartAddr + u16WordNum<<1) >= M24128_ADDR_LIMIT)
	{
		return I2C_W_R_EXCEED_LIMIT;	//Error: write area exceeds EEPROM limit
	}
	else if (0 == u16WordNum)
	{
		return I2C_W_R_DATA_AMOUNT_IS_ZERO;	//Error: byte of data amount is 0.
	}

	LCK_pend(&LCK_EEPROM, SYS_FOREVER);	// 2012-05-10
	
	for(i = 0; i < I2C_PAGE_SIZE; i++)
	{
		data[i] = 0;
	}

	// LCK_pend(&LCK_EEPROM, SYS_FOREVER);	2012-05-10
	
	//every while loop writes a PAGE (upto 64 bytes or 32 words),
	//u16WordNum_remain shows how many words are left. When it is 0, jump out.
	CurrentMsgPtr = &I2cMsgOut;
	u16Addr_cur = u16StartAddr;
	u16WordNum_remain = u16WordNum;
	
	while(u16WordNum_remain > 0)
	{
		u16PageAddr_next = \
			(u16Addr_cur & M24128_ADDR_PAGE_MASK) + M24128_ADDR_PAGE_LEN;
		u16EachNum = (u16PageAddr_next - u16Addr_cur) >> 1;
			
		if(u16WordNum_remain < u16EachNum)
		{
			u16EachNum = u16WordNum_remain;
			u16WordNum_remain = 0;
		}
		else
		{
			u16WordNum_remain -= u16EachNum;
		}
		
		I2C_ConvAddr(u16Addr_cur, &i2cStartAddr);
		I2cMsgOut.SlaveAddress = I2C_SLAVE_ADDR;
		I2cMsgOut.NumOfBytes = u16EachNum<<1;
		I2cMsgOut.MemoryHighAddr = i2cStartAddr.u16BlockAddr;
		I2cMsgOut.MemoryLowAddr = i2cStartAddr.u16ByteAddr;
		
		for(i = 0; i < I2cMsgOut.NumOfBytes; i += 2)
		{	//low address storage lower 8 bits
			//high address storage higher 8 bits
			data[i]   =  (*pSrcAddr) & 0x00FF;
			data[i+1] = ((*pSrcAddr) & 0xFF00)>>8;
			pSrcAddr++;
		}
		I2cMsgOut.p_MsgBuffer = data;
		u16Error = I2C_Write_Page(&I2cMsgOut);
			
		if(I2C_SUCCESS == u16Error)
		{	//wait for M24128 completing internal write.
			u16Error = I2C_PollACK(&I2cMsgIn);
			if(I2C_ERROR == u16Error)
			{
				LCK_post(&LCK_EEPROM);
				return I2C_W_FAILED;	//WRITE EEPROM FAILED
			}
			else
			{	
				if(u16WordNum_remain > 0)
				{
					u16Addr_cur += (u16EachNum<<1);
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			LCK_post(&LCK_EEPROM);
			return I2C_W_FAILED;	//WRITE EEPROM FAILED
		}
	}
	LCK_post(&LCK_EEPROM);
	return I2C_W_R_OK;
}

/*=============================================================================*
 * FUNCTION: 
 *		I2C_Write_Page()
 *		
 * PURPOSE : 
 *		Write a Page.
 *		M24128's Page Write mode allows up to 64 bytes to be written in a single
 *	write cycle, provided that they are all located in the same 'row' in memory:
 *	that is, the most significant memory address bits (b13-b6) are the same.
 *
 * INPUT: 
 *		TypeI2cMsg *msg----Address of a TypeI2cMsg type structure.
 *		It should be address of I2cMsgOut.
 *
 * RETURN: 
 *		Uint16 type value:
 *		I2C_SUCCESS
 *		I2C_ERROR
 *
 * CALLS: 
 *		I2C_Reset()
 *		I2C_WriteData()
 *		SEM_pendBinary()
 *
 * CALLED BY: 
 *		I2C_Write()
 *		
 * MORE INFORMATION:
 *		Destination address and source data are ready in I2cMsgOut before it is 
 *	called.
 *		It Handles Internal I2C error check and retry operation.
 *============================================================================*/
Uint16 I2C_Write_Page(TypeI2cMsg *msg)
{
	Uint16 u16Error;
	Uint16 u16RetryTimes_PageW;	//Page WRITE retry times
	Uint16 u16RetryTimes_Reset;	//I2C Reset retry times
	Uint16 u16ResetFlag;		//I2C_NO--not need RESET, I2C_YES--need RESET
	
	Uint16 num_of_bytes;

	u16RetryTimes_PageW = 0;
	u16RetryTimes_Reset = 0;
	u16ResetFlag = I2C_NO;

	num_of_bytes = msg->NumOfBytes;
	
	if (msg->MsgStatus != I2C_MSGSTAT_INACTIVE)
	{
		u16Error = I2C_Reset();
		if(I2C_SUCCESS == u16Error)
		{
			msg->MsgStatus = I2C_MSGSTAT_INACTIVE;
		}
		else
		{
			return I2C_ERROR;
		}
	}
	
	msg->MsgStatus = I2C_MSGSTAT_SEND_WITHSTOP;
	
	while(u16RetryTimes_Reset < I2C_MAX_TIMES_RESET)
	{	//--------dummy WRITE--------
		while(u16RetryTimes_PageW < I2C_MAX_TIMES_PAGE_WRITE)
		{	
			msg->NumOfBytes = num_of_bytes;
			u16Error = I2C_WriteData(msg);
			
			if(I2C_SUCCESS != u16Error)
			{	//retry WRITE
				u16RetryTimes_PageW++;
				continue;
			}
			else
			{	
				u16Error = SEM_pendBinary(&SEM_I2C_isr, I2C_MAX_MS_SEMPEND);
				
				if(u16Error)
				{
					if(I2C_SUCCESS == u16I2cIsrError)
					{
						if(I2C_MSGSTAT_INACTIVE == msg->MsgStatus)
						{	//write success! jump out.
							u16RetryTimes_PageW = 0;
							break;
						}
						else
						{	//write failed! retry WRITE.
							u16RetryTimes_PageW++;
							continue;
						}
					}
					else if(I2C_NACK_ERROR == u16I2cIsrError)
					{	// SWI will generate a STOP, then a SCD will happen.
						u16Error = SEM_pendBinary(&SEM_I2C_isr, \
													I2C_MAX_MS_SEMPEND);
						if(u16Error)
						{	
							u16RetryTimes_PageW++;	//retry WRITE.
							continue;
						}
						else
						{	//FATAL ERROR: can't enter ISR
							msg->MsgStatus = I2C_MSGSTAT_INACTIVE;
							return I2C_ERROR;
						}
					}
					else	//----illegal interrupt source
					{	//enable I2C Reset
						u16ResetFlag = I2C_YES;
						break;
					}
				}
				else
				{	//FATAL ERROR: can't enter ISR
					msg->MsgStatus = I2C_MSGSTAT_INACTIVE;
					return I2C_ERROR;
				}
			}
		}	//----WRITE at most I2C_MAX_TIMES_PAGE_WRITE time ends here!
		
		if(u16RetryTimes_PageW >= I2C_MAX_TIMES_PAGE_WRITE)
		{	//set flag for I2C Reset
			u16ResetFlag = I2C_YES;
		}
		else
		{
			if(I2C_NO == u16ResetFlag)
			{
				break;
			}
		}

		//--------RESET I2C--------
		if(I2C_YES == u16ResetFlag)
		{
			u16ResetFlag = I2C_NO;
			u16RetryTimes_Reset++;
			u16RetryTimes_PageW = 0;
			
			u16Error = I2C_Reset();
			
			if(I2C_ERROR == u16Error)
			{
				msg->MsgStatus = I2C_MSGSTAT_INACTIVE;
				SEM_pendBinary(&SEM_I2C_isr, 0);	//clear unexpected SEM
				return I2C_ERROR;
			}
			else
			{
				msg->MsgStatus = I2C_MSGSTAT_SEND_WITHSTOP;
				continue;
			}
		}
	}	//----I2C Reset I2C_MAX_TIMES_RESET times end here!
	
	u16RetryTimes_Reset = 0;
	u16RetryTimes_PageW = 0;

	SEM_pendBinary(&SEM_I2C_isr, 0);	//clear unexpected SEM

	if(u16RetryTimes_Reset < I2C_MAX_TIMES_RESET)
	{
		if(I2C_MSGSTAT_INACTIVE == msg->MsgStatus)
		{
			return I2C_SUCCESS;
		}
		else
		{
			msg->MsgStatus = I2C_MSGSTAT_INACTIVE;
			return I2C_ERROR;
		}
	}
	else
	{
		msg->MsgStatus = I2C_MSGSTAT_INACTIVE;
		return I2C_ERROR;
	}
}

/*=============================================================================*
 * FUNCTION: 
 *		I2C_WriteData()
 *
 * PURPOSE : 
 *		Operate registers of I2C Module to execute data writing.
 *
 * INPUT: 
 *		TypeI2cMsg *msg----Address of a TypeI2cMsg type structure.
 *		It should be address of I2cMsgOut.
 *
 * RETURN: 
 *		Uint16 type value:
 *		I2C_STP_NOT_READY_ERROR
 *		I2C_BUS_BUSY_ERROR
 *		I2C_SUCCESS
 *
 * CALLS: 
 *		void
 *
 * CALLED BY: 
 *		I2C_Write_Page()
 *		
 *============================================================================*/
Uint16 I2C_WriteData(TypeI2cMsg *msg)
{
	Uint16 i, data_cnt;

	// Wait until the STP bit is cleared from any previous master communication.
	// Clearing of this bit by the module is delayed until after the SCD bit is
	// set. If this bit is not checked prior to initiating a new message, the
	// I2C could get confused.
	if (1 == I2caRegs.I2CMDR.bit.STP)
	{
		return I2C_STP_NOT_READY_ERROR;
	}
	else if (1 == I2caRegs.I2CSTR.bit.BB)
	{	// Check if bus busy
		return I2C_BUS_BUSY_ERROR;
	}
	else
	{	
		if(I2caRegs.I2CFFTX.bit.TXFFST)	//clear TXFIFO
		{	
			I2caRegs.I2CFFTX.bit.TXFFRST = 0;
			while(I2caRegs.I2CFFTX.bit.TXFFST);
			I2caRegs.I2CFFTX.bit.TXFFRST = 1;
		}

		msg->MsgStatus = I2C_MSGSTAT_WRITE_BUSY;
		// Setup slave address
		I2caRegs.I2CSAR = msg->SlaveAddress;
		// Setup data to send
		I2caRegs.I2CDXR = msg->MemoryHighAddr;
		I2caRegs.I2CDXR = msg->MemoryLowAddr; 
		// Setup number of bytes to send			// 2011-12-23 multi bytes write
		if(msg->NumOfBytes <= I2C_MAX_BUFFER_SIZE-2)// 2 address bytes
		{
			data_cnt = msg->NumOfBytes;
		}
		else
		{
			//msg->p_MsgBuffer = msg->MsgBuffer + 2;
			data_cnt = I2C_MAX_BUFFER_SIZE - 2;
		}
		for (i=0; i<data_cnt; i++)                                    
		{                                         
			//I2caRegs.I2CDXR = *(msg->MsgBuffer+i); 
			I2caRegs.I2CDXR = *(msg->p_MsgBuffer);
			msg->p_MsgBuffer++;
		}
		msg->NumOfBytes -= data_cnt;
		I2caRegs.I2CMDR.all = 0x66A0;	// with start, no stop, with RM
		//bit15,   0,   NACKMOD
		//bit14,   1,   FREE 	when encounter a breakpoint: 0=stop immediately;
// 1=run free
		//bit13,   1,   STT		1=generate a START condition. The combination of
// RM, STT and STP see Table 6
		//bit12,   0,   reserved

		//bit11,   0,   STP		1=generate a STOP condition, see Table 6
		//bit10,   1,   MST		0=slave, 1=master
		//bit9,    1,   TRX		0=receiver, 1=transmitter
		//bit8,    0,   XA		0=7 bit address, 1=10 bit address

		//bit7,    1,   RM		0=Nonrepeat mode, 1=Repeat mode
		//bit6,    0,   DLB		0=disable digital loopback, 1=enable
		//bit5,    1,   IRS		0=reset/disable I2C module, 1=enable
		//bit4,    0,   STB		0=not in START byte mode, 1=in. This bit is to 
//help a slave that need extra time to detect START condition

		//bit3,    0,   FDF		0=disable free data mode, 1=enable
		//bit2-0,  000, BC		000=8 bits per data byte
		//						001=1 bits per data byte
		//						010=2 bits per data byte
		//						011=3 bits per data byte
		//						100=4 bits per data byte
		//						101=5 bits per data byte
		//						110=6 bits per data byte
		//						111=7 bits per data byte
	}
	return I2C_SUCCESS;
}

/*=============================================================================*
 * FUNCTION: 
 *		I2C_Read_InByte()
 *
 * PURPOSE : 
 *		Provide EEPROM read interface to other threads.
 *		Analyze EEPROM address and data amount to be read, split read zone with
 *	4 bytes as one groups, then call "I2C_Read_MultiByte()" to complete every 
 *	group reading.
 *
 * INPUT: 
 *		Uint16 u16StartAddr----EEPROM address start to be read out,
 *		Uint16* pSrcAddr-------storage address of data be read out, each Uint16-
 * type memory contains 1 byte data in 8 LSBs.
 *		Uint16 u16ByteNum------byte amount of data to be read out.
 *
 * RETURN: 
 *		Uint16 type value:
 *		I2C_W_R_OK
 *		I2C_W_R_EXCEED_LIMIT
 *		I2C_W_R_DATA_AMOUNT_IS_ZERO
 *		I2C_R_FAILED
 *
 * CALLS: 
 *		I2C_ConvAddr()
 *		I2C_Read_MultiByte()
 *		I2C_PollACK()
 *
 * CALLED BY: 
 *     Main.c  
 *
 * MORE INFORMATION:
 *		Internal I2C error check and retry is handled by I2C_Read_MultiByte().
 *		This function has all data read.
 *============================================================================*/
Uint16 I2C_Read_InByte(  Uint16 u16StartAddr, 
                         Uint16* pDstiAddr, 
                         Uint16 u16ByteNum   )
{
	Uint16 i;
	Uint16 u16EachNum;
	Uint16 u16EEPROMAddr;
	Uint16 u16Error;
	TypeI2cAddr i2cStartAddr;

	//check if u16StartAddr is a valid M24128 address
	if ((u16StartAddr + u16ByteNum) >= M24128_ADDR_LIMIT)
	{
		return I2C_W_R_EXCEED_LIMIT;
	}
	else if (0 == u16ByteNum)
	{
		return I2C_W_R_DATA_AMOUNT_IS_ZERO;
	}

	LCK_pend(&LCK_EEPROM, SYS_FOREVER);
	//prepare for first reading.
	CurrentMsgPtr = &I2cMsgIn;
	u16EEPROMAddr = u16StartAddr;
	I2C_ConvAddr(u16EEPROMAddr, &i2cStartAddr);
	I2cMsgIn.SlaveAddress = I2C_SLAVE_ADDR;
	I2cMsgIn.MemoryHighAddr = i2cStartAddr.u16BlockAddr;
	I2cMsgIn.MemoryLowAddr = i2cStartAddr.u16ByteAddr;
	//I2cMsgIn.p_MsgBuffer = pDstiAddr;
	u16EachNum = u16ByteNum;

	//every while loop reads 4 bytes
	while (u16EachNum)
	{	
		if (u16EachNum > I2C_MAX_BUFFER_SIZE)
		{
			I2cMsgIn.NumOfBytes = I2C_MAX_BUFFER_SIZE;
			u16EachNum -= I2C_MAX_BUFFER_SIZE;
			u16EEPROMAddr += I2C_MAX_BUFFER_SIZE;	//address for next reading
		}
		else
		{
			I2cMsgIn.NumOfBytes = u16EachNum;
			u16EachNum = 0;
		}
		u16Error = I2C_Read_MultiByte(&I2cMsgIn);
		if (0 == u16Error)	//read success!
		{	
			for(i = 0; i < I2cMsgIn.NumOfBytes; i++)
			{
				*pDstiAddr = I2cMsgIn.MsgBuffer[i];
				pDstiAddr++;
				I2cMsgIn.MsgBuffer[i] = 0;
			}

			if(0 != u16EachNum)
			{	//prepare for next reading.
				I2C_ConvAddr(u16EEPROMAddr, &i2cStartAddr);
				I2cMsgIn.SlaveAddress = I2C_SLAVE_ADDR;
				I2cMsgIn.MemoryHighAddr = i2cStartAddr.u16BlockAddr;
				I2cMsgIn.MemoryLowAddr = i2cStartAddr.u16ByteAddr;
			}
		}
		else
		{
			LCK_post(&LCK_EEPROM);
			return I2C_R_FAILED;	//Error: read EEPROM failed
		}
	}
	LCK_post(&LCK_EEPROM);
	return I2C_W_R_OK;
}
/*=============================================================================*
 * FUNCTION: 
 *		I2C_Read_InWord()
 *
 * PURPOSE : 
 *		Provide EEPROM read interface to other threads.
 *		Analyze EEPROM address and data amount to be read, split read zone with
 *	4 bytes as one groups, then call "I2C_Read_MultiByte()" to complete every 
 *	group reading.
 *
 * INPUT: 
 *		Uint16 u16StartAddr----EEPROM address start to be read out,
 *		Uint16* pSrcAddr-------storage address of data be read out,
 *		Uint16 u16WordNum------word amount of data to be read out.
 *
 * RETURN: 
 *		Uint16 type value:
 *		I2C_W_R_OK
 *		I2C_W_R_EXCEED_LIMIT
 *		I2C_W_R_DATA_AMOUNT_IS_ZERO
 *		I2C_R_FAILED
 *
 * CALLS: 
 *		I2C_ConvAddr()
 *		I2C_Read_MultiByte()
 *		I2C_PollACK()
 *
 * CALLED BY: 
 *     Main.c  
 *
 * MORE INFORMATION:
 *		Internal I2C error check and retry is handled by I2C_Read_MultiByte().
 *		This function has all data read.
 *============================================================================*/
Uint16 I2C_Read_InWord(  Uint16 u16StartAddr, 
                         Uint16* pDstiAddr, 
                         Uint16 u16WordNum   )
{
	Uint16 i;
	Uint16 u16EachNum;	// in byte
	Uint16 u16EEPROMAddr;
	Uint16 u16Error;
	TypeI2cAddr i2cStartAddr;
	//Uint16 data[I2C_MAX_BUFFER_SIZE];
	
	//check if u16StartAddr is a valid M24128 address
	if ((u16StartAddr + 2 * u16WordNum) >= M24128_ADDR_LIMIT)
	{
		return I2C_W_R_EXCEED_LIMIT;
	}
	else if (0 == u16WordNum)
	{
		return I2C_W_R_DATA_AMOUNT_IS_ZERO;
	}
	
	LCK_pend(&LCK_EEPROM, SYS_FOREVER);
	//prepare for first reading.
	CurrentMsgPtr = &I2cMsgIn;
	u16EEPROMAddr = u16StartAddr;
	I2C_ConvAddr(u16EEPROMAddr, &i2cStartAddr);
	I2cMsgIn.SlaveAddress = I2C_SLAVE_ADDR;
	I2cMsgIn.MemoryHighAddr = i2cStartAddr.u16BlockAddr;
	I2cMsgIn.MemoryLowAddr = i2cStartAddr.u16ByteAddr;
	//I2cMsgIn.p_MsgBuffer = data;
	u16EachNum = 2 * u16WordNum;

	//every while loop reads 4 bytes or 2 words
	while (u16EachNum)
	{	
		if (u16EachNum > I2C_MAX_BUFFER_SIZE)
		{
			I2cMsgIn.NumOfBytes = I2C_MAX_BUFFER_SIZE;
			u16EachNum -= I2C_MAX_BUFFER_SIZE;
			u16EEPROMAddr += I2C_MAX_BUFFER_SIZE;	//address for next reading
		}
		else
		{
			I2cMsgIn.NumOfBytes = u16EachNum;
			u16EachNum = 0;
		}
		u16Error = I2C_Read_MultiByte(&I2cMsgIn);
		if (0 == u16Error)	//read success!
		{	//copy data, clear buffer
			for(i = 0; i < I2cMsgIn.NumOfBytes; i+=2)
			{
				*pDstiAddr = (I2cMsgIn.MsgBuffer[i+1]<<8) | I2cMsgIn.MsgBuffer[i];
				pDstiAddr++;
				I2cMsgIn.MsgBuffer[i+1] = I2cMsgIn.MsgBuffer[i] = 0;
			}
			if(0 != u16EachNum)
			{	//prepare for next reading.
				I2C_ConvAddr(u16EEPROMAddr, &i2cStartAddr);
				I2cMsgIn.SlaveAddress = I2C_SLAVE_ADDR;
				I2cMsgIn.MemoryHighAddr = i2cStartAddr.u16BlockAddr;
				I2cMsgIn.MemoryLowAddr = i2cStartAddr.u16ByteAddr;
				//I2cMsgIn.p_MsgBuffer = data;
			}
		}
		else
		{
			LCK_post(&LCK_EEPROM);
			return I2C_R_FAILED;	//Error: read EEPROM failed
		}
	}
	LCK_post(&LCK_EEPROM);
	return I2C_W_R_OK;
}

/*=============================================================================*
 * FUNCTION: 
 *		I2C_Read_MultiByte()
 *
 * PURPOSE : 
 *		Read at most 4 bytes.
 *		M24128 supports more than 4 bytes to be read in Sequential Random Read
 *	mode, but 28035 I2C module's FIFO size is 4, so we read at most 4 bytes in
 *	this function.
 *
 * INPUT: 
 *		TypeI2cMsg *msg----Address of a TypeI2cMsg type structure.
 *		It should be address of I2cMsgIn.
 *
 * RETURN: 
 *		Uint16 type value:
 *		I2C_SUCCESS
 *		I2C_ERROR
 *
 * CALLS: 
 *		I2C_Reset()
 *		I2C_ReadData()
 *		SEM_pendBinary()
 *
 * CALLED BY: 
 *		I2C_Read()
 *	
 * MORE INFORMATION:
 *		Destination address and source data are ready in I2cMsgIn before it is 
 *	called.
 *		It Handles Internal I2C error check and retry operation.
 * 
 *============================================================================*/
Uint16 I2C_Read_MultiByte(TypeI2cMsg *msg)
{	
	Uint16 u16Error;
	Uint16 u16RetryTimes_dW;	//dummy WRITE retry times
	Uint16 u16RetryTimes_Reset;	//I2C Reset retry times
	Uint16 u16ResetFlag;		//I2C_NO--not need RESET, I2C_YES--need RESET
	
	u16RetryTimes_dW = 0;
	u16RetryTimes_Reset = 0;
	u16ResetFlag = I2C_NO;
	
	if (msg->MsgStatus != I2C_MSGSTAT_INACTIVE)
	{
		u16Error = I2C_Reset();
		if(I2C_ERROR == u16Error)
		{
			return I2C_ERROR;
		}
		else
		{
			msg->MsgStatus = I2C_MSGSTAT_INACTIVE;
		}
	}
	
	msg->MsgStatus = I2C_MSGSTAT_SEND_NOSTOP;
	
	while(u16RetryTimes_Reset < I2C_MAX_TIMES_RESET)
	{	//--------dummy WRITE--------
		while(u16RetryTimes_dW < I2C_MAX_TIMES_DYMMY_WRITE)
		{	
			u16Error = I2C_ReadData(msg);
			
			if(I2C_SUCCESS != u16Error)
			{	//retry dummy WRITE
				u16RetryTimes_dW++;
				continue;
			}
			else
			{	
				u16Error = SEM_pendBinary(&SEM_I2C_isr, I2C_MAX_MS_SEMPEND);
				if(u16Error)	//return from ISR
				{
    				if(I2C_SUCCESS == u16I2cIsrError)
    				{
    					if(I2C_MSGSTAT_RESTART == msg->MsgStatus)
    					{
    						u16RetryTimes_dW = 0;
    						break;		//dummy WRITE success, jump to RESTART
    					}
    					else
    					{	
    						u16RetryTimes_dW++;
    						continue;	//dummy WRITE failed, retry dummy WRITE
    					}
    				}
    				else if(I2C_NACK_ERROR == u16I2cIsrError)//retry dummy WRITE
    				{	//wait for SCD interrupt return
    					u16Error = SEM_pendBinary(&SEM_I2C_isr, I2C_MAX_MS_SEMPEND);
						if(u16Error)
    					{	
    						u16RetryTimes_dW++;
    						continue;
						}
						else
						{
							msg->MsgStatus = I2C_MSGSTAT_INACTIVE;
							return I2C_ERROR;
						}
    				}
    				else	//----illegal interrupt source
    				{	
    					u16ResetFlag = I2C_YES;	//retry I2C Reset
    					break;
    				}
				}
				else	//FATAL ERROR----can't enter ISR
				{
					msg->MsgStatus = I2C_MSGSTAT_INACTIVE;
					return I2C_ERROR;
				}
			}
		}	//----dummy WRITE I2C_MAX_TIMES_DYMMY_WRITE time ends here!
		
		if(u16RetryTimes_dW >= I2C_MAX_TIMES_DYMMY_WRITE)
		{	//retry I2C Reset
			u16ResetFlag = I2C_YES;
		}
		else if(I2C_NO == u16ResetFlag)
		{	//--------RESTART--------	
			u16Error = I2C_ReadData(msg);

			if(I2C_SUCCESS != u16Error)
			{	//retry I2C Reset
				u16ResetFlag = I2C_YES;
			}
			else
			{	
				u16Error = SEM_pendBinary(&SEM_I2C_isr, I2C_MAX_MS_SEMPEND);
				if(u16Error)
				{
    				if(I2C_SUCCESS == u16I2cIsrError)	
    				{	
    					if(I2C_MSGSTAT_INACTIVE == msg->MsgStatus)
    					{
    						break;	// read success!
    					}
    					else
    					{	//RESTART failed
    						u16ResetFlag = I2C_YES;	//retry I2C Reset
    					}
    				}
    				else//----illegal interrupt source
    				{	
    					u16ResetFlag = I2C_YES;	//retry I2C Reset
    				}
				}
				else	//FATAL ERROR----can't enter ISR
				{
					msg->MsgStatus = I2C_MSGSTAT_INACTIVE;
					return I2C_ERROR;
				}
			}
		}
		//note: no else here!

		//--------RESET I2C--------
		if(I2C_YES == u16ResetFlag)
		{
			u16ResetFlag = I2C_NO;
			u16RetryTimes_Reset++;
			u16RetryTimes_dW = 0;
			
			u16Error = I2C_Reset();
			
			if(I2C_ERROR == u16Error)
			{
				msg->MsgStatus = I2C_MSGSTAT_INACTIVE;
				SEM_pendBinary(&SEM_I2C_isr, 0);	//clear unexpected SEM
				return I2C_ERROR;
			}
			else
			{
				msg->MsgStatus = I2C_MSGSTAT_SEND_NOSTOP;
				continue;
			}
		}
	}	//----I2C Reset 3 times end here!

	u16RetryTimes_dW = 0;
	SEM_pendBinary(&SEM_I2C_isr, 0);	//clear unexpected SEM

	if(u16RetryTimes_Reset < I2C_MAX_TIMES_RESET)
	{	
		u16RetryTimes_Reset = 0;

		if(I2C_MSGSTAT_INACTIVE == msg->MsgStatus)
		{
			return I2C_SUCCESS;
		}
		else
		{
			msg->MsgStatus = I2C_MSGSTAT_INACTIVE;
			return I2C_ERROR;
		}
	}
	else
	{
		u16RetryTimes_Reset = 0;
		msg->MsgStatus = I2C_MSGSTAT_INACTIVE;
		return I2C_ERROR;
	}
}

/*=============================================================================*
 * FUNCTION: 
 *		I2C_ReadData()
 *
 * PURPOSE : 
 *		Operate registers of I2C Module to execute data reading.
 *
 * INPUT: 
 *		TypeI2cMsg *msg----Address of a TypeI2cMsg type structure.
 *		It should be address of I2cMsgIn.
 *
 * RETURN: 
 *		Uint16 type value:
 *		I2C_STP_NOT_READY_ERROR
 *		I2C_BUS_BUSY_ERROR
 *		I2C_SUCCESS
 *
 * CALLS: 
 *     void
 *
 * CALLED BY: 
 *		I2C_Read_MultiByte()
 * 
 *============================================================================*/
Uint16 I2C_ReadData(TypeI2cMsg *msg)
{
	// Wait until the STP bit is cleared from any previous master communication.
	// Clearing of this bit by the module is delayed until after the SCD bit is
	// set. If this bit is not checked prior to initiating a new message, the
	// I2C could get confused.
	if (1 == I2caRegs.I2CMDR.bit.STP)
	{
		return I2C_STP_NOT_READY_ERROR;
	}

	if(I2C_MSGSTAT_SEND_NOSTOP == msg->MsgStatus)
	{
		// Check if bus busy
		if (1 == I2caRegs.I2CSTR.bit.BB)
		{
			return I2C_BUS_BUSY_ERROR;
		}

		if(I2caRegs.I2CFFTX.bit.TXFFST)	//clear TXFIFO
		{	
			I2caRegs.I2CFFTX.bit.TXFFRST = 0;
			while(I2caRegs.I2CFFTX.bit.TXFFST);
			I2caRegs.I2CFFTX.bit.TXFFRST = 1;
		}

		msg->MsgStatus = I2C_MSGSTAT_SEND_NOSTOP_BUSY;
		//----dummy write
		I2caRegs.I2CSAR = msg->SlaveAddress;	
		I2caRegs.I2CCNT = 2;
		I2caRegs.I2CDXR = msg->MemoryHighAddr;
		I2caRegs.I2CDXR = msg->MemoryLowAddr;
		I2caRegs.I2CMDR.all = 0x2620;				// Send data to setup EEPROM address
	}
	else if(I2C_MSGSTAT_RESTART == msg->MsgStatus)
	{	
		msg->MsgStatus = I2C_MSGSTAT_READ_BUSY;
		//----read
		I2caRegs.I2CSAR = msg->SlaveAddress;	
		I2caRegs.I2CCNT = msg->NumOfBytes;			// Setup how many bytes to expect
		I2caRegs.I2CMDR.all = 0x2C20;				// Send restart as master receiver
	}

	return I2C_SUCCESS;
}
/*=============================================================================*
**void ClearEEPROM(void) 将eprom空间全部写0XFF;
*=============================================================================*/
void ClearEEPROM(void) 
{
	Uint16 pageBuf[I2C_PAGE_SIZE];
	Uint16 i;

	for(i=0; i<I2C_PAGE_SIZE; i++)
	{
		pageBuf[i]=0xFF;
	}
	
	for(i=0; i<M24128_ADDR_LIMIT; i+=I2C_PAGE_SIZE)
	{
		I2C_Write_InByte(i,pageBuf,I2C_PAGE_SIZE);
	}
}
/*=============================================================================*
 * FUNCTION: 
 *		SWI_F_I2Ca()
 *
 * PURPOSE : 
 *		DSP/BIOS SWI function, handles I2C interrupt routine.
 *
 * INPUT: 
 *     void
 *
 * RETURN: 
 *     void
 *
 * CALLS: 
 *     void
 *
 * CALLED BY: 
 *     DSP/BIOS shedule routine.
 * 
 *============================================================================*/
void SWI_F_I2Ca(void)     // I2C-A
{
	Uint16  i;

	u16I2cIsrError = I2C_SUCCESS;
	
	// Read interrupt source
	u16IntSource = I2caRegs.I2CISRC.all;
	
	// Interrupt source = stop condition detected
	if(I2C_SCD_ISRC == u16IntSource)
	{
		// If completed message was writing data, reset msg to inactive state
		if (I2C_MSGSTAT_WRITE_BUSY == CurrentMsgPtr->MsgStatus)
		{
			if(0 != I2caRegs.I2CFFTX.bit.TXFFST)
			{	
				u16I2cIsrError = I2C_NACK_ERROR;
			}

			CurrentMsgPtr->MsgStatus = I2C_MSGSTAT_INACTIVE;
		}
		else
		{
			// If a message receives a NACK during the address setup portion of the
			// EEPROM read, the code further below included in the register access ready
			// interrupt source code will generate a stop condition. After the stop
			// condition is received (here), set the message status to try again.
			// User may want to limit the number of retries before generating an error.
			if(I2C_MSGSTAT_SEND_NOSTOP_BUSY == CurrentMsgPtr->MsgStatus)
			{
				CurrentMsgPtr->MsgStatus = I2C_MSGSTAT_SEND_NOSTOP;
				u16I2cIsrError = I2C_NACK_ERROR;
			}
			// If completed message was reading EEPROM data, reset msg to inactive state
			// and read data from FIFO.
			else if (I2C_MSGSTAT_READ_BUSY == CurrentMsgPtr->MsgStatus)
			{
				CurrentMsgPtr->MsgStatus = I2C_MSGSTAT_INACTIVE;
				for(i = 0; i < CurrentMsgPtr->NumOfBytes; i++)
				{
					CurrentMsgPtr->MsgBuffer[i] = I2caRegs.I2CDRR;
				}
			}
		}
	}  // end of stop condition detected

	// Interrupt source = Register Access Ready
	// This interrupt is used to determine when the EEPROM address setup portion of the
	// read data communication is complete. Since no stop bit is commanded, this flag
	// tells us when the message has been sent instead of the SCD flag. If a NACK is
	// received, clear the NACK bit and command a stop. Otherwise, move on to the read
	// data portion of the communication.
	else if(I2C_ARDY_ISRC == u16IntSource)
	{
		if(1 == I2caRegs.I2CSTR.bit.NACK)
		{
			I2caRegs.I2CMDR.bit.STP = 1;
			I2caRegs.I2CSTR.all = I2C_CLR_NACK_BIT;
			
			if(I2C_MSGSTAT_SEND_NOSTOP_BUSY == CurrentMsgPtr->MsgStatus)
			{
				u16I2cIsrError = I2C_NACK_ERROR;
			}
			else if(I2C_MSGSTAT_WRITE_BUSY == CurrentMsgPtr->MsgStatus)
			{
				u16I2cIsrError = I2C_NACK_ERROR;
			}
		}
		else	// receive ACK form device
		{
			if(I2C_MSGSTAT_SEND_NOSTOP_BUSY == CurrentMsgPtr->MsgStatus)
			{
				CurrentMsgPtr->MsgStatus = I2C_MSGSTAT_RESTART;
			}
			else if(I2C_MSGSTAT_WRITE_BUSY == CurrentMsgPtr->MsgStatus)	// 2011-12-31 multi byte write
			{	
				if(0 == I2caRegs.I2CFFTX.bit.TXFFST)	// Only if all data in FIFO have been send out
				{
					if(CurrentMsgPtr->NumOfBytes > I2C_MAX_BUFFER_SIZE)
					{
						for (i=0; i<I2C_MAX_BUFFER_SIZE; i++)
						{
							I2caRegs.I2CDXR = *(CurrentMsgPtr->p_MsgBuffer);
							CurrentMsgPtr->p_MsgBuffer++;
						}
						CurrentMsgPtr->NumOfBytes -= I2C_MAX_BUFFER_SIZE;
					}
					else if(CurrentMsgPtr->NumOfBytes > 0)
					{
						for (i=0; i<CurrentMsgPtr->NumOfBytes; i++)
						{
							I2caRegs.I2CDXR = *(CurrentMsgPtr->p_MsgBuffer);
							CurrentMsgPtr->p_MsgBuffer++;
						}
						CurrentMsgPtr->NumOfBytes = 0;
					}
					else
					{
						I2caRegs.I2CMDR.bit.STP = 1;	// RM
					}
				}
				return;	// when write, only SCD post SEM of I2C.
			}
		}
	}  // end of register access ready
	else if(I2C_NO_ISRC != u16IntSource)
	{	// Generate some error due to invalid interrupt source
		u16I2cIsrError = I2C_ERROR;
	}

	if(I2C_NO_ISRC != u16IntSource)
	{
		SEM_postBinary(&SEM_I2C_isr);
	}
}
//---------------------------------------------------------------------------
// Example: InitI2CGpio:
//---------------------------------------------------------------------------
// This function initializes GPIO pins to function as I2C pins
//
// Each GPIO pin can be configured as a GPIO pin or up to 3 different
// peripheral functional pins. By default all pins come up as GPIO
// inputs after reset.
//
// Caution:
// Only one GPIO pin should be enabled for SDAA operation.
// Only one GPIO pin shoudl be enabled for SCLA operation.
// Comment out other unwanted lines.

void InitI2CGpio()
{

   EALLOW;

/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

	GpioCtrlRegs.GPAPUD.bit.GPIO28 = 0;    // Enable pull-up for GPIO28 (SDAA)
	GpioCtrlRegs.GPAPUD.bit.GPIO29 = 0;	   // Enable pull-up for GPIO29 (SCLA)

//	GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0;    // Enable pull-up for GPIO32 (SDAA)
//	GpioCtrlRegs.GPBPUD.bit.GPIO33 = 0;	   // Enable pull-up for GPIO33 (SCLA)

/* Set qualification for selected pins to asynch only */
// This will select asynch (no qualification) for the selected pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 3;  // Asynch input GPIO28 (SDAA)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO29 = 3;  // Asynch input GPIO29 (SCLA)

//	GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 3;  // Asynch input GPIO32 (SDAA)
//  GpioCtrlRegs.GPBQSEL1.bit.GPIO33 = 3;  // Asynch input GPIO33 (SCLA)

/* Configure I2C pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be I2C functional pins.
// Comment out other unwanted lines.

	GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 2;   // Configure GPIO28 for SDAA operation
	GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 2;   // Configure GPIO29 for SCLA operation

//	GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 1;   // Configure GPIO32 for SDAA operation
//	GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 1;   // Configure GPIO33 for SCLA operation

    EDIS;
}

/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void ReadFromEeprom(void)
{
	I2Cerror = I2C_Read_InWord(BOOTSTATE_ADDR,&Orig_epromflg,1);   //I2C_Read_InByte的主要作用是移动数据，但能返回数值。
	if(I2Cerror != 0)                                                   //以表示执行时出现的状况，
	{
		s_SysFaultMessage.bit.unrecoverEEPROM_R = 1;                  //返回常量I2C_W_R_OK==0则表示一切正常，否则说明有问题。
	}										
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
Uint16 writedefaultsafety(Uint16 Country)
{
	//Uint16 i;
	Uint16 Rewritetime=0;
	Uint16 I2C_error_temp1,I2C_error_temp2,I2C_error_temp3,I2C_error_temp4,I2C_error_temp5,I2C_error_temp6;
	Uint16 I2C_error_temp7,I2C_error_temp8,I2C_error_temp9,I2C_error_temp10;//,I2C_error_temp11;

	FactoryMode = FactoryModeDefaultValue;
//	for(i=0;i<COUNTRYNUM;i++)
//	{
		while(Rewritetime<3)
		{

		I2C_error_temp1 = I2C_Write_InWord(SAFETY_STARTValue_ADDR,(Uint16*)&SafetyDefaultSTARTTable[Country][0],8);
		I2C_error_temp2 = I2C_Write_InWord(SAFETY_GridProtectVValue_ADDR,(Uint16*)&SafetyDefaultVTable[Country][0],10);
		I2C_error_temp3 = I2C_Write_InWord(SAFETY_GridProtectFreqValue_ADDR,(Uint16*)&SafetyDefaultFreTable[Country][0],9);
		I2C_error_temp4 = I2C_Write_InWord(SAFETY_GridProtectDCIValue_ADDR,(Uint16*)&SafetyDefaultDCITable[Country][0],6);
		I2C_error_temp5 = I2C_Write_InWord(SAFETY_RemotPowerControlValue_ADDR,(Uint16*)&SafetyDefaultRemotPowerControlTable[Country][0],3);
		I2C_error_temp6 = I2C_Write_InWord(SAFETY_FreqPowerControlValue_ADDR,(Uint16*)&SafetyDefaultFreqPowerControlTable[Country][0],7);
		I2C_error_temp7 = I2C_Write_InWord(SAFETY_ReactiveValue_ADDR,(Uint16*)&SafetyDefaultReactiveTable[Country][0],21);
		I2C_error_temp8 = I2C_Write_InWord(SAFETY_LVRTValue_ADDR,(Uint16*)&SafetyDefaultLVRTTable[Country][0],13);
		I2C_error_temp9 = I2C_Write_InWord(SAFETY_IsoIslandandSoOn_ADDR,(Uint16*)&SafetyDefaultIsoIslandandSoOnTable[Country][0],4);
		I2C_error_temp10 = I2C_Write_InWord(FactoryMode_ADDR,&FactoryMode,1);
//		I2C_error_temp11 = I2C_Write_InWord(Calibration_ADDR,(Uint16*)&CalibrationDefaultTable,32);

		
		I2Cerror = (I2C_error_temp1||I2C_error_temp2||I2C_error_temp3||I2C_error_temp4||I2C_error_temp5
			  ||I2C_error_temp6||I2C_error_temp7||I2C_error_temp8||I2C_error_temp9||I2C_error_temp10);//||I2C_error_temp11);
 
			if(I2Cerror != 0)        //将“安规表”放入SAFETY_TABLE_START_ADDR地址开始的E2PROOM中。每隔SAFETY_MAX_SIZE一个
			{
				Rewritetime++;
				continue;
			}
			else
			{
				Rewritetime	= 0;
				break;
			}
		}///while END
		if(Rewritetime>=3)
		{
			return I2C_ERROR;         //连续RETRY 达3次（I2Cerror != 0）则返回“错误”值
		}
	//}
	return 0; 	 
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
Uint16 readsafetyvalue(void)         //从E2PROM读取某国安规参数,存到自己变量中，并且判断是否超限
{	
	Uint16 I2C_error_temp1,I2C_error_temp2,I2C_error_temp3,I2C_error_temp4,I2C_error_temp5,I2C_error_temp6;
	Uint16 I2C_error_temp7,I2C_error_temp8,I2C_error_temp9,I2C_error_temp10,I2C_error_temp11;
	Uint16 Rewritetime=0;

	while( Rewritetime<3 )
	{		
		I2C_error_temp1 = I2C_Read_InWord(SAFETY_STARTValue_ADDR,SafetySTARTTable,8);
		I2C_error_temp2 = I2C_Read_InWord(SAFETY_GridProtectVValue_ADDR,SafetyVTable,10);
		I2C_error_temp3 = I2C_Read_InWord(SAFETY_GridProtectFreqValue_ADDR,SafetyFreTable,9);
		I2C_error_temp4 = I2C_Read_InWord(SAFETY_GridProtectDCIValue_ADDR,SafetyDCITable,6);
		I2C_error_temp5 = I2C_Read_InWord(SAFETY_RemotPowerControlValue_ADDR,SafetyRemotPowerControlTable,3);
		I2C_error_temp6 = I2C_Read_InWord(SAFETY_FreqPowerControlValue_ADDR,SafetyFreqPowerControlTable,7);
		I2C_error_temp7 = I2C_Read_InWord(SAFETY_ReactiveValue_ADDR,SafetyReactiveTable,21);
		I2C_error_temp8 = I2C_Read_InWord(SAFETY_LVRTValue_ADDR,SafetyLVRTTable,13);
		I2C_error_temp9 = I2C_Read_InWord(SAFETY_IsoIslandandSoOn_ADDR,SafetyIsoIslandandSoOnTable,4);
		I2C_error_temp10 = I2C_Read_InWord(FactoryMode_ADDR,&FactoryMode,1);
		I2C_error_temp11 = I2C_Read_InWord(Calibration_ADDR,CalibrationTable,32);

		I2Cerror = (I2C_error_temp1||I2C_error_temp2||I2C_error_temp3||I2C_error_temp4||I2C_error_temp5
				  ||I2C_error_temp6||I2C_error_temp7||I2C_error_temp8||I2C_error_temp9||I2C_error_temp10||I2C_error_temp11);

		if(I2Cerror != 0)
		{
			Rewritetime++;
			continue;
		}
		else
		{
			Rewritetime	= 0;
			break;
		}
	}
	if (Rewritetime>=3)
	{
		return I2C_ERROR;
	}

	return 0;				
}
//===========================================================================
// End of file.
//===========================================================================
