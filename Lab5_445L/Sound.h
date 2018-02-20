// Sound.h
// This module contains the SysTick ISR that plays sound
// Runs on LM4F120 or TM4C123
// Program written by: put your names here
// Date Created: 3/6/17 
// Last Modified: 3/6/17 
// Lab number: 6
// Hardware connections
#include <stdint.h>
// Header files contain the prototypes for public functions
// this file explains what the module does

//data structure for music
#define NUM_NOTES 3
	
typedef const struct {
	uint32_t note[NUM_NOTES];
	uint32_t time[NUM_NOTES];
} Song;

Song * song_ptr;
uint8_t * instrument_ptr;
uint8_t instrument_len;

// **************Sound_Init*********************
// Initialize Systick periodic interrupts
// Called once, with sound initially off
// Input: interrupt period
//           Units to be determined by YOU
//           Maximum to be determined by YOU
//           Minimum to be determined by YOU
// Output: none
void Sound_Init(void);


// **************Sound_Play*********************
// Start sound output, and set Systick interrupt period 
// Input: interrupt period
//           Units to be determined by YOU
//           Maximum to be determined by YOU
//           Minimum to be determined by YOU
//         input of zero disable sound output
// Output: none
void Sound_Play_Note(uint32_t period);
#define C    2389   // 1046.5 Hz
#define B    2531   // 987.8 Hz
#define BF   2681   // 932.3 Hz
#define A    2841   // 880 Hz
#define AF   3010   // 830.6 Hz
#define G    3189   // 784 Hz
#define GF  3378   // 740 Hz
#define F   3579   // 698.5 Hz
#define E   3792   // 659.3 Hz
#define EF  4018   // 622.3 Hz
#define D   4257   // 587.3 Hz
#define DF  4510   // 554.4 Hz

//pass the sound controller the index of the song to play (assuming we have multiple songs)
void Sound_Play_Song(uint8_t song, uint8_t instrument);
void Sound_Pause_Song(void);
void Sound_Rewind_Song(void);
void Sound_Change_Instrument(uint8_t index);
void Sound_Update_Note(void);
void Sound_Resume_Song(void);
