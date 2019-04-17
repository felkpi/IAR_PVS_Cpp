/***************************************************************************
 *	Хедеры для п/п инициализации аппаратуры платы PVS
 ***************************************************************************/
#ifndef __INITPVS_H
#define __INITPVS_H

#include "STM32f4xx.h"

#define	GPIOA_MODER_MASK	0x3C000000	// маска для сброса GPIOA_MODER
#define	GPIOA_MODER			0x4004ABFC	// кофигурация GPIOA_MODER
// PA15=1 (OUT), PA14=0 (SWDCLK), PA13=0 (SWDIO), PA12=0 (IN)
// PA11=0 (IN), PA10=0 (IN), PA9=1 (OUT), PA8=0 (IN)
// PA7=2 (SPI1_MOSI), PA6=2 (SPI1_MISO), PA5=2 (SPI1_SCK), PA4=3 (DAC)
// PA3=3 (ADC_IN3), PA2=3 (ADC_IN2), PA1=3 (ADC_IN1), PA0=0 (IN)
#define	GPIOA_OTYPER_MASK	0x6000		// маска для сброса GPIOA_OTYPER
#define	GPIOA_OTYPER		0x0000		// кофигурация GPIOA_OTYPER
// PA15=0 (PP), PA14=0 (PP), PA13=0 (PP), PA12=0 (PP)
// PA11=0 (PP), PA10=0 (PP), PA9=0 (PP), PA8=0 (PP)
// PA7=0 (PP), PA6=0 (PP), PA5=0 (PP), PA4=0 (PP)
// PA3=0 (PP), PA2=0 (PP), PA1=0 (PP), PA0=0 (PP)
#define	GPIOA_OSPEEDR_MASK	0x3C000000	// маска для сброса GPIOA_OSPEEDR
#define	GPIOA_OSPEEDR		0x80008800	// кофигурация GPIOA_OSPEEDR
// PA15=2 (50MHz), PA14=0 (x), PA13=0 (x), PA12=0 (x)
// PA11=0 (x), PA10=0 (x), PA9=0 (2MHz), PA8=0 (x)
// PA7=2 (50MHz), PA6=0 (x), PA5=2 (50MHz), PA4=0 (x)
// PA3=0 (x), PA2=0 (x), PA1=0 (x), PA0=0 (x)
#define	GPIOA_PUPDR_MASK	0x3C000000	// маска для сброса GPIOA_PUPDR
#define	GPIOA_PUPDR			0x00020002	// кофигурация GPIOA_PUPDR
// PA15=0 (x), PA14=0 (x), PA13=0 (x), PA12=0 (x)
// PA11=0 (x), PA10=0 (x), PA9=0 (x), PA8=2 (PD)
// PA7=0 (x), PA6=0 (x), PA5=0 (x), PA4=0 (x)
// PA3=0 (x), PA2=0 (x), PA1=0 (x), PA0=2 (PD)
#define	GPIOA_AFRL			0x55500000	// кофигурация GPIOA_AFRL
// PA7=5 (SPI1_MOSI), PA6=5 (SPI1_MISO), PA5=5 (SPI1_SCK), PA4=0 (x)
// PA3=0 (x), PA2=0 (x), PA1=0 (x), PA0=0 (x)
#define	GPIOA_AFRH			0x00000000	// кофигурация GPIOA_AFRH
// PA15=0 (x), PA14=0 (x), PA13=0 (x), PA12=0 (x)
// PA11=0 (x), PA10=0 (x), PA9=0 (x), PA8=0 (x)

#define	GPIOB_MODER_MASK	0x000000C0	// маска для сброса GPIOB_MODER
#define	GPIOB_MODER			0xAA605101	// кофигурация GPIOB_MODER
// PB15=2 (SPI2_MOSI), PB14=2 (SPI2_MISO), PB13=2 (SPI2_SCK), PB12=2 (SPI2_NSS)
// PB11=1 (OUT), PB10=2 (TIM2_CH3), PB9=0 (IN), PB8=0 (IN)
// PB7=1 (OUT), PB6=1 (OUT), PB5=0 (IN), PB4=1 (OUT)
// PB3=0 (SWO), PB2=0 (IN), PB1=0 (IN), PB0=1 (OUT)
#define	GPIOB_OTYPER_MASK	0x0008		// маска для сброса GPIOB_OTYPER
#define	GPIOB_OTYPER		0x0000		// кофигурация GPIOB_OTYPER
// PB15=0 (PP), PB14=0 (PP), PB13=0 (PP), PB12=0 (PP)
// PB11=0 (PP), PB10=0 (PP), PB9=0 (PP), PB8=0 (PP)
// PB7=0 (PP), PB6=0 (PP), PB5=0 (PP), PB4=0 (PP)
// PB3=0 (PP), PB2=0 (PP), PB1=0 (PP), PB0=0 (PP)
#define	GPIOB_OSPEEDR_MASK	0x000000C0	// маска для сброса GPIOB_OSPEEDR
#define	GPIOB_OSPEEDR		0x4540A202	// кофигурация GPIOB_OSPEEDR
// PB15=1 (25MHz), PB14=0 (x), PB13=1 (25MHz), PB12=1 (25MHz)
// PB11=1 (25MHz), PB10=0 (x), PB9=0 (2MHz), PB8=0 (x)
// PB7=2 (50MHz), PB6=2 (50MHz), PB5=0 (x), PB4=2 (50MHz)
// PB3=0 (x), PB2=0 (x), PB1=0 (x), PB0=2 (50MHz)
#define	GPIOB_PUPDR_MASK	0x000000C0	// маска для сброса GPIOB_PUPDR
#define	GPIOB_PUPDR			0x000A0000	// кофигурация GPIOB_PUPDR
// PB15=0 (x), PB14=0 (x), PB13=0 (x), PB12=0 (x)
// PB11=0 (x), PB10=0 (x), PB9=2 (PD), PB8=2 (PD)
// PB7=0 (x), PB6=0 (x), PB5=0 (x), PB4=0 (x)
// PB3=0 (x), PB2=0 (x), PB1=0 (x), PB0=0 (x)
#define	GPIOB_AFRL			0x00000000	// кофигурация GPIOB_AFRL
// PB7=0 (x), PB6=0 (x), PB5=0 (x), PB4=0 (x)
// PB3=0 (x), PB2=0 (x), PB1=0 (x), PB0=0 (x)
#define	GPIOB_AFRH			0x55550100	// кофигурация GPIOB_AFRH
// PB15=5 (SPI2_MOSI), PB14=5 (SPI2_MISO), PB13=5 (SPI2_SCK), PB12=5 (SPI2_NSS)
// PB11=0 (x), PB10=1 (TIM2_CH3), PB9=0 (x), PB8=0 (x)

#define	GPIOC_MODER			0x0264003C	// кофигурация GPIOC_MODER
// PC15=0 (IN), PC14=0 (IN), PC13=0 (IN), PC12=2 (SPI3_MOSI)
// PC11=1 (OUT), PC10=2 (SPI3_SCK), PC9=1 (OUT), PC8=0 (IN)
// PC7=0 (IN), PC6=0 (IN), PC5=0 (IN), PC4=0 (IN)
// PC3=0 (IN), PC2=3 (Analog), PC1=3 (Analog), PC0=0 (IN)
#define	GPIOC_OTYPER		0x0000		// кофигурация GPIOC_OTYPER
// PC15=0 (PP), PC14=0 (PP), PC13=0 (PP), PC12=0 (PP)
// PC11=0 (PP), PC10=0 (PP), PC9=0 (PP), PC8=0 (PP)
// PC7=0 (PP), PC6=0 (PP), PC5=0 (PP), PC4=0 (PP)
// PC3=0 (PP), PC2=0 (PP), PC1=0 (PP), PC0=0 (PP)
#define	GPIOC_OSPEEDR		0x02A00000	// кофигурация GPIOC_OSPEEDR
// PC15=0 (x), PC14=0 (x), PC13=0 (x), PC12=2 (50MHz)
// PC11=2 (50MHz), PC10=2 (50MHz), PC9=0 (x), PC8=0 (x)
// PC7=0 (x), PC6=0 (x), PC5=0 (x), PC4=0 (x)
// PC3=0 (x), PC2=0 (x), PC1=0 (x), PC0=0 (x)
#define	GPIOC_PUPDR			0xA802AA82	// кофигурация GPIOC_PUPDR
// PC15=2 (PD), PC14=2 (PD), PC13=2 (PD), PC12=0 (x)
// PC11=0 (x), PC10=0 (x), PC9=0 (x), PC8=2 (PD)
// PC7=2 (PD), PC6=2 (PD), PC5=2 (PD), PC4=2 (PD)
// PC3=2 (PD), PC2=0 (x), PC1=0 (x), PC0=2 (PD)
#define	GPIOC_AFRL			0x00000000	// кофигурация GPIOC_AFRL
// PC7=0 (x), PC6=0 (x), PC5=0 (x), PC4=0 (x)
// PC3=0 (x), PC2=0 (x), PC1=0 (x), PC0=0 (x)
#define	GPIOC_AFRH			0x00060600	// кофигурация GPIOC_AFRH
// PC15=0 (x), PC14=0 (x), PC13=0 (x), PC12=6 (SPI3_MOSI)
// PC11=0 (x), PC10=6 (SPI3_SCK), PC0=3 (x), PC8=0 (x)

#define	GPIOD_MODER			0x00006800	// кофигурация GPIOD_MODER
// PD15=0 (IN), PD14=0 (IN), PD13=0 (IN), PD12=0 (IN)
// PD11=0 (IN), PD10=0 (IN), PD9=0 (IN), PD8=0 (IN)
// PD7=1 (OUT), PD6=2 (USART2_RX), PD5=2 (USART2_TX), PD4=0 (IN)
// PD3=0 (IN), PD2=0 (IN), PD1=0 (IN), PD0=0 (IN)
#define	GPIOD_OTYPER		0x0000		// кофигурация GPIOD_OTYPER
// PD15=0 (PP), PD14=0 (PP), PD13=0 (PP), PD12=0 (PP)
// PD11=0 (PP), PD10=0 (PP), PD9=0 (PP), PD8=0 (PP)
// PD7=0 (PP), PD6=0 (PP), PD5=0 (PP), PD4=0 (PP)
// PD3=0 (PP), PD2=0 (PP), PD1=0 (PP), PD0=0 (PP)
#define	GPIOD_OSPEEDR		0x00004400	// кофигурация GPIOD_OSPEEDR
// PD15=0 (x), PD14=0 (x), PD13=0 (x), PD12=0 (x)
// PD11=0 (x), PD10=0 (x), PD9=0 (x), PD8=0 (x)
// PD7=1 (25MHz), PD6=0 (x), PD5=1 (25MHz), PD4=0 (x)
// PD3=0 (x), PD2=0 (x), PD1=0 (x), PD0=0 (x)
#define	GPIOD_PUPDR			0xAAAA02A0	// кофигурация GPIOD_PUPDR
// PD15=2 (PD), PD14=2 (PD), PD13=2 (PD), PD12=2 (PD)
// PD11=2 (PD), PD10=2 (PD), PD9=2 (PD), PD8=2 (PD)
// PD7=0 (x), PD6=0 (x), PD5=0 (x), PD4=0 (x)
// PD3=0 (x), PD2=0 (x), PD1=0 (x), PD0=0 (x)
#define	GPIOD_AFRL			0x07700000	// кофигурация GPIOD_AFRL
// PD7=0 (x), PD6=7 (USART2_RX), PD5=7 (USART2_TX), PD4=0 (x)
// PD3=0 (x), PD2=0 (x), PD1=0 (x), PD0=0 (x)
#define	GPIOD_AFRH			0x00000000	// кофигурация GPIOD_AFRH
// PD15=0 (x), PD14=0 (x), PD13=0 (x), PD12=0 (x)
// PD11=0 (x), PD10=0 (x), PD9=0 (x), PD8=0 (x)

#define	GPIOE_MODER			0x00000010	// кофигурация GPIOE_MODER
// PE15=0 (IN), PE14=0 (IN), PE13=0 (IN), PE12=0 (IN)
// PE11=0 (IN), PE10=0 (IN), PE9=0 (IN), PE8=0 (IN)
// PE7=0 (IN), PE6=0 (IN), PE5=0 (IN), PE4=1 (OUT)
// PE3=0 (IN), PE2=0 (IN), PE1=0 (IN), PE0=0 (IN)
#define	GPIOE_OTYPER		0x0000		// кофигурация GPIOE_OTYPER
// PE15=0 (PP), PE14=0 (PP), PE13=0 (PP), PE12=0 (PP)
// PE11=0 (PP), PE10=0 (PP), PE9=0 (PP), PE8=0 (PP)
// PE7=0 (PP), PE6=0 (PP), PE5=0 (PP), PE4=0 (PP)
// PE3=0 (PP), PE2=0 (PP), PE1=0 (PP), PE0=0 (PP)
#define	GPIOE_OSPEEDR		0x00000800	// кофигурация GPIOE_OSPEEDR
// PE15=0 (x), PE14=0 (x), PE13=0 (x), PE12=0 (x)
// PE11=0 (x), PE10=0 (x), PE9=0 (x), PE8=0 (x)
// PE7=0 (x), PE6=0 (x), PE5=2 (50MHz), PE4=0 (x)
// PE3=0 (x), PE2=0 (x), PE1=0 (x), PE0=0 (x)
#define	GPIOE_PUPDR			0x2AAAA2AA	// кофигурация GPIOE_PUPDR
// PE15=0 (x), PE14=2 (PD), PE13=2 (PD), PE12=2 (PD)
// PE11=2 (PD), PE10=2 (PD), PE9=2 (PD), PE8=2 (PD)
// PE7=2 (PD), PE6=2 (PD), PE5=0 (x), PE4=2 (PD)
// PE3=2 (PD), PE2=2 (PD), PE1=2 (PD), PE0=2 (PD)
#define	GPIOE_AFRL			0x00000000	// кофигурация GPIOE_AFRL
// PE7=0 (x), PE6=0 (x), PE5=0 (x), PE4=0 (x)
// PE3=0 (x), PE2=0 (x), PE1=0 (x), PE0=0 (x)
#define	GPIOE_AFRH			0x00000000	// кофигурация GPIOE_AFRH
// PE15=0 (x), PE14=0 (x), PE13=0 (x), PE12=0 (x)
// PE11=0 (x), PE10=0 (x), PE9=0 (x), PE8=0 (x)

void InitGPIO(void);
void InitCRC(void);
void InitDAC(void);
void InitADC1(void);
void InitTIM2(void);
void InitTIM6(void);

#define	SPI1_CR1_NOT_SPE	((uint16_t)0x0B1C)
//----------------------------------------------------------------------------------------------------------------
//  15  |  14  |  13  |  12  |  11  |  10  |   9  |   8  |   7  |   6  |   5  |   4  |   3  |   2  |   1  |   0  |
//----------------------------------------------------------------------------------------------------------------
// BIDI | BIDI | CRC  | CRC  | DFF  | RX   | SSM  | SSI  | LSB  | SPE  |       BR[2:0]      | MSTR | CPOL | CPHA |
// MODE |  OE  | EN   | NEXT |      | ONLY |      |      | FIRST|      |      (fclk/16)     |      |      |      |
//----------------------------------------------------------------------------------------------------------------
//   0  |   0  |   0  |   0  |   1  |   0  |   1  |   1  |   0  |   0  |   0  |   1  |   1  |   1  |   0  |   0  |
//----------------------------------------------------------------------------------------------------------------
#define	SPI1_CR2			((uint16_t)0x0000)
//-----------------------------------------------------------------------------------------------------------------
//  15  |  14  |  13  |  12  |  11  |  10  |   9  |   8  |   7  |   6  |   5  |   4  |   3  |   2  |   1   |   0   |
//------------------------------------------------------------------------------------------------------------------
//                 Reserved                              | TXEIE|RXNEIE| ERRIE| FRF  | Res  | SSOE |TXDMAEN|RXDMAEN|
//                                                       |      |      |      |      |      |      |       |       |
//------------------------------------------------------------------------------------------------------------------
//   0  |   0  |   0  |   0  |   0  |   0  |   0  |   0  |   0  |   0  |   0  |   0  |   0  |   0  |   0   |   0   |
//------------------------------------------------------------------------------------------------------------------
void	InitSPI1(void);

#define	SPI3_CR1_NOT_SPE	((uint16_t)0x0B0D)
//----------------------------------------------------------------------------------------------------------------
//  15  |  14  |  13  |  12  |  11  |  10  |   9  |   8  |   7  |   6  |   5  |   4  |   3  |   2  |   1  |   0  |
//----------------------------------------------------------------------------------------------------------------
// BIDI | BIDI | CRC  | CRC  | DFF  | RX   | SSM  | SSI  | LSB  | SPE  |       BR[2:0]      | MSTR | CPOL | CPHA |
// MODE |  OE  | EN   | NEXT |      | ONLY |      |      | FIRST|      |      (fclk/4)      |      |      |      |
//----------------------------------------------------------------------------------------------------------------
//   0  |   0  |   0  |   0  |   1  |   0  |   1  |   1  |   0  |   0  |   0  |   0  |   1  |   1  |   0  |   1  |
//----------------------------------------------------------------------------------------------------------------
#define	SPI3_CR2			((uint16_t)0x0002)
//-----------------------------------------------------------------------------------------------------------------
//  15  |  14  |  13  |  12  |  11  |  10  |   9  |   8  |   7  |   6  |   5  |   4  |   3  |   2  |   1   |   0   |
//------------------------------------------------------------------------------------------------------------------
//                 Reserved                              | TXEIE|RXNEIE| ERRIE| FRF  | Res  | SSOE |TXDMAEN|RXDMAEN|
//                                                       |      |      |      |      |      |      |       |       |
//------------------------------------------------------------------------------------------------------------------
//   0  |   0  |   0  |   0  |   0  |   0  |   0  |   0  |   0  |   0  |   0  |   0  |   0  |   0  |   1   |   0   |
//------------------------------------------------------------------------------------------------------------------
void InitSPI3(void);

void InitDMA_SPI3(void);

#define	DMA1_S7CR_NOEN		((uint32_t)0x00022C40)
//----------------------------------------------------------------------------------------------------------------
//  31  |  30  |  29  |  28  |  27  |  26  |  25  |  24  |  23  |  22  |  21  |  20  |  19  |  18  |  17  |  16  |
//----------------------------------------------------------------------------------------------------------------
// RES  | RES  | RES  | RES  |    CHSEL[2:0]      | MBURST[1:0] | PBURST[1:0] | RES  |  CT  | DBM  |   PL[1:0]   |
//----------------------------------------------------------------------------------------------------------------
//   0  |   0  |   0  |   0  |   0  |   0  |   0  |   0  |   0  |   0  |   0  |   0  |   0  |   0  |   1  |   0  |
//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
//  15  |  14  |  13  |  12  |  11  |  10  |   9  |   8  |   7  |   6  |   5  |   4  |   3  |   2  |   1  |   0  |
//----------------------------------------------------------------------------------------------------------------
//PINCOS|  MSIZE[1:0] |  PSIZE[1:0] | MINC | PINC | CIRC |   DIR[1:0]  |PFCTRL| TCIE | HTIE | TEIE |DMEIE |  EN  |
//----------------------------------------------------------------------------------------------------------------
//   0  |   0  |   1  |   0  |   1  |   1  |   0  |   0  |   0  |   1  |   0  |   0  |   0  |   0  |   0  |   0  |
//----------------------------------------------------------------------------------------------------------------

extern uint16_t T1_5, T2_5, T3_5, T4_5;

#define defaultMbBRP 0x04				// Parity - EVEN, BaudRate = 19,2КБод
void InitRS485(void);					// инициализация RS485
void ReInitConfig(void);

#endif /* __INITPVS_H */
