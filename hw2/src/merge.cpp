//
// Author : Imaad Ukaye
// Course : CSCI-531
// DATE   : 01/2 9/2017
//
#include <string.h>
#include <stdlib.h>
#include<iostream>
#include "merge.h"

// Take in 2 encrypted PBM files, bitwise OR them, write to fout

void throwMergeError(char str[]) {
	fprintf(stderr, str);
	exit(1);
}
void combineInputs(FILE *filePointerOne, FILE *filePointerTwo, int width,
		int height) {
	unsigned char *val1 = new unsigned char[2];
	unsigned char *val2 = new unsigned char[2];

	//  Headers Information
	printf("P4\n");
	printf("%d %d\n", width, height);

	// Iterate through all rows and columns
	int i = 0;
	while (i < height * ((width + 7) / 8)) {
		// Read one byte from each file
		fread(val1, 1, 1, filePointerOne);
		fread(val2, 1, 1, filePointerTwo);
		printf("%c", val1[0] | val2[0]);
		i++;
	}
	delete (val1);
	delete (val2);
}

// Merge 2 PBM files and create an output file
void merge(FILE *in1, FILE *in2, int msg_type) {

	int width1;
	int height1;
	int width2;
	int height2;
	int baseInt = 10;
	char *temp_buf = new char[15];
	char *fileHeader = new char[3];

	// Make sure the magic number (P4) of both PBM files is correct
	fgets(fileHeader, 3, in1);
	if (strcmp(fileHeader, "P4") != 0) {
		// Invalid Header
		throwMergeError((char*) "File1 : Invalid Header Information\n");
	}
	// remove \n
	fgets(fileHeader, 2, in1);

	fgets(fileHeader, 3, in2);
	if (strcmp(fileHeader, "P4") != 0) {
		// Invalid Header
		throwMergeError((char*) "File2 : Invalid Header Information\n");
	}
	// remove \n
	fgets(fileHeader, 2, in2);

	// Read height/width values from PBM file
	fgets(temp_buf, 15, in1);
	width1 = (int) strtol(temp_buf, &temp_buf, baseInt);
	height1 = (int) strtol(temp_buf, NULL, baseInt);
	fgets(temp_buf, 15, in2);
	width2 = (int) strtol(temp_buf, &temp_buf, baseInt);
	height2 = (int) strtol(temp_buf, NULL, baseInt);
	if (width1 != width2 || height1 != height2) {
		// Height/width don't match
		throwMergeError((char*) "Height or Width in the files do not match");
	}

	// Merge all the bits together and write to fout
	combineInputs(in1, in2, width1, height1);

}
