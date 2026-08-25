#ifndef RSDEBUG_H_
#define RSDEBUG_H_
#include "DSP2833x_Device.h"
#include "IQmathLib.h"
#include "rs485.h"
#ifdef rsdebug_GLOBALS
#define rsdebug_GLOBALS
#else
#define rsdebug_GLOBALS extern
#endif
typedef union
{
	struct
	{
		Uint16 Rs485State     : 2;  // 0 等待接收；1接收完毕数据处理，2正在发送
		Uint16 rsvd1      	  		: 14;	//reserved
	}bit;
	Uint16 all;
}RS485FLAGB;
extern RS485FLAGB Rs485FlagRegb;
extern int otimeb;
extern unsigned char RBUFb[8];
extern unsigned char TBUFb[58];
extern unsigned char TBUFb1[24];
extern unsigned char Receivecountb,sendcntb,sendlenb;
extern unsigned int SendWaitCntb;
void NormalData();
interrupt void SCIBRX_ISR(void);
interrupt void SCIBTX_ISR(void) ;
void DebugData(unsigned char cmd);
void SciReceiveDatab(void);
void SeteData(unsigned char cmd);
void CMDOperator(unsigned char cmd);

#endif /* RSDEBUG_H_ */
