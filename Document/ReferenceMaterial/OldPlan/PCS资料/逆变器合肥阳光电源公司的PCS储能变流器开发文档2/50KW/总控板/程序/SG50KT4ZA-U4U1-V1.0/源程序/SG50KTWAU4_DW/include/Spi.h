#ifndef _SPI_H
#define _SPI_H
#define CS GpioDataRegs.GPADAT.bit.GPIO19
void SpiInit();
unsigned int ReadChar(unsigned int add);
void WriteInt32(unsigned int aadd,long int abc);
void WriteInt(unsigned int add,unsigned int ab);
void WriteChar(unsigned int add,unsigned int a);
unsigned int ReadInt(unsigned int add);
long int ReadInt32(unsigned int add);
#endif

