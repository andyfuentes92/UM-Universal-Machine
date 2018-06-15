/* Andres Fuentes and Caelyn Dovey
 * COMP40: Homework #6 UM
 * 4/12/2018
 * um.c: Main executable that runs the universal machine (um).
 */
#include "cpu.h"

int main(int argc, char **argv)
{
        if (argc != 2) {
                fprintf(stderr, "Must provide a single .um file!\n");
                exit(EXIT_FAILURE);
        }
        /* else run the um */
        initialize_um(argv[1]);
}
