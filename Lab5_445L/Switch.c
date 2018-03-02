// Switch.c
// Runs on TMC4C123
// Use GPIO in edge time mode to request interrupts on any
// edge of PF4 and start Timer0B. In Timer0B one-shot
// interrupts, record the state of the switch once it has stopped
// bouncing.
// Daniel Valvano
// May 3, 2015

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

// PF4 connected to a negative logic switch using internal pull-up (trigger on both edges)
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "sound.h"
#include <stdbool.h>

extern bool playSong;

#define PF4                     (*((volatile uint32_t *)0x40025040))
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

#define GPIO_PORTF_LOCK_R       (*((volatile uint32_t *)0x40025520))
#define GPIO_PORTF_CR_R         (*((volatile uint32_t *)0x40025524))
#define PF0            				 (*((volatile uint32_t *)0x40025004))
#define GPIO_LOCK_KEY           0x4C4F434B  // Unlocks the GPIO_CR register

#define GPIO_PORTF_ICR_R        (*((volatile uint32_t *)0x4002541C))

//define PD3 and PD2
#define GPIO_PORTD_BASE         (*((volatile uint32_t *)0x40007000))  // GPIO Port D
//#define PD2 										(*((volatile uint32_t *)(GPIO_PIN_2 + GPIO_PORTD_BASE)))
//#define PD3											(*((volatile uint32_t *)(GPIO_PIN_3 + GPIO_PORTD_BASE)))

volatile static unsigned long TouchPF4;     // true on //Touch
volatile static unsigned long TouchPF0;
volatile static unsigned long TouchPD3;
volatile static unsigned long TouchPD2;
volatile static unsigned long ReleasePF4;
volatile static unsigned long ReleasePF0;
volatile static unsigned long ReleasePD3;
volatile static unsigned long ReleasePD2;
volatile static unsigned long LastPF4;      // previous
volatile static unsigned long LastPF0;
volatile static unsigned long LastPD3;
volatile static unsigned long LastPD2;

void (*TouchTask)(void);    // user function to be executed on //Touch
void (*ReleaseTask)(void);  // user function to be executed on release
void Sound_Play_Song(uint8_t song, uint8_t instrument);



#define GPIO_LOCK_KEY           0x4C4F434B  // Unlocks the GPIO_CR register
#define PF0                     (*((volatile uint32_t *)0x40025004))
#define PF4                     (*((volatile uint32_t *)0x40025040))
#define SWITCHES                (*((volatile uint32_t *)0x40025044))
#define SW1       0x10                      // on the left side of the Launchpad board
#define SW2       0x01                      // on the right side of the Launchpad board
#define PF1                     (*((volatile uint32_t *)0x40025008))
#define PF2                     (*((volatile uint32_t *)0x40025010))
#define PF3                     (*((volatile uint32_t *)0x40025020))

//------------LED_Init------------
// Initialize GPIO Port F for negative logic switches on PF0 and
// PF4 as the Launchpad is wired.  Weak internal pull-up
// resistors are enabled, and the NMI functionality on PF0 is disabled.
// LEDs on PF3,2,1 are enabled
// Input: none
// Output: none
void LED_Init(void){       
  SYSCTL_RCGCGPIO_R |= 0x20;     // 1) activate Port F
  while((SYSCTL_PRGPIO_R & 0x20)!=0x20){}; // wait to finish activating     
  GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;// 2a) unlock GPIO Port F Commit Register
  GPIO_PORTF_CR_R = 0x1F;        // 2b) enable commit for PF4-PF0     
  GPIO_PORTF_AMSEL_R &= ~0x1F;   // 3) disable analog functionality on PF4-PF0     
  GPIO_PORTF_PCTL_R = 0x00000000;// 4) configure PF0-PF4 as GPIO
  GPIO_PORTF_DIR_R = 0x0E;       // 5) make PF0 and PF4 in PF3-1 output                        
  GPIO_PORTF_AFSEL_R = 0;        // 6) disable alt funct on PF0 and PF4
  GPIO_PORTF_PUR_R = 0x11;       // enable weak pull-up on PF0 and PF4
  GPIO_PORTF_DEN_R = 0x1F;       // 7) enable digital I/O on PF0-PF4
}

//------------LED_BlueToggle------------
// Toggle blue LED
// Input: none
// Output: none
void LED_BlueToggle(void){
  PF2 ^= 0x04;
}
//------------LED_GreenToggle------------
// Toggle green LED
// Input: none
// Output: none
void LED_GreenToggle(void){
  PF3 ^= 0x08;
}

#define PD2 										(*((volatile uint32_t *)0x40007010))
#define PD3											(*((volatile uint32_t *)0x40007020))

static void GPIOArm(void){
  GPIO_PORTF_ICR_R = 0x11;      // (e) clear flag4 and flag0
  GPIO_PORTF_IM_R |= 0x11;      // (f) arm interrupt on PF4 and PF0 *** No IME bit as mentioned in Book ***
  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF00FFFF)|0x00A00000; // (g) priority 5
  NVIC_EN0_R = 0x40000000;      // (h) enable interrupt 30 in NVIC  
	
	GPIO_PORTD_ICR_R = 0x0C;//clear flag3 and flag2
	GPIO_PORTD_IM_R |= 0x0C;//arm interrupt on PD2 and PD3
	NVIC_PRI0_R = (NVIC_PRI0_R&0x0FFFFFFF)|0xA0000000; //priority 5
	NVIC_EN0_R = 0x00000008; //enable interrupt 3 in NVIC
}
// Initialize switch interface on PF4 
// Inputs:  pointer to a function to call on //Touch (falling edge),
//          pointer to a function to call on release (rising edge)
// Outputs: none 
// Initialize switch interface on PF4 and PF0
void Switch_Init(){
  // **** general initialization ****
  SYSCTL_RCGCGPIO_R |= 0x00000028; // (a) activate clock for port F and port d
  while((SYSCTL_PRGPIO_R & 0x00000020) == 0 || (SYSCTL_PRGPIO_R & 0x00000008) == 0){};
	
	GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
	GPIO_PORTF_CR_R = 0xFF;
  GPIO_PORTF_DIR_R &= ~0x10;    // (c) make PF4 in (built-in button)
  GPIO_PORTF_AFSEL_R &= ~0x10;  //     disable alt funct on PF4
  GPIO_PORTF_DEN_R |= 0x10;     //     enable digital I/O on PF4   
  GPIO_PORTF_PCTL_R &= ~0x000F0000; // configure PF4 as GPIO
  GPIO_PORTF_AMSEL_R = 0;       //     disable analog functionality on PF
  GPIO_PORTF_PUR_R |= 0x10;     //     enable weak pull-up on PF4
  GPIO_PORTF_IS_R &= ~0x10;     // (d) PF4 is edge-sensitive
  GPIO_PORTF_IBE_R |= 0x10;     //     PF4 is both edges

  GPIO_PORTF_DIR_R &= ~0x01;    // (c) make PF0 in (built-in button)
  GPIO_PORTF_AFSEL_R &= ~0x01;  //     disable alt funct on PF0
  GPIO_PORTF_DEN_R |= 0x01;     //     enable digital I/O on PF0   
  GPIO_PORTF_PCTL_R &= ~0x000000FF; // configure PF0 as GPIO
  GPIO_PORTF_AMSEL_R = 0;       //     disable analog functionality on PF
  GPIO_PORTF_PUR_R |= 0x01;     //     enable weak pull-up on PF0
  GPIO_PORTF_IS_R &= ~0x01;     // (d) PF0 is edge-sensitive
  GPIO_PORTF_IBE_R |= 0x01;     //     PF0 is both edges

  GPIO_PORTD_DIR_R &= ~0x08;    // (c) make PD2 PD3 inputs
  GPIO_PORTD_AFSEL_R &= ~0x08;  //     disable alt funct on PD23
  GPIO_PORTD_DEN_R |= 0x08;     //     enable digital I/O on PD23   
  GPIO_PORTD_PCTL_R = 0; // configure PD23 as GPIO
  GPIO_PORTD_AMSEL_R = 0;       //     disable analog functionality on PD
  //GPIO_PORTD_PUR_R |= 0x0C;     //     enable weak pull-up on PD23
  GPIO_PORTD_IS_R &= ~0x08;     // (d) PD23 is edge-sensitive
  GPIO_PORTD_IBE_R |= 0x08;     //     PD23 is both edges
	
	GPIO_PORTD_DIR_R &= ~0x04;    // (c) make PD2 PD3 inputs
  GPIO_PORTD_AFSEL_R &= ~0x04;  //     disable alt funct on PD23
  GPIO_PORTD_DEN_R |= 0x04;     //     enable digital I/O on PD23   
  GPIO_PORTD_PCTL_R = 0; // configure PD23 as GPIO
  GPIO_PORTD_AMSEL_R = 0;       //     disable analog functionality on PD
  //GPIO_PORTD_PUR_R |= 0x0C;     //     enable weak pull-up on PD23
  GPIO_PORTD_IS_R &= ~0x04;     // (d) PD23 is edge-sensitive
  GPIO_PORTD_IBE_R |= 0x04;     //     PD23 is both edges
  //PD2, and PD3

  GPIOArm();

  SYSCTL_RCGCTIMER_R |= 0x01;   // 0) activate TIMER0

  //SYSCTL_RCGCTIMER_R |= 0x01;   // 0) activate TIMER0
 }

int deBounce(void)
{
   int debounceWait=0;
   int isPushed=0;
//   int isReleased=0;
   while(1)
   {
      if(PF4==1)
      {
         isPushed++;
 //        isReleased=0;
         debounceWait++;
      }
      else
      {
//         isReleased++;
         isPushed=0;
         debounceWait++;
      }
      if(isPushed>=500)
      {
         return 1;
      }
 //     if(isReleased>=500 || debounceWait>=1000)
      {
         return 0;
      }
   }
}
// Interrupt on rising or falling edge of PF4 (CCP0)
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
void GPIOPortF_Handler(void){

  GPIO_PORTF_IM_R &= ~0x11;
	LED_GreenToggle();
	LED_BlueToggle();
	if((GPIO_PORTF_DATA_R>>4)%0x01==0)
	if(PF4 == 0)
	{
		DelayWait10ms(3);
		Sound_Play_Song(0,0);
		GPIO_PORTF_ICR_R = 0x10;
		GPIO_PORTF_IM_R |= 0x11;
	}
	else if(PF0 == 0)
	{
		DelayWait10ms(3);
		if(playSong == true){
			Sound_Pause_Song();
			GPIO_PORTF_ICR_R = 0x01;
			GPIO_PORTF_IM_R |= 0x11;
		}
		else {
			Sound_Resume_Song();
			GPIO_PORTF_ICR_R = 0x01;
			GPIO_PORTF_IM_R |= 0x11;
		}
	}
	else
	{
		DelayWait10ms(3);
		GPIO_PORTF_ICR_R = 0xFF;
		GPIO_PORTF_IM_R |= 0x11;
		
	}
}

void GPIOPortD_Handler(void){
  
  GPIO_PORTD_IM_R &= ~0x0C; 
	if(PD3 == 0)
	{
		DelayWait10ms(3);
		Sound_Rewind_Song();
		GPIO_PORTD_ICR_R = 0xFF;
		GPIO_PORTD_IM_R |= 0x0C;
	}
	else if(PD2 == 0)
	{
		DelayWait10ms(3);
		Sound_Rewind_Song();
		GPIO_PORTD_ICR_R = 0xFF;
		GPIO_PORTD_IM_R |= 0x0C;
	}
	else
	{
		DelayWait10ms(3);
		GPIO_PORTD_ICR_R = 0xFF;
		GPIO_PORTD_IM_R |= 0x0C;
		
	}
	
}

// Wait for switch to be pressed 
// There will be minimum time delay from //Touch to when this function returns
// Inputs:  none
// Outputs: none 
void Switch_WaitPress(void){
  //while(//Touch==0){}; // wait for press
  //Touch = 0;  // set up for next time
}

// Wait for switch to be released 
// There will be minimum time delay from release to when this function returns
// Inputs:  none
// Outputs: none 
void Switch_WaitRelease(void){
//  while(Release==0){}; // wait
//  Release = 0; // set up for next time
}

// Return current value of the switch 
// Repeated calls to this function may bounce during //Touch and release events
// If you need to wait for the switch, use WaitPress or WaitRelease
// Inputs:  none
// Outputs: false if switch currently pressed, true if released 
unsigned long Switch_Input(void){
  return PF4;
}
