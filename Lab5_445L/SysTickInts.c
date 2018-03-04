#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "SysTickInts.h"
#include <stdbool.h>
#include "MAX5353.h"

#define NVIC_ST_CTRL_CLK_SRC    0x00000004  // Clock Source
#define NVIC_ST_CTRL_INTEN      0x00000002  // Interrupt enable
#define NVIC_ST_CTRL_ENABLE     0x00000001  // Counter mode
#define PF2                     (*((volatile uint32_t *)0x40025010))

//globals used for sound generation
extern bool playSong;
extern const uint16_t * instrument_ptr;
extern uint32_t instrument_len;
extern void Debug_Dump(void);
uint16_t DAC_Index;

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

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
  NVIC_ST_CTRL_R = NVIC_ST_CTRL_ENABLE+NVIC_ST_CTRL_CLK_SRC+NVIC_ST_CTRL_INTEN;
  DAC_Index = 0;
  EndCritical(sr);
}

void SysTick_Handler(void)
{
	//toggle PF2 for debugging on scope
	//PF2 ^= 0x04;
	//PF2 ^= 0x04;
	
	if (!playSong)
	{
		return;
	}
	else {
		//Debug_Dump();
		//play the data from the correct data structure
		DAC_Out(instrument_ptr[DAC_Index]);
		DAC_Index = (DAC_Index + 1) % 4096;
		//sound test
//		DAC_Out(wave[DAC_Index&0x1F]);
//		DAC_Index++;
		//Debug_Dump(); //minimally instrusive debugging software dump
	}
	
	//PF2 ^= 0x04; //toggle LED for debugging
	
}
