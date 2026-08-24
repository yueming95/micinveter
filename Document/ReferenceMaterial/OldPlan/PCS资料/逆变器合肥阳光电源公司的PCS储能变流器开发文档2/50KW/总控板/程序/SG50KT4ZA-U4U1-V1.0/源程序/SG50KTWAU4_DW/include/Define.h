#ifndef Define_H
#define Define_H
#include "IQmathLib.h"
#define CarrierWaveFreq 5000   //PWM载波频率
#define InterruptFreq 5000  //为中断控制频率，与实际的pwm载波频率可以不一致
#define BOOSTPR 6000//(Uint16)(BOOSTPR2/2)  5K 6000
#define IntCnt 1//(Uint16)(CarrierWaveFreq/InerruptFreq)
#define Cycle 100//(Uint16)(CarrierWaveFreq/Freq)
//时间定义
#define TM30S 150000//(Uint32)(InterruptFreq*30)
#define TM20S 100000//(Uint32)(InterruptFreq*20)
#define TM10S 50000//(Uint32)(InterruptFreq*10)
#define TM8S  40000
#define TM5S  25000//(Uint32)(InterruptFreq*5)
#define TM3S  15000//(Uint32)(InterruptFreq*3)
#define TM2S  10000
#define TM1S  5000
#define TM400MS 2000
#define TM10MS 50
#define TM100MS 500
#define TM500MS 2500
#define TM600MS 3000
#define TM4M   1200000

#define KIpv _IQ(0.0411987)		
#define KUpv _IQ(0.282246)
#define STARTUPV _IQ(640)    // 启动电压
#define STOPUPV  _IQ(620)    // 停机电压
#define UPVOVER _IQ(900)
#endif

