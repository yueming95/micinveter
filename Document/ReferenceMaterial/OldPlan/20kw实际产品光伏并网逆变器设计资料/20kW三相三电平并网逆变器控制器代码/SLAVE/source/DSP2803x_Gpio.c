// TI File $Revision: /main/1 $
// Checkin $Date: December 5, 2008   18:01:01 $
//###########################################################################
//
// FILE:	DSP2803x_Gpio.c
//
// TITLE:	DSP2803x General Purpose I/O Initialization & Support Functions.
//
//###########################################################################
// $TI Release: 2803x C/C++ Header Files V1.21 $
// $Release Date: December 1, 2009 $
//###########################################################################

#include "DSP2803x_Device.h"     // DSP2803x Headerfile Include File
#include "DSP2803x_Examples.h"   // DSP2803x Examples Include File

//---------------------------------------------------------------------------
// InitGpio:
//---------------------------------------------------------------------------
// This function initializes the Gpio to a known (default) state.
//
// For more details on configuring GPIO's as peripheral functions,
// refer to the individual peripheral examples and/or GPIO setup example.
void InitGpio(void)
{
	EALLOW;
	
	// Each GPIO pin can be:
	// a) a GPIO input/output
	// b) peripheral function 1
	// c) peripheral function 2
	// d) peripheral function 3
	// By default, all are GPIO Inputs
	GpioCtrlRegs.GPAMUX1.all = 0x0000;     // GPIO functionality GPIO0-GPIO15
	GpioCtrlRegs.GPAMUX2.all = 0x0000;     // GPIO functionality GPIO16-GPIO31
	GpioCtrlRegs.GPBMUX1.all = 0x0000;     // GPIO functionality GPIO32-GPIO44

	// To avoid input noise, disable AIO functions. AIO are disabled in default.
	GpioCtrlRegs.AIOMUX1.all = 0x0000;     // Dig.IO funct. applies to AIO2,4,6,10,12,14

	// GPIO0(PIN69) SCPUtoMCPU (fast communication, output)
	GpioCtrlRegs.GPAMUX1.bit.GPIO0	= 0;
	GpioDataRegs.GPACLEAR.bit.GPIO0 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO0   = 1;
	// GPIO1(PIN68) MCPUtoSCPU (fast communication, input)
	GpioCtrlRegs.GPAMUX1.bit.GPIO1	= 0;
	GpioCtrlRegs.GPADIR.bit.GPIO1   = 0;

	// GPIO2(PIN67) DSP-State
	GpioCtrlRegs.GPAMUX1.bit.GPIO2	= 0;
	GpioDataRegs.GPACLEAR.bit.GPIO2 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO2   = 1;

	// GPIO10(PIN65) S.SEL_C
	GpioCtrlRegs.GPAMUX1.bit.GPIO10	= 0;
	GpioDataRegs.GPACLEAR.bit.GPIO10= 1;
	GpioCtrlRegs.GPADIR.bit.GPIO10  = 1;
	// GPIO3(PIN66) S.SEL_D
	GpioCtrlRegs.GPAMUX1.bit.GPIO3	= 0;
	GpioDataRegs.GPACLEAR.bit.GPIO3 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO3   = 1;

	// GPIO4(PIN63) S_BOOST_OFF
	GpioCtrlRegs.GPAMUX1.bit.GPIO4	= 0;
	GpioDataRegs.GPASET.bit.GPIO4   = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO4   = 1;
	// GPIO5(PIN62) S.AUX_OFF
	GpioCtrlRegs.GPAMUX1.bit.GPIO5	= 0;
	GpioDataRegs.GPACLEAR.bit.GPIO5 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO5   = 1;
	// GPIO40(PIN64) S_INV_OFF
	GpioCtrlRegs.GPBMUX1.bit.GPIO40 = 0;
	GpioDataRegs.GPBCLEAR.bit.GPIO40= 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO40	= 1;

	// GPIO18(PIN41) S.Relay-R1
	GpioCtrlRegs.GPAMUX2.bit.GPIO18	= 0;
	GpioDataRegs.GPACLEAR.bit.GPIO18= 1;
	GpioCtrlRegs.GPADIR.bit.GPIO18	= 1;
	// GPIO17(PIN42) S.Relay-R2
	GpioCtrlRegs.GPAMUX2.bit.GPIO17	= 0;
	GpioDataRegs.GPACLEAR.bit.GPIO17= 1;
	GpioCtrlRegs.GPADIR.bit.GPIO17	= 1;
	// GPIO8(PIN43) S.Relay-S1
	GpioCtrlRegs.GPAMUX1.bit.GPIO8	= 0;
	GpioDataRegs.GPACLEAR.bit.GPIO8 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO8   = 1;
	// GPIO25(PIN44) S.Relay-S2
	GpioCtrlRegs.GPAMUX2.bit.GPIO25	= 0;
	GpioDataRegs.GPACLEAR.bit.GPIO25= 1;
	GpioCtrlRegs.GPADIR.bit.GPIO25	= 1;
	// GPIO44(PIN45) S.Relay-T1
	GpioCtrlRegs.GPBMUX1.bit.GPIO44 = 0;
	GpioDataRegs.GPBCLEAR.bit.GPIO44= 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO44	= 1;
	// GPIO16(PIN46) S.Relay-T2
	GpioCtrlRegs.GPAMUX2.bit.GPIO16	= 0;
	GpioDataRegs.GPACLEAR.bit.GPIO16= 1;
	GpioCtrlRegs.GPADIR.bit.GPIO16	= 1;

	// GPIO27(PIN31) S.Relay-ISO1
	GpioCtrlRegs.GPAMUX2.bit.GPIO27	= 0;
	GpioDataRegs.GPACLEAR.bit.GPIO27= 1;
	GpioCtrlRegs.GPADIR.bit.GPIO27	= 1;
	// GPIO31(PIN32) S.Relay-ISO2
	GpioCtrlRegs.GPAMUX2.bit.GPIO31	= 0;
	GpioDataRegs.GPACLEAR.bit.GPIO31= 1;
	GpioCtrlRegs.GPADIR.bit.GPIO31	= 1;

	// GPIO6(PIN50) FAN PWM (EPWM4A)
	GpioCtrlRegs.GPAMUX1.bit.GPIO6	= 1;
	// GPIO7(PIN49) FAN control
	GpioCtrlRegs.GPAMUX1.bit.GPIO7	= 0;
	GpioDataRegs.GPACLEAR.bit.GPIO7 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO7   = 1;
	// GPIO22(PIN1) S.FAN.ST1
	GpioCtrlRegs.GPAMUX2.bit.GPIO22	= 0;
	GpioCtrlRegs.GPADIR.bit.GPIO22	= 0;
	// GPIO23(PIN4) S.FAN.ST2
	GpioCtrlRegs.GPAMUX2.bit.GPIO23	= 0;
	GpioCtrlRegs.GPADIR.bit.GPIO23	= 0;
	// GPIO42(PIN5) S.FAN.ST3
	GpioCtrlRegs.GPBMUX1.bit.GPIO42 = 0;
	GpioCtrlRegs.GPBDIR.bit.GPIO42	= 0;
	// GPIO43(PIN6) S.FAN.ST4
	GpioCtrlRegs.GPBMUX1.bit.GPIO43 = 0;
	GpioCtrlRegs.GPBDIR.bit.GPIO43	= 0;
	// GPIO26(PIN37) S.FAN.ST5
	GpioCtrlRegs.GPAMUX2.bit.GPIO26	= 0;
	GpioCtrlRegs.GPADIR.bit.GPIO26	= 0;
	// GPIO9(PIN39) S.FAN.ST6
	GpioCtrlRegs.GPAMUX1.bit.GPIO9	= 0;
	GpioCtrlRegs.GPADIR.bit.GPIO9   = 0;

	// GPIO13(PIN76) SPISOMIB
	GpioCtrlRegs.GPAMUX1.bit.GPIO13	= 3;
	// GPIO14(PIN77) SPICLKB
	GpioCtrlRegs.GPAMUX1.bit.GPIO14	= 3;
	// GPIO15(PIN75) SPISTEB
	GpioCtrlRegs.GPAMUX1.bit.GPIO15	= 3;
	// GPIO24(PIN80) SPISIMOB
	GpioCtrlRegs.GPAMUX2.bit.GPIO24	= 3;

	// GPIO19(PIN55) S.VgridR.ZERO (ECAP1)
	GpioCtrlRegs.GPAMUX2.bit.GPIO19	= 3;

	// GPIO20(PIN78) S.DSP_RDY
	GpioCtrlRegs.GPAMUX2.bit.GPIO20	= 0;
	GpioDataRegs.GPACLEAR.bit.GPIO20= 1;
	GpioCtrlRegs.GPADIR.bit.GPIO20	= 1;
	// GPIO21(PIN79) MDSP_OK
	GpioCtrlRegs.GPAMUX2.bit.GPIO21	= 0;
	GpioCtrlRegs.GPADIR.bit.GPIO21	= 0;

	// GPIO28(PIN40) SCIRXDA
	GpioCtrlRegs.GPAMUX2.bit.GPIO28	= 1;
	// GPIO29(PIN34) SCITXDA
	GpioCtrlRegs.GPAMUX2.bit.GPIO29	= 1;

	// GPIO30(PIN33) SYNC AD with MCPU
	GpioCtrlRegs.GPAMUX2.bit.GPIO30	= 0;
	GpioCtrlRegs.GPADIR.bit.GPIO30	= 0;

	// GPIO32(PIN2) I2C-SDAA
	GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 1;
	// GPIO33(PIN3) I2C-SCLA
	GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 1;
	// GPIO34(PIN74) EEPROM write control
	GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0;
	GpioDataRegs.GPBSET.bit.GPIO34  = 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO34	= 1;

	// GPIO35(PIN59) JTAG-TDI
	// GPIO36(PIN60) JTAG-TMS
	// GPIO37(PIN58) JTAG-TDO
	// GPIO38(PIN57) JTAG-TCK

	// GPIO39(PIN56) TPC64
	GpioCtrlRegs.GPBMUX1.bit.GPIO39 = 0;
	GpioDataRegs.GPBCLEAR.bit.GPIO39= 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO39	= 1;
	// GPIO11(PIN61) TPC65
	GpioCtrlRegs.GPAMUX1.bit.GPIO11	= 0;
	GpioDataRegs.GPACLEAR.bit.GPIO11= 1;
	GpioCtrlRegs.GPADIR.bit.GPIO11  = 1;

	// GPIO12(PIN47) NC
	GpioCtrlRegs.GPAMUX1.bit.GPIO12	= 0;
	GpioCtrlRegs.GPADIR.bit.GPIO12  = 0;
	// GPIO41(PIN48) NC
	GpioCtrlRegs.GPBMUX1.bit.GPIO41 = 0;
	GpioCtrlRegs.GPBDIR.bit.GPIO41	= 0;


	// Each input can have different qualification
	// a) input synchronized to SYSCLKOUT
	// b) input qualified by a sampling window
	// c) input sent asynchronously (valid for peripheral inputs only)
	GpioCtrlRegs.GPAQSEL1.all = 0x0000;    // GPIO0 -GPIO15 Synch to SYSCLKOUT
	GpioCtrlRegs.GPAQSEL2.all = 0x0000;    // GPIO16-GPIO31 Synch to SYSCLKOUT
	GpioCtrlRegs.GPBQSEL1.all = 0x0000;    // GPIO32-GPIO44 Synch to SYSCLKOUT

	// SPI取消引脚GPIO输入判定
	GpioCtrlRegs.GPAQSEL1.bit.GPIO15	= 3;
	GpioCtrlRegs.GPAQSEL1.bit.GPIO13	= 3;
	GpioCtrlRegs.GPAQSEL1.bit.GPIO14	= 3;
	GpioCtrlRegs.GPAQSEL2.bit.GPIO24	= 3;

	// All pull-ups are disabled ...
	GpioCtrlRegs.GPAPUD.all = 0xFFFF;    // Pullup's disabled GPIO0-GPIO31
	GpioCtrlRegs.GPBPUD.all = 0xFFFF;    // Pullup's disabled GPIO32-GPIO44

	//... except SPI and I2C(slave only)
	GpioCtrlRegs.GPAPUD.bit.GPIO15	= 0;	// pin 75, /SPISTEB, 与主CPU通讯SPI使能
	GpioCtrlRegs.GPAPUD.bit.GPIO14	= 0;	// pin 77, SPICLKB,  与主CPU通讯SPI时钟
	GpioCtrlRegs.GPAPUD.bit.GPIO13	= 0;	// pin 76, SPISOMIB, 与主CPU通讯SPI发送
	GpioCtrlRegs.GPAPUD.bit.GPIO24	= 0;	// pin 80, SPISIMOB, 与主CPU通讯SPI接收
	GpioCtrlRegs.GPBPUD.bit.GPIO32	= 0;	// pin 2,  SDAA, I2C data  line
	GpioCtrlRegs.GPBPUD.bit.GPIO33	= 0;	// pin 3,  SCLA, I2C clock line
	EDIS;
/*	
		// Enable Xint1 and XINT2 in the PIE: Group 1 interrupt 4 & 5
	// Enable int1 which is connected to WAKEINT:
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          // Enable the PIE block
	PieCtrlRegs.PIEIER1.bit.INTx5 = 1;          // Enable PIE Gropu 1 INT5
	// GPIO0 is XINT1, GPIO1 is XINT2
	EALLOW;
	GpioIntRegs.GPIOXINT2SEL.bit.GPIOSEL = 30;   // XINT2 is GPIO30
	EDIS;

	// Configure XINT1
	XIntruptRegs.XINT2CR.bit.POLARITY = 2;      // Rising edge interrupt

	// Enable XINT1 and XINT2
	XIntruptRegs.XINT2CR.bit.ENABLE = 1;        // Enable XINT2  
	
//	PieCtrlRegs.PIEIER1.bit.INTx5 = 1;
	IER |= M_INT1;                              // Enable CPU int1
//	EINT;    */                                   // Enable Global Interrupts
             				
}

//===========================================================================
// End of file.
//===========================================================================
