//
// Author : Imaad Ukaye
// Course : CSCI-531
// DATE   : 01/19/2017
//
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>

#include "decodeBase64.h"
#include "encodeBase64.h"
#include "hexDump.h"

// Main method of HW1
char ERROR_MSG[] =
		"Malformed command. Syntax: 'hw1 <hexdump|enc-base64|dec-base64) <fileName>'\n";

// Performs the required functionality after decoding input arguments
void invokeApplication(int argc, char *argv[], int msgType) {
	char ERROR_FILENOTFOUND[] = "Input file '%s' does not exist";

	// if the file name is provided in the command-line argument
	if (argc == 3) {
		FILE *filePointer = NULL;
		filePointer = fopen(argv[2], "r");
		if (filePointer == NULL) {
			fprintf(stderr, ERROR_FILENOTFOUND, argv[2]);
			exit(1);
		}
		// Calls the required modules based on decoded inputs from the command line
		switch (msgType) {
		case 1:
			hexdump(filePointer, msgType);
			break;
		case 2:
			encodeBase64(filePointer, msgType);
			break;
		case 3:
			decodeBase64(filePointer, msgType);
			break;
		default:
			fprintf(stderr, ERROR_MSG);
			exit(1);

		}
	}
	// If only 2 arguments are provided i.e. input is to be taken from stdin
	else if (argc == 2) {
		switch (msgType) {
		case 1:
			hexdump(stdin, msgType);
			break;
		case 2:
			encodeBase64(stdin, msgType);
			break;
		case 3:
			decodeBase64(stdin, msgType);
			break;
		default:
			fprintf(stderr, ERROR_MSG);
			exit(1);

		}
	}
}

// this function performs preprocessing, i.e. identifies the message type for the corresponding module to be invoked
int parseInputs(int argc, char *argv[]) {
	char hexString[] = "hexdump";
	char encString[] = "enc-base64";
	char decString[] = "dec-base64";

	int msg_type = -1;
	// If the number of arguments are invalid
	if (argc < 2 || argc >= 4) {
		fprintf(stderr, ERROR_MSG);
		exit(1);
	}

	// identify message type
	else {
		if (strcmp(argv[1], hexString) == 0) {
			msg_type = 1;
		} else if (strcmp(argv[1], encString) == 0) {
			msg_type = 2;
		} else if (strcmp(argv[1], decString) == 0) {
			msg_type = 3;
		}
	}
	return msg_type;
}
int main(int argc, char *argv[]) {

	//Look for correct command line arguments
	int msg_type = parseInputs(argc, argv);
	//Load the corresponding module based on Input arguments
	invokeApplication(argc, argv, msg_type);

	return 0;
}

