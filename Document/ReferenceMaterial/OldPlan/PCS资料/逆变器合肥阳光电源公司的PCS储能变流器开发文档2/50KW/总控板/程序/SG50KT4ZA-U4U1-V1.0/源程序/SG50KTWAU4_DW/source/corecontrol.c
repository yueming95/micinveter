#define corecontrol_GLOBALS
#include "Define.h"
#include "IQmathLib.h" 
#include "corecontrol.h"
#include "rs485.h"
#include "pid.h"
#include "rsdebug.h"
//#define ADC_usDELAY  5000L
//#define ADC_usDELAY2 20L

#pragma CODE_SECTION(ADCReuslt, "ramfuncs");
#pragma CODE_SECTION(Debugcode, "ramfuncs");
#pragma CODE_SECTION(StateFun, "ramfuncs");
#pragma CODE_SECTION(MpptFun, "ramfuncs");
#pragma CODE_SECTION(TimeFun, "ramfuncs");
#pragma CODE_SECTION(IgdRefCalc,"ramfuncs");
void Debugcode()  //调试代码
{
	otimeb++;
	SysCnt.CycleCnt++;
	if(SysCnt.CycleCnt>=Cycle) //5K时Cycle=100
	{
		SysCnt.CycleCnt=0;
		buf1[SysCnt.CycleCnt] = *bufaddr;	
	}
	//more
}

void MpptFun()   
{
//------------滞环比较的扰动观测法---------------------------------//
	MpptRegs.P=InputFilter.Pin;	//本次计算功率
	MpptRegs.DetaP = MpptRegs.P - MpptRegs.Pb;  	

	MpptRegs.Abccnt++;
	if(MpptRegs.Abccnt>2)
	{
		MpptRegs.Abccnt=0;
		if(MpptRegs.DetaP<MpptRegs.DetaPMin)
		{
			MpptRegs.Tag++;
		}else
		{
			MpptRegs.Tag--;
		}		
		if(MpptRegs.Tag==2)
		{
			MpptRegs.Uout=MpptRegs.Uc;
		}else if(MpptRegs.Tag==-2)
		{
			MpptRegs.Uout=MpptRegs.Ua;
		}else
		{
			MpptRegs.Uout=MpptRegs.Ub;
		}
		MpptRegs.Ub=MpptRegs.Uout;
	}else if(MpptRegs.Abccnt==1)
	{
		MpptRegs.Tag=0;
		MpptRegs.Pb=MpptRegs.P;
		MpptRegs.Uout=MpptRegs.Uout+MpptRegs.Du;   //去c点	电压加步长
		MpptRegs.Uc=MpptRegs.Uout;
	}else			// MpptRegs.Abccnt == 2
	{
		MpptRegs.Uout=MpptRegs.Uout-_IQ15mpy(_IQ15(2),MpptRegs.Du); //去a点
		MpptRegs.Ua=MpptRegs.Uout;
		if(MpptRegs.DetaP<MpptRegs.DetaPMin)
		{
			MpptRegs.Tag--;
		}else
		{
			MpptRegs.Tag++;
		}
	}
	if(MpptRegs.FirstCnt<8)   //首次进入
	{
		MpptRegs.Uout=_IQ15mpy((InputFilter.Upv>>4),_IQ15(0.98));
		MpptRegs.Ub=MpptRegs.Uout;
		MpptRegs.Pb=MpptRegs.P;
		MpptRegs.FirstCnt++;
		MpptRegs.Tag=0;
		MpptRegs.Abccnt=0;
	}

	if(MpptRegs.Uout>_IQ15(850))  MpptRegs.Uout=_IQ15(850);
	else if(MpptRegs.Uout<_IQ15(635)) MpptRegs.Uout=_IQ15(635);
}
void IgdRefCalc()
{
	//AC侧启动	(StateReg.bit.ACState == 3)
	if(StateReg.bit.ACState == 3)
	{
		if(SysCnt.MpptStart<=TM2S)  //启动后1s开始执行mppt
		{
			SysCnt.MpptStart++;
			Udc_Given=InputFilter.Upv-_IQ(3);
			StopReg2.all=0;
		}else
		{
			if(SysCnt.MpptCnt<=TM600MS)    //1S执行一次mppt
			{
				SysCnt.MpptCnt++;
			}else
			{
				MpptFun();
				Udc_Given=(MpptRegs.Uout<<4);
				SysCnt.MpptCnt=0;
			}
		//	Udc_Given=Udc_Given-_IQ(0.002);
		//	if(Udc_Given<_IQ(575)) Udc_Given=_IQ(575);
			UPid.Ref = Udc_Given;
			UPid.Tf= InputFilter.Upv;	// InputSignal.Upv;
			PidCalc(&UPid);
		//	MpptRegs.IgdGiven=MpptRegs.IgdGiven+_IQ(0.0001);
		//	if(MpptRegs.IgdGiven>_IQ(15)) MpptRegs.IgdGiven=_IQ(15);
		}
			
	      MpptRegs.IgdGiven = UPid.Out;
	}
	else
	{
		UPid.Out = 0;
		UPid.Pi = 0;	
		SysCnt.MpptStart = 0;
		MpptRegs.FirstCnt = 0;
		MpptRegs.IgdGiven = 0;
	}
}
void StateFun()  //状态控制
{
	if(InputFilter.Upv<UPVOVER)
	{
		if(SysFlag.bit.PvOk==0)
		{
			if(InputFilter.Upv>=STARTUPV )
			{
				SysCnt.StartCnt++;
				if(SysCnt.StartCnt>=StartTime)
				{
					SysCnt.StartCnt=StartTime;
					SysFlag.bit.PvOk=1;
					StartTime=TM4M; // 保护后再次开机需要5分钟
				}
			}else
			{
				if(SysCnt.StartCnt>0)
				{
					SysCnt.StartCnt--;
					StopReg2.bit.Pbit18=1;   // 再次开机延时

				}else
				{
					SysCnt.StartCnt=0;
				}
			}
		}else
		{
			SysCnt.StartCnt=0;
		}
		SysCnt.UpvUpcnt=0;
	}else
	{
		SysCnt.UpvUpcnt++;
		if(SysCnt.UpvUpcnt>=TM10MS)
		{
			SysFlag.bit.PvOk=0;
			SysCnt.StartCnt=0;
			StopReg2.bit.Pbit19=1;
			SysCnt.UpvUpcnt=TM10MS;
		}
	}
	if(SysFlag.bit.ACStop==1)
	{
		SysCnt.StartCnt=0;
		SysFlag.bit.PvOk=0;
		SysFlag.bit.ACStop=0;
	}
	if(InputFilter.Upv<=STOPUPV)
	{
		SysCnt.StopCnt++;
		if(InputFilter.Upv<_IQ(500))
		{
			SysCnt.StopCnt=SysCnt.StopCnt+5;    // 20150713
		}
		if(SysCnt.StopCnt>=TM1S)
		{
			SysFlag.bit.PvOk=0;
			StopReg2.bit.Pbit17=1;  // 电池电压低
			SysCnt.StopCnt=TM500MS;
		}
	}else
	{
		if(SysCnt.StopCnt>0)
		{
			SysCnt.StopCnt--;
		}else
		{
			SysCnt.StopCnt=0;
		}
	}
//	SysFlag.bit.PvOk=1;		//调试用
	if((Rs485FlagReg.bit.Open==1)&&(SysFlag.bit.PvOk==1))
	{
		SysFlag.bit.Open=1;     // 开机
	}else
	{
		SysFlag.bit.Open=0;    // 关机
	}
	if(Rs485FlagReg.bit.Reset==1)
	{	
		SysFlag.bit.Reset=1;
		Rs485FlagReg.bit.Reset=0;
	}
	if(SysFlag.bit.Reset==1)
	{
		SysCnt.Resetcnt++;
		StopReg2.all=0;
		SysCnt.StartCnt=StartTime;
		if(SysCnt.Resetcnt>=TM400MS)
		{
			SysCnt.Resetcnt=0;
			SysFlag.bit.Reset=0;
		}
	}else
	{
		SysCnt.Resetcnt=0;
	}
}
void TimeFun()
{
	TimeReg.Intercnt++;
	if(TimeReg.Intercnt>=5000)
	{
		TimeReg.Intercnt=0;
		TimeReg.second++;

	}
	if(TimeReg.second>=60)
	{
		TimeReg.minute++;
		TimeReg.second=0;
	}
	if(TimeReg.minute>=60)
	{
		TimeReg.hours++;
		TimeReg.minute=0;
	}
	if(TimeReg.hours>=60)
	{
		TimeReg.hours=0;
	}
	
}
void ADCReuslt()
{
	_iq19 temp,tempu,tempi;

	AdcRegs.ADCCHSELSEQ1.bit.CONV00=0;//A0
	AdcRegs.ADCCHSELSEQ1.bit.CONV01=1;//A1
	AdcRegs.ADCCHSELSEQ1.bit.CONV02=2;//A0
	AdcRegs.ADCCHSELSEQ1.bit.CONV03=3;//A1
	AdcRegs.ADCCHSELSEQ2.bit.CONV04=4;//A0
	AdcRegs.ADCCHSELSEQ2.bit.CONV05=5;//A1
	AdcRegs.ADCCHSELSEQ2.bit.CONV06=6;//A0
	AdcRegs.ADCCHSELSEQ2.bit.CONV07=7;//A1
	AdcRegs.ADCCHSELSEQ3.bit.CONV08=8;//A0
	AdcRegs.ADCCHSELSEQ3.bit.CONV09=9;//A1
	AdcRegs.ADCCHSELSEQ3.bit.CONV10=10;//B2
	AdcRegs.ADCCHSELSEQ3.bit.CONV11=11;//B3
	AdcRegs.ADCCHSELSEQ4.bit.CONV12=12;//B4
	AdcRegs.ADCCHSELSEQ4.bit.CONV13=13; //B5
	AdcRegs.ADCCHSELSEQ4.bit.CONV14=14; //B5
	AdcRegs.ADCCHSELSEQ4.bit.CONV15=15; //B7
	AdcRegs.ADCMAXCONV.all=15;
	
	AdcRegs.ADCTRL2.all=0x2000;  //软件开启AD采样
	while(AdcRegs.ADCST.bit.INT_SEQ1==0){}   //等待AD采样结束
	AdcRegs.ADCST.bit.INT_SEQ1_CLR=1;
	AdcRegs.ADCTRL2.bit.RST_SEQ1=1;

   
    AI.Upv=AdcRegs.ADCRESULT8>>4;
    temp=AI.Upv;                                                    //UdcB
	temp=temp<<GLOBAL_Q;
	InputSignal.Upv=_IQmpy(KUpv,temp);
	AI.Ipv=AdcRegs.ADCRESULT0>>4;
	temp=AI.Ipv;                                                  //UpvA
	temp=temp<<GLOBAL_Q;
	InputSignal.Ipv=_IQmpy(KIpv,temp);//

	InputFilter.Upv=_IQmpy(InputSignal.Upv,_IQ(0.01))+_IQmpy(InputFilter.Upv,_IQ(0.99));
	InputFilter.Ipv=_IQmpy(InputSignal.Ipv,_IQ(0.01))+_IQmpy(InputFilter.Ipv,_IQ(0.99));
	InputSignal.Pin=_IQ15mpy((InputFilter.Upv>>4),(InputFilter.Ipv>>4));	//本次计算功率
	InputFilter.Pin=_IQ15mpy(InputSignal.Pin,_IQ15(0.1))+_IQ15mpy(InputFilter.Pin,_IQ15(0.9));
	MonitorS.Upv=InputFilter.Upv>>GLOBAL_Q;  						
	MonitorS.Ipv=_IQmpy(InputFilter.Ipv,_IQ(10))>>GLOBAL_Q;
	tempu=MonitorS.Ugareal;
	tempu=_IQdiv((tempu<<19),_IQ(1000));
	tempi=MonitorS.Igareal;
	tempi=_IQmpy((tempi<<19),_IQ(0.3));
	InputSignal.POut=_IQmpy(tempu,tempi) ;//      (int32)MonitorS.Ugareal*MonitorS.Igareal*3/10000
	if(InputSignal.POut>_IQ(0))
	{
		MonitorS.POut=_IQmpy(InputSignal.POut,_IQ(10))>>19;
	}else
	{
		MonitorS.POut=0;
	}
}

void ACPower()
{
		if(InputFilter.Upv>=_IQ(400))
		{
			SysCnt.PowerOncnt++;
			if(SysCnt.PowerOncnt>=TM1S)
			{
				DO.bit.PowerOn=1;
				SysCnt.PowerOncnt=TM1S;
				SysCnt.PowerOffcnt=0;
			}
		}else if(InputFilter.Upv<=_IQ(300))
		{
			SysCnt.PowerOffcnt++;
			if(SysCnt.PowerOffcnt>=TM1S)
			{
				DO.bit.PowerOn=0;
				SysCnt.PowerOffcnt=TM1S;
				SysCnt.PowerOncnt=0;
			}
		}
}
