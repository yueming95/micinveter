//##############################################################################
//
// FILE:  uinv_dcdc_settings.h
//
// TITLE: Settings for the DC/DC converter stage
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

#ifndef UINV_DCDC_SETTINGS_H_
#define UINV_DCDC_SETTINGS_H_

#include "uinv_settings.h"

#define uINV_DCDC_BOOST_PWM_FREQ  (240000)


// Board settings
#define uINV_DCDC_VS_SCALE    ((float32_t)40.2f) // V/V
#define uINV_DCDC_CS_SCALE    ((float32_t)10.0f) // A/V

// Protection settings

#define uINV_DCDC_CONVERT_VOLTAGE(v) ((uint32_t) ((v/uINV_DCDC_VS_SCALE/ANA_REF)*ADC_SCALE))

#define uINV_DCDC_VIN_PROTECTION_MAX   uINV_DCDC_CONVERT_VOLTAGE((float32_t)65.0f)
#define uINV_DCDC_VIN_PROTECTION_MIN   uINV_DCDC_CONVERT_VOLTAGE((float32_t)0.0f)
#define uINV_DCDC_VS_PROTECTION_MAX    uINV_DCDC_CONVERT_VOLTAGE((float32_t)85.0f)
#define uINV_DCDC_VS_PROTECTION_MIN    uINV_DCDC_CONVERT_VOLTAGE((float32_t)0.0f)

#define uINV_DCDC_PI_SAMPLE_DIV ((uint32_t)6)
#define uINV_DCDC_PI       DCL_PI
#define uINV_DCDC_PI_RUN   DCL_runPI_C2
#define uINV_DCDC_PI_RESET DCL_resetPI
#define uINV_DCDC_PI_P_I ((float32_t)0.553f)
#define uINV_DCDC_PI_I_I ((float32_t)138.964f *(uINV_DCDC_PI_SAMPLE_DIV)/uINV_DCDC_BOOST_PWM_FREQ)

#define uINV_DCDC_PI_P_V ((float32_t)-6.2676f)
#define uINV_DCDC_PI_I_V ((float32_t)-4338.2318f *(uINV_DCDC_PI_SAMPLE_DIV)/uINV_DCDC_BOOST_PWM_FREQ)

#define uINV_DCDC_BOOST_FF_MIN ((float32_t)0.1f)
#define uINV_DCDC_BOOST_FF_MAX ((float32_t)0.7f)

#define uINV_DCDC_BOOST_PFM_TRANSITION ((float32_t)0.2f)
#define uINV_DCDC_BOOST_PWM_TRANSITION ((float32_t)0.25f)

#define uINV_DCDC_BOOST_DUTY_MIN ((float32_t)0.03f)

#define uINV_DCDC_VOLTAGE_FILTER_K      ((float32_t)0.21f)
#define uINV_DCDC_SS_K                  ((float32_t)0.00001f)

#define uINV_DCDC_MPPT_AVG_MULTIPLIER         ((float32_t)0.00314f) // 20Hz @ 40kHz sampling
#define uINV_DCDC_MPPT_DCDC_INCC_I_MAX_I        ((float32_t)0.7f)
#define uINV_DCDC_MPPT_DCDC_INCC_I_MIN_I        ((float32_t)0.0f)
#define uINV_DCDC_MPPT_DCDC_INCC_I_STEPSIZE     ((float32_t)0.005f)
#define uINV_DCDC_MPPT_COUNTER_THR ((uint32_t)10000)
#define uINV_DCDC_BOOST_PFM_TURN_ON_VOLT_US ((float32_t)6.0f)

// Boost settings
#define uINV_DCDC_BOOST_PWM_PERIOD (CLK_FREQ/(uINV_DCDC_BOOST_PWM_FREQ))

#define uINV_DCDC_BOOST1_EN (1)
#define uINV_DCDC_BOOST2_EN (1)
#define uINV_DCDC_BOOST3_EN (1)
#define uINV_DCDC_BOOST4_EN (1)

#define uINV_DCDC_BOOST1_CURRENT_CONTROL (1)
#define uINV_DCDC_BOOST2_CURRENT_CONTROL (1)
#define uINV_DCDC_BOOST3_CURRENT_CONTROL (1)
#define uINV_DCDC_BOOST4_CURRENT_CONTROL (1)

#define uINV_DCDC_BOOST1_VOLTAGE_CONTROL (0)
#define uINV_DCDC_BOOST2_VOLTAGE_CONTROL (0)
#define uINV_DCDC_BOOST3_VOLTAGE_CONTROL (0)
#define uINV_DCDC_BOOST4_VOLTAGE_CONTROL (0)

#define uINV_DCDC_BOOST1_MPPT (1)
#define uINV_DCDC_BOOST2_MPPT (1)
#define uINV_DCDC_BOOST3_MPPT (1)
#define uINV_DCDC_BOOST4_MPPT (1)

// LLC settings
#define uINV_DCDC_LLC_DEF_FREQ ((uint32_t)400000)
#define uINV_DCDC_LLC_DEF_PERIOD ((CLK_FREQ / uINV_DCDC_LLC_DEF_FREQ)/2)
#define uINV_DCDC_LLC_DEF_COMPARE (uINV_DCDC_LLC_DEF_PERIOD/2)
#define uINV_DCDC_LLC_DEF_DT ((uint32_t)5)
#define uINV_DCDC_LLC_DEF_DELAY ((uint32_t)2) // 2 ticks for PWM sync

#define uINV_DCDC_SR_DEF_DT ((uint32_t)12) // 100ns
#define uINV_DCDC_SR_MIN_TICKS ((uint32_t)40)
#define uINV_DCDC_SR_TRIP_DELAY_TICKS ((uint32_t)15) // 125ns
#define uINV_DCDC_SR_DEF_DELAY ((int32_t)2+1) // 2 ticks for PWM sync, 1 ticks for the isolator and driver delays compensation

#if ((UINV_LAB == 1) || (UINV_LAB == 5) || (UINV_LAB == 6) || (UINV_LAB == 8) || (UINV_LAB == 11))
#define uINV_DCDC_SR_EN (1)
#define uINV_DCDC_LLC_EN (1)
#endif


#endif /* UINV_DCDC_SETTINGS_H_ */
