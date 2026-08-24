#ifndef _CAN_H
#define _CAN_H
#include "DSP2833x_Device.h"
#include "IQmathLib.h"
#define TMCAN 500  //5KÆµÂÊ 100ms
#define ID 1
#ifdef can_GLOBALS
#define can_GLOBALS
#else
#define can_GLOBALS extern
#endif
void CanData();
typedef union
{	
	struct
	{
		Uint16 Lbit         : 16;  
		Uint16 Hbit         : 16;  
	}bit;
	Uint32 all;
}CANDAT;
struct INTERFACE
{
	CANDAT SendL;
	CANDAT SendH;
	CANDAT SendL1;
	CANDAT SendH1;
	CANDAT RdatL1;
    CANDAT RdatH1;
	CANDAT RdatL2;
	CANDAT RdatH2;
	CANDAT RdatL3;
	CANDAT RdatH3;
};
can_GLOBALS struct INTERFACE InterfaceCan;
can_GLOBALS Uint16 CanSendTime;
void CanDataPack();
void PowerCan();
#endif


