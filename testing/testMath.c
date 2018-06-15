/* Andres Fuentes and Caelyn Dovey
 * COMP40: Homework #6 UM
 * 4/12/2018
 * testMath: Testing for our math module
 */
#include "mathinstructions.h"
void add_test(uint32_t wordA, uint32_t wordB);
void mult_test(uint32_t wordA, uint32_t wordB);
void div_test();
void nand_test();

int main()
{
        uint32_t wordA, wordB;
        wordA = 4294967295; 
        wordB = 3;
        add_test(wordA, wordB);
        mult_test(wordA, wordB);
        div_test();
        nand_test();

}

void add_test(uint32_t wordA, uint32_t wordB)
{
        fprintf(stderr, "ADDITION TESTS:\n");
        fprintf(stderr, "%u (should be 2)\n", addition(wordA, wordB));
        fprintf(stderr, "3 + 7 = %d\n", addition(3,7));
        fprintf(stderr, "\n");
}

void mult_test(uint32_t wordA, uint32_t wordB)
{
        (void) wordB;
        fprintf(stderr, "MULTIPLICATION:\n");
        fprintf(stderr, "%u (should be 4294967295\n", 
                multiplication((wordA)/2, 2));
        
        fprintf(stderr, "%u (should be 0)\n", 
                multiplication((wordA)/2 +1, 2));

        fprintf(stderr, "%u (should = 21)\n", multiplication(3, 7));
        fprintf(stderr, "%u (should = 55)\n", multiplication(11, 5));
        fprintf(stderr, "\n");
}

void div_test()
{
        fprintf(stderr, "DIVISION TESTS:\n");
        fprintf(stderr, "%u (should = 21)\n", division(63, 3));
        fprintf(stderr, "%u (should floor to 7)\n", division(15,2));
        fprintf(stderr, "Trying to divide by zero. Expected assertion!\n");
        //fprintf(stderr, "%u\n", division(3, 0));
        fprintf(stderr, "\n");
}

void nand_test()
{
        fprintf(stderr, "NAND TESTS:\n");
        fprintf(stderr, "%d (should = -43 if interpretted as signed)\n", 
                (int)nand(107, 42));
        fprintf(stderr, "\n");
}
