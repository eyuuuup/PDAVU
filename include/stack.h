#ifndef STACK_H
#define STACK_H
#include <stdlib.h>

typedef uint8_t byte_t; /* raw memory will be typed as uint8 */
typedef int32_t word_t; /* the basic unit of the ijvm will be an int32 */

int init_stack();

void push(byte_t element);

word_t pop();

word_t top();

int size();

void destroy_stack();

void print_stack();

#endif