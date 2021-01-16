/* 
 * VCS_DCT.h
 * Zoe Hsieh, Rebecca Miller, 3/7/20
 * hw4 : arith
 * 
 * interface -- functions that convert VCS elements to 
 * DCT elements and vice versa
 */

#ifndef VCS_DCT_H
#define VCS_DCT_H

#include <stdio.h>
#include <stdlib.h>
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "data_types.h"
#include "arith40.h"
#include "assert.h"

/* name         : VCS_to_DCT
 * purpose      : creates a UArray2 of DCT_elems structs to hold 
 *                the necessary values (VCS elements
 *                converted to DCT elements) for each pixel.
 * inputs       : A2Methods_UArray2 VCS_elems, A2Methods_T methods
 * outputs      : A2Methods_UArray2 with new size (width/2 and height/2) 
 *                from the passed in UArray2 since each 2x2 block is in 
 *                one pixel
 * side effects : mallocs 2d array of DCT_elems which must be freed
 * expects      : non-null and populated 2D array, methods 
 *                interface for 2D arrays
 */
A2Methods_UArray2 VCS_to_DCT(A2Methods_UArray2 VCS_elems, A2Methods_T methods);


/* name         : DCT_to_VCS
 * purpose      : creates a UArray2 of VCS_elems structs to hold the 
 *                necessary values (DCT elements converted to VCS 
 *                elements) for each pixel.
 * inputs       : A2Methods_UArray2 DCT_values, A2Methods_T methods
 * outputs      : A2Methods_UArray2 with new size (width*2 and height*2) 
 *                from the passed in UArray2 since each pixel creates 
 *                values for a 2x2 block of 
 *                4 pixels each
 * side effects : mallocs 2d array of VCS_elems which must be freed
 * expects      : non-null and populated 2D array, methods 
 *                interface for 2D arrays
 */
A2Methods_UArray2 DCT_to_VCS(A2Methods_UArray2 DCT_values, 
                             A2Methods_T methods);

#endif
