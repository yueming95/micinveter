#ifndef _DATA_H
#define _DATA_H
#include "DSP2833x_Device.h"
#include "IQmathLib.h"
#ifdef data_GLOBALS
#define data_GLOBALS
#else
#define data_GLOBALS extern
#endif
#define HEADADDR 0x01;   // 历史记录
#define DAYTIMEADDR 0xA0 // 日发电时间
#define DAYENERGYADDR 0xAE  // 日发电量
#define ALLTIMEADDR 0x82 // 总发电时间
#define ALLENERGYADDR 0x86   // 总发电量
#define NOWTIMEADDR 0x90  // 当前时间
typedef union
{	
	struct 
	{
		Uint16 writehis            :1;
		Uint16 FirstStart        :1;
		Uint16 DataWrite        : 1 ;
		Uint16 rsvd2        : 1;
		Uint16 TimeOK           : 1;
		Uint16 rsvd1      	  : 11;	//reserved
	}bit;
	Uint16 all;
}TASK;
data_GLOBALS TASK TaskFlag;
typedef struct
{
	unsigned char hismon;
	unsigned char hisday;
	unsigned char hishou;
	unsigned char hismin;
	Uint32 event1;
	Uint32 event2;
}HISTORY;
data_GLOBALS HISTORY HisReg[10];
typedef struct
{
	unsigned char daytime;    // 真实小时数放大10倍
	Uint16 dayenergy;  //kwh放大100倍
	Uint16 alltime;     // 真实小时数
	Uint32 allenergy;  // kwh放大100倍
}HISDATA;
data_GLOBALS HISDATA HisDataReg;
data_GLOBALS unsigned char hishead;
data_GLOBALS unsigned char timecnt,firststartcnt;
data_GLOBALS _iq Energy1min,Energytemp;
void HisOperator();
void ReadHistory();
void WriteHis();
void DataFun() ;
void DataRecord();
void ReadData();
void WriteData();
#endif

