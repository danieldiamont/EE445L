#include "sound.h"
#include <stdint.h>
#include "tm4c123gh6pm.h"

#define NVIC_ST_CTRL_COUNT      0x00010000  // Count flag
#define NVIC_ST_CTRL_CLK_SRC    0x00000004  // Clock Source
#define NVIC_ST_CTRL_INTEN      0x00000002  // Interrupt enable
#define NVIC_ST_CTRL_ENABLE     0x00000001  // Counter mode
#define NVIC_ST_RELOAD_M        0x00FFFFFF  // Counter load value

void SysTick_Wait(uint32_t delay);
void SysTick_Wait10ms(uint32_t delay);
void SysTick_Init(void);
//static unsigned long wave[32]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
static unsigned long wave[]={0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1};
static uint8_t idx=0;
// Initialize SysTick with busy wait running at bus clock.
void SysTick_Init(void){
  
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE); //Enable GPIOD peripheral
	
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE)) //wait for port d to be ready
	{
	}
	
	//init pin configuration
	//set pin 1 as output (speaker)
	//GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD); //enable weak-pull up
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_2); //set PD1 as output
	
	NVIC_ST_CTRL_R &= ~0x01;
	NVIC_ST_RELOAD_R= 16000;
	NVIC_ST_CURRENT_R=0;
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x40000000; 
	NVIC_ST_CTRL_R|=0x07;
	idx=0;
}
// Time delay using busy wait.
// The delay parameter is in units of the core clock. (units of 20 nsec for 50 MHz clock)

void SysTick_Handler()
{
	idx= (idx+1)&0x0F;
	//GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1,) ^= (0x10)*wave[idx];
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, GPIO_PIN_2*wave[idx]);
}
