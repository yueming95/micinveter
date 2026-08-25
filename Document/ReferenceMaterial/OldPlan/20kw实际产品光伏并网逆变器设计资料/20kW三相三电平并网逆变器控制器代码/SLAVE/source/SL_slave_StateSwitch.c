/***********************************************************************

 *    FILENAME : SL_slave_StateSwitch.c
 *
 *    PURPOSE  : 
 *    HISTORY  :
 *           DATE          VERSION      AUTHOR        NOTE
 ************************************************************************/
#include "SL_slave.h"
void SlaveRelayShortCheck(void);
void SlaveRelayOpenCheck(void);
extern int16 RelayTemp11;
extern int16 RelayTemp12;
extern int16 RelayTemp13;
extern Uint16 g_SPICommCnt;
extern Uint16 FactoryMode;
/**********************************************************************
* FUNCION :  TSK_F_StateSwitch
* PURPOSE : 
* CALLED BY: DSP/BIOS  kernel task  every 2ms 
**********************************************************************/
void TSK_F_StateSwitch(void)
{
	ReadFromEeprom();//读取EEPROM表示是否有安规参数的标志寄存器Orig_epromflg
	while(1)
	{
		SEM_pend(&SEM_StateSwitch_2msPRD, SYS_FOREVER);

		DigitalIOCheck();
		SlaveRelayOpenCheck();
		switch (s_SysCurrentState)
		{
			case WaitState:
				SlaveWaitState();
			break;  
			case CheckState:
				SlaveCheckState();
			break;
			case NormalState:   
				SlaveNormalState();
			break;   
			case FaultState:    
				SlaveFaultState();
			break;
			case PermanentState:        
				SlavePermanentState();
			break; 
			case FlashState:
            	SlaveFlashState();
			break;
			default:
				SlavePermanentState(); 
			break;	  
		}//end of switch
	}
}

/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void SlaveWaitState(void)
{
	Uint16 temp1 = 0;
	if(ReadyToEnterFlashState())
	{
		EnterFlashState();
	}
	else if(ReadyToEnterPermanentState())
    {
        EnterPermanentState();
	}
	else if(ReadyToEnterFaultState())
    {
		EnterFaultState();
	}
	else if(ReadyToEnterCheckState())
	{
		EnterCheckState();
	}
	else
	{
		s_SysCurrentState = WaitState;

		if(ProcessVariable.i16ISOCheckMode_Input == 0 && ProcessVariable.i16IsoCheckcount > 1500)
		{
			if(CalcResultAve.i16VPV1 > 2450 || CalcResultAve.i16VPV2 > 2450 )//PV1 and PV2
			{
				if(CalcResultAve.i16VPV1 > CalcResultAve.i16VPV2)
				{
					temp1 = CalcResultAve.i16VPV1 - CalcResultAve.i16VPV2;
				}
				else
				{
					temp1 = CalcResultAve.i16VPV2 - CalcResultAve.i16VPV1;
				}

				if(CalcResultAve.i16VPV2 <= 500)	//PV1 only
				{
					ProcessVariable.i16ISOCheckMode_Input = 1;
				}
				else if(CalcResultAve.i16VPV1 <= 500) //PV2 only
				{
					ProcessVariable.i16ISOCheckMode_Input = 2;
				}
				else if(temp1 <= 200) //in parallel
				{
					ProcessVariable.i16ISOCheckMode_Input = 3;
				}
				else	//independent
				{
					ProcessVariable.i16ISOCheckMode_Input = 4;
				}
				temp1 = FactoryMode&0x00FF;
				if(129 == temp1)
				{
					ProcessVariable.i16ISOCheckMode_Input = 1;
				}
				else if(130 == temp1)
				{
					ProcessVariable.i16ISOCheckMode_Input = 2;
				}
			}
		}

		if((s_StateCheck.bit.PVIsoCheckOver == 0)&&(ProcessVariable.i16IsoCheckcount < 200000)&&(1 == s_StateCheck.bit.HandShakeOver))
		{
			ProcessVariable.i16IsoCheckcount++;
		}
			
		if((0 == s_StateCheck.bit.PVIsoCheckOver) 
		&& (1 == s_StateCheck.bit.GFCICheckOVER)
		&& (ProcessVariable.i16ISOCheckMode_Input!=0)
		&&	(ProcessVariable.i16IsoCheckStep == 0 
			|| (ProcessVariable.i16IsoCheckStep == 1 && ProcessVariable.i16IsoCheckcount > 1501)//0
			|| (ProcessVariable.i16IsoCheckStep == 2 && ProcessVariable.i16IsoCheckcount > 1501)))//00
		{
			if(1 == SafetyIsoIslandandSoOnTable[2])
			{
				PvInsulationCheck();
			}
			else
			{
				s_StateCheck.bit.PVIsoCheckOver =1;//需要保证握手成功？默认 需要测量绝缘阻抗。握手不成功时，不会执行此语句
			}	
		}

/*		if(0 == s_StateCheck.bit.FanCheckOver)
		{
			SlaveFanCheck();
		}
*/
	}
}

/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void SlaveCheckState(void)
{
	if(ReadyToEnterFlashState())
	{
		EnterFlashState();
	}
	else if(ReadyToEnterPermanentState())
    {
        EnterPermanentState();
	}
	else if(ReadyToEnterFaultState())
    {
        EnterFaultState();
	}
	else if(ReadyToEnterWaitState())
	{
		EnterWaitState();
	}
	else if(ReadyToEnterNormalState())
	{
		EnterNormalState();
	}
	else
	{
		s_SysCurrentState = CheckState;
		s_StateCheck.bit.PVIsoCheckOver =0;
		if(1 == s_StateCheck.bit.BusOverRef)
		{
			if(0 == s_StateCheck.bit.AcRlyCheckOver)
			{	
				SlaveRelayShortCheck();

			}
		}
		if((1 == s_StateCheck.bit.MasterRelayOFF)&&(1 == s_StateCheck.bit.AcRlyCheckOver))
		{
			if(0 == s_StateCheck.bit.SlaveRelayON)
			{
				S_GRID_RELAY_R1_ON = 1;
				S_GRID_RELAY_R2_ON = 1;
				S_GRID_RELAY_S1_ON = 1;
				S_GRID_RELAY_S2_ON = 1;
				S_GRID_RELAY_T1_ON = 1;
				S_GRID_RELAY_T2_ON = 1;
				s_StateCheck.bit.SlaveRelayON =1;
			}
		}
	}
}

/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void SlaveNormalState(void)
{ 
	if(ReadyToEnterFlashState())
	{
		EnterFlashState();
	}
	else if(ReadyToEnterPermanentState())
    {
        EnterPermanentState();
	}
	else if(ReadyToEnterFaultState())
    {
        EnterFaultState();
	}
	else if(ReadyToEnterWaitState())
	{
		EnterWaitState();
	}
	else
	{
		s_SysCurrentState = NormalState;
	}
}

/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void SlaveFaultState(void) 
{ 
	static Uint16 RelayRecoverCnt = 0;	

	static Uint16 RelayUnRecoverCnt = 0;	
//	static int16 IsoUnRecoverCnt = 0;
	if(ReadyToEnterFlashState())
	{
		EnterFlashState();
	}
	else if(ReadyToEnterPermanentState())
    {
        EnterPermanentState();
	}
	else
    {
		if(1 == s_SysFaultMessage.bit.RecoverRelayFail)
		{
			RelayRecoverCnt++;
			if(RelayRecoverCnt >30000)
			{
				RelayRecoverCnt = 0;
				s_SysFaultMessage.bit.RecoverRelayFail = 0;
				RelayUnRecoverCnt++;
				if(RelayUnRecoverCnt > 3)
				{
					s_SysFaultMessage.bit.unrecoverRelayFail = 1;
					RelayUnRecoverCnt = 0;
				}
			}			
		}
		else
		{
			RelayRecoverCnt = 0;
		}

		if(FaultBackCheck())
        {
            EnterWaitState();
		}
		else
		{
			s_SysCurrentState = FaultState;
		}
	}
}

/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void SlavePermanentState(void)
{
	if(ReadyToEnterFlashState())
	{
		EnterFlashState();
	}
	else
	{
		s_SysCurrentState = PermanentState;
	}
}

/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void SlaveFlashState(void)
{
	PWMOFFandRelayDisconnect();
}

/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
Uint16 ReadyToEnterWaitState(void)
{
	if((WaitState == s_SysCurrentState_Master)&&(0 == s_SysFaultMessage.bit.MChip_Fault)&&(0 == s_SysFaultMessage.bit.SpiCommLose))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
Uint16 ReadyToEnterCheckState(void)
{
	if(CheckState == s_SysCurrentState_Master)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
Uint16 ReadyToEnterNormalState(void)
{
	if(NormalState == s_SysCurrentState_Master)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
Uint16 ReadyToEnterFaultState(void)
{
	if((FaultState == s_SysCurrentState_Master)||(1 == s_SysFaultMessage.bit.MChip_Fault)||(1 == s_SysFaultMessage.bit.SpiCommLose))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
Uint16 ReadyToEnterPermanentState(void)
{
	if(s_SysCurrentState_Master == PermanentState)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
Uint16 ReadyToEnterFlashState(void)
{
	//s_StateCheck.bit.SlaveRelayON =0;
	return 0;
}

/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void EnterWaitState(void)
{
	s_SysCurrentState = WaitState; 
	s_StateCheck.Word.byte2 = 0;
	PWMOFFandRelayDisconnect();
	s_StateCheck.bit.AcRlyCheckOver = 0;	//
	s_StateCheck.bit.SlaveRelayON =0;
}

/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void EnterCheckState(void)
{
	//S_BOOST_PWM_ENABLE;
	PWMOutputsEnable();
	s_SysCurrentState = CheckState;
	s_StateCheck.bit.AcRlyCheckOver = 0;	// 
	s_StateCheck.bit.SlaveRelayON =0;
}

/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void EnterNormalState(void)
{
//	S_INV_PWM_ENABLE;
	s_SysCurrentState = NormalState;
//	s_StateCheck.bit.SlaveRelayON =0;
}

/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void EnterFaultState(void)
{
	s_SysCurrentState = FaultState;
	PWMOFFandRelayDisconnect();
	s_StateCheck.bit.SlaveRelayON =0;
}

/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void EnterPermanentState(void)
{
	s_SysCurrentState = PermanentState;
	PWMOFFandRelayDisconnect();
	s_StateCheck.bit.SlaveRelayON =0;
}

/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void EnterFlashState(void)
{
	s_SysCurrentState = FlashState;
	PWMOFFandRelayDisconnect();
	s_StateCheck.bit.SlaveRelayON =0;
}

/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void PWMOFFandRelayDisconnect(void)
{
	PWMOutputsDisable();

	S_GRID_RELAY_R1_OFF = 1;
	S_GRID_RELAY_R2_OFF = 1;
	S_GRID_RELAY_S1_OFF = 1;
	S_GRID_RELAY_S2_OFF = 1;
	S_GRID_RELAY_T1_OFF = 1;
	S_GRID_RELAY_T2_OFF = 1;
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
Uint16 FaultBackCheck(void)
{
	if((WaitState == s_SysCurrentState_Master)&&(0 == s_SysFaultMessage.bit.MChip_Fault)&&(0 == s_SysFaultMessage.bit.SpiCommLose))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/

void SlaveFanCheck(void)
{
	s_StateCheck.bit.FanCheckOver = 1; 
}

/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/

void SlaveRelayShortCheck(void)
{

	static  Uint16  s_u16Cnt_RelayCheck= 0;	
	static  Uint16  s_u16Cnt_RelayShortCheck= 0;
	int16	RelayTemp1, RelayTemp2, RelayTemp3 = 0;

	switch(s_u16Cnt_RelayCheck)
	{
		case 0:
			S_GRID_RELAY_R1_ON = 1;
			S_GRID_RELAY_S1_ON = 1;
			S_GRID_RELAY_T1_ON = 1;
			S_GRID_RELAY_T2_ON = 1;
			s_u16Cnt_RelayCheck++;//test R2,S2
			break;
				
		case 200:
			S_GRID_RELAY_R1_OFF = 1;
			S_GRID_RELAY_S1_OFF = 1;
			s_u16Cnt_RelayCheck++;
			break;
				
		case 220:
			S_GRID_RELAY_R2_ON = 1;
			S_GRID_RELAY_S2_ON = 1;
			S_GRID_RELAY_T1_ON = 1;
			S_GRID_RELAY_T2_ON = 1;
			s_u16Cnt_RelayCheck++;//test R1,S1
			break;

		case 1500:
			S_GRID_RELAY_S2_OFF = 1;
			S_GRID_RELAY_T2_OFF = 1;
			s_u16Cnt_RelayCheck++;
			break;
				
		case 1520:
			S_GRID_RELAY_R1_ON = 1;
			s_u16Cnt_RelayCheck++;// R1,R2 on;T1 on,T2 off;S1,S2 off
			break;
				
		case 3000:
			S_GRID_RELAY_T1_OFF = 1;
			s_u16Cnt_RelayCheck++;
			break;

		case 3020:
			S_GRID_RELAY_T2_ON = 1;
			s_u16Cnt_RelayCheck++;
			break;
				
		case 3200:
			S_GRID_RELAY_R1_OFF = 1;
			S_GRID_RELAY_R2_OFF = 1;
			S_GRID_RELAY_S1_OFF = 1;
			S_GRID_RELAY_S2_OFF = 1;
			S_GRID_RELAY_T1_OFF = 1;
			S_GRID_RELAY_T2_OFF = 1;			
			s_u16Cnt_RelayCheck++;
			break;	

		default:
			s_u16Cnt_RelayCheck++;
			break;			
	}
	if(s_u16Cnt_RelayCheck < 3200)
	{	
		RelayTemp1 = CalcResultRms.i16VGrid_RSrms - CalcResultRms.i16VInv_RSrms;
		RelayTemp2 = CalcResultRms.i16VGrid_STrms - CalcResultRms.i16VInv_STrms;
		RelayTemp3 = CalcResultRms.i16VGrid_TRrms - CalcResultRms.i16VInv_TRrms;
		RelayTemp11 = RelayTemp1;
		RelayTemp12 = RelayTemp2;
		RelayTemp13 = RelayTemp3;
		if(0 == s_SysFaultMessage.bit.unrecoverRelayFail)
		{
			if((RelayTemp1 < 150) ||(RelayTemp2 < 150)||(RelayTemp3 < 150))
			{
				s_u16Cnt_RelayShortCheck++;
				if(s_u16Cnt_RelayShortCheck > 100)
				{
					s_SysFaultMessage.bit.RecoverRelayFail = 1;
					s_StateCheck.bit.AcRlyCheckOver = 1;
					s_u16Cnt_RelayCheck = 0;
					s_u16Cnt_RelayShortCheck = 0;
				} 
			}
			else
			{
				s_u16Cnt_RelayShortCheck = 0;
			}
		}
	}
	if(4500 == s_u16Cnt_RelayCheck)
	{
		s_StateCheck.bit.AcRlyCheckOver = 1;
		s_u16Cnt_RelayCheck = 0;
		s_u16Cnt_RelayShortCheck = 0;
	}
	
//	s_StateCheck.bit.AcRlyCheckOver = 1;
}

void SlaveRelayOpenCheck(void)
{
	Uint16 u16DeltaV1,u16DeltaV2,u16DeltaV3;
	static Uint16 u16RelayOpenFault = 0;
	static Uint16 u16RelayOpenCnt = 0;	

	u16DeltaV1 = abs(CalcResultRms.i16VGrid_RSrms - CalcResultRms.i16VInv_RSrms);
	u16DeltaV2 = abs(CalcResultRms.i16VGrid_STrms - CalcResultRms.i16VInv_STrms);
	u16DeltaV3 = abs(CalcResultRms.i16VGrid_TRrms - CalcResultRms.i16VInv_TRrms);

	if((0 == s_StateCheck.bit.MasterRelayOFF) && (1 == s_StateCheck.bit.SlaveRelayON))
	{
		u16RelayOpenCnt++;
	}
	else
	{
		u16RelayOpenCnt = 0;
	}
	
	if(u16RelayOpenCnt > 20)
	{
		if((u16DeltaV1 > 150)||(u16DeltaV2 > 150)||(u16DeltaV3 > 150))
		{
			u16RelayOpenFault ++;
			if( u16RelayOpenFault > 60)
			{
				s_SysFaultMessage.bit.RecoverRelayFail = 1;
			}
		}
		else
		{
			u16RelayOpenFault = 0;
		}
	}
	else
	{
		u16RelayOpenFault = 0;
	}

	if(u16RelayOpenCnt > 90)
	{
		u16RelayOpenCnt = 90;
	}
}

/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void PvInsulationCheck(void)
{
	static Uint32 PVvolt1 = 0;
	static Uint32 PVvolt2 = 0;
	static Uint32 ISOUoVolt1 = 0;
	static Uint32 ISOUoVolt2 = 0;
	static Uint16 i16IsoCheckDelay = 0;
	static Uint16 BigInputFlag = 0;
	static Uint16 BigInputOverFlag = 0;

	static Uint64 ISO1_P = 0;
	static Uint64 ISO1_N = 0;

	Uint64 temp1 = 0;
	Uint64 temp2 = 0;
	Uint64 temp3 = 0;
	Uint64 temp4 = 0;
	Uint64 temp5 = 0;
	Uint16 temp6 = 0;

	temp6 = SafetyIsoIslandandSoOnTable[3];

	if(0 == BigInputOverFlag)
	{
		if(CalcResultAve.i16VPV1 > CalcResultAve.i16VPV2)
		{
			BigInputFlag = 1;
			BigInputOverFlag = 1;
		}
		else
		{
			BigInputFlag = 2;
			BigInputOverFlag = 1;
		}
	}

	if(i16IsoCheckDelay<5000)
	{
		i16IsoCheckDelay++;
	}

	if(i16IsoCheckDelay <= 2000)
	{
		if(ProcessVariable.i16ISOCheckMode_Input == 1||ProcessVariable.i16ISOCheckMode_Input == 3||ProcessVariable.i16ISOCheckMode_Input == 4)
		{
			PVvolt1 = CalcResultAve.i16VPV1;
			ISOUoVolt1 = (Uint32)CalcResultAve.i16VISO1 * 334 + ((Uint32)CalcResultAve.i16VISO1>>2); //341=(11218+33)/33 (Uint32)CalcResultAve.i16VISO1 * 341;    //341=(11218+33)/33//
			CalcResultAve.i16VISO1_test0 = CalcResultAve.i16VISO1;//5M, 5000*2 + 499*2 + 33 = 11031;(11031)/33 = 334.27
		}
		else if(ProcessVariable.i16ISOCheckMode_Input == 2)
		{
			PVvolt2 = CalcResultAve.i16VPV2;
			ISOUoVolt1 = (Uint32)CalcResultAve.i16VISO1 * 334 + ((Uint32)CalcResultAve.i16VISO1>>2);//(Uint32)CalcResultAve.i16VISO1 * 341;    //341=(11218+33)/33
			CalcResultAve.i16VISO1_test0 = CalcResultAve.i16VISO1;
		}
				
		if(i16IsoCheckDelay == 2000)
		{
			if(ProcessVariable.i16ISOCheckMode_Input == 1||ProcessVariable.i16ISOCheckMode_Input == 3)
			{
				ISO1_RELAY_ON = 1;
			}
			else if(ProcessVariable.i16ISOCheckMode_Input == 2)
			{
				ISO2_RELAY_ON = 1;
			}
			else if(ProcessVariable.i16ISOCheckMode_Input == 4)
			{
				if(BigInputFlag == 1)
				{
					ISO1_RELAY_ON = 1;
				}
				else if(BigInputFlag == 2)
				{
					ISO2_RELAY_ON = 1;
				}
				else
				{
				}
			}
		}
	}
	else if(i16IsoCheckDelay<=5000)
	{
		if(ProcessVariable.i16ISOCheckMode_Input == 1||ProcessVariable.i16ISOCheckMode_Input == 3||ProcessVariable.i16ISOCheckMode_Input == 2)
		{		
			ISOUoVolt2 = (Uint32)CalcResultAve.i16VISO1 * 334 + ((Uint32)CalcResultAve.i16VISO1>>2); //(Uint32)CalcResultAve.i16VISO1 * 341;    //341=(11218+33)/33
			CalcResultAve.i16VISO1_test00 = CalcResultAve.i16VISO1;
		}
		if(ProcessVariable.i16ISOCheckMode_Input == 4)
		{
			PVvolt1 = CalcResultAve.i16VPV1;
			PVvolt2 = CalcResultAve.i16VPV2;
			ISOUoVolt2 = (Uint32)CalcResultAve.i16VISO1 * 334 + ((Uint32)CalcResultAve.i16VISO1>>2); //(Uint32)CalcResultAve.i16VISO1 * 341;    //341=(11218+33)/33
			CalcResultAve.i16VISO1_test00 = CalcResultAve.i16VISO1;
		}	

		if(i16IsoCheckDelay == 5000)
		{			
			ISO1_RELAY_OFF = 1;			
			ISO2_RELAY_OFF = 1;

			if(ProcessVariable.i16ISOCheckMode_Input == 1||ProcessVariable.i16ISOCheckMode_Input == 3)
			{
				if((ISOUoVolt2 <= ISOUoVolt1)||(PVvolt1*100 <= ISOUoVolt2))
				{
					ISO1_P = 0;
				}
				else
				{
					temp1 = ((Uint64)ISOUoVolt1)*((Uint64)PVvolt1*100-ISOUoVolt2);
					temp2 = (Uint64)PVvolt1*100*((Uint64)ISOUoVolt2-ISOUoVolt1)*1400;  //1400
					ISO1_P = temp2 / temp1;
				}

				temp3 = (Uint64)((Uint64)PVvolt1*100-ISOUoVolt1)*((Uint64)PVvolt1*100-ISOUoVolt2);
				temp4 = (Uint64)PVvolt1*100*((Uint64)ISOUoVolt2-ISOUoVolt1)*1400*11031;//11251;   //11031
				temp5 = temp4 / temp3;

				if(temp5 >= 121682960)
				{
					ISO1_N = 10000;
				}
				else
				{
					temp3 = temp5;   //11218+33
					temp4 = 11031 - temp5/11031;//temp4 = 11031 - temp5/11031;
					ISO1_N = temp3 / temp4;
				}

				CalcResultAve.i16VISO1_test1 = ISO1_P;
				CalcResultAve.i16VISO1_test11 = ISO1_N;
				if(ISO1_P > 30000)
				{
					CalcResultAve.i16VISO1_testoutput1 = 30000;
				}
				else
				{
					CalcResultAve.i16VISO1_testoutput1 = ISO1_P;
				}
				if(ISO1_N > 30000)
				{
					CalcResultAve.i16VISO1_testoutput3 = 30000;
				}
				else
				{
					CalcResultAve.i16VISO1_testoutput3 = ISO1_N;
				}
			//	CalcResultAve.i16VISO1_testoutput1 = ISO1_P;
			//	CalcResultAve.i16VISO1_testoutput3 = ISO1_N;
			}
			else if(ProcessVariable.i16ISOCheckMode_Input == 2)
			{
				if((ISOUoVolt2 <= ISOUoVolt1)||(PVvolt2*100<=ISOUoVolt2))
				{
					ISO1_P = 0;
				}
				else
				{
					temp1 = ((Uint64)ISOUoVolt1)*((Uint64)PVvolt2*100-ISOUoVolt2);
					temp2 = (Uint64)PVvolt2*100*((Uint64)ISOUoVolt2-ISOUoVolt1)*1400;  //1400
					ISO1_P = temp2 / temp1;
				}

				temp3 = (Uint64)((Uint64)PVvolt2*100-ISOUoVolt1)*((Uint64)PVvolt2*100-ISOUoVolt2);
				temp4 = (Uint64)PVvolt2*100*((Uint64)ISOUoVolt2-ISOUoVolt1)*1400*11031;   //1400
				temp5 = temp4 / temp3;

				if(temp5 >= 121682960)
				{
					ISO1_N = 10000;
				}
				else
				{
					temp3 = temp5;   //11218+33
					temp4 = 11031 - temp5/11031;
					ISO1_N = temp3 / temp4;
				}
				CalcResultAve.i16VISO1_test2 = ISO1_P;
				CalcResultAve.i16VISO1_test22 = ISO1_N;
				//CalcResultAve.i16VISO1_testoutput2 = ISO1_P;
				//CalcResultAve.i16VISO1_testoutput3 = ISO1_N; 
				if(ISO1_P > 30000)
				{
					CalcResultAve.i16VISO1_testoutput2 = 30000;
				}
				else
				{
					CalcResultAve.i16VISO1_testoutput2 = ISO1_P;
				}
				if(ISO1_N > 30000)
				{
					CalcResultAve.i16VISO1_testoutput3 = 30000;
				}
				else
				{
					CalcResultAve.i16VISO1_testoutput3 = ISO1_N;
				}

			}
			else if(ProcessVariable.i16ISOCheckMode_Input == 4)
			{

				if(1 == BigInputFlag)
				{
					temp1 = ((int64)ISOUoVolt1)*((int64)PVvolt1*100-ISOUoVolt2);
					temp2 = (int64)PVvolt1*100*((int64)ISOUoVolt2-ISOUoVolt1)*1400;  //1400
					ISO1_P = temp2 / temp1;

					temp3 = (int64)((int64)PVvolt1*100-ISOUoVolt1)*((int64)PVvolt1*100-ISOUoVolt2);
					temp4 = (int64)PVvolt1*100*((int64)ISOUoVolt2-ISOUoVolt1)*1400*11031;   //1400
					temp5 = temp4 / temp3;

					if(temp5 >= 121682960)
					{
						ISO1_N = 10000;
					}
					else
					{
						temp3 = temp5;   //11218+33
						temp4 = 11031 - temp5/11031;
						ISO1_N = temp3 / temp4;
					}

					CalcResultAve.i16VISO1_test1 = ISO1_P;
					CalcResultAve.i16VISO1_test11 = ISO1_N;
					CalcResultAve.i16VISO1_testoutput1 = ISO1_P;
					CalcResultAve.i16VISO1_testoutput3 = ISO1_N;
					if(ISO1_P > 30000)
					{
						CalcResultAve.i16VISO1_testoutput1 = 30000;
					}
					else
					{
						CalcResultAve.i16VISO1_testoutput1 = ISO1_P;
					}
					if(ISO1_N > 30000)
					{
						CalcResultAve.i16VISO1_testoutput3 = 30000;
					}
					else
					{
						CalcResultAve.i16VISO1_testoutput3 = ISO1_N;
					}
				}
				else if(2 == BigInputFlag)
				{

					temp1 = ((int64)ISOUoVolt1)*((int64)PVvolt2*100-ISOUoVolt2);
					temp2 = (int64)PVvolt2*100*((int64)ISOUoVolt2-ISOUoVolt1)*1400;  //1400
					ISO1_P = temp2 / temp1;

					temp3 = (int64)((int64)PVvolt2*100-ISOUoVolt1)*((int64)PVvolt2*100-ISOUoVolt2);
					temp4 = (int64)PVvolt2*100*((int64)ISOUoVolt2-ISOUoVolt1)*1400*11031;   //1400
					temp5 = temp4 / temp3;

					if(temp5 >= 121682960)
					{
						ISO1_N = 10000;
					}
					else
					{
						temp3 = temp5;   //11218+33
						temp4 = 11031 - temp5/11031;
						ISO1_N = temp3 / temp4;
					}
				
					CalcResultAve.i16VISO1_test1 = ISO1_P;
					CalcResultAve.i16VISO1_test11 = ISO1_N;
					CalcResultAve.i16VISO1_testoutput2 = ISO1_P;
					CalcResultAve.i16VISO1_testoutput3 = ISO1_N;
					if(ISO1_P > 30000)
					{
						CalcResultAve.i16VISO1_testoutput2 = 30000;
					}
					else
					{
						CalcResultAve.i16VISO1_testoutput2 = ISO1_P;
					}
					if(ISO1_N > 30000)
					{
						CalcResultAve.i16VISO1_testoutput3 = 30000;
					}
					else
					{
						CalcResultAve.i16VISO1_testoutput3 = ISO1_N;
					}
				}
			}
			else
			{
			}
			if(ProcessVariable.i16ISOCheckMode_Input == 1||ProcessVariable.i16ISOCheckMode_Input == 3)
			{	
				i16IsoCheckDelay = 0;
				if(CalcResultAve.i16VISO1_testoutput1<temp6||CalcResultAve.i16VISO1_testoutput3<temp6)     //1M
				{					
					CalcResultAve.i16lost = 1;
					s_StateCheck.bit.PVIsoCheckOver = 0;
					ProcessVariable.i16IsoCheckcount = 0;					
					if(ProcessVariable.i16IsoCheckStep<2)
					{
						ProcessVariable.i16IsoCheckStep++;					
					}
					else
					{
						s_SysFaultMessage.bit.PvIsoFault = 1;
						s_StateCheck.bit.PVIsoCheckOver = 0;
						ProcessVariable.i16IsoCheckStep = 0;
					}
				}	
				else 
				{
					s_SysFaultMessage.bit.PvIsoFault = 0;					
					s_StateCheck.bit.PVIsoCheckOver = 1;
					ProcessVariable.i16IsoCheckStep = 0;	
				}
			}
			else if(ProcessVariable.i16ISOCheckMode_Input == 2)
			{
				i16IsoCheckDelay = 0;
				if(CalcResultAve.i16VISO1_testoutput2<temp6||CalcResultAve.i16VISO1_testoutput3<temp6)   //1M
				{					
					CalcResultAve.i16lost = 1;
					s_StateCheck.bit.PVIsoCheckOver = 0;
					ProcessVariable.i16IsoCheckcount = 0;
					if(ProcessVariable.i16IsoCheckStep<2)
					{
						ProcessVariable.i16IsoCheckStep++;
					}
					else
					{
						s_SysFaultMessage.bit.PvIsoFault = 1;
						s_StateCheck.bit.PVIsoCheckOver = 0;
						ProcessVariable.i16IsoCheckStep = 0;
					}
				}	
				else 
				{
					s_SysFaultMessage.bit.PvIsoFault = 0;					
					s_StateCheck.bit.PVIsoCheckOver = 1;
					ProcessVariable.i16IsoCheckStep = 0;	
				}
			}
			else if(ProcessVariable.i16ISOCheckMode_Input == 4)
			{
				i16IsoCheckDelay = 0;
					if(1 == BigInputFlag)
					{
						CalcResultAve.i16VISO1_testoutput2 = 1999;  //TEST
						if(CalcResultAve.i16VISO1_testoutput1<temp6||CalcResultAve.i16VISO1_testoutput3<temp6)   //1M
						{
						
//							i16IsoCheckDelay = 450;//8000;//380;
							CalcResultAve.i16lost = 1;
							s_StateCheck.bit.PVIsoCheckOver = 0;
							ProcessVariable.i16IsoCheckcount = 0;//9000;//0;
							if(ProcessVariable.i16IsoCheckStep<2)
							{
								ProcessVariable.i16IsoCheckStep++;
							}
							else
							{
								s_SysFaultMessage.bit.PvIsoFault = 1;
								s_StateCheck.bit.PVIsoCheckOver = 0;
								ProcessVariable.i16IsoCheckStep = 0;
							}
						}	
						else 
						{
							s_SysFaultMessage.bit.PvIsoFault = 0;					
							s_StateCheck.bit.PVIsoCheckOver = 1;
							ProcessVariable.i16IsoCheckStep = 0;	
						}
					}
					else if(2 == BigInputFlag)
					{
						CalcResultAve.i16VISO1_testoutput1 = 2999;  //TEST
						if(CalcResultAve.i16VISO1_testoutput2<temp6||CalcResultAve.i16VISO1_testoutput3<temp6)   //1M
						{						
//							i16IsoCheckDelay = 450;//8000;//380;
							CalcResultAve.i16lost = 1;
							s_StateCheck.bit.PVIsoCheckOver = 0;
							ProcessVariable.i16IsoCheckcount = 0;//9000;//0;
							if(ProcessVariable.i16IsoCheckStep<2)
							{
								ProcessVariable.i16IsoCheckStep++;
							}
							else
							{
								s_SysFaultMessage.bit.PvIsoFault = 1;
								s_StateCheck.bit.PVIsoCheckOver = 0;
								ProcessVariable.i16IsoCheckStep = 0;
							}
						}	
						else 
						{
							s_SysFaultMessage.bit.PvIsoFault = 0;					
							s_StateCheck.bit.PVIsoCheckOver = 1;
							ProcessVariable.i16IsoCheckStep = 0;	
						}
					}
				}
		}
	}
}

/**********************************************************************
* FUNCION :  
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void SPICommCheck(void)
{
	static Uint16 ExitSPiFaultCnt = 0;

	if(0 == s_SysFaultMessage.bit.SpiCommLose)
	{
		if(g_SPICommCnt > 500)
		{
			s_SysFaultMessage.bit.SpiCommLose = 1;
		}
	}
	else
	{
		if(g_SPICommCnt < 20)
		{
			ExitSPiFaultCnt++;
			if(ExitSPiFaultCnt > 2500)
			{
				s_SysFaultMessage.bit.SpiCommLose = 0;
				ExitSPiFaultCnt = 0;
			}
		}
	}
	g_SPICommCnt++;
}
//===========================================================================
// No more.
//===========================================================================

