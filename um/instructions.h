/* hw 6: UM
 * instructions.h
 * Zoe Hsieh, Joy Kimambo
 * 4/13/20
 * 
 * purpose: functions to run and define the instructions 
 */

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include "assert.h"
#include <string.h>
#include <stdint.h>
#include "bitpack.h"
#include "memory.h"


/* Manages all instruciton to be run*/
extern void run_instructs(Memory main_memory);

/*14 RISC Instructions  -- see function contracts in implemetation --*/
extern void cond_move(uint32_t A, uint32_t B, uint32_t C);
extern void add(uint32_t A, uint32_t B, uint32_t C) ;
extern void mult(uint32_t A, uint32_t B, uint32_t C);
extern void divide(uint32_t A, uint32_t B, uint32_t C);
extern void nand(uint32_t A, uint32_t B, uint32_t C);
extern void halt(Memory memory);
extern void output(uint32_t C);
extern void input(FILE *instruction, uint32_t C);
extern void load_program(Memory memory, uint32_t B, uint32_t C);
extern void load_seg(Memory memory, uint32_t A, uint32_t B, uint32_t C);
extern void store_seg(Memory memory, uint32_t A, uint32_t B, uint32_t value);
extern void map_seg(Memory memory, uint32_t B, uint32_t C);
extern void unmap_seg(Memory memory, uint32_t C);
extern void load_value(uint32_t A, int value);

#endif