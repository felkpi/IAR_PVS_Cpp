/*******************************************************************************
 * file:	GlobalPVS.c (��������������� ������������)
 * author:	A.Vlashchuk
 * version:	V1.0.0
 * date:	20-September-2015
 * brief:	���������� ����������
 *******************************************************************************/

#include "GlobalPVS.h"

//------------------------------------------------------------------------------
//	���������� ����������
//------------------------------------------------------------------------------
volatile uint16_t usBufDMA_SPI3[2];							// ����� DMA ��������� ���� (AD5662)

volatile uint16_t usBuf_AD7689[BUF_AD7689_SIZE];			// ��������� ����� ��� AD7689
volatile uint16_t usBuf_AD5662[BUF_AD5662_SIZE];			// ��������� ����� ��� AD5662

volatile uint8_t ucInBuf_RS485[RX_BUF_RS485_SIZE];			// ��������� ����� ������ � RS485 (������� �����)
volatile uint8_t ucOutBuf_RS485[TX_BUF_RS485_SIZE];			// ��������� ����� ������ � RS485 (�������� �����)

volatile uint32_t wr_index_AD7689, rd_index_AD7689;			// ������� ������ � ������ ���������� ������ ��� AD7689
volatile uint32_t wr_index_AD5662, rd_index_AD5662;			// ������� ������ � ������ ���������� ������ ��� AD5662

volatile uint32_t wr_index_RxRS485, rd_index_RxRS485;		// ������� ������ � ������ �������� ���������� ������ RS485
volatile uint32_t wr_index_TxRS485, rd_index_TxRS485;		// ������� ������ � ������ ��������� ���������� ������ RS485

volatile union	defFlags	unFlags;						// �����
volatile struct	defBBFlags	*pBBFlags;						// ������� alias bit-band SRAM

const float cfKU[3] = {FKU_2mm, FKU_4mm, FKU_5mm};			// ������ ������ ���. ���������� �� ��������� ��������� ������� (sPVSConfig.bRangeD)
const float cfKI[3] = {FKI_2mm, FKI_4mm, FKI_5mm};			// ������ ������ ���. ���� �� ��������� ��������� ������� (sPVSConfig.bRangeD)
const float cfKIV[3] = {FKI_125um, FKI_250um, FKI_500um};	// ������ ������ ���. ���� �� ��������� ��������� ������� ����������������(sPVSConfig.bRangeV)
const uint16_t cusUOUT_0[3] = {UOUT_0280, UOUT_0200, UOUT_0160};	// ��������� �������� UOUT

#pragma pack(1)
#pragma location = FLASH_SECTOR_3
const struct PVSConfig cPVSConfig =
{
	1,4,														// (0)		mbAddr & MbBRP (MbAddr=1; 19,6����; EVEN)
	1,0,0,0,2,0,0,0,											// (1)		MainCfg
	0,0,0,5,													// (2)		CallibrCfg (7�-���������� ��� ����. ����., 5� ������)
	'P','V','S',0,												// (3,4)	TypePVS[4]
	1,															// (5)		NumPVS
	0x0100,														// (6)		VersPVS (01.0.0)
	0x0209,0x2015,												// (7,8)	ProductDate[2]
	0x0403,0x2015,												// (9,10)	CallibrDate[2]
	'�','�','�','-','�','-','1',',','0','-','1','0','-','1','7','6','-','5',0,0,	// (11...20)LabelPVS[20] "���-�-1,0-10-176-5"
	'3','8','�','�','3','�','�','�',0,0,						// (21...25)Material[10] "38��3���"
	200,350,850,1350,1850,2350,2500,							// (26...32)umCallibr[7]
	1350,														// (33)		umCallibr0
	1073*16,1255*16,1875*16,2538*16,3198*16,3809*16,3978*16,	// (34...40)vNCallibr[7]
	20000,40000,												// (41,42)	vTCallibr[2]
	21000,42000,												// (43,44)	vTMCallibr[2]
	21000,42000,												// (45,46)	vTDCallibr[2]
	25,															// (47)		TNMCallibr
	80,															// (48)		TMCallibr
	25,															// (49)		TNDCallibr
	100,														// (50)		TDCallibr
	85,															// (51)		TMFault
	120,														// (52)		TDFault
	10, 														// (53)		tClearFault
	500,														// (54)		umGapL (�����1 ������� ���������)
	1000,														// (55)		umGapH (�����2 ������� ���������)
	165,														// (56)		umPR1 (������������1 ��� ���)
	260,														// (57)		umPR2 (������������2 ��� ���)
	320,														// (58)		umFailure (������ ��� ���)
	10, 														// (59)		tDelaySignal
	'�','�','�','�',											// (60,61)	Password[4] "����"
	'N','A','E','K',											// (62,63)	MasterPass[4] "NAEK"
	0x71FF														// (64)		CRC16
};

//const uint8_t CfgFLASH[16384-sizeof(cPVSConfig)] = {0,0};

struct PVSConfig sPVSConfig;	// ��������� PVSconfig � SRAM

struct PVSControl sPVSControl;	// ��������� sPVSControl � SRAM

struct PVSState sPVSState;		// ��������� PVSState � SRAM

union defModBus unModBus;		// ���� ��������� ModBus

const uint32_t *pcCRC32 = (uint32_t*)(FLASH_SECTOR_2-4);		// ��������� �� CRC32 ��������� �� �����
