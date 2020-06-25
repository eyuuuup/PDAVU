#include <ijvm.h>
#include <stack.h>
#include <frames.h>
#include "utility.h"

void BIPUSH()
{
    printf("BIPUSH ");
    initMachine->counter++;

    printf("%d, or %x\n", initMachine->textData[initMachine->counter], initMachine->textData[initMachine->counter]);
    push(initMachine->textData[initMachine->counter]);
    initMachine->counter++;
}

void DUP()
{
    printf("DUP\n");
    initMachine->counter++;

    word_t copy = top();
    push(copy);
}

bool ERR()
{
    printf("ERR\n");
    initMachine->counter++;

    return false;
}

void GOTO()
{
    printf("GOTO ");
    initMachine->counter++;

    signed short args = getArgs(false);
    initMachine->counter = initMachine->counter + args - 1;
    printf(", or %d", args);
    printf("\n");
}

bool HALT()
{
    printf("HALT\n");
    initMachine->counter++;

    return false;
}

void IADD()
{
    printf("IADD\n");
    initMachine->counter++;

    int firstElement = pop();
    int secondElement = pop();
    int result = firstElement + secondElement;
    push(result);
}

void IAND()
{
    printf("IAND\n");
    initMachine->counter++;

    int firstElement = pop();
    int secondElement = pop();
    int result = firstElement & secondElement;
    push(result);
}

void IFEQ()
{
    printf("IFEQ ");
    initMachine->counter++;

    signed short args = getArgs(false);
    if (pop() == 0)
    {
        initMachine->counter = initMachine->counter + args - 1;
        printf("TRUE");
    }
    else
    {
        initMachine->counter = initMachine->counter + 2;
    }
    printf(", or %d", args);
    printf("\n");
}

void IFLT()
{
    printf("IFLT ");
    initMachine->counter++;

    signed short args = getArgs(false);
    if (pop() < 0)
    {
        initMachine->counter = initMachine->counter + args - 1;
        printf("TRUE");
    }
    else
    {
        initMachine->counter = initMachine->counter + 2;
    }
    printf(", or %d", args);
    printf("\n");
}

void IF_ICMPEQ()
{
    printf("ICMPEQ ");
    initMachine->counter++;

    signed short args = getArgs(false);

    word_t firstPop = pop();
    word_t secondPop = pop();

    if (firstPop == secondPop)
    {
        initMachine->counter = initMachine->counter + args - 1;
        printf("TRUE");
    }
    else
    {
        initMachine->counter = initMachine->counter + 2;
    }
    printf(", or %d", args);
    printf("\n");
}

void IINC()
{
    printf("IINC ");
    initMachine->counter++;

    int index;
    if (wide)
    {
        index = getArgs(false);
        printf("%d\n", index);
        initMachine->counter = initMachine->counter + 2;
        wide = false;
    }
    else
    {
        index = initMachine->textData[initMachine->counter];
        initMachine->counter++;
    }
    printf("%d ", index);

    int value = (int8_t)initMachine->textData[initMachine->counter];
    printf("%d\n", initMachine->textData[initMachine->counter]);

    int acValue = find_var(index) + value;
    add_frame(index, acValue);
    initMachine->counter++;
}

void ILOAD()
{
    printf("ILOAD ");

    initMachine->counter++;
    if (wide)
    {
        byte_t firstElement = initMachine->textData[initMachine->counter];
        byte_t secondElement = initMachine->textData[initMachine->counter + 1];
        int args = (firstElement << 8) | secondElement;
        printf("%d\n", args);
        push(find_var(args));

        initMachine->counter = initMachine->counter + 2;
        wide = false;
    }
    else
    {
        int id = initMachine->textData[initMachine->counter];
        printf("%x\n", initMachine->textData[initMachine->counter]);
        push(find_var(id));
        initMachine->counter++;
    }
}

void IN()
{
    printf("IN\n");
    initMachine->counter++;

    if (in == NULL)
    {
        set_input(stdin);
    }

    int result = fgetc(in);
    if (result == EOF)
    {
        push(0);
    }
    else
        push(result);
}

void INVOKEVIRTUAL()
{
    printf("INVOKEVIRTUAL ");
    initMachine->counter++;

    //get opcode (short)
    signed short index = getArgs(true);

    //get pointer to next area and save current pointer
    word_t pointer = get_constant(index);
    int prevPointer = initMachine->counter;
    backPointer[backPointerSize] = prevPointer;
    backPointerSize++;
    initMachine->counter = pointer;

    //get first short amount of args
    signed short amountArgs = getArgs(true);
    printf("%d ", amountArgs);

    //get second short area size
    signed short areaSize = getArgs(true);
    printf("%d\n", areaSize);

    for (int i = 1; i < amountArgs; i++)
    {
        int flip = amountArgs - i;
        add_frame(flip, pop());
    }

    pop();
    save_sp();
    printf("\n");
}

void IOR()
{
    printf("IOR\n");
    initMachine->counter++;

    int firstElement = pop();
    int secondElement = pop();
    int result = firstElement | secondElement;
    push(result);
}

void IRETURN()
{
    printf("IRETURN\n");
    initMachine->counter++;

    int prevPointer = backPointer[backPointerSize - 1];
    backPointerSize--;
    initMachine->counter = prevPointer;

    int resultValue = top();
    reset_sp();
    push(resultValue);
}

void ISTORE()
{
    printf("ISTORE ");

    initMachine->counter++;
    if (wide)
    {
        byte_t firstElement = initMachine->textData[initMachine->counter];
        printf("%x ", initMachine->textData[initMachine->counter]);
        byte_t secondElement = initMachine->textData[initMachine->counter + 1];
        printf("%x ", initMachine->textData[initMachine->counter + 1]);
        int args = (firstElement << 8) | secondElement;
        printf("%d\n", args);

        int data = pop();
        add_frame(args, data);

        initMachine->counter = initMachine->counter + 2;
        wide = false;
    }
    else
    {
        printf("%x\n", initMachine->textData[initMachine->counter]);
        int id = initMachine->textData[initMachine->counter];
        int data = pop();

        add_frame(id, data);
        initMachine->counter++;
    }
}

void ISUB()
{
    printf("ISUB\n");
    initMachine->counter++;

    int firstElement = pop();
    int secondElement = pop();
    int result = secondElement - firstElement;
    push(result);
}

void LDC_W()
{
    printf("LDC_W ");

    initMachine->counter++;
    byte_t firstElement = initMachine->textData[initMachine->counter];
    printf("%x ", initMachine->textData[initMachine->counter]);
    byte_t secondElement = initMachine->textData[initMachine->counter + 1];
    printf("%x ", initMachine->textData[initMachine->counter + 1]);

    unsigned short index = (firstElement << 8) | secondElement;
    printf(", or %d\n", index);

    word_t args = get_constant(index);
    push(args);

    initMachine->counter = initMachine->counter + 2;
    printf("\n");
}

void NOP()
{
    printf("NOP\n");
    initMachine->counter++;
}

void OUT()
{
    printf("OUT\n");

    initMachine->counter++;
    char result = pop();

    if (out == NULL)
    {
        FILE *fp = stdout;
        set_output(fp);
    }

    fwrite(&result, sizeof(char), 1, out);
}

void POP()
{
    printf("POP\n");
    initMachine->counter++;
    pop();
}

void SWAP()
{
    printf("SWAP\n");
    initMachine->counter++;

    int firstElement = pop();
    int secondElement = pop();
    push(firstElement);
    push(secondElement);
}

void WIDE()
{
    initMachine->counter++;
    printf("WIDE\n");
    wide = true;
    step();
}

signed short getArgs(bool increase)
{
    byte_t firstElement;
    byte_t secondElement;

    if (increase)
    {
        firstElement = initMachine->textData[initMachine->counter];
        printf("%x ", initMachine->textData[initMachine->counter]);
        initMachine->counter++;

        secondElement = initMachine->textData[initMachine->counter];
        printf("%x ", initMachine->textData[initMachine->counter]);
        initMachine->counter++;
    }
    else
    {
        firstElement = initMachine->textData[initMachine->counter];
        printf("%x ", initMachine->textData[initMachine->counter]);

        secondElement = initMachine->textData[initMachine->counter + 1];
        printf("%x ", initMachine->textData[initMachine->counter + 1]);
    }

    signed short args = (firstElement << 8) | secondElement;

    return args;
}