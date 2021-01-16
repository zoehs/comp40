/* 
 * bitpack.c
 * Zoe Hsieh, Rebecca Miller, 3/7/20
 * hw4 : arith
 * 
 * 6 bitpacking functions for unsigned and signed 
 * integers: fit, get, and new
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "except.h"
#include "assert.h"

#include "bitpack.h"

#define ALL_ONES 0xffffffffffffffff
#define ALL_BUT_FIRST_ONE 0x7fffffffffffffff


/**************************** WIDTH TEST FUNCTIONS ***************************/


/* purpose : determine if given unsigned number fits in given width
 */
bool Bitpack_fitsu(uint64_t n, unsigned width)
{
    if ( width == 0 ) {
          if ( n == 0 ) {
              return true;
          }
          else {
              return false;
          }
    }
    else if ( width > 64 ) {
        return true;
    }

    uint64_t max_range = ALL_ONES >> (64 - width);

    if ( n > max_range ) {
          return false;
    }
    else {
          return true;
    }
}


/* purpose : determine if given signed number fits in given width
 */
bool Bitpack_fitss(int64_t n, unsigned width)
{ 
    if ( width == 0 ) {
          if ( n == 0 ) {
              return true;
          }
          else {
              return false;
          }
    }
    else if ( width > 64 ) {
        return true;
    }

    int64_t max_range = ALL_BUT_FIRST_ONE >> (64 - width);
    int64_t min_range = ALL_ONES << (width - 1);

    if ( n > max_range || n < min_range ) {
          return false;
    }
    else {
          return true;
    }
}


/************************* FIELD EXTRACTION FUNCTIONS ************************/


/* purpose : get unsigned field beginning with lsb with width bits from word
 */
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
    assert(width <= 64);
    assert(width + lsb <= 64);
  
    if ( width == 0 ) {
        return 0;
    }
    
    uint64_t mask = ALL_ONES >> (64 - width) << lsb;
    return ( word & mask ) >> lsb; 
}


/* purpose : get signed field beginning with lsb with width bits from word
 */
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
    assert(width <= 64);
    assert(width + lsb <= 64);
  
    if ( width == 0 ) {
        return 0;
    }
    else if ( width == 64 ) {
        return word;
    }
  
    int64_t mask = ALL_BUT_FIRST_ONE >> (64 - width - 1) << lsb;
    return (( (int64_t)word & mask ) << (64 - width - lsb)) >> (64 - width);
}


/*************************** FIELD UPDATE FUNCTIONS **************************/


/* purpose : write in unsigned value to field beginning with lsb with width 
 *           bits to word
 */
uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, 
                      uint64_t value)
{
    assert(width <= 64);
    assert(width + lsb <= 64);
  
    if ( ! Bitpack_fitsu(value, width) ) {
        RAISE(Bitpack_Overflow);
    }
  
    uint64_t temp_value = value << lsb;
    uint64_t mask = ~(ALL_ONES >> (64 - width) << lsb);
    return (word & mask) | temp_value;
}


/* purpose : write in signed value to field beginning with lsb with width bits
 *           to word
 */
uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb, 
                      int64_t value)
{ 
    assert(width <= 64);
    assert(width + lsb <= 64);
  
    if ( ! Bitpack_fitss(value, width) ) {
        RAISE(Bitpack_Overflow);
    }
  
    int64_t temp_value = value;
  
    if ( width != 64 ) {
        temp_value = (ALL_BUT_FIRST_ONE >> (64 - width - 1) << lsb) 
                      & (value << lsb);
    }
  
    
    uint64_t mask = ~(ALL_ONES >> (64 - width) << lsb);
    return (word & mask) | temp_value; 
}


/* triggered when client creates field with value that doesn't fit */
Except_T Bitpack_Overflow = { "Overflow packing bits" };
