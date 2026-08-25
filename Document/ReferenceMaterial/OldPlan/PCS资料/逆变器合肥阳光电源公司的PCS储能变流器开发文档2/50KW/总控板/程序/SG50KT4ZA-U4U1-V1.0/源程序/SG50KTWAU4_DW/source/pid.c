#define pid_GLOBALS
#include "pid.h"
#pragma CODE_SECTION(PidCalc, "ramfuncs");
void PidCalc(PID *v)
{
	v->Err=v->Ref-v->Tf; //error
	if(v->Err>=_IQ(50))  v->Err=_IQ(50);
	if(v->Err<=_IQ(-5)) v->Err=_IQ(-5);
	v->Out=(v->Pi+_IQmpy(v->Kp,v->Err)); //output
	
	if(v->Out>v->OutMax)
	{
	//	v->Pi=v->Pi+_IQmpy(v->Ki,v->Err)+_IQmpy(v->Kc,(v->OutMax-v->Out));
		v->Out=v->OutMax;
	}else if(v->Out<v->OutMin)
	{
	//	v->Pi =v->Pi+_IQmpy(v->Ki,v->Err)+_IQmpy(v->Kc,(v->OutMin-v->Out));
		v->Out=v->OutMin;
	}
	else
	{
		v->Pi=v->Pi+_IQmpy(v->Ki,v->Err);
	}
}

