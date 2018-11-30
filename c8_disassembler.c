#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

void disassambleChip8Op(unsigned char  *codebuffer, int pc)
{
  uint8_t *code = &codebuffer[pc];

  //the first nibble of hex instructions is the opcode
  uint8_t firstnib = (code[0] >> 4);

  printf("%04x %02x %02x ", pc, code[0], code[1]);
  
  //the main switch statement for the opcodes
  switch(firstnib)
    {
    case 0x00:
      {
	switch(code[1])
	  {
	  case 0xe0: printf("%-10s" , "CLS"); break;
	  case 0xee: printf("%-10s", "RTS"); break;
	  default: printf("UNKNOWN 0"); break;
	  }
      }
      break;
    case 0x01: printf("%-10s $%01x%02x", "JUMP", code[0]&0x0f, code[1]); break;
    case 0x02: printf("%-10s $%01x%02x", "CALL", code[0]&0x0f, code[1]); break;
    case 0x03: printf("%-10s V%01x, #$%02x", "SKIP.EQ", code[0]&0x0f, code[1]); break;
    case 0x04: printf("%-10s V%01x, #$%02x", "SKIP.NE", code[0]&0x0f, code[1]); break;
    case 0x05: printf("%-10s V%01x, V%01x", "SKIP.EQ", code[0]&0x0f, code[1] >> 4); break;
    case 0x06: printf("%-10s V%01x, #$%02x", "MVI", code[0]&0x0f, code[1]); break;
    case 0x07: printf(" 7 not handled yet"); break;
    case 0x08:
      {
	uint8_t lastnib = code[1] & 0x0f;
	switch (lastnib)
	  {
	  case 0: printf("%-10s V%01X, V%01X", "MOV", code[0]&0x0f, code[1] >> 4); break;
	  case 1: printf("%-10s V%01X, V%01X", "OR", code[0]&0x0f, code[1] >> 4); break;
	  case 2: printf("%-10s V%01X, V%01X", "AND", code[0]&0x0f, code[1] >> 4); break;
	  case 3: printf("%-10s V%01X, V%01X", "XOR", code[0]&0x0f, code[1] >> 4); break;
	  case 4: printf("%-10s V%01X, V%01X", "ADD.", code[0]&0x0f, code[1] >> 4); break;
	  case 5: printf("%-10s V%01X, V%01X", "SUB.", code[0]&0x0f, code[1] >> 4); break;
	  case 6: printf("%-10s V%01X", "SHR.", code[0]&0x0f); break;
	  case 7: printf("%-10s V%01X, V%01X", "SUBB.", code[0]&0x0f, code[1] >> 4); break;
	  case 0xe: printf("%-10s V%01x", "SHL.", code[0]&0x0); break;
	  default: printf("UNKNOWN 8"); break;
	  }
      }
      break;
    case 0x09: printf("%-10s V%01x, V%01x", "SKIP.NE", code[0]&0x0f, code[1] >> 4); break;
    case 0x0a:
      {
	uint8_t addresshi = code[0] & 0x0f;
	printf("%-10s I,$%01x%02x", "MVI", addresshi, code[1]);
      }
      break;
    case 0x0b: printf("%-10s $%01x%02x(V0)", "JUMP", code[0]&0x0f, code[1]); break;
    case 0x0c: printf(" c not handled yet"); break;
    case 0x0d: printf(" d not handled yet"); break;
    case 0x0e: printf(" e not handled yet"); break;
    case 0x0f: printf(" f not handled yet"); break;
    }
}

int main(int argc, char** argv)
{
  FILE *f = fopen(argv[1], "rb");
  if(f == NULL) {
    printf(" error: Couldn't open %s\n", argv[1]);
      exit(1);
  }

  //move the file pointer to the end of the file
  fseek(f, 0L, SEEK_END);

  //the size is the current position of the file pointer
  int fsize = ftell(f);

  //set the file pointer back to the beginning of the file
  fseek(f, 0L, SEEK_SET);

  //CHIP-8 programs written for the original system start at
  //location 0x200

  //read file into memory at 0x200 and close it
  unsigned char *buffer = malloc(fsize + 0x200);
  fread(buffer+0x200, fsize, 1, f);
  fclose(f);

  //make the program counter start at 0x200
  int pc = 0x200;
  while (pc < (fsize + 0x200)) {
    disassambleChip8Op(buffer, pc);
    pc += 2;
    printf("\n");
  }
  free(buffer);
  return 0;
  
}
