/* 
 * RGB_VCS.c
 * Zoe Hsieh, Rebecca Miller, 3/7/20
 * hw4 : arith
 * 
 * converts RGB floats to VCS elements and vice versa
 */

#include "RGB_VCS.h"

struct Extra_Data {
    A2Methods_UArray2 pixels; // 2d array of either rgb floats or vcs values
    A2Methods_T methods;      // methods for 2d arrays
};
typedef struct Extra_Data Extra_Data;

void get_vcs(int col, int row, A2Methods_UArray2 pixels, void *elem, void *cl);
void get_rgb_floats(int col, int row, A2Methods_UArray2 pixels, void *elem, 
                    void *cl);


/****************************** PUBLIC FUNCTIONS *****************************/

/* purpose : convert 2d array of rgb floats to vcs values
 */
A2Methods_UArray2 RGB_to_VCS(A2Methods_UArray2 rgb_floats, A2Methods_T methods)
{ 
  
    VCS_elems temp;
    A2Methods_UArray2 vcs_values = methods->new(methods->width(rgb_floats), 
                                                methods->height(rgb_floats), 
                                                sizeof(*temp));
  
    Extra_Data data = {vcs_values, methods};
  
    methods->map_default(rgb_floats, get_vcs, &data);
  
    return data.pixels;
}


/* purpose : convert 2d array of vcs values to rgb floats
 */
A2Methods_UArray2 VCS_to_RGB(A2Methods_UArray2 vcs_vals, A2Methods_T methods)
{
  
    Rgb_f temp;
    A2Methods_UArray2 rgb_floats = methods->new(methods->width(vcs_vals), 
                                                methods->height(vcs_vals), 
                                                sizeof(*temp));
    
    Extra_Data data = {rgb_floats, methods};
  
    methods->map_default(vcs_vals, get_rgb_floats, &data);
  
    return data.pixels;
}

/**************************** COMPRESSION HELPERS ****************************/


/* name         : get_vcs
 * purpose      : convert one pixel worth of rgb floats to vcs equivalents
 * inputs       : ints for col and row, 2d array of rgb floats, void * to
 *                current elem, closure
 * outputs      : none
 * side effects : parallel element in vcs 2d arr is initialized
 * expects      : non-null Rgb_f elem, non-null Extra_Data closure
 * note         : intended as apply for 2d array map
 */
void get_vcs(int col, int row, A2Methods_UArray2 pixels, void *elem, void *cl)
{
    (void) col;
    (void) row; 
    (void) pixels;
  
    assert(cl != NULL);
    assert(elem != NULL);
    
    Extra_Data *data = cl;
    assert(data->pixels != NULL);
    assert(data->methods != NULL);
  
    A2Methods_UArray2 vcs_pixels  = data->pixels;
    A2Methods_T methods           = data->methods;

    Rgb_f floats = elem;
  
    VCS_elems vcs_val = malloc(sizeof(*vcs_val));
    vcs_val->y  = (0.299 * floats->rf) + (0.587 * floats->gf) 
                   + (0.114 * floats->bf);
    vcs_val->pb = (-0.168736 * floats->rf) - (0.331264 * floats->gf) 
                   + (0.5 * floats->bf);
    vcs_val->pr = (0.5 * floats->rf) - (0.418688 * floats->gf) 
                   - (0.081312 * floats->bf);
  
    *(VCS_elems)methods->at(vcs_pixels, col, row) = *vcs_val;
    free(vcs_val);
}

/*************************** DECOMPRESSION HELPERS ***************************/


/* name         : get_rgb_floats
 * purpose      : convert one pixel worth of vcs values to rgb float 
 *                equivalents
 * inputs       : ints for col and row, 2d array of vcs values, void *
 *                to current elem, closure
 * outputs      : none
 * side effects : parallel element of 2d array for rgb floats initialized
 * expects      : non-null VCS_elems element, non-null Extra_Data closure
 * note         : intended as apply for 2d array map
 */
void get_rgb_floats(int col, int row, A2Methods_UArray2 pixels, void *elem, 
                    void *cl)
{
    (void) col;
    (void) row; 
    (void) pixels;
    
    Extra_Data *data = cl;
    assert(data->pixels != NULL);
    assert(data->methods != NULL);
  
    A2Methods_UArray2 rgb_floats  = data->pixels;
    A2Methods_T methods           = data->methods;

    VCS_elems vcs_vals = elem;

    Rgb_f floats = malloc(sizeof(*floats));
    floats->rf = (1.0 * vcs_vals->y) + (0.0 * vcs_vals->pb) 
                  + (1.402 * vcs_vals->pr);
    floats->gf = (1.0 * vcs_vals->y) - (0.344136 * vcs_vals->pb) 
                  - (0.714136 * vcs_vals->pr);
    floats->bf = (1.0 * vcs_vals->y) + (1.772 * vcs_vals->pb) 
                  + (0.0 * vcs_vals->pr);      
      
    *(Rgb_f) methods->at(rgb_floats, col, row) = *floats;
    free(floats);
}
