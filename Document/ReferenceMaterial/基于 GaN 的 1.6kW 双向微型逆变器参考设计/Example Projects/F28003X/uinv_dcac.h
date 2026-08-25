//##############################################################################
//
// FILE:  uinv_dcac.h
//
// TITLE: Solution header file for the DC/AC converter stage
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

#ifndef UINV_DCAC_H_
#define UINV_DCAC_H_

#include "uinv_dcac_settings.h"
#include "stdint.h"
#include "rampgen.h"
#include "dlog_4ch.h"
#include "spll_1ph_sogi.h"
#include "DCLF32.h"
#include "uinv_dcdc.h"


//Definition of the function in C
void uINV_DCAC_Init();
void uINV_DCAC_test();
void uINV_DCAC_GlobalVariablesInit();
void uINV_DCAC_GaN_temp();

//Ramp Generator parameters
 extern uint32_t uINV_ACDC_ISR1_FREQUENCY_HZ;
 extern uint32_t uINV_ACDC_AC_FREQ_HZ;
 extern RAMPGEN uINV_DCAC_rgen;
 extern float32_t uINV_DCAC_Index_Modulation;
 extern float32_t uINV_DCAC_Index_Modulation_Amp;
 extern float32_t uINV_DCAC_Index_Modulation_K;
 extern float32_t uINV_ACDC_Duty_Cycle;
 extern float32_t uINV_DCAC_Amplitude;

 extern int32_t uINV_DCAC_POS;

 extern int32_t uINV_ACDC_vAC_sensed_FILTER_POS;
 extern float32_t uINV_ACDC_vAC_sensed_FILTER_K_1;

// PWM Setting
 extern float32_t uINV_DCAC_Counter_PWM_Unit;
 //

/////////////////////////ACDC Settings//////////////////////////

// Virtual Oscilloscope
 extern DLOG_4CH uINV_DCAC_dLog1;
 extern DLOG_4CH uINV_DCAC_dLog2;
 extern float32_t uINV_DCAC_dBuff1[uINV_DCAC_DLOG_SIZE];
 extern float32_t uINV_DCAC_dBuff2[uINV_DCAC_DLOG_SIZE];
 extern float32_t uINV_DCAC_dBuff3[uINV_DCAC_DLOG_SIZE];
 extern float32_t uINV_DCAC_dBuff4[uINV_DCAC_DLOG_SIZE];
 extern float32_t uINV_DCAC_dBuff5[uINV_DCAC_DLOG_SIZE_OSC2];
 extern float32_t uINV_DCAC_dBuff6[uINV_DCAC_DLOG_SIZE_OSC2];
 extern float32_t uINV_DCAC_dBuff7[uINV_DCAC_DLOG_SIZE_OSC2];
 extern float32_t uINV_DCAC_dBuff8[uINV_DCAC_DLOG_SIZE_OSC2];
 extern float32_t uINV_DCAC_dVal1;
 extern float32_t uINV_DCAC_dVal2;
 extern float32_t uINV_DCAC_dVal3;
 extern float32_t uINV_DCAC_dVal4;
 extern float32_t uINV_DCAC_dVal5;
 extern float32_t uINV_DCAC_dVal6;
 extern float32_t uINV_DCAC_dVal7;
 extern float32_t uINV_DCAC_dVal8;
//

//Temperature Measurements
 extern float32_t uINV_ACDC_HS_GaN_Temp_Duty;
 extern float32_t uINV_ACDC_LS_GaN_Temp_Duty;
 extern float32_t uINV_ACDC_HS_GaN_Temp;
 extern float32_t uINV_ACDC_LS_GaN_Temp;
 extern uint32_t uINV_ACDC_GaN_Temp_Counter;

// 90 degree phase shifted
 extern float32_t uINV_ACDC_filter_1;
 extern float32_t uINV_ACDC_filter_2;
 extern float32_t uINV_ACDC_90_Shifted;

 extern SPLL_1PH_SOGI uINV_spll_1ph;



 extern int32_t uINV_DCAC_Relay_Enable;
//


//Analog Measurements
 extern float32_t uINV_ACDC_iAC_sensed_pu;
 extern float32_t uINV_DCAC_vAC_sensed_pu;
 extern float32_t uINV_ACDC_vDC_sensed_pu;
 extern float32_t uINV_ACDC_iDC_sensed_pu;


 extern float32_t uINV_ACDC_iAC_sensed;
 extern float32_t uINV_DCAC_vAC_sensed;
 extern float32_t uINV_ACDC_vDC_sensed;
 extern float32_t uINV_ACDC_iDC_sensed;
//

 // Internal conditioning of the measurements
 extern float32_t uINV_ACDC_vDC_sensed_FILTER;
 extern float32_t uINV_ACDC_vDC_sensed_NOTCH;

 extern float32_t uINV_ACDC_vAC_sensed_FILTER;

 extern float32_t uINV_ACDC_vAC_sensed_FILTER_1H;


//Control Variables
 extern volatile float32_t uINV_ACDC_iAC_real_Ref;
 extern volatile float32_t uINV_ACDC_iAC_imag_Ref;
 extern volatile float32_t uINV_DCAC_iAC_Ref;
 extern volatile float32_t uINV_ACDC_iAC_real_Ref_Prev;
 extern volatile float32_t uINV_ACDC_iAC_amp_Ref;
 extern volatile float32_t uINV_ACDC_iAC_real_Ref_OUT_PI;

 extern volatile float32_t uINV_ACDC_vDC_Ref;
 extern volatile float32_t uINV_ACDC_vDC_Ref_Slewed;



 //Control Variables Error
 extern float32_t uINV_ACDC_iAC_loop_err; // you control the amplitude of the real part
 extern float32_t  uINV_ACDC_vDC_loop_err; // you control the amplitude of the real part

 extern float32_t uINV_ACDC_iAC_loop_out_PI;
 extern float32_t uINV_ACDC_iAC_loop_out_PR1;
 extern float32_t uINV_ACDC_iAC_loop_out_PR3;
 extern float32_t uINV_ACDC_iAC_loop_out_PR5;
 extern float32_t uINV_ACDC_iAC_loop_out_PR7;
 extern float32_t uINV_ACDC_iAC_loop_out_PR9;
 extern float32_t uINV_ACDC_iAC_loop_out_SUM;

 extern DCL_DF22 gi_r1;
 extern DCL_DF22 gi_r3;
 extern DCL_DF22 gi_r5;
 extern DCL_DF22 gi_r7;
 extern DCL_DF22 gi_r9;

 extern DCL_DF22 gi_Vac_Filter; //Filtering out only the 50 Hz component as a FFW
 extern float32_t kiI_1H_Vac,woI_1H_Vac,wrcI_1H_Vac;

 extern  float32_t uINV_ACDC_FFW_K;
 extern  float32_t uINV_ACDC_FFW_K_Slewed;

 extern DCL_DF22 VDC_NOTCH_FILTER_2_Fe;


 extern float32_t kpI_1H;
 extern float32_t kiI_1H, kiI_3H, kiI_5H, kiI_7H, kiI_9H;
 extern float32_t woI_1H, woI_3H, woI_5H, woI_7H, woI_9H;
 extern float32_t wrcI_1H, wrcI_3H, wrcI_5H, wrcI_7H, wrcI_9H;


 extern DCL_PI gi_ACDC_pi;
 extern DCL_PI gv_ACDC_pi;


//


//State Machines
 extern int32_t uINV_ACDC_fault;
 extern int32_t uINV_ACDC_flag;
 extern int32_t uINV_ACDC_stop;
 extern volatile uint16_t uINV_DCAC_clearPWMTrip;
 extern volatile uint16_t uINV_DCAC_update_Duty_Flag;


 extern volatile uint16_t uINV_EN_PR_K_calculation;

 extern volatile uint16_t uINV_DCAC_Counter_1;
 extern volatile uint16_t uINV_DCAC_Counter_Disable_FFW;

 extern volatile float32_t uINV_REF_CHANGE1; // you control the amplitude of the real part
 extern volatile float32_t uINV_REF_CHANGE2; // you control the amplitude of the real part
 extern volatile float32_t uINV_REF_CHANGE3; // you control the amplitude of the real part





 //Definition of the function in C
 void computeDF22_NotchFltrCoeff(DCL_DF22 *v, float32_t sampling_freq,
                                 float32_t notch_freq, float32_t c1, float32_t c2);
 void computeDF22_PRcontrollerCoeff(DCL_DF22 *v, float32_t kp, float32_t ki, float32_t wo,
                                    float32_t fs, float32_t wrc);
 //Definition of the function in C










#pragma FUNC_ALWAYS_INLINE(uINV_DCAC_readCurrentAndVoltageSignals)
static inline void uINV_DCAC_readCurrentAndVoltageSignals()
{
    //combine later on the measurements
     uINV_ACDC_vDC_sensed_pu =  ADC_readResult(SOC_DC_BUS_VS_RESULT_BASE, SOC_DC_BUS_VS)*uINV_ACDC_ADC_PU_SCALE_FACTOR;  //
     uINV_ACDC_iDC_sensed_pu =  ADC_readResult(SOC_DC_BUS_CS1_RESULT_BASE, SOC_DC_BUS_CS1)*uINV_ACDC_ADC_PU_SCALE_FACTOR*2-1.0f; //bipolar
     uINV_DCAC_vAC_sensed_pu =  ADC_readResult(SOC_AC_VS_RESULT_BASE, SOC_AC_VS)*uINV_ACDC_ADC_PU_SCALE_FACTOR*2-1.0f; //bipolar
     uINV_ACDC_iAC_sensed_pu =  ADC_readResult(SOC_AC_CS1_RESULT_BASE, SOC_AC_CS1)*uINV_ACDC_ADC_PU_SCALE_FACTOR*2-1.0f; //bipolar

     uINV_ACDC_vDC_sensed = uINV_ACDC_vDC_sensed_pu*uINV_ACDC_vDC_MAX + uINV_ACDC_vDC_Offset;
     uINV_ACDC_iDC_sensed = uINV_ACDC_iDC_sensed_pu*uINV_ACDC_iDC_MAX + uINV_ACDC_iDC_Offset;
     uINV_DCAC_vAC_sensed = uINV_DCAC_vAC_sensed_pu*uINV_ACDC_vAC_MAX + uINV_ACDC_vAC_Offset;
     uINV_ACDC_iAC_sensed = uINV_ACDC_iAC_sensed_pu*uINV_ACDC_iAC_MAX + uINV_ACDC_iAC_Offset;


     //Low Pass Filtering of the voltages
     uINV_ACDC_vDC_sensed_FILTER += uINV_DC_FILTERING_CONSTANT*(uINV_ACDC_vDC_sensed-uINV_ACDC_vDC_sensed_FILTER);
     uINV_ACDC_vAC_sensed_FILTER += uINV_AC_FILTERING_CONSTANT*(uINV_DCAC_vAC_sensed-uINV_ACDC_vAC_sensed_FILTER);

     //PR for filtering out the Voltage
     uINV_ACDC_vAC_sensed_FILTER_1H= DCL_runDF22_C4(&gi_Vac_Filter, uINV_ACDC_vAC_sensed_FILTER);

     //Notch Filtering
     uINV_ACDC_vDC_sensed_NOTCH = DCL_runDF22_C4(&VDC_NOTCH_FILTER_2_Fe, uINV_ACDC_vDC_sensed_FILTER);

     //Overvoltageprotection

    if(uINV_ACDC_vDC_sensed_FILTER > uINV_ACDC_vDC_TRIP_LIMIT_V)
    {
     uINV_ACDC_fault=1;
    }
 }

 static inline void uINV_ACDC_clearPWMTripFlags(uint32_t base)
 {
     //
     // Clear all trip flags
     //
     EPWM_clearTripZoneFlag(base, (EPWM_TZ_INTERRUPT_OST |
                                   EPWM_TZ_INTERRUPT_CBC |
                                   EPWM_TZ_INTERRUPT_DCAEVT1));

     //
     // Clear all source flags for the One-shot trip zone
     //
     EPWM_clearOneShotTripZoneFlag(base, (EPWM_TZ_OST_FLAG_OST1 |
                                          EPWM_TZ_OST_FLAG_OST2 |
                                          EPWM_TZ_OST_FLAG_OST3 |
                                          EPWM_TZ_OST_FLAG_DCAEVT1));
}



#pragma FUNC_ALWAYS_INLINE(uINV_DCAC_update_Duty)
static inline void uINV_DCAC_update_Duty()
{
     if((EPWM_getTripZoneFlagStatus(DCAC_BASE) & EPWM_TZ_FLAG_DCBEVT1)) {uINV_ACDC_fault=1;}

     //Saturation control. Avoid to reach extreme duty cycle conditions
     uINV_DCAC_Index_Modulation = (uINV_DCAC_Index_Modulation > (float32_t)(uINV_DCAC_Index_Modulation_HIGH_PU))?
             (float32_t)(uINV_DCAC_Index_Modulation_HIGH_PU):uINV_DCAC_Index_Modulation;
     uINV_DCAC_Index_Modulation = (uINV_DCAC_Index_Modulation < (float32_t)(uINV_DCAC_Index_Modulation_LOW_PU))?
             (float32_t)(uINV_DCAC_Index_Modulation_LOW_PU):uINV_DCAC_Index_Modulation;

     if(uINV_ACDC_fault==0)
     {

            if(uINV_DCAC_Index_Modulation >= 0.03f)
            {

                EPWM_clearTripZoneFlag(DCAC_BASE, EPWM_TZ_FORCE_EVENT_OST |EPWM_TZ_FLAG_OST);
                GPIO_writePin(uINV_DCAC_Top_LF, 0);
                GPIO_writePin(uINV_DCAC_Bottom_LF, 1);
                uINV_ACDC_Duty_Cycle=(uINV_DCAC_Index_Modulation);
                uINV_DCAC_POS=1;

            }
            else if(uINV_DCAC_Index_Modulation < 0.03f && uINV_DCAC_Index_Modulation >= 0.015f)
            {

                GPIO_writePin(uINV_DCAC_Top_LF, 0);
                GPIO_writePin(uINV_DCAC_Bottom_LF, 0);
                uINV_ACDC_Duty_Cycle=(uINV_DCAC_Index_Modulation);
                EPWM_clearTripZoneFlag(DCAC_BASE, EPWM_TZ_FORCE_EVENT_OST |EPWM_TZ_FLAG_OST);
                uINV_ACDC_flag=1;

            }
            else if(uINV_DCAC_Index_Modulation < 0.015f && uINV_DCAC_Index_Modulation >= -0.015f)
            {
                GPIO_writePin(uINV_DCAC_Top_LF, 0);
                GPIO_writePin(uINV_DCAC_Bottom_LF, 0);
                EPWM_forceTripZoneEvent(DCAC_BASE, EPWM_TZ_FORCE_EVENT_OST);
                if( uINV_DCAC_POS==1)
                {
                    uINV_ACDC_Duty_Cycle=(1+(uINV_DCAC_Index_Modulation));
                }
                else
                {
                    uINV_ACDC_Duty_Cycle=(uINV_DCAC_Index_Modulation);
                }

                if(uINV_ACDC_stop==1)
                {
                    uINV_ACDC_fault=1;
                }

            }
            else if(uINV_DCAC_Index_Modulation < -0.015f && uINV_DCAC_Index_Modulation >= -0.030f)
            {
                GPIO_writePin(uINV_DCAC_Top_LF, 0);
                GPIO_writePin(uINV_DCAC_Bottom_LF, 0);

                uINV_ACDC_Duty_Cycle=(1+(uINV_DCAC_Index_Modulation));
                EPWM_clearTripZoneFlag(DCAC_BASE, EPWM_TZ_FORCE_EVENT_OST |EPWM_TZ_FLAG_OST);
            }
            else
            {
                EPWM_clearTripZoneFlag(DCAC_BASE, EPWM_TZ_FORCE_EVENT_OST |EPWM_TZ_FLAG_OST);
                GPIO_writePin(uINV_DCAC_Top_LF, 1);
                GPIO_writePin(uINV_DCAC_Bottom_LF , 0);
                uINV_ACDC_Duty_Cycle=(1+(uINV_DCAC_Index_Modulation));
                uINV_DCAC_POS=0;
            }
     }
     else
     {
         GPIO_writePin(uINV_DCAC_Relay,0);
         GPIO_writePin(uINV_DCAC_Top_LF,0);
         GPIO_writePin(uINV_DCAC_Bottom_LF,0);
         EPWM_forceTripZoneEvent(DCAC_BASE, EPWM_TZ_FORCE_EVENT_OST);
         uINV_DCDC_fail();
     }
     EPWM_setCounterCompareValue(DCAC_BASE, EPWM_COUNTER_COMPARE_A, uINV_ACDC_Duty_Cycle*uINV_DCAC_Counter_PWM_Unit);
}

#pragma FUNC_ALWAYS_INLINE(uINV_RAMP_VDC)
static inline void uINV_RAMP_VDC()
{


        if((uINV_ACDC_vDC_Ref - uINV_ACDC_vDC_Ref_Slewed) > (2.0f * uINV_VOLTS_PER_SECOND_SLEW) *
                                        (ISR_CONTROL_FREQUENCY_AC_INVERSE))
     {
            uINV_ACDC_vDC_Ref_Slewed =  uINV_ACDC_vDC_Ref_Slewed + (uINV_VOLTS_PER_SECOND_SLEW) *
                               (ISR_CONTROL_FREQUENCY_AC_INVERSE);
     }
     else if((uINV_ACDC_vDC_Ref - uINV_ACDC_vDC_Ref_Slewed) <
                        - (2.0f * uINV_VOLTS_PER_SECOND_SLEW) *
                               (ISR_CONTROL_FREQUENCY_AC_INVERSE))
     {
         uINV_ACDC_vDC_Ref_Slewed =  uINV_ACDC_vDC_Ref_Slewed - (uINV_VOLTS_PER_SECOND_SLEW) *
                               (ISR_CONTROL_FREQUENCY_AC_INVERSE);
     }
     else
     {
         uINV_ACDC_vDC_Ref_Slewed =  uINV_ACDC_vDC_Ref;
     }


}

#pragma FUNC_ALWAYS_INLINE(uINV_DCAC_runISR1_lab1)  //Run in open loop
 static inline void uINV_DCAC_runISR1_lab1()
 {
     RAMPGEN_run(&uINV_DCAC_rgen);

     uINV_DCAC_Index_Modulation = uINV_DCAC_Amplitude*(__sinpuf32(uINV_DCAC_rgen.out));

     if(uINV_DCAC_clearPWMTrip == 1U)
     {
         // Wait for zero-crossing then clear trips and start power stage
         if(uINV_DCAC_Index_Modulation < 0.006f && uINV_DCAC_Index_Modulation >= -0.006f)
               {
                 uINV_DCAC_clearPWMTrip = 0;
                 uINV_DCAC_update_Duty_Flag=1;
                 EPWM_clearTripZoneFlag(DCAC_BASE, EPWM_TZ_INTERRUPT |EPWM_TZ_FLAG_OST);
                 uINV_DCAC_Counter_1=0;
               }
     }

     if(uINV_DCAC_update_Duty_Flag == 1U)
     {
             uINV_DCAC_update_Duty();
     }

     uINV_DCAC_dVal1 = uINV_DCAC_Index_Modulation;
     uINV_DCAC_dVal2 = uINV_DCAC_vAC_sensed_pu;
     uINV_DCAC_dVal3 = uINV_DCAC_vAC_sensed ;
     uINV_DCAC_dVal4 = uINV_DCAC_Index_Modulation;
     DLOG_4CH_run(&uINV_DCAC_dLog1);
     //

 }

#pragma FUNC_ALWAYS_INLINE(uINV_DCAC_runISR1_lab2) //Run in open loop together with PLL.
 static inline void uINV_DCAC_runISR1_lab2()
 {
     RAMPGEN_run(&uINV_DCAC_rgen);

     uINV_DCAC_Index_Modulation = uINV_DCAC_Amplitude*(__sinpuf32(uINV_DCAC_rgen.out));
     SPLL_1PH_SOGI_run(&uINV_spll_1ph,
                       uINV_DCAC_vAC_sensed);

     if(uINV_DCAC_clearPWMTrip == 1U)
     {
         // Wait for zero-crossing then clear trips and start power stage
         if(__sinpuf32(uINV_DCAC_rgen.out) < 0.05f && __sinpuf32(uINV_DCAC_rgen.out) >= -0.05f)
               {
                     uINV_DCAC_clearPWMTrip = 0;
                     uINV_DCAC_update_Duty_Flag=1;
                     EPWM_clearTripZoneFlag(DCAC_BASE, EPWM_TZ_INTERRUPT |EPWM_TZ_FLAG_OST);
                     uINV_DCAC_Counter_1=0;
               }
     }

     if(uINV_DCAC_update_Duty_Flag == 1U)
     {

             uINV_DCAC_update_Duty();

     }

     uINV_DCAC_dVal1 = uINV_DCAC_Index_Modulation;
     uINV_DCAC_dVal2 = (uINV_DCAC_vAC_sensed/(uINV_ACDC_vDC_sensed+1));
     uINV_DCAC_dVal3 = uINV_ACDC_iAC_sensed ;
     uINV_DCAC_dVal4 = uINV_DCAC_vAC_sensed;
     DLOG_4CH_run(&uINV_DCAC_dLog1);
 }

#pragma FUNC_ALWAYS_INLINE(uINV_DCAC_runISR1_lab3) //DC Link voltage control. Resistor connected on the AC terminals.
static inline void uINV_DCAC_runISR1_lab3()
{
     RAMPGEN_run(&uINV_DCAC_rgen);

     SPLL_1PH_SOGI_run(&uINV_spll_1ph,uINV_ACDC_vAC_sensed_FILTER);

     if(uINV_DCAC_clearPWMTrip == 1U)
     {
         uINV_DCAC_clearPWMTrip = 0;
         uINV_DCAC_update_Duty_Flag=1;
         EPWM_clearTripZoneFlag(DCAC_BASE, EPWM_TZ_INTERRUPT |EPWM_TZ_FLAG_OST);
         uINV_DCAC_Counter_1=0;
     }

     if(uINV_DCAC_update_Duty_Flag == 1U)
     {

            uINV_ACDC_vDC_loop_err=uINV_ACDC_vDC_Ref_Slewed-uINV_ACDC_vDC_sensed_NOTCH;
            uINV_DCAC_Index_Modulation_Amp=(DCL_runPI_C2(&gv_ACDC_pi,uINV_ACDC_vDC_loop_err,0));

            uINV_DCAC_Index_Modulation_K=uINV_ACDC_vDC_sensed_NOTCH/(uINV_ACDC_vDC_sensed_FILTER+1);
            uINV_DCAC_Index_Modulation = -1.0f*uINV_DCAC_Index_Modulation_Amp*(__sinpuf32(uINV_DCAC_rgen.out))*uINV_DCAC_Index_Modulation_K;
            uINV_DCAC_update_Duty();
     }

     uINV_DCAC_dVal1 = __sinpuf32(uINV_DCAC_rgen.out);
     uINV_DCAC_dVal2 = uINV_DCAC_vAC_sensed;
     uINV_DCAC_dVal3 = uINV_ACDC_iAC_sensed;
     uINV_DCAC_dVal4 = uINV_ACDC_vDC_sensed;
     DLOG_4CH_run(&uINV_DCAC_dLog1);
}

#pragma FUNC_ALWAYS_INLINE(uINV_DCAC_runISR1_lab4) //Run current control loop across external resistor.
static inline void uINV_DCAC_runISR1_lab4()
{

     RAMPGEN_run(&uINV_DCAC_rgen);

     SPLL_1PH_SOGI_run(&uINV_spll_1ph,
                       uINV_DCAC_vAC_sensed_pu);

     //Reference Generation, still to implement the correct saturations.
     uINV_DCAC_iAC_Ref =uINV_ACDC_iAC_real_Ref_Prev*(__sinpuf32(uINV_DCAC_rgen.out))+uINV_ACDC_iAC_imag_Ref*(__cospuf32(uINV_DCAC_rgen.out));

     if(uINV_DCAC_clearPWMTrip == 1U)   // Start the machine!!!!!!!!!!!!!
     {
         // Wait for zero-crossing then clear trips and start power stage
         if(__sinpuf32(uINV_DCAC_rgen.out) < 0.05f && __sinpuf32(uINV_DCAC_rgen.out) >= -0.05f )
               {
                     uINV_DCAC_clearPWMTrip = 0;
                     uINV_DCAC_update_Duty_Flag=1;
                     EPWM_clearTripZoneFlag(DCAC_BASE, EPWM_TZ_INTERRUPT |EPWM_TZ_FLAG_OST);
                     uINV_DCAC_Counter_1=0; // so you can trigger during the start up
               }
     }
     if(uINV_DCAC_update_Duty_Flag == 1U)
     {

            uINV_ACDC_iAC_loop_err=uINV_DCAC_iAC_Ref-uINV_ACDC_iAC_sensed;
            uINV_ACDC_iAC_loop_out_PI= DCL_runPI_C2(&gi_ACDC_pi,uINV_ACDC_iAC_loop_err,0);
            uINV_ACDC_iAC_loop_out_PR1= DCL_runDF22_C1(&gi_r1, uINV_ACDC_iAC_loop_err);
            uINV_ACDC_iAC_loop_out_PR3= DCL_runDF22_C1(&gi_r3, uINV_ACDC_iAC_loop_err);
            uINV_ACDC_iAC_loop_out_PR5= DCL_runDF22_C1(&gi_r5, uINV_ACDC_iAC_loop_err);
            uINV_ACDC_iAC_loop_out_PR7= DCL_runDF22_C1(&gi_r7, uINV_ACDC_iAC_loop_err);
            uINV_ACDC_iAC_loop_out_PR9= DCL_runDF22_C1(&gi_r9, uINV_ACDC_iAC_loop_err);
            uINV_DCAC_Index_Modulation = (uINV_ACDC_iAC_loop_out_PI+uINV_ACDC_iAC_loop_out_PR1+uINV_ACDC_iAC_loop_out_PR3+uINV_ACDC_iAC_loop_out_PR5+uINV_ACDC_iAC_loop_out_PR7+uINV_ACDC_iAC_loop_out_PR9)/(uINV_ACDC_vDC_sensed_FILTER+5);
            uINV_DCAC_update_Duty();

     }


     //Oscilloscope
     uINV_DCAC_dVal1 = __sinpuf32(uINV_DCAC_rgen.out);
     uINV_DCAC_dVal2 = uINV_DCAC_vAC_sensed ;
     uINV_DCAC_dVal3 = uINV_ACDC_iAC_sensed;
     uINV_DCAC_dVal4 = uINV_DCAC_Index_Modulation;
     DLOG_4CH_run(&uINV_DCAC_dLog1);
     //Oscilloscope
     //
}

#pragma FUNC_ALWAYS_INLINE(uINV_DCAC_runISR1_lab5) //Run in close loop together with PLL toward the grid. No DC link voltage control
 static inline void uINV_DCAC_runISR1_lab5()
 {

     SPLL_1PH_SOGI_run(&uINV_spll_1ph,
                       uINV_DCAC_vAC_sensed_pu);

     //Zero crossing of the grid
     if((uINV_ACDC_vAC_sensed_FILTER-uINV_ACDC_vAC_sensed_FILTER_K_1) >= 0.4f)
     {
         uINV_ACDC_vAC_sensed_FILTER_POS=1;
     }
     else
     {
         uINV_ACDC_vAC_sensed_FILTER_POS=0;
     }
     uINV_ACDC_vAC_sensed_FILTER_K_1=uINV_ACDC_vAC_sensed_FILTER;
     //Zero crossing of the grid



     //Reference Generation, still to implement the correct saturations.
     uINV_DCAC_iAC_Ref =uINV_ACDC_iAC_real_Ref_Prev*(uINV_spll_1ph.sine)+uINV_ACDC_iAC_imag_Ref*(uINV_spll_1ph.cosine);

     if(uINV_DCAC_clearPWMTrip == 1U)   // Start the machine!!!!!!!!!!!!!
     {
         // Wait for zero-crossing then clear trips and start power stage
         if(uINV_ACDC_vAC_sensed_FILTER < 2 && uINV_ACDC_vAC_sensed_FILTER >= -2 && uINV_ACDC_vAC_sensed_FILTER_POS==1)
               {
                     uINV_DCAC_POS=1;
                     uINV_DCAC_clearPWMTrip = 0;
                     uINV_DCAC_update_Duty_Flag=1;
                     EPWM_clearTripZoneFlag(DCAC_BASE, EPWM_TZ_INTERRUPT |EPWM_TZ_FLAG_OST);
                     uINV_DCAC_Counter_1=0;
               }
     }
     if(uINV_DCAC_update_Duty_Flag == 1U)
     {

            uINV_ACDC_iAC_loop_err=uINV_DCAC_iAC_Ref-uINV_ACDC_iAC_sensed;
            uINV_ACDC_iAC_loop_out_PI= DCL_runPI_C2(&gi_ACDC_pi,uINV_ACDC_iAC_loop_err,0);
            uINV_ACDC_iAC_loop_out_PR1= DCL_runDF22_C1(&gi_r1, uINV_ACDC_iAC_loop_err);
            uINV_ACDC_iAC_loop_out_PR3= DCL_runDF22_C1(&gi_r3, uINV_ACDC_iAC_loop_err);
            uINV_ACDC_iAC_loop_out_PR5= DCL_runDF22_C1(&gi_r5, uINV_ACDC_iAC_loop_err);
            uINV_ACDC_iAC_loop_out_PR7= DCL_runDF22_C1(&gi_r7, uINV_ACDC_iAC_loop_err);
            uINV_ACDC_iAC_loop_out_PR9= DCL_runDF22_C1(&gi_r9, uINV_ACDC_iAC_loop_err);
            uINV_DCAC_Index_Modulation = (uINV_ACDC_vAC_sensed_FILTER*uINV_ACDC_FFW_K_Slewed+uINV_ACDC_iAC_loop_out_PI+uINV_ACDC_iAC_loop_out_PR1+uINV_ACDC_iAC_loop_out_PR3+uINV_ACDC_iAC_loop_out_PR5+uINV_ACDC_iAC_loop_out_PR7+uINV_ACDC_iAC_loop_out_PR9)/(uINV_ACDC_vDC_sensed_FILTER+5);
            uINV_DCAC_update_Duty();

            if(uINV_DCAC_Counter_Disable_FFW<20000) // you disable the FFW after 1 s because it is only used during start up
            {
                uINV_DCAC_Counter_Disable_FFW++;
            }
            else
            {
                uINV_ACDC_FFW_K=0;
            }

     }

     //Oscilloscope
     uINV_DCAC_dVal1 = uINV_DCAC_iAC_Ref;
     uINV_DCAC_dVal2 = uINV_DCAC_vAC_sensed ;
     uINV_DCAC_dVal3 = uINV_ACDC_vAC_sensed_FILTER_1H;
     uINV_DCAC_dVal4 = uINV_DCAC_Index_Modulation;
     DLOG_4CH_run(&uINV_DCAC_dLog1);
 }

#pragma FUNC_ALWAYS_INLINE(uINV_DCAC_runISR1_lab6) //Run in close loop together with PLL toward the grid and voltage control loop.
 static inline void uINV_DCAC_runISR1_lab6()
 {

     SPLL_1PH_SOGI_run(&uINV_spll_1ph,
                       uINV_DCAC_vAC_sensed_pu);

     //Zero crossing of the grid
     if((uINV_ACDC_vAC_sensed_FILTER-uINV_ACDC_vAC_sensed_FILTER_K_1) >= 0.4f)
     {
         uINV_ACDC_vAC_sensed_FILTER_POS=1;
     }
     else
     {
         uINV_ACDC_vAC_sensed_FILTER_POS=0;
     }
     uINV_ACDC_vAC_sensed_FILTER_K_1=uINV_ACDC_vAC_sensed_FILTER;

     if(uINV_DCAC_clearPWMTrip == 1U)   // Start the machine!!!!!!!!!!!!!
     {
         // Wait for zero-crossing then clear trips and start power stage
         if(uINV_ACDC_vAC_sensed_FILTER < 2 && uINV_ACDC_vAC_sensed_FILTER >= -2 && uINV_ACDC_vAC_sensed_FILTER_POS==1)
               {
                     uINV_DCAC_POS=1;
                     uINV_DCAC_clearPWMTrip = 0;
                     uINV_DCAC_update_Duty_Flag=1;
                     EPWM_clearTripZoneFlag(DCAC_BASE, EPWM_TZ_INTERRUPT |EPWM_TZ_FLAG_OST);
                     uINV_DCAC_Counter_1=0;
               }
     }
     if(uINV_DCAC_update_Duty_Flag == 1U)
     {

         // Voltage Control Loops
             uINV_ACDC_vDC_loop_err=uINV_ACDC_vDC_Ref_Slewed-uINV_ACDC_vDC_sensed_NOTCH;
             uINV_ACDC_iAC_real_Ref_OUT_PI= -DCL_runPI_C2(&gv_ACDC_pi,uINV_ACDC_vDC_loop_err,0);
             uINV_DCAC_iAC_Ref =uINV_ACDC_iAC_real_Ref_OUT_PI*(uINV_spll_1ph.sine)+uINV_ACDC_iAC_imag_Ref*(uINV_spll_1ph.cosine);
         // Voltage Control Loops
         // Current Control Loop
            uINV_ACDC_iAC_loop_err=uINV_DCAC_iAC_Ref-uINV_ACDC_iAC_sensed;
            uINV_ACDC_iAC_loop_out_PI= DCL_runPI_C2(&gi_ACDC_pi,uINV_ACDC_iAC_loop_err,0);
            uINV_ACDC_iAC_loop_out_PR1= DCL_runDF22_C1(&gi_r1, uINV_ACDC_iAC_loop_err);
            uINV_ACDC_iAC_loop_out_PR3= DCL_runDF22_C1(&gi_r3, uINV_ACDC_iAC_loop_err);
            uINV_ACDC_iAC_loop_out_PR5= DCL_runDF22_C1(&gi_r5, uINV_ACDC_iAC_loop_err);
            uINV_ACDC_iAC_loop_out_PR7= DCL_runDF22_C1(&gi_r7, uINV_ACDC_iAC_loop_err);
            uINV_ACDC_iAC_loop_out_PR9= DCL_runDF22_C1(&gi_r9, uINV_ACDC_iAC_loop_err);
            uINV_DCAC_Index_Modulation = (uINV_ACDC_vAC_sensed_FILTER*uINV_ACDC_FFW_K_Slewed+uINV_ACDC_iAC_loop_out_PI+uINV_ACDC_iAC_loop_out_PR1+uINV_ACDC_iAC_loop_out_PR3+uINV_ACDC_iAC_loop_out_PR5+uINV_ACDC_iAC_loop_out_PR7+uINV_ACDC_iAC_loop_out_PR9)/(uINV_ACDC_vDC_sensed_FILTER+5);
         // Current Control Loop
            uINV_DCAC_update_Duty();

            if(uINV_DCAC_Counter_Disable_FFW<20000)
            {
                uINV_DCAC_Counter_Disable_FFW++;
            }
            else
            {
                uINV_ACDC_FFW_K=0;
                gv_ACDC_pi.Umax = 11;
                gv_ACDC_pi.Umin = -11;
            }
     }
     //Oscilloscope
     uINV_DCAC_dVal1 = uINV_DCAC_iAC_Ref;
     uINV_DCAC_dVal2 = uINV_DCAC_vAC_sensed ;
     uINV_DCAC_dVal3 = uINV_ACDC_vAC_sensed_FILTER_1H;
     uINV_DCAC_dVal4 = uINV_DCAC_Index_Modulation;
     DLOG_4CH_run(&uINV_DCAC_dLog1);
 }

#endif /* UINV_DCAC_H_ */
