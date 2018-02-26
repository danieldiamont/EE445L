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
//const uint16_t wave[32] = {
//  2048*2,2448*2,2832*2,3186*2,3496*2,3751*2,3940*2,4057*2,4095*2,4057*2,3940*2,
//  3751*2,3496*2,3186*2,2832*2,2448*2,2048*2,1648*2,1264*2,910*2,600*2,345*2,
//  156*2,39*2,0*2,39*2,156*2,345*2,600*2,910*2,1264*2,1648*2};
		
//const uint16_t wave[32] = {4096, 1801, 2083, 2344, 2572, 2759, 2898, 
//	2984, 3012, 2984, 2898, 1015, 946, 862, 766, 662, 554, 446, 342, 
//	246, 162, 34, 15, 3, 0, 3, 15, 34, 59, 90, 125};

//const uint16_t wave[32] = {4096, 1801, 2083, 2344, 2572, 2759, 2898, 
//	2984, 3012, 1098, 1066, 1015, 946, 862, 766, 662, 554, 164, 125, 90,
//	59, 34, 15, 3, 0, 1, 5, 12, 21, 33, 46};

const uint16_t wave[256]={4096, 1501, 1484, 1456, 1417, 1369, 1311, 1245, 1171, 1092, 1007, 918, 
	827, 734, 642, 552, 465, 382, 304, 233, 171, 116, 72, 38, 14, 2, 0, 11, 32, 64, 107, 159, 220,
	289, 366, 448, 534, 624, 716, 808, 900, 989, 1075, 1156, 1231, 1299, 1358, 1408, 1449, 1479, 1498,
	1506, 1503, 1488, 1462, 1426, 1379, 1323, 1259, 1187, 1108, 1024, 936, 845, 753, 661, 570, 482, 398,
	319, 247, 182, 126, 80, 44, 18, 3, 0, 8, 27, 57, 97, 148, 207, 275, 350, 431, 517, 606, 697, 790, 882,
	972, 1058, 1140, 1216, 1286, 1347, 1399, 1442, 1474, 1495, 1505, 1504, 1492, 1468, 1434, 1389, 1335,
	1272, 1202, 1124, 1041, 954, 863, 771, 679, 588, 499, 414, 334, 261, 195, 137, 88, 50, 22, 5, 0, 5,
	22, 50, 88, 137, 195, 261, 334, 414, 499, 588, 679, 771, 863, 954, 1041, 1124, 1202, 1272, 1335, 1389,
	1434, 1468, 1492, 1504, 1505, 1495, 1474, 1442, 1399, 1347, 1286, 1216, 1140, 1058, 972, 882, 790, 697,
	606, 517, 431, 350, 275, 207, 148, 97, 57, 27, 8, 0, 3, 18, 44, 80, 126, 182, 247, 319, 398, 482, 570, 
	661, 753, 845, 936, 1024, 1108, 1187, 1259, 1323, 1379, 1426, 1462, 1488, 1503, 1506, 1498, 1479, 1449,
	1408, 1358, 1299, 1231, 1156, 1075, 989, 900, 808, 716, 624, 534, 448, 366, 289, 220, 159, 107, 64, 32,
	11, 0, 2, 14, 38, 72, 116, 171, 233, 304, 382, 465, 552, 642, 734, 827, 918, 1007, 1092, 1171, 1245, 
	1311, 1369, 1417, 1456, 1484};


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
	noteIndex = (noteIndex + 1) % NUM_NOTES; //change to the next note
	Sound_Play_Note(songs[songIndex].note[noteIndex]);
	TIMER1_TAILR_R = songs[songIndex].time[noteIndex]-1;
}

void Sound_Pause_Song(){
	//set flag to false
	playSong = false;
	TIMER1_CTL_R = 0x00000000;
}

void Sound_Resume_Song(){
	//set flag to true
	playSong = true;
	TIMER1_CTL_R = 0x00000001;
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





