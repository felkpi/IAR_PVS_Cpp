/*******************************************************************************
 *	Хедер для основного модуля PVS
 *
 *
 *
 *******************************************************************************/
#ifndef __PVS_H
#define __PVS_H

#include "STM32f4xx.h"

//==============================================================================
// Определения параметров и макросов
//==============================================================================
#define	BUF_AD7689_SIZE		256			// размер кольцевого буфера АЦП AD7689
#define	BUF_AD5662_SIZE		256			// размер кольцевого буфера ЦАП AD5662

#define	RX_BUF_RS485_SIZE	256			// размер кольцевого буфера чтения с RS485
#define	TX_BUF_RS485_SIZE	256			// размер кольцевого буфера записи в RS485

#define IOUT_1mA	65536/24			// 1 мА
#define IOUT_4mA	65536/6				// 4 мА
#define IOUT_12mA	65536/2				// 12 мА
#define IOUT_16mA	65536*16/24			// 16 мA
#define IOUT_20mA	65536*20/24			// 20 мА
#define IOUT_24mA	65535				// 24 мА

#define FIOUT_1mA	65536.0/24.0
#define FIOUT_4mA	65536.0/6.0
#define FIOUT_12mA	65536.0/2.0
#define FIOUT_16mA	65536.0*16.0/24.0
#define FIOUT_20mA	65536.0*20.0/24.0
#define FIOUT_24mA	65535.0

#define	FKI_2mm		FIOUT_16mA/2000.0	// наклон выходной кривой вых. тока при диапазоне 2мм
#define	FKI_4mm		FIOUT_16mA/4000.0	// наклон выходной кривой вых. тока при диапазоне 4мм
#define	FKI_5mm		FIOUT_16mA/5000.0	// наклон выходной кривой вых. тока при диапазоне 5мм

#define	FKI_125um	FIOUT_16mA/125.0	// наклон выходной кривой вых. тока при диапазоне виброперемещения 10...125мкм
#define	FKI_250um	FIOUT_16mA/250.0	// наклон выходной кривой вых. тока при диапазоне виброперемещения 15...250мкм
#define	FKI_500um	FIOUT_16mA/500.0	// наклон выходной кривой вых. тока при диапазоне виброперемещения 25...500мкм

#define UOUT_2048	65536				// -20,48 В
#define UOUT_1880	60160				// -18,8 В
#define UOUT_1800	57600				// -18,0 В
#define UOUT_1760	56320				// -17,6 В
#define	UOUT_1600	51200				// -16,0 В
#define UOUT_1080	34560				// -10,8 В
#define UOUT_1000	32000				// -10,0 В
#define UOUT_0960	30720				// -9,6 В
#define UOUT_0280	8960				// -2,8 В
#define UOUT_0200	6400				// -2 В
#define UOUT_0160	5120				// -1,6 В

#define	FUOUT_1600	51200.0				// 16,0 В

#define	FKU_2mm		FUOUT_1600/2000.0	// наклон выходной кривой вых. напряжения при диапазоне 2мм
#define	FKU_4mm		FUOUT_1600/4000.0	// наклон выходной кривой вых. напряжения при диапазоне 4мм
#define	FKU_5mm		FUOUT_1600/5000.0	// наклон выходной кривой вых. напряжения при диапазоне 5мм
	 
#define	ALFA	0.00385					// коэффициент температурного сопротивления  платиного датчика температуры
#define	ALFAM1	1.0/ALFA				// 1/ALFA
	 
#define	T1S		100000					// число интервалов 10мкс в 1с
#define	T60S	100000*60				// число интервалов 10мкс в 60с (1мин)
	 
#define	S8		0x80
#define	S8U		0xFF
#define	S16		0x8000
#define	S16U	0xFFFF
#define	S32		0x80000000
#define	S32U	0xFFFFFFFF
	 
#define BYTE0(x) ((uint8_t*)&(x))[0]
#define BYTE1(x) ((uint8_t*)&(x))[1]
#define BYTE2(x) ((uint8_t*)&(x))[2]
#define BYTE3(x) ((uint8_t*)&(x))[3]

#define WORD0(x) ((uint16_t*)&(x))[0]
#define WORD1(x) ((uint16_t*)&(x))[1]

#define LOWBYTE(x) ((uint8_t*)&(x))[0]
#define HIGHBYTE(x) ((uint8_t*)&(x))[1]

#define WORDLE(x) (((uint16_t)(((uint8_t*)&(x))[1])<<8)|(uint16_t)(((uint8_t*)&(x))[0]))	// Little Endian
#define WORDBE(x) (((uint16_t)(((uint8_t*)&(x))[0])<<8)|(uint16_t)(((uint8_t*)&(x))[1]))	// Big Endian

#endif /* __PVS_H */
