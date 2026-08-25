#define dq_GLOBALS
#include "IQmathLib.h"         // Include header for IQmath library 
#include "DQ.h"
#include "Define.h"
//三相静止坐标系到两相旋转坐标系变换函数，幅值不变即D与Sa的幅值相等，Q为0
void DQFun(CLARKEPARK *v)
{
	v->iqcos=_IQ28cos(v->Theta);
	v->iqsin=_IQ28sin(v->Theta);
	v->D = _IQ22mpy(_IQ28mpy(_2DIV3, v->iqcos)>>6, v->Sa) -
		  _IQ22mpy((_IQ28mpy(_1DIV3, v->iqcos) - _IQ28mpy(_1SQRT3, v->iqsin))>>6, v->Sb) -
		  _IQ22mpy((_IQ28mpy(_1DIV3, v->iqcos) + _IQ28mpy(_1SQRT3, v->iqsin))>>6, v->Sc);
	v->Q = - _IQ22mpy(_IQ28mpy(_2DIV3, v->iqsin)>>6, v->Sa) +
		  _IQ22mpy((_IQ28mpy(_1DIV3, v->iqsin) + _IQ28mpy(_1SQRT3, v->iqcos))>>6, v->Sb) +
		  _IQ22mpy((_IQ28mpy(_1DIV3, v->iqsin) - _IQ28mpy(_1SQRT3, v->iqcos))>>6, v->Sc);
}



