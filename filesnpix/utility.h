/* 
 * utility.h
 * Zoe Hsieh, Rebecca Miller, 1/31/20
 * 
 * interface of general utility functions
 */

#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "except.h"

void free_string(char **s); /* frees memory for string 
                             * and sets pointer to null */
void check_null_ptr(void *p); /* checks if p is NULL and 
                               * terminates if it is */


#endif