
SECTIONS
{
    codestart           : > BEGIN_FLASH		PAGE = 0
	ramfuncs			: LOAD = FLASH_MONITOR,
                          RUN = L123SARAM,
                          LOAD_START(_RamfuncsLoadStart),
                          LOAD_SIZE(_RamfuncsLoadSize),
                          RUN_START(_RamfuncsRunStart),
                          PAGE = 0
                          
	csmpasswds          : > PASSWORDS  PAGE = 0
	csm_rsvd            : > CSM_RSVD    PAGE = 0

	ControlLoopInRAM	: LOAD = FLASH_APP,
						  RUN = L123SARAM,
						  LOAD_START(_ControlLoopInRAMLoadStart),
                          LOAD_SIZE(_ControlLoopInRAMLoadSize),
                          RUN_START(_ControlLoopInRAMRunStart),
                          PAGE = 0

   .start : LOAD = 0x003F4000, PAGE = 0 { bios.a28L <boot.o28L> (.sysinit) }
}