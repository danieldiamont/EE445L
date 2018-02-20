// Sound.c
// This module contains the SysTick ISR that plays sound
// Runs on LM4F120 or TM4C123
// Program written by: Daniel Diamont and Karim Sabar
// Date Created: 3/6/17 
// Last Modified: 3/13/17 
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********

// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data
#include <stdint.h>
#include "MAX5353.h"
#include "tm4c123gh6pm.h"
#include "SysTickInts.h"
#include "Sound.h"
#include "timer.h"
#include <stdbool.h>
#include "Timer1A.h"
#include "Timer0A.h"


// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

Song songs[1] = {{D,E,F,G},{10000,10000,10000}}; //start out with only one song

//basic waveform for one note
volatile uint8_t wave[32] = {8,9,11,12,13,14,14,15,15,15,14,14,13,12,11,9,8,7,5,4,3,2,2,1,1,1,2,2,3,4,5,7};

uint8_t noteIndex;
uint8_t songIndex;
uint8_t instrumentIndex;
bool playSong;

// **************Sound_Init*********************
// Initialize Systick periodic interrupts
// Called once, with sound initially off
// Input: interrupt period
//           Units to be determined by YOU
//           Maximum to be determined by YOU
//           Minimum to be determined by YOU
// Output: none
void Sound_Init(void){
		DAC_Init(0); //init DAC and set to constant value
}


// **************Sound_Play*********************
// Start sound output, and set Systick interrupt period 
// Input: interrupt period
//           Units to be determined by YOU
//           Maximum to be determined by YOU
//           Minimum to be determined by YOU
//         input of zero disable sound output
// Output: none
void Sound_Play_Note(uint32_t period){ //basically sets the note by adjusting systick
	
	NVIC_ST_RELOAD_R = period-1;	
}

//set the song
void Sound_Play_Song(uint8_t song, uint8_t instrument){
	songIndex = song; //set index of song
	song_ptr = &(songs[songIndex]);

	noteIndex = 0;		//set initial noteIndex

	//set instrument	
	instrumentIndex = instrument;

	Timer1A_Init(Sound_Update_Note, songs[songIndex].time[noteIndex]); //init note change timer
	Sound_Play_Note(songs[songIndex].note[noteIndex]); //init waveform timer
	playSong = true;	//set playSong flag to true

}

void Sound_Update_Note(){
	//check flags???
	noteIndex = (noteIndex + 1) % NUM_NOTES; //change to the next note
	Sound_Play_Note(songs[songIndex].note[noteIndex]);
	TIMER1_TAILR_R = songs[songIndex].time[noteIndex]);
}

void Sound_Pause_Song(){
	//set flag to false
	playSong = false;
}

void Sound_Resume_Song(){
	//set flag to true
	playSong = true;
}

void Sound_Rewind_Song(){
	noteIndex = 0;
}

void Sound_Change_Instrument(uint8_t instrument){
	//change instrument
	instrumentIndex = instrument;
	//instrument_ptr
	//instrument_len
}





