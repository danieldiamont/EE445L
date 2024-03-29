// main.c
// Name: Daniel Diamont
// TA: Saadallah Kassir
// Created: 1/30/2018
// Last Modified: 1/30/2018


// Runs on TM4C123
// This program periodically samples ADC channel 0 and stores the
// result to a global variable that can be accessed with the JTAG
// debugger and viewed with the variable watch feature.
// Daniel Valvano
// September 5, 2015

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2015

 Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

// center of X-ohm potentiometer connected to PE2/AIN1
// bottom of X-ohm potentiometer connected to ground
// top of X-ohm potentiometer connected to +3.3V 
#include <stdint.h>
#include <stdbool.h>
#include "ADCSWTrigger.h"
#include "tm4c123gh6pm.h"
#include "PLL.h"
#include "ST7735.h"
#include <math.h> //absolute value
#include "fixed.h"

#include "SysTickInts.h"

#define PF4   					(*((volatile uint32_t *)0x40025040))
#define PF3							(*((volatile uint32_t *)0x40025020))
#define PF2             (*((volatile uint32_t *)0x40025010))
#define PF1             (*((volatile uint32_t *)0x40025008))
#define PF0             (*((volatile uint32_t *)0x40025004))
	
//function declarations
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode
void Get_Time_Differences(void); //calculate the time differences from the Debug_Buffer
uint32_t Get_Time_Jitter(void); //calculate the time jitter from the Time_Difference buffer
uint32_t Get_Average(void); //calculate the mean of the ADC data so that we can create the plot
void Plot_PMF(void);
void Pause(void);
void DelayWait10ms(uint32_t n);
	
//Debug Buffer Set up
#define BUFFER_SIZE 1001
#define BUFFER_LEN 2
#define time 0
#define data 1
#define MAX_ALLOWABLE_JITTER 64 // 800 ns / 12.5 ns intervals = 64.

uint32_t Debug_Buffer[BUFFER_SIZE][BUFFER_LEN]; //where [t][i][0] is the time and [t][i][1] is the data
uint16_t Debug_Cnt = 0;

bool Data_Collection_Complete = false;
bool shouldRead = false;

//global for passing information between threads
volatile uint32_t ADCvalue;
uint32_t Time_Difference[BUFFER_SIZE-1];
uint32_t Time_Jitter;

void Debug_Dump(uint8_t n){
	if(Debug_Cnt < BUFFER_SIZE){
			switch (n){
				case (time):
					Debug_Buffer[Debug_Cnt][time] = TIMER1_TAR_R;
					break;								
				case (data):
					Debug_Buffer[Debug_Cnt][data] = ADCvalue;
					Debug_Cnt++;
					break;
		}
	}
	else
		Data_Collection_Complete = true;
}


// This debug function initializes Timer0A to request interrupts
// at a 100 Hz frequency.  It is similar to FreqMeasure.c.
void Timer0A_Init100HzInt(void){
  volatile uint32_t delay;
  DisableInterrupts();
  // **** general initialization ****
  SYSCTL_RCGCTIMER_R |= 0x01;      // activate timer0
  delay = SYSCTL_RCGCTIMER_R;      // allow time to finish activating
  TIMER0_CTL_R &= ~TIMER_CTL_TAEN; // disable timer0A during setup
  TIMER0_CFG_R = 0;                // configure for 32-bit timer mode
  // **** timer0A initialization ****
                                   // configure for periodic mode
  TIMER0_TAMR_R = TIMER_TAMR_TAMR_PERIOD;
  TIMER0_TAILR_R = 799999;         // start value for 100 Hz interrupts
  TIMER0_IMR_R |= TIMER_IMR_TATOIM;// enable timeout (rollover) interrupt
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;// clear timer0A timeout flag
  TIMER0_CTL_R |= TIMER_CTL_TAEN;  // enable timer0A 32-b, periodic, interrupts
  // **** interrupt initialization ****
                                   // Timer0A=priority 2
  NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x40000000; // top 3 bits
  NVIC_EN0_R = 1<<19;              // enable interrupt 19 in NVIC
}
void Timer0A_Handler(void){
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;    // acknowledge timer0A timeout
  PF2 ^= 0x04;                   // profile
	Debug_Dump(time);
  PF2 ^= 0x04;                   // profile
  ADCvalue = ADC0_InSeq3();				//read new value from the ADC sequencer
	Debug_Dump(data);
  PF2 ^= 0x04;                   // profile
}

// ***************** TIMER1_Init ****************
// Activate TIMER1 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
// Outputs: none
void Timer1_Init(){
  SYSCTL_RCGCTIMER_R |= 0x02;   // 0) activate TIMER1
//  PeriodicTask = task;          // user function
  TIMER1_CTL_R = 0x00000000;    // 1) disable TIMER1A during setup
  TIMER1_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER1_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER1_TAILR_R = 0xFFFFFFFF;    // 4) reload value
  TIMER1_TAPR_R = 0;            // 5) bus clock resolution
  TIMER1_ICR_R = 0x00000001;    // 6) clear TIMER1A timeout flag
  TIMER1_CTL_R = 0x00000001;    // 10) enable TIMER1A
}

int main(void){
  PLL_Init(Bus80MHz);                   // 80 MHz
  SYSCTL_RCGCGPIO_R |= 0x20;            // activate port F
	while((SYSCTL_PRGPIO_R&0x20)==0){}; // allow time for clock to start
	ST7735_InitR(INITR_REDTAB); //initialzie LCD screen
	ST7735_FillScreen(ST7735_BLACK);
		
	//initialize Port F
	GPIO_PORTF_DIR_R |= 0x0D;             // make PF2, PF1, PF3 out (built-in LED)
	GPIO_PORTF_PUR_R |= 0x10;         // 5) pullup for PF4
  GPIO_PORTF_AFSEL_R &= ~0x1D;          // disable alt funct on PF1 PF2, PF3 and PF4
  GPIO_PORTF_DEN_R |= 0x1D;             // enable digital I/O on PF1-PF4
                                        // configure PF1-PF4 as GPIO
  GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFFF00F)+0x00000000;
  GPIO_PORTF_AMSEL_R = 0;               // disable analog functionality on PF
		
	ST7735_OutString("Test Lines\nDraw a Star!\nDraw an X!");
	ST7735_PlotClear(0,BUFFER_SIZE/2);  // range from 0 to BUFFER_SIZE
		
	//Star Drawing
	ST7735_Line(50,50,100,100,ST7735_BLUE); //negative slope
	ST7735_Line(50,100,100,50,ST7735_BLUE); //positive slope
	//ST7735_Line(0,100,50,120,ST7735_BLUE); //random
	ST7735_Line(75,50,75,100,ST7735_BLUE);//vertical line
	ST7735_Line(50,75,100,75,ST7735_BLUE); //horizontal line
		
	//X drawing
	ST7735_Line(30,90,10,70,ST7735_BLUE);
	ST7735_Line(30,70,10,90,ST7735_BLUE);
	Pause();
	
	ST7735_FillScreen(ST7735_BLACK);//clear screen
	ST7735_SetCursor(0,0);
	ST7735_OutString("Acquiring data...");
  ADC0_InitSWTriggerSeq3_Ch9();         // allow time to finish activating
	
  PF2 = 0;                      // turn off LED
	
	//SysTick_Init(7920); //Systick interrupts every 99 us
	Timer0A_Init100HzInt();               // set up Timer0A for 100 Hz interrupts
	Timer1_Init();												// set up Timer1A to return the current time up to 53 s in 12.5 ns units
	
  EnableInterrupts();
	
	
  while(1){
    PF1 ^= 0x02;  // toggles when running in main
		//GPIO_PORTF_DATA_R ^= 0x02;
		//PF1 = ((PF1*12345678)/1234567+0x02)*(PF1/(1234567+0x02));  // this line causes jitter
		
		if(Data_Collection_Complete){ //begin processing
			
			long sr = StartCritical(); //disable interrupts
			
			ST7735_FillScreen(ST7735_BLACK);
			
			Get_Time_Differences();			
			Time_Jitter	= Get_Time_Jitter();
			
			ST7735_SetCursor(0,0);
			ST7735_OutString("Time Jitter: ");
			ST7735_OutUDec(Time_Jitter);
			
			
			if(Time_Jitter > MAX_ALLOWABLE_JITTER){
				ST7735_OutString("\ntoo much jitter...");
			}			
			
			ST7735_OutString("\nPMF w/ HW avg: ");
			ST7735_OutUDec(ADC0_SAC_R);
			ST7735_OutString("\nVoltage: ");
			int averageVoltage = (Get_Average()*3300+2048)/4096; //fixed point
			ST7735_sDecOut2(averageVoltage/10);
			ST7735_OutString(" V");
			
			DelayWait10ms(20); //wait two seconds
			
			//plot PMF on LCD Screen
			ST7735_PlotClear(0,BUFFER_SIZE/2);  // range from 0 to BUFFER_SIZE
			ST7735_SetCursor(0,0);
			
			Plot_PMF(); //plot PMF on the screen
			
			//HW averaging factor increment request by the user... (switch at PF4)
			Pause();
			
			ST7735_FillScreen(ST7735_BLACK);
			
			//increase HW averaging factor
			ADC0_ACTSS_R &= ~0x0008;        // disable sample sequencer 3
			ADC0_SAC_R = (ADC0_SAC_R + 2) % 8; //wrap around
			ADC0_ACTSS_R |= 0x0008;					// enable sample sequencer 3
			Debug_Cnt = 0;
			Data_Collection_Complete = false;
			
			ST7735_OutString("Acquiring data...");
			
			EndCritical(sr);			
				
		}
	}
}

void Get_Time_Differences(void){
	//calculate time differences
			for(int i = 0; i < BUFFER_SIZE - 1; i++){ //total of 999 computations
				Time_Difference[i] = Debug_Buffer[i][time] - Debug_Buffer[i+1][time]; //compute the difference between time dump items
			}
}

uint32_t Get_Time_Jitter(void){
	
	uint32_t max;
	uint32_t min;
	
	if(Time_Difference[2] >= Time_Difference[1]) {
		max = Time_Difference[2];
		min = Time_Difference[1];
	}
	else
	{
		min = Time_Difference[2];
		max = Time_Difference[1];
	}
	
	//find min and max
	for(int i= 3; i < BUFFER_SIZE-1; i++){
		if(Time_Difference[i] < min) min = Time_Difference[i];
		else if(Time_Difference[i] > max) max = Time_Difference[i];
	}
	
	return max-min;
}

uint32_t Get_Average(void){
	uint32_t avg = 0;
	uint32_t sum = 0;
	for(int i=1; i < BUFFER_SIZE; i++){
		sum += Debug_Buffer[i][data];
	}
	avg = sum/(BUFFER_SIZE-1);
	return avg;
}

void Plot_PMF(void){
    {   
			//make an array of all the data points to plot
			int freq[127];
			int offset = 64; //128/2 = 64
			int count = 0;
			int avg = Get_Average();
			
			for(int i=0; i < 127;i++){
				count = 0;
				for(int j=1;j< BUFFER_SIZE; j++){
					if(Debug_Buffer[j][data] == avg-offset+i)
						count++;
				}
				freq[i] = count;
				count = 0;
			}
			for(int j=0;j<127; j++){
				ST7735_PlotBar(freq[j]); // called BUFFER_SIZE times
				ST7735_PlotNext();
			}		 
    } 	
}

void Pause(void){
  while(PF4==0x00){ 
    DelayWait10ms(10);
  }
  while(PF4==0x10){
    DelayWait10ms(10);
  }
}

void DelayWait10ms(uint32_t n){
	
	uint32_t volatile timer;
	
  while(n){
    timer = 727240*2/91;  // 10msec
    while(timer){
	  	timer--;
    }
    n--;
  }
}


void SysTick_Handler(){
	PF3 ^= 0x04;
	PF3 ^= 0x04;
//	int a = 123098;
//	a = (a*6808080)/2440404040;
	PF3 ^= 0x04;
}
