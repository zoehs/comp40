/* 
 * simlines.c
 * Zoe Hsieh, Rebecca Miller, 1/31/20
 * 
 * interface for general custom functions for dealing with strings
 */

#ifndef STRING_FUNCS_H
#define STRING_FUNCS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include "except.h"
#include "utility.h"

char *to_simple_string(char *line, size_t len); /* makes a new string that has
                                                 * no extra spaces or weird 
                                                 * characters */
bool is_alphanum(char c, bool underscore); /* determines if the character 
                                            * is a letter, number, 
                                            * or underscore */

#endif