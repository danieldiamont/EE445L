#ifndef __CLOCK_H__
#define __CLOCK_H__

#include <stdint.h>
#include <math.h>
#include "switches.h"
#include "ST7735.h"
#include <stdbool.h>
#include "fixed.h"

#define CENTER_X 64
#define CENTER_Y 96

#define PF1             (*((volatile uint32_t *)0x40025008))

#define NUM_COLORS 6

void Draw_Clock(void);
void Change_Background(void);

int X_Rotation(int cur_x, int cur_y);
int Y_Rotation(int cur_x, int cur_y);

void Set_Time(uint32_t hr, uint32_t min);
void Update_Time(void);

#endif
