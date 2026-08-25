/*=============================================================================*
 *  FILENAME : SC_define.h 
 *  PURPOSE  : Header files for global macro defines 
 *  HISTORY  :
 *    DATE            VERSION        AUTHOR            NOTE
 *
 *============================================================================*/
/* Macros for Constants and Parameter Definitions */
/* 288 sampling points in 20ms, 48*3 = 144(10ms), 144*2(20ms) = 288 */

#ifndef SC_DEFINE_H
#define SC_DEFINE_H

/*When release, _IPOMS_ macro should be cancelled*/
#define _IPOMS_
/************************************************************************************/
//parameters for PI loop
#define KPI_PLL	(8.485e-6f)//18.485e-6f//1.697e-5f//2.863e-5f//1.697e-5f//5.588e-6f//1.36988e-5f		
#define KP_PLL	(8.41e-6f)//1.682e-5f//2.82e-5f//1.682e-5f//5.571e-6f//1.365e-5f	

#define KPI_BOOST_VPID	(1.823e-2)//(1.225e-2)//(4.45e-3f)//(1.255e-2f)//(4.45e-3f)//(16.021)//(4.45e-3f)//(2.225e-3f)//(1.30e-3f)//(2.639e-2f)//(1.3e-2f)
#define KP_BOOST_VPID	(1.821e-2f)//(1.221e-2f)//(4.442e-3f)//(1.221e-2f)//(4.442e-3f)//(16.014)//(4.442e-3f)//(2.221e-3f)//(1.0e-3f)//(2.6e-2f)//(1.0e-2f)

#define KPI_BOOST_IPID	(10.005)//(14.174)//(14.874)//(18.593)//(0.1573)//(18.593)//(10.005)//(46.23)//(100.2)//(100.005)
#define KP_BOOST_IPID	(9.975)//(13.945)//(13.945)//(17.431)//(0.157)//(17.431)//(9.975)//(46.0)//(100.0)//(100.003)

#define KPI_BUS_PID	(0.0583)//(0.284)//(0.0583)
#define KP_BUS_PID	(0.0581)//(0.282)//(0.0581)

#define KPI_CURRENT_PID	(25.833)//(26.233)//(5.653)//(26.833)//(25.462)//(25.9165)//(12.9165)//25.833//51.666f				//250*2=5us,375-->7.5us        //2011.3.21 xlg
#define KP_CURRENT_PID	(25.21)//(5.65)//(25.21)//(25.60525)//(12.60525)//25.2105//50.421f				//250*2=5us,375-->7.5us        //2011.3.21 xlg

#define KP_VBUS_BOOST_PID (2)

#define KPI_BUSBANLANCE_PID	(1.0332)
#define KP_BUSBANLANCE_PID	(1.0008)
#define BUS_BANLANCE_MAX	60
/************************************************************************************/
#define DELTA_POWERDC_30W_30K	(10)
#define DELTA_POWERDC_10W_10K	(6)//11558 //5779
#define DELTA_POWERDC__5W_8K	(5)
#define DELTA_POWERDC__5W_6K	(3)

#define DeltaMPPT19W_5K	(10) //21960//11558

#define DeltaMPPTV		(4.0)    // 1V

//#define DeltaPowerDC6W_5K 2311//6934 //3467
//#define DeltaPowerDC3W_4K 3467
//#define DeltaPowerDC3W_3K 5779
   
#define PV15V			(25.0)
#define PV250V			(250)



#define VBUS_REF_MINLIMIT_500V		(500)//300 // 176*2.45 + 50 = 481
#define VBUS_REF_MAXLIMIT_730V		(730)

#define VALUE_2PI	(6.283185307)
#define VALUE_PI 	(3.141592654)
/************************************************************************************/
/* DSP/BIOS Macros for ADC calibration */
#define ADC_CAL_FUNC_PTR		((void (*)(void))0x380080)

/************************************************************************************/
#define GET_HBYTE_OF_WORD(WORD)			((Uint8)((WORD) >> 8))
#define GET_LBYTE_OF_WORD(WORD)			((Uint8)((WORD) & 0x00FF))

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

#define FABS(a) (((a) > 0) ? (a) : (-(a)))

/************************************************************************************/
// Power stage selection
#define CHOICE_KIND1_STATE         (GpioDataRegs.GPCDAT.bit.GPIO76)
#define CHOICE_KIND2_STATE         (GpioDataRegs.GPCDAT.bit.GPIO77)
#define CHOICE_KIND3_STATE         (GpioDataRegs.GPCDAT.bit.GPIO78)
#define CHOICE_KIND4_STATE         (GpioDataRegs.GPCDAT.bit.GPIO79)

/************************************************************************************/
//ad channel
#define AD_DEFAULT_AC_OFFSET	(2048.0)
#define VBUS_MEASURE_GAIN		(184.84 * 3 / 4096)//(186.69 * 3 / 4096)
#define VGRID_MEASURE_GAIN		(297.12 * 3 / 4096)//(290.66 * 3 / 4096)//
#define VPV_MEASURE_GAIN		(344.24 * 3 / 4096)//(347.68 * 3 / 4096)
#define GFCI_MEASURE_GAIN		(649.1 * 3 / 4096)

#define DCI_MEASURE_GAIN_10K	(1132.0 * 3 / 4096)
#define DCI_MEASURE_GAIN_15K		(1132.0 * 3 / 4096)
#define DCI_MEASURE_GAIN_17K		(1132.0 * 3 / 4096)
#define DCI_MEASURE_GAIN_20K	(1132.0 * 3 / 4096)

#define IGRID_MEASURE_GAIN_10K	(19.67 * 3 / 4096)//(34.57 * 3 / 4096)
#define IGRID_MEASURE_GAIN_15K	(25.72 * 3 / 4096)
#define IGRID_MEASURE_GAIN_17K	(29.35 * 3 / 4096)//(34.18 * 3 / 4096)
#define IGRID_MEASURE_GAIN_20K	(34.18 * 3 / 4096)

#define IPV_MEASURE_GAIN_10K		(6.7088 * 3 / 4096)
#define IPV_MEASURE_GAIN_15K		(8.6159 * 3 / 4096)
#define IPV_MEASURE_GAIN_17K	(8.6159 * 3 / 4096)//(9.8814 * 3 / 4096)
#define IPV_MEASURE_GAIN_20K    (9.8814  * 3 / 4096)//0.00724

//#define GFCI_MEASURE_GAIN_30K   (649.1 * 3 / 4096)

/* A2----IBoost2  */
#define	ADC_RESULT_VPV2		(AdcMirror.ADCRESULT0)
/* B2----VGrid_R  */
#define	ADC_RESULT_VBUSP		(AdcMirror.ADCRESULT1)
/* A3----IBoost1  */
#define	ADC_RESULT_VPV1		(AdcMirror.ADCRESULT2)
/* B3----VGrid_S  */
#define	ADC_RESULT_VBUSN		(AdcMirror.ADCRESULT3)
/* A4----IPV2  */
#define	ADC_RESULT_IPV21			(AdcMirror.ADCRESULT4)
/* B4----VGrid_T */
#define	ADC_RESULT_VGRID_R		(AdcMirror.ADCRESULT5)
/* A5----IPV1  */
#define	ADC_RESULT_IPV11			(AdcMirror.ADCRESULT6)
/* B5----GFCI */
#define	ADC_RESULT_VGRID_S			(AdcMirror.ADCRESULT7)
/* A6----VPV2 */
#define	ADC_RESULT_IPV2			(AdcMirror.ADCRESULT8)
/* B6----VBus+ */
#define	ADC_RESULT_VGRID_T		(AdcMirror.ADCRESULT9)
/* A7----VPV1 */
#define	ADC_RESULT_IPV1			(AdcMirror.ADCRESULT10)
/* B7----VBus- */
#define	ADC_RESULT_GFCI		(AdcMirror.ADCRESULT11)
/* A0----IGrid_S  */
#define	ADC_RESULT_IGRID_S		(AdcMirror.ADCRESULT12)
/* B0----IGrid_T    */
#define	ADC_RESULT_IGRID_T		(AdcMirror.ADCRESULT13)
/* A1----IGrid_R  */
#define	ADC_RESULT_IGRID_R		(AdcMirror.ADCRESULT14)
/* B1----ILDci */
#define	ADC_RESULT_IDCI			(AdcMirror.ADCRESULT15)

/************************************************************************************/
// AD DCI select
#define	M_SELECT_DCI_R			(GpioDataRegs.GPACLEAR.bit.GPIO31=1,GpioDataRegs.GPBCLEAR.bit.GPIO39=1)
#define	M_SELECT_DCI_S			(GpioDataRegs.GPACLEAR.bit.GPIO31=1,GpioDataRegs.GPBSET.bit.GPIO39=1)
#define	M_SELECT_DCI_T			(GpioDataRegs.GPASET.bit.GPIO31=1,GpioDataRegs.GPBCLEAR.bit.GPIO39=1)

/************************************************************************************/
// Digital IO protection signal
#define AC_OCP_LEVEL			(GpioDataRegs.GPADAT.bit.GPIO12)
#define PV1_OCP_LEVEL			(GpioDataRegs.GPADAT.bit.GPIO13)
#define PV2_OCP_LEVEL			(GpioDataRegs.GPADAT.bit.GPIO14)
#define BUS_OVP_LEVEL			(GpioDataRegs.GPADAT.bit.GPIO15)
#define AUX_OK_LEVEL			(GpioDataRegs.GPCDAT.bit.GPIO71)
#define RCHIP_WD_LEVEL			(GpioDataRegs.GPBDAT.bit.GPIO37)

#define	M_INVPWM_ENABLEOCP			(GpioDataRegs.GPASET.bit.GPIO16= 1)
#define	M_INVPWM_DISABLEOCP			(GpioDataRegs.GPACLEAR.bit.GPIO16= 1)
#define M_INVPWM_RESET_LEVEL			(GpioDataRegs.GPADAT.bit.GPIO16)

#define	M_INVPWM_UNLOCK			(GpioDataRegs.GPASET.bit.GPIO17=1)
#define	M_INVPWM_LOCK			(GpioDataRegs.GPACLEAR.bit.GPIO17=1)
/************************************************************************************/
//PWM
/* (period/2) for symmetric PWM with 75MHz TBCLK */		 
#define PWM_HALF_PERIOD				(1875)//----->FREQ 16.02K  //((HSPCLK_FREQ/PWM_FREQ)/2)
/* (period/2) for symmetric PWM with 75MHz TBCLK */		 
#define PWM_PERIOD					(PWM_HALF_PERIOD * 2) //(HSPCLK_FREQ/PWM_FREQ)
/* Switching frequency for inverter power stage, 1 standard for 1Hz */
#define PWM_FREQ					(HSPCLK_FREQ/PWM_PERIOD)//(16000)
#define APWM_PERIOD					(PWM_PERIOD * 2)
/* 5us deadband with 75MHz TBCLK, 5*75 = 375 */	 
#define DEAD_BAND_DURATION			(80)			//250*2=5us,375-->7.5us

//#define EPWM1_TIMER_TBPRD			(32767)//2000	// Period register
#define SVPWM_DUTY_LIMIT			(float32)(PWM_HALF_PERIOD * 1.15)//(2690)
#define SVPWM_DUTY_LIMIT_2          (float32)(SVPWM_DUTY_LIMIT * SVPWM_DUTY_LIMIT)//(7236100)
/************************************************************************************/
//Grid Frequency Limits Specifications
#define RATED_GIRD_FREQUENCY		(50)	// Unit, Hz
#define RATED_60HZ_GIRD_FREQUENCY	(60)	// Unit, Hz
#define RATED_DEVIATION_GRID_FREQ	(1)		// Unit, Hz
#define MAX_GRID_FREQUENCY			(RATED_GIRD_FREQUENCY + RATED_DEVIATION_GRID_FREQ)	
#define MIN_GRID_FREQUENCY			(RATED_GIRD_FREQUENCY - RATED_DEVIATION_GRID_FREQ)
#define CoffStepToFre				(PWM_FREQ/VALUE_2PI)
/************************************************************************************/
// Limits Specifications in ProtectionLogic
#define IAC_UNBALANCE_LIMIT 		(3)
#define PV_UNDER_VOLT_LIMIT			(250)
#define PV_OVER_VOLT_LIMIT			(960)
#define BUS_UNBALANCE_LIMIT 		(50)
#define BUS_OVER_VOLT_LIMIT			(960)	
#define BUS_SINGLE_INSTANT_OVER_VOLT_LIMIT		(550)
#define BUS_INSTANT_OVER_VOLT_LIMIT	(1000)	
#define AD_OFFSET_VGRID_LIMIT		(15)
#define AD_OFFSET_IGRID_LIMIT		(2)
#define AD_OFFSET_DCI_LIMIT			(500)
/************************************************************************************/

#define BOOST_VLOOPOUT_MAX		(24.5)

#define BOOST_DUTY_MAX			(APWM_PERIOD * 0.75)

/************************************************************************************/
// MPPT InputMode
#define	INDEPENDENT_IN			(1)
#define	PARALLEL_IN				(0)
#define	DC_SOURCE_IN			(2)

/************************************************************************************/
//PLL limits
#define PLL_MAX_GRID_FREQUENCY		(RATED_GIRD_FREQUENCY * 0.11)  //55.5Hz
#define PLL_MIN_GRID_FREQUENCY		(RATED_GIRD_FREQUENCY * 0.11)  //44.5Hz
#define THETA_STEP_RATED			(RATED_GIRD_FREQUENCY * VALUE_2PI / PWM_FREQ)
#define THETA_STEP_MAX_LIMIT		(PLL_MAX_GRID_FREQUENCY * VALUE_2PI / PWM_FREQ)//0.02042035
#define THETA_STEP_MIN_LIMIT		(PLL_MIN_GRID_FREQUENCY * VALUE_2PI / PWM_FREQ)//0.0109956

/************************************************************************************/
// Grid relay operation
#define M_GRID_RELAY_ON				(GpioDataRegs.GPCSET.bit.GPIO72)
#define M_GRID_RELAY_OFF			(GpioDataRegs.GPCCLEAR.bit.GPIO72)
/************************************************************************************/
// GFCI 
#define GFCI_50mA_ON				(GpioDataRegs.GPCSET.bit.GPIO73 = 1)
#define GFCI_50mA_OFF				(GpioDataRegs.GPCCLEAR.bit.GPIO73 = 1)

/************************************************************************************/
// Energy measure gain 
#define	ENERGY_MEASURE_GAIN			(1.38889e-4f)

/************************************************************************************/
// SPI 
#define SPI_FIFO_LENTH		(16)
#define SPI_COM_LENGTH		(32)
/************************************************************************************/
// SCI port
#define ID_SCIA				(0)
#define ID_SCIB				(1)
#define ID_SCIC				(2)

#define MAX_SCI_NO			(3)

#define NULL				(0)

#define	SCI_TX_RDY			(0)
#define	SCI_TX_BUSY			(1)

#define	SCI_RX_RDY			(0)
#define	SCI_RX_EMPTY		(1)

#define MODE_INT			(0)
#define MODE_INQUIRE		(1)

#define QUE_BUF_NORMAL		(0)
#define QUE_BUF_FULL		(1)
#define QUE_BUF_EMPTY		(2)

#ifdef _IPOMS_
#define SCIA_BUF_SIZE		(64)
#else
#define SCIA_BUF_SIZE		(0)
#endif
#define SCIB_BUF_SIZE		(128)
#define SCIC_BUF_SIZE		(0)		
#define MAX_SCI_BUF_SIZE	(SCIA_BUF_SIZE + SCIB_BUF_SIZE + SCIC_BUF_SIZE)

#define	SCI_UPDATE_COMM_BAUDRATE	(9600)
#define	IPOMS_COMM_BAUDRATE	(9600)

#define IPOMS_MAX_COMMAND_LENGTH 	(50)
#define IPOMS_CHAR_ENTER 	(13)

// Sci macro for IPOMS
#define	ID_SCI_IPOMS		(ID_SCIA)

#define	MASTER_MAIN_VERSION   0x56	
#define	MASTER_MAIN_VERSION0   0x30	
#define	MASTER_SECONDARY_VERSION1   0x30
#define	MASTER_SECONDARY_VERSION2   0x35
#endif
//--- end of file -----------------------------------------------------

