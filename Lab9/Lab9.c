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
	
	//uint8_t counter = 0;
	uint32_t j = 0;
	uint8_t N = 128;
	
	#define BUF_SIZE 50 //10 hz cutoff frequency for digital LPF
	uint32_t buf[BUF_SIZE] = {2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500,2500};
	
	ST7735_PlotClear(1000,4000);
	
  while(1){
		
		if(FiFo_Get(&data) == 1){
			//begin critical section
			DisableInterrupts();
			//perform data conversion
			uint32_t temp = Convert(&data);
			
			uint32_t sum = 0;
			uint32_t avg = 0;
			
			for(int i = 1; i < BUF_SIZE; i++){
				buf[i] = buf[i-1];
			}
			
			buf[0] = temp;
			
			for(int i = 0; i < BUF_SIZE; i++){
					sum = sum + buf[i];
			}
			
			avg = sum/BUF_SIZE;
			
			
			ST7735_SetCursor(0,0);
			//display results on LCD screen
			ST7735_OutString("Temp: ", ST7735_YELLOW);
			ST7735_sDecOut2(temp, ST7735_YELLOW);
			ST7735_OutString(" degC\nADC value:", ST7735_YELLOW);
			ST7735_OutUDec(data, ST7735_YELLOW);
			
//			if(counter < 12){
//				UART_OutUDec(temp);
//				UART_OutString("\n\r");
//				counter++;
//			}
			
			ST7735_PlotPoint(avg);  // Measured temperature
			ST7735_PlotNext();
			if((j&(N-1))==0){          // fs sampling, fs/N samples plotted per second
				ST7735_PlotClear(2000,4000);  // overwrites N points on same line
			}
			j++;                       // counts the number of samples
			
			EnableInterrupts();
			
		}		
	}
}
