#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

void disassambleChip8Op(unsigned char *codebuffer, int pc)
{
    uint8_t *code = &codebuffer[pc];

    //the first nibble of hex instructions is the opcode
    uint8_t firstnib = (code[0] >> 4);

    printf("%04x %02x %02x ", pc, code[0], code[1]);

    //the main switch statement for the opcodes
    switch (firstnib)
    {
    case 0x00:
    {
        switch (code[1])
        {
        case 0xe0:
            printf("%-10s", "CLS");
            break;
        case 0xee:
            printf("%-10s", "RTS");
            break;
        default:
            printf("UNKNOWN 0");
            break;
        }
    }
    break;
    case 0x01:
        printf("%-10s $%01x%02x", "JUMP", code[0] & 0x0f, code[1]);
        break;
    case 0x02:
        printf("%-10s $%01x%02x", "CALL", code[0] & 0x0f, code[1]);
        break;
    case 0x03:
        printf("%-10s V%01x, #$%02x", "SKIP.EQ", code[0] & 0x0f, code[1]);
        break;
    case 0x04:
        printf("%-10s V%01x, #$%02x", "SKIP.NE", code[0] & 0x0f, code[1]);
        break;
    case 0x05:
        printf("%-10s V%01x, V%01x", "SKIP.EQ", code[0] & 0x0f, code[1] >> 4);
        break;
    case 0x06:
        printf("%-10s V%01x, #$%02x", "MVI", code[0] & 0x0f, code[1]);
        break;
    case 0x07:
        printf("%-10s V%01X, %02X", "ADD", code[0] & 0x0f, code[1]);
        break;
    case 0x08:
    {
        uint8_t lastnib = code[1] & 0x0f;
        switch (lastnib)
        {
        case 0:
            printf("%-10s V%01X, V%01X", "MOV", code[0] & 0x0f, code[1] >> 4);
            break;
        case 1:
            printf("%-10s V%01X, V%01X", "OR", code[0] & 0x0f, code[1] >> 4);
            break;
        case 2:
            printf("%-10s V%01X, V%01X", "AND", code[0] & 0x0f, code[1] >> 4);
            break;
        case 3:
            printf("%-10s V%01X, V%01X", "XOR", code[0] & 0x0f, code[1] >> 4);
            break;
        case 4:
            printf("%-10s V%01X, V%01X", "ADD.", code[0] & 0x0f, code[1] >> 4);
            break;
        case 5:
            printf("%-10s V%01X, V%01X", "SUB.", code[0] & 0x0f, code[1] >> 4);
            break;
        case 6:
            printf("%-10s V%01X", "SHR.", code[0] & 0x0f);
            break;
        case 7:
            printf("%-10s V%01X, V%01X", "SUBB.", code[0] & 0x0f, code[1] >> 4);
            break;
        case 0xe:
            printf("%-10s V%01x", "SHL.", code[0] & 0x0);
            break;
        default:
            printf("UNKNOWN 8");
            break;
        }
    }
    break;
    case 0x09:
        printf("%-10s V%01x, V%01x", "SKIP.NE", code[0] & 0x0f, code[1] >> 4);
        break;
    case 0x0a:
    {
        uint8_t addresshi = code[0] & 0x0f;
        printf("%-10s I,$%01x%02x", "MVI", addresshi, code[1]);
    }
    break;
    case 0x0b:
        printf("%-10s $%01x%02x(V0)", "JUMP", code[0] & 0x0f, code[1]);
        break;
    case 0x0c:
        printf("%-10s V%01X,#$%02X", "RNDMSK", code[0] & 0x0f, code[1]);
        break;
    case 0x0d:
        printf("%-10s V%01X, V%01X, #$%01X", "SPRITE", code[0] & 0x0f, code[1] >> 4, code[1] & 0x0f);
        break;
    case 0x0e:
    {
        switch (code[1])
        {
        case 0x9e:
            printf("%-10s V%01X", "SKIPKEY.Y", code[0] & 0x0f);
            break;
        case 0xa1:
            printf("%-10s V%01X", "SKIPKEY.N", code[0] & 0x0f);
            break;
        default:
            printf("UNKNOWN E");
            break;
        }
    }
    break;
    case 0x0f:
    {
        switch (code[1])
        {
        case 0x07:
            printf("%-10s V%01X, DELAY", "MOV", code[0] & 0x0f);
            break;
        case 0x0a:
            printf("%-10s V%01X", "WAITKEY", code[0] & 0x0f);
            break;
        case 0x15:
            printf("%-10s DELAY, V%01X", "MOV", code[0] & 0x0f);
            break;
        case 0x18:
            printf("%-10s SOUND, V%01X", "MOV", code[0] & 0x0f);
            break;
        case 0x1e:
            printf("%-10s I, V%01X", "ADD", code[0] & 0x0f);
            break;
        case 0x29:
            printf("%-10s I, V%01X", "SPRITECHAR", code[0] & 0x0f);
            break;
        case 0x33:
            printf("%-10s (I), V%01X", "MOVBCD", code[0] & 0x0f);
            break;
        case 0x55:
            printf("%-10s (I), V0-V%01X", "MOVM", code[0] & 0x0f);
            break;
        case 0x65:
            printf("%-10s V0-V%01X, (I)", "MOVM", code[0] & 0x0f);
            break;
        default:
            printf("UNKNOWN F");
            break;
        }
    }
    break;
    }
}

typedef struct Chip8State
{
    uint8_t V[16]; //registers 0-f
    uint8_t key_states[16]; //array of all the keys and their states, on or off
    uint16_t I; //memory address register
    uint16_t SP; //stack pointer
    uint16_t PC; //program counter
    uint8_t sound; //sound timer 
    uint8_t delay; // delay timer
    uint8_t *memory; //memory of the emulator
    uint8_t *screen; // this is memory[0xf00]
} Chip8State;

Chip8State* initChip8(void) 
{
    Chip8State* state = calloc(sizeof(Chip8State), 1);

    //initialize the members 
    state->memory = calloc(4096, 1); //initialize the memory of the device
    state->screen = &state->memory[0xf00]; //where the screen starts in memory

    state->SP = 0xfa0;
    state->PC = 0x200;
    
    return state;
}

void EmulateChip8Op(Chip8State *state) 
{
    uint8_t *op = &state->memory[state->PC];

    uint8_t highnib = (*op & 0x0f) >> 4;
    switch(highnib) {
        case 0x00: UnimplementedInstruction(state); break;
        case 0x01: //JUMP to address NNN : 1NNN
            {
                uint16_t target = ((code[0] & 0xf) << 8) | code[1];
                state->PC = target;
            }
            break;
        case 0x02: UnimplementedInstruction(state); break;
        case 0x03: //SKIP.EQ
            {
                uint8_t register = code[0] & 0x0f;
                if(state->V[register] == code[2]) {
                    state->PC += 2;
                }
                state->PC +=2;
            }
            break;
        case 0x04: //SKIP.NE    VX, NN
            {
                uint8_t register = code[0] & 0x0f;
                if (state->V[register] != code[2])
                {
                    state->PC += 2;
                }
                state->PC += 2;
            }
            break;
        case 0x05: //SKIP.EQ    VX,VY
            {
                uint8_t reg1 = code[0] & 0x0f;
                uint8_t reg2 = code[1] >> 4;
                if (state->V[reg1] != state->V[reg2])
                {
                    state->PC += 2;
                }
                state->PC += 2;
            }
            break;
        case 0x06: //MOV        VX,NN
            {
                uint8_t reg = code[0] & 0x0f;
                state->V[reg] = code[1];
                state->PC += 2;
            }
            break;
        case 0x07: //ADD    VX, NN
            {
                uint8_t reg = code[0] & 0x0f;
                state->V[reg] += code[1];
                state->PC += 2;
            }
            break;
        case 0x08: UnimplementedInstruction(state); break;
        case 0x09: //SKIP.NE    VX, VY
            {
                uint8_t reg1 = code[0] & 0x0f;
                uint8_t reg2 = code[1] >> 4;
                if(state->V[reg1] != state->V[reg2]) {
                    state->PC += 2;
                }
                state->PC += 2;
            }
            break;
        case 0x0a:
            {
                uint16_t addr = ((code[0] & 0x0f) << 8) | code[1];
                state->PC += 2;
            }
            break;
        case 0x0b: 
            {
                uint16_t addr = ((code[0] & 0x0f) << 8) | code[1];
                state->PC = addr + state->V[0];
                state->PC += 2;
            }
            break;
        case 0x0c: 
            {
                uint8_t reg = code[0] & 0x0f;
                state->V[reg] = random() & code[1];
                state->PC += 2;
            }
            break;
        case 0x0d: UnimplementedInstruction(state); break;
        case 0x0e: 
            {
                uint8_t reg = code[0] & 0x0f;
                switch(code[1]) {
                    case 0x9e: 
                        {
                            if(state->key_states[state->V[reg]] != 0) {
                                state->PC += 2;
                            }
                        }
                        break;
                    case 0xa1: 
                        {
                            if(state->key_states[state->V[reg] == 0]) {
                                state->PC += 2;
                            }
                        }
                        break;
                    default: UnimplementedInstruction(state); break;
                }
            }
            state->PC += 2;
            break;
        case 0x0f: 
            uint8_t reg = code[0] & 0x0f;
            switch(code[1]) {
                case 0x07: state->V[reg] = state->delay; break; //MOV VX, DELAY
                case 0x15: state->delay = state->V[reg]; break; //MOV DELAY, VX
                case 0x18: state->sound = state->V[reg]; break; //MOV SOUND, VX
                case 0x33: //BSD MOV
                    {
                        //value of register to binary coded decimal
                        uint8_t ones, tens, hundreds;
                        uint8_t value = state->V[reg];

                        //assignment of the values
                        ones = value % 10;
                        value /= 10;
                        tens = value % 10;
                        hundreds = value / 10;

                        //make the necessary memory changes
                        state->memory[state->I] = hundreds;
                        state->memory[state->I + 1] = tens;
                        state->memory[state->I + 2] = ones;

                        state->PC += 2;
                    }
                    break;
                
            }
            break; 
    }
}