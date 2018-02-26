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
#define GPIO_PIN_2              (*((volatile uint32_t *)0x00000004))  // GPIO pin 2
#define GPIO_PIN_3              (*((volatile uint32_t *)0x00000008))  // GPIO pin 3
#define GPIO_PORTD_BASE         (*((volatile uint32_t *)0x40007000))  // GPIO Port D
#define PD2 										(*((volatile uint32_t *)(GPIO_PIN_2 | GPIO_PORTD_BASE)))
#define PD3											(*((volatile uint32_t *)(GPIO_PIN_3 | GPIO_PORTD_BASE)))

static void GPIOArm(void){
  GPIO_PORTF_ICR_R = 0x11;      // (e) clear flag4 and flag0
  GPIO_PORTF_IM_R |= 0x11;      // (f) arm interrupt on PF4 and PF0 *** No IME bit as mentioned in Book ***
  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF00FFFF)|0x00A00000; // (g) priority 5
  NVIC_EN0_R = 0x40000000;      // (h) enable interrupt 30 in NVIC  
}
// Initialize switch interface on PF4 and PF0
void Switch_Init(){
  // **** general initialization ****
  SYSCTL_RCGCGPIO_R |= 0x00000020; // (a) activate clock for port F
  while((SYSCTL_PRGPIO_R & 0x00000020) == 0){};
	
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
  GPIO_PORTF_PCTL_R &= ~0x0000000F; // configure PF0 as GPIO
  GPIO_PORTF_AMSEL_R = 0;       //     disable analog functionality on PF
  GPIO_PORTF_PUR_R |= 0x01;     //     enable weak pull-up on PF0
  GPIO_PORTF_IS_R &= ~0x01;     // (d) PF0 is edge-sensitive
  GPIO_PORTF_IBE_R |= 0x01;     //     PF0 is both edges

  GPIO_PORTD_DIR_R &= ~0x0C;    // (c) make PD2 PD3 in (built-in button)
  GPIO_PORTD_AFSEL_R &= ~0x0C;  //     disable alt funct on PD23
  GPIO_PORTD_DEN_R |= 0x0C;     //     enable digital I/O on PD23   
  GPIO_PORTD_PCTL_R &= ~0x0000FF00; // configure PD23 as GPIO
  GPIO_PORTD_AMSEL_R = 0;       //     disable analog functionality on PD
  GPIO_PORTD_PUR_R |= 0x0C;     //     enable weak pull-up on PD23
  GPIO_PORTD_IS_R &= ~0x0C;     // (d) PD23 is edge-sensitive
  GPIO_PORTD_IBE_R |= 0x0C;     //     PD23 is both edges
  //PD2, and PD3

  GPIOArm();

  SYSCTL_RCGCTIMER_R |= 0x01;   // 0) activate TIMER0
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
  
  GPIO_PORTD_IM_R &= ~0x0C;     // disarm interrupt on all of port D
  //if(LastP){    // 0x11 means either PF4 or PF0 was previously released
    //Touch = 1;       // //Touch occurred
//    (*TouchTask)();  // execute user task
  }
  //else{
//    Release = 1;       // release occurred
   // (*ReleaseTask)();  // execute user task
  //}
//  Timer0Arm(); // start one shot
//}
// Interrupt 10 ms after rising edge of PF4
void Timer0A_Handler(void){
  TIMER0_IMR_R = 0x00000000;    // disarm timeout interrupt
  //Last = PF4;  // switch state
  GPIOArm();   // start GPIO
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
