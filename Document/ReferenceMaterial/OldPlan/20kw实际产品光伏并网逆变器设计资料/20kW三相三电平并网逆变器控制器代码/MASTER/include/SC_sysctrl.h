/*=============================================================================*
 *
 *  PRODUCT  : Main include file
 *
 *  FILENAME : SC_sysctrl.h 
 *  PURPOSE  : main include file involves constants and other including
 *			   files besides standard DSP2833x header files.  
 *
 *  HISTORY  :
 *    DATE            VERSION        AUTHOR            NOTE
 *
 *----------------------------------------------------------------------------
 *  GLOBAL VARIABLES
 *    NAME                                    DESCRIPTION
 *
 *      
 *----------------------------------------------------------------------------
 *  GLOBAL FUNCTIONS
 *    NAME                                    DESCRIPTION
 *      
 *    
 *============================================================================*/
#ifndef SC_SYSCTRL_H
#define SC_SYSCTRL_H

/* Define Macros for system level clock parameters */
#define SYSCLOCK_FREQ		(150000000)
#define HSPCLK_FREQ			(SYSCLOCK_FREQ/2)		// defined in SysCtrlRegs
#define LSPCLK_FREQ			(SYSCLOCK_FREQ/4)		// defined in SysCtrlRegs
#define TBCLK_FREQ			(SYSCLOCK_FREQ/2)		// EPwm Timer clock

extern Uint16 hwi_vec_loadstart;
extern Uint16 hwi_vec_loadend;
extern Uint16 hwi_vec_runstart;

extern Uint16 trcdata_loadstart;
extern Uint16 trcdata_loadend;
extern Uint16 trcdata_runstart;

extern Uint16 g_u16InitFlash_InRAM_loadstart;
extern Uint16 g_u16InitFlash_InRAM_loadend;
extern Uint16 g_u16InitFlash_InRAM_runstart;

extern Uint16 g_u16ControlLoopInRAM_loadstart;
extern Uint16 g_u16ControlLoopInRAM_loadend;
extern Uint16 g_u16ControlLoopInRAM_runstart;

#endif
//===========================================================================
// No more.
//===========================================================================

