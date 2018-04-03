//Daniel Diamont
//Robert Noe
//TA: Saadallah Kassir

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Switch.h"
#include "ADCT0ATrigger.h"
#include <stdbool.h>
#include "PLL.h"

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

//DEBUGGING BUFFER CODE FOR MEASURING TIME SPENT IN ISR
#define BUFFER_SIZE 512
uint32_t Debug_Buffer[BUFFER_SIZE]; //take 512 samples
uint16_t Debug_Cnt = 0;

bool Data_Collection_Complete = false;
uint8_t shouldRead = 0;

//global for passing information between threads
uint32_t Time_Difference[BUFFER_SIZE-1];

void Get_Time_Differences(void){
	//calculate time differences
			for(int i = 0; i < BUFFER_SIZE - 1; i++){ //total of 999 computations
				Time_Difference[i] = Debug_Buffer[i] - Debug_Buffer[i+1]; //compute the difference between time dump items
			}
			
			while(1)
			{
				//freeze the program
			}
}

void Debug_Dump(){

}

int main(void){      

	PLL_Init(Bus80MHz); //bus to 80 MHz
  Switch_Init(); //initialize switches
  SYSCTL_RCGCGPIO_R |= 0x00000020;         // activate port F
  ADC0_InitTimer0ATriggerSeq3(0, 8000000); // ADC channel 0, 10 Hz sampling
  GPIO_PORTF_DIR_R |= 0x04;                // make PF2 out (built-in LED)
  GPIO_PORTF_AFSEL_R &= ~0x04;             // disable alt funct on PF2
  GPIO_PORTF_DEN_R |= 0x04;                // enable digital I/O on PF2
                                           // configure PF2 as GPIO
  GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFFF0FF)+0x00000000;
  GPIO_PORTF_AMSEL_R = 0;                  // disable analog functionality on PF
  GPIO_PORTF_DATA_R &= ~0x04;              // turn off LED
	Heartbeat_Init();
	LED_Init();
  // other initialization
  EnableInterrupts();
	//Sound_Play_Song(1,0);
	
  while(1){
		//do nothing... just test the sound
//		Sound_Play_Song(1,0);
//		DelayWait10ms(100);
		//if(Debug_Cnt == 512){
			//Get_Time_Differences();
		//}
		
	}
}
