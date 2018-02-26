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
#include <stdbool.h>
#include "Timer1A.h"
#include "Timer0A.h"

// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

Song songs[1] = {
	{{E,E,F,F,G,G,G,G,F,F,E,E,D,D,C,C,C,C,D,D,E,E,E,E,E,D,D,D,D,E,E,
		E,E,F,F,G,G,G,G,F,F,E,E,D,D,C,C,C,C,D,D,E,E,D,D,D,D,C,C,C,C,D,D,D,D,
		E,E,C,C,D,D,E,F,E,E,C,C,D,D,E,F,E,E,D,D,C,C,D,D,G*2,G*2,E,E,E,E,E,E,F,F,G,G,
		G,G,F,F,E,E,D,D,C,C,C,C,D,D,E,E,D,D,D,D,C,C,C,C,E,E},
	{MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,
		MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,
		MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,
		MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,
		MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,
		MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,
		MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,
		MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,
		MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,
		MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,
		MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,
		MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,
		MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,
		MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,
		MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,
		MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,
		MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,
		MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,
		MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,
		MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,
		MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,
		MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,
		MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,
		MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,
		MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,
		MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,
		MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,
		MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,
		MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,
		MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,
		MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,
		MAX_RELOAD_VAL,MAX_RELOAD_VAL,MAX_RELOAD_VAL,}}}; //start out with only one song

//basic waveform for one note
const uint16_t wave[32] = {
  2048*2,2448*2,2832*2,3186*2,3496*2,3751*2,3940*2,4057*2,4095*2,4057*2,3940*2,
  3751*2,3496*2,3186*2,2832*2,2448*2,2048*2,1648*2,1264*2,910*2,600*2,345*2,
  156*2,39*2,0*2,39*2,156*2,345*2,600*2,910*2,1264*2,1648*2};

uint8_t noteIndex;
uint8_t songIndex;
uint8_t instrumentIndex;
Song * song_ptr;
const uint16_t * instrument_ptr;
uint8_t instrument_len;

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
		DAC_Init(0x1000); //init DAC and set to voltatge reference value
		SysTick_Init(A);//init value for SysTick
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
	instrument_ptr = wave;
	instrument_len = 32;

	Timer1A_Init(Sound_Update_Note, songs[songIndex].time[noteIndex]); //init note change timer
	Sound_Play_Note(songs[songIndex].note[noteIndex]); //init waveform timer
	playSong = true;	//set playSong flag to true

}

void Sound_Update_Note(){
	//check flags???
	noteIndex = (noteIndex + 1) % NUM_NOTES; //change to the next note
	Sound_Play_Note(songs[songIndex].note[noteIndex]);
	TIMER1_TAILR_R = songs[songIndex].time[noteIndex]-1;
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





