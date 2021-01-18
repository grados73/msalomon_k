/*
 * ring_buffer.c
 *
 *  Created on: Jan 12, 2021
 *      Author: grad_
 */

#include "main.h"
#include "ring_buffer.h"

RB_Status RB_Read(RingBuffer *rb, uint8_t *Value) // ktory buffor i do jakiej zmiennej to przypisujemy
{
	if(rb->Head == rb->Tail) // ogon udezyl w glowe, nie ma nic do odczytu
	{
		return RB_NODATA;
	}
	else // jest cos do odczytania
	{
		*Value = rb->Buffer[rb->Tail]; // czytamy z buffera z aktualnej pozycji taila i przypisujemy to do *Value
		rb->Tail = (rb->Tail + 1) % RING_BUFFER_SIZE; // przesuwanie talia o jeden
		return RB_OK;
	}



}

RB_Status RB_Write(RingBuffer *rb, uint8_t Value) // ktory buffor i do jakiej zmiennej zapisujemy
{
	uint8_t TempHead = (rb->Head + 1) % RING_BUFFER_SIZE; // modulo to zabezpieczenie zeby nie przekroczyc rozmiaru bufora

	if(TempHead == rb->Tail) // glowa udezyla w ogon, pelen RingBuffer, nie ma gdzie zapisywac
	{
		return RB_FULLRB;
	}
	else
	{
		rb->Buffer[rb->Head] = Value; // przypisanie wartosci
		rb->Head = TempHead; // zwiększa nie Head'a o jeden bo w inicjalizacji TempHeada zwiekszamy Head o 1
		return RB_OK;
	}
}
