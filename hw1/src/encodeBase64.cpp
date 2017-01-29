//
// Author : Imaad Ukaye
// Course : CSCI-531
// DATE   : 01/19/2017
//
#include <stdio.h>
#include <string.h>
#include "encodeBase64.h"

int charCount = 0;
void printEncoding(unsigned char encodingOutputBuffer[], int size) {
	char NEWLINE[] = "\n";
	int index = 0;
	while (index < size) {
		printf("%c", encodingOutputBuffer[index]);
		charCount++;
		if (charCount > 63) {
			printf(NEWLINE);
			charCount = 0;
		}
		index++;
	}
}
void encodeBase64(FILE *inputStreamPointer, int msg_type) {
	char encodingTable[] =
			"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	char CHAR_EQUAL = '=';
	char NEWLINE[] = "\n";
	int bytes_read = 0;
	int MAX_INPUT_BUFFER_SIZE = 1024;
	int INPUT_BUFFER_SIZE = 3;
	int OUTPUT_BUFFER_SIZE = 4;

	unsigned char temporaryInputBuffer[MAX_INPUT_BUFFER_SIZE];
	unsigned char encodingInputBuffer[INPUT_BUFFER_SIZE];
	unsigned char encodingOutputBuffer[OUTPUT_BUFFER_SIZE];

	memset(encodingInputBuffer, 0, INPUT_BUFFER_SIZE);
	memset(encodingOutputBuffer, 0, OUTPUT_BUFFER_SIZE);
	memset(temporaryInputBuffer, 0, MAX_INPUT_BUFFER_SIZE);

	bytes_read = fread(temporaryInputBuffer, 1, INPUT_BUFFER_SIZE,
			inputStreamPointer);

	while (bytes_read > 0) {
		int copyIndex = 0;
		while (copyIndex < INPUT_BUFFER_SIZE) {
			encodingInputBuffer[copyIndex] = temporaryInputBuffer[copyIndex];
			copyIndex++;
		}

		int i1 = encodingInputBuffer[0];
		int i2 = encodingInputBuffer[1];
		int i3 = encodingInputBuffer[2];

		encodingOutputBuffer[0] = encodingTable[i1 >> 2];
		int tempByteOne = ((i1 & 0x03) << 4);
		int tempByteTwo = ((i2 & 0xf0) >> 4);
		encodingOutputBuffer[1] = encodingTable[tempByteOne | tempByteTwo];
		if (bytes_read > 1) {
			tempByteOne = ((i2 & 0x0f) << 2);
			tempByteTwo = ((i3 & 0xc0) >> 6);
			encodingOutputBuffer[2] = encodingTable[tempByteOne | tempByteTwo];
		} else {
			encodingOutputBuffer[2] = CHAR_EQUAL;
		}
		if (bytes_read > 2) {
			encodingOutputBuffer[3] = encodingTable[i3 & 0x3f];
		} else {
			encodingOutputBuffer[3] = CHAR_EQUAL;
		}

		printEncoding(encodingOutputBuffer, OUTPUT_BUFFER_SIZE);

		memset(temporaryInputBuffer, 0, INPUT_BUFFER_SIZE);
		memset(encodingInputBuffer, 0, INPUT_BUFFER_SIZE);
		memset(encodingOutputBuffer, 0, OUTPUT_BUFFER_SIZE);

		bytes_read = fread(temporaryInputBuffer, 1, INPUT_BUFFER_SIZE,
				inputStreamPointer);
	}

	if (charCount != 0) {
		printf(NEWLINE);
	}

}
