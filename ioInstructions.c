/* Andres Fuentes and Caelyn Dovey
 * COMP40: Homework #6 UM
 * 4/12/2018
 * ioInstructions.c: Includes the implementation for public i/o functions.
 */

#include "ioInstructions.h"

/* Takes a uint32_t word and outputs it to the I/O device. Returns void. */
void output(uint32_t word)
{
        fprintf(stdout, "%c", word);
}

/* Takes no parameters, and returns a uint32_t containing input from the I/O 
 * device. Input must be at most 8 bits.*/
uint32_t input()
{
        uint32_t zero = 0;
        char input = (char)fgetc(stdin);
        if (input != EOF) {
                return (uint32_t) input;
        }
        else {
                return ~zero;
        }
}
