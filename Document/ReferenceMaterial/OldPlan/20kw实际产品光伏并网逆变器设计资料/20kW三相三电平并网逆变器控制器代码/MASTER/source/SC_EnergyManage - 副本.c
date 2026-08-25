/***********************************************************************
 *    FILENAME : SL_EnergyManager.c
 *
 *    PURPOSE  : Manage energy.
 *    HISTORY  :
 *           DATE           VERSION      AUTHOR         NOTE
 ************************************************************************/
#include "SC_master.h"

/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void PInvReconnectInit(void)
{
	g_GridManager.InvCon.u16ConnectTime = 30;//-->s
	g_GridManager.InvCon.f32ConnectSpeed = 250;//20;
	g_GridManager.InvCon.u16ReconnectTime = 60;
	g_GridManager.InvCon.f32ReconnnetSpeed = 10;
	g_GridManager.InvCon.f32VOback = 250;
	g_GridManager.InvCon.f32VUback = 220;
	g_GridManager.InvCon.f32FOback = 50.1;
	g_GridManager.InvCon.f32VUback = 49.9;
	g_GridManager.InvCon.u16PLimit = g_Rated.f32OutputPower;
}

void PActiveLimitInit(void)
{
	g_SafetyActive.u16Enable = 1;
	g_SafetyActive.f32PLimitCommand = g_Rated.f32OutputPower;
	g_SafetyActive.u16PLimit = g_Rated.f32OutputPower;
}
void PFLimitInit(void)
{
	g_GridManager.PFreq.u16Enable = 1;
	g_GridManager.PFreq.f32FreqPoint = 50.2;
	g_GridManager.PFreq.f32Slop = 0.4;
	g_GridManager.PFreq.f32FOback = 50.05;
	g_GridManager.PFreq.f32FUback = 47.5;
	g_GridManager.PFreq.u16BackMode = 0x1;
	g_GridManager.PFreq.u16WaitTime = 3;
	g_GridManager.PFreq.f32BackSpeed = 0.1;
	g_GridManager.PFreq.u16StartFlag = 0;
	g_GridManager.PFreq.u16PowerM =	g_Rated.f32OutputPower;
	g_GridManager.PFreq.u16PowerMback = g_Rated.f32OutputPower;
	g_GridManager.PFreq.u16FPLimit=	g_Rated.f32OutputPower;
}

void InvQInit(void)
{
	g_GridManager.InvQ.u16Enable =0x01;
	g_GridManager.InvQ.f32Cosphi = 1;
	g_GridManager.InvQ.f32Qvar =0;
	g_GridManager.InvQ.f32Cosphi1 = 1.0;
	g_GridManager.InvQ.f32Pwatt1 =0.06 * g_Rated.f32OutputPower;
	g_GridManager.InvQ.f32Cosphi2 = 0.9;
	g_GridManager.InvQ.f32Pwatt2 =0.1 * g_Rated.f32OutputPower;
	g_GridManager.InvQ.f32Cosphi3 = -0.9;
	g_GridManager.InvQ.f32Pwatt3 =0.2 * g_Rated.f32OutputPower;
	g_GridManager.InvQ.f32Cosphi4 = -0.9;
	g_GridManager.InvQ.f32Pwatt4 = 1.0 * g_Rated.f32OutputPower;//100%Pn
	g_GridManager.InvQ.f32LockinV = 300;//341.5;//1.05Upn;
	g_GridManager.InvQ.f32LockoutV = 280;//325.2;//1.0Upn;
	g_GridManager.InvQ.f32U1s = 351.2;//248.4;//1.08Upn;
	g_GridManager.InvQ.f32U2s = 357.7;//253;//1.2Upn;
	g_GridManager.InvQ.f32U1i = 299.2;//211.6;//0.92Upn;
	g_GridManager.InvQ.f32U2i = 292.7;//207;//0.9Upn;
	g_GridManager.InvQ.f32LockinP = 0.2 * g_Rated.f32OutputPower;//0.20Pn
	g_GridManager.InvQ.f32LockoutP = 0.05 * g_Rated.f32OutputPower;
	g_GridManager.InvQ.f32QvarMax = 0.4845 * g_Rated.f32OutputPower;
	g_GridManager.InvQ.f32QTime = 0.004843;//2s//0.4843/2000*20
	g_GridManager.InvQ.f32Tanphi =0;
	g_GridManager.InvQ.u16LockFlag = 0;	
	g_GridManager.InvQ.u16QULockFlag = 0; 
}

void LvrtInit(void)
{
	g_GridManager.Lvrt.u16Enable = 0x00;
	g_GridManager.Lvrt.f32VLvrt = 292.7;//207;//0.9Un
	g_GridManager.Lvrt.u16Vpoint1 = 0;
	g_GridManager.Lvrt.u16Tpoint1 = 150;//*2 ms
	g_GridManager.Lvrt.u16Vpoint2 = 97.6;//69;//0.3Un
	g_GridManager.Lvrt.u16Tpoint2 = 0;//*2 ms
	g_GridManager.Lvrt.u16Vpoint3 = 97.6;//69;
	g_GridManager.Lvrt.u16Tpoint3 = 500;//*2 ms
	g_GridManager.Lvrt.u16Vpoint4 = 292.7;//207;
	g_GridManager.Lvrt.u16Tpoint4 = 0;//*2 ms
	g_GridManager.Lvrt.u16K = 0.2;
	g_GridManager.Lvrt.u16Tback = 100;//*2ms
	g_GridManager.Lvrt.u16PLvrtback = 250;//0.0005 * g_Rated.f32OutputPower;//25%Pn/s
	g_GridManager.Lvrt.u16StartFlag = 0;
	g_GridManager.Lvrt.u16LvrtNormalDelay = 0;
	g_GridManager.Lvrt.f32VdposFltered = 320;
	g_GridManager.Lvrt.f32IqSave = 0;
	g_GridManager.Lvrt.f32IqRef = 0;
	g_GridManager.Lvrt.f32PLvrtLimit = g_Rated.f32OutputPower;
}

void GridManagerInit(void)
{
	PInvReconnectInit();
	PActiveLimitInit();
	PFLimitInit();
	InvQInit();
	LvrtInit();
}

/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
float32 Line(float32 x,float32 x1,float32 y1,float32 x2,float32 y2)
{
	float32 y=0;
	if(0 == (x2-x1))
	{
		y=y1;
	}
	else
	{
		y=(y2-y1)/(x2-x1) * x + y1;
	}
	return y;
}

/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void PConnectLimit(void)
{ 
	if(NormalState == g_SysCurrentState)
	{
		if((g_PActiveLimit.f32PActiveSoftStart + g_f32PowerLoadSpeed)< g_Rated.f32OutputPower)
		{
			g_PActiveLimit.f32PActiveSoftStart += g_f32PowerLoadSpeed;
		}
		else
		{
			g_PActiveLimit.f32PActiveSoftStart = g_Rated.f32OutputPower;
			g_f32PowerLoadSpeed = g_SafetyConnect.f32PupSlop;	
		}
	}
	else
	{
		g_PActiveLimit.f32PActiveSoftStart = g_Rated.f32OutputPower * 0.02;
	}
	//g_PActiveLimit.f32PActiveSoftStart = g_Rated.f32OutputPower;
}

/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void PTempetureLimit(void)
{	
	int16 TInvMax = 0;
	int16 TBoostMax = 0;
	int16 TAmb = 0;
	float32 TInvPLimit = 0;
	float32 TBoostPLimit = 0;
	float32 TAmbPLimit = 0;
	float32 PLimit = 0;

	TAmb = g_i16_Temperature_Amb;
	TInvMax = g_i16_Temperature_Inv;
	TBoostMax = g_i16_Temperature_Boost;
//	TAmb = 80;//test power limit
	if(TAmb > g_Rated.i16TAmbDratingPoint)// 45degree ambient,272 grid volt. get 70.
	{
    	TAmbPLimit = g_Rated.f32OutputPower - (TAmb- g_Rated.i16TAmbDratingPoint) * g_Rated.f32OutputPower*0.1;
		g_StateCheck.bit.OverTempDerating = 1;
	}
	else
	{
		TAmbPLimit = g_Rated.f32OutputPower;
	}

	if(	TInvMax > g_Rated.i16TInvDratingPoint)
	{
		TInvPLimit = g_Rated.f32OutputPower - (TInvMax- g_Rated.i16TInvDratingPoint) * g_Rated.f32OutputPower*0.05;
		g_StateCheck.bit.OverTempDerating = 1;
	}
	else
	{
		TInvPLimit = g_Rated.f32OutputPower;
	}
	if( TBoostMax >g_Rated.i16TNTCDratingPoint)//
	{
		TBoostPLimit = g_Rated.f32OutputPower - (TBoostMax- g_Rated.i16TNTCDratingPoint) * g_Rated.f32OutputPower*0.06;
		g_StateCheck.bit.OverTempDerating = 1;
	}
	else
	{
		TBoostPLimit = g_Rated.f32OutputPower;
	}

	PLimit = MIN(TInvPLimit , TBoostPLimit);
	PLimit = MIN(PLimit , TAmbPLimit);
	if(PLimit == g_Rated.f32OutputPower)
	{
		g_StateCheck.bit.OverTempDerating = 0;
	}
	if(PLimit < g_Rated.f32OutputPower * 0.2)
	{
		PLimit = g_Rated.f32OutputPower * 0.2;
	}
//	PLimit = 20000;  //临时数据
	g_PActiveLimit.f32SmaxTemperature = PLimit; 
}

/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void PInputLimit(void)
{
	float32 PLimit;

	if(1 == g_MPPTState.bit.PV1ON)
	{
		if(g_CalcResult.Ave.f32VPV1 > 840)
		{
			PLimit = g_Rated.f32HalfInputPower - (g_CalcResult.Ave.f32VPV1 -840) * g_Rated.f32HalfInputPower * 0.006;
		}
		else if(g_CalcResult.Ave.f32VPV1 > g_Rated.f32VinUnderVolt)
		{
			PLimit = g_CalcResult.Ave.f32VPV1 * (g_Rated.f32InputCurrent -0.2);
			if(PLimit > g_Rated.f32HalfInputPower)
			{
				PLimit = g_Rated.f32HalfInputPower;
			}
		}
		else
		{
			PLimit = g_CalcResult.Ave.f32VPV1 * g_CalcResult.Ave.f32VPV1 * g_Rated.f32VinUnderDerated;//440 *24*(V*V/440/440)
		}
		if(PLimit < 2000)
		{
			PLimit = 2000;
		}
	}
	else
	{
		PLimit = 1000;
	}
	g_PActiveLimit.f32Pinput1 = PLimit;//10500;
	
	if (1 == g_MPPTState.bit.PV2ON)
	{
		if(g_CalcResult.Ave.f32VPV2 > 840)
		{
			PLimit = g_Rated.f32HalfInputPower - (g_CalcResult.Ave.f32VPV2 -840) * g_Rated.f32HalfInputPower * 0.006;
		}
		else if(g_CalcResult.Ave.f32VPV2 > g_Rated.f32VinUnderVolt)
		{
			PLimit = g_CalcResult.Ave.f32VPV2 * (g_Rated.f32InputCurrent - 0.2);
			if(PLimit > g_Rated.f32HalfInputPower)
			{
				PLimit = g_Rated.f32HalfInputPower;
			}
		}
		else
		{
			PLimit = g_CalcResult.Ave.f32VPV2 * g_CalcResult.Ave.f32VPV2 * g_Rated.f32VinUnderDerated;//440 *24*(V*V/440/440)
		}
		if(PLimit < 2000)
		{
			PLimit = 2000;
		}
	}
	else
	{
		PLimit = 1000;
	}
	g_PActiveLimit.f32Pinput2 = PLimit;//10500;

	PLimit = g_PActiveLimit.f32Pinput1 + g_PActiveLimit.f32Pinput2;
	if(PLimit > g_Rated.f32InputPower)
	{
		PLimit = g_Rated.f32InputPower;
	}
	g_PActiveLimit.f32PinputAll = PLimit;//10500;
}

/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void PActiveCommandLimit(void)
{
	float32 f32Temp;

	if(1 == g_SaftyConfig.bit.RemoteDeratingEnable)
	{
		 f32Temp= g_SafetyActive.f32PLimitCommand * g_Rated.f32OutputPower;
	}
	else
	{
		f32Temp = g_Rated.f32OutputPower;
	}
	if((g_CalcResult.Power.f32OutputWatt - g_Rated.f32OutputPower * 0.02)> f32Temp)
	{
		g_PActiveLimit.f32PActiveCommand =	g_CalcResult.Power.f32OutputWatt - g_Rated.f32OutputPower * 0.018;
	}
	else
	{
		g_PActiveLimit.f32PActiveCommand = f32Temp;	
	}
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void PFWaitBack(void)
{
	static Uint16 TimeWaitcnt =0;

	if((g_CalcResult.Ave.f32FGrid > g_GridManager.PFreq.f32FUback) && (g_CalcResult.Ave.f32FGrid < g_GridManager.PFreq.f32FOback))
	{
		TimeWaitcnt++;
		if(TimeWaitcnt > g_GridManager.PFreq.u16WaitTime)
		{
			if(0x04 == g_GridManager.PFreq.u16BackMode)//CEI0-16
			{
				g_GridManager.PFreq.u16PowerMback = g_GridManager.PFreq.u16PowerM - g_CalcResult.Power.f32OutputWatt;
			}
			else
			{
				g_GridManager.PFreq.u16PowerMback = g_GridManager.PFreq.u16PowerM;
			}
			g_GridManager.PFreq.u16StartFlag = 0;			
		}
	}
	else
	{
		TimeWaitcnt =0;
	}
}

void PFreqPback(void)
{
	float32 f32PBackSpeedTemp;
	Uint16  u16OverTime;
	static Uint16 u16OverTimecnt = 0;

	if(0x04 == g_GridManager.PFreq.u16BackMode)//CEI0-16
	{
		if(g_GridManager.PFreq.u16PowerMback > 0.25 * g_Rated.f32OutputPower)
		{
			f32PBackSpeedTemp = 0.2 *  g_GridManager.PFreq.u16PowerMback / g_Rated.f32OutputPower;
		}
		else
		{
			f32PBackSpeedTemp = g_GridManager.PFreq.f32BackSpeed;
		}
	}
	else if(0x02 == g_GridManager.PFreq.u16BackMode)//CEI0-21
	{
		if((g_GridManager.PFreq.u16PowerMback > g_CalcResult.Power.f32OutputWatt))
		{
			u16OverTime = (Uint16)((float32)(g_GridManager.PFreq.u16PowerMback / g_Rated.f32OutputPower) * 60000) + 1500; //1500 -->0.5min
			u16OverTimecnt++;

			if(u16OverTimecnt > u16OverTime)
			{
				g_GridManager.PFreq.u16PowerMback = 0;
			}

			if(g_GridManager.PFreq.u16PowerMback > 0.25 * g_Rated.f32OutputPower)
			{
				f32PBackSpeedTemp = 0.2 *  g_GridManager.PFreq.u16PowerMback / g_Rated.f32OutputPower;
			}
			else
			{
				f32PBackSpeedTemp = 0.05;
			}
		}
		else
		{
			u16OverTimecnt = 0;
			f32PBackSpeedTemp = g_GridManager.PFreq.f32BackSpeed;
		}
	}
	else
	{
		f32PBackSpeedTemp = g_GridManager.PFreq.f32BackSpeed;
	}
			
	g_GridManager.PFreq.u16FPLimit += (f32PBackSpeedTemp * g_Rated.f32OutputPower * 0.00033334);//0.3333*1.001-->20ms
							
	if(g_GridManager.PFreq.u16FPLimit >= g_Rated.f32OutputPower)
	{
		g_GridManager.PFreq.u16FPLimit = g_Rated.f32OutputPower;
	}
}

//g_GridManager.PFreq.u16PowerM   : 频率降载前的功率，
//g_GridManager.PFreq.u16FPLimit  :频率降载过程中的功率限制值，
//g_GridManager.PFreq.u16PowerMback: 频率恢复后，预计要恢复的功率
void PFreqLimit(void)
{
	Uint16 PLimitTemp = g_Rated.f32OutputPower;
	static Uint16 OFreqcnt = 2;

	if(g_CalcResult.Ave.f32FGrid > g_GridManager.PFreq.f32FreqPoint)
	{
		OFreqcnt++;
		if(OFreqcnt > 5)
		{
			OFreqcnt = 5;
			g_GridManager.PFreq.u16StartFlag =1;
			g_GridManager.PFreq.u16PowerMback =	g_Rated.f32OutputPower;	// ???
			//calculate power according power-fre curve			
			PLimitTemp = g_GridManager.PFreq.u16PowerM  * (1 - (g_CalcResult.Ave.f32FGrid - g_GridManager.PFreq.f32FreqPoint) * g_GridManager.PFreq.f32Slop);
			//get minimum power at power-fre curve when fre derating
			if(PLimitTemp < 50)
			{
				PLimitTemp = 50;
			}
			//if(	0x01 == g_GridManager.PFreq.u16BackMode)//4105
			//{
				g_GridManager.PFreq.u16FPLimit = PLimitTemp;
			//}
			//else
			//{
			//	if(g_GridManager.PFreq.u16FPLimit > PLimitTemp)
			//	{
			//		g_GridManager.PFreq.u16FPLimit = PLimitTemp;
			//	}
			//}
		}
	}
	else
	{
		OFreqcnt--;
		if(OFreqcnt < 2)
		{
			OFreqcnt=1;
		
			if(0 ==g_GridManager.PFreq.u16StartFlag)
			{
				//why do not write in following?
				g_GridManager.PFreq.u16PowerM = g_CalcResult.Power.f32OutputWatt;
//				if((g_CalcResult.Power.f32OutputWatt < (g_GridManager.PFreq.u16PowerM - 200))||(g_CalcResult.Power.f32OutputWatt > g_GridManager.PFreq.u16PowerM))
//				{
//					g_GridManager.PFreq.u16PowerM = g_CalcResult.Power.f32OutputWatt;
//				}
//				else
//				{
//				}

				PFreqPback();
			}
			else
			{
				PFWaitBack();
			}
		}
	}
}

/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/

void ConstCosphi(void)
{
	float32 f32Tanphi = 0;
	float32 f32DeltaTanphi = 0;

	f32Tanphi = sqrt(1-g_GridManager.InvQ.f32Cosphi * g_GridManager.InvQ.f32Cosphi)/g_GridManager.InvQ.f32Cosphi;
	
	f32DeltaTanphi = f32Tanphi - g_GridManager.InvQ.f32Tanphi;

	if(	f32DeltaTanphi > 0.00483)
	{
		g_GridManager.InvQ.f32Tanphi +=0.00483;
	}
	else if(f32DeltaTanphi < -0.00483)
	{
		g_GridManager.InvQ.f32Tanphi -=0.00483;
	}
	else
	{
		g_GridManager.InvQ.f32Tanphi = f32Tanphi;
	}

	if(g_GridManager.InvQ.f32Tanphi > 0.4845)
	{
		g_GridManager.InvQ.f32Tanphi = 0.4845;
	}
	else if(g_GridManager.InvQ.f32Tanphi < -0.4845)
	{
		g_GridManager.InvQ.f32Tanphi = -0.4845;
	}
	else
	{
	} 

}

void ConstQvar(void)
{
	float32 f32Tanphi = 0;
	float32 f32DeltaTanphi = 0;

	if(g_GridManager.InvQ.f32Qvar > 0.4845 * g_Rated.f32OutputPower)
	{
		g_GridManager.InvQ.f32Qvar = 0.4845 * g_Rated.f32OutputPower;	
	}
	else if(g_GridManager.InvQ.f32Qvar < -0.4845 * g_Rated.f32OutputPower)
	{
		g_GridManager.InvQ.f32Qvar = -0.4845 * g_Rated.f32OutputPower;
	}
	f32Tanphi = g_GridManager.InvQ.f32Qvar / g_CalcResult.Power.f32OutputWatt;

	f32DeltaTanphi = f32Tanphi - g_GridManager.InvQ.f32Tanphi;
	if(	f32DeltaTanphi > 0.00243)
	{
		g_GridManager.InvQ.f32Tanphi +=0.00243;
	}
	else if(f32DeltaTanphi < -0.00243)
	{
		g_GridManager.InvQ.f32Tanphi -=0.00243;
	}
	else
	{
		g_GridManager.InvQ.f32Tanphi = f32Tanphi;
	}
			
}

void QLineMode(void)
{
	float32 f32Tanphi = 0;
	float32 f32Cosphitemp = 1;
	float32 f32DeltaTanphi = 0;

	if(g_PLLCal.f32Vd_Pos > g_GridManager.InvQ.f32LockinV)
	{
		g_GridManager.InvQ.u16LockFlag = 1;
	}
	else if(g_PLLCal.f32Vd_Pos < g_GridManager.InvQ.f32LockoutV)
	{
		g_GridManager.InvQ.u16LockFlag = 0;
	}
	else
	{
	}

	if(1 == g_GridManager.InvQ.u16LockFlag)
	{
		if(g_CalcResult.Power.f32OutputWatt <= g_GridManager.InvQ.f32Pwatt1)
		{
			f32Cosphitemp = g_GridManager.InvQ.f32Cosphi1;			
		}
		else if(g_CalcResult.Power.f32OutputWatt <= g_GridManager.InvQ.f32Pwatt2)
		{
			f32Cosphitemp = Line(g_CalcResult.Power.f32OutputWatt,g_GridManager.InvQ.f32Pwatt1,g_GridManager.InvQ.f32Cosphi1,g_GridManager.InvQ.f32Pwatt2,g_GridManager.InvQ.f32Cosphi2);		
		}
		else if(g_CalcResult.Power.f32OutputWatt <= g_GridManager.InvQ.f32Pwatt3)
		{
			f32Cosphitemp = Line(g_CalcResult.Power.f32OutputWatt,g_GridManager.InvQ.f32Pwatt2,g_GridManager.InvQ.f32Cosphi2,g_GridManager.InvQ.f32Pwatt3,g_GridManager.InvQ.f32Cosphi3);		
		}
		else if(g_CalcResult.Power.f32OutputWatt <= g_GridManager.InvQ.f32Pwatt4)
		{
			f32Cosphitemp = Line(g_CalcResult.Power.f32OutputWatt,g_GridManager.InvQ.f32Pwatt3,g_GridManager.InvQ.f32Cosphi3,g_GridManager.InvQ.f32Pwatt4,g_GridManager.InvQ.f32Cosphi4);			
		}
		else
		{
			f32Cosphitemp = g_GridManager.InvQ.f32Cosphi4;
		}
		f32Tanphi = sqrt(1-f32Cosphitemp * f32Cosphitemp)/f32Cosphitemp;
	}
	else
	{
		f32Tanphi = 0;
	}

	f32DeltaTanphi = f32Tanphi - g_GridManager.InvQ.f32Tanphi;
	if(	f32DeltaTanphi > 0.00483)
	{
		g_GridManager.InvQ.f32Tanphi +=0.00483;
	}
	else if(f32DeltaTanphi < -0.00483)
	{
		g_GridManager.InvQ.f32Tanphi -=0.00483;
	}
	else
	{
		g_GridManager.InvQ.f32Tanphi = f32Tanphi;
	}

}

void QUAmode(void)
{
	float32 f32Tanphi = 0;
	float32 f32Qvartemp;
	float32 f32DeltaTanphi = 0;

	if(g_CalcResult.Power.f32OutputWatt > g_GridManager.InvQ.f32LockinP)
	{
		g_GridManager.InvQ.u16LockFlag =1;
	}
	else if(g_CalcResult.Power.f32OutputWatt < g_GridManager.InvQ.f32LockoutP)
	{
		g_GridManager.InvQ.u16LockFlag = 0;
	}
	else
	{
	}

	if(1 == g_GridManager.InvQ.u16LockFlag)
	{
		if(g_PLLCal.f32Vd_Pos >g_GridManager.InvQ.f32U2s)
		{
			f32Qvartemp = g_GridManager.InvQ.f32QvarMax;
		}
		else if(g_PLLCal.f32Vd_Pos >g_GridManager.InvQ.f32U1s)
		{
			f32Qvartemp = Line(g_PLLCal.f32Vd_Pos,g_GridManager.InvQ.f32U1s,0,g_GridManager.InvQ.f32U2s,g_GridManager.InvQ.f32QvarMax);
		}
		else if(g_PLLCal.f32Vd_Pos >g_GridManager.InvQ.f32U1i)
		{
			f32Qvartemp =0;
		}
		else if(g_PLLCal.f32Vd_Pos >g_GridManager.InvQ.f32U2i)
		{
			f32Qvartemp = Line(g_PLLCal.f32Vd_Pos,g_GridManager.InvQ.f32U1i,0,g_GridManager.InvQ.f32U2i,g_GridManager.InvQ.f32QvarMax);	
		}
		else
		{
			f32Qvartemp = - g_GridManager.InvQ.f32QvarMax;
		}	
		f32Tanphi = f32Qvartemp/g_CalcResult.Power.f32OutputWatt;
	}
	else
	{
		f32Tanphi = 0;
	}
	
	f32DeltaTanphi = f32Tanphi - g_GridManager.InvQ.f32Tanphi;
	if(	f32DeltaTanphi > g_GridManager.InvQ.f32QTime)
	{
		g_GridManager.InvQ.f32Tanphi +=g_GridManager.InvQ.f32QTime;
	}
	else if(f32DeltaTanphi < -g_GridManager.InvQ.f32QTime)
	{
		g_GridManager.InvQ.f32Tanphi -=g_GridManager.InvQ.f32QTime;
	}
	else
	{
		g_GridManager.InvQ.f32Tanphi = f32Tanphi;
	}
	
}

void QUBmode(void)
{
	float32 f32Tanphi = 0;
	float32 f32Qvartemp = 0;
	float32 f32DeltaTanphi = 0;
	
	if(g_CalcResult.Power.f32OutputWatt > g_GridManager.InvQ.f32LockinP)
	{
		g_GridManager.InvQ.u16LockFlag = 1;
	}
	else if(g_CalcResult.Power.f32OutputWatt < g_GridManager.InvQ.f32LockoutP)
	{
		g_GridManager.InvQ.u16LockFlag = 0;
	}
	else
	{
	}

	if(1 == g_GridManager.InvQ.u16LockFlag)
	{
		if(g_PLLCal.f32Vd_Pos >g_GridManager.InvQ.f32U2s)
		{
			g_GridManager.InvQ.u16QULockFlag = 1;
		}
		else if((g_PLLCal.f32Vd_Pos <= g_GridManager.InvQ.f32U1s)&&(g_PLLCal.f32Vd_Pos >= g_GridManager.InvQ.f32U1i))
		{
			g_GridManager.InvQ.u16QULockFlag = 0;
		}
		else if(g_PLLCal.f32Vd_Pos <g_GridManager.InvQ.f32U2i)
		{
			g_GridManager.InvQ.u16QULockFlag = 2;
		}
		else
		{
		}
		
		if(1 == g_GridManager.InvQ.u16QULockFlag)
		{
			f32Qvartemp = g_GridManager.InvQ.f32QvarMax;
		}
		else if(2 == g_GridManager.InvQ.u16QULockFlag)
		{
			f32Qvartemp = - g_GridManager.InvQ.f32QvarMax;
		}
		else
		{
			f32Qvartemp = 0;
		}

		f32Tanphi = f32Qvartemp/g_CalcResult.Power.f32OutputWatt;
	}
	else
	{
		f32Tanphi = 0;
		g_GridManager.InvQ.u16QULockFlag = 0;
	}

	f32DeltaTanphi = f32Tanphi - g_GridManager.InvQ.f32Tanphi;

	if(	f32DeltaTanphi > g_GridManager.InvQ.f32QTime)
	{
		g_GridManager.InvQ.f32Tanphi +=g_GridManager.InvQ.f32QTime;
	}
	else if(f32DeltaTanphi < -g_GridManager.InvQ.f32QTime)
	{
		g_GridManager.InvQ.f32Tanphi -=g_GridManager.InvQ.f32QTime;
	}
	else
	{
		g_GridManager.InvQ.f32Tanphi = f32Tanphi;
	}

}

void InvQvar(void)
{
	static Uint16 InvModeOld = 0;
	static Uint16 InvModeNew = 0;

	InvModeOld = InvModeNew;
	InvModeNew = g_GridManager.InvQ.u16Enable;
	if(InvModeOld != InvModeNew)
	{
		g_GridManager.InvQ.u16LockFlag = 0;//all data initial
		g_GridManager.InvQ.u16QULockFlag = 0;
	}

	switch(g_GridManager.InvQ.u16Enable)
	{
		case 0x01:ConstCosphi();break;
		case 0x02:ConstQvar();break;
		case 0x04:QLineMode();break;
		case 0x08:QUAmode();break;
		case 0x10:QUBmode();break;
		default:g_GridManager.InvQ.f32Tanphi = 0;break;
	}

	if(g_CalcResult.Power.f32OutputWatt < 0.02 * g_Rated.f32OutputPower)
	{
		g_GridManager.InvQ.f32Tanphi = 0;
	}
	g_GridManager.InvQ.f32Tanphi = 0;
}

/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/

void LvrtUnblancecheck(void)
{
	static Uint16  u16VNegcnt = 0;
	static Uint16  u16VNegcnttemp = 0;
	static Uint16  u16VNegcnttemp2 = 0;

	if(0 == g_GridManager.Lvrt.u16StartFlag)
	{
		if((g_PLLCal.f32VNeg > 3600) && (g_GridManager.Lvrt.u16LvrtNormalDelay > 4999) && (NormalState == g_SysCurrentState))
		{
			if(0 == (g_GridManager.Lvrt.u16StartFlag & 0x02))
			{
				if(g_GridManager.Lvrt.f32VdposFltered < 50)
				{
					u16VNegcnt = 400;	
				}
				else
				{
					u16VNegcnt = 100;
				}
				u16VNegcnttemp ++;
				if(	u16VNegcnttemp > u16VNegcnt	)
				{
					g_GridManager.Lvrt.u16StartFlag |=0x02;
					u16VNegcnttemp = 0;
					u16VNegcnttemp2 = 0;
				}
			}
		}
		else
		{
			u16VNegcnttemp =0;
		}
	}

	if(0x02 == (g_GridManager.Lvrt.u16StartFlag & 0x02))
	{
		g_GridManager.Lvrt.f32IqRef = 0.5;
		g_GridManager.Lvrt.f32PLvrtLimit = 0.02 * g_Rated.f32OutputPower;

		if((g_PLLCal.f32VNeg < 1600)&& (NormalState == g_SysCurrentState))
		{
			u16VNegcnttemp2++;
			if(	u16VNegcnttemp2 > 15)
			{
				g_GridManager.Lvrt.u16StartFlag &=0x01;	
				u16VNegcnttemp = 0;
				u16VNegcnttemp2 = 0;
			}
		}
	}
	else
	{
		u16VNegcnttemp2 =0;
	}

}

void LvrtVdcalc()
{
	static float32 f32VdposSum = 0;
	static Uint16  u16Vdposcnt =0;

	f32VdposSum += g_PLLCal.f32Vd_Pos;
	u16Vdposcnt ++;

	if(u16Vdposcnt >=50)
	{
		g_GridManager.Lvrt.f32VdposFltered = f32VdposSum * 0.02; 
		u16Vdposcnt =0;
		f32VdposSum =0;	
	}

	if(NormalState == g_SysCurrentState)
	{
		LvrtUnblancecheck();
	}
	else
	{
		g_GridManager.Lvrt.u16StartFlag = 0;	
	}
}

void LvrtVlowcheck(void)
{
	static Uint16 u16pointcnt = 0;
	static Uint16 u16cnt1 = 0;
	static Uint16 u16cnt2 = 0;
	static Uint16 u16cnt3 = 0;
	static Uint16 u16cnt4 = 0;
	static Uint16 u16cnt5 = 0;
	static Uint16 u16cnt6 = 0;
	static Uint16 u16cnt7 = 0;
	static Uint16 u16backflag =0;
	static Uint16 u16backcnt = 0;
	float32 f32VPointtemp;

	if(0 != g_GridManager.Lvrt.u16StartFlag)
	{
		u16pointcnt++;
		if(u16pointcnt <=g_GridManager.Lvrt.u16Tpoint1)
		{
			if(g_GridManager.Lvrt.f32VdposFltered < g_GridManager.Lvrt.u16Vpoint1)
			{
				u16cnt1++;
				if(u16cnt1 > 5)
				{
					g_SysFaultMessage.bit.VLvrtlow =1;	
				}
			}
			else
			{
				u16cnt1 = 0;
			}
		}
		else if(u16pointcnt <=g_GridManager.Lvrt.u16Tpoint2)
		{
			f32VPointtemp = Line(u16pointcnt,g_GridManager.Lvrt.u16Tpoint1,g_GridManager.Lvrt.u16Vpoint1,g_GridManager.Lvrt.u16Tpoint2,g_GridManager.Lvrt.u16Vpoint2);		
			if(g_GridManager.Lvrt.f32VdposFltered < f32VPointtemp)
			{
				u16cnt2++;
				if(u16cnt2 > 5)
				{
					g_SysFaultMessage.bit.VLvrtlow =1;
				}
			}
			else
			{
				u16cnt2 = 0;	
			}
			
		}
		else if(u16pointcnt <=g_GridManager.Lvrt.u16Tpoint3)
		{
			f32VPointtemp = Line(u16pointcnt,g_GridManager.Lvrt.u16Tpoint2,g_GridManager.Lvrt.u16Vpoint2,g_GridManager.Lvrt.u16Tpoint3,g_GridManager.Lvrt.u16Vpoint3);		
			if(g_GridManager.Lvrt.f32VdposFltered < f32VPointtemp)
			{
				u16cnt3++;
				if(u16cnt3 > 5)
				{
					g_SysFaultMessage.bit.VLvrtlow =1;
				}
			}
			else
			{
				u16cnt3 = 0;	
			}
		}
		else if(u16pointcnt <=g_GridManager.Lvrt.u16Tpoint4)
		{
			f32VPointtemp = Line(u16pointcnt,g_GridManager.Lvrt.u16Tpoint3,g_GridManager.Lvrt.u16Vpoint3,g_GridManager.Lvrt.u16Tpoint4,g_GridManager.Lvrt.u16Vpoint4);		
			if(g_GridManager.Lvrt.f32VdposFltered < f32VPointtemp)
			{
				u16cnt4++;
				if(u16cnt4 > 5)
				{
					g_SysFaultMessage.bit.VLvrtlow =1;
				}
			}
			else
			{
				u16cnt4 =0;		
			}
		}
		else
		{
			if(g_GridManager.Lvrt.f32VdposFltered < g_GridManager.Lvrt.u16Vpoint4)
			{
				g_SysFaultMessage.bit.VLvrtlow =1;	
			}
			u16cnt1 = 0;
			u16cnt2 = 0;
			u16cnt3 = 0;
			u16cnt4 = 0;				
		}
		u16cnt5 =0;	
		
		if(g_GridManager.Lvrt.f32VdposFltered > g_GridManager.Lvrt.f32VLvrt)
		{
			u16cnt7++;
			if(u16cnt7 > 20)
			{
				g_GridManager.Lvrt.u16StartFlag &= 0x02;
				u16backflag = 1;
			}
		}
		else
		{
			u16cnt7 = 0;
		}	
	}
	else
	{
		u16pointcnt = 0;
		u16cnt1 = 0;
		u16cnt2 = 0;
		u16cnt3 = 0;
		u16cnt4 = 0;
			
		if(1 == g_SysFaultMessage.bit.VLvrtlow)
		{ 
			u16cnt5 ++;
			if(u16cnt5 > 1500)
			{
				g_SysFaultMessage.bit.VLvrtlow = 0;	
			}
		}
		else
		{
			u16cnt5 = 0;
		}

		if(g_GridManager.Lvrt.f32VdposFltered < g_GridManager.Lvrt.f32VLvrt)
		{
			u16cnt6 ++;
			if(u16cnt6 > 3)
			{
				u16cnt6 = 0;
				g_GridManager.Lvrt.u16StartFlag |=0x01;
				g_GridManager.Lvrt.f32IqSave = g_BusPIDCal.f32IqRef;
				g_GridManager.Lvrt.f32IqRef = g_GridManager.Lvrt.f32IqSave;
				g_GridManager.Lvrt.f32PLvrtLimit = 0.02 * g_Rated.f32OutputPower;				
			}
		}
		else
		{
			u16cnt6 = 0;	
		}

		if(0x01 == u16backflag)
		{
			u16backcnt++;
			if(u16backcnt > g_GridManager.Lvrt.u16Tback)
			{
				u16backflag = 0;
			}
			g_GridManager.Lvrt.f32PLvrtLimit = 0.02 * g_Rated.f32OutputPower;		
		}
		else
		{
		
			g_GridManager.Lvrt.f32PLvrtLimit += g_GridManager.Lvrt.u16PLvrtback;
			if(g_GridManager.Lvrt.f32PLvrtLimit > g_Rated.f32OutputCurrentPeak)
			{
				g_GridManager.Lvrt.f32PLvrtLimit = g_Rated.f32OutputCurrentPeak;
			} 		
		}
	}
}

void LvrtIdq(void)
{
	float32 f32DeltaV = 0;
	float32 f32DeltaIq = 0;
	float32 f32LvrtIq = 0;

	if(0x01 == g_GridManager.Lvrt.u16StartFlag)
	{
		f32DeltaV = (325.2f - g_GridManager.Lvrt.f32VdposFltered)/325.2f;
		
		if(FABS(f32DeltaV)< 0.1)
		{
			f32DeltaV = 0;
		}

		f32DeltaIq = f32DeltaV * g_GridManager.Lvrt.u16K * g_Rated.f32OutputCurrentPeak;
		f32LvrtIq = f32DeltaIq + g_GridManager.Lvrt.f32IqSave;

		if(f32LvrtIq > g_Rated.f32OutputCurrentPeak * 0.99)
		{
			f32LvrtIq = g_Rated.f32OutputCurrentPeak * 0.99;	
		}
		else if(f32LvrtIq < -g_Rated.f32OutputCurrentPeak * 0.99)
		{
			f32LvrtIq = -g_Rated.f32OutputCurrentPeak * 0.99;
		}
		else
		{
		}

		if( (f32LvrtIq - g_GridManager.Lvrt.f32IqRef) > 0.2 * g_Rated.f32OutputCurrentPeak)
		{
			g_GridManager.Lvrt.f32IqRef += 0.2 * g_Rated.f32OutputCurrentPeak;
		}
		else if( (f32LvrtIq - g_GridManager.Lvrt.f32IqRef) < -0.2 * g_Rated.f32OutputCurrentPeak)
		{
			g_GridManager.Lvrt.f32IqRef -= 0.2 * g_Rated.f32OutputCurrentPeak;
		}
		else
		{
			g_GridManager.Lvrt.f32IqRef = f32LvrtIq;
		}
		g_GridManager.Lvrt.f32PLvrtLimit = 0.02 * g_Rated.f32OutputPower;
	}
}

void LvrtManager(void)
{
	LvrtVlowcheck();
	LvrtIdq();	
}

/**********************************************************************
* FUNCION :  POutputVoltLimit() , use the minimum of f32PActiveVo and g_PActiveLimit.f32STemprature
* CALLED BY: use the minimum of f32PActiveVo and f32STemprature,  Q command to calac f32PReactiveCommand
**********************************************************************/
void POutputVoltLimit(void)//ê?3?μ?á÷?T??
{
	float32 f32VGridMIN = 0;

	f32VGridMIN = MIN(g_CalcResult.Rms.f32VGridR , g_CalcResult.Rms.f32VGridS);
	f32VGridMIN = MIN(f32VGridMIN , g_CalcResult.Rms.f32VGridT);
	if(f32VGridMIN < 100)
	{
		f32VGridMIN = 100;
	}
	g_PActiveLimit.f32SmaxVo = 3 * f32VGridMIN * g_Rated.f32OutputCurrentRms;
}

/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: OPLoadLimit20ms
**********************************************************************/

void OPLoadLimit20ms(void)
{
	float32 f32Temp1 = 0;
	float32 f32Temp2 =0;
	float32 f32Temp3 =0;
	float32 f32Temp4 =0;

	float32 f32PLimit;

	if(1 == g_SaftyConfig.bit.FreDeratingEnable)
	{
		PFreqLimit();
	}
	else
	{
		g_GridManager.PFreq.u16FPLimit = g_Rated.f32OutputPower;
	}
	g_PActiveLimit.f32PActiveFreq = g_GridManager.PFreq.u16FPLimit;

	POutputVoltLimit();
	PTempetureLimit();
	if(g_PActiveLimit.f32SmaxVo < g_PActiveLimit.f32SmaxTemperature)
	{
		g_PActiveLimit.f32SMaxLimit = g_PActiveLimit.f32SmaxVo;
	}
	else
	{
		g_PActiveLimit.f32SMaxLimit = g_PActiveLimit.f32SmaxTemperature;
	}
	InvQvar();//according to f32SMaxLimit and Q command,
	if(0 == g_SaftyConfig.bit.ReactiveEnable)
	{
		g_PActiveLimit.f32PReactiveCommand = g_PActiveLimit.f32SMaxLimit;
	}

	PConnectLimit();
	PActiveCommandLimit();
	PInputLimit();
	f32Temp1 = g_PActiveLimit.f32PActiveFreq;
	f32Temp2 = g_PActiveLimit.f32PReactiveCommand;// calc according to Temperaturec,output voltage and Q command
	f32Temp3 = g_PActiveLimit.f32PActiveSoftStart;
	f32Temp4 = g_PActiveLimit.f32PActiveCommand;	

	f32PLimit = MIN(f32Temp1,f32Temp2);
	f32PLimit = MIN(f32PLimit,f32Temp3);
	g_PActiveLimit.f32PActiveMaxLimit = MIN(f32PLimit,f32Temp4);

}

/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void OILoadLimit20ms(void)
{
	float32 IPLimit = 0;
	float32 VGridMAX = 0;

	VGridMAX = MAX(g_CalcResult.Rms.f32VGridR , g_CalcResult.Rms.f32VGridS);
	VGridMAX = MAX(VGridMAX , g_CalcResult.Rms.f32VGridT);

	if(VGridMAX<100)
	{
		VGridMAX = 100;
	}
//	IPLimit = g_GridManager.f32PLimit20ms/VGridMIN * 0.471;
	IPLimit = g_PActiveLimit.f32PActiveMaxLimit * 0.471/VGridMAX;
	if(IPLimit > g_Rated.f32OutputCurrentPeak)//
	{
		IPLimit = g_Rated.f32OutputCurrentPeak;//	
	}
	if(IPLimit < 0.2)
	{
		IPLimit = 0.2;
	}

	if(1 == g_MPPTState.bit.FactoryMode)
	{
		if(g_SafetyReg.FactoryCurrentLimit > IPLimit)
		{
			g_BusPIDCal.f32IdRefMax = IPLimit;
		}
		else
		{
			g_BusPIDCal.f32IdRefMax = g_SafetyReg.FactoryCurrentLimit;
		}		

	}
	else
	{
		g_BusPIDCal.f32IdRefMax = IPLimit + 0.5;//IPLimit;
	}
}

//===========================================================================
// No more.
//===========================================================================
