//
// Author : Imaad Ukaye
// Course : CSCI-531
// DATE   : 01/2 9/2017
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/md5.h>

#define byteLength MD5_DIGEST_LENGTH/2

// A simple stream cipher; outputs 'len' bytes of MD5 stream cipher
void encrypt(char *p, char *outputFileName, FILE *inputFile, int msg_type);
void printString(unsigned char str[]);
void printEncryptedStream(FILE* outputPointer, unsigned char output[][2],
		int width);
void generateFileNames(char* outputFileName, char** outputFileOne,
		char** outputFileTwo);
void generateDataFiles(FILE* filePointerOne, FILE* filePointerTwo,
		unsigned char* inputStreamBuffer, unsigned char* keyStreamBuffer,
		int width, int height);
