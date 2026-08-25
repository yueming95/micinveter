//##############################################################################
//
// FILE:  uinv_settings.h
//
// TITLE: Settings for the solution
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

#ifndef UINV_SETTINGS_H_
#define UINV_SETTINGS_H_


#define CLK_FREQ        (150000000)
#define ANA_REF         (3.3f)
#define ADC_SCALE       (4095)


// Lab has to be selected there.
// LAB 1 : PWM and ADC check
// LAB 2 : Run Boost Converters in open-loop
// LAB 3 : Run Boost Converters in close-loop
// LAB 4 : Run Boost Converters in close-loop plus MPPT algorithm
// LAB 5 : Run CLLLC Converter
// LAB 6 : Run CLLLC Converter plus boost Converters
// LAB 7 : Run DC/AC Converter in Open Loop across a resistive load
// LAB 8 : Run DC/DC together with current control with DC/AC Converter in Close Loop across a resistive load
// LAB 9 : Run DC/AC Converter in current control Loop across a resistive load
// LAB 10: Run DC/AC Converter in current control Loop across grid
// LAB 11: Run DC/DC together with current control with DC/AC Converter in current control Loop across grid
#define UINV_LAB (2)

#define UINV_DCAC_COMMS_ENABLED (1)
#define UINV_DCAC_COMMS_COUNTER (2000)


#endif /* UINV_SETTINGS_H_ */
