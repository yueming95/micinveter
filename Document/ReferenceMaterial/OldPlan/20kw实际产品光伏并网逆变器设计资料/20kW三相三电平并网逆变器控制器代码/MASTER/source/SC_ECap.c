/***********************************************************************
 *    FILENAME : SC_ECap.c
 *
 *    PURPOSE  : 
 *    HISTORY  :
 *           DATE          VERSION      AUTHOR        NOTE
 ************************************************************************/
#include "SC_master.h"

//---------------------------------------------------------------------------
// InitECap:
//---------------------------------------------------------------------------
// This function initializes the eCAP(s) to a known state.
//
void InitECap(void)
{	
	// Setup APWM mode on eCAP1, set period and compare registers                    
	ECap1Regs.ECCTL2.bit.CAP_APWM  = 1;				// Active low
	// 0 ECAP module operates in capture mode. This mode forces the following configuration:
		//， Inhibits TSCTR resets via CTR = PRD event
		//， Inhibits shadow loads on CAP1 and 2 registers
		//， Permits user to enable CAP1-4 register load
		//， CAPx/APWMx pin operates as a capture input
	// 1 ECAP module operates in APWM mode. This mode forces the following configuration:
		//， Resets TSCTR on CTR = PRD event (period boundary
		//， Permits shadow loading on CAP1 and 2 registers
		//， Disables loading of time-stamps into CAP1-4 registers
		//， CAPx/APWMx pin operates as a APWM output

	ECap1Regs.ECCTL2.bit.APWMPOL   = 0;
	// 0, Output is active high;
	// 1, Output is active low;

	ECap1Regs.ECCTL2.bit.SYNCI_EN  = 1;				// Enable sync mode
	// 0, Disable sync-in option
	// 1, Enable counter(TSCTR) to be loaded from CTRPHS register upon either a SYNCI signal or a S/W force event

	ECap1Regs.ECCTL2.bit.SYNCO_SEL = 0;	
	// 00,select sync-in event to be the sync-out signal (pass through)
	// 01,select CTR = PRD event to be the sync-out signal
	// 10,Disable sync out signal
	// 11,Disable sync out signal

	ECap1Regs.ECEINT.all           = 0;				// enable Compare Equal Int
	// bit15-8      0's:    reserved
	// bit7         0:      CTR=CMP, 0 = compare interrupt disabled
	// bit6         0:      CTR=PRD, 0 = period interrupt disabled
	// bit5         0:      CTROVF,  0 = overflow interrupt disabled
	// bit4         0:      CEVT4,   0 = event 4 interrupt disabled
	// bit3         0:      CEVT3,   0 = event 3 interrupt disabled
	// bit2         0:      CEVT2,   0 = event 2 interrupt disabled
	// bit1         0:      CEVT1,   0 = event 1 interrupt disabled
	// bit0         0:      reserved

	ECap1Regs.CAP1 = APWM_PERIOD-1;			// Set Period value, 3750 = 150MHz/40kHz
	ECap1Regs.CAP2 = 0;//500;			// Set Compare value
	ECap1Regs.CTRPHS = 0;//1875;		// delay 5.7us


	ECap2Regs.ECCTL2.bit.CAP_APWM = 1;	// Active low
	// 0 ECAP module operates in capture mode. This mode forces the following configuration:
		//， Inhibits TSCTR resets via CTR = PRD event
		//， Inhibits shadow loads on CAP1 and 2 registers
		//， Permits user to enable CAP1-4 register load
		//， CAPx/APWMx pin operates as a capture input
	// 1 ECAP module operates in APWM mode. This mode forces the following configuration:
		//， Resets TSCTR on CTR = PRD event (period boundary
		//， Permits shadow loading on CAP1 and 2 registers
		//， Disables loading of time-stamps into CAP1-4 registers
		//， CAPx/APWMx pin operates as a APWM output

	ECap2Regs.ECCTL2.bit.APWMPOL = 0;
	// 0, Output is active high;
	// 1, Output is active low;
	ECap2Regs.ECCTL2.bit.SYNCI_EN = 1;	// Enable sync mode
	// 0, Disable sync-in option
	// 1, Enable counter(TSCTR) to be loaded from CTRPHS register upon either a SYNCI signal or a S/W force event
	ECap2Regs.ECCTL2.bit.SYNCO_SEL = 0;	
	// 00,select sync-in event to be the sync-out signal (pass through)
	// 01,select CTR = PRD event to be the sync-out signal
	// 10,Disable sync out signal
	// 11,Disable sync out signal
	ECap2Regs.ECEINT.all = 0; // enable Compare Equal Int
	// bit15-8      0's:    reserved
	// bit7         0:      CTR=CMP, 0 = compare interrupt disabled
	// bit6         0:      CTR=PRD, 0 = period interrupt disabled
	// bit5         0:      CTROVF, 0 = overflow interrupt disabled
	// bit4         0:      CEVT4, 1 = event 4 interrupt enabled
	// bit3         0:      CEVT3, 0 = event 3 interrupt disabled
	// bit2         0:      CEVT2, 0 = event 2 interrupt disabled
	// bit1         0:      CEVT1, 0 = event 1 interrupt disabled
	// bit0         0:      reserved

	ECap2Regs.CAP1 = APWM_PERIOD-1;			// Set Period value, 3750 = 150MHz/40kHz,
	ECap2Regs.CAP2 = 0;//500;			// Set Compare value
	ECap2Regs.CTRPHS = 3875;//5625;//7020;//4680;


	// Setup APWM mode on CAP3, set period and compare registers
	ECap3Regs.ECCTL2.bit.CAP_APWM = 1;	// Active low
	// 0 ECAP module operates in capture mode. This mode forces the following configuration:
		//， Inhibits TSCTR resets via CTR = PRD event
		//， Inhibits shadow loads on CAP1 and 2 registers
		//， Permits user to enable CAP1-4 register load
		//， CAPx/APWMx pin operates as a capture input
	// 1 ECAP module operates in APWM mode. This mode forces the following configuration:
		//， Resets TSCTR on CTR = PRD event (period boundary
		//， Permits shadow loading on CAP1 and 2 registers
		//， Disables loading of time-stamps into CAP1-4 registers
		//， CAPx/APWMx pin operates as a APWM output

	ECap3Regs.ECCTL2.bit.APWMPOL = 0;
	// 0, Output is active high;
	// 1, Output is active low;
	ECap3Regs.ECCTL2.bit.SYNCI_EN = 1;	// Enable sync mode
	// 0, Disable sync-in option
	// 1, Enable counter(TSCTR) to be loaded from CTRPHS register upon either a SYNCI signal or a S/W force event
	ECap3Regs.ECCTL2.bit.SYNCO_SEL = 0;	
	// 00,select sync-in event to be the sync-out signal (pass through)
	// 01,select CTR = PRD event to be the sync-out signal
	// 10,Disable sync out signal
	// 11,Disable sync out signal
	ECap3Regs.ECEINT.all = 0; // enable Compare Equal Int
	// bit15-8      0's:    reserved
	// bit7         0:      CTR=CMP, 0 = compare interrupt disabled
	// bit6         0:      CTR=PRD, 0 = period interrupt disabled
	// bit5         0:      CTROVF, 0 = overflow interrupt disabled
	// bit4         0:      CEVT4, 1 = event 4 interrupt enabled
	// bit3         0:      CEVT3, 0 = event 3 interrupt disabled
	// bit2         0:      CEVT2, 0 = event 2 interrupt disabled
	// bit1         0:      CEVT1, 0 = event 1 interrupt disabled
	// bit0         0:      reserved

	ECap3Regs.CAP1 = APWM_PERIOD-1;			// Set Period value, 3750 = 150MHz/40kHz
	ECap3Regs.CAP2 = 0;//500;			// Set Compare value
	ECap3Regs.CTRPHS = 4680;		// Set Compare value

	// Setup APWM mode on CAP3, set period and compare registers
	ECap4Regs.ECCTL2.bit.CAP_APWM = 1;	// Active low
	// 0 ECAP module operates in capture mode. This mode forces the following configuration:
		//， Inhibits TSCTR resets via CTR = PRD event
		//， Inhibits shadow loads on CAP1 and 2 registers
		//， Permits user to enable CAP1-4 register load
		//， CAPx/APWMx pin operates as a capture input
	// 1 ECAP module operates in APWM mode. This mode forces the following configuration:
		//， Resets TSCTR on CTR = PRD event (period boundary
		//， Permits shadow loading on CAP1 and 2 registers
		//， Disables loading of time-stamps into CAP1-4 registers
		//， CAPx/APWMx pin operates as a APWM output

	ECap4Regs.ECCTL2.bit.APWMPOL = 0;
	// 0, Output is active high;
	// 1, Output is active low;
	ECap4Regs.ECCTL2.bit.SYNCI_EN = 1;	// Enable sync mode
	// 0, Disable sync-in option
	// 1, Enable counter(TSCTR) to be loaded from CTRPHS register upon either a SYNCI signal or a S/W force event
	ECap4Regs.ECCTL2.bit.SYNCO_SEL = 0;	
	// 00,select sync-in event to be the sync-out signal (pass through)
	// 01,select CTR = PRD event to be the sync-out signal
	// 10,Disable sync out signal
	// 11,Disable sync out signal
	ECap4Regs.ECEINT.all = 0; // enable Compare Equal Int
	// bit15-8      0's:    reserved
	// bit7         0:      CTR=CMP, 0 = compare interrupt disabled
	// bit6         0:      CTR=PRD, 0 = period interrupt disabled
	// bit5         0:      CTROVF, 0 = overflow interrupt disabled
	// bit4         0:      CEVT4, 1 = event 4 interrupt enabled
	// bit3         0:      CEVT3, 0 = event 3 interrupt disabled
	// bit2         0:      CEVT2, 0 = event 2 interrupt disabled
	// bit1         0:      CEVT1, 0 = event 1 interrupt disabled
	// bit0         0:      reserved

	ECap4Regs.CAP1 = APWM_PERIOD-1;			// Set Period value, 3750 = 150MHz/40kHz
	ECap4Regs.CAP2 = 0;//500;			// Set Compare value
	ECap4Regs.CTRPHS = 1875;		// 
	// Start counters
	ECap1Regs.ECCTL2.bit.TSCTRSTOP = 1;
	ECap2Regs.ECCTL2.bit.TSCTRSTOP = 1;
	ECap3Regs.ECCTL2.bit.TSCTRSTOP = 1;
	ECap4Regs.ECCTL2.bit.TSCTRSTOP = 1;

//	Configure eCAP5 unit to test frequency
//GpioCtrlRegs.GPBPUD.bit.GPIO48 = 0;    // Enable pull-up on GPIO48 (CAP5)
//GpioCtrlRegs.GPBQSEL2.bit.GPIO48 = 0; // Synch to SYSCLKOUT GPIO48 (CAP5)

	ECap5Regs.ECEINT.all = 0x0000;			// Disable all eCAP interrupts
	ECap5Regs.ECCLR.all = 0xFFFF;       // Clear all CAP interrupt flags
	ECap5Regs.ECCTL1.bit.CAPLDEN = 0;	// Disabled loading of capture results
	ECap5Regs.ECCTL2.bit.TSCTRSTOP = 0;	// Stop the counter

	ECap5Regs.ECCTL2.bit.CONT_ONESHT = 0;			// 0-continuous mode
	ECap5Regs.ECCTL2.bit.STOP_WRAP   = 0;			// Wrap after Capture Event 1 in continuous mode
	ECap5Regs.ECCTL1.bit.CAP1POL     = 1;			// 0-Capture Event 1 triggered on a rising  edge (RE)
													// 1-Capture Event 1 triggered on a falling edge (FE)
	ECap5Regs.ECCTL1.bit.CAP2POL     = 0;
	ECap5Regs.ECCTL1.bit.CAP3POL     = 0;
	ECap5Regs.ECCTL1.bit.CAP4POL     = 0;
	ECap5Regs.ECCTL1.bit.CTRRST1     = 1;			// 1-Reset counter after Event 1 time-stamp has been captured (used in difference mode operation)
	ECap5Regs.ECCTL1.bit.CTRRST2     = 1;
	ECap5Regs.ECCTL1.bit.CTRRST3     = 1;
	ECap5Regs.ECCTL1.bit.CTRRST4     = 1;
	ECap5Regs.ECCTL2.bit.SYNCI_EN    = 0;			// 0-Disable sync-in option
	ECap5Regs.ECCTL2.bit.SYNCO_SEL   = 2;			// 2-Disable sync out signal
	ECap5Regs.ECCTL1.bit.PRESCALE    = 0;			// Event Filter prescale : 0-Divide by 1
	ECap5Regs.ECCTL2.bit.TSCTRSTOP   = 1;			// Time Stamp (TSCTR) Counter Stop (freeze) Control : 1-free-running
	ECap5Regs.ECCTL2.bit.REARM       = 1;			// Arms the one-shot sequence as follows:
													// 1) Resets the Mod4 counter to zero
													// 2) Unfreezes the Mod4 counter
													// 3) Enables capture register loads
	ECap5Regs.ECCTL1.bit.CAPLDEN     = 1;			// Enable CAP1-4 register loads at capture event time.
	ECap5Regs.ECEINT.bit.CEVT1       = 1;			// Enable Capture Event 1 as an interrupt source

//	Configure eCAP6 unit to test S-phase grid's frequency
	ECap6Regs.ECEINT.all             = 0x0000;		// Disable all eCAP interrupts
	ECap6Regs.ECCLR.all              = 0xFFFF;		// Clear all CAP interrupt flags
	ECap6Regs.ECCTL1.bit.CAPLDEN     = 0;			// Disabled loading of capture results
	ECap6Regs.ECCTL2.bit.TSCTRSTOP   = 0;			// Stop the counter

	ECap6Regs.ECCTL2.bit.CONT_ONESHT = 0;
	ECap6Regs.ECCTL2.bit.STOP_WRAP   = 0;
	ECap6Regs.ECCTL1.bit.CAP1POL     = 0;
	ECap6Regs.ECCTL1.bit.CAP2POL     = 0;
	ECap6Regs.ECCTL1.bit.CAP3POL     = 0;
	ECap6Regs.ECCTL1.bit.CAP4POL     = 0;
	ECap6Regs.ECCTL1.bit.CTRRST1     = 1;
	ECap6Regs.ECCTL1.bit.CTRRST2     = 1;
	ECap6Regs.ECCTL1.bit.CTRRST3     = 1;
	ECap6Regs.ECCTL1.bit.CTRRST4     = 1;
	ECap6Regs.ECCTL2.bit.SYNCI_EN    = 0;
	ECap6Regs.ECCTL2.bit.SYNCO_SEL   = 2;
	ECap6Regs.ECCTL1.bit.PRESCALE    = 0;
	ECap6Regs.ECCTL2.bit.TSCTRSTOP   = 1;
	ECap6Regs.ECCTL2.bit.REARM       = 1;
	ECap6Regs.ECCTL1.bit.CAPLDEN     = 1;
	ECap6Regs.ECEINT.bit.CEVT1       = 1;

	PieCtrlRegs.PIEIER4.bit.INTx5    = 1;			// Enable ECAP5_INT in PIE group 4
	PieCtrlRegs.PIEIER4.bit.INTx6    = 1;			// Enable ECAP6_INT in PIE group 4
	IER |= M_INT4;									// Enable INT4 in IER to enable PIE group 4
}

//===========================================================================
// End of file.
//===========================================================================
