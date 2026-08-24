#ifndef _CORECONTROL_H
#define _CORECONTROL_H
#include "DSP2833x_Device.h"
#include "IQmathLib.h"
#ifdef corecontrol_GLOBALS
#define corecontrol_GLOBALS
#else
#define corecontrol_GLOBALS extern
#endif

// --- Foot Define --
typedef struct
{
	int16 Upv;
	int16 Ipv;

}ADCIN;
corecontrol_GLOBALS ADCIN AI;
typedef struct
{
	_iq Upv;
	_iq Ipv;
	_iq15 Pin;
}FILTER;
corecontrol_GLOBALS FILTER InputFilter;
typedef struct
{
	_iq Upv;
	_iq Ipv;
	_iq15 Pin;  //实际功率KW
	_iq POut;  //实际功率KW 
}INPUTSIGNAL;
corecontrol_GLOBALS INPUTSIGNAL InputSignal;
typedef struct
{
	int16 Upv;
	int16 Ipv;
	int16 Ugareal;
	int16 Ugbreal;
	int16 Ugcreal;
	int16 Igareal;
	int16 Igbreal;
	int16 Igcreal;
	Uint16 Pin;    // 实际功率KW放大10倍
	Uint16 POut;  //实际功率KW放大10倍
}MONITOR;
corecontrol_GLOBALS MONITOR MonitorS;


typedef union
{	
	struct
	{
		Uint16 EStop          : 1;  //紧停
		Uint16 FreqIn           : 1;  //电网频率
		Uint16 Freq1          : 1;
		Uint16 Freq2          : 1;
		Uint16 Freq3          : 1;
		Uint16 Freq4          : 1;
		Uint16 rsvd1		  : 10;  //reserved
	}bit;
	Uint16 all;
}GPIODI;
corecontrol_GLOBALS GPIODI DI;						//开关量输入
typedef union
{	
	struct
	{
		Uint16 Pbit0          : 1;  //紧停
		Uint16 Pbit1          : 1;  //紧停
		Uint16 Pbit2          : 1;  //紧停
		Uint16 Pbit3          : 1;  //紧停
		Uint16 Pbit4          : 1;  //紧停
		Uint16 Pbit5          : 1;  //紧停
		Uint16 Pbit6          : 1;  //紧停
		Uint16 Pbit7          : 1;  //紧停
		Uint16 Pbit8          : 1;  //紧停
		Uint16 Pbit9          : 1;  //紧停
		Uint16 Pbit10          : 1;  //紧停
		Uint16 Pbit11          : 1;  //紧停
		Uint16 Pbit12          : 1;  //紧停
		Uint16 Pbit13          : 1;  //紧停
		Uint16 Pbit14          : 1;  //紧停
		Uint16 Pbit15          : 1;  //紧停
		Uint16 Pbit16          : 1;  //紧停
		Uint16 Pbit17         : 1;  //紧停
		Uint16 Pbit18         :1;
		Uint16 Pbit19           : 1;  //电网频率
		Uint16 Pbit20           : 1;  //电网频率
		Uint16 Pbit21           : 1;  //电网频率
		Uint16 Pbit22           : 1;  //电网频率
		Uint16 Pbit23           : 1;  //电网频率
		Uint16 Pbit24           : 1;  //电网频率
		Uint16 Pbit25           : 1;  //电网频率
		Uint16 Pbit26           : 1;  //电网频率
		Uint16 Pbit27           : 1;  //电网频率
		Uint16 rsvd2		  : 4;  //reserved
	}bit;
	Uint32 all;
}STOPCODE1;
corecontrol_GLOBALS STOPCODE1 StopReg1;	
typedef union
{	
	struct
	{
		Uint16 Pbit0          : 1;  //紧停
		Uint16 Pbit1          : 1;  //紧停
		Uint16 Pbit2          : 1;  //紧停
		Uint16 Pbit3          : 1;  //紧停
		Uint16 Pbit4          : 1;  //紧停
		Uint16 Pbit5          : 1;  //紧停
		Uint16 Pbit6          : 1;  //紧停
		Uint16 Pbit7          : 1;  //紧停
		Uint16 Pbit8          : 1;  //紧停
		Uint16 Pbit9          : 1;  //紧停
		Uint16 Pbit10          : 1;  //紧停
		Uint16 Pbit11          : 1;  //紧停
		Uint16 Pbit12          : 1;  //紧停
		Uint16 Pbit13          : 1;  //紧停
		Uint16 Pbit14          : 1;  //紧停
		Uint16 Pbit15          : 1;  //紧停
		Uint16 Pbit16          : 1;  //紧停
		Uint16 Pbit17         : 1;  //紧停
		Uint16 Pbit18         :1;
		Uint16 Pbit19           : 1;  //电网频率
		Uint16 Pbit20           : 1;  //电网频率
		Uint16 Pbit21           : 1;  //电网频率
		Uint16 Pbit22           : 1;  //电网频率
		Uint16 Pbit23           : 1;  //电网频率
		Uint16 Pbit24           : 1;  //电网频率
		Uint16 Pbit25           : 1;  //电网频率
		Uint16 Pbit26           : 1;  //电网频率
		Uint16 Pbit27           : 1;  //电网频率
		Uint16 rsvd2		  : 4;  //reserved
	}bit;
	Uint32 all;
}STOPCODE2;
corecontrol_GLOBALS STOPCODE2 StopReg2;
typedef union
{	
	struct 
	{
		Uint16 RunL     	  : 1;  //运行灯
		Uint16 FaultL   	  : 1;  //故障灯
		Uint16 PowerOn    : 1;  //并网合闸
		Uint16 GridSwitch2    : 1;  //并网合闸
		Uint16 rsvd1      	  : 12;	//reserved
	}bit;
	Uint16 all;
}GPIODO;
corecontrol_GLOBALS GPIODO DO;						//开关量输出

typedef union
{	
	struct 
	{
		Uint16 Open           : 1;  //
		Uint16 Reset          : 1;
		Uint16 FanUp          : 1;
		Uint16 PvOk           : 1;
		Uint16 ACStop           : 1;
		Uint16 rsvd1      	  : 11;	//reserved
	}bit;
	Uint16 all;
}DEBUG;
corecontrol_GLOBALS DEBUG SysFlag;						//

typedef struct
{     
	unsigned char  CycleCnt; 
	Uint16 MpptCnt;
	Uint16 MpptStart;
	Uint16 UpvUpcnt;
	int32 StartCnt;
	Uint16 Resetcnt;
	Uint16 StopCnt;
	Uint16 PowerOncnt;
	Uint16 PowerOffcnt;
	unsigned char UPidCnt;
}CNT;
corecontrol_GLOBALS CNT SysCnt;
typedef struct
{
	Uint16 year;     
	unsigned char  month; 
	unsigned char  day; 
	unsigned char  hours;
	unsigned char  minute;
	unsigned char  second; 
	unsigned char  secondbar;
	unsigned char oldday;
	Uint16 Intercnt;  
}TIME;
corecontrol_GLOBALS TIME TimeReg;
typedef union
{	
	struct 
	{
		Uint16 ACState     : 2;
		Uint16 DCState      : 2;
		Uint16 ACStatebark     : 2;
		Uint16 rsvd1     : 10;	//reserved
	}bit;
	Uint16 all;
}STATE;
corecontrol_GLOBALS STATE StateReg;
typedef struct
{
	_iq15 P;
	_iq15 Pb;
	_iq15 Ua;
	_iq15 Ub;
	_iq15 Uc;
	_iq15 Ka;
	_iq15 Du;
	_iq15 Uout;
	_iq15 IgdGiven;
	int Tag;
	int Abccnt;
	_iq15 DetaPMin; //负数常数
	_iq15 DetaP;
	int FirstCnt;
}MPPTREG;
corecontrol_GLOBALS MPPTREG MpptRegs;					//状态标志

corecontrol_GLOBALS int16 buf1[100];
corecontrol_GLOBALS int16 *bufaddr;
corecontrol_GLOBALS Uint16 adc[8];
corecontrol_GLOBALS int32 StartTime;

corecontrol_GLOBALS _iq Udc_Given;
void Debugcode();
void StateFun();
void MpptFun();
void ADCReuslt();
void IgdRefCalc();
void TimeFun();
void ACPower();
#endif

