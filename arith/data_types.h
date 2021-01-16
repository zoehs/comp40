/* 
 * data_types.h
 * Zoe Hsieh, Rebecca Miller, 3/7/20
 * hw4 : arith
 * 
 * holds all structs needed to store different variable/
 * element types
 */

#include <stdlib.h>

#ifndef DATA_TYPES_H
#define DATA_TYPES_H

struct Rgb_f {
    float rf, gf, bf; // floats for rgb divided by denominator
};
typedef struct Rgb_f *Rgb_f;

struct VCS_elems {
    float y, pb, pr; // floats for y, pb, pr
};
typedef struct VCS_elems *VCS_elems;

struct DCT_elems {
    unsigned a;                  // scaled unsigned a
    signed b, c, d;              // quantized signed bcd
    unsigned pr_index, pb_index; // indexed unsigned pb, pr
};
typedef struct DCT_elems *DCT_elems;

struct DCT_intermediate {
    float a_f, b_f, c_f, d_f; // float non-quantized abcd
    float pr_avg, pb_avg;     // float average pr, pb for 4 pix
};
typedef struct DCT_intermediate *DCT_intermediate;

#endif
