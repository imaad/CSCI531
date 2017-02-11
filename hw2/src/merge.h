//
// Author : Imaad Ukaye
// Course : CSCI-531
// DATE   : 01/2 9/2017
//
#ifndef MERGE_H
#define MERGE_H

#include <stdio.h>
#include <string.h>

// Take in 2 encrypted PBM files, bitwise OR them, write to fout
void combineInputs(FILE *fin1, FILE *fin2, int width, int height);

// Merge 2 PBM files and create an output file
void merge(FILE *in1, FILE *in2, int msg_type);

void throwMergeError(char str[]);

#endif
