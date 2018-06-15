/* Andres Fuentes and Caelyn Dovey
 * COMP40: Homework #6 UM
 * 4/12/2018
 * mathinstructions.h: Includes the declarations for public math functions.
 */
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

/* Takes two uint32_t words and returns their sum as a uint32_t.*/
uint32_t addition(uint32_t wordA, uint32_t wordB);

/* Takes two uint32_t words and returns their product as a uint32_t.*/
uint32_t multiplication(uint32_t wordA, uint32_t wordB);

/* Takes two uint32_t words and returns their quotient as a uint32_t.*/
uint32_t division(uint32_t wordA, uint32_t wordB);

/* Takes two uint32_t words and returns the complement of the wordA ⋀ wordB as 
 * a uint32_t.*/
uint32_t nand(uint32_t wordA, uint32_t wordB);

