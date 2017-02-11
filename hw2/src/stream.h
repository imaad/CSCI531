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

// Generates len Byte key for encryption derieved from p
void stream(char *p, int streamlength, int msg_type, unsigned char *encryptionBuffer);
