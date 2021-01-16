/* 
 * bitpack.h
 * Zoe Hsieh, Rebecca Miller, 3/7/20
 * hw4 : arith
 * 
 * interface: functions that deal with bitpacking
 */

#ifndef BITPACK_H
#define BITPACK_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "except.h"
#include "assert.h"


/* name         : Bitpack_fitsu
 * purpose      : determine if given unsigned number fits in given width
 * inputs       : uint64_t to check, unsigned width in bits
 * outputs      : true if fits, otherwise false
 * side effects : none
 * expects      : none
 */
bool Bitpack_fitsu(uint64_t n, unsigned width);


/* name         : Bitpack_fitss
 * purpose      : determine if given signed number fits in given width
 * inputs       : int64_t to check, unsigned width in bits
 * outputs      : true if fits, otherwise false
 * side effects : none
 * expects      : none
 */
bool Bitpack_fitss(int64_t n, unsigned width);


/* name         : Bitpack_getu
 * purpose      : get unsigned field beginning with lsb with width bits from 
 *                word
 * inputs       : uint64_t bitpacked word, width in bits and lsb of field to 
 *                retrieve
 * outputs      : uint64_t representation of field retrieved
 * side effects : none
 * expects      : width <= 64 and width + lsb <= 64
 */
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb);


/* name         : Bitpack_gets
 * purpose      : get signed field beginning with lsb with width bits from word
 * inputs       : uint64_t bitpacked word, width in bits and lsb of field to
 *                retrieve
 * outputs      : int64_t representation of field retrieved
 * side effects : none
 * expects      : width <= 64 and width + lsb <= 64
 */
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb);


/* name         : Bitpack_newu
 * purpose      : write in unsigned value to field beginning with lsb with 
 *                width bits to word
 * inputs       : uint64_t bitpacked word, width in bits and lsb of field to 
 *                write in,
 *                unsigned value to fill field with
 * outputs      : uint64_t that is original word but with field modified
 * side effects : none
 * expects      : width <= 64, width + lsb <= 64, value fits in width bits
 */
uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, 
                      uint64_t value);


/* name         : Bitpack_news
 * purpose      : write in signed value to field beginning with lsb with width 
 *                bits to word
 * inputs       : uint64_t bitpacked word, width in bits and lsb of field to 
 *                write in, signed value to fill field with
 * outputs      : uint64_t that is original word but with field modified
 * side effects : none
 * expects      : width <= 64, width + lsb <= 64, value fits in width bits
 */
uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb, 
                      int64_t value);

/* triggered when client creates field with value that doesn't fit */
extern Except_T Bitpack_Overflow;

#endif
