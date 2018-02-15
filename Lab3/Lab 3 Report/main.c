// main.c
// Name: Daniel Diamont
// TA: Saadallah Kassir
// Created: 2/5/2018
// Last Modified: 2/5/2018


#include <stdint.h>
#include <stdbool.h>
#include "PLL.h"
#include "ST7735.h"
#include <math.h> //absolute value
#include "fixed.h"
#include "sound.h"
#include "switches.h"
//#include "sevensegment.h"
#include "timer.h"
#include "clock.h"

//TivaWare
#include "driverlib/pin_map.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "inc/hw_types.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/debug.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/comp.h"
#include "driverlib/sysctl.h"
#include "inc/hw_comp.h"

/*
* Bit-specific addressing for PORT_F*
*/
#define PF4   					(*((volatile uint32_t *)0x40025040))
#define PF3							(*((volatile uint32_t *)0x40025020))
#define PF2             (*((volatile uint32_t *)0x40025010))
#define PF1             (*((volatile uint32_t *)0x40025008))
#define PF0             (*((volatile uint32_t *)0x40025004))
#define LOCK_REG				(*((volatile uint32_t *)(GPIO_O_LOCK + GPIO_PORTF_BASE)))
#define CR_REG					(*((volatile uint32_t *)(GPIO_O_CR + GPIO_PORTF_BASE)))
	
//#define CYCLES_PER_MINUTE 4800000000
#define CYCLES_PER_MINUTE 80000000

//interrupt enable/disable declarations
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

void Pause(void);
void DelayWait10ms(uint32_t n);

void TestSound(void);
void TestTimer(void);
void TestGraphics(void);
void Test7SegmentDisplay(void);
void TestSwitches(void);

void GPIO_PortF_Init(void);
void LCD_Init(void);
void Clock_Init(void);
void Change_Background(void);

void Update_Time(void);

extern uint8_t play_Flag;
extern uint32_t hour;
extern uint32_t minute;

extern uint32_t alarmhour;
extern uint32_t alarmminute;
extern bool should_Update;
extern uint8_t isAM;
extern uint8_t alarmAM;

int main(void){
	
	SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

//	 uint32_t sysClockFreq;
//	 sysClockFreq = SysCtlClockGet();
	
	GPIO_PortF_Init(); //initialize Port F
	PortD_Init();
	LCD_Init(); //init LCD screen
	//WideTimer0A_Init(&Update_Time, CYCLES_PER_MINUTE); //initialize timer to countdown per minute
	
	SysTick_Init();
	ST7735_FillScreen(0);
	ST7735_SetCursor(0,0);
	ST7735_OutString("Enter start time",ST7735_YELLOW);
	DelayWait10ms(1000);
	play_Flag = 0;
	
	Set_Time();
	
	WideTimer0A_Init(&Update_Time, CYCLES_PER_MINUTE); //initialize timer to countdown per minute
	Draw_Clock();
	
	
  EnableInterrupts();
	
  while(1){	
		
		Draw_Clock();
		
		DelayWait10ms(100);
		
		if(hour == alarmhour && minute == alarmminute && alarmAM == isAM){
			play_Flag = 1;
		}
		
		if(PF0 == 0x00){
			play_Flag = 0;
		}
		
		if(PF4 == 0x00){
			should_Update = false;
			Set_Time();
		}
		
		if(GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_2) == GPIO_PIN_2){
			Change_Background();
		}
		
		//main program goes here
		
		//test sound module
//		TestSound();
//		//test timer module
		//TestTimer();
//		//test graphics module
//		TestGraphics();
		//test switch module
//		TestSwitches();
//		//test 7-segment module
//		Test7SegmentDisplay();
		
	}
}

void Pause(void){
  while(PF4 == 0x00){ 
    DelayWait10ms(10);
  }
  while(PF4==0x10){
    DelayWait10ms(10);
  }
}

//void DelayWait10ms(uint32_t n){
//	
//	uint32_t volatile timer;
//	
//  while(n){
//    timer = 727240*2/91;  // 10msec
//    while(timer){
//	  	timer--;
//    }
//    n--;
//  }
//}

void GPIO_PortF_Init(){
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); //Enable GPIOF peripheral
	
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)) //wait for port f to be ready
	{
	}
	
	//init pin configuration
	//set pins 0 and 4 as inputs (switch inputs)
	//set pin 1 as output (heartbeat)
	LOCK_REG = GPIO_LOCK_KEY; //unlock the port
	CR_REG = 0x000000FF; //commit the unlock
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4); //set PF0 and PF4 as inputs
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU); //enable weak-pull up
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);	//set PF1 as output
}

void LCD_Init(){
	ST7735_InitR(INITR_REDTAB); //initialzie LCD screen
	ST7735_FillScreen(ST7735_BLACK);
}

void TestSound(){
	SysTick_Init();
	
	while(1){}
}

void TestSwitches(){
}

void TestTimer(){	
	ST7735_FillScreen(0);
	ST7735_SetCursor(0,0);
	ST7735_OutString("Timer Test\n\nLED toggles @ 1 hz\n\nFreq",ST7735_YELLOW);
	ST7735_OutUDec(SysCtlClockGet(),ST7735_YELLOW);
	Pause();
}

void Test7SegmentDisplay(){
}

void TestGraphics(){
	SysTick_Init();
	ST7735_FillScreen(0);
	ST7735_SetCursor(0,0);
	ST7735_OutString("Graphics Test..",ST7735_YELLOW);
	ST7735_OutString("Start time: 12:44" ,ST7735_YELLOW); //print out the time
	DelayWait10ms(1000);
	play_Flag = 1;
	Draw_Clock();
	
	Pause();
	
	Change_Background();
	ST7735_OutString("Graphics Test complete",ST7735_YELLOW);
	Pause();
}
