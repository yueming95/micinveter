/*=============================================================================*
 *
 *  PRODUCT  : PWM Module configuration and basic functions
 *  FILENAME : SC_EPwm.c 
 *  PURPOSE  : ePWM Module configuration and Timer/PWMoutput En/Disable process.. 
 *  HISTORY  :
 *           DATE          VERSION      AUTHOR        NOTE
 *    config EPWM, as following:
 *	     EPWM1A/B,EPWM2A/B,EPWM3A/B with easy programable 
 *		(1)switch frequency, (2)deadband,(3)MinDuty/MaxDuty and 
 *		(4)enhanced precise SOC point scheme.
 *
 *============================================================================*/
#include "SC_master.h"

/*=============================================================================*
 * FUNCTION: InitEPwm()
 * PURPOSE : Initializes the Enhanced PWM modules.
 *			 To precisely capture the most valuable sampling point L current 
 *			 with high frequency sampling for Energy, additional Timer5 and 
 *			 Timer6 are used to supply additional leading-type SOC and equal 
 *			 interval sampling, with :
 *			 TBCLK=SYSCLKOUT/(HSPCLKDIV*CLKDIV)=150MHz/(2*1)=75MHz
 * CALLED BY:  Main.c  
 *============================================================================*/
void InitEPwm(void)
{ // start of InitEPwm()

	/* Must disable the clock to the ePWM modules if you want all ePMW modules 
	   synchronized, according to : TMS320x2833x, 2823x System Control and 
	   Interrupts Reference Guide (Rev. C).pdf. */

	asm(" EALLOW");					// Enable EALLOW protected register access
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
	asm(" EDIS");					// Disable EALLOW protected register access

	// Configure ePWM1/2/3/4/5/6 for symmetric PWM

	// ePWM1 Configuration 

	EPwm1Regs.TBPRD = PWM_HALF_PERIOD;		//1875  actual period = 2*PWM_HALF_PERIOD
	EPwm1Regs.TBPHS.half.TBPHS = 0x0;		// Set timer phase, zero

	EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;		// up-down mode
	EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;		// Master module
	EPwm1Regs.TBCTL.bit.PRDLD = TB_SHADOW;		// reload from shadow
	EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO; 	// sync CTR=0
	EPwm1Regs.TBCTL.bit.SWFSYNC = 0x0;
	EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV2;	// prescaler = 2
	EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;
	EPwm1Regs.TBCTL.bit.PHSDIR = TB_DOWN;		// don't care
	EPwm1Regs.TBCTL.bit.FREE_SOFT = 0x3;	
	//bit15-14     11:     	FREE/SOFT, 11 = ignore emulation suspend
	//bit13        0:      	PHSDIR, 0 = count down after sync event, 
							//1 = count up after sync event
	//bit12-10     000:    	CLKDIV, 000 = 2^0, 001 = 2^1, ..., 111 = 2^7, 
							//TBCLK = SYSCLKOUT/(HSPCLKDIV*CLKDIV)
	//bit9-7       001:    	HSPCLKDIV, Prescaler, 000 = 1, 001 = 2, 
							//010 = 4, ..., 110 = 12, 111 = 14.
	//bit6         0:      	SWFSYNC, 0 = no software sync produced, 
							//1 = writing a 1 forces a one-time sync pulse
	//bit5-4       01:     	SYNCOSEL, sync-output-select, 00 = EPWMxSYNC, 
							//01 =>CTR=0, 10 =>CTR=CMPB, 11 = sync-out disabled
	//bit3         0:      	PRDLD, 0 = reload PRD on counter=0 from shadow, 
							//1 = without shadow
	//bit2         0:      	PHSEN, 0 = phase control disabled, master mode, 
							//1 = slave mode 
	//bit1-0       11:     	CTRMODE, Counter Mode, 00 = Up, 01 = Down, 
							//10 = Up-Down, 11 = Stop timer stopped (disabled)
	EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
	EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	//EPwm1Regs.CMPCTL.all = 0x0005;			// Compare control register 0101
	//bit15-10	0:    	reserved
	//bit9		0:      SHDWBFULL, read-only
	//bit8		0:      SHDWAFULL, read-only
	//bit7		0:      reserved
	//bit6		0:      SHDWBMODE, 0 = shadow mode, 1= immediate
	//bit5		0:      reserved
	//bit4		0:      SHDWAMODE, 0 = shadow mode, 1= immediate
	//bit3-2	01:     LOADBMODE, 00 => CTR=0; 01 =>CTR=PRD, 
						//10 => CTR=0 or CTR=PRD, 11, disable
	//bit1-0	01:     LOADAMODE, 00 => CTR=0; 01 =>CTR=PRD, 
						//10 => CTR=0 or CTR=PRD, 11, disable

	// config EPWM1A output when(1)incrementing and (2)equals CMPA
	EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;			
	// config EPWM1A output when(1)decrementing and (2)equals CMPA
	EPwm1Regs.AQCTLA.bit.CAD = AQ_SET;			
	//Action-qualifier control register A	
	//bit15-12     0000:   reserved
	//bit11-10     00:     CBD, 00 = do nothing
	//bit9-8       00:     CBU, 00 = do nothing
	//bit7-6       10:     CAD, 00 = disable, 01, clear, 10 = set, 11 = toggle
	//bit5-4       01:     CAU, 00 = disable, 01, clear, 10 = set, 11 = toggle
	//bit3-2       00:     PRD, 00 = do nothing
	//bit1-0       00:     ZRO, 00 = do nothing
	// force AQC output low
	// config EPWM1A output when(1)incrementing and (2)equals CMPA
	EPwm1Regs.AQCTLB.bit.CBU = AQ_CLEAR;			
	// config EPWM1A output when(1)decrementing and (2)equals CMPA
	EPwm1Regs.AQCTLB.bit.CBD = AQ_SET;			
	//Action-qualifier control register A	
	//bit15-12     0000:   reserved
	//bit11-10     00:     CBD, 00 = do nothing
	//bit9-8       00:     CBU, 00 = do nothing
	//bit7-6       01:     CAD, 00 = disable, 01, clear, 10 = set, 11 = toggle
	//bit5-4       10:     CAU, 00 = disable, 01, clear, 10 = set, 11 = toggle
	//bit3-2       00:     PRD, 00 = do nothing
	//bit1-0       00:     ZRO, 00 = do nothing
	// AQCSFRC load mode
	EPwm1Regs.AQSFRC.bit.RLDCSF = 0x3;			
	//00 Load on event counter equauls zero
	//01 Load on event counter equauls period
	//10 Load on event counter equauls zero or period
	//11 Load immediately
	
	EPwm1Regs.DBFED = DEAD_BAND_DURATION;		// Dead band duration for FED
	EPwm1Regs.DBRED = DEAD_BAND_DURATION;		// Dead band duration for RED
	
	EPwm1Regs.PCCTL.bit.CHPEN = CHP_DISABLE;				// PWM chopper unit disabled
	
	// 1100 Enable SOCA to ADC, Enable INT and INTSEL.
	EPwm1Regs.ETSEL.bit.SOCAEN  = 1;
	EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_ZERO;
	EPwm1Regs.ETSEL.bit.SOCBSEL = ET_CTR_ZERO;
	//EPwm1Regs.ETSEL.all = 0x0900;	//0000 1001 0000 0000		
	//bit15		0:		SOCBEN, 0 = disable SOCB
	//bit14-12	000:	SOCBSEL, don't care
	//bit11		1:		SOCAEN, 1 = enable SOCA, 0 = disable
	//bit10-8	001:	SOCASEL, EPWMxSOCA selection, 000 = reserved, 
						//001 => TBCTR=0, 010 => TBCTR=TBPRD, 011 = reserved
						//100 = CMPA inc, 101 = CMPA dec, 110 = CMPB inc, 
						//111 = CMPB dec.
	//bit7-4	0000:	reserved
	//bit3		0:		INTEN, EPWMx_INT, 0 = disable interrupt, 1 = enable
	//bit2-0	000:	INTSEL, 000 = Reserved, 001 -> TBCTR=0; 
						//010 -> TBCTR=TBPRD; 011 = Reserved; 100 = CMPA Up, 
						//101 = CMPA Down
						//110 = CMPB Up; 111 = CMPB down.
	// 0000 0001 0000 0000 Configure SOCA
	EPwm1Regs.ETPS.bit.SOCAPRD = ET_1ST;
	//EPwm1Regs.ETPS.all = 0x0100;	//0000 0001 0000 0000		
	//bit15-14	00:		EPWMxSOCB, read-only
	//bit13-12	00:		SOCBPRD, 00 = disable, 
						//01,10,11=gerneate EPWMxSOCB pulse on the 1/2/3 event.
	//bit11-10	00:	 	EPWMxSOCA, read-only
	//bit9-8	01:		SOCAPRD, 00 = disable, 
						//01,10,11=gerneate EPWMxSOCA pulse on the 1/2/3 event.
	//bit7-4	0000:	reserved
	//bit3-2	00:		INTCNT, read-only 
	//bit1-0	00:		INTPRD, 00 = disable, 
						//01,10,11=gerneate EPWMx_INT on the 1/2/3 event.

		

	// ePWM2 Configuration 
	EPwm2Regs.TBPRD = PWM_HALF_PERIOD;		// actual period = 2*PWM_HALF_PERIOD
	EPwm2Regs.TBPHS.half.TBPHS = 0x0;		// Set timer phase, zero

	EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;		// up-down mode
	EPwm2Regs.TBCTL.bit.PHSEN = TB_DISABLE;		// Master module
	EPwm2Regs.TBCTL.bit.PRDLD = TB_SHADOW;		// reload from shadow
	EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN; 	// EPWMxSYNC
	EPwm2Regs.TBCTL.bit.SWFSYNC = 0x0;
	EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV2;	// prescaler = 2
	EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV1;
	EPwm2Regs.TBCTL.bit.PHSDIR = TB_DOWN;		// don't care
	EPwm2Regs.TBCTL.bit.FREE_SOFT = 0x3;	
	//bit15-14     11:     	FREE/SOFT, 11 = ignore emulation suspend
	//bit13        0:      	PHSDIR, 0 = count down after sync event, 
							//1 = count up after sync event
	//bit12-10     000:    	CLKDIV, 000 = 2^0, 001 = 2^1, ..., 111 = 2^7, 
							//TBCLK = SYSCLKOUT/(HSPCLKDIV*CLKDIV)
	//bit9-7       001:    	HSPCLKDIV, Prescaler, 000 = 1, 001 = 2, 
							//010 = 4, ..., 110 = 12, 111 = 14.
	//bit6         0:      	SWFSYNC, 0 = no software sync produced, 
							//1 = writing a 1 forces a one-time sync pulse
	//bit5-4       00:     	SYNCOSEL, sync-output-select, 00 = EPWMxSYNC, 
							//01 =>CTR=0, 10 =>CTR=CMPB, 11 = sync-out disabled
	//bit3         0:      	PRDLD, 0 = reload PRD on counter=0 from shadow, 
							//1 = without shadow
	//bit2         0:      	PHSEN, 0 = phase control disabled, master mode, 
							//1 = slave mode 
	//bit1-0       11:     	CTRMODE, Counter Mode, 00 = Up, 01 = Down, 
							//10 = Up-Down, 11 = Stop timer stopped (disabled)
	EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
	EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	//EPwm2Regs.CMPCTL.all = 0x0005;			// Compare control register
	//bit15-10	0:    	reserved
	//bit9		0:      SHDWBFULL, read-only
	//bit8		0:      SHDWAFULL, read-only
	//bit7		0:      reserved
	//bit6		0:      SHDWBMODE, 0 = shadow mode, 1= immediate
	//bit5		0:      reserved
	//bit4		0:      SHDWAMODE, 0 = shadow mode, 1= immediate
	//bit3-2	01:     LOADBMODE, 00 => CTR=0; 01 =>CTR=PRD, 
						//10 => CTR=0 or CTR=PRD, 11, disable
	//bit1-0	01:     LOADAMODE, 00 => CTR=0; 01 =>CTR=PRD, 
						//10 => CTR=0 or CTR=PRD, 11, disable

	// config EPWM1A output when(1)incrementing and (2)equals CMPA
	EPwm2Regs.AQCTLA.bit.CAU = AQ_CLEAR;			
	// config EPWM1A output when(1)decrementing and (2)equals CMPA
	EPwm2Regs.AQCTLA.bit.CAD = AQ_SET;			
	//Action-qualifier control register A	
	//bit15-12     0000:   reserved
	//bit11-10     00:     CBD, 00 = do nothing
	//bit9-8       00:     CBU, 00 = do nothing
	//bit7-6       01:     CAD, 00 = disable, 01, clear, 10 = set, 11 = toggle
	//bit5-4       10:     CAU, 00 = disable, 01, clear, 10 = set, 11 = toggle
	//bit3-2       00:     PRD, 00 = do nothing
	//bit1-0       00:     ZRO, 00 = do nothing
	// config EPWM1A output when(1)incrementing and (2)equals CMPA
	EPwm2Regs.AQCTLB.bit.CBU = AQ_CLEAR;			
	// config EPWM1A output when(1)decrementing and (2)equals CMPA
	EPwm2Regs.AQCTLB.bit.CBD = AQ_SET;			
	//Action-qualifier control register A	
	//bit15-12     0000:   reserved
	//bit11-10     00:     CBD, 00 = do nothing
	//bit9-8       00:     CBU, 00 = do nothing
	//bit7-6       01:     CAD, 00 = disable, 01, clear, 10 = set, 11 = toggle
	//bit5-4       10:     CAU, 00 = disable, 01, clear, 10 = set, 11 = toggle
	//bit3-2       00:     PRD, 00 = do nothing
	//bit1-0       00:     ZRO, 00 = do nothing

	// AQCSFRC load mode
	EPwm2Regs.AQSFRC.bit.RLDCSF = 0x3;			
	//00 Load on event counter equauls zero
	//01 Load on event counter equauls period
	//10 Load on event counter equauls zero or period
	//11 Load immediately
	
	EPwm2Regs.DBFED = DEAD_BAND_DURATION;		// Dead band duration for FED
	EPwm2Regs.DBRED = DEAD_BAND_DURATION;		// Dead band duration for RED
	
	EPwm2Regs.PCCTL.bit.CHPEN = CHP_DISABLE;				// PWM chopper unit disabled


	// ePWM3 Configuration 
	EPwm3Regs.TBPRD = PWM_HALF_PERIOD;		// actual period = 2*PWM_HALF_PERIOD
	EPwm3Regs.TBPHS.half.TBPHS = 0x0;		// Set timer phase, zero

	EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;		// up-down mode
	EPwm3Regs.TBCTL.bit.PHSEN = TB_DISABLE;		// Master module
	EPwm3Regs.TBCTL.bit.PRDLD = TB_SHADOW;		// reload from shadow
	EPwm3Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN; 	// sync when CTR to zero
	EPwm3Regs.TBCTL.bit.SWFSYNC = 0x0;
	EPwm3Regs.TBCTL.bit.HSPCLKDIV = TB_DIV2;	// prescaler = 2
	EPwm3Regs.TBCTL.bit.CLKDIV = TB_DIV1;
	EPwm3Regs.TBCTL.bit.PHSDIR = TB_DOWN;		// don't care
	EPwm3Regs.TBCTL.bit.FREE_SOFT = 0x3;	
	//bit15-14     11:     	FREE/SOFT, 11 = ignore emulation suspend
	//bit13        0:      	PHSDIR, 0 = count down after sync event, 
							//1 = count up after sync event
	//bit12-10     000:    	CLKDIV, 000 = 2^0, 001 = 2^1, ..., 111 = 2^7, 
							//TBCLK = SYSCLKOUT/(HSPCLKDIV*CLKDIV)
	//bit9-7       001:    	HSPCLKDIV, Prescaler, 000 = 1, 001 = 2, 
							//010 = 4, ..., 110 = 12, 111 = 14.
	//bit6         0:      	SWFSYNC, 0 = no software sync produced, 
							//1 = writing a 1 forces a one-time sync pulse
	//bit5-4       01:     	SYNCOSEL, sync-output-select, 00 = EPWMxSYNC, 
							//01 =>CTR=0, 10 =>CTR=CMPB, 11 = sync-out disabled
	//bit3         0:      	PRDLD, 0 = reload PRD on counter=0 from shadow, 
							//1 = without shadow
	//bit2         0:      	PHSEN, 0 = phase control disabled, master mode, 
							//1 = slave mode 
	//bit1-0       11:     	CTRMODE, Counter Mode, 00 = Up, 01 = Down, 
							//10 = Up-Down, 11 = Stop timer stopped (disabled)
	EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
	EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm3Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	//EPwm3Regs.CMPCTL.all = 0x0005;			// Compare control register
	//bit15-10	0:    	reserved
	//bit9		0:      SHDWBFULL, read-only
	//bit8		0:      SHDWAFULL, read-only
	//bit7		0:      reserved
	//bit6		0:      SHDWBMODE, 0 = shadow mode, 1= immediate
	//bit5		0:      reserved
	//bit4		0:      SHDWAMODE, 0 = shadow mode, 1= immediate
	//bit3-2	01:     LOADBMODE, 00 => CTR=0; 01 =>CTR=PRD, 
						//10 => CTR=0 or CTR=PRD, 11, disable
	//bit1-0	01:     LOADAMODE, 00 => CTR=0; 01 =>CTR=PRD, 
						//10 => CTR=0 or CTR=PRD, 11, disable

	// config EPWM3A output when(1)incrementing and (2)equals CMPA
	EPwm3Regs.AQCTLA.bit.CAU = AQ_CLEAR;			
	// config EPWM3A output when(1)decrementing and (2)equals CMPA
	EPwm3Regs.AQCTLA.bit.CAD = AQ_SET;			
	//Action-qualifier control register A	
	//bit15-12     0000:   reserved
	//bit11-10     00:     CBD, 00 = do nothing
	//bit9-8       00:     CBU, 00 = do nothing
	//bit7-6       01:     CAD, 00 = disable, 01, clear, 10 = set, 11 = toggle
	//bit5-4       10:     CAU, 00 = disable, 01, clear, 10 = set, 11 = toggle
	//bit3-2       00:     PRD, 00 = do nothing
	//bit1-0       00:     ZRO, 00 = do nothing
	EPwm3Regs.AQCTLB.bit.CBU = AQ_CLEAR;			
	// config EPWM1A output when(1)decrementing and (2)equals CMPA
	EPwm3Regs.AQCTLB.bit.CBD = AQ_SET;			
	//Action-qualifier control register A	
	//bit15-12     0000:   reserved
	//bit11-10     00:     CBD, 00 = do nothing
	//bit9-8       00:     CBU, 00 = do nothing
	//bit7-6       01:     CAD, 00 = disable, 01, clear, 10 = set, 11 = toggle
	//bit5-4       10:     CAU, 00 = disable, 01, clear, 10 = set, 11 = toggle
	//bit3-2       00:     PRD, 00 = do nothing
	//bit1-0       00:     ZRO, 00 = do nothing
	// AQCSFRC load mode
	EPwm3Regs.AQSFRC.bit.RLDCSF = 0x3;			
	//00 Load on event counter equauls zero
	//01 Load on event counter equauls period
	//10 Load on event counter equauls zero or period
	//11 Load immediately

	
	EPwm3Regs.DBFED = DEAD_BAND_DURATION;		// Dead band duration for FED
	EPwm3Regs.DBRED = DEAD_BAND_DURATION;		// Dead band duration for RED
	
	EPwm3Regs.PCCTL.bit.CHPEN = CHP_DISABLE;				// PWM chopper unit disabled

	// ePWM4 Configuration 
	EPwm4Regs.TBPRD = PWM_HALF_PERIOD;		// actual period = 2*PWM_HALF_PERIOD
	EPwm4Regs.TBPHS.half.TBPHS = 0x0;		// Set timer phase, zero

	EPwm4Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;		// up-down mode
	EPwm4Regs.TBCTL.bit.PHSEN = TB_DISABLE;		// Master module
	EPwm4Regs.TBCTL.bit.PRDLD = TB_SHADOW;		// reload from shadow
	EPwm4Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN; 	// sync when CTR to zero
	EPwm4Regs.TBCTL.bit.SWFSYNC = 0x0;
	EPwm4Regs.TBCTL.bit.HSPCLKDIV = TB_DIV2;	// prescaler = 2
	EPwm4Regs.TBCTL.bit.CLKDIV = TB_DIV1;
	EPwm4Regs.TBCTL.bit.PHSDIR = TB_DOWN;		// don't care
	EPwm4Regs.TBCTL.bit.FREE_SOFT = 0x3;	
	//bit15-14     11:     	FREE/SOFT, 11 = ignore emulation suspend
	//bit13        0:      	PHSDIR, 0 = count down after sync event, 
							//1 = count up after sync event
	//bit12-10     000:    	CLKDIV, 000 = 2^0, 001 = 2^1, ..., 111 = 2^7, 
							//TBCLK = SYSCLKOUT/(HSPCLKDIV*CLKDIV)
	//bit9-7       001:    	HSPCLKDIV, Prescaler, 000 = 1, 001 = 2, 
							//010 = 4, ..., 110 = 12, 111 = 14.
	//bit6         0:      	SWFSYNC, 0 = no software sync produced, 
							//1 = writing a 1 forces a one-time sync pulse
	//bit5-4       01:     	SYNCOSEL, sync-output-select, 00 = EPWMxSYNC, 
							//01 =>CTR=0, 10 =>CTR=CMPB, 11 = sync-out disabled
	//bit3         0:      	PRDLD, 0 = reload PRD on counter=0 from shadow, 
							//1 = without shadow
	//bit2         0:      	PHSEN, 0 = phase control disabled, master mode, 
							//1 = slave mode 
	//bit1-0       11:     	CTRMODE, Counter Mode, 00 = Up, 01 = Down, 
							//10 = Up-Down, 11 = Stop timer stopped (disabled)
	EPwm4Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	EPwm4Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
	EPwm4Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm4Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	//EPwm4Regs.CMPCTL.all = 0x0005;			// Compare control register
	//bit15-10	0:    	reserved
	//bit9		0:      SHDWBFULL, read-only
	//bit8		0:      SHDWAFULL, read-only
	//bit7		0:      reserved
	//bit6		0:      SHDWBMODE, 0 = shadow mode, 1= immediate
	//bit5		0:      reserved
	//bit4		0:      SHDWAMODE, 0 = shadow mode, 1= immediate
	//bit3-2	01:     LOADBMODE, 00 => CTR=0; 01 =>CTR=PRD, 
						//10 => CTR=0 or CTR=PRD, 11, disable
	//bit1-0	01:     LOADAMODE, 00 => CTR=0; 01 =>CTR=PRD, 
						//10 => CTR=0 or CTR=PRD, 11, disable

	// config EPWM1A output when(1)incrementing and (2)equals CMPA
	EPwm4Regs.AQCTLA.bit.CAU = AQ_CLEAR;			
	// config EPWM1A output when(1)decrementing and (2)equals CMPA
	EPwm4Regs.AQCTLA.bit.CAD = AQ_SET;			
	//Action-qualifier control register A	
	//bit15-12     0000:   reserved
	//bit11-10     00:     CBD, 00 = do nothing
	//bit9-8       00:     CBU, 00 = do nothing
	//bit7-6       01:     CAD, 00 = disable, 01, clear, 10 = set, 11 = toggle
	//bit5-4       10:     CAU, 00 = disable, 01, clear, 10 = set, 11 = toggle
	//bit3-2       00:     PRD, 00 = do nothing
	//bit1-0       00:     ZRO, 00 = do nothing
	EPwm4Regs.AQCTLB.bit.CBU = AQ_CLEAR;			
	// config EPWM1A output when(1)decrementing and (2)equals CMPA
	EPwm4Regs.AQCTLB.bit.CBD = AQ_SET;			
	//Action-qualifier control register A	
	//bit15-12     0000:   reserved
	//bit11-10     00:     CBD, 00 = do nothing
	//bit9-8       00:     CBU, 00 = do nothing
	//bit7-6       01:     CAD, 00 = disable, 01, clear, 10 = set, 11 = toggle
	//bit5-4       10:     CAU, 00 = disable, 01, clear, 10 = set, 11 = toggle
	//bit3-2       00:     PRD, 00 = do nothing
	//bit1-0       00:     ZRO, 00 = do nothing
	EPwm4Regs.AQSFRC.bit.RLDCSF = 0x3;			
	//00 Load on event counter equauls zero
	//01 Load on event counter equauls period
	//10 Load on event counter equauls zero or period
	//11 Load immediately
	// force AQC output low
	
	EPwm4Regs.DBFED = DEAD_BAND_DURATION;		// Dead band duration for FED
	EPwm4Regs.DBRED = DEAD_BAND_DURATION;		// Dead band duration for RED
	
	EPwm4Regs.PCCTL.bit.CHPEN = CHP_DISABLE;				// PWM chopper unit disabled

	// ePWM5 Configuration 
	EPwm5Regs.TBPRD = PWM_HALF_PERIOD;		// actual period = 2*PWM_HALF_PERIOD
	EPwm5Regs.TBPHS.half.TBPHS = 0x0;		// Set timer phase, zero

	EPwm5Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;		// up-down mode	
	EPwm5Regs.TBCTL.bit.PHSEN = TB_DISABLE;		// Master module
	EPwm5Regs.TBCTL.bit.PRDLD = TB_SHADOW;		// reload from shadow
	EPwm5Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN; 	// sync when CTR to zero
	EPwm5Regs.TBCTL.bit.SWFSYNC = 0x0;
	EPwm5Regs.TBCTL.bit.HSPCLKDIV = TB_DIV2;	// prescaler = 2
	EPwm5Regs.TBCTL.bit.CLKDIV = TB_DIV1;
	EPwm5Regs.TBCTL.bit.PHSDIR = TB_DOWN;		// don't care
	EPwm5Regs.TBCTL.bit.FREE_SOFT = 0x3;	
	//bit15-14     11:     	FREE/SOFT, 11 = ignore emulation suspend
	//bit13        0:      	PHSDIR, 0 = count down after sync event, 
							//1 = count up after sync event
	//bit12-10     000:    	CLKDIV, 000 = 2^0, 001 = 2^1, ..., 111 = 2^7, 
							//TBCLK = SYSCLKOUT/(HSPCLKDIV*CLKDIV)
	//bit9-7       001:    	HSPCLKDIV, Prescaler, 000 = 1, 001 = 2, 
							//010 = 4, ..., 110 = 12, 111 = 14.
	//bit6         0:      	SWFSYNC, 0 = no software sync produced, 
							//1 = writing a 1 forces a one-time sync pulse
	//bit5-4       00:     	SYNCOSEL, sync-output-select, 00 = EPWMxSYNC, 
							//01 =>CTR=0, 10 =>CTR=CMPB, 11 = sync-out disabled
	//bit3         0:      	PRDLD, 0 = reload PRD on counter=0 from shadow, 
							//1 = without shadow
	//bit2         0:      	PHSEN, 0 = phase control disabled, master mode, 
							//1 = slave mode 
	//bit1-0       11:     	CTRMODE, Counter Mode, 00 = Up, 01 = Down, 
							//10 = Up-Down, 11 = Stop timer stopped (disabled)
	EPwm5Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	EPwm5Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
	EPwm5Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm5Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	//EPwm5Regs.CMPCTL.all = 0x0005;			// Compare control register
	//bit15-10	0:    	reserved
	//bit9		0:      SHDWBFULL, read-only
	//bit8		0:      SHDWAFULL, read-only
	//bit7		0:      reserved
	//bit6		0:      SHDWBMODE, 0 = shadow mode, 1= immediate
	//bit5		0:      reserved
	//bit4		0:      SHDWAMODE, 0 = shadow mode, 1= immediate
	//bit3-2	01:     LOADBMODE, 00 => CTR=0; 01 =>CTR=PRD, 
						//10 => CTR=0 or CTR=PRD, 11, disable
	//bit1-0	01:     LOADAMODE, 00 => CTR=0; 01 =>CTR=PRD, 
						//10 => CTR=0 or CTR=PRD, 11, disable

	// config EPWM1A output when(1)incrementing and (2)equals CMPA
	EPwm5Regs.AQCTLA.bit.CAU = AQ_CLEAR;			
	// config EPWM1A output when(1)decrementing and (2)equals CMPA
	EPwm5Regs.AQCTLA.bit.CAD = AQ_SET;			
	//Action-qualifier control register A	
	//bit15-12     0000:   reserved
	//bit11-10     00:     CBD, 00 = do nothing
	//bit9-8       00:     CBU, 00 = do nothing
	//bit7-6       01:     CAD, 00 = disable, 01, clear, 10 = set, 11 = toggle
	//bit5-4       10:     CAU, 00 = disable, 01, clear, 10 = set, 11 = toggle
	//bit3-2       00:     PRD, 00 = do nothing
	//bit1-0       00:     ZRO, 00 = do nothing
	EPwm5Regs.AQCTLB.bit.CBU = AQ_CLEAR;			
	// config EPWM1A output when(1)decrementing and (2)equals CMPA
	EPwm5Regs.AQCTLB.bit.CBD = AQ_SET;			
	//Action-qualifier control register A	
	//bit15-12     0000:   reserved
	//bit11-10     00:     CBD, 00 = do nothing
	//bit9-8       00:     CBU, 00 = do nothing
	//bit7-6       01:     CAD, 00 = disable, 01, clear, 10 = set, 11 = toggle
	//bit5-4       10:     CAU, 00 = disable, 01, clear, 10 = set, 11 = toggle
	//bit3-2       00:     PRD, 00 = do nothing
	//bit1-0       00:     ZRO, 00 = do nothing
	EPwm5Regs.AQSFRC.bit.RLDCSF = 0x3;			
	//00 Load on event counter equauls zero
	//01 Load on event counter equauls period
	//10 Load on event counter equauls zero or period
	//11 Load immediately
	// force AQC output low
	
	EPwm5Regs.DBFED = DEAD_BAND_DURATION;		// Dead band duration for FED
	EPwm5Regs.DBRED = DEAD_BAND_DURATION;		// Dead band duration for RED
	
	EPwm5Regs.PCCTL.bit.CHPEN = CHP_DISABLE;				// PWM chopper unit disabled

	// ePWM6 Configuration 
	EPwm6Regs.TBPRD = PWM_HALF_PERIOD;		// actual period = 2*PWM_HALF_PERIOD
	EPwm6Regs.TBPHS.half.TBPHS = 0x0;		// Set timer phase, zero

	EPwm6Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;		// up-down mode
	EPwm6Regs.TBCTL.bit.PHSEN = TB_DISABLE;		// master module
	EPwm6Regs.TBCTL.bit.PRDLD = TB_SHADOW;		// reload from shadow
	EPwm6Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN; 	// sync when CTR to zero
	EPwm6Regs.TBCTL.bit.SWFSYNC = 0x0;
	EPwm6Regs.TBCTL.bit.HSPCLKDIV = TB_DIV2;	// prescaler = 2
	EPwm6Regs.TBCTL.bit.CLKDIV = TB_DIV1;
	EPwm6Regs.TBCTL.bit.PHSDIR = TB_DOWN;		// don't care
	EPwm6Regs.TBCTL.bit.FREE_SOFT = 0x3;	
	//bit15-14     11:     	FREE/SOFT, 11 = ignore emulation suspend
	//bit13        0:      	PHSDIR, 0 = count down after sync event, 
							//1 = count up after sync event
	//bit12-10     000:    	CLKDIV, 000 = 2^0, 001 = 2^1, ..., 111 = 2^7, 
							//TBCLK = SYSCLKOUT/(HSPCLKDIV*CLKDIV)
	//bit9-7       001:    	HSPCLKDIV, Prescaler, 000 = 1, 001 = 2, 
							//010 = 4, ..., 110 = 12, 111 = 14.
	//bit6         0:      	SWFSYNC, 0 = no software sync produced, 
							//1 = writing a 1 forces a one-time sync pulse
	//bit5-4       01:     	SYNCOSEL, sync-output-select, 00 = EPWMxSYNC, 
							//01 =>CTR=0, 10 =>CTR=CMPB, 11 = sync-out disabled
	//bit3         0:      	PRDLD, 0 = reload PRD on counter=0 from shadow, 
							//1 = without shadow
	//bit2         0:      	PHSEN, 0 = phase control disabled, master mode, 
							//1 = slave mode 
	//bit1-0       11:     	CTRMODE, Counter Mode, 00 = Up, 01 = Down, 
							//10 = Up-Down, 11 = Stop timer stopped (disabled)
	EPwm6Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
	EPwm6Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
	EPwm6Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm6Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
	//EPwm6Regs.CMPCTL.all = 0x0005;			// Compare control register
	//bit15-10	0:    	reserved
	//bit9		0:      SHDWBFULL, read-only
	//bit8		0:      SHDWAFULL, read-only
	//bit7		0:      reserved
	//bit6		0:      SHDWBMODE, 0 = shadow mode, 1= immediate
	//bit5		0:      reserved
	//bit4		0:      SHDWAMODE, 0 = shadow mode, 1= immediate
	//bit3-2	01:     LOADBMODE, 00 => CTR=0; 01 =>CTR=PRD, 
						//10 => CTR=0 or CTR=PRD, 11, disable
	//bit1-0	01:     LOADAMODE, 00 => CTR=0; 01 =>CTR=PRD, 
						//10 => CTR=0 or CTR=PRD, 11, disable

	// config EPWM1A output when(1)incrementing and (2)equals CMPA
	EPwm6Regs.AQCTLA.bit.CAU = AQ_CLEAR;			
	// config EPWM1A output when(1)decrementing and (2)equals CMPA
	EPwm6Regs.AQCTLA.bit.CAD = AQ_SET;			
	//Action-qualifier control register A	
	//bit15-12     0000:   reserved
	//bit11-10     00:     CBD, 00 = do nothing
	//bit9-8       00:     CBU, 00 = do nothing
	//bit7-6       01:     CAD, 00 = disable, 01, clear, 10 = set, 11 = toggle
	//bit5-4       10:     CAU, 00 = disable, 01, clear, 10 = set, 11 = toggle
	//bit3-2       00:     PRD, 00 = do nothing
	//bit1-0       00:     ZRO, 00 = do nothing
	EPwm6Regs.AQCTLB.bit.CBU = AQ_CLEAR;			
	// config EPWM1A output when(1)decrementing and (2)equals CMPA
	EPwm6Regs.AQCTLB.bit.CBD = AQ_SET;			
	//Action-qualifier control register A	
	//bit15-12     0000:   reserved
	//bit11-10     00:     CBD, 00 = do nothing
	//bit9-8       00:     CBU, 00 = do nothing
	//bit7-6       01:     CAD, 00 = disable, 01, clear, 10 = set, 11 = toggle
	//bit5-4       10:     CAU, 00 = disable, 01, clear, 10 = set, 11 = toggle
	//bit3-2       00:     PRD, 00 = do nothing
	//bit1-0       00:     ZRO, 00 = do nothing
	EPwm6Regs.AQSFRC.bit.RLDCSF = 0x3;			
	//00 Load on event counter equauls zero
	//01 Load on event counter equauls period
	//10 Load on event counter equauls zero or period
	//11 Load immediately
	// force AQC output low
	
	EPwm6Regs.DBFED = DEAD_BAND_DURATION;		// Dead band duration for FED
	EPwm6Regs.DBRED = DEAD_BAND_DURATION;		// Dead band duration for RED

	EPwm6Regs.PCCTL.bit.CHPEN = CHP_DISABLE;				// PWM chopper unit disabled

	// initial TB counter
	EPwm1Regs.TBCTR = 0x0000;		// Clear timer counter
	EPwm2Regs.TBCTR = 0x0000;		// Clear timer counter
	EPwm3Regs.TBCTR = 0x0000;		// Clear timer counter
	EPwm4Regs.TBCTR = 0x0000;		// Clear timer counter
	EPwm5Regs.TBCTR = 0x0000;		// Clear timer counter
	EPwm6Regs.TBCTR = 0x0000;		// Clear timer counter
	asm(" EALLOW");					// Enable EALLOW protected register access
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;	// HSPCLK to ePWM modules enabled
	asm(" EDIS");					// Disable EALLOW protected register access

	EPwm1Regs.DBCTL.bit.OUT_MODE =DB_FULL_ENABLE;
	EPwm1Regs.DBCTL.bit.POLSEL =DB_ACTV_HIC;
	EPwm1Regs.DBCTL.bit.IN_MODE =DBA_RED_DBB_FED;
	//EPwm1Regs.DBCTL.all = 0x002B;//2B   //0000 0000 0010 1011
	//Dead-Band Generator Control register 	
	//bit15-6	0000:   reserved
	//bit5-4	10:     IN_MODE, select input source for RED and FED, EPWMxA/EPWMxB
						// 00 = A->both, 01 = B->RED and A->FED, 
						// 10 = A->RED and B->FED, 11 = B->both
	//bit3-2	10:		POLSEL, polarity select for output, 00 = AH, 01 = ALC, 
						//10 = AHC, 11 = AL
	//bit1-0	11:		OUT_MODE, 00 = no DB, 01 = No RED, 10 = No FED, 
						//11 = DB fully enabled
	EPwm1Regs.AQCSFRC.bit.CSFA = 0x1;
	EPwm1Regs.AQCSFRC.bit.CSFB = 0x2;
	//00 Forcing disabled, i.e., has no effect
	//01 Forces a continuous low on output B
	//10 Forces a continuous high on output B
	//11 Software forcing is disabled and has no effect
	
	EPwm2Regs.DBCTL.bit.OUT_MODE =DB_FULL_ENABLE;
	EPwm2Regs.DBCTL.bit.POLSEL =DB_ACTV_HIC;
	EPwm2Regs.DBCTL.bit.IN_MODE =DBA_RED_DBB_FED;
	//EPwm2Regs.DBCTL.all = 0x002B;
	
	EPwm2Regs.AQCSFRC.bit.CSFA = 0x1;
	EPwm2Regs.AQCSFRC.bit.CSFB = 0x2;
	//00 Forcing disabled, i.e., has no effect
	//01 Forces a continuous low on output B
	//10 Forces a continuous high on output B
	//11 Software forcing is disabled and has no effect
	

	EPwm3Regs.DBCTL.bit.OUT_MODE =DB_FULL_ENABLE;
	EPwm3Regs.DBCTL.bit.POLSEL =DB_ACTV_HIC;
	EPwm3Regs.DBCTL.bit.IN_MODE =DBA_RED_DBB_FED;
	//EPwm3Regs.DBCTL.all = 0x002B;
	
	EPwm3Regs.AQCSFRC.bit.CSFA = 0x1;
	EPwm3Regs.AQCSFRC.bit.CSFB = 0x2;
	//00 Forcing disabled, i.e., has no effect
	//01 Forces a continuous low on output B
	//10 Forces a continuous high on output B
	//11 Software forcing is disabled and has no effect

	
	EPwm4Regs.DBCTL.bit.OUT_MODE =DB_FULL_ENABLE;
	EPwm4Regs.DBCTL.bit.POLSEL =DB_ACTV_HIC;
	EPwm4Regs.DBCTL.bit.IN_MODE =DBA_RED_DBB_FED;
	//EPwm4Regs.DBCTL.all = 0x002B;

	EPwm4Regs.AQCSFRC.bit.CSFA = 0x1;
	EPwm4Regs.AQCSFRC.bit.CSFB = 0x2;
	//00 Forcing disabled, i.e., has no effect
	//01 Forces a continuous low on output B
	//10 Forces a continuous high on output B
	//11 Software forcing is disabled and has no effect

	
	EPwm5Regs.DBCTL.bit.OUT_MODE =DB_FULL_ENABLE;
	EPwm5Regs.DBCTL.bit.POLSEL =DB_ACTV_HIC;
	EPwm5Regs.DBCTL.bit.IN_MODE =DBA_RED_DBB_FED;
	//EPwm5Regs.DBCTL.all = 0x002B;
	
	EPwm5Regs.AQCSFRC.bit.CSFA = 0x1;
	EPwm5Regs.AQCSFRC.bit.CSFB = 0x2;
	//00 Forcing disabled, i.e., has no effect
	//01 Forces a continuous low on output B
	//10 Forces a continuous high on output B
	//11 Software forcing is disabled and has no effect

	
	EPwm6Regs.DBCTL.bit.OUT_MODE =DB_FULL_ENABLE;
	EPwm6Regs.DBCTL.bit.POLSEL =DB_ACTV_HIC;
	EPwm6Regs.DBCTL.bit.IN_MODE =DBA_RED_DBB_FED;
	//EPwm6Regs.DBCTL.all = 0x002B;

	EPwm6Regs.AQCSFRC.bit.CSFA = 0x1;
	EPwm6Regs.AQCSFRC.bit.CSFB = 0x2;
	//00 Forcing disabled, i.e., has no effect
	//01 Forces a continuous low on output B
	//10 Forces a continuous high on output B
	//11 Software forcing is disabled and has no effect

} // end of InitEPwm()


//---------------------------------------------------------------------------
// Example: InitEPwmSyncGpio: 
//---------------------------------------------------------------------------
// This function initializes GPIO pins to function as ePWM Synch pins
//

void InitEPwmSyncGpio(void)
{

   EALLOW;

/* Configure EPWMSYNCI  */
   
/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user. 
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

   GpioCtrlRegs.GPAPUD.bit.GPIO6 = 0;    // Enable pull-up on GPIO6 (EPWMSYNCI)
// GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0;   // Enable pull-up on GPIO32 (EPWMSYNCI)    

/* Set qualification for selected pins to asynch only */
// This will select synch to SYSCLKOUT for the selected pins.
// Comment out other unwanted lines.

   GpioCtrlRegs.GPAQSEL1.bit.GPIO6 = 0;   // Synch to SYSCLKOUT GPIO6 (EPWMSYNCI)
// GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 0;  // Synch to SYSCLKOUT GPIO32 (EPWMSYNCI)    

/* Configure EPwmSync pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be EPwmSync functional pins.
// Comment out other unwanted lines.   

   GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 2;    // Enable pull-up on GPIO6 (EPWMSYNCI)
// GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 2;   // Enable pull-up on GPIO32 (EPWMSYNCI)    



/* Configure EPWMSYNC0  */

/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user. 
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

// GpioCtrlRegs.GPAPUD.bit.GPIO6 = 0;    // Enable pull-up on GPIO6 (EPWMSYNC0)
   GpioCtrlRegs.GPBPUD.bit.GPIO33 = 0;   // Enable pull-up on GPIO33 (EPWMSYNC0)    

// GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 3;    // Enable pull-up on GPIO6 (EPWMSYNC0)
   GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 2;   // Enable pull-up on GPIO33 (EPWMSYNC0)    

}



//---------------------------------------------------------------------------
// Example: InitTzGpio: 
//---------------------------------------------------------------------------
// This function initializes GPIO pins to function as Trip Zone (TZ) pins
//
// Each GPIO pin can be configured as a GPIO pin or up to 3 different
// peripheral functional pins. By default all pins come up as GPIO
// inputs after reset.  
// 

void InitTzGpio(void)
{
   
}
/*=============================================================================*
 * FUNCTION: PWMOutputsEnable()
 * PURPOSE : ePWM1A/B, 2A/B, 3A/B, 4A/B, 5A/B, 6A/B, enabled, INV driver are enabled.
 * CALLED BY: 
 *     Main.c or MainStatusMachine.c 
 * 
 *============================================================================*/
void PWMOutputsEnable(void)
{ // start of PWMOutputsEnable()

	HWI_disable();
	EPwm1Regs.AQCSFRC.bit.CSFB = 0x0;
	EPwm2Regs.AQCSFRC.bit.CSFB = 0x0;
	EPwm3Regs.AQCSFRC.bit.CSFB = 0x0;
	EPwm4Regs.AQCSFRC.bit.CSFB = 0x0;
	EPwm5Regs.AQCSFRC.bit.CSFB = 0x0;
	EPwm6Regs.AQCSFRC.bit.CSFB = 0x0;
	DelayUs(50);		
	EPwm1Regs.AQCSFRC.bit.CSFA = 0x0;
	EPwm2Regs.AQCSFRC.bit.CSFA = 0x0;
	EPwm3Regs.AQCSFRC.bit.CSFA = 0x0;
	EPwm4Regs.AQCSFRC.bit.CSFA = 0x0;
	EPwm5Regs.AQCSFRC.bit.CSFA = 0x0;
	EPwm6Regs.AQCSFRC.bit.CSFA = 0x0;
	HWI_enable();

} // end of PWMOutputsEnable()


/*=============================================================================*
 * FUNCTION: PWMOutputsDisable()
 * PURPOSE : ePWM1A/B, 2A/B, and 3A/B enabled, PWM Timer5 also disabled
 * CALLED BY: 
 *     Main.c or MainStatusMachine.c 
 * 
 *============================================================================*/
void PWMOutputsDisable(void)
{
	// Boost off
	ECap1Regs.CAP4 = 0;
	ECap2Regs.CAP4 = 0;
	ECap3Regs.CAP4 = 0;
	ECap4Regs.CAP4 = 0;
	// INV off
	EPwm1Regs.AQCSFRC.bit.CSFA = 0x1;
	EPwm2Regs.AQCSFRC.bit.CSFA = 0x1;
	EPwm3Regs.AQCSFRC.bit.CSFA = 0x1;
	EPwm4Regs.AQCSFRC.bit.CSFA = 0x1;
	EPwm5Regs.AQCSFRC.bit.CSFA = 0x1;
	EPwm6Regs.AQCSFRC.bit.CSFA = 0x1;

	DelayUs(3);

	EPwm1Regs.AQCSFRC.bit.CSFB = 0x2;
	EPwm2Regs.AQCSFRC.bit.CSFB = 0x2;
	EPwm3Regs.AQCSFRC.bit.CSFB = 0x2;
	EPwm4Regs.AQCSFRC.bit.CSFB = 0x2;
	EPwm5Regs.AQCSFRC.bit.CSFB = 0x2;
	EPwm6Regs.AQCSFRC.bit.CSFB = 0x2;
} 

//===========================================================================
// End of file.
//===========================================================================
