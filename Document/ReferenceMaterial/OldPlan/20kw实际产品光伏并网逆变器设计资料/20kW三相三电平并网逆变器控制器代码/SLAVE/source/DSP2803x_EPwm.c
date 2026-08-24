// TI File $Revision: /main/3 $
// Checkin $Date: November 10, 2009   14:05:11 $
//###########################################################################
//
// FILE:   DSP2803x_EPwm.c
//
// TITLE:  DSP2803x EPwm Initialization & Support Functions.
//
//###########################################################################
// $TI Release: 2803x C/C++ Header Files V1.21 $
// $Release Date: December 1, 2009 $
//###########################################################################

#include "SL_slave.h"

//---------------------------------------------------------------------------
// InitEPwm:
//---------------------------------------------------------------------------
// This function initializes the EPwm(s) to a known state.
//
void InitEPwm(void)
{
/*
The proper procedure for enabling ePWM clocks is as follows:
1.Enable ePWM module clocks in the PCLKCR1 register.
2.Set TBCLKSYNC to 0.
3.Configure prescaler values and ePWM modes.
4.Set TBCLKSYNC to 1.
*/
	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
	EDIS;

// EPWM 1A (up),1B(down) for inverter brige Left
// setup TBCLK
	EPwm1Regs.TBPRD                = 1500;				// EPWM1_TIMER_TBPRD; 20KHz
	EPwm1Regs.TBPHS.half.TBPHS     = 0x0000;			// 时基计数器TBCTR的phase，没用
	EPwm1Regs.TBCTL.bit.CTRMODE    = TB_COUNT_UPDOWN;	// 计数模式up-down

	EPwm1Regs.TBCTL.bit.PHSEN      = TB_DISABLE;		// 同步事件被忽略，TBCTR不会从phase加载，phase就是TBPHS的值
	EPwm1Regs.TBCTL.bit.PRDLD      = TB_SHADOW;			// 当TBCTR为0时，周期寄存器TBPRD的值从影子寄存器加载(而非直接写入)

	EPwm1Regs.TBCTL.bit.SYNCOSEL   = TB_SYNC_DISABLE;	// 禁能EPWM1SYNCO信号

	EPwm1Regs.TBCTL.bit.HSPCLKDIV  = TB_DIV1;			// Clock ratio to SYSCLKOUT
	EPwm1Regs.TBCTL.bit.CLKDIV     = TB_DIV1;

	EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;			// 计数器比较A(CMPA)操作模式:影子模式
	EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;			// 计数器比较B(CMPB)操作模式:影子模式

	EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_PRD;		// Load on Zero
	EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_PRD;		// 当TBCTR==TBPRD时从影子选择模式加载CMPB

// set Compare values
	EPwm1Regs.AQCTLA.bit.CAU       = AQ_CLEAR;     		// count up,  =CMPA,PWMA=1 计数器递增且等于CMPA时，强制EPWM1A输出低
	EPwm1Regs.AQCTLA.bit.CAD       = AQ_SET;			// count down,=CMPA,PWMA=1 计数器递减且等于CMPA时，强制EPWM1A输出高

	EPwm1Regs.AQCTLB.bit.CBU       = AQ_CLEAR;			// same as A 计数器递增且等于CMPB时，强制EPWM1B输出低
	EPwm1Regs.AQCTLB.bit.CBD       = AQ_SET;			// same as A 计数器递减且等于CMPB时，强制EPWM1B输出高

	EPwm1Regs.AQSFRC.bit.RLDCSF    = 0x3;				// software force immediately 直接操作，不通过影子寄存器

// configure the Start of Conversion for the ADC.
	EPwm1Regs.ETSEL.bit.SOCAEN     = 1;					// Enable SOC on A group
	EPwm1Regs.ETSEL.bit.SOCASEL    = ET_CTR_ZERO;		// Select SOC from counter = ZERO
	EPwm1Regs.ETPS.bit.SOCAPRD     = ET_1ST;			// Generate pulse on 1st event 

	EPwm1Regs.DBRED                = 100;				// 上升沿死区：2/3 us
	EPwm1Regs.DBFED                = 100;				// 下降沿死区：2/3 us

// Setup counter mode

// EPWM 2A (up),2B(down) for inverter bridge Right
// setup TBCLK
	EPwm2Regs.TBPRD                = 1500;				// EPWM2_TIMER_TBPRD; 20KHz
	EPwm2Regs.TBPHS.half.TBPHS     = 0x0000;          
	EPwm2Regs.TBCTL.bit.CTRMODE    = TB_COUNT_UPDOWN; 

	EPwm2Regs.TBCTL.bit.PHSEN      = TB_DISABLE;   
	EPwm2Regs.TBCTL.bit.PRDLD      = TB_SHADOW;

	EPwm2Regs.TBCTL.bit.SYNCOSEL   = TB_SYNC_IN;   

	EPwm2Regs.TBCTL.bit.HSPCLKDIV  = TB_DIV1;			// Clock ratio to SYSCLKOUT
	EPwm2Regs.TBCTL.bit.CLKDIV     = TB_DIV1;

	EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;

	EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_PRD;		// Load on Zero
	EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_PRD;

// set Compare values
	EPwm2Regs.AQCTLA.bit.CAU       = AQ_CLEAR;			// count up, =CMPA,PWMA=1
	EPwm2Regs.AQCTLA.bit.CAD       = AQ_SET;			// count down,=CMPA,PWMA=1

	EPwm2Regs.AQCTLB.bit.CBU       = AQ_CLEAR;			// same as A
	EPwm2Regs.AQCTLB.bit.CBD       = AQ_SET; 

	EPwm2Regs.AQSFRC.bit.RLDCSF    = 0x3;				// software force immediately

	EPwm2Regs.DBRED                = 100;
	EPwm2Regs.DBFED                = 100;

// EPWM 3A (up),3B(down) for DC/DC bridge 
// setup TBCLK
	EPwm3Regs.TBPRD                = 1500;				// EPWM2_TIMER_TBPRD; 20KHz
	EPwm3Regs.TBPHS.half.TBPHS     = 0x0000;          
	EPwm3Regs.TBCTL.bit.CTRMODE    = TB_COUNT_UPDOWN; 

	EPwm3Regs.TBCTL.bit.PHSEN      = TB_DISABLE;   
	EPwm3Regs.TBCTL.bit.PRDLD      = TB_SHADOW;

	EPwm3Regs.TBCTL.bit.SYNCOSEL   = TB_SYNC_IN;   

	EPwm3Regs.TBCTL.bit.HSPCLKDIV  = TB_DIV1;			// Clock ratio to SYSCLKOUT
	EPwm3Regs.TBCTL.bit.CLKDIV     = TB_DIV1;

	EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm3Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;

	EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_PRD;		// Load on Zero
	EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_PRD;

// set Compare values
	EPwm3Regs.AQCTLA.bit.CAU       = AQ_CLEAR;			// count up,  =CMPA,PWMA=1
	EPwm3Regs.AQCTLA.bit.CAD       = AQ_SET;			// count down,=CMPA,PWMA=1

	EPwm3Regs.AQCTLB.bit.CBU       = AQ_CLEAR;			// same as A
	EPwm3Regs.AQCTLB.bit.CBD       = AQ_SET; 

	EPwm3Regs.AQSFRC.bit.RLDCSF    = 0x3;				// software force immediately

	EPwm3Regs.DBRED                = 100;
	EPwm3Regs.DBFED                = 100;

	EPwm3Regs.DBCTL.bit.IN_MODE    = DBA_RED_DBB_FED;	// 10
	EPwm3Regs.DBCTL.bit.OUT_MODE   = DB_FULL_ENABLE;	// 11
	EPwm3Regs.DBCTL.bit.POLSEL     = DB_ACTV_HIC;		// 10

	EPwm3Regs.AQCSFRC.bit.CSFA     = AQ_CLEAR;
	EPwm3Regs.AQCSFRC.bit.CSFB     = AQ_SET;   

// EPWM 4A for boost1, EPWM4B for boost2
	EPwm4Regs.TBPRD                = 1500;				// EPWM2_TIMER_TBPRD; 20KHz
	EPwm4Regs.TBPHS.half.TBPHS     = 0x0000;          
	EPwm4Regs.TBCTL.bit.CTRMODE    = TB_COUNT_UPDOWN; 

	EPwm4Regs.TBCTL.bit.PHSEN      = TB_DISABLE;   
	EPwm4Regs.TBCTL.bit.PRDLD      = TB_SHADOW;

	EPwm4Regs.TBCTL.bit.SYNCOSEL   = TB_SYNC_IN;   

	EPwm4Regs.TBCTL.bit.HSPCLKDIV  = TB_DIV1;			// Clock ratio to SYSCLKOUT
	EPwm4Regs.TBCTL.bit.CLKDIV     = TB_DIV1;

	EPwm4Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	EPwm4Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;

	EPwm4Regs.CMPCTL.bit.LOADAMODE = CC_CTR_PRD;		// Load on Zero
	EPwm4Regs.CMPCTL.bit.LOADBMODE = CC_CTR_PRD;

// set Compare values
	EPwm4Regs.AQCTLA.bit.CAU       = AQ_CLEAR;			// count up,  =CMPA,PWMA=1
	EPwm4Regs.AQCTLA.bit.CAD       = AQ_SET;			// count down,=CMPA,PWMA=1

	EPwm4Regs.AQCTLB.bit.CBU       = AQ_CLEAR;			// same as A
	EPwm4Regs.AQCTLB.bit.CBD       = AQ_SET; 


	EPwm1Regs.TBCTR = 0x0000; 
	EPwm2Regs.TBCTR = 0x0000;  
	EPwm3Regs.TBCTR = 0x0000;  
	EPwm4Regs.TBCTR = 0x0000;  
	EPwm5Regs.TBCTR = 0x0000;  
	EPwm6Regs.TBCTR = 0x0000;   

	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
	EDIS;


	EPwm1Regs.DBCTL.bit.IN_MODE  = DBA_RED_DBB_FED;		// 10
	EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;		// 11
	EPwm1Regs.DBCTL.bit.POLSEL   = DB_ACTV_HIC;			// 10

	EPwm1Regs.AQCSFRC.bit.CSFA   = AQ_CLEAR;
	EPwm1Regs.AQCSFRC.bit.CSFB   = AQ_SET;


	EPwm2Regs.DBCTL.bit.IN_MODE  = DBA_RED_DBB_FED;		// 10
	EPwm2Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;		// 11
	EPwm2Regs.DBCTL.bit.POLSEL   = DB_ACTV_HIC;			// 10

	EPwm2Regs.AQCSFRC.bit.CSFA   = AQ_CLEAR;
	EPwm2Regs.AQCSFRC.bit.CSFB   = AQ_SET;  


	EPwm3Regs.DBCTL.bit.IN_MODE  = DBA_RED_DBB_FED;		// 10
	EPwm3Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;		// 11
	EPwm3Regs.DBCTL.bit.POLSEL   = DB_ACTV_HIC;			// 10

	EPwm3Regs.AQCSFRC.bit.CSFA   = AQ_CLEAR;
	EPwm3Regs.AQCSFRC.bit.CSFB   = AQ_SET;      
}

void PWMOutputsEnable(void)
{
	S_BOOST_PWM_ENABLE;
	S_INV_PWM_ENABLE;
} 


void PWMOutputsDisable(void)
{ 
	S_BOOST_PWM_DISABLE;
	S_INV_PWM_DISABLE;
} 
//===========================================================================
// End of file.
//===========================================================================
