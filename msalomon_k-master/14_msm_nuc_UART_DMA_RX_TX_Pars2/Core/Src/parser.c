/*
 * parser.c
 *
 *  Created on: Jan 25, 2021
 *      Author: grad_
 */


#include "main.h"
#include "parser.h"
#include "string.h"
#include "uartdma.h"
#include "stdlib.h"
#include "stdio.h"

extern UARTDMA_HandleTypeDef huartdma2;
char Message[64]; // Transmit buffer
char MyName[32] = {"No Name"}; // Name string

/*
 * 		LED=1\n 	// LED On
 * 		LED=0\n 	// LED Off
 */
void UART_ParseLED()
{
	uint8_t LedState; // Received state variable

	char* ParsePointer = strtok(NULL, ","); // Look for next token or end of string
	// Should be now: ParsePointer == 1'\0'

	if(strlen(ParsePointer) > 0) // If string exists
	{
		if(ParsePointer[0] < '0' || ParsePointer[0] > '9') // Chceck if there are only numbers
		{
			UARTDMA_Print(&huartdma2, "LED wrong value. Don't use letters dude!\r\n"); // Print message
			return;	// And exit parsing
		}

		LedState = atoi(ParsePointer); // If there are no chars, change string to integer

		if(LedState == 1) // LED ON
		{
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
			UARTDMA_Print(&huartdma2, "LED On\r\n");
		}
		else if(LedState == 0) // LED OFF
		{
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
			UARTDMA_Print(&huartdma2, "LED Off\r\n");
		}
		else // Wrong state number
		{
			UARTDMA_Print(&huartdma2, "LED wrong value. Use 0 or 1.\r\n");
		}
	}
}

/*
 * ENV=X,Y,Z\0 // X - temperature, Y - humidity, Z - pressure
 */
void UART_ParseENV()
{
	uint8_t i,j; // Iterators
	float EnvParameters[3]; // Temperature, humidity, pressure

	for(i = 0; i<3; i++) // 3 parameters are expected
	{
		char* ParsePointer = strtok(NULL, ","); // Look for next token or end of string

		if(strlen(ParsePointer) > 0) // If string exists
		{
			for(j=0; ParsePointer[j] != 0; j++) // Loop over all chars in current strong-block
			{
				if((ParsePointer[j] < '0' || ParsePointer[j] > '9') && ParsePointer[j] != '.' ) // Check if there are only numbers or dot sign
				{
					sprintf(Message, "ENV wrong value. Don't use letters dude!\r\n"); // If not, Error message
					UARTDMA_Print(&huartdma2, Message); // Print message
					return;	// And exit parsing
				}

				EnvParameters[i] = atof(ParsePointer); // If there are no chars, change string to integer
			}
		}
		else
		{
			sprintf(Message, "ENV too less values. ENV=X,Y,Z\\n\r\n"); // If not, Error message
			UARTDMA_Print(&huartdma2, Message); // Print message
			return;	// And exit parsing
		}
	}

	// Print back received data
	sprintf(Message, "Temperature: %f\r\n", EnvParameters[0]);
	UARTDMA_Print(&huartdma2, Message);

	sprintf(Message, "Humidity: %f\r\n", EnvParameters[1]);
	UARTDMA_Print(&huartdma2, Message);

	sprintf(Message, "Pressure: %f\r\n", EnvParameters[2]);
	UARTDMA_Print(&huartdma2, Message);
}

/*
 * 		NAME=X\n	// Change name for X
 * 		NAME=?\n	// introduce yourself
 */
void UART_ParseNAME()
{
	char* ParsePointer = strtok(NULL, ","); // Get next string till token ',' or \0

	if(strlen(ParsePointer) > 0) // If string exists
	{
		if(strcmp(ParsePointer, "?") == 0) // If '?' is behind '='
		{
			sprintf(Message, "My name is %s\r\n", MyName); // Introduce yourself
		}
		else
		{
			strcpy(MyName, ParsePointer); // Change name for string passed in received message
			sprintf(Message, "Name changed to %s\r\n", MyName);
		}
	}
	else
	{
		// Error
		sprintf(Message, "Name cannot be empty!\r\n");
	}

	// Send back a message
	UARTDMA_Print(&huartdma2, Message);
}
/*
 * Parsing headers:
 * 		LED=1\n 	// LED On
 * 		LED=0\n 	// LED Off
 * 		ENV=X,Y,Z\n // X - temperature, Y - humidity, Z - pressure
 * 		NAME=X\n	// Change name for X
 * 		NAME=?\n	// introduce yourself
 */
void UART_ParseLine(UARTDMA_HandleTypeDef *huartdma)
{
	char BufferReceive[64];

	if(!UARTDMA_GetLineFromReceiveBuffer(huartdma, BufferReceive))
	{
		// Header
		char* ParsePointer = strtok(BufferReceive, "="); // LED\0   1\0
		// ParsePointer == LED\0

	  if(strcmp(ParsePointer, "LED") == 0)
	  {
		  UART_ParseLED();
	  }
	  else if(strcmp(ParsePointer, "ENV") == 0)
	  {
		  UART_ParseENV();
	  }
	  else if(strcmp(ParsePointer, "NAME") == 0)
	  {
		  UART_ParseNAME();
	  }

	}
}

