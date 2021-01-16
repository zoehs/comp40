/* 
 * codeword.c
 * Zoe Hsieh, Rebecca Miller, 3/7/20
 * hw4 : arith
 * 
 * packs and unpacks codewords
 */

#include "codeword.h"

struct Extra_Data {
    A2Methods_UArray2 blocks; // 2D array of either codewords or DCT_elems
    A2Methods_T methods;      // methods for 2D arrays
};
typedef struct Extra_Data Extra_Data;

#define LSB_a 26
#define LSB_b 20
#define LSB_c 14
#define LSB_d 8
#define LSB_pb 4
#define LSB_pr 0

#define WID_a 6
#define WID_b 6
#define WID_c 6
#define WID_d 6
#define WID_pb 4
#define WID_pr 4

void packing(int col, int row, A2Methods_UArray2 dct_arr, void *elem, 
                      void *cl);
void unpacking(int col, int row, A2Methods_UArray2 words_arr, void *elem, 
                      void *cl);


/****************************** PUBLIC FUNCTIONS *****************************/


/* purpose : pack DCT_elems into 32 bit codewords
 */
A2Methods_UArray2 pack_codeword(A2Methods_UArray2 dct_arr, A2Methods_T methods)
{
    assert(dct_arr != NULL);
    assert(methods != NULL);
  
    A2Methods_UArray2 words = methods->new(methods->width(dct_arr), 
                                           methods->height(dct_arr), 
                                           sizeof(uint32_t));
    Extra_Data data = {words, methods};
    
    methods->map_default(dct_arr, packing, &data);  
  
    return data.blocks; 
}


/* purpose : unpack 32 bit codewords into DCT_elems 
 */
A2Methods_UArray2 unpack_codeword(A2Methods_T words, A2Methods_T methods)
{
    assert(words != NULL);
    assert(methods != NULL);
  
    unsigned width  = methods->width(words);
    unsigned height = methods->height(words);
  
    DCT_elems temp;
    A2Methods_UArray2 dct_arr = methods->new(width, height, sizeof(*temp));
    Extra_Data data = {dct_arr, methods};
  
    methods->map_default(words, unpacking, &data);  

    return data.blocks; 
}


/**************************** COMPRESSION HELPERS ****************************/

/* name         : packing
 * purpose      : bitpack a single codeword
 * inputs       : ints for col and row, 2d arr of DCT_elems, void *
 *                to current elem, closure
 * outputs      : none
 * side effects : initializes 32 bit codeword in parallel 2d array
 * expects      : col and row in range, non-null elem, non-null closure
 * note         : intended to be apply for 2d array map
 */
void packing(int col, int row, A2Methods_UArray2 dct_arr, void *elem, 
                      void *cl)
{
    assert(cl != NULL);
    assert(elem != NULL);
    assert(dct_arr != NULL);
  
    (void)dct_arr;
    Extra_Data *data = cl;
    assert(data->methods != NULL);
    A2Methods_T methods = data->methods;
  
    assert(0 <= col && col < methods->width(dct_arr));
    assert(0 <= row && row < methods->height(dct_arr));
  
    DCT_elems curr_block = elem;
    
    uint32_t word = Bitpack_newu(0, WID_a, LSB_a, curr_block->a);
    word = Bitpack_news(word, WID_b, LSB_b, curr_block->b);
    word = Bitpack_news(word, WID_c, LSB_c, curr_block->c);
    word = Bitpack_news(word, WID_d, LSB_d, curr_block->d);
    word = Bitpack_newu(word, WID_pb, LSB_pb, curr_block->pb_index);
    word = Bitpack_newu(word, WID_pr, LSB_pr, curr_block->pr_index);
  
    *(uint32_t *)(methods->at(data->blocks, col, row)) = word;
}


/*************************** DECOMPRESSION HELPERS ***************************/


/* name         : unpacking
 * purpose      : unpack a single codeword
 * inputs       : ints for col and row, 2d arr of codewords, void *
 *                to current elem, closure
 * outputs      : none
 * side effects : initializes DCT_elems in parallel 2D array
 * expects      : col and row in range, non-null elem, non-null closure
 * note         : intended to be apply for 2d array map
 */
void unpacking(int col, int row, A2Methods_UArray2 words_arr, void *elem, 
                      void *cl)
{
    assert(cl != NULL);
    assert(elem != NULL);
    assert(words_arr != NULL);  
  
    (void)words_arr;
    Extra_Data *data = cl;
    assert(data->methods != NULL);
    A2Methods_T methods = data->methods;
  
    assert(0 <= col && col < methods->width(words_arr));
    assert(0 <= row && row < methods->height(words_arr));
  
    uint32_t word       = *(uint32_t *)elem;
    DCT_elems block_dct = methods->at(data->blocks, col, row);
  
    block_dct->a = Bitpack_getu(word, WID_a, LSB_a);
    block_dct->b = Bitpack_gets(word, WID_b, LSB_b);
    block_dct->c = Bitpack_gets(word, WID_c, LSB_c); 
    block_dct->d = Bitpack_gets(word, WID_d, LSB_d);
    block_dct->pb_index = Bitpack_getu(word, WID_pb, LSB_pb);
    block_dct->pr_index = Bitpack_getu(word, WID_pr, LSB_pr);
}
