#ifndef _GLOBALVAR_H
#define _GLOBALVAR_H
#include "DSP2833x_Device.h"
#include "IQmathLib.h"
#include "SVGen.h"
#ifdef main_GLOBALS
#define main_EXT
#else
#define main_EXT extern
#endif
struct STATE_BIT
{
	Uint16 start:1;
	Uint16 wait :1;
	Uint16 run   :1;
	Uint16 stop : 1;
	Uint16 fault :1;
	Uint16 mppt :1;
	Uint16 reserved :10;	
};
typedef union
{
	struct STATE_BIT bit;
	Uint16 all;
}STATE;
main_EXT STATE StateRegs;
typedef struct
{
	int Ug1;
	int Ug2;
	int Ug3;
	int Ig1;
	int Ig2;
	int Ig3;
	int T;
	int Udc;
	int ILou;
	int Idc;
}ADCIN;
main_EXT ADCIN AI;
typedef struct
{
	Uint16 LEDRun;
	Uint16 MidCheck;
	Uint32 StartCheck;
	Uint32 PVCheck;
	Uint32 FU_ON;
	Uint32 ZHU_ON;
	Uint32 Fault;
	Uint32 IdcCheck;
	Uint32 PVCheck2;
	Uint16 UPidCnt;
	Uint16 DCcount2;
	Uint16 IqCount;
	Uint32 MpptCnt;
	Uint32 MpptStart;
	unsigned char OpenThetaCnt;
	unsigned char CycleCnt;
}SYSCOUNTER;
main_EXT SYSCOUNTER SysCnt;
struct PROTECT_BIT
{
	Uint16 UgH :1;
	Uint16 UgL :1;
	Uint16 IgH :1;
	Uint16 FreqE :1;
	Uint16 UdcH :1;
	Uint16 UdcL :1;
	Uint16 PDP  :1;
	Uint16 PreCharge :1;//预充电
	Uint16 gudao : 1;
	Uint16 loudian : 1;
	Uint16 FreqH2 : 1;
	Uint16 FreqL2 : 1;
	Uint16 UgL2 : 1;
	Uint16 UgH2 : 1;
	Uint16 StartCheck :1;//开机自检
	Uint16 UserStop :1;//用户停止指令
	Uint16 EStop:1;  //紧急停止
	Uint16 reserved :14;
	Uint16 Clear :1;
};
typedef union
{
	struct PROTECT_BIT bit;
	Uint32 all;
}PROTECT;
main_EXT PROTECT ProtectRegs;
main_EXT PROTECT ProtectIn;
typedef struct
{
	Uint32 UdcH ;      
	Uint32 UdcL;       
	Uint32 UgH;	      
	Uint32 UgL ;       
	Uint32 IgH ;             
	Uint32 loudian ;        
	Uint32 FreqE ;
	Uint32 FreqH2;
	Uint32 FreqL2;
	Uint32 UgL2;
	Uint32 UgH2;     	      
	Uint32 Clear_Cnt;			//保护清除延时计数器
	Uint32 Clear_TM;			//保护清除延时计数值
}PROTECTCNT;
main_EXT PROTECTCNT ProtectCnt;		//监视字建立延时计数器
typedef struct
{
	_iq19 Ug1;
	_iq19 Ug2;
	_iq19 Ug3;
	_iq19 Ig1;
	_iq19 Ig2;
	_iq19 Ig3;
	_iq22 Freq;
	_iq19 Udc;
	_iq19 Idc;
	_iq19 Pn;
	_iq19 T;
}INPUT;
main_EXT INPUT InputSignal;
main_EXT _iq19 UdcF;
main_EXT _iq19 IdcF;
main_EXT int Ug1Mid,Ug2Mid,Ug3Mid,Ig1Mid,Ig2Mid,Ig3Mid;
main_EXT long Ug1Add,Ug2Add,Ug3Add,Ig1Add,Ig2Add,Ig3Add;
main_EXT _iq21 Udc_Given;
main_EXT _iq21 Udc_Given_com;
main_EXT _iq21 Iq_Given_com;
main_EXT _iq21 Id_Given_com;
main_EXT _iq21 Iq_Given;
main_EXT _iq21 Id_Given;
main_EXT _iq28 Vd,Vq,Alfa,Beta;
main_EXT SVGEN svgen;
main_EXT Uint32 PVCheckTM;
main_EXT _iq19 *bufaddr;
main_EXT _iq19 buf1[100],buf2[100],buf3[100];
main_EXT unsigned char Display;
main_EXT Uint32 TotalCO2;
main_EXT Uint32 TotalEnergy;
main_EXT Uint32 TotalTime;
#endif
