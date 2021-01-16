/* 
 * brightness.c
 * Zoe Hsieh, Rebecca Miller, 1/31/20
 * hw1 : filesnpix
 * 
 * main driver to calculate average brightness of image
 */

#include <stdio.h>
#include <stdlib.h>
#include "pnmrdr.h"

void find_numerator(size_t *numerators, int size, struct Pnmrdr_T **reader);
size_t sum_arr(size_t *arr, int size);
float div_to_float(float numer, size_t denom); 

/************************************ MAIN ***********************************/


int main(int argc, char *argv[])
{ 
    FILE *image = NULL;
  
    if ( argc > 2 ) {
        fprintf(stderr, "Error: too many arguments\n");
        exit(EXIT_FAILURE);
    }
  
    else if ( argc == 1 ) {
        image = stdin;
    }
  
    else {
        image = fopen(argv[1], "rb");
    
    if ( image == NULL ) {
        fprintf(stderr, "Error: could not open %s\n", argv[1]);
        fclose(image);
        exit(EXIT_FAILURE);
        }
    }
  
    struct Pnmrdr_T *reader = Pnmrdr_new(image);
  
    Pnmrdr_mapdata data = Pnmrdr_data(reader);
    size_t width        = data.width;
    size_t height       = data.height;
    size_t denom        = data.denominator;
  
    if ( width == 0 && height == 0 ) {
        fprintf(stderr, "Error: no valid image provided\n");
        exit(EXIT_FAILURE);
    }
  
    int size = width * height;
    size_t numerators[size];
  
    find_numerator(numerators, size, &reader);

    size_t all_numer       = sum_arr(numerators, size);
    float avg_numer        = div_to_float(all_numer, size);
    float avg_brightness   = div_to_float(avg_numer, denom);
  
    printf("%.3f\n", avg_brightness);
  
    Pnmrdr_free(&reader);

    fclose(image);
  
    return EXIT_SUCCESS;
}

/****************************** HELPER FUNCTIONS *****************************/

/*
 * name : find_numerator
 * purp : iterating through each pixel and retrieving the 
 *        numerator and storing it in an array
 * args : size_t array of numerators, int size of array, Pnmrdr_T **reader
 * rets : none
 */

void find_numerator(size_t *numerators, int size, struct Pnmrdr_T **reader) 
{
    int i;
    for ( i = 0; i < size; i++ ) {
        numerators[i] = Pnmrdr_get(*reader);
    }
}

/*
 * name : div_to_float
 * purp : divides size_t by float to get a float
 * args : float numer, size_t denom
 * rets : float quotient
 */

float div_to_float(float numer, size_t denom) 
{
    return numer / denom;
}

/*
 * name : sum_arr
 * purp : sums all of the elements in array
 * args : size_t *arr, int size
 * rets : size_t
 */

size_t sum_arr(size_t *arr, int size)
{
    int i;
    size_t total = 0;
  
    for ( i = 0; i < size; i++ ) {
        total += arr[i];
    }
  
    return total;
}
