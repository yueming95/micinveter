/*=============================================================================*
 *                          ALL RIGHTS RESERVED
 *
 *
 *  FILENAME : SL_slave_define.h 
 *  PURPOSE  : Header files for global macro defines.
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
/* Macros for Constants and Parameter Definitions */

#define SYSCLOCK_FREQ		(60000000)
#define LSPCLK_FREQ			(SYSCLOCK_FREQ/4)		// defined in SysCtrlRegs
//#define TBCLK_FREQ			(SYSCLOCK_FREQ)		// EPwm Timer clock

/* 288 sampling points in 20ms, 48*3 = 144(10ms), 144*2(20ms) = 288 */

/* 4.8KHz switching frequency for inverter power stage */
//#define PWM_FREQ			20000	
/* (period/2) for 4.8 kHz symmetric PWM with 75MHz TBCLK */		 
//#define PWM_PERIOD		(HSPCLK_FREQ/PWM_FREQ)

/* (period/2) for 4.8 kHz symmetric PWM with 75MHz TBCLK */		 
//#define PWM_HALF_PERIOD		((HSPCLK_FREQ/PWM_FREQ)/2)
/* 5us deadband with 75MHz TBCLK, 5*75 = 375 */	 
//#define DeadBand_Duration	50				//250*2=5us,375-->7.5us


//#define EPWM1_TIMER_TBPRD  32767//2000  // Period register
#ifndef SL_DEFINE_H
#define SL_DEFINE_H

/************************************************************************************/
#define GET_HBYTE_OF_WORD(WORD)			((Uint8)((WORD) >> 8))
#define GET_LBYTE_OF_WORD(WORD)			((Uint8)((WORD) & 0x00FF))

/************************************************************************************/
// Grid relay operation
#define S_GRID_RELAY_R1_ON			(GpioDataRegs.GPASET.bit.GPIO18)
#define S_GRID_RELAY_R1_OFF			(GpioDataRegs.GPACLEAR.bit.GPIO18)

#define S_GRID_RELAY_R2_ON			(GpioDataRegs.GPASET.bit.GPIO17)
#define S_GRID_RELAY_R2_OFF			(GpioDataRegs.GPACLEAR.bit.GPIO17)

#define S_GRID_RELAY_S1_ON			(GpioDataRegs.GPASET.bit.GPIO8)
#define S_GRID_RELAY_S1_OFF 		(GpioDataRegs.GPACLEAR.bit.GPIO8)

#define S_GRID_RELAY_S2_ON			(GpioDataRegs.GPASET.bit.GPIO25)
#define S_GRID_RELAY_S2_OFF			(GpioDataRegs.GPACLEAR.bit.GPIO25)

#define S_GRID_RELAY_T1_ON			(GpioDataRegs.GPBSET.bit.GPIO44)
#define S_GRID_RELAY_T1_OFF			(GpioDataRegs.GPBCLEAR.bit.GPIO44)

#define S_GRID_RELAY_T2_ON			(GpioDataRegs.GPASET.bit.GPIO16)
#define S_GRID_RELAY_T2_OFF			(GpioDataRegs.GPACLEAR.bit.GPIO16)

#define S_BOOST_PWM_ENABLE			(GpioDataRegs.GPACLEAR.bit.GPIO4 = 1)
#define S_INV_PWM_ENABLE			(GpioDataRegs.GPBCLEAR.bit.GPIO40 = 1)

#define S_BOOST_PWM_DISABLE			(GpioDataRegs.GPASET.bit.GPIO4 = 1)
#define S_INV_PWM_DISABLE			(GpioDataRegs.GPBSET.bit.GPIO40 = 1)

#define S_DC_Fan_Enable		    	(GpioDataRegs.GPASET.bit.GPIO7 = 1)
#define S_DC_Fan_Disable		    (GpioDataRegs.GPACLEAR.bit.GPIO7 = 1)

#define ISO1_RELAY_ON               (GpioDataRegs.GPASET.bit.GPIO27)
#define ISO1_RELAY_OFF				(GpioDataRegs.GPACLEAR.bit.GPIO27)

#define ISO2_RELAY_ON				(GpioDataRegs.GPASET.bit.GPIO31)
#define ISO2_RELAY_OFF				(GpioDataRegs.GPACLEAR.bit.GPIO31)



#define AD_DEFAULT_AC_OFFSET  2048

#define Delta15V  150
#define Delta05HZ  50
#define Delta100mA  100

#define SciTxBuffNum  67

#define AD_Channel_Offset_VGRID_Limit 150
#define AD_Channel_Offset_VINV_Limit 150
#define AD_Channel_Offset_DCI_Limit 100
#define AD_Channel_Offset_GFCI_Limit 100

#define MChip_WD_LEVEL   (GpioDataRegs.GPADAT.bit.GPIO21)
#define DC_FAN1_FB_Level (GpioDataRegs.GPADAT.bit.GPIO22)
#define DC_FAN2_FB_Level (GpioDataRegs.GPADAT.bit.GPIO23)

#define	ADC_RESULT_VGRID_R		(AdcResult.ADCRESULT10)
#define	ADC_RESULT_VGRID_S		(AdcResult.ADCRESULT11)
#define	ADC_RESULT_VGRID_T		(AdcResult.ADCRESULT12)
#define	ADC_RESULT_VINV_R		(AdcResult.ADCRESULT13)
#define	ADC_RESULT_VINV_S		(AdcResult.ADCRESULT14)
#define	ADC_RESULT_VINV_T		(AdcResult.ADCRESULT15)
#define	ADC_RESULT_DCI_R		(AdcResult.ADCRESULT6)
#define	ADC_RESULT_DCI_S		(AdcResult.ADCRESULT7)
#define	ADC_RESULT_DCI_T		(AdcResult.ADCRESULT8)
#define	ADC_RESULT_GFCI			(AdcResult.ADCRESULT9)
#define	ADC_RESULT_VPV1			(AdcResult.ADCRESULT0)
#define	ADC_RESULT_VPV2			(AdcResult.ADCRESULT1)
#define	ADC_RESULT_VISO1		(AdcResult.ADCRESULT2)
#define	ADC_RESULT_VISO2		(AdcResult.ADCRESULT3)
#define	ADC_RESULT_MUX_X		(AdcResult.ADCRESULT4)
#define	ADC_RESULT_MUX_Y		(AdcResult.ADCRESULT5)

#define	THETA_STEP_MAX_LIMIT   12240657//11811160(20kHz,43Hz)
#define	THETA_STEP_MIN_LIMIT   9234179//9663676(20kHz,57Hz)

#define	SLAVE_MAIN_VERSION   0x56	
#define	SLAVE_MAIN_VERSION0   0x30	
#define	SLAVE_SECONDARY_VERSION1   0x30
#define	SLAVE_SECONDARY_VERSION2   0x34

#endif


//--- end of file -----------------------------------------------------

