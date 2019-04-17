/*******************************************************************************
 * file:	Flash.h
 * author:	A.Vlashchuk
 * version:	V1.0.0
 * date:	31-July-2015
 * brief:	’едер дл€ Flash.c
 *******************************************************************************/
#ifndef __FLASH_H
#define __FLASH_H

#include "STM32f4xx.h"

#define	KEY1	0x45670123
#define	KEY2	0xCDEF89AB
#define	OPTKEY1	0x08192A3B
#define	OPTKEY2	0x4C5D6E7F

#define	FLASH_SECTOR_0	FLASH_BASE
#define	FLASH_SECTOR_1	(FLASH_BASE + 0x00004000)
#define	FLASH_SECTOR_2	(FLASH_BASE + 0x00008000)
#define	FLASH_SECTOR_3	(FLASH_BASE + 0x0000C000)
#define	FLASH_SECTOR_4	(FLASH_BASE + 0x00010000)
#define	FLASH_SECTOR_5	(FLASH_BASE + 0x00020000)
#define	FLASH_SECTOR_6	(FLASH_BASE + 0x00040000)
#define	FLASH_SECTOR_7	(FLASH_BASE + 0x00060000)

void Flash_Unlock(void);										// unlock Flash
void Flash_Lock(void);											// lock Flash
void Flash_PSIZE_SNB(uint32_t PSize, uint32_t SectorNumber);	// установить параллелизм и номер сектора Flash (сбрасывает все остальное)
//void Flash_Sector_Erase(void);								// стирание сектора (до этого должны быть установлены PSIZE и SNB)
//void Flash_Sector3_Erase(void);								// —тирание сектора 3 с максимальной скоростью, PSIZE = 2 (x32)
//void Flash_Prog_Byte(uint32_t address, uint8_t data);			// программирование байта (адрес должен соответствовать SNB)
//void Flash_Prog_NBytes(uint8_t* pDataFlash, uint8_t *pDataRAM, uint32_t NBytes);	//	ѕрограммирование N байт
//																						(PSIZE = 0 и SNB должно быть установлено до того)
//void Flash_Prog_HWord(uint32_t address, uint16_t data);		// программирование полуслова (PSIZE = 1)
//void Flash_Prog_NHWords(uint8_t* pDataFlash, uint8_t *pDataRAM, uint32_t NHWords);	// ѕрограммирование N полуслов
//																						(PSIZE = 1 и SNB должно быть установлено до того)
void Flash_Prog_Word(uint32_t address, uint32_t data);			// программирование слова (PSIZE = 2)
//void Flash_Prog_NWords(uint8_t* pDataFlash, uint8_t *pDataRAM, uint32_t NHWords);	// ѕрограммирование N слов
//																						(PSIZE = 2 и SNB должно быть установлено до того)
uint8_t sPVSConfigWtoF(void);									// записать структуру sPVSConfig во Flash
void Flash_Guard(void);											// ”становка защиты (защита от записи 0...2 блоки и уровень 1 защиты от чтени€)

#endif /* __FLASH_H */
