// TI File $Revision: /main/2 $
// Checkin $Date: April 20, 2009   17:40:49 $
//###########################################################################
//
// FILE:	DSP2803x_Lin.c
//
// TITLE:	DSP2803x LIN Initialization & Support Functions.
//
//###########################################################################
// $TI Release: 2803x C/C++ Header Files V1.21 $
// $Release Date: December 1, 2009 $
//###########################################################################

#include "DSP2803x_Device.h"
#include "DSP2803x_Examples.h"

volatile Uint16 LinaRxBuf[8];	//for test

void SWI_F_LINa(void)
{
	//insert data into queue
	LinaRegs.SCITD = LinaRxBuf[0];
}

//---------------------------------------------------------------------------
// InitLina:
//---------------------------------------------------------------------------
// This function initializes the SCI/BLIN module to SCI.
//
void InitLina(void)
{
	EALLOW;

    //Reset module and release reset.
	LinaRegs.SCIGCR0.bit.RESET = 0;
	DELAY_US(50);
    LinaRegs.SCIGCR0.bit.RESET = 1;

    //SCI into software reset mode
    LinaRegs.SCIGCR1.bit.SWnRST = 0;

    //Select SCI Mode
    LinaRegs.SCIGCR1.bit.LINMODE = 0;		// LIN mode is disabled; SCI compability mode is enabled.

    //Configure SCI mode
	LinaRegs.SCIGCR1.bit.CLK_MASTER = 1; 	// Enable clock to the SCI module
	LinaRegs.SCIGCR1.bit.COMMMODE	= 0; 	// Idle-line mode is used
	LinaRegs.SCIGCR1.bit.TIMINGMODE	= 1;	// SCI timing mode bit.
	LinaRegs.SCIGCR1.bit.PARITYENA 	= 0; 	// Parity disabled.
	LinaRegs.SCIGCR1.bit.PARITY		= 0;	// unused
	LinaRegs.SCIGCR1.bit.STOP		= 0;	// one stop bit used.
	LinaRegs.SCIGCR1.bit.SLEEP		= 0;	// sleep mode is disabled
	LinaRegs.SCIGCR1.bit.MBUFMODE	= 1;	// the multi-buffer mode enabled
	LinaRegs.SCIGCR1.bit.LOOPBACK 	= 0; 	// Disable loopback selfchecking
	LinaRegs.SCIGCR1.bit.RXENA 		= 1; 	// Enable RX pin
	LinaRegs.SCIGCR1.bit.TXENA 		= 1; 	// Enable TX pin

	//Set all interrupts to disabled
    //LinaRegs.SCICLEARINT.all 		= 0xFFFFFFFF;
	//LinaRegs.SCISETINT.bit.SETRXINT	= 1;	// 使能数据接收中断
	//PieCtrlRegs.PIEIER9.bit.INTx3 	= 1;	// Enable LINAINT0 in PIE group 9
	//IER |= M_INT9;							// Enable INT9 in IER to enable PIE group 9
    
	 //IODFT Configuarations
    LinaRegs.IODFTCTRL.bit.IODFTENA = 0; 	// IODFT testing module disabled
    LinaRegs.IODFTCTRL.bit.LPBENA	= 0; 	// IODFT loopback disabled

	//SCI Character Size and Length
	LinaRegs.SCIFORMAT.bit.CHAR		= 7;	// the character is 8 bits long
    LinaRegs.SCIFORMAT.bit.LENGTH 	= 0; 	// Frame lenth is 1 byte

    //Baud Rate Settings - 60MHz device
	//19200bps, see spruge2a Page 70 Table 19.
	LinaRegs.BRSR.bit.SCI_LIN_PSH	= 0;
    LinaRegs.BRSR.bit.SCI_LIN_PSL	= 96;
    LinaRegs.BRSR.bit.M				= 11;

	//Set all interrupts to disabled
    LinaRegs.SCICLEARINT.all 		= 0xFFFFFFFF;
	LinaRegs.SCISETINT.bit.SETRXINT	= 1;			// Enable RX interrupt
	LinaRegs.SCICLEARINTLVL.all 	= 0xFFFFFFFF;	// Set Int level of all interrupts to LVL 0

    //Release	SCI from software reset state - End of Config
    LinaRegs.SCIGCR1.bit.SWnRST = 1;

	EDIS;

	PieCtrlRegs.PIEIER9.bit.INTx3 = 1;		// Enable INT 9.3 in the PIE, LIN0_INTA is INT9.3
   	IER |= M_INT9; 							// Enable CPU Interrupt 9 
}

//===========================================================================
// End of file.
//===========================================================================

