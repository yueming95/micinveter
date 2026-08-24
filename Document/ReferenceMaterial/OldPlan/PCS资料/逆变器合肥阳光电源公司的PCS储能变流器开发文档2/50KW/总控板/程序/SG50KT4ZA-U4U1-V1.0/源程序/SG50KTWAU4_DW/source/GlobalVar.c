
#include "corecontrol.h"
STATE StateRegs;
ADCIN AI;
SYSCOUNTER SysCnt;
PLL PllRegs;
PROTECT ProtectRegs;
PROTECT ProtectIn;
PROTECTCNT ProtectCnt;
INPUT InputSignal;
_iq19 UdcF;
_iq19 IdcF;
int Ug1Mid,Ug2Mid,Ug3Mid,Ig1Mid,Ig2Mid,Ig3Mid;
long Ug1Add,Ug2Add,Ug3Add,Ig1Add,Ig2Add,Ig3Add;
_iq21 Udc_Given;
_iq21 Udc_Given_com;
_iq21 Iq_Given_com;
_iq21 Id_Given_com;
_iq21 Iq_Given;
_iq21 Id_Given;
_iq28 Vd,Vq,Alfa,Beta;
SVGEN svgen = SVGEN_DEFAULTS;
Uint32 PVCheckTM;
_iq19 *bufaddr;
_iq19 buf1[100],buf2[100],buf3[100];
unsigned char Display;
Uint32 TotalCO2;
Uint32 TotalEnergy;
Uint32 TotalTime;

