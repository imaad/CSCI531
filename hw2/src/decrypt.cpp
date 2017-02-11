//
// Author : Imaad Ukaye
// Course : CSCI-531
// DATE   : 01/29/2017
//

#include <string.h>
#include <stdlib.h>
#include "decrypt.h"

// generates decrypted data from the input file
void decryptDataFile(FILE *inputFilePointer, int width, int height) {

	// Variable declaration initialization
	int BUFFER_LENGTH = (width + 7) / 8;
	unsigned char upperRow[BUFFER_LENGTH];
	unsigned char lowerRow[BUFFER_LENGTH];
	unsigned char decryptionMask;
	unsigned char outputMask = 0x0000;
	unsigned char currentChar = 0x0000;

	// Iterate through all rows and columns
	int heightIndex = 0;
	while (heightIndex < height) {
		// Read upper and lower rows from the input
		fread(upperRow, 1, BUFFER_LENGTH, inputFilePointer);
		fread(lowerRow, 1, BUFFER_LENGTH, inputFilePointer);

		for (int widthIndex = 0; widthIndex < BUFFER_LENGTH; widthIndex++) {
			if (widthIndex & 1) {
				outputMask = 0x0008; // 0000 0000 0000 1000
			} else {
				currentChar = 0x00;
				outputMask = 0x0080; // 0000 0000 1000 0000
			}

			// Read the first byte from the input
			decryptionMask = 0x00c0; 		//	0000 0000 1100 0000
			int bitIndex = 0;
			// 2 bits 4 times to read the whole byte
			while (bitIndex < 4) {
				//printf("%d\n",k);
				// Checks if both current bits from both lines are black
				if ((decryptionMask == (upperRow[widthIndex] & decryptionMask))
						&& ((decryptionMask
								== (lowerRow[widthIndex] & decryptionMask)))) {
					// decode it as black pixel
					currentChar = (outputMask >> bitIndex) | currentChar;
				}
				decryptionMask = decryptionMask >> 2;
				bitIndex++;
			}
			// If its not black pixel, it is white
			int widthLimit = (widthIndex + 1) * 8 + 4;
			if ((widthIndex & 1) || widthLimit >= width) {
				printf("%c", currentChar);
			}
		}
		heightIndex = heightIndex + 2;
	}

}

// Decrypts  merged PBM file
void decrypt(FILE *inputFilePointer, int msg_type) {

	int width;
	int height;
	int base10 = 10;
	char *temp_buf = (char*) malloc(10);
	char *fileheader = (char*) malloc(4);

	// Header validation
	fgets(fileheader, 3, inputFilePointer);
	if (strcmp(fileheader, "P4") != 0) {
		// Incorrect header
		fprintf(stderr, "ERROR: invalid PBM file for input.\n");
		exit(0);
	}
	// skip \n
	fgets(fileheader, 2, inputFilePointer);

	// Read height/width values from  file
	fgets(temp_buf, 10, inputFilePointer);
	// extract width/height using strtol
	width = (int) strtol(temp_buf, &temp_buf, base10);
	height = (int) strtol(temp_buf, NULL, base10);

	// if either width or height is odd, throw error
	if ((width & 1) || (height & 1)) {
		fprintf(stderr, "Invalid Width/Height in the header");
		exit(0);
	}

	// Create a new file and write header
	printf("P4\n");
	int newWidth = width / 2;
	int newHeight = height / 2;
	printf("%d %d\n", newWidth, newHeight);

	// decrypt and generate the data of the decrypted message
	decryptDataFile(inputFilePointer, width, height);
}
