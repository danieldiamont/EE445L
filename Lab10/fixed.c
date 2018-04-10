//File Name: fixed.c
//Author(s): Daniel Diamont
//Initial Creation Date: 1/19/2018
//Description: This file contains functions that display numbers on the LCD screen as decimal or binary fixed point.
//						 Additionally, this file contains functions that take a bitmap, converts the data to fixed point, and
//						 displays the images on the LCD screen.
//						 
//Lab Number: 15650
//TA: Saadallah Kassir
//Date of last revision: 1/21/2018
//Hardware Configuration (which pin is connected to what external hardware): refer to Lab1.c

#include "fixed.h"
#include <stdlib.h>
#include <stdbool.h>

//globals
//static int MinXplot = 0;
static int MaxXplot = 127;
static int MinYplot = 32;
static int MaxYplot = 159;

static int RangeX = 0;
static int RangeY = 0;

static int X_min	= 0;
static int Y_max = 0;

/****************ST7735_sDecOut2***************
 converts fixed point number to LCD
 format signed 32-bit with resolution 0.01
 range -99.99 to +99.99
 Inputs:  signed 32-bit integer part of fixed-point number
 Outputs: none
 send exactly 6 characters to the LCD 
Parameter LCD display
 12345    " **.**"
  2345    " 23.45"  
 -8100    "-81.00"
  -102    " -1.02" 
    31    "  0.31" 
-12345    "-**.**"
 */ 
void ST7735_sDecOut2(int32_t n,uint16_t color)
{
	bool isNeg = false; //this flag will later be used to check to see if n is negative or positive
	
	//check to see if the number is in range
	if(n >= -9999 && n <= 9999)
	{
		if (n < 0)
		{
			isNeg = true;
			n = abs(n);
		}
		
		//format data
		int integer;
		int fraction;
		
		//count up number of digits
		int count = 0;
		int n_copy = n;
		while(n_copy > 0)
		{
			n_copy /= 10;
			count++;
		}
		
		int intsToPrint = 1;
		
		if(count > 2)
			intsToPrint = count - 2;

		for(int i = 0; i < 2; i++)
		{
			fraction = 10*fraction + n%10;//collect the fraction
			n /= 10;
		}
		
		for(int k = 0; k < intsToPrint; k++)
		{
			integer = 10*integer + n%10;//collect the integer
			n /= 10;
		}
		
		//send info out to LCD screen
		if(intsToPrint == 1)
			ST7735_OutChar(' ',color);//print out a space
			
		if(isNeg)
			ST7735_OutChar('-',color);
		else
			ST7735_OutChar(' ',color);
		
		for(int j = 0; j < intsToPrint; j++)//print the integer characters
		{
			ST7735_OutChar(0x30 + integer%10,color);
			integer /= 10;
		}
		
		ST7735_OutChar('.',color);
		
		ST7735_OutChar(0x30 + fraction%10,color);//print the fraction characters
		ST7735_OutChar(0x30 + fraction/10,color);		
	}
	
	else //if n is out of range...
	{
		if(n < 0)
			ST7735_OutString("-**.**",color);
		else
			ST7735_OutString(" **.**",color);
	}	
}


/**************ST7735_uBinOut6***************
 unsigned 32-bit binary fixed-point with a resolution of 1/64. 
 The full-scale range is from 0 to 999.99. 
 If the integer part is larger than 256000, it signifies an error. 
 The ST7735_uBinOut6 function takes an unsigned 32-bit integer part 
 of the binary fixed-point number and outputs the fixed-point value on the LCD
 Inputs:  unsigned 32-bit integer part of binary fixed-point number
 Outputs: none
 send exactly 6 characters to the LCD 
Parameter LCD display
     0	  "  0.00"
     1	  "  0.01"
    16    "  0.25"
    25	  "  0.39"
   125	  "  1.95"
   128	  "  2.00"
  1250	  " 19.53"
  7500	  "117.19"
 63999	  "999.99"
 64000	  "***.**"
*/
void ST7735_uBinOut6(uint32_t n,uint16_t color)
{	
	//check to see if the number is in range
	if(n <= 63999)
	{		
		//turn n into a binary fixed point with resolution of 1/64
		uint32_t num = (n*100 + 32)>>6;
		
		//format data before sending to LCD screen
		int integer;
		int fraction;
		
		//count up number of digits
		int count = 0;
		int n_copy = num;
		while(n_copy > 0)
		{
			n_copy /= 10;
			count++;
		}
		
		int intsToPrint = 1;
		
		if(count > 2)
			intsToPrint = count - 2;
		
		for(int i = 0; i < 2; i++)
		{
			fraction = 10*fraction + num%10;//collect the fraction
			num /= 10;
		}
		
		for(int j = 0; j < intsToPrint; j++)
		{
			integer = 10*integer + num%10;//ceollect the integer
			num /= 10;
		}
		
		//send out info to LCD screen
		ST7735_OutChar(' ',color);//print out space
		
		if(intsToPrint < 3)
			for(int i = 0; i < 3-intsToPrint; i++)
				ST7735_OutChar(' ',color);
		
		for(int x = 0; x < intsToPrint; x++)
		{
			ST7735_OutChar(0x30 + integer%10,color);//print out the integer characters
			integer /= 10;
		}
		ST7735_OutChar('.',color);
		ST7735_OutChar(0x30 + fraction%10,color);//print out the fraction characters
		ST7735_OutChar(0x30 + fraction/10,color);
		
	}
	
	else
	{
		ST7735_OutString(" ***.**",color);
	}
}

/**************ST7735_XYplotInit***************
 Specify the X and Y axes for an x-y scatter plot
 Draw the title and clear the plot area
 Inputs:  title  ASCII string to label the plot, null-termination
          minX   smallest X data value allowed, resolution= 0.001
          maxX   largest X data value allowed, resolution= 0.001
          minY   smallest Y data value allowed, resolution= 0.001
          maxY   largest Y data value allowed, resolution= 0.001
 Outputs: none
 assumes minX < maxX, and miny < maxY
*/
void ST7735_XYplotInit(char *title, int32_t minX, int32_t maxX, int32_t minY, int32_t maxY)
{
	//black screen
	//ST7735_FillScreen(color);
	ST7735_SetCursor(0,0);
	
	//draw the title
	//ST7735_OutString(title);
	
	//clear plot area
	//ST7735_PlotClear(minY,maxY);
	
	//set the bounds for the plot
//	MinXplot = minX;
	MinYplot = MinYplot + minY;
	MaxXplot = maxX;
	MaxYplot = MaxYplot + maxY;//check this line
//maxYplot = minYplot + maxY; //this is not working
	
	//set the range for the fixed point conversion
	RangeX = maxX-minX;
	RangeY = maxY-minY;
	
	//set the offsets for the fixed point conversion
	X_min = minX;
	Y_max = maxY;
	
}

/**************ST7735_XYplot***************
 Plot an array of (x,y) data
 Inputs:  num    number of data points in the two arrays
          bufX   array of 32-bit fixed-point data, resolution= 0.001
          bufY   array of 32-bit fixed-point data, resolution= 0.001
 Outputs: none
 assumes ST7735_XYplotInit has been previously called
 neglect any points outside the minX maxY minY maxY bounds
*/
void ST7735_XYplot(uint32_t num, int32_t bufX[], int32_t bufY[],uint16_t color)
{	
	for(int x = 0; x < num; x++)//for all items in each buffer...
	{
		if(bufX[x] < MaxXplot && bufY[x] < MaxYplot)//if the data is within the bounds...
		{
			//draw four points
			ST7735_DrawPixel( ((bufX[x]-X_min)*127)/RangeX, 32+(((Y_max-bufY[x])*127)/RangeY), color);
			ST7735_DrawPixel( 1+((bufX[x]-X_min)*127)/RangeX, 32+(((Y_max-bufY[x])*127)/RangeY), color);
			ST7735_DrawPixel( ((bufX[x]-X_min)*127)/RangeX, 1+32+(((Y_max-bufY[x])*127)/RangeY), color);
			ST7735_DrawPixel( 1+((bufX[x]-X_min)*127)/RangeX, 1+32+(((Y_max-bufY[x])*127)/RangeY), color);
		}
			
	}
	//reset the bounds
	//MinXplot = 0;
	MinYplot = 32;
	MaxXplot = 127;
	MaxYplot = 159;

}
