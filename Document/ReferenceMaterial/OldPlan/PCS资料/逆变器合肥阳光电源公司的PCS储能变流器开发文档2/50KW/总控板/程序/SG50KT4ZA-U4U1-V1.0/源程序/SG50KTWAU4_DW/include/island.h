#ifndef _ISLAND_H
#define _ISLAND_H
#include "IQmathLib.h"
#ifdef island_GLOBALS
#define island_GLOBALS
#else
#define island_GLOBALS extern
#endif
void IslandFun();
island_GLOBALS _iq22 IsUa[4];
island_GLOBALS float IsUaCycle;
island_GLOBALS float IsFreq;
island_GLOBALS _iq22 IsWg;
island_GLOBALS _iq22 IsKi;
island_GLOBALS _iq22 IsPi;
island_GLOBALS _iq22 IsTheta;
island_GLOBALS _iq22 IsKp;
island_GLOBALS _iq22 IsIa;
island_GLOBALS unsigned int IsCnt;
#endif

