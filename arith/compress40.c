/* 
 * compress40.c
 * Zoe Hsieh, Rebecca Miller, 3/7/20
 * hw4 : arith
 * 
 * main driver to compress or decompress given image
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "Pnm.h"
#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "RGB_ints_floats.h"
#include "RGB_VCS.h"
#include "VCS_DCT.h"
#include "data_types.h"
#include "codeword.h"
#include "bitpack.h"
#include "compress40.h"

struct Extra_Data {
    A2Methods_T methods;
    FILE * input;
};
typedef struct Extra_Data Extra_Data;

void print_codeword(int col, int row, A2Methods_UArray2 codewords, void *elem,
                    void *cl);
void read_codeword(int col, int row, A2Methods_UArray2 codewords, void *elem, 
                   void *cl);

/****************************** PUBLIC FUNCTIONS *****************************/


/* purpose : go through all steps necessary to compress and print image
 */
void compress40(FILE *input)
{
    assert( input != NULL );
    A2Methods_T methods = uarray2_methods_plain;
    
    Pnm_ppm bigImage = Pnm_ppmread(input, methods);
    assert(bigImage != NULL);
    
    if ( bigImage->width % 2 != 0 ) {
        (bigImage->width)--;
    }
    if ( bigImage->height % 2 != 0 ) {
        (bigImage->height)--;
    }

    A2Methods_UArray2 rgb_floats = RGB_ints_to_floats(bigImage->pixels, 
                                                      methods, 
                                                      bigImage->denominator,
                                                      bigImage->width, 
                                                      bigImage->height);
    A2Methods_UArray2 vcs_vars   = RGB_to_VCS(rgb_floats, methods);
    A2Methods_UArray2 dct_vars   = VCS_to_DCT(vcs_vars, methods);
    A2Methods_UArray2 codewords  = pack_codeword(dct_vars, methods);
  
    /*code word printing */
    printf("COMP40 Compressed image format 2\n%u %u", bigImage->width, 
           bigImage->height);
    printf("\n");
    methods->map_default(codewords, print_codeword, NULL);
    
    methods->free(&rgb_floats);
    methods->free(&vcs_vars);
    methods->free(&dct_vars);
    methods->free(&codewords);
  
    Pnm_ppmfree(&bigImage);
}


/* purpose : go through all steps necessary to decompress and print image
 */
void decompress40(FILE *input)
{
    assert( input != NULL );
    A2Methods_T methods = uarray2_methods_plain;
    
    unsigned height, width;
    int read = fscanf(input, "COMP40 Compressed image format 2\n%u %u", &width,
                      &height);
    assert(read == 2);
    int c = getc(input);
    assert(c == '\n');
  
    unsigned denominator = 255;
    
    A2Methods_UArray2 words = methods->new(width / 2, height / 2, 
                                           sizeof(uint32_t));
  
    Extra_Data data = {methods, input}; 
      
    methods->map_default(words, read_codeword, &data);
    
    A2Methods_UArray2 dct_vars   = unpack_codeword(words, methods);
    A2Methods_UArray2 vcs_vars   = DCT_to_VCS(dct_vars, methods);
    A2Methods_UArray2 rgb_floats = VCS_to_RGB(vcs_vars, methods);
    A2Methods_UArray2 array      = RGB_floats_to_ints(rgb_floats, methods, 
                                                      denominator);
  
    struct Pnm_ppm pixmap = { .width = width, .height = height, 
                              .denominator = denominator, .pixels = array, 
                              .methods = methods
                            };
                                    
    Pnm_ppmwrite(stdout, &pixmap);
  
    methods->free(&array);
    methods->free(&dct_vars);
    methods->free(&rgb_floats);
    methods->free(&vcs_vars);
}


/*************************** OUTPUT / INPUT HELPERS **************************/


/* name         : print_codeword
 * purpose      : print singular codeword in big-endian order
 * inputs       : int col and row of codeword in image, 2d array of codewords,
 *                void * to codeword at this spot, closure
 * outputs      : none
 * side effects : codeword has been printed to stdout
 * expects      : non-null element
 * note         : intended to be apply function for map
 */
void print_codeword(int col, int row, A2Methods_UArray2 codewords, void *elem, 
                    void *cl)
{
    (void)col;
    (void)row;
    (void)codewords;
    (void)cl;
    
    assert(elem != NULL);

    int byte;
    for ( byte = 4; byte > 0; byte-- ) {
        char b = Bitpack_getu(*(uint64_t *)elem, 8, ( 8 * byte ) - 8);
        putchar(b);
    }
}


/* name         : read_codeword
 * purpose      : read single codeword from stdin into 2D array
 * inputs       : int col and row of codeword in image, 2D array of codewords, 
 *                void * to elem at this position, closure
 * outputs      : none
 * side effects : this slot of 2D array has filled codeword
 * expects      : in bounds col and row, non-null 2d array, non-null closure
 * note         : intended to be apply function for map
 */
void read_codeword(int col, int row, A2Methods_UArray2 codewords, void *elem, 
                   void *cl)
{
    assert(codewords != NULL);
    assert(cl != NULL);
  
    Extra_Data *data = cl;
    assert(data->methods != NULL);
    assert(data->input != NULL);
    
    A2Methods_T methods = data->methods;
    FILE *smallImage    = data->input;
  
    assert(0 <= col && col < methods->width(codewords));
    assert(0 <= row && row < methods->height(codewords));
    
    (void)elem;
    
    int byte, field;
    uint32_t word = 0;
  
    for ( byte = 4; byte > 0; byte-- ) {
        field = getc(smallImage);
        assert(field != EOF);
        word = Bitpack_newu(word, 8, ( 8 * byte ) - 8, field);
    }

    *(uint32_t *)(methods->at(codewords, col, row)) = word;
}
