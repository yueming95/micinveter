#define island_GLOBALS
#include "island.h"
#include "Pid.h"
#include "DQ.h"
#include "Define.h"
void IslandFun()
{
	InputSignal.Freq=_IQ22(IsFreq);
//	IsWg=_IQ22mpy(InputSignal.Freq,_IQ22(6.283185307179586476925286766559));
	IsWg=_IQ22mpy((_IQ22mpy(_IQ22(3),InputSignal.Freq)-_IQ22(99.9)),_IQ22(6.283185307179586476925286766559));
	IsPi += _IQ22mpy(IsKi , IsWg);  		//对角速度进行积分
	if(IsPi>_IQ22(6.283185307179586476925286766559))
	{
		IsPi = IsPi-_IQ22(6.283185307179586476925286766559);
	}
	IsTheta = IsPi;
}


