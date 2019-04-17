/***************************************************************************
 *	Хедеры для п/п хенлеров прерываний PVS
 ***************************************************************************/

#ifndef __INTERRUPTPVS_H
#define __INTERRUPTPVS_H

#include "stm32f4xx.h"
#include "PVS.h"
#include "GlobalPVS.h"
#include "hardwarePVS.h"

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

#endif /* __INTERRUPTPVS_H */

/******************* (C) COPYRIGHT 2015 NAEK Energoatom *****END OF FILE****/
