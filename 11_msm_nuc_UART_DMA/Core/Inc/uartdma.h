
#ifndef INC_UARTDMA_H_
#define INC_UARTDMA_H_

#include "ring_buffer.h"

#define DMA_RX_BUFFER_SIZE 64
#define DMA_TX_BUFFER_SIZE 64

typedef struct
{
	UART_HandleTypeDef* huart;

	//
	// Receive buffer stuff
	//
	uint8_t DMA_RX_Buffer[DMA_RX_BUFFER_SIZE];
	RingBuffer UART_RX_Buffer;
	uint8_t UartRXBufferLines; // ilosc linijek, ktora przyszla, tzn ile przyszlo znakow \n

	//
	// Transmit buffer stuff
	//
	RingBuffer UART_TX_Buffer;
	uint8_t DMA_TX_Buffer[DMA_TX_BUFFER_SIZE];
	uint8_t UartTXBufferLines;

}UARTDMA_HandleTypeDef;

void UARTDMA_Init(UARTDMA_HandleTypeDef *huartdma, UART_HandleTypeDef *huart);

void UARTDMA_UartIrqHandler(UARTDMA_HandleTypeDef *huartdma);

void UARTDMA_DmaReceiveIrqHandler(UARTDMA_HandleTypeDef *huartdma);

#endif /* INC_UARTDMA_H_*/
