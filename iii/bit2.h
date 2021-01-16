/* 
 * bit2.h
 * Zoe Hsieh, Rebecca Miller, 2/10/20
 * hw2 : iii
 * 
 * interface for 2D bit set
 */

#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include "bit.h"

#ifndef BIT2_INCLUDED
#define BIT2_INCLUDED

#define T Bit2_T
typedef struct T *T;


/* 
 * name    : Bit2_new
 * purpose : to create a new 2D bit set, Bit2_T
 * inputs  : int number of columns, int number of rows
 * outputs : pointer to the 2D bit set
 * expects : nonnegative integers for width and height
 */
extern T Bit2_new(int width, int height);


/* 
 * name    : Bit2_width
 * purpose : returns the width (# of columns) (as an int) of the given array
 * inputs  : pointer to 2D bit set (Bit2_T)
 * outputs : int value of width
 * expects : non-null Bit2_T
 */
extern int Bit2_width(T set);


/* 
 * name    : Bit2_height
 * purpose : returns the height (# of rows) (as an int) of the given array
 * inputs  : pointer to 2D bit set (Bit2_T)
 * outputs : int value of height
 * expects : non-null Bit2_T
 */
extern int Bit2_height(T set);


/* 
 * name    : Bit2_get
 * purpose : checks if (row,col) exists in set and if it's 0 or 1
 * inputs  : pointer to 2D bit set (Bit2_T), 
 * outputs : int value of elem if in set, otherwise 0
 * expects : non-null Bit2_T, 0 <= col < width, 0 <= row < height
 */
extern int Bit2_get(T set, int col, int row);

/* 
 * name    : Bit2_put
 * purpose : put new bit value into chosen position in bit set
 * inputs  : pointer to 2D bit set, int vals for col and row, val of 
 *           desired bit
 * outputs : previous value of elem being overwritten
 * expects : non-null Bit2_T, 0 <= col < width, 0 <= row < height, bit == 0 
 *           or bit == 1
 */
extern int Bit2_put(T set, int col, int row, int bit);


/* 
 * name    : Bit2_map_row_major
 * purpose : prints out every index (row,col), iterating through with col 
 *           changing faster than row
 * inputs  : Bit2_T set, apply(int col, int row, T set, int val, void *cl), 
 *           void *cl
 * outputs : none (cl altered by reference)
 * expects : non-null UArray2_T, valid function with matching parameters
 */
extern void Bit2_map_row_major(T set, void apply(int row, int col, 
                               T set, int val, void *cl), void *cl);

/* 
 * name    : Bit2_map_col_major
 * purpose : prints out every index (row,col), iterating through with row
 *           changing faster than col
 * inputs  : Bit2_T set, apply(int col, int row, T set, int val, void *cl), 
 *           void *cl
 * outputs : none (cl altered by reference)
 * expects : non-null UArray2_T, valid function with matching parameters
 */
extern void Bit2_map_col_major(T set, void apply(int row, int col, 
                               T set, int val, void *cl), void *cl);


/* 
 * name    : Bit2_free
 * purpose : frees the bit set
 * inputs  : T *set -- pointer to the (pointer to the) bit set
 * outputs : none
 * expects : non-null and not already freed Bit2_T
 */
extern void Bit2_free(T *set);


#undef T
#endif
