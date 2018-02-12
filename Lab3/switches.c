#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "switches.h"

//static uint32_t isPushedHour=0;
//static uint32_t wasPushedHour=0;
//static uint32_t isPushedMinute=0;
//static uint32_t wasPushedMinute=0;

static uint32_t alarmbutton=0;

uint32_t alarmhour=0;
uint32_t alarmminute=0;

extern uint8_t play_Flag;


void delay1ms0(uint32_t n){uint32_t volatile time;
  while(n){
    time = 72724*2/91;  // 1msec, tuned at 80 MHz
    while(time){
	  	time--;
    }
    n--;
  }
}

void PortD_Init(void)
{
//this may or may not be correct
	volatile unsigned long delay;
	SYSCTL_RCGCGPIO_R |= 0x00000020;     // 1) activate clock for Port F
  	delay = SYSCTL_RCGCGPIO_R;           // NOP equivalent 
	GPIO_PORTF_DIR_R &= ~0x70;//was 0x60 for 2 ports, now 3
	GPIO_PORTF_AFSEL_R &=~0x70;
	GPIO_PORTF_DEN_R |= 0x70;
	
	SYSCTL_RCGCGPIO_R |=0x00000008; //port d
	delay = SYSCTL_RCGCGPIO_R;
	GPIO_PORTD_DIR_R &= ~0x0C;
	GPIO_PORTD_AFSEL_R &= ~0x0C; //pins PD2 and PD3
	GPIO_PORTD_DEN_R |= 0x0C;
}

void pushHour(uint32_t *hour)
{
	alarmbutton=(GPIO_PORTF_DATA_R>>4)&0x01;
	if(alarmbutton==0) return;
	delay1ms0(300);
//	wasPushedHour=isPushedHour;
//	isPushedHour=(GPIO_PORTD_DATA_R>>3)&0x01;
//	*hour=(*hour+(isPushedHour*(1-wasPushedHour)))%13;	
	if(((GPIO_PORTD_DATA_R>>2)&0x01) == 1){
		*hour = (*hour+1) % 13;
	}
}

void pushMinute(uint32_t * minute)
{
	alarmbutton=(GPIO_PORTF_DATA_R>>4)&0x01;
	if(alarmbutton==0) return;
	delay1ms0(300);
//	wasPushedMinute=isPushedMinute;
//	isPushedMinute=(GPIO_PORTD_DATA_R>>4)&0x01;
//	*minute=(*minute+(isPushedMinute*(1-wasPushedMinute)))%13;
		if(((GPIO_PORTD_DATA_R>>3)&0x01) == 1){
		*minute = (*minute+1) % 60;
	}
}

void alarmpushHour(void)
{
	alarmbutton=(GPIO_PORTF_DATA_R>>4)&0x01;
	if(alarmbutton==1) return;
	delay1ms0(300);
//	wasPushedHour=isPushedHour;
//	isPushedHour=(GPIO_PORTD_DATA_R>>3)&0x01;
//	alarmhour=(alarmhour+(isPushedHour*(1-wasPushedHour)))%13;
 if(((GPIO_PORTD_DATA_R>>2)&0x01) == 1){
		alarmhour = (alarmhour+1) % 13;
	}	
}

void alarmpushMinute(void)
{
	alarmbutton=(GPIO_PORTF_DATA_R>>4)&0x01;
	if(alarmbutton==1) return;
	delay1ms0(300);
//	wasPushedMinute=isPushedMinute;
//	isPushedMinute=(GPIO_PORTD_DATA_R>>4)&0x01;
//	alarmminute=(alarmminute+(isPushedMinute*(1-wasPushedMinute)))%13;	
	if(((GPIO_PORTD_DATA_R>>3)&0x01) == 1){
		alarmminute = (alarmminute+1) % 60;
	}	
}

void setOffAlarm(int hour, int minute)
{
	if(hour==alarmhour && minute==alarmminute)
	{
		play_Flag=1;
	}
	else
	{
		play_Flag=0;
	}

}

void displayAlarmTime(void) //This needs to be finshed, I'm tryint ofigure out how to display onto ST7735 appropriately
{
	alarmbutton=(GPIO_PORTF_DATA_R>>4)&0x01;
	if(alarmbutton==0) //display blank
	{
		
	}
	else //display time
	{
		
	}
}
