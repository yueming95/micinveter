// TI File $Revision: /main/1 $
// Checkin $Date: December 5, 2008   18:00:28 $
//###########################################################################
//
// FILE:	DSP2803x_I2cExample.h
//
// TITLE:	2803x I2C Example Code Definitions.
//
//###########################################################################
// $TI Release: 2803x C/C++ Header Files V1.21 $
// $Release Date: December 1, 2009 $
//###########################################################################

#ifndef DSP2803x_I2C_DEFINES_H
#define DSP2803x_I2C_DEFINES_H
//--------------------------------------------
// Defines
//--------------------------------------------

// Error Messages
#define I2C_ERROR               0xFFFF
#define I2C_ARB_LOST_ERROR      0x0001
#define I2C_NACK_ERROR          0x0002
#define I2C_BUS_BUSY_ERROR      0x1000
#define I2C_STP_NOT_READY_ERROR 0x5555
#define I2C_NO_FLAGS            0xAAAA
#define I2C_SUCCESS             0x0000

// Clear Status Flags
#define I2C_CLR_AL_BIT          0x0001
#define I2C_CLR_NACK_BIT        0x0002
#define I2C_CLR_ARDY_BIT        0x0004
#define I2C_CLR_RRDY_BIT        0x0008
#define I2C_CLR_SCD_BIT         0x0020

// Interrupt Source Messages
#define I2C_NO_ISRC             0x0000
#define I2C_ARB_ISRC            0x0001
#define I2C_NACK_ISRC           0x0002
#define I2C_ARDY_ISRC           0x0003
#define I2C_RX_ISRC             0x0004
#define I2C_TX_ISRC             0x0005
#define I2C_SCD_ISRC            0x0006
#define I2C_AAS_ISRC            0x0007

// I2CMSG structure defines
#define I2C_NO_STOP  0
#define I2C_YES_STOP 1
#define I2C_RECEIVE  0
#define I2C_TRANSMIT 1
#define I2C_MAX_BUFFER_SIZE 4
#define	I2C_PAGE_SIZE	64

// I2C Slave State defines
#define I2C_NOTSLAVE      0
#define I2C_ADDR_AS_SLAVE 1
#define I2C_ST_MSG_READY  2

// I2C Slave Receiver messages defines
#define I2C_SND_MSG1 1
#define I2C_SND_MSG2 2

// I2C State defines
#define I2C_IDLE               0
#define I2C_SLAVE_RECEIVER     1
#define I2C_SLAVE_TRANSMITTER  2
#define I2C_MASTER_RECEIVER    3
#define I2C_MASTER_TRANSMITTER 4

// I2C  Message Commands for I2CMSG struct
#define I2C_MSGSTAT_INACTIVE          0x0000
#define I2C_MSGSTAT_SEND_WITHSTOP     0x0010
#define I2C_MSGSTAT_WRITE_BUSY        0x0011
#define I2C_MSGSTAT_SEND_NOSTOP       0x0020
#define I2C_MSGSTAT_SEND_NOSTOP_BUSY  0x0021
#define I2C_MSGSTAT_RESTART           0x0022
#define I2C_MSGSTAT_READ_BUSY         0x0023

// Generic defines
#define I2C_TRUE  1
#define I2C_FALSE 0
#define I2C_YES   1
#define I2C_NO    0
#define I2C_DUMMY_BYTE 0

//note: Although the address of M24128 is 0xA0, but in 7-bit address mode,
//the address is 0x50. see I2CSAR Field Descriptions.
#define I2C_SLAVE_ADDR        0x50
#define I2C_NUMBYTES          4
#define I2C_EEPROM_HIGH_ADDR  0x00
#define I2C_EEPROM_LOW_ADDR   0x00

#define	M24128_ADDR_LIMIT			0x4000//共16k byte，每页 有64个byte，共有256页
#define	M24128_ADDR_PAGE_MASK		0xFFC0//? 换成二进制，后面有六个零。而转换为十进制为63.
#define	M24128_ADDR_PAGE_LEN		0x40//每页 有64个byte

#define I2C_W_R_OK						0
#define I2C_W_R_EXCEED_LIMIT			1
#define	I2C_W_R_DATA_AMOUNT_IS_ZERO		2
#define	I2C_R_FAILED					3
#define I2C_W_FAILED					4

#define	I2C_MAX_TIMES_PAGE_WRITE		3
#define	I2C_MAX_TIMES_DYMMY_WRITE		3
#define	I2C_MAX_TIMES_RESET				3
#define	I2C_MAX_MS_SEMPEND	           100/*10*/
/*********SAFETY_TABLE****************************************/   
#define COUNTRYNUM                   16//国家数  
    
/*******EEPROM Address for Safety Allocation**************************/
#define	SAFETY_MAX_SIZE				0x0040
#define	SAFETY_MAX_NUMBER			(0x800/SAFETY_MAX_SIZE)	//最大国家数
#define	SAFETY_PARA_NUM			 	 24
#define	SAFETY_PARA_NUM_V			 13
#define	SAFETY_PARA_NUM_Fre			 14
#define	SAFETY_PARA_NUM_DCI			 7
//#define SCIRECEIVE_PARA_NUM        (SAFETY_PARA_NUM * 2+6)
/*******EEPROM Address for Safety Allocation**************************/

#define	ENERGY_OUTPUT_ADDR			0x9C0      //发电量
#define	ENERGY_DATE_ADDR			0x9C2        //日期 
/*******EEPROM Address for Production Today Allocation****************/
#define	BOOTSTATE_ADDR              0x2
#define	SAFETY_COUNTRY_ADDR			0x4          //出厂状态旗标地址

/*******EEPROM Address for Safety Country*****************************/

//#define	TOTAL_MODE_SELECT_ADDR   	0x100 				
#define	MPPTMODE_ADDR           	0x100 		  //mppt模式选择
#define	NOMPPT_I_LIMIT_ADDR         0x102 		  //mppt电流限制值地址
#define	INPUTMODE_ADDR         		0x104 		  //mppt电流限制值地址
#define	FactoryMode_ADDR   	0x100 	
#define	Calibration_ADDR   	0x180 


#define	SAFETY_STARTValue_ADDR	    0x280    //11页，12页，280,290,2A0,2B0...
#define	SAFETY_GridProtectVValue_ADDR	    0x300  //13页，14页
#define	SAFETY_GridProtectFreqValue_ADDR	    0x380  //15页 16页
#define	SAFETY_GridProtectDCIValue_ADDR	    0x400	//17页18页
#define	SAFETY_RemotPowerControlValue_ADDR	    0x480	//19页20页
#define	SAFETY_FreqPowerControlValue_ADDR	    0x500	//19页20页
#define	SAFETY_ReactiveValue_ADDR	    0x580	//21页 22页
#define	SAFETY_LVRTValue_ADDR	    0x600	//23页 24页
#define	SAFETY_OffsetValue_ADDR	    0x680 //// 25页 26页
#define	SAFETY_IsoIslandandSoOn_ADDR  0x700  //// 27页 28页   end

//--------------------------------------------
// Structures
//--------------------------------------------

// I2C Message Structure
struct I2CMSG {
  Uint16 MsgStatus;				// Word stating what state msg is in:
  								//   I2C_MSGCMD_INACTIVE = do not send msg
  								//   I2C_MSGCMD_BUSY = msg start has been sent,
  								//                     awaiting stop
  								//   I2C_MSGCMD_SEND_WITHSTOP = command to send
  								//       master trans msg complete with a stop bit
  								//   I2C_MSGCMD_SEND_NOSTOP = command to send
  								//       master trans msg without the stop bit
  								//   I2C_MSGCMD_RESTART = command to send a restart
  								//       as a master receiver with a stop bit
  Uint16 SlaveAddress;			// I2C address of slave msg is intended for
  Uint16 NumOfBytes;			// Num of valid bytes in (or to be put in MsgBuffer)
  Uint16 MemoryHighAddr;		// EEPROM address of data associated with msg (high byte)
  Uint16 MemoryLowAddr;			// EEPROM address of data associated with msg (low byte)
  Uint16 *p_MsgBuffer;			// point to data buffer
  Uint16 MsgBuffer[I2C_MAX_BUFFER_SIZE];
};
typedef struct I2CMSG TypeI2cMsg;

struct DEF_I2CADDR
{
	Uint16 u16BlockAddr;
	Uint16 u16ByteAddr;
};
typedef struct DEF_I2CADDR TypeI2cAddr;

void   InitI2C(void);
Uint16 I2C_Reset(void);
void   I2C_ConvAddr(Uint16, TypeI2cAddr * );
Uint16 I2C_PollACK(TypeI2cMsg * );
Uint16 I2C_Write_InByte(Uint16, Uint16* , Uint16);
Uint16 I2C_Write_InWord(Uint16, Uint16* , Uint16);
Uint16 I2C_Write_Page(TypeI2cMsg * );
Uint16 I2C_WriteData(TypeI2cMsg * );
Uint16 I2C_Read_InByte(Uint16, Uint16* , Uint16);
Uint16 I2C_Read_InWord(Uint16, Uint16* , Uint16);
Uint16 I2C_Read_MultiByte(TypeI2cMsg * );
Uint16 I2C_ReadData(TypeI2cMsg * );

extern Uint16 I2Cerror;

extern const Uint16 SafetyDefaultVTable[COUNTRYNUM][10];
extern const Uint16 SafetyDefaultFreTable[COUNTRYNUM][9];
extern const Uint16 SafetyDefaultDCITable[COUNTRYNUM][6];
extern const Uint16 SafetyDefaultRemotPowerControlTable[COUNTRYNUM][3];
extern const Uint16 SafetyDefaultFreqPowerControlTable[COUNTRYNUM][7];
extern const Uint16 SafetyDefaultReactiveTable[COUNTRYNUM][21];
extern const Uint16 SafetyDefaultLVRTTable[COUNTRYNUM][13];	
extern const Uint16 SafetyDefaultSTARTTable[COUNTRYNUM][8];
extern const Uint16 SafetyDefaultOffsetTable[COUNTRYNUM][29];
extern const Uint16 SafetyDefaultAutoTestTable[COUNTRYNUM][39];
extern const Uint16 SafetyDefaultIsoIslandandSoOnTable[COUNTRYNUM][4];
extern const Uint16 CalibrationDefaultTable[32];
extern const Uint16 FactoryModeDefaultValue;

extern Uint16 SafetyVTable[10];
extern Uint16 SafetyFreTable[9];
extern Uint16 SafetyDCITable[6];
extern Uint16 SafetyRemotPowerControlTable[3];
extern Uint16 SafetyFreqPowerControlTable[7];
extern Uint16 SafetyReactiveTable[21];
extern Uint16 SafetyLVRTTable[13];	
extern Uint16 SafetySTARTTable[8];
extern Uint16 SafetyIsoIslandandSoOnTable[4];
extern Uint16 CalibrationTable[32];
extern Uint16 FactoryMode;

Uint16 writedefaultsafety(Uint16 Country);
Uint16 WriteChangedsafety(void);
Uint16 recoverdefaultsafety(Uint16 country);
Uint16 readsafetyvalue(void);
void  SetToDefaultsafetyvalue(Uint16 country);
void  bootreadfromEE(void);
void  ClearEEPROM(void);
#endif  // end of DSP2803x_I2C_DEFINES_H definition

//===========================================================================
// End of file.
//===========================================================================
