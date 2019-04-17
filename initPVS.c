/*************************************************************************
 * ������������� PVS
 * -----------------------
 * void InitGPIO(void) - ������������� (��������� ���������) GPIO
 *************************************************************************/
#include <string.h>
#include "PVS.h"
#include "initPVS.h"
#include "GlobalPVS.h"
#include "ModBus.h"
#include "Safety.h"

/*************************************************************************
 * Function Name: initGPIO
 * Parameters: void
 * Return: void
 *
 * Description: ������������� ��� ����� GPIO
 *
 *************************************************************************/
void InitGPIO(void)
{
	// ������������� GPIOA
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER &= GPIOA_MODER_MASK;
	GPIOA->MODER |= GPIOA_MODER;
	GPIOA->OTYPER &= GPIOA_OTYPER_MASK;
	GPIOA->OTYPER |= GPIOA_OTYPER;
	GPIOA->OSPEEDR &= GPIOA_OSPEEDR_MASK;
	GPIOA->OSPEEDR |= GPIOA_OSPEEDR;
	GPIOA->PUPDR &= GPIOA_PUPDR_MASK;
	GPIOA->PUPDR |= GPIOA_PUPDR;
	GPIOA->AFR[0] |= GPIOA_AFRL;
	GPIOA->AFR[1] |= GPIOA_AFRH;

	// ������������� GPIOB
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	GPIOB->MODER &= GPIOB_MODER_MASK;
	GPIOB->MODER |= GPIOB_MODER;
	GPIOB->OTYPER &= GPIOB_OTYPER_MASK;
	GPIOB->OTYPER |= GPIOB_OTYPER;
	GPIOB->OSPEEDR &= GPIOB_OSPEEDR_MASK;
	GPIOB->OSPEEDR |= GPIOB_OSPEEDR;
	GPIOB->PUPDR &= GPIOB_PUPDR_MASK;
	GPIOB->PUPDR |= GPIOB_PUPDR;
	GPIOB->AFR[0] |= GPIOB_AFRL;
	GPIOB->AFR[1] |= GPIOB_AFRH;

	// ������������� GPIOC
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	GPIOC->MODER = GPIOC_MODER;
	GPIOC->OTYPER = GPIOC_OTYPER;
	GPIOC->OSPEEDR = GPIOC_OSPEEDR;
	GPIOC->PUPDR = GPIOC_PUPDR;
	GPIOC->AFR[0] = GPIOC_AFRL;
	GPIOC->AFR[1] = GPIOC_AFRH;

	// ������������� GPIOD
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD->MODER = GPIOD_MODER;
	GPIOD->OTYPER = GPIOD_OTYPER;
	GPIOD->OSPEEDR = GPIOD_OSPEEDR;
	GPIOD->PUPDR = GPIOD_PUPDR;
	GPIOD->AFR[0] = GPIOD_AFRL;
	GPIOD->AFR[1] = GPIOD_AFRH;

	// ������������� GPIOE
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	GPIOE->MODER = GPIOE_MODER;
	GPIOE->OTYPER = GPIOE_OTYPER;
	GPIOE->OSPEEDR = GPIOE_OSPEEDR;
	GPIOE->PUPDR = GPIOE_PUPDR;
	GPIOE->AFR[0] = GPIOE_AFRL;
	GPIOE->AFR[1] = GPIOE_AFRH;
}

/*************************************************************************
 * Function Name: InitTIM2
 * Parameters: void
 * Return: void
 *
 * Description: �������������� TIM2 (TIM2_CH3 - PB10)
 *
 *************************************************************************/
void InitTIM2(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;		// ���������� ������������ TIM2 (84 MHz)

	TIM2->CR1 = 0;							// CKD=0; ARPE=0; CMS=0; DIR=0; OPM=0; URS=0; UDIS=0; 
	//	TIM2->CR2 = 0;		 					// TI1S=0; MMS=0; CCDS=0; CCUS=0; CCPC=0;
	//	TIM2->SMCR = 0;						// ETP=0; ECE=0; ETPS=0; ETF=0; MSM=0; TS=0; SMS=0;
	//	TIM2->DIER = 0;						// TDE=0;COMDE=0;CC4DE=0;CC3DE=0;CC2DE=0;CC1DE=0;UDE=0;BIE=0;TIE=0;COMIE=0;CC4IE=0;CC3IE=0;CC2IE=0;CC1IE=0;UIE=0;
	//	TIM2->EGR = 0;						// BG=0;TG=0;COMG=0;CC4G=0;CC3G=0;CC2G=0;CC1G=0;UG=0;
	//	TIM2->CCMR1 = 0;					// IC2F=0;IC2PSC=0;CC2S=0;IC1F=0;IC1PSC=0;CC1S=0;
	//	TIM2->RCR = 0;
	TIM2->CCMR2 = TIM_CCMR2_IC3F_1 |TIM_CCMR2_IC3F_0 | TIM_CCMR2_CC3S_0 ;	// IC3F=3;IC3PSC=0;IC3S=1;
	TIM2->CCER = TIM_CCER_CC3E;	 			// CC3P=0;CC3E=1; (Rising Edge)
	TIM2->PSC = 13;							// CK_CNT = 84MHz/14 = 6MHz
	TIM2->ARR = 0xFFFFFFFF;
	TIM2->EGR = TIM_EGR_UG;
	TIM2->CR1 |= TIM_CR1_CEN;				// CEN = 1;
}

/*************************************************************************
 * Function Name: InitTIM6
 * Parameters: void
 * Return: void
 *
 * Description: �������������� Basic Timer TIM6 (��� �������� �����)
 *
 *************************************************************************/
void InitTIM6(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;		// ���������� ������������ TIM6 (84 MHz)

	TIM6->CR1 = 0;							// ARPE=0; OPM=0; URS=0; UDIS=0; CEN = 0
	//	TIM6->CR2 = 0;		 				// MMS=0
	//	TIM2->DIER = 0;						// UDE=0; UIE=0
	//	TIM2->EGR = 0;						// BG=0;TG=0;COMG=0;CC4G=0;CC3G=0;CC2G=0;CC1G=0;UG=0;
	TIM6->PSC = 83;							// CK_CNT = 84MHz/84 = 1 MHz
	TIM6->ARR = 0xFFFF;
	TIM6->EGR = TIM_EGR_UG;
	TIM6->CNT = 0;
	TIM6->CR1 |= TIM_CR1_CEN;				// CEN = 1;
}

/*************************************************************************
 * Function Name: InitCRC
 * Parameters: void
 * Return: void
 *
 * Description: �������������� ������ ���������� CRC
 *
 *************************************************************************/
void InitCRC(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;	// ��������� ������������ CRC
	CRC->CR = 1;						// ������������� ������ CRC (CRC_DR = 0xFFFFFFFF)
	CRC->IDR = 0;
}

/*************************************************************************
 * Function Name: InitDAC
 * Parameters: void
 * Return: void
 *
 * Description: �������������� DAC
 *
 *************************************************************************/
void InitDAC(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_DACEN;
	DAC->CR = DAC_CR_EN1;
	DAC->DHR12R1 = 0;
}

/*************************************************************************
 * Function Name: InitADC1
 * Parameters: void
 * Return: void
 *
 * Description: �������������� ADC1
 *
 *************************************************************************/
void InitADC1(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;			// ����������� �����
	ADC->CCR = ADC_CCR_ADCPRE_1;				// ��������� = PCLK/6 (14 MHz)
	ADC1->SQR3 = ADC_SQR3_SQ1_0;				// ����� ADC1_IN1
	ADC1->CR2 = ADC_CR2_ADON;					// �������� ADC1
	ADC1->CR2 |= ADC_CR2_SWSTART;
	while (!(ADC1->SR & ADC_SR_EOC));
}

/*************************************************************************
 * Function Name: InitSPI
 * Parameters: void
 * Return: void
 *
 * Description: �������������� SPI1, 2, 3
 *
 *************************************************************************/
void InitSPI1(void)
{
	// ������������� SPI1
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;		// ���������� �����
	SPI1->CR1 = SPI1_CR1_NOT_SPE;			// SPE = 0;
	SPI1->CR2 = SPI1_CR2;
	SPI1->CR1 |= SPI_CR1_SPE;				// SPE = 1;
}

/*
void InitSPI2(void)						// ��� serial Flash
{
	// ������������� SPI2
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;		// ���������� �����
	SPI1->CR1 = SPI1_CR1_NOT_SPE;			// SPE = 0;
	SPI1->CR2 = SPI1_CR2;
	SPI1->CR1 |= SPI_CR1_SPE;				// SPE = 1;
}
*/

void InitSPI3(void)
{
	// ������������� SPI3
	RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;		// ���������� �����
	SPI3->CR1 = SPI3_CR1_NOT_SPE;			// SPE = 0;
	SPI3->CR2 = SPI3_CR2;					// ���������� �������� �� DMA
	SPI3->CR1 |= SPI_CR1_SPE;				// SPE = 1;
}

/*************************************************************************
 * Function Name: InitDMA_SPI3
 * Parameters: void
 * Return: void
 *
 * Description: �������������� �������� SPI3 �� DMA
 *
 *************************************************************************/
void InitDMA_SPI3(void)
{
	// ������������� DMA
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;		// ���������� ����� DMA1
	DMA1->HIFCR = (DMA_HIFCR_CTCIF7 | DMA_HIFCR_CHTIF7 | DMA_HIFCR_CTEIF7 | DMA_HIFCR_CDMEIF7 | DMA_HIFCR_CFEIF7);
	DMA1_Stream7->CR = DMA1_S7CR_NOEN;
	while (DMA1_Stream7->CR & DMA_SxCR_EN);
	DMA1_Stream7->PAR = (uint32_t)(&(SPI3->DR));
	DMA1_Stream7->M0AR = (uint32_t)usBufDMA_SPI3;
}

// ������� �������� BRR ���: Fpclk = 22���, OVER = 0 (/16)
// �������� USARTDIV, DIV_Mantissa � DIV_Fraction ��� OVER=0 (/16) � ������� 22���
//-----------------------------------------------------------------
// ��/�	|	#	| BaudRate	| USARTDIV	| DIV_Mantissa	| DIV_Fraction
//-----------------------------------------------------------------
//	1	|	0	|	1200	| 2187,5	| 2187 (0x88B)	| 0,5 (0x8)
//	2	|	1	|	2400	| 1093,75	|  1093 (0x445)	| 0,75 (0xC)
//	3	|	2	|	4800	|  546,875	|  546 (0x222)	| 0,875 (0xE)
//	4	|	3	|	9600	|  273,437	|  273 (0x111)	| 0,437 (0x7)
//	5	|  	4	|	19200	|  136,719	|  136 (0x88)	| 0,719 (0xC)
//	6	|  	5	|	38400	|   68,359	|   68 (0x44)	| 0,359 (0x6)
//	7	|  	6	|	56000	|   46,875	|   46 (0x2E)	| 0,875 (0xE)
//  8	|	-	|	57600	|	45,573	|	45 (0x2D)	| 0,573 (0x9)
//  9	| 	7	|	115200	|   22,786	|   22 (0x16)	| 0,786 (0xD)
//-----------------------------------------------------------------
const uint16_t Table_BRR[] = {0x88B8, 0x445C, 0x222E, 0x1117, 0x088C, 0x0446, 0x2EE, 0x16D};

const uint16_t Table_T1_5[] = {138, 69, 35, 18, 9, 8, 8, 8};		//T1,5 � �������� 100���
const uint16_t Table_T2_5[] = {230, 115, 58, 29, 15, 11, 10, 9};	//T2,5 � �������� 100���
const uint16_t Table_T3_5[] = {320, 160, 80, 40, 20, 18, 18, 18};	//�3,5 � �������� 100���
const uint16_t Table_T4_5[] = {412, 206, 103, 52, 26, 21, 20, 19};	//�4,5 � �������� 100���
uint16_t T1_5, T2_5, T3_5, T4_5;

/*************************************************************************
 * Function Name: InitRS485 (USART2 ��� RS485)
 * Parameters: void
 * Return: void
 *
 * Description: �������������� USART2 �������� ����������
 *				�� ������ �������� 0x000 Modbus
 * - USART2 ��������� �� ���� APB1
 * - ������� ������������ ���� - HCLK/4, �.�. 42MHz
 *************************************************************************/
void InitRS485(void)
{
	uint16_t wTemp;
// �������� ������ ModBus
	if (!sPVSState.bWork)
		sPVSConfig.MbBRP = defaultMbBRP;		// ����� "InitConfig" (19,2����, Parity = EVEN)
// ������������� USART2 (�� �����)
	GPIOD->BSRRH = 0x0080;						// PD7 = 0 (�����)
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;		// ������������ ����������
	USART2->CR1 &= ~(USART_CR1_OVER8 | USART_CR1_UE | USART_CR1_M | USART_CR1_PCE | USART_CR1_RE | USART_CR1_TE);
	USART2->BRR = Table_BRR[sPVSConfig.MbBRP & 0x0F];
	if ((sPVSConfig.MbBRP & 0xF0) == 0)
		wTemp = USART_CR1_PCE;					// Parity = EVEN
	else if ((sPVSConfig.MbBRP & 0xF0) == 0x10)
		wTemp = USART_CR1_PCE | USART_CR1_PS;	// Parity = ODD
	else
		wTemp = 0;								// Parity = NONE
	wTemp |= (USART_CR1_UE | USART_CR1_M);		// USART enable, 9 data bits
	USART2->CR1 = wTemp;
	USART2->CR2 = 0; 		// 1 stop bit (������ ����-��� ��� ���������� �������� ����������� ��� "1" � 9 ���� ������
	USART2->CR1 |= USART_CR1_RE;
// ������������ ���������� ModBus
	T1_5 = Table_T1_5[sPVSConfig.MbBRP & 0x0F];
	T2_5 = Table_T2_5[sPVSConfig.MbBRP & 0x0F];
	T3_5 = Table_T3_5[sPVSConfig.MbBRP & 0x0F];
	T4_5 = Table_T4_5[sPVSConfig.MbBRP & 0x0F];
}

/*******************************************************************************
* Function Name: SysTick_ReConfig
* Parameters: ����� ����� ����� ������������
*******************************************************************************/
uint32_t SysTick_ReConfig(uint32_t ticks)
{
  if ((ticks - 1) > SysTick_LOAD_RELOAD_Msk)  return (1);      // Reload value impossible

  SysTick->LOAD  = ticks - 1;                                  // set reload register
  SysTick->VAL   = 0;                                          // Load the SysTick Counter Value
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                   SysTick_CTRL_TICKINT_Msk   |
                   SysTick_CTRL_ENABLE_Msk;                    // Enable SysTick IRQ and SysTick Timer
  return (0);                                                  // Function successful
}

/*******************************************************************************
 * Function Name: ReInitConfig
 * Parameters: void
 * Return: void
 *
 * Description:	������������� ������, ���������� � ������������� �������
 *				������������ ��� sPVSState.bConfig = 1 �(���) sPVSState.bRS485
 *
 * ������� ��� ���������� ����������� ������� (����������� ����������) �������� ���:
 *	T = N * cfKPt - 1/ALFA,
 *	��� N - ��������� ��� (0...65536)
 *	ALFA = 0.00385 - ����������� �������������� ������������� Pt - ����������
 *	fKPt = cfUref/(iNref*(cfR0+Rc)*cfIpt*ALFA))
 *		cfUref = 3,07455 (����������� �������� �������� ���������� ���)
 *		iNref = 65536 (��� 16-������ ���)
 *		cfR0 = 1000 �� (����������� ������������� ����������� ������� ����������� ��� t = 0 �����)
 *		Rc = cfCableRTD[sPVSConfig.bRTD] (������������� ������ ������� �����������)
 *		cfIt = 1,02145E-3 (����� ����� ������ �����������)
 ******************************************************************************/
void ReInitConfig(void)
{
	const float cfUref = 3.07455;									// ����������� ������� ���������� ���
	const int32_t iNref = 65536;									// ������������� ����������� ��� - 16 ��� (12*4)
	const float cfR0 = 1000.0;										// ����������� ������������� Pt ��� 0 �����
	const float cfCableRTD[16] = {0.0, 0.2, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0};
	const float cfIpt = 1.02145E-3;									// ����� ����� ������ �����������
	
	uint16_t *pcPVSConfig;
	uint16_t wCRC16;
	int16_t shTmp;
	int32_t iTmp;
	float fTmp, fK2;
	
	if (sPVSState.bConfig && (unModBus.bReg == 0))
	{
		SysTick->CTRL = 0UL; 										// ��������� SysTick
		
		sPVSState.usErrPVS = 0;										// �������� ����� ErrPVS
		// ������������ ��������� PVSConfig �� Flash � SRAM
		pcPVSConfig = (uint16_t*)&cPVSConfig;
		while (*pcPVSConfig != 0xFFFF)
		{
			pcPVSConfig += PVS_CONFIG_HWORDS;
			if ((uint32_t)pcPVSConfig >= FLASH_SECTOR_4)
				break;
		}
		pcPVSConfig -= PVS_CONFIG_HWORDS;
		memcpy(&sPVSConfig, pcPVSConfig, PVS_CONFIG_BYTES);
		
		// ��������� ������������ CRC16
		wCRC16 = wCRC16t((uint8_t *)&sPVSConfig, (PVS_CONFIG_BYTES-2));
		if (wCRC16 != sPVSConfig.CRC16)
			sPVSState.bErrFlash = 1;
		CRC16t_CycleVerify();										// ������������� ����������� ��������� � ��������� ���������

		// ��������� ��������� sPVSState
		sPVSState.fumCallibr0 = sPVSConfig.umCallibr0;

		if (sPVSConfig.bNCal == 0)									// 7-�������� ���������� ��� ���������� �����������
		{
			sPVSState.fumCallibr[0] = sPVSConfig.umCallibr[0];
			sPVSState.fvNCallibr[0] = sPVSConfig.vNCallibr[0];
			for (int i=1; i<=6; i++)
			{
				sPVSState.fumCallibr[i] = sPVSConfig.umCallibr[i];
				sPVSState.fvNCallibr[i] = sPVSConfig.vNCallibr[i];
				if (iTmp = (int32_t)sPVSConfig.vNCallibr[i] - (int32_t)sPVSConfig.vNCallibr[i-1])
				{
					fTmp = iTmp;
					sPVSState.fKx[i-1] = (sPVSState.fumCallibr[i] - sPVSState.fumCallibr[i-1])/fTmp;
				}
				else												// �������������� ������ (������� �� "0")
				{
					sPVSState.bErrFlash = 1;
					sPVSState.fKx[i-1] = 0;
				}
			}
		}
		else
			sPVSState.bErrFlash = 1;								// � ����� ������ ��������� �������, ����� 7-�, ���������� �����������
			
		// ���������� ������������� ������������� �� ����������� ������
		if (sPVSConfig.bTMCal == 1)									// 1-� ���������� (�.8) �� ����������� ������
		{
			if (!sPVSState.bErrFlash)
			{
				iTmp = sPVSConfig.TMCallibr - sPVSConfig.TNMCallibr;
				if (iTmp)
				{
					fTmp = iTmp;
					sPVSState.fK1xTM = ((float)(sPVSConfig.vTMCallibr[0] - sPVSConfig.vTCallibr[0]))/fTmp;
				}
				else
				{
					sPVSState.bErrFlash = 1;
					sPVSState.fK1xTM = 0;
				}
			}
		}
		else if (sPVSConfig.bTMCal == 2)							// 2-� ���������� (�.8 � �.9) �� ����������� ������
		{
			if (!sPVSState.bErrFlash)
			{
				iTmp = sPVSConfig.TMCallibr - sPVSConfig.TNMCallibr;
				if (iTmp)
				{
					fTmp = iTmp;
					sPVSState.fK1xTM = (float)(sPVSConfig.vTMCallibr[0] - sPVSConfig.vTCallibr[0])/fTmp;
					fK2 = (float)(sPVSConfig.vTMCallibr[1] - sPVSConfig.vTCallibr[1])/fTmp;
					shTmp = sPVSConfig.vTCallibr[1]-sPVSConfig.vTCallibr[0];
					if (shTmp)
					{
						sPVSState.fK2xTM = (fK2 - sPVSState.fK1xTM)/shTmp;
						sPVSState.fvTCallibr[0] = sPVSConfig.vTCallibr[0];
						sPVSState.fvTCallibr[1] = sPVSConfig.vTCallibr[1];
					}
					else
					{
						sPVSState.bErrFlash = 1;
						sPVSState.fK2xTM = 0;
					}
				}
				else
				{
					sPVSState.bErrFlash = 1;
					sPVSState.fK1xTM = 0;
				}
			}
		}

		// ���������� ������������� ������������� �� ����������� �������
		if (sPVSConfig.bTDCal == 1)									// 1-� ���������� (�.8) �� ����������� �������
		{
			if (!sPVSState.bErrFlash)
			{
				iTmp = sPVSConfig.TDCallibr - sPVSConfig.TNDCallibr;
				if (iTmp)
				{
					fTmp = iTmp;
					sPVSState.fK1xTD = ((float)(sPVSConfig.vTDCallibr[0] - sPVSConfig.vTCallibr[0]))/fTmp;
				}
				else
				{
					sPVSState.bErrFlash = 1;
					sPVSState.fK1xTD = 0;
				}
			}
		}
		else if (sPVSConfig.bTDCal == 2)							// 2-� ���������� (�.8 � �.9) �� ����������� �������
		{
			if (!sPVSState.bErrFlash)
			{
				iTmp = sPVSConfig.TDCallibr - sPVSConfig.TNDCallibr;
				if (iTmp)
				{
					fTmp = iTmp;
					sPVSState.fK1xTD = (float)(sPVSConfig.vTDCallibr[0] - sPVSConfig.vTCallibr[0])/fTmp;
					fK2 = (float)(sPVSConfig.vTDCallibr[1] - sPVSConfig.vTCallibr[1])/fTmp;
					shTmp = sPVSConfig.vTCallibr[1]-sPVSConfig.vTCallibr[0];
					if (shTmp)
					{
						sPVSState.fK2xTD = (fK2 - sPVSState.fK1xTD)/shTmp;
						sPVSState.fvTCallibr[0] = sPVSConfig.vTCallibr[0];
						sPVSState.fvTCallibr[1] = sPVSConfig.vTCallibr[1];
					}
					else
					{
						sPVSState.bErrFlash = 1;
						sPVSState.fK2xTD = 0;
					}
				}
				else
				{
					sPVSState.bErrFlash = 1;
					sPVSState.fK1xTD = 0;
				}
			}
		}
		
		sPVSState.fKPt = cfUref/(iNref*(cfR0+cfCableRTD[sPVSConfig.bRTD])*cfIpt*ALFA);	// ��������� ����������� �������������� Pt
		sPVSState.bConfig = 0;
		if (sPVSConfig.bPVS == 0)									// ���
			iTmp = SystemCoreClock / 100000;						// ������� ������������ SysTick = 100���
		else if (sPVSConfig.bPVS == 1)								// ���
			iTmp = SystemCoreClock / 200000;						// ������� ������������ SysTick = 200���
		else														// ���
			iTmp = SystemCoreClock / 100000;						// ������� ������������ SysTick = 100���
		if (SysTick_ReConfig(iTmp))									// ���������������� SysTick
			while (1);												// ����� �������� ������ (������������ ������������� SysTick)
	}

	if (sPVSState.bRS485 && (unModBus.bReg == 0))
	{
		InitRS485();												// �� �����
		sPVSState.bRS485 = 0;
	}
	sPVSState.bWork = 1;											// ����� "Work" (MasterPass ������ ����������)
}
