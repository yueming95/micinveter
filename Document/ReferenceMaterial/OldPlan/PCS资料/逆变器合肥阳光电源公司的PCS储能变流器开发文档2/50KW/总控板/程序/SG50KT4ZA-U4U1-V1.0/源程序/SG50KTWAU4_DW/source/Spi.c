#include "Spi.h"
#include "DSP2833x_Examples.h"
//-------本文件为通过spi口存取AT25010A的所有函数封装---------------------//

extern void DSP28x_usDelay(Uint32 Count);
void SpiInit()  //spi初始化
{
	CS=1;
	SpiaRegs.SPICCR.all=0x0007;
	SpiaRegs.SPICTL.all=0x000E;//主模式，使能发送，禁止中断 
	SpiaRegs.SPIBRR=7;//37.5/8
	SpiaRegs.SPIFFRX.all=0x600F;
	SpiaRegs.SPIFFTX.all=0xE000;
	SpiaRegs.SPICCR.bit.SPISWRESET=1;
}
void WriteChar(unsigned int add,unsigned int a)  //写一个字节a到地址为add 的EEROM空间
{
	unsigned int tmp1,tmp2;	
	CS=0;
	SpiaRegs.SPITXBUF=0x0600;
	while(SpiaRegs.SPIFFRX.bit.RXFFST==0){}
	SpiaRegs.SPIRXBUF=SpiaRegs.SPIRXBUF;
	CS=1;
	DELAY_US(1);
	CS=0;
	tmp1=add<<8;
	tmp2=a<<8;
	SpiaRegs.SPITXBUF=0x0200;
	SpiaRegs.SPITXBUF=tmp1;
	SpiaRegs.SPITXBUF=tmp2;
	while(SpiaRegs.SPIFFRX.bit.RXFFST<3){}
	SpiaRegs.SPIRXBUF=SpiaRegs.SPIRXBUF;
	SpiaRegs.SPIRXBUF=SpiaRegs.SPIRXBUF;
	SpiaRegs.SPIRXBUF=SpiaRegs.SPIRXBUF;
	CS=1;
	DELAY_US(1);
}
void WriteInt(unsigned int add,unsigned int ab)//向起始地址为add的空间写无符号整形ab
{
	unsigned int tmp;
	tmp=ab>>8;
	WriteChar(add,ab);  //先写低八位
	DELAY_US(3000);
	WriteChar(add+1,tmp);//再写高八位
	DELAY_US(3000);
}
void WriteInt32(unsigned int aadd,long int abc) //向起始地址为aadd的空间写有符号长整形abc
{
	unsigned int temp;
	temp=abc;
	WriteInt(aadd,temp);
	temp=abc>>16;
	WriteInt(aadd+2,temp);	 
}
unsigned int ReadChar(unsigned int add) //从地址add度一个字节
{
	unsigned int data;
	unsigned int tmpadd;
	CS=0;
	tmpadd=add<<8;
	SpiaRegs.SPITXBUF=0x0300;
	SpiaRegs.SPITXBUF=tmpadd;
	SpiaRegs.SPITXBUF=0;
	while(SpiaRegs.SPIFFRX.bit.RXFFST<3){}
	SpiaRegs.SPIRXBUF=SpiaRegs.SPIRXBUF;
	SpiaRegs.SPIRXBUF=SpiaRegs.SPIRXBUF;
	data=SpiaRegs.SPIRXBUF;
	CS=1;
	DELAY_US(1);
	return data;	 
}
unsigned int ReadInt(unsigned int add) //从起始地址为add的空间读取无符号整形
{
	unsigned int dataint;
	dataint=ReadChar(add+1);
	dataint=dataint<<8;
	dataint=ReadChar(add)+dataint;
	return dataint;
}
long int ReadInt32(unsigned int add)    //从起始地址为add的空间读取有符号长整形
{
	long int dataint32,temp32;
	unsigned int dataint16;
	dataint16=ReadInt(add+2);
	dataint32=dataint16;
	dataint32=dataint32<<16;
	temp32=ReadInt(add);
	dataint32=(dataint32 & 0xFFFF0000) | (temp32 & 0x0000FFFF);
	return dataint32;
}


