#define main_GLOBALS
#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "IQmathLib.h"
#include "Define.h"
#include "corecontrol.h"
#include "can.h"
#include "dataoperation.h"
#include "rs485.h"
#include "Spi.h"
#include "pid.h"
#include "dataoperation.h"
#include "rsdebug.h"
extern Uint16 EwpmCnt;
extern Uint16 IQmathLoadStart;
extern Uint16 IQmathLoadEnd;
extern Uint16 IQmathRunStart;
void InitVar();
unsigned char readflag,writeflag;
void main()
{
	DINT;
	InitSysCtrl();
	MemCopy(&RamfuncsLoadStart,&RamfuncsLoadEnd,&RamfuncsRunStart);//复制到RAM
    MemCopy(&IQmathLoadStart, &IQmathLoadEnd, &IQmathRunStart);			//拷贝IQmath库到RAM
    InitFlash();
	InitPieCtrl();
	InitPieVectTable();
	InitGpio();
	InitECanGpio();
	IER = 0x0000;
    IFR = 0x0000;
	EnableInterrupts();
	InitSci();
	InitAdc();
	InitECan();
	InitECap();
	InitEPwm();
	InitSpiGpio();
	SpiInit();
    EINT;   // 使能全局中断INTM
    ERTM;   // 使能全局实时中断
	InitVar();
	while(1)
	{
		HisOperator();
		DataRecord();
		SciReceiveDatab();
	    if(EwpmCnt ==5000)
		{
			GpioDataRegs.GPCDAT.bit.GPIO87=!GpioDataRegs.GPCDAT.bit.GPIO87;
			EALLOW;													//Can出错处理
	     	if (SciaRegs.SCIRXST.bit.RXERROR)                       //SCI出错处理              
			{
				SciaRegs.SCICTL1.bit.SWRESET = 0;
				SciaRegs.SCICTL1.bit.SWRESET = 1;	
			}
	     	if (ScibRegs.SCIRXST.bit.RXERROR)                       //SCI出错处理              
			{
				ScibRegs.SCICTL1.bit.SWRESET = 0;
				ScibRegs.SCICTL1.bit.SWRESET = 1;	
			}
			EDIS;	
			EwpmCnt=0;
		}
		LCDDisplay();
	}
}
void InitVar()
{
    Uint32 i;
	StateReg.all = 0;
	TaskFlag.all=0;

	InputFilter.Upv=0;
	InputFilter.Ipv=0;
	
	InterfaceCan.RdatL1.all=0;
	InterfaceCan.RdatL2.all=0;
	InterfaceCan.RdatL3.all=0;
	InterfaceCan.RdatH1.all=0;
	InterfaceCan.RdatH2.all=0;
	InterfaceCan.RdatH3.all=0;


	Rs485FlagReg.bit.Rs485State=0;
	Rs485FlagReg.bit.Cleareleccapacity=0;
	Rs485FlagReg.bit.Open=1;  // 上电自动开机
	TimeReg.minute=0;
	StartTime=TM30S;  // 开机时间
	SysCnt.StartCnt=0;
	SysFlag.bit.ACStop=0;

	UPid.Kp=_IQ(-0.05); // -0.05
	UPid.Ki=_IQ(-0.0002);  // 0.0004
	UPid.OutMax=_IQ(107);
	UPid.OutMin=_IQ(0);

	SysCnt.MpptStart=0;
	MpptRegs.DetaPMin=_IQ15(-60);  // -70
	MpptRegs.Du=_IQ15(3);  // 3
	firststartcnt=0;
	SendTimeCnt=0;

	Energytemp=0;
	Energy1min=0;
	HisDataReg.daytime=0;
	HisDataReg.alltime=0;
	HisDataReg.allenergy=0;
	HisDataReg.dayenergy=0;

	HmiFlagReg.bit.PortState=0;
	HmiFlagReg.bit.StartLCD=1;
	PageIndex=1;
	HPDataIndex=6;//dtt1016
	SendLength=0;
	HmiFlagReg.bit.SendReady=0;
	for(i=0;i<24;i++)
	{
       TBUFb1[i]=0;
	}
	TimeGetCnt=35950;//
}

