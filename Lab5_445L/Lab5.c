//Daniel Diamont
//TA: Saadallah Kassir

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Sound.h"
#include "Switch.h"
#include "TExaS.h"
#include "MAX5353.h"
#include "Timer1A.h"

void EnableInterrupts(void);
void DisableInterrupts(void);
extern void DelayWait10ms(uint32_t n);

void Heartbeat_Init(void) {
				volatile uint8_t delay;
				SYSCTL_RCGCGPIO_R |= 0x20; //init port f
				delay = 42;
				GPIO_PORTF_AMSEL_R &= ~0x04; // 3) disable analog function on PF2
				GPIO_PORTF_PCTL_R &= ~0x00000004; // 4) enable regular GPIO
				GPIO_PORTF_DIR_R |= 0x04;   // 5) output on pf2
				GPIO_PORTF_AFSEL_R &= ~0x04; // 6) regular function on PE5-0
				GPIO_PORTF_DEN_R |= 0x04;    // 7) enable digital on PE1-0
	
				SYSCTL_RCGCGPIO_R |= 0x01; //init port a
				delay = 42;
				GPIO_PORTA_AMSEL_R &= ~0x04; // 3) disable analog function on 
				GPIO_PORTA_PCTL_R &= ~0x00000004; // 4) enable regular GPIO
				GPIO_PORTA_DIR_R |= 0x04;   // 5) output on p
				GPIO_PORTA_AFSEL_R &= ~0x04; // 6) regular functio
				GPIO_PORTA_DEN_R |= 0x04;    
}

//void task1(void);
//void task2(void);

int main(void){      
  //TExaS_Init(SW_PIN_PE3210,DAC_PIN_PB3210,ScopeOn);    // bus clock at 80 MHz
  TExaS_Init(SW_PIN_PE3210,DAC_PIN_PB3210,ScopeOn);    // bus clock at 80 MHz
  Switch_Init();
	Heartbeat_Init();
	LED_Init();
  Sound_Init();
  // other initialization
  EnableInterrupts();
	//Sound_Play_Song(1,0);
	
  while(1){
		//do nothing... just test the sound
//		Sound_Play_Song(1,0);
//		DelayWait10ms(100);
		
	}
}
