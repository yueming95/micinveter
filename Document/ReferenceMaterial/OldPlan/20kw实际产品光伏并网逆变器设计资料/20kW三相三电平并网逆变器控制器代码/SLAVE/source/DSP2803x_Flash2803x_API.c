//###########################################################################
//
// FILE:  Example_Flash28035_API.c	
//
// TITLE: F28035 Flash API Example
//
// NOTE:  This example runs from Flash.  First program the example
//        into flash.  The code will then copy the API's to RAM and 
//        modify the flash. 
//
//
//###########################################################################
// $TI Release: F2803x API Release V1.00 $
// $Release Date: Apr 17, 2009 $
//###########################################################################


/*---- Flash API include file -------------------------------------------------*/
#include "Flash2803x_API_Library.h"

/*---- example include file ---------------------------------------------------*/
#include "DSP2803x_Flash2803x_API.h"

/*---- Standard headers -------------------------------------------------------*/
#include <stdio.h>   
#include "SL_slave.h" 
#include "DSP2803x_GlobalPrototypes.h"                          

/*--- Callback function.  Function specified by defining Flash_CallbackPtr */
void MyCallbackFunction(void); 
Uint32 MyCallbackCounter; // Just increment a counter in the callback function
                              
/*--- Global variables used to interface to the flash routines */
FLASH_ST FlashStatus;

/*---------------------------------------------------------------------------
  Data/Program Buffer used for testing the flash API functions
---------------------------------------------------------------------------*/
#define  WORDS_IN_FLASH_BUFFER 0x100               // Programming data buffer, Words
Uint16  Buffer[WORDS_IN_FLASH_BUFFER];

/*---------------------------------------------------------------------------
  Sector address info
---------------------------------------------------------------------------*/
typedef struct {
     Uint16 *StartAddr;
     Uint16 *EndAddr;
} SECTOR;

#define OTP_START_ADDR  0x3D7800
#define OTP_END_ADDR    0x3D7BFF


#define FLASH_END_ADDR    0x3F7FFF

#if (FLASH_F28035 || FLASH_F28034)
#define FLASH_START_ADDR  0x3E8000
SECTOR Sector[8]= {
         (Uint16 *) 0x3E8000,(Uint16 *) 0x3E9FFF,
         (Uint16 *) 0x3EA000,(Uint16 *) 0x3EBFFF,
         (Uint16 *) 0x3EC000,(Uint16 *) 0x3EDFFF,
         (Uint16 *) 0x3EE000,(Uint16 *) 0x3EFFFF,
         (Uint16 *) 0x3F0000,(Uint16 *) 0x3F1FFF,
         (Uint16 *) 0x3F2000,(Uint16 *) 0x3F3FFF,
         (Uint16 *) 0x3F4000,(Uint16 *) 0x3F5FFF,
         (Uint16 *) 0x3F6000,(Uint16 *) 0x3F7FFF,
};

#elif (FLASH_F28033 || FLASH_F28032)
#define FLASH_START_ADDR  0x3F0000
SECTOR Sector[8] = {
         (Uint16 *) 0x3F0000,(Uint16 *) 0x3F0FFF,
         (Uint16 *) 0x3F1000,(Uint16 *) 0x3F1FFF,
         (Uint16 *) 0x3F2000,(Uint16 *) 0x3F2FFF,
         (Uint16 *) 0x3F3000,(Uint16 *) 0x3F3FFF,
         (Uint16 *) 0x3F4000,(Uint16 *) 0x3F4FFF,
         (Uint16 *) 0x3F5000,(Uint16 *) 0x3F5FFF,
         (Uint16 *) 0x3F6000,(Uint16 *) 0x3F6FFF,
         (Uint16 *) 0x3F7000,(Uint16 *) 0x3F7FFF,
};
#endif

#define Device_cal (void   (*)(void))0x3D7C80

// Dummy variables for header file structure.
// Without these declarations Flash_CPUScaleFactor and
// Flash_CallbackPtr will not end up in the correct place.

#pragma CODE_SECTION(Example_CallFlashAPI,"ramfuncs");
void REST_SYS(void);

void Example_CallFlashAPI(void)
{
//   Uint16  i;
   Uint16  Status;
 //  Uint16  *Flash_ptr;     // Pointer to a location in flash
 //  Uint32  Length;         // Number of 16-bit values to be programmed
 //  float32 Version;        // Version of the API in floating point
  // Uint16  VersionHex;     // Version of the API in decimal encoded hex

//	Buffer[0] = 0xFFFF;
//	Buffer[1] = 0xFFFF;
//	Status = Flash_Program((Uint16 * )0x3f5ffe,Buffer,2,&FlashStatus);
	Status =  Flash_Erase((SECTORB),&FlashStatus);
	if(Status != STATUS_SUCCESS) 
    {
        Example_Error(Status);
    }
	EnableDog();
	REST_SYS();
}



void Example_ToggleTest(Uint16 PinNumber)
{
       Uint32 mask;
     
       // Before calling the Toggle Test, we must setup
       // the MUX and DIR registers. 

       if(PinNumber > (Uint16)34)
       {
           asm("    ESTOP0");  // Stop here. Invalid option.
           for(;;);   
       }

       // Pins GPIO16-GPIO31
       else if(PinNumber >= 32) 
       {
           EALLOW;
           mask = ~( ((Uint32)1 << (PinNumber-16)*2) | ((Uint32)1 << (PinNumber-32)*2+1) );
           GpioCtrlRegs.GPBMUX1.all &= mask;
           GpioCtrlRegs.GPBDIR.all = GpioCtrlRegs.GPADIR.all | ((Uint32)1 << (PinNumber-32) );
           Flash_ToggleTest(&GpioDataRegs.GPBTOGGLE.all, ((Uint32)1 << PinNumber) );   
           EDIS;
       }
       
       // Pins GPIO16-GPIO31
       else if(PinNumber >= 16)
       {   
           EALLOW;
           mask = ~( ((Uint32)1 << (PinNumber-16)*2) | ((Uint32)1 << (PinNumber-16)*2+1) );
           GpioCtrlRegs.GPAMUX2.all &= mask;
           GpioCtrlRegs.GPADIR.all = GpioCtrlRegs.GPADIR.all | ((Uint32)1 << PinNumber);
           Flash_ToggleTest(&GpioDataRegs.GPATOGGLE.all, ((Uint32)1 << PinNumber) );   
           EDIS;
       }
       
       // Pins GPIO0-GPIO15
       else 
       {
           EALLOW;
           mask = ~( ((Uint32)1 << PinNumber*2) | ((Uint32)1 << PinNumber*2+1 ));
           GpioCtrlRegs.GPAMUX1.all &= mask;
           GpioCtrlRegs.GPADIR.all = GpioCtrlRegs.GPADIR.all | ((Uint32)1 << PinNumber);
           EDIS;
           Flash_ToggleTest(&GpioDataRegs.GPATOGGLE.all, ((Uint32)1 << PinNumber) );   
       }

}


/*------------------------------------------------------------------
  Simple memory copy routine to move code out of flash into SARAM
-----------------------------------------------------------------*/

void Example_MemCopy(Uint16* SourceAddr, Uint16* SourceEndAddr, Uint16* DestAddr)
{
    while(SourceAddr < SourceEndAddr)
    { 
       *DestAddr++ = *SourceAddr++;
    }
    return;
}


/*------------------------------------------------------------------
  For this example, if an error is found just stop here
-----------------------------------------------------------------*/
#pragma CODE_SECTION(Example_Error,"ramfuncs");
void Example_Error(Uint16 Status)
{

//  Error code will be in the AL register. 
    asm("    ESTOP0");
    asm("    SB 0, UNC");
}


/*------------------------------------------------------------------
  For this example, once we are done just stop here
-----------------------------------------------------------------*/
#pragma CODE_SECTION(Example_Done,"ramfuncs");
void Example_Done(void)
{

    asm("    ESTOP0");
    asm("    SB 0, UNC");
}       


/*------------------------------------------------------------------
  Callback function - must be executed from outside flash/OTP
-----------------------------------------------------------------*/
#pragma CODE_SECTION(MyCallbackFunction,"ramfuncs");
void MyCallbackFunction(void)
{       
    // Toggle pin, service external watchdog etc
    MyCallbackCounter++;
    asm("    NOP");
}



