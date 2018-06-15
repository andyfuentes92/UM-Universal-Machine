/* Andres Fuentes and Caelyn Dovey
 * COMP40: Homework #6 UM
 * 4/12/2018
 * cpu.c: Includes the implementation for all public and private functions 
 * in the cpu module.
 */
#include "cpu.h"

uint32_t REG[8] = { 0 };
uint32_t PRG_CTR = 0;

struct regNums{
        uint32_t OP,A,B,C, value;
}; typedef struct regNums regNums;

static regNums get_A(uint32_t word);
static regNums get_ABC(uint32_t word);
static regNums get_word_info(uint32_t word);
static void conditional_move(struct regNums wordInfo);

/* Takes a file ptr, returns void. Initializes um with program give by user.*/
void initialize_um(char *umProgram)
{
        load_init_program(umProgram);
        uint32_t instruction;
        while (true){
                instruction = segmented_load(0, PRG_CTR);
                regNums r = get_word_info(instruction);

                if (r.OP > 13) {
                        exit(EXIT_FAILURE);
                }
                switch(r.OP) {
                case 0:
                        conditional_move(r);
                        break;
                case 1:
                        REG[r.A] = segmented_load(REG[r.B], REG[r.C]);
                        break;
                case 2:
                        segmented_store(REG[r.C], REG[r.A], REG[r.B]);
                        break;
                case 3:
                        REG[r.A] = addition(REG[r.B], REG[r.C]);
                        break;
                case 4:
                        REG[r.A] = multiplication(REG[r.B], REG[r.C]);
                        break;
                case 5:
                        REG[r.A] = division(REG[r.B], REG[r.C]);
                        break;
                case 6:
                        REG[r.A] = nand(REG[r.B], REG[r.C]);
                        break;
                case 7:
                        terminate_um();
                        break;
                case 8:
                        REG[r.B] = map_segment(REG[r.C]);
                        break;
                case 9:
                        unmap_segment(REG[r.C]);
                        break;
                case 10:
                        output(REG[r.C]);
                        break;
                case 11:
                        REG[r.C] = input();
                        break;
                case 12:
                        load_program(REG[r.B]);
                        PRG_CTR = REG[r.C];
                        break;
                case 13:
                        REG[r.A] = r.value;
                        break; 
                }
                if (r.OP != 12) {
                        PRG_CTR++;
                }
        }
}

/*****************************************************************************/
/***************************PRIVATE FUNCTIONS*********************************/
/*****************************************************************************/

/* Takes a struct containing 3 ints each referring to a register. Checks if the
 * value in register C is zero, if not, sets register A’s value to register 
 * B’s value. Returns void.*/
static void conditional_move(regNums wordInfo)
{
        if (REG[wordInfo.C] != 0) {
                REG[wordInfo.A] = REG[wordInfo.B];
        }
}

/* Takes a uint32_t word and extracts the op code which specifies an 
 * instruction. Returns an unsigned int 0 - 13 */
static regNums get_word_info(uint32_t word)
{
        uint32_t opCode = Bitpack_getu(word, 4, 28);
        regNums wordInfo;
        if (opCode == 13) {
                wordInfo = get_A(word);
        }
        else {
                wordInfo = get_ABC(word);
        }
        wordInfo.OP = opCode;
        return wordInfo;
}

/* Takes a uint32_t word and returns the last 9 bits, stored as unsigned ints a
 * struct type regNums */
static regNums get_ABC(uint32_t word)
{
        uint32_t A = Bitpack_getu(word, 3, 6);
        uint32_t B = Bitpack_getu(word, 3, 3);
        uint32_t C = Bitpack_getu(word, 3, 0);
        regNums wordInfo;
        wordInfo.A     = A;
        wordInfo.B     = B;
        wordInfo.C     = C;
        wordInfo.value = 0;

        return wordInfo;
}

/* Takes a uint32_t word and returns the last the three bits immediately less
 * significant than opcode describe a single register A */
static regNums get_A(uint32_t word)
{
        uint32_t A     = Bitpack_getu(word, 3, 25);
        uint32_t value = Bitpack_getu(word, 25, 0);
        regNums wordInfo;
        wordInfo.A = A;
        wordInfo.B = 0;
        wordInfo.C = 0;
        wordInfo.value = value;

        return wordInfo;
}
