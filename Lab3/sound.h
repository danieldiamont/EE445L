#ifndef __SOUND_H__
#define __SOUND_H__

#include "inc/hw_sysctl.h"
#include "driverlib/pin_map.h"
#include "inc/hw_memmap.h"
//#include "inc/hw_ints.h"
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

void SysTick_Init(void);
void SysTick_Handler(void);

#endif
