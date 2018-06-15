/* Andres Fuentes and Caelyn Dovey
 * COMP40: Homework #6 UM
 * 4/12/2018
 * testMemory.c: Unit testing for all memory related functions.
 */
#include "memory.h"

void init_stack_test();
void free_ID_stack_test();
void grab_ID_test();
void return_ID_test();
void initialize_free_seg_test();
void map_segment_test(); //tests map, unmap, and freetable
void segmented_load_test();
void load_program_test();
void load_init_program_test(char *umProgram);

Stack_T ID_STACK;

int main(int argc, char **argv){
        ID_STACK = Stack_new();
        run_init_program_test();
        init_stack_test();
        free_ID_stack_test();
        grab_ID_test();
        return_ID_test();
        initialize_free_seg_test();
        (void) argc;
        map_segment_test();
        segmented_load_test();
        load_program_test();
        load_init_program_test(argv[1]);
        terminate_um();
}

void free_ID_stack_test() 
{
        fprintf(stderr,
                "Run with valgrind. Expected no memory leaks/memory errors.\n");
        uint32_t *value1 = malloc(sizeof(int));

        *value1 = 2;
        uint32_t *value2 = malloc(sizeof(int));
        *value2 = 3;

        fprintf(stderr, "Pushing onto stack\n");
        Stack_push(ID_STACK, (void *)value1);
        Stack_push(ID_STACK, (void *)value2);

        fprintf(stderr, "Calling free id stack\n");
        free_ID_stack();
        if (ID_STACK == NULL){
                fprintf(stderr, "Success!\n");
        }
}

void init_stack_test()
{
        fprintf(stderr,"Testing initialize_stack.\n");
        if (*(uint32_t *)Stack_pop(ID_STACK) != 0) {
                fprintf(stderr,"Failed. First element != 0!\n");
        }
        
        if (Stack_empty(ID_STACK) == 0) {
                fprintf(stderr,"Failed. Stack is empty!\n");
        }
}

void grab_ID_test()
{
        if (grab_ID() != 0) {
                fprintf(stderr, "Returned ID should be 0!\n");  
        }
        /* Grab ID() ensures there is at least 1 element in the stack at all 
           times. When empty (after the first grab), grab_ID pushes the next ID
           onto the stack.*/ 
        if (grab_ID() != 1) {
                fprintf(stderr, "Returned ID should be 2!\n");  
        }
        return_ID(7);
        if (grab_ID() != 7) {
                fprintf(stderr, "Returned ID should be 7!\n");
        }
        if (grab_ID() != 2) {
                fprintf(stderr, "Returned ID should be 2!\n");
        }
}

void return_ID_test()
{
        uint32_t testID = 23;
        return_ID(testID);
        if (*(uint32_t *)Stack_pop(ID_STACK) != 23) {
                fprintf(stderr, "Invalid ID returned.\n");
        }
}

void initialize_free_seg_test()
{
        Seq_T seg1 = initialize_seg(5);
        Seq_T seg2 = initialize_seg(2);
        fprintf(stderr, "Seq1 length: %d\n", Seq_length(seg1));
        fprintf(stderr, "Seq2 length: %d\n", Seq_length(seg2));
        
        fprintf(stderr, "Five zeroes then 2 zeroes");
        for (int i = 0; i < Seq_length(seg1); i++) {
                fprintf(stderr, "%d ", *(int *)Seq_get(seg1, i));
        }
        fprintf(stderr, "\n");
        for (int j = 0; j < Seq_length(seg2); j++) {
                fprintf(stderr, "%d ", *(int *)Seq_get(seg2, j));
        }
        
        fprintf(stderr, "\n");
        free_seg(seg1);
        free_seg(seg2);
        
        fprintf(stderr, "Attempting to get from a freed seq (error)");
        /* Seq_get(seg1, 2); */
}

void map_segment_test()
{

        for (int i = 0; i < 30; i++) {
                map_segment(i);
        }
        fprintf(stderr, "Table length: %d (should be 30)\n", 
                Seq_length(SEG_TABLE));
        unmap_segment(17);
        //map_segment(2);
        fprintf(stderr, "Table length: %d (should be 30)\n", 
                Seq_length(SEG_TABLE));

        for (int i = 0; i < Seq_length(SEG_TABLE); i++) {
                if (Seq_get(SEG_TABLE, i) != NULL) {
                fprintf(stderr, "%d\n",
                        Seq_length((Seq_T)Seq_get(SEG_TABLE,i)));
                }
                else {
                        fprintf(stderr, "NULL\n");
                }
        }
}

void segmented_load_test()
{
        for (int i = 0; i < 4; i++){
                map_segment(10);
                for (int j = 0; j < 10; j ++) {
                        segmented_store((uint32_t)j, (uint32_t)i, (unsigned) j);
                }
        } 
        uint32_t output;
        for (int i = 0; i < 4; i++) {
                
                fprintf(stderr, "Seg #: %d\n", i);
                for (int j = 0; j < 10; j ++) {
                        output = segmented_load((uint32_t)i, (unsigned) j);
                        fprintf(stderr, "%u ", output);
                }
                fprintf(stderr, "\n");
        } 
}

void load_program_test()
{
        for (int i = 0; i < 4; i++) {
                map_segment(10);
                for (int j = 0; j < 9; j ++) {
                        segmented_store((uint32_t)j, (uint32_t)i, (unsigned) j);
                }
                segmented_store((uint32_t)i, (uint32_t) i, 9);
        } 
        
        uint32_t output;
        for (int i = 0; i < 4; i++) {
                
                fprintf(stderr, "Seg #: %d\n", i);
                for (int j = 0; j < 10; j ++) {
                        output = segmented_load((uint32_t)i, (unsigned) j);
                        fprintf(stderr, "%u ", output);
                }
                fprintf(stderr, "\n");
        } 
        load_program(3);
        load_program(2);

        fprintf(stderr, "~~~~~~~Checking output of load program~~~~~~\n");
        for (int i = 0; i < 4; i++) {
                
                fprintf(stderr, "Seg #: %d\n", i);
                for (int j = 0; j < 10; j ++) {
                        output = segmented_load((uint32_t)i, (unsigned) j);
                        fprintf(stderr, "%u ", output);
                }
                fprintf(stderr, "\n");
        }
        load_program(0);
}


void load_init_program_test(char *umProgram)
{
        load_init_program(umProgram);
        //Check that umProgram ended up in SEG_TABLE[0]
        int size = Seq_length((Seq_T)Seq_get(SEG_TABLE, 0));
        for (int i = 0; i < size; i++) {
                fprintf(stderr, "%u ", segmented_load(0,i));
        }
}


