// TI File $Revision: /main/3 $
// Checkin $Date: July 6, 2009   16:09:26 $
//###########################################################################
//
// FILE:	DSP2803x_DefaultIsr.c
//
// TITLE:	DSP2803x Device Default Interrupt Service Routines.
//
// This file contains shell ISR routines for the 2803x PIE vector table.
// Typically these shell ISR routines can be used to populate the entire PIE
// vector table during device debug.  In this manner if an interrupt is taken
// during firmware development, there will always be an ISR to catch it.
//
// As develpment progresses, these ISR rotuines can be eliminated and replaced
// with the user's own ISR routines for each interrupt.  Since these shell ISRs
// include infinite loops they will typically not be included as-is in the final
// production firmware.
//
//###########################################################################
// $TI Release: 2803x C/C++ Header Files V1.21 $
// $Release Date: December 1, 2009 $
//###########################################################################

#include "DSP28x_Project.h"
#include "F28035_BIOS_flashcfg.h"
#include "SL_slave_variable.h"

extern Uint8 SciTxBuff[80];
extern Uint8 SciRxBuff[80];
extern Uint8 SciTxPoint;
extern Uint8 SciReceivePonit;


//test
volatile Uint8 testBuf[20];


//Interrupts used by HF project master-DSP
// INT1.1


// INT 4.1
 void ECAP1_INT_ISR(void)    // ECAP-1
{
	// Insert ISR Code here

	// To receive more interrupts from this PIE group, acknowledge this interrupt
	PieCtrlRegs.PIEACK.all |= PIEACK_GROUP4;
}

// INT6.3
void SPIRXINTB_ISR(void)    // SPI-B
{
	extern Uint16 SPI_Packet_point;
	Uint16 u16_rxTemp;
		
	u16_rxTemp = SpibRegs.SPIRXBUF;
	if(u16_rxTemp == (((Uint16)'M'<<8)|0xAA))
	{
		SPI_Packet_point = 0;
	}
	g_SpiRxBuf[SPI_Packet_point] = u16_rxTemp;
	if(SPI_Packet_point < 31)
	{
		SPI_Packet_point++;
		SpibRegs.SPITXBUF = g_SpiTxBuf[SPI_Packet_point];
	}
	else if(SPI_Packet_point == 31)
	{
		SEM_post(&SEM_SPIb);
		SpibRegs.SPITXBUF = g_SpiTxBuf[0];	// for next time
	}
	// To receive more interrupts from this PIE group, acknowledge this interrupt
	PieCtrlRegs.PIEACK.all |= PIEACK_GROUP6;
}

// INT8.1
 void I2CINT1A_ISR(void)     // I2C-A
{
	// Insert ISR Code here
	SWI_post(&SWI_I2Ca);
	// To receive more interrupts from this PIE group, acknowledge this interrupt
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;
}

// INT9.1
void SCIRXINTA_ISR(void)     // SCI-A Rx
{
	Uint16 SciCheckSum;
	Uint8 temp;
	Uint8 i;
	// Insert ISR Code here
	temp = (Uint8)(SciaRegs.SCIRXBUF.all & 0x00FF);
	switch(SciReceivePonit)
	{
		case 0x00:
			if(temp == 0x01)
			{
				SciRxBuff[SciReceivePonit++] = temp;
			}
		break;
		default:
			if(SciReceivePonit == (SciRxBuff[1]+3))//Rev full 
			{
				SciRxBuff[SciReceivePonit++] = temp;

				SciCheckSum = 0;
				//checksum
				for(i=0;i<(SciRxBuff[1]+2);i++)
				{
					SciCheckSum +=	SciRxBuff[i];
				}

				if(   (SciRxBuff[SciRxBuff[1]+2] == (Uint8)(SciCheckSum & 0x00FF))
				   && (SciRxBuff[SciRxBuff[1]+3] == (Uint8)(SciCheckSum >> 8)) )
				{
					SEM_post(&SEM_SCIa);
					SciReceivePonit = 0;
				}
				else
				{
					SciReceivePonit = 0;
				}
			}
			else
			{
				SciRxBuff[SciReceivePonit++] = temp;
			}
		break;
	}
	// To receive more interrupts from this PIE group, acknowledge this interrupt
	PieCtrlRegs.PIEACK.all |= PIEACK_GROUP9;
}

// INT9.2
 void SCITXINTA_ISR(void)     // SCI-A Tx
{
	PieCtrlRegs.PIEACK.all |= PIEACK_GROUP9;
	// Insert ISR Code here
	if(SciTxPoint <= (SciTxBuff[1]+3) )
	{
		SciaRegs.SCITXBUF = SciTxBuff[SciTxPoint];
		if(SciTxPoint == (SciTxBuff[1]+3))
		{
			testBuf[SciTxPoint] = SciTxBuff[SciTxPoint];
		}
			
		SciTxPoint++;
	}
	else
	{
		SciaRegs.SCICTL1.bit.TXENA = 0;
		SciaRegs.SCICTL1.bit.RXENA = 1;	
		SciaRegs.SCICTL2.bit.TXINTENA = 0;
		SciaRegs.SCICTL2.bit.RXBKINTENA = 1;
						
	}
	// To receive more interrupts from this PIE group, acknowledge this interrupt
	
}


//=======================================================//
//=======================================================//


// Connected to INT13 of CPU (use MINT13 mask):
// ISR can be used by the user.
 void INT13_ISR(void)     // INT13 or CPU-Timer1
{
  // Insert ISR Code here

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

 void INT14_ISR(void)     // INT14 or CPU-Timer2
{
  // Insert ISR Code here

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  //asm ("      ESTOP0");
  //for(;;);
}

 void DATALOG_ISR(void)   // Datalogging interrupt
{
   // Insert ISR Code here

   // Next two lines for debug only to halt the processor here
   // Remove after inserting ISR Code
   asm ("      ESTOP0");
   for(;;);
}

 void RTOSINT_ISR(void)   // RTOS interrupt
{
  // Insert ISR Code here

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

 void EMUINT_ISR(void)    // Emulation interrupt
{
  // Insert ISR Code here

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

 void NMI_ISR(void)       // Non-maskable interrupt
{
  // Insert ISR Code here

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

 void ILLEGAL_ISR(void)   // Illegal operation TRAP
{
  // Insert ISR Code here

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm("          ESTOP0");
  for(;;);

}

 void USER1_ISR(void)     // User Defined trap 1
{
  // Insert ISR Code here

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);

}

 void USER2_ISR(void)     // User Defined trap 2
{
  // Insert ISR Code here

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);

}

 void USER3_ISR(void)     // User Defined trap 3
{
  // Insert ISR Code here

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

 void USER4_ISR(void)     // User Defined trap 4
{
  // Insert ISR Code here

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

 void USER5_ISR(void)     // User Defined trap 5
{
  // Insert ISR Code here

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

 void USER6_ISR(void)     // User Defined trap 6
{
  // Insert ISR Code here

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

 void USER7_ISR(void)     // User Defined trap 7
{
  // Insert ISR Code here

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

 void USER8_ISR(void)     // User Defined trap 8
{
  // Insert ISR Code here

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

 void USER9_ISR(void)     // User Defined trap 9
{
  // Insert ISR Code here

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

 void USER10_ISR(void)    // User Defined trap 10
{
  // Insert ISR Code here

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

 void USER11_ISR(void)    // User Defined trap 11
{
  // Insert ISR Code here

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

 void USER12_ISR(void)     // User Defined trap 12
{
 // Insert ISR Code here

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);

}

// -----------------------------------------------------------
// PIE Group 1 - MUXed into CPU INT1
// -----------------------------------------------------------
// INT1.1
/* void ADCINT1_ISR(void)   // ADC  (Can also be ISR for INT10.1 when enabled)
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
   AdcRegs.ADCINTFLGCLR.bit.ADCINT1 	= 	1;				// clear ADCINT1 flag reinitialize for next SOC

}
*/
// INT1.2
 void ADCINT2_ISR(void)  // ADC  (Can also be ISR for INT10.2 when enabled)
{

  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code

  asm("	  ESTOP0");
  for(;;);

}

// INT1.3 - Reserved

// INT1.4
 void  XINT1_ISR(void)
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);

}

// INT1.5
void  XINT2_ISR(void)
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

	// To receive more interrupts from this PIE group, acknowledge this interrupt
//	PieCtrlRegs.PIEIFR1.bit.INTx5 = 0;
	PieCtrlRegs.PIEACK.all |= PIEACK_GROUP1;
//	asm ("      ESTOP0");
//	for(;;);

}

// INT1.6
 void  ADCINT9_ISR(void)     // ADC
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT1.7
 void  TINT0_ISR(void)      // CPU-Timer 0
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT1.8
 void  WAKEINT_ISR(void)    // WD, LOW Power
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// -----------------------------------------------------------
// PIE Group 2 - MUXed into CPU INT2
// -----------------------------------------------------------

// INT2.1
 void EPWM1_TZINT_ISR(void)    // EPWM-1
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT2.2
 void EPWM2_TZINT_ISR(void)    // EPWM-2
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT2.3
 void EPWM3_TZINT_ISR(void)    // EPWM-3
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT2.4
 void EPWM4_TZINT_ISR(void)    // EPWM-4
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT2.5
 void EPWM5_TZINT_ISR(void)    // EPWM-5
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT2.6
 void EPWM6_TZINT_ISR(void)    // EPWM-6
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT2.7
 void EPWM7_TZINT_ISR(void)    // EPWM-7
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT2.8 - Reserved

// -----------------------------------------------------------
// PIE Group 3 - MUXed into CPU INT3
// -----------------------------------------------------------

// INT 3.1
 void EPWM1_INT_ISR(void)     // EPWM-1
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT3.2
 void EPWM2_INT_ISR(void)     // EPWM-2
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT3.3
 void EPWM3_INT_ISR(void)    // EPWM-3
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT3.4
 void EPWM4_INT_ISR(void)    // EPWM-4
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT3.5
 void EPWM5_INT_ISR(void)    // EPWM-5
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT3.5
 void EPWM6_INT_ISR(void)    // EPWM-6
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT3.5
 void EPWM7_INT_ISR(void)    // EPWM-7
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT3.8 - Reserved

// -----------------------------------------------------------
// PIE Group 4 - MUXed into CPU INT4
// -----------------------------------------------------------

// INT 4.1
/* void ECAP1_INT_ISR(void)    // ECAP-1
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}
*/
// INT4.2 - Reserved
// INT4.3 - Reserved
// INT4.4 - Reserved
// INT4.5 - Reserved
// INT4.6 - Reserved
// INT4.7 - Reserved
// INT4.8 - Reserved

// -----------------------------------------------------------
// PIE Group 5 - MUXed into CPU INT5
// -----------------------------------------------------------

// INT 5.1
 void EQEP1_INT_ISR(void)    // EQEP-1
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP5;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT5.2 - Reserved
// INT5.3 - Reserved
// INT5.4 - Reserved
// INT5.5 - Reserved
// INT5.6 - Reserved
// INT5.7 - Reserved
// INT5.8 - Reserved

// -----------------------------------------------------------
// PIE Group 6 - MUXed into CPU INT6
// -----------------------------------------------------------

// INT6.1
 void SPIRXINTA_ISR(void)    // SPI-A
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP6;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT6.2
 void SPITXINTA_ISR(void)     // SPI-A
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP6;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT6.3
/* void SPIRXINTB_ISR(void)    // SPI-B
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP6;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}
*/
// INT6.4
 void SPITXINTB_ISR(void)     // SPI-B
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP6;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT6.5 - Reserved
// INT6.6 - Reserved
// INT6.7 - Reserved
// INT6.8 - Reserved

// -----------------------------------------------------------
// PIE Group 7 - MUXed into CPU INT7
// -----------------------------------------------------------

// -----------------------------------------------------------
// PIE Group 8 - MUXed into CPU INT8
// -----------------------------------------------------------
/*
// INT8.1
 void I2CINT1A_ISR(void)     // I2C-A
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}
*/
// INT8.2
 void I2CINT2A_ISR(void)     // I2C-A
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT8.3 - Reserved
// INT8.4 - Reserved
// INT8.5 - Reserved
// INT8.6 - Reserved
// INT8.7 - Reserved
// INT8.8 - Reserved

// -----------------------------------------------------------
// PIE Group 9 - MUXed into CPU INT9
// -----------------------------------------------------------
/*
// INT9.1
 void SCIRXINTA_ISR(void)     // SCI-A
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);

}

// INT9.2
 void SCITXINTA_ISR(void)     // SCI-A
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);

}
*/
// INT9.3
 void LIN0INTA_ISR(void)     // LIN-A
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);

}

// INT9.4
 void LIN1INTA_ISR(void)     // LIN-A
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);

}

// INT9.5
 void ECAN0INTA_ISR(void)  // eCAN-A
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);

}

// INT9.6
 void ECAN1INTA_ISR(void)  // eCAN-A
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);

}

// INT9.7 - Reserved
// INT9.8 - Reserved

// -----------------------------------------------------------
// PIE Group 10 - MUXed into CPU INT10
// -----------------------------------------------------------

// INT10.1 - Reserved or ADCINT1_ISR
// INT10.2 - Reserved or ADCINT2_ISR

// INT10.3
 void ADCINT3_ISR(void)    // ADC
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT10.4
 void ADCINT4_ISR(void)    // ADC
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT10.5
 void ADCINT5_ISR(void)    // ADC
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT10.6
 void ADCINT6_ISR(void)    // ADC
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT10.7
 void ADCINT7_ISR(void)    // ADC
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT10.8
 void ADCINT8_ISR(void)    // ADC
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}


// -----------------------------------------------------------
// PIE Group 11 - MUXed into CPU INT11
// -----------------------------------------------------------

// INT11.1
 void CLA1_INT1_ISR(void)   // MCLA
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP11;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code

  asm ("      ESTOP0");
  for(;;);

}

// INT11.2
 void CLA1_INT2_ISR(void)  // MCLA
{

  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP11;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code

  asm("	  ESTOP0");
  for(;;);

}

// INT11.3
 void CLA1_INT3_ISR(void)    // MCLA
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP11;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT11.4
 void CLA1_INT4_ISR(void)    // MCLA
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP11;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT11.5
 void CLA1_INT5_ISR(void)    // MCLA
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP11;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT11.6
 void CLA1_INT6_ISR(void)    // MCLA
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP11;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT11.7
 void CLA1_INT7_ISR(void)    // MCLA
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP11;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// INT11.8
 void CLA1_INT8_ISR(void)    // MCLA
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP11;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// -----------------------------------------------------------
// PIE Group 12 - MUXed into CPU INT12
// -----------------------------------------------------------

// INT12.1
 void XINT3_ISR(void)  // External interrupt 3
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);

}
// INT12.2 - Reserved
// INT12.3 - Reserved
// INT12.4 - Reserved
// INT12.5 - Reserved
// INT12.6 - Reserved

// INT12.7
 void LVF_ISR(void)  // CLA1 - overflow
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);

}

// INT12.8
 void LUF_ISR(void)  // CLA1 - underflow
{
  // Insert ISR Code here

  // To receive more interrupts from this PIE group, acknowledge this interrupt
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);

}

//---------------------------------------------------------------------------
// Catch All Default ISRs:
//

 void EMPTY_ISR(void)  // Empty ISR - only does a return.
{

}

 void PIE_RESERVED(void)  // Reserved space.  For test.
{
  asm ("      ESTOP0");
  for(;;);
}

 void rsvd_ISR(void)      // For test
{
  asm ("      ESTOP0");
  for(;;);
}

//===========================================================================
// End of file.
//===========================================================================

