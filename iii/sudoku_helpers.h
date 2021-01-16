/* 
 * sudoku_helpers.h
 * Zoe Hsieh, Rebecca Miller, 2/10/20
 * hw2 : iii
 * 
 * declarations for helper functions to check if sudoku is solved
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "assert.h"
#include "uarray2.h"

#define GRID_SIZE 9

struct unique_set {
    size_t *arr;
    int index;
};
typedef struct unique_set *unique_set;


/* 
 * name    : row_check
 * purpose : checks each row if each pixel value in the row is unique 
 * inputs  : UArray2_T sudo
 * outputs : none
 * expects : populated, non-null 9x9 UArray2_T
 */
void row_check(UArray2_T sudo);

/* 
 * name    : col_check
 * purpose : checks each column to see if each pixel value is unique or not
 * inputs  : UArray2_T sudo
 * outputs : none
 * expects : populated, non-null 9x9 UArray2_T
 */
void col_check(UArray2_T sudo);

/* 
 * name    : submaps_check
 * purpose : checks each submap (3x3 grid) if all pixel values are unique
 * inputs  : UArray2_T sudo
 * outputs : none
 * expects : populated, non-null 9x9 UArray2_T
 */
void submaps_check(UArray2_T sudo);

/* 
 * name    : check_unique
 * purpose : checks within an array of 9 pixels to see if all pixels are 
 *           unique
 * inputs  : int col, int row, UArray2_T sudo, void *elem, void *cl
 * outputs : none
 * expects : struct including arr and curr index for *cl
 */
void check_unique(int col, int row, UArray2_T sudo, void *elem, void *cl);

/* 
 * name    : init_arr
 * purpose : initializes a new array for unique_set arrays
 * inputs  : size_t *arr, int size
 * outputs : none
 * expects : size <= length of arr (this is the job of the user and 
 *           cannot be confirmed here)
 */
void init_arr(size_t *arr, int size);

/* 
 * name    : existing_vals_unique
 * purpose : checks if the pixel value passed in already exists in the 
 *           unique set array or not
 * inputs  : size_t elem, size_t *arr, int *last_used_index
 * outputs : bool - true if it is unique, false - if the pixel value 
 *           already exists
 * expects : non-null arr with values at least until last_used_arr 
 *           (this part is the job of the client),
 *           0 <= *last_used_index < size of arr
 */
bool existing_vals_unique(size_t elem, size_t *arr, int *last_used_index);
