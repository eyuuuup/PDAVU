#ifndef STACK_H
#define STACK_H
#include <stdlib.h>

typedef uint8_t byte_t; /* raw memory will be typed as uint8 */
typedef int32_t word_t; /* the basic unit of the ijvm will be an int32 */

/**
 * Initializes the stack
 * Returns 0 on success
 **/
int init_stack();

/**
 * Pushes given element on the stack, converting it to an word and signing it
 **/
void push(byte_t element);

/** 
 * Pops a value of the stack and returns it
 **/
word_t pop();

/**
 * Returns top value of the stack, but does not pop it
 **/
word_t top();

/** 
 * Returns the size of the stack
 **/
int size();

/**
 * Free pointers made thus destroys the stack
 **/
void destroy_stack();

/**
 * Prints the stack with signed integers
 **/
void print_stack();

#endif