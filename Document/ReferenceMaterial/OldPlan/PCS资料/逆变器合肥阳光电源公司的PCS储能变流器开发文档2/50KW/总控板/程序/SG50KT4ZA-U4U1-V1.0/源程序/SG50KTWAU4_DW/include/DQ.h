#ifndef _DQ_H
#define _DQ_H
#include "DSP2833x_Device.h"
#include "IQmathLib.h"
#ifdef dq_GLOBALS
#define dq_GLOBALS
#else
#define dq_GLOBALS extern
#endif
typedef struct
{
	_iq28 Theta;
	_iq28 iqcos;
	_iq28 iqsin;
	_iq22 D;
	_iq22 Q;
	_iq22 Sa;
	_iq22 Sb;
	_iq22 Sc;
}CLARKEPARK;
void DQFun(CLARKEPARK *v);
dq_GLOBALS CLARKEPARK CP_u;
dq_GLOBALS CLARKEPARK CP_i;
#endif

