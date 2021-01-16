/* 
 * simlines.c
 * Zoe Hsieh, Rebecca Miller, 1/31/20
 * hw1 : filesnpix
 * 
 * finds similar lines in files given, and prints the 
 * filename and line # of where it occurs
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "readaline.h"
#include "atom.h"
#include "table.h"
#include "seq.h"
#include "except.h"
#include "string_funcs.h"
#include "utility.h"

struct Match {
    char *filename;
    size_t line_num;
};
typedef struct Match *Match;

void output_format(const void *key, void **value, void *cl);
void add_key_pair(Table_T matches, const void *key, Match one_val);
void free_values(const void *key, void **value, void *cl);
void make_match(Match temp, char *name, size_t num); 

/************************************ MAIN ***********************************/

int main(int argc, char *argv[])
{ 
    Table_T matches = Table_new(100, NULL, NULL); 
    int i;

    for ( i = 1; i < argc; i++ ) {
        FILE *curr_file = fopen(argv[i], "r");
        
        if ( curr_file == NULL ) {
            
            exit(EXIT_FAILURE);
        }
        
        size_t j        = 1;
        char *curr_line;
        size_t bytes = readaline(curr_file, &curr_line);

        while ( bytes != 0 ) {

            char *better_line = to_simple_string(curr_line, bytes); 
            const char *key   = Atom_string(better_line);  

            void *temp = malloc(sizeof(argv[i]) + sizeof(j)); 
            check_null_ptr(temp);
            Match sim = temp;
            make_match(sim, argv[i], j);

            add_key_pair(matches, key, sim);
            j++;

            free_string(&curr_line);
            free_string(&better_line);

            bytes = readaline(curr_file, &curr_line);
        }

        free_string(&curr_line);
        fclose(curr_file);
    }

    int match_num = 0;
    Table_map(matches, output_format, &match_num);
    Table_map(matches, free_values, NULL);

    Table_free(&matches);

    return EXIT_SUCCESS;
}

/****************************** HELPER FUNCTIONS *****************************/

/*
 * name : add_key_pair
 * purp : adds new element to sequence value
 * args : Table_T matches, const void *key, Match one_val
 * rets : none
 */

void add_key_pair(Table_T matches, const void *key, Match one_val) 
{
    Seq_T temp = Table_get(matches, key);

    if ( temp == NULL ) {

        Seq_T values = Seq_new(50);
        Seq_addhi(values, one_val);
        Table_put(matches, key, values);
    }
    else {

        Seq_addhi(temp, one_val);
        Table_put(matches, key, temp); 
        temp = NULL;
    }
}

/*
 * name : output_format
 * purp : outputs the similar lines in the format specified
 * args : const void *key, void **value, void *cl
 * rets : none
 */

void output_format(const void *key, void **value, void *cl) 
{

    int len = Seq_length(*value);
    int *line_num = cl;
    const char *line_name = key;
    Seq_T match_block = *value; 

    if ( len > 1 && is_alphanum(line_name[0], true) ) {
        if ( *line_num > 0 ) {
            printf("\n");  
        }
        printf("%s\n", line_name); 
        int i;
    
        for ( i = 0; i < len; i++ ) {

            Match curr_match = Seq_get(match_block, i);
            printf("%-20s %7zu\n", curr_match->filename, curr_match->line_num);

        }
        (*line_num)++;
        cl = line_num;
    }
}

/*
 * name : free_values
 * purp : free values for memory that was a
 * args : const void *key, void **value, void *cl
 * rets : none
 */

void free_values(const void *key, void **value, void *cl) 
{
    (void)key;
    (void)cl;
  
    int len = Seq_length(*value);
    Seq_T match_block = *value; 

    int i;

    for ( i = 0; i < len; i++ ) {
        free(Seq_get(match_block, i));
    }
  
  Seq_free(&match_block);
}

/*
 * name : make_match
 * purp : assigns values to the struct of a new match
 * args : Match temp, char *name (filename), size_t num (line number)
 * rets : none
 */

void make_match(Match temp, char *name, size_t num) 
{
    temp->filename = name;
    temp->line_num = num;
}
  