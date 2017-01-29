//
// Author : Imaad Ukaye
// Course : CSCI-531
// DATE   : 01/19/2017
//

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include "decodeBase64.h"

// Prints an error when invalid Characters are encountered
void throwError(char c) {
	fprintf(stderr,
			"Input contains invalid Base64 Character '%c'.\nValid Characters are A-Z,a-z,0-9,+,/ and possibly = or ==",
			c);
	exit(1);
}

// This method gets the Base64 integer equivalent of ASCII character and returns its index
int getBase64EncodingValue(char input) {
	// Base64 Character set
	char encodingTable[] =
			"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	int index = 0;
	while (index < 64) {
		if (input == encodingTable[index]) {
			break;
		}
		index++;
	}
	return index;
}
// Prints decoded characters
void printCharacters(unsigned char decodingOutputBuffer[], int length) {
	int index = 0;
	while (index < length) {
		printf("%c", decodingOutputBuffer[index]);

		index++;
	}

}
// Decodes base64 encoded message
void decodeBase64(FILE *in, int msg_type) {

	// Constants definition
	char NEWLINE[] = "\n";
	unsigned char EQUAL = '=';

	//Buffer size definition
	int MAX_INPUT_BUFFER_SIZE = 1024;
	int INPUT_BUFFER_SIZE = 4;
	int OUTPUT_BUFFER_SIZE = 3;

	// Buffer definition
	unsigned char tempBuffer[MAX_INPUT_BUFFER_SIZE];
	unsigned char decodingInputBuffer[INPUT_BUFFER_SIZE];
	unsigned char decodingOutputBuffer[OUTPUT_BUFFER_SIZE];

	// Initialize all Buffer to avoid garbage values
	memset(decodingInputBuffer, 0, INPUT_BUFFER_SIZE);
	memset(decodingOutputBuffer, 0, OUTPUT_BUFFER_SIZE);
	memset(tempBuffer, 0, MAX_INPUT_BUFFER_SIZE);

	// Logical flag defination
	bool isEndofString = false;

	unsigned int index = 0;
	// Reads the input Stream one character at a time
	while (fread(tempBuffer, 1, 1, in) > 0) {

		unsigned char c = tempBuffer[0];
		// if the character is a valid base64 character
		if ((c >= 0x61 && c <= 0x7a) || (c >= 0x41 && c <= 0x5a)
				|| (c >= 0x30 && c <= 0x39) || (c == 0x2b) || (c == 0x2f))

				{
			decodingInputBuffer[index] = getBase64EncodingValue(c);
		}
		// if new Line '\n' is encountered, we skip it.
		else if (c == NEWLINE[0]) {
			continue;

		}
		// If = is encountered, it implies we have reached end of the input and set flag to start decoding
		else if (c == EQUAL) {
			decodingInputBuffer[index] = 0;
			isEndofString = true;
		}
		// if none of the above cases is met, throw an error
		else {
			throwError(c);

		}
		if (!isEndofString)
			index++;

		// When we have 4 characters or = is encountered, we can start the decoding process
		if (index > 3 || isEndofString) {

			char tempByteOne;
			char tempByteTwo;
			char i1 = decodingInputBuffer[0];
			char i2 = decodingInputBuffer[1];
			char i3 = decodingInputBuffer[2];
			char i4 = decodingInputBuffer[3];

			//Computing 1st decoded character
			tempByteOne = (i1 << 2);
			tempByteTwo = (i2 & 0x30) >> 4;
			decodingOutputBuffer[0] = (tempByteOne | tempByteTwo);

			//Computing 2nd decoded character
			if (index > 2) {
				tempByteOne = ((i2 & 0x0f) << 4);
				tempByteTwo = (i3 & 0x3c) >> 2;
				decodingOutputBuffer[1] = (tempByteOne | tempByteTwo);
			}
			//Computing 3rd decoded character
			if (index > 3) {
				tempByteOne = ((i3 & 0x03) << 6);
				tempByteTwo = (i4 & 0x3f);
				decodingOutputBuffer[2] = tempByteOne | tempByteTwo;
			}

			// Print the decoded output.
			printCharacters(decodingOutputBuffer, index - 1);

			// if the input is not completely parse we continue else we break
			if (!isEndofString) {
				index = 0;
				// Clear all the output Buffer
				memset(decodingOutputBuffer, 0, 4);
			}
			// if we encountered = sign, end the loop
			else {
				break;
			}
		}

	}

}
