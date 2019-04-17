/***************************************************************************
 *	Хедеры для п/п инициализации аппаратуры платы PVS
 ***************************************************************************/
#ifndef __HARDWAREPVS_H
#define __HARDWAREPVS_H

#include "PVS.h"
#include "GlobalPVS.h"
#include "ModBus.h"
#include "FiltersButt.h"
#include "FiltersEtc.h"

// Определения для GPIOA
#define	PA4_PIN							((uint16_t)0x0010)
#define	DAC1_PIN						PA4_PIN
#define	PA15_PIN						((uint16_t)0x8000)
#define	TIM1_CH1_PIN					PA15_PIN
#define	TIM2_CH1_PIN					PA15_PIN
#define	DAC_SYNC_PIN					PA15_PIN

// Определения для GPIOB
#define	PB0_PIN							((uint16_t)0x0001)
#define	TIM3_CH3_PIN					PB0_PIN
#define	ADC_CNV_PIN						PB0_PIN
#define	PB3_PIN							((uint16_t)0x0008)
#define	SWO_PIN							PB3_PIN
#define	PB4_PIN							((uint16_t)0x0010)
#define HL2_RED_PIN						PB4_PIN
#define	PB5_PIN							((uint16_t)0x0020)
#define	S1_PIN							PB5_PIN
#define	PB6_PIN							((uint16_t)0x0040)
#define	HL2_GREEN_PIN					PB6_PIN
#define	PB7_PIN							((uint16_t)0x0080)
#define HL1_PIN							PB7_PIN
#define	PB11_PIN						((uint16_t)0x0800)
#define WP_PIN							PB11_PIN

// Определения для GPIOC
#define	PC9_PIN							((uint16_t)0x0200)
#define	PC11_PIN						((uint16_t)0x0800)
#define	CLEAR_AD5749_PIN				PC11_PIN

void	LedHL1_On(void);				// включение желтого светодиода HL1 (RS485)
void	LedHL1_Off(void);				// выключение желтого светодиода HL1 (RS485)
void	LedHL2_Red_On(void);			// включение красного светодиода HL2 (НЕИСПРАВНОСТЬ)
void	LedHL2_Red_Off(void);			// выключение красного светодиода HL2 (НЕИСПРАВНОСТЬ)
void	LedHL2_Green_On(void);			// включение зеленого светодиода HL2 (ВКЛ)
void	LedHL2_Green_Off(void);			// выключение зеленого светодиода HL2 (ВКЛ)
void	ClearAD5749_On(void);			// установка на ноге Clear AD5749 = 1 (5В)
void	ClearAD5749_Off(void);			// установка на ноге Clear AD5749 = 0 (0В)
void	SetPC9(void);					// PC9 = 1;
void	ResetPC9(void);					// PC9 = 0;
void	ConvertData(void);				// функция обработки данных
void	CalcResult(void);				// подпрограмма вычисления результата
void	GoADC(void);
void	sbPVSControl(void);				// контроль работоспособности

extern	volatile uint32_t uiTimer1s, uiTimer60s;	// таймеры для измерения 1с, 60с с дискретой 20мкс

#endif /* __HARDWAREPVS_H */
