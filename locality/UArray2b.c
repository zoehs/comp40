/* 
 * UArray2b.c
 * Jun Sun Tak, Zoe Hsieh, 2/20/20
 * hw3 : locality
 * 
 */


#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "assert.h"
#include "uarray2b.h"
#include "uarray2.h"
#include "math.h"

#define T UArray2b_T

struct T {
	UArray2_T arr;
	int size;
	int blocksize;
	int arr_height; // Total number of cells across vertical blocks
	int arr_width; // Total number of cells across horizontal blocks
	int height;
	int width;
	 
};

/* 
 * name    : UArray2b_new
 * purpose : creates a new uarray2b 
 * inputs  : UArray2_T sudo
 * outputs : UArray2b_T
 */
T UArray2b_new(int width, int height, int size, int blocksize)
{
	assert(width > 0 && height > 0 && size > 0 && blocksize >= 1);
	UArray2b_T block_arr = malloc(sizeof(*block_arr));
	assert(block_arr != NULL);

	int num_block_vert = (int)ceil((double)height / blocksize);
	int num_block_hori = (int)ceil((double)width / blocksize);
	int num_cells_vert = num_block_vert * blocksize;
	int num_cells_hori = num_block_hori * blocksize;
	UArray2_T arr_2 = UArray2_new(num_cells_vert, num_cells_hori, size);
	assert(arr_2 != NULL);

	block_arr -> arr = arr_2; 
	block_arr -> size = size;
	block_arr -> blocksize = blocksize;
	block_arr -> arr_height = num_cells_vert;
	block_arr -> arr_width = num_cells_hori;
	block_arr -> height = height;
	block_arr -> width = width;

	return block_arr;
}

/* 
 * name    : UArray2b_new_64K_block
 * purpose : creates a new UArray2b with a default blocksize
 * inputs  : int width, int height, int size
 * outputs : UArray2b_T
 */
T UArray2b_new_64K_block(int width, int height, int size)
{
	assert(width > 0 && height > 0 && size > 0);
	int blocksize = -1; // Will always get reassigned

	if(size <= 64000) {
		blocksize = (int)floor(sqrt(64000 / size));
	} else {
		blocksize = 1;
	}
	if(height < blocksize || width < blocksize){
		if(height >= width) {
			blocksize = width;
		} else {
			blocksize = height;
		}
	}

	UArray2b_T block_arr = UArray2b_new(width, height, size, blocksize);
	return block_arr;
}

/* 
 * name    : UArray2b_free
 * purpose : frees the UArray2b
 * inputs  : T *block_arr
 * outputs : none
 */
void UArray2b_free(T *block_arr)
{
	assert(block_arr != NULL && *block_arr != NULL);
	UArray2_free(&(*block_arr) -> arr);
	free(*block_arr);
}

/* 
 * name    : UArray2b_width
 * purpose : gives the width of the array
 * inputs  : T block_arr
 * outputs : int width
 */
int UArray2b_width(T block_arr)
{
	assert(block_arr != NULL);
	return block_arr -> width;
}

/* 
 * name    : UArray2b_aheight
 * purpose : gives the height of the array
 * inputs  : T block_arr
 * outputs : int height
 */
int UArray2b_height(T block_arr)
{
	assert(block_arr != NULL);
	return block_arr -> height;
}

/* 
 * name    : UArray2b_size
 * purpose : gives the size
 * inputs  : T block_arr
 * outputs : int size
 */
int UArray2b_size(T block_arr)
{
	assert(block_arr != NULL);
	return block_arr -> size;
}

/* 
 * name    : UArray2b_blocksize
 * purpose : gives the blocksize
 * inputs  : T block_arr
 * outputs : int blocksize
 */
int UArray2b_blocksize(T block_arr)
{
	assert(block_arr != NULL);
	return block_arr -> blocksize;
}

/* 
 * name    : UArray2b_at
 * purpose : finds the element at the position given
 * inputs  : T array2b, int column, int row
 * outputs : void pointer of element
 */
void *UArray2b_at(T array2b, int column, int row)
{
	assert(array2b != NULL);
	assert(column >= 0 && column < UArray2b_width(array2b));
	assert(row >= 0 && row < UArray2b_height(array2b));
	void *return_elem;
	int blocksize = UArray2b_blocksize(array2b);
	if(blocksize == 1){
		return_elem = UArray2_at(array2b -> arr, column, row);
		return return_elem;
	}

	int block_qty = array2b -> arr_width / blocksize;
	int block_num = (row / blocksize * block_qty) + (column / blocksize);
	int block_elem = ((row % blocksize) * blocksize) + (column % blocksize);
	int index = (blocksize * blocksize * block_num) + block_elem;
	int i = index / (array2b -> arr_width);
	int j = index % (array2b -> arr_width);
	return_elem = UArray2_at(array2b -> arr, j, i);

	return return_elem;
}

/* 
 * name    : UArray2b_map
 * purpose : maps through the array2b
 * inputs  : T array2b, void apply(int col, int row, T array2b, 
 *           void *elem, void *cl), void *cl
 * outputs : none
 */
void UArray2b_map(T array2b, void apply(int col, int row, T array2b, 
								        void *elem, void *cl), 
				             void *cl)
{
	assert(array2b != NULL);
	int blocksize = UArray2b_blocksize(array2b);
	int block_width = array2b -> arr_width / blocksize;
	int array_length = array2b -> arr_width * array2b -> arr_height;

	for(int i = 0; i < array_length; i++)
	{
		int block_num = i / (blocksize * blocksize);
		int block_elem = i - (blocksize * blocksize * block_num);

		int row = (blocksize * (block_num / block_width) + (
			       block_elem / blocksize));
		int col = (blocksize * (block_num % block_width) + (
			       block_elem / blocksize));
		if(row >= UArray2b_height(array2b) || col >= UArray2b_width(array2b)){ 
			continue;
		}
		apply(col, row, array2b, UArray2b_at(array2b, col, row), cl);
	}

}
