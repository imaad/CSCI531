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

bool testing = true;

void printString(unsigned char str[]) {
	int i = 0;
	while (str[i] != '\0') {
		printf("%c", str[i]);
		i++;
	}
	printf("\n");
}
// Print the two-row output for a given file
void print_rowout(FILE* out, unsigned char row_out[][2], int width) {
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < width; j++) {
			fprintf(out, "%c", row_out[j][i]);
		}
	}
}

// Open two new PBM files for writing and begin encryption
void generateFileNames(char* outputFileName, char** outputFileOne,
		char** outputFileTwo) {

	strcpy(*outputFileOne, outputFileName);
	strcpy(*outputFileTwo, outputFileName);
	strcat(*outputFileOne, ".1.pbm");
	strcat(*outputFileTwo, ".2.pbm");
}
void splitFiles(FILE* fout1, FILE* fout2, unsigned char* in_buf,
		unsigned char* key_buf, int width, int height) {
	unsigned char inputMask = 0x80;
	unsigned char keyMask = 0x80;
	unsigned char* inputBuffer;
	unsigned char* keyBuffer;
	int inputBufferIndex = 0;
	int keyBufferIndex = 0;

	unsigned int f1;
	unsigned int f2;
	unsigned char row_out1[(2 * width + 7) / 8][2]; // Function computes ceiling (2*width)/8 in bytes
	unsigned char row_out2[(2 * width + 7) / 8][2];
	// Iterate through each row of the PBM file
	inputBuffer = (unsigned char*) (in_buf);
	keyBuffer = (unsigned char*) (key_buf);
	fprintf(fout1, "P4\n");
	fprintf(fout1, "%d %d\n", width * 2, height * 2);
	fprintf(fout2, "P4\n");
	fprintf(fout2, "%d %d\n", width * 2, height * 2);
	//printf("INP: ");
	//printString(curr_in);
	//printf("KEY: ");
	//printString(curr_key);
	for (int heightIndex = 0; heightIndex < height; heightIndex++) {
		// Iterate through each column, byte-wise
		int index = 0;
		for (int widthIndex = 0; widthIndex < (width + 7) / 8; widthIndex++) {

			// Reset the encrypted bits for new file
			f1 = 0x00000000;
			f2 = 0x00000000;


			// Iterate through each bit of every byte in the row
			unsigned int whiteMask = 0x4000; // 0100 0000 0000 0000
			unsigned int bigMask = 0x8000; // 1000 0000 0000 0000
			for (int k = 0; k < 8; k++) {
				if ((widthIndex * 8 + k) >= width) {
					if (k == 4) {
						// Printing don't cares from the start, just skip this byte
						inputBufferIndex++;
						inputMask = 0x80;
						break;
					}
				} else {
					// Identify whether the current bit is a 1 or 0
					if ((unsigned int) ((inputBuffer[inputBufferIndex]
							& inputMask)) > 0) {
						// Black pixel
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
						// White pixel
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
					// Update key mask
					keyMask = keyMask >> 1;
					if (!(keyMask & 0xff)) {
						keyBufferIndex++;
						keyMask = 0x80;
					}
				}
				inputMask = inputMask >> 1;
				if (!(inputMask & 0xff)) {
					inputBufferIndex++;
					inputMask = 0x80;
				}
				whiteMask = whiteMask >> 2;
				bigMask = bigMask >> 2;
			}
			unsigned char byteOne;

			unsigned char byteTwo;
			byteOne = (f1 >> 24);
			byteTwo = (f1 >> 16);
			// Save char values from the shorts in correct order, based on endianness
			row_out1[index][0] = byteOne;
			row_out1[index + 1][0] = byteTwo;

			byteOne = ((f1 & 0x0000ffff) >> 8) & 0xff;
			byteTwo = ((f1 & 0x0000ffff)) & 0xff;
			row_out1[index][1] = byteOne;
			row_out1[index + 1][1] = byteTwo;

			byteOne = (f2 >> 24);
			byteTwo = (f2 >> 16);
			row_out2[index][0] = byteOne;
			row_out2[index + 1][0] = byteTwo;

			byteOne = ((f2 & 0x0000ffff) >> 8) & 0xff;
			byteTwo = ((f2 & 0x0000ffff)) & 0xff;
			row_out2[index][1] = byteOne;
			row_out2[index + 1][1] = byteTwo;

			index += 2;
		}
		// Output the two rows to each file
		print_rowout(fout1, row_out1, (2 * width + 7) / 8);
		print_rowout(fout2, row_out2, (2 * width + 7) / 8);
	}
}
void open_files(char *outputFileName, unsigned char *in_buf,
		unsigned char *key_buf, long width, long height) {
	FILE *fileOne, *fileTwo;

	// Generate two filenames
	char* outputFileOne = new char[100];		// Arbitrary filename length
	char* outputFileTwo = new char[100];

	generateFileNames(outputFileName, &outputFileOne, &outputFileTwo);
	// Open two new files for reading
	fileOne = fopen(outputFileOne, "w");
	fileTwo = fopen(outputFileTwo, "w");

	splitFiles(fileOne, fileTwo, in_buf, key_buf, width, height);

	// We're done here, close up shop
	delete[] outputFileOne;
	delete[] outputFileTwo;
	fclose(fileOne);
	fclose(fileTwo);
}
void encrypt(char *pphrase, char *outputFileName, FILE *pnbFile, int msg_type) {

	char fileHeader[2];
	long width;
	long height;
	char *temp_buf = new char[100];
	unsigned char *key_buf;
	unsigned char *in_buf;

	// Make sure the magic number (P4) of the PBM file is correct
	fgets(fileHeader, 3, pnbFile);
	//printf("%s", fileHeader);
	if (0 != strcmp(fileHeader, "P4")) {
		// Incorrect magic number
		fprintf(stderr, "ERROR: invalid PBM file for input.\n");
		return;
	}
	// Remove \n
	fgets(temp_buf, 2, pnbFile);

	// Read height/width values from PBM file
	fgets(temp_buf, 10, pnbFile);
	width = (int) strtol(temp_buf, &temp_buf, 10);
	height = (int) strtol(temp_buf, NULL, 10);
	long inputbufferSize =
			(width % 8 == 0) ?
					(height * width / 8) : (height * (width / 8 + 1));
	// Read all data from input file
	if (width % 8 == 0) {
		in_buf = new unsigned char[inputbufferSize];
		fread(in_buf, 1, (inputbufferSize), pnbFile);
	} else {
		in_buf = new unsigned char[inputbufferSize];
		fread(in_buf, 1, inputbufferSize, pnbFile);
	}

	// Read stream cipher
	key_buf = new unsigned char[inputbufferSize];
	stream(pphrase, inputbufferSize, msg_type, key_buf);

	// Open two files for input
	open_files(outputFileName, in_buf, key_buf, width, height);

// Free up the big memory buffers used

}
