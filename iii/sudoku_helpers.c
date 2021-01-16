/* 
 * sudoku_helpers.c
 * Zoe Hsieh, Rebecca Miller, 2/10/20
 * hw2 : iii
 * 
 * helper functions to check if sudoku is solved
 */

#include "sudoku_helpers.h"

/* 
 * purpose : checks within an array of 9 pixels to see if all pixels are unique
 */
void check_unique(int col, int row, UArray2_T sudo, void *elem, void *pix_tracker)
{
    assert(pix_tracker);
    assert(elem);
  
    (void)sudo; 
    (void)col;
    (void)row;
  
    unique_set pix_set = pix_tracker;
    size_t *num        = elem;
    
    if ( pix_set->index == GRID_SIZE ) {
        init_arr(pix_set->arr, GRID_SIZE);
        pix_set->index = 0;
    }
    if ( existing_vals_unique(*num, pix_set->arr, &(pix_set->index)) ) {
        pix_set->arr[pix_set->index] = *num;    
        pix_set->index++;
    }
    else { 
        exit(1);
    }
}


/* 
 * purpose : initializes a new array for unique_set arrays
 * expects : size <= length of arr (this is the job of the user and cannot be confirmed here)
 */
void init_arr(size_t *arr, int size)
{
    int i;
    for ( i = 0; i < size; i++ ) {
        arr[i] = 0;
    }
}


/* 
 * purpose : checks if the pixel value passed in already exists in the unique set array or not
 * expects : non-null arr with values at least until last_used_arr (this part is the job of the client), 
 *       0 <= *last_used_index < size of arr
 */
bool existing_vals_unique(size_t elem, size_t *arr, int *last_used_index)
{  
    assert(arr);
    assert(0 <= *last_used_index);
  
    int i;
    for ( i = 0; i < *last_used_index; i++ ) {
        if ( arr[i] == elem) {
            return false;
        }
    }
  
    return true;
}


/* 
 * purpose : checks each row if each pixel value in the row is unique 
 */
void row_check(UArray2_T sudo) 
{
    assert(sudo);
  
    unique_set pix_row = malloc(sizeof(size_t) * GRID_SIZE);
    pix_row->arr       = malloc(sizeof(size_t) * GRID_SIZE);
    assert(pix_row);
    assert(pix_row->arr);
  
    init_arr(pix_row->arr, GRID_SIZE);  
    pix_row->index = 0;
    
    UArray2_map_row_major(sudo, check_unique, pix_row);
    
    free(pix_row->arr);
    free(pix_row);
}


/* 
 * purpose : checks each column to see if each pixel value is unique or not
 */
void col_check(UArray2_T sudo) 
{ 
    assert(sudo);
  
    unique_set pix_col = malloc(sizeof(size_t) * GRID_SIZE);
    pix_col->arr       = malloc(sizeof(size_t) * GRID_SIZE);
    assert(pix_col);
    assert(pix_col->arr);
    
    init_arr(pix_col->arr, GRID_SIZE);  
    pix_col->index = 0;
    
    UArray2_map_col_major(sudo, check_unique, pix_col);
    
    free(pix_col->arr);
    free(pix_col);
}


/* 
 * purpose : checks each submap (3x3 grid) if all pixel values are unique
 */
void submaps_check(UArray2_T sudo) 
{     
    assert(sudo);
  
    int x, y, x2, y2;
    unique_set pix_box;
  
    for ( y = 0; y < GRID_SIZE; y+=3 ) {
        for ( x = 0; x < GRID_SIZE; x += 3 ) {
            pix_box        = malloc(sizeof(size_t) * GRID_SIZE);
            pix_box->arr   = malloc(sizeof(size_t) * GRID_SIZE);
            assert(pix_box);
            assert(pix_box->arr);
        
            init_arr(pix_box->arr, GRID_SIZE);
            pix_box->index = 0;
        
            for ( y2 = 0; y2 < 3; y2++ ) {
                for ( x2 = 0; x2 < 3; x2++ ) {
                    size_t elem = *(size_t *)UArray2_at(sudo, x + x2, y + y2); 
                    pix_box->arr[pix_box->index] = elem;
                    
                    check_unique(x + x2, y + y2, sudo, &(pix_box->arr[pix_box->index]), pix_box);
                }
            }
            free(pix_box->arr);
            free(pix_box);
        }
    }
}