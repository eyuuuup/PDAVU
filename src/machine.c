#include <ijvm.h>

struct ijvm_machine {
  int counter;
  int constantSize;
  int textSize;
  byte_t *constantData;
  byte_t *textData;
  byte_t *data;
};

static struct ijvm_machine initMachine;

int init_ijvm(char *binary_file)
{
  
  struct ijvm_machine machine;
  int sizeBytes;
  FILE *fp;

  //get amount of bytes in file by moving pointer up and down
  fp = fopen(binary_file, "rb");
  fseek(fp, 0, SEEK_END);
  sizeBytes = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  
  char buffer[sizeBytes];
  byte_t hexValues[sizeBytes];

  //read file containing sizeBytes of bytes and split them up in sizeof(char)
  fread(buffer, sizeof(char), sizeBytes, fp);
  
  int constantSize;
  int constantDataIndex;
  int textSize;
  int textDataIndex;
  for (int i = 0; i < sizeBytes; i++) {
    byte_t output = buffer[i];
    hexValues[i] = output;
    //arrived at last byte of constant pool size, add the bytes of the size and note the next index as data
    if(i == 11) {
      constantSize = hexValues[i] + hexValues[i-1] + hexValues[i-2] + hexValues[i-3];
      constantDataIndex = i + 1;
    }

    //arrived at last byte of text size, add the bytes of the size and note the next index as data
    if(i == (11+constantSize+8)) {
      textSize = hexValues[i] + hexValues[i-1] + hexValues[i-2] + hexValues[i-3];
      textDataIndex = i + 1;
    }

  }

  //copy over data to store in struct
  byte_t constantData[constantSize];
  for (int i = 0; i < constantSize; i++) {
    constantData[i] = hexValues[constantDataIndex + i];
  }

  byte_t textData[textSize];
  for (int i = 0; i < textSize; i++) {
    textData[i] = hexValues[textDataIndex + i];
    
  }

  
  machine.counter = 0;
  machine.textSize = textSize;
  machine.constantSize = constantSize;
  machine.data = hexValues;
  machine.constantData = constantData;
  machine.textData = textData;
  initMachine = machine;

  
  for (int i = 0; i < initMachine.constantSize; i++) {
    printf("%x ", initMachine.constantData[i]);
  }
  printf("\n");
  for (int i = 0; i < initMachine.textSize; i++) {
    printf("%x ", initMachine.textData[i]);
  }
  printf("\n");
  
  return 0;
}



void destroy_ijvm()
{

}

void run()
{

  while(initMachine.counter < initMachine.textSize) {
    switch (initMachine.textData[initMachine.counter])
    {
    case 0x10:
      printf("BIPUSH ");
      initMachine.counter++;
      printf("%d\n", initMachine.textData[initMachine.counter]);
      initMachine.counter++;
      break;
    case 0x60:
      printf("IADD\n");
      initMachine.counter++;
      break;
    case 0xFD:
      printf("OUT\n");
      initMachine.counter++;
      break;
    case 0xFF:
      printf("HALT\n");
      initMachine.counter++;
      break;
    default:
      break;
    }
  }
}

void set_input(FILE *fp)
{
  // TODO: implement me
}

void set_output(FILE *fp)
{
  // TODO: implement me
}

byte_t *get_text() { 
  return initMachine.textData;
}

int get_program_counter() {
  return initMachine.counter;
}

byte_t get_instruction() {

}

