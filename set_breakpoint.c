//
// Created by Marco Kaniecki on 2022-06-04.
//
#include "emulator.h"

unsigned short custom_breakpoint_loc;
unsigned short data_overwritten_at_breakpoint;

// sets breakpoint after instruction at highest memory location
void set_default_breakpoint(unsigned short adr)
{
    memory.word[(adr >> 1) + 1] = BREAK_INSTRUCTION;
}


// let the user stop program execution at specific locations
void set_custom_breakpoint(unsigned short adr)
{
    data_overwritten_at_breakpoint = memory.word[adr];

    memory.word[adr] = BREAK_INSTRUCTION;
    printf("Breakpoint changed to 0x%04X\n", adr);

    custom_breakpoint_loc = adr;
}