// FiFo.c
// Runs on LM4F120/TM4C123
// Provide functions that implement the Software FiFo Buffer

#include <stdint.h>


#define SIZE 9				//need a size 9 buffer because it's supposed to hold 8 elements
int32_t static PutI; //index
int32_t static GetI; //index
uint32_t static counter;    //counter keeps track of how full the SW FIFO is
int32_t static FIFO[SIZE]; //declare SW fifo

// *********** FiFo_Init**********
// Initializes a software FIFO of a
// fixed size and sets up indexes for
// put and get operations
void FiFo_Init() {
	PutI = GetI = 1; // 1 <= <index> <= 8
	counter = 0;		//keeps track of size
}

// *********** FiFo_Put**********
// Adds an element to the FIFO
// Input: Character to be inserted
// Output: 1 for success and 0 for failure
//         failure is when the buffer is full
uint32_t FiFo_Put(uint32_t data) {
	if (counter == SIZE-1) return 0; //checks to see if the FIFO is full
	
	FIFO[PutI] = data; //save the data into the SW fifo
	PutI = (PutI+1)%SIZE; //increase index; use % to wrap around if necessary
	counter++;
	return 1;
}

// *********** FiFo_Get**********
// Gets an element from the FIFO
// Input: Pointer to a character that will get the character read from the buffer
// Output: 1 for success and 0 for failure
//         failure is when the buffer is empty
uint32_t FiFo_Get(uint32_t *datapt)
{
	if (counter == 0) return 0;	//returns 0 if the buffer is empty
	*datapt = FIFO[GetI];				//else, m[datapt] = FIFO[GetI];
	GetI = (GetI+1)%SIZE;				//increase index and wrap around if necessary
	counter--;
	return 1;
}
