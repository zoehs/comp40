/* 
 * unblackedges.c
 * Zoe Hsieh, Rebecca Miller, 2/10/20
 * hw2 : iii
 * 
 * driver to remove black edge pixels from pbm
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "assert.h"
#include "pnmrdr.h"
#include "uarray2.h"
#include "bit2.h"


#define BLACK 1
#define WHITE 0

void read_in_pic(FILE *picture);
void get_pix(int col, int row, Bit2_T pixels, int elem, void *cl);
void init_status_arr(int col, int row, UArray2_T status_arr, void *elem, 
                     void *cl);
void change_black_edges(Bit2_T pic, UArray2_T arr);
void spiral_clockwise(Bit2_T pic, UArray2_T arr);
void spiral_counterclockwise(Bit2_T pic, UArray2_T arr);
void check_col(int start_row, int end_row, int col, Bit2_T pic, UArray2_T arr);
void check_row(int start_col, int end_col, int row, Bit2_T pic, UArray2_T arr);
void check_around_pixel(int col, int row, Bit2_T pic, int elem, 
                        void *status_arr);
bool is_edge(int width, int height, int col, int row);
void pbmwrite(FILE *outputfp, Bit2_T bitmap);
void print_one_pixel(int col, int row, Bit2_T bitmap, int elem, void *file);


/************************************ MAIN ***********************************/

int main(int argc, char *argv[])
{
    assert(argc <= 2);
    FILE *image = NULL;

    if ( argc == 1 ) {
      image = stdin;
    }
    else {
        image = fopen(argv[1], "rb");
        assert(image);
    }
    read_in_pic(image);
  
    return EXIT_SUCCESS;
}


/************************** INITIALIZATION FUNCTIONS *************************/

/* 
 * name    : read_in_pic
 * purpose : main function to read from pbm, whiten black edges, and write 
 *           to stdout
 * inputs  : FILE *picture in pbm format
 * outputs : none
 * expects : non-null picture file in valid pbm format
 */
void read_in_pic(FILE *picture)
{
    assert(picture);
    
    /* initialize Pnmrdr */
    struct Pnmrdr_T *reader = Pnmrdr_new(picture);
    Pnmrdr_mapdata pixels   = Pnmrdr_data(reader);
    int width               = pixels.width;
    int height              = pixels.height;
    Pnmrdr_maptype type     = pixels.type;
    assert(type == Pnmrdr_bit);
    
    /* read in pixels */
    Bit2_T pic = Bit2_new(width, height);
    Bit2_map_row_major(pic, get_pix, reader);
    Pnmrdr_free(&reader);
    
    /* initialize parallel bool UArray2_T */
    UArray2_T black_edges = UArray2_new(width, height, sizeof(bool));
    UArray2_map_row_major(black_edges, init_status_arr, NULL); 
    
    /* unblackedges and print */
    change_black_edges(pic, black_edges);
    pbmwrite(stdout, pic);
    
    /* clean up */
    Bit2_free(&pic);
    UArray2_free(&black_edges);
    fclose(picture);
}


/* 
 * name    : get_pix
 * purpose : read all pixels (numerators) from Pnmrdr into Bit2_T
 * inputs  : int col, int row, Bit2_T pixels, void *elem, void *cl (Pnmreader)
 * outputs : none
 * expects : 0 <= col < width of Bit2, 0 <= row < height of Bit2, 
 *           non-null Bit2, non-null and open Pnmrdr pointer for *cl
 */
void get_pix(int col, int row, Bit2_T pixels, int elem, void *cl)
{
    assert(pixels);
    assert(cl);

    int width  = Bit2_width(pixels);
    int height = Bit2_height(pixels);
    assert(0 <= col && col < width && 0 <= row && row < height);
    (void)elem;
  
    Pnmrdr_T reader = cl;
    size_t pix      = Pnmrdr_get(reader);
  
    Bit2_put(pixels, col, row, pix);
}


/* 
 * name    : init_status_arr
 * purpose : apply function for UArray2's map, initialize a UArray2 array  
 *           of structs to hold information of each pixel 
 * inputs  : int col, int row, UArray2_T status_arr, void *elem, void *cl
 * outputs : returns nothing; status struct set to have all false values
 * expects : non-null UArray2_T, 0 <= col < width, 0 <= row < height
 */
void init_status_arr(int col, int row, UArray2_T status_arr, void *elem, 
                     void *cl)
{
    assert(status_arr);
    assert(0 <= col && col < UArray2_width(status_arr));
    assert(0 <= row && row < UArray2_height(status_arr));
  
    (void)elem;
    (void)cl;

  
    (*(bool *)UArray2_at(status_arr, col, row)) = false;
    
}

/**************************** UNBLACKING FUNCTIONS ***************************/

/*
 * name    : change_black_edges
 * purpose : faciliates location and conversion to white of black edges
 * inputs  : Bit2_T containing image bits, UArray2_T containing black_edge 
 *           bools
 * outputs : returns nothing; Bit2_T mutated to remove black edges and 
 *           UArray2_T updated to reflect where black edges used to be
 * expects : non-null and initialized Bit2_T and UArray2_T
 */
void change_black_edges(Bit2_T pic, UArray2_T arr)
{
    assert(pic);
    assert(arr);
  
    spiral_clockwise(pic, arr);
    spiral_counterclockwise(pic, arr);
    Bit2_map_row_major(pic, check_around_pixel, arr);
    Bit2_map_col_major(pic, check_around_pixel, arr);
}


/*
 * name    : spiral_clockwise
 * purpose : iterate through all pixels outside to inside in spiral fashion
 *           and convert black edge pixels to white clockwise
 * inputs  : Bit2_T containing image bits, UArray2_T containing black_edge 
 *           bools
 * outputs : returns nothing; Bit2_T mutated to remove black edges and 
 *           UArray2_T updated to reflect where black edges used to be
 * expects : non-null and initialized Bit2_T and UArray2_T
 */
void spiral_clockwise(Bit2_T pic, UArray2_T arr) 
{
    assert(pic);
    assert(arr);
  
    int i = 0;
    int width          = Bit2_width(pic);
    int height         = Bit2_height(pic);
    int cols_remaining = width;
    int rows_remaining = height;
  
    while ( true ) {
        /* top row */
        check_row(i, width - 1 - i, i, pic, arr); 
        if ( --cols_remaining == 0) {
            break;
        }
        
        /* right column */
        check_col(i + 1, height - 1 - i, width - 1 - i, pic, arr);
        if ( --rows_remaining == 0) {
            break;
        }
      
        /* bottom row */
        check_row(width - 2 - i, i, height - 1 - i, pic, arr); 
        if ( --cols_remaining == 0) {
            break;
        }
        
        /* left column */
        check_col(height - 2 - i, i + 1, i, pic, arr); 
        if ( --rows_remaining == 0) {
            break;
        }
      
        i++;
    }
}   


/*
 * name    : spiral_counterclockwise
 * purpose : iterate through all pixels outside to inside in spiral fashion
 *           and convert black edge pixels to white counterclockwise
 * inputs  : Bit2_T containing image bits, UArray2_T containing black_edge 
 *           bools
 * outputs : returns nothing; Bit2_T mutated to remove black edges and 
 *           UArray2_T updated to reflect where black edges used to be
 * expects : non-null and initialized Bit2_T and UArray2_T
 */
void spiral_counterclockwise(Bit2_T pic, UArray2_T arr)
{
    assert(pic);
    assert(arr);
  
    int i = 0;
    int width          = Bit2_width(pic);
    int height         = Bit2_height(pic);
    int cols_remaining = width;
    int rows_remaining = height;
  
    while ( true ) {
      /* left column */
      check_col(i, height - 1 - i, i, pic, arr);   
      if ( --rows_remaining == 0) {
          break;
      }
      
      /* bottom row */
      check_row(i + 1, width - 1 - i, height - 1 - i, pic, arr);
      if ( --cols_remaining == 0) {
          break;
      }
      
      /* right column */
      check_col(height - 2 - i, i, width - 1 - i, pic, arr); 
      if ( --rows_remaining == 0) {
          break;
      }
      
      /* top row */
      check_row(width - 2 - i, i + 1, i, pic, arr); 
      if ( --cols_remaining == 0) {
          break;
      }
      
      i++;
    }
}


/*
 * name    : check_col
 * purpose : iterate through pixels in one (partial) col and check for black 
 *           edges
 * inputs  : row to start, row to end, col to check, Bit2_T bits and UArray2_T
 *           bools
 * outputs : returns nothing; Bit2_T mutated to remove black edges from that 
 *           one col and UArray2_T updated to reflect where black edges used 
 *           to be
 * expects : non-void Bit2_T and UArray2_T, 0 <= start_row, end_row < height 
 *           of Bit2_T and UArray2_T, 0 <= col < width of Bit2_T and UArray2_T
 */
void check_col(int start_row, int end_row, int col, Bit2_T pic, UArray2_T arr) 
{
    assert(pic);
    assert(arr);
    int elem = -1;
    
    int width = Bit2_width(pic);
    int height = Bit2_height(pic);
    assert(0 <= start_row && start_row < height);
    assert(0 <= end_row && end_row < height);
    assert(0 <= col && col < width);
  
    int y;
  
    if ( start_row <= end_row ) {
        for ( y = start_row; y <= end_row; y++ ) {
            check_around_pixel(col, y, pic, elem, arr);
        }
    }
    else {
      
        for ( y = start_row; y >= end_row; y-- ) {
            check_around_pixel(col, y, pic, elem, arr);
        } 
    }
}


/*
 * name    : check_row
 * purpose : iterate through pixels in one (partial) row and check for 
 *           black edges
 * inputs  : column to start, column to end, row to check, Bit2_T bits and 
 *           UArray2_T bools
 * outputs : returns nothing; Bit2_T mutated to remove black edges from that 
 *           one row and UArray2_T updated to reflect where black edges 
 *           used to be
 * expects : non-void Bit2_T and UArray2_T, 0 <= start_col, end_col < width of
 *           Bit2_T and UArray2_T, 0 <= row < height of Bit2_T and UArray2_T
 */
void check_row(int start_col, int end_col, int row, Bit2_T pic, UArray2_T arr)
{
    assert(pic);
    assert(arr);
    int elem = -1;
    
    int width = Bit2_width(pic);
    int height = Bit2_height(pic);
    assert(0 <= start_col && start_col < width);
    assert(0 <= end_col && end_col < width);
    assert(0 <= row && row < height);
  
    int x;
  
    if ( start_col <= end_col ) {
      for ( x = start_col; x <= end_col; x++ ) {
            check_around_pixel(x, row, pic, elem, arr);
        }
    }
    else {
      for ( x = start_col; x >= end_col; x-- ) {
            check_around_pixel(x, row, pic, elem, arr);
      } 
    }
}


/* 
 * name    : check_around_pixel
 * purpose : checks each direction around the current pixel index to see 
 *           if it is touching any black edges.
 *           if it is, then the pixel is turned to white
 * inputs  : int col, int row, Bit2_T pic, UArray2_T status arr
 * outputs : none
 * expects : non-null Bit2_T and UArray2_T, 0 <= col < width, 
 *           0 <= row < height
 */
void check_around_pixel(int col, int row, Bit2_T pic, int elem, 
                        void *status_arr)
{  
    assert(pic);
    assert(status_arr);
    (void)elem;
    
    UArray2_T arr = status_arr;
    int width  = Bit2_width(pic);
    int height = Bit2_height(pic);
    assert(0 <= col && col < width && 0 <= row && row < height);
    
    if ( Bit2_get(pic, col, row) == BLACK ) {
        if ( is_edge(width, height, col, row) ) {
            (*(bool *)UArray2_at(arr, col, row)) = true;
            Bit2_put(pic, col, row, WHITE);
        }
        else {
            bool new_black_edge = false;

            if ( col != 0 ) {
                if ((*(bool *)UArray2_at(arr, col - 1, row)) == true) {
                    new_black_edge = true;
                }
            }

            if ( col != (width - 1 )) {
                if ((*(bool *)UArray2_at(arr, col + 1, row)) == true) {
                    new_black_edge = true;
                } 
            }
            if ( row != 0 ) {
                if ((*(bool *)UArray2_at(arr, col, row - 1)) == true) {
                    new_black_edge = true;
                }
            }
            if ( row != ( height - 1 )) {
                if ((*(bool *)UArray2_at(arr, col, row + 1)) == true) {
                    new_black_edge = true;
                }
            }
            if ( new_black_edge == true) {
                (*(bool *)UArray2_at(arr, col, row)) = true;
                Bit2_put(pic, col, row, WHITE);
            }
        }
    }
    
}

  

/* 
 * name    : is_edge
 * purpose : figures out if the current pixel index is on the edge (border)
 *           of our image
 * inputs  : int width, int height, int col, int row
 * outputs : true is it is an edge pixel, false if isn't
 * expects : width >= 0, height >= 0, 0 <= col < width, 0 <= row < height
 */
bool is_edge(int width, int height, int col, int row) 
{
    assert(width >= 0 && height >= 0);
    assert(0 <= col && col < width && 0 <= row && row < height);
  
    return col == 0 || col == width - 1 || row == 0 || row == height - 1;
}


/***************************** PRINTING FUNCTIONS ****************************/


/* 
 * name    : pbmwrite
 * purpose : the print function that will put the new image with unblack 
 *           edges to an output stream
 * inputs  : FILE *outputfp, Bit2_T bitmap
 * outputs : none
 * expects : non-null Bit2_T and output stream
 */
void pbmwrite(FILE *outputfp, Bit2_T bitmap)
{
    assert(outputfp && bitmap);
  
    fprintf(outputfp, "P1\n");
    fprintf(outputfp, "%d %d\n", Bit2_width(bitmap), Bit2_height(bitmap));
  
    Bit2_map_row_major(bitmap, print_one_pixel, outputfp);
  
}


/* 
 * name    : print_one_pixel
 * purpose : prints out a single pixel of the current index and a new line 
 *           each time it is called by map_row_major
 * inputs  : int col, int row, Bit2_T bitmap, int elem, void *cl
 * outputs : none
 * expects : non-null Bit2_T and output stream
 */
void print_one_pixel(int col, int row, Bit2_T bitmap, int elem, void *file)
{
    (void) col;
    (void) row;
    (void) bitmap;
    fprintf(file, "%d\n", elem);
}
