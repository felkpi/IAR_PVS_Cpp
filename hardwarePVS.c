/*******************************************
 *	�/� ������ � ����������� ��������� PVS
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
// ��������� � �������������� ������ ���������� ��� 0...4095 (0...0xFFF)
// �������� ��������� 0...3,072�
// ���������c�� ������� 0,75�� = 0,075����.
//------------------------------------------------------------------------------
// �������������� ������� TMP36:
//	- 10��/�����
//	- ��� 25����� ���������� �� ������ 0,75�
//	- ��� 0����� ���������� �� ������ 0,5�
//------------------------------------------------------------------------------
// ��������� ���������� +5VA
//	- KP5VA - ����������� �������������� KP5VA = 3074,55*2/65536= 0,093828 ��
//------------------------------------------------------------------------------
// ��������� ���������� -5VA
//	��������� ������������ �� ������� UM5VA = 10 - KM5VA*N,
//		��� N - ������ (0...65535)
//		KM5VA - ����������� �������������� KM5VA = 3 * 3074,55 / 65536 = 0,140742
//------------------------------------------------------------------------------
// �������������� ����������� ������� �����������
//	- ������������� 1��� (��� 0 ����.�) + ������������� ��������������� ������ sPVSConfig.bRTD (0...3 ��)
//	- ����.���������� 3850ppm/����.� (3,85 ��/���� �)
//	- ������������� ��� 1,021446 ��
//	- ������ ��������� ��� 3,07455 �
//	- ���������� �� ����� ��� ��� 0 ����� = 1,021446 � (��������� 16-������� ��� - N0 = 21773)
//	- ���������� �� ����� ��� ��� 100 ����� = 1,021446 * 1,385 = 1,4147 � (��������� 16-������� ��� - N100 = 30155,24)
//	- ����������� �������������� ����������� K = 100/(30155,24-21773,77) = 0,01193
//	- ������� �������������� T = K * (N - N0) (� �����)
//------------------------------------------------------------------------------
void	GoADC(void)
{
	const float cfKP5VA = 0.093828;									// ����������� �������������� ��� +5VA
	const float cfKM5VA = 0.140742;									// ����������� �������������� ��� -5VA

	static uint16_t wCur1[3], wCur2[16], wCur3[16], wCur4[16];		// ������� ��������� ��� ���������� �������� ��������
	static uint16_t wInd1, wInd2, wInd3, wInd4;						// ������� ���������
	static uint16_t wTemp2, wTemp3, wTemp4;							// ������� �������

	//-------------------------
	// ������ ����������� TMP36
	//-------------------------
	if ((ADC1->SQR3 & ADC_SQR3_SQ1) == ADC_SQR3_SQ1_0)			
	{
		wCur1[wInd1] = ADC1->DR;
		if (++wInd1 >= 3)
			wInd1 = 0;
		sPVSState.shTempModule = ((int16_t)(wCur1[0]+wCur1[1]+wCur1[2])-2000)/40;	// ����������� � �����
		if (sPVSState.shTempModule > sPVSConfig.TMFault)
			sPVSState.bErrTempM = 1;								// ���������� ��� ������ ����������� ������
		else
			sPVSState.bErrTempM = 0;								// ���� ��� ������ ����������� ������
		ADC1->SQR3 = ADC_SQR3_SQ1_1;
		ADC1->CR2 |= ADC_CR2_SWSTART;
	}
	//-----------------------------------------------------
	// ��������� ���������� +5VA � ������� �������� 10k-10k
	//-----------------------------------------------------
	else if ((ADC1->SQR3 & ADC_SQR3_SQ1) == ADC_SQR3_SQ1_1)			// �������� 1/2 � +5VA (+2,5�)
	{
		wTemp2 -= wCur2[wInd2];
		wCur2[wInd2] = ADC1->DR;
		wTemp2 += wCur2[wInd2];										// ������� ����� �� ��������� 16 ���������
		wInd2++;
		wInd2 &= 0x0F;
		sPVSState.shP5VA = (int16_t)(wTemp2 * cfKP5VA);				// ���������� +5VA � ��
		if ((sPVSState.shP5VA < 4850)||(sPVSState.shP5VA > 5150))	// +5VA ������ �������� � ���� +/-3% (�.�. 4,85�...5,15�)
			sPVSState.bErrP5VA = 1;									// ���������� ��� ������ ���������� ������� +5VA
		else
			sPVSState.bErrP5VA = 0;									// ���� ��� ������ ���������� ������� +5VA
		ADC1->SQR3 |= ADC_SQR3_SQ1_0;
		ADC1->CR2 |= ADC_CR2_SWSTART;
	}
	//-----------------------------------------------------
	// ��������� ���������� -5VA � ������� �������� 10k-20k
	//-----------------------------------------------------
	else if ((ADC1->SQR3 & ADC_SQR3_SQ1) == (ADC_SQR3_SQ1_0 | ADC_SQR3_SQ1_1))	// �������� 2/3 +5VA � -5VA (+1,66�)
	{
		wTemp3 -= wCur3[wInd3];
		wCur3[wInd3] = ADC1->DR;
		wTemp3 += wCur3[wInd3];										// ������� ����� �� ��������� 16 ���������
		wInd3++;
		wInd3 &= 0x0F;
		
		sPVSState.shM5VA = (int16_t)(wTemp3 * cfKM5VA - 10000);		// ���������� -5VA � ��
		if ((sPVSState.shM5VA > -4750)||(sPVSState.shM5VA < -5250))	// -5VA ������ �������� � ���� +/-5% (�.�. 4,75�...5,25�)
			sPVSState.bErrM5VA = 1;									// ���������� ��� ������ ���������� ������� -5VA
		else
			sPVSState.bErrM5VA = 0;									// ���� ��� ������ ���������� ������� -5VA
		ADC1->SQR3 |= ADC_SQR3_SQ1_3;
		ADC1->CR2 |= ADC_CR2_SWSTART;
	}
	//-----------------------------------------------------
	// ������� ���������� ������ �����������
	//-----------------------------------------------------
	else													
	{
		wTemp4 -= wCur4[wInd4];
		wCur4[wInd4] = ADC1->DR;
		wTemp4 += wCur4[wInd4];										// ������� ����� �� ��������� 16 ���������
		wInd4++;
		wInd4 &= 0x0F;
		
		sPVSState.shTempSensor = (int16_t)((float)wTemp4*sPVSState.fKPt - 1/ALFA);	// ����������� � �����
		
		if (sPVSState.shTempSensor > sPVSConfig.TDFault)
			sPVSState.bErrTempS = 1;								// ���������� ��� ������ ����������� �������
		else
			sPVSState.bErrTempS = 0;								// ���� ��� ������ ����������� �������
		ADC1->SQR3 = ADC_SQR3_SQ1_0;
		ADC1->CR2 |= ADC_CR2_SWSTART;
	}
}

//==============================================================================
// �������� ����������������� PVS (�������������� ��� � 100 ���)
//------------------------------------------------------------------------------
void sbPVSControl(void)
{
	static uint32_t uiCounterErr;
	// �������� ��������� ������������� �����
	if (GPIOB->IDR & GPIO_IDR_IDR_5)
		sPVSState.bREGIM = 1;				// ��� ��������� ������������� �����:	0 - "������ ��" (���.2), 1 - "�����" (���.1)
	else
		sPVSState.bREGIM = 0;				// 										0 - "��������" (���.2), 1 - "�����" (���.1)

	GoADC();								// �������� ����������� �������, ������, +5VA � -5VA
	
	if (sPVSConfig.bFmtOut)					// �������� ������ ��� ���������� ����� ��������� ���� ��� ����������
	{										// U
		if (GPIOD->IDR & GPIO_IDR_IDR_1)	// �������� PD1
			sPVSState.bErrBrakeLine = 0;
		else
			sPVSState.bErrBrakeLine = 1;
	}
	else
	{										// I 4...20mA
		if (GPIOD->IDR & GPIO_IDR_IDR_0)	// �������� PD0
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
// �������������� ������ (������������� ���������, ������������) �� �������� ������ AD7689
//------------------------------------------------------------------------------
// ������� ���������� �������� �������� �������������� ������������ ���
// ��� ������������ ������������� ���������
//  fK3 = fK1 + (fK2-fK1)/(L2-L1)*(L3-L1)
//	fK1, (fK2-fK1)/(L2-L1) - ����������� �������
//------------------------------------------------------------------------------
float fConvert_AD7689(uint16_t usData)				// usData - ������ ��� �� ���������� ������
{
	float fData, fDT, fK;
	
	static uint8_t ucInd;
	static uint16_t usCur[256];						// ����� �������� �������� ��� (�� 256 ������)
	static uint32_t uiTemp;

	uiTemp -= usCur[ucInd];							// ������� ����� �� 256 ������
	usCur[ucInd] = usData;
	uiTemp += usData;								// ������� ����� �� ��������� 256 ���������
	ucInd++;
	sPVSState.usDataADC = uiTemp >> 8;				// ������� ������� �� 256 ��������� (��� ������������� ���������)
	
	fData = usData;

	if (sPVSConfig.bTMCal == 1)						// 1-� ��������� (�.8) �� ����������� ������
	{
		fDT = sPVSState.shTempModule - sPVSConfig.TNMCallibr;	// ���������� ����������� ������
		fData -= sPVSState.fK1xTM * fDT;			// ����������� � ���������� ����������� �������� ���
	}
	else if (sPVSConfig.bTMCal == 2)				// 2-� ��������� (�.8 � �.9) �� ����������� ������
	{
		fDT = sPVSState.shTempModule - sPVSConfig.TNMCallibr;	// ���������� ����������� ������
		fK = sPVSState.fK1xTM + sPVSState.fK2xTM * (fData - sPVSState.fvTCallibr[0]);
		fData -= fK*fDT;
	}

	if (sPVSConfig.bTDCal == 1)						// 1-� ��������� (�.8) �� ����������� �������
	{
		fDT = sPVSState.shTempSensor - sPVSConfig.TNDCallibr;	// ���������� ����������� �������
		fData -= sPVSState.fK1xTD * fDT;			// ����������� � ���������� ����������� �������� ���
	}
	else if (sPVSConfig.bTDCal == 2)				// 2-� ��������� (�.8 � �.9) �� ����������� �������
	{
		fDT = sPVSState.shTempSensor - sPVSConfig.TNDCallibr;	// ���������� ����������� �������
		fK = sPVSState.fK1xTD + sPVSState.fK2xTD * (fData - sPVSState.fvTCallibr[0]);
		fData -= fK*fDT;
	}

	if (fData <= 0)
		usData = 0;
	else
		usData = (uint16_t)fData;
	
	if (usData < sPVSConfig.vNCallibr[0])
		sPVSState.bErrDatchL = 1;					// ������ ������� (����� �����)
	else if (usData > sPVSConfig.vNCallibr[6])
		sPVSState.bErrDatchH = 1;					// ������ ������� (������� �����)
	else
		sPVSState.bErrDatchH = sPVSState.bErrDatchL = 0;
	
	if (sPVSConfig.bNCal == 0)						// 7-� ������������ ��� ���������� �����������
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

volatile uint32_t uiTimer1s, uiTimer60s;						// ������� ��� ��������� 1� � �� 60� (�������� 10���)
//------------------------------------------------------------------------------
// ��������, �� ��������� �� ���� SYNC, bMode = 1 (���), ���.2
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
		uiTim++;												// ������ ��������� ������� �������� ����
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
				uiCount++;										// ������� �������� ����
				uiTim1 = uiTim;
				uiTim = 0;
				uiBeat = 0;
			}
			else
			{
				sPVSState.bTMes = 1;
				uiTimer1s = T1S;
				uiTimer60s = T60S;								// ����� ���� ��������� ������� ��������
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
				uiTimer60s = T60S;								// ����� ���� ��������� ������� ��������
				uiCount = 0;
			}
		}
		sPVSState.usDataOut = IOUT_4mA;
	}
	else if (sPVSState.shGap <= sPVSConfig.umGapL)
	{
		if (sPVSState.usDataOut == IOUT_4mA)					// ������ ������� 1 �������
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
// ������� ��������� ������
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
	while (rd_index != wr_index_AD7689)									// �������� ���� �� � ������ ��� ������
	{
		usData = usBuf_AD7689[rd_index];
		if (++rd_index == BUF_AD7689_SIZE)
			rd_index = 0;
		fData = fConvert_AD7689(usData);								// �������������� ������ - ������������ � ������������� ���������
		sPVSState.shGap = (int16_t)fData;								// ���������� �������� ������ (� ���)
//----------------------------------------------------------------------------------------------------------------------------
// ��������, �� ��������� �� ���� SYNC
//----------------------------------------------------------------------------------------------------------------------------
		if (((sPVSConfig.bMode == 0)||((sPVSConfig.bMode == 2)&&(sPVSState.bREGIM == 0)))&&(sPVSConfig.bPVS == 1))	// ��� (��������)
			Tahometr_NOSYNC();
//----------------------------------------------------------------------------------------------------------------------------
// - ��������� ������� (� ������� ��������) �� ����� SYNC (PB10, TIM2_CH3), ������� ���������� 6 ���
// - ������������ � ������������� �������� ������ �� ������
//----------------------------------------------------------------------------------------------------------------------------
		else
		{
			if (TIM2->SR & TIM_SR_CC3IF)								// ������ �� ��������� ������
			{
				TIM2->SR = ~TIM_SR_CC3IF;
				if (sPVSState.bSyncP)
				{
					sPVSState.bSync = 1;
					uiTim2 = TIM2->CCR3;
					uiTim = uiTim2 - uiTim1;							// ������ �������� 
					uiSpin = 3600000000/uiTim;							// �������� ��������, ��/��� * 10 (�������� 0,1 ��/���)
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
				if ((uiTim2 - uiTim1) > 360000000)						// ���� ������ �������� ������ 1 ���
				{
					sPVSState.bSyncP = 0;
					sPVSState.bSync = 0;
					uiSpin = 0;
				}
				else if (sPVSState.bSync)								// SYNC �������
				{
					if (sPVSState.shGap < shMin)
						shMin = sPVSState.shGap;
					if (sPVSState.shGap > shMax)
						shMax = sPVSState.shGap;
					if (uiBeat < 60)
					{
						uiTmp = uiTim1 + (uiTim * uiBeat)/60;			// 60 ������� �� ������ ��������
						if ((uiTmp ^ uiTim2) & S32)						// �������� �� ������������ ������
						{
							if (uiTmp & S32)							// ������ �����, uiTim2 <= uiTmp
								uiBeat |= S32;
						}
						else if (uiTim2 >= uiTmp)						// ���������� �����
							uiBeat |= S32;
					}
				}
			}
//----------------------------------------------------------------------------------------------------------------------------
// ��� (+ ��������� ������� � ������� �������� �� ����� SYNC
//----------------------------------------------------------------------------------------------------------------------------
			if (sPVSConfig.bPVS == 0)									// =================== ��� ===============================
			{
				fData -= sPVSState.fumCallibr0;							// ��������� �� ����������� 0 (������ �����)
				if (sPVSConfig.bLPF == 0)							
					fData = df2sos_LPF_500Hz_50kHzSR(fData);			// ��� 500�� (SR = 50���)
				else if (sPVSConfig.bLPF == 1)
					fData = df2sos_LPF_1000Hz_50kHzSR(fData);			// ��� 1000�� (SR = 50���)
				if (sPVSConfig.bLPFD)									// ��������� (�������� 0,1�)
				{
					fData1 += fData;
					if (!(--iDec1))
					{
						fData2 = fData1 / 2500;
						iDec1 = 2500;									// ���������� �� 2500 ������ (����� ���������� 0,05�)
						fData1 = 0;
						fData2 = fCurAver(fData2);						// ������� ������� (SR = 20Hz)
						sPVSState.shDisplacement = (uint16_t)(fData2);	// ������ ����� (� ���)
					}
				}
				else
				{
					sPVSState.shDisplacement = (uint16_t)(fData);		// ������ ����� (� ���)
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
					sPVSState.usDataOut = (uint16_t)(cfKI[sPVSConfig.bRangeD] * fData2) + IOUT_12mA;	// ������ ��� ������ �� ����� 4...20��

				wr_index = wr_index_AD5662;
				usBuf_AD5662[wr_index] = sPVSState.usDataOut;
				if (++wr_index == BUF_AD5662_SIZE)
					wr_index = 0;
				wr_index_AD5662 = wr_index;
			}
//----------------------------------------------------------------------------------------------------------------------------
// ��� (+ ��������� ������� � ������� �������� �� ����� SYNC)
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
				// ������ ���������� �������� ������ � ���� ���� ��� ���������� 
				fData -= sPVSState.fumCallibr[1];
				if (!sPVSConfig.bFmtOut)								// ����� ��� 4...20��
				{
					if (sPVSState.bErrAll)
						sPVSState.usDataOut = IOUT_1mA;
					else
						sPVSState.usDataOut = (uint16_t)(cfKI[sPVSConfig.bRangeD] * fData) + IOUT_4mA;
				}
				else													// ����� ����������
					sPVSState.usDataOut = (uint16_t)(cfKU[sPVSConfig.bRangeD] * fData) + cusUOUT_0[sPVSConfig.bRangeD];
				wr_index = wr_index_AD5662;
				usBuf_AD5662[wr_index] = sPVSState.usDataOut;
				if (++wr_index == BUF_AD5662_SIZE)
					wr_index = 0;
				wr_index_AD5662 = wr_index;
			}
//----------------------------------------------------------------------------------------------------------------------------
// ��� (+ ��������� ������� � ������� �������� �� ����� SYNC)
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
					sPVSState.shGap = (int16_t)iResult;					// ���������� �������� ������ (� ���)
					fData1 = (float)iResult;
//------------------------------------------------------------------------------
//					iResult = iMax - iMin;
//				sPVSState.shVibration = (int16_t)iResult;			// ������ ���������������� (� ���)
//				fPp1 = iResult;
//				iMin = 20000;
//				iMax = 0;
//
//				if (iData > iMax)
//					iMax = iData;
//				if (iData < iMin)
//					iMin = iData;
//
//			sPVSState.shGap = (int16_t)fData;						// ���������� �������� ������ (� ���)
//			fData1 = fData;
				}

				if (sPVSConfig.bLPF == 0)
					fData = df2sos_LPF_500Hz_50kHzSR(fData);			// ��� 500�� (SR = 50���)
				else if (sPVSConfig.bLPF == 1)
					fData = df2sos_LPF_1000Hz_50kHzSR(fData);			// ��� 1000�� (SR = 50���)
				//------------------------------------------------------------------
				if (--iDec1 == 0)
				{
					if (sPVSConfig.bLPF == 0)
					{
						iDec1 = 10;										// ��������� �� SR = 5���
						if (sPVSConfig.bHPF == 0)
							fData = df2sos_HPF_5Hz_5kHzSR(fData);		// ��� 5�� (SR = 5kHz)
						else if (sPVSConfig.bHPF == 1)
							fData = df2sos_HPF_10Hz_5kHzSR(fData);		// ��� 10�� (SR = 5kHz)
					}
					else if (sPVSConfig.bLPF == 1)
					{
						iDec1 = 5;										// ��������� �� SR = 10���
						if (sPVSConfig.bHPF == 0)
							fData = df2sos_HPF_5Hz_10kHzSR(fData);		// ��� 5�� (SR = 10kHz)
						if (sPVSConfig.bHPF == 1)
							fData = df2sos_HPF_10Hz_10kHzSR(fData);		// ��� 10�� (SR = 10kHz)
					}
					if (fData > fMax)
						fMax = fData;
					if (fData < fMin)
						fMin = fData;
					if (--iDec2 == 0)									// ��������� �� ���� 2 ���. ����� (0,4� ��� 2,5...1000�� � 0,2� ��� 5...2000��)
					{
						iDec2 = 2000;
						fPp = fMax - fMin;
//					fPp -= 20.0;
//					if (fPp <= 0)
//						fPp = 0.0;
						fMax = fMin = 0.0;
						fPp = fCurAver2(fPp);
						sPVSState.shVibration = (int16_t)(fPp);			// ������� ������� ������� ���������������� (� ���)
						fPp1 = fPp;
					}
				}
//------------------------------------------------------------------------------		
				if ((sPVSState.bREGIM && (sPVSConfig.bMode == 2))||(sPVSConfig.bMode == 1))	// �� ������ I - �����
				{
					fData2 = fData1 - sPVSState.fumCallibr[1];
					if (sPVSState.bErrAll)
						sPVSState.usDataOut = IOUT_1mA;
					else
						sPVSState.usDataOut = (uint16_t)(cfKI[sPVSConfig.bRangeD] * fData2) + IOUT_4mA;
				}
				else													// �� ������ I ����������������
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
