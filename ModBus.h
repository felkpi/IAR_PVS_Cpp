/*******************************************************************************
 * file:	ModBus.h
 * author:	A.Vlashchuk
 * version:	V1.0.0
 * date:	26-April-2015
 * brief:	Хедер для ModBus.c
 *******************************************************************************/
#ifndef __MODBUS_H
#define __MODBUS_H

#define BUF_RS485_SIZE 256

#include "STM32f4xx.h"

extern union							// приемный буфер ModBus
{
	uint8_t ucInBufRS485[BUF_RS485_SIZE];
	uint16_t wInBufRS485[BUF_RS485_SIZE/2];
};

extern union							// буфер передачи ModBus
{
	uint8_t ucOutBufRS485[BUF_RS485_SIZE];
	uint16_t wOutBufRS485[BUF_RS485_SIZE/2];
};

extern uint16_t volatile counter_T1_5, counter_T2_5, counter_T3_5, counter_T4_5;

void ProcModBus(void);					// п/п отработки ModBus

#endif /* __MODBUS_H */
