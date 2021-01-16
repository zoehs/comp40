/* 
 * bit2.c
 * Zoe Hsieh, Rebecca Miller, 2/5/20
 * hw2 : iii
 * 
 * implementation for 2D bit set
 */

#include "bit2.h"

#define T Bit2_T

struct T {
    int width, height;
    Bit_T data;
};

int Bit2_1D_index(T set, int col, int row);

/* 
 * purpose : to create a new 2D bit set, Bit2_T
 * note    : dynamically allocates memory which user must free later
 */
T Bit2_new(int width, int height)
{
    assert(height >= 0 && width >= 0);

    T new_bit = malloc(height * width * sizeof(int));
    assert(new_bit);    
    
    new_bit->height  = height;
    new_bit->width   = width;
    
    new_bit->data = Bit_new(height * width);
    
    return new_bit;
}


/* 
 * purpose : returns the width (# of columns) (as an int) of the given array
 */
int Bit2_width(T set)
{
    assert(set);
    return set->width;
}


/* 
 * purpose : returns the height (# of rows) (as an int) of the given array
 */
int Bit2_height(T set)
{
    assert(set);
    return set->height;
}


/* 
 * purpose : checks if (row,col) exists in set and if it's 0 or 1
 * 
 */
int Bit2_get(T set, int col, int row)
{
    assert(set);
    assert(0 <= row && row < set->height && 0 <= col && col < set->width);
  
    int pos = Bit2_1D_index(set, col, row);
    return Bit_get(set->data, pos);
}


/* 
 * purpose : put new bit value into chosen position in bit set
 * note    : maps 2D indices to 1D index under the hood
 */
int Bit2_put(T set, int col, int row, int bit)
{
    assert(set);
    assert(0 <= row && row < set->height && 0 <= col && col < set->width);
    assert(bit == 0 || bit == 1);

    int pos = Bit2_1D_index(set, col, row);
    
    return Bit_put(set->data, pos, bit);
}


/* 
 * purpose : prints out every index (row,col), iterating through with col 
 *       changing faster than row
 */
void Bit2_map_row_major(T set, void apply(int col, int row, 
                        T set, int val, void *cl), void *cl)
{
    assert(set);
    assert(apply);
  
    int x, y;
  
    for ( y = 0; y < set->height; y++ ) { 
        for ( x = 0; x < set->width; x++ ) {
            apply(x, y, set, Bit2_get(set, x, y), cl);
        }
    }
}


/* 
 * purpose : prints out every index (row,col), iterating through with row
 *           changing faster than col
 */
void Bit2_map_col_major(T set, void apply(int col, int row, 
                               T set, int val, void *cl), void *cl)
{ 
    assert(set);
    assert(apply);
  
    int x, y; 
  
    for ( x = 0; x < set->width; x++ ) { 
        for ( y = 0; y < set->height; y++ ) {
            apply(x, y, set, Bit2_get(set, x, y), cl);
        }
    }
}


/* 
 * purpose : frees the bit set
 */
void Bit2_free(T *set)
{
    assert(set && *set);
  
    Bit_free(&((*set)->data));
    free(*set);
}


/****************************** HELPER FUNCTIONS *****************************/

/* 
 * name    : UArray2_1D_index
 * purpose : maps row and col indices of virtual 2D arr to one index on 1D arr
 * inputs  : pointer to 2D unboxed array, int row and col indices
 * outputs : int index of desired element in 1D arr
 * expects : non-null UArray2_T, 0 <= row < height, 0 <= col < width
 */
int Bit2_1D_index(T set, int col, int row)
{
    assert(set);
    assert(0 <= row && row < set->height && 0 <= col && col < set->width);
  
    int index = (row * set->width) + col;
    return index;
}
