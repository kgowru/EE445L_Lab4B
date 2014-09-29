#ifndef SWITCH_H
#define SWITCH_H

/*
	A header file for the Input class in the Alarm Clock Lab.
*/

#include <stdint.h>

void switchInit(void(*dSpeed)(void), void(*iSpeed)(void));

void GPIOPortE_Handler(void);

#endif
