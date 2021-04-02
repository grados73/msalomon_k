/*
 * TFT_ILI9341.c
 *
 *  Created on: Apr 2, 2021
 *      Author: grad_
 */

#include "TFT_ILI9341.h"
SPI_HandleTypeDef *Tft_hspi;

void ILI9341_Init(SPI_HandleTypeDef *hspi)
{
	Tft_hspi = hspi;

}
