#include "DSP2833x_Device.h"
#include "IQmathLib.h"
#include "corecontrol.h"
#include "define.h"
#include "can.h"
#include "dataoperation.h"
#pragma CODE_SECTION(Epwm1_isr, "ramfuncs");
#pragma CODE_SECTION(TZ5_isr, "ramfuncs");
#pragma CODE_SECTION(ecap1_isr, "ramfuncs");
Uint16 EwpmCnt;
interrupt void Epwm1_isr(void) //0.2ms
{
    EwpmCnt++;
	ADCReuslt();
	StateFun();
	IgdRefCalc();
	Debugcode();
    DataFun();
    PowerCan();
    CanData();
    TimeFun();
    ACPower();
	LCDINT();
	GpioDataRegs.GPACLEAR.bit.GPIO6=DO.bit.PowerOn;
	GpioDataRegs.GPASET.bit.GPIO6=!DO.bit.PowerOn;

	
	EPwm1Regs.ETCLR.bit.INT=1;//清中断标志
	PieCtrlRegs.PIEACK.all=PIEACK_GROUP3;	
}
interrupt void TZ5_isr(void)
{
	EALLOW; 
	EPwm1Regs.TZCTL.all=0x000A;  //pwm强制地
	EPwm2Regs.TZCTL.all=0x000A;
	EPwm2Regs.TZCTL.all=0x000A;
	EDIS;
}
interrupt void ecap1_isr(void)
{
    ECap1Regs.ECCLR.bit.CEVT1 = 1;
    ECap1Regs.ECCLR.bit.INT = 1;
    ECap1Regs.ECCTL2.bit.REARM = 1;
	
   // Acknowledge this interrupt to receive more interrupts from group 4
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;
}

