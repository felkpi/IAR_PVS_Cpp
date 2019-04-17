/*******************************************************************************
 * file:	FiltrsEtc.c
 * author:	A.Vlashchuk
 * version:	V1.0.0
 * date:	23-July-2015
 * brief:	Цифровые фильтры (не Баттерворта и не Чебышева)
 *******************************************************************************/
#include "PVS.h"
#include "FiltersEtc.h"
#include "GlobalPVS.h"

//--------------------------------
// Текущее среднее (2...128 точек)
//--------------------------------
float fCurAver(float fData)
{
	const uint32_t uiTable[7] = {2, 4, 8, 16, 32, 64, 128};
	uint32_t uiTIndex;
	static uint32_t uiIndex;
	static float fBuf[128], fSumma;

	uiTIndex = sPVSConfig.bLPFD;
	if (!uiTIndex)
		return fData;
	uiTIndex--;

	fBuf[uiIndex] = fData;
	fSumma += fData;
	if (++uiIndex == uiTable[uiTIndex])
		uiIndex = 0;
	fData = fSumma / uiTable[uiTIndex];
	fSumma -= fBuf[uiIndex];
	return fData;
}

float fCurAver2(float fData)
{
	const uint32_t uiTable[7] = {1, 2, 4, 8, 16, 32, 64};
	const float fTable[7] = {1.0, 2.0, 4.0, 8.0, 16.0, 32.0, 64.0};
	uint32_t uiTIndex;
	static uint32_t uiIndex, uiCount;
	static float fBuf[6400], fSumma;

	uiTIndex = sPVSConfig.bLPFD & 0x7;
	if ((uiTIndex == 0)||(uiTIndex == 1)||(uiTIndex == 7))
		return fData;	// без обработки если время усреднения 0,1с || 0,2с || усреднение отключено
	else if (sPVSConfig.bLPF == 0)						// LPF 500Гц
	{
		if (uiTIndex == 2)
			return fData;
		uiTIndex >>= 2;
	}
	else if (sPVSConfig.bLPF == 1)						// LPF 1000Гц
		uiTIndex >>= 1;
	fBuf[uiIndex] = fData;
	if (++uiIndex == uiTable[uiTIndex])
		uiIndex = 0;
	fSumma += fData;
	if (++uiCount >= uiTable[uiTIndex])
	{
		uiCount--;
		fData = fSumma / fTable[uiTIndex];
		fSumma -= fBuf[uiIndex];
	}
	return fData;
}
