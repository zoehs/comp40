/* hw 6: UM
 * memory.h
 * Zoe Hsieh, Joy Kimambo
 * 4/13/20
 * 
 * purpose: functions that free and initialize memory
 */

#ifndef MEMORY_H
#define MEMORY_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assert.h"
#include <seq.h>


struct Memory {
    Seq_T seg_holder; /*2D structure that holds program and all
                    data types required by program*/
    int program_counter; /*holds index of next instruction*/
    Seq_T seg_unmapped; /* keeps track of all unmapped segment indexes */
    
};
typedef struct Memory *Memory;


extern Memory Mem_init(Seq_T seg0); 
extern void Memory_free(Memory memory);
           
             
#endif

