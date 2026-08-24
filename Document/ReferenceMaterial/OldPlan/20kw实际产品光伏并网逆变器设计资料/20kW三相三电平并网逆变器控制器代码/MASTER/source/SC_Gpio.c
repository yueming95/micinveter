/***********************************************************************
 *    FILENAME : SC_Gpio.c
 *
 *    PURPOSE  : 
 *    HISTORY  :
 *           DATE          VERSION      AUTHOR        NOTE
 ************************************************************************/
#include "SC_master.h"

/**********************************************************************
* FUNCION :  InitGpio
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void InitGpio(void)
{
   asm(" EALLOW");					// Enable EALLOW protected register access

	/* Group A pins */

	// QUALPRD = SYSCLKOUT for all group A GPIO
	GpioCtrlRegs.GPACTRL.all  = 0x00000000;	
		
	// No qualification for all group A GPIO 0-15
	GpioCtrlRegs.GPAQSEL1.all = 0x00000000;		
	// No qualification for all group A GPIO 16-31
	GpioCtrlRegs.GPAQSEL2.all = 0x00000000;		
	// All group A GPIO are inputs
	GpioCtrlRegs.GPADIR.all   = 0x00000000;		
	// All Pullups disabled due to external pull down
	GpioCtrlRegs.GPAPUD.all   = 0xFFFFFFFF;		

	// GPIO 0-11(PIN5-7,10-13,16-20) are used as ePWM port
	GpioCtrlRegs.GPAMUX1.bit.GPIO0  = 1;		// 0=GPIO  1=EPWM1A     2=rsvd       
												// 3=rsvd
	GpioCtrlRegs.GPAMUX1.bit.GPIO1  = 1;		// 0=GPIO  1=EPWM1B     2=ECAP6      
												// 3=rsvd
	GpioCtrlRegs.GPAMUX1.bit.GPIO2  = 1;		// 0=GPIO  1=EPWM2A     2=rsvd       
												// 3=rsvd
	GpioCtrlRegs.GPAMUX1.bit.GPIO3  = 1;		// 0=GPIO  1=EPWM2B     2=ECAP5      
												// 3=rsvd
	GpioCtrlRegs.GPAMUX1.bit.GPIO4  = 1;		// 0=GPIO  1=EPWM3A     2=rsvd       
												// 3=rsvd
	GpioCtrlRegs.GPAMUX1.bit.GPIO5  = 1;		// 0=GPIO  1=EPWM3B     2=MFSRA      
												// 3=ECAP1
	GpioCtrlRegs.GPAMUX1.bit.GPIO6  = 1;		// 0=GPIO  1=EPWM4A     2=EPWMSYNCI  
												// 3=EPWMSYNCO
	GpioCtrlRegs.GPAMUX1.bit.GPIO7  = 1;		// 0=GPIO  1=EPWM4B     2=MCLKRA     
												// 3=ECAP2
	GpioCtrlRegs.GPAMUX1.bit.GPIO8  = 1;		// 0=GPIO  1=EPWM5A     2=CANTXB     
												// 3=ADCSOCAO
	GpioCtrlRegs.GPAMUX1.bit.GPIO9  = 1;		// 0=GPIO  1=EPWM5B     2=SCITXDB    
												// 3=ECAP3
	GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 1;		// 0=GPIO  1=EPWM6A     2=CANRXB     
												// 3=ADCSOCBO
	GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 1;		// 0=GPIO  1=EPWM6B     2=SCIRXDB    


	// GPIO12(PIN21) input pin for AC Over Current Protection
	GpioCtrlRegs.GPAMUX1.bit.GPIO12  = 0;		// 0=GPIO  1=TZ1        2=CANTXB	3=SPISIMOB
	GpioCtrlRegs.GPADIR.bit.GPIO12   = 0;
	// GPIO13(PIN24) input pin for boost1 Over Current Protection
	GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 0;		// 0=GPIO  1=TZ2        2=CANRXB    3=SPISOMIB
	GpioCtrlRegs.GPADIR.bit.GPIO13  = 0;
	// GPIO14(PIN25) input pin for boost2 Over Current Protection
	GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 0;		// 0=GPIO  1=TZ3        2=SCITXDB   3=SPICLKB
	GpioCtrlRegs.GPADIR.bit.GPIO14  = 0;
	// GPIO15(PIN26) input pin for bus Over voltage Protection
	GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 0;		// 0=GPIO  1=TZ4        2=SCIRXDB   3=SPISTEB
	GpioCtrlRegs.GPADIR.bit.GPIO15  = 0;

	// GPIO16-21(PIN27,28,62-65) unused I/O, set input pin.
	GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 0;		// 0=GPIO  1=SPISIMOA   2=CANTXB   3=TZ5
	GpioDataRegs.GPACLEAR.bit.GPIO16= 1;		//
	GpioCtrlRegs.GPADIR.bit.GPIO16  = 1;			
	GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 0;		// 0=GPIO  1=SPISOMIA   2=CANRXB   3=TZ6  
	GpioDataRegs.GPACLEAR.bit.GPIO17= 1;
	GpioCtrlRegs.GPADIR.bit.GPIO17  = 1;			

	GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 0;		// 0=GPIO  1=SPICLKA    2=SCITXDB    3=CANRXA
	GpioCtrlRegs.GPADIR.bit.GPIO18  = 0;	
	GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 0;		// 0=GPIO  1=SPISTEA    2=SCIRXDB    3=CANTXA
	GpioCtrlRegs.GPADIR.bit.GPIO19  = 0;	
	GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 0;		// 0=GPIO  1=EQEP1A     2=MDXA       3=CANTXB
	GpioCtrlRegs.GPADIR.bit.GPIO20  = 0;	
	GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 0;		// 0=GPIO  1=EQEP1B     2=MDRA       3=CANRXB
	GpioCtrlRegs.GPADIR.bit.GPIO21  = 0;	
	
	// GPIO22-23(PIN66,67) used as SCI communicate port with LCD board
	GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 3;		// 0=GPIO  1=EQEP1S     2=MCLKXA     
												// 3=SCITXDB
	GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 3;		// 0=GPIO  1=EQEP1I     2=MFSXA      
												// 3=SCIRXDB
	// GPIO24-27(PIN68,69,72,73) used as eCAP, for Boost APWM
	GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 1;		// 0=GPIO  1=ECAP1      2=EQEP2A     
												// 3=MDXB
	GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 1;		// 0=GPIO  1=ECAP2      2=EQEP2B     
												// 3=MDRB
	GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 1;		// 0=GPIO  1=ECAP3      2=EQEP2I     
												// 3=MCLKXB
	GpioCtrlRegs.GPAMUX2.bit.GPIO27 = 1;		// 0=GPIO  1=ECAP4      2=EQEP2S     
												// 3=MFSXB
	// GPIO28-29(PIN141,2) used as Update SCI port											  
    GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 1;		// 0=GPIO  1=SCIRXDA    2=XZCS6      3=XZCS6
    GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 1;		// 0=GPIO  1=SCITXDA    2=XA19       3=XA19	

	// GPIO30(PIN1) unused I/O, set input pin.
	GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 0;		// 0=GPIO  1=CANRXA     2=XA18       3=XA18
	GpioCtrlRegs.GPADIR.bit.GPIO30  = 0;

	// GPIO31(PIN176) Output pin for M.SEL_B
	GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 0;		// 0=GPIO  1=CANTXA     2=XA17       3=XA17
	GpioDataRegs.GPACLEAR.bit.GPIO31= 1;		// output 0 by default	
	GpioCtrlRegs.GPADIR.bit.GPIO31  = 1;		// set to output

	/* Group B pins */
	
	// QUALPRD = SYSCLKOUT for all group B GPIO
	GpioCtrlRegs.GPBCTRL.all  = 0x00000000;		
	// No qualification for all group B GPIO 32-47
	GpioCtrlRegs.GPBQSEL1.all = 0x00000000;		
	// No qualification for all group B GPIO 48-63
	GpioCtrlRegs.GPBQSEL2.all = 0x00000000;		
	// All group B GPIO are inputs
	GpioCtrlRegs.GPBDIR.all   = 0x00000000;		
	// All group B pullups disabled due to external pull down
	GpioCtrlRegs.GPBPUD.all   = 0xFFFFFFFF;	
		
	// GPIO32(PIN74) used as AD sync with Slave CPU
	GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 3;		// 0=GPIO  1=SDAA      2=EPWMSYNCI  
												// 3=ADCSOCAO
	// GPIO33-36(PIN75,142,148,145) unused I/O, set input pin.
	GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 0;		// 0=GPIO  1=SCLA      2=EPWMSYNCO  3=ADCSOCBO
	GpioCtrlRegs.GPBDIR.bit.GPIO33  = 0;	
	GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0;		// 0=GPIO  1=ECAP1     2=XREADY     3=XREADY
	GpioCtrlRegs.GPBDIR.bit.GPIO34  = 0;	
    GpioCtrlRegs.GPBMUX1.bit.GPIO35 = 0;		// 0=GPIO  1=SCITXDA   2=XR/W       3=XR/W
	GpioCtrlRegs.GPBDIR.bit.GPIO35  = 0;	
    GpioCtrlRegs.GPBMUX1.bit.GPIO36 = 0;		// 0=GPIO  1=SCIRXDA   2=XZCS0      3=XZCS0
	GpioCtrlRegs.GPBDIR.bit.GPIO36  = 0;	

	// GPIO37(PIN150) Input pin for redundant chip ok signal.   
	GpioCtrlRegs.GPBMUX1.bit.GPIO37 = 0;		// 0=GPIO  1=ECAP2     2=XZCS7      3=XZCS7
	GpioCtrlRegs.GPBDIR.bit.GPIO37  = 0;
	// GPIO38(PIN137) unused I/O, set input pin.
	GpioCtrlRegs.GPBMUX1.bit.GPIO38 = 0;		// 0=GPIO  1=rsvd      2=XWE0       3=XWE0
	GpioCtrlRegs.GPBDIR.bit.GPIO38  = 0;
	// GPIO39(PIN175) Output pin for  M.SEL_A
	GpioCtrlRegs.GPBMUX1.bit.GPIO39 = 0;		// 0=GPIO  1=rsvd      2=XA16       3=XA16
	GpioDataRegs.GPBCLEAR.bit.GPIO39= 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO39  = 1;
	// GPIO40(PIN151) Output pin for redundant chip hardware WD.
	GpioCtrlRegs.GPBMUX1.bit.GPIO40 = 0;		// 0=GPIO  1=rsvd      2=XA0/XWE1   3=XA0/XWE1
	GpioDataRegs.GPBCLEAR.bit.GPIO40= 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO40  = 1;
	// GPIO41-42(PIN152,153) is used for fast communication with slave CPU
	GpioCtrlRegs.GPBMUX1.bit.GPIO41 = 0;		// 0=GPIO  1=rsvd      2=XA1        3=XA1
	GpioDataRegs.GPBCLEAR.bit.GPIO41= 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO41  = 1;
	GpioCtrlRegs.GPBMUX1.bit.GPIO42 = 0;		// 0=GPIO  1=rsvd      2=XA2        3=XA2
	GpioCtrlRegs.GPBDIR.bit.GPIO42  = 0;
	// GPIO43-47(PIN156-158,161,162) unused I/O, set input pin.
	GpioCtrlRegs.GPBMUX1.bit.GPIO43 = 0;		// 0=GPIO  1=rsvd      2=XA3        3=XA3
	GpioCtrlRegs.GPBDIR.bit.GPIO43  = 0;
	GpioCtrlRegs.GPBMUX1.bit.GPIO44 = 0;		// 0=GPIO  1=rsvd      2=XA4        3=XA4
	GpioCtrlRegs.GPBDIR.bit.GPIO44  = 0;
	GpioCtrlRegs.GPBMUX1.bit.GPIO45 = 0;		// 0=GPIO  1=rsvd      2=XA5        3=XA5
	GpioCtrlRegs.GPBDIR.bit.GPIO45  = 0;
	GpioCtrlRegs.GPBMUX1.bit.GPIO46 = 0;		// 0=GPIO  1=rsvd      2=XA6        3=XA6
	GpioCtrlRegs.GPBDIR.bit.GPIO46  = 0;
	GpioCtrlRegs.GPBMUX1.bit.GPIO47 = 0;		// 0=GPIO  1=rsvd      2=XA7        3=XA7
	GpioCtrlRegs.GPBDIR.bit.GPIO47  = 0;
	// GPIO48,49(PIN88,89) are used for CAP, to detect R, S frequency
	GpioCtrlRegs.GPBMUX2.bit.GPIO48 = 1;		// 0=GPIO  1=ECAP5     2=XD31       
												// 3=XD31
	GpioCtrlRegs.GPBMUX2.bit.GPIO49 = 1;		// 0=GPIO  1=ECAP6     2=XD30       
												// 3=XD30
	// GPIO50-53(PIN90,91,94,95) are used for test
	GpioCtrlRegs.GPBMUX2.bit.GPIO50 = 0;		// 0=GPIO  1=EQEP1A    2=XD29        3=XD29
	GpioDataRegs.GPBCLEAR.bit.GPIO50= 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO50  = 1;
	GpioCtrlRegs.GPBMUX2.bit.GPIO51 = 0;		// 0=GPIO  1=EQEP1B    2=XD28        3=XD28
	GpioDataRegs.GPBCLEAR.bit.GPIO51= 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO51  = 1;
	GpioCtrlRegs.GPBMUX2.bit.GPIO52 = 0;		// 0=GPIO  1=EQEP1S    2=XD27        3=XD27
	GpioDataRegs.GPBCLEAR.bit.GPIO52= 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO52  = 1;
	GpioCtrlRegs.GPBMUX2.bit.GPIO53 = 0;		// 0=GPIO  1=EQEP1I    2=XD26        3=XD26
	GpioDataRegs.GPBCLEAR.bit.GPIO53= 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO53  = 1;
	// GPIO54-57(PIN96-99) are SPI port
	GpioCtrlRegs.GPBMUX2.bit.GPIO54 = 1;		// 0=GPIO  1=SPISIMOA  2=XD25       
												// 3=XD25
	GpioCtrlRegs.GPBMUX2.bit.GPIO55 = 1;		// 0=GPIO  1=SPISOMIA  2=XD24       
												// 3=XD24
	GpioCtrlRegs.GPBMUX2.bit.GPIO56 = 1;		// 0=GPIO  1=SPICLKA   2=XD23       
												// 3=XD23
	GpioCtrlRegs.GPBMUX2.bit.GPIO57 = 1;		// 0=GPIO  1=SPISTEA   2=XD22       
												// 3=XD22
	// GPIO58-63(PIN100,110-114) unused I/O, set input pin.
	GpioCtrlRegs.GPBMUX2.bit.GPIO58 = 0;		// 0=GPIO  1=MCLKRA    2=XD21    3=XD21
	GpioCtrlRegs.GPBDIR.bit.GPIO58  = 0;
	GpioCtrlRegs.GPBMUX2.bit.GPIO59 = 0;		// 0=GPIO  1=MFSRA     2=XD20    3=XD20
	GpioCtrlRegs.GPBDIR.bit.GPIO59  = 0;
	GpioCtrlRegs.GPBMUX2.bit.GPIO60 = 0;		// 0=GPIO  1=MCLKRB    2=XD19    3=XD19
	GpioCtrlRegs.GPBDIR.bit.GPIO60  = 0;
	GpioCtrlRegs.GPBMUX2.bit.GPIO61 = 0;		// 0=GPIO  1=MFSRB     2=XD18    3=XD18
	GpioCtrlRegs.GPBDIR.bit.GPIO61  = 0;
	GpioCtrlRegs.GPBMUX2.bit.GPIO62 = 0;		// 0=GPIO  1=SCIRXDC   2=XD17    3=XD17
	GpioCtrlRegs.GPBDIR.bit.GPIO62  = 0;
	GpioCtrlRegs.GPBMUX2.bit.GPIO63 = 0;		// 0=GPIO  1=SCITXDC   2=XD16    3=XD16
	GpioCtrlRegs.GPBDIR.bit.GPIO63  = 0;


	/* Group C pins */

	GpioCtrlRegs.GPCDIR.all = 0x00000000;		// All group C GPIO are inputs
	GpioCtrlRegs.GPCPUD.all = 0xFFFFFFFF;		// All group C pullups disabled
												// due to external pull down
	
	// GPIO64-69(PIN115,116,119,122-124) unused I/O, set input pin.
	GpioCtrlRegs.GPCMUX1.bit.GPIO64 = 0;		// 0=GPIO  1=GPIO      2=XD15        3=XD15
	GpioCtrlRegs.GPCDIR.bit.GPIO64  = 0;
	GpioCtrlRegs.GPCMUX1.bit.GPIO65 = 0;		// 0=GPIO  1=GPIO      2=XD14        3=XD14
	GpioCtrlRegs.GPCDIR.bit.GPIO65  = 0;
	GpioCtrlRegs.GPCMUX1.bit.GPIO66 = 0;		// 0=GPIO  1=GPIO      2=XD13        3=XD13
	GpioCtrlRegs.GPCDIR.bit.GPIO66  = 0;
	GpioCtrlRegs.GPCMUX1.bit.GPIO67 = 0;		// 0=GPIO  1=GPIO      2=XD12        3=XD12
	GpioCtrlRegs.GPCDIR.bit.GPIO67  = 0;
	GpioCtrlRegs.GPCMUX1.bit.GPIO68 = 0;		// 0=GPIO  1=GPIO      2=XD11        3=XD11
	GpioCtrlRegs.GPCDIR.bit.GPIO68  = 0;
	GpioCtrlRegs.GPCMUX1.bit.GPIO69 = 0;		// 0=GPIO  1=GPIO      2=XD10        3=XD10
	GpioCtrlRegs.GPCDIR.bit.GPIO69  = 0;

	// GPIO70(PIN127) is used for shut down aux power
	GpioCtrlRegs.GPCMUX1.bit.GPIO70 = 0;		// 0=GPIO  1=GPIO      2=XD9         3=XD9
	GpioDataRegs.GPCCLEAR.bit.GPIO70= 1;
	GpioCtrlRegs.GPCDIR.bit.GPIO70  = 1;
	// GPIO71(PIN128) is used for aux power monitor
	GpioCtrlRegs.GPCMUX1.bit.GPIO71 = 0;		// 0=GPIO  1=GPIO      2=XD8         3=XD8
	GpioCtrlRegs.GPCDIR.bit.GPIO71  = 0;
	// GPIO72(PIN129) is used for relay control
	GpioCtrlRegs.GPCMUX1.bit.GPIO72 = 0;		// 0=GPIO  1=GPIO      2=XD7         3=XD7
	GpioDataRegs.GPCCLEAR.bit.GPIO72= 1;
	GpioCtrlRegs.GPCDIR.bit.GPIO72  = 1;
	// GPIO73(PIN130) is used for GFCI 50mA self check enable
	GpioCtrlRegs.GPCMUX1.bit.GPIO73 = 0;		// 0=GPIO  1=GPIO      2=XD6         3=XD6
	GpioDataRegs.GPCCLEAR.bit.GPIO73= 1;
	GpioCtrlRegs.GPCDIR.bit.GPIO73  = 1;
	// GPIO74-75(PIN131,132) unused I/O, set input pin.
	GpioCtrlRegs.GPCMUX1.bit.GPIO74 = 0;		// 0=GPIO  1=GPIO      2=XD5         3=XD5
	GpioCtrlRegs.GPCDIR.bit.GPIO74  = 0;
	GpioCtrlRegs.GPCMUX1.bit.GPIO75 = 0;		// 0=GPIO  1=GPIO      2=XD4         3=XD4
	GpioCtrlRegs.GPCDIR.bit.GPIO75  = 0;
	// GPIO76-79(PIN133-136) are used for power stage select
	GpioCtrlRegs.GPCMUX1.bit.GPIO76 = 0;		// 0=GPIO  1=GPIO      2=XD3         3=XD3
	GpioCtrlRegs.GPCDIR.bit.GPIO76  = 0;
	GpioCtrlRegs.GPCMUX1.bit.GPIO77 = 0;		// 0=GPIO  1=GPIO      2=XD2         3=XD2
	GpioCtrlRegs.GPCDIR.bit.GPIO77  = 0;
	GpioCtrlRegs.GPCMUX1.bit.GPIO78 = 0;		// 0=GPIO  1=GPIO      2=XD1         3=XD1
	GpioCtrlRegs.GPCDIR.bit.GPIO78  = 0;
	GpioCtrlRegs.GPCMUX1.bit.GPIO79 = 0;		// 0=GPIO  1=GPIO      2=XD0         3=XD0
	GpioCtrlRegs.GPCDIR.bit.GPIO79  = 0;

	// GPIO80-83(PIN163-165,168) unused I/O, set input pin.
	GpioCtrlRegs.GPCMUX2.bit.GPIO80 = 0;		// 0=GPIO  1=GPIO      2=XA8         3=XA8
	GpioCtrlRegs.GPCDIR.bit.GPIO80  = 0;
	GpioCtrlRegs.GPCMUX2.bit.GPIO81 = 0;		// 0=GPIO  1=GPIO      2=XA9         3=XA9
	GpioCtrlRegs.GPCDIR.bit.GPIO81  = 0;
	GpioCtrlRegs.GPCMUX2.bit.GPIO82 = 0;		// 0=GPIO  1=GPIO      2=XA10        3=XA10
	GpioCtrlRegs.GPCDIR.bit.GPIO82  = 0;
	GpioCtrlRegs.GPCMUX2.bit.GPIO83 = 0;		// 0=GPIO  1=GPIO      2=XA11        3=XA11
	GpioCtrlRegs.GPCDIR.bit.GPIO83  = 0;
	// GPIO84-87(PIN169,172-174) are used as Boot mode pins
	GpioCtrlRegs.GPCMUX2.bit.GPIO84 = 0;		// 0=GPIO  1=GPIO      2=XA12        3=XA12
	GpioCtrlRegs.GPCDIR.bit.GPIO84  = 0;
	GpioCtrlRegs.GPCMUX2.bit.GPIO85 = 0;		// 0=GPIO  1=GPIO      2=XA13        3=XA13
	GpioCtrlRegs.GPCDIR.bit.GPIO85  = 0;
	GpioCtrlRegs.GPCMUX2.bit.GPIO86 = 0;		// 0=GPIO  1=GPIO      2=XA14        3=XA14
	GpioCtrlRegs.GPCDIR.bit.GPIO86  = 0;
	GpioCtrlRegs.GPCMUX2.bit.GPIO87 = 0;		// 0=GPIO  1=GPIO      2=XA15        3=XA15
	GpioCtrlRegs.GPCDIR.bit.GPIO87  = 0;


	/* Re-assign Input qualification for every digital input according to above 
	settings:
 	(1) 6 samples, 
 	(2) no qualification is assigned to Voltage zero crossing detection port(GPIO23/24/25)
 	(3) 00- Syn with SysClock, 01- 3 samples, 10- 6 samples, 11- Asyn
	*/
	// GPIO37 Input pin for redundant chip ok signal.		
	GpioCtrlRegs.GPBQSEL1.bit.GPIO37 = 0x2;	

	// GPIO71 input pin used to detect Aux ok or not		
	// But there is not GPCQSEL!

	/* External interrupt selection */
	GpioIntRegs.GPIOXINT1SEL.all = 0x0000;		// GPIO0 is XINT1 source
	GpioIntRegs.GPIOXINT2SEL.all = 0x0000;		// GPIO0 is XINT2 source
	GpioIntRegs.GPIOXINT3SEL.all = 0x0000;		// GPIO32 is XINT3 source
	GpioIntRegs.GPIOXINT4SEL.all = 0x0000;		// GPIO32 is XINT4 source
	GpioIntRegs.GPIOXINT5SEL.all = 0x0000;		// GPIO32 is XINT5 source
	GpioIntRegs.GPIOXINT6SEL.all = 0x0000;		// GPIO32 is XINT6 source
	GpioIntRegs.GPIOXINT7SEL.all = 0x0000;		// GPIO32 is XINT7 source
	GpioIntRegs.GPIOXNMISEL.all  = 0x0000;		// GPIO0 is XNMI source

	XIntruptRegs.XINT1CR.all = 0x0000;			// XINT1 disabled
	XIntruptRegs.XINT2CR.all = 0x0000;			// XINT2 disabled
	XIntruptRegs.XINT3CR.all = 0x0000;			// XINT3 disabled
	XIntruptRegs.XINT4CR.all = 0x0000;			// XINT4 disabled
	XIntruptRegs.XINT5CR.all = 0x0000;			// XINT5 disabled
	XIntruptRegs.XINT6CR.all = 0x0000;			// XINT6 disabled
	XIntruptRegs.XINT7CR.all = 0x0000;			// XINT7 disabled
	XIntruptRegs.XNMICR.all  = 0x0000;			// XNMI disabled

	/* Low-power mode selection */
	// Any one of GPIO 0-31 can be used as LPM wakeup pin.
	// No pin selected for HALT and STANBY wakeup (reset default)
	GpioIntRegs.GPIOLPMSEL.all = 0x00000000;	

	/* Finish up */
	asm(" EDIS");			// Disable EALLOW protected register access

}	
//===========================================================================
// End of file.
//===========================================================================

