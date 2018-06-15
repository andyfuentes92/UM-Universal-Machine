/* Andres Fuentes and Caelyn Dovey
 * COMP40: Homework #6 UM
 * 4/12/2018
 * testIO.c: Testing for our IO module.
 */
#include "ioInstructions.h"

int main()
{
        
        fprintf(stderr, "Testing output\n");
        uint32_t word2 = 177;
        output(word2);
        fprintf(stderr, "\n");
        /* Test on a file containing the chars: 1a2. Expected output is
         * 1
         * a 
         * 2
         * -1 */
        fprintf(stderr, "Testing input\n");
        fprintf(stderr, "%c \n", input());
        fprintf(stderr, "%c \n", input());
        fprintf(stderr, "%c \n", input());
        fprintf(stderr, "%d \n", input());
        
}

