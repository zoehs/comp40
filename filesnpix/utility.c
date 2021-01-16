/* 
 * utility.c
 * Zoe Hsieh, Rebecca Miller, 1/31/20
 * 
 * implementation of general utility functions
 */

#include "utility.h"

/*
 * name : free_string
 * purp : frees memory for string and sets pointer to null
 * args : char ** (pointer to string)
 * rets : none
 */
  
void free_string(char **s)
{
    free(*s);
    *s = NULL;
}

/*
 * name : check_null_ptr
 * purp : checks if p is NULL and terminates if it is
 * args : void *p
 * rets : none
 */

void check_null_ptr(void *p)
{
    if ( p == NULL ) {
        exit(EXIT_FAILURE);
    }
}

