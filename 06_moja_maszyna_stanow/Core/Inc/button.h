/*
 * button.h
 *
 *  Created on: Jan 7, 2021
 *      Author: grad_
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

typedef enum
{
	IDLE = 0,
	DEBOUNCE, //1
	PRESSED,  //2
	REPEAT,   //3
	RELEASE   //4
} BUTTON_STATE;


typedef struct button_struct
{
	BUTTON_STATE	State;
	GPIO_TypeDef*	GpioPort;
	uint16_t		GpioPin;
	uint32_t		LastTick;
	uint32_t		TimerIdle;
	uint32_t		TimerDebounce;
	uint32_t		TimerPressed;
	uint32_t		TimerRepeat;
	void(*ButtonPress)(void); // Callback funkcji obslugującej wcisniecie, wskaznik na funkcję przyjmująca voida
	void(*ButtonLongPress)(void);
	void(*ButtonRepeatPress)(void);
	void(*ButtonRelease)(void);
}TButton;

//FIRST BUTTON - BLUE BUTTON
void ButtonInitKey(TButton* Key, GPIO_TypeDef*	GpioPort, uint16_t	GpioPin, uint32_t TimerIdle, uint32_t TimerDebounce,
		uint32_t TimerPressed, uint32_t TimerRepeat);

void ButtonRegisterPressCallback(TButton* Key, void* Callback);
void ButtonRegisterLongPressCallback(TButton* Key, void* Callback);
void ButtonRegisterRepeatPressCallback(TButton* Key, void* Callback);
void ButtonRegisterReleasePressCallback(TButton* Key, void* Callback);

void ButtonTask(TButton* Key);

//SECOND BUTTON - EXTERNAL BUTTON
void EButtonInitKey(TButton* EKey, GPIO_TypeDef*	GpioPort, uint16_t	GpioPin, uint32_t TimerIdle, uint32_t TimerDebounce,
		uint32_t TimerPressed, uint32_t TimerRepeat);

void EButtonRegisterPressCallback(TButton* EKey, void* Callback);
void EButtonRegisterLongPressCallback(TButton* EKey, void* Callback);
void EButtonRegisterRepeatPressCallback(TButton* EKey, void* Callback);
void EButtonRegisterReleasePressCallback(TButton* EKey, void* Callback);

void EButtonTask(TButton* EKey);

#endif /* INC_BUTTON_H_ */
