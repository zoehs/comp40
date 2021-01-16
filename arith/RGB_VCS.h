/* 
 * RGB_VCS.h
 * Zoe Hsieh, Rebecca Miller, 3/7/20
 * hw4 : arith
 * 
 * interface - functions for converting RGB to VCS and vice
 * versa
 */

#ifndef RGB_VCS_H
#define RGB_VCS_H

#include <stdio.h>
#include <stdlib.h>
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "data_types.h"
#include "assert.h"

/* name         : RGB_to_VCS
 * purpose      : creates a UArray2 of VCS_elems structs to hold 
 *                the necessary values (rgb floats
 *                converted to VCS elements) for each pixel
 * inputs       : A2Methods_UArray2 rgb_floats, A2Methods_T methods
 * outputs      : A2Methods_UArray2
 * side effects : mallocs 2D arrray of VCS_elems which must be freed
 * expects      : non-null and populated 2D array, methods 
 *                interface for 2D arrays
 */
A2Methods_UArray2 RGB_to_VCS(A2Methods_UArray2 rgb_floats, 
                             A2Methods_T methods);

/* name         : VCS_to_RGB
 * purpose      : creates a UArray2 of RGB float structs to hold 
 *                necessary vales (floats) for each pixel
 * inputs       : A2Methods_UArray2 vcs_vals, A2Methods_T methods
 * outputs      : A2Methods_Array2
 * side effects : mallocs 2D array of Rgb_f which must be freed
 * expects      : non-null and populated 2D array, methods 
 *                interface for 2D arrays
 */
A2Methods_UArray2 VCS_to_RGB(A2Methods_UArray2 vcs_vals, A2Methods_T methods);

#endif
