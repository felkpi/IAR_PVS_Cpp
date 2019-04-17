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

union							// буфер приема ModBus
{
	uint8_t ucInBufRS485[BUF_RS485_SIZE];
	uint16_t wInBufRS485[BUF_RS485_SIZE/2];
};

union							// буфер передачи ModBus
{
	uint8_t ucOutBufRS485[BUF_RS485_SIZE];
	uint16_t wOutBufRS485[BUF_RS485_SIZE/2];
};

uint16_t volatile counter_T1_5, counter_T2_5, counter_T3_5, counter_T4_5;	// счетчик интервалов T1.5, T2.5, T3.5, T4.5 (T - время передачи 1 байта по UART)

//------------------------------------------------------------------------------
// InitConfig, отработка кода FC = 16 (Write Multiple Registers) в режиме InitConfig
//------------------------------------------------------------------------------
// Работает только в режиме InitConfig (BroadCasting и UniCasting)
//
// Принятая информация целиком пишется во флеш
// возвращает код ошибки
//------------------------------------------------------------------------------
uint8_t InitConfig(uint16_t* pSendLength, uint16_t wNBytesInBuf)
{
// pSendLength - указатель на длину выходного буфера
// wNBytesInBuf - число байт в буфере ucInBufRS485
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
	ucOutBufRS485[2] = ucInBufRS485[2];		// стартовый адрес
	ucOutBufRS485[3] = ucInBufRS485[3];
	ucOutBufRS485[4] = ucInBufRS485[4];		// количество регистров
	ucOutBufRS485[5] = ucInBufRS485[5];
	wCRC = wCRC16t(ucOutBufRS485, 6);
	ucOutBufRS485[6] = LOWBYTE(wCRC);
	ucOutBufRS485[7] = HIGHBYTE(wCRC);
	*pSendLength = 8;
	return sPVSConfigWtoF();				// записать структуру sPVSConfig во Flash, возврат кода ошибки (0 или 4)
}

//------------------------------------------------------------------------------
// Отработка кода FC = 6 (Write Single Register)
//------------------------------------------------------------------------------
// Пишется только во флеш, работает в режиме Work (как BroadCast так и UniCast)
// Разрешается перезаписывать только параметры интерфейса (StAddr = 0)
//------------------------------------------------------------------------------
uint8_t WriteSingleRegFC6(uint16_t* pSendLength, uint16_t wNBytesInBuf)
{
// pSendLength - указатель на длину выходного буфера
// wNBytesInBuf - число байт в буфере ucInBufRS485
	uint16_t wStAddr, wRegVal, wCRC, *pAddr16;

	wStAddr = WORDBE(ucInBufRS485[2]);		// адрес регистра
	wRegVal = WORDBE(ucInBufRS485[4]);		// значение регистра (MbAddr и MbBRP)
	if (wStAddr)							// StAddr != 0
		return 0x02;						// 0x02 - Invalid Address Value(неправильный адрес)
	if ((LOWBYTE(wRegVal) == 0)||(LOWBYTE(wRegVal) > 247))
		return 0x03;						// 0x03 - Invalid Data Value (неправильный адрес)
	if (wNBytesInBuf != 8)
		return 0x03;						// неправильный размер посылки
	pAddr16 = (uint16_t*)&sPVSConfig;
	*pAddr16 = wRegVal;						// перезапись sPVSConfig
	wCRC = wCRC16t((uint8_t*)&sPVSConfig, (PVS_CONFIG_BYTES-2));
	sPVSConfig.CRC16 = wCRC;

	ucOutBufRS485[0] = ucInBufRS485[0];
	ucOutBufRS485[1] = ucInBufRS485[1];
	ucOutBufRS485[2] = ucInBufRS485[2];		// адрес регистра
	ucOutBufRS485[3] = ucInBufRS485[3];
	ucOutBufRS485[4] = ucInBufRS485[4];		// значение регистра
	ucOutBufRS485[5] = ucInBufRS485[5];
	wCRC = wCRC16t(ucOutBufRS485, 6);
	ucOutBufRS485[6] = LOWBYTE(wCRC);
	ucOutBufRS485[7] = HIGHBYTE(wCRC);
	*pSendLength = 8;
	return sPVSConfigWtoF();				// записать структуру sPVSConfig во Flash, возврат кода ошибки (0 или 4)
}

//------------------------------------------------------------------------------
// Отработка кода FC = 16 (Write Multiple Registers)
//------------------------------------------------------------------------------
// Пишется только во флеш, работает в режиме Work (BroadCast и UniCast)
// Адр.0 - без пароля
// Адр. 1...59 - с паролем (PassWord)
// Адр. 60,61 (Password) - с мастер-паролем (MasterPass)
//------------------------------------------------------------------------------
uint8_t WriteMultipleRegFC16(uint16_t* pSendLength, uint16_t wNBytesInBuf)
{
// pSendLength - указатель на длину выходного буфера
// wNBytesInBuf - число байт в буфере ucInBufRS485
	uint8_t *pAddr8, ucNBytes;
	uint16_t wStAddr, wNRegs, wCRC;
	uint32_t index;

	wStAddr = WORDBE(ucInBufRS485[2]);
	wNRegs =  WORDBE(ucInBufRS485[4]);
	ucNBytes = ucInBufRS485[6];

	if ((wStAddr + wNRegs) >= PVS_CONFIG_HWORDS)
		return 0x02;								// 0x02 - Invalid Address Value (неправильный адрес)
	if (ucNBytes != (2 * wNRegs))
		return 0x03;								// 0x03 - Invalid Data Value (неправильные данные)
	if ((ucNBytes + 9) != wNBytesInBuf)
		return 0x03;								// 0x03 - Invalid Data Value (неправильные данные)

	if ((wStAddr == 0)&&(wNRegs == 1))
		;											// запись MbAddr & MbBRP, пароль не нужен
	else if ((wStAddr + wNRegs) == 62)				// мастер-пароль не нужен, только пароль
	{
		if ((sPVSConfig.Password[0] != ucInBufRS485[4+ucNBytes])||(sPVSConfig.Password[1] != ucInBufRS485[3+ucNBytes])||
			(sPVSConfig.Password[2] != ucInBufRS485[6+ucNBytes])||(sPVSConfig.Password[3] != ucInBufRS485[5+ucNBytes]))
			return 0x07;							// неправильный пароль

	}
	else if ((wStAddr + wNRegs) == 64)				// пароль перезаписывется, используется мастер-пароль
	{
		if ((sPVSConfig.MasterPass[0] != ucInBufRS485[4+ucNBytes])||(sPVSConfig.MasterPass[1] != ucInBufRS485[3+ucNBytes])||
			(sPVSConfig.MasterPass[2] != ucInBufRS485[6+ucNBytes])||(sPVSConfig.MasterPass[3] != ucInBufRS485[5+ucNBytes]))
			return 0x08;							// неправильный мастер-пароль
	}
	else
		return 0x03;								// 0x03 - Invalid Data Value (неправильные данные)

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
	ucOutBufRS485[2] = ucInBufRS485[2];				// адрес регистра
	ucOutBufRS485[3] = ucInBufRS485[3];
	ucOutBufRS485[4] = ucInBufRS485[4];				// количество регистров
	ucOutBufRS485[5] = ucInBufRS485[5];
	wCRC = wCRC16t(ucOutBufRS485, 6);
	ucOutBufRS485[6] = LOWBYTE(wCRC);
	ucOutBufRS485[7] = HIGHBYTE(wCRC);
	*pSendLength = 8;
	return sPVSConfigWtoF();						// записать структуру sPVSConfig во Flash
}

//------------------------------------------------------------------------------
// Отработка кода FC = 3 (Read Holding Registers)
// Чтение конфигурации
//------------------------------------------------------------------------------
uint8_t ReadHoldingRegFC3(uint16_t *pSendLength)
{
	// pSendLength - указатель на длину выходного буфера
	// wNBytesInBuf - число байт в буфере ucInBufRS485
	uint8_t	*pAddr8, ucNBytes, index;
	uint16_t wCRC, wStAddr, wNRegs;

	wStAddr = WORDBE(ucInBufRS485[2]);
	wNRegs =  WORDBE(ucInBufRS485[4]);

	if ((wStAddr + wNRegs) > 60)
		return 0x02;									// 0x02 - Invalid Address Value (пароли нельзя читать)

	pAddr8 = (uint8_t*)&sPVSConfig + wStAddr*2;
	ucNBytes = wNRegs*2;
	ucOutBufRS485[0] = ucInBufRS485[0];
	ucOutBufRS485[1] = ucInBufRS485[1];
	ucOutBufRS485[2] = ucNBytes;						// число байт данных
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
	return 0;											// нормальный воврат
}

//------------------------------------------------------------------------------
// Отработка кода FC = 4 (Read Input Registers)
//------------------------------------------------------------------------------
// Чтение состояния sPVSState: Adr = 0...127
//------------------------------------------------------------------------------
// 0		union		BitState;			//	структура битов положения переключателей LED и т.п.
// 1		union		ErrPVS;				//	биты ошибок
// 2		int16_t		shGap;				//	текущее мгновенное значение зазора без цифровой фильтрации (в мкм)
// 3		int16_t		shDisplacement;		//	осевой сдвиг (в мкм)
// 4		int16_t		shVibration;		//	размах виброперемещений (в мкм)
// 5		uint16_t	usSpin;				//	круговая скорость вращения (в 0,1 об/мин)
// 6		int16_t		shTempModule;		//	измеренная температура модуля PVS (в град.С)
// 7		int16_t		shTempSensor;		//	измеренная температура датчика PVS (в град.С)
// 8		uint16_t	usDataADC;			//	данные АЦП (для калибровки)
// 9		uint16_t	usDataOut;			//	данные для выдачи на выходной ЦАП (без ошибки)
// 10		int16_t		shP5VA;				//	напряжение питания +5VA (дискрета 0,01В)
// 11		int16_t		shM5VA;				//	напряжение питания -5VA (дискрета 0,01В)
// 12...71	int16_t		shBeat[60];			//	мгновенные значения зазора/осевого сдвига/ через 6 град (60 отсчетов на оборот)
// 72,73	uint32_t	CRC32;				//	CRC32 программы во флеш, подсчитанная в процессе работы
// 74...85	float		fKx[6];				//	наклон калибровочной кривой в точках калибровки (при нормальной температуре)
// 			float		fKxTM[7];			//	коэффициенты при х-точечной калибровке по температуре модуля
// 86...99	float		fKxTD[7];			//	коэффициенты при х-точечной калибровке по температуре датчика
//100...113	float		fumCallibr[7];		//	точки калибровки в мкм (с учетом температуры или без)
//114...127	float		fvNCallibr[7];		//	точки калибровки в отсчетах АЦП при нормальной температуре
// 128,129	float		fumCallibr0;		//	"0" калибровочной кривой для ПОВ, в мкм
// 130,131	float		fSpinSI;			//	скорость вращения (1/Гц = об/с)
// 132,133	float		fKPt;				//	коэффициент преобразования Pt (платинового терморезистора) для вычисления температуры
// ------------------------------------------
uint8_t ReadInputRegFC4(uint16_t *pSendLength)
{
	// pSendLength - указатель на длину выходного буфера
	// wNBytesInBuf - число байт в буфере ucInBufRS485
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
	ucOutBufRS485[2] = ucNBytes;						// число байт данных
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
	return 0;											// нормальный воврат
}

//------------------------------------------------------------------------------
// Обмен по ModBus
//------------------------------------------------------------------------------
void ProcModBus(void)
{
	static uint16_t SendLength, usNT1_5;
	static uint16_t indexRS485;			// указатель и счетчик байт при приеме/передачи по RS485

	uint16_t wCRC, wDR;
	uint8_t ucErrorCode, ucDR;

	if (!unModBus.bReg)
	//----------------------- режим IDLE ---------------------------------------
	{
		LedHL1_Off();											// Погасить LED RS485
		if (USART2->SR & USART_SR_RXNE)
		{
			if ((USART2->SR & USART_SR_PE)&&(!(sPVSConfig.MbBRP & 0x20)))	// проверка на отсутствие ошибки четности
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
				usNT1_5 = 3;									// счетчик числа переполнений counter_T1_5
				unModBus.usModBus = 1;							// -> режим ПРИЕМ
			}
		}
	}
	else if (unModBus.bReg == 1)
	//----------------------- режим ПРИЕМ --------------------------------------
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
			unModBus.bReg = 2;									// -> режим ПАРСИНГ
		else if (USART2->SR & USART_SR_RXNE)
		{
			counter_T1_5 = T1_5;
			counter_T3_5 = T3_5;
			if ((USART2->SR & USART_SR_PE)&&(!(sPVSConfig.MbBRP & 0x20)))	// проверка на отсутствие ошибки четности
				unModBus.bPErr = 1;
			if (indexRS485 >= BUF_RS485_SIZE)
				unModBus.usModBus = 0;							// при переполнении буфера -> режим IDLE
			else
				ucInBufRS485[indexRS485] = USART2->DR;
			indexRS485++;
		}
	}
	else if (unModBus.bReg == 2)
	//----------------------- режим ПАРСИНГ ------------------------------------
	{
		if (unModBus.bPErr || unModBus.bT1_5Err)
			unModBus.usModBus = 0;								// ошибка формата (четность и t1,5)
		else if (wCRC16t(ucInBufRS485, indexRS485))
			unModBus.usModBus = 0;								// ошибка CRC
		else if (!sPVSState.bWork)
		// ------------------ режим InitConfig ---------------------------------
		{
			if (indexRS485 != (PVS_CONFIG_BYTES+ 7))
				unModBus.usModBus = 0;							// неправильная длина посылки, блокировка ответа
			else
			{
				ucErrorCode = InitConfig(&SendLength, indexRS485);	// режим InitConfig
				if (!ucInBufRS485[0])
					unModBus.usModBus = 0;						// BroadCasting, блокировка ответа
			}
		}
		// ------------------ режим WORK ---------------------------------------
		else if (!ucInBufRS485[0])								// BroadCasting
		{
			if (ucInBufRS485[1] == 6)							// FC = 6 (Write Single Register)
				ucErrorCode = WriteSingleRegFC6(&SendLength, indexRS485);
			else if (ucInBufRS485[1] == 16)						// FC = 16 (Write Multiple Registers)
				ucErrorCode = WriteMultipleRegFC16(&SendLength, indexRS485);
			unModBus.usModBus = 0;								// FC не поддерживается, блокировка ответа
		}
		else if (ucInBufRS485[0] != sPVSConfig.MbAddr)
			unModBus.usModBus = 0;								// не совпадает адрес на шине RS485
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
			else												// FC не поддерживается
				ucErrorCode = 1;
		}
		// ------------------ Подготовка к передаче ----------------------------
		if (unModBus.bReg)										// ПАРСИНГ, UniCast
		{
			if (ucErrorCode)									// отработка ошибки
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
			GPIOD->BSRRL = 0x0080;								// PD7 = 1 (передача)
			unModBus.bReg = 3;									// -> режим ПЕРЕДАЧА
		}
	}
	else
	//----------------------- режим ПЕРЕДАЧА -----------------------------------
	{
		LedHL1_On();											// засветить LED RS485
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
			GPIOD->BSRRH = 0x0080;								// PD7 = 0 (прием)
			USART2->CR1 &= ~USART_CR1_TE;
			USART2->CR1 |= USART_CR1_RE;
			ucDR = USART2->DR;									// Новая вставка
			unModBus.usModBus = 0;								// -> режим IDLE
		}
	}
}
