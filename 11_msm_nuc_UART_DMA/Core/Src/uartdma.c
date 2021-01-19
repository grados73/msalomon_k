/*
 * uartdma.c
 *
 *  Created on: Jan 18, 2021
 *      Author: grad_
 */

#include "main.h"
#include "uartdma.h"

typedef struct
{

	__IO uint32_t ISR; // DMA interrupt status register
	__IO uint32_t Reserved0;
	__IO uint32_t IFCR; // DMA interrupt flag clear register
} DMA_Base_Registers;

void UARTDMA_UartIrqHandler(UARTDMA_HandleTypeDef *huartdma)
{
	if(huartdma->huart->Instance->SR &UART_FLAG_IDLE) // check if Idle flag is set
	{
		volatile uint32_t tmp;
		tmp = huartdma->huart->Instance->SR; // Read Status Register
		tmp = huartdma->huart->Instance->DR; // Read Data Register

		huartdma->huart->hdmarx->Instance->CR &= ~DMA_SxCR_EN; // Disable DMA

		tmp = tmp; // nic nie robi, zeby usunac warninga, ze nic nie robimy z zmienna tma
	}
}

void UARTDMA_DmaReceiveIrqHandler(UARTDMA_HandleTypeDef *huartdma)
{
	uint16_t Length, i;
	uint8_t *DmaBufferPointer;
	DMA_Base_Registers *DmaRegisters = (DMA_Base_Registers *) huartdma->huart->hdmarx->StreamBaseAddress;

	if(__HAL_DMA_GET_IT_SOURCE(huartdma->huart->hdmarx, DMA_IT_TC) != RESET) // sprawdza czy zrodlo przerwania to transmit complete
	{
		DmaRegisters->IFCR = DMA_FLAG_TCIF0_4 << huartdma->huart->hdmarx->StreamIndex; // czysci flage transfer complete

		Length = DMA_RX_BUFFER_SIZE - huartdma->huart->hdmarx->Instance->NDTR; // Pozyskanie dlugosci transmitowanej wiadomosci

		DmaBufferPointer = huartdma->DMA_RX_Buffer;

		for(i = 0; i< Length; i++)
		{
			RB_Write(&huartdma->UART_RX_Buffer, DmaBufferPointer[i]); // przepisujemy zawartosc z buffora dma do bufora uartu
		}

		DmaRegisters->IFCR = 0x3FU << huartdma->huart->hdmarx->StreamIndex; // w rejestrze od przerwań kasuje wszystkie mozliwe przerwania
		huartdma->huart->hdmarx->Instance->M0AR = (uint32_t) huartdma->DMA_RX_Buffer; // ustawia adres pamięci na poczatkowy adres
		huartdma->huart->hdmarx->Instance->NDTR = DMA_RX_BUFFER_SIZE; // informujemy go ile tych danych ma oddebrac
		huartdma->huart->hdmarx->Instance->CR |= DMA_SxCR_EN; //startujemy dma
	}
}

//
// INITIALIZATION
//

void UARTDMA_Init(UARTDMA_HandleTypeDef *huartdma, UART_HandleTypeDef *huart)
{
	huartdma->huart = huart;

	// IDLE Enable - wlaczenie przerwan on stanu bezczynnosci, tzn zakonczenia nadawania przez 2 strone
	__HAL_UART_ENABLE_IT(huartdma->huart, UART_IT_IDLE);

	// DMA TC Enable - wlaczenie przerwan od konca transmisji, osiagniecia rozmiaru bufora (64), Transfer complete
	__HAL_DMA_ENABLE_IT(huartdma->huart->hdmarx, DMA_IT_TC);
	__HAL_DMA_ENABLE_IT(huartdma->huart->hdmarx, DMA_IT_TC);

	//Run DMA UART on Buffer RX
	HAL_UART_Receive_DMA(huartdma->huart, huartdma-> DMA_RX_Buffer, DMA_RX_BUFFER_SIZE);

	// DMA HT Disable - wylaczenie przerwania od polowy transmisji - half transfer
	__HAL_DMA_DISABLE_IT(huartdma->huart->hdmarx, DMA_IT_HT);
	__HAL_DMA_DISABLE_IT(huartdma->huart->hdmatx, DMA_IT_HT);
}
