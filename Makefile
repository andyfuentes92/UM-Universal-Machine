# Makefile for UM (Comp 40 Assignment 6)
# Andres Fuentes and Caelyn Dovey 
# Last updated: April 13th, 2018


############## Variables ###############

CC = gcc # The compiler being used

# Updating include path to use Comp 40 .h files and CII interfaces
IFLAGS = -I/comp/40/include -I/usr/sup/cii40/include/cii

CFLAGS = -g -std=gnu99 -O2 -Wall -Wextra -Werror -Wfatal-errors -pedantic $(IFLAGS)

# Linking flags
LDFLAGS = -g -L/comp/40/lib64 -L/usr/sup/cii40/lib64

# Libraries needed for linking
LDLIBS = -l40locality -lnetpbm -lcii40 -larith40 -lm -lrt

INCLUDES = $(shell echo *.h)

############### Rules ###############

all: um testMath testIO testCPU writetests #testMemory 

# To get *any* .o file, compile its .c file with the following rule.
%.o: %.c $(INCLUDES)
	$(CC) $(CFLAGS) -c $< -o $@

## Linking step (.o -> executable program)

um: um.o cpu.o memory.o mathinstructions.o ioInstructions.o bitpack.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

writetests: umlab.o bitpack.o umlabwrite.o 
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

#testMemory will not compile because it tests private memory functions, which 
# were made public at the time of testing. 
#testMemory: memory.o bitpack.o testMemory.o 
#	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

testMath: mathinstructions.o testMath.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

testIO: ioInstructions.o testIO.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

testCPU: cpu.o testCPU.o memory.o mathinstructions.o ioInstructions.o bitpack.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

clean:
	rm -f memory *.o

