//Daniel Diamont
//TA: Saadallah Kassir

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Switch.h"
#include "ADCT0ATrigger.h"
#include <stdbool.h>
#include "PLL.h"
#include "ST7735.h"
#include "FiFo.h"
#include "fixed.h"
#include "UART.h"
#include "calib.h"

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


extern uint32_t LUT [];

uint32_t Convert(uint32_t * ptr){
	
	return LUT[*ptr] - OFFSET;
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
	
	ST7735_InitR(INITR_REDTAB); //initialzie LCD screen
	ST7735_FillScreen(ST7735_BLACK);
	
	//initialize software fifo
	UART_Init();
	FiFo_Init();
	uint32_t period = 80000; //sample at 1 kHz
	ADC0_InitTimer0ATriggerSeq3PD3(period);
	
  EnableInterrupts();
	
	uint32_t data;
	
	uint8_t counter = 0;
	
  while(1){
		
		if(FiFo_Get(&data) == 1){
			//begin critical section
			DisableInterrupts();
			//perform data conversion
			uint32_t temp = Convert(&data);
			ST7735_SetCursor(0,0);
			//display results on LCD screen
			ST7735_OutString("Temp: ", ST7735_YELLOW);
			ST7735_sDecOut2(temp, ST7735_YELLOW);
			ST7735_OutString(" degC", ST7735_YELLOW);
			ST7735_OutString("\nADC value: ", ST7735_YELLOW);
			ST7735_OutUDec(data, ST7735_YELLOW);
			
			if(counter < 12){
				UART_OutUDec(temp);
				UART_OutString("\n\r");
				counter++;
			}
			
			EnableInterrupts();
			
		}		
	}
}
