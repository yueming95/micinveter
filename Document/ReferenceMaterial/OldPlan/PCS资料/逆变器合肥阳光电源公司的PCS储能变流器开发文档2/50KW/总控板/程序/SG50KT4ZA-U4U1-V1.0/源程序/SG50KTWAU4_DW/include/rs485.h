#ifndef _RS485_H
#define _RS485_H
#include "DSP2833x_Device.h"
#include "IQmathLib.h"
#ifdef rs485_GLOBALS
#define rs485_GLOBALS
#else
#define rs485_GLOBALS extern
#endif
struct HMIFLAG_S
{
	unsigned int PortState:3;     // 0: 空闲（等待接收） 1: 正在接收  2：   3：接收数据处理 4: 正在发送
	unsigned int StartLCD :1;
	unsigned int SendReady :1;   // 1:一组发送数据准备完毕
	unsigned int reserve:11;
};

union HMIFLAG
{
	struct HMIFLAG_S bit;
	unsigned int all;
};
rs485_GLOBALS union HMIFLAG HmiFlagReg;
rs485_GLOBALS unsigned char SendLength;   //发送byte总个数
rs485_GLOBALS unsigned char SendData[50];

rs485_GLOBALS unsigned int RecData[35];
rs485_GLOBALS unsigned char SendMaxByte; //当前一次最大发送byte个数
typedef union
{	
	struct 
	{
		Uint16 Open           : 1;
		Uint16 Reset          : 1;
		Uint16 Rs485State     : 2;  // 0 等待接收；1接收完毕数据处理，2正在发送
		Uint16 Cleareleccapacity    : 1; 
		Uint16 rsvd1      	  		: 11;	//reserved
	}bit;
	Uint16 all;
}RS485FLAG;
rs485_GLOBALS RS485FLAG Rs485FlagReg;
rs485_GLOBALS unsigned int Otime2,SendTimeCnt;
rs485_GLOBALS unsigned char RecCnt,PageIndex;
rs485_GLOBALS	unsigned char SendDataIndex,HPDataIndex;//数据发送
rs485_GLOBALS	unsigned int SendDelayCnt;//发送延迟计数器
rs485_GLOBALS  unsigned char SendTime;    //以16个byte发送的总次数
rs485_GLOBALS	unsigned char SendCurTime; //以16个byte发送当前的次数
rs485_GLOBALS	unsigned int TimeGetCnt;

Uint16 CRCcodeDelt(unsigned char *BUFaddress,Uint16 CRCnum) ;
interrupt void sciaRxFifoIsr(void);
void CMDOperator(unsigned char cmd);
void HomePage();
void LCDINT(void);
void LCDDisplay();
void ReadTime(unsigned int cmd[]);
void ReadPV(unsigned int cmd[]);
unsigned int HextoDec(unsigned int a);
#endif

