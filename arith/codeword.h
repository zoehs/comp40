/* 
 * codeword.h
 * Zoe Hsieh, Rebecca Miller, 3/7/20
 * hw4 : arith
 * 
 * interface: functions that pack and unpack the codewords
 */

#ifndef CODEWORD_H
#define CODEWORD_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "except.h"
#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "data_types.h"
#include "bitpack.h"


/* name         : pack_codeword
 * purpose      : bitpack codewords into codeword 2d array from DCT values
 * inputs       : 2d array of DCT values, methods for 2d arrays
 * outputs      : 2d array of 32 bit codewords
 * side effects : mallocs 2d array of codewords which must be freed
 * expects      : non-null dct_arr, non-null methods
 */
A2Methods_UArray2 pack_codeword(A2Methods_UArray2 dct_arr, 
                                A2Methods_T methods);


/* name         : unpack_codeword
 * purpose      : undoes bitpacking from codeword 2d array into 2d array of DCT
 * inputs       : 2d array of 32 bit codewords, methods for 2d arrays
 * outputs      : 2d array of DCT_elems
 * side effects : mallocs 2d array of DCT_elems which must be freed
 * expects      : non-null dct_arr, non-null methods
 */
A2Methods_UArray2 unpack_codeword(A2Methods_T words, A2Methods_T methods);

#endif
