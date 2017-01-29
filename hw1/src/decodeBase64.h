//
// Author : Imaad Ukaye
// Course : CSCI-531
// DATE   : 01/19/2017
//

#ifndef DECBASE64_H
#define DECBASE64_H

// Header file to decode Base64 files and inputs
void decodeBase64(FILE *, int msg_type);
void printCharacters(unsigned char decodingOutputBuffer[], int length);
void throwError(char c);
#endif
