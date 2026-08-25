//##############################################################################
//
// FILE:  uinv_main.c
//
// TITLE: Main file for the solution
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

//
// Included Files
//
#include "driverlib.h"
#include "device.h"
#include "board.h"
#include "c2000ware_libraries.h"

#include "uinv_dcdc.h"
#include "uinv_dcac.h"
#include "uinv_comms.h"

// Functions Riccardo
//#include "rampgen.h"
//#include "dlog_4ch.h"



//
// Variable declarations for state machine
//
void (*Alpha_State_Ptr)(void);  // Base States pointer
void (*A_Task_Ptr)(void);       // State pointer A branch
void (*B_Task_Ptr)(void);       // State pointer B branch

//
// State Machine function prototypes
//------------------------------------
// Alpha states
//
void A0(void);  //state A0
void B0(void);  //state B0

//
// A branch states
//
void A1(void);  //state A1

//
// B branch states
//
void B1(void);  //state B1
void B2(void);  //state B2
void B3(void);  //state B3




//
// Main
//
void main(void)
{
    //
    // Disable sync(Freeze clock to PWM as well). GTBCLKSYNC is applicable
    // only for multiple core devices. Uncomment the below statement if
    // applicable.
    //
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

    //
    // Initialize device clock and peripherals
    //
    Device_init();

    //
    // Disable pin locks and enable internal pull-ups.
    //
    Device_initGPIO();

    //
    // Tasks State-machine init
    //
    Alpha_State_Ptr = &A0;
    A_Task_Ptr = &A1;
    B_Task_Ptr = &B1;

    //
    // Initialize PIE and clear PIE registers. Disables CPU interrupts.
    //
    Interrupt_initModule();

    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    //
    Interrupt_initVectorTable();

    DEVICE_DELAY_US(200000);
    //
    // PinMux and Peripheral Initialization
    //
    Board_init();

    EPWM_forceTripZoneEvent(DCAC_BASE, EPWM_TZ_FORCE_EVENT_OST);
    EPWM_forceTripZoneEvent(LLCSR_BASE, EPWM_TZ_FORCE_EVENT_OST);
    EPWM_forceTripZoneEvent(LLC1_BASE, EPWM_TZ_FORCE_EVENT_OST);
    EPWM_forceTripZoneEvent(LLC2_BASE, EPWM_TZ_FORCE_EVENT_OST);

    EPWM_forceTripZoneEvent(BOOST1_BASE, EPWM_TZ_FORCE_EVENT_OST);
    EPWM_forceTripZoneEvent(BOOST2_BASE, EPWM_TZ_FORCE_EVENT_OST);
    EPWM_forceTripZoneEvent(BOOST3_BASE, EPWM_TZ_FORCE_EVENT_OST);
    EPWM_forceTripZoneEvent(BOOST4_BASE, EPWM_TZ_FORCE_EVENT_OST);

    uINV_DCAC_GlobalVariablesInit();

    uINV_comms_init(); // Init SCI, reset FIFOs

    uINV_DCDC_init();
    uINV_DCAC_Init();  /// I put here the PWM Init

    uINV_DCDC_test(); // Low priority updates of frequencies and parameters for debug
    uINV_comms_test(); // Sending data every few seconds

    //
    // C2000Ware Library initialization
    //
    C2000Ware_libraries_init();

    //
    // Enable sync and clock to PWM
    //
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

    //
    // Enable Global Interrupt (INTM) and real time interrupt (DBGM)
    //
    EINT;
    ERTM;

    while(1)
    {
        //
        // State machine entry & exit point
        //
        (*Alpha_State_Ptr)();   // jump to an Alpha state (A0,B0,...)

    }
}


// Interrupt after DC/AC measurements
__attribute__((ramfunc))
interrupt void ISR1()
{
    ADC_clearInterruptStatus(ADC_A_BASE, ADC_INT_NUMBER1);

    GPIO_writePin(uINV_DCAC_TestPoint_2,1); // For profiling. Pin number 4 of connector J3

    uINV_DCAC_readCurrentAndVoltageSignals();

#if   UINV_LAB == 1
    uINV_DCAC_runISR1_lab1(); // Run DC/AC Converter in Open Loop
#elif UINV_LAB == 2
    uINV_DCAC_runISR1_lab2(); // Run DC/AC Converter in Open Loop across a resistor. PLL is enabled
#elif UINV_LAB == 3
    // Do nothing
#elif UINV_LAB == 4
    // Do nothing
#elif UINV_LAB == 5
    // Do nothing
#elif UINV_LAB == 6
    // Do nothing
#elif UINV_LAB == 7
    uINV_DCAC_runISR1_lab3(); // DC Link voltage control. Resistor connected on the AC terminals. (Boost and CLLLC are OFF)
#elif UINV_LAB == 8
    uINV_DCAC_runISR1_lab3(); // DC Link voltage control. Resistor connected on the AC terminals. (Boost and CLLLC are ON)
#elif UINV_LAB == 9
    uINV_DCAC_runISR1_lab4(); // Run current control loop across external resistor. (Boost and CLLLC are OFF)
#elif UINV_LAB == 10
    uINV_DCAC_runISR1_lab5(); //Run in close loop together with PLL toward the grid. No DC link voltage control (Boost and CLLLC are OFF)
#elif UINV_LAB == 11
    uINV_DCAC_runISR1_lab6(); //Run in close loop together with PLL toward the grid and voltage control loop. (Boost and CLLLC are ON)
#endif

  //Datalogger that you can trigger when you push uINV_Counter=0
    if(uINV_DCAC_Counter_1 < uINV_ACDC_DLOG_TOTAL_SAMPLES) // you can trigger this event by putting at zero uINV_DCAC_Counter.
    {
        uINV_DCAC_dVal5 = uINV_DCAC_Counter_1-1; // Counter so I can trigger.
        uINV_DCAC_dVal6 = uINV_ACDC_iAC_real_Ref_OUT_PI;
        uINV_DCAC_dVal7 = uINV_ACDC_iAC_sensed;  //uINV_ACDC_iAC_loop_out_PR1
        uINV_DCAC_dVal8 = gi_ACDC_pi.i10;
        DLOG_4CH_run(&uINV_DCAC_dLog2);
        uINV_DCAC_Counter_1++;
    }
    //Datalogger that you can trigger when you push uINV_Counter=0
    GPIO_writePin(uINV_DCAC_TestPoint_2,0);  // For profiling. Pin number 4 of connector J3
    Interrupt_clearACKGroup(INT_ADC_A_1_INTERRUPT_ACK_GROUP);
}

// Interrupt after DC/DC measurements
__attribute__((ramfunc))
interrupt void ISR2(void) {
    ADC_clearInterruptStatus(ADC_B_BASE, ADC_INT_NUMBER1);
    GPIO_writePin(uINV_DCAC_TestPoint_1,1);  // For profiling

    uINV_DCDC_Flag_Slew++;

    uINV_DCDC_readCurrentAndVoltageSignals();

#if UINV_LAB != 1 // Lab 1 has no protections
    uINV_DCDC_checkProtections();
#endif

#if UINV_LAB == 1
    uINV_DCDC_lab1(); // PWM and ADC check
#elif UINV_LAB == 2
    uINV_DCDC_lab2(); // Run Boost Converters in open-loop
#elif UINV_LAB == 3
    uINV_DCDC_lab3(); // Run Boost Converters in close-loop
#elif UINV_LAB == 4
    uINV_DCDC_lab4(); // Run Boost Converters in close-loop plus MPPT algorithm
#elif UINV_LAB == 5
    uINV_DCDC_lab5(); // Run CLLLC Converter
#elif UINV_LAB == 6
    uINV_DCDC_lab6(); // Run CLLLC Converter plus boost Converters
#elif UINV_LAB == 7
    uINV_DCDC_lab7(); // Run DC/AC Converter in Open Loop across a resistive load
#elif UINV_LAB == 8
    uINV_DCDC_lab8(); // Run DC/DC together with current control with DC/AC Converter in Open Loop across a resistive load
#elif UINV_LAB == 9
    uINV_DCDC_lab9(); // Run current control loop across external resistor. (Boost and CLLLC are OFF)
#elif UINV_LAB == 10
    uINV_DCDC_lab10(); //Run in close loop together with PLL toward the grid. No DC link voltage control (Boost and CLLLC are OFF)
#elif UINV_LAB == 11
    uINV_DCDC_lab11(); // Run DC/DC together with current control with DC/AC Converter in Open Loop across a resistive load
#endif


    uINV_DCDC_releaseTrip();
    uINV_DCDC_updateBoostsDuty();

    GPIO_writePin(uINV_DCAC_TestPoint_1,0);  // For profiling

    //
    // Check if overflow has occurred
    //
    if(true == ADC_getInterruptOverflowStatus(ADC_B_BASE, ADC_INT_NUMBER1))
    {
        ADC_clearInterruptOverflowStatus(ADC_B_BASE, ADC_INT_NUMBER1);
        ADC_clearInterruptStatus(ADC_B_BASE, ADC_INT_NUMBER1);
    }
    Interrupt_clearACKGroup(INT_ADC_B_1_INTERRUPT_ACK_GROUP);
}


//
//  STATE-MACHINE SEQUENCING AND SYNCRONIZATION FOR SLOW BACKGROUND TASKS
//

//
//--------------------------------- FRAME WORK --------------------------------
//
void A0(void)
{

    //
    // loop rate synchronizer for A-tasks
    //
    (*A_Task_Ptr)();        // jump to an A Task (A1,A2,A3,...)
    Alpha_State_Ptr = &B0;      // Comment out to allow only A tasks
}

void B0(void)
{

    //
    // loop rate synchronizer for B-tasks
    //
    (*B_Task_Ptr)();        // jump to a B Task (B1,B2,B3,...)
    Alpha_State_Ptr = &A0;      // Allow C state tasks
}

//
//  A - TASKS (executed in every 1 msec)
//
void A1(void)
{
    A_Task_Ptr = &A1;


    if(uINV_DCDC_Flag_Slew>=100)
    {
        if(uINV_DCDC_b1.current_ref_slewed < uINV_DCDC_b1.current_ref_pu)
            uINV_DCDC_b1.current_ref_slewed = uINV_DCDC_b1.current_ref_slewed + 0.0001f;
        else if(uINV_DCDC_b1.current_ref_slewed >= uINV_DCDC_b1.current_ref_pu){
            uINV_DCDC_b1.current_ref_slewed = uINV_DCDC_b1.current_ref_slewed - 0.0001f;
    }

    if(uINV_DCDC_b1.current_ref_slewed > 0.7f){
        uINV_DCDC_b1.current_ref_slewed = 0.7f;
    }
    else if( uINV_DCDC_b1.current_ref_slewed < -0.7f){
        uINV_DCDC_b1.current_ref_slewed = -0.7f;
    }



    //Demo Modification Slewing current reference
    if(uINV_DCDC_b2.current_ref_slewed < uINV_DCDC_b2.current_ref_pu)
        uINV_DCDC_b2.current_ref_slewed = uINV_DCDC_b2.current_ref_slewed + 0.0001f;
    else if(uINV_DCDC_b2.current_ref_slewed >= uINV_DCDC_b2.current_ref_pu){
        uINV_DCDC_b2.current_ref_slewed = uINV_DCDC_b2.current_ref_slewed - 0.0001f;
    }

    if(uINV_DCDC_b2.current_ref_slewed > 0.7f){
        uINV_DCDC_b2.current_ref_slewed = 0.7f;
    }
    else if( uINV_DCDC_b2.current_ref_slewed < -0.7f){
        uINV_DCDC_b2.current_ref_slewed = -0.7f;
    }



    if(uINV_DCDC_b3.current_ref_slewed < uINV_DCDC_b3.current_ref_pu)
        uINV_DCDC_b3.current_ref_slewed = uINV_DCDC_b3.current_ref_slewed + 0.0001f;
    else if(uINV_DCDC_b3.current_ref_slewed >= uINV_DCDC_b3.current_ref_pu){
        uINV_DCDC_b3.current_ref_slewed = uINV_DCDC_b3.current_ref_slewed - 0.0001f;
    }

    if(uINV_DCDC_b3.current_ref_slewed > 0.7f){
        uINV_DCDC_b3.current_ref_slewed = 0.7f;
    }
    else if( uINV_DCDC_b3.current_ref_slewed < -0.7f){
        uINV_DCDC_b3.current_ref_slewed = -0.7f;
    }


    if(uINV_DCDC_b4.current_ref_slewed < uINV_DCDC_b4.current_ref_pu)
        uINV_DCDC_b4.current_ref_slewed = uINV_DCDC_b4.current_ref_slewed + 0.0001f;
    else if(uINV_DCDC_b4.current_ref_slewed >= uINV_DCDC_b4.current_ref_pu){
        uINV_DCDC_b4.current_ref_slewed = uINV_DCDC_b4.current_ref_slewed - 0.0001f;
    }

    if(uINV_DCDC_b4.current_ref_slewed > 0.7f){
        uINV_DCDC_b4.current_ref_slewed = 0.7f;
    }
    else if( uINV_DCDC_b4.current_ref_slewed < -0.7f){
        uINV_DCDC_b4.current_ref_slewed = -0.7f;
    }





    if(uINV_ACDC_vDC_Ref_Slewed < uINV_ACDC_vDC_Ref)
        uINV_ACDC_vDC_Ref_Slewed = uINV_ACDC_vDC_Ref_Slewed + 0.1f;
    else if(uINV_ACDC_vDC_Ref_Slewed >= uINV_ACDC_vDC_Ref){
        uINV_ACDC_vDC_Ref_Slewed = uINV_ACDC_vDC_Ref_Slewed - 0.1f;
    }

    if(uINV_ACDC_vDC_Ref_Slewed > 400.0f){
        uINV_ACDC_vDC_Ref_Slewed = 400.0f;
    }


    if(uINV_ACDC_FFW_K_Slewed < uINV_ACDC_FFW_K)
        uINV_ACDC_FFW_K_Slewed = uINV_ACDC_FFW_K_Slewed + 0.001f;
    else if(uINV_ACDC_FFW_K_Slewed >= uINV_ACDC_FFW_K){
        uINV_ACDC_FFW_K_Slewed = uINV_ACDC_FFW_K_Slewed - 0.001f;
    }

    uINV_DCDC_Flag_Slew=0;
    }
}

//
//  B - TASKS
//
void B1(void)
{
    uINV_DCDC_test(); // Low priority updates of frequencies and parameters for debug

    B_Task_Ptr = &B2;
}


void B2(void)
{
    uINV_DCAC_test(); // Low priority temp updates

    B_Task_Ptr = &B3;
}

void B3(void)
{
    uINV_comms_test();

    B_Task_Ptr = &B1;
}



//
// End of File
//

