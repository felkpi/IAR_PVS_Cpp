/*******************************************
 *	П/п работы с аппаратными ресурсами PVS
 *******************************************/
#include "hardwarePVS.h"

void	LedHL1_On(void)
{
	GPIOB->BSRRL = HL1_PIN;
	sPVSState.bHL1 = 1;
}

void	LedHL1_Off(void)
{
	GPIOB->BSRRH = HL1_PIN;
	sPVSState.bHL1 = 0;
}

void	LedHL2_Red_On(void)
{
	GPIOB->BSRRL = HL2_RED_PIN;
	sPVSState.bHL2Red = 1;
}

void	LedHL2_Red_Off(void)
{
	GPIOB->BSRRH = HL2_RED_PIN;
	sPVSState.bHL2Red = 0;
}

void	LedHL2_Green_On(void)
{
	GPIOB->BSRRL = HL2_GREEN_PIN;
	sPVSState.bHL2Green = 1;
}

void	LedHL2_Green_Off(void)
{
	GPIOB->BSRRH = HL2_GREEN_PIN;
	sPVSState.bHL2Green = 0;
}

void	ClearAD5749_On(void)
{
	GPIOC->BSRRL = CLEAR_AD5749_PIN;
}

void	ClearAD5749_Off(void)
{
	GPIOC->BSRRH = CLEAR_AD5749_PIN;
}

void	SetPC9(void)
{
	GPIOC->BSRRL = PC9_PIN;
}

void	ResetPC9(void)
{
	GPIOC->BSRRH = PC9_PIN;
}

//==============================================================================
// Измерение и преобразование данных внутренним АЦП 0...4095 (0...0xFFF)
// Диапазон измерения 0...3,072В
// Дискретноcть отсчета 0,75мВ = 0,075град.
//------------------------------------------------------------------------------
// Характеристики датчика TMP36:
//	- 10мВ/градС
//	- при 25градС напряжение на выходе 0,75В
//	- при 0градС напряжение на выходе 0,5В
//------------------------------------------------------------------------------
// Измерение напряжения +5VA
//	- KP5VA - коэффициент преобразования KP5VA = 3074,55*2/65536= 0,093828 мВ
//------------------------------------------------------------------------------
// Измерение напряжения -5VA
//	Измерение производится по формуле UM5VA = 10 - KM5VA*N,
//		где N - отсчет (0...65535)
//		KM5VA - коэффициент преобразования KM5VA = 3 * 3074,55 / 65536 = 0,140742
//------------------------------------------------------------------------------
// Характеристики платинового датчика температуры
//	- сопротивление 1кОм (при 0 град.С) + сопротивление соединительного кабеля sPVSConfig.bRTD (0...3 Ом)
//	- темп.коєффициент 3850ppm/град.С (3,85 Ом/град С)
//	- измерительный ток 1,021446 мА
//	- предел измерения АЦП 3,07455 В
//	- напряжение на входе АЦП при 0 градС = 1,021446 В (показания 16-битного АЦП - N0 = 21773)
//	- напряжение на входе АЦП при 100 градС = 1,021446 * 1,385 = 1,4147 В (показания 16-битного АЦП - N100 = 30155,24)
//	- коэффициент преобразование температуры K = 100/(30155,24-21773,77) = 0,01193
//	- формула преобразования T = K * (N - N0) (в градС)
//------------------------------------------------------------------------------
void	GoADC(void)
{
	const float cfKP5VA = 0.093828;									// коэффициент преобразования для +5VA
	const float cfKM5VA = 0.140742;									// коэффициент преобразования для -5VA

	static uint16_t wCur1[3], wCur2[16], wCur3[16], wCur4[16];		// массивы измерений для вычисления текущего среднего
	static uint16_t wInd1, wInd2, wInd3, wInd4;						// индексы измерений
	static uint16_t wTemp2, wTemp3, wTemp4;							// текущие средние

	//-------------------------
	// датчик температуры TMP36
	//-------------------------
	if ((ADC1->SQR3 & ADC_SQR3_SQ1) == ADC_SQR3_SQ1_0)			
	{
		wCur1[wInd1] = ADC1->DR;
		if (++wInd1 >= 3)
			wInd1 = 0;
		sPVSState.shTempModule = ((int16_t)(wCur1[0]+wCur1[1]+wCur1[2])-2000)/40;	// температура в градС
		if (sPVSState.shTempModule > sPVSConfig.TMFault)
			sPVSState.bErrTempM = 1;								// установлен бит ошибки температуры модуля
		else
			sPVSState.bErrTempM = 0;								// снят бит ошибки температуры модуля
		ADC1->SQR3 = ADC_SQR3_SQ1_1;
		ADC1->CR2 |= ADC_CR2_SWSTART;
	}
	//-----------------------------------------------------
	// измерение напряжения +5VA с помощью делителя 10k-10k
	//-----------------------------------------------------
	else if ((ADC1->SQR3 & ADC_SQR3_SQ1) == ADC_SQR3_SQ1_1)			// делитель 1/2 к +5VA (+2,5В)
	{
		wTemp2 -= wCur2[wInd2];
		wCur2[wInd2] = ADC1->DR;
		wTemp2 += wCur2[wInd2];										// текущая сумма за последние 16 измерений
		wInd2++;
		wInd2 &= 0x0F;
		sPVSState.shP5VA = (int16_t)(wTemp2 * cfKP5VA);				// напряжение +5VA в мВ
		if ((sPVSState.shP5VA < 4850)||(sPVSState.shP5VA > 5150))	// +5VA должно попадать в окно +/-3% (т.е. 4,85В...5,15В)
			sPVSState.bErrP5VA = 1;									// установлен бит ошибки напряжения питания +5VA
		else
			sPVSState.bErrP5VA = 0;									// снят бит ошибки напряжения питания +5VA
		ADC1->SQR3 |= ADC_SQR3_SQ1_0;
		ADC1->CR2 |= ADC_CR2_SWSTART;
	}
	//-----------------------------------------------------
	// измерение напряжения -5VA с помощью делителя 10k-20k
	//-----------------------------------------------------
	else if ((ADC1->SQR3 & ADC_SQR3_SQ1) == (ADC_SQR3_SQ1_0 | ADC_SQR3_SQ1_1))	// делитель 2/3 +5VA и -5VA (+1,66В)
	{
		wTemp3 -= wCur3[wInd3];
		wCur3[wInd3] = ADC1->DR;
		wTemp3 += wCur3[wInd3];										// текущая сумма за последние 16 измерений
		wInd3++;
		wInd3 &= 0x0F;
		
		sPVSState.shM5VA = (int16_t)(wTemp3 * cfKM5VA - 10000);		// напряжение -5VA в мВ
		if ((sPVSState.shM5VA > -4750)||(sPVSState.shM5VA < -5250))	// -5VA должно попадать в окно +/-5% (т.е. 4,75В...5,25В)
			sPVSState.bErrM5VA = 1;									// установлен бит ошибки напряжения питания -5VA
		else
			sPVSState.bErrM5VA = 0;									// снят бит ошибки напряжения питания -5VA
		ADC1->SQR3 |= ADC_SQR3_SQ1_3;
		ADC1->CR2 |= ADC_CR2_SWSTART;
	}
	//-----------------------------------------------------
	// внешний платиновый датчик температуры
	//-----------------------------------------------------
	else													
	{
		wTemp4 -= wCur4[wInd4];
		wCur4[wInd4] = ADC1->DR;
		wTemp4 += wCur4[wInd4];										// текущая сумма за последние 16 измерений
		wInd4++;
		wInd4 &= 0x0F;
		
		sPVSState.shTempSensor = (int16_t)((float)wTemp4*sPVSState.fKPt - 1/ALFA);	// температура в градС
		
		if (sPVSState.shTempSensor > sPVSConfig.TDFault)
			sPVSState.bErrTempS = 1;								// установлен бит ошибки температуры датчика
		else
			sPVSState.bErrTempS = 0;								// снят бит ошибки температуры датчика
		ADC1->SQR3 = ADC_SQR3_SQ1_0;
		ADC1->CR2 |= ADC_CR2_SWSTART;
	}
}

//==============================================================================
// Контроль работоспособности PVS (осуществляется раз в 100 мкс)
//------------------------------------------------------------------------------
void sbPVSControl(void)
{
	static uint32_t uiCounterErr;
	// Проверка состояния переключателя РЕЖИМ
	if (GPIOB->IDR & GPIO_IDR_IDR_5)
		sPVSState.bREGIM = 1;				// бит состояния переключателя РЕЖИМ:	0 - "РАЗМАХ ВП" (РЕЖ.2), 1 - "ЗАЗОР" (РЕЖ.1)
	else
		sPVSState.bREGIM = 0;				// 										0 - "ТАХОМЕТР" (РЕЖ.2), 1 - "ЗАЗОР" (РЕЖ.1)

	GoADC();								// контроль температуры датчика, модуля, +5VA и -5VA
	
	if (sPVSConfig.bFmtOut)					// контроль обрыва или перегрузки линий выходного тока или напряжения
	{										// U
		if (GPIOD->IDR & GPIO_IDR_IDR_1)	// проверка PD1
			sPVSState.bErrBrakeLine = 0;
		else
			sPVSState.bErrBrakeLine = 1;
	}
	else
	{										// I 4...20mA
		if (GPIOD->IDR & GPIO_IDR_IDR_0)	// проверка PD0
			sPVSState.bErrBrakeLine = 0;
		else
			sPVSState.bErrBrakeLine = 1;
	}

	if (sPVSState.usErrPVS & 0xFFFE)
	{
	 	sPVSState.bErrAll = 1;
		LedHL2_Green_Off();
		LedHL2_Red_On();
		uiCounterErr = (uint32_t)sPVSConfig.tClearFault * 1000;
	}
	else if (uiCounterErr)
	{
		if (!(--uiCounterErr))
		{
		 	sPVSState.bErrAll = 0;
			LedHL2_Green_On();
			LedHL2_Red_Off();
		}
	}
}

//==============================================================================
// Преобразование данных (температурная коррекция, линеаризация) из входного буфера AD7689
//------------------------------------------------------------------------------
// Формула вычисления среднего значения температурного коэффициента при
// при двухточечной температурной коррекции
//  fK3 = fK1 + (fK2-fK1)/(L2-L1)*(L3-L1)
//	fK1, (fK2-fK1)/(L2-L1) - вычисляются заранее
//------------------------------------------------------------------------------
float fConvert_AD7689(uint16_t usData)				// usData - данные АЦП из кольцевого буфера
{
	float fData, fDT, fK;
	
	static uint8_t ucInd;
	static uint16_t usCur[256];						// буфер текущего среднего АЦП (по 256 точкам)
	static uint32_t uiTemp;

	uiTemp -= usCur[ucInd];							// текущая сумма по 256 точкам
	usCur[ucInd] = usData;
	uiTemp += usData;								// текущая сумма за последние 256 измерений
	ucInd++;
	sPVSState.usDataADC = uiTemp >> 8;				// текущее среднее за 256 измерений (без температурной коррекции)
	
	fData = usData;

	if (sPVSConfig.bTMCal == 1)						// 1-т коррекция (т.8) по температуре модуля
	{
		fDT = sPVSState.shTempModule - sPVSConfig.TNMCallibr;	// приращение температуры модуля
		fData -= sPVSState.fK1xTM * fDT;			// приведенное к нормальной температуре значение АЦП
	}
	else if (sPVSConfig.bTMCal == 2)				// 2-т коррекция (т.8 и т.9) по температуре модуля
	{
		fDT = sPVSState.shTempModule - sPVSConfig.TNMCallibr;	// приращение температуры модуля
		fK = sPVSState.fK1xTM + sPVSState.fK2xTM * (fData - sPVSState.fvTCallibr[0]);
		fData -= fK*fDT;
	}

	if (sPVSConfig.bTDCal == 1)						// 1-т коррекция (т.8) по температуре датчика
	{
		fDT = sPVSState.shTempSensor - sPVSConfig.TNDCallibr;	// приращение температуры датчика
		fData -= sPVSState.fK1xTD * fDT;			// приведенное к нормальной температуре значение АЦП
	}
	else if (sPVSConfig.bTDCal == 2)				// 2-т коррекция (т.8 и т.9) по температуре датчика
	{
		fDT = sPVSState.shTempSensor - sPVSConfig.TNDCallibr;	// приращение температуры датчика
		fK = sPVSState.fK1xTD + sPVSState.fK2xTD * (fData - sPVSState.fvTCallibr[0]);
		fData -= fK*fDT;
	}

	if (fData <= 0)
		usData = 0;
	else
		usData = (uint16_t)fData;
	
	if (usData < sPVSConfig.vNCallibr[0])
		sPVSState.bErrDatchL = 1;					// ошибка датчика (малый зазор)
	else if (usData > sPVSConfig.vNCallibr[6])
		sPVSState.bErrDatchH = 1;					// ошибка датчика (большой зазор)
	else
		sPVSState.bErrDatchH = sPVSState.bErrDatchL = 0;
	
	if (sPVSConfig.bNCal == 0)						// 7-т линеаризация при нормальной температуре
	{
		if (usData < sPVSConfig.vNCallibr[1])
			fData = sPVSState.fumCallibr[1] - sPVSState.fKx[0] * (sPVSState.fvNCallibr[1] - fData);
		else if (usData < sPVSConfig.vNCallibr[2])
			fData = sPVSState.fKx[1] * (fData - sPVSState.fvNCallibr[1]) + sPVSState.fumCallibr[1];
		else if (usData < sPVSConfig.vNCallibr[3])
			fData = sPVSState.fKx[2] * (fData - sPVSState.fvNCallibr[2]) + sPVSState.fumCallibr[2];
		else if (usData < sPVSConfig.vNCallibr[4])
			fData = sPVSState.fKx[3] * (fData - sPVSState.fvNCallibr[3]) + sPVSState.fumCallibr[3];
		else if (usData < sPVSConfig.vNCallibr[5])
			fData = sPVSState.fKx[4] * (fData - sPVSState.fvNCallibr[4]) + sPVSState.fumCallibr[4];
		else
			fData = sPVSState.fKx[5] * (fData - sPVSState.fvNCallibr[5]) + sPVSState.fumCallibr[5];
	}
	return	fData;
}

volatile uint32_t uiTimer1s, uiTimer60s;						// таймеры для измерения 1с и до 60с (дискрета 10мкс)
//------------------------------------------------------------------------------
// Тахометр, не реагирует на вход SYNC, bMode = 1 (ПЗВ), РЕЖ.2
//------------------------------------------------------------------------------
void Tahometr_NOSYNC(void)
{
	static int16_t shMin, shMax;
	static uint32_t	uiCount, uiTim, uiTim1, uiBeat;
	uint32_t uiTmp, wr_index;

	sPVSState.bSync = 0;
	if (!uiTimer60s)
	{
		sPVSState.usSpin = 0;
		sPVSState.bTMes = 0;
		uiTim1 = 0;
	}
	else
	{
		uiTim++;												// таймер измерения периода вращения вала
		if (uiTim1)
		{
			uiTmp = uiTim1 * uiBeat / 60;
			if ((uiTim >= uiTmp)&&(uiBeat < 60))
			{
				sPVSState.shBeat[uiBeat] = sPVSState.shGap;
				uiBeat++;
			}
		}
	}
	if (sPVSState.shGap >= sPVSConfig.umGapH)
	{
		if (sPVSState.usDataOut == IOUT_12mA)
		{
			if (sPVSState.bTMes)
			{
				uiCount++;										// счетчик оборотов вала
				uiTim1 = uiTim;
				uiTim = 0;
				uiBeat = 0;
			}
			else
			{
				sPVSState.bTMes = 1;
				uiTimer1s = T1S;
				uiTimer60s = T60S;								// новый цикл измерения периода вращения
				uiCount = 0;
				uiTim = 0;
				shMin = sPVSState.shGap;
				shMax = sPVSState.shGap;
			}
			if (!uiTimer1s)
			{
				uint64_t llCount = (uint64_t)uiCount*60000000LL/(T60S-uiTimer60s);
				if (llCount > 65535LL)
					sPVSState.usSpin = S16U;
				else
					sPVSState.usSpin = llCount;
				uiTimer1s = T1S;
				uiTimer60s = T60S;								// новый цикл измерения периода вращения
				uiCount = 0;
			}
		}
		sPVSState.usDataOut = IOUT_4mA;
	}
	else if (sPVSState.shGap <= sPVSConfig.umGapL)
	{
		if (sPVSState.usDataOut == IOUT_4mA)					// начало отсчета 1 оборота
		{
			sPVSState.shGapMin = shMin;
			sPVSState.shGapMax = shMax;
			shMin = sPVSState.shGap;
			shMax = sPVSState.shGap;
		}
		sPVSState.usDataOut = IOUT_12mA;
	}
	if (sPVSState.shGap < shMin)
		shMin = sPVSState.shGap;
	if (sPVSState.shGap > shMax)
		shMax = sPVSState.shGap;
	wr_index = wr_index_AD5662;
	usBuf_AD5662[wr_index] = sPVSState.usDataOut;
	if (++wr_index == BUF_AD5662_SIZE)
		wr_index = 0;
	wr_index_AD5662 = wr_index;
}

//==============================================================================
// Функция обработки данных
//------------------------------------------------------------------------------
void ConvertData(void)
{
	static int16_t shMin, shMax;
	static int32_t iDec1 = 5, iDec2 = 2000, iCount, iSumma;
	static uint32_t	uiTim, uiTim1, uiTim2, uiSpin, uiBeat;
	static float fData, fData1, fData2, fPp1, fMax, fMin;

	uint16_t usData;
	int32_t iData, iResult;
	uint32_t uiTmp, rd_index, wr_index;
	float fPp;
	
	rd_index = rd_index_AD7689;
	while (rd_index != wr_index_AD7689)									// проверка если ли в буфере АЦП данные
	{
		usData = usBuf_AD7689[rd_index];
		if (++rd_index == BUF_AD7689_SIZE)
			rd_index = 0;
		fData = fConvert_AD7689(usData);								// Преобразование данных - линеаризация и температурная коррекция
		sPVSState.shGap = (int16_t)fData;								// мгновенное значение зазора (в мкм)
//----------------------------------------------------------------------------------------------------------------------------
// Тахометр, не реагирует на вход SYNC
//----------------------------------------------------------------------------------------------------------------------------
		if (((sPVSConfig.bMode == 0)||((sPVSConfig.bMode == 2)&&(sPVSState.bREGIM == 0)))&&(sPVSConfig.bPVS == 1))	// ПЗВ (Тахометр)
			Tahometr_NOSYNC();
//----------------------------------------------------------------------------------------------------------------------------
// - измерение периода (и частоты вращения) по входу SYNC (PB10, TIM2_CH3), частота заполнения 6 МГц
// - минимального и максимального значения зазора за оборот
//----------------------------------------------------------------------------------------------------------------------------
		else
		{
			if (TIM2->SR & TIM_SR_CC3IF)								// Захват по переднему фронту
			{
				TIM2->SR = ~TIM_SR_CC3IF;
				if (sPVSState.bSyncP)
				{
					sPVSState.bSync = 1;
					uiTim2 = TIM2->CCR3;
					uiTim = uiTim2 - uiTim1;							// Период вращения 
					uiSpin = 3600000000/uiTim;							// Скорость вращения, об/мин * 10 (дискрета 0,1 об/мин)
					uiTim1 = uiTim2;
					uiBeat = 0;
					sPVSState.shGapMin = shMin;
					sPVSState.shGapMax = shMax;
					shMin = sPVSState.shGap;
					shMax = sPVSState.shGap;
				}
				else
				{
					sPVSState.bSyncP = 1;
					uiTim1 = TIM2->CCR3;
					shMin = sPVSState.shGap;
					shMax = sPVSState.shGap;
				}
			}
			if (sPVSState.bSyncP)
			{
				uiTim2 = TIM2->CNT;
				if ((uiTim2 - uiTim1) > 360000000)						// если период вращения больше 1 мин
				{
					sPVSState.bSyncP = 0;
					sPVSState.bSync = 0;
					uiSpin = 0;
				}
				else if (sPVSState.bSync)								// SYNC активен
				{
					if (sPVSState.shGap < shMin)
						shMin = sPVSState.shGap;
					if (sPVSState.shGap > shMax)
						shMax = sPVSState.shGap;
					if (uiBeat < 60)
					{
						uiTmp = uiTim1 + (uiTim * uiBeat)/60;			// 60 уставок на период вращения
						if ((uiTmp ^ uiTim2) & S32)						// проверка на одинаковость знаков
						{
							if (uiTmp & S32)							// разные знаки, uiTim2 <= uiTmp
								uiBeat |= S32;
						}
						else if (uiTim2 >= uiTmp)						// одинаковые знаки
							uiBeat |= S32;
					}
				}
			}
//----------------------------------------------------------------------------------------------------------------------------
// ПОВ (+ измерение периода и частоты вращения по входу SYNC
//----------------------------------------------------------------------------------------------------------------------------
			if (sPVSConfig.bPVS == 0)									// =================== ПОВ ===============================
			{
				fData -= sPVSState.fumCallibr0;							// коррекция на виртуальный 0 (осевой сдвиг)
				if (sPVSConfig.bLPF == 0)							
					fData = df2sos_LPF_500Hz_50kHzSR(fData);			// ФНЧ 500Гц (SR = 50кГц)
				else if (sPVSConfig.bLPF == 1)
					fData = df2sos_LPF_1000Hz_50kHzSR(fData);			// ФНЧ 1000Гц (SR = 50кГц)
				if (sPVSConfig.bLPFD)									// децимация (дискрета 0,1с)
				{
					fData1 += fData;
					if (!(--iDec1))
					{
						fData2 = fData1 / 2500;
						iDec1 = 2500;									// усреднение по 2500 точкам (время усреднения 0,05с)
						fData1 = 0;
						fData2 = fCurAver(fData2);						// текущее среднее (SR = 20Hz)
						sPVSState.shDisplacement = (uint16_t)(fData2);	// осевой сдвиг (в мкм)
					}
				}
				else
				{
					sPVSState.shDisplacement = (uint16_t)(fData);		// осевой сдвиг (в мкм)
					fData2 = fData;
				}

				if (uiSpin <= S16U)
					sPVSState.usSpin = uiSpin;
				else
					sPVSState.usSpin = S16U;
				if (uiBeat & S32)
				{
					uiBeat &= ~S32;
					sPVSState.shBeat[uiBeat] = sPVSState.shDisplacement;
					uiBeat++;
				}
				if (sPVSState.bErrAll)
					sPVSState.usDataOut = IOUT_1mA;
				else
					sPVSState.usDataOut = (uint16_t)(cfKI[sPVSConfig.bRangeD] * fData2) + IOUT_12mA;	// данные для выдачи на выход 4...20мА

				wr_index = wr_index_AD5662;
				usBuf_AD5662[wr_index] = sPVSState.usDataOut;
				if (++wr_index == BUF_AD5662_SIZE)
					wr_index = 0;
				wr_index_AD5662 = wr_index;
			}
//----------------------------------------------------------------------------------------------------------------------------
// ПЗВ (+ измерение периода и частоты вращения по входу SYNC)
//----------------------------------------------------------------------------------------------------------------------------
			else if (sPVSConfig.bPVS == 1)
			{
				if (uiSpin <= S16U)
					sPVSState.usSpin = uiSpin;
				else
					sPVSState.usSpin = S16U;
				if (uiBeat & S32)
				{
					uiBeat &= ~S32;
					sPVSState.shBeat[uiBeat] = sPVSState.shGap;
					uiBeat++;
				}
				// выдача измеренных значений зазора в виде тока или напряжения 
				fData -= sPVSState.fumCallibr[1];
				if (!sPVSConfig.bFmtOut)								// выход ток 4...20мА
				{
					if (sPVSState.bErrAll)
						sPVSState.usDataOut = IOUT_1mA;
					else
						sPVSState.usDataOut = (uint16_t)(cfKI[sPVSConfig.bRangeD] * fData) + IOUT_4mA;
				}
				else													// выход напряжение
					sPVSState.usDataOut = (uint16_t)(cfKU[sPVSConfig.bRangeD] * fData) + cusUOUT_0[sPVSConfig.bRangeD];
				wr_index = wr_index_AD5662;
				usBuf_AD5662[wr_index] = sPVSState.usDataOut;
				if (++wr_index == BUF_AD5662_SIZE)
					wr_index = 0;
				wr_index_AD5662 = wr_index;
			}
//----------------------------------------------------------------------------------------------------------------------------
// ПВВ (+ измерение периода и частоты вращения по входу SYNC)
//----------------------------------------------------------------------------------------------------------------------------
			else
			{
				iData = (int32_t)fData;
				iSumma += iData;
//
				if (++iCount == 50000)
				{
					iResult = iSumma / iCount;
					iCount = 0;
					iSumma = 0;
					sPVSState.shGap = (int16_t)iResult;					// мгновенное значение зазора (в мкм)
					fData1 = (float)iResult;
//------------------------------------------------------------------------------
//					iResult = iMax - iMin;
//				sPVSState.shVibration = (int16_t)iResult;			// размах виброперемещений (в мкм)
//				fPp1 = iResult;
//				iMin = 20000;
//				iMax = 0;
//
//				if (iData > iMax)
//					iMax = iData;
//				if (iData < iMin)
//					iMin = iData;
//
//			sPVSState.shGap = (int16_t)fData;						// мгновенное значение зазора (в мкм)
//			fData1 = fData;
				}

				if (sPVSConfig.bLPF == 0)
					fData = df2sos_LPF_500Hz_50kHzSR(fData);			// ФНЧ 500Гц (SR = 50кГц)
				else if (sPVSConfig.bLPF == 1)
					fData = df2sos_LPF_1000Hz_50kHzSR(fData);			// ФНЧ 1000Гц (SR = 50кГц)
				//------------------------------------------------------------------
				if (--iDec1 == 0)
				{
					if (sPVSConfig.bLPF == 0)
					{
						iDec1 = 10;										// децимация до SR = 5кГц
						if (sPVSConfig.bHPF == 0)
							fData = df2sos_HPF_5Hz_5kHzSR(fData);		// ФВЧ 5Гц (SR = 5kHz)
						else if (sPVSConfig.bHPF == 1)
							fData = df2sos_HPF_10Hz_5kHzSR(fData);		// ФВЧ 10Гц (SR = 5kHz)
					}
					else if (sPVSConfig.bLPF == 1)
					{
						iDec1 = 5;										// децимация до SR = 10кГц
						if (sPVSConfig.bHPF == 0)
							fData = df2sos_HPF_5Hz_10kHzSR(fData);		// ФВЧ 5Гц (SR = 10kHz)
						if (sPVSConfig.bHPF == 1)
							fData = df2sos_HPF_10Hz_10kHzSR(fData);		// ФВЧ 10Гц (SR = 10kHz)
					}
					if (fData > fMax)
						fMax = fData;
					if (fData < fMin)
						fMin = fData;
					if (--iDec2 == 0)									// децимация на окно 2 тыс. точек (0,4с для 2,5...1000Гц и 0,2с для 5...2000Гц)
					{
						iDec2 = 2000;
						fPp = fMax - fMin;
//					fPp -= 20.0;
//					if (fPp <= 0)
//						fPp = 0.0;
						fMax = fMin = 0.0;
						fPp = fCurAver2(fPp);
						sPVSState.shVibration = (int16_t)(fPp);			// текущее среднее размаха виброперемещений (в мкм)
						fPp1 = fPp;
					}
				}
//------------------------------------------------------------------------------		
				if ((sPVSState.bREGIM && (sPVSConfig.bMode == 2))||(sPVSConfig.bMode == 1))	// на выходе I - ЗАЗОР
				{
					fData2 = fData1 - sPVSState.fumCallibr[1];
					if (sPVSState.bErrAll)
						sPVSState.usDataOut = IOUT_1mA;
					else
						sPVSState.usDataOut = (uint16_t)(cfKI[sPVSConfig.bRangeD] * fData2) + IOUT_4mA;
				}
				else													// на выходе I ВИБРОПЕРЕМЕЩЕНИЕ
				{
					if (sPVSState.bErrAll)
						sPVSState.usDataOut = IOUT_1mA;
					else
						sPVSState.usDataOut = (uint16_t)(cfKIV[sPVSConfig.bRangeV] * fPp1) + IOUT_4mA;
				}
				wr_index = wr_index_AD5662;
				usBuf_AD5662[wr_index] = sPVSState.usDataOut;
				if (++wr_index == BUF_AD5662_SIZE)
					wr_index = 0;
				wr_index_AD5662 = wr_index;
			}
		}
	}
	rd_index_AD7689 = rd_index;
}
