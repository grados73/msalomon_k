/*
 * ring_buffer.h
 *
 *  Created on: Jan 12, 2021
 *      Author: grad_
 */

#ifndef INC_RING_BUFFER_H_
#define INC_RING_BUFFER_H_

#define RING_BUFFER_SIZE 32

typedef enum
{
	RB_OK = 0,
	RB_ERROR = 1,
	RB_NODATA = 3,
	RB_FULLRB = 4
}RB_Status;

typedef struct
{
	uint8_t Head;
	uint8_t Tail;
	uint8_t Buffer[RING_BUFFER_SIZE];
}RingBuffer;

RB_Status RB_Read(RingBuffer *rb, uint8_t *Value); // ktory buffor i do jakiej zmiennej to przypisujemy

RB_Status RB_Write(RingBuffer *rb, uint8_t Value); // ktory buffor i do jakiej zmiennej zapisujemy


#endif /* INC_RING_BUFFER_H_ */
