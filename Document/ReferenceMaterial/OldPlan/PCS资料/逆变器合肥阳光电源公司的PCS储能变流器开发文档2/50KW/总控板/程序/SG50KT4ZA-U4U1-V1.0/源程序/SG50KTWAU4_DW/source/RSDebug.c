#define rsdebug_GLOBALS
#include "DSP2833x_Device.h"
#include "IQmathLib.h"
#include "corecontrol.h"
#include "define.h"
#include "rsdebug.h"
#include "dataoperation.h"
#pragma CODE_SECTION(SCIBRX_ISR, "ramfuncs");
#pragma CODE_SECTION(SCIBTX_ISR, "ramfuncs");
 RS485FLAGB Rs485FlagRegb;
 int otimeb;
 unsigned char RBUFb[8];
 unsigned char TBUFb[58];
 unsigned char TBUFb1[24];
 unsigned char Receivecountb,sendcntb,sendlenb;
 unsigned int SendWaitCntb;
 int dtt=0;

interrupt void SCIBRX_ISR(void)
{

     Uint16 aa;   dtt++;
	//  cc++;
     aa = ScibRegs.SCIRXBUF.all;
     if(Receivecountb<12)
     {
		 aa &= 0x00ff;
		 RBUFb[Receivecountb] = aa;
		 Receivecountb++;
     }else
     {
    	 Receivecountb=11;
     }
	 otimeb=0;
	PieCtrlRegs.PIEACK.all|=0x0100;      // Issue PIE ACK
	EINT;
}

interrupt void SCIBTX_ISR(void)
{
	if(ScibRegs.SCICTL2.bit.TXRDY==1)
	{
		if(sendcntb<sendlenb)
		{
   			ScibRegs.SCITXBUF= (TBUFb[sendcntb] & 0x00ff);          	/*send package 应答的数组*/
	 		sendcntb++;
	 	}else
		{
	 			SendWaitCntb=15;//TM3MS;
		}

	}
    /*disable TXREN(禁止发送)(使能接收)*/
    PieCtrlRegs.PIEACK.all=PIEACK_GROUP9;	//响应中断,注意要写
	EINT;
}
unsigned char functioncodeb,nENDb;
void SciReceiveDatab(void)
{
  	Uint16 temp;
	Uint16 RxCRCb;
	if(otimeb > 75)
   	{
        if(Receivecountb> 0)     /*超时处理*/
        {
  	  		otimeb=0;
			Rs485FlagRegb.bit.Rs485State=1;
        }
   	}
	if(RBUFb[0]!=0x01)
	{
		Receivecountb=0;
	}
	if(Rs485FlagRegb.bit.Rs485State==1)
  	{
		Rs485FlagRegb.bit.Rs485State=0;
		if(Receivecountb>=8)
		{
			temp=RBUFb[Receivecountb-1];
		  //  RxCRCb=((temp & 0xff)+((RBUFb[Receivecountb-2]<<8) & 0xff00));//高字节在前
		    RxCRCb=(((temp<<8) & 0xff00)+((RBUFb[Receivecountb-2]) & 0xff));//高字节在前
	     	nENDb = Receivecountb-2;
			if(RxCRCb!=CRCcodeDelt(RBUFb,nENDb))
	     	{
				Receivecountb = 0;
				Rs485FlagRegb.bit.Rs485State=0;
				return;
			}
			functioncodeb=RBUFb[1];
			switch(functioncodeb)
			{
				case 0x01:
					NormalData();
					break;
				case 0x02:
					DebugData(RBUFb[2]);
					break;
				case 0x03:
					SeteData(RBUFb[2]);
				break;
				default: break;
			}
		}
     	Receivecountb = 0;

	}
	if(Rs485FlagRegb.bit.Rs485State==0)
	{
		ScibRegs.SCICTL2.bit.TXINTENA=0;    //中断
		SendWaitCntb=1;
	}
	if((Rs485FlagRegb.bit.Rs485State==2)&&(SendWaitCntb>1))
	{
		SendWaitCntb--;
		if(SendWaitCntb<=1)
		{
			SendWaitCntb=1;
			Rs485FlagRegb.bit.Rs485State=0;
		}
	}
}

void NormalData()
{
int16 temp;
	Uint16 CRCresult;
	int i;
	TBUFb[0]=0x01;
	TBUFb[1]=0x01;
	if(MonitorS.Ugareal <0) MonitorS.Ugareal =0;
	if(MonitorS.Igareal <0) MonitorS.Igareal =0;				//DataLen
	TBUFb[2]=MonitorS.Upv & 0x00FF;
	TBUFb[3]=(MonitorS.Upv>>8) & 0x00FF;
	TBUFb[4]=MonitorS.Ipv & 0x00FF;
	TBUFb[5]=(MonitorS.Ipv>>8) & 0x00FF;
	TBUFb[6]=MonitorS.Ugareal & 0x00FF;
	TBUFb[7]=(MonitorS.Ugareal>>8) & 0x00FF;
	TBUFb[8]= MonitorS.Igareal & 0x00FF;
	TBUFb[9]=(MonitorS.Igareal>>8) & 0x00FF;//0xFF;
	TBUFb[10]=HisDataReg.allenergy & 0xFF;
	TBUFb[11]=(HisDataReg.allenergy>>8) & 0xFF;
	TBUFb[12]=(HisDataReg.allenergy>>16) & 0xFF;
	TBUFb[13]=(HisDataReg.allenergy>>24) & 0xFF;
	TBUFb[14]=HisDataReg.dayenergy & 0xFF;
	TBUFb[15]=(HisDataReg.dayenergy>>8) & 0xFF;
	TBUFb[16]=HisDataReg.daytime & 0xFF;
	TBUFb[17]=StateReg.all & 0xFF;
	// 故障信息
	TBUFb[18]=StopReg1.all & 0xFF;
	TBUFb[19]=(StopReg1.all>>8) & 0xFF;
	TBUFb[20]=(StopReg1.all>>16) & 0xFF;
	TBUFb[21]=(StopReg1.all>>24) & 0xFF;
	TBUFb[22]=StopReg2.all & 0xFF;
	TBUFb[23]=(StopReg2.all>>8) & 0xFF;
	TBUFb[24]=(StopReg2.all>>16) & 0xFF;
	TBUFb[25]=(StopReg2.all>>24) & 0xFF;
	TBUFb[26]=MonitorS.POut & 0x00FF;
	TBUFb[27]=(MonitorS.POut>>8) & 0x00FF;
	for(i=0;i<24;i++)          //150814
	{
	  TBUFb[28+i]=TBUFb1[i] & 0x00FF;
    }

	sendlenb=58;//34;
	temp=sendlenb-2;
	CRCresult=CRCcodeDelt(TBUFb,temp);  //di字节在前
	TBUFb[sendlenb-2] = (CRCresult&0xff);
	TBUFb[sendlenb-1] = (CRCresult&0xff00)>>8;

	sendcntb=1;
	ScibRegs.SCICTL2.bit.TXINTENA=1;   // 启动发送中断
	ScibRegs.SCITXBUF= TBUFb[0];
	Rs485FlagRegb.bit.Rs485State=2;   // 启动发送


}
void DebugData(unsigned char cmd)
{
	Uint16 temp;
	Uint16 CRCresult;
		TBUFb[0]=0x01;
		TBUFb[1]=0x02;
		if(cmd==0xAA)
		{
			TBUFb[2]=0xAA;					//DataLen
			TBUFb[3]=01;
			TBUFb[4]=02;
			sendlenb=7;//31;
		}else if(cmd==0xBB)
		{
			TBUFb[2]=0xBB;					//DataLen
			TBUFb[3]=0x03;
			TBUFb[4]=0x04;
			sendlenb=7;//13;
		}
		temp=sendlenb-2;
		CRCresult=CRCcodeDelt(TBUFb,temp);  //gao字节在前
		TBUFb[sendlenb-1] = (CRCresult&0xff);
		TBUFb[sendlenb-2] = (CRCresult&0xff00)>>8;
		sendcntb=1;
		ScibRegs.SCICTL2.bit.TXINTENA=1;   // 启动发送中断
		ScibRegs.SCITXBUF= TBUFb[0];
		Rs485FlagRegb.bit.Rs485State=2;   // 启动发送
}
void SeteData(unsigned char cmd)
{
	Uint16 temp;
	Uint16 CRCresult;

	if(cmd==0x07)
	{
		// set local time
	}
		TBUFb[0]=0x01;
		TBUFb[1]=0x03;
		TBUFb[2]=0x55;					//DataLen
		TBUFb[3]=0x00;
		TBUFb[4]=0x00;
		TBUFb[5]=0x00;
		sendlenb=8;

		temp=sendlenb-2;
		CRCresult=CRCcodeDelt(TBUFb,temp);  //gao字节在前
		TBUFb[sendlenb-1] = (CRCresult&0xff);
		TBUFb[sendlenb-2] = (CRCresult&0xff00)>>8;
		sendcntb=1;
		ScibRegs.SCICTL2.bit.TXINTENA=1;   // 启动发送中断
		ScibRegs.SCITXBUF= TBUFb[0];
		Rs485FlagRegb.bit.Rs485State=2;   // 启动发送

}

