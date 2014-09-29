#include <stdint.h>
#include "PLL.h"
#include "tach.h"
#include "motor.h"
#include "switch.h"
#include "ST7735.h"

void WaitForInterrupt(void);
void DisableInterrupts(void);
void EnableInterrupts(void);
//void IncrementSpeed(void);

int main (void) {
	DisableInterrupts();
	PLL_Init();
	motorInit(40000, 30000);
	tachInit();
	//EnableInterrupts();
	ST7735_InitR(INITR_REDTAB);
	//ST7735_OutString("Tach");
	EnableInterrupts();
	/*
  ST7735_OutString("Graphics test\n");
  ST7735_OutString("cubic function\n");
  ST7735_PlotClear(0,4095);  // range from 0 to 4095
  for(int j=0;j<128;j++){
    ST7735_PlotPoints(j*j/2+900-(j*j/256)*j,32*j); // cubic,linear
    ST7735_PlotNext(); 
  }   // called 128 times
  while(1){
  }*/
	//incrementSpeed();
	//incrementSpeed();
	while(1){};
}
