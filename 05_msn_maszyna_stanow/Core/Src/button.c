/*
 * button.c
 *
 *  Created on: Jan 7, 2021
 *      Author: grad_
 */
#include "main.h"
#include "button.h"

TButton BlueKey;

void ButtonInitKey(TButton* Key, GPIO_TypeDef*	GpioPort, uint16_t	GpioPin, uint32_t TimerIdle, uint32_t TimerDebounce,
		uint32_t TimerPressed, uint32_t TimerRepeat)
{
	Key->GpioPort =GpioPort;
	Key->GpioPin = GpioPin;
	Key->State = IDLE;
	Key->Timer = 0;
	Key->TimerIdle = TimerIdle;
	Key->TimerDebounce = TimerDebounce;
	Key->TimerPressed = TimerPressed;
	Key->TimerRepeat = TimerRepeat;
}

void ButtonRegisterPressCallback(TButton* Key, void* Callback)
{
	Key-> ButtonPress = Callback; // Przypisanie funkcji buttonpress do callbacka
}
void ButtonRegisterLongPressCallback(TButton* Key, void* Callback)
{
	Key-> ButtonLongPress = Callback; // Przypisanie funkcji buttonlongpress do callbacka
}
void ButtonRegisterRepeatPressCallback(TButton* Key, void* Callback)
{
	Key-> ButtonRepeatPress = Callback; // Przypisanie funkcji repeatpress do callbacka
}

void ButtonIdleRoutine(TButton* Key)
{
	if((HAL_GetTick() - Key->LastTick) >= Key->TimerIdle)
	{
		if(HAL_GPIO_ReadPin(Key->GpioPort, Key->GpioPin) == GPIO_PIN_SET) // czy jest wcisniete
		{
			Key->State = DEBOUNCE;
			Key->Timer = Key->TimerDebounce;
		}
		else
		{
			Key->State = IDLE;
			Key->Timer = Key->TimerIdle;
		}
		Key->LastTick = HAL_GetTick();
	}
}

void ButtonDebounceRoutine(TButton* Key)
{
	if(HAL_GPIO_ReadPin(Key->GpioPort, Key->GpioPin) == GPIO_PIN_SET)
		{
			if((HAL_GetTick()- Key->LastTick) >= Key -> Timer)
			{
				if(Key->ButtonPress != NULL)
				{
					Key->ButtonPress();
				}
				Key->State = PRESSED;
				Key->Timer = Key->TimerPressed;

				Key->LastTick = HAL_GetTick();
			}
		}
		else
		{
			Key->State = IDLE;
			Key->Timer = Key->TimerIdle;
		}

}


void ButtonPressedRoutine(TButton* Key)
{
	if(HAL_GPIO_ReadPin(Key->GpioPort, Key->GpioPin) == GPIO_PIN_SET)
			{
				if((HAL_GetTick()- Key->LastTick) >= Key -> Timer)
				{
					if(Key->ButtonLongPress != NULL)
					{
						Key->ButtonLongPress();
					}
					Key->State = REPEAT;
					Key->Timer = Key->TimerRepeat;

					Key->LastTick = HAL_GetTick();
				}
			}
			else
			{
				Key->State = IDLE;
				Key->Timer = Key->TimerIdle;
			}

}

void ButtonRepeatRoutine(TButton* Key)
{
		if(HAL_GPIO_ReadPin(Key->GpioPort, Key->GpioPin) == GPIO_PIN_SET)
				{
					if((HAL_GetTick()- Key->LastTick) >= Key -> Timer)
					{
						if(Key->ButtonRepeatPress != NULL)
						{
							Key->ButtonRepeatPress();
						}
						Key->LastTick = HAL_GetTick();
					}
				}
				else
				{
					Key->State = IDLE;
					Key->Timer = Key->TimerIdle;
				}
}

void ButtonTask(TButton* Key)
{
	switch(Key -> State){
	case IDLE:
		ButtonIdleRoutine(Key);
		break;
	case DEBOUNCE:
		ButtonDebounceRoutine(Key);
		break;
	case PRESSED:
		ButtonPressedRoutine(Key);
		break;
	case REPEAT:
		ButtonRepeatRoutine(Key);
		break;
	default:
		break;




	}
}
