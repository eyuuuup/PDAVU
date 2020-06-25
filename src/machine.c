#include <ijvm.h>
#include <stack.h>
#include <utility.h>
#include <frames.h>
#include <instructions.h>

int backPointerSize = 0;
int backPointer[10];
struct ijvm_machine *initMachine;
FILE *out = NULL;
FILE *in = NULL;
bool wide = false;

int init_ijvm(char *binary_file)
{
  int sizeBytes = 0;
  FILE *fp;

  // Get amount of bytes in file by moving pointer up and down
  fp = fopen(binary_file, "rb");
  fseek(fp, 0, SEEK_END);
  sizeBytes = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  char buffer[sizeBytes];

  // Allocate memory because it will be used in global struct
  byte_t *hexValues = (byte_t *)malloc(sizeof(char) * sizeBytes + 1);

  // Read file containing sizeBytes of bytes and split them up in sizeof(char)
  fread(buffer, sizeof(char), sizeBytes, fp);

  int constantSize = 0;
  int constantDataIndex = 0;
  int textSize = 0;
  int textDataIndex = 0;
  for (int i = 0; i < sizeBytes; i++)
  {

    byte_t output = buffer[i];
    hexValues[i] = output;

    // Arrived at last byte of constant pool size, add the bytes of the size and note the next index as data
    if (i == 11)
    {
      //int args = (firstElement << 8) | secondElement;
      constantSize = hexValues[i] | (hexValues[i - 1] << 8) | (hexValues[i - 2] << 16) | (hexValues[i - 3] << 24);
      constantDataIndex = i + 1;
    }

    // Arrived at last byte of text size, add the bytes of the size and note the next index as data
    if (i == (11 + constantSize + 8))
    {
      textSize = hexValues[i] | (hexValues[i - 1] << 8) | (hexValues[i - 2] << 16) | (hexValues[i - 3] << 24);
      textDataIndex = i + 1;
    }
  }

  // Copy over data to store in struct
  byte_t *constantData = (byte_t *)malloc(sizeof(char) * constantSize + 1);
  for (int i = 0; i < constantSize; i++)
  {
    constantData[i] = hexValues[constantDataIndex + i];
  }

  // Allocate memory because it will be used in global struct
  byte_t *textData = (byte_t *)malloc(sizeof(char) * textSize + 1);
  for (int i = 0; i < textSize; i++)
  {
    textData[i] = hexValues[textDataIndex + i];
  }

  // Allocate memory for global struct and point to members
  initMachine = (struct ijvm_machine *)malloc(sizeof(struct ijvm_machine) + 1);
  initMachine->counter = 0;
  initMachine->textSize = textSize;
  initMachine->constantSize = constantSize;
  initMachine->data = hexValues;
  initMachine->constantData = constantData;
  initMachine->textData = textData;

  init_stack();
  print_blocks();

  return 0;
}

void destroy_ijvm()
{
  free(initMachine->data);
  free(initMachine->constantData);
  free(initMachine->textData);
  free(initMachine);
  destroy_stack();
  backPointerSize = 0;
  destroy_list();
  printf("---------------------------\n");
}

void run()
{
  while (initMachine->counter < initMachine->textSize)
  {
    bool continueProgram = step();
    if (!continueProgram)
    {
      break;
    }
  }
}

bool step()
{
  printf("\nCURRENT INSTRUCTION: %x\n", get_instruction());
  print_stack();
  print_list();
  switch (initMachine->textData[initMachine->counter])
  {
  case OP_BIPUSH:
  {
    BIPUSH();
    break;
  }
  case OP_DUP:
  {
    DUP();
    break;
  }
  case OP_ERR:
  {
    return ERR();
    break;
  }
  case OP_GOTO:
  {
    GOTO();
    break;
  }
  case OP_HALT:
  {
    return HALT();
    break;
  }
  case OP_IADD:
  {
    IADD();
    break;
  }
  case OP_IAND:
  {
    IAND();
    break;
  }
  case OP_IFEQ:
  {
    IFEQ();
    break;
  }
  case OP_IFLT:
  {
    IFLT();
    break;
  }
  case OP_ICMPEQ:
  {
    IF_ICMPEQ();
    break;
  }
  case OP_IINC:
  {
    IINC();
    break;
  }
  case OP_ILOAD:
  {
    ILOAD();
    break;
  }
  case OP_IN:
  {
    IN();
    break;
  }
  case OP_INVOKEVIRTUAL:
  {
    INVOKEVIRTUAL();
    break;
  }
  case OP_IOR:
  {
    IOR();
    break;
  }
  case OP_IRETURN:
  {
    IRETURN();
    break;
  }
  case OP_ISTORE:
  {
    ISTORE();
    break;
  }
  case OP_ISUB:
  {
    ISUB();
    break;
  }
  case OP_LDC_W:
  {
    LDC_W();
    break;
  }
  case OP_NOP:
  {
    NOP();
    break;
  }
  case OP_OUT:
  {
    OUT();
    break;
  }
  case OP_POP:
  {
    POP();
    break;
  }
  case OP_SWAP:
  {
    SWAP();
    break;
  }
  case OP_WIDE:
  {
    WIDE();
    break;
  }
  default:
    initMachine->counter++;
    break;
  }
  return true;
}

void set_input(FILE *fp)
{
  in = fp;
}

void set_output(FILE *fp)
{
  out = fp;
}

byte_t *get_text()
{
  return initMachine->textData;
}

int get_program_counter()
{
  return initMachine->counter;
}

byte_t get_instruction()
{
  return initMachine->textData[initMachine->counter];
}

word_t get_constant(int index)
{
  byte_t array[4];

  for (int i = 0; i < 4; i++)
  {
    array[i] = initMachine->constantData[index * 4 + i];
  }

  word_t result = (array[0] << 24) | (array[1] << 16) | (array[2] << 8) | array[3];
  return result;
}

word_t get_local_variable(int index)
{
  int value = find_var(index);
  return value;
}

int text_size()
{
  return initMachine->textSize;
}

void print_blocks()
{
  for (int i = 0; i < initMachine->constantSize; i++)
  {
    printf("%x ", initMachine->constantData[i]);
  }
  printf("\n");
  for (int i = 0; i < initMachine->textSize; i++)
  {
    printf("%x ", initMachine->textData[i]);
  }
  printf("\n");
}