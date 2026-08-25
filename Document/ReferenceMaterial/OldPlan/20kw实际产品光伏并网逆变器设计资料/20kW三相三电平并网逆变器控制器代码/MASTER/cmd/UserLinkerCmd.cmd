/*=============================================================================*
 *         Copyright(c) 2009-2010, Samil Power Co., Ltd.
 *                          ALL RIGHTS RESERVED
 *
 *  PRODUCT  : User defined CMD file
 *
 *  FILENAME : UserLinkerCmd.cmd 
 *  PURPOSE  : User defined linker file specially for code running in RAM.  
 *
 *  HISTORY  :
 *    DATE            VERSION        AUTHOR            NOTE
 *============================================================================*/

SECTIONS
{
	// For system entry configuration and security

	// Used by file CodeStartBranch.asm
	codestart         : > BEGIN_FLASH,           PAGE = 0           
	
	// Used by file passwords.asm
	csm_rsvd          : > CSM_RSVD,              PAGE = 0           
	
	// Used by file passwords.asm
	passwords         : > PASSWORDS,             PAGE = 0                       
	
	// For Flash configuration running in CSM RAM from Flash 
	InitFlash_InRAM	:	LOAD = FLASH_ABCDEFGH, PAGE = 0           
                        RUN = L0123SARAM,      PAGE = 0           	
                        LOAD_START(_g_u16InitFlash_InRAM_loadstart),
                        LOAD_END(_g_u16InitFlash_InRAM_loadend),
                        RUN_START(_g_u16InitFlash_InRAM_runstart)
	
	// For real-tinme control loop running in CSM RAM from Flash
	ControlLoopInRAM	:	LOAD = FLASH_ABCDEFGH, PAGE = 0           
                        	RUN = L0123SARAM,      PAGE = 0           
                       	 	LOAD_START(_g_u16ControlLoopInRAM_loadstart),
                        	LOAD_END(_g_u16ControlLoopInRAM_loadend),
                        	RUN_START(_g_u16ControlLoopInRAM_runstart)

}

SECTIONS
{
 
   /* Allocate program areas: */
   /* The Flash API functions can be grouped together as shown below.
      The defined symbols _Flash28_API_LoadStart, _Flash28_API_LoadEnd
      and _Flash28_API_RunStart are used to copy the API functions out
      of flash memory and into SARAM */

   Flash28_API:
   {
        -lFlash28335_API_V210.lib(.econst) 
        -lFlash28335_API_V210.lib(.text)
   }                   LOAD = FLASH_ABCDEFGH, 
                       RUN = L0123SARAM,  
                       LOAD_START(_Flash28_API_LoadStart),
                       LOAD_END(_Flash28_API_LoadEnd),
                       RUN_START(_Flash28_API_RunStart),
                       PAGE = 0
	ramfuncs     : LOAD = FLASH_ABCDEFGH, 
	              RUN = L0123SARAM, 
	              LOAD_START(_RamfuncsLoadStart),
	              LOAD_END(_RamfuncsLoadEnd),
	              RUN_START(_RamfuncsRunStart),
				  PAGE = 0
   .start : LOAD = 0x00330000, PAGE = 0 { bios.a28FP <boot.o28FP>
              (.sysinit) }
}

//--- end of file -----------------------------------------------------
