/* 
 * uarray2.c
 * Zoe Hsieh, Rebecca Miller, 2/5/20
 * hw2 : iii
 * 
 * implementation for 2D unboxed array
 */

#include "uarray2.h"

#define T UArray2_T

struct T {
    int height, width;
    int size;
    UArray_T data;
};

int UArray2_1D_index(T arr, int colcol, int rowrow);
  

/* 
 * purpose : to create a new 2D unboxed array, UArray2_T
 * note    : dynamically allocates memory which user must free later
 */
T UArray2_new(int width, int height, int elem_size)
{
    assert(height >= 0 && width >= 0);
    assert(elem_size > 0);

    T new_arr = malloc(height * width * elem_size);
    assert(new_arr);
    
    new_arr->height  = height;
    new_arr->width   = width;
    new_arr->size    = elem_size;
    
    new_arr->data = UArray_new(height * width, elem_size);
    
    return new_arr;
}


/* 
 * purpose : returns the width (# of columns) (as an int) of the given array
 */
int UArray2_width(T arr)
{
    assert(arr);
    return arr->width;
}


/* 
 * purpose : returns the height (# of rows) (as an int) of the given array
 */
int UArray2_height(T arr)
{
    assert(arr);
    return arr->height;
}


/* 
 * purpose : returns size in bytes of each element
 */
int UArray2_size(T arr)
{
    assert(arr);    
    return arr->size;
}


/* 
 * purpose : return pointer to element at given location in UArray2_T
 * note    : uses helper function to associate 2D indices with 1D index
 *           under the hood
 */
void *UArray2_at(T arr, int col, int row)
{  
    assert(arr);
    assert(0 <= row && row < arr->height && 0 <= col && col < arr->width);
  
    void *element;
    int pos = UArray2_1D_index(arr, col, row);
    element = UArray_at(arr->data, pos);
  
    return element;
}

  
/* 
 * purpose : prints out every index (row,col), iterating through with col 
 *           changing faster than row
 */

void UArray2_map_row_major(T arr, void apply(int col, int row, T arr, 
                                  void *elem, void *cl), void *cl)
{
    assert(arr);
    assert(apply);
  
    int x, y;
  
    for ( y = 0; y < arr->height; y++ ) { 
        for ( x = 0; x < arr->width; x++ ) {
            apply(x, y, arr, UArray2_at(arr, x, y), cl);
        }
    }
}


/* 
 * purpose : prints out every index (row,col), iterating through with row
 *           changing faster than col
 */

void UArray2_map_col_major(T arr, void apply(int col, int row, T arr, 
                                  void *elem, void *cl), void *cl)
{
    assert(arr);
    assert(apply);
  
    int x, y; 
  
    for ( x = 0; x < arr->width; x++ ) { 
        for ( y = 0; y < arr->height; y++ ) {
            apply(x, y, arr, UArray2_at(arr, x, y), cl);
        }
    }   
}


/* 
 * purpose : frees memory that was allocated for unboxed 2D array
 */
void UArray2_free(T *arr)
{
  assert(arr && *arr);
  
  UArray_free(&((*arr)->data));
  free(*arr);
}


/****************************** HELPER FUNCTIONS *****************************/

/* 
 * name    : UArray2_1D_index
 * purpose : maps row and col indices of virtual 2D arr to one index on 1D arr
 * inputs  : pointer to 2D unboxed array, int row and col indices
 * outputs : int index of desired element in 1D arr
 * expects : non-null UArray2_T, 0 <= row < height, 0 <= col < width
 */
int UArray2_1D_index(T arr, int col, int row)
{
    assert(arr);
    assert(0 <= row && row < arr->height && 0 <= col && col < arr->width);
  
    int index = (row * arr->width) + col;
    return index;
}
