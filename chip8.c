/* Includes */
#include "chip8.h"

/******************************************************************************
 * GLOBAL VARIABLES
 *****************************************************************************/
uint16_t        currOpcode; /* Holds current opcode */
unsigned char   memory[4096]; /* The actual memory of the system */
unsigned char   V[16]; /* The 16, 8-bit opcodes */
uint16_t        I; /* Index register */
uint16_t        pc; /* Program counter */
unsigned char   screen[64 * 32]; /* The screen array */
uint16_t        stack[16]; /* Memory Stack */
uint16_t        sp; /* stack pointer */
unsigned char   keys[16]; /* All valid keyboard keys */

/******************************************************************************
 * Functions
 *****************************************************************************/
int main(int argc, char* argv[]) 
{
  /* TODO - literally writing the emulator */
}
