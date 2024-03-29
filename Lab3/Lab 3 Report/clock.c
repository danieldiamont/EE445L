#include "clock.h"
#include "switches.h"

extern uint32_t alarmhour;
extern uint32_t alarmminute;

bool should_Update;

uint8_t alarmAM;

uint8_t isAM;

// 180 points on a circle of radius 2.000
const int32_t CircleXbuf[180] = {2000, 1999, 1995, 1989, 1981, 1970, 1956, 1941, 1923, 1902, 1879, 1854, 1827, 1798, 1766, 1732, 1696, 1658, 1618, 1576, 1532, 1486, 1439, 1389, 1338, 1286, 1231, 1176, 1118, 1060, 1000, 939, 877, 813, 749, 684, 618, 551, 484, 416, 347, 278, 209, 140, 70, 0, -70, -140, -209, -278, -347, -416, -484, -551, -618, -684, -749, -813, -877, -939, -1000, -1060, -1118, -1176, -1231, -1286, -1338, -1389, -1439, -1486, -1532, -1576, -1618, -1658, -1696, -1732, -1766, -1798, -1827, -1854, -1879, -1902, -1923, -1941, -1956, -1970, -1981, -1989, -1995, -1999, -2000, -1999, -1995, -1989, -1981, -1970, -1956, -1941, -1923, -1902, -1879, -1854, -1827, -1798, -1766, -1732, -1696, -1658, -1618, -1576, -1532, -1486, -1439, -1389, -1338, -1286, -1231, -1176, -1118, -1060, -1000, -939, -877, -813, -749, -684, -618, -551, -484, -416, -347, -278, -209, -140, -70, 0, 70, 140, 209, 278, 347, 416, 484, 551, 618, 684, 749, 813, 877, 939, 1000, 1060, 1118, 1176, 1231, 1286, 1338, 1389, 1439, 1486, 1532, 1576, 1618, 1658, 1696, 1732, 1766, 1798, 1827, 1854, 1879, 1902, 1923, 1941, 1956, 1970, 1981, 1989, 1995, 1999
};
const int32_t CircleYbuf[180] = {0, 70, 140, 209, 278, 347, 416, 484, 551, 618, 684, 749, 813, 877, 939, 1000, 1060, 1118, 1176, 1231, 1286, 1338, 1389, 1439, 1486, 1532, 1576, 1618, 1658, 1696, 1732, 1766, 1798, 1827, 1854, 1879, 1902, 1923, 1941, 1956, 1970, 1981, 1989, 1995, 1999, 2000, 1999, 1995, 1989, 1981, 1970, 1956, 1941, 1923, 1902, 1879, 1854, 1827, 1798, 1766, 1732, 1696, 1658, 1618, 1576, 1532, 1486, 1439, 1389, 1338, 1286, 1231, 1176, 1118, 1060, 1000, 939, 877, 813, 749, 684, 618, 551, 484, 416, 347, 278, 209, 140, 70, 0, -70, -140, -209, -278, -347, -416, -484, -551, -618, -684, -749, -813, -877, -939, -1000, -1060, -1118, -1176, -1231, -1286, -1338, -1389, -1439, -1486, -1532, -1576, -1618, -1658, -1696, -1732, -1766, -1798, -1827, -1854, -1879, -1902, -1923, -1941, -1956, -1970, -1981, -1989, -1995, -1999, -2000, -1999, -1995, -1989, -1981, -1970, -1956, -1941, -1923, -1902, -1879, -1854, -1827, -1798, -1766, -1732, -1696, -1658, -1618, -1576, -1532, -1486, -1439, -1389, -1338, -1286, -1231, -1176, -1118, -1060, -1000, -939, -877, -813, -749, -684, -618, -551, -484, -416, -347, -278, -209, -140, -70
};

const int min_x_coord_LUT[] = {64, 68, 72, 76, 80, 84, 88, 92, 96, 100, 104, 108, 112, 116, 120, 116, 112, 108, 104, 100, 96, 92, 88, 84, 80, 76, 72, 68, 64, 60, 56, 52, 48, 44, 40, 36, 32, 28, 24, 20, 16, 12, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60};

const int min_y_coord_LUT[] = {56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96, 100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140, 144, 148, 152, 156, 160, 164, 168, 172, 148, 144, 140, 136, 132, 128, 124, 120, 116, 112, 108, 104, 100, 96, 92, 88, 84, 80, 76, 72, 68, 64, 60, 56, 52, 48, 44, 40};

const int hr_x_LUT[] =  {64, 77, 80, 90, 80 , 77 , 64 , 50 , 50 , 40, 45, 50};
	
const int hr_y_LUT[] = 	{70, 75, 88, 96, 106, 114, 128, 114, 106, 96, 88, 75};
	
uint32_t hour;
uint32_t minute;

uint16_t hour_x_coord;
uint16_t hour_y_coord;

uint16_t minute_x_coord;
uint16_t minute_y_coord;

uint16_t colorArr[NUM_COLORS] = {ST7735_BLACK, ST7735_RED, ST7735_BLUE, ST7735_WHITE, ST7735_GREEN, ST7735_YELLOW};
uint8_t colorIndex;

void Draw_Clock(){
	
	//get color
	uint16_t color = colorArr[colorIndex];
	
	//background
	ST7735_FillScreen(0xFFFF-color); //fill in background as the negative of @param color
	//draw circle
	ST7735_XYplotInit("",-2000, 2000, -2000, 2000);
  ST7735_XYplot(180,(int32_t *)CircleXbuf,(int32_t *)CircleYbuf, color);
	
	//draw numbers in correct location
	ST7735_SetCursor(10,4);
	ST7735_OutUDec(12,color);	
	ST7735_SetCursor(15,5);
	ST7735_OutUDec(1,color);	
	ST7735_SetCursor(18,7);
	ST7735_OutUDec(2,color);	
	ST7735_SetCursor(19,9);
	ST7735_OutUDec(3,color);	
	ST7735_SetCursor(18,12);
	ST7735_OutUDec(4,color);	
	ST7735_SetCursor(15,14);
	ST7735_OutUDec(5,color);	
	ST7735_SetCursor(11,15);
	ST7735_OutUDec(6,color);
	ST7735_SetCursor(6,14);
	ST7735_OutUDec(7,color);
	ST7735_SetCursor(3,12);
	ST7735_OutUDec(8,color);
	ST7735_SetCursor(2,9);
	ST7735_OutUDec(9,color);
	ST7735_SetCursor(3,7);
	ST7735_OutUDec(10,color);
	ST7735_SetCursor(5,5);
	ST7735_OutUDec(11,color);
	
	ST7735_SetCursor(0,0);
	ST7735_OutUDec(hour,ST7735_YELLOW);
	ST7735_OutChar(':',ST7735_YELLOW);
	if(minute < 10)
		ST7735_OutChar('0',ST7735_YELLOW);
	ST7735_OutUDec(minute,ST7735_YELLOW);
	
	if(isAM == 0)
	{
			ST7735_OutString(" AM",ST7735_YELLOW);
	}
	else
	{
		ST7735_OutString(" PM",ST7735_YELLOW);
	}
	
	//Draw lines
	ST7735_Line(CENTER_X,CENTER_Y,hr_x_LUT[hour],
					hr_y_LUT[hour],color); //draw hour hand
				
	int val = minute;
	if(val == 0) val = 57;
	else if(val == 1) val = 58;
	else if(val == 2) val = 59;
	else
		val = minute - 3;
	
	ST7735_Line(CENTER_X,CENTER_Y,
				min_x_coord_LUT[val],
						min_y_coord_LUT[val],color);//draw minute hand x2>x1 and y1>y2
	
	ST7735_SetCursor(14,2);
	
}
	
void Change_Background(){
	
	colorIndex = (colorIndex + 1) % (NUM_COLORS+1); //update index and wrap around if necessary
	Draw_Clock(); 
}

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

void Set_Time(){ //yet to implement
	
	//figure out the angle for the hour and the angle for the minutes
	while(1){
		
		while(PF0 == 0x01)
		{
			ST7735_SetCursor(0,0);
			ST7735_FillScreen(0);
			ST7735_OutString("\nHour: ",ST7735_YELLOW);
			ST7735_OutUDec(hour, ST7735_YELLOW);
			ST7735_OutString("\nMinute :",ST7735_YELLOW);
			ST7735_OutUDec(minute, ST7735_YELLOW);
			ST7735_OutString("\nAM (0) or PM (1): ",ST7735_YELLOW);
			if(isAM == 0)
			{
				ST7735_OutString("AM",ST7735_YELLOW);
			}
			else
			{
				ST7735_OutString("PM",ST7735_YELLOW);
			}
			ST7735_OutString("\nAlarm Hour :",ST7735_YELLOW);
			ST7735_OutUDec(alarmhour, ST7735_YELLOW);
			ST7735_OutString("\nAlarm Minute :",ST7735_YELLOW);
			ST7735_OutUDec(alarmminute, ST7735_YELLOW);
			ST7735_OutString("\nAM (0) or PM (1): ",ST7735_YELLOW);
			if(alarmAM == 0)
			{
				ST7735_OutString("AM",ST7735_YELLOW);
			}
			else
			{
				ST7735_OutString("PM",ST7735_YELLOW);
			}
		
			pushHour(&hour);
			pushMinute(&minute);
			alarmpushHour();
			alarmpushMinute();
			
			DelayWait10ms(10);
			
		}
		
		if(PF0 == 0x00)
		{
			should_Update = true;
			break;
		}
	}
	
}
	
void Update_Time(){
	if(should_Update == true)
	{
		if(minute == 59)
		{
			if(hour == 11)
			{
				isAM ^= 1;
			}
			hour = (hour+1) % 12;
		}	
		minute = (minute+1) % 60;
	}
	
}
