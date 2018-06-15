/* Andres Fuentes and Caelyn Dovey
 * COMP40: Homework #6 UM
 * 4/12/2018
 * memory.h: Includes the declarations for public memory functions.
 */

#include <stdio.h>
#include <stdlib.h>
#include <seq.h>
#include <stack.h>
#include <assert.h>
#include <bitpack.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/* Takes no parameters and returns void. Starts running the program with segID 
   0*/
void load_init_program(char *umProgram);

/* Creates a segment containing numWords number of 32bit words and initializes 
   them to 0. Returns void.*/
uint32_t map_segment(unsigned numWords);

/* Unmaps segment identified by segID. Returns void.  */
void unmap_segment(uint32_t segID);

/* Takes a segment identified by segID, and an int offset within the segment, 
   and returns the word located at that address.*/
uint32_t segmented_load(uint32_t segID, unsigned offset);

/* Takes a uint32_t word, an unsigned segment ID, and an unsigned offset, and 
   returns void. Inserts the word into the segment identified by the segment ID
   at the specified offset.*/
void segmented_store(uint32_t word, uint32_t segID, unsigned offset);

/* Takes an unsigned segment ID and an offset value and returns void. Runs the 
   program stored in segment ID starting the given offset.*/
void load_program(uint32_t segID);

/* Takes no parameters, returns void. Frees all memory associated with um and 
   exits success. Called by halt and run_init_program. */
void terminate_um();

