/* Andres Fuentes and Caelyn Dovey
 * COMP40: Homework #6 UM
 * 4/12/2018
 * ioInstructions.h: Includes the declarations for public i/o functions.
 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

/* Takes a uint32_t word and outputs it to the I/O device. Returns void. Only
 * values from 0-255 (inclusive) are allowed.*/
void output(uint32_t word);

/* Takes no parameters, and returns a uint32_t containing input from the I/O 
 * device. Input must be at most 8 bits.*/
uint32_t input();



