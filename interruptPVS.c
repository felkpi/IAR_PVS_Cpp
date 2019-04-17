/*******************************************************************************
 * @file    SysTick/stm32f4xx_it.h
 * @author  MCD Application Team (modified 23/04/2015 A.Vlashchuk)
 * @version V1.0.0
 * @date    19-September-2011
 * @brief   This file contains the headers of the interrupt handlers.
 *******************************************************************************/
#include "interruptPVS.h"

#define	AD7689CFG	((uint16_t)0xE124)
//----------------------------------------------------------------------------------------------------------------
//  15  |  14  |  13  |  12  |  11  |  10  |   9  |   8  |   7  |   6  |   5  |   4  |   3  |   2  |   1  |   0  |
//----------------------------------------------------------------------------------------------------------------
// CFG  |INCC2 |INCC1 |INCC0 | IN2  | IN1  | IN0  |  BW  | REF2 | REF1 | REF0 | SEQ1 | SEQ0 |  RB  |   X  |   X  |
//----------------------------------------------------------------------------------------------------------------
//   1  |   1  |   1  |   0  |   0  |   0  |   0  |   1  |   0  |   0  |   1  |   0  |   0  |   1  |   0  |   0  |
//----------------------------------------------------------------------------------------------------------------

/*******************************************************************************
 * Function Name: SysTick_Handler
 * --------------------------------
 * Прерывания каждые 10мкс или 5мкс
 *******************************************************************************/
void SysTick_Handler(void)
{
	uint16_t usData;										// данные ADC или DAC
	static uint32_t uiCount = 10;							// счетчик прерываний

	if (uiCount & 1)
	{
		GPIOB->BSRRH = ADC_CNV_PIN;							// Сброс пина ADC
		GPIOA->BSRRL = DAC_SYNC_PIN;						// Установка пина DAC
		if (SPI1->SR & SPI_SR_TXE)
			SPI1->DR = AD7689CFG;							// Запись в ADC конфигурационного слова
	}
	else
	{
		GPIOB->BSRRL = ADC_CNV_PIN;							// Установка пина ADC
		GPIOA->BSRRH = DAC_SYNC_PIN;						// Сброс пина DAC
		if (SPI1->SR & SPI_SR_RXNE)
			usData = SPI1->DR;								// Чтение данных с ADC

		// Запись в кольцевой буфер АЦП
		usBuf_AD7689[wr_index_AD7689] = usData;
		if (++wr_index_AD7689 == BUF_AD7689_SIZE)
			wr_index_AD7689 = 0;
//		pBBFlags->bAD7689 = 1;								// данные в буфере АЦП готовы

		// Чтение из кольцевого буфера ЦАП
//		if (pBBFlags->bAD5662)
		if (rd_index_AD5662 != wr_index_AD5662)
		{
//			pBBFlags->bAD5662 = 0;
			usData = usBuf_AD5662[rd_index_AD5662];
			if (++rd_index_AD5662 == BUF_AD5662_SIZE)
				rd_index_AD5662 = 0;
			// загрузка DMA для вывода данных в ЦАП
			usBufDMA_SPI3[0] = usData >> 8;
			usBufDMA_SPI3[1] = usData << 8;
			while (DMA1_Stream7->CR & DMA_SxCR_EN);
			DMA1->HIFCR = (DMA_HIFCR_CTCIF7 | DMA_HIFCR_CHTIF7 | DMA_HIFCR_CTEIF7 | DMA_HIFCR_CDMEIF7 | DMA_HIFCR_CFEIF7);
			DMA1_Stream7->NDTR = 2;
			DMA1_Stream7->CR |= DMA_SxCR_EN;				// Разрешение DMA
		}
		
		// отработка таймеров
		if (uiTimer1s)
			uiTimer1s--;
		if (uiTimer60s)
			uiTimer60s--;
	}

	if (!(--uiCount))										// деление на 10 или 20 в зависимости от 100 или 200кГц прерываний
	{
		if (sPVSConfig.bPVS == 1)
			uiCount = 20;
		else
			uiCount = 10;
		pBBFlags->b100uS = 1;								// установить флаг 100мкс
		if (counter_T1_5)
			counter_T1_5--;
//		if (counter_T2_5)
//			counter_T2_5--;
		if (counter_T3_5)
			counter_T3_5--;
//		if (counter_T4_5)
//			counter_T4_5--;
	}
}

/*******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                          */
/*******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/******************* (C) COPYRIGHT 2014 NAEK Energoatom *****END OF FILE****/
