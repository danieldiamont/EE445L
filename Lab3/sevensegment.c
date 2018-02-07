#include "ST7735.h"
#include "fixed.h"
#include <stdint.h>
#include "tm4c123gh6pm.h"

static int table[10] = {64,121,36,48,49,18,2,120,0,2};
static int table0[3] = {127, 64, 121};
//sound, 7-seg, switch
int decodeToDisplay(int num)
{
	return table[num];
}

void delay(void)
{
	for(int i =0; i<10; i++){}
}

void PortB_Init(void)
{
	volatile unsigned long delay;
	SYSCTL_RCGCGPIO_R |= 0x00000002;     // 1) activate clock for Port B
  	delay = SYSCTL_RCGCGPIO_R;           // NOP equivalent 
	GPIO_PORTB_DIR_R |= 0x7F;
	GPIO_PORTB_AFSEL_R &=~0x7F;
	GPIO_PORTB_DEN_R |= 0x7F;
}

void PortD_Init(void) //this function also initializes the sound port
{
	volatile unsigned long delay;
	SYSCTL_RCGCGPIO_R |= 0x00000008;     // 1) activate clock for Port D
  	delay = SYSCTL_RCGCGPIO_R;           // NOP equivalent 
	GPIO_PORTD_DIR_R |= 0x1F;
	GPIO_PORTD_AFSEL_R &=~0x1F;
	GPIO_PORTD_DEN_R |= 0x1F;
}

//GPIO_PORTB_DATA_R

void upDateSevenSegmentDisplay(uint32_t hours, uint32_t minutes)
{
	hours=hours%13;
	minutes=minutes%60; //cleanse input
	uint32_t hoursTens = hours/10;
	uint32_t hoursOnes = hours%10;
	uint32_t minutesTens = minutes/10;
	uint32_t minutesOnes = minutes%10;	
	
	
	GPIO_PORTD_DATA_R &= ~0x0F; //clear Port D pins
	
	
	GPIO_PORTB_DATA_R &= ~0x7F; //clear Port B pins
	
	GPIO_PORTB_DATA_R |= table0[hoursTens];
	
	GPIO_PORTD_DATA_R |= 0x01;
	//delay of some kind here?  This is where it writes to the display
	delay();
	GPIO_PORTD_DATA_R &= ~0x0F;
	
	//Second Write
	GPIO_PORTB_DATA_R &= ~0x7F;
	GPIO_PORTB_DATA_R |= table[hoursOnes];
	
	GPIO_PORTD_DATA_R |=0x02;
	//delay?
	delay();
	GPIO_PORTD_DATA_R &= ~0x0F;

	//Third write
	GPIO_PORTB_DATA_R &= ~0x7F;
	GPIO_PORTB_DATA_R |=table[minutesTens];
	
	GPIO_PORTD_DATA_R |=0x04;
	//delay?
	delay();
	GPIO_PORTD_DATA_R &= ~0x0F;
	
	//Fourth write
	GPIO_PORTB_DATA_R &= ~0x7F;
	GPIO_PORTB_DATA_R |=table[minutesOnes];
	
	GPIO_PORTD_DATA_R |= 0x08;
	//delay?
	delay();
	GPIO_PORTD_DATA_R &= ~0x0F;

	GPIO_PORTB_DATA_R &= ~0x7F;
}