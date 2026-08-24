//##############################################################################
//
// FILE:  uinv_dcac_settings.h
//
// TITLE: Settings for the DC/AC converter stage
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





#ifndef UINV_DCAC_SETTINGS_H_
#define UINV_DCAC_SETTINGS_H_


#include "uinv_settings.h"

#define uINV_DCAC_BOOST_PWM_FREQ  (119808)

//Virtual Oscilloscope
#define uINV_DCAC_DLOG_SIZE      (100)
#define uINV_ACDC_DLOG_TRIGGER   ((float32_t)0.01)
#define uINV_ACDC_DLOG_SCALE     (10)

#define uINV_DCAC_DLOG_SIZE_OSC2      (800)
#define uINV_ACDC_DLOG_SCALE_OSC2     (8)

#define uINV_ACDC_DLOG_TOTAL_SAMPLES     (uINV_DCAC_DLOG_SIZE_OSC2*uINV_ACDC_DLOG_SCALE_OSC2+1)
//

// Temperature Measurements
#define uINV_ACDC_GaN_Temp_Scaling      (4000)   // take a measurements of the duty cycle every 4000 cycles.
#define uINV_ACDC_GaN_Temp_Coefficient     ((float32_t)162.3)
#define uINV_ACDC_GaN_Temp_Offset     ((float32_t)20.1)
//


//Scaling of the measurements and setting of the protections
#define uINV_ACDC_ADC_PU_SCALE_FACTOR ((float32_t) 0.000244140625f)    //


#define uINV_ACDC_vDC_MAX     ((float32_t) 492.58)   //
#define uINV_ACDC_vDC_Offset     ((float32_t) 0.48)   // Unit V

#define uINV_ACDC_iDC_MAX     ((float32_t) 162.3)   // Not approved
#define uINV_ACDC_iDC_Offset     ((float32_t) 0)   // Unit A

#define uINV_ACDC_vAC_MAX     ((float32_t) 396.65)   //
#define uINV_ACDC_vAC_Offset     ((float32_t) 2.82)   // Unit V

#define uINV_ACDC_iAC_MAX     ((float32_t) -16)   //
#define uINV_ACDC_iAC_Offset     ((float32_t) -0.035)   // Unit A




#define uINV_ACDC_iAC_TRIP_LIMIT_AMPS     ((float32_t)14)   // Unit A
#define uINV_ACDC_iAC_TRIP_LIMIT_POS     (int16_t)(2048+abs(2048*(uINV_ACDC_iAC_TRIP_LIMIT_AMPS/uINV_ACDC_iAC_MAX)))   // Unit A
#define uINV_ACDC_iAC_TRIP_LIMIT_NEG     (int16_t)(2048-abs(2048*(uINV_ACDC_iAC_TRIP_LIMIT_AMPS/uINV_ACDC_iAC_MAX)))   // Unit A



#define uINV_ACDC_vDC_TRIP_LIMIT_V    ((float32_t) 440 )  // Unit A

//


// 90 Degree phase shift

#define uINV_DC_FILTERING_CONSTANT     ((float32_t) 0.2696)   // Ts x 2 x pi x fcut Cut off frequency of 12 kHz
#define uINV_AC_FILTERING_CONSTANT     ((float32_t) 0.2696)   // Ts x 2 x pi x fcut Cut off frequency of 12 kHz

//


//Grid and contoller parameters


#define AC_FREQ    ((float32_t) 50)   //
#define ISR_CONTROL_FREQUENCY_AC ((float32_t) uINV_DCAC_BOOST_PWM_FREQ/6 )



#define uINV_SPLL_COEFF_B0      ((float32_t) 166.974338481427f)
#define uINV_SPLL_COEFF_B1      ((float32_t) -166.26611653716f)
#define uINV_SPLL_COEFF_K1      ((float32_t) 0.00188141f)
#define uINV_SPLL_COEFF_K2      ((float32_t) -0.99623717f)



//PI Controller Current
#define gi_ACDC_pi_KP ((float32_t)1.6)
#define gi_ACDC_pi_KI ((float32_t)0)
#define gi_ACDC_pi_MAX ((float32_t)400)
#define gi_ACDC_pi_MIN ((float32_t)-400)


#if UINV_LAB == 7
//PI Controller Voltage for resistive control on the AC still to define the correct one
#define gv_ACDC_pi_KP ((float32_t)0.0223)
#define gv_ACDC_pi_KI ((float32_t)0.0017)
#define gv_ACDC_pi_MAX ((float32_t)-0.05)
#define gv_ACDC_pi_MIN ((float32_t)-0.95)

#elif UINV_LAB == 8
#define gv_ACDC_pi_KP ((float32_t)0.0223)
#define gv_ACDC_pi_KI ((float32_t)0.0017)
#define gv_ACDC_pi_MAX ((float32_t)-0.05)
#define gv_ACDC_pi_MIN ((float32_t)-0.95)

#else
#define gv_ACDC_pi_KP ((float32_t)0.5)
#define gv_ACDC_pi_KI ((float32_t)0.00001)
#define gv_ACDC_pi_MAX ((float32_t)2)
#define gv_ACDC_pi_MIN ((float32_t)-2)

#endif

////////COEFF PR Controllers
#define  KPI_1H  ((float32_t) 0.0f)
#define  KII_1H ((float32_t) 20000.0f)
#define  WRCI_1H ((float32_t) 0.15f)
#define  KII_3H ((float32_t) 5000.0f)
#define  WRCI_3H ((float32_t) 0.12f)
#define  KII_5H ((float32_t) 5000.0f)
#define  WRCI_5H ((float32_t) 0.12f)
#define  KII_7H ((float32_t) 5000.0f)
#define  WRCI_7H ((float32_t) 0.12f)
#define  KII_9H ((float32_t) 5000.0f)
#define  WRCI_9H ((float32_t) 0.12f)

#define  KII_1H_VAC  ((float32_t) 1.0f)

#define  WRCI_1H_VAC ((float32_t) 0.5f)

//Limitation in the duty cycles
#define  uINV_DCAC_Index_Modulation_HIGH_PU ((float32_t) 0.98f)
#define  uINV_DCAC_Index_Modulation_LOW_PU ((float32_t) -0.98f)



//Slew Rate Control
#define uINV_VOLTS_PER_SECOND_SLEW ((float32_t)1000)//
#define ISR_CONTROL_FREQUENCY_AC_INVERSE (1.0 /ISR_CONTROL_FREQUENCY_AC)

//#define (uint32)uINV_DCAC_PWM_period = CLK_FREQ/DC_AC_pwm_freq/2


#endif /* UINV_DCAC_SETTINGS_H_ */
