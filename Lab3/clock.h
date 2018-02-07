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

//hour hand offsets
#define HOUR_X_6_12 64
#define HOUR_X_1_5	74
#define HOUR_X_2_4	84
#define HOUR_X_3	94
#define HOUR_X_7_11 54
#define HOUR_X_8_10 44
#define HOUR_X_9	34

#define HOUR_Y_3_9	96
#define HOUR_Y_4_8	106
#define HOUR_Y_5_7	116
#define HOUR_Y_2_10	86
#define HOUR_Y_1_11	76
#define HOUR_Y_12	66
#define HOUR_Y_6	126

#define MINUTE_X_6_12 64
#define MINUTE_X_1_5	54
#define MINUTE_X_2_4	84
#define MINUTE_X_3	124
#define MINUTE_X_7_11 84
#define MINUTE_X_8_10 54
#define MINUTE_X_9	14

#define MINUTE_Y_3_9	96
#define MINUTE_Y_4_8	106
#define MINUTE_Y_5_7	116
#define MINUTE_Y_2_10	86
#define MINUTE_Y_1_11	76
#define MINUTE_Y_12	36
#define MINUTE_Y_6	146

#define NUM_COLORS 6

void Draw_Clock(void);
void Change_Background(void);

int X_Rotation(int cur_x, int cur_y, int angle);
int Y_Rotation(int cur_x, int cur_y, int angle);

void Set_Time(uint32_t hr, uint32_t min);
void Update_Time(void);

#endif
