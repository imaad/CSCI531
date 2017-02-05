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
void encrypt(char *pphrase, char *out, FILE *pnbFile, int msg_type);
