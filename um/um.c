/* hw 6: UM
 * um.c
 * Zoe Hsieh, Joy Kimambo
 * 4/13/20
 * 
 * purpose: um is an emulator of a UNIVERSAL MACHINE
 *          This program reads in um programs and can execute all
 *          instructions as would a UM
 * 
 * Notes: Program makes use of Memory, Load , Instructions and Bitpack 
 *        modules to emulate the UM.
 */

#include <stdio.h>
#include <stdlib.h>
#include "assert.h"
#include <string.h>
#include "load.h" 
#include "memory.h"
#include <sys/stat.h>

extern int file_size(char* um_file);
int main(int argc, char *argv[]) 
{
    /* read in file */
    assert(argc == 2);

    FILE* um_file = fopen(argv[1], "r");
    assert(um_file != NULL);

    struct stat info;
    stat(argv[1], &info);
    int size = info.st_size;

    if (size == 4 || size % 4 != 0) {
        fprintf(stderr, "Incorrect file format\n");
        fprintf(stderr, "Expected format: [filename].um\n");
        exit(EXIT_FAILURE);
    }

    load_umfile(um_file, size);    
    
    exit(EXIT_SUCCESS);
}
