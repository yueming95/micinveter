                                                                             // TI File $Revision: /main/3 $
// Checkin $Date: November 19, 2009   10:24:27 $
//###########################################################################
//
// FILE:    DSP2803x_Adc.c
//
// TITLE:   DSP2803x ADC Initialization & Support Functions.
//
//###########################################################################
// $TI Release: 2803x C/C++ Header Files V1.21 $
// $Release Date: December 1, 2009 $
//###########################################################################

#include "DSP2803x_Device.h"     // DSP2803x Headerfile Include File
#include "DSP2803x_Examples.h"   // DSP2803x Examples Include File

#define ADC_usDELAY  1000L

void AdcOffsetSelfCal(void);
void Adc_PowerOn(void);

void Adc_PowerOn(void)
{
    // extern void DSP28x_usDelay(Uint32 Count);

    // *IMPORTANT*
    // The Device_cal function, which copies the ADC calibration values from TI reserved
    // OTP into the ADCREFSEL and ADCOFFTRIM registers, occurs automatically in the
    // Boot ROM. If the boot ROM code is bypassed during the debug process, the
    // following function MUST be called for the ADC to function according
    // to specification. The clocks to the ADC MUST be enabled before calling this
    // function.
    // See the device data manual and/or the ADC Reference
    // Manual for more information.

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;
    (*Device_cal)();
    EDIS;

    // To powerup the ADC the ADCENCLK bit should be set first to enable
    // clocks, followed by powering up the bandgap, reference circuitry, and ADC core.
    // Before the first conversion is performed a 5ms delay must be observed
    // after power up to give all analog circuits time to power up and settle

    // Please note that for the delay function below to operate correctly the
    // CPU_RATE define statement in the DSP2803x_Examples.h file must
    // contain the correct CPU clock period in nanoseconds.
    EALLOW;
    AdcRegs.ADCCTL1.bit.ADCBGPWD  = 1;      // Power ADC BG
    AdcRegs.ADCCTL1.bit.ADCREFPWD = 1;      // Power reference
    AdcRegs.ADCCTL1.bit.ADCPWDN   = 1;      // Power ADC
    AdcRegs.ADCCTL1.bit.ADCENABLE = 1;      // Enable ADC
    AdcRegs.ADCCTL1.bit.ADCREFSEL = 1;      // Select out BG
    EDIS;

    DELAY_US(5000);         // Delay 5ms before converting ADC channels

}
void AdcOffsetSelfCal()
{
    Uint16 AdcConvMean;
    EALLOW;
    AdcRegs.ADCCTL1.bit.ADCREFSEL = 1;                  //Select out reference mode
    AdcRegs.ADCCTL1.bit.VREFLOCONV = 1;                 //Select VREFLO internal connection on B5
    AdcChanSelect(13);                                  //Select channel B5 for all SOC
    AdcRegs.ADCOFFTRIM.bit.OFFTRIM = 80;                //Apply artificial offset (+80) to account for a negative offset that may reside in the ADC core
    AdcConvMean = AdcConversion();                      //Capture ADC conversion on VREFLO
    AdcRegs.ADCOFFTRIM.bit.OFFTRIM = 80 - AdcConvMean;  //Set offtrim register with new value (i.e remove artical offset (+80) and create a two's compliment of the offset error)
    AdcRegs.ADCCTL1.bit.VREFLOCONV = 0;                 //Select external ADCIN5 input pin on B5
    EDIS;
}
//---------------------------------------------------------------------------
// InitAdc:
//---------------------------------------------------------------------------
// This function initializes ADC to a known state.
//
// NOTE: ADC INIT IS DIFFERENT ON 2803x DEVICES COMPARED TO OTHER 28X DEVICES
//
void InitAdc(void)
{
	EALLOW;
	AdcRegs.ADCSAMPLEMODE.bit.SIMULEN0	= 	0;	// Sequential Sampling Mode
	AdcRegs.ADCSAMPLEMODE.bit.SIMULEN2	= 	0;
	AdcRegs.ADCSAMPLEMODE.bit.SIMULEN4	= 	0;
	AdcRegs.ADCSAMPLEMODE.bit.SIMULEN6	= 	0;
	AdcRegs.ADCSAMPLEMODE.bit.SIMULEN8	= 	0;
	AdcRegs.ADCSAMPLEMODE.bit.SIMULEN10	= 	0;
	AdcRegs.ADCSAMPLEMODE.bit.SIMULEN12	= 	0;
	AdcRegs.ADCSAMPLEMODE.bit.SIMULEN14	= 	0;

	// ADCCLK is fixed to SYSCLOCK

	// SOCx Channel Select
	AdcRegs.ADCSOC0CTL.bit.CHSEL 	= 0xC;	// SOC0----ADCINB4----i16VPV1
	AdcRegs.ADCSOC1CTL.bit.CHSEL 	= 0xD;	// SOC1----ADCINA7----i16VPV2
	AdcRegs.ADCSOC2CTL.bit.CHSEL 	= 0xE;	// SOC2----ADCINA3----i16Viso1
	AdcRegs.ADCSOC3CTL.bit.CHSEL 	= 0xF;	// SOC3----ADCINA4----i16Viso2
	AdcRegs.ADCSOC4CTL.bit.CHSEL 	= 0xA;	// SOC4----ADCINA2----MUX-X
	AdcRegs.ADCSOC5CTL.bit.CHSEL 	= 0xB;	// SOC5----ADCINA1----MUX-Y
	AdcRegs.ADCSOC6CTL.bit.CHSEL 	= 1;	// SOC6----ADCINB6----i16DCI_R
	AdcRegs.ADCSOC7CTL.bit.CHSEL 	= 0;	// SOC7----ADCINB3----i16DCI_S
	AdcRegs.ADCSOC8CTL.bit.CHSEL 	= 8;	// SOC8----ADCINB7----i16DCI_T
	AdcRegs.ADCSOC9CTL.bit.CHSEL 	= 9;	// SOC9----ADCINB5----GFCI
	AdcRegs.ADCSOC10CTL.bit.CHSEL 	= 7;	// SOC10---ADCINA7----VGrid_R
	AdcRegs.ADCSOC11CTL.bit.CHSEL 	= 6;	// SOC11---ADCINA6----VGrid_S
	AdcRegs.ADCSOC12CTL.bit.CHSEL 	= 5;	// SOC12---ADCINA5----VGrid_T
	AdcRegs.ADCSOC13CTL.bit.CHSEL 	= 4;	// SOC13---ADCINA4----VInv_R
	AdcRegs.ADCSOC14CTL.bit.CHSEL 	= 3;	// SOC14---ADCINA3----VInv_S
	AdcRegs.ADCSOC15CTL.bit.CHSEL 	= 2;	// SOC15---ADCINA2----VInv_T

	// SOCx Acquisition Prescale, 6 ~ 63 is valid
	AdcRegs.ADCSOC0CTL.bit.ACQPS	=	6;	// Sample window is 7 cycles long (6 + 1 clock cycles).
	AdcRegs.ADCSOC1CTL.bit.ACQPS	=	6;
	AdcRegs.ADCSOC2CTL.bit.ACQPS	=	6;
	AdcRegs.ADCSOC3CTL.bit.ACQPS	=	6;
	AdcRegs.ADCSOC4CTL.bit.ACQPS	=	6;
	AdcRegs.ADCSOC5CTL.bit.ACQPS	=	6;
	AdcRegs.ADCSOC6CTL.bit.ACQPS	=	6;
	AdcRegs.ADCSOC7CTL.bit.ACQPS	=	6;
	AdcRegs.ADCSOC8CTL.bit.ACQPS	=	6;
	AdcRegs.ADCSOC9CTL.bit.ACQPS	=	6;
	AdcRegs.ADCSOC10CTL.bit.ACQPS	=	6;
	AdcRegs.ADCSOC11CTL.bit.ACQPS	=	6;
	AdcRegs.ADCSOC12CTL.bit.ACQPS	=	6;
	AdcRegs.ADCSOC13CTL.bit.ACQPS	=	6;
	AdcRegs.ADCSOC14CTL.bit.ACQPS	=	6;
	AdcRegs.ADCSOC15CTL.bit.ACQPS	=	6;

	// SOCx Trigger Source Select
	AdcRegs.ADCSOC0CTL.bit.TRIGSEL	=	5;	// 5 -- ADCTRIG5 -ePWM1, ADCSOCA
	AdcRegs.ADCSOC1CTL.bit.TRIGSEL	=	5;
	AdcRegs.ADCSOC2CTL.bit.TRIGSEL	=	5;
	AdcRegs.ADCSOC3CTL.bit.TRIGSEL	=	5;	
	AdcRegs.ADCSOC4CTL.bit.TRIGSEL	=	5;
	AdcRegs.ADCSOC5CTL.bit.TRIGSEL	=	5;	
	AdcRegs.ADCSOC6CTL.bit.TRIGSEL	=	5;
	AdcRegs.ADCSOC7CTL.bit.TRIGSEL	=	5;	
	AdcRegs.ADCSOC8CTL.bit.TRIGSEL	=	5;
	AdcRegs.ADCSOC9CTL.bit.TRIGSEL	=	5;	
	AdcRegs.ADCSOC10CTL.bit.TRIGSEL	=	5;
	AdcRegs.ADCSOC11CTL.bit.TRIGSEL	=	5;	
	AdcRegs.ADCSOC12CTL.bit.TRIGSEL	=	5;
	AdcRegs.ADCSOC13CTL.bit.TRIGSEL	=	5;	
	AdcRegs.ADCSOC14CTL.bit.TRIGSEL	=	5;
	AdcRegs.ADCSOC15CTL.bit.TRIGSEL	=	5;	

	// ADC interrupt
	AdcRegs.INTSEL1N2.bit.INT1CONT  = 0;	// Disable ADCINT1 Continuous mode
	AdcRegs.INTSEL1N2.bit.INT1SEL	= 15;	// Setup EOC15 to trigger ADCINT1 to fire
	AdcRegs.ADCCTL1.bit.INTPULSEPOS	= 1;	// ADCINT trips after AdcResults latch
	AdcRegs.INTSEL1N2.bit.INT1E     = 1;	// Enabled ADCINT1
	DELAY_US(1000L);						// Delay before converting ADC channels
	EDIS;

	PieCtrlRegs.PIEIER1.bit.INTx1 = 1;		// Enable INT 1.1 in the PIE, ADCINT1 is INT1.1
   	IER |= M_INT1; 							// Enable CPU Interrupt 1 
}

/*  AdcChanSelect-
    This function selects the ADC channel to convert by setting all SOC channel selects to a single channel.

     * IMPORTANT * This function will overwrite previous SOC channel select settings. Recommend saving
           the previous settings.
 */
void AdcChanSelect(Uint16 ch_no)
{
    AdcRegs.ADCSOC0CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC1CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC2CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC3CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC4CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC5CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC6CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC7CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC8CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC9CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC10CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC11CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC12CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC13CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC14CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC15CTL.bit.CHSEL= ch_no;
} //end AdcChanSelect

/* AdcConversion -
   This function initiates several ADC conversions and returns the average. It uses ADCINT1 and ADCINT2
   to "ping-pong" between SOC0-7 and SOC8-15 and is referred to as "ping-pong" sampling.

     * IMPORTANT * This function will overwrite previous ADC settings. Recommend saving previous settings.
*/
Uint16 AdcConversion(void)
{
    Uint16 index, SampleSize, Mean, ACQPS_Value;
    Uint32 Sum;

    index       = 0;            //initialize index to 0
    SampleSize  = 256;          //set sample size to 256 (**NOTE: Sample size must be multiples of 2^x where is an integer >= 4)
    Sum         = 0;            //set sum to 0
    Mean        = 999;          //initialize mean to known value

    //Set the ADC sample window to the desired value (Sample window = ACQPS + 1)
    ACQPS_Value = 6;
    AdcRegs.ADCSOC0CTL.bit.ACQPS  = ACQPS_Value;
    AdcRegs.ADCSOC1CTL.bit.ACQPS  = ACQPS_Value;
    AdcRegs.ADCSOC2CTL.bit.ACQPS  = ACQPS_Value;
    AdcRegs.ADCSOC3CTL.bit.ACQPS  = ACQPS_Value;
    AdcRegs.ADCSOC4CTL.bit.ACQPS  = ACQPS_Value;
    AdcRegs.ADCSOC5CTL.bit.ACQPS  = ACQPS_Value;
    AdcRegs.ADCSOC6CTL.bit.ACQPS  = ACQPS_Value;
    AdcRegs.ADCSOC7CTL.bit.ACQPS  = ACQPS_Value;
    AdcRegs.ADCSOC8CTL.bit.ACQPS  = ACQPS_Value;
    AdcRegs.ADCSOC9CTL.bit.ACQPS  = ACQPS_Value;
    AdcRegs.ADCSOC10CTL.bit.ACQPS = ACQPS_Value;
    AdcRegs.ADCSOC11CTL.bit.ACQPS = ACQPS_Value;
    AdcRegs.ADCSOC12CTL.bit.ACQPS = ACQPS_Value;
    AdcRegs.ADCSOC13CTL.bit.ACQPS = ACQPS_Value;
    AdcRegs.ADCSOC14CTL.bit.ACQPS = ACQPS_Value;
    AdcRegs.ADCSOC15CTL.bit.ACQPS = ACQPS_Value;


    //Enable ping-pong sampling

    // Enabled ADCINT1 and ADCINT2
    AdcRegs.INTSEL1N2.bit.INT1E = 1;
    AdcRegs.INTSEL1N2.bit.INT2E = 1;

    // Disable continuous sampling for ADCINT1 and ADCINT2
    AdcRegs.INTSEL1N2.bit.INT1CONT = 0;
    AdcRegs.INTSEL1N2.bit.INT2CONT = 0;

    AdcRegs.ADCCTL1.bit.INTPULSEPOS = 1;    //ADCINTs trigger at end of conversion

    // Setup ADCINT1 and ADCINT2 trigger source
    AdcRegs.INTSEL1N2.bit.INT1SEL = 6;      //EOC6 triggers ADCINT1
    AdcRegs.INTSEL1N2.bit.INT2SEL = 14;     //EOC14 triggers ADCINT2
//	AdcRegs.INTSEL1N2.bit.INT1SEL = 15;	//setup EOC15 to trigger ADCINT1 to fire
    // Setup each SOC's ADCINT trigger source
    AdcRegs.ADCINTSOCSEL1.bit.SOC0  = 2;    //ADCINT2 starts SOC0-7
    AdcRegs.ADCINTSOCSEL1.bit.SOC1  = 2;
    AdcRegs.ADCINTSOCSEL1.bit.SOC2  = 2;
    AdcRegs.ADCINTSOCSEL1.bit.SOC3  = 2;
    AdcRegs.ADCINTSOCSEL1.bit.SOC4  = 2;
    AdcRegs.ADCINTSOCSEL1.bit.SOC5  = 2;
    AdcRegs.ADCINTSOCSEL1.bit.SOC6  = 2;
    AdcRegs.ADCINTSOCSEL1.bit.SOC7  = 2;
    AdcRegs.ADCINTSOCSEL2.bit.SOC8  = 1;    //ADCINT1 starts SOC8-15
    AdcRegs.ADCINTSOCSEL2.bit.SOC9  = 1;
    AdcRegs.ADCINTSOCSEL2.bit.SOC10 = 1;
    AdcRegs.ADCINTSOCSEL2.bit.SOC11 = 1;
    AdcRegs.ADCINTSOCSEL2.bit.SOC12 = 1;
    AdcRegs.ADCINTSOCSEL2.bit.SOC13 = 1;
    AdcRegs.ADCINTSOCSEL2.bit.SOC14 = 1;
    AdcRegs.ADCINTSOCSEL2.bit.SOC15 = 1;

    DELAY_US(ADC_usDELAY);                  // Delay before converting ADC channels


    //ADC Conversion

    AdcRegs.ADCSOCFRC1.all = 0x00FF;  // Force Start SOC0-7 to begin ping-pong sampling

    while( index < SampleSize ){

        //Wait for ADCINT1 to trigger, then add ADCRESULT0-7 registers to sum
        while (AdcRegs.ADCINTFLG.bit.ADCINT1 == 0){}
        AdcRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;   //Must clear ADCINT1 flag since INT1CONT = 0
        Sum += AdcResult.ADCRESULT0;
        Sum += AdcResult.ADCRESULT1;
        Sum += AdcResult.ADCRESULT2;
        Sum += AdcResult.ADCRESULT3;
        Sum += AdcResult.ADCRESULT4;
        Sum += AdcResult.ADCRESULT5;
        Sum += AdcResult.ADCRESULT6;
        Sum += AdcResult.ADCRESULT7;

        //Wait for ADCINT2 to trigger, then add ADCRESULT8-15 registers to sum
        while (AdcRegs.ADCINTFLG.bit.ADCINT2 == 0){}
        AdcRegs.ADCINTFLGCLR.bit.ADCINT2 = 1;   //Must clear ADCINT2 flag since INT2CONT = 0
        Sum += AdcResult.ADCRESULT8;
        Sum += AdcResult.ADCRESULT9;
        Sum += AdcResult.ADCRESULT10;
        Sum += AdcResult.ADCRESULT11;
        Sum += AdcResult.ADCRESULT12;
        Sum += AdcResult.ADCRESULT13;
        Sum += AdcResult.ADCRESULT14;
        Sum += AdcResult.ADCRESULT15;

        index+=16;

    } // end data collection

    //Disable ADCINT1 and ADCINT2 to STOP the ping-pong sampling
    AdcRegs.INTSEL1N2.bit.INT1E = 0;
    AdcRegs.INTSEL1N2.bit.INT2E = 0;

    Mean = Sum / SampleSize;    //Calculate average ADC sample value

    return Mean;                //return the average

}//end AdcConversion

//===========================================================================
// End of file.
//===========================================================================

