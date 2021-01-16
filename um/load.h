/*
 *   filename:load.h
 *   Written by: Joy Kimambo and Zoe Hseih
 * 
 *   Summary:Loads UM program into 0 segement of memory and 
 *                 executes all instructions
 * 
 *   Notes: Makes use of the Memory, Bitpack, and Instructions modules
 *          to complete purpose
 */

#ifndef LOAD_H
#define LOAD_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "bitpack.h"
#include "memory.h"
#include "instructions.h"

extern void load_umfile(FILE *fp, int file_size);

#endif