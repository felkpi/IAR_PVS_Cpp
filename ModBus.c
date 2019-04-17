/*******************************************************************************
 * file:	ModBus.c
 * author:	A.Vlashchuk
 * version:	V1.0.0
 * date:	26-April-2015
 * brief:	Main program body
 *******************************************************************************/
#include "ModBus.h"
#include "GlobalPVS.h"
#include "initPVS.h"
#include "hardwarePVS.h"
#include "Safety.h"

union							// ����� ������ ModBus
{
	uint8_t ucInBufRS485[BUF_RS485_SIZE];
	uint16_t wInBufRS485[BUF_RS485_SIZE/2];
};

union							// ����� �������� ModBus
{
	uint8_t ucOutBufRS485[BUF_RS485_SIZE];
	uint16_t wOutBufRS485[BUF_RS485_SIZE/2];
};

uint16_t volatile counter_T1_5, counter_T2_5, counter_T3_5, counter_T4_5;	// ������� ���������� T1.5, T2.5, T3.5, T4.5 (T - ����� �������� 1 ����� �� UART)

//------------------------------------------------------------------------------
// InitConfig, ��������� ���� FC = 16 (Write Multiple Registers) � ������ InitConfig
//------------------------------------------------------------------------------
// �������� ������ � ������ InitConfig (BroadCasting � UniCasting)
//
// �������� ���������� ������� ������� �� ����
// ���������� ��� ������
//------------------------------------------------------------------------------
uint8_t InitConfig(uint16_t* pSendLength, uint16_t wNBytesInBuf)
{
// pSendLength - ��������� �� ����� ��������� ������
// wNBytesInBuf - ����� ���� � ������ ucInBufRS485
	uint8_t *pAddr8, NBytes;
	uint16_t StAddr, NRegs, wCRC;
	uint32_t index;

	if (ucInBufRS485[1] != 16)
		return 0x01;						// 0x01 - Invalid Function Code
	StAddr = WORDBE(ucInBufRS485[2]);
	NRegs =  WORDBE(ucInBufRS485[4]);
	NBytes = ucInBufRS485[6];
	if ((StAddr != 0)||(NRegs != 64))
		return 0x02;						// 0x02 - Invalid Data Address
	if ((ucInBufRS485[0] == 0)||(ucInBufRS485[0] > 247))
		return 0x03;						// 0x03 - Invalid Data Value
	if (NBytes != (2 * NRegs))
		return 0x03;						// 0x03 - Invalid Data Value

	pAddr8 = (uint8_t*)&sPVSConfig;
	index = 7;
	while (NBytes)
	{
		*pAddr8 = ucInBufRS485[index+1];
		pAddr8++;
		*pAddr8 = ucInBufRS485[index];
		pAddr8++;
		index += 2;
		NBytes--;
	}
	sPVSConfig.CRC16 = wCRC16t((uint8_t*)&sPVSConfig, (PVS_CONFIG_BYTES - 2));

	ucOutBufRS485[0] = ucInBufRS485[0];
	ucOutBufRS485[1] = ucInBufRS485[1];
	ucOutBufRS485[2] = ucInBufRS485[2];		// ��������� �����
	ucOutBufRS485[3] = ucInBufRS485[3];
	ucOutBufRS485[4] = ucInBufRS485[4];		// ���������� ���������
	ucOutBufRS485[5] = ucInBufRS485[5];
	wCRC = wCRC16t(ucOutBufRS485, 6);
	ucOutBufRS485[6] = LOWBYTE(wCRC);
	ucOutBufRS485[7] = HIGHBYTE(wCRC);
	*pSendLength = 8;
	return sPVSConfigWtoF();				// �������� ��������� sPVSConfig �� Flash, ������� ���� ������ (0 ��� 4)
}

//------------------------------------------------------------------------------
// ��������� ���� FC = 6 (Write Single Register)
//------------------------------------------------------------------------------
// ������� ������ �� ����, �������� � ������ Work (��� BroadCast ��� � UniCast)
// ����������� �������������� ������ ��������� ���������� (StAddr = 0)
//------------------------------------------------------------------------------
uint8_t WriteSingleRegFC6(uint16_t* pSendLength, uint16_t wNBytesInBuf)
{
// pSendLength - ��������� �� ����� ��������� ������
// wNBytesInBuf - ����� ���� � ������ ucInBufRS485
	uint16_t wStAddr, wRegVal, wCRC, *pAddr16;

	wStAddr = WORDBE(ucInBufRS485[2]);		// ����� ��������
	wRegVal = WORDBE(ucInBufRS485[4]);		// �������� �������� (MbAddr � MbBRP)
	if (wStAddr)							// StAddr != 0
		return 0x02;						// 0x02 - Invalid Address Value(������������ �����)
	if ((LOWBYTE(wRegVal) == 0)||(LOWBYTE(wRegVal) > 247))
		return 0x03;						// 0x03 - Invalid Data Value (������������ �����)
	if (wNBytesInBuf != 8)
		return 0x03;						// ������������ ������ �������
	pAddr16 = (uint16_t*)&sPVSConfig;
	*pAddr16 = wRegVal;						// ���������� sPVSConfig
	wCRC = wCRC16t((uint8_t*)&sPVSConfig, (PVS_CONFIG_BYTES-2));
	sPVSConfig.CRC16 = wCRC;

	ucOutBufRS485[0] = ucInBufRS485[0];
	ucOutBufRS485[1] = ucInBufRS485[1];
	ucOutBufRS485[2] = ucInBufRS485[2];		// ����� ��������
	ucOutBufRS485[3] = ucInBufRS485[3];
	ucOutBufRS485[4] = ucInBufRS485[4];		// �������� ��������
	ucOutBufRS485[5] = ucInBufRS485[5];
	wCRC = wCRC16t(ucOutBufRS485, 6);
	ucOutBufRS485[6] = LOWBYTE(wCRC);
	ucOutBufRS485[7] = HIGHBYTE(wCRC);
	*pSendLength = 8;
	return sPVSConfigWtoF();				// �������� ��������� sPVSConfig �� Flash, ������� ���� ������ (0 ��� 4)
}

//------------------------------------------------------------------------------
// ��������� ���� FC = 16 (Write Multiple Registers)
//------------------------------------------------------------------------------
// ������� ������ �� ����, �������� � ������ Work (BroadCast � UniCast)
// ���.0 - ��� ������
// ���. 1...59 - � ������� (PassWord)
// ���. 60,61 (Password) - � ������-������� (MasterPass)
//------------------------------------------------------------------------------
uint8_t WriteMultipleRegFC16(uint16_t* pSendLength, uint16_t wNBytesInBuf)
{
// pSendLength - ��������� �� ����� ��������� ������
// wNBytesInBuf - ����� ���� � ������ ucInBufRS485
	uint8_t *pAddr8, ucNBytes;
	uint16_t wStAddr, wNRegs, wCRC;
	uint32_t index;

	wStAddr = WORDBE(ucInBufRS485[2]);
	wNRegs =  WORDBE(ucInBufRS485[4]);
	ucNBytes = ucInBufRS485[6];

	if ((wStAddr + wNRegs) >= PVS_CONFIG_HWORDS)
		return 0x02;								// 0x02 - Invalid Address Value (������������ �����)
	if (ucNBytes != (2 * wNRegs))
		return 0x03;								// 0x03 - Invalid Data Value (������������ ������)
	if ((ucNBytes + 9) != wNBytesInBuf)
		return 0x03;								// 0x03 - Invalid Data Value (������������ ������)

	if ((wStAddr == 0)&&(wNRegs == 1))
		;											// ������ MbAddr & MbBRP, ������ �� �����
	else if ((wStAddr + wNRegs) == 62)				// ������-������ �� �����, ������ ������
	{
		if ((sPVSConfig.Password[0] != ucInBufRS485[4+ucNBytes])||(sPVSConfig.Password[1] != ucInBufRS485[3+ucNBytes])||
			(sPVSConfig.Password[2] != ucInBufRS485[6+ucNBytes])||(sPVSConfig.Password[3] != ucInBufRS485[5+ucNBytes]))
			return 0x07;							// ������������ ������

	}
	else if ((wStAddr + wNRegs) == 64)				// ������ ���������������, ������������ ������-������
	{
		if ((sPVSConfig.MasterPass[0] != ucInBufRS485[4+ucNBytes])||(sPVSConfig.MasterPass[1] != ucInBufRS485[3+ucNBytes])||
			(sPVSConfig.MasterPass[2] != ucInBufRS485[6+ucNBytes])||(sPVSConfig.MasterPass[3] != ucInBufRS485[5+ucNBytes]))
			return 0x08;							// ������������ ������-������
	}
	else
		return 0x03;								// 0x03 - Invalid Data Value (������������ ������)

	pAddr8 = (uint8_t*)&sPVSConfig + wStAddr*2;
	index = 7;
	while (wNRegs)
	{
		*pAddr8 = ucInBufRS485[index+1];
		pAddr8++;
		*pAddr8 = ucInBufRS485[index];
		pAddr8++;
		index += 2;
		wNRegs--;
	}
	wCRC = wCRC16t((uint8_t*)&sPVSConfig, (PVS_CONFIG_BYTES-2));
	sPVSConfig.CRC16 = wCRC;

	ucOutBufRS485[0] = ucInBufRS485[0];
	ucOutBufRS485[1] = ucInBufRS485[1];
	ucOutBufRS485[2] = ucInBufRS485[2];				// ����� ��������
	ucOutBufRS485[3] = ucInBufRS485[3];
	ucOutBufRS485[4] = ucInBufRS485[4];				// ���������� ���������
	ucOutBufRS485[5] = ucInBufRS485[5];
	wCRC = wCRC16t(ucOutBufRS485, 6);
	ucOutBufRS485[6] = LOWBYTE(wCRC);
	ucOutBufRS485[7] = HIGHBYTE(wCRC);
	*pSendLength = 8;
	return sPVSConfigWtoF();						// �������� ��������� sPVSConfig �� Flash
}

//------------------------------------------------------------------------------
// ��������� ���� FC = 3 (Read Holding Registers)
// ������ ������������
//------------------------------------------------------------------------------
uint8_t ReadHoldingRegFC3(uint16_t *pSendLength)
{
	// pSendLength - ��������� �� ����� ��������� ������
	// wNBytesInBuf - ����� ���� � ������ ucInBufRS485
	uint8_t	*pAddr8, ucNBytes, index;
	uint16_t wCRC, wStAddr, wNRegs;

	wStAddr = WORDBE(ucInBufRS485[2]);
	wNRegs =  WORDBE(ucInBufRS485[4]);

	if ((wStAddr + wNRegs) > 60)
		return 0x02;									// 0x02 - Invalid Address Value (������ ������ ������)

	pAddr8 = (uint8_t*)&sPVSConfig + wStAddr*2;
	ucNBytes = wNRegs*2;
	ucOutBufRS485[0] = ucInBufRS485[0];
	ucOutBufRS485[1] = ucInBufRS485[1];
	ucOutBufRS485[2] = ucNBytes;						// ����� ���� ������
	index = 3;
	while (wNRegs)
	{
		ucOutBufRS485[index+1] = *pAddr8;
		pAddr8++;
		ucOutBufRS485[index] = *pAddr8;
		pAddr8++;
		index += 2;
		wNRegs--;
	}
	wCRC = wCRC16t(ucOutBufRS485, index);
	ucOutBufRS485[index++] = LOWBYTE(wCRC);
	ucOutBufRS485[index++] = HIGHBYTE(wCRC);
	*pSendLength = index;
	return 0;											// ���������� ������
}

//------------------------------------------------------------------------------
// ��������� ���� FC = 4 (Read Input Registers)
//------------------------------------------------------------------------------
// ������ ��������� sPVSState: Adr = 0...127
//------------------------------------------------------------------------------
// 0		union		BitState;			//	��������� ����� ��������� �������������� LED � �.�.
// 1		union		ErrPVS;				//	���� ������
// 2		int16_t		shGap;				//	������� ���������� �������� ������ ��� �������� ���������� (� ���)
// 3		int16_t		shDisplacement;		//	������ ����� (� ���)
// 4		int16_t		shVibration;		//	������ ���������������� (� ���)
// 5		uint16_t	usSpin;				//	�������� �������� �������� (� 0,1 ��/���)
// 6		int16_t		shTempModule;		//	���������� ����������� ������ PVS (� ����.�)
// 7		int16_t		shTempSensor;		//	���������� ����������� ������� PVS (� ����.�)
// 8		uint16_t	usDataADC;			//	������ ��� (��� ����������)
// 9		uint16_t	usDataOut;			//	������ ��� ������ �� �������� ��� (��� ������)
// 10		int16_t		shP5VA;				//	���������� ������� +5VA (�������� 0,01�)
// 11		int16_t		shM5VA;				//	���������� ������� -5VA (�������� 0,01�)
// 12...71	int16_t		shBeat[60];			//	���������� �������� ������/������� ������/ ����� 6 ���� (60 �������� �� ������)
// 72,73	uint32_t	CRC32;				//	CRC32 ��������� �� ����, ������������ � �������� ������
// 74...85	float		fKx[6];				//	������ ������������� ������ � ������ ���������� (��� ���������� �����������)
// 			float		fKxTM[7];			//	������������ ��� �-�������� ���������� �� ����������� ������
// 86...99	float		fKxTD[7];			//	������������ ��� �-�������� ���������� �� ����������� �������
//100...113	float		fumCallibr[7];		//	����� ���������� � ��� (� ������ ����������� ��� ���)
//114...127	float		fvNCallibr[7];		//	����� ���������� � �������� ��� ��� ���������� �����������
// 128,129	float		fumCallibr0;		//	"0" ������������� ������ ��� ���, � ���
// 130,131	float		fSpinSI;			//	�������� �������� (1/�� = ��/�)
// 132,133	float		fKPt;				//	����������� �������������� Pt (����������� ��������������) ��� ���������� �����������
// ------------------------------------------
uint8_t ReadInputRegFC4(uint16_t *pSendLength)
{
	// pSendLength - ��������� �� ����� ��������� ������
	// wNBytesInBuf - ����� ���� � ������ ucInBufRS485
	uint8_t	*pAddr8, ucNBytes, index;
	uint16_t wCRC, wStAddr, wNRegs;

	wStAddr = WORDBE(ucInBufRS485[2]);
	wNRegs =  WORDBE(ucInBufRS485[4]);

	if (((wStAddr + wNRegs)*2) > sizeof(sPVSState))
		return 0x02;									// 0x02 - Invalid Address Value

	pAddr8 = (uint8_t*)&sPVSState + wStAddr*2;
	ucNBytes = wNRegs*2;
	ucOutBufRS485[0] = ucInBufRS485[0];
	ucOutBufRS485[1] = ucInBufRS485[1];
	ucOutBufRS485[2] = ucNBytes;						// ����� ���� ������
	index = 3;
	while (wNRegs)
	{
		ucOutBufRS485[index+1] = *pAddr8;
		pAddr8++;
		ucOutBufRS485[index] = *pAddr8;
		pAddr8++;
		index += 2;
		wNRegs--;
	}
	wCRC = wCRC16t(ucOutBufRS485, index);
	ucOutBufRS485[index++] = LOWBYTE(wCRC);
	ucOutBufRS485[index++] = HIGHBYTE(wCRC);
	*pSendLength = index;
	return 0;											// ���������� ������
}

//------------------------------------------------------------------------------
// ����� �� ModBus
//------------------------------------------------------------------------------
void ProcModBus(void)
{
	static uint16_t SendLength, usNT1_5;
	static uint16_t indexRS485;			// ��������� � ������� ���� ��� ������/�������� �� RS485

	uint16_t wCRC, wDR;
	uint8_t ucErrorCode, ucDR;

	if (!unModBus.bReg)
	//----------------------- ����� IDLE ---------------------------------------
	{
		LedHL1_Off();											// �������� LED RS485
		if (USART2->SR & USART_SR_RXNE)
		{
			if ((USART2->SR & USART_SR_PE)&&(!(sPVSConfig.MbBRP & 0x20)))	// �������� �� ���������� ������ ��������
				unModBus.bPErr = 1;
			else
				unModBus.bPErr = 0;
			ucDR = USART2->DR;
			if ((ucDR != 0xFF) && (!unModBus.bPErr))
			{
				ucInBufRS485[0] = ucDR;
				indexRS485 = 1;
				counter_T1_5 = T1_5;
				counter_T3_5 = T3_5;
				usNT1_5 = 3;									// ������� ����� ������������ counter_T1_5
				unModBus.usModBus = 1;							// -> ����� �����
			}
		}
	}
	else if (unModBus.bReg == 1)
	//----------------------- ����� ����� --------------------------------------
	{
		if (!counter_T1_5)
		{
			if (usNT1_5)
			{
				counter_T1_5 = T1_5;
				if (!(--usNT1_5))
					unModBus.bT1_5Err = 1;
			}
		}
		if (!counter_T3_5)
			unModBus.bReg = 2;									// -> ����� �������
		else if (USART2->SR & USART_SR_RXNE)
		{
			counter_T1_5 = T1_5;
			counter_T3_5 = T3_5;
			if ((USART2->SR & USART_SR_PE)&&(!(sPVSConfig.MbBRP & 0x20)))	// �������� �� ���������� ������ ��������
				unModBus.bPErr = 1;
			if (indexRS485 >= BUF_RS485_SIZE)
				unModBus.usModBus = 0;							// ��� ������������ ������ -> ����� IDLE
			else
				ucInBufRS485[indexRS485] = USART2->DR;
			indexRS485++;
		}
	}
	else if (unModBus.bReg == 2)
	//----------------------- ����� ������� ------------------------------------
	{
		if (unModBus.bPErr || unModBus.bT1_5Err)
			unModBus.usModBus = 0;								// ������ ������� (�������� � t1,5)
		else if (wCRC16t(ucInBufRS485, indexRS485))
			unModBus.usModBus = 0;								// ������ CRC
		else if (!sPVSState.bWork)
		// ------------------ ����� InitConfig ---------------------------------
		{
			if (indexRS485 != (PVS_CONFIG_BYTES+ 7))
				unModBus.usModBus = 0;							// ������������ ����� �������, ���������� ������
			else
			{
				ucErrorCode = InitConfig(&SendLength, indexRS485);	// ����� InitConfig
				if (!ucInBufRS485[0])
					unModBus.usModBus = 0;						// BroadCasting, ���������� ������
			}
		}
		// ------------------ ����� WORK ---------------------------------------
		else if (!ucInBufRS485[0])								// BroadCasting
		{
			if (ucInBufRS485[1] == 6)							// FC = 6 (Write Single Register)
				ucErrorCode = WriteSingleRegFC6(&SendLength, indexRS485);
			else if (ucInBufRS485[1] == 16)						// FC = 16 (Write Multiple Registers)
				ucErrorCode = WriteMultipleRegFC16(&SendLength, indexRS485);
			unModBus.usModBus = 0;								// FC �� ��������������, ���������� ������
		}
		else if (ucInBufRS485[0] != sPVSConfig.MbAddr)
			unModBus.usModBus = 0;								// �� ��������� ����� �� ���� RS485
		else													// Unicasting
		{
			if ((ucInBufRS485[1] == 3) && (indexRS485 == 8))		// FC = 3 (Read Holding Registers)
				ucErrorCode = ReadHoldingRegFC3(&SendLength);
			else if ((ucInBufRS485[1] == 4) && (indexRS485 == 8))	// FC = 4 (Read Input Registers)
				ucErrorCode = ReadInputRegFC4(&SendLength);
			else if (ucInBufRS485[1] == 6)						// FC = 6 (Write Single Register)
				ucErrorCode = WriteSingleRegFC6(&SendLength, indexRS485);
			else if (ucInBufRS485[1] == 16)						// FC = 16 (Write Multiple Registers)
				ucErrorCode = WriteMultipleRegFC16(&SendLength, indexRS485);
			else												// FC �� ��������������
				ucErrorCode = 1;
		}
		// ------------------ ���������� � �������� ----------------------------
		if (unModBus.bReg)										// �������, UniCast
		{
			if (ucErrorCode)									// ��������� ������
			{
				ucOutBufRS485[0] = ucInBufRS485[0];				// MbAddr
				ucOutBufRS485[1] = ucInBufRS485[1]|0x80;		// FC
				ucOutBufRS485[2] = ucErrorCode;
				wCRC = wCRC16t(ucOutBufRS485, 3);
				ucOutBufRS485[3] = LOWBYTE(wCRC);
				ucOutBufRS485[4] = HIGHBYTE(wCRC);
				SendLength = 5;
			}
			indexRS485 = 0;
			USART2->CR1 &= ~USART_CR1_RE;
			USART2->CR1 |= USART_CR1_TE;
			GPIOD->BSRRL = 0x0080;								// PD7 = 1 (��������)
			unModBus.bReg = 3;									// -> ����� ��������
		}
	}
	else
	//----------------------- ����� �������� -----------------------------------
	{
		LedHL1_On();											// ��������� LED RS485
		if (USART2->SR & USART_SR_TXE)
		{
			if (indexRS485 < SendLength)
			{
				wDR = (uint16_t)ucOutBufRS485[indexRS485]|0x100;
				USART2->DR = wDR;
				indexRS485++;
			}
		}
		if (USART2->SR & USART_SR_TC)
		{
			GPIOD->BSRRH = 0x0080;								// PD7 = 0 (�����)
			USART2->CR1 &= ~USART_CR1_TE;
			USART2->CR1 |= USART_CR1_RE;
			ucDR = USART2->DR;									// ����� �������
			unModBus.usModBus = 0;								// -> ����� IDLE
		}
	}
}
