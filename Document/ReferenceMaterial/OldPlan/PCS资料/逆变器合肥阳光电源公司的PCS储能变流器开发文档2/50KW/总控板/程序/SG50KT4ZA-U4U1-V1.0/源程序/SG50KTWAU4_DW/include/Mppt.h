#ifndef _MPPT_H
#define _MPPT_H
#include "IQmathLib.h"
#ifdef mppt_GLOBALS
#define mppt_EXT
#else
#define mppt_EXT extern
#endif
/*typedef struct
{
	_iq19 In_1;
	_iq19 Un_1;
	_iq19 Pn_1;
	_iq19 DetaU;
	_iq19 DetaI;
	_iq19 Ka;
	_iq19 Du;
	_iq19 Uout;
	_iq19 DetauMin;
	_iq19 DetaiMin;
}MPPT;*/
/*typedef struct
{
	_iq15 P;
	_iq15 Pb;
	_iq15 Ua;
	_iq15 Ub;
	_iq15 Uc;
	_iq15 Ka;
	_iq15 Du;
	_iq15 Uout;
	unsigned char Tag;
	unsigned char Abccnt;
	_iq15 DetaPMin; //负数常数
	_iq15 DetaP;
	unsigned char FirstCnt;
}MPPT;
mppt_EXT MPPT MpptRegs;
void MpptFun();*/
#endif


