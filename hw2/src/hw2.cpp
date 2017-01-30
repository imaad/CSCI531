//
// Author : Imaad Ukaye
// Course : CSCI-531
// DATE   : 01/2 9/2017
//
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>

void copy_string(char destination[], char source[], int skip) {
	int c = 0;

	while (source[c + skip] != '\0') {
		destination[c] = source[c + skip];
		c++;
	}
	destination[c] = '\0';
}
// Main method of HW1
char ERROR_MSG[] =
		"Malformed command. Syntax: 'hw1 <hexdump|enc-base64|dec-base64) <fileName>'\n";

// Performs the required functionality after decoding input arguments
void invokeApplication(int argc, char *argv[], int msgType) {
	char ERROR_FILENOTFOUND[] = "Input file '%s' does not exist";

	// if the file name is provided in the command-line argument
	if (msgType == 22 || msgType == 42) {
		FILE *filePointer = NULL;
		filePointer = fopen(argv[2], "r");
		if (filePointer == NULL) {
			fprintf(stderr, ERROR_FILENOTFOUND, argv[2]);
			exit(1);
		}
		// Calls the required modules based on decoded inputs from the command line
		switch (msgType) {
		case 1:
			break;
		case 21:
			break;
		case 22:
			break;
		case 3:
			break;
		case 41:
			break;
		case 42:
			break;
		default:
			fprintf(stderr, ERROR_MSG);
			exit(1);

		}
	}
	// If only 2 arguments are provided i.e. input is to be taken from stdin

}

// this function performs preprocessing, i.e. identifies the message type for the corrseesponding module to be invoked
int parseInputs(int argc, char *argv[]) {
	char streamString[] = "stream";
	char encryptString[] = "encrypt";
	char mergeString[] = "merge";
	char decryptString[] = "decrypt";

	int msg_type = -1;
	// If the number of arguments are invalid
	if (argc < 2 || argc > 5) {
		fprintf(stderr, ERROR_MSG);
		exit(1);
	}

	// identify message type
	else {
		if (strcmp(argv[1], streamString) == 0) {
			char passPhrase[16];
			char strlen[16];
			int length = -1;
			if (argc != 4) {
				fprintf(stderr, ERROR_MSG);
				exit(1);
			}
			for (int i = 2; i < 4; ++i) {
				if (strncmp(argv[i], "-p=", 3) == 0) {
					copy_string(passPhrase, argv[i], 3);

				} else if (strncmp(argv[i], "-l=", 3) == 0) {
					copy_string(strlen, argv[i], 3);
					length = atoi(strlen);
				} else {
					fprintf(stderr, ERROR_MSG);
					exit(1);
				}
			}
			printf("Pass: %s\n", passPhrase);
			printf("Length :%d", length);
			msg_type = 1;
		} else if (strcmp(argv[1], encryptString) == 0) {
			char passPhrase[16];
			char strOut[16];

			if (argc < 4 || argc > 5) {
				fprintf(stderr, ERROR_MSG);
				exit(1);
			}
			for (int i = 2; i < 4; ++i) {
				if (strncmp(argv[i], "-p=", 3) == 0) {
					copy_string(passPhrase, argv[i], 3);

				} else if (strncmp(argv[i], "-out=", 5) == 0) {
					copy_string(strOut, argv[i], 5);
				} else {
					fprintf(stderr, ERROR_MSG);
					exit(1);
				}
			}
			printf("Pass: %s\n", passPhrase);
			printf("Out: %s\n", strOut);
			if (argc == 4)
				msg_type = 21;
			else
				msg_type = 22;
		} else if (strcmp(argv[1], mergeString) == 0) {
			if (argc != 4) {
				fprintf(stderr, ERROR_MSG);
				exit(1);
			}
			msg_type = 3;
		} else if (strcmp(argv[1], decryptString) == 0) {
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
int main(int argc, char *argv[]) {

//Look for correct command line arguments
	printf("ARGC: %d\n", argc);

	int msg_type = parseInputs(argc, argv);
//Load the corresponding module based on Input arguments
//invokeApplication(argc, argv, msg_type);
	printf("%d", msg_type);

	return 0;
}

