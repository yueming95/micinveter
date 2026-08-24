// TI File $Revision: /main/2 $
// Checkin $Date: March 1, 2007   16:06:07 $
//###########################################################################
//
// FILE:	DSP2833x_Sci.c
//
// TITLE:	DSP2833x SCI Initialization & Support Functions.
//
//###########################################################################
// $TI Release: DSP2833x Header Files V1.01 $
// $Release Date: September 26, 2007 $
//###########################################################################

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

//---------------------------------------------------------------------------
// InitSci: 
//---------------------------------------------------------------------------
// This function initializes the SCI(s) to a known state.
//
void InitSci(void)
{
	// Initialize SCI-A:
	InitSciaGpio();
	//tbd...
 	EALLOW;

	//  Initialize SCI-A:
  	SciaRegs.SCICCR.all = 0x0007;    		  // 1 stop bit,  No loopback                                    			  // No parity,8 char bits                                  			  // async mode, idle-line protocol
  	SciaRegs.SCICTL1.all = 0x0003;   		  // enable TX, RX, internal SCICLK,                                    			  // Disable RX ERR, SLEEP, TXWAKE
  	SciaRegs.SCIHBAUD    = 243>>8;   		  //Baund:121 38400bps(LSPCLK = 37.5MHz)
  	SciaRegs.SCILBAUD    = 243&0x00FF; 
	SciaRegs.SCIFFTX.bit.TXFIFOXRESET=1; //重新使能发送FIFO的操作
	SciaRegs.SCIFFTX.bit.SCIFFENA=1;     //使能SCI FIFO的功能
	SciaRegs.SCIFFTX.bit.TXFFST=0;       //发送FIFO队列为空
	SciaRegs.SCIFFTX.bit.TXFFINT=0;      //没有产生发送FIFO中断
	SciaRegs.SCIFFTX.bit.TXFFINTCLR=0;     //没有清除TXFFINT的标志位
	SciaRegs.SCIFFTX.bit.TXFFIENA=0;     //使能发送FIFO中断
	SciaRegs.SCIFFTX.bit.TXFFIL=0;     //发送中断级别为0，也就是当发送FIFO为空时发生中断

	SciaRegs.SCIFFRX.bit.RXFFOVF=0;      //接收FIFO没有溢出
	SciaRegs.SCIFFRX.bit.RXFFOVRCLR=1;    //清除RXFFOVF标志位
	SciaRegs.SCIFFRX.bit.RXFIFORESET=1;  //重新使能接收FIFO的操作
	SciaRegs.SCIFFRX.bit.RXFFST=0;      //接收FIFO队列为空
	SciaRegs.SCIFFRX.bit.RXFFINT=0;      //没有产生接收中断
	SciaRegs.SCIFFRX.bit.RXFFINTCLR=1;   //清除接收中断标志位
	SciaRegs.SCIFFRX.bit.RXFFIENA=1;     //使能FIFO接收中断
	SciaRegs.SCIFFRX.bit.RXFFIL=1;       //FIFO接收中断级别为8.也就是说当接收FIFO中有8个字符时发生中断

	SciaRegs.SCICTL1.bit.SWRESET=1;        //重启SCI

 	EDIS; 	
	InitScibGpio();
	//tbd...
 	EALLOW;

	//  Initialize SCI-A:
	ScibRegs.SCICCR.all=0x0000;		      	//无奇偶检测,选择空闲线多处理器模式,
	ScibRegs.SCICCR.bit.SCICHAR=0x7;        //字符长度为8位.no parity,idle-line,one stopbit,8 charbits

	ScibRegs.SCICTL1.all=0x0000;	      	/*0x0003;  SW_RESET=0,soft reset;enable TX,RX;disable  RXERR,TXWAKE,SLEEP,  */
	ScibRegs.SCICTL1.bit.RXENA=1;	      	//SCI接收使能
	ScibRegs.SCICTL1.bit.TXENA=1;	      	//SCI发送使能

	ScibRegs.SCICTL2.all=0x0000;
	ScibRegs.SCICTL2.bit.RXBKINTENA=1;    	//使能接收缓冲器/间断中断 enable RX INT,disable TX INT




   ScibRegs.SCIHBAUD    = 487>>8;   	   //Baund:487-9600;243-19200;121-38400bps(LSPCLK = 37.5MHz)
   ScibRegs.SCILBAUD    = 487&0x00FF;

	ScibRegs.SCIPRI.all=0;

	ScibRegs.SCICTL1.bit.SWRESET=1;		    /* SW_RESET=1, relinquish SCI from reset*/
//  PFDATDIR &= 0xffdf;    /*IOPF5=RDEN =0,enable 485 RE */
 	EDIS;
	// Initialize SCI-B:

	//tbd...
  
      // Initialize SCI-C:

      //tbd...
}	

//---------------------------------------------------------------------------
// Example: InitSciGpio: 
//---------------------------------------------------------------------------
// This function initializes GPIO pins to function as SCI pins
//
// Each GPIO pin can be configured as a GPIO pin or up to 3 different
// peripheral functional pins. By default all pins come up as GPIO
// inputs after reset.  
// 
// Caution: 
// Only one GPIO pin should be enabled for SCITXDA/B operation.
// Only one GPIO pin shoudl be enabled for SCIRXDA/B operation. 
// Comment out other unwanted lines.

void InitSciGpio()
{
#if DSP28_SCIA   
   InitSciaGpio();
#endif // if DSP28_SCIA  
#if DSP28_SCIB   
   InitScibGpio();
#endif // if DSP28_SCIB  
#if DSP28_SCIC
   InitScicGpio();
#endif // if DSP28_SCIC
}

void InitSciaGpio()
{
   EALLOW;

/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled disabled by the user.  
// This will enable the pullups for the specified pins.

//	GpioCtrlRegs.GPAPUD.bit.GPIO28 = 0;    // Enable pull-up for GPIO28 (SCIRXDA)
//	GpioCtrlRegs.GPAPUD.bit.GPIO29 = 0;	   // Enable pull-up for GPIO29 (SCITXDA)
	GpioCtrlRegs.GPBPUD.bit.GPIO35 = 0;    // Enable pull-up for GPIO28 (SCIRXDA)
	GpioCtrlRegs.GPBPUD.bit.GPIO36 = 0;	   // Enable pull-up for GPIO29 (SCITXDA)

/* Set qualification for selected pins to asynch only */
// Inputs are synchronized to SYSCLKOUT by default.  
// This will select asynch (no qualification) for the selected pins.

//	GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 3;  // Asynch input GPIO28 (SCIRXDA)
	GpioCtrlRegs.GPBQSEL1.bit.GPIO36 = 3;

/* Configure SCI-A pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be SCI functional pins.

//	GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 1;   // Configure GPIO28 for SCIRXDA operation
//	GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 1;   // Configure GPIO29 for SCITXDA operation
	GpioCtrlRegs.GPBMUX1.bit.GPIO35 = 1;   // Configure GPIO28 for SCIRXDA operation
	GpioCtrlRegs.GPBMUX1.bit.GPIO36 = 1;
	
    EDIS;
}

#if DSP28_SCIB 
void InitScibGpio()
{
   EALLOW;
	
/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled disabled by the user.  
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

  GpioCtrlRegs.GPAPUD.bit.GPIO9 = 0;     // Enable pull-up for GPIO9  (SCITXDB)
//  GpioCtrlRegs.GPAPUD.bit.GPIO14 = 0;    // Enable pull-up for GPIO14 (SCITXDB)
//	GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;	   // Enable pull-up for GPIO18 (SCITXDB)
//  GpioCtrlRegs.GPAPUD.bit.GPIO22 = 0;    // Enable pull-up for GPIO22 (SCITXDB)

	
  GpioCtrlRegs.GPAPUD.bit.GPIO11 = 0;    // Enable pull-up for GPIO11 (SCIRXDB)
//  GpioCtrlRegs.GPAPUD.bit.GPIO15 = 0;    // Enable pull-up for GPIO15 (SCIRXDB)
//    GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;	   // Enable pull-up for GPIO19 (SCIRXDB)
//  GpioCtrlRegs.GPAPUD.bit.GPIO23 = 0;    // Enable pull-up for GPIO23 (SCIRXDB)

/* Set qualification for selected pins to asynch only */
// This will select asynch (no qualification) for the selected pins.
// Comment out other unwanted lines.

 GpioCtrlRegs.GPAQSEL1.bit.GPIO11 = 3;  // Asynch input GPIO11 (SCIRXDB)
//  GpioCtrlRegs.GPAQSEL1.bit.GPIO15 = 3;  // Asynch input GPIO15 (SCIRXDB)
// 	GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3;  // Asynch input GPIO19 (SCIRXDB)
//  GpioCtrlRegs.GPAQSEL2.bit.GPIO23 = 3;  // Asynch input GPIO23 (SCIRXDB)

/* Configure SCI-B pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be SCI functional pins.
// Comment out other unwanted lines.

  GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 2;    // Configure GPIO9 for SCITXDB operation
//  GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 2;   // Configure GPIO14 for SCITXDB operation
//	GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 2;   // Configure GPIO18 for SCITXDB operation
//  GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 3;   // Configure GPIO22 for SCITXDB operation
	
  GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 2;   // Configure GPIO11 for SCIRXDB operation
//  GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 2;   // Configure GPIO15 for SCIRXDB operation
//    GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 2;   // Configure GPIO19 for SCIRXDB operation
//  GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 3;   // Configure GPIO23 for SCIRXDB operation
	
    EDIS;
}
#endif // if DSP28_SCIB 

#if DSP28_SCIC
void InitScicGpio()
{
   EALLOW;

/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled disabled by the user.  
// This will enable the pullups for the specified pins.

	GpioCtrlRegs.GPBPUD.bit.GPIO62 = 0;    // Enable pull-up for GPIO62 (SCIRXDC)
	GpioCtrlRegs.GPBPUD.bit.GPIO63 = 0;	   // Enable pull-up for GPIO63 (SCITXDC)

/* Set qualification for selected pins to asynch only */
// Inputs are synchronized to SYSCLKOUT by default.  
// This will select asynch (no qualification) for the selected pins.

	GpioCtrlRegs.GPBQSEL2.bit.GPIO62 = 3;  // Asynch input GPIO62 (SCIRXDC)

/* Configure SCI-C pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be SCI functional pins.

	GpioCtrlRegs.GPBMUX2.bit.GPIO62 = 1;   // Configure GPIO62 for SCIRXDC operation
	GpioCtrlRegs.GPBMUX2.bit.GPIO63 = 1;   // Configure GPIO63 for SCITXDC operation
	
    EDIS;
}
#endif // if DSP28_SCIC 

	
//===========================================================================
// End of file.
//===========================================================================
