//
// Author : Imaad Ukaye
// Course : CSCI-531
// DATE   : 01/2 9/2017
//
#include <string.h>
#include <stdlib.h>
#include<iostream>
#include "merge.h"

// Generates an error when unexpected inputs arrive
void throwMergeError(char str[]) {
	fprintf(stderr, str);
	exit(1);
}

// Combines two PBM files by using OR and writes to stdout
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
		// Read data from files
		fread(val1, 1, 1, filePointerOne);
		fread(val2, 1, 1, filePointerTwo);
		// print Output equals 'OR' of two input bytes
		printf("%c", val1[0] | val2[0]);
		i++;
	}
	// free space
	delete (val1);
	delete (val2);
}

// Merge 2 PBM files into a single output file
void merge(FILE *filePointerOne, FILE *filePointerTwo, int msg_type) {

	// Local Variable declaration
	int width1;
	int height1;
	int width2;
	int height2;
	int baseInt = 10;
	char *temp_buf = new char[15];
	char *fileHeader = new char[3];

	// Header Validation
	fgets(fileHeader, 3, filePointerOne);
	if (strcmp(fileHeader, "P4") != 0) {
		// Invalid Header
		throwMergeError((char*) "File1 : Invalid Header Information\n");
	}
	// remove \n
	fgets(fileHeader, 2, filePointerOne);

	fgets(fileHeader, 3, filePointerTwo);
	if (strcmp(fileHeader, "P4") != 0) {
		// Invalid Header
		throwMergeError((char*) "File2 : Invalid Header Information\n");
	}
	// remove \n
	fgets(fileHeader, 2, filePointerTwo);

	// Read height/width values from the files and validate
	fgets(temp_buf, 15, filePointerOne);
	width1 = (int) strtol(temp_buf, &temp_buf, baseInt);
	height1 = (int) strtol(temp_buf, NULL, baseInt);
	fgets(temp_buf, 15, filePointerTwo);
	width2 = (int) strtol(temp_buf, &temp_buf, baseInt);
	height2 = (int) strtol(temp_buf, NULL, baseInt);
	if (width1 != width2 || height1 != height2) {
		// Height/width don't match
		throwMergeError((char*) "Height or Width in the files do not match");
	}

	// OR both inputs to get the merged file
	combineInputs(filePointerOne, filePointerTwo, width1, height1);

}
