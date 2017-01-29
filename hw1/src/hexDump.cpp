//
// Author : Imaad Ukaye
// Course : CSCI-531
// DATE   : 01/19/2017
//
#include "hexDump.h"
#include "hexConstants.h"
#include "iostream"

// Prints a character stream of size 16 bytes on the screen
void printPlainText(unsigned char string[]) {
	int index = 0;
	while (index < 16) {
		printf("%c", string[index]);
		index++;
	}
	// adds \n after every plain text message
	printf(NEWLINE);
}

// Generates hexDump of the input provided
void hexdump(FILE *inputStreamPointer, int msg_type) {
	int counter = 0;

	//Buffer size definition
	int MAX_INPUT_BUFFER_SIZE = 1024;
	int INPUT_BUFFER_SIZE = 1;
	int OUTPUT_BUFFER_SIZE = 16;

	// Buffer definition
	unsigned char outputString[OUTPUT_BUFFER_SIZE];
	unsigned char tempBuffer[MAX_INPUT_BUFFER_SIZE];

	// Row count Index
	int rowIndex = 0;

	// Row flag definition Flags
	bool isFirstRow = false;
	bool isLastRow = false;

	// Clear all Buffer
	memset(outputString, 0, 16);
	memset(tempBuffer, 0, MAX_INPUT_BUFFER_SIZE);

	int read_bytes = fread(tempBuffer, 1, INPUT_BUFFER_SIZE,
			inputStreamPointer);
	while (read_bytes > 0) {

		// if row index is 0, set flag of 1st character of the row to true
		if (rowIndex == 0) {
			isFirstRow = true;
		}

		//if its the first character print address column
		if (isFirstRow) {

			printf(HEX_SIX_SPACE, counter);
			isFirstRow = false;
		}

		// Print hex equivalent of the current character
		printf(HEX_TWO_SPACE, tempBuffer[0]);

		//If the data byte is >=0x20 and <=0x7e, use the corresponding ASCII character
		if (tempBuffer[0] >= 0x20 && tempBuffer[0] <= 0x7e) {
			outputString[rowIndex] = tempBuffer[0];
		}
		//If the data byte is <=0x1f or equals to 0x7f, print a period character
		else if (tempBuffer[0] <= 0x1f || tempBuffer[0] == 0x7f) {
			outputString[rowIndex] = CHAR_DOT;
		}
		//If the data byte is >=0x80, print a ~ character.
		else if (tempBuffer[0] >= 0x80) {
			outputString[rowIndex] = CHAR_TILDE;
		}
		counter++;
		rowIndex++;
		// if the data byte is greater than 15 bytes, set isLastRow to true
		if (rowIndex > 15) {
			isLastRow = true;
		}

		// Spacing after 8 bytes
		if (rowIndex % 8 == 0)
			printf(BLANK_SPACE);

		// if its the last character i.e. 16th data byte, print the ASCII characters and a New Line
		if (isLastRow) {
			isLastRow = false;
			printPlainText(outputString);
			// clear the buffer with 0
			memset(outputString, 0, 16);
			//reset row index for the next row
			rowIndex = 0;
		}
		// read next Byte
		read_bytes = fread(tempBuffer, 1, 1, inputStreamPointer);
	}

	// if the last row has less than 16 bytes, print the HEX with HYPHEN and ASCII with BLANK SPACE
	if (rowIndex >= 1) {
		unsigned int index = rowIndex;
		while (index < 16) {
			printf(HYPHEN_SPACE);
			counter++;
			outputString[rowIndex] = BLANK_SPACE[0];
			rowIndex++;
			// Spacing after 8 bytes
			if (rowIndex % 8 == 0)
				printf(BLANK_SPACE);

			// Print ASCII for the last row
			if (rowIndex > 15) {
				rowIndex = 0;
				printPlainText(outputString);
				// reset the buffer
				memset(outputString, 0, 16);

			}
			index++;
		}
	}
}
