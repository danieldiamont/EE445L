#ifndef __CONTROLLER_H__ // do not include more than once
#define __CONTROLLER_H__

// **************SysTick_Init*********************
// Initialize Systick periodic interrupts
// Input: interrupt period
//        Units of period are 12.5ns (assuming 50 MHz clock)
//        Maximum is 2^24-1
//        Minimum is determined by lenght of ISR
// Output: none
void SysTick_Init(uint32_t period);

// Interrupt service routine
// Executed every 20ns*(period)
void SysTick_Handler(void);

//getters and setters
int32_t GetSP(void);
void SetSP(int32_t setPoint);

void Controller_Init(uint16_t period, uint16_t duty);

#endif // __CONTROLLER_H__
