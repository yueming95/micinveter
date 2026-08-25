// TI File $Revision: /main/6 $
// Checkin $Date: May 7, 2007   16:26:05 $
//###########################################################################
//
// FILE:	DSP2833x_ECan.c
//
// TITLE:	DSP2833x Enhanced CAN Initialization & Support Functions.
//
//###########################################################################
// $TI Release: DSP2833x Header Files V1.01 $
// $Release Date: September 26, 2007 $
//###########################################################################

#include "DSP2833x_Device.h"     // DSP28 Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP28 Examples Include File


//---------------------------------------------------------------------------
// InitECan:
//---------------------------------------------------------------------------
// This function initializes the eCAN module to a known state.
//
void InitECan(void)
{
   EALLOW;
  GpioCtrlRegs.GPAPUD.bit.GPIO12 = 0;   // Enable pull-up for GPIO12 (CANTXB)
  GpioCtrlRegs.GPAPUD.bit.GPIO13 = 0;   // Enable pull-up for GPIO13 (CANRXB)
  GpioCtrlRegs.GPAQSEL1.bit.GPIO13 = 3; // Asynch qual for GPIO13 (CANRXB)
  GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 2;  // Configure GPIO12 for CANTXB operation
  GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 2;  // Configure GPIO13 for CANRXB operation
   EDIS;

 InitECana();
 InitECanb();
}




void InitECana(void)		// Initialize eCAN-A module
{
/* Create a shadow register structure for the CAN control registers. This is
 needed, since only 32-bit access is allowed to these registers. 16-bit access
 to these registers could potentially corrupt the register contents or return 
 false data. This is especially true while writing to/reading from a bit 
 (or group of bits) among bits 16 - 31 */

struct ECAN_REGS ECanaShadow;
struct MBOX      MboxShadow;
   EALLOW;		// EALLOW enables access to protected bits

/* Configure eCAN RX and TX pins for CAN operation using eCAN regs*/  

    ECanaShadow.CANTIOC.all = ECanaRegs.CANTIOC.all;
    ECanaShadow.CANTIOC.bit.TXFUNC = 1;
    ECanaRegs.CANTIOC.all = ECanaShadow.CANTIOC.all;

    ECanaShadow.CANRIOC.all = ECanaRegs.CANRIOC.all;
    ECanaShadow.CANRIOC.bit.RXFUNC = 1;
    ECanaRegs.CANRIOC.all = ECanaShadow.CANRIOC.all;

/* Configure eCAN for HECC mode - (reqd to access mailboxes 16 thru 31) */

	ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
	ECanaShadow.CANMC.bit.SCB = 1;
	ECanaShadow.CANMC.bit.DBO = 1;
	ECanaShadow.CANMC.bit.ABO = 1;					//CAN故障离线自动恢复
	ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;

/* Initialize all bits of 'Master Control Field' to zero */
// Some bits of MSGCTRL register come up in an unknown state. For proper operation,
// all bits (including reserved bits) of MSGCTRL must be initialized to zero

    ECanaMboxes.MBOX0.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX1.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX2.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX3.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX4.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX5.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX6.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX7.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX8.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX9.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX10.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX11.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX12.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX13.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX14.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX15.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX16.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX17.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX18.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX19.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX20.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX21.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX22.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX23.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX24.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX25.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX26.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX27.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX28.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX29.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX30.MSGCTRL.all = 0x00000000;
    ECanaMboxes.MBOX31.MSGCTRL.all = 0x00000000;

// TAn, RMPn, GIFn bits are all zero upon reset and are cleared again
//	as a matter of precaution.

	ECanaRegs.CANTA.all	= 0xFFFFFFFF;	/* Clear all TAn bits */

	ECanaRegs.CANRMP.all = 0xFFFFFFFF;	/* Clear all RMPn bits */

	ECanaRegs.CANGIF0.all = 0xFFFFFFFF;	/* Clear all interrupt flag bits */
	ECanaRegs.CANGIF1.all = 0xFFFFFFFF;


/* Configure bit timing parameters for eCANB*/

	ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
	ECanaShadow.CANMC.bit.CCR = 1 ;            // Set CCR = 1

    ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;

    ECanaShadow.CANES.all = ECanaRegs.CANES.all;

    do
	{
	    ECanaShadow.CANES.all = ECanaRegs.CANES.all;
	} while(ECanaShadow.CANES.bit.CCE != 1 ); 		// Wait for CCE bit to be  cleared..


    ECanaShadow.CANBTC.all = 0;

                          // CPU_FRQ_150MHz is defined in DSP2833x_Examples.h
	/* The following block for all 150 MHz SYSCLKOUT - default. Bit rate = 1 Mbps */
	ECanaShadow.CANBTC.bit.BRPREG = 49;//4;//49;//4;//75M/(5*15)=1M
	ECanaShadow.CANBTC.bit.TSEG2REG =2;//2;//2;//=2;
	ECanaShadow.CANBTC.bit.TSEG1REG = 7;//10;//7;//10;


    ECanaShadow.CANBTC.bit.SAM = 1;
    ECanaRegs.CANBTC.all = ECanaShadow.CANBTC.all;

    ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
	ECanaShadow.CANMC.bit.CCR = 0 ;            // Set CCR = 0
    ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;

    ECanaShadow.CANES.all = ECanaRegs.CANES.all;

    do
    {
        ECanaShadow.CANES.all = ECanaRegs.CANES.all;
    } while(ECanaShadow.CANES.bit.CCE != 0 ); 		// Wait for CCE bit to be  cleared..


/* Disable all Mailboxes  */
 	ECanaRegs.CANME.all = 0;		// Required before writing the MSGIDs
//MBOX1 send
	MboxShadow.MSGID.all = ECanaMboxes.MBOX1.MSGID.all;
	MboxShadow.MSGID.bit.IDE = 0;				//使用标准帧
	MboxShadow.MSGID.bit.STDMSGID =101;
	MboxShadow.MSGID.bit.AAM = 1;
	ECanaMboxes.MBOX1.MSGID.all = MboxShadow.MSGID.all;
	ECanaMboxes.MBOX1.MSGCTRL.bit.DLC=8;
//MBOX2 Receive
	MboxShadow.MSGID.all = ECanaMboxes.MBOX2.MSGID.all;
	MboxShadow.MSGID.bit.IDE = 0;				//使用标准帧
	MboxShadow.MSGID.bit.STDMSGID =102;
	MboxShadow.MSGID.bit.AME = 0;
	MboxShadow.MSGID.bit.AAM = 0;
	ECanaMboxes.MBOX2.MSGID.all = MboxShadow.MSGID.all;
	ECanaMboxes.MBOX2.MSGCTRL.bit.DLC=8;
//MBOX3 receive
	MboxShadow.MSGID.all = ECanaMboxes.MBOX3.MSGID.all;
	MboxShadow.MSGID.bit.IDE = 0;				//使用标准帧
	MboxShadow.MSGID.bit.STDMSGID =103;
	MboxShadow.MSGID.bit.AAM = 0;
	MboxShadow.MSGID.bit.AME = 0;
	ECanaMboxes.MBOX3.MSGID.all = MboxShadow.MSGID.all;
	ECanaMboxes.MBOX3.MSGCTRL.bit.DLC=8;
//MBOX4 receive
	MboxShadow.MSGID.all = ECanaMboxes.MBOX4.MSGID.all;
	MboxShadow.MSGID.bit.IDE = 0;				//使用标准帧
	MboxShadow.MSGID.bit.STDMSGID =104;
	MboxShadow.MSGID.bit.AAM = 0;
	MboxShadow.MSGID.bit.AME = 0;
	ECanaMboxes.MBOX4.MSGID.all = MboxShadow.MSGID.all;
	ECanaMboxes.MBOX4.MSGCTRL.bit.DLC=8;
	ECanaShadow.CANMD.all=ECanaRegs.CANMD.all;
	ECanaShadow.CANMD.bit.MD1=0;   //Mbox1 send 
	ECanaShadow.CANMD.bit.MD3=1;   //Mbox3 receive 
	ECanaShadow.CANMD.bit.MD2=1;  //mbox2 receive
	ECanaShadow.CANMD.bit.MD4=1;  //mbox4 receive 
 
	ECanaRegs.CANMD.all=ECanaShadow.CANMD.all;

	ECanaShadow.CANME.all=ECanaRegs.CANME.all;
	ECanaShadow.CANME.bit.ME1=1;  //enable mbosx1
	ECanaShadow.CANME.bit.ME2=1;  //enable mbox2
	ECanaShadow.CANME.bit.ME3=1;  //enable mbox2
	ECanaShadow.CANME.bit.ME4=1;  //enable mbox2
	ECanaRegs.CANME.all=ECanaShadow.CANME.all;
//====================================================
    EDIS;
}


void InitECanb(void)		// Initialize eCAN-B module
{
/* Create a shadow register structure for the CAN control registers. This is
 needed, since only 32-bit access is allowed to these registers. 16-bit access
 to these registers could potentially corrupt the register contents or return 
 false data. This is especially true while writing to/reading from a bit 
 (or group of bits) among bits 16 - 31 */

struct ECAN_REGS ECanbShadow;
struct MBOX      MboxShadow;
   EALLOW;		// EALLOW enables access to protected bits

/* Configure eCAN RX and TX pins for CAN operation using eCAN regs*/  

    ECanbShadow.CANTIOC.all = ECanbRegs.CANTIOC.all;
    ECanbShadow.CANTIOC.bit.TXFUNC = 1;
    ECanbRegs.CANTIOC.all = ECanbShadow.CANTIOC.all;

    ECanbShadow.CANRIOC.all = ECanbRegs.CANRIOC.all;
    ECanbShadow.CANRIOC.bit.RXFUNC = 1;
    ECanbRegs.CANRIOC.all = ECanbShadow.CANRIOC.all;

/* Configure eCAN for HECC mode - (reqd to access mailboxes 16 thru 31) */

	ECanbShadow.CANMC.all = ECanbRegs.CANMC.all;
	ECanbShadow.CANMC.bit.SCB = 1;
	ECanbShadow.CANMC.bit.DBO = 1;
	ECanbShadow.CANMC.bit.ABO = 1;					//CAN故障离线自动恢复
	ECanbRegs.CANMC.all = ECanbShadow.CANMC.all;

/* Initialize all bits of 'Master Control Field' to zero */
// Some bits of MSGCTRL register come up in an unknown state. For proper operation,
// all bits (including reserved bits) of MSGCTRL must be initialized to zero

    ECanbMboxes.MBOX0.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX1.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX2.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX3.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX4.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX5.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX6.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX7.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX8.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX9.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX10.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX11.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX12.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX13.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX14.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX15.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX16.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX17.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX18.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX19.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX20.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX21.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX22.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX23.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX24.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX25.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX26.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX27.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX28.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX29.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX30.MSGCTRL.all = 0x00000000;
    ECanbMboxes.MBOX31.MSGCTRL.all = 0x00000000;

// TAn, RMPn, GIFn bits are all zero upon reset and are cleared again
//	as a matter of precaution.

	ECanbRegs.CANTA.all	= 0xFFFFFFFF;	/* Clear all TAn bits */

	ECanbRegs.CANRMP.all = 0xFFFFFFFF;	/* Clear all RMPn bits */

	ECanbRegs.CANGIF0.all = 0xFFFFFFFF;	/* Clear all interrupt flag bits */
	ECanbRegs.CANGIF1.all = 0xFFFFFFFF;


/* Configure bit timing parameters for eCANB*/

	ECanbShadow.CANMC.all = ECanbRegs.CANMC.all;
	ECanbShadow.CANMC.bit.CCR = 1 ;            // Set CCR = 1
    ECanbRegs.CANMC.all = ECanbShadow.CANMC.all;

    ECanbShadow.CANES.all = ECanbRegs.CANES.all;

    do
	{
	    ECanbShadow.CANES.all = ECanbRegs.CANES.all;
	} while(ECanbShadow.CANES.bit.CCE != 1 ); 		// Wait for CCE bit to be  cleared..


    ECanbShadow.CANBTC.all = 0;

    // CPU_FRQ_150MHz is defined in DSP2833x_Examples.h
	/* The following block for all 150 MHz SYSCLKOUT - default. Bit rate = 1 Mbps */
	ECanbShadow.CANBTC.bit.BRPREG = 4;//49;//49;//  1M75M/(50*12)=125kbps
	ECanbShadow.CANBTC.bit.TSEG2REG = 2;//2;//2;
	ECanbShadow.CANBTC.bit.TSEG1REG = 10;//7;//7;

    ECanbShadow.CANBTC.bit.SAM = 1;
    ECanbRegs.CANBTC.all = ECanbShadow.CANBTC.all;

    ECanbShadow.CANMC.all = ECanbRegs.CANMC.all;
	ECanbShadow.CANMC.bit.CCR = 0 ;            // Set CCR = 0
    ECanbRegs.CANMC.all = ECanbShadow.CANMC.all;

    ECanbShadow.CANES.all = ECanbRegs.CANES.all;

    do
    {
        ECanbShadow.CANES.all = ECanbRegs.CANES.all;
    } while(ECanbShadow.CANES.bit.CCE != 0 ); 		// Wait for CCE bit to be  cleared..


/* Disable all Mailboxes  */
 	ECanbRegs.CANME.all = 0;		// Required before writing the MSGIDs

    EDIS;
//============邮箱配置================================
	ECanbRegs.CANME.all = 0; //禁止所有邮箱

//MBOX1 send
	MboxShadow.MSGID.all = ECanbMboxes.MBOX5.MSGID.all;
	MboxShadow.MSGID.bit.IDE = 0;				//使用标准帧
	MboxShadow.MSGID.bit.STDMSGID =109;
	MboxShadow.MSGID.bit.AAM = 1;
	ECanbMboxes.MBOX5.MSGID.all = MboxShadow.MSGID.all;
	ECanbMboxes.MBOX5.MSGCTRL.bit.DLC=8;
/*/MBOX2 Receive
	MboxShadow.MSGID.all = ECanbMboxes.MBOX2.MSGID.all;
	MboxShadow.MSGID.bit.IDE = 0;				//使用标准帧
	MboxShadow.MSGID.bit.STDMSGID =102;
	MboxShadow.MSGID.bit.AME = 0;
	ECanbMboxes.MBOX2.MSGID.all = MboxShadow.MSGID.all;
	ECanbMboxes.MBOX2.MSGCTRL.bit.DLC=8;
//MBOX3 receive
	MboxShadow.MSGID.all = ECanbMboxes.MBOX3.MSGID.all;
	MboxShadow.MSGID.bit.IDE = 0;				//使用标准帧
	MboxShadow.MSGID.bit.STDMSGID =103;
	MboxShadow.MSGID.bit.AAM = 0;
	ECanbMboxes.MBOX3.MSGID.all = MboxShadow.MSGID.all;
	ECanbMboxes.MBOX3.MSGCTRL.bit.DLC=8;
//MBOX4 receive
	MboxShadow.MSGID.all = ECanbMboxes.MBOX4.MSGID.all;
	MboxShadow.MSGID.bit.IDE = 0;				//使用标准帧
	MboxShadow.MSGID.bit.STDMSGID =104;
	MboxShadow.MSGID.bit.AAM = 0;
	ECanbMboxes.MBOX4.MSGID.all = MboxShadow.MSGID.all;
	ECanbMboxes.MBOX4.MSGCTRL.bit.DLC=8;*/
	ECanbShadow.CANMD.all=ECanbRegs.CANMD.all;
	ECanbShadow.CANMD.bit.MD5=0;   //Mbox5 send 
//	ECanbShadow.CANMD.bit.MD3=1;   //Mbox3 receive 
//	ECanbShadow.CANMD.bit.MD2=1;  //mbox2 receive
//	ECanbShadow.CANMD.bit.MD4=1;  //mbox4 receive 
 
	ECanbRegs.CANMD.all=ECanbShadow.CANMD.all;

	ECanbShadow.CANME.all=ECanbRegs.CANME.all;
	ECanbShadow.CANME.bit.ME5=1;  //enable mbosx5
//	ECanbShadow.CANME.bit.ME2=1;  //enable mbox2
//	ECanbShadow.CANME.bit.ME3=1;  //enable mbox2
//	ECanbShadow.CANME.bit.ME4=1;  //enable mbox2
	ECanbRegs.CANME.all=ECanbShadow.CANME.all;
//====================================================
}


//---------------------------------------------------------------------------
// Example: InitECanGpio:
//---------------------------------------------------------------------------
// This function initializes GPIO pins to function as eCAN pins
//
// Each GPIO pin can be configured as a GPIO pin or up to 3 different
// peripheral functional pins. By default all pins come up as GPIO
// inputs after reset.
//
// Caution:
// Only one GPIO pin should be enabled for CANTXA/B operation.
// Only one GPIO pin shoudl be enabled for CANRXA/B operation.
// Comment out other unwanted lines.


void InitECanGpio(void)
{
   InitECanaGpio();
   InitECanbGpio();
}

void InitECanaGpio(void)
{
   EALLOW;

/* Enable internal pull-up for the selected CAN pins */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

	GpioCtrlRegs.GPAPUD.bit.GPIO30 = 0;	    // Enable pull-up for GPIO30 (CANRXA)
//	GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;	    // Enable pull-up for GPIO18 (CANRXA)

	GpioCtrlRegs.GPAPUD.bit.GPIO31 = 0;	    // Enable pull-up for GPIO31 (CANTXA)
//	GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;	    // Enable pull-up for GPIO19 (CANTXA)

/* Set qualification for selected CAN pins to asynch only */
// Inputs are synchronized to SYSCLKOUT by default.
// This will select asynch (no qualification) for the selected pins.

    GpioCtrlRegs.GPAQSEL2.bit.GPIO30 = 3;   // Asynch qual for GPIO30 (CANRXA)
//  GpioCtrlRegs.GPAQSEL2.bit.GPIO18 = 3;   // Asynch qual for GPIO18 (CANRXA)


/* Configure eCAN-A pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be eCAN functional pins.

	GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 1;	// Configure GPIO30 for CANRXA operation
//  GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 3;	// Configure GPIO18 for CANRXA operation
	GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 1;	// Configure GPIO31 for CANTXA operation
//  GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 3;	// Configure GPIO19 for CANTXA operation

    EDIS;
}

void InitECanbGpio(void)
{
   EALLOW;

/* Enable internal pull-up for the selected CAN pins */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

//	GpioCtrlRegs.GPAPUD.bit.GPIO8 = 0;	  // Enable pull-up for GPIO8  (CANTXB)
  GpioCtrlRegs.GPAPUD.bit.GPIO12 = 0;   // Enable pull-up for GPIO12 (CANTXB)
  //  GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;   // Enable pull-up for GPIO16 (CANTXB)
//  GpioCtrlRegs.GPAPUD.bit.GPIO20 = 0;   // Enable pull-up for GPIO20 (CANTXB)

//	GpioCtrlRegs.GPAPUD.bit.GPIO10 = 0;	  // Enable pull-up for GPIO10 (CANRXB)
  GpioCtrlRegs.GPAPUD.bit.GPIO13 = 0;   // Enable pull-up for GPIO13 (CANRXB)
  //  GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;   // Enable pull-up for GPIO17 (CANRXB)
//  GpioCtrlRegs.GPAPUD.bit.GPIO21 = 0;   // Enable pull-up for GPIO21 (CANRXB)

/* Set qualification for selected CAN pins to asynch only */
// Inputs are synchronized to SYSCLKOUT by default.
// This will select asynch (no qualification) for the selected pins.
// Comment out other unwanted lines.

//  GpioCtrlRegs.GPAQSEL1.bit.GPIO10 = 3; // Asynch qual for GPIO10 (CANRXB)
  GpioCtrlRegs.GPAQSEL1.bit.GPIO13 = 3; // Asynch qual for GPIO13 (CANRXB)
  //  GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 3; // Asynch qual for GPIO17 (CANRXB)
//  GpioCtrlRegs.GPAQSEL2.bit.GPIO21 = 3; // Asynch qual for GPIO21 (CANRXB)

/* Configure eCAN-B pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be eCAN functional pins.

//	GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 2;   // Configure GPIO8 for CANTXB operation
  GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 2;  // Configure GPIO12 for CANTXB operation
   // GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 2;  // Configure GPIO16 for CANTXB operation
//  GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 3;  // Configure GPIO20 for CANTXB operation

//	GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 2;  // Configure GPIO10 for CANRXB operation
  GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 2;  // Configure GPIO13 for CANRXB operation
  //  GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 2;  // Configure GPIO17 for CANRXB operation
//  GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 3;  // Configure GPIO21 for CANRXB operation

    EDIS;
}

/******************************************************/
/* Bit configuration parameters for 150 MHz SYSCLKOUT */
/******************************************************/
/*

The table below shows how BRP(reg) field must be changed to achieve different bit
rates with a BT of 15, for a 80% SP:
---------------------------------------------------------------
BT = 15, TSEG1(reg) = 10, TSEG2(reg) = 2, Sampling Point = 80%
---------------------------------------------------------------
1   Mbps : BRP(reg)+1 = 10 		: CAN clock = 15 MHz
500 kbps : BRP(reg)+1 = 20 		: CAN clock = 7.5 MHz
250 kbps : BRP(reg)+1 = 40 		: CAN clock = 3.75 MHz
125 kbps : BRP(reg)+1 = 80 		: CAN clock = 1.875 MHz
100 kbps : BRP(reg)+1 = 100 	: CAN clock = 1.5 MHz
50  kbps : BRP(reg)+1 = 200 	: CAN clock = 0.75 MHz

The table below shows how to achieve different sampling points with a BT of 15:
----------------------------------------------------------------------
Achieving desired SP by changing TSEG1(reg) & TSEG2(reg) with BT = 15
----------------------------------------------------------------------

TSEG1(reg) = 10, TSEG2(reg) = 2, SP = 80%
TSEG1(reg) = 9, TSEG2(reg) = 3, SP = 73%
TSEG1(reg) = 8, TSEG2(reg) = 4, SP = 66%
TSEG1(reg) = 7, TSEG2(reg) = 5, SP = 60%

The table below shows how BRP(reg) field must be changed to achieve different bit
rates with a BT of 10, for a 80% SP:
--------------------------------------------------------------
BT = 10, TSEG1(reg) = 6, TSEG2(reg) = 1, Sampling Point = 80%
--------------------------------------------------------------
1   Mbps : BRP(reg)+1 = 15 	: CAN clock = 10 MHz
500 kbps : BRP(reg)+1 = 30 	: CAN clock = 5 MHz
250 kbps : BRP(reg)+1 = 60 	: CAN clock = 2.5 MHz
125 kbps : BRP(reg)+1 = 120	: CAN clock = 1.25 MHz
100 kbps : BRP(reg)+1 = 150 : CAN clock = 1 MHz
50  kbps : BRP(reg)+1 = 300 : CAN clock = 0.5 MHz

*/

/***************************************************/
/* Bit configuration parameters for 100 MHz SYSCLKOUT*/
/***************************************************/
/*

The table below shows how BRP(reg) field must be changed to achieve different bit
rates with a BT of 10, for a 80% SP:
---------------------------------------------------
BT = 10, TSEG1 = 6, TSEG2 = 1, Sampling Point = 80%
---------------------------------------------------
1   Mbps : BRP(reg)+1 = 10 	: CAN clock = 10 MHz
500 kbps : BRP(reg)+1 = 20 	: CAN clock = 5 MHz
250 kbps : BRP(reg)+1 = 40 	: CAN clock = 2.5 MHz
125 kbps : BRP(reg)+1 = 80 	: CAN clock = 1.25 MHz
100 kbps : BRP(reg)+1 = 100 : CAN clock = 1 MHz
50  kbps : BRP(reg)+1 = 200 : CAN clock = 0.5 MHz

The table below shows how BRP(reg) field must be changed to achieve different bit
rates with a BT of 20:
---------------------------------------------------
BT = 20
---------------------------------------------------
1   Mbps : BRP(reg)+1 = 5
500 kbps : BRP(reg)+1 = 10
250 kbps : BRP(reg)+1 = 20
125 kbps : BRP(reg)+1 = 40
100 kbps : BRP(reg)+1 = 50
50  kbps : BRP(reg)+1 = 100

The table below shows how to achieve different sampling points with a BT of 20:
------------------------------------------------------------
Achieving desired SP by changing TSEG1 & TSEG2 with BT = 20
------------------------------------------------------------

TSEG1(reg) = 15, TSEG2(reg) = 2, SP = 85%
TSEG1(reg) = 14, TSEG2(reg) = 3, SP = 80%
TSEG1(reg) = 13, TSEG2(reg) = 4, SP = 75%
TSEG1(reg) = 12, TSEG2(reg) = 5, SP = 70%
TSEG1(reg) = 11, TSEG2(reg) = 6, SP = 65%
TSEG1(reg) = 10, TSEG2(reg) = 7, SP = 60%

Note: BRP(reg), TSEG1(reg) & TSEG2(reg) indicate the actual value that is written
 into the bit fields of the CAN control registers. These values are increased by 1
 by the CAN module when these registers are accessed.

*/

//===========================================================================
// End of file.
//===========================================================================

