/***********************************************************************
 *    FILENAME : SC_Sci.c
 *
 *    PURPOSE  : 
 *    HISTORY  :
 *           DATE          VERSION      AUTHOR        NOTE
 ************************************************************************/
#include "SC_master.h"

static SciStruct					SciList[MAX_SCI_NO];
static QUEUE						QList[MAX_SCI_NO];
static Uint8						szSciRxBuf[MAX_SCI_BUF_SIZE];
static volatile struct SCI_REGS		*pSciRegsIndex[MAX_SCI_NO] = {&SciaRegs,&ScibRegs,&ScicRegs};

/**********************************************************************
* FUNCION :  InitQueue
* PURPOSE :  
* CALLED BY:
**********************************************************************/
static void InitQueue(QUEUE *pQue, Uint8 *pu8Start, Uint16 u16BufSize)
{
	pQue->u16Length = 0;
	pQue->u16Size = u16BufSize;
	pQue->pu8In = pu8Start;
	pQue->pu8Out = pu8Start;
	pQue->pu8Start = pu8Start;
}
/**********************************************************************
* FUNCION :  QueDataIn
* PURPOSE :  
* CALLED BY:
**********************************************************************/
Uint16 QueDataIn(QUEUE *pQue, Uint16 u16QueData)
{
	if(pQue->u16Length == pQue->u16Size)
	{
		if(pQue->pu8In == pQue->pu8Start)
		{
			*(pQue->pu8Start + pQue->u16Size - 1) = u16QueData;
		}
		else
		{
			*(pQue->pu8In - 1) = u16QueData;
		}
		return QUE_BUF_FULL;
	}
	else
	{
		*(pQue->pu8In) = u16QueData;
		pQue->u16Length += 1;
		if(pQue->pu8In == pQue->pu8Start + pQue->u16Size - 1)
		{
			pQue->pu8In = pQue->pu8Start;
		}
		else
		{
			pQue->pu8In += 1;
		}
		return QUE_BUF_NORMAL;
	}
}
/**********************************************************************
* FUNCION :  QueDataOut
* PURPOSE :  
* CALLED BY:
**********************************************************************/
static Uint16 QueDataOut(QUEUE *pQue, Uint8 *pQueData)
{
	if(0 == pQue->u16Length)
	{
		return QUE_BUF_EMPTY;
	}
	else
	{
		*pQueData = *(pQue->pu8Out);
		pQue->u16Length -= 1;
		if(pQue->pu8Out == (pQue->pu8Start + pQue->u16Size - 1))
		{
			pQue->pu8Out = pQue->pu8Start;
		}
		else
		{
			pQue->pu8Out += 1;
		}
		return QUE_BUF_NORMAL;
	}
}
/**********************************************************************
* FUNCION :  SetScia
* PURPOSE :  This function initializes the SCIa data structure.
* CALLED BY: 
**********************************************************************/
static void SetScia(void)
{
	SciStruct	*pSci;
	QUEUE		*pq;
	Uint8		*pSCIBuf;
	
	pSci = &SciList[ID_SCIA];
	g_pSciIndex[ID_SCIA] = pSci;

	pSci->pqRx = &QList[ID_SCIA];
	pq = pSci->pqRx;
	pSCIBuf = szSciRxBuf;
	InitQueue(pq, pSCIBuf, SCIA_BUF_SIZE);
	
	pSci->u8TxStatus = SCI_TX_RDY;
	pSci->u16TxLength = 0;
	
}
/**********************************************************************
* FUNCION :  SetScib
* PURPOSE :  This function initializes the SCIb data structure.
* CALLED BY: 
**********************************************************************/
static void SetScib(void)
{
	SciStruct	*pSci;
	QUEUE		*pq;
	Uint8		*pSCIBuf;
	
	pSci = &SciList[ID_SCIB];
	g_pSciIndex[ID_SCIB] = pSci;

	pSci->pqRx = &QList[ID_SCIB];
	pq = pSci->pqRx;
	pSCIBuf = szSciRxBuf + SCIA_BUF_SIZE;
	InitQueue(pq, pSCIBuf, SCIB_BUF_SIZE);

	pSci->u8TxStatus = SCI_TX_RDY;
	pSci->u16TxLength = 0;
}

/**********************************************************************
* FUNCION :  InitScia
* PURPOSE :  This function initializes the SCIa to a known state.
* CALLED BY: 
**********************************************************************/
void InitScia(Uint32 u32BaudRate, Uint16 u16RxMode)
{
	Uint16 u16Br;

	// Initialize SCI-A:
	SciaRegs.SCICCR.all = 0x0007;			// 1 stop bit, No loopback, No parity, async mode, 
   											// idle-line protocol, 8 char bits,  
   	SciaRegs.SCICTL1.all = 0x0003;  		// RX_err_INT disable, sleepMode disable, RX/TX enable
                                  			// Disable RX ERR, SLEEP, TXWAKE
   	SciaRegs.SCICTL2.bit.TXINTENA = 1;		// 0 = disable, 1 = enable TXRDY INT, in FIFO, this INT used as FIFO_Int
   	SciaRegs.SCICTL2.bit.RXBKINTENA = 1;	// 1 = enable RXrdy/BRKINT, 0 = disable  	
   	SciaRegs.SCIFFTX.all = 0xE040;		
   	SciaRegs.SCIFFCT.all = 0x00;
   	SciaRegs.SCICTL1.all = 0x0023;     		// Relinquish SCI from Reset, by software reset
   	SciaRegs.SCIFFTX.bit.TXFIFOXRESET = 1;	// enable TX
   	SciaRegs.SCIFFRX.bit.RXFIFORESET = 1;	// enable RX
	SciaRegs.SCIPRI.bit.FREE = 1; 			// free run enabled, 0 = soft emulation suspend, 1 = free run, 

	u16Br = ((LSPCLK_FREQ/u32BaudRate)/8)-1;
	SciaRegs.SCIHBAUD = GET_HBYTE_OF_WORD(u16Br);	// BRR setting, H byte, when BaudRate=9600.	brr=0x01E7
   	SciaRegs.SCILBAUD = GET_LBYTE_OF_WORD(u16Br);	// BRR setting, L byte
	
	if(MODE_INT == u16RxMode)
   	{
		SciaRegs.SCIFFRX.all = 0x6061;
		PieCtrlRegs.PIEIER9.bit.INTx1 = 1;		// Enable SCIA_RXINT in PIE group 9
		IER |= M_INT9;							// Enable INT9 in IER to enable PIE group 9
	}
	else
	{
		SciaRegs.SCIFFRX.all = 0x2041;
	}
	
	SetScia();
}
/**********************************************************************
* FUNCION :  InitScib
* PURPOSE :  This function initializes the SCIb to a known state.
* CALLED BY: 
**********************************************************************/
void InitScib(Uint32 u32BaudRate, Uint16 u16RxMode)
{
	Uint16 u16Br;

	// Initialize SCI-B:
	ScibRegs.SCICCR.all = 0x0007;			// 1 stop bit, No loopback, No parity, async mode, 
   											// idle-line protocol, 8 char bits,  
   	ScibRegs.SCICTL1.all = 0x0003;  		// RX_err_INT disable, sleepMode disable, RX/TX enable
                                  			// Disable RX ERR, SLEEP, TXWAKE
   	ScibRegs.SCICTL2.bit.TXINTENA = 1;		// 0 = disable, 1 = enable TXRDY INT, in FIFO, this INT used as FIFO_Int
   	ScibRegs.SCICTL2.bit.RXBKINTENA = 1;	// 1 = enable RXrdy/BRKINT, 0 = disable  	
   	ScibRegs.SCIFFTX.all = 0xE040;		
   	ScibRegs.SCIFFCT.all = 0x00;
   	ScibRegs.SCICTL1.all = 0x0023;     		// Relinquish SCI from Reset, by software reset
   	ScibRegs.SCIFFTX.bit.TXFIFOXRESET = 1;	// enable TX
   	ScibRegs.SCIFFRX.bit.RXFIFORESET = 1;	// enable RX
	ScibRegs.SCIPRI.bit.FREE = 1; 			// free run enabled, 0 = soft emulation suspend, 1 = free run, 

	u16Br = ((LSPCLK_FREQ/u32BaudRate)/8)-1;
	ScibRegs.SCIHBAUD = GET_HBYTE_OF_WORD(u16Br);	// BRR setting, H byte, when BaudRate=9600.	brr=0x01E7
   	ScibRegs.SCILBAUD = GET_LBYTE_OF_WORD(u16Br);	// BRR setting, L byte
	
	if(MODE_INT == u16RxMode)
   	{
		ScibRegs.SCIFFRX.all = 0x6061;
		PieCtrlRegs.PIEIER9.bit.INTx3 = 1;		// Enable SCIB_RXINT in PIE group 9
		IER |= M_INT9;							// Enable INT9 in IER to enable PIE group 9
	}
	else
	{
		ScibRegs.SCIFFRX.all = 0x2041;
	}
	
	SetScib();
}
/**********************************************************************
* FUNCION :  InitSci
* PURPOSE : 
* CALLED BY:
**********************************************************************/
void InitSci(void)
{
	g_pSciIndex[ID_SCIA] = NULL;
	g_pSciIndex[ID_SCIB] = NULL;
	g_pSciIndex[ID_SCIC] = NULL;

#ifdef	_IPOMS_
	InitScia(IPOMS_COMM_BAUDRATE, MODE_INT);
#endif
	InitScib(SCI_UPDATE_COMM_BAUDRATE, MODE_INT);
}

/**********************************************************************
* FUNCION :  SciErrorCheck
* PURPOSE :  
* CALLED BY:
**********************************************************************/
void SciErrorCheck(Uint16 SciId)
{
	if( 1 == pSciRegsIndex[SciId]->SCIRXST.bit.RXERROR)
	{
		pSciRegsIndex[SciId]->SCICTL1.bit.SWRESET = 0;
		
		DelayUs(2000);
		switch(SciId)
		{
			case ID_SCIA:
				SetScia();
			break;
			case ID_SCIB:
				SetScib();
			break;
			default:
			break;
		}
		
		pSciRegsIndex[SciId]->SCICTL1.bit.SWRESET = 1;
	}
}

/**********************************************************************
* FUNCION :  SciRead
* PURPOSE :  
* CALLED BY:
**********************************************************************/
Uint16 SciRead(Uint16 SciId, Uint8 *pBuf)
{
	Uint16 		u16Tmp;
	QUEUE		*pq;
	SciStruct	*pSci;	
	
	SciErrorCheck(SciId);
	pSci = g_pSciIndex[SciId];
	pq = pSci->pqRx;
	u16Tmp = QueDataOut(pq, pBuf);
	if(u16Tmp == QUE_BUF_EMPTY)
	{
		return SCI_RX_EMPTY;
	}
	else
	{
		return SCI_RX_RDY;
	}
}
/**********************************************************************
* FUNCION :  SciWrite
* PURPOSE :  
* CALLED BY:
**********************************************************************/
Uint16 SciWrite(Uint16 SciId, Uint8 *pBuf, Uint16 u16Length)
{
	Uint16 i;
	SciStruct		*pSci;
	 
	pSci = g_pSciIndex[SciId];	
	if(pSci->u8TxStatus == SCI_TX_BUSY)
	{
		return SCI_TX_BUSY;
	}
	
	pSci->u16TxLength = u16Length;
	pSci->u8TxStatus = SCI_TX_BUSY;	
	for(i = 0;i < u16Length;i++)
	{
		while(pSciRegsIndex[SciId]->SCIFFTX.bit.TXFFST != 0) 
		{ 
		}
    	pSciRegsIndex[SciId]->SCITXBUF = *(pBuf + i);
	}
	pSci->u8TxStatus = SCI_TX_RDY;

	return SCI_TX_RDY;
}

//===========================================================================
// End of file.
//===========================================================================

