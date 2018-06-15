/* Andres Fuentes and Caelyn Dovey
 * COMP40: Homework #6 UM
 * 4/12/2018
 * memory.c: Includes the implementation for all public and private functions 
 * in the memory module.
 */

#include "memory.h"

Seq_T SEG_TABLE;
Stack_T ID_STACK;

static Stack_T initialize_stack();
static Seq_T initialize_seg(unsigned numWords);
static uint32_t grab_ID();
static void return_ID(uint32_t segID);
static void free_seg(Seq_T seg);
static void free_ID_stack();
static void free_table();

/* Takes no parameters, returns void. Frees SEG_TABLE, ID_STACK, and exits. 
 * Called by halt and run_init_program. */
void terminate_um()
{
        free_table();
        free_ID_stack();
        exit(EXIT_SUCCESS);
}

/* Takes a file ptr and returns void. Runs the initial .um file passed in. */
void load_init_program(char* umProgram)
{
        /* Initialize SEG_TABLE and ID_STACK */
        SEG_TABLE = Seq_new(1000);
        ID_STACK  = initialize_stack();

        /* Use stat to get fileSize from the umProgram */
        struct stat buf;
        stat(umProgram, &buf);
        int fileSize = buf.st_size;

        /* Check that file contains a multiple of 32bit words */
        assert(fileSize % 4 == 0);

        /* Create a new segment of unknown length and fill with umProgram 
         * contents */
        Seq_T seg = Seq_new(0);
        int c;

        FILE *fp = fopen(umProgram, "r");

        for (int i = 0; i < fileSize/4; i++) {
                uint32_t *word = calloc(1, sizeof(*word));
                for (int j = 24; j >= 0; j -= 8) {
                        c = fgetc(fp);
                        *word = Bitpack_newu(*word, 8, j, (uint32_t)c);
                }
                Seq_addhi(seg, (void *)word);
        }
        fclose(fp);
        /* Remove ID zero from our stack */
        grab_ID();
        /* Add initial program to SEG_TABLE at segID = 0 */
        Seq_addhi(SEG_TABLE, (void *)seg);

}

/* Takes an unsigned numWords and returns a uint32_t segment ID. Creates a new 
 * segment of size numbwords and inserts it into the SEG_TABLE at segID */
uint32_t map_segment(unsigned numWords)
{
        Seq_T seg   = initialize_seg(numWords);
        uint32_t ID = grab_ID();
        /* If using new ID, addhi */
        if (ID == (uint32_t)Seq_length(SEG_TABLE)) {
                Seq_addhi(SEG_TABLE, (void *)seg);
        }
        else {
        /* If reusing ID, insert @ location ID. */
                Seq_put(SEG_TABLE, ID, (void *)seg);
        }

        return ID;
}

/* Takes a uint32_t segID and returns void. Frees the segment at segID */
void unmap_segment(uint32_t segID)
{
        assert(segID != 0);
        /* Put ID back onto stack */
        return_ID(segID);

        Seq_T seg = Seq_get(SEG_TABLE, segID);
        free_seg(seg);
        Seq_put(SEG_TABLE, segID, NULL);
}

/* Takes a uint32_t segID, and an unsigned offset, and returns the word in 
 * segment segID at location offset. */
uint32_t segmented_load(uint32_t segID, unsigned offset)
{
        /* Get sequence with segment id segID */
        Seq_T seg = Seq_get(SEG_TABLE, segID);

        /* Get word at specified offset within segment seg */
        uint32_t word = *(uint32_t *)Seq_get(seg, offset);
        return word;
}

/* Takes a uint32_t word, uint32_t segID, and an unsigned offset. Returns void.
 * Places word into segment segID at location offset within the segment. */
void segmented_store(uint32_t word, uint32_t segID, unsigned offset)
{      
        /* Get sequence with segment id segID */
        Seq_T seg = Seq_get(SEG_TABLE, segID);

        /* Get ptr to word at specified offset within segment seg, and change 
           its value */
        uint32_t *existingWord = (uint32_t *)Seq_get(seg, offset);
        *existingWord          = word;
}

/* Takes a uint32_t segID, and a unsigned program counter and returns void. 
 * Loads program at segID into SEG_TABLE[0]. */
void load_program(uint32_t segID)
{
        if (segID == 0) {
                return;
        }

        /* Copy program and move to $m[0]*/
        Seq_T seg = Seq_get(SEG_TABLE, segID);

        /* Copy segment */
        Seq_T newSeg = initialize_seg(Seq_length(seg));
        for (int i = 0; i < Seq_length(seg); i++) {
                uint32_t existingWord = *(uint32_t *)Seq_get(seg, i);
                uint32_t *newSegWord  = (uint32_t *)Seq_get(newSeg, i);
                *newSegWord           = existingWord;
        }

        /* Put newSeg into ID 0, and free oldZeroSeg */
        Seq_T oldZeroSeg = Seq_put(SEG_TABLE, 0, (void *)newSeg);
        
        free_seg(oldZeroSeg);
}

/*****************************************************************************/
/***************************PRIVATE FUNCTIONS*********************************/
/*****************************************************************************/

/* Takes a Seq_T and frees all associated memory. Returns void. */
void free_seg(Seq_T seg)
{
        uint32_t *word;
        /* Iterate about segment, freeing each word */
        for (int i = 0; i < Seq_length(seg); i++) {
                word = (uint32_t *)Seq_get(seg, i);
                free(word);
        }
        Seq_free(&seg);
}

/* Takes a uint32_t segment ID & returns nothing. Pushes ID back onto stack for
 * reuse */
void return_ID(uint32_t segID)
{
        uint32_t *returnID = malloc(sizeof(*returnID));
        *returnID          = segID;
        Stack_push(ID_STACK, (void *)returnID);
}

/* Takes no parameters and returns void. Frees int ID's stored in the stack, 
 * then frees the stack itself.*/ 
void free_ID_stack()
{
        uint32_t *ID;
        while (Stack_empty(ID_STACK) != 1) {
                ID = (uint32_t *)Stack_pop(ID_STACK);
                free(ID);
        }
        Stack_free(&ID_STACK);
}

/* Takes no parameters & returns the next available uint32_t on ID stack.*/
uint32_t grab_ID()
{
        /* Grab next available ID */
        uint32_t *ID     = (uint32_t *)Stack_pop(ID_STACK);
        uint32_t localID = *ID;
        free(ID);

        /* If grabbing emptied stack, push ID + 1 onto stack */
        if (Stack_empty(ID_STACK) == 1){
                uint32_t *nextID = malloc(sizeof(*nextID));
                *nextID          = localID + 1;
                Stack_push(ID_STACK, (void *)nextID);
        }
        return localID;
}

/* Takes no parameters & returns a Hanson Stack of ints. Initialized with 0.*/
Stack_T initialize_stack()
{
        ID_STACK = Stack_new();
        uint32_t *initial_val = malloc(sizeof(uint32_t));
        *initial_val = 0;
        Stack_push(ID_STACK, (void *)initial_val);
        return ID_STACK;
}

/* Takes no parameters & returns a new sequence of calloced 32-bit words. */
Seq_T initialize_seg(unsigned numWords)
{
        Seq_T seq = Seq_new(numWords);
        for (unsigned i = 0; i < numWords; i++) {
                uint32_t *word = calloc(1, sizeof(*word));
                Seq_addlo(seq, (void *)word);
        }
        return seq;
}

/* Takes no paramters and returns void. Frees all memory associated with SEG_
 * TABLE */
void free_table()
{
        for (int i = 0; i < Seq_length(SEG_TABLE); i++) {
                Seq_T segment = (Seq_T)Seq_get(SEG_TABLE, i);
                
                if(segment != NULL) {
                        free_seg(segment);
                }
        }
        Seq_free(&SEG_TABLE);
}

