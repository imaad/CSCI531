//
// Author : Imaad Ukaye
// Course : CSCI-531
// DATE   : 01/2 9/2017
//
#ifndef MERGE_H
#define MERGE_H

#include <stdio.h>
#include <string.h>

// Combines two PBM files and write to stdout
void combineInputs(FILE *filePointerOne, FILE *filePointerTwo, int width, int height);
// Merge 2 PBM files into a single output file
void merge(FILE *filePointerOne, FILE *filePointerTwo, int msg_type);
// Generates an error when unexpected inputs arrive
void throwMergeError(char str[]);

#endif
