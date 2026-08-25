#define data_GLOBALS
#pragma CODE_SECTION(DataFun, "ramfuncs");
#include "IQmathLib.h" 
#include "corecontrol.h"
#include "dataoperation.h"
#include "Spi.h"
#include "DSP2833x_Examples.h"
void ReadHistory()   // 读取历史记录
{
	unsigned char i;
	Uint16 addr;
	addr=HEADADDR;
	hishead=ReadChar(addr);
	addr++;
	for(i=0;i<10;i++)
	{
		HisReg[i].hismon=ReadChar(addr);
		addr++;
		HisReg[i].hisday=ReadChar(addr);
		addr++;
		HisReg[i].hishou=ReadChar(addr);
		addr++;
		HisReg[i].hismin=ReadChar(addr);
		addr++;
		HisReg[i].event1=ReadInt32(addr);
		addr=addr+4;
		HisReg[i].event2=ReadInt32(addr);
		addr=addr+4;
	}
}	
/*void WriteHisAll()  // 写历史记录
{
	unsigned char i;
	Uint16 addr,temp;
	addr=HEADADDR;
	temp=(hishead & 0x00ff);
	WriteChar(addr,temp);
	addr++;
	DELAY_US(3000);
	for(i=0;i<10;i++)
	{
		temp=(HisReg[i].hismon & 0x00ff);
	
		WriteChar(addr,temp);
		DELAY_US(3000);
		addr++;
		temp=(HisReg[i].hisday & 0x00ff);
		WriteChar(addr,temp);
		DELAY_US(3000);
		addr++;
		temp=(HisReg[i].hishou & 0x00ff);
		WriteChar(addr,temp);
		DELAY_US(3000);
		addr++;
		temp=(HisReg[i].hismin & 0x00ff);
		WriteChar(addr,temp);
		DELAY_US(3000);
		addr++;
		WriteInt32(addr,HisReg[i].event1);
		addr=addr+4;
		WriteInt32(addr,HisReg[i].event2);
		addr=addr+4;
	}		
}*/
void WriteHis()  // 写历史记录
{
	Uint16 addr,temp;
	addr=HEADADDR;
	temp=(hishead & 0x00ff);
	WriteChar(addr,temp);
	addr=addr+1+12*hishead;
	DELAY_US(3000);
		temp=(HisReg[hishead].hismon & 0x00ff);

		WriteChar(addr,temp);
		DELAY_US(3000);
		addr++;
		temp=(HisReg[hishead].hisday & 0x00ff);
		WriteChar(addr,temp);
		DELAY_US(3000);
		addr++;
		temp=(HisReg[hishead].hishou & 0x00ff);
		WriteChar(addr,temp);
		DELAY_US(3000);
		addr++;
		temp=(HisReg[hishead].hismin & 0x00ff);
		WriteChar(addr,temp);
		DELAY_US(3000);
		addr++;
		WriteInt32(addr,HisReg[hishead].event1);
		addr=addr+4;
		WriteInt32(addr,HisReg[hishead].event2);
		DELAY_US(3000);
}
Uint16 writehis=0;
void HisOperator()  // 放入主循环中
{
	if((StateReg.bit.ACStatebark!=StateReg.bit.ACState)&&(TaskFlag.bit.FirstStart==1))
	{
		TaskFlag.bit.writehis=1;
		if(StateReg.bit.ACState==0)
		{
			SysFlag.bit.ACStop=1;
		}
	}
	StateReg.bit.ACStatebark=StateReg.bit.ACState;
	if((TaskFlag.bit.FirstStart==0)&&(TaskFlag.bit.TimeOK==1))
	{
		ReadData();
		ReadHistory();
		hishead++;
		if(hishead>=10) hishead=0;
		HisReg[hishead].hismon=TimeReg.month;
		HisReg[hishead].hisday=TimeReg.day;
		HisReg[hishead].hishou=TimeReg.hours;
		HisReg[hishead].hismin=TimeReg.minute;
		HisReg[hishead].event1=0xffffffff;
		HisReg[hishead].event2=0;
		WriteHis();
		if(TimeReg.day!=TimeReg.oldday)
		{
			HisDataReg.daytime=0;
			HisDataReg.dayenergy=0;
			WriteData();
		}
		TaskFlag.bit.FirstStart=1;
	}
	if(TaskFlag.bit.writehis==1)
	{
		writehis++;
		hishead++;
		if(hishead>=10) hishead=0;
		HisReg[hishead].hismon=TimeReg.month;
		HisReg[hishead].hisday=TimeReg.day;
		HisReg[hishead].hishou=TimeReg.hours;
		HisReg[hishead].hismin=TimeReg.minute;
		if(StateReg.bit.ACState>0)  // 运行状态
		{
			HisReg[hishead].event1=0;
			HisReg[hishead].event2=0;
		}else
		{
			HisReg[hishead].event1=StopReg1.all;        // add errocode
			HisReg[hishead].event2=StopReg2.all;			// add errocode
		}
		WriteHis();
		TaskFlag.bit.writehis=0;
	}
}	
void DataFun()  // 数据计算，放入中断中
{
	/*if(TimeReg.second!=TimeReg.secondbar)
	{
		firststartcnt++;
		if(firststartcnt==15)
		{
			TaskFlag.bit.FirstStart=1;
		}else if(firststartcnt>15)
		{
			firststartcnt=16;
		}
	}*/
	TimeReg.secondbar=TimeReg.second;
	if(StateReg.bit.ACState>0) 
	{
		if((TimeReg.second==0)&&(TimeReg.Intercnt==0))
		{
			timecnt++;
		}
		if(timecnt>=6)
		{
			HisDataReg.daytime++;
			timecnt=0;
		}
		if((TimeReg.minute==0)&&(TimeReg.second==0)&&(TimeReg.Intercnt==0))
		{
			HisDataReg.alltime++;
		}
		if(TimeReg.Intercnt==0)
		{
				Energytemp+=InputSignal.POut;
		}
		if((TimeReg.second==0)&&(TimeReg.Intercnt==0))
		{
			Energy1min=_IQdiv(Energytemp,_IQ(36));  // 1分钟发电量放大了100倍
			HisDataReg.dayenergy+=(Energy1min>>19);
			HisDataReg.allenergy+=(Energy1min>>19);
			Energytemp=0;
		}
	}
	if((TimeReg.minute==50)&&(TimeReg.second==0)&&(TimeReg.Intercnt==0)&&(TaskFlag.bit.FirstStart==1))
	{
		TaskFlag.bit.DataWrite=1;
	}

}
void DataRecord()
{
	if(TaskFlag.bit.DataWrite==1)
	{
		WriteData();
		TaskFlag.bit.DataWrite=0;
	}	
}
Uint16 Dayenergyaddr,Allenergyaddr,Daytimeaddr,Alltimeaddr;
void ReadData()
{
	Uint16 addr;
	addr=DAYENERGYADDR;
	HisDataReg.dayenergy=ReadInt(addr);
	addr=ALLENERGYADDR;
	HisDataReg.allenergy=ReadInt32(addr);
	addr=DAYTIMEADDR;
	HisDataReg.daytime=ReadChar(addr);
	addr=ALLTIMEADDR;
	HisDataReg.alltime=ReadInt(addr);
	addr=NOWTIMEADDR;
	TimeReg.oldday=ReadChar(addr);
}
void WriteData()
{
	Uint16 addr,temp;
	addr=DAYENERGYADDR;
	WriteInt(addr,HisDataReg.dayenergy);

	addr=ALLENERGYADDR;
	WriteInt32(addr,HisDataReg.allenergy);
	addr=DAYTIMEADDR;
	temp=(HisDataReg.daytime & 0x00ff);
	WriteChar(addr,temp);
	DELAY_US(3000);
	addr=ALLTIMEADDR;
	WriteInt(addr,HisDataReg.alltime);
	DELAY_US(3000);
	addr=NOWTIMEADDR;
	temp=(TimeReg.day & 0x00ff);
	WriteChar(addr,temp);
	DELAY_US(3000);
}


