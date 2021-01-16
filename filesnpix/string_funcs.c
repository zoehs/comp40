/* 
 * simlines.c
 * Zoe Hsieh, Rebecca Miller, 1/31/20
 * 
 * implementation for general custom functions for dealing with strings
 */

#include "string_funcs.h"

/*
 * name : to simple_string
 * purp : makes a new string that has no extra spaces or weird characters
 * args : char *line, size_t length of the line
 * rets : char *
 */

char *to_simple_string(char *line, size_t len)
{
    size_t i, j = 0;
    bool space         = false;
    bool letter_exists = false;

    char *new_line = malloc(sizeof(char) * len + 1);
    check_null_ptr(new_line);
  
    for ( i = 0; i < len; i++ ) {
        if ( is_alphanum(line[i], true) ) {
            new_line[j] = line[i];
            j++;
            space = false;
            letter_exists = true;
        }
        else {      
            if ( !space && letter_exists ) {
                new_line[j] = ' ';
                j++;
                space = true;
            }
        }
    } 
  
    new_line[j] = '\0';
  
    return new_line;
}

/*
 * name : is_alphanum
 * purp : determines if the character is a letter, or number, 
 *      or underscore if specified true
 * args : char c, bool underscore
 * rets : true false
 */

bool is_alphanum(char c, bool underscore)
{
    if ( ( c >= '0' && c <= '9' ) || 
         ( c >= 'A' && c <= 'Z' ) || 
         ( c >= 'a' && c <= 'z' ) ) {
    return true;
    }
    else if ( underscore && c == '_' ) {
        return true;
    } 
    else {
        return false;
    }
}

