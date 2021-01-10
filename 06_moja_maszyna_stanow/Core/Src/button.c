/*
 * button.c
 *
 *  Created on: Jan 7, 2021
 *      Author: grad_
 */
#include "main.h"
#include "button.h"

TButton BlueKey;
TButton ExternalKey;

// MOVE IT TO button_struct!!!
uint32_t BlinksCounter = 0;
uint32_t TimerBlinks = 100;
uint32_t LastBlink = 0;

void ButtonInitKey(TButton* Key, GPIO_TypeDef*	GpioPort, uint16_t	GpioPin, uint32_t TimerIdle, uint32_t TimerDebounce,
		uint32_t TimerPressed, uint32_t TimerRepeat)
{
	Key->GpioPort =GpioPort;
	Key->GpioPin = GpioPin;
	Key->State = IDLE;
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
void ButtonRegisterReleasePressCallback(TButton* Key, void* Callback)
{
	Key-> ButtonRelease = Callback; // Przypisanie funkcji ReleaseRoutine do callbacka
}

void ButtonIdleRoutine(TButton* Key)
{
	if((HAL_GetTick() - Key->LastTick) >= Key->TimerIdle) // jesli minal czas co jaki sprawdzamy wcisniecie przyciku
	{
		if(HAL_GPIO_ReadPin(Key->GpioPort, Key->GpioPin) == GPIO_PIN_SET) // czy jest wcisniety ciagle
		{
			Key->State = DEBOUNCE; // zmiana stanu
		}
		else
		{
			Key->State = IDLE;
		}
		Key->LastTick = HAL_GetTick();
	}
}

void ButtonDebounceRoutine(TButton* Key)
{
	if(HAL_GPIO_ReadPin(Key->GpioPort, Key->GpioPin) == GPIO_PIN_SET)
		{
			if((HAL_GetTick()- Key->LastTick) >= Key->TimerDebounce) // jesli minal czas przewidziany na drganie stykow
			{
				if(Key->ButtonPress != NULL) // czy jest taka funkcja jak ButtonPress w strukturze Key
				{
					Key->ButtonPress();
				}
				Key->State = PRESSED;
				Key->LastTick = HAL_GetTick();
			}
		}
		else
		{
			Key->State = IDLE;
		}

}


void ButtonPressedRoutine(TButton* Key)
{
	if(HAL_GPIO_ReadPin(Key->GpioPort, Key->GpioPin) == GPIO_PIN_SET)
			{
				if((HAL_GetTick()- Key->LastTick) >= Key -> TimerPressed) // czas reakcji na ciagle wcisniecie
				{
					if(Key->ButtonLongPress != NULL)
					{
						Key->ButtonLongPress();
					}
					Key->State = REPEAT;
					Key->LastTick = HAL_GetTick();
				}
			}
			else
			{
				Key->State = RELEASE;
			}

}

void ButtonRepeatRoutine(TButton* Key)
{
		if(HAL_GPIO_ReadPin(Key->GpioPort, Key->GpioPin) == GPIO_PIN_SET)
				{
					if((HAL_GetTick()- Key->LastTick) >= Key -> TimerRepeat)
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
					Key->State = RELEASE;
				}
}

void ButtonReleaseRoutine(TButton* Key)
{
		if(HAL_GPIO_ReadPin(Key->GpioPort, Key->GpioPin) == GPIO_PIN_RESET)
			{
				if(BlinksCounter < 6)
				{
					if(HAL_GetTick() - LastBlink >= TimerBlinks)
					{
						Key->ButtonRelease();
						BlinksCounter++;
						LastBlink = HAL_GetTick();
					}

				}
				else
				{
					Key->State = IDLE;
					BlinksCounter = 0;
				}

			}
		else
			{
				Key->State = DEBOUNCE;
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
	case RELEASE:
		ButtonReleaseRoutine(Key);
		break;
	default:
		break;

	}
}

// SECOND BUTTON - EXTERNAL BUTTON

void EButtonInitKey(TButton* EKey, GPIO_TypeDef*	GpioPort, uint16_t	GpioPin, uint32_t TimerIdle, uint32_t TimerDebounce,
		uint32_t TimerPressed, uint32_t TimerRepeat)
{
	EKey->GpioPort =GpioPort;
	EKey->GpioPin = GpioPin;
	EKey->State = IDLE;
	EKey->TimerIdle = TimerIdle;
	EKey->TimerDebounce = TimerDebounce;
	EKey->TimerPressed = TimerPressed;
	EKey->TimerRepeat = TimerRepeat;
}


void EButtonRegisterPressCallback(TButton* EKey, void* Callback)
{
	EKey-> ButtonPress = Callback; // Przypisanie funkcji buttonpress do callbacka
}
void EButtonRegisterLongPressCallback(TButton* EKey, void* Callback)
{
	EKey-> ButtonLongPress = Callback; // Przypisanie funkcji buttonlongpress do callbacka
}
void EButtonRegisterRepeatPressCallback(TButton* EKey, void* Callback)
{
	EKey-> ButtonRepeatPress = Callback; // Przypisanie funkcji repeatpress do callbacka
}
void EButtonRegisterReleasePressCallback(TButton* EKey, void* Callback)
{
	EKey-> ButtonRelease = Callback; // Przypisanie funkcji ReleaseRoutine do callbacka
}

void EButtonIdleRoutine(TButton* EKey)
{
	if((HAL_GetTick() - EKey->LastTick) >= EKey->TimerIdle) // jesli minal czas co jaki sprawdzamy wcisniecie przyciku
	{
		if(HAL_GPIO_ReadPin(EKey->GpioPort, EKey->GpioPin) == GPIO_PIN_SET) // czy jest wcisniety ciagle
		{
			EKey->State = DEBOUNCE; // zmiana stanu
		}
		else
		{
			EKey->State = IDLE;
		}
		EKey->LastTick = HAL_GetTick();
	}
}

void EButtonDebounceRoutine(TButton* EKey)
{
	if(HAL_GPIO_ReadPin(EKey->GpioPort, EKey->GpioPin) == GPIO_PIN_SET)
		{
			if((HAL_GetTick()- EKey->LastTick) >= EKey->TimerDebounce) // jesli minal czas przewidziany na drganie stykow
			{
				if(EKey->ButtonPress != NULL) // czy jest taka funkcja jak ButtonPress w strukturze Key
				{
					EKey->ButtonPress();
				}
				EKey->State = PRESSED;
				EKey->LastTick = HAL_GetTick();
			}
		}
		else
		{
			EKey->State = IDLE;
		}

}


void EButtonPressedRoutine(TButton* EKey)
{
	if(HAL_GPIO_ReadPin(EKey->GpioPort, EKey->GpioPin) == GPIO_PIN_SET)
			{
				if((HAL_GetTick()- EKey->LastTick) >= EKey -> TimerPressed) // czas reakcji na ciagle wcisniecie
				{
					if(EKey->ButtonLongPress != NULL)
					{
						EKey->ButtonLongPress();
					}
					EKey->State = REPEAT;
					EKey->LastTick = HAL_GetTick();
				}
			}
			else
			{
				EKey->State = RELEASE;
			}

}

void EButtonRepeatRoutine(TButton* EKey)
{
		if(HAL_GPIO_ReadPin(EKey->GpioPort, EKey->GpioPin) == GPIO_PIN_SET)
				{
					if((HAL_GetTick()- EKey->LastTick) >= EKey -> TimerRepeat)
					{
						if(EKey->ButtonRepeatPress != NULL)
						{
							EKey->ButtonRepeatPress();
						}
						EKey->LastTick = HAL_GetTick();
					}
				}
				else
				{
					EKey->State = RELEASE;
				}
}
void EButtonReleaseRoutine(TButton* EKey)
{
	if(HAL_GPIO_ReadPin(EKey->GpioPort, EKey->GpioPin) == GPIO_PIN_RESET)
		{
			if(BlinksCounter < 6)
			{
				if(HAL_GetTick() - LastBlink >= TimerBlinks)
				{
					EKey->ButtonRelease();
					BlinksCounter++;
					LastBlink = HAL_GetTick();
				}

			}
			else
			{
				EKey->State = IDLE;
				BlinksCounter = 0;
			}

		}
	else
		{
			EKey->State = DEBOUNCE;
		}
}

void EButtonTask(TButton* EKey)
{
	switch(EKey -> State){
	case IDLE:
		EButtonIdleRoutine(EKey);
		break;
	case DEBOUNCE:
		EButtonDebounceRoutine(EKey);
		break;
	case PRESSED:
		EButtonPressedRoutine(EKey);
		break;
	case REPEAT:
		EButtonRepeatRoutine(EKey);
		break;
	case RELEASE:
		EButtonReleaseRoutine(EKey);
		break;
	default:
		break;

	}
}



