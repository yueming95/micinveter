//##############################################################################
//
// FILE:  uinv_dcdc.h
//
// TITLE: Solution file for the DC/DC converter stage
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

#ifndef UINV_DCDC_H_
#define UINV_DCDC_H_

#include "stdint.h"

#include "uinv_dcdc_settings.h"
#include "uinv_dcdc_mppt.h"

#include "DCL.h"
#include "DCLF32.h"

// VARIABLES

// for Boosts


typedef struct uINV_DCDC_GLOBALS_t {
    int32_t vs;
    int32_t iref;
    int32_t temp;

    float32_t vs_pu;
    float32_t vs_pu_filtered;
    float32_t vs_real;
} uINV_DCDC_GLOBALS_t;

typedef struct uINV_DCDC_BOOST_t {
    int32_t cs1;
    int32_t cs2;
    int32_t vs;
    bool en;

    float32_t voltage_ref_pu;
    float32_t voltage_pu;
    float32_t voltage_pu_filtered;
    float32_t voltage_pu_avg;

    float32_t current_ref_pu;
    float32_t current_pu;
    float32_t current_pu_avg;
    float32_t current_ref_slewed;    //Demo Modification

    float32_t ff_duty;
    float32_t pi_duty;
    float32_t sr_duty;
    float32_t ss_factor;
    float32_t inactive_duty;

    uint32_t cmp;
    uint32_t sr_cmp;
} uINV_DCDC_BOOST_t;

extern volatile int uINV_DCDC_clearPWMTrip;
extern volatile int uINV_DCDC_started;

extern uINV_DCDC_GLOBALS_t uINV_DCDC_globals;
extern uINV_DCDC_BOOST_t uINV_DCDC_b1;
extern uINV_DCDC_BOOST_t uINV_DCDC_b2;
extern uINV_DCDC_BOOST_t uINV_DCDC_b3;
extern uINV_DCDC_BOOST_t uINV_DCDC_b4;

extern uINV_DCDC_PI uINV_DCDC_b1_pi_current;
extern uINV_DCDC_PI uINV_DCDC_b2_pi_current;
extern uINV_DCDC_PI uINV_DCDC_b3_pi_current;
extern uINV_DCDC_PI uINV_DCDC_b4_pi_current;

extern uINV_DCDC_PI uINV_DCDC_b1_pi_voltage;
extern uINV_DCDC_PI uINV_DCDC_b2_pi_voltage;
extern uINV_DCDC_PI uINV_DCDC_b3_pi_voltage;
extern uINV_DCDC_PI uINV_DCDC_b4_pi_voltage;

extern uinv_dcdc_mppt_t uINV_DCDC_b1_mppt_incc;
extern uinv_dcdc_mppt_t uINV_DCDC_b2_mppt_incc;
extern uinv_dcdc_mppt_t uINV_DCDC_b3_mppt_incc;
extern uinv_dcdc_mppt_t uINV_DCDC_b4_mppt_incc;

extern uint32_t uINV_DCDC_mppt_counter;

extern uint32_t uINV_DCDC_Current_Protection_1;
extern uint32_t uINV_DCDC_Current_Protection_2;
extern uint32_t uINV_DCDC_Current_Protection_3;
extern uint32_t uINV_DCDC_Current_Protection_4;

extern uint32_t uINV_DCDC_Voltage_Protection_1;
extern uint32_t uINV_DCDC_Voltage_Protection_2;
extern uint32_t uINV_DCDC_Voltage_Protection_3;
extern uint32_t uINV_DCDC_Voltage_Protection_4;

extern uint32_t uINV_DCDC_Overvoltage;

// for LLC
extern float32_t sr_coef;

extern uint32_t uINV_DCDC_llc_freq_Hz;
extern uint32_t uINV_DCDC_llc_period_ticks;
extern uint32_t uINV_DCDC_llc_compare_ticks;

extern uint32_t uINV_DCDC_boost_freq_Hz;
extern uint32_t uINV_DCDC_boost_period_ticks;

extern int32_t uINV_DCDC_llc1_period_ticks; // 50%
extern int32_t uINV_DCDC_llc1_cmp_ticks; // 50%
extern int32_t uINV_DCDC_llc1_shift_ticks; // 0%

extern int32_t uINV_DCDC_llc2_period_ticks; // 50%
extern int32_t uINV_DCDC_llc2_cmp_ticks; // 50%
extern int32_t uINV_DCDC_llc2_shift_ticks; // 50%

extern int32_t uINV_DCDC_llc_deadtime_ticks; // 33ns

extern int32_t uINV_DCDC_sr_period_ticks; // 50%
extern int32_t uINV_DCDC_sr_cmp_ticks;
extern int32_t uINV_DCDC_sr_shift_ticks;
extern int32_t uINV_DCDC_sr_delay_ticks;
extern int32_t uINV_DCDC_sr_deadtime_ticks;


extern int32_t uINV_DCDC_Flag_Slew;

extern int32_t uINV_ACDC_fault;



// MACRO
//Multiplier = (2*pi*f_cutoff)/f_sampling
#define EMAVG_MACRO(in, out, multiplier) out = ((in - out)*multiplier) + out;






// FUNCTIONS
void uINV_DCDC_init();
void uINV_DCDC_test();
void uINV_DCDC_fail();

static inline void
FAST_setCounterCompareValue_A(uint32_t base, uint16_t compCount)
{

    //
    // Write to COMPA or COMPB bits
    //
    HWREGH(base + EPWM_O_CMPA + 0x1U) = compCount;

}
static inline void
FAST_setCounterCompareValue_B(uint32_t base, uint16_t compCount)
{

    //
    // Write to COMPA or COMPB bits
    //
    HWREGH(base + EPWM_O_CMPA + EPWM_COUNTER_COMPARE_B + 0x1U) = compCount;

}
static inline void
FAST_setCounterCompareValue_C(uint32_t base, uint16_t compCount)
{

    //
    // Write to COMPA or COMPB bits
    //
    HWREGH(base + EPWM_O_CMPA + EPWM_COUNTER_COMPARE_C) = compCount;

}
static inline void
FAST_setCounterCompareValue_D(uint32_t base, uint16_t compCount)
{

    //
    // Write to COMPA or COMPB bits
    //
    HWREGH(base + EPWM_O_CMPA + EPWM_COUNTER_COMPARE_D) = compCount;

}

#pragma FUNC_ALWAYS_INLINE(uINV_DCDC_readCurrentAndVoltageSignals)
static inline void uINV_DCDC_readCurrentAndVoltageSignals()
{
    // Protections & Globals
    uINV_DCDC_globals.iref = ADC_readResult(SOC_IREF_RESULT_BASE, SOC_IREF);
    uINV_DCDC_globals.vs   = ADC_readResult(SOC_LLC_VS_RESULT_BASE, SOC_LLC_VS);
    uINV_DCDC_globals.vs_pu   = (float32_t) uINV_DCDC_globals.vs / (float32_t) ADC_SCALE;
    uINV_DCDC_globals.vs_pu_filtered += (uINV_DCDC_globals.vs_pu - uINV_DCDC_globals.vs_pu_filtered) * uINV_DCDC_VOLTAGE_FILTER_K;
    uINV_DCDC_globals.vs_real=uINV_DCDC_globals.vs_pu_filtered*uINV_DCDC_VS_SCALE*2;

    // Channels
#if uINV_DCDC_BOOST1_EN
    uINV_DCDC_b1.cs1 = ADC_readResult(SOC_BOOST1_CS1_RESULT_BASE, SOC_BOOST1_CS1);
    uINV_DCDC_b1.cs2 = ADC_readResult(SOC_BOOST1_CS2_RESULT_BASE, SOC_BOOST1_CS2);
    uINV_DCDC_b1.vs  = ADC_readResult(SOC_BOOST1_VS_RESULT_BASE , SOC_BOOST1_VS);
    uINV_DCDC_b1.voltage_pu = uINV_DCDC_b1.vs / ((float32_t)ADC_SCALE);
    uINV_DCDC_b1.current_pu = (uINV_DCDC_b1.cs1 + uINV_DCDC_b1.cs2 - 2*uINV_DCDC_globals.iref) / ((float32_t)ADC_SCALE);
    uINV_DCDC_b1.voltage_pu_filtered += (uINV_DCDC_b1.voltage_pu - uINV_DCDC_b1.voltage_pu_filtered) * uINV_DCDC_VOLTAGE_FILTER_K;
#endif
#if uINV_DCDC_BOOST2_EN
    uINV_DCDC_b2.cs1 = ADC_readResult(SOC_BOOST2_CS1_RESULT_BASE, SOC_BOOST2_CS1);
    uINV_DCDC_b2.cs2 = ADC_readResult(SOC_BOOST2_CS2_RESULT_BASE, SOC_BOOST2_CS2);
    uINV_DCDC_b2.vs  = ADC_readResult(SOC_BOOST2_VS_RESULT_BASE , SOC_BOOST2_VS);
    uINV_DCDC_b2.voltage_pu = uINV_DCDC_b2.vs / ((float32_t)ADC_SCALE);
    uINV_DCDC_b2.current_pu = (uINV_DCDC_b2.cs1 + uINV_DCDC_b2.cs2 - 2*uINV_DCDC_globals.iref) / ((float32_t)ADC_SCALE);
    uINV_DCDC_b2.voltage_pu_filtered += (uINV_DCDC_b2.voltage_pu - uINV_DCDC_b2.voltage_pu_filtered) * uINV_DCDC_VOLTAGE_FILTER_K;
#endif
#if uINV_DCDC_BOOST3_EN
    uINV_DCDC_b3.cs1 = ADC_readResult(SOC_BOOST3_CS1_RESULT_BASE, SOC_BOOST3_CS1);
    uINV_DCDC_b3.cs2 = ADC_readResult(SOC_BOOST3_CS2_RESULT_BASE, SOC_BOOST3_CS2);
    uINV_DCDC_b3.vs  = ADC_readResult(SOC_BOOST3_VS_RESULT_BASE , SOC_BOOST3_VS);
    uINV_DCDC_b3.voltage_pu = uINV_DCDC_b3.vs / ((float32_t)ADC_SCALE);
    uINV_DCDC_b3.current_pu = (uINV_DCDC_b3.cs1 + uINV_DCDC_b3.cs2 - 2*uINV_DCDC_globals.iref) / ((float32_t)ADC_SCALE);
    uINV_DCDC_b3.voltage_pu_filtered += (uINV_DCDC_b3.voltage_pu - uINV_DCDC_b3.voltage_pu_filtered) * uINV_DCDC_VOLTAGE_FILTER_K;
#endif
#if uINV_DCDC_BOOST4_EN
    uINV_DCDC_b4.cs1 = ADC_readResult(SOC_BOOST4_CS1_RESULT_BASE, SOC_BOOST4_CS1);
    uINV_DCDC_b4.cs2 = ADC_readResult(SOC_BOOST4_CS2_RESULT_BASE, SOC_BOOST4_CS2);
    uINV_DCDC_b4.vs  = ADC_readResult(SOC_BOOST4_VS_RESULT_BASE , SOC_BOOST4_VS);
    uINV_DCDC_b4.voltage_pu = uINV_DCDC_b4.vs / ((float32_t)ADC_SCALE);
    uINV_DCDC_b4.current_pu = (uINV_DCDC_b4.cs1 + uINV_DCDC_b4.cs2 - 2*uINV_DCDC_globals.iref) / ((float32_t)ADC_SCALE);
    uINV_DCDC_b4.voltage_pu_filtered += (uINV_DCDC_b4.voltage_pu - uINV_DCDC_b4.voltage_pu_filtered) * uINV_DCDC_VOLTAGE_FILTER_K;
#endif
}

static inline void uINV_DCDC_checkProtections() {
    if(uINV_DCDC_globals.vs > uINV_DCDC_VS_PROTECTION_MAX) {
        uINV_DCDC_fail();
        uINV_DCDC_Overvoltage=1;
    }

    if((uINV_DCDC_b1.current_pu > 0.75f) || (uINV_DCDC_b1.current_pu < -0.75f)){
        uINV_DCDC_fail();
        uINV_DCDC_Current_Protection_1=1;
    }
    if((uINV_DCDC_b2.current_pu > 0.75f) || (uINV_DCDC_b2.current_pu < -0.75f)){
        uINV_DCDC_fail();
        uINV_DCDC_Current_Protection_2=1;
    }
    if((uINV_DCDC_b3.current_pu > 0.75f) || (uINV_DCDC_b3.current_pu < -0.75f)){
        uINV_DCDC_fail();
        uINV_DCDC_Current_Protection_3=1;
    }
    if((uINV_DCDC_b4.current_pu > 0.75f) || (uINV_DCDC_b4.current_pu < -0.75f)){
        uINV_DCDC_fail();
        uINV_DCDC_Current_Protection_4=1;
    }


    //Demo Modification end

#if uINV_DCDC_BOOST1_EN
    if((uINV_DCDC_b1.vs > uINV_DCDC_VIN_PROTECTION_MAX) || (uINV_DCDC_b1.vs < uINV_DCDC_VIN_PROTECTION_MIN)) {
        uINV_DCDC_fail();
        uINV_DCDC_Voltage_Protection_1=1;
    }
#endif
#if uINV_DCDC_BOOST2_EN
    if((uINV_DCDC_b2.vs > uINV_DCDC_VIN_PROTECTION_MAX) || (uINV_DCDC_b2.vs < uINV_DCDC_VIN_PROTECTION_MIN)) {
        uINV_DCDC_fail();
        uINV_DCDC_Voltage_Protection_2=1;
    }
#endif
#if uINV_DCDC_BOOST3_EN
    if((uINV_DCDC_b3.vs > uINV_DCDC_VIN_PROTECTION_MAX) || (uINV_DCDC_b3.vs < uINV_DCDC_VIN_PROTECTION_MIN)) {
        uINV_DCDC_fail();
        uINV_DCDC_Voltage_Protection_3=1;
    }
#endif
#if uINV_DCDC_BOOST4_EN
    if((uINV_DCDC_b4.vs > uINV_DCDC_VIN_PROTECTION_MAX) || (uINV_DCDC_b4.vs < uINV_DCDC_VIN_PROTECTION_MIN)) {
        uINV_DCDC_fail();
        uINV_DCDC_Voltage_Protection_4=1;
    }
#endif


}

static init_boost_pi_i(uINV_DCDC_PI *pi) {
    uINV_DCDC_PI_RESET(pi);
    pi->Kp = uINV_DCDC_PI_P_I;
    pi->Ki = uINV_DCDC_PI_I_I;
    pi->i10 = 0.0f;
    pi->Umax = 100.0f;
    pi->Umin = -100.0f;
    pi->i6 = 1.0f;
    pi->i11 = 0.0f;
    pi->Imax = 1.0f;
    pi->Imin = -1.0f;
}

static init_boost_pi_v(uINV_DCDC_PI *pi) {
    uINV_DCDC_PI_RESET(pi);
    pi->Kp = uINV_DCDC_PI_P_V;
    pi->Ki = uINV_DCDC_PI_I_V;
    pi->i10 = 0.0f;
    pi->Umax = 1.0f;
    pi->Umin = 0.0f;
    pi->i6 = 1.0f;
    pi->i11 = 0.0f;
    pi->Imax = 1.0f;
    pi->Imin = -1.0f;
}

static void init_boost_mppt(uinv_dcdc_mppt_t *incc) {
    uinv_dcdc_mppt_init(incc);
    incc->MaxI = uINV_DCDC_MPPT_DCDC_INCC_I_MAX_I;
    incc->MinI = uINV_DCDC_MPPT_DCDC_INCC_I_MIN_I;
    incc->Stepsize = uINV_DCDC_MPPT_DCDC_INCC_I_STEPSIZE;
    incc->mppt_enable = 1;
}

static inline void uINV_DCDC_updateBoostsDuty()
{
#if uINV_DCDC_BOOST1_EN
    if(uINV_DCDC_b1.en) {
        HWREGH(BOOST1_BASE + EPWM_O_TBPRD) = uINV_DCDC_boost_period_ticks;
        FAST_setCounterCompareValue_A(BOOST1_BASE, uINV_DCDC_b1.cmp);
        FAST_setCounterCompareValue_B(BOOST1_BASE, uINV_DCDC_b1.sr_cmp);
    } else {
        EPWM_setTimeBasePeriod(BOOST1_BASE, uINV_DCDC_boost_period_ticks);
        FAST_setCounterCompareValue_A(BOOST1_BASE, 0);
        FAST_setCounterCompareValue_B(BOOST1_BASE, uINV_DCDC_boost_period_ticks);
        FAST_setCounterCompareValue_C(BOOST1_BASE, 0);
        FAST_setCounterCompareValue_D(BOOST1_BASE, 0);
    }
#endif
#if uINV_DCDC_BOOST2_EN
    if(uINV_DCDC_b2.en) {
        HWREGH(BOOST2_BASE + EPWM_O_TBPRD) = uINV_DCDC_boost_period_ticks;
        FAST_setCounterCompareValue_A(BOOST2_BASE, uINV_DCDC_b2.cmp);
        FAST_setCounterCompareValue_B(BOOST2_BASE, uINV_DCDC_b2.sr_cmp);
    } else {
        HWREGH(BOOST2_BASE + EPWM_O_TBPRD) = uINV_DCDC_boost_period_ticks;
        FAST_setCounterCompareValue_A(BOOST2_BASE, 0);
        FAST_setCounterCompareValue_B(BOOST2_BASE, uINV_DCDC_boost_period_ticks);
        FAST_setCounterCompareValue_C(BOOST2_BASE, 0);
        FAST_setCounterCompareValue_D(BOOST2_BASE, 0);
    }
#endif
#if uINV_DCDC_BOOST3_EN
    if(uINV_DCDC_b3.en) {
        HWREGH(BOOST3_BASE + EPWM_O_TBPRD) = uINV_DCDC_boost_period_ticks;
        FAST_setCounterCompareValue_A(BOOST3_BASE, uINV_DCDC_b3.cmp);
         FAST_setCounterCompareValue_B(BOOST3_BASE, uINV_DCDC_b3.sr_cmp);
    } else {
        HWREGH(BOOST3_BASE + EPWM_O_TBPRD) = uINV_DCDC_boost_period_ticks;
        FAST_setCounterCompareValue_A(BOOST3_BASE, 0);
        FAST_setCounterCompareValue_B(BOOST3_BASE, uINV_DCDC_boost_period_ticks);
        FAST_setCounterCompareValue_C(BOOST3_BASE, 0);
        FAST_setCounterCompareValue_D(BOOST3_BASE, 0);
    }
#endif
#if uINV_DCDC_BOOST4_EN
    if(uINV_DCDC_b4.en) {
        HWREGH(BOOST4_BASE + EPWM_O_TBPRD) = uINV_DCDC_boost_period_ticks;
        FAST_setCounterCompareValue_A(BOOST4_BASE, uINV_DCDC_b4.cmp);
        FAST_setCounterCompareValue_B(BOOST4_BASE, uINV_DCDC_b4.sr_cmp);
    } else {
        HWREGH(BOOST4_BASE + EPWM_O_TBPRD) = uINV_DCDC_boost_period_ticks;
        FAST_setCounterCompareValue_A(BOOST4_BASE, 0);
        FAST_setCounterCompareValue_B(BOOST4_BASE, uINV_DCDC_boost_period_ticks);
        FAST_setCounterCompareValue_C(BOOST4_BASE, 0);
        FAST_setCounterCompareValue_D(BOOST4_BASE, 0);
    }
#endif
}

static inline void uINV_DCDC_reset_boost() {
    uINV_DCDC_b1.en = 0;
    uINV_DCDC_b2.en = 0;
    uINV_DCDC_b3.en = 0;
    uINV_DCDC_b4.en = 0;

    uINV_DCDC_b1.ss_factor = 0;
    uINV_DCDC_b2.ss_factor = 0;
    uINV_DCDC_b3.ss_factor = 0;
    uINV_DCDC_b4.ss_factor = 0;

    uINV_DCDC_b1.cmp = 0;
    uINV_DCDC_b2.cmp = 0;
    uINV_DCDC_b3.cmp = 0;
    uINV_DCDC_b4.cmp = 0;
    uINV_DCDC_b1.sr_cmp = 0;
    uINV_DCDC_b2.sr_cmp = 0;
    uINV_DCDC_b3.sr_cmp = 0;
    uINV_DCDC_b4.sr_cmp = 0;

    init_boost_pi_i(&uINV_DCDC_b1_pi_current);
    init_boost_pi_i(&uINV_DCDC_b2_pi_current);
    init_boost_pi_i(&uINV_DCDC_b3_pi_current);
    init_boost_pi_i(&uINV_DCDC_b4_pi_current);

    init_boost_pi_v(&uINV_DCDC_b1_pi_voltage);
    init_boost_pi_v(&uINV_DCDC_b2_pi_voltage);
    init_boost_pi_v(&uINV_DCDC_b3_pi_voltage);
    init_boost_pi_v(&uINV_DCDC_b4_pi_voltage);

    init_boost_mppt(&uINV_DCDC_b1_mppt_incc);
    init_boost_mppt(&uINV_DCDC_b2_mppt_incc);
    init_boost_mppt(&uINV_DCDC_b3_mppt_incc);
    init_boost_mppt(&uINV_DCDC_b4_mppt_incc);

    uINV_DCDC_b1_mppt_incc.mppt_enable = 0;
    uINV_DCDC_b2_mppt_incc.mppt_enable = 0;
    uINV_DCDC_b3_mppt_incc.mppt_enable = 0;
    uINV_DCDC_b4_mppt_incc.mppt_enable = 0;

    uINV_DCDC_updateBoostsDuty();

    uINV_DCDC_b1.current_ref_pu = 0.0f;
    uINV_DCDC_b2.current_ref_pu = 0.0f;
    uINV_DCDC_b3.current_ref_pu = 0.0f;
    uINV_DCDC_b4.current_ref_pu = 0.0f;

    uINV_DCDC_b1.current_ref_slewed = 0.03f;
    uINV_DCDC_b2.current_ref_slewed = 0.03f;
    uINV_DCDC_b2.current_ref_slewed = 0.03f;
    uINV_DCDC_b4.current_ref_slewed = 0.00f;

    uINV_DCDC_b1.voltage_ref_pu = 0.0125f;
    uINV_DCDC_b2.voltage_ref_pu = 0.0125f;
    uINV_DCDC_b3.voltage_ref_pu = 0.0125f;
    uINV_DCDC_b4.voltage_ref_pu = 0.0125f;


    uINV_DCDC_b1.inactive_duty = uINV_DCDC_b2.inactive_duty = uINV_DCDC_b2.inactive_duty = uINV_DCDC_b2.inactive_duty = 5.0f;
}

static inline void uINV_DCDC_init_boost() {

    EPWM_forceTripZoneEvent(LLCSR_BASE, EPWM_TZ_FORCE_EVENT_OST);
    EPWM_forceTripZoneEvent(LLC1_BASE, EPWM_TZ_FORCE_EVENT_OST);
    EPWM_forceTripZoneEvent(LLC2_BASE, EPWM_TZ_FORCE_EVENT_OST);

    EPWM_forceTripZoneEvent(BOOST1_BASE, EPWM_TZ_FORCE_EVENT_OST);
    EPWM_forceTripZoneEvent(BOOST2_BASE, EPWM_TZ_FORCE_EVENT_OST);
    EPWM_forceTripZoneEvent(BOOST3_BASE, EPWM_TZ_FORCE_EVENT_OST);
    EPWM_forceTripZoneEvent(BOOST4_BASE, EPWM_TZ_FORCE_EVENT_OST);


    EPWM_setTimeBasePeriod(BOOST1_BASE, uINV_DCDC_BOOST_PWM_PERIOD);
    EPWM_setTimeBasePeriod(BOOST2_BASE, uINV_DCDC_BOOST_PWM_PERIOD);
    EPWM_setTimeBasePeriod(BOOST3_BASE, uINV_DCDC_BOOST_PWM_PERIOD);
    EPWM_setTimeBasePeriod(BOOST4_BASE, uINV_DCDC_BOOST_PWM_PERIOD);

    uINV_DCDC_boost_period_ticks = (CLK_FREQ/(uINV_DCDC_boost_freq_Hz*2));
    EPWM_setPhaseShift(BOOST1_BASE,                                 2);// Zero        and Up   counting
    EPWM_setPhaseShift(BOOST2_BASE, uINV_DCDC_boost_period_ticks/2 +2);// Half period and Up   counting
    EPWM_setPhaseShift(BOOST3_BASE, uINV_DCDC_boost_period_ticks   -2);// Full period and Down counting
    EPWM_setPhaseShift(BOOST4_BASE, uINV_DCDC_boost_period_ticks/2 -2);// Half period and Down counting

    EPWM_setOneShotSyncOutTrigger(DCAC_BASE, EPWM_OSHT_SYNC_OUT_TRIG_SYNC);

    uINV_DCDC_reset_boost();
}



static inline void uINV_DCDC_releaseTrip() {
    if(uINV_DCDC_clearPWMTrip == 1U)
    {

        uINV_DCDC_reset_boost();

        uINV_DCDC_clearPWMTrip = 0;
        uINV_DCDC_started = 1;

#if uINV_DCDC_LLC_EN
        EPWM_clearTripZoneFlag(LLC1_BASE, EPWM_TZ_INTERRUPT |EPWM_TZ_FLAG_OST);
        EPWM_clearTripZoneFlag(LLC2_BASE, EPWM_TZ_INTERRUPT |EPWM_TZ_FLAG_OST);
#endif
#if uINV_DCDC_SR_EN
        EPWM_clearTripZoneFlag(LLCSR_BASE, EPWM_TZ_INTERRUPT |EPWM_TZ_FLAG_OST);
#endif
    }
}


#pragma FUNC_ALWAYS_INLINE(uINV_DCDC_lab1)  //PWM and ADC check
inline void uINV_DCDC_lab1()
{
#if uINV_DCDC_BOOST1_EN
    uINV_DCDC_b1.cmp = uINV_DCDC_boost_period_ticks * uINV_DCDC_b1.ff_duty;
#endif
#if uINV_DCDC_BOOST2_EN
    uINV_DCDC_b2.cmp = uINV_DCDC_boost_period_ticks * uINV_DCDC_b2.ff_duty;
#endif
#if uINV_DCDC_BOOST3_EN
    uINV_DCDC_b3.cmp = uINV_DCDC_boost_period_ticks * uINV_DCDC_b3.ff_duty;
#endif
#if uINV_DCDC_BOOST4_EN
    uINV_DCDC_b4.cmp = uINV_DCDC_boost_period_ticks * uINV_DCDC_b4.ff_duty;
#endif
}

#pragma FUNC_ALWAYS_INLINE(uINV_DCDC_lab2)  //Run boosts in open loop
inline void uINV_DCDC_lab2()
{
#if uINV_DCDC_BOOST1_EN
    if(uINV_DCDC_b1.ff_duty > 0.7f){     //Limiting duty-cycle on inputs to 0 to 0.7
        uINV_DCDC_b1.ff_duty= 0.7f;
    }
    else if(uINV_DCDC_b1.ff_duty < 0){
        uINV_DCDC_b1.ff_duty = 0;
    }
    uINV_DCDC_b1.cmp = uINV_DCDC_boost_period_ticks * uINV_DCDC_b1.ff_duty;
    uINV_DCDC_b1.sr_cmp = (uINV_DCDC_boost_period_ticks * uINV_DCDC_b1.ff_duty)+1;
#endif
#if uINV_DCDC_BOOST2_EN
    if(uINV_DCDC_b2.ff_duty > 0.7f){     //Limiting duty-cycle on inputs to 0 to 0.7
        uINV_DCDC_b2.ff_duty= 0.7f;
    }
    else if(uINV_DCDC_b2.ff_duty < 0){
        uINV_DCDC_b2.ff_duty = 0;
    }
    uINV_DCDC_b2.cmp = uINV_DCDC_boost_period_ticks * uINV_DCDC_b2.ff_duty;
    uINV_DCDC_b2.sr_cmp = (uINV_DCDC_boost_period_ticks * uINV_DCDC_b2.ff_duty)+1;
#endif
#if uINV_DCDC_BOOST3_EN
    if(uINV_DCDC_b3.ff_duty > 0.7f){     //Limiting duty-cycle on inputs to 0 to 0.7
        uINV_DCDC_b3.ff_duty= 0.7f;
    }
    else if(uINV_DCDC_b3.ff_duty < 0){
        uINV_DCDC_b3.ff_duty = 0;
    }
    uINV_DCDC_b3.cmp = uINV_DCDC_boost_period_ticks * uINV_DCDC_b3.ff_duty;
    uINV_DCDC_b3.sr_cmp = (uINV_DCDC_boost_period_ticks * uINV_DCDC_b3.ff_duty)+1;
#endif
#if uINV_DCDC_BOOST4_EN
    if(uINV_DCDC_b4.ff_duty > 0.7f){     //Limiting duty-cycle on inputs to 0 to 0.7
        uINV_DCDC_b4.ff_duty= 0.7f;
    }
    else if(uINV_DCDC_b4.ff_duty < 0){
        uINV_DCDC_b4.ff_duty = 0;
    }
    uINV_DCDC_b4.cmp = uINV_DCDC_boost_period_ticks * uINV_DCDC_b4.ff_duty;
    uINV_DCDC_b4.sr_cmp = (uINV_DCDC_boost_period_ticks * uINV_DCDC_b4.ff_duty)+1;
#endif
}


#pragma FUNC_ALWAYS_INLINE(uINV_DCDC_lab3)  // Run boosts with current control
inline void uINV_DCDC_lab3()
{
    if(uINV_DCDC_started) {
#if uINV_DCDC_BOOST1_EN
        if(uINV_DCDC_b1.en) {
            uINV_DCDC_b1.pi_duty = uINV_DCDC_PI_RUN(&uINV_DCDC_b1_pi_current,uINV_DCDC_b1.current_ref_slewed*(20.0f) , uINV_DCDC_b1.current_pu*(20.0f));
            uINV_DCDC_b1.pi_duty = (uINV_DCDC_b1.pi_duty) / ((uINV_DCDC_globals.vs_pu_filtered + 0.02f) * 132.66f);
            uINV_DCDC_b1.ff_duty = 1.0f - (uINV_DCDC_b1.voltage_pu_filtered / (uINV_DCDC_globals.vs_pu_filtered + 0.02f));

            //Demo Modification end
            float32_t duty = uINV_DCDC_b1.pi_duty + uINV_DCDC_b1.ff_duty;
            duty *= uINV_DCDC_b1.ss_factor;
            if(duty < uINV_DCDC_BOOST_DUTY_MIN) {
                duty = uINV_DCDC_BOOST_DUTY_MIN;
            }
            float32_t sr_duty = 1.0f - duty;

            duty *= uINV_DCDC_b1.ss_factor;
            sr_duty *= uINV_DCDC_b1.ss_factor;
            if(uINV_DCDC_b1.ss_factor < 1.0f) {
                uINV_DCDC_b1.ss_factor += uINV_DCDC_SS_K;
            }


            uINV_DCDC_b1.cmp = uINV_DCDC_boost_period_ticks * (duty);
            uINV_DCDC_b1.sr_cmp = uINV_DCDC_boost_period_ticks * (1.0f - sr_duty);
            if(uINV_DCDC_b1.sr_cmp < uINV_DCDC_b1.cmp) {
                uINV_DCDC_b1.sr_cmp = uINV_DCDC_b1.cmp;
            }
        }
#endif
#if uINV_DCDC_BOOST2_EN
        if(uINV_DCDC_b2.en) {
            uINV_DCDC_b2.pi_duty = uINV_DCDC_PI_RUN(&uINV_DCDC_b2_pi_current,uINV_DCDC_b2.current_ref_slewed*(20.0f) , uINV_DCDC_b2.current_pu*(20.0f));
            uINV_DCDC_b2.pi_duty = (uINV_DCDC_b2.pi_duty) / ((uINV_DCDC_globals.vs_pu_filtered + 0.02f) * 132.66f);
            uINV_DCDC_b2.ff_duty = 1.0f - (uINV_DCDC_b2.voltage_pu_filtered / (uINV_DCDC_globals.vs_pu_filtered + 0.02f));

            float32_t duty = uINV_DCDC_b2.pi_duty + uINV_DCDC_b2.ff_duty;
            duty *= uINV_DCDC_b2.ss_factor;
            if(duty < uINV_DCDC_BOOST_DUTY_MIN) {
                duty = uINV_DCDC_BOOST_DUTY_MIN;
            }
            float32_t sr_duty = 1.0f - duty;

            duty *= uINV_DCDC_b2.ss_factor;
            sr_duty *= uINV_DCDC_b2.ss_factor;
            if(uINV_DCDC_b2.ss_factor < 1.0f) {
                uINV_DCDC_b2.ss_factor += uINV_DCDC_SS_K;
            }


            uINV_DCDC_b2.cmp = uINV_DCDC_boost_period_ticks * (duty);
            uINV_DCDC_b2.sr_cmp = uINV_DCDC_boost_period_ticks * (1.0f - sr_duty);
            if(uINV_DCDC_b2.sr_cmp < uINV_DCDC_b2.cmp) {
                uINV_DCDC_b2.sr_cmp = uINV_DCDC_b2.cmp;
            }

        }
#endif
#if uINV_DCDC_BOOST3_EN
        if(uINV_DCDC_b3.en) {
            uINV_DCDC_b3.pi_duty = uINV_DCDC_PI_RUN(&uINV_DCDC_b3_pi_current,uINV_DCDC_b3.current_ref_slewed*(20.0f) , uINV_DCDC_b3.current_pu*(20.0f));
            uINV_DCDC_b3.pi_duty = (uINV_DCDC_b3.pi_duty) / ((uINV_DCDC_globals.vs_pu_filtered + 0.02f) * 132.66f);
            uINV_DCDC_b3.ff_duty = 1.0f - (uINV_DCDC_b3.voltage_pu_filtered / (uINV_DCDC_globals.vs_pu_filtered + 0.02f));

            float32_t duty = uINV_DCDC_b3.pi_duty + uINV_DCDC_b3.ff_duty;
            duty *= uINV_DCDC_b3.ss_factor;
            if(duty < uINV_DCDC_BOOST_DUTY_MIN) {
                duty = uINV_DCDC_BOOST_DUTY_MIN;
            }
            float32_t sr_duty = 1.0f - duty;

            duty *= uINV_DCDC_b3.ss_factor;
            sr_duty *= uINV_DCDC_b3.ss_factor;
            if(uINV_DCDC_b3.ss_factor < 1.0f) {
                uINV_DCDC_b3.ss_factor += uINV_DCDC_SS_K;
            }


            uINV_DCDC_b3.cmp = uINV_DCDC_boost_period_ticks * (duty);
            uINV_DCDC_b3.sr_cmp = uINV_DCDC_boost_period_ticks * (1.0f - sr_duty);
            if(uINV_DCDC_b3.sr_cmp < uINV_DCDC_b3.cmp) {
                uINV_DCDC_b3.sr_cmp = uINV_DCDC_b3.cmp;
            }

        }
#endif
#if uINV_DCDC_BOOST4_EN
        if(uINV_DCDC_b4.en) {
            uINV_DCDC_b4.pi_duty = uINV_DCDC_PI_RUN(&uINV_DCDC_b4_pi_current,uINV_DCDC_b4.current_ref_slewed*(20.0f) , uINV_DCDC_b4.current_pu*(20.0f));
            uINV_DCDC_b4.pi_duty = (uINV_DCDC_b4.pi_duty) / ((uINV_DCDC_globals.vs_pu_filtered + 0.02f) * 132.66f);
            uINV_DCDC_b4.ff_duty = 1.0f - (uINV_DCDC_b4.voltage_pu_filtered / (uINV_DCDC_globals.vs_pu_filtered + 0.02f));

            float32_t duty = uINV_DCDC_b4.pi_duty + uINV_DCDC_b4.ff_duty;
            duty *= uINV_DCDC_b4.ss_factor;
            if(duty < uINV_DCDC_BOOST_DUTY_MIN) {
                duty = uINV_DCDC_BOOST_DUTY_MIN;
            }
            float32_t sr_duty = 1.0f - duty;

            duty *= uINV_DCDC_b4.ss_factor;
            sr_duty *= uINV_DCDC_b4.ss_factor;
            if(uINV_DCDC_b4.ss_factor < 1.0f) {
                uINV_DCDC_b4.ss_factor += uINV_DCDC_SS_K;
            }


            uINV_DCDC_b4.cmp = uINV_DCDC_boost_period_ticks * (duty);
            uINV_DCDC_b4.sr_cmp = uINV_DCDC_boost_period_ticks * (1.0f - sr_duty);
            if(uINV_DCDC_b4.sr_cmp < uINV_DCDC_b4.cmp) {
                uINV_DCDC_b4.sr_cmp = uINV_DCDC_b4.cmp;
            }
        }
#endif
    }
}

#pragma FUNC_ALWAYS_INLINE(uINV_DCDC_lab4)  // Run boosts with MPPT // TODO: Not tested
inline void uINV_DCDC_lab4()
{
    uINV_DCDC_lab3(); // Current control

#if uINV_DCDC_BOOST1_EN
    EMAVG_MACRO(uINV_DCDC_b1.voltage_pu, uINV_DCDC_b1.voltage_pu_avg, uINV_DCDC_MPPT_AVG_MULTIPLIER);
    EMAVG_MACRO(uINV_DCDC_b1.current_pu, uINV_DCDC_b1.current_pu_avg, uINV_DCDC_MPPT_AVG_MULTIPLIER);
#endif
#if uINV_DCDC_BOOST2_EN
    EMAVG_MACRO(uINV_DCDC_b2.voltage_pu, uINV_DCDC_b2.voltage_pu_avg, uINV_DCDC_MPPT_AVG_MULTIPLIER);
    EMAVG_MACRO(uINV_DCDC_b2.current_pu, uINV_DCDC_b2.current_pu_avg, uINV_DCDC_MPPT_AVG_MULTIPLIER);
#endif
#if uINV_DCDC_BOOST3_EN
    EMAVG_MACRO(uINV_DCDC_b3.voltage_pu, uINV_DCDC_b3.voltage_pu_avg, uINV_DCDC_MPPT_AVG_MULTIPLIER);
    EMAVG_MACRO(uINV_DCDC_b3.current_pu, uINV_DCDC_b3.current_pu_avg, uINV_DCDC_MPPT_AVG_MULTIPLIER);
#endif
#if uINV_DCDC_BOOST4_EN
    EMAVG_MACRO(uINV_DCDC_b4.voltage_pu, uINV_DCDC_b4.voltage_pu_avg, uINV_DCDC_MPPT_AVG_MULTIPLIER);
    EMAVG_MACRO(uINV_DCDC_b4.current_pu, uINV_DCDC_b4.current_pu_avg, uINV_DCDC_MPPT_AVG_MULTIPLIER);
#endif

    uINV_DCDC_mppt_counter++;
#if uINV_DCDC_BOOST1_EN
    if(uINV_DCDC_mppt_counter == uINV_DCDC_MPPT_COUNTER_THR-1) {
        uINV_DCDC_b1_mppt_incc.Ipv = uINV_DCDC_b1.current_pu_avg;
        uINV_DCDC_b1_mppt_incc.Vpv = uINV_DCDC_b1.voltage_pu_avg;
        uinv_dcdc_mppt_run(&uINV_DCDC_b1_mppt_incc);
        uINV_DCDC_b1.current_ref_pu = uINV_DCDC_b1_mppt_incc.ImppOut;
    }
#endif
#if uINV_DCDC_BOOST2_EN
    if(uINV_DCDC_mppt_counter == uINV_DCDC_MPPT_COUNTER_THR-2) {
        uINV_DCDC_b2_mppt_incc.Ipv = uINV_DCDC_b2.current_pu_avg;
        uINV_DCDC_b2_mppt_incc.Vpv = uINV_DCDC_b2.voltage_pu_avg;
        uinv_dcdc_mppt_run(&uINV_DCDC_b2_mppt_incc);
        uINV_DCDC_b2.current_ref_pu = uINV_DCDC_b2_mppt_incc.ImppOut;
    }
#endif
#if uINV_DCDC_BOOST3_EN
    if(uINV_DCDC_mppt_counter == uINV_DCDC_MPPT_COUNTER_THR-3) {
        uINV_DCDC_b3_mppt_incc.Ipv = uINV_DCDC_b3.current_pu_avg;
        uINV_DCDC_b3_mppt_incc.Vpv = uINV_DCDC_b3.voltage_pu_avg;
        uinv_dcdc_mppt_run(&uINV_DCDC_b3_mppt_incc);
        uINV_DCDC_b3.current_ref_pu = uINV_DCDC_b3_mppt_incc.ImppOut;
    }
#endif
#if uINV_DCDC_BOOST4_EN
    if(uINV_DCDC_mppt_counter == uINV_DCDC_MPPT_COUNTER_THR-4) {
        uINV_DCDC_b4_mppt_incc.Ipv = uINV_DCDC_b4.current_pu_avg;
        uINV_DCDC_b4_mppt_incc.Vpv = uINV_DCDC_b4.voltage_pu_avg;
        uinv_dcdc_mppt_run(&uINV_DCDC_b4_mppt_incc);
        uINV_DCDC_b4.current_ref_pu = uINV_DCDC_b4_mppt_incc.ImppOut;
    }
#endif
    if(uINV_DCDC_mppt_counter == uINV_DCDC_MPPT_COUNTER_THR) {
        uINV_DCDC_mppt_counter = 0;
    }
}


#pragma FUNC_ALWAYS_INLINE(uINV_DCDC_lab5)  // Run CLLLC Converter
inline void uINV_DCDC_lab5()
{
    // Boosts disabled in this mode
    EPWM_forceTripZoneEvent(BOOST1_BASE, EPWM_TZ_FORCE_EVENT_OST);
    EPWM_forceTripZoneEvent(BOOST2_BASE, EPWM_TZ_FORCE_EVENT_OST);
    EPWM_forceTripZoneEvent(BOOST3_BASE, EPWM_TZ_FORCE_EVENT_OST);
    EPWM_forceTripZoneEvent(BOOST4_BASE, EPWM_TZ_FORCE_EVENT_OST);
}

#pragma FUNC_ALWAYS_INLINE(uINV_DCDC_lab6)  // Run CLLLC Converter plus boost Converters
inline void uINV_DCDC_lab6()
{
    // The same boost control as in lab3
    uINV_DCDC_lab3();
}

#pragma FUNC_ALWAYS_INLINE(uINV_DCDC_lab7)  // Run DC/AC Converter in Open Loop across a resistive load
inline void uINV_DCDC_lab7()
{
    // Boosts disabled in this lab
    EPWM_forceTripZoneEvent(BOOST1_BASE, EPWM_TZ_FORCE_EVENT_OST);
    EPWM_forceTripZoneEvent(BOOST2_BASE, EPWM_TZ_FORCE_EVENT_OST);
    EPWM_forceTripZoneEvent(BOOST3_BASE, EPWM_TZ_FORCE_EVENT_OST);
    EPWM_forceTripZoneEvent(BOOST4_BASE, EPWM_TZ_FORCE_EVENT_OST);
}

#pragma FUNC_ALWAYS_INLINE(uINV_DCDC_lab8)  // Run DC/DC together with current control with DC/AC Converter in Open Loop across a resistive load
inline void uINV_DCDC_lab8()
{
    uINV_DCDC_lab3();
}

#pragma FUNC_ALWAYS_INLINE(uINV_DCDC_lab9)  // Run current control loop across external resistor. (Boost and CLLLC are OFF)
inline void uINV_DCDC_lab9()
{
    // Boosts disabled in this lab
    EPWM_forceTripZoneEvent(BOOST1_BASE, EPWM_TZ_FORCE_EVENT_OST);
    EPWM_forceTripZoneEvent(BOOST2_BASE, EPWM_TZ_FORCE_EVENT_OST);
    EPWM_forceTripZoneEvent(BOOST3_BASE, EPWM_TZ_FORCE_EVENT_OST);
    EPWM_forceTripZoneEvent(BOOST4_BASE, EPWM_TZ_FORCE_EVENT_OST);
}

#pragma FUNC_ALWAYS_INLINE(uINV_DCDC_lab10)  //Run in close loop together with PLL toward the grid. No DC link voltage control (Boost and CLLLC are OFF)
inline void uINV_DCDC_lab10()
{
    // Boosts disabled in this lab
    EPWM_forceTripZoneEvent(BOOST1_BASE, EPWM_TZ_FORCE_EVENT_OST);
    EPWM_forceTripZoneEvent(BOOST2_BASE, EPWM_TZ_FORCE_EVENT_OST);
    EPWM_forceTripZoneEvent(BOOST3_BASE, EPWM_TZ_FORCE_EVENT_OST);
    EPWM_forceTripZoneEvent(BOOST4_BASE, EPWM_TZ_FORCE_EVENT_OST);
}

#pragma FUNC_ALWAYS_INLINE(uINV_DCDC_lab11)  //Run in close loop together with PLL toward the grid and voltage control loop. (Boost and CLLLC are ON)
inline void uINV_DCDC_lab11()
{
    uINV_DCDC_lab3();
}



#endif /* UINV_DCDC_H_ */
