#include "timer.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value


void (*PeriodicTask)(void);   // user function

void Timer1A_Init(void(*task)(void), uint32_t period){
	long sr;
	sr = StartCritical(); 
  SYSCTL_RCGCTIMER_R |= 0x01;   // 0) activate TIMER0
  PeriodicTask = task;          // user function
  TIMER1_CTL_R = 0x00000000;    // 1) disable TIMER0A during setup
  TIMER1_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER1_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER1_TAILR_R = period-1;    // 4) reload value
  TIMER1_TAPR_R = 0;            // 5) bus clock resolution
  TIMER1_ICR_R = 0x00000001;    // 6) clear TIMER0A timeout flag
  TIMER1_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI23_R = (NVIC_PRI23_R&0xFF0FFFFF)|0x00800000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 110, interrupt number 94
  NVIC_EN2_R = 0x40000000;           // 9) enable IRQ 94 in NVIC
  TIMER1_CTL_R = 0x00000001;    // 10) enable wTIMER0A
  EndCritical(sr);
}

void Timer1A_Handler(void)
{
	//acknowledge interrupt
	TIMER1_ICR_R = TIMER_ICR_TATOCINT;
	
	//do whatever you need to do
}
