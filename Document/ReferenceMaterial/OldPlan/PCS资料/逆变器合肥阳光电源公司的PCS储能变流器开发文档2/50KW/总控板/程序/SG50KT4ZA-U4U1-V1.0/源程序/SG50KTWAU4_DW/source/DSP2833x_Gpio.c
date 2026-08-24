// TI File $Revision: /main/1 $
// Checkin $Date: August 18, 2006   13:46:25 $
//###########################################################################
//
// FILE:	DSP2833x_Gpio.c
//
// TITLE:	DSP2833x General Purpose I/O Initialization & Support Functions.
//
//###########################################################################
// $TI Release: DSP2833x Header Files V1.01 $
// $Release Date: September 26, 2007 $
//###########################################################################

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

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
  /* GpioCtrlRegs.GPAMUX1.all = 0x0000;     // GPIO functionality GPIO0-GPIO15
   GpioCtrlRegs.GPAMUX2.all = 0x0000;     // GPIO functionality GPIO16-GPIO31
   GpioCtrlRegs.GPBMUX1.all = 0x0000;     // GPIO functionality GPIO32-GPIO39
   GpioCtrlRegs.GPBMUX2.all = 0x0000;     // GPIO functionality GPIO48-GPIO63
   GpioCtrlRegs.GPCMUX1.all = 0x0000;     // GPIO functionality GPIO64-GPIO79
   GpioCtrlRegs.GPCMUX2.all = 0x0000;     // GPIO functionality GPIO80-GPIO95

   GpioCtrlRegs.GPADIR.all = 0x0000;      // GPIO0-GPIO31 are inputs
   GpioCtrlRegs.GPBDIR.all = 0x0000;      // GPIO32-GPIO63 are inputs   
   GpioCtrlRegs.GPCDIR.all = 0x0000;      // GPI064-GPIO95 are inputs

   // Each input can have different qualification
   // a) input synchronized to SYSCLKOUT
   // b) input qualified by a sampling window
   // c) input sent asynchronously (valid for peripheral inputs only)
   GpioCtrlRegs.GPAQSEL1.all = 0x0000;    // GPIO0-GPIO15 Synch to SYSCLKOUT 
   GpioCtrlRegs.GPAQSEL2.all = 0x0000;    // GPIO16-GPIO31 Synch to SYSCLKOUT
   GpioCtrlRegs.GPBQSEL1.all = 0x0000;    // GPIO32-GPIO39 Synch to SYSCLKOUT 
   GpioCtrlRegs.GPBQSEL2.all = 0x0000;    // GPIO48-GPIO63 Synch to SYSCLKOUT 

   // Pull-ups can be enabled or disabled. 
   GpioCtrlRegs.GPAPUD.all = 0x0000;      // Pullup's enabled GPIO0-GPIO31
   GpioCtrlRegs.GPBPUD.all = 0xffff;//0x0000;      // Pullup's enabled GPIO32-GPIO63
   GpioCtrlRegs.GPCPUD.all = 0x0000;      // Pullup's enabled GPIO64-GPIO79*/

   //GpioCtrlRegs.GPAPUD.all = 0xFFFF;    // Pullup's disabled GPIO0-GPIO31
   //GpioCtrlRegs.GPBPUD.all = 0xFFFF;    // Pullup's disabled GPIO32-GPIO34
   //GpioCtrlRegs.GPCPUD.all = 0xFFFF     // Pullup's disabled GPIO64-GPIO79
   /*//主接触器反馈输入
	GpioCtrlRegs.GPAPUD.bit.GPIO24=0;
	GpioCtrlRegs.GPAMUX2.bit.GPIO24=0;
	GpioCtrlRegs.GPADIR.bit.GPIO24=0;*/

//主接触器反馈输入
	GpioCtrlRegs.GPBPUD.bit.GPIO44=0;
	GpioCtrlRegs.GPBMUX1.bit.GPIO44=0;
	GpioCtrlRegs.GPBDIR.bit.GPIO44=0;
//KIN1输入
	GpioCtrlRegs.GPBPUD.bit.GPIO46=0;
	GpioCtrlRegs.GPBMUX1.bit.GPIO46=0;
	GpioCtrlRegs.GPBDIR.bit.GPIO46=0;
//LED1输出
	GpioCtrlRegs.GPCPUD.bit.GPIO76=0;
	GpioCtrlRegs.GPCMUX1.bit.GPIO76=0;
	GpioDataRegs.GPCSET.bit.GPIO76=1;
	GpioCtrlRegs.GPCDIR.bit.GPIO76=1;
//LED2输出
	GpioCtrlRegs.GPCPUD.bit.GPIO77=0;
	GpioCtrlRegs.GPCMUX1.bit.GPIO77=0;
	GpioDataRegs.GPCSET.bit.GPIO77=1;
	GpioCtrlRegs.GPCDIR.bit.GPIO77=1;	
//运行灯输出
	GpioCtrlRegs.GPCMUX2.bit.GPIO87=0;
	GpioCtrlRegs.GPCDIR.bit.GPIO87=1;
//预充电输出
	GpioCtrlRegs.GPCMUX2.bit.GPIO84=0;
	GpioCtrlRegs.GPCDIR.bit.GPIO84=1;
	GpioCtrlRegs.GPCPUD.bit.GPIO84=1;
	GpioDataRegs.GPCSET.bit.GPIO84=1;
//驱动上电继电器输出
	GpioCtrlRegs.GPCMUX2.bit.GPIO80=0;
	GpioCtrlRegs.GPCDIR.bit.GPIO80=1;
	GpioDataRegs.GPCCLEAR.bit.GPIO80=1;
//故障灯输出
	GpioCtrlRegs.GPCMUX2.bit.GPIO85=0;
	GpioCtrlRegs.GPCDIR.bit.GPIO85=1;
	GpioCtrlRegs.GPCPUD.bit.GPIO85=1;     // Pullup's disabled GPIO64-GPIO79
	GpioDataRegs.GPCCLEAR.bit.GPIO85=1;
//直流接触器断开输出
	GpioCtrlRegs.GPCMUX2.bit.GPIO82=0;
	GpioCtrlRegs.GPCDIR.bit.GPIO82=1;
	GpioCtrlRegs.GPCPUD.bit.GPIO82=1;
	GpioDataRegs.GPCCLEAR.bit.GPIO82=1;
//并网接触器输出
	GpioCtrlRegs.GPCMUX2.bit.GPIO83=0;
	GpioCtrlRegs.GPCDIR.bit.GPIO83=1;
	GpioCtrlRegs.GPCPUD.bit.GPIO83=1;     // Pullup's disabled GPIO64-GPIO79
	GpioDataRegs.GPCSET.bit.GPIO83=1;

	GpioCtrlRegs.GPAMUX1.bit.GPIO6=0;
	GpioCtrlRegs.GPADIR.bit.GPIO6=1;
	GpioCtrlRegs.GPAPUD.bit.GPIO6=1;
	GpioDataRegs.GPACLEAR.bit.GPIO6=1;
//输出
/*	GpioCtrlRegs.GPAMUX1.bit.GPIO0=0;
	GpioCtrlRegs.GPADIR.bit.GPIO0=1;
		GpioCtrlRegs.GPAMUX1.bit.GPIO1=0;
	GpioCtrlRegs.GPADIR.bit.GPIO1=1;
		GpioCtrlRegs.GPAMUX1.bit.GPIO2=0;
	GpioCtrlRegs.GPADIR.bit.GPIO2=1;
		GpioCtrlRegs.GPAMUX1.bit.GPIO3=0;
	GpioCtrlRegs.GPADIR.bit.GPIO3=1;
		GpioCtrlRegs.GPAMUX1.bit.GPIO4=0;
	GpioCtrlRegs.GPADIR.bit.GPIO4=1;
		GpioCtrlRegs.GPAMUX1.bit.GPIO5=0;
	GpioCtrlRegs.GPADIR.bit.GPIO5=1;*/
   EDIS;

}	
	
//===========================================================================
// End of file.
//===========================================================================
