#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "switches.h"

static uint32_t isPushedHour=0;
static uint32_t wasPushedHour=0;
static uint32_t isPushedMinute=0;
static uint32_t wasPushedMinute=0;
void PortF_Init(void)
{
//this may or may not be correct
	volatile unsigned long delay;
	SYSCTL_RCGCGPIO_R |= 0x00000020;     // 1) activate clock for Port F
  	delay = SYSCTL_RCGCGPIO_R;           // NOP equivalent 
	GPIO_PORTF_DIR_R &= ~0x60;
	GPIO_PORTF_AFSEL_R &=~0x60;
	GPIO_PORTF_DEN_R |= 0x60;
}

void pushHour(uint32_t *hour)
{
	wasPushedHour=isPushedHour;
	isPushedHour=(GPIO_PORTF_DATA_R/32)%2;
	*hour=(*hour+(isPushedHour*(1-wasPushedHour)))%13;	
}

void pushMinute(uint32_t * minute)
{
	wasPushedMinute=isPushedMinute;
	isPushedMinute=(GPIO_PORTF_DATA_R/64)%2;
	*minute=(*minute+(isPushedMinute*(1-wasPushedHour)))%13;	
}
