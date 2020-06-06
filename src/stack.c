#include <ijvm.h>
#include <stdlib.h>
#include <stack.h>

struct stack {
    word_t *stackArray;
    int maxSize;
    int stackSize;
};

static struct stack *stack;


int init_stack() {
    stack = (struct stack *)malloc(sizeof(struct stack));
    stack -> stackArray = (word_t *)malloc(sizeof(word_t) * 10);
    stack -> maxSize = 10;
    stack -> stackSize = 0;
    return 0;
}

void push(byte_t element) {
    if(stack -> stackSize >= stack -> maxSize)
    {
        stack -> stackArray = (word_t *)realloc(stack -> stackArray, stack -> maxSize * 2);
        stack -> maxSize = stack -> maxSize * 2;
    }
    
    word_t result = (word_t) element;
    stack -> stackArray[stack -> stackSize] = result;
    stack -> stackSize++;
    print_stack();
}

word_t pop() {
    if(stack -> stackSize > 0) {
        word_t topElement = stack -> stackArray[stack -> stackSize - 1];
        stack -> stackSize--;
        return topElement;
    }
}

word_t top() {
    if(stack -> stackSize > 0) {
        return stack -> stackArray[stack -> stackSize - 1];
    }
}

int size() {
    return stack -> stackSize;
}

word_t *get_stack() {
    return stack -> stackArray;
}

word_t tos() {
    if(stack -> stackSize > 0) {
        char result = top();
        printf("TOS: %d \n", result);
        return result;
    }
}

void destroy_stack() {
    free(stack -> stackArray);
    free(stack);
}

void print_stack() {
    printf("\nSTACK\n");
    for (int i = 0; i < stack -> stackSize; i++) {
        printf("%d ", stack -> stackArray[i]);
    }
    printf("\nSTACK\n");
}