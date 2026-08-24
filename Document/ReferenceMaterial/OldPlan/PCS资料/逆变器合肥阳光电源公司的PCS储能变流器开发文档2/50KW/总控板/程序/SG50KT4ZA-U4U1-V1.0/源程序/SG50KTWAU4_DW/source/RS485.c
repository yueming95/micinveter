#define rs485_GLOBALS
#include "DSP2833x_Device.h"
#include "IQmathLib.h"
#include "corecontrol.h"
#include "define.h"
#include "rs485.h"
#include "rsdebug.h"   //150814
#include "dataoperation.h"
#pragma CODE_SECTION(sciaRxFifoIsr, "ramfuncs");
#pragma CODE_SECTION(LCDINT, "ramfuncs");
/***********************************************
中断接收，中断发送，不使用fifo
************************************************/
Uint16 CRCcodeDelt(unsigned char *BUFaddress,Uint16 CRCnum)      /*获得校验码*/  
{  
   int i,j;
   unsigned char *pr;
   unsigned char prtemp;
   Uint16 regCRC,crctemp;
   pr = BUFaddress;                   
   regCRC = 0xFFFF;			    	/*crc校验寄存器全置1*/ 
   for(i = 0;i < CRCnum; i++, pr++)   		/*对缓冲区的数据进行校验*/
 {   
	prtemp=*pr;
	crctemp=(prtemp & 0x00ff);
     regCRC^=crctemp;	
    for(j=0;j<8;j++)  		        /*移位八次,如果寄存器移出的最低位LSB为1,则寄存器的值与0xa001异或;如果LSB为0,则直接移位*/
   {
     if(regCRC&0x0001)		    /*判断crc校验寄存器最后一位是否为1*/
     {
       regCRC>>=1;			   	/*移后的位为1时,crc校验寄存器与0xA001异或*/
	   regCRC^=0xA001;	   
      }
     else   regCRC>>=1;
    }  
 }
	return(regCRC);
} 

Uint16 bb;
//unsigned int test[6];
interrupt void sciaRxFifoIsr(void)
{    	 
	unsigned int temp;
		  bb++;
	temp=SciaRegs.SCIRXBUF.all;
	temp=temp & 0x00FF;
/*	if(temp==0x005A)//||(temp==0x0002))   
	{
		RecCnt=0;
		HmiFlagReg.bit.PortState=1;
	}*/

	if(RecCnt<35)
	{
		RecData[RecCnt]=temp;
		RecCnt++;
	}else
	{
		RecCnt=34;
	}
	if((RecData[0]!=0x005a)&&(RecData[0]!=0x0002))
	{
		RecCnt=0;
	}else if(RecCnt==1)	
	{
	//	test[0]++;
		HmiFlagReg.bit.PortState=1;
	}
	
	Otime2=0;
	SciaRegs.SCIFFRX.bit.RXFFOVRCLR = 1;   // Clear Overflow flag
	SciaRegs.SCIFFRX.bit.RXFFINTCLR = 1;   // Clear Interrupt flag
	PieCtrlRegs.PIEACK.all |= 0x100;       // Issue PIE ack*/
}
/*interrupt void SCIATX_ISR(void)   
{

    PieCtrlRegs.PIEACK.all=PIEACK_GROUP9;	//响应中断,注意要写
	EINT;
}*/
unsigned char nEND;

void LCDINT(void)
{   
	int i;

	Otime2++;
	if(HmiFlagReg.bit.StartLCD==1)
	{
		SendTimeCnt++;	
	}

	if(SendTimeCnt>TM100MS)
	{
		TimeGetCnt++;
		SendTimeCnt=0;
		if(HmiFlagReg.bit.PortState==0)
		{
			HmiFlagReg.bit.PortState=4;  // 100MS发送一次
			SendDataIndex=0;
			SendDelayCnt=0;
			SendTime=SendLength>>4;
			SendCurTime=0;
			RecCnt=0;
			RecData[0]=0;
		}
	}
	if(HmiFlagReg.bit.PortState==4)
	{
		// enable send IO
		SendDelayCnt++;
		if ((SendDelayCnt%50==0)&&(SendCurTime <= SendTime))
		{	
			if (SendCurTime < SendTime)
			{
				SendMaxByte = 16;
			}
			else
			{
			    SendMaxByte = SendLength & 0x0f;
				HmiFlagReg.bit.SendReady=0;
			}
			SendCurTime++;
	   		for(i=0; i< SendMaxByte; i++)
   			{
   				SciaRegs.SCITXBUF = SendData[SendDataIndex] & 0x00FF;        // Send data16
				SendDataIndex++;
			}
		}
		if (SendDelayCnt>=200)
		{
			SendDelayCnt=0;
			HmiFlagReg.bit.PortState=0;
		}
	}
	if(HmiFlagReg.bit.PortState==0)
	{
		//disable send IO
	}	
}
void CMDOperator(unsigned char cmd)
{
//	Uint16 temp;
//	Uint16 CRCresult;
	if(cmd==0x01)
	{
		Rs485FlagReg.bit.Open=1;   // 开机
	}else if(cmd==0x02)
	{
		Rs485FlagReg.bit.Open=0;  // 关机
		StopReg2.bit.Pbit16=1;
	}else if(cmd==0x03)
	{
		Rs485FlagReg.bit.Reset=1;   // 复位
	}
}
void LCDDisplay()
{
  	Uint16 temp;	 
//	Uint16 RxCRC;
	if(Otime2 > 100)
	{ 
	  	Otime2=0;
		if(HmiFlagReg.bit.PortState==1)//正在接收
		{
			HmiFlagReg.bit.PortState=3;   // 接收超时进入接收数据处理状态
	//		test[1]++;
		}
	}
	if(HmiFlagReg.bit.PortState==3)
	{
		if((RecCnt>=9)&&(RecData[0]==0x005A)&&(RecData[1]==0x00A5))
		{
			temp=RecData[RecCnt-1];
		 //	RxCRC=(((temp<<8) & 0xff00)+RBUF[Receivecount-2]);
  //		    RxCRC=((temp & 0xff)+((RecData[RecCnt-2]<<8) & 0xff00));
	     //	nEND = RecCnt-2;
		/*	if(RxCRC!=CRCcodeDelt(RBUF,nEND))
	     	{
				Receivecount = 0;
				Rs485FlagReg.bit.Rs485State=0;
				return;
			}*/
			if((RecData[2]==0x0008)&&(RecData[3]==0x0083)&&(RecData[4]==0x0000)&&(RecData[5]==0x0020))
			{
				CMDOperator(RecData[8]);
			}
			if((RecData[2]==0x000C)&&(RecData[3]==0x0081)&&(RecData[4]==0x0020)&&(RecData[5]==0x0007))
			{
				ReadTime(RecData);
			}
		}
	//	test[2]=RecCnt;
	//	test[3]=RecData[0];
	//	test[4]=RecData[1];;
	//	test[5]=RecData[2];

		if((RecCnt>=29)&&(RecData[0]==0x0002)&&(RecData[1]==0x0004)&&(RecData[2]==0x0018))
		{
             ReadPV(RecData);
		}

     	RecCnt = 0;
		RecData[0]=0;
		HmiFlagReg.bit.PortState=0;
	}

	if(HmiFlagReg.bit.SendReady==0)
	{
		if(TimeGetCnt>=36000)
		{
			HPDataIndex=6;    // 时间校准dtt1016
			TimeGetCnt=0;
		}
		switch (PageIndex)
		{
			case 0://页面ID
			{
				HPDataIndex=6; 
				break;
			}
			case 1:
			{
				HomePage();
				break;
			}
			default : break;
		}	
	}
}
void ReadTime(unsigned int cmd[])
{
	TimeReg.year=HextoDec(cmd[6]);
	TimeReg.month=HextoDec(cmd[7]);
	TimeReg.day=HextoDec(cmd[8]);
	TimeReg.hours=HextoDec(cmd[10]);
	TimeReg.minute=HextoDec(cmd[11]);
	TimeReg.second=HextoDec(cmd[12]);   
	TaskFlag.bit.TimeOK=1;
}
void ReadPV(unsigned int cmd[])   //dtt1016
{
    int i;
	i=0;
   while(i<24)
   {
     TBUFb1[i]=cmd[i+4];  //取PV电流数据
     TBUFb1[i+1]=cmd[i+3];
	 i=i+2;
	}
   /*	for(i=0;i<24;i++)
	{
       TBUFb1[i]=cmd[i+3];  //取PV电流数据
	}*/
}
unsigned int HextoDec(unsigned int a)
{
	Uint16 temp;
	temp=((a & 0x000f)+((a>>4) & 0x000f)*10);
	return temp;
}
Uint16 pvCnt=0;
void HomePage()
{
	Uint32 temp;
	Uint32 tempStateReg;
    Uint16 i;
    Uint16 CRCresult;
    unsigned char SendDatatemp[50];
   
	if(MonitorS.Ugareal <0) MonitorS.Ugareal =0;
	if(MonitorS.Igareal <0) MonitorS.Igareal =0;
	switch (HPDataIndex)
	{
		case 0:     // 通用数据
				if(MonitorS.Ugareal <0) MonitorS.Ugareal =0;
				if(MonitorS.Igareal <0) MonitorS.Igareal =0;
				if(StateReg.bit.ACState==0)  tempStateReg= 0xB4FDBBFA; //待机
				else if(StateReg.bit.ACState==1)  tempStateReg= 0xC8EDC6F4; //软启
				else if(StateReg.bit.ACState==3)  tempStateReg= 0xB9A4D7F7; //工作
				SendData[0]=0x5A;
				SendData[1]=0xA5;
				SendData[2]=0x15;					//DataLen
				SendData[3]=0x82;
				SendData[4]=0x00;				
				SendData[5]=0x01;								
				SendData[6]=(MonitorS.POut>>8) & 0x00FF;
				SendData[7]=MonitorS.POut & 0x00FF;
				SendData[8]=(tempStateReg>>24) & 0xFF;
				SendData[9]= (tempStateReg>>16) & 0xFF;
				SendData[10]=(tempStateReg>>8) & 0xFF;
				SendData[11]= tempStateReg & 0xFF;
				SendData[12]=(MonitorS.Ugareal>>8) & 0x00FF;
				SendData[13]=MonitorS.Ugareal & 0x00FF;
				SendData[14]=(MonitorS.Igareal>>8) & 0xFF;
				SendData[15]=MonitorS.Igareal & 0x00FF;
				SendData[16]=(MonitorS.Upv>>8) & 0x00FF;
				SendData[17]=MonitorS.Upv & 0x00FF;
				SendData[18]=(MonitorS.Ipv>>8) & 0x00FF;
				SendData[19]=MonitorS.Ipv & 0x00FF;
				temp=HisDataReg.dayenergy/100;
				SendData[20]=(temp>>8) & 0xFF;
				SendData[21]=temp & 0xFF;
				SendLength=24;
				temp=SendLength-5;
				for(i=0;i<19;i++)
				{
					SendDatatemp[i]=SendData[i+3];
				}
				CRCresult=CRCcodeDelt(SendDatatemp,temp);  //di字节在前
				SendData[SendLength-2] = (CRCresult&0xff);
				SendData[SendLength-1] = (CRCresult&0xff00)>>8;
				HPDataIndex=1; 
				break;
		case 1:      // 开关机命令
				SendData[0] = 0x5A;		//数据头
				SendData[1] = 0xA5;        	//6F
				SendData[2] = 0x06; 			//X坐标   62
				SendData[3] = 0x83; 
				SendData[4] = 0x00;		//Y坐标
				SendData[5] = 0x20;
				SendData[6] = 0x01;		
				SendLength=9;
				temp=SendLength-5;
				for(i=0;i<4;i++)
				{
					SendDatatemp[i]=SendData[i+3];
				}
				CRCresult=CRCcodeDelt(SendDatatemp,temp);  //di字节在前
				SendData[SendLength-2] = (CRCresult&0xff);
				SendData[SendLength-1] = (CRCresult&0xff00)>>8;
				HPDataIndex=2; 
				break;
		case 2:   //故障代码   
				SendData[0]=0x5A;
				SendData[1]=0xA5;
				SendData[2]=0x11;					//DataLen
				SendData[3]=0x82;
				SendData[4]=0x00;				
				SendData[5]=0x09;	
				temp=HisDataReg.allenergy/10000;
				SendData[6]=(temp>>24) & 0xFF;
				SendData[7]=(temp>>16) & 0xFF;
				SendData[8]=(temp>>8) & 0xFF;
				SendData[9]=temp & 0xFF;
				SendData[10]=(StopReg1.all>>24) & 0xFF;	 // 故障信息		
				SendData[11]=(StopReg1.all>>16) & 0xFF;
				SendData[12]=(StopReg1.all>>8) & 0xFF;
				SendData[13]=StopReg1.all & 0xFF;
				SendData[14]=(StopReg2.all>>24) & 0xFF;
				SendData[15]=(StopReg2.all>>16) & 0xFF;
				SendData[16]=(StopReg2.all>>8) & 0xFF;
				SendData[17]=StopReg2.all & 0xFF;	
				SendLength=20;
				temp=SendLength-5;
				for(i=0;i<15;i++)
				{
					SendDatatemp[i]=SendData[i+3];
				}
				CRCresult=CRCcodeDelt(SendDatatemp,temp);  //di字节在前
				SendData[SendLength-2] = (CRCresult&0xff);
				SendData[SendLength-1] = (CRCresult&0xff00)>>8;
				HPDataIndex=3;  
				break;
		case 3:      // 获取U5 PV数据  dtt1016 高字节
				SendData[0] = 0x02;		//数据头
				SendData[1] = 0x04;        	//6F
				SendData[2] = 0x18; 			//X坐标   62
				SendData[3] = 0x00; 
				SendData[4] = 0x00;		//Y坐标
				SendData[5] = 0x00;
				SendLength=8;
				temp=SendLength-2;
				CRCresult=CRCcodeDelt(SendData,temp);  //gao字节在前
				SendData[SendLength-1] = (CRCresult&0xff);
				SendData[SendLength-2] = (CRCresult&0xff00)>>8;
				HPDataIndex=4; 
				break;
		case 4:      // pv指令，上传显示屏  dtt151023 两条指令连续发显示屏只能显示第一条指令
		    	if(pvCnt<=1)
				{
			    	SendData[0] = 0x5A;		//数据头
			    	SendData[1] = 0xA5;        	//6F
			     	SendData[2] = 0x19; 			//X坐标   62
			    	SendData[3] = 0x82; 
			    	SendData[4] = 0x00;		//Y坐标
			    	SendData[5] = 0x15;
			    	for(i=0;i<20;i++)          //150814
			    	{
			    	  SendData[6+i]=TBUFb1[i] & 0x00FF;
			        }
			    	SendLength=28;
					temp=SendLength-5;
					for(i=0;i<23;i++)
					{
						SendDatatemp[i]=SendData[i+3];
					}
					CRCresult=CRCcodeDelt(SendDatatemp,temp);  //di字节在前
					SendData[SendLength-2] = (CRCresult&0xff);
					SendData[SendLength-1] = (CRCresult&0xff00)>>8;
				}else{
                       	SendData[0] = 0x5A;		//数据头
			        	SendData[1] = 0xA5;        	//6F
			         	SendData[2] = 0x09; 			//X坐标   62
			        	SendData[3] = 0x82; 
			        	SendData[4] = 0x00;		//Y坐标
		        		SendData[5] = 0x30;
			        	SendData[6]=TBUFb1[20] & 0x00FF;
			        	SendData[7]=TBUFb1[21] & 0x00FF;
			        	SendData[8]=TBUFb1[22] & 0x00FF;
			        	SendData[9]=TBUFb1[23] & 0x00FF;
						SendLength=12;
						temp=SendLength-5;
						for(i=0;i<7;i++)
						{
							SendDatatemp[i]=SendData[i+3];
						}
						CRCresult=CRCcodeDelt(SendDatatemp,temp);  //di字节在前
						SendData[SendLength-2] = (CRCresult&0xff);
						SendData[SendLength-1] = (CRCresult&0xff00)>>8;
						pvCnt=0;
				     }
				pvCnt++;
				HPDataIndex=0;				break;
		case 6:      // 时间校准  dtt1016
				SendData[0] = 0x5A;		//数据头
				SendData[1] = 0xA5;        	//6F
				SendData[2] = 0x05; 			//X坐标   62
				SendData[3] = 0x81; 
				SendData[4] = 0x20;		//Y坐标
				SendData[5] = 0x07;
				SendLength=8;
				temp=SendLength-5;
				for(i=0;i<3;i++)
				{
					SendDatatemp[i]=SendData[i+3];
				}
				CRCresult=CRCcodeDelt(SendDatatemp,temp);  //di字节在前
				SendData[SendLength-2] = (CRCresult&0xff);
				SendData[SendLength-1] = (CRCresult&0xff00)>>8;
				HPDataIndex=0; 
				break; 
		default:break;		
	}
	HmiFlagReg.bit.SendReady=1;
}

