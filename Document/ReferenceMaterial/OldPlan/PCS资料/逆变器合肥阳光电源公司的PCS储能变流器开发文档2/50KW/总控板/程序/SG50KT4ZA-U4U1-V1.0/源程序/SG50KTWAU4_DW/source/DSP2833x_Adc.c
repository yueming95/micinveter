// TI File $Revision: /main/4 $
// Checkin $Date: July 30, 2007   14:15:53 $
//###########################################################################
//
// FILE:	DSP2833x_Adc.c
//
// TITLE:	DSP2833x ADC Initialization & Support Functions.
//
//###########################################################################
// $TI Release: DSP2833x Header Files V1.01 $
// $Release Date: September 26, 2007 $
//###########################################################################

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

#define ADC_CKPS   0x0   // ADC module clock = HSPCLK/1      = 25.5MHz/(1)   = 25.0 MHz
#define ADC_SHCLK  0x1   // S/H width in ADC module periods                  = 2 ADC cycle
#define ADC_usDELAY2 20L
#define ADC_usDELAY  5000L
//---------------------------------------------------------------------------
// InitAdc:
//---------------------------------------------------------------------------
// This function initializes ADC to a known state.
//
void InitAdc(void)
{
    extern void DSP28x_usDelay(Uint32 Count);


    // *IMPORTANT*
	// The ADC_cal function, which  copies the ADC calibration values from TI reserved
	// OTP into the ADCREFSEL and ADCOFFTRIM registers, occurs automatically in the
	// Boot ROM. If the boot ROM code is bypassed during the debug process, the
	// following function MUST be called for the ADC to function according
	// to specification. The clocks to the ADC MUST be enabled before calling this
	// function.
	// See the device data manual and/or the ADC Reference
	// Manual for more information.

	EALLOW;
	SysCtrlRegs.HISPCP.all=0x0003;//HSPCLK=SYSCLKOUT/2*ADC_MODCLK2=150/(2*3)=25MHz
	EDIS;
	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.ADCENCLK=1;
	ADC_cal();
	EDIS;
	AdcRegs.ADCTRL3.all=0x00E0;
	DELAY_US(ADC_usDELAY);
	AdcRegs.ADCTRL1.bit.ACQ_PS=ADC_SHCLK;//脉冲宽度=（1+ACQ_PS）*ADCLK
	AdcRegs.ADCTRL3.bit.ADCCLKPS=ADC_CKPS;//设置转换频率
	AdcRegs.ADCTRL1.bit.CPS=0;
	AdcRegs.ADCTRL1.bit.SEQ_CASC=1;//级联模式
	AdcRegs.ADCTRL3.bit.SMODE_SEL=0;//设置顺序采样模式
	AdcRegs.ADCTRL1.bit.CONT_RUN=0;//启动停止模式
	AdcRegs.ADCTRL1.bit.SEQ_OVRD=1;//enable sequencer override feature
	AdcRegs.ADCREFSEL.bit.REF_SEL=1;  //external reference2.048V
	DELAY_US(1000000L);//1000000L



    // To powerup the ADC the ADCENCLK bit should be set first to enable
    // clocks, followed by powering up the bandgap, reference circuitry, and ADC core.
    // Before the first conversion is performed a 5ms delay must be observed
	// after power up to give all analog circuits time to power up and settle

    // Please note that for the delay function below to operate correctly the
	// CPU_CLOCK_SPEED define statement in the DSP2833x_Examples.h file must
	// contain the correct CPU clock period in nanoseconds.


}
void StartAdc(void) //起动AD
{
	//AdcRegs.ADCTRL2.bit.SOC_SEQ1=1;  
    AdcRegs.ADCTRL2.bit.SOC_SEQ1=1; 
    while(AdcRegs.ADCST.bit.SEQ1_BSY==0);//等待转换完成
	asm(" NOP ");
    asm(" NOP ");
    asm(" CLRC SXM ");
}

//===========================================================================
// End of file.
//===========================================================================
