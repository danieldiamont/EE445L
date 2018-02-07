// Lab1.c "extra credit"
// Runs on TM4C123

//Student name: Daniel Diamont
//Lab Number: 15650
//TA: Saadallah Kassir

#include <stdio.h>
#include <stdint.h>
#include "string.h"
#include "PLL.h"
#include "tm4c123gh6pm.h"

void DelayWait10ms(uint32_t n);
void PortF_Init(void);
void Pause(void);

extern void Test3(void);
extern void Test4(void);
	
int Test1(void){
	// version 1: C floating point
	// run with compiler options selected for floating-point hardware
	volatile float T;    // temperature in C
	volatile uint32_t N; // 12-bit ADC value
	
  for(N=0; N<4096; N++){
    T = 10.0+ 0.009768*N; 	
  }
	return 0;
}

int Test2(void) {
	
	// version 2: C fixed-point
	volatile uint32_t T;    // temperature in 0.01 C
	volatile uint32_t N;    // 12-bit ADC value
	
  for(N=0; N<4096; N++){
    T = 1000 + ((125*N+64)>>7); 	
  }
	return 0;
}

void DelayWait10ms(uint32_t n){uint32_t volatile time;
  while(n){
    time = 727240*2/91;  // 10msec
    while(time){
	  	time--;
    }
    n--;
  }
}

int main(void) {
	PLL_Init();
	PortF_Init();
	
//	Test1(); //run with compiler options for floating point hardware
//	int blah = 0;
//	blah += 42;

//	Test2(); //make sure floating point is disabled
//	int blah = 0;
//	blah += 42;
//	
//	Test3(); //run with compiler options for floating point hardware
//	int blah = 0;
//	blah += 42;
	Test4(); //make sure floating point is disabled
	int blah = 0;
	blah += 42;
}

#define PF2   (*((volatile uint32_t *)0x40025010))
#define PF3   (*((volatile uint32_t *)0x40025020))
#define PF4   (*((volatile uint32_t *)0x40025040))

void Pause(void){
  while(PF4==0x00){ 
    DelayWait10ms(10);
  }
  while(PF4==0x10){
    DelayWait10ms(10);
  }
}

void PortF_Init(void){ 
  SYSCTL_RCGCGPIO_R |= 0x20;        // 1) activate clock for Port F
  while((SYSCTL_PRGPIO_R&0x20)==0){}; // allow time for clock to start
                                    // 2) no need to unlock PF2, PF4
  GPIO_PORTF_PCTL_R &= ~0x000F0F00; // 3) regular GPIO
  GPIO_PORTF_AMSEL_R &= ~0x14;      // 4) disable analog function on PF2, PF4
  GPIO_PORTF_PUR_R |= 0x10;         // 5) pullup for PF4
  GPIO_PORTF_DIR_R |= 0x04;         // 5) set direction to output
  GPIO_PORTF_AFSEL_R &= ~0x14;      // 6) regular port function
  GPIO_PORTF_DEN_R |= 0x14;         // 7) enable digital port
}


