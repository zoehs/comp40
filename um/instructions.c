/* hw 6: UM
 * instructions.c
 * Zoe Hsieh, Joy Kimambo
 * 4/13/20
 * 
 * purpose: defines 14-RISC style instructions executed by the UM, and runs
 *          the instructions
 * 
 * Note: due to time restrictions, no assertions are made.
 *       Failure modes include: segfaults
 */


#include "instructions.h"

typedef uint32_t UM_instruction;

typedef enum Um_opcode {
        CMOV = 0, SEG_LOAD, SEG_STORE, ADD, MUL, DIV,
        NAND, HALT, MAP, UNMAP, OUT, IN, LOAD_PROG, LOAD_VAL
} Um_opcode;            /* OP_codes correlate to instuctions that should be 
                           executed */

uint32_t registers[8] = {0};


/*
 *  Function: Excecutes one of the 14 RISC instructions 
 *  Inputs: Memory 
 *  Outputs: None
 * 
 *  CREs Raised: if Memory or seg_holder is NULL
 *              
 *  Notes: This function acts as main component that decodes
 *         all instructions stored in to execute them
 * 
 */
void run_instructs(Memory main_memory)
{
    assert(main_memory != NULL);

    /* ensure memory segment is mapepd*/
    Seq_T seg_hold = main_memory -> seg_holder;
    assert(seg_hold != NULL);
    
    /* get length of instruction stream*/
    Seq_T seg_zero = Seq_get(seg_hold, 0);
    int stream_length = Seq_length(Seq_get(seg_hold, 0));

    while((main_memory->program_counter) < stream_length) {

        seg_zero = Seq_get(seg_hold, 0);
        stream_length = Seq_length(Seq_get(seg_hold, 0));

        /* get next instruction*/
        UM_instruction um_i = (uintptr_t)Seq_get(seg_zero, 
                                                 main_memory->program_counter);
        
        uint32_t op_code, reg_a, reg_b, reg_c, value;

        op_code = Bitpack_getu(um_i,4,28);

        if (op_code == LOAD_VAL) {
            reg_a = Bitpack_getu(um_i, 3, 25);
            value = Bitpack_getu(um_i, 25, 0);

            load_value(reg_a, value);
            (main_memory->program_counter)++;
        }
        else {
            reg_a = Bitpack_getu(um_i,3,6);
            reg_b = Bitpack_getu(um_i,3,3);
            reg_c = Bitpack_getu(um_i,3,0);

            /* determine RISC-instruction and excecute*/
            switch (op_code)
            {
                case CMOV:
                    cond_move(reg_a, reg_b, reg_c);
                    (main_memory->program_counter)++;
                    break;

                case SEG_LOAD:
                    load_seg(main_memory, reg_a, reg_b, reg_c);
                    (main_memory->program_counter)++;
                    break;

                case SEG_STORE:
                    store_seg(main_memory, reg_a, reg_b, reg_c);
                    (main_memory->program_counter)++;
                    break;

                case ADD:
                    add(reg_a, reg_b, reg_c);
                    (main_memory->program_counter)++;
                    break;

                case MUL:
                    mult(reg_a, reg_b, reg_c);
                    (main_memory->program_counter)++;
                    break;
                    
                case DIV:
                    divide(reg_a, reg_b, reg_c);
                    (main_memory->program_counter)++;
                    break;
                    
                case NAND:
                    nand(reg_a, reg_b, reg_c);
                    (main_memory->program_counter)++;
                    break;
                
                case HALT:
                    halt(main_memory);
                    (main_memory->program_counter)++;
                    break;

                case MAP:
                    map_seg(main_memory, reg_b, reg_c);
                    (main_memory->program_counter)++;
                    break;
                
                case UNMAP:
                    unmap_seg(main_memory, reg_c);
                    (main_memory->program_counter)++;
                    break;

                case OUT:
                    output(reg_c);
                    (main_memory->program_counter)++;
                    break;

                case IN:
                    input(stdin, reg_c);
                    (main_memory->program_counter)++;
                    break;
                
                case LOAD_PROG:
                    load_program(main_memory, reg_b, reg_c);
                    break;
            }
        }
    }
    
}


/*
 *  Function: conditional move
 *  Inputs: 3 registers specified by UM_instruction
 *  Outputs: none
 * 
 *  Notes: checks if register C is 0, if not then register A = register B
 */ 
void cond_move(uint32_t A, uint32_t B, uint32_t C)
 {
    if (registers[C] != 0) {
        registers[A] = registers[B];
    }
 }

/*
 *  Function: add
 *  Inputs: 3 uint32_t s
 *         1.register  A - where the sum of b + c is stored
 *         2. registers B and C - store numbers to be added
 *  Outputs: none
 * 
 *  Notes: adds values stored in register B and C and mod 2^32 the result, 
 *         stores result in register A
 */
void add(uint32_t A, uint32_t B, uint32_t C) 
 {
    registers[A] = (registers[B] + registers[C]) % 4294967296;
 }

/*
 *  Function: multiplication
 *  Inputs: 3 uint32_t 
 *          1. register A - stores product
 *          2. registers B and C - holds the numbers to multiplied 
 *  Outputs: none
 * 
 *  Notes: multiplies register B and C and mod 2^32, stores in register A
 */
void mult(uint32_t A, uint32_t B, uint32_t C) 
{
    registers[A] = (registers[B] * registers[C]) % 4294967296;
}

/*
 *  Function: division
 *  Inputs: 3 uint32_t 
 *          1. register A - stores quotient
 *          2. register B - stores dividend
 *          3. register C - stores divisor
 *  Outputs: none
 * 
 *  Notes: divides registers B by C, stores result in register A
 */
void divide(uint32_t A, uint32_t B, uint32_t C)
{
    registers[A] = (registers[B] / registers[C]);
}

/*
 *  Function: NAND
 *  Inputs: 3 uint32_t
 *         1. register A - stores result of ~(bitwise &)
 *         2. register B and C, subject to the bitwise logical statements
 *  Outputs: none
 * 
 *  Notes: NANDS register B and C, stores in register A
 */
void nand(uint32_t A, uint32_t B, uint32_t C)
{
    registers[A] = ~(registers[B] & registers[C]);
}
 
/*
 *  Function: halt
 *  Inputs: Memory struct
 *  Outputs: none
 * 
 *  Notes: frees all memory and exits program
 */
void halt(Memory memory) 
{
    Memory_free(memory);
    exit(0);
}

/*
 *  Function: output
 *  Inputs: regiser C - stores char to be output
 *  Outputs: none
 * 
 *  CRE's raised : checks if value outputted is possible
 *  Notes: outputs register C to stdout
 */
void output(uint32_t C)
{    
    assert(registers[C] < 256);
    putchar(registers[C]);
}

/*
 *  Function: input
 *  Inputs: FILE *instruction, 
 *          register C - stores input from stdin
 *  Outputs: none
 * 
 *  Notes: reads in file from stdin and stores in C
 */
void input(FILE *instruction, uint32_t C)
{    
    uint32_t instruct = getc(instruction);
    
    if (instruct == (uint32_t) EOF) {
       instruct = ~0;
    }
    registers[C] = instruct;
}

/*
 *  Function: load_program
 *  Inputs: Memory memory, 2 uint32_t's:
 *          1. Register B - stores index of segment whose program is loaded
 *          2. Register C - The word/ instruction offset for which the um
 *                          should continue to execute
 *  Outputs: none
 * 
 *  Notes: duplicates sequence at register B and replaces memory's segment 0
 *         program counter is set to register C
 */
void load_program(Memory memory, uint32_t B, uint32_t C)
{   
    if (registers[B] != 0) {
        /* free old segement 0 */
        Seq_T seg = Seq_get(memory->seg_holder, 0);
        Seq_free(&seg);

        /*new segment */
        Seq_T curr_seg = Seq_get(memory->seg_holder, registers[B]);
        int length = Seq_length(curr_seg);

        /* copy into Memory's seg0 */
        Seq_T copy_seg = Seq_new(length);
        int i;
        for ( i = 0; i < length; i++) {
            Seq_addhi(copy_seg, Seq_get(curr_seg, i));
        }
        Seq_put(memory->seg_holder, 0, copy_seg);
    }
    memory->program_counter = registers[C]; 
}

/*  Function: loads value stored in segment B, word C
 *             and stores into a register A
 *  Inputs: Mmeory, 3 uint32_t's
 *          1. Register B - stores index of segment to load
 *          2. Register C - stores index of word to load
 *          3. Register A - location of where loaded value is stored
 *  Outputs: None
 *               
 *  Notes: get the segment at location (register B, C) and stores in reg A
 *  
 */ 
void load_seg(Memory memory, uint32_t A, uint32_t B, uint32_t C)
{
    Seq_T seg = Seq_get(memory->seg_holder, registers[B]);
    registers[A] = (uintptr_t)Seq_get(seg, registers[C]);
}

/*
 *  Function: store segment
 *  Inputs: Memory memory, 3 uint32_t's:
 *          1. Register A: index of segment to load
 *          2. Register B: index of word to load
 *          3. Register C: word to store
 *
 *  Outputs: none
 * 
 *  Notes: stores contents of register C in memory @ location register A,B
 */
void store_seg(Memory memory, uint32_t A, uint32_t B, uint32_t C)
{    
    Seq_T seg = Seq_get(memory->seg_holder, registers[A]);
    Seq_put(seg, registers[B], (void *) (uintptr_t) registers[C]);
}

/*
 *  Function: map segment
 *  Inputs: Memory memory, 2 uint32_t s
 *          regisger B -  will store the segment number that new
 *                        segment is mapped to
 *          register C -  stores length of new segment to be mpped
 *  Outputs: none
 * 
 *  Notes: creates segment with length C
 *         if unmapped segments is empty, just add new seg 
 *         if not empty, free a segment taken from unmapped seg indexes
 *         and replace 
 * 
 */
void map_seg(Memory memory, uint32_t B, uint32_t C)
{
    int length = registers[C];
    Seq_T new_seg = Seq_new(length);

    int i;
    for ( i = 0; i < length; i++) {
        Seq_addhi(new_seg, 0);
    }
    
    uint32_t seg_index = 0;
 
    /*if unmapped is empty, add to it */
    if (Seq_length(memory->seg_unmapped) == 0) {
        Seq_addhi(memory->seg_holder, new_seg);
        seg_index = (Seq_length(memory->seg_holder) - 1);
    }
    else {
        seg_index = (uintptr_t) Seq_remlo(memory->seg_unmapped);
        Seq_T seg = Seq_get(memory->seg_holder, seg_index);
        Seq_free(&seg);

        Seq_put(memory->seg_holder, seg_index, new_seg);    
    }
    registers[B] = seg_index;
}

/*
 *  Function: unmap segment
 *  Inputs: Memory memory,  register C - segment to be unmapped
 *  Outputs: none
 * 
 *  Notes:
 *        When testing: does unmap make index point to NULL?
 */
void unmap_seg(Memory memory, uint32_t C)
{   
    /* Update the seg_unmapped to store the index of the unmapped segment */
    Seq_addhi(memory->seg_unmapped, (void *)(uintptr_t) registers[C]);
}

/*
 *  Function: loads value into register
 *  Inputs: index of register to store value
 *           value thats stored
 *  Outputs: none
 * 
 *  Notes: loads value into register A
 */
void load_value(uint32_t A, int value)
{
    registers[A] = value;
}
