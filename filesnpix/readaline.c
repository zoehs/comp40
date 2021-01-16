/* 
 * readaline.c
 * Zoe Hsieh, Rebecca Miller, 1/31/20
 * hw1 : filesnpix
 * 
 * reads in a line and returns the number of bytes
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "readaline.h"

void check_null_p(void *p);
void check_null_cpp(char **cpp);
void check_invalid_char(int c);
void expand_p(size_t *size, size_t index, char **datapp);


/*
 * name : readaline
 * purp : determines size of a line and reads line from file
 * args : FILE * (pointer to file), char ** (pointer to string)
 * rets : size_t size of line in bytes
 * note : it is the responsibility of the caller to free the pointer
 * note : if called when nothing left to read, sets datapp to NULL 
 *        and rets 0
 */

size_t readaline(FILE *inputfd, char **datapp)
{    
    check_null_p(inputfd);
    check_null_cpp(datapp);

    size_t i = 0;
    int temp;
  
    temp = fgetc(inputfd);
    check_invalid_char(temp);
  
    if ( temp == -1 ) {
        datapp = NULL;
        return 0;
    }
  
    size_t size = sizeof(char) * 200;
    *datapp     = malloc(size);
    check_null_p(*datapp);
  
    do { 
        if ( i > 0 ) {
            temp = fgetc(inputfd);
            check_invalid_char(temp);
        }
    
        if ( temp == -1 ) {
            break; /* end of file */
        }
        else {
            (*datapp)[i] = temp;
            expand_p(&size, i, datapp);
        }
        i++;
    } while ( (*datapp)[i - 1] != '\n' );

    if ( i == 0 ) {
        *datapp = NULL;
        datapp  = NULL;
    }
    else {
        (*datapp)[i] = '\0';
    }
    return i;
}

/****************************** HELPER FUNCTIONS *****************************/

/*
 * name : check_null_p
 * purp : checks if p is NULL and terminates if it is
 * args : void *p
 * rets : none
 */

void check_null_p(void *p)
{
    if ( p == NULL ) {
        exit(EXIT_FAILURE);
    }
}

/*
 * name : check_null_cpp
 * purp : checks if cpp is NULL and terminates if it is
 * args : char *pp
 * rets : none
 */

void check_null_cpp(char **cpp) 
{
    if ( cpp == NULL ) {
        exit(EXIT_FAILURE);
    }
}

/*
 * name : check_invalid_char
 * purp : checks for ASCII characters and terminates if invalid
 * args : int c
 * rets : none
 * note : -1 is not considered invalid since this may indicate eof
 */

void check_invalid_char(int c)
{
    if ( c < -1 || c > 255 ) {
        exit(EXIT_FAILURE);
    }
}

/*
 * name : expand_p
 * purp : reallocates size of *datapp when the size of the already 
 *        allocated space has been filled
 * args : size_t *size (current max size of the string), size_t index 
 *        (size of the current index), char **datapp (pointer to the string)
 * rets : none
 */

void expand_p(size_t *size, size_t index, char **datapp)
{
    if ( index == *size - 1 ) {
        *size *= 2;
        *datapp = realloc(*datapp, *size);
    }
}
