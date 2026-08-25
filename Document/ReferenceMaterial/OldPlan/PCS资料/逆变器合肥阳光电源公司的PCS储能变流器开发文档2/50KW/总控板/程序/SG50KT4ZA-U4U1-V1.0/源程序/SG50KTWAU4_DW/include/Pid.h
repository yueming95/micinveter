#ifndef _PID_H
#define _PID_H
#include "IQmathLib.h"
#ifdef pid_GLOBALS
#define pid_GLOBALS
#else
#define pid_GLOBALS extern
#endif
//iq21 =-1024~1024
typedef struct
{
	_iq Kp;
	_iq Ki;
	_iq Out;
	_iq OutMax;
	_iq OutMin;
	_iq Err;
	_iq Kc;
	_iq Ref;
	_iq Tf;
	_iq Pi;
}PID;
void PidCalc(PID *v);
pid_GLOBALS PID UPid;
#endif

