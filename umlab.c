/*
 * umlab.c
 * 
 * The functions defined in this lab should be linked against umlabwrite.c
 * to produce a unit test writing program. Any additional functions and unit
 * tests written for the lab go here.
 */

#include <stdint.h>
#include <stdio.h>
#include <bitpack.h>
#include <assert.h>
#include <seq.h>

typedef uint32_t Um_instruction;
typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;

typedef enum Um_register { r0 = 0, r1, r2, r3, r4, r5, r6, r7 } Um_register;


/* Functions that return the two instruction types */
Um_instruction three_register(Um_opcode op, int ra, int rb, int rc);
Um_instruction loadval(unsigned ra, unsigned val);

/* int main() { */

/*         fprintf(stderr, "%u\n", loadval(7, 410));  */

/* } */

/* Wrapper functions for each of the instructions */

static inline Um_instruction halt(void) 
{
        return three_register(HALT, 0, 0, 0);
}

static inline Um_instruction conditional_move(Um_register a, Um_register b, 
                                              Um_register c)
{
        return three_register(CMOV, a, b, c);
}
static inline Um_instruction segmented_load(Um_register a, Um_register b, 
                                              Um_register c)
{
        return three_register(SLOAD, a, b, c);
}

static inline Um_instruction segmented_store(Um_register a, Um_register b, 
                                              Um_register c)
{
        return three_register(SSTORE, a, b, c);
}

static inline Um_instruction add(Um_register a, Um_register b, Um_register c) 
{
        return three_register(ADD, a, b, c);
}

static inline Um_instruction multiply(Um_register a, Um_register b, 
                                              Um_register c)
{
        return three_register(MUL, a, b, c);
}

static inline Um_instruction divide(Um_register a, Um_register b,
                                    Um_register c)
{
        return three_register(DIV, a, b, c);
}

static inline Um_instruction nand(Um_register a, Um_register b, Um_register c)
{
        return three_register(NAND, a, b, c);
}

static inline Um_instruction map_segment(Um_register b, Um_register c)
{
        return three_register(ACTIVATE, 0, b, c);
}

static inline Um_instruction unmap_segment(Um_register c)
{
        return three_register(INACTIVATE, 0, 0, c);
}

static inline Um_instruction output(Um_register c)
{
        return three_register(OUT, 0, 0, c);
}

static inline Um_instruction input(Um_register c)
{
        return three_register(IN, 0, 0, c);
}

static inline Um_instruction loadp(Um_register b, Um_register c)
{
        return three_register(LOADP, 0, b, c);
}

/* Functions for working with streams */

static inline void emit(Seq_T stream, Um_instruction inst)
{
        assert(sizeof(inst) <= sizeof(uintptr_t));
        Seq_addhi(stream, (void *)(uintptr_t)inst);
}

extern void Um_write_sequence(FILE *output, Seq_T stream);

inline void Um_write_sequence(FILE *output, Seq_T stream) 
{
        for (int i = 0; i < (Seq_length(stream)); i++) {
                uint32_t current;
                current = (Um_instruction)(uintptr_t)Seq_get(stream, i);
                for (int i = 24; i >= 0; i-= 8)
                        fprintf(output, "%c", 
                                (char)Bitpack_getu(current, 8, i));
        }
}


/* Unit tests for the UM */

void emit_halt_test(Seq_T stream)
{
        emit(stream, halt());
}

void emit_verbose_halt_test(Seq_T stream)
{
        emit(stream, halt());
        emit(stream, loadval(r1, 'B'));
        emit(stream, output(r1));
        emit(stream, loadval(r1, 'a'));
        emit(stream, output(r1));
        emit(stream, loadval(r1, 'd'));
        emit(stream, output(r1));
        emit(stream, loadval(r1, '!'));
        emit(stream, output(r1));
        emit(stream, loadval(r1, '\n'));
        emit(stream, output(r1));
}
void emit_add_test(Seq_T stream)
{
        emit(stream, loadval(r2, 50));
        emit(stream, loadval(r3, 55));
        emit(stream, add(1,2,3));
        emit(stream, output(r1));
        emit(stream, halt());
}

void emit_input_output_test(Seq_T stream)
{
        emit(stream, input(r1));
        emit(stream, input(r2));
        emit(stream, input(r3));
        emit(stream, input(r4));
        emit(stream, input(r5));
        emit(stream, input(r6));
        
        emit(stream, output(r1));
        emit(stream, output(r2));
        emit(stream, output(r3));
        emit(stream, output(r4));
        emit(stream, output(r5));
        emit(stream, output(r6));

        emit(stream, halt());
}

void emit_cmov1_test(Seq_T stream)
{
        emit(stream, loadval(r3, 97));
        emit(stream, loadval(r4, 98));
        emit(stream, conditional_move(r3,r4,r5));
        emit(stream, output(r3));
        emit(stream, output(r4));
        emit(stream, halt());
        
}

void emit_cmov2_test(Seq_T stream)
{
        emit(stream, loadval(r3, 101));
        emit(stream, loadval(r4, 97));
        emit(stream, loadval(r5, 98));
        emit(stream, conditional_move(r3,r4,r5));
        emit(stream, output(r3));
        emit(stream, output(r4));
        emit(stream, halt());   
}

void emit_MS_test(Seq_T stream)
{
        /*store 100 in r4 */
        emit(stream,loadval(r4, 100));
        for (int i = 1; i < 10; i++){
                /* Map 10 segments, storing index i in r3 */
                emit(stream, loadval(r3, i));
                /* Create a segment, store id in r1, size of r3 (i) */
                emit(stream, map_segment(r1, r3));
                /* Add 100 to segID and store in r5 */
                emit(stream, add(r5, r1, r4));
                /* OUtput r5 (should be 101,102,etc */
                emit(stream, output(r5));
        }
        emit(stream, halt());
}

void emit_memory_verbose_test(Seq_T stream)
{
        emit(stream, loadval(r4, 10));
        /* mapseg, store ID in r2, of size r4 (9) */

        emit(stream, map_segment(r2, r4));
        for (int i = 0; i < 10; i++){
                /* Map 10 segments, storing index i in r3 */
                emit(stream, loadval(r3, i));
                /* Create a segment, store id in r1, size of r4 (10) */
                emit(stream, map_segment(r1, r4));
                /* Store r1 (segID of mapped seg) into IDsegment created earlier
                   at offset i */
                emit(stream, segmented_store(r2,r3,r1));
        }

        /* Now, unmap all previously created segments, with ID's stored in 
           segment with ID in r2 */
        for (int i = 0; i < 10; i++){
                emit(stream, loadval(r3, i));
                /* r8 gets r2[r3], accessing segIDs stored in r2 */
                emit(stream, segmented_load(r6, r2, r3));
                /* unmap segment with ID in r8 */
                emit(stream, unmap_segment(r6));
        }
        emit(stream, halt());
}

void emit_mult_and_div_test(Seq_T stream)
{
        
        emit(stream, loadval(r2, 12));
        emit(stream, loadval(r3, 10));
        emit(stream, multiply(r7, r2, r3));
        emit(stream, output(r7)); //120 (x)

        emit(stream, loadval(r1, '\n'));
        emit(stream, output(r1));

        emit(stream, divide(r4, r7, r2));
        emit(stream, loadval(r6, 100));
        emit(stream, add(r4, r4, r6));
        emit(stream, output(r4)); //110

        emit(stream, loadval(r1, '\n'));
        emit(stream, output(r1));

        emit(stream, divide(r4, r7, r3));
        emit(stream, add(r4, r4, r6));
        emit(stream, output(r4)); //112

        emit(stream, loadval(r1, '\n'));
        emit(stream, output(r1));

        emit(stream, halt());
}

void emit_nand_test(Seq_T stream)
{
        emit(stream, loadval(r1, 382));
        emit(stream, loadval(r2, 422));
        emit(stream, nand(r3, r1, r2));
        emit(stream, loadval(r7, 2));
        emit(stream, divide(r3, r3, r7));
        emit(stream, output(r3));

        emit(stream, loadval(r5, '\n'));
        emit(stream, output(r5));
        emit(stream, halt());
}

void emit_load_program_test(Seq_T stream){
        emit(stream, loadval(r1, 3));   
        emit(stream, loadval(r2, 103)); 
        emit(stream, loadval(r1, 6));   
        emit(stream, loadp(0,r1));      
        emit(stream, loadval(r1,2));    
        emit(stream, loadval(r2,102));  
        emit(stream, output(r2));       
        emit(stream, loadval(r1,'\n')); 
        emit(stream, output(r1));       
        emit(stream, halt());           
}

Um_instruction three_register(Um_opcode op, int ra, int rb, int rc)
{
        Um_instruction instruction = 0;
        instruction += ra;
        instruction <<= 3; 
        instruction += rb;
        instruction <<= 3; 
        instruction += rc;
        op <<= 28;
        instruction += op;
        return instruction;
}

Um_instruction loadval(unsigned ra, unsigned val) 
{
        Um_instruction instruction = 0;
        ra <<= 25;
        instruction += ra;
        instruction += val;
        int op = 13 << 28;
        instruction += op;

        return instruction;
}
