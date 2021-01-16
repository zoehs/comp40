/* hw 6: UM
 * load.c
 * Zoe Hsieh, Joy Kimambo
 * 2/2/20
 * 
 * purpose: The load module reads a um program and stores all program
 *          instructions to be executed into memory
 *          It is also responsible for ensuring the program is executed
 *          
 */

#include "load.h"

/*
 *  Function: reads, stores and runs um program 
 *  Input: FILE * (the um program), file size (bytes)
 *  Ouput: None
 * 
 *  Notes: 
 * 
 */
void load_umfile(FILE *fp, int file_size) 
{
    /*get segment 0*/
    Seq_T seg_zero = Seq_new(file_size);

    int byte_a, byte_b, byte_c, byte_d;
    while ((byte_a = fgetc(fp)) != EOF) {

        /*get 32 bit*/
        byte_b = fgetc(fp);
        byte_c = fgetc(fp);
        byte_d = fgetc(fp);
        
        uint32_t codeword = 0;
        /*pack the bits in big-endian*/
        codeword = Bitpack_newu(codeword, 8, 24, byte_a);
        codeword = Bitpack_newu(codeword, 8, 16, byte_b);        
        codeword = Bitpack_newu(codeword, 8, 8, byte_c);        
        codeword = Bitpack_newu(codeword, 8, 0, byte_d);        

        /* add code to instruction sequence */
        Seq_addhi(seg_zero, (void *) (uintptr_t) codeword);

    }
    Memory memory = Mem_init(seg_zero);

    fclose(fp);

    run_instructs(memory);

    Memory_free(memory);
}
