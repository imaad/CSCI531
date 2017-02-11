//
// Author : Imaad Ukaye
// Course : CSCI-531
// DATE   : 01/29/2017
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stream.h"
#include <iostream>

//testing mode flag
bool testing = true;

// Prints a char string with \n in the end
void printString(unsigned char str[]) {
	int index = 0;
	while (str[index] != '\0') {
		printf("%c", str[index]);
		index++;
	}
	printf("\n");
}
// Print the two-row output for a given file
void printEncryptedStream(FILE* outputPointer, unsigned char output[][2],
		int width) {
	for (int index = 0; index < width; index++) {
		fprintf(outputPointer, "%c", output[index][0]);
	}
	for (int index = 0; index < width; index++) {
		fprintf(outputPointer, "%c", output[index][1]);
	}
}

// Generates file names for the encrypted files
void generateFileNames(char* outputFileName, char** outputFileOne,
		char** outputFileTwo) {

	strcpy(*outputFileOne, outputFileName);
	strcpy(*outputFileTwo, outputFileName);
	strcat(*outputFileOne, ".1.pbm");
	strcat(*outputFileTwo, ".2.pbm");
}

//Generates the encrypted data in the files
void generateDataFiles(FILE* filePointerOne, FILE* filePointerTwo,
		unsigned char* inputStreamBuffer, unsigned char* keyStreamBuffer,
		int width, int height) {

	// Variable declaration
	int inputBufferIndex = 0;
	int keyBufferIndex = 0;
	unsigned char inputMask = 0x80;
	unsigned char keyMask = 0x80;
	//Upper Limit Buffer Length
	int bufferLength = (2 * width + 7) / 8;
	unsigned int f1;
	unsigned int f2;
	unsigned char topRow[bufferLength][2];
	unsigned char bottomRow[bufferLength][2];
	unsigned char* inputBuffer;
	unsigned char* keyBuffer;
	inputBuffer = (unsigned char*) (inputStreamBuffer);
	keyBuffer = (unsigned char*) (keyStreamBuffer);

	// Prints file header
	fprintf(filePointerOne, "P4\n");
	fprintf(filePointerOne, "%d %d\n", width * 2, height * 2);
	fprintf(filePointerTwo, "P4\n");
	fprintf(filePointerTwo, "%d %d\n", width * 2, height * 2);
	// Looping through the input row-wise
	for (int heightIndex = 0; heightIndex < height; heightIndex++) {
		// Looping through each column
		int index = 0;
		for (int widthIndex = 0; widthIndex < (width + 7) / 8; widthIndex++) {

			// Reset the encrypted bits for new file
			// Top row
			f1 = 0x00000000;
			// Bottom row
			f2 = 0x00000000;

			// Iterate through each bit of every byte in the row
			unsigned int whiteMask = 0x4000; // 0100 0000 0000 0000
			unsigned int bigMask = 0x8000; // 1000 0000 0000 0000
			int bit = 0;
			// Encrypt Twos bit at a time (Two for top, two for bottom)
			while (bit < 8) {
				if ((widthIndex * 8 + bit) >= width) {
					if (!(bit ^ 4)) {
						inputBufferIndex++;
						inputMask = 0x80;
						break;
					}
				} else {
					// The following code checks for possible combinations of white/black and key 0,1 and sets the corresponding encrypted bits specified by mask table
					if ((unsigned int) ((inputBuffer[inputBufferIndex]
							& inputMask)) > 0) {
						// If Black pixel is identified
						if ((unsigned int) ((keyBuffer[keyBufferIndex] & keyMask))
								> 0) {
							// Black pixel, key=1
							f1 = ((((f1 & 0xffff0000) >> 16) | whiteMask) << 16)
									| f1;
							f1 = ((((f1 & 0x0000ffff)) | bigMask)) | f1;
							f2 = ((((f2 & 0xffff0000) >> 16) | bigMask) << 16)
									| f2;
							f2 = ((((f2 & 0x0000ffff)) | whiteMask)) | f2;
						} else {
							// Black pixel, key=0
							f1 = ((((f1 & 0xffff0000) >> 16) | bigMask) << 16)
									| f1;
							f1 = ((((f1 & 0x0000ffff)) | whiteMask)) | f1;

							f2 = ((((f2 & 0xffff0000) >> 16) | whiteMask) << 16)
									| f2;
							f2 = ((((f2 & 0x0000ffff)) | bigMask)) | f2;
						}
					} else {
						// White pixel is identified
						if ((unsigned int) ((keyBuffer[keyBufferIndex] & keyMask))
								> 0) {
							// White pixel, key=1
							f1 = ((((f1 & 0xffff0000) >> 16) | whiteMask) << 16)
									| f1;
							f1 = ((((f1 & 0x0000ffff)) | bigMask)) | f1;

							f2 = ((((f2 & 0xffff0000) >> 16) | whiteMask) << 16)
									| f2;
							f2 = ((((f2 & 0x0000ffff)) | bigMask)) | f2;

						} else {
							// White pixel, key=0
							f1 = ((((f1 & 0xffff0000) >> 16) | bigMask) << 16)
									| f1;
							f1 = ((((f1 & 0x0000ffff)) | whiteMask)) | f1;
							f2 = ((((f2 & 0xffff0000) >> 16) | bigMask) << 16)
									| f2;
							f2 = ((((f2 & 0x0000ffff)) | whiteMask)) | f2;

						}
					}
					// Shift key to use the next bit
					keyMask = keyMask >> 1;
					// if key is 0000 0000 0000 0000 then reset
					if (!(keyMask & 0xff)) {
						keyBufferIndex++;
						keyMask = 0x80;
					}
				}
				inputMask = inputMask >> 1;
				// if input is 0000 0000 0000 0000 then reset
				if (!(inputMask & 0xff)) {
					inputBufferIndex++;
					inputMask = 0x80;
				}
				//Shifting the mask by 2, to get the next 2x2 pixel block
				whiteMask = whiteMask >> 2;
				bigMask = bigMask >> 2;
				bit++;
			}
			unsigned char byteOne;
			unsigned char byteTwo;
			// Converting the 4 byte into individual bytes to be represented as rows
			byteOne = (f1 >> 24);
			byteTwo = (f1 >> 16);
			topRow[index][0] = byteOne;
			topRow[index + 1][0] = byteTwo;

			byteOne = ((f1 & 0x0000ffff) >> 8) & 0xff;
			byteTwo = ((f1 & 0x0000ffff)) & 0xff;
			topRow[index][1] = byteOne;
			topRow[index + 1][1] = byteTwo;

			byteOne = (f2 >> 24);
			byteTwo = (f2 >> 16);
			bottomRow[index][0] = byteOne;
			bottomRow[index + 1][0] = byteTwo;

			byteOne = ((f2 & 0x0000ffff) >> 8) & 0xff;
			byteTwo = ((f2 & 0x0000ffff)) & 0xff;
			bottomRow[index][1] = byteOne;
			bottomRow[index + 1][1] = byteTwo;

			index += 2;
		}
		// Print first row
		printEncryptedStream(filePointerOne, topRow, bufferLength);
		// Print Second row
		printEncryptedStream(filePointerTwo, bottomRow, bufferLength);
	}
}
void generateFileHeader(char *outputFileName, unsigned char *in_buf,
		unsigned char *key_buf, long width, long height) {
	FILE *fileOne, *fileTwo;

	// File Pointer for output files
	char* outputFileOne = new char[100];
	char* outputFileTwo = new char[100];

	generateFileNames(outputFileName, &outputFileOne, &outputFileTwo);
	// Open two new files for reading
	fileOne = fopen(outputFileOne, "w");
	fileTwo = fopen(outputFileTwo, "w");

	generateDataFiles(fileOne, fileTwo, in_buf, key_buf, width, height);

	// Free memory
	delete[] outputFileOne;
	delete[] outputFileTwo;
	fclose(fileOne);
	fclose(fileTwo);
}
void encrypt(char *p, char *outputFileName, FILE *inputFile, int msg_type) {

	char fileHeader[2];
	long width;
	long height;
	char *tempBuffer = new char[100];
	unsigned char *keyBuffer;
	unsigned char *inputBuffer;

	// Validating header
	fgets(fileHeader, 3, inputFile);
	//printf("%s", fileHeader);
	if (0 != strcmp(fileHeader, "P4")) {
		// Incorrect magic number
		fprintf(stderr, "Invalid PBM file\n");
		return;
	}
	// Remove \n
	fgets(tempBuffer, 2, inputFile);

	// Read height/width values from PBM file
	fgets(tempBuffer, 10, inputFile);
	// Extract width and height from the same line using strtol
	width = (int) strtol(tempBuffer, &tempBuffer, 10);
	height = (int) strtol(tempBuffer, NULL, 10);
	// calculating input buffer size
	long inputbufferSize =
			(width % 8 == 0) ?
					(height * width / 8) : (height * (width / 8 + 1));
	// Read the actual data
	if (width % 8 == 0) {
		inputBuffer = new unsigned char[inputbufferSize];
		fread(inputBuffer, 1, (inputbufferSize), inputFile);
	} else {
		inputBuffer = new unsigned char[inputbufferSize];
		fread(inputBuffer, 1, inputbufferSize, inputFile);
	}

	keyBuffer = new unsigned char[inputbufferSize];
	// Generate a stream derieved from p to encrypt
	stream(p, inputbufferSize, msg_type, keyBuffer);

	// Open two files for input
	generateFileHeader(outputFileName, inputBuffer, keyBuffer, width, height);

}
