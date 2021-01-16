/* 
 * VCS_DCT.c
 * Zoe Hsieh, Rebecca Miller, 3/7/20
 * hw4 : arith
 * 
 * converts VCS elements to DCT elements and vice versa
 */

#include "VCS_DCT.h"

DCT_intermediate vcs_to_abcd_floats(VCS_elems pix1, VCS_elems pix2, 
                                    VCS_elems pix3, VCS_elems pix4);
void get_abcd_ints(DCT_intermediate block_intermediate, DCT_elems block_dct);
float condense_float(float x, float max, float min);
signed quantize_float(float x, float vals[], int length);
DCT_intermediate get_abcd_floats(DCT_elems block_dct);
void storing_y(A2Methods_UArray2 vcs_values, DCT_intermediate block, int row, 
               int col, A2Methods_T methods);


/****************************** PUBLIC FUNCTIONS *****************************/


/* purpose : convert 2d array of vcs values to dct values
 */
A2Methods_UArray2 VCS_to_DCT(A2Methods_UArray2 vcs_arr, A2Methods_T methods)
{
    int width = methods->width(vcs_arr);
    int height = methods->height(vcs_arr);
                 
    DCT_elems temp;
    A2Methods_UArray2 dct_arr = methods->new(width / 2, height / 2, 
                                             sizeof(*temp));
  
    int row, col;
    for ( row = 0; row < height; row += 2 ) {
        for ( col = 0; col < width; col += 2 ) {
            
            VCS_elems pix1 = methods->at(vcs_arr, col, row);
            
            VCS_elems pix2 = methods->at(vcs_arr, col + 1, row);
            
            VCS_elems pix3 = methods->at(vcs_arr, col, row + 1);
            
            VCS_elems pix4 = methods->at(vcs_arr, col + 1, row + 1);
          
            DCT_elems block_dct = methods->at(dct_arr, col/2, row/2);
          
            DCT_intermediate block_intermediate = vcs_to_abcd_floats(pix1, 
                                                                     pix2, 
                                                                     pix3, 
                                                                     pix4);
            get_abcd_ints(block_intermediate, block_dct);
          
            free(block_intermediate);
        }
    }
                 
    return dct_arr;
}
         
/* purpose : convert 2d array of dct values to vcs values
 */
A2Methods_UArray2 DCT_to_VCS(A2Methods_UArray2 DCT_values, A2Methods_T methods)
{
    
    int width = methods->width(DCT_values);
    int height = methods->height(DCT_values);
    VCS_elems temp;
    A2Methods_UArray2 vcs_values = methods->new(width * 2, height * 2, 
                                                sizeof(*temp));
  
    int row, col;
  
    for ( row = 0; row < height; row++ ) {
        for ( col = 0; col < width; col++ ) {
            
            DCT_elems block_dct = methods->at(DCT_values, col, row);
            
            DCT_intermediate curr_block = get_abcd_floats(block_dct);
            
            storing_y(vcs_values, curr_block, row, col, methods);
            free(curr_block);
        }
    }
    
    return vcs_values;   
}


/**************************** COMPRESSION HELPERS ****************************/
      

/* name         : vcs_to_abcd_floats
 * purpose      : convert vcs values for blocks of 4 pixels to equivalent dct 
 *                float values
 * inputs       : VCS_elems structs for 2 x 2 block of pixels
 * outputs      : DCT_intermediate holding 6 condensed values
 * side effects : DCT_intermediate block initialized with abcd and averaged pb 
 *                and pr
 * expects      : non-null and initialized pix1 - 4
 */
DCT_intermediate vcs_to_abcd_floats(VCS_elems pix1, VCS_elems pix2, 
                                    VCS_elems pix3, VCS_elems pix4)
{
    assert(pix1 != NULL);
    assert(pix2 != NULL);
    assert(pix3 != NULL);
    assert(pix4 != NULL);
  
    DCT_intermediate block = malloc(sizeof(*block));
    
    /* getting y */
    float y1 = pix1->y;
    float y2 = pix2->y;
    float y3 = pix3->y;
    float y4 = pix4->y;
  
    /* converting to a, b, c, d */
    block->a_f = (y4 + y3 + y2 + y1) / 4.0;
    block->b_f = (y4 + y3 - y2 - y1) / 4.0;
    block->c_f = (y4 - y3 + y2 - y1) / 4.0;
    block->d_f = (y4 - y3 - y2 + y1) / 4.0;
  
    /* getting PB, PR average*/
    block->pb_avg = (pix1->pb + pix2->pb + pix3->pb + pix4->pb) / 4;
    block->pr_avg = (pix1->pr + pix2->pr + pix3->pr + pix4->pr) / 4;
  
    return block;
}


/* name         : get_abcd_ints
 * purpose      : converts 2d array of dct floats to quantized integers
 * inputs       : DCT_intermediate struct of float values, DCT_elems struct
 *                of empty int values
 * outputs      : none
 * side effects : DCT_elems struct initialized with quantized abcd and indexed
 *                pb and pr
 * expects      : non-null and initialized DCT_intermediate, non-null DCT_elems
 */
void get_abcd_ints(DCT_intermediate block_intermediate, DCT_elems block_dct)
{
    assert(block_intermediate != NULL);
    assert(block_dct != NULL);
  
    /* quantization of a, b, c, d */
    block_dct->a = (unsigned)(block_intermediate->a_f * 63);
  
    /* condense b, c, d */
    float b_f = condense_float(block_intermediate->b_f, 0.3, -0.3);
    float c_f = condense_float(block_intermediate->c_f, 0.3, -0.3);
    float d_f = condense_float(block_intermediate->d_f, 0.3, -0.3);
  
    float quant[] = {-0.30, -0.28, -0.25, -0.23, -0.20, -0.18, -0.16, -0.15,
                     -0.13, -0.12, -0.11, -0.1, -0.095, -0.088, -0.082, -0.077,
                     -0.072, -0.066, -0.060, -0.055, -0.050, -0.044, -0.039, 
                     -0.033, -0.028, -0.022, -0.018, -0.011, -0.008, -0.005, 
                     -0.002, 0, 0.002, 0.005, 0.008, 0.011, 0.018, 0.022, 
                     0.028, 0.033, 0.039, 0.044, 0.050, 0.055, 0.060, 0.066, 
                     0.072, 0.077, 0.082, 0.088, 0.095, 0.1, 0.11, 0.12, 0.13, 
                     0.15, 0.16, 0.18, 0.20, 0.23, 0.25, 0.28, 0.30};
  
    block_dct->b = quantize_float(b_f, quant, 63);
    block_dct->c = quantize_float(c_f, quant, 63);
    block_dct->d = quantize_float(d_f, quant, 63);
  
    /* getting PB, PR index */
    block_dct->pb_index = Arith40_index_of_chroma(block_intermediate->pb_avg);
    block_dct->pr_index = Arith40_index_of_chroma(block_intermediate->pr_avg);
}


/* name         : condense_float
 * purpose      : forces float into range between min and max
 * inputs       : float x to condense, floats for max and min values
 * outputs      : float that's condensed x
 * side effects : none
 * expects      : 3 floats
 */
float condense_float(float x, float max, float min)
{
    if ( x > max ) {
        return max;
    }
    else if ( x < min ) {
        return min;
    }
    else {
        return x;
    }
}


/* name         : quantize_float
 * purpose      : quantizes float to index corresponding to closest value
 * inputs       : float x to quantize, arr of floats vals to compare x to,
 *                length of vals
 * outputs      : signed quantized version of x
 * side effects : none
 * expects      : non-null vals, length is length of vals
 * note         : quantizes by comparing to array of 31 elements (15 negative,
 *                15 positive, and 0), finding closest one, and returning 
 *                its index - 15
 */
signed quantize_float(float x, float vals[], int length)
{
    assert(vals != NULL);
    signed i;
  
    for ( i = 0; i < (signed) length; i++ ) {
        if ( x < vals[i] ) {
            float lowerDist = x - vals[i - 1];
            float upperDist = vals[i] - x;

            if ( upperDist < lowerDist ) { /* closer to val on right */
                return i - 31;
            }
            else { /* closer to val on left, goes with smaller value if there's
                      a tie */            
                return i - 32;
            }
        }
        else if ( x == vals[i] ) {
            return i - 31;
        }
    }
    return length - 32;
}


/*************************** DECOMPRESSION HELPERS ***************************/


/* name         : get_abcd_floats
 * purpose      : convert DCT_elems to DCT_intermediate by attempting to undo
 *                quantization
 * inputs       : DCT_elems block
 * outputs      : DCT_intermediate block for same pixels
 * side effects : initializes DCT_intermediate with values that have lost 
 *                accuracy
 * expects      : non-null DCT_elems struct
 */
DCT_intermediate get_abcd_floats(DCT_elems block_dct)
{
    assert(block_dct != NULL);
  
    DCT_intermediate block_intermediate = malloc(sizeof(*block_intermediate));
  
    block_intermediate->a_f = (float)(block_dct->a) / 63;
    
    float quant[] = {-0.30, -0.28, -0.25, -0.23, -0.20, -0.18, -0.16, -0.15,
                     -0.13, -0.12, -0.11, -0.1, -0.095, -0.088, -0.082, -0.077,
                     -0.072, -0.066, -0.060, -0.055, -0.050, -0.044, -0.039, 
                     -0.033, -0.028, -0.022, -0.018, -0.011, -0.008, -0.005, 
                     -0.002, 0, 0.002, 0.005, 0.008, 0.011, 0.018, 0.022, 
                     0.028, 0.033, 0.039, 0.044, 0.050, 0.055, 0.060, 0.066, 
                     0.072, 0.077, 0.082, 0.088, 0.095, 0.1, 0.11, 0.12, 0.13, 
                     0.15, 0.16, 0.18, 0.20, 0.23, 0.25, 0.28, 0.30};
  
    block_intermediate->b_f = quant[block_dct->b + 31];
    block_intermediate->c_f = quant[block_dct->c + 31];
    block_intermediate->d_f = quant[block_dct->d + 31];
  
    block_intermediate->pb_avg = Arith40_chroma_of_index(block_dct->pb_index);
    block_intermediate->pr_avg = Arith40_chroma_of_index(block_dct->pr_index);
  
    return block_intermediate;
}  


/* name         : storing_y
 * purpose      : retrieve values for individual pixels from block of 
 *                DCT_intermediate
 * inputs       : 2d array of vcs_values to fill, DCT_intermediate block, int 
 *                row and col of upper left pixel in block, methods to use for 
 *                2d array
 * outputs      : none
 * side effects : 4 pixels in that block in vcs_values 2d array initialized
 * expects      : non-null vcs_values, non-null block, row and col in range, 
 *                non-null methods
 */
void storing_y(A2Methods_UArray2 vcs_values, DCT_intermediate block, int row, 
               int col, A2Methods_T methods)
{         
    assert(vcs_values != NULL);
    assert(block != NULL);
    assert(methods != NULL);
    assert(0 <= col && col < methods->width(vcs_values));
    assert(0 <= row && row < methods->height(vcs_values));
  
    /* y1 */
    VCS_elems ul = malloc(sizeof(*ul));        
    ul->y  = condense_float((block->a_f) - (block->b_f) - (block->c_f) 
                             + (block->d_f), 1.0, 0.0);
    ul->pb = condense_float(block->pb_avg, 0.5, -0.5);
    ul->pr = condense_float(block->pr_avg, 0.5, -0.5);
    *(VCS_elems)methods->at(vcs_values, 2 * col, 2 * row) = *ul;
    free(ul);

    /*y2*/
    VCS_elems ur = malloc(sizeof(*ur));
    ur->y  = condense_float((block->a_f) - (block->b_f) + (block->c_f) 
                             - (block->d_f), 1, 0);
    ur->pb = condense_float(block->pb_avg, 0.5, -0.5);
    ur->pr = condense_float(block->pr_avg, 0.5, -0.5);
    *(VCS_elems)methods->at(vcs_values, (2 * col) + 1, 2 * row) = *ur;
    free(ur);

    /*y3 */
    VCS_elems ll = malloc(sizeof(*ll));
    ll->y  = condense_float((block->a_f) + (block->b_f) - (block->c_f) 
                             - (block->d_f), 1, 0);
    ll->pb = condense_float(block->pb_avg, 0.5, -0.5);
    ll->pr = condense_float(block->pr_avg, 0.5, -0.5);
    *(VCS_elems)methods->at(vcs_values, 2 * col, (2 * row) + 1) = *ll;
    free(ll);
  
    /*y4 */ 
    VCS_elems lr = malloc(sizeof(*lr));
    lr->y  = condense_float((block->a_f) + (block->b_f) + (block->c_f) 
                             + (block->d_f), 1, 0);
    lr->pb = condense_float(block->pb_avg, 0.5, -0.5);
    lr->pr = condense_float(block->pr_avg, 0.5, -0.5);
    *(VCS_elems)methods->at(vcs_values, (2 * col) + 1, (2 * row) + 1) = *lr;
    
    free(lr);
}
