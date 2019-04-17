/***************************************************************************
 *	������ ��� �/� ������������� ���������� ����� PVS
 ***************************************************************************/
#ifndef __HARDWAREPVS_H
#define __HARDWAREPVS_H

#include "PVS.h"
#include "GlobalPVS.h"
#include "ModBus.h"
#include "FiltersButt.h"
#include "FiltersEtc.h"

// ����������� ��� GPIOA
#define	PA4_PIN							((uint16_t)0x0010)
#define	DAC1_PIN						PA4_PIN
#define	PA15_PIN						((uint16_t)0x8000)
#define	TIM1_CH1_PIN					PA15_PIN
#define	TIM2_CH1_PIN					PA15_PIN
#define	DAC_SYNC_PIN					PA15_PIN

// ����������� ��� GPIOB
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

// ����������� ��� GPIOC
#define	PC9_PIN							((uint16_t)0x0200)
#define	PC11_PIN						((uint16_t)0x0800)
#define	CLEAR_AD5749_PIN				PC11_PIN

void	LedHL1_On(void);				// ��������� ������� ���������� HL1 (RS485)
void	LedHL1_Off(void);				// ���������� ������� ���������� HL1 (RS485)
void	LedHL2_Red_On(void);			// ��������� �������� ���������� HL2 (�������������)
void	LedHL2_Red_Off(void);			// ���������� �������� ���������� HL2 (�������������)
void	LedHL2_Green_On(void);			// ��������� �������� ���������� HL2 (���)
void	LedHL2_Green_Off(void);			// ���������� �������� ���������� HL2 (���)
void	ClearAD5749_On(void);			// ��������� �� ���� Clear AD5749 = 1 (5�)
void	ClearAD5749_Off(void);			// ��������� �� ���� Clear AD5749 = 0 (0�)
void	SetPC9(void);					// PC9 = 1;
void	ResetPC9(void);					// PC9 = 0;
void	ConvertData(void);				// ������� ��������� ������
void	CalcResult(void);				// ������������ ���������� ����������
void	GoADC(void);
void	sbPVSControl(void);				// �������� �����������������

extern	volatile uint32_t uiTimer1s, uiTimer60s;	// ������� ��� ��������� 1�, 60� � ��������� 20���

#endif /* __HARDWAREPVS_H */
