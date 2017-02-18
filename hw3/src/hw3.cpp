//
// Author : Imaad Ukaye
// Course : CSCI-531
// DATE   : 01/2 9/2017
//
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include "keygen.h"

char passPhrase[16];
char stringLength[16];
char keyFile[16];
char* mergeOutputFileName;
int period = -1;

// This method copies source characters to destination characters and skips the 'skip' number of characters in the source string
void skipCopyString(char destination[], char source[], int skip) {
	int c = 0;

	while (source[c + skip] != '\0') {
		destination[c] = source[c + skip];
		c++;
	}
	destination[c] = '\0';
}
char ERROR_MSG[] =
		"Invalid arguments. The commandline syntax for hw2 is as follows:\nhw3 keygen -p=pphrase -t=period\nhw3 crypt -k=keyfile [file]\nhw3 invkey keyfile\nhw3 histo -t=period -i=which [file]\nhw3 solve -l=max_t file";

// Performs the required functionality after decoding input arguments
void invokeApplication(int argc, char *argv[], int msgType) {
	char ERROR_FILENOTFOUND[] = "Input file '%s' does not exist";
	FILE *filePointerOne = NULL;
	FILE *filePointerTwo = NULL;

	// The following code initializes file handlers for each sub module and throws error if file is not found
	if (msgType == 21) {
		filePointerOne = stdin;
	} else if (msgType == 22) {
		filePointerOne = fopen(argv[4], "r");
		if (filePointerOne == NULL) {
			fprintf(stderr, ERROR_FILENOTFOUND, argv[4]);
			exit(1);
		}
	} else if (msgType == 3) {
		filePointerOne = fopen(argv[2], "r");
		if (filePointerOne == NULL) {
			fprintf(stderr, ERROR_FILENOTFOUND, argv[2]);
			exit(1);
		}

		filePointerTwo = fopen(argv[3], "r");
		if (filePointerTwo == NULL) {
			fprintf(stderr, ERROR_FILENOTFOUND, argv[3]);
			exit(1);
		}

	} else if (msgType == 41) {
		filePointerOne = stdin;
	} else if (msgType == 42) {
		filePointerOne = fopen(argv[2], "r");
		if (filePointerOne == NULL) {
			fprintf(stderr, ERROR_FILENOTFOUND, argv[2]);
			exit(1);
		}

	}

	// Calls the required modules based on decoded inputs from the command line
	switch (msgType) {
	case 1:
		// Call Stream submodule
		keygen(passPhrase,period);
		break;
	case 21:
		// Call encrypt submodule with stdin
		//encrypt(passPhrase, strOut, filePointerOne, msgType);
		break;
	case 22:
		// Call encrypt submodule with file pointer
		//encrypt(passPhrase, strOut, filePointerOne, msgType);
		break;
	case 3:
		// Call merge submodule
		//merge(filePointerOne, filePointerTwo, msgType);
		break;
	case 41:
		// Call decrypt submodule with stdin
		//decrypt(filePointerOne, msgType);
		break;
	case 42:
		// Call decrypt submodule with file Pointer
		//decrypt(filePointerOne, msgType);
		break;
	default:
		// If none of the above condition is met, throw error
		fprintf(stderr, ERROR_MSG);
		exit(1);

	}
}
// If only 2 arguments are provided i.e. input is to be taken from stdin

// this function performs preprocessing, i.e. identifies the message type for the corresponding module to be invoked
int parseInputs(int argc, char *argv[]) {
	char keygenString[] = "keygen";
	char cryptString[] = "crypt";
	char invKeyString[] = "invkey";
	char histoString[] = "histo";
	char solveString[] = "solve";

	int msg_type = -1;
	// If the number of arguments are invalid
	if (argc < 2 || argc > 5) {
		fprintf(stderr, ERROR_MSG);
		exit(1);
	}

	// identify message type
	else {
		if (strcmp(argv[1], keygenString) == 0) {

			if (argc != 4) {
				fprintf(stderr, ERROR_MSG);
				exit(1);
			}
			for (int i = 2; i < 4; ++i) {
				if (strncmp(argv[i], "-p=", 3) == 0) {
					skipCopyString(passPhrase, argv[i], 3);

				} else if (strncmp(argv[i], "-t=", 3) == 0) {
					skipCopyString(stringLength, argv[i], 3);
					period = atoi(stringLength);
				} else {
					fprintf(stderr, ERROR_MSG);
					exit(1);
				}
			}
			//printf("Pass: %s\n", passPhrase);
			//printf("Length :%d\n", length);
			msg_type = 1;
		} else if (strcmp(argv[1], cryptString) == 0) {

			if (argc > 4 || argc < 3) {
				fprintf(stderr, ERROR_MSG);
				exit(1);
			}
			for (int i = 2; i < 3; ++i) {
				 if (strncmp(argv[i], "-k=", 3) == 0) {
					skipCopyString(keyFile, argv[i], 3);
				} else {
					fprintf(stderr, ERROR_MSG);
					exit(1);
				}
			}
			if (argc == 3)
				msg_type = 21;
			else
				msg_type = 22;
		} else if (strcmp(argv[1], invKeyString) == 0) {
			if (argc != 3) {
				fprintf(stderr, ERROR_MSG);
				exit(1);
			}
			msg_type = 3;
		} else if (strcmp(argv[1], histoString) == 0) {
			if (argc < 2 || argc > 3) {
				fprintf(stderr, ERROR_MSG);
				exit(1);
			}
			if (argc == 2)
				msg_type = 41;
			else
				msg_type = 42;
		}
	}
	return msg_type;
}
// Main method of HW2
int main(int argc, char *argv[]) {

	//Look for correct command line arguments and generates a message type for each sub module.
	int msg_type = parseInputs(argc, argv);
	//printf("%d",msg_type);
	//Load the corresponding module based on Input arguments
	invokeApplication(argc, argv, msg_type);

	return 0;
}

