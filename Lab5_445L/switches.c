#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "switches.h"

extern uint8_t play_Flag;


void delay1ms0(uint32_t n){uint32_t volatile time;
  while(n){
    time = 72724*2/91;  // 1msec, tuned at 80 MHz
    while(time){
	  	time--;
    }
    n--;
  }
}

void switches_Init(void)
{
//this may or may not be correct
	volatile unsigned long delay;
	SYSCTL_RCGCGPIO_R |= 0x00000028;     // 1) activate clock for Port F
  	delay = SYSCTL_RCGCGPIO_R;           // NOP equivalent 
	GPIO_PORTF_DIR_R &= ~0x11;
	GPIO_PORTF_AFSEL_R &=~0x11;			//pins PF4 and PF0
	GPIO_PORTF_DEN_R |= 0x11;
	//set up edge triggered interrupts
	
	SYSCTL_RCGCGPIO_R |=0x00000008; //port d
	delay = SYSCTL_RCGCGPIO_R;
	GPIO_PORTD_DIR_R &= ~0x0C;
	GPIO_PORTD_AFSEL_R &= ~0x0C; //pins PD2 and PD3
	GPIO_PORTD_DEN_R |= 0x0C;
	//set up edge triggered interrupts
}

void GPIO_PortF_Handler(){
	//acknowledge interrupt
	
	//switch debouncing
	
	//action
}

void GPIO_PortD_Handler(){
	//acknowledge interrupt
	
	//switch debouncing
	
	//action
}
