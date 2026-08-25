//##############################################################################
//
// FILE:  uinv_comms.h
//
// TITLE: Solution header file for communications
//
//##############################################################################
// $TI Release: TIDA_010933 v2.00.00.00 $
// $Release Date: Wed Aug  7 17:16:55 CDT 2024 $
// $Copyright:
// Copyright (C) 2024 Texas Instruments Incorporated - http://www.ti.com/
// ALL RIGHTS RESERVED
// $
//##############################################################################


#ifndef UINV_COMMS_H_
#define UINV_COMMS_H_

#include "stdint.h"

typedef enum {
    SOLAR_UINVERTER_FLAG_RSD_ENABLED = (1 << 0),     /* Rapid shutdown status */
} uinv_solar_uinverter_flag_t;

typedef struct solar_uinverter_s {
    uint32_t panel_id;         /* To identify microinverter, and individual panels. R/W (W during commissioning, or randomly generated ID) */
    float pv_panel_v[4];       /* Panel voltage */
    float pv_panel_a[4];       /* Panel current */
    float pv_panel_w[4];       /* Power extracted from PV panel */
    float mppt_llc_in_v;       /* Voltage input to LLC */
    float mppt_llc_in_a;       /* Current input to LLC */
    float mppt_llc_in_w;       /* Power input into LLC */
    float llc_out_v;           /* Voltage output from LLC */
    float llc_out_a;           /* Current output from LLC */
    float llc_out_w;           /* Power output from LLC */
    float inverter_grid_out_v; /* Voltage output into grid */
    float inverter_grid_out_a; /* Current output into grid */
    float inverter_grid_out_w; /* Power output into grid */
    float inverter_grid_freq;  /* Grid frequency */
    uint32_t fault;            /* Fault flags */
    /* TODO: array from data plot */

} uinv_solar_uinverter_t;


void uINV_comms_init();
void uINV_comms_test();
void uINV_comms_update_data();
void uINV_comms_send();

#endif /* UINV_COMMS_H_ */
