/*
 * parser.h
 *
 *  Created on: Jan 25, 2021
 *      Author: grad_
 */


#ifndef INC_PARSER_H_
#define INC_PARSER_H_
#include "uartdma.h"

void UART_ParseLine(UARTDMA_HandleTypeDef *huartdma);

#endif /* INC_PARSER_H_ */
