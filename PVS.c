/*******************************************************************************
 * file:	PVS.c (��������������� ������������)
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
// ���� (��������� ������������ �����)
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
// ������������� ������
	unFlags.uiFlags = 0;
	pBBFlags = (volatile struct defBBFlags *)(BB_ALIAS((uint32_t)(&unFlags)));
// ������������� ���������� ��������
	InitGPIO();
// ��������� (��� ����������) ����������� ����� �������������� �������� ����� ����
	if (*pcCRC32 == 0xFFFFFFFF)
	{
		// ���������� CRC32 ������ ��������
		dwCRC32 = CRC32_CalcStAddrToFinAddr((uint32_t*)FLASH_SECTOR_0, (uint32_t*)pcCRC32);
		// ������ CRC32	� ��������� ������� ����� �������
		Flash_Unlock();
		Flash_PSIZE_SNB((uint32_t)2, (((uint32_t)pcCRC32)>>14)&0x000F);	// PSIZE = 2 (x32), SNB = 0
		Flash_Prog_Word((uint32_t)pcCRC32, dwCRC32);
		Flash_Lock();
//		Flash_Guard();													// ��������� ������ ������ - nWRP (0...2) RDP (Lev.1)
	}
// Setup LSI
	RCC->CSR |= RCC_CSR_LSION;
	while (!(RCC->CSR & RCC_CSR_LSIRDY));
// Setup IWDG (�� ������ ���������������)
/*	while (IWDG->SR & IWDG_SR_PVU);		// �������� ���������� ������������ ��������� IWDG->PR � IWDG->RLR
	IWDG->KR = 0x5555;
	IWDG->PR = 0x0003;					// ��������� 1���
	while (IWDG->SR & IWDG_SR_RVU);		// �������� ���������� ������������ �������� IWDG->RLR
	IWDG->KR = 0x5555;
	IWDG->RLR = 1000;					// ��� ������� ������ IWDG < 1�
	IWDG->KR = 0xCCCC;					// ����� IWDG � �������� 4,096�
*/
	dwCRC32 = CRC32_CalcStAddrToFinAddr((uint32_t*)FLASH_SECTOR_0, (uint32_t*)pcCRC32);
	while (dwCRC32 != *pcCRC32)
		dwCRC32 = CRC32_CalcStAddrToFinAddr((uint32_t*)FLASH_SECTOR_0, (uint32_t*)pcCRC32);	// �������� ������ �� IWDG
	InitCRC();
// ������������� ADC1, DAC
	InitADC1();
	InitDAC();
	
// ������������� Timer2
	InitTIM2();
// �������� �����������
//	LedHL1_On();
	LedHL1_Off();
//	LedHL2_Red_On();
	LedHL2_Red_Off();
	LedHL2_Green_On();
//	LedHL2_Green_Off();

//	�������� ����������� ������� CLEAR
//	������� CLEAR
//	ClearAD5749_On();
//	ClearAD5749_Off();

// ������������� SPI1
	InitSPI1();
// ������������� SPI3
	InitSPI3();
// ������������� DMA ��� SPI3
	InitDMA_SPI3();
	while (DMA1_Stream5->CR & DMA_SxCR_EN);			// �������� ��������� ����� DMA

// Setup SysTick Timer for 10us interrupts.
	if (SysTick_Config(SystemCoreClock / 100000))	// Interrupt Period = 10uS (100���)
		while (1);									// Capture error

// ����������� ����� ������ ������ ("InitConfig" ��� "Work")
	if (!(GPIOA->IDR & GPIO_IDR_IDR_10))
	{
//------------------------------------------------------------------------------
// ����� InitConfig
//------------------------------------------------------------------------------
		sPVSState.bWork = 0;						// ����� "InitConfig" (����� ���������� MasterPass)
		InitRS485();								// �� �����
		while (!(GPIOA->IDR & GPIO_IDR_IDR_10))
		{
			ProcModBus();							// ����� "InitConfig" (����� ���������� MasterPass)
			IWDG->KR = 0xAAAA;						// ������������ IWDG � �������� 1�
		}
	}
//------------------------------------------------------------------------------
// ����� Work
//------------------------------------------------------------------------------
	sPVSState.bConfig = 1;
	sPVSState.bRS485 = 1;
	ReInitConfig();

//------------------------------------------------
// ������������� TIM6 (������� ������������ �����)
//------------------------------------------------
	InitTIM6();
	CycleTimeMax = 0;
	while(1)
	{
//------------------------------------
// ���� (��������� ������������ �����)
//------------------------------------
		CycleTest();
//------------------------------------
		ReInitConfig();								// ��������������� ������ � ������ ������������� (bConfig = 1 �/��� bRS485 = 1)
		if (pBBFlags->b100uS)						// �������� ����� 100���
		{
			pBBFlags->b100uS = 0;
			sbPVSControl();							// �������� ����������������� PVS (��� � 100���)
		}
		ConvertData();								// �������� ������� � �������������� ������� ������� ������
		ProcModBus();								// ��������� ModBus
// �������� ����������� ���� � ����� IWDG
		CRC16t_CycleVerify();
		CRC32_CycleVerify();
		IWDG->KR = 0xAAAA;							// ������������ IWDG � �������� 1�
	}
}
/******************* (C) COPYRIGHT 2015 NAEK ENERGOATOM *****END OF FILE****/
