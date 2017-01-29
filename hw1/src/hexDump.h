//
// Author : Imaad Ukaye
// Course : CSCI-531
// DATE   : 01/19/2017
//

#ifndef HEXDUMP_H
#define HEXDUMP_H

#include <stdio.h>
#include <string.h>
// Header file to implement hexdump funtionality

// prints a stream of chars with \n in the end of line
void printPlainText(unsigned char lineOut[]);

// Takes inputs from the input stream provided by the user and generates a hexdump for the same
void hexdump(FILE*, int msg_type);
#endif
