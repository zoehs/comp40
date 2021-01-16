/* 
 * RGB_ints_floats.h
 * Zoe Hsieh, Rebecca Miller, 3/7/20
 * hw4 : arith
 * 
 * interface -- functions that deal with converting
 * RGB ints to RGB floats and vice versa
 */

#ifndef RGB_INTS_FLOATS_H
#define RGB_INTS_FLOATS_H

#include <stdio.h>
#include <stdlib.h>
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "Pnm.h"
#include "data_types.h"
#include "assert.h"

/* name         : RGB_ints_to_floats
 * purpose      : convert a 2D array of Pnm_rgb structs holding
 *                RGB ints to a 2D array of Rgb_f structs holding
 *                RGB floats
 * inputs       : 2D array of Pnm_rgb structs, methods to use,
 *                denominator of image
 * outputs      : 2D array of populated Rgb_f structs
 * side effects : mallocs A2Methods_UArray 2 that's returned;
 *                must be freed
 * expects      : non-null and populated 2D array, methods 
 *                interface for 2D arrays, denominator > 0
 */
A2Methods_UArray2 RGB_ints_to_floats(A2Methods_UArray2 pixels, 
                                     A2Methods_T methods, 
                                     unsigned denominator,
                                     unsigned width, 
                                     unsigned height);


/* name         : RGB_floats_to_ints
 * purpose      : convert a 2D array of Rgb_f structs holding RGB
 *                floats to a 2D array of Pnm_rgb structs holding
 *                RGB ints
 * inputs       : 2D array of Rgb_f structs, methods to use, 
 *                denominator of image
 * outputs      : 2D array of populated Pnm_rgb structs
 * side effects : mallocs A2Methods_UArray 2 that's returned;
 *                must be freed
 * expects      : non-null and populated 2D array, methods 
 *                interface for 2D arrays, denominator > 0
 */
A2Methods_UArray2 RGB_floats_to_ints(A2Methods_UArray2 pixels, 
                                     A2Methods_T methods, 
                                     unsigned denominator);

#endif