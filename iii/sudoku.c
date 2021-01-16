/* 
 * sudoku.c
 * Zoe Hsieh, Rebecca Miller, 2/10/20
 * hw2 : iii
 * 
 * driver to check if sudoku is solved
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "assert.h"
#include "pnmrdr.h"
#include "uarray2.h"
#include "sudoku_helpers.h"

void check_sudoku(FILE *image);
void get_pix(int col, int row, UArray2_T sudo, void *elem, void *cl);

int main(int argc, char *argv[])
{ 
    FILE *grid = NULL;
    assert(argc <= 2);

    if ( argc == 1 ) {
      grid = stdin;
    }
  
    else {
        grid = fopen(argv[1], "rb");
        assert(grid);
    }
    
    check_sudoku(grid);
    
    return EXIT_SUCCESS;  
}


/* 
 * name    : check_sudoku
 * purpose : reads in the Pnmrdr file of the sudoku grid into UArray2 set up,
 *           and then checks if it is a good sudoku
 * inputs  : FILE *grid
 * outputs : none
 * expects : non-null, open FILE *
 */
void check_sudoku(FILE *grid)
{
    assert(grid);
  
    struct Pnmrdr_T *reader = Pnmrdr_new(grid);
  
    Pnmrdr_mapdata data = Pnmrdr_data(reader);
    int width           = data.width;
    int height          = data.height;
    int denom           = data.denominator;
    Pnmrdr_maptype type = data.type;
    assert(type == Pnmrdr_gray);
  
    assert(denom == 9 && width == 9 && height == 9);
  
    UArray2_T sudoku = UArray2_new(width, height, sizeof(size_t));
    UArray2_map_row_major(sudoku, get_pix, reader); 
    
    row_check(sudoku);
    col_check(sudoku);      
    submaps_check(sudoku);
  
    UArray2_free(&sudoku);
    Pnmrdr_free(&reader);
    fclose(grid);
}


/* 
 * name    : get_pix
 * purpose : read all pixels (numerators) from Pnmrdr into UArray2_T
 * inputs  : int col, int row, UArray2_T sudo, void *elem, void *reader
 * outputs : none
 * expects : 0 <= col < width of UArray2, 0 <= row < height of UArray2, 
 *           non-null UArray2_T, non-null and open Pnmrdr pointer for *cl
 */
void get_pix(int col, int row, UArray2_T sudo, void *elem, void *cl)
{
    assert(sudo);
    assert(cl);

    int width  = UArray2_width(sudo);
    int height = UArray2_height(sudo);
    assert(0 <= col && col < width && 0 <= row && row < height);
    (void)elem;
  
    Pnmrdr_T reader = cl;
    size_t pix      = Pnmrdr_get(reader);
  
    assert(pix > 0);
  
    *(size_t *)UArray2_at(sudo, col, row) = pix;
}
