//##############################################################################
//
// FILE:  uinv_comms.c
//
// TITLE:  Solution file for communications
//
//##############################################################################
// $TI Release: TIDA_010933 v2.00.00.00 $
// $Release Date: Wed Aug  7 17:16:55 CDT 2024 $
// $Copyright:
// Copyright (C) 2024 Texas Instruments Incorporated - http://www.ti.com/
// ALL RIGHTS RESERVED
// $
//##############################################################################

#include "driverlib.h"
#include "device.h"
#include "board.h"

#include "uinv_comms.h"
#include "uinv_dcac.h"
#include "uinv_dcdc.h"

uinv_solar_uinverter_flag_t uINV_comms_flag = (uinv_solar_uinverter_flag_t)0;
uinv_solar_uinverter_t uINV_comms_data;

void uINV_comms_init() {
    SCI_resetChannels(SCI_COMMS_BASE);
    SCI_resetRxFIFO(SCI_COMMS_BASE);
    SCI_resetTxFIFO(SCI_COMMS_BASE);
    SCI_clearInterruptStatus(SCI_COMMS_BASE, SCI_INT_TXFF | SCI_INT_RXFF);
    SCI_enableFIFO(SCI_COMMS_BASE);
    SCI_enableModule(SCI_COMMS_BASE);

    uINV_comms_data.panel_id = 0xC001501A;
}

void uINV_comms_test() {
    static uint32_t comms_counter = 0;

#if UINV_DCAC_COMMS_ENABLED == 1
    //
    // Check if any error in RX
    //
    uint16_t rxStatus = SCI_getRxStatus(SCI_COMMS_BASE);
    if((rxStatus & SCI_RXSTATUS_ERROR) != 0)
    {
        //
        //If Execution stops here there is some error
        //Analyze SCI_getRxStatus() API return value
        //
        uINV_DCDC_fail();
        EPWM_forceTripZoneEvent(DCAC_BASE, EPWM_TZ_FORCE_EVENT_OST);

        ESTOP0;
    }

    //
    // Check if data received
    //
    uint16_t fifoStatus = SCI_getRxFIFOStatus(SCI_COMMS_BASE);
    if(fifoStatus != SCI_FIFO_RX0) {
        uint16_t data  = (uint16_t)(HWREGH(SCI_COMMS_BASE + SCI_O_RXBUF) & SCI_RXBUF_SAR_M);
        uINV_comms_flag = (uinv_solar_uinverter_flag_t)data;
    }
    if((uINV_comms_flag & SOLAR_UINVERTER_FLAG_RSD_ENABLED) != 0) {
        //
        // Check if shutdown requested
        //
        uINV_DCDC_fail();
        EPWM_forceTripZoneEvent(DCAC_BASE, EPWM_TZ_FORCE_EVENT_OST);
    } else {
        if(comms_counter++ == UINV_DCAC_COMMS_COUNTER) {
            uINV_comms_update_data();
            uINV_comms_send();
            comms_counter = 0;
        }
    }
#endif
}

void uINV_comms_send() {
    //
    // Send data to SCI
    //
    SCI_writeCharArray(SCI_COMMS_BASE, (const uint16_t *)&uINV_comms_data, sizeof(uINV_comms_data));
}

void uINV_comms_update_data() {

    uINV_comms_data.fault = (!uINV_DCDC_started);

    uINV_comms_data.pv_panel_v[0] = uINV_DCDC_b1.voltage_pu*uINV_DCDC_VS_SCALE*ANA_REF;
    uINV_comms_data.pv_panel_a[0] = uINV_DCDC_b1.current_pu*uINV_DCDC_CS_SCALE*ANA_REF;
    uINV_comms_data.pv_panel_w[0] = uINV_comms_data.pv_panel_v[0] * uINV_comms_data.pv_panel_a[0];

    uINV_comms_data.pv_panel_v[1] = uINV_DCDC_b2.voltage_pu*uINV_DCDC_VS_SCALE*ANA_REF;
    uINV_comms_data.pv_panel_a[1] = uINV_DCDC_b2.current_pu*uINV_DCDC_CS_SCALE*ANA_REF;
    uINV_comms_data.pv_panel_w[1] = uINV_comms_data.pv_panel_v[1] * uINV_comms_data.pv_panel_a[1];

    uINV_comms_data.pv_panel_v[2] = uINV_DCDC_b3.voltage_pu*uINV_DCDC_VS_SCALE*ANA_REF;
    uINV_comms_data.pv_panel_a[2] = uINV_DCDC_b3.current_pu*uINV_DCDC_CS_SCALE*ANA_REF;
    uINV_comms_data.pv_panel_w[2] = uINV_comms_data.pv_panel_v[2] * uINV_comms_data.pv_panel_a[2];

    uINV_comms_data.pv_panel_v[3] = uINV_DCDC_b4.voltage_pu*uINV_DCDC_VS_SCALE*ANA_REF;
    uINV_comms_data.pv_panel_a[3] = uINV_DCDC_b4.current_pu*uINV_DCDC_CS_SCALE*ANA_REF;
    uINV_comms_data.pv_panel_w[3] = uINV_comms_data.pv_panel_v[3] * uINV_comms_data.pv_panel_a[3];

    uINV_comms_data.mppt_llc_in_v = uINV_DCDC_globals.vs_pu*uINV_DCDC_VS_SCALE*ANA_REF;
    uINV_comms_data.mppt_llc_in_w = uINV_comms_data.pv_panel_w[0] + uINV_comms_data.pv_panel_w[1] + uINV_comms_data.pv_panel_w[2] + uINV_comms_data.pv_panel_w[3];

    uINV_comms_data.llc_out_v = uINV_ACDC_vDC_sensed_NOTCH;
    uINV_comms_data.llc_out_a = uINV_ACDC_iDC_sensed;

    uINV_comms_data.inverter_grid_out_v = uINV_ACDC_vAC_sensed_FILTER;

}


