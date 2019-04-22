#include <stdio.h>
#include <stdlib.h>

void show_usage_and_exit() {
  printf("./disassemble <file name>\n");
  exit(1);
}

int main(int argc, char** argv) {
  //make sure that the arguments are correct
  if(argc != 2) {
    show_usage_and_exit();
  }

  //open the program file
  FILE *file;
  char* filename = argv[1];
  if((file = fopen(filename, "rb")) == NULL) {
    printf("The file could not be opened!\n");
    exit(1);
  } else {
    printf("%s opened successfully\n", filename);
  }

  //find the size of the file
  fseek(file, 0L, SEEK_END);
  int size = ftell(file);
  fseek(file, 0L, SEEK_SET);

  //save the contents to a buffer
  unsigned char *buffer = malloc(size);
  fread(buffer, sizeof(*buffer), size, file);

  //print the contents
  for(int i = 0; i < size; i++) {
      printf("%02x ", buffer[i]);
  }

  //free the memory
  free(buffer);
  fclose(file);
}
