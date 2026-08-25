//##############################################################################
//
// FILE:  uinv_dcdc.c
//
// TITLE: solution hardware abstraction layer,
//        This file consists of common variables and functions
//        for a particular hardware board, like functions to read current
//        and voltage signals on the board and functions to setup the
//        basic peripherals of the board
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

#include "driverlib.h"
#include "device.h"
#include "board.h"
#include "c2000ware_libraries.h"

#include "uinv_dcdc.h"

static void uINV_DCDC_init_boost();
static void uINV_DCDC_init_protection();
static void uINV_DCDC_init_llc();

volatile int uINV_DCDC_clearPWMTrip = 0;
volatile int uINV_DCDC_started = 0;


int32_t uINV_DCDC_Flag_Slew= 0;

#pragma RETAIN (uINV_DCDC_globals)
uINV_DCDC_GLOBALS_t uINV_DCDC_globals;

#pragma RETAIN (uINV_DCDC_b1)
#pragma RETAIN (uINV_DCDC_b2)
#pragma RETAIN (uINV_DCDC_b3)
#pragma RETAIN (uINV_DCDC_b4)
uINV_DCDC_BOOST_t uINV_DCDC_b1;
uINV_DCDC_BOOST_t uINV_DCDC_b2;
uINV_DCDC_BOOST_t uINV_DCDC_b3;
uINV_DCDC_BOOST_t uINV_DCDC_b4;

#pragma RETAIN (uINV_DCDC_b1_pi_current)
#pragma RETAIN (uINV_DCDC_b2_pi_current)
#pragma RETAIN (uINV_DCDC_b3_pi_current)
#pragma RETAIN (uINV_DCDC_b4_pi_current)
uINV_DCDC_PI uINV_DCDC_b1_pi_current;
uINV_DCDC_PI uINV_DCDC_b2_pi_current;
uINV_DCDC_PI uINV_DCDC_b3_pi_current;
uINV_DCDC_PI uINV_DCDC_b4_pi_current;

#pragma RETAIN (uINV_DCDC_b1_pi_voltage)
#pragma RETAIN (uINV_DCDC_b2_pi_voltage)
#pragma RETAIN (uINV_DCDC_b3_pi_voltage)
#pragma RETAIN (uINV_DCDC_b4_pi_voltage)
uINV_DCDC_PI uINV_DCDC_b1_pi_voltage;
uINV_DCDC_PI uINV_DCDC_b2_pi_voltage;
uINV_DCDC_PI uINV_DCDC_b3_pi_voltage;
uINV_DCDC_PI uINV_DCDC_b4_pi_voltage;

#pragma RETAIN (uINV_DCDC_b1_mppt_incc)
#pragma RETAIN (uINV_DCDC_b2_mppt_incc)
#pragma RETAIN (uINV_DCDC_b3_mppt_incc)
#pragma RETAIN (uINV_DCDC_b4_mppt_incc)
uinv_dcdc_mppt_t uINV_DCDC_b1_mppt_incc;
uinv_dcdc_mppt_t uINV_DCDC_b2_mppt_incc;
uinv_dcdc_mppt_t uINV_DCDC_b3_mppt_incc;
uinv_dcdc_mppt_t uINV_DCDC_b4_mppt_incc;

uint32_t uINV_DCDC_mppt_counter = 0;

//PRotection flags Demo
uint32_t uINV_DCDC_Current_Protection_1=0;
uint32_t uINV_DCDC_Current_Protection_2=0;
uint32_t uINV_DCDC_Current_Protection_3=0;
uint32_t uINV_DCDC_Current_Protection_4=0;

uint32_t uINV_DCDC_Voltage_Protection_1=0;
uint32_t uINV_DCDC_Voltage_Protection_2=0;
uint32_t uINV_DCDC_Voltage_Protection_3=0;
uint32_t uINV_DCDC_Voltage_Protection_4=0;
uint32_t uINV_DCDC_Overvoltage=0;


//////////////// LLC ////////////////
float32_t sr_coef = 0.9f;

#pragma RETAIN (uINV_DCDC_llc_freq_Hz)
uint32_t uINV_DCDC_llc_freq_Hz = uINV_DCDC_LLC_DEF_FREQ;
#pragma RETAIN (uINV_DCDC_llc_period_ticks)
uint32_t uINV_DCDC_llc_period_ticks = uINV_DCDC_LLC_DEF_PERIOD;
#pragma RETAIN (uINV_DCDC_llc_compare_ticks)
uint32_t uINV_DCDC_llc_compare_ticks = uINV_DCDC_LLC_DEF_COMPARE;

#pragma RETAIN (uINV_DCDC_boost_freq_Hz)
uint32_t uINV_DCDC_boost_freq_Hz = uINV_DCDC_BOOST_PWM_FREQ;
#pragma RETAIN (uINV_DCDC_boost_period_ticks)
uint32_t uINV_DCDC_boost_period_ticks = (CLK_FREQ/(uINV_DCDC_BOOST_PWM_FREQ*2));

#pragma RETAIN (uINV_DCDC_llc1_period_ticks)
int32_t uINV_DCDC_llc1_period_ticks = uINV_DCDC_LLC_DEF_PERIOD; // 50%
#pragma RETAIN (uINV_DCDC_llc1_cmp_ticks)
int32_t uINV_DCDC_llc1_cmp_ticks = uINV_DCDC_LLC_DEF_COMPARE; // 50%
#pragma RETAIN (uINV_DCDC_llc1_shift_ticks)
int32_t uINV_DCDC_llc1_shift_ticks = 0; // 0%

#pragma RETAIN (uINV_DCDC_llc2_period_ticks)
int32_t uINV_DCDC_llc2_period_ticks = uINV_DCDC_LLC_DEF_PERIOD; // 50%
#pragma RETAIN (uINV_DCDC_llc2_cmp_ticks)
int32_t uINV_DCDC_llc2_cmp_ticks = uINV_DCDC_LLC_DEF_COMPARE; // 50%
#pragma RETAIN (uINV_DCDC_llc2_shift_ticks)
int32_t uINV_DCDC_llc2_shift_ticks = uINV_DCDC_LLC_DEF_PERIOD; // 50%
#pragma RETAIN (uINV_DCDC_llc2_delay_ticks)
int32_t uINV_DCDC_llc2_delay_ticks = uINV_DCDC_LLC_DEF_DELAY;

#pragma RETAIN (uINV_DCDC_llc_deadtime_ticks)
int32_t uINV_DCDC_llc_deadtime_ticks = uINV_DCDC_LLC_DEF_DT;

#pragma RETAIN (uINV_DCDC_sr_period_ticks)
int32_t uINV_DCDC_sr_period_ticks = uINV_DCDC_LLC_DEF_PERIOD; // 50%
#pragma RETAIN (uINV_DCDC_sr_cmp_ticks)
int32_t uINV_DCDC_sr_cmp_ticks = uINV_DCDC_LLC_DEF_COMPARE;
#pragma RETAIN (uINV_DCDC_sr_shift_ticks)
int32_t uINV_DCDC_sr_shift_ticks = uINV_DCDC_LLC_DEF_PERIOD;
#pragma RETAIN (uINV_DCDC_sr_delay_ticks)
int32_t uINV_DCDC_sr_delay_ticks = uINV_DCDC_SR_DEF_DELAY;
#pragma RETAIN (uINV_DCDC_sr_deadtime_ticks)
int32_t uINV_DCDC_sr_deadtime_ticks = uINV_DCDC_SR_DEF_DT;
#pragma RETAIN (uINV_DCDC_sr_deadtime_ticks)
int32_t uINV_DCDC_sr_mintime_ticks = uINV_DCDC_SR_MIN_TICKS;
#pragma RETAIN (uINV_DCDC_sr_trip_delay_ticks)
int32_t uINV_DCDC_sr_trip_delay_ticks = uINV_DCDC_SR_TRIP_DELAY_TICKS;


//////////////// Misc ////////////////
#pragma RETAIN (uINV_DCDC_sr_enabled)
uint32_t uINV_DCDC_sr_enabled = 1;
#pragma RETAIN (uINV_DCDC_boost_enabled)
uint32_t uINV_DCDC_boost_enabled = 0;
#pragma RETAIN (uINV_DCDC_avg_enable)
uint32_t uINV_DCDC_avg_enable = 1;

#pragma RETAIN (uINV_DCDC_temp_deg)
float32_t uINV_DCDC_temp_deg = 0;
#pragma RETAIN (uINV_DCDC_temp_volt)
float32_t uINV_DCDC_temp_volt = 0;

// MACRO
#define DCDC_CLLLC_SS_STEPS (1024)

void uINV_DCDC_init() {
    uINV_DCDC_init_protection();
    uINV_DCDC_init_boost();
    uINV_DCDC_init_llc();

    GPIO_writePin(ISO_PWR_EN, uINV_DCDC_sr_enabled);
    DEVICE_DELAY_US(10000);
}



static void uINV_DCDC_init_protection() {
    //
    // Trigger event when DCBH is high
    //
    EPWM_setTripZoneDigitalCompareEventCondition(LLCSR_BASE,
                                                 EPWM_TZ_DC_OUTPUT_B1,
                                                 EPWM_TZ_EVENT_DCXH_HIGH);
    EPWM_setTripZoneDigitalCompareEventCondition(LLC1_BASE,
                                                 EPWM_TZ_DC_OUTPUT_B1,
                                                 EPWM_TZ_EVENT_DCXH_HIGH);
    EPWM_setTripZoneDigitalCompareEventCondition(LLC2_BASE,
                                                 EPWM_TZ_DC_OUTPUT_B1,
                                                 EPWM_TZ_EVENT_DCXH_HIGH);
    EPWM_setTripZoneDigitalCompareEventCondition(BOOST1_BASE,
                                                 EPWM_TZ_DC_OUTPUT_B1,
                                                 EPWM_TZ_EVENT_DCXH_HIGH);
    EPWM_setTripZoneDigitalCompareEventCondition(BOOST2_BASE,
                                                 EPWM_TZ_DC_OUTPUT_B1,
                                                 EPWM_TZ_EVENT_DCXH_HIGH);
    EPWM_setTripZoneDigitalCompareEventCondition(BOOST3_BASE,
                                                 EPWM_TZ_DC_OUTPUT_B1,
                                                 EPWM_TZ_EVENT_DCXH_HIGH);
    EPWM_setTripZoneDigitalCompareEventCondition(BOOST4_BASE,
                                                 EPWM_TZ_DC_OUTPUT_B1,
                                                 EPWM_TZ_EVENT_DCXH_HIGH);
    //
    // Configure DCBH to use TRIP4 as an input
    //
    EPWM_enableDigitalCompareTripCombinationInput(LLCSR_BASE,
                                                  EPWM_DC_COMBINATIONAL_TRIPIN4,
                                                  EPWM_DC_TYPE_DCBH);
    EPWM_enableDigitalCompareTripCombinationInput(LLC1_BASE,
                                                  EPWM_DC_COMBINATIONAL_TRIPIN4,
                                                  EPWM_DC_TYPE_DCBH);
    EPWM_enableDigitalCompareTripCombinationInput(LLC2_BASE,
                                                  EPWM_DC_COMBINATIONAL_TRIPIN4,
                                                  EPWM_DC_TYPE_DCBH);

    EPWM_enableDigitalCompareTripCombinationInput(BOOST1_BASE,
                                                  EPWM_DC_COMBINATIONAL_TRIPIN4,
                                                  EPWM_DC_TYPE_DCBH);
    EPWM_enableDigitalCompareTripCombinationInput(BOOST2_BASE,
                                                  EPWM_DC_COMBINATIONAL_TRIPIN4,
                                                  EPWM_DC_TYPE_DCBH);
    EPWM_enableDigitalCompareTripCombinationInput(BOOST3_BASE,
                                                  EPWM_DC_COMBINATIONAL_TRIPIN4,
                                                  EPWM_DC_TYPE_DCBH);
    EPWM_enableDigitalCompareTripCombinationInput(BOOST4_BASE,
                                                  EPWM_DC_COMBINATIONAL_TRIPIN4,
                                                  EPWM_DC_TYPE_DCBH);


    // Enable DCB as OST

    EPWM_enableTripZoneSignals(LLCSR_BASE, EPWM_TZ_SIGNAL_DCBEVT1);
    EPWM_enableTripZoneSignals(LLC1_BASE, EPWM_TZ_SIGNAL_DCBEVT1);
    EPWM_enableTripZoneSignals(LLC2_BASE, EPWM_TZ_SIGNAL_DCBEVT1);

    EPWM_enableTripZoneSignals(BOOST1_BASE, EPWM_TZ_SIGNAL_DCBEVT1);
    EPWM_enableTripZoneSignals(BOOST2_BASE, EPWM_TZ_SIGNAL_DCBEVT1);
    EPWM_enableTripZoneSignals(BOOST3_BASE, EPWM_TZ_SIGNAL_DCBEVT1);
    EPWM_enableTripZoneSignals(BOOST4_BASE, EPWM_TZ_SIGNAL_DCBEVT1);

    //
    // Configure the DCB path to be unfiltered and asynchronous
    //
    EPWM_setDigitalCompareEventSource(LLCSR_BASE,
                                      EPWM_DC_MODULE_B,
                                      EPWM_DC_EVENT_1,
                                      EPWM_DC_EVENT_SOURCE_ORIG_SIGNAL);
    EPWM_setDigitalCompareEventSource(LLC1_BASE,
                                      EPWM_DC_MODULE_B,
                                      EPWM_DC_EVENT_1,
                                      EPWM_DC_EVENT_SOURCE_ORIG_SIGNAL);
    EPWM_setDigitalCompareEventSource(LLC2_BASE,
                                      EPWM_DC_MODULE_B,
                                      EPWM_DC_EVENT_1,
                                      EPWM_DC_EVENT_SOURCE_ORIG_SIGNAL);

    EPWM_setDigitalCompareEventSource(BOOST1_BASE,
                                      EPWM_DC_MODULE_B,
                                      EPWM_DC_EVENT_1,
                                      EPWM_DC_EVENT_SOURCE_ORIG_SIGNAL);
    EPWM_setDigitalCompareEventSource(BOOST2_BASE,
                                      EPWM_DC_MODULE_B,
                                      EPWM_DC_EVENT_1,
                                      EPWM_DC_EVENT_SOURCE_ORIG_SIGNAL);
    EPWM_setDigitalCompareEventSource(BOOST3_BASE,
                                      EPWM_DC_MODULE_B,
                                      EPWM_DC_EVENT_1,
                                      EPWM_DC_EVENT_SOURCE_ORIG_SIGNAL);
    EPWM_setDigitalCompareEventSource(BOOST4_BASE,
                                      EPWM_DC_MODULE_B,
                                      EPWM_DC_EVENT_1,
                                      EPWM_DC_EVENT_SOURCE_ORIG_SIGNAL);

    //
    // Sync the ePWM time base clock
    //
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

    //
    // Configure TRIP4 to be CTRIP1H using the ePWM X-BAR
    //
    XBAR_setEPWMMuxConfig(XBAR_TRIP4, XBAR_EPWM_MUX00_CMPSS1_CTRIPH);
    XBAR_enableEPWMMux(XBAR_TRIP4, XBAR_MUX00);

    GPIO_writePin(LED1, 1);


}

static void uINV_DCDC_init_llc() {
}

extern volatile uint16_t uINV_DCAC_Counter_1;
void uINV_DCDC_fail() {
    EPWM_forceTripZoneEvent(LLCSR_BASE, EPWM_TZ_FORCE_EVENT_OST);
    EPWM_forceTripZoneEvent(LLC1_BASE, EPWM_TZ_FORCE_EVENT_OST);
    EPWM_forceTripZoneEvent(LLC2_BASE, EPWM_TZ_FORCE_EVENT_OST);

    EPWM_forceTripZoneEvent(BOOST1_BASE, EPWM_TZ_FORCE_EVENT_OST);
    EPWM_forceTripZoneEvent(BOOST2_BASE, EPWM_TZ_FORCE_EVENT_OST);
    EPWM_forceTripZoneEvent(BOOST3_BASE, EPWM_TZ_FORCE_EVENT_OST);
    EPWM_forceTripZoneEvent(BOOST4_BASE, EPWM_TZ_FORCE_EVENT_OST);

    uINV_ACDC_fault = 1;

    uINV_DCDC_b1.en = 0;
    uINV_DCDC_b2.en = 0;
    uINV_DCDC_b3.en = 0;
    uINV_DCDC_b4.en = 0;

    uINV_DCDC_b1.ss_factor = 0;
    uINV_DCDC_b2.ss_factor = 0;
    uINV_DCDC_b3.ss_factor = 0;
    uINV_DCDC_b4.ss_factor = 0;

    GPIO_writePin(LED1, 0);
    uINV_DCAC_Counter_1=1;
    uINV_DCDC_started = 0;
}


float32_t uINV_DCDC_term(int raw_ADC)
{
    static float32_t VBias = ANA_REF;
    static unsigned int ADC_BITS = 4096;

    // THRM calculations - 4th order polynomial regression
    int THRM_ADC = raw_ADC;

    float32_t THRM_A0 =-4.232811E+02;
    float32_t THRM_A1 = 4.728797E+02;
    float32_t THRM_A2 =-1.988841E+02;
    float32_t THRM_A3 = 4.869521E+01;
    float32_t THRM_A4 =-1.158754E+00;


    uINV_DCDC_temp_volt = (VBias/ADC_BITS) * THRM_ADC;
    uINV_DCDC_temp_deg = (THRM_A4 * powf( uINV_DCDC_temp_volt,4)) + (THRM_A3 * powf( uINV_DCDC_temp_volt,3)) + (THRM_A2 * powf( uINV_DCDC_temp_volt,2)) + (THRM_A1 *  uINV_DCDC_temp_volt) + THRM_A0;
    return uINV_DCDC_temp_deg;
}

void uINV_DCDC_test() {
    static int ss_factor_llc = 0;
    DEVICE_DELAY_US(300);

    if(uINV_DCDC_started) {
        ss_factor_llc++;
        if(ss_factor_llc > DCDC_CLLLC_SS_STEPS) {
            ss_factor_llc = DCDC_CLLLC_SS_STEPS;
        }
    } else {
        EPWM_forceTripZoneEvent(LLCSR_BASE, EPWM_TZ_FORCE_EVENT_OST);
        EPWM_forceTripZoneEvent(LLC1_BASE, EPWM_TZ_FORCE_EVENT_OST);
        EPWM_forceTripZoneEvent(LLC2_BASE, EPWM_TZ_FORCE_EVENT_OST);

        EPWM_forceTripZoneEvent(BOOST1_BASE, EPWM_TZ_FORCE_EVENT_OST);
        EPWM_forceTripZoneEvent(BOOST2_BASE, EPWM_TZ_FORCE_EVENT_OST);
        EPWM_forceTripZoneEvent(BOOST3_BASE, EPWM_TZ_FORCE_EVENT_OST);
        EPWM_forceTripZoneEvent(BOOST4_BASE, EPWM_TZ_FORCE_EVENT_OST);

        ss_factor_llc = 0;
    }

    if(ss_factor_llc == DCDC_CLLLC_SS_STEPS-40) {
        uINV_DCDC_b1.en = 1;
#if uINV_DCDC_BOOST1_EN
        EPWM_clearTripZoneFlag(BOOST1_BASE, EPWM_TZ_INTERRUPT |EPWM_TZ_FLAG_OST);
#endif
    }
    if(ss_factor_llc == DCDC_CLLLC_SS_STEPS-30) {
        uINV_DCDC_b2.en = 1;
#if uINV_DCDC_BOOST2_EN
        EPWM_clearTripZoneFlag(BOOST2_BASE, EPWM_TZ_INTERRUPT |EPWM_TZ_FLAG_OST);
#endif
    }
    if(ss_factor_llc == DCDC_CLLLC_SS_STEPS-20) {
        uINV_DCDC_b3.en = 1;
#if uINV_DCDC_BOOST3_EN
        EPWM_clearTripZoneFlag(BOOST3_BASE, EPWM_TZ_INTERRUPT |EPWM_TZ_FLAG_OST);
#endif
    }
    if(ss_factor_llc == DCDC_CLLLC_SS_STEPS-10) {
        uINV_DCDC_b4.en = 1;
#if uINV_DCDC_BOOST4_EN
        EPWM_clearTripZoneFlag(BOOST4_BASE, EPWM_TZ_INTERRUPT |EPWM_TZ_FLAG_OST);
#endif
    }
    if(ss_factor_llc == DCDC_CLLLC_SS_STEPS-1) {
#if uINV_DCDC_SR_EN
        EPWM_clearTripZoneFlag(LLCSR_BASE, EPWM_TZ_INTERRUPT |EPWM_TZ_FLAG_OST);
#endif
    }

    if(ss_factor_llc == DCDC_CLLLC_SS_STEPS) {
        uINV_DCDC_boost_enabled = 1;
        uINV_DCDC_b1_mppt_incc.mppt_enable = 1;
        uINV_DCDC_b2_mppt_incc.mppt_enable = 1;
        uINV_DCDC_b3_mppt_incc.mppt_enable = 1;
        uINV_DCDC_b4_mppt_incc.mppt_enable = 1;

        if(uINV_DCDC_b3.en) { // Enable channels step by step
            uINV_DCDC_b4.en = 1;
        }
        if(uINV_DCDC_b2.en) {
            uINV_DCDC_b3.en = 1;
        }
        if(uINV_DCDC_b1.en) {
            uINV_DCDC_b2.en = 1;
        }
        uINV_DCDC_b1.en = 1;
    }

    if(uINV_DCDC_boost_freq_Hz < 100000) {
        uINV_DCDC_boost_freq_Hz = 100000;
    }
    if(uINV_DCDC_boost_freq_Hz > 500000) {
        uINV_DCDC_boost_freq_Hz = 500000;
    }
    uINV_DCDC_boost_period_ticks = (CLK_FREQ/(uINV_DCDC_boost_freq_Hz*2));

    if(uINV_DCDC_llc_freq_Hz < 100000) {
        uINV_DCDC_llc_freq_Hz = 100000;
    }
    if(uINV_DCDC_llc_freq_Hz > 750000) {
        uINV_DCDC_llc_freq_Hz = 750000;
    }

    GPIO_writePin(ISO_PWR_EN, uINV_DCDC_sr_enabled);


    // Uptate LLC parameters
    uINV_DCDC_llc_period_ticks = (CLK_FREQ / uINV_DCDC_llc_freq_Hz) / 2;

    uINV_DCDC_llc1_period_ticks = uINV_DCDC_llc_period_ticks;
    uINV_DCDC_llc2_period_ticks = uINV_DCDC_llc_period_ticks;
    uINV_DCDC_sr_period_ticks = uINV_DCDC_llc_period_ticks;

    uINV_DCDC_llc1_cmp_ticks = uINV_DCDC_llc_period_ticks/2;
    uINV_DCDC_llc2_cmp_ticks = uINV_DCDC_llc_period_ticks/2;
    uINV_DCDC_llc2_shift_ticks =  ((uINV_DCDC_llc_period_ticks * ss_factor_llc) / DCDC_CLLLC_SS_STEPS);

    uINV_DCDC_sr_shift_ticks = (uINV_DCDC_llc_period_ticks/2);
    uINV_DCDC_sr_cmp_ticks = uINV_DCDC_llc2_shift_ticks;
    if(uINV_DCDC_sr_cmp_ticks > uINV_DCDC_llc_period_ticks-uINV_DCDC_sr_trip_delay_ticks) {
        uINV_DCDC_sr_cmp_ticks = uINV_DCDC_llc_period_ticks-uINV_DCDC_sr_trip_delay_ticks;
    }

    int32_t llc2_shift_reg = uINV_DCDC_llc2_shift_ticks + uINV_DCDC_llc2_delay_ticks;
    int32_t sr_shift_reg = uINV_DCDC_sr_shift_ticks - uINV_DCDC_sr_delay_ticks;

    if(llc2_shift_reg < 1) uINV_DCDC_llc2_shift_ticks=1; // Shift cannot be zero
    if(sr_shift_reg < 1) uINV_DCDC_sr_shift_ticks=1;

    EPWM_setTimeBasePeriod(LLC1_BASE, uINV_DCDC_llc1_period_ticks);
    EPWM_setTimeBasePeriod(LLC2_BASE, uINV_DCDC_llc2_period_ticks);
    EPWM_setTimeBasePeriod(LLCSR_BASE, uINV_DCDC_sr_period_ticks);

    EPWM_setCounterCompareValue(LLC1_BASE, EPWM_COUNTER_COMPARE_A, uINV_DCDC_llc1_cmp_ticks);
    EPWM_setCounterCompareValue(LLC2_BASE, EPWM_COUNTER_COMPARE_A, uINV_DCDC_llc2_cmp_ticks);
    EPWM_setCounterCompareValue(LLCSR_BASE, EPWM_COUNTER_COMPARE_A, uINV_DCDC_sr_cmp_ticks);
    EPWM_setCounterCompareValue(LLCSR_BASE, EPWM_COUNTER_COMPARE_B, uINV_DCDC_llc_period_ticks-uINV_DCDC_sr_cmp_ticks);

    if(llc2_shift_reg <= uINV_DCDC_llc_period_ticks) {
        EPWM_setPhaseShift(LLC2_BASE, llc2_shift_reg);
        EPWM_setCountModeAfterSync(LLC2_BASE, EPWM_COUNT_MODE_UP_AFTER_SYNC);
    } else {
        EPWM_setPhaseShift(LLC2_BASE, uINV_DCDC_llc_period_ticks - (llc2_shift_reg - uINV_DCDC_llc_period_ticks));
        EPWM_setCountModeAfterSync(LLC2_BASE, EPWM_COUNT_MODE_DOWN_AFTER_SYNC);
    }
    EPWM_setPhaseShift(LLCSR_BASE, sr_shift_reg);

    EPWM_setRisingEdgeDelayCount(LLCSR_BASE, uINV_DCDC_sr_deadtime_ticks);
    EPWM_setFallingEdgeDelayCount(LLCSR_BASE, uINV_DCDC_sr_deadtime_ticks);

    EPWM_setDigitalCompareWindowOffset(LLCSR_BASE, uINV_DCDC_sr_period_ticks-7); // -7 because of delay in trip logic
    EPWM_setDigitalCompareWindowLength(LLCSR_BASE, uINV_DCDC_sr_mintime_ticks);

    EPWM_setRisingEdgeDelayCount(LLC1_BASE, uINV_DCDC_llc_deadtime_ticks);
    EPWM_setFallingEdgeDelayCount(LLC1_BASE, uINV_DCDC_llc_deadtime_ticks);

    EPWM_setRisingEdgeDelayCount(LLC2_BASE, uINV_DCDC_llc_deadtime_ticks);
    EPWM_setFallingEdgeDelayCount(LLC2_BASE, uINV_DCDC_llc_deadtime_ticks);

    ///
    uINV_DCDC_globals.temp = ADC_readResult(SOC_TEMP_RESULT_BASE, SOC_TEMP);
    uint32_t temp_adc = uINV_DCDC_globals.temp;
    uINV_DCDC_term(temp_adc);
//    if(uINV_DCDC_temp_deg > 70) {
//        uINV_DCDC_fail();
//    }
}


