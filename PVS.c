/*******************************************************************************
 * file:	PVS.c (ПРЕОБРАЗОВАТЕЛЬ ВИХРЕТОКОВЫЙ)
 * author:	A.Vlashchuk
 * version:	01.0.0
 * date:	19-December-2015
 * brief:	Main program body
 *******************************************************************************/

#include <string.h>
#include "PVS.h"
#include "initPVS.h"
#include "FiltersButt.h"
#include "Flash.h"
#include "GlobalPVS.h"
#include "hardwarePVS.h"
#include "ModBus.h"
#include "Safety.h"

//------------------------------------
// Тест (измерение длительности цикла)
//------------------------------------
uint16_t CycleTimeMax, CycleTimeCurr;

void CycleTest(void)
{
	CycleTimeCurr = TIM6->CNT;
	if (CycleTimeCurr > CycleTimeMax)
		CycleTimeMax = CycleTimeCurr;
	TIM6->CNT = 0;
}

int	main(void)
{
//	uint16_t wCRC16;
	uint32_t dwCRC32;
// Инициализация флагов
	unFlags.uiFlags = 0;
	pBBFlags = (volatile struct defBBFlags *)(BB_ALIAS((uint32_t)(&unFlags)));
// Инициализация аппаратних ресурсов
	InitGPIO();
// Проверяем (или записываем) контрольную сумму метрологически значимой части флеш
	if (*pcCRC32 == 0xFFFFFFFF)
	{
		// вычисление CRC32 памяти программ
		dwCRC32 = CRC32_CalcStAddrToFinAddr((uint32_t*)FLASH_SECTOR_0, (uint32_t*)pcCRC32);
		// запись CRC32	в последнее двойное слово сектора
		Flash_Unlock();
		Flash_PSIZE_SNB((uint32_t)2, (((uint32_t)pcCRC32)>>14)&0x000F);	// PSIZE = 2 (x32), SNB = 0
		Flash_Prog_Word((uint32_t)pcCRC32, dwCRC32);
		Flash_Lock();
//		Flash_Guard();													// Установка защиты памяти - nWRP (0...2) RDP (Lev.1)
	}
// Setup LSI
	RCC->CSR |= RCC_CSR_LSION;
	while (!(RCC->CSR & RCC_CSR_LSIRDY));
// Setup IWDG (не забыть раскомментарить)
/*	while (IWDG->SR & IWDG_SR_PVU);		// ожидание завершения перезагрузки регистров IWDG->PR и IWDG->RLR
	IWDG->KR = 0x5555;
	IWDG->PR = 0x0003;					// прескалер 1кГц
	while (IWDG->SR & IWDG_SR_RVU);		// ожидание завершения перезагрузки регистра IWDG->RLR
	IWDG->KR = 0x5555;
	IWDG->RLR = 1000;					// для периода сброса IWDG < 1с
	IWDG->KR = 0xCCCC;					// старт IWDG с периодом 4,096с
*/
	dwCRC32 = CRC32_CalcStAddrToFinAddr((uint32_t*)FLASH_SECTOR_0, (uint32_t*)pcCRC32);
	while (dwCRC32 != *pcCRC32)
		dwCRC32 = CRC32_CalcStAddrToFinAddr((uint32_t*)FLASH_SECTOR_0, (uint32_t*)pcCRC32);	// Ожидание сброса по IWDG
	InitCRC();
// Инициализация ADC1, DAC
	InitADC1();
	InitDAC();
	
// Инициализация Timer2
	InitTIM2();
// Проверка светодиодов
//	LedHL1_On();
	LedHL1_Off();
//	LedHL2_Red_On();
	LedHL2_Red_Off();
	LedHL2_Green_On();
//	LedHL2_Green_Off();

//	Проверка прохождения сигнала CLEAR
//	Дергаем CLEAR
//	ClearAD5749_On();
//	ClearAD5749_Off();

// Инициализация SPI1
	InitSPI1();
// Инициализация SPI3
	InitSPI3();
// Инициализация DMA для SPI3
	InitDMA_SPI3();
	while (DMA1_Stream5->CR & DMA_SxCR_EN);			// Ожидание окончания цикла DMA

// Setup SysTick Timer for 10us interrupts.
	if (SysTick_Config(SystemCoreClock / 100000))	// Interrupt Period = 10uS (100кГц)
		while (1);									// Capture error

// Анализируем режим работы модуля ("InitConfig" или "Work")
	if (!(GPIOA->IDR & GPIO_IDR_IDR_10))
	{
//------------------------------------------------------------------------------
// Режим InitConfig
//------------------------------------------------------------------------------
		sPVSState.bWork = 0;						// режим "InitConfig" (можно установить MasterPass)
		InitRS485();								// на прием
		while (!(GPIOA->IDR & GPIO_IDR_IDR_10))
		{
			ProcModBus();							// режим "InitConfig" (можно установить MasterPass)
			IWDG->KR = 0xAAAA;						// перезагрузка IWDG с периодом 1с
		}
	}
//------------------------------------------------------------------------------
// Режим Work
//------------------------------------------------------------------------------
	sPVSState.bConfig = 1;
	sPVSState.bRS485 = 1;
	ReInitConfig();

//------------------------------------------------
// Инициализация TIM6 (таймера длительности цикла)
//------------------------------------------------
	InitTIM6();
	CycleTimeMax = 0;
	while(1)
	{
//------------------------------------
// Тест (измерение длительности цикла)
//------------------------------------
		CycleTest();
//------------------------------------
		ReInitConfig();								// реинициализация модуля в случае необходимости (bConfig = 1 и/или bRS485 = 1)
		if (pBBFlags->b100uS)						// проверка флага 100мкс
		{
			pBBFlags->b100uS = 0;
			sbPVSControl();							// проверка работоспособности PVS (раз в 100мкс)
		}
		ConvertData();								// проверка наличия и преобразование формата входных данных
		ProcModBus();								// Отработка ModBus
// Проверка контрольных сумм и сброс IWDG
		CRC16t_CycleVerify();
		CRC32_CycleVerify();
		IWDG->KR = 0xAAAA;							// перезагрузка IWDG с периодом 1с
	}
}
/******************* (C) COPYRIGHT 2015 NAEK ENERGOATOM *****END OF FILE****/
