#include "SC_master.h" 
//#include "Example_Flash2823x_API.h"
#define PLLCR_VALUE  0x000A     // SYSCLKOUT = (OSCLK*10)/2

volatile Uint8 SciRxBuff[80];
volatile Uint8 SciReceivePonit=0;

Uint16 InitPLLForFlashAPI(void)
{
	if (SysCtrlRegs.PLLSTS.bit.MCLKSTS != 1)
   {
       if (SysCtrlRegs.PLLCR.bit.DIV != PLLCR_VALUE)
       {
   
          
          EALLOW;
          // Before setting PLLCR turn off missing clock detect
          SysCtrlRegs.PLLSTS.bit.MCLKOFF = 1;
          SysCtrlRegs.PLLCR.bit.DIV = PLLCR_VALUE;
          EDIS;
   
          // Wait for PLL to lock.
          // During this time the CPU will switch to OSCCLK/2 until
          // the PLL is stable.  Once the PLL is stable the CPU will 
          // switch to the new PLL value. 
          //
          // This time-to-lock is monitored by a PLL lock counter.   
          //   
          // The watchdog should be disabled before this loop, or fed within 
          // the loop.   
   
          EALLOW;
          SysCtrlRegs.WDCR= 0x0068;
          EDIS;
   
          // Wait for the PLL lock bit to be set.  
          // Note this bit is not available on 281x devices.  For those devices
          // use a software loop to perform the required count. 
   
          while(SysCtrlRegs.PLLSTS.bit.PLLLOCKS != 1) { }
          
          EALLOW;
          SysCtrlRegs.PLLSTS.bit.MCLKOFF = 0;
          EDIS;
       }
	   return STATUS_SUCCESS;
   }
   
   // If the PLL is in limp mode, shut the system down
   else 
   {
      // Replace this line with a call to an appropriate
      // SystemShutdown(); function. 
      return  WRONG_PLL_MODE;
   }
}

void Scib_Communication(void)
{

//    Uint16 Buffer[2];
    
    if(0x90==SciRxBuff[3] && 'F'==SciRxBuff[4]
      && 'D'==SciRxBuff[5] && 'S'==SciRxBuff[6]
      && 'P'==SciRxBuff[7] && 'M'==SciRxBuff[8])
	{
       //asm("	SETC	INTM");
//		PWMOFFandRelayDisconnect();
		g_StateCheck.bit.FlashFlag = 1;
	}
}


