/*
	This class details the drivers for the 2 buttons used in this lab.
	Authors: Kapil Gowru and Matthew Normyle
*/
// momentary, positive logic button connected to PE4 (trigger on rising edge)
// momentary, positive logic button connected to PE5 (trigger on rising edge)

#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#include "SysTickInts.h"
#define NVIC_EN0_INT4           0x00000010  // Interrupt 4 enable

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

extern unsigned char decrementBounced;
extern unsigned char incrementBounced;

void(*decrementSpeedSwitch)(void);
void(*incrementSpeedSwitch)(void);
// global variables visible in Watch window of debugger
// set when corresponding button pressed
//volatile uint32_t SW1, SW2; // semaphores
void switchInit(void(*dSpeed)(void), void(*iSpeed)(void)){
  DisableInterrupts();

  SYSCTL_RCGCGPIO_R |= 0x00000010; // activate port E
	decrementSpeedSwitch = dSpeed;
	incrementSpeedSwitch = iSpeed;
  GPIO_PORTE_AMSEL_R &= ~0x30;// disable analog function on PE5-4
  GPIO_PORTE_PCTL_R &= ~0x00FF0000; // configure PE5-4 as GPIO 
  GPIO_PORTE_DIR_R &= ~0x30;  // make PE5-4 in 
  GPIO_PORTE_AFSEL_R &= ~0x30;// disable alt funct on PE5-4 
  GPIO_PORTE_DEN_R |= 0x30;   // enable digital I/O on PE5-4
  GPIO_PORTE_IS_R &= ~0x30;   // PE5-4 is edge-sensitive 
  GPIO_PORTE_IBE_R &= ~0x30;  // PE5-4 is not both edges 
  GPIO_PORTE_IEV_R |= 0x30;   // PE5-4 rising edge event
  GPIO_PORTE_ICR_R = 0x30;    // clear flag5-4
  GPIO_PORTE_IM_R |= 0x30;    // enable interrupt on PE5-4
                              // GPIO PortE=priority 2
  NVIC_PRI1_R = (NVIC_PRI1_R&0xFFFFFF00)|0x00000040; // bits 5-7
  NVIC_EN0_R = NVIC_EN0_INT4; // enable interrupt 4 in NVIC
  EnableInterrupts();
}
void GPIOPortE_Handler(void){
  if(GPIO_PORTE_RIS_R&0x10){  // poll PE4
    GPIO_PORTE_ICR_R = 0x10;  // acknowledge flag4
    if(decrementBounced ==0){
			(*decrementSpeedSwitch)();
			decrementBounced =1;
			SysTick_Init(8000000);
		}
  }
  if(GPIO_PORTE_RIS_R&0x20){  // poll PE5
    GPIO_PORTE_ICR_R = 0x20;  // acknowledge flag5
    if(incrementBounced == 0){
			(*incrementSpeedSwitch)();
			incrementBounced =1;
			SysTick_Init(8000000);
			
		}
  }
}
