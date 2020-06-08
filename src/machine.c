#include <ijvm.h>
#include <stdlib.h>
#include <stack.h>
#include <utility.h>

//TODO CONSTANT AND TEXT BLOCKS INTO STRUCTS
struct ijvm_machine {
  int counter;
  int constantSize;
  int textSize;
  byte_t *constantData;
  byte_t *textData;
  byte_t *data;
};

static struct ijvm_machine *initMachine;
FILE *out;
FILE *in;

int init_ijvm(char *binary_file) {
  int sizeBytes;
  FILE *fp;

  // Get amount of bytes in file by moving pointer up and down
  fp = fopen(binary_file, "rb");
  fseek(fp, 0, SEEK_END);
  sizeBytes = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  
  char buffer[sizeBytes];

  // Allocate memory because it will be used in global struct
  byte_t *hexValues = (byte_t *)malloc(sizeof(char) * sizeBytes);

  // Read file containing sizeBytes of bytes and split them up in sizeof(char)
  fread(buffer, sizeof(char), sizeBytes, fp);
  
  int constantSize;
  int constantDataIndex;
  int textSize;
  int textDataIndex;
  for (int i = 0; i < sizeBytes; i++) {

    byte_t output = buffer[i];
    hexValues[i] = output;

    // Arrived at last byte of constant pool size, add the bytes of the size and note the next index as data
    if(i == 11) {
      constantSize = hexValues[i] + hexValues[i-1] + hexValues[i-2] + hexValues[i-3];
      constantDataIndex = i + 1;
    }

    // Arrived at last byte of text size, add the bytes of the size and note the next index as data
    if(i == (11+constantSize+8)) {
      textSize = hexValues[i] + hexValues[i-1] + hexValues[i-2] + hexValues[i-3];
      textDataIndex = i + 1;
    }
  }

  // Copy over data to store in struct
  byte_t *constantData = (byte_t *)malloc(sizeof(char) * constantSize);
  for (int i = 0; i < constantSize; i++) {
    constantData[i] = hexValues[constantDataIndex + i];
  }

  // Allocate memory because it will be used in global struct
  byte_t *textData = (byte_t *)malloc(sizeof(char) * textSize);
  for (int i = 0; i < textSize; i++) {
    textData[i] = hexValues[textDataIndex + i];
  }

  // Allocate memory for global struct and point to members
  initMachine = (struct ijvm_machine *)malloc(sizeof(struct ijvm_machine));
  initMachine -> counter = 0;
  initMachine -> textSize = textSize;
  initMachine -> constantSize = constantSize;
  initMachine -> data = hexValues;
  initMachine -> constantData = constantData;
  initMachine -> textData = textData;

  init_stack();

  print_blocks();
  
  return 0;
}

void destroy_ijvm() {
  free(initMachine -> data);
  free(initMachine -> constantData);
  free(initMachine -> textData);
  free(initMachine);
  destroy_stack();
  printf("---------------------------\n");
}

void run() {
  while(initMachine->counter < initMachine->textSize) {
    step();
  }
}

bool step() {
    printf("\nCURRENT INSTRUCTION: %x\n", get_instruction());

    switch (initMachine->textData[initMachine->counter]) {
    case OP_BIPUSH:
    {
      printf("BIPUSH ");

      initMachine->counter++;
      printf("%d, or %x\n", initMachine->textData[initMachine->counter],initMachine->textData[initMachine->counter]);
      push(initMachine->textData[initMachine->counter]);
      initMachine->counter++;

      break;
    }
    case OP_DUP:
    {
      printf("DUP\n");

      initMachine->counter++;
      word_t copy = top();
      push(copy);

      break;
    }
    case OP_ERR:
    {
      printf("ERR\n");

      initMachine->counter++;
      return false;

      break;
    }
    case OP_GOTO:
    {
      printf("GOTO ");

      initMachine->counter++;
      byte_t firstElement = initMachine->textData[initMachine->counter];
      printf("%x ", initMachine->textData[initMachine->counter]);
      byte_t secondElement = initMachine->textData[initMachine->counter + 1];
      printf("%x ", initMachine->textData[initMachine->counter + 1]);

      // https://stackoverflow.com/questions/56716457/how-to-convert-2-bytes-into-a-signed-short-in-c
      // https://stackoverflow.com/questions/37616480/converting-2-bytes-to-signed-int/37616634
      signed short args = (firstElement << 8) | secondElement;
      initMachine -> counter = initMachine -> counter + args - 1;
      printf(", or %d", args);
      printf("\n");

      break;
    }
    case OP_HALT:
    {
      printf("HALT\n");

      initMachine->counter++;
      return false;

      break;
    }
    case OP_IADD:
    {
      printf("IADD\n");

      initMachine->counter++;
      int firstElement = pop();
      int secondElement = pop();
      int result = firstElement + secondElement;
      push(result);

      break;
    }
    case OP_IAND:
    {
      printf("IAND\n");

      initMachine->counter++;
      int firstElement = pop();
      int secondElement = pop();  
      int result = firstElement & secondElement;
      push(result);
      
      break;
    }
    case OP_IFEQ:
    {
      printf("IFEQ ");

      initMachine->counter++;
      byte_t firstElement = initMachine->textData[initMachine->counter];
      printf("%x ", initMachine->textData[initMachine->counter]);
      byte_t secondElement = initMachine->textData[initMachine->counter + 1];
      printf("%x ", initMachine->textData[initMachine->counter + 1]);

      // https://stackoverflow.com/questions/56716457/how-to-convert-2-bytes-into-a-signed-short-in-c
      // https://stackoverflow.com/questions/37616480/converting-2-bytes-to-signed-int/37616634
      signed short args = (firstElement << 8) | secondElement;
      
      //maybe tos??
      if(pop() == 0) {
        initMachine -> counter = initMachine -> counter + args - 1;
      } else {
        initMachine -> counter = initMachine -> counter + 2;
      }
      
      printf(", or %d", args);
      printf("\n");

      break;
    }
    case OP_IFLT:
    {
      printf("IFLT ");

      initMachine->counter++;
      byte_t firstElement = initMachine->textData[initMachine->counter];
      printf("%x ", initMachine->textData[initMachine->counter]);
      byte_t secondElement = initMachine->textData[initMachine->counter + 1];
      printf("%x ", initMachine->textData[initMachine->counter + 1]);

      // https://stackoverflow.com/questions/56716457/how-to-convert-2-bytes-into-a-signed-short-in-c
      // https://stackoverflow.com/questions/37616480/converting-2-bytes-to-signed-int/37616634
      signed short args = (firstElement << 8) | secondElement;
      
      //maybe tos??
      if(pop() < 0) {
        initMachine -> counter = initMachine -> counter + args - 1;
      } else {
        initMachine -> counter = initMachine -> counter + 2;
      }
      
      printf(", or %d", args);
      printf("\n");

      break;
    }
    case OP_ICMPEQ:
    {
      printf("ICMPEQ ");

      initMachine->counter++;
      byte_t firstElement = initMachine->textData[initMachine->counter];
      printf("%x ", initMachine->textData[initMachine->counter]);
      byte_t secondElement = initMachine->textData[initMachine->counter + 1];
      printf("%x ", initMachine->textData[initMachine->counter + 1]);

      // https://stackoverflow.com/questions/56716457/how-to-convert-2-bytes-into-a-signed-short-in-c
      // https://stackoverflow.com/questions/37616480/converting-2-bytes-to-signed-int/37616634
      signed short args = (firstElement << 8) | secondElement;
      
      word_t firstPop = pop();
      word_t secondPop = pop();

      //maybe tos??
      if(firstPop == secondPop) {
        initMachine -> counter = initMachine -> counter + args - 1;
      } else {
        initMachine -> counter = initMachine -> counter + 2;
      }
      
      printf(", or %d", args);
      printf("\n");

      break;
    }
    case OP_IINC:
    {
      printf("IINC");

      initMachine->counter++;
      printf("%x\n", initMachine->textData[initMachine->counter]);
      initMachine->counter++;
      printf("%x\n", initMachine->textData[initMachine->counter]);
      initMachine->counter++;

      break;
    }
    case OP_ILOAD:
    {
      printf("ILOAD");

      initMachine->counter++;
      printf("%x\n", initMachine->textData[initMachine->counter]);
      initMachine->counter++;

      break;
    }
    case OP_IN:
    {
      printf("IN\n");

      initMachine->counter++;
      int result = getc(in);

      if (result == EOF) {
        push(0);
      } else push(result);
      
      break;
    }
    case OP_INVOKEVIRTUAL:
    {
      printf("INVOKEVIRTUAL");

      initMachine->counter++;
      for(int i = 0; i < 2; i++) {
        printf("%x ", initMachine->textData[initMachine->counter]);
        initMachine->counter++;
      };
      printf("\n");

      break;
    }
    case OP_IOR:
    {
      printf("IOR\n");

      initMachine->counter++;
      int firstElement = pop();
      int secondElement = pop();
      int result = firstElement | secondElement;
      push(result);

      break;
    }
    case OP_IRETURN:
    {
      printf("IRETURN\n");

      initMachine->counter++;

      break;
    }
    case OP_ISTORE:
    {
      printf("ISTORE");

      initMachine->counter++;
      printf("%x\n", initMachine->textData[initMachine->counter]);
      initMachine->counter++;

      break;
    }
    case OP_ISUB:
    {
      printf("ISUB\n");

      initMachine->counter++;
      int firstElement = pop();
      int secondElement = pop();
      int result = secondElement - firstElement;
      push(result);
      
      break;
    }
    case OP_LDC_W:
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
      initMachine -> counter = initMachine -> counter + 2;
      printf("\n");

      break;
    }
    case OP_NOP:
    {
      printf("NOP\n");

      initMachine->counter++;

      break;
    }
    case OP_OUT:
    {
      printf("OUT\n");

      initMachine->counter++;
      int result = pop();
      fprintf(out,"%d", result);

      break;
    }
    case OP_POP:
    {
      printf("POP\n");

      initMachine->counter++;
      pop();
      
      break;
    }
    case OP_SWAP:
    {
      printf("SWAP\n");

      initMachine->counter++;
      int firstElement = pop();
      int secondElement = pop();
      push(firstElement);
      push(secondElement);
      
      break;
    }
    case OP_WIDE:
    {
      initMachine->counter++;
      printf("WIDE\n");
      
      break;
    }
    default:
      initMachine->counter++;
      break;
    }
    return true;
}

void set_input(FILE *fp) {
  in = fp;
}

void set_output(FILE *fp) {
  out = fp;
}

byte_t *get_text() { 
  return initMachine->textData;
}

int get_program_counter() {
  return initMachine->counter;
}

byte_t get_instruction() {
  return initMachine->textData[initMachine->counter];
}

word_t get_constant(int index) {
  byte_t array[4];

  for(int i = 0; i < 4; i++) {
    array[i] = initMachine -> constantData[index * 4 + i];
  }

  word_t result = (array[0] << 24) | (array[1] << 16) | (array[2] << 8) | array[3];
  return result;

}

word_t get_local_variable(int index) {
  return 0;
}

int text_size() {
  return initMachine->textSize;
}

void print_blocks() {
  for (int i = 0; i < initMachine -> constantSize; i++) {
    printf("%x ", initMachine -> constantData[i]);
  }
  printf("\n");
  for (int i = 0; i < initMachine -> textSize; i++) {
    printf("%x ", initMachine -> textData[i]);
  }
  printf("\n");
}