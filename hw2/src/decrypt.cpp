//
// Author : Imaad Ukaye
// Course : CSCI-531
// DATE   : 01/29/2017
//

#include <string.h>
#include <stdlib.h>
#include "decrypt.h"

// Read in a file, print out decrypted bit values
void decrypt_file(FILE *inputFilePointer, int width, int height) {

	int BUFFER_LENGTH = (width + 7) / 8;
	unsigned char upperRow[BUFFER_LENGTH];
	unsigned char lowerRow[BUFFER_LENGTH];
	unsigned char mask;
	unsigned char out_mask = 0x00;

	unsigned char currentChar = 0x0000;

	// Iterate through all rows and columns
	int heightIndex = 0;
	while (heightIndex < height) {
		// Read two rows from the input
		fread(upperRow, 1, BUFFER_LENGTH, inputFilePointer);
		fread(lowerRow, 1, BUFFER_LENGTH, inputFilePointer);

		for (int widthIndex = 0; widthIndex < BUFFER_LENGTH; widthIndex++) {
			if (widthIndex & 1) {
				out_mask = 0x0008; // 0000 0000 0000 1000
			} else {
				currentChar = 0x00;
				out_mask = 0x0080;
			}

			// Read the first byte from the row, 2 bits at a time
			mask = 0x00c0; 		//	0000 0000 1100 0000
			int bitIndex = 0;
			while (bitIndex < 4) {
				//printf("%d\n",k);
				// Checks if both current bits from both lines are black
				if ((mask == (upperRow[widthIndex] & mask))
						&& ((mask == (lowerRow[widthIndex] & mask)))) {
					// Black pixel
					currentChar = (out_mask >> bitIndex) | currentChar;
				}
				mask = mask >> 2;
				bitIndex++;
			}
			int widthLimit = (widthIndex + 1) * 8 + 4;
			if ((widthIndex & 1) || widthLimit >= width) {
				printf("%c", currentChar);
			}
		}
		heightIndex = heightIndex + 2;
	}

}

// Decrypt a merged input PBM file, returning new PBM file
void decrypt(FILE *inputFilePointer, int msg_type) {

	int width;
	int height;
	int base10 = 10;
	char *temp_buf = (char*) malloc(10);
	char *fileheader = (char*) malloc(4);

	fgets(fileheader, 3, inputFilePointer);
	if (strcmp(fileheader, "P4") != 0) {
		// Incorrect magic number
		fprintf(stderr, "ERROR: invalid PBM file for input.\n");
		exit(0);
	}
	fgets(fileheader, 2, inputFilePointer);

	// Read height/width values from PBM file
	fgets(temp_buf, 10, inputFilePointer);
	width = (int) strtol(temp_buf, &temp_buf, base10);
	height = (int) strtol(temp_buf, NULL, base10);

	if ((width & 1) || (height & 1)) {
		fprintf(stderr, "Invalid Width/Height in the header");
		exit(0);
	}

	// Create a new file
	printf("P4\n");
	int newWidth = width / 2;
	int newHeight = height / 2;
	printf("%d %d\n", newWidth, newHeight);

	decrypt_file(inputFilePointer, width, height);

	// PSEUDO-CODE
	// - Read in the file and verify its header
	// - Print PBM file header
	// - Repeat through all the lines:
	//   - Read two full lines of bytes
	//   - Take one byte from both lines
	//   - Use mask 0xC0 shifted appropriately to determine
	//     if there are two black bits on each line
	//   - If so, flip a bit in out_val to 1 in correct spot
	//   - Repeat until line complete, then print char out_val
}
