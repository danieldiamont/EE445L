//Daniel Diamont
//TA: Saadallah Kassir

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Sound.h"
#include "SysTickInts.h"
#include "switches.h"
#include "TExaS.h"
#include "MAX5353.h"

volatile uint32_t DAC_Index;
volatile uint8_t playFlag = 0; //0 off; 1 on
volatile uint8_t *data_struct_ptr;
uint8_t startedPlaying = 0; //0 false; 1 true

void EnableInterrupts(void);
void DisableInterrupts(void);

//data structure for three notes
volatile uint8_t Note[32] = {8,9,11,12,13,14,14,15,15,15,14,14,13,12,11,9,8,7,5,4,3,2,2,1,1,1,2,2,3,4,5,7};	
	
//data structure for music
#define NUM_NOTES 100
	
typedef const struct {
	uint32_t song[NUM_NOTES];
	uint32_t time[NUM_NOTES];
	int index;
} Song;

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

int main(void){      
  TExaS_Init(SW_PIN_PE3210,DAC_PIN_PB3210,ScopeOn);    // bus clock at 80 MHz
  switches_Init();
  Sound_Init(0);
	Heartbeat_Init();
  // other initialization
  EnableInterrupts();
	
	data_struct_ptr = Note;
		
  while(1){
	}
}
