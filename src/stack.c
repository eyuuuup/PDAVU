#include <ijvm.h>
#include <stdlib.h>
#include <stack.h>

struct stack {
    word_t *stackArray;
    int maxSize;
    int stackSize;
};

static struct stack *stack;
int sp;

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
        stack -> stackArray = (word_t *)realloc(stack -> stackArray, stack -> maxSize * 3);
        stack -> maxSize = stack -> maxSize * 3;
    }
    
    //forcing it to be signed but why?
    word_t result = (char) element;
    stack -> stackArray[stack -> stackSize] = result;
    stack -> stackSize++;
    print_stack();
}

word_t pop() {
    if(size() > 0) {
        word_t topElement = stack -> stackArray[stack -> stackSize - 1];
        stack -> stackSize--;
        return topElement;
        print_stack();
    }
}

word_t top() {
    if(size() > 0) {
        return stack -> stackArray[stack -> stackSize - 1];
    }
}

int size() {
    return stack -> stackSize;
}

word_t *get_stack() {
    return stack -> stackArray;
}

int stack_size() {
    return size();
}

word_t tos() {
    if(size() > 0) {
        word_t result = top();
        return result;
    }
}

void save_sp() {
    sp = stack->stackSize;
}

void reset_sp() {
    stack->stackSize = sp;
}

void destroy_stack() {
    free(stack -> stackArray);
    free(stack);
}

void print_stack() {
    printf("STACK: ");
    for (int i = 0; i < stack -> stackSize; i++) {
        printf("%d ", stack -> stackArray[i]);
    }
    printf("<-TOP\n");
}