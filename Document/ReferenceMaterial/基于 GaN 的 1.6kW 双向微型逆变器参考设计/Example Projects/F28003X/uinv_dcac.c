//##############################################################################
//
// FILE:  uinv_dcac.c
//
// TITLE:  Solution file for the DC/AC converter stage
//
//#############################################################################
// $TI Release: TIDA_010933 v2.00.00.00 $
// $Release Date: Wed Aug  7 17:16:55 CDT 2024 $
// $Copyright:
// Copyright (C) 2024 Texas Instruments Incorporated - http://www.ti.com/
//
// ALL RIGHTS RESERVED
// $
//#############################################################################

#include <uinv_dcac_settings.h>

#include "driverlib.h"
#include "device.h"
#include "board.h"
#include "c2000ware_libraries.h"

#include "uinv_dcac.h"
#include "spll_1ph_sogi.h"
#include "DCLF32.h"
#include "rampgen.h"
#include "dlog_4ch.h"



/////////////////////////ACDC Settings//////////////////////////

//PWM Frequency

 float32_t uINV_DCAC_Counter_PWM_Unit;



//Ramp Generator parameters
 uint32_t uINV_ACDC_ISR1_FREQUENCY_HZ;
 uint32_t uINV_ACDC_AC_FREQ_HZ;
 RAMPGEN uINV_DCAC_rgen;
 float32_t uINV_DCAC_Index_Modulation;
 float32_t uINV_DCAC_Index_Modulation_Amp;
 float32_t uINV_DCAC_Index_Modulation_K;
 float32_t uINV_ACDC_Duty_Cycle;
 float32_t uINV_DCAC_Amplitude;

// Polarity
 int32_t uINV_DCAC_POS=0;

 int32_t uINV_ACDC_vAC_sensed_FILTER_POS=0;
 float32_t uINV_ACDC_vAC_sensed_FILTER_K_1=0;

/////////////////////////ACDC Settings//////////////////////////

// Virtual Oscilloscope
DLOG_4CH uINV_DCAC_dLog1;
DLOG_4CH uINV_DCAC_dLog2;
float32_t uINV_DCAC_dBuff1[uINV_DCAC_DLOG_SIZE];
float32_t uINV_DCAC_dBuff2[uINV_DCAC_DLOG_SIZE];
float32_t uINV_DCAC_dBuff3[uINV_DCAC_DLOG_SIZE];
float32_t uINV_DCAC_dBuff4[uINV_DCAC_DLOG_SIZE];
float32_t uINV_DCAC_dBuff5[uINV_DCAC_DLOG_SIZE_OSC2 ];
float32_t uINV_DCAC_dBuff6[uINV_DCAC_DLOG_SIZE_OSC2 ];
float32_t uINV_DCAC_dBuff7[uINV_DCAC_DLOG_SIZE_OSC2 ];
float32_t uINV_DCAC_dBuff8[uINV_DCAC_DLOG_SIZE_OSC2 ];
float32_t uINV_DCAC_dVal1;
float32_t uINV_DCAC_dVal2;
float32_t uINV_DCAC_dVal3;
float32_t uINV_DCAC_dVal4;
float32_t uINV_DCAC_dVal5;
float32_t uINV_DCAC_dVal6;
float32_t uINV_DCAC_dVal7;
float32_t uINV_DCAC_dVal8;

//Temperature Measurements
#pragma RETAIN (uINV_ACDC_HS_GaN_Temp_Duty)
#pragma RETAIN (uINV_ACDC_LS_GaN_Temp_Duty)
#pragma RETAIN (uINV_ACDC_HS_GaN_Temp)
#pragma RETAIN (uINV_ACDC_LS_GaN_Temp)
float32_t uINV_ACDC_HS_GaN_Temp_Duty;
float32_t uINV_ACDC_LS_GaN_Temp_Duty;
float32_t uINV_ACDC_HS_GaN_Temp;
float32_t uINV_ACDC_LS_GaN_Temp;
float32_t uINV_DCDC_HS_GaN_Temp_Duty;
float32_t uINV_DCDC_LS_GaN_Temp_Duty;
float32_t uINV_DCDC_HS_GaN_Temp;
float32_t uINV_DCDC_LS_GaN_Temp;
uint32_t uINV_ACDC_GaN_Temp_Counter=0;

// 90 Degree Phase shifting PLL
float32_t uINV_ACDC_filter_1;
float32_t uINV_ACDC_filter_2;
float32_t uINV_ACDC_90_Shifted;

#pragma RETAIN (uINV_reset_PLL)
#pragma RETAIN (uINV_DCAC_Relay_Enable)
int32_t uINV_reset_PLL;
int32_t uINV_DCAC_Relay_Enable;

SPLL_1PH_SOGI uINV_spll_1ph;

//Analog Measurements
#pragma RETAIN (uINV_ACDC_iAC_sensed_pu)
#pragma RETAIN (uINV_DCAC_vAC_sensed_pu)
#pragma RETAIN (uINV_ACDC_vDC_sensed_pu)
#pragma RETAIN (uINV_ACDC_iDC_sensed_pu)
float32_t uINV_ACDC_iAC_sensed_pu;
float32_t uINV_DCAC_vAC_sensed_pu;
float32_t uINV_ACDC_vDC_sensed_pu;
float32_t uINV_ACDC_iDC_sensed_pu;


#pragma RETAIN (uINV_ACDC_iAC_sensed)
#pragma RETAIN (uINV_DCAC_vAC_sensed)
#pragma RETAIN (uINV_ACDC_vDC_sensed)
#pragma RETAIN (uINV_ACDC_iDC_sensed)
float32_t uINV_ACDC_iAC_sensed;
float32_t uINV_DCAC_vAC_sensed;
float32_t uINV_ACDC_vDC_sensed;
float32_t uINV_ACDC_iDC_sensed;
//



//Filtering of the measurements and other actions
#pragma RETAIN (uINV_ACDC_vDC_sensed_FILTER)
#pragma RETAIN (uINV_ACDC_vDC_sensed_NOTCH)
float32_t uINV_ACDC_vDC_sensed_FILTER;
float32_t uINV_ACDC_vDC_sensed_NOTCH;

#pragma RETAIN (uINV_ACDC_vAC_sensed_FILTER)
float32_t uINV_ACDC_vAC_sensed_FILTER;

float32_t uINV_ACDC_vAC_sensed_FILTER_1H;

//State Machines
#pragma RETAIN (uINV_ACDC_fault)
#pragma RETAIN (uINV_ACDC_flag)
#pragma RETAIN (uINV_ACDC_stop)
int32_t uINV_ACDC_fault;
int32_t uINV_ACDC_flag;
int32_t uINV_ACDC_stop;

//Control Variables Reference values
#pragma RETAIN (uINV_ACDC_iAC_real_Ref)
#pragma RETAIN (uINV_ACDC_iAC_imag_Ref)
#pragma RETAIN (uINV_DCAC_iAC_Ref)
#pragma RETAIN (uINV_ACDC_iAC_amp_Ref)
volatile float32_t uINV_ACDC_iAC_real_Ref; // you control the amplitude of the real part
volatile float32_t uINV_ACDC_iAC_real_Ref_Prev; // you will use this one to monitor the peak current.
volatile float32_t uINV_ACDC_iAC_imag_Ref; // you control the amplitude of the imag part
volatile float32_t uINV_DCAC_iAC_Ref; // final results of real and imag together.
volatile float32_t uINV_ACDC_iAC_amp_Ref;

volatile float32_t uINV_ACDC_iAC_real_Ref_OUT_PI;

#pragma RETAIN (uINV_ACDC_vDC_Ref)
#pragma RETAIN (uINV_ACDC_vDC_Ref_Slewed)
volatile float32_t uINV_ACDC_vDC_Ref;
volatile float32_t uINV_ACDC_vDC_Ref_Slewed;

//Control Variables Error
float32_t uINV_ACDC_iAC_loop_err; // you control the amplitude of the real part
float32_t  uINV_ACDC_vDC_loop_err; // you control the amplitude of the real part

float32_t uINV_ACDC_iAC_loop_out_PI;
float32_t uINV_ACDC_iAC_loop_out_PR1;
float32_t uINV_ACDC_iAC_loop_out_PR3;
float32_t uINV_ACDC_iAC_loop_out_PR5;
float32_t uINV_ACDC_iAC_loop_out_PR7;
float32_t uINV_ACDC_iAC_loop_out_PR9;
float32_t uINV_ACDC_iAC_loop_out_SUM;


DCL_PI gi_ACDC_pi = PI_DEFAULTS;
DCL_PI gv_ACDC_pi = PI_DEFAULTS;

DCL_DF22 gi_r1 = DF22_DEFAULTS;
DCL_DF22 gi_r3 = DF22_DEFAULTS;
DCL_DF22 gi_r5 = DF22_DEFAULTS;
DCL_DF22 gi_r7 = DF22_DEFAULTS;
DCL_DF22 gi_r9 = DF22_DEFAULTS;

//to see
DCL_DF22 gi_Vac_Filter = DF22_DEFAULTS;  //Filtering out only the 50 Hz component as a FFW
float32_t kiI_1H_Vac,woI_1H_Vac,wrcI_1H_Vac; // Ki is equal to zero
float32_t uINV_ACDC_FFW_K=1;
float32_t uINV_ACDC_FFW_K_Slewed=1;

//to see

// for tuning PR and R controllers
float32_t kpI_1H;
float32_t kiI_1H, kiI_3H, kiI_5H, kiI_7H, kiI_9H;
float32_t woI_1H, woI_3H, woI_5H, woI_7H, woI_9H;
float32_t wrcI_1H, wrcI_3H, wrcI_5H, wrcI_7H, wrcI_9H;


DCL_DF22 VDC_NOTCH_FILTER_2_Fe= DF22_DEFAULTS;

volatile uint16_t uINV_DCAC_clearPWMTrip;
volatile uint16_t uINV_DCAC_update_Duty_Flag;
volatile uint16_t uINV_EN_PR_K_calculation;

//debugging
volatile uint16_t uINV_DCAC_Counter_1;
volatile uint16_t uINV_DCAC_Counter_Disable_FFW;

volatile float32_t uINV_REF_CHANGE1; // you control the amplitude of the real part
volatile float32_t uINV_REF_CHANGE2; // you control the amplitude of the real part
volatile float32_t uINV_REF_CHANGE3; // you control the amplitude of the real part
//debugging

void uINV_DCAC_Init() {


    //Overcurrent Fault Setting

    // Trigger event when DCBH is high
    EPWM_setTripZoneDigitalCompareEventCondition(DCAC_BASE,
                                                 EPWM_TZ_DC_OUTPUT_B1,
                                                 EPWM_TZ_EVENT_DCXH_HIGH);

    // Configure DCBH to use TRIP5 as an input
    EPWM_enableDigitalCompareTripCombinationInput(DCAC_BASE,
                                                  EPWM_DC_COMBINATIONAL_TRIPIN5,
                                                  EPWM_DC_TYPE_DCBH);

    // Enable DCB as OST
    EPWM_enableTripZoneSignals(DCAC_BASE, EPWM_TZ_SIGNAL_DCBEVT1);



    // Configure the DCB path to be unfiltered and asynchronous
    EPWM_setDigitalCompareEventSource(DCAC_BASE,
                                      EPWM_DC_MODULE_B,
                                      EPWM_DC_EVENT_1,
                                      EPWM_DC_EVENT_SOURCE_ORIG_SIGNAL);


    //
    // Sets the value of the internal DAC of the low and high comparator.
    //
    CMPSS_setDACValueHigh(myCMPSS_BASE,uINV_ACDC_iAC_TRIP_LIMIT_POS);
    CMPSS_setDACValueLow(myCMPSS_BASE,uINV_ACDC_iAC_TRIP_LIMIT_NEG);



    //GPIO INIT

    GPIO_writePin(uINV_DCAC_Relay, 0);
    GPIO_writePin(ENABLE, 0); // Enable signal of the LF gate driver
    GPIO_writePin(uINV_DCAC_Top_LF, 0);
    GPIO_writePin(uINV_DCAC_Bottom_LF, 0);

}


////////////////Notch filter and PR controllers//////////

void computeDF22_PRcontrollerCoeff(DCL_DF22 *v, float32_t kp, float32_t ki,
                                   float32_t wo, float32_t fs, float32_t wrc)
{
    float32_t temp1, temp2, wo_adjusted;
    wo_adjusted = 2.0f*fs*tanf(wo/(2.0f*fs));

    temp1 = 4.0f*fs*fs+wo_adjusted*wo_adjusted+4.0f*fs*wrc;
    temp2 = 4.0f*ki*wrc*fs/temp1;
    v->b0 = temp2;
    v->b1 = 0;
    v->b2=-temp2;
    v->a1=((-8.0f*fs*fs+2*wo_adjusted*wo_adjusted)/temp1);
    v->a2=((temp1-8.0f*fs*wrc)/temp1);
    v->x1 = 0;
    v->x2 = 0;

    if(kp!=0)
    {
        v->b0+=kp;
        v->b1+=kp*v->a1;
        v->b2+=kp*v->a2;
    }

    v->a1=(v->a1);
    v->a2=(v->a2);
}

void computeDF22_NotchFltrCoeff(DCL_DF22 *v, float32_t Fs, float32_t notch_freq,
                                float32_t c1, float32_t c2)
{
    float32_t temp1;
    float32_t temp2;
    float32_t wn2;
    float32_t Ts;
    Ts = 1.0f/Fs;

    // pre warp the notch frequency
    wn2 = 2.0f*Fs*tanf(notch_freq*CONST_PI_32*Ts);  /////////// Here there was a PI, not able to localized in the project and used directly CONST_PI_32!!!!!!/////

    temp1= 4.0f*Fs*Fs + 4.0f* wn2 * c2 * Fs + wn2*wn2;
    temp2= 1.0f/ (4.0f*Fs*Fs + 4.0f* wn2 * c1 * Fs + wn2*wn2);

    v->b0 = temp1* temp2;
    v->b1 = (-8.0f*Fs*Fs + 2.0f* wn2* wn2)* temp2;
    v->b2 = (4.0f*Fs*Fs-4*wn2*c2*Fs+wn2*wn2)*temp2;
    v->a1 = (-8.0f*Fs*Fs + 2.0f* wn2* wn2)*temp2;
    v->a2 = (4.0f*Fs*Fs-4.0f*wn2*c1*Fs+wn2*wn2)*temp2;

}

////////////////Notch filter and PR controllers//////////

void uINV_DCAC_GlobalVariablesInit()
{
    uINV_ACDC_fault=0;
    uINV_ACDC_stop=0;
    uINV_ACDC_flag=0;
    uINV_ACDC_ISR1_FREQUENCY_HZ=20000;
    uINV_ACDC_AC_FREQ_HZ = 50;
    uINV_DCAC_Index_Modulation=0;
    uINV_ACDC_Duty_Cycle=0;
    uINV_DCAC_Amplitude=0.83636;
    uINV_DCAC_Counter_PWM_Unit = (CLK_FREQ/(uINV_DCAC_BOOST_PWM_FREQ*2));




    uINV_ACDC_filter_1=0;
    uINV_ACDC_filter_2=0;


    uINV_reset_PLL=0;


    uINV_DCAC_clearPWMTrip=0;
    uINV_DCAC_update_Duty_Flag=0;

 //It generates a triangolar signal for cosphi and sinphi  Init
    RAMPGEN_reset(&uINV_DCAC_rgen);

    RAMPGEN_config(&uINV_DCAC_rgen,
                   ISR_CONTROL_FREQUENCY_AC,
                   uINV_ACDC_AC_FREQ_HZ);

 //It generates a triangolar signal for cosphi and sinphi  Init

//Oscilloscope Init Function
    DLOG_4CH_reset(&uINV_DCAC_dLog1);
    DLOG_4CH_config(&uINV_DCAC_dLog1,
                    &uINV_DCAC_dVal1, &uINV_DCAC_dVal2, &uINV_DCAC_dVal3, &uINV_DCAC_dVal4,
                    uINV_DCAC_dBuff1, uINV_DCAC_dBuff2, uINV_DCAC_dBuff3, uINV_DCAC_dBuff4,
                    uINV_DCAC_DLOG_SIZE, uINV_ACDC_DLOG_TRIGGER, uINV_ACDC_DLOG_SCALE);


    DLOG_4CH_reset(&uINV_DCAC_dLog2);
    DLOG_4CH_config(&uINV_DCAC_dLog2,
                    &uINV_DCAC_dVal5, &uINV_DCAC_dVal6, &uINV_DCAC_dVal7, &uINV_DCAC_dVal8,
                    uINV_DCAC_dBuff5, uINV_DCAC_dBuff6, uINV_DCAC_dBuff7, uINV_DCAC_dBuff8,
                    uINV_DCAC_DLOG_SIZE_OSC2, uINV_ACDC_DLOG_TRIGGER, uINV_ACDC_DLOG_SCALE_OSC2);

 //Oscilloscope Init Function





    //Initialization of the variables of the PLL

    SPLL_1PH_SOGI_reset(&uINV_spll_1ph);

    SPLL_1PH_SOGI_config(&uINV_spll_1ph,
                             50,
                             20000,
                             uINV_SPLL_COEFF_B0,
                             uINV_SPLL_COEFF_B1);

    SPLL_1PH_SOGI_coeff_calc(&uINV_spll_1ph);

    //Initialization of the variables of the PLL


    //Initialization of the variables of the control variables.

    uINV_ACDC_iAC_real_Ref=0;
    uINV_ACDC_iAC_imag_Ref=0;
    uINV_DCAC_iAC_Ref=0;
    uINV_ACDC_iAC_amp_Ref=0;




    uINV_ACDC_iAC_loop_err=0;

    uINV_ACDC_iAC_loop_out_PI=0;
    uINV_ACDC_iAC_loop_out_PR1=0;
    uINV_ACDC_iAC_loop_out_PR3=0;
    uINV_ACDC_iAC_loop_out_PR5=0;
    uINV_ACDC_iAC_loop_out_PR7=0;
    uINV_ACDC_iAC_loop_out_PR9=0;
    uINV_ACDC_iAC_loop_out_SUM=0;

// Calculation of the Proportional REsonant coefficient


    kpI_1H = KPI_1H;
    kiI_1H = KII_1H;
    wrcI_1H= WRCI_1H;
    woI_1H = 2.0*CONST_PI_32*AC_FREQ;
    computeDF22_PRcontrollerCoeff(&gi_r1, kpI_1H,kiI_1H,woI_1H,
                                   ISR_CONTROL_FREQUENCY_AC,wrcI_1H);


    kiI_3H = KII_3H;
    wrcI_3H = WRCI_3H;
    woI_3H = 2.0*CONST_PI_32*AC_FREQ*3;
    computeDF22_PRcontrollerCoeff(&gi_r3, 0,kiI_3H,woI_3H,
                                  ISR_CONTROL_FREQUENCY_AC,wrcI_3H);

    kiI_5H = KII_5H;
    wrcI_5H = WRCI_5H;
    woI_5H = 2.0*CONST_PI_32*AC_FREQ*5;
    computeDF22_PRcontrollerCoeff(&gi_r5, 0,kiI_5H,woI_5H,
                                  ISR_CONTROL_FREQUENCY_AC,wrcI_5H);

    kiI_7H = KII_7H;
    wrcI_7H = WRCI_7H;
    woI_7H = 2.0*CONST_PI_32*AC_FREQ*7;
    computeDF22_PRcontrollerCoeff(&gi_r7, 0,kiI_7H,woI_7H,
                                  ISR_CONTROL_FREQUENCY_AC,wrcI_7H);

    kiI_9H = KII_9H;
    wrcI_9H = WRCI_9H;
    woI_9H = 2.0*CONST_PI_32*AC_FREQ*9;
    computeDF22_PRcontrollerCoeff(&gi_r9, 0,kiI_9H,woI_9H,
                                  ISR_CONTROL_FREQUENCY_AC,wrcI_9H);


    gi_ACDC_pi.Kp = gi_ACDC_pi_KP;
    gi_ACDC_pi.Ki = gi_ACDC_pi_KI;
    gi_ACDC_pi.Umax = gi_ACDC_pi_MAX;
    gi_ACDC_pi.Umin = gi_ACDC_pi_MIN;

    gv_ACDC_pi.Kp =  gv_ACDC_pi_KP;
    gv_ACDC_pi.Ki = gv_ACDC_pi_KI;
    gv_ACDC_pi.Umax = gv_ACDC_pi_MAX;
    gv_ACDC_pi.Umin = gv_ACDC_pi_MIN;

    computeDF22_NotchFltrCoeff(&VDC_NOTCH_FILTER_2_Fe, (ISR_CONTROL_FREQUENCY_AC), (AC_FREQ*2.0) , 0.25f, 0.00001f);
    computeDF22_NotchFltrCoeff(&gi_Vac_Filter, (ISR_CONTROL_FREQUENCY_AC), 760 , 0.25f, 0.01f);

}

void uINV_DCAC_test() {



    if(uINV_reset_PLL == 1)
    {
        uINV_reset_PLL = 0;
        SPLL_1PH_SOGI_reset(&uINV_spll_1ph);
    }

    if(uINV_DCAC_Relay_Enable == 1)
    {
        GPIO_writePin(uINV_DCAC_Relay, 1);
        uINV_DCAC_Relay_Enable = 0;

    }


    //Code to enable the virtual logger to trigger when you change the reference current
    if(uINV_ACDC_iAC_real_Ref != uINV_ACDC_iAC_real_Ref_Prev)
    {
        uINV_ACDC_iAC_real_Ref_Prev=uINV_ACDC_iAC_real_Ref;
        uINV_DCAC_Counter_1=0; //reset the counter
    }

    //Code for changing in real time the gains of the proportional resonant controllers
    if(uINV_EN_PR_K_calculation == 1)
    {
        //Parameters to change kpI_1H  kiI_1H
        computeDF22_PRcontrollerCoeff(&gi_r1, kpI_1H,kiI_1H,woI_1H,
                                       ISR_CONTROL_FREQUENCY_AC,wrcI_1H);

        computeDF22_PRcontrollerCoeff(&gi_r3, 0,kiI_3H,woI_3H,
                                      ISR_CONTROL_FREQUENCY_AC,wrcI_3H);


        computeDF22_PRcontrollerCoeff(&gi_r5, 0,kiI_5H,woI_5H,
                                      ISR_CONTROL_FREQUENCY_AC,wrcI_5H);

        computeDF22_PRcontrollerCoeff(&gi_r7, 0,kiI_7H,woI_7H,
                                      ISR_CONTROL_FREQUENCY_AC,wrcI_7H);

        computeDF22_PRcontrollerCoeff(&gi_r9, 0,kiI_9H,woI_9H,
                                      ISR_CONTROL_FREQUENCY_AC,wrcI_9H);

        uINV_EN_PR_K_calculation=0;
    }

    uINV_DCAC_GaN_temp();
}

void uINV_DCAC_GaN_temp() {

         //Code for temperature measurements
         uINV_ACDC_GaN_Temp_Counter= uINV_ACDC_GaN_Temp_Counter+1;
         if(uINV_ACDC_GaN_Temp_Counter == uINV_ACDC_GaN_Temp_Scaling) {
             uINV_ACDC_GaN_Temp_Counter = 0;
         }
         if(uINV_ACDC_GaN_Temp_Counter==(0*uINV_ACDC_GaN_Temp_Scaling/4))
         {
         //Code for acquiring the temperatures
             uINV_ACDC_HS_GaN_Temp_Duty= ((float)ECAP_getEventTimeStamp(myECAP0_BASE, ECAP_EVENT_3)-(float)ECAP_getEventTimeStamp(myECAP0_BASE, ECAP_EVENT_2))/(ECAP_getEventTimeStamp(myECAP0_BASE, ECAP_EVENT_3)-ECAP_getEventTimeStamp(myECAP0_BASE, ECAP_EVENT_1));
             uINV_ACDC_HS_GaN_Temp=uINV_ACDC_GaN_Temp_Coefficient*uINV_ACDC_HS_GaN_Temp_Duty+uINV_ACDC_GaN_Temp_Offset;
             ECAP_selectECAPInput(myECAP0_BASE,ECAP_INPUT_INPUTXBAR2);
             ECAP_resetCounters(myECAP0_BASE);
             ECAP_reArm(myECAP0_BASE);
         }
         else if(uINV_ACDC_GaN_Temp_Counter==(1*uINV_ACDC_GaN_Temp_Scaling/4))
         {
         //Code for acquiring the temperatures
             uINV_ACDC_LS_GaN_Temp_Duty= ((float)ECAP_getEventTimeStamp(myECAP0_BASE, ECAP_EVENT_3)-(float)ECAP_getEventTimeStamp(myECAP0_BASE, ECAP_EVENT_2))/(ECAP_getEventTimeStamp(myECAP0_BASE, ECAP_EVENT_3)-ECAP_getEventTimeStamp(myECAP0_BASE, ECAP_EVENT_1));
             uINV_ACDC_LS_GaN_Temp=uINV_ACDC_GaN_Temp_Coefficient*uINV_ACDC_LS_GaN_Temp_Duty+uINV_ACDC_GaN_Temp_Offset;
             ECAP_selectECAPInput(myECAP0_BASE,ECAP_INPUT_INPUTXBAR3);
             ECAP_resetCounters(myECAP0_BASE);
             ECAP_reArm(myECAP0_BASE);
         }
         else if(uINV_ACDC_GaN_Temp_Counter==(2*uINV_ACDC_GaN_Temp_Scaling/4))
         {
         //Code for acquiring the temperatures
             uINV_DCDC_HS_GaN_Temp_Duty= ((float)ECAP_getEventTimeStamp(myECAP0_BASE, ECAP_EVENT_3)-(float)ECAP_getEventTimeStamp(myECAP0_BASE, ECAP_EVENT_2))/(ECAP_getEventTimeStamp(myECAP0_BASE, ECAP_EVENT_3)-ECAP_getEventTimeStamp(myECAP0_BASE, ECAP_EVENT_1));
             uINV_DCDC_HS_GaN_Temp=uINV_ACDC_GaN_Temp_Coefficient*uINV_DCDC_HS_GaN_Temp_Duty+uINV_ACDC_GaN_Temp_Offset;
             ECAP_selectECAPInput(myECAP0_BASE,ECAP_INPUT_INPUTXBAR4);
             ECAP_resetCounters(myECAP0_BASE);
             ECAP_reArm(myECAP0_BASE);
         }
         else if(uINV_ACDC_GaN_Temp_Counter==(3*uINV_ACDC_GaN_Temp_Scaling/4))
         {
         //Code for acquiring the temperatures
             uINV_DCDC_LS_GaN_Temp_Duty= ((float)ECAP_getEventTimeStamp(myECAP0_BASE, ECAP_EVENT_3)-(float)ECAP_getEventTimeStamp(myECAP0_BASE, ECAP_EVENT_2))/(ECAP_getEventTimeStamp(myECAP0_BASE, ECAP_EVENT_3)-ECAP_getEventTimeStamp(myECAP0_BASE, ECAP_EVENT_1));
             uINV_DCDC_LS_GaN_Temp=uINV_ACDC_GaN_Temp_Coefficient*uINV_DCDC_LS_GaN_Temp_Duty+uINV_ACDC_GaN_Temp_Offset;
             ECAP_selectECAPInput(myECAP0_BASE,ECAP_INPUT_INPUTXBAR1);
             ECAP_resetCounters(myECAP0_BASE);
             ECAP_reArm(myECAP0_BASE);
         }
         else
         {
             // do nothing
         }
 }
