#include <ijvm.h>
#include <stdlib.h>
#include <stack.h>
#include <util.h>

struct stack
{
    word_t *stackArray;
    int maxSize;
    int stackSize;
};

static struct stack *stack;
int sp;

int init_stack()
{
    stack = (struct stack *)malloc(sizeof(struct stack) + 1);
    stack->stackArray = (word_t *)malloc(sizeof(word_t) * 10 + 1);
    stack->maxSize = 10;
    stack->stackSize = 0;
    return 0;
}

void push(word_t element)
{
    if (stack->stackSize >= stack->maxSize)
    {
        stack->stackArray = (word_t *)realloc(stack->stackArray, stack->maxSize * 10 * 2 + 1);
        stack->maxSize = stack->maxSize * 2;
    }

    word_t result;
    if (element > 255)
    {
        result = element;
    }
    else
    {
        result = (char)element;
    }
    stack->stackArray[stack->stackSize] = result;
    stack->stackSize++;
}

word_t pop()
{
    if (size() > 0)
    {
        word_t topElement = stack->stackArray[stack->stackSize - 1];
        stack->stackSize--;
        return topElement;
    } else return -1;
}

word_t top()
{
    if (size() > 0)
    {
        return stack->stackArray[stack->stackSize - 1];
    } else return -1;
}

int size()
{
    return stack->stackSize;
}

word_t *get_stack()
{
    return stack->stackArray;
}

int stack_size()
{
    return size();
}

word_t tos()
{
    if (size() > 0)
    {
        word_t result = top();
        return result;
    } else return -1;
}

void save_sp()
{
    sp = stack->stackSize;
}

void reset_sp()
{
    stack->stackSize = sp;
}

void destroy_stack()
{
    free(stack->stackArray);
    free(stack);
}

void print_stack()
{
    dprintf("STACK: ");
    for (int i = 0; i < stack->stackSize; i++)
    {
        dprintf("%d ", stack->stackArray[i]);
    }
    dprintf("<-TOP\n");
}