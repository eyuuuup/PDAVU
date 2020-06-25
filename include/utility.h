#ifndef UTILITY_H
#define UTILITY_H

struct ijvm_machine
{
    int counter;
    int constantSize;
    int textSize;
    byte_t *constantData;
    byte_t *textData;
    byte_t *data;
};

extern int backPointerSize;
extern int backPointer[10];
extern struct ijvm_machine *initMachine;
extern FILE *out;
extern FILE *in;
extern bool wide;

/**
 * Prints out the constant and text blokcs on two lines.
 **/
void print_blocks();

/**
 * Gets the next two bytes to use as an argument.
 * Returns signed shorts
 * bool increase increases counter if true, if false it doesn't increase counter
 **/
signed short getArgs(bool increase);

#endif