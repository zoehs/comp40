/*
 * Testing Memory Module
 * 
 * Written: Joy Kimambo and Zoe Hsieh
 * Date: 04/06/2020
 * 
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "memory.h"

extern void init_free();


const int EMPTY_MEM = sizeof(Seq_T)*2 + sizeof(uintptr_t); 
int main()
{
   /* test 1*/
   init_free();

}

/*
 * Test: ensure Memory is correctly initialized and freed
 * 
 */
void init_free()
{
  Seq_T seg_zero = Seq_new(2);

  Seq_put(seg_zero, 1, (void *)20);
  Seq_put(seg_zero, 0, (void *)1029);
  
  Memory main_mem = Mem_init(seg_zero);
  
  /* check correct size of empty memory*/
  if(sizeof(*main_mem) == EMPTY_MEM){
      fprintf(stderr, "SIZE TEST: PASSED\n");
  }
  
  Memory_free(main_mem);

}