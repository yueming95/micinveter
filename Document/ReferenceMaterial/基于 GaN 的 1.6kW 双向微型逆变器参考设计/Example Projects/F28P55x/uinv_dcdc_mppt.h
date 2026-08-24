//##############################################################################
//
// FILE:  uinv_dcdc_mppt.h
//
// TITLE:  Solution header file for the MPPT stage
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

#ifndef UINV_DCDC_MPPT_H_
#define UINV_DCDC_MPPT_H_

typedef struct {
    float32_t  Ipv;
    float32_t  Vpv;
    float32_t  MaxI;
    float32_t  MinI;
    float32_t  Stepsize;
    float32_t  ImppOut;
    // internal variables
    float32_t  Step;
    float32_t  PpvOld;
    float32_t  IpvOld;
    int16_t mppt_first;
    int mppt_enable;
} uinv_dcdc_mppt_t;

static void uinv_dcdc_mppt_init(uinv_dcdc_mppt_t *v) {
    v->mppt_first = 1;
}

#pragma FUNC_ALWAYS_INLINE(uinv_dcdc_mppt_run)
static void uinv_dcdc_mppt_run(uinv_dcdc_mppt_t *v) {
    float32_t P = v->Vpv * v->Ipv;

    if(v->mppt_enable) {
        if(v->mppt_first) {
            v->ImppOut= v->Ipv;
            v->PpvOld = P;
            v->IpvOld = v->Ipv;
            v->mppt_first = 0;
        } else {
            float32_t deltaI = v->Ipv - v->IpvOld;

            if(deltaI > 0) { // If we are increasing current
                if(P > v->PpvOld) { // and power increases
                    v->ImppOut=v->Ipv+v->Stepsize;
                } else { // and power decreases
                    v->ImppOut=v->Ipv-v->Stepsize;
                }
            } else { // If we are decreasing current
                if(P > v->PpvOld) { // and power increases
                    v->ImppOut=v->Ipv-v->Stepsize;
                } else { // and power decreases
                    v->ImppOut=v->Ipv+v->Stepsize;
                }
            }
        }
    }
    v->ImppOut=(v->ImppOut<v->MinI) ? v->MinI : v->ImppOut;
    v->ImppOut=(v->ImppOut>v->MaxI) ? v->MaxI : v->ImppOut;

    v->IpvOld = v->Ipv;
    v->PpvOld = P;
}

#endif /* UINV_DCDC_MPPT_H_ */
