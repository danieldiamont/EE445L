#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "SysTickInts.h"
#include <stdbool.h>
#include "MAX5353.h"

#define NVIC_ST_CTRL_CLK_SRC    0x00000004  // Clock Source
#define NVIC_ST_CTRL_INTEN      0x00000002  // Interrupt enable
#define NVIC_ST_CTRL_ENABLE     0x00000001  // Counter mode

extern bool playSong;
extern const uint16_t * instrument_ptr;
extern uint32_t instrument_len;


uint16_t DAC_Index;

//	const uint16_t wave[32] = {
//  2048,2448,2832,3186,3496,3751,3940,4057,4095,4057,3940,
//  3751,3496,3186,2832,2448,2048,1648,1264,910,600,345,
//  156,39,0,39,156,345,600,910,1264,1648};

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
	if (!playSong)
	{
		return;
	}
	else {
		//play the data from the correct data structure
		DAC_Out(instrument_ptr[DAC_Index]);
		DAC_Index = (DAC_Index + 1) % 4096;
		//sound test
//		DAC_Out(wave[DAC_Index&0x1F]);
//		DAC_Index++;
	}
	
}
