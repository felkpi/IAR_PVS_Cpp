/*******************************************************************************
 * file:	Flash.c
 * author:	A.Vlashchuk
 * version:	V1.0.0
 * date:	31-July-2015
 * brief:	������ � Flash
 *******************************************************************************/
#include "Flash.h"
#include "GlobalPVS.h"
#include "InitPVS.h"

//------------------------------------------------------------------------------
// Unlock Flash
//------------------------------------------------------------------------------
void Flash_Unlock(void)
{
	if (FLASH->CR & FLASH_CR_LOCK)
	{
		FLASH->KEYR = KEY1;
		FLASH->KEYR = KEY2;
	}
}

//------------------------------------------------------------------------------
// Lock Flash
//------------------------------------------------------------------------------
void Flash_Lock(void)
{
	FLASH->CR |= FLASH_CR_LOCK;
}

//------------------------------------------------------------------------------
// ���������� ����������� � ����� ������� Flash (���������� ��� ���������)
//------------------------------------------------------------------------------
// PSize - ����������� (0 - x8, 1 - x16, 2 - x32, 3 - x64)
// SectorNumber - ����� ������� (0...11, 0...7 ��� STM32F407VET6)
//------------------------------------------------------------------------------
void Flash_PSIZE_SNB(uint32_t PSize, uint32_t SectorNumber)
{
	FLASH->CR = ((PSize & 3) << 8) | ((SectorNumber & 0x0F) << 3);
}

//------------------------------------------------------------------------------
// �������� ������� (�� ����� ������ ���� ����������� PSIZE � SNB)
//------------------------------------------------------------------------------
void Flash_Sector_Erase(void)
{
	while (FLASH->SR & FLASH_SR_BSY);
	FLASH->CR |= FLASH_CR_SER;
	FLASH->CR |= FLASH_CR_STRT;
	while (FLASH->SR & FLASH_SR_BSY);
	FLASH->CR &= ~FLASH_CR_SER;
}

//------------------------------------------------------------------------------
// �������� ������� 3 � ������������ ��������� (PSIZE = x32)
//------------------------------------------------------------------------------
void Flash_Sector3_Erase(void)
{
	while (FLASH->SR & FLASH_SR_BSY);
	FLASH->CR = ((uint32_t)0x0218);
	FLASH->CR |= FLASH_CR_SER;
	FLASH->CR |= FLASH_CR_STRT;
	while (FLASH->SR & FLASH_SR_BSY);
	FLASH->CR &= ~FLASH_CR_SER;
}

//------------------------------------------------------------------------------
// ���������������� 1 ����� (PSIZE = 0 � SNB ������ ���� ����������� �� ����)
//------------------------------------------------------------------------------
void Flash_Prog_Byte(uint32_t address, uint8_t data)
{
	uint8_t *eedata;
	while (FLASH->SR & FLASH_SR_BSY);
	FLASH->CR |= FLASH_CR_PG;
	eedata = (uint8_t*)address;
	*eedata = data;
	while (FLASH->SR & FLASH_SR_BSY);
	FLASH->CR &= ~FLASH_CR_PG;
}

//------------------------------------------------------------------------------
// ���������������� N ���� (PSIZE = 0 � SNB ������ ���� ����������� �� ����)
//------------------------------------------------------------------------------
void Flash_Prog_NBytes(uint8_t* pDataFlash, uint8_t *pDataRAM, uint32_t NBytes)
{
	while (FLASH->SR & FLASH_SR_BSY);
	FLASH->CR |= FLASH_CR_PG;
	while (NBytes)
	{
		*pDataFlash = *pDataRAM;
		while (FLASH->SR & FLASH_SR_BSY);
		pDataFlash++;
		pDataRAM++;
		NBytes--;
	}
	FLASH->CR &= ~FLASH_CR_PG;
}

//------------------------------------------------------------------------------
// ���������������� 1 ��������� (PSIZE = 1 � SNB ������ ���� ����������� �� ����)
//------------------------------------------------------------------------------
void Flash_Prog_HWord(uint32_t address, uint16_t data)
{
	uint16_t *eedata;
	while (FLASH->SR & FLASH_SR_BSY);
	FLASH->CR |= FLASH_CR_PG;
	eedata = (uint16_t*)address;
	*eedata = data;
	while (FLASH->SR & FLASH_SR_BSY);
	FLASH->CR &= ~FLASH_CR_PG;
}

//------------------------------------------------------------------------------
// ���������������� N �������� (PSIZE = 1 � SNB ������ ���� ����������� �� ����)
//------------------------------------------------------------------------------
void Flash_Prog_NHWords(uint16_t* pDataFlash, uint16_t *pDataRAM, uint32_t NHWords)
{
	while (FLASH->SR & FLASH_SR_BSY);
	FLASH->CR |= FLASH_CR_PG;
	while (NHWords)
	{
		*pDataFlash = *pDataRAM;
		while (FLASH->SR & FLASH_SR_BSY);
		pDataFlash++;
		pDataRAM++;
		NHWords--;
	}
	FLASH->CR &= ~FLASH_CR_PG;
}

//------------------------------------------------------------------------------
// ���������������� 1 ����� (PSIZE = 2 � SNB ������ ���� ����������� �� ����)
//------------------------------------------------------------------------------
void Flash_Prog_Word(uint32_t address, uint32_t data)
{
	uint32_t *eedata;

	while (FLASH->SR & FLASH_SR_BSY);
	FLASH->CR |= FLASH_CR_PG;
	eedata =(uint32_t*)address;
	*eedata = data;
	while (FLASH->SR & FLASH_SR_BSY);
	FLASH->CR &= ~FLASH_CR_PG;
}

//------------------------------------------------------------------------------
// ���������������� N ���� (PSIZE = 2 � SNB ������ ���� ����������� �� ����)
//------------------------------------------------------------------------------
void Flash_Prog_NWords(uint32_t* pDataFlash, uint32_t *pDataRAM, uint32_t NWords)
{
	while (FLASH->SR & FLASH_SR_BSY);
	FLASH->CR |= FLASH_CR_PG;
	while (NWords)
	{
		*pDataFlash = *pDataRAM;
		while (FLASH->SR & FLASH_SR_BSY);
		pDataFlash++;
		pDataRAM++;
		NWords--;
	}
	FLASH->CR &= ~FLASH_CR_PG;
}

//------------------------------------------------------------------------------
// �������� ��������� sPVSConfig �� ����
// ����������:	0 - ���������� ������
//				4 - ��� ������ (������ ������ �� ����)
//------------------------------------------------------------------------------
uint8_t sPVSConfigWtoF(void)
{
	uint32_t wCnt;
	uint16_t *pDataFlash16, *pDataRAM16;

// �������� �� ������������� ������
	pDataFlash16 = (uint16_t*)&cPVSConfig;

	while (*pDataFlash16 != 0xFFFF)
	{
		pDataFlash16 += PVS_CONFIG_HWORDS;
		if ((uint32_t)pDataFlash16 >= FLASH_SECTOR_4)
			break;
	}

	pDataFlash16 -= PVS_CONFIG_HWORDS;
	pDataRAM16 = (uint16_t*)&sPVSConfig;
	
	wCnt = PVS_CONFIG_HWORDS;
	while (wCnt)
	{
		if (*pDataFlash16 == *pDataRAM16)
		{
			if (--wCnt == 0)
			{
				if (!(sPVSState.bConfig || sPVSState.bRS485))
					return 0;						// ��� ������������� � ������
			}
		}
		else
		{
			if (pDataRAM16 == (uint16_t*)&sPVSConfig)
				sPVSState.bRS485 = 1;				// ���������� ��������� ���������� RS485
			else if (pDataRAM16 != (uint16_t*)&sPVSConfig.CRC16)
				sPVSState.bConfig = 1;				// ���������� ��������� ������������
			wCnt--;
		}
		pDataFlash16++;
		pDataRAM16++;
	}
// ������ �� ����	
	if (((uint32_t)(pDataFlash16 + PVS_CONFIG_HWORDS)) >= FLASH_SECTOR_4)
	{
		Flash_Sector3_Erase();
		pDataFlash16 = (uint16_t*)FLASH_SECTOR_3;
	}
	pDataRAM16 = (uint16_t*)&sPVSConfig;

	Flash_Unlock();
	Flash_PSIZE_SNB((uint32_t)1, (uint32_t)3);		// PSIZE = 1 (x16), SNB = 3
	Flash_Prog_NHWords(pDataFlash16, pDataRAM16, PVS_CONFIG_HWORDS);
	Flash_Lock();
// �������� ������ �� ����
	for (int i = 0; i < PVS_CONFIG_HWORDS; i++)
	{
		if (*pDataRAM16 != *pDataFlash16)
			return 0x04;							// ������ ������ �� ����
		pDataRAM16++;
		pDataFlash16++;
	}
	return 0;										// ���������� ������ �� ����
}

//------------------------------------------------------------------------------
// Unlock Option Bytes
//------------------------------------------------------------------------------
void Flash_OptUnlock(void)
{
	if (FLASH->OPTCR & FLASH_OPTCR_OPTLOCK)
	{
		FLASH->OPTKEYR = OPTKEY1;
		FLASH->OPTKEYR = OPTKEY2;
	}
}

//------------------------------------------------------------------------------
// Lock Option Bytes
//------------------------------------------------------------------------------
void Flash_OptLock(void)
{
	FLASH->OPTCR |= FLASH_OPTCR_OPTLOCK;
}

//------------------------------------------------------------------------------
// ��������� ������ (������ �� ������ 0...2 ����� � ������� 1 ������ �� ������)
//------------------------------------------------------------------------------
void Flash_Guard(void)
{
	Flash_OptUnlock();
	while (FLASH->SR & FLASH_SR_BSY);
	FLASH->OPTCR = 0x0FF800EC;			// ������� 0...2 - nWRP, RDP Lev.1 (0x00)
	FLASH->OPTCR |= FLASH_OPTCR_OPTSTRT;
	while (FLASH->SR & FLASH_SR_BSY);
	Flash_OptLock();
}
