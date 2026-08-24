#define can_GLOBALS
#include "can.h"
#include "Define.h"
#include "corecontrol.h"
#include "IQmathLib.h" 
#include "pid.h"
#pragma CODE_SECTION(CanDataPack, "ramfuncs");
#pragma CODE_SECTION(CanData, "ramfuncs");
void CanDataPack()
{	
	InterfaceCan.SendL.bit.Hbit=(SysFlag.all & 0x0007);

}
void CanData()
{
	struct ECAN_REGS ECanaShadow;
	Uint16 temp;
	CanSendTime++;
	if(CanSendTime>=TMCAN)
	{
		CanDataPack();
	//============接收消息===========================
		ECanaShadow.CANRMP.all = ECanaRegs.CANRMP.all;
		if (ECanaShadow.CANRMP.bit.RMP2 == 1)			//接收到消息
		{
			InterfaceCan.RdatL1.all = ECanaMboxes.MBOX2.MDL.all;      
			InterfaceCan.RdatH1.all = ECanaMboxes.MBOX2.MDH.all;
			ECanaShadow.CANRMP.bit.RMP2 = 1;
			ECanaRegs.CANRMP.all = ECanaShadow.CANRMP.all;
		}
		if (ECanaShadow.CANRMP.bit.RMP3 == 1)			//接收到消息
		{
			InterfaceCan.RdatL2.all = ECanaMboxes.MBOX3.MDL.all;      
			InterfaceCan.RdatH2.all = ECanaMboxes.MBOX3.MDH.all;
			ECanaShadow.CANRMP.bit.RMP3 = 1;
			ECanaRegs.CANRMP.all = ECanaShadow.CANRMP.all;
		}
		if (ECanaShadow.CANRMP.bit.RMP4 == 1)			//接收到消息
		{
			InterfaceCan.RdatL3.all = ECanaMboxes.MBOX4.MDL.all;      
			InterfaceCan.RdatH3.all = ECanaMboxes.MBOX4.MDH.all;
			ECanaShadow.CANRMP.bit.RMP4 = 1;
			ECanaRegs.CANRMP.all = ECanaShadow.CANRMP.all;
		}
//==============接收消息处理================================
		temp=(InterfaceCan.RdatH1.bit.Lbit & 0x0003);
		StateReg.all=(StateReg.all & 0xfffC)+temp;  // AC侧状态
		StopReg1.all=InterfaceCan.RdatL1.all;    // AC侧故障代码
		MonitorS.Ugareal=InterfaceCan.RdatL3.bit.Lbit;
		MonitorS.Igareal=InterfaceCan.RdatL3.bit.Hbit;
		temp=InterfaceCan.RdatL2.bit.Lbit;
		StopReg2.all=(StopReg2.all & 0xFFFF0000)+temp; // DC侧故障代码
		temp=((InterfaceCan.RdatH2.bit.Lbit<<2) & 0x000c);
		StateReg.all=((StateReg.all & 0xfff3) | temp);  // DC侧状态

	//=============发送消息=============================
		ECanaMboxes.MBOX1.MDL.all = InterfaceCan.SendL.all;// & 0xffffff00)|CANHEAD; //CANHEAD用于接收方校验是否受到的是正确数据
		ECanaMboxes.MBOX1.MDH.all=InterfaceCan.SendH.all;
		ECanaShadow.CANTRS.all = ECanaRegs.CANTRS.all;
		ECanaShadow.CANTRS.bit.TRS1=1;
		ECanaRegs.CANTRS.all = ECanaShadow.CANTRS.all;
		if (ECanaShadow.CANTA.bit.TA1 == 1)	//发送成功
		{		
			ECanaShadow.CANTA.bit.TA1 = 1;
			ECanaRegs.CANTA.all = ECanaShadow.CANTA.all;
		}
	}
}

void PowerCan()
{
	struct ECAN_REGS ECanbShadow;
	InterfaceCan.SendL1.all = MpptRegs.IgdGiven;
	//=============发送消息=============================
	ECanbMboxes.MBOX5.MDL.all = InterfaceCan.SendL1.all;// & 0xffffff00)|CANHEAD; //CANHEAD用于接收方校验是否受到的是正确数据
	ECanbMboxes.MBOX5.MDH.all = InterfaceCan.SendH1.all;
	ECanbShadow.CANTRS.all = ECanbRegs.CANTRS.all;
	ECanbShadow.CANTRS.bit.TRS5=1;
	ECanbRegs.CANTRS.all = ECanbShadow.CANTRS.all;
	if (ECanbShadow.CANTA.bit.TA5== 1)	//发送成功
	{		
		ECanbShadow.CANTA.bit.TA5 = 1;
		ECanbRegs.CANTA.all = ECanbShadow.CANTA.all;
	}
}


