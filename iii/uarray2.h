/* 
 * uarray2.h
 * Zoe Hsieh, Rebecca Miller, 2/10/20
 * hw2 : iii
 * 
 * interface for 2D unboxed array
 */

#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include "uarray.h"

#ifndef UARRAY2_INCLUDED
#define UARRAY2_INCLUDED

#define T UArray2_T
typedef struct T *T;

/* 
 * name    : UArray2_new
 * purpose : to create a new 2D unboxed array, UArray2_T
 * inputs  : int number of columns, int number of rows, int size in bytes of 
 *           each element
 * outputs : pointer to the 2D unboxed array
 * expects : nonnegative integers for width and height, positive int for 
 *           elem_size
 */
extern T UArray2_new(int width, int height, int elem_size);


/* 
 * name    : UArray2_width
 * purpose : returns the width (# of columns) (as an int) of the given array
 * inputs  : pointer to 2D unboxed array (UArray2_T)
 * outputs : int value of width
 * expects : non-null UArray2_T
 */
extern int UArray2_width(T arr);


/* 
 * name    : UArray2_height
 * purpose : returns the height (# of rows) (as an int) of the given array
 * inputs  : pointer to 2D unboxed array (UArray2_T)
 * outputs : int value of height
 * expects : non-null UArray2_T
 */
extern int UArray2_height(T arr);


/* 
 * name    : UArray2_size
 * purpose : returns size in bytes of each element
 * inputs  : pointer to 2D unboxed array (UArray2_T)
 * outputs : int value of the size of every element 
 *           (should be the same size for all)
 * expects : non-null UArray2_T
 */
extern int UArray2_size(T arr);


/* 
 * name    : UArray2_at
 * purpose : return pointer to element at given location in UArray2_T
 * inputs  : pointer to UArray2_T, desired ints for row and col
 * outputs : void * to element stored there
 * expects : non-null UArray2_T, 0 <= row < height, 0 <= col < width
 */
extern void *UArray2_at(T arr, int col, int row);
 

/* 
 * name    : UArray2_map_row_major
 * purpose : prints out every index (row,col), iterating through with col
 *           changing faster than row
 * inputs  : pointer to 2D unboxed array, apply function (args: int row, int 
 *           col, void *elem, void *cl), and void *cl 
 * outputs : none (cl altered by reference)
 * expects : non-null UArray2_T, valid function with matching parameters
 */
extern void UArray2_map_row_major(T arr, void apply(int col, int row, T arr, 
                                  void *elem, void *cl), void *cl);

/* 
 * name    : UArray2_map_col_major
 * purpose : prints out every index (row,col), iterating through with row
 *           changing faster than col
 * inputs  : pointer to 2D unboxed array, apply function (args: int row, int 
 *           col, void *elem, void *cl), and void *cl
 * outputs : none (cl altered by reference)
 * expects : non-null UArray2_T, valid function with matching parameters
 */
extern void UArray2_map_col_major(T arr, void apply(int col, int row, T arr, 
                                  void *elem, void *cl), void *cl);

/* 
 * name    : UArray2_free
 * purpose : frees memory that was allocated for unboxed 2D array
 * inputs  : address of the 2D unboxed array (UArray2_T)
 * outputs : none
 * expects : non-null and not already freed UArray2_T
 */
extern void UArray2_free(T *arr);

#undef T

#endif
