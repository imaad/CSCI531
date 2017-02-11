//
// Author : Imaad Ukaye
// Course : CSCI-531
// DATE   : 01/29/2017
//

#ifndef DECRYPT_H
#define DECRYPT_H

#include <stdio.h>
#include <string.h>

// Read in a file, print out decrypted bit values
void decryptDataFile(FILE *inputFilePointer, int width, int height);

// Decrypt a merged input PBM file, returning new PBM file
void decrypt(FILE *inputFilePointer, int msg_type);

#endif
