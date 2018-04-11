#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Controller.h"
#include "PWM.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode


//globals used for PID controller
extern uint32_t Period; //24-bit, 12.5 ns units
uint8_t N = 100;			//number of teeth
uint32_t Speed;		//motor speed in 0.1 rps
int32_t E;				//speed error in 0.1 rps
int32_t U;				//duty cycle 40 to 39960
int32_t setPoint; // set point motor speed in 0.1 rps


// **************SysTick_Init*********************
// Initialize SysTick periodic interrupts
// Input: interrupt period
//        Units of period are 12.5ns (assuming 50 MHz clock)
//        Maximum is 2^24-1
//        Minimum is determined by length of ISR
// Output: none
void SysTick_Init(uint32_t period){long sr;
  sr = StartCritical();
  NVIC_ST_CTRL_R = 0;         // disable SysTick during setup
  NVIC_ST_RELOAD_R = period-1;// reload value
  NVIC_ST_CURRENT_R = 0;      // any write to current clears it
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x40000000; // priority 2
                              // enable SysTick with core clock and interrupts
  NVIC_ST_CTRL_R = 0x07;
  EndCritical(sr);
}

void SysTick_Handler(void){
	Speed = 200000000/Period; //0.1 rps
	E = setPoint*10 - Speed;						//0.1 rps
	U = U + ((3*E)>>6);						//discrete integral
	if(U < 40) U = 40;				//constrain output (integral anti-windup)
	if(U > 39960) U = 39960;	// 40 to 39960
	PWM0B_Duty(U);						// adjust the duty cycle of the PWM output accordingly
}

void SetSP(int32_t sp){
	if(sp > 40){
		setPoint = 40;
	}
	else if(sp < 0){
		setPoint = 0;
	}
	else{
		setPoint = sp; //sets in rps
	}
}

int32_t GetSP(){
	return setPoint; //returns in rps
}

void Controller_Init(uint16_t period, uint16_t duty){
	SysTick_Init(4000); // PI algorithm should run ten times faster than motor
	PWM0B_Init(period,duty);
	setPoint = 0;
}



