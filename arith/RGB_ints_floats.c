/* 
 * RGB_ints_floats.c
 * Zoe Hsieh, Rebecca Miller, 3/7/20
 * hw4 : arith
 * 
 * converts RGB ints to RGB floats and vice versa
 */

#include "RGB_ints_floats.h"

struct Extra_Data {
    A2Methods_UArray2 rgb; // 2d arr of either rgb ints or floats
    unsigned denom;        // denominator of rgb ints
    A2Methods_T methods;   // methods for 2d arrays
};
typedef struct Extra_Data Extra_Data;

void pix_int_to_float(int col, int row, Pnm_rgb elem, void *cl);
void pix_float_to_int(int col, int row, A2Methods_UArray2 pixels, 
                      void *elem, void *cl);
void condense_rgb_f(Rgb_f pix_f);
void condense_rgb(Pnm_rgb pix);

/****************************** PUBLIC FUNCTIONS *****************************/


/* purpose : convert RGB ints to RGB floats
 * note    : uses Extra_Data struct to pass in necessary data to apply function
 */
A2Methods_UArray2 RGB_ints_to_floats(A2Methods_UArray2 pixels, 
                                     A2Methods_T methods, 
                                     unsigned denominator,
                                     unsigned width, unsigned height)
{   
    assert(pixels != NULL);
    assert(methods != NULL);
    assert(denominator > 0);
  
    Rgb_f temp;
    A2Methods_UArray2 float_arr = methods->new(width, height, 
                                               sizeof(*temp));
  
    Extra_Data data = {float_arr, denominator, methods};
  
    assert(pixels != NULL);
    assert(&data != NULL);
    
    unsigned row, col;
  
    for ( row = 0; row < height; row++ ) {
        for ( col = 0; col < width; col++ ) {
            pix_int_to_float(col, row, methods->at(pixels, col, row), &data);
        }
    }
  
    return data.rgb;
}
  
/* purpose : convert RGB floats to RGB ints
 * note    : uses Extra_Data struct to pass in necessary data to apply function
 */
A2Methods_UArray2 RGB_floats_to_ints(A2Methods_UArray2 pixels, 
                                     A2Methods_T methods, 
                                     unsigned denominator)
{ 
    assert(pixels != NULL);
    assert(methods != NULL);
    assert(denominator > 0);
  
    Pnm_rgb temp;
    A2Methods_UArray2 int_arr = methods->new(methods->width(pixels), 
                                             methods->height(pixels), 
                                             sizeof(*temp));
  
    Extra_Data data = {int_arr, denominator, methods};

    methods->map_default(pixels, pix_float_to_int, &data);
  
    return data.rgb;
}
  
/**************************** COMPRESSION HELPERS ****************************/


/* name         : pix_int_to_float
 * purpose      : converts a single Pnm_rgb struct to proportional Rgb_f
 * inputs       : int col and row within UArray2, UArray2 they're inside,
 *                void * to Pnm_rgb within element, and closure
 * outputs      : none
 * side effects : stores populated Rgb_f struct inside parallel UArray2 element
 * expects      : non-void elem of type Pnm_rgb, non-void and populated closure
 *                of type &Extra_Data
 */
void pix_int_to_float(int col, int row, Pnm_rgb elem, void *cl)
{
    assert(elem != NULL);
    assert(cl != NULL);

    Extra_Data *data = cl;
    assert(data->methods != NULL);
    assert(data->rgb != NULL);
    assert(data->denom > 0);
    
    A2Methods_UArray2 rgb_floats  = data->rgb;
    float denominator             = (float) data->denom;
    A2Methods_T methods           = data->methods;

    Pnm_rgb ints = elem;
  
    Rgb_f floats = malloc(sizeof(*floats));
    floats->rf = (float)(ints->red)   / denominator;
    floats->gf = (float)(ints->green) / denominator;
    floats->bf = (float)(ints->blue)  / denominator;
  
    *(Rgb_f)(methods->at(rgb_floats, col, row)) = *floats;
    free(floats);
}


/*************************** DECOMPRESSION HELPERS ***************************/


/* name         : pix_float_to_int
 * purpose      : converts a single Rgb_f struct to proportional Pnm_rgb
 * inputs       : int col and row within UArray2, UArray2 they're inside,
 *                void * to Rgb_f within element, and closure
 * outputs      : none
 * side effects : stores populated Pnm_rgb struct inside parallel UArray2 
 *                element
 * expects      : non-void elem of type Rgb_f, non-void and populated closure
 *                of type &Extra_Data
 */
void pix_float_to_int(int col, int row, A2Methods_UArray2 pixels, void *elem, 
                      void *cl)
{
    assert(elem != NULL);
    assert(cl != NULL);
  
    (void) col;
    (void) row; 
    (void) pixels;
  
    Extra_Data *data = cl;
    assert(data->methods != NULL);
    assert(data->rgb != NULL);
    assert(data->denom > 0);
  
    A2Methods_UArray2 rgb_ints  = data->rgb;
    float denominator           = (float) data->denom;
    A2Methods_T methods         = data->methods;

    Rgb_f floats = elem;
  
    condense_rgb_f(floats);
    
    Pnm_rgb ints = malloc(sizeof(*ints));
    ints->red   = (unsigned)((floats->rf) * denominator);
    ints->green = (unsigned)((floats->gf) * denominator);
    ints->blue  = (unsigned)((floats->bf) * denominator);
  
    condense_rgb(ints);
  

    *(Pnm_rgb)(methods->at(rgb_ints, col, row)) = *ints;
    free(ints);
}


/* name         : condense_rgb_f
 * purpose      : force rgb floats into nonnegative range
 * inputs       : Rgb_f struct
 * outputs      : none
 * side effects : red, green, and blue in struct are nonnegative
 * expects      : non-null, initialized Rgb_f struct
 */
void condense_rgb_f(Rgb_f pix_f)
{
    assert(pix_f != NULL);
  
    if ( pix_f->rf < 0 ) {
        pix_f->rf = 0;
    }
  
    if ( pix_f->gf < 0 ) {
        pix_f->gf = 0;
    }
  
    if ( pix_f->bf < 0 ) {
        pix_f->bf = 0;
    }
}


/* name         : condense_rgb
 * purpose      : force rgb ints to be no greater than denominator
 * inputs       : Pnm_rgb struct
 * outputs      : none
 * side effects : red, green, and blue of Pnm_rgb <= 255
 * expects      : non-null, initialized Pnm_rgb
 */
void condense_rgb(Pnm_rgb pix)
{
    assert(pix != NULL);
  
    if ( pix->red > 255 ) {
        pix->red = 255;
    }
  
    if ( pix->green > 255 ) {
        pix->green = 255;
    }
  
    if ( pix->blue > 255 ) {
        pix->blue = 255;
    }
}
