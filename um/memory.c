/* hw 6: UM
 * memory.c
 * Zoe Hsieh, Joy Kimambo
 * 4/13/20
 * 
 * purpose: Initializes and Frees our memory struct
 */ 

#include "memory.h"

/*
 *  Function: initializes memory (creating space for segmented memory)
 *  Inputs: segment 0 - Seq_T storing a programs instruction
 *  Outputs: Instance of Memory struct (see definition)
 * 
 *  Notes: Called at very beginning of emulator
 *  Test complete: init_free in test-memory
 */

Memory Mem_init(Seq_T seg0)
{
    Memory main_mem = malloc(sizeof(struct Memory)); 
    assert(main_mem != NULL);

    main_mem -> seg_holder = Seq_new(0);
    Seq_addhi(main_mem->seg_holder, seg0);

    main_mem -> program_counter = 0;
    main_mem -> seg_unmapped = Seq_new(0);

    return main_mem;

}

/*
 *  Function: frees Memory
 *  Inputs: Memory to be freed
 *  Outputs: None
 * 
 *  Notes: Frees each segment in seg_holder, seg_holder, seg_unmapped, and 
 *         the struct itself
 */
void Memory_free(Memory memory)
{
    
    assert(memory != NULL);
    
    Seq_T segments = memory -> seg_holder;

    int seg_num = Seq_length(segments);   
    int i;   
   
    for (i = 0; i < seg_num; i++)
    {
        Seq_T curr_seg= Seq_get(segments, i);
        Seq_free(&curr_seg);
    }

    Seq_free(&segments);
    Seq_free(&(memory -> seg_unmapped));

    free(memory);
}


