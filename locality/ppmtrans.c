/* 
 * ppmtrans.c
 * Jun Sun Tak, Zoe Hsieh, 2/20/20
 * hw3 : locality
 * 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "pnm.h"
#include "cputiming.h"


#define SET_METHODS(METHODS, MAP, WHAT) do {                    \
        methods = (METHODS);                                    \
        assert(methods != NULL);                                \
        map = methods->MAP;                                     \
        if (map == NULL) {                                      \
                fprintf(stderr, "%s does not support "          \
                                WHAT "mapping\n",               \
                                argv[0]);                       \
                exit(1);                                        \
        }                                                       \
} while (0)

static void
usage(const char *progname)
{
        fprintf(stderr, "Usage: %s [-rotate <angle>] "
                        "[-{row,col,block}-major] [filename]\n",
                        progname);
        exit(1);
}

FILE *open_file(int, char *argv[], int);
Pnm_ppm transform(int, A2Methods_T, Pnm_ppm, A2Methods_mapfun *, char *);
Pnm_ppm landscape_orientation(int rotation, Pnm_ppm pix_arr, 
                              A2Methods_UArray2 arr, A2Methods_T methods, 
                              A2Methods_mapfun *map, char *);
Pnm_ppm portrait_orientation(int rotation, Pnm_ppm pix_arr, 
                             A2Methods_UArray2 arr, A2Methods_T methods, 
                             A2Methods_mapfun *map, char *);
void rotate_90(int i, int j, A2Methods_UArray2 arr, A2Methods_Object *ptr, 
               void *cl);
void rotate_180(int i, int j, A2Methods_UArray2 arr, A2Methods_Object *ptr, 
                void *cl);
void rotate_270(int i, int j, A2Methods_UArray2 arr, A2Methods_Object *ptr,
                void *cl);
void flip_hori(int i, int j, A2Methods_UArray2 arr, A2Methods_Object *ptr, 
               void *cl);
void flip_vert(int i, int j, A2Methods_UArray2 arr, A2Methods_Object *ptr, 
               void *cl);
void flip_transpose(int i, int j, A2Methods_UArray2 arr, A2Methods_Object *ptr,
                    void *cl);



int main(int argc, char *argv[]) 
{
        char *time_file_name = NULL; (void)time_file_name;
        int   rotation       = 0;
        
        int   i;

        /* default to UArray2 methods */
        A2Methods_T methods = uarray2_methods_plain; 
        assert(methods);

        /* default to best map */
        A2Methods_mapfun *map = methods->map_default; 
        assert(map);

        for (i = 1; i < argc; i++) {
                if (strcmp(argv[i], "-row-major") == 0) {
                        SET_METHODS(uarray2_methods_plain, map_row_major, 
                    "row-major");
                } else if (strcmp(argv[i], "-col-major") == 0) {
                        SET_METHODS(uarray2_methods_plain, map_col_major, 
                    "column-major");
                } else if (strcmp(argv[i], "-block-major") == 0) {
                        SET_METHODS(uarray2_methods_blocked, map_block_major,
                                    "block-major");
                } else if (strcmp(argv[i], "-rotate") == 0) {
                        if (!(i + 1 < argc)) {      /* no rotate value */
                                usage(argv[0]);
                        }
                        char *endptr;
                        rotation = strtol(argv[++i], &endptr, 10);
                        if (!(rotation == 0 || rotation == 90 ||
                            rotation == 180 || rotation == 270 ||
                            rotation == 1 || rotation == 2 || rotation == 3)) {
                                fprintf(stderr, 
                    "Rotation must be 0, 90 180 or 270\n");
                                usage(argv[0]);
                        }
                        if (!(*endptr == '\0')) {    /* Not a number */
                                usage(argv[0]);
                        }
                } else if (strcmp(argv[i], "-time") == 0) {
                        time_file_name = argv[++i];
                } else if (strcmp(argv[i], "-flip") == 0) {
                        if(!(i + 1 < argc )){
                            usage(argv[0]);
                        }
                        if(strcmp(argv[++i], "horizontal") == 0){
                            rotation = 1;
                        } else if(stcmp(argv[i], "vertical") == 0) {
                            rotation = 2;
                        }
                        // IMPLEMENT ERROR BLA BLA BLA
                } else if (strcmp(argv[i], "-transpose") == 0) {
                        rotation = 3;
                } else if (*argv[i] == '-') {
                        fprintf(stderr, "%s: unknown option '%s'\n", argv[0],
                argv[i]);
                } else if (argc - i > 1) {
                        fprintf(stderr, "Too many arguments\n");
                        usage(argv[0]);
                } else {
                        break;
                }
        }

        printf("timefile name: %s\n", time_file_name);
        FILE *fp = open_file(argc, argv, i);
        assert(fp);
        Pnm_ppm pix_arr = Pnm_ppmread(fp, methods); (void)pix_arr;
        pix_arr = transform(rotation, methods, pix_arr, map, time_file_name);
        
        fclose(fp);
        Pnm_ppmfree(&pix_arr);
}


/* 
 * name    : open_file
 * purpose : opens file, and returns pointer to the file
 * inputs  : int argc, char *argv[], int i
 * outputs : FILE pointer
 */
FILE *open_file(int argc, char *argv[], int i)
{
    FILE *fp = stdin;
    if(argc > i){
        fp = fopen(argv[i], "r");
        assert(fp != NULL);
    }
    return fp;

}


/* 
 * name    : transform
 * purpose : calls functions to be ready to be transformed
 * inputs  : int rotation, A2Methods_T methods, Pnm_ppm pix_arr, 
 *           A2Methods_mapfun *map, char *time_file
 * outputs : Pnm_ppm pix_arr
 */
Pnm_ppm transform(int rotation, A2Methods_T methods, Pnm_ppm pix_arr, 
                  A2Methods_mapfun *map, char *time_file)
{
    A2Methods_UArray2 arr = pix_arr -> pixels; 
    
    if(rotation == 0){
        return pix_arr;
    } 
    else if(rotation == 90 || rotation == 270) {
        
        return landscape_orientation(rotation, pix_arr, arr, methods, 
                                     map, time_file);
    } 
    else if(rotation == 180 || rotation == 1 || 
            rotation == 2 || rotation == 3) {

        return portrait_orientation(rotation, pix_arr, arr, methods, map, 
                                    time_file);
    }

    return pix_arr;
}


/* 
 * name    : portrait_orientation
 * purpose : calls the specific transformation, and starts the map and timer
 * inputs  : int rotation, Pnm_ppm pix_arr, 
             A2Methods_UArray2 arr, A2Methods_T methods, 
             A2Methods_mapfun *map, char *time_file
 * outputs : Pnm_ppm pix_arr
 */
Pnm_ppm portrait_orientation(int rotation, Pnm_ppm pix_arr, 
                             A2Methods_UArray2 arr, A2Methods_T methods, 
                             A2Methods_mapfun *map, char *time_file)
{

    int width = methods -> width(arr);
    int height = methods -> height(arr);
    int size = methods -> size(arr);
    int blocksize = methods -> blocksize(arr);
    double time_elapsed = 0.0;
    pix_arr -> pixels = methods -> new_with_blocksize(width, height, 
                                                      size, blocksize);
    CPUTime_T timer = NULL;

    if (time_file != NULL){
        CPUTime_T timer = CPUTime_New();
        
        if(rotation == 180){
            CPUTime_Start(timer);
            map(arr, rotate_180, pix_arr);
            time_elapsed = CPUTime_Stop(timer);
        } 
        else if(rotation == 1) {
            CPUTime_Start(timer);
            map(arr, flip_hori, pix_arr);
            time_elapsed = CPUTime_Stop(timer);
        } 
        else if(rotation == 2) {
            CPUTime_Start(timer);
            map(arr, flip_vert, pix_arr);
            time_elapsed = CPUTime_Stop(timer);
        } 
        else if(rotation == 3){
            printf("a\n");
            CPUTime_Start(timer);
            map(arr, flip_transpose, pix_arr);
            time_elapsed = CPUTime_Stop(timer);
        }

    } else {
        if(rotation == 180){
            map(arr, rotate_180, pix_arr);
        } 
        else if(rotation == 1) {
            map(arr, flip_hori, pix_arr);
        } 
        else if(rotation == 2) {
            map(arr, flip_vert, pix_arr);
        } 
        else if(rotation == 3){
            map(arr, flip_transpose, pix_arr);
        }
    }

    if(timer == NULL){
        printf("Time elapsed: %lf\n", time_elapsed);
    }

    methods -> free(&arr);
    return pix_arr;
}


/* 
 * name    : landscape_orientation
 * purpose : calls the specific transformation, and starts the map and timer
 * inputs  : int rotation, Pnm_ppm pix_arr, A2Methods_UArray2 arr, 
 *           A2Methods_T methods, A2Methods_mapfun *map, char *time_file
 * outputs : Pnm_ppm pix_arr
 */
Pnm_ppm landscape_orientation(int rotation, Pnm_ppm pix_arr, 
                              A2Methods_UArray2 arr, A2Methods_T methods, 
                              A2Methods_mapfun *map, char *time_file)
{
    (void)time_file;
    int width = methods -> height(arr);
    int height = methods -> width(arr);
    int size = methods -> size(arr);
    int blocksize = methods -> blocksize(arr);

    pix_arr -> pixels = methods -> new_with_blocksize(width, height, size, 
                                                      blocksize);

    if(rotation == 90){
        map(arr, rotate_90, pix_arr);
    } else if (rotation == 270) {
        map(arr, rotate_270, pix_arr);
    }
    int temp = pix_arr -> width;
    pix_arr -> width = pix_arr -> height;
    pix_arr -> height = temp;

    methods -> free(&arr);
    return pix_arr;
}


/* 
 * name    : rotate_90
 * purpose : rotates the given image 90 degrees clockwise
 * inputs  : int i, int j, A2Methods_UArray2 arr, A2Methods_Object *ptr, 
 *           void *cl
 * outputs : none
 */
void rotate_90(int i, int j, A2Methods_UArray2 arr, A2Methods_Object *ptr, 
               void *cl)
{
    (void)arr;
    Pnm_ppm pix_arr = (Pnm_ppm)cl;
    int row = i;
    int col = pix_arr -> height - j - 1;
    *(Pnm_rgb)(pix_arr -> methods -> at(pix_arr -> 
                                        pixels, col, row)) = *(Pnm_rgb)ptr;
}


/* 
 * name    : rotate_180
 * purpose : rotates the given image 180 degrees 
 * inputs  : int i, int j, A2Methods_UArray2 arr, A2Methods_Object *ptr, 
 *           void *cl
 * outputs : none
 */
void rotate_180(int i, int j, A2Methods_UArray2 arr, A2Methods_Object *ptr, 
                void *cl)
{
    (void)arr;
    Pnm_ppm pix_arr = (Pnm_ppm)cl;
    int row = pix_arr -> height - j - 1;
    int col = pix_arr -> width - i - 1;
    *(Pnm_rgb)(pix_arr -> methods -> at(pix_arr -> 
                                        pixels, col, row)) = *(Pnm_rgb)ptr;
}


/* 
 * name    : rotate_270
 * purpose : rotates the given image 90 degrees counter clockwise (aka 270 degrees)
 * inputs  : int i, int j, A2Methods_UArray2 arr, A2Methods_Object *ptr, 
 *           void *cl
 * outputs : none
 */
void rotate_270(int i, int j, A2Methods_UArray2 arr, A2Methods_Object *ptr, 
                void *cl)
{   
    (void)arr;
    Pnm_ppm pix_arr = (Pnm_ppm)cl;
    int row = pix_arr -> width - i - 1;
    int col = j;
    *(Pnm_rgb)(pix_arr -> methods -> at(pix_arr -> 
                                        pixels, col, row)) = *(Pnm_rgb)ptr;
    
}


/* 
 * name    : flip_hori
 * purpose : flips the image horizontally 
 * inputs  : int i, int j, A2Methods_UArray2 arr, A2Methods_Object *ptr, 
 *           void *cl
 * outputs : none
 */
void flip_hori(int i, int j, A2Methods_UArray2 arr, A2Methods_Object *ptr, 
     void *cl)
{
    (void)arr; (void)j;
    Pnm_ppm pix_arr = (Pnm_ppm)cl;
    int col = pix_arr -> width - i - 1;
    *(Pnm_rgb)(pix_arr -> methods -> at(pix_arr -> 
                                        pixels, col, j)) = *(Pnm_rgb)ptr;
}


/* 
 * name    : flip_vert
 * purpose : flips the image vertically 
 * inputs  : int i, int j, A2Methods_UArray2 arr, A2Methods_Object *ptr, 
 *           void *cl
 * outputs : none
 */
void flip_vert(int i, int j, A2Methods_UArray2 arr, A2Methods_Object *ptr, 
               void *cl)
{
    (void)arr; (void)j;
    Pnm_ppm pix_arr = (Pnm_ppm)cl;
    int row = pix_arr -> height - j - 1;
    *(Pnm_rgb)(pix_arr -> methods -> at(pix_arr -> 
                                        pixels, i, row)) = *(Pnm_rgb)ptr;   
}


/* 
 * name    : flip_transpose
 * purpose : flips the image over the y=x line (transposed image)
 * inputs  : int i, int j, A2Methods_UArray2 arr, A2Methods_Object *ptr, 
 *           void *cl
 * outputs : none
 */
void flip_transpose(int i, int j, A2Methods_UArray2 arr, A2Methods_Object *ptr, 
                    void *cl)
{
    (void)arr;
    Pnm_ppm pix_arr = (Pnm_ppm)cl;
    int col = pix_arr -> width - i - 1;
    int row = pix_arr -> height - j - 1;
    *(Pnm_rgb)(pix_arr -> methods -> at(pix_arr -> 
                                        pixels, col, row)) = *(Pnm_rgb)ptr;
}

