/***********************************************************************

 *    FILENAME : SL_slave_Gridperiodcalc.c
 *
 *    PURPOSE  : 
 *    HISTORY  :
 *           DATE          VERSION      AUTHOR        NOTE
 *
 *
 ************************************************************************/
#include "SL_slave.h"

// int16  TempAbm[126] = {-25,-24,-23,-22,-21,-20,-19,-18,-17,-16,-15,-14,-13,-12,-11,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99};
const int16 VEnvTemp[128] = {4096,3680,3678,3676,3674,3673,3670,3668,3666,3664,3661,3659,3656,3654,//-13
3651,3648,3645,3641,3638,3635,3631,3627,3623,3619,3615,3611,3606,3602,3597,3592,3587,3581,3576,3570,//7
3564,3558,3552,3545,3539,3532,3525,3517,3510,3502,3494,3486,3477,3469,3460,3450,3441,3431,3421,3411,//27
3401,3390,3379,3368,3356,3344,3332,3320,3307,3294,3281,3267,3254,3240,3225,3210,3196,3180,3165,3149,//47
3133,3116,3100,3083,3065,3048,3030,3012,2993,2975,2956,2937,2917,2898,2878,2858,2837,2817,2796,2775,//67
2754,2732,2711,2689,2667,2645,2623,2600,2577,2555,2532,2509,2486,2462,2439,2416,2392,2369,2345,2321,//87
2297,2274,2250,2226,2202,2178,2154,2131,2107,2083,2059,2036,2012,0};//101

const int16 VInvBoostTemp[208] = {4096,2845,2844,2843,2842,2841,2840,2839,2838,2836,2835,2834,2832,2830,2828,2826, 
2824,2822,2819,2817,2814,2812,2809,2805,2802,2798,2795,2791,2787,2782,2778,2774,2769,2763,2758,2752,2747,2740,
2733,2726,2719,2712,2703,2695,2686,2678,2669,2658,2648,2637,2627,2616,2603,2590,2578,2565,2552,2537,2522,2507,
2492,2477,2459,2442,2424,2407,2389,2369,2349,2329,2309,2289,2267,2245,2222,2200,2178,2154,2129,2105,2080,2056,
2030,2004,1978,1952,1926,1899,1871,1844,1816,1789,1761,1733,1706,1678,1650,1622,1594,1566,1538,1510,1482,1455,
1427,1400,1372,1344,1317,1289,1262,1234,1210,1186,1161,1137,1113,1089,1066,1042,1019,995,973,951,929,907,885,
865,845,826,806,786,768,750,731,713,695,679,663,647,631,615,600,586,571,557,542,529,517,504,492,479,468,456,
445,433,422,412,402,393,383,373,364,355,347,338,329,321,314,306,299,291,284,277,271,264,257,251,245,240,234, 
228,223,218,212,207,202,198,193,189,184,180,176,172,168,164,160,157,153,150,146,143,140,137,133,130,127,0};

const int16 V16KBoostTemp[128] ={4096,3599,3582,3564,3546,3526,3506,3485,3464,3441,3418,3393,3368,3342,3315,3287,3259,3229,3199,3168,
3136,3103,3070,3036,3001,2965,2928,2891,2853,2815,2776,2736,2696,2656,2615,2573,2531,2489,2447,2404,
2361,2318,2274,2231,2187,2144,2101,2057,2014,1971,1928,1885,1843,1801,1759,1718,1677,1637,1597,1557,
1518,1480,1442,1405,1368,1332,1297,1262,1228,1195,1162,1130,1099,1068,1038,1009,981,953,926,899,874,
848,824,800,777,754,733,711,691,670,651,632,613,595,578,561,545,529,514,499,484,470,456,443,430,418,
406,394,383,372,361,351,341,331,322,313,304,295,287,279,271,264,256,249,242,236,229,0};

/**********************************************************************
* FUNCION :  Adc_Getvalue1
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void Adc_Getvalue1(void)
{
    int16 i16temp1,i16temp2,i16temp3;

	i16temp1 = ADC_RESULT_VGRID_R - AD_DEFAULT_AC_OFFSET;		
	i16temp2 = ADC_RESULT_VGRID_S - AD_DEFAULT_AC_OFFSET;
	i16temp3 = ADC_RESULT_VGRID_T - AD_DEFAULT_AC_OFFSET;

	GetRealValue.i16VGRID_R = (int16)(((int32)i16temp1 * ADGain.i16VGRID_R)>>12) - ADChannelOffset.i16VGRID_R;
	GetRealValue.i16VGRID_S = (int16)(((int32)i16temp2 * ADGain.i16VGRID_S)>>12) - ADChannelOffset.i16VGRID_S;
	GetRealValue.i16VGRID_T = (int16)(((int32)i16temp3 * ADGain.i16VGRID_T)>>12) - ADChannelOffset.i16VGRID_T;	

	i16temp1 = ADC_RESULT_VINV_R - AD_DEFAULT_AC_OFFSET;
	i16temp2 = ADC_RESULT_VINV_S - AD_DEFAULT_AC_OFFSET;				                      	                             
	i16temp3 = ADC_RESULT_VINV_T - AD_DEFAULT_AC_OFFSET;
	
	GetRealValue.i16VINV_R = (int16)(((int32)i16temp1 * ADGain.i16VINV_R)>>12) - ADChannelOffset.i16VINV_R;
	GetRealValue.i16VINV_S = (int16)(((int32)i16temp2 * ADGain.i16VINV_S)>>12) - ADChannelOffset.i16VINV_S;
	GetRealValue.i16VINV_T = (int16)(((int32)i16temp3 * ADGain.i16VINV_T)>>12) - ADChannelOffset.i16VINV_T;	
		
	i16temp1 = ADC_RESULT_DCI_R - AD_DEFAULT_AC_OFFSET;
	i16temp2 = ADC_RESULT_DCI_S - AD_DEFAULT_AC_OFFSET;				                      	                             
	i16temp3 = ADC_RESULT_DCI_T - AD_DEFAULT_AC_OFFSET;   	

	GetRealValue.i16DCI_R = (int16)(((int32)i16temp1 * ADGain.i16DCI_R)>>12) - ADChannelOffset.i16DCI_R;
	GetRealValue.i16DCI_S = (int16)(((int32)i16temp2 * ADGain.i16DCI_S)>>12) - ADChannelOffset.i16DCI_S;
	GetRealValue.i16DCI_T = (int16)(((int32)i16temp3 * ADGain.i16DCI_T)>>12) - ADChannelOffset.i16DCI_T;	
}

/**********************************************************************
* FUNCION :  Adc_Getvalue2
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void Adc_Getvalue2(void)
{
    int16 i16temp1;

	static Uint16 i16ADSelectCounter = 0; 

	GetRealValue.i16VPV1 = (int16)(((int32)ADC_RESULT_VPV1 * ADGain.i16VPV1)>>12) - ADOffsetSum.i16VPV1;
	GetRealValue.i16VPV2 = (int16)(((int32)ADC_RESULT_VPV2 * ADGain.i16VPV2)>>12) - ADOffsetSum.i16VPV2;

	GetRealValue.i16VISO1 = (int16)(((int32)ADC_RESULT_VISO1 * ADGain.i16VISO1)>>12);
	GetRealValue.i16VISO2 = (int16)(((int32)ADC_RESULT_VISO2 * ADGain.i16VISO2)>>12);	

	i16temp1 = ADC_RESULT_GFCI - AD_DEFAULT_AC_OFFSET;
	GetRealValue.i16GFCI = (int16)(((int32)i16temp1 * ADGain.i16GFCI)>>12) - ADChannelOffset.i16GFCI;  	
    
	switch(i16ADSelectCounter)
	{
		case 0:
			GetRealValue.i16TempAmb = (int16)ADC_RESULT_MUX_X;//(int16)(((int32)ADC_RESULT_MUX_X * ADGain.i16TempAmb)>>12);
			GetRealValue.i16TempINVR = (int16)(((int32)ADC_RESULT_MUX_Y * ADGain.i16TempINVR)>>12);			

			GpioDataRegs.GPASET.bit.GPIO10 = 1; 
			GpioDataRegs.GPACLEAR.bit.GPIO3 = 1; 

			i16ADSelectCounter = 1;
		break;
		case 1:
			GetRealValue.i16TempBoost2 = (int16)(((int32)ADC_RESULT_MUX_X * ADGain.i16TempBoost2)>>12);
			GetRealValue.i16TempINVT = (int16)(((int32)ADC_RESULT_MUX_Y * ADGain.i16TempINVT)>>12);			
			
			GpioDataRegs.GPACLEAR.bit.GPIO10 = 1; 
			GpioDataRegs.GPASET.bit.GPIO3 = 1; 

			i16ADSelectCounter = 2;
		break;
		case 2:
			GetRealValue.i16TempBoost1 = (int16)(((int32)ADC_RESULT_MUX_X * ADGain.i16TempBoost1)>>12);
			GetRealValue.i16TempINVS = (int16)(((int32)ADC_RESULT_MUX_Y * ADGain.i16TempINVS)>>12);			
			
			GpioDataRegs.GPACLEAR.bit.GPIO10 = 1; 
			GpioDataRegs.GPACLEAR.bit.GPIO3 = 1; 

			i16ADSelectCounter = 0;
		break;
		default:			
			GpioDataRegs.GPACLEAR.bit.GPIO10 = 1; 
			GpioDataRegs.GPACLEAR.bit.GPIO3 = 1; 

			i16ADSelectCounter = 0;
		break;	
	}
}
/**********************************************************************
* FUNCION :ADAccCalc()
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void ADAccCalc(void)
{
	ADAccAve.i16Counter++;
	ADAccRms.i16Counter++;

	ADAccAve.i32VPV1 += GetRealValue.i16VPV1;
	ADAccAve.i32VPV2 += GetRealValue.i16VPV2;

	ADAccAve.i32VISO1 += GetRealValue.i16VISO1;
	ADAccAve.i32VISO2 += GetRealValue.i16VISO2;
	
	ADAccAve.i32TempINVR += GetRealValue.i16TempINVR;
	ADAccAve.i32TempINVS += GetRealValue.i16TempINVS;
	ADAccAve.i32TempINVT += GetRealValue.i16TempINVT;

	ADAccAve.i32TempBoost1 += GetRealValue.i16TempBoost1;
	ADAccAve.i32TempBoost2 += GetRealValue.i16TempBoost2;

	ADAccAve.i32TempAmb += GetRealValue.i16TempAmb;

//	if (1 == s_StateCheck.bit.AD_initial)
//	{
		ADAccAve.i32VGRID_R += GetRealValue.i16VGRID_R;
		ADAccAve.i32VGRID_S += GetRealValue.i16VGRID_S;
		ADAccAve.i32VGRID_T += GetRealValue.i16VGRID_T;

		ADAccAve.i32VINV_R += GetRealValue.i16VINV_R;
		ADAccAve.i32VINV_S += GetRealValue.i16VINV_S;
		ADAccAve.i32VINV_T += GetRealValue.i16VINV_T;

		ADAccAve.i32DCI_R += GetRealValue.i16DCI_R;
		ADAccAve.i32DCI_S += GetRealValue.i16DCI_S;
		ADAccAve.i32DCI_T += GetRealValue.i16DCI_T;

		ADAccAve.i32GFCI += GetRealValue.i16GFCI;
//	}

	ADAccRms.i32VGRID_R +=	(int32)GetRealValue.i16VGRID_R * GetRealValue.i16VGRID_R;
	ADAccRms.i32VGRID_S +=	(int32)GetRealValue.i16VGRID_S * GetRealValue.i16VGRID_S;
	ADAccRms.i32VGRID_T +=	(int32)GetRealValue.i16VGRID_T * GetRealValue.i16VGRID_T;

	ADAccRms.i32VINV_R  +=	(int32)GetRealValue.i16VINV_R * GetRealValue.i16VINV_R;
	ADAccRms.i32VINV_S  +=	(int32)GetRealValue.i16VINV_S * GetRealValue.i16VINV_S;
	ADAccRms.i32VINV_T  +=	(int32)GetRealValue.i16VINV_T * GetRealValue.i16VINV_T;

	ADAccRms.i32DCI_R   +=	(int32)GetRealValue.i16DCI_R * GetRealValue.i16DCI_R;
	ADAccRms.i32DCI_S   +=	(int32)GetRealValue.i16DCI_S * GetRealValue.i16DCI_S;
	ADAccRms.i32DCI_T   +=	(int32)GetRealValue.i16DCI_T * GetRealValue.i16DCI_T;

	ADAccRms.i32VGrid_RSrms += (int32)(GetRealValue.i16VGRID_R - GetRealValue.i16VGRID_S) * (GetRealValue.i16VGRID_R - GetRealValue.i16VGRID_S);
	ADAccRms.i32VGrid_STrms += (int32)(GetRealValue.i16VGRID_S - GetRealValue.i16VGRID_T) * (GetRealValue.i16VGRID_S - GetRealValue.i16VGRID_T);
	ADAccRms.i32VGrid_TRrms += (int32)(GetRealValue.i16VGRID_T - GetRealValue.i16VGRID_R) * (GetRealValue.i16VGRID_T - GetRealValue.i16VGRID_R);

	ADAccRms.i32VInv_RSrms += (int32)(GetRealValue.i16VINV_R - GetRealValue.i16VINV_S) * (GetRealValue.i16VINV_R - GetRealValue.i16VINV_S);
	ADAccRms.i32VInv_STrms += (int32)(GetRealValue.i16VINV_S - GetRealValue.i16VINV_T) * (GetRealValue.i16VINV_S - GetRealValue.i16VINV_T);
	ADAccRms.i32VInv_TRrms += (int32)(GetRealValue.i16VINV_T - GetRealValue.i16VINV_R) * (GetRealValue.i16VINV_T - GetRealValue.i16VINV_R);

	ADAccRms.i32GFCI  += (int32)GetRealValue.i16GFCI * GetRealValue.i16GFCI;

	if(1 == s_StateCheck.bit.Zero_Crossing_Flag)
	{
		s_StateCheck.bit.Zero_Crossing_Flag = 0;

		ADSumAve.i16Counter = ADAccAve.i16Counter;
		ADSumRms.i16Counter = ADAccRms.i16Counter;

		ADSumAve.i32VPV1 = ADAccAve.i32VPV1;
		ADSumAve.i32VPV2 = ADAccAve.i32VPV2;

		ADSumAve.i32VISO1 = ADAccAve.i32VISO1;
		ADSumAve.i32VISO2 = ADAccAve.i32VISO2;
		
		ADSumAve.i32TempINVR = ADAccAve.i32TempINVR;
		ADSumAve.i32TempINVS = ADAccAve.i32TempINVS;
		ADSumAve.i32TempINVT = ADAccAve.i32TempINVT ;

		ADSumAve.i32TempBoost1 = ADAccAve.i32TempBoost1;
		ADSumAve.i32TempBoost2 = ADAccAve.i32TempBoost2;

		ADSumAve.i32TempAmb = ADAccAve.i32TempAmb;

		ADSumAve.i32VGRID_R = ADAccAve.i32VGRID_R;
		ADSumAve.i32VGRID_S = ADAccAve.i32VGRID_S;
		ADSumAve.i32VGRID_T = ADAccAve.i32VGRID_T;

		ADSumAve.i32VINV_R = ADAccAve.i32VINV_R;
		ADSumAve.i32VINV_S = ADAccAve.i32VINV_S;
		ADSumAve.i32VINV_T = ADAccAve.i32VINV_T;

		ADSumAve.i32DCI_R = ADAccAve.i32DCI_R;
		ADSumAve.i32DCI_S = ADAccAve.i32DCI_S;
		ADSumAve.i32DCI_T = ADAccAve.i32DCI_T;

		ADSumAve.i32GFCI = ADAccAve.i32GFCI;

		ADSumRms.i32VGRID_R = ADAccRms.i32VGRID_R;
		ADSumRms.i32VGRID_S = ADAccRms.i32VGRID_S;
		ADSumRms.i32VGRID_T = ADAccRms.i32VGRID_T;

		ADSumRms.i32VINV_R = ADAccRms.i32VINV_R;
		ADSumRms.i32VINV_S = ADAccRms.i32VINV_S;
		ADSumRms.i32VINV_T = ADAccRms.i32VINV_S;

		ADSumRms.i32DCI_R = ADAccRms.i32DCI_R;
		ADSumRms.i32DCI_S = ADAccRms.i32DCI_S;
		ADSumRms.i32DCI_T = ADAccRms.i32DCI_T;

		ADSumRms.i32VGrid_RSrms = ADAccRms.i32VGrid_RSrms;
		ADSumRms.i32VGrid_STrms = ADAccRms.i32VGrid_RSrms;
		ADSumRms.i32VGrid_TRrms = ADAccRms.i32VGrid_TRrms;

		ADSumRms.i32VInv_RSrms = ADAccRms.i32VInv_RSrms;
		ADSumRms.i32VInv_STrms = ADAccRms.i32VInv_STrms;
		ADSumRms.i32VInv_TRrms = ADAccRms.i32VInv_TRrms;

		ADSumRms.i32GFCI = ADAccRms.i32GFCI;

		ADAccAve.i16Counter = 0;
		ADAccAve.u16Counter = 0;

		ADAccAve.i32VPV1 = 0;
		ADAccAve.i32VPV2 = 0;
		ADAccAve.i32VISO1 = 0;
		ADAccAve.i32VISO2 = 0;
		
		ADAccAve.i32TempINVR = 0;
		ADAccAve.i32TempINVS = 0;
		ADAccAve.i32TempINVT = 0;

		ADAccAve.i32TempBoost1 = 0;
		ADAccAve.i32TempBoost2 = 0;

		ADAccAve.i32TempAmb = 0;

		ADAccAve.i32VGRID_R = 0;
		ADAccAve.i32VGRID_S = 0;
		ADAccAve.i32VGRID_T = 0;

		ADAccAve.i32VINV_R = 0;
		ADAccAve.i32VINV_S = 0;
		ADAccAve.i32VINV_T = 0;

		ADAccAve.i32DCI_R = 0;
		ADAccAve.i32DCI_S = 0;
		ADAccAve.i32DCI_T = 0;

		ADAccAve.i32GFCI = 0;

		ADAccRms.i16Counter = 0;
		ADAccRms.u16Counter = 0;

		ADAccRms.i32VGRID_R = 0;
		ADAccRms.i32VGRID_S = 0;
		ADAccRms.i32VGRID_T = 0;

		ADAccRms.i32VINV_R = 0;
		ADAccRms.i32VINV_S = 0;
		ADAccRms.i32VINV_T = 0;

		ADAccRms.i32DCI_R = 0;
		ADAccRms.i32DCI_S = 0;
		ADAccRms.i32DCI_T = 0;

		ADAccRms.i32VGrid_RSrms = 0;
		ADAccRms.i32VGrid_STrms = 0;
		ADAccRms.i32VGrid_TRrms = 0;

		ADAccRms.i32VInv_RSrms = 0;
		ADAccRms.i32VInv_STrms = 0;
		ADAccRms.i32VInv_TRrms = 0;

		ADAccRms.i32GFCI = 0;

		SEM_post(&SEM_TskGridPeriodCalcReady);
	}
}

/**********************************************************************
* FUNCION : TSK_F_GridPeriod
* PURPOSE : Calculate average values every grid period.
*			Calculate RMS values every grid period.
* HISTORY :  DATE             AUTHOR        NOTE
*
**********************************************************************/
void TSK_F_GridPeriodCalc(void)
{
	static Uint16 IsoRecoverCnt = 0;
	static Uint16 PLLOverCnt = 0;
	while(1)
	{
		SEM_pend(&SEM_TskGridPeriodCalcReady, SYS_FOREVER);

		PLLOverCnt++;
		if(PLLOverCnt > 800)
		{
			PLLOverCnt = 801;
			s_StateCheck.bit.PLLOver = 1;
		}	
			
		if(0 == ADSumAve.i16Counter || 0 == ADSumRms.i16Counter)
		{
			continue;
		}

		VGrid_ave_Calc();
		VInv_ave_Calc();
		DCI_ave_Calc();
		GFCI_ave_Calc();

		if(1 == s_StateCheck.bit.AD_initial && 1 == s_StateCheck.bit.PLLOver)
		{
//			VGrid_ave_Calc();
//			VInv_ave_Calc();
//			DCI_ave_Calc();
//			GFCI_ave_Calc();

			ADOffsetCalibration();
		}


		VPV_ave_Calc();
		TEMP_ave_Calc();
		  
		VGrid_rms_Calc();
		VInv_rms_Calc();
		DCI_rms_Calc();
		GFCI_rms_Calc();

		TemperatureCheck();	

		if(1 == s_SysFaultMessage.bit.PvIsoFault)
		{
			IsoRecoverCnt++;
			if(IsoRecoverCnt > 3000)
			{
				IsoRecoverCnt = 0;
				s_SysFaultMessage.bit.PvIsoFault = 0;
				s_StateCheck.bit.PVIsoCheckOver = 0;
			}			
		}
		else
		{
			IsoRecoverCnt = 0;
		}


	}
}

/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void VPV_ave_Calc()
{
	CalcResultAve.i16VPV1 = ADSumAve.i32VPV1 / ADSumAve.i16Counter;
	CalcResultAve.i16VPV2 = ADSumAve.i32VPV2 / ADSumAve.i16Counter;

	CalcResultAve.i16VISO1 = ADSumAve.i32VISO1 / ADSumAve.i16Counter;
 	CalcResultAve.i16VISO2 = ADSumAve.i32VISO2 / ADSumAve.i16Counter;
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void TEMP_ave_Calc()
{
	int16 i16temp;
	static Uint16 u16Count =0;
	
	u16Count ++;
	if(u16Count >50)
	{
		u16Count = 1;
	}

	switch(u16Count)
	{
		case   5:
			TEMP_INVR_Calc();

		break;
		
		case  15:
			TEMP_INVS_Calc();

		break;
		
		case 25:
			TEMP_INVT_Calc();

		break;

		case 35:
			TEMP_Boost_Calc();//

		break;
		case 45:
			TEMP_Amb_Calc();
		break;
		default :
		break;		
	}		
	if(CalcResultAve.i16TempINVR > CalcResultAve.i16TempINVS)
	{
		i16temp = CalcResultAve.i16TempINVR;
	}
	else
	{
		i16temp = CalcResultAve.i16TempINVS;
	}

	if(i16temp > CalcResultAve.i16TempINVT)
	{
		CalcResultAve.i16TempINVMax = i16temp;
	}
	else
	{
		CalcResultAve.i16TempINVMax = CalcResultAve.i16TempINVT;
	}



}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void TEMP_INVR_Calc()
{
	int16 i16temp1;
	int16 i16temp2;
	int16 i = 0;

	static int16 i16EnvTempSum = 0;
	static int16 i16TempCnt = 0;

	i16temp1 = ADSumAve.i32TempINVR / ADSumAve.i16Counter;

	for(i=0;i<206;i++)
	{
		if((i16temp1<VInvBoostTemp[i])&&(i16temp1>=VInvBoostTemp[i+1]))
		{
			i16temp2 = i-55;
			break;
		}			
	}

	i16TempCnt++;
	i16EnvTempSum += i16temp2;
	if(i16TempCnt>=4)
	{
		CalcResultAve.i16TempINVR = (i16EnvTempSum>>2);
		i16EnvTempSum = 0;
		i16TempCnt = 0;
	}
}

void TEMP_INVS_Calc()
{
	int16 i16temp1;
	int16 i16temp2;
	int16 i = 0;

	static int16 i16EnvTempSum = 0;
	static int16 i16TempCnt = 0;

	i16temp1 = ADSumAve.i32TempINVS / ADSumAve.i16Counter;

	for(i=0;i<207;i++)
	{
		if((i16temp1<VInvBoostTemp[i])&&(i16temp1>=VInvBoostTemp[i+1]))
		{
			i16temp2 = i-55;
			break;
		}			
	}

	i16TempCnt++;
	i16EnvTempSum += i16temp2;
	if(i16TempCnt>=4)
	{
		CalcResultAve.i16TempINVS = (i16EnvTempSum>>2);
		i16EnvTempSum = 0;
		i16TempCnt = 0;
	}
}
void TEMP_INVT_Calc()
{
	int16 i16temp1;
	int16 i16temp2;
	int16 i = 0;

	static int16 i16EnvTempSum = 0;
	static int16 i16TempCnt = 0;

	i16temp1 = ADSumAve.i32TempINVT / ADSumAve.i16Counter;

	for(i=0;i<207;i++)
	{
		if((i16temp1<VInvBoostTemp[i])&&(i16temp1>=VInvBoostTemp[i+1]))
		{
			i16temp2 = i-55;
			break;
		}			
	}

	i16TempCnt++;
	i16EnvTempSum += i16temp2;
	if(i16TempCnt>=4)
	{
		CalcResultAve.i16TempINVT = (i16EnvTempSum>>2);
		i16EnvTempSum = 0;
		i16TempCnt = 0;
	}
}
void TEMP_Boost_Calc()
{
	int16 i16temp1;
	int16 i16temp2;
	int16 i = 0;

	static int16 i16EnvTempSum = 0;
	static int16 i16TempCnt = 0;

	i16temp1 = ADSumAve.i32TempBoost1 / ADSumAve.i16Counter;
		
	for(i=0;i<127;i++)
	{
		if((i16temp1<V16KBoostTemp[i])&&(i16temp1>=V16KBoostTemp[i+1]))
		{
			i16temp2 = i-20;
			break;
		}			
	}

	i16TempCnt++;
	i16EnvTempSum += i16temp2;
	if(i16TempCnt>=4)
	{
		CalcResultAve.i16TempBoost1 = (i16EnvTempSum>>2);
		i16EnvTempSum = 0;
		i16TempCnt = 0;
	}

}
void TEMP_Amb_Calc()
{
	int16 i16temp1;
	int16 i16temp2;
	int16 i = 0;

	static int16 i16EnvTempSum = 0;
	static int16 i16TempCnt = 0;

	i16temp1 = ADSumAve.i32TempAmb / ADSumAve.i16Counter;

	for(i=0;i<127;i++)
	{
		if((i16temp1<VEnvTemp[i])&&(i16temp1>=VEnvTemp[i+1]))
		{
			i16temp2 = i-26;
			break;
		}			
	}

	i16TempCnt++;
	i16EnvTempSum += i16temp2;
	if(i16TempCnt>=4)
	{
		CalcResultAve.i16TempAmb = (i16EnvTempSum>>2);
		i16EnvTempSum = 0;
		i16TempCnt = 0;
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void VGrid_ave_Calc()
{
	CalcResultAve.i16VGRID_R = (ADSumAve.i32VGRID_R / ADSumAve.i16Counter)*4;
	CalcResultAve.i16VGRID_S = (ADSumAve.i32VGRID_S / ADSumAve.i16Counter)*4;
	CalcResultAve.i16VGRID_T = (ADSumAve.i32VGRID_T / ADSumAve.i16Counter)*4;
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void VInv_ave_Calc()	
{
	CalcResultAve.i16VINV_R = (ADSumAve.i32VINV_R / ADSumAve.i16Counter)*4;
	CalcResultAve.i16VINV_S = (ADSumAve.i32VINV_S / ADSumAve.i16Counter)*4;
	CalcResultAve.i16VINV_T = (ADSumAve.i32VINV_T / ADSumAve.i16Counter)*4;
}
						
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 	
**********************************************************************/
void DCI_ave_Calc()
{
	CalcResultAve.i16DCI_R = ADSumAve.i32DCI_R / ADSumAve.i16Counter;
	CalcResultAve.i16DCI_S = ADSumAve.i32DCI_S / ADSumAve.i16Counter;
	CalcResultAve.i16DCI_T = ADSumAve.i32DCI_T / ADSumAve.i16Counter;
}

/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void GFCI_ave_Calc()
{
	CalcResultAve.i16GFCI = ADSumAve.i32GFCI / ADSumAve.i16Counter;
}

/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void VGrid_rms_Calc()
{
	CalcResultRms.i16VGRID_R = sqrt(ADSumRms.i32VGRID_R / ADSumRms.i16Counter)*4;
	CalcResultRms.i16VGRID_S = sqrt(ADSumRms.i32VGRID_S / ADSumRms.i16Counter)*4;
	CalcResultRms.i16VGRID_T = sqrt(ADSumRms.i32VGRID_T / ADSumRms.i16Counter)*4;

	CalcResultRms.i16VGrid_RSrms = sqrt(ADSumRms.i32VGrid_RSrms / ADSumRms.i16Counter)*4;
	CalcResultRms.i16VGrid_STrms = sqrt(ADSumRms.i32VGrid_STrms / ADSumRms.i16Counter)*4;
	CalcResultRms.i16VGrid_TRrms = sqrt(ADSumRms.i32VGrid_TRrms / ADSumRms.i16Counter)*4;

}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void VInv_rms_Calc()	
{
	CalcResultRms.i16VINV_R = sqrt(ADSumRms.i32VINV_R  / ADSumRms.i16Counter)*4;
	CalcResultRms.i16VINV_S = sqrt(ADSumRms.i32VINV_S  / ADSumRms.i16Counter)*4;
	CalcResultRms.i16VINV_T = sqrt(ADSumRms.i32VINV_T  / ADSumRms.i16Counter)*4;

	CalcResultRms.i16VInv_RSrms = sqrt(ADSumRms.i32VInv_RSrms  / ADSumRms.i16Counter)*4;
	CalcResultRms.i16VInv_STrms = sqrt(ADSumRms.i32VInv_STrms  / ADSumRms.i16Counter)*4;
	CalcResultRms.i16VInv_TRrms = sqrt(ADSumRms.i32VInv_TRrms  / ADSumRms.i16Counter)*4;
}
						
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void DCI_rms_Calc()
{
	CalcResultRms.i16DCI_R = sqrt(ADSumRms.i32DCI_R  / ADSumRms.i16Counter);
	CalcResultRms.i16DCI_S = sqrt(ADSumRms.i32DCI_S  / ADSumRms.i16Counter);
	CalcResultRms.i16DCI_T = sqrt(ADSumRms.i32DCI_T  / ADSumRms.i16Counter);
}

/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void GFCI_rms_Calc()
{
	CalcResultRms.i16GFCI = sqrt(ADSumRms.i32GFCI / ADSumRms.i16Counter);
}

/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void GridFreqCalc()
{
	static Uint32 Count = 0;
	static Uint32 Grid_Pos_Flag = 1;
	static Uint32 Grid_Neg_Flag = 0;

	Count++;
	if((PLLCal.i16ThetaCos < 1000)&&(1 == Grid_Neg_Flag))
	{
		Grid_Pos_Flag = 1;
		Grid_Neg_Flag = 0;
		CalcResultRms.i16GridRFreq = 20000/Count;
		Count = 0;
	}
	else if((PLLCal.i16ThetaCos > 2048)&&(1 == Grid_Pos_Flag))
	{
		Grid_Pos_Flag = 0;
		Grid_Neg_Flag = 1;
	}
	else
	{
	}
}
/**********************************************************************
* FUNCION :
* PURPOSE :  
* CALLED BY: 
**********************************************************************/
void ADOffsetCalibration(void)
{
	static Uint16 cnt = 0;

	if(cnt <= 32)
	{
		if(cnt > 0)	// 从锁相后第2个周期开始累加，确保完整周期
		{
			ADOffsetSum.i32VGRID_R += CalcResultAve.i16VGRID_R;
			ADOffsetSum.i32VGRID_S += CalcResultAve.i16VGRID_S;
			ADOffsetSum.i32VGRID_T += CalcResultAve.i16VGRID_T;

			ADOffsetSum.i32VINV_R += CalcResultAve.i16VINV_R;
			ADOffsetSum.i32VINV_S += CalcResultAve.i16VINV_S;
			ADOffsetSum.i32VINV_T += CalcResultAve.i16VINV_T;

			ADOffsetSum.i32DCI_R += CalcResultAve.i16DCI_R;
			ADOffsetSum.i32DCI_S += CalcResultAve.i16DCI_S;
			ADOffsetSum.i32DCI_T += CalcResultAve.i16DCI_T;

			ADOffsetSum.i32GFCI += CalcResultAve.i16GFCI;
		}
		cnt++;
	}
	else
	{
		ADOffsetCheck();

		ADOffsetSum.i32VGRID_R = 0;
		ADOffsetSum.i32VGRID_S = 0;
		ADOffsetSum.i32VGRID_T = 0;

		ADOffsetSum.i32VINV_R = 0;
		ADOffsetSum.i32VINV_S = 0;
		ADOffsetSum.i32VINV_T = 0;

		ADOffsetSum.i32DCI_R = 0;
		ADOffsetSum.i32DCI_R = 0;
		ADOffsetSum.i32DCI_R = 0;

		ADOffsetSum.i32GFCI = 0;

		cnt = 0;
	}
}
//===========================================================================
// No more.
//===========================================================================


