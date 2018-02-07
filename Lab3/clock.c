#include "clock.h"


// 180 points on a circle of radius 2.000
const int32_t CircleXbuf[180] = {2000, 1999, 1995, 1989, 1981, 1970, 1956, 1941, 1923, 1902, 1879, 1854, 1827, 1798, 1766, 1732, 1696, 1658, 1618, 1576, 1532, 1486, 1439, 1389, 1338, 1286, 1231, 1176, 1118, 1060, 1000, 939, 877, 813, 749, 684, 618, 551, 484, 416, 347, 278, 209, 140, 70, 0, -70, -140, -209, -278, -347, -416, -484, -551, -618, -684, -749, -813, -877, -939, -1000, -1060, -1118, -1176, -1231, -1286, -1338, -1389, -1439, -1486, -1532, -1576, -1618, -1658, -1696, -1732, -1766, -1798, -1827, -1854, -1879, -1902, -1923, -1941, -1956, -1970, -1981, -1989, -1995, -1999, -2000, -1999, -1995, -1989, -1981, -1970, -1956, -1941, -1923, -1902, -1879, -1854, -1827, -1798, -1766, -1732, -1696, -1658, -1618, -1576, -1532, -1486, -1439, -1389, -1338, -1286, -1231, -1176, -1118, -1060, -1000, -939, -877, -813, -749, -684, -618, -551, -484, -416, -347, -278, -209, -140, -70, 0, 70, 140, 209, 278, 347, 416, 484, 551, 618, 684, 749, 813, 877, 939, 1000, 1060, 1118, 1176, 1231, 1286, 1338, 1389, 1439, 1486, 1532, 1576, 1618, 1658, 1696, 1732, 1766, 1798, 1827, 1854, 1879, 1902, 1923, 1941, 1956, 1970, 1981, 1989, 1995, 1999
};
const int32_t CircleYbuf[180] = {0, 70, 140, 209, 278, 347, 416, 484, 551, 618, 684, 749, 813, 877, 939, 1000, 1060, 1118, 1176, 1231, 1286, 1338, 1389, 1439, 1486, 1532, 1576, 1618, 1658, 1696, 1732, 1766, 1798, 1827, 1854, 1879, 1902, 1923, 1941, 1956, 1970, 1981, 1989, 1995, 1999, 2000, 1999, 1995, 1989, 1981, 1970, 1956, 1941, 1923, 1902, 1879, 1854, 1827, 1798, 1766, 1732, 1696, 1658, 1618, 1576, 1532, 1486, 1439, 1389, 1338, 1286, 1231, 1176, 1118, 1060, 1000, 939, 877, 813, 749, 684, 618, 551, 484, 416, 347, 278, 209, 140, 70, 0, -70, -140, -209, -278, -347, -416, -484, -551, -618, -684, -749, -813, -877, -939, -1000, -1060, -1118, -1176, -1231, -1286, -1338, -1389, -1439, -1486, -1532, -1576, -1618, -1658, -1696, -1732, -1766, -1798, -1827, -1854, -1879, -1902, -1923, -1941, -1956, -1970, -1981, -1989, -1995, -1999, -2000, -1999, -1995, -1989, -1981, -1970, -1956, -1941, -1923, -1902, -1879, -1854, -1827, -1798, -1766, -1732, -1696, -1658, -1618, -1576, -1532, -1486, -1439, -1389, -1338, -1286, -1231, -1176, -1118, -1060, -1000, -939, -877, -813, -749, -684, -618, -551, -484, -416, -347, -278, -209, -140, -70
};

static uint32_t hour;
static uint32_t minute;

uint16_t hour_x_coord;
uint16_t hour_y_coord;

uint16_t minute_x_coord;
uint16_t minute_y_coord;

int hour_angle; //in radians
int minute_angle; //in radians

int X_Rotation(int cur_x, int cur_y, int angle){
		int x = cos(angle)*(cur_x-CENTER_X) - sin(angle)*(cur_y-CENTER_Y) + CENTER_X;
		
		return x;
}
int Y_Rotation(int cur_x, int cur_y, int angle){
		int y = sin(angle)*(cur_x-CENTER_X) + cos(angle)*(cur_y-CENTER_Y) + CENTER_Y;
	
	return y;
}

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
	ST7735_SetCursor(17,7);
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
	
	//Draw lines
	ST7735_Line(CENTER_X,CENTER_Y,
				X_Rotation(hour_x_coord,hour_y_coord,hour_angle),
					Y_Rotation(hour_x_coord,hour_y_coord,hour_angle),color); //draw hour hand
				
	ST7735_Line(CENTER_X,CENTER_Y,
				X_Rotation(minute_x_coord,minute_y_coord,minute_angle),
					Y_Rotation(minute_x_coord,minute_y_coord,minute_angle),color);//draw minute hand x2>x1 and y1>y2
	
	ST7735_SetCursor(14,2);
	
}

void Change_Background(){
	
	colorIndex = (colorIndex + 1) % (NUM_COLORS+1); //update index and wrap around if necessary
	Draw_Clock(); 
}

void Set_Time(uint32_t hr, uint32_t min){ //yet to implement
	
	//figure out the angle for the hour and the angle for the minutes
	
}
	
void Update_Time(){
	
	pushHour(&hour);
	pushMinute(&minute);
	
}
