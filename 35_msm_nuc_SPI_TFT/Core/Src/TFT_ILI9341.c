/*
 * TFT_ILI9341.c
 *
 *  Created on: Apr 2, 2021
 *      Author: grados73
 */

#include "main.h"
#include "TFT_ILI9341.h"
SPI_HandleTypeDef *Tft_hspi;

////////////////////////////////////////////////////////////////////////////////////
//
//Basic Function - napisz je dla swojego mikrokontrolera (write those for your MCU)
//
////////////////////////////////////////////////////////////////////////////////////

static void ILI9341_Delay(uint32_t ms)
{
	HAL_Delay(ms);
}

static void ILI9341_SendToTFT(uint8_t *Byte, uint32_t Length)
{
	HAL_SPI_Transmit(Tft_hspi, Byte, Length, ILI9341_SPI_TIMEOUT);
}

static void ILI9341_SendComand(uint8_t Command)
{
	// CS LOW
#if (ILI9341_USE_CS == 1)
	ILI9341_CS_LOW;
#endif

	// DC to Command - DC to LOW
	ILI_9341_DC_LOW;

	// Sent to TFT by SPI 1byte
	ILI9341_SendToTFT(&Command, 1);

	// CS HIGH
#if (ILI9341_USE_CS == 1)
	ILI9341_CS_HIGH;
#endif


}

////////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////////


void ILI9341_Init(SPI_HandleTypeDef *hspi)
{
	Tft_hspi = hspi;

#if (ILI9341_USE_HW_RESET == 1) // uzywamy hardwer resetu
	ILI9341_RST_LOW;
	ILI9341_Delay(10);
	ILI9341_RST_HIGH;
	ILI9341_Delay(10);


#else // jesli nie uzywamy hardwer resetu
	ILI9341_SendCommand(ILI9341_SWRESET); // wyslij komende softwerowego resetu
	ILI9341_Delay(150); // oczekiwanie na inicjalizacje
#endif


}
