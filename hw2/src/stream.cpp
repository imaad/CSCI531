//
// Author : Imaad Ukaye
// Course : CSCI-531
// DATE   : 01/2 9/2017
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/md5.h>

// A simple stream cipher; outputs 'len' bytes of MD5 stream cipher
void stream(char *pphrase, int streamlength, unsigned char out_buf[],
		int msg_type) {

	unsigned char md5_buf[MD5_DIGEST_LENGTH];
	int str_len = strlen(pphrase) + 2 + MD5_DIGEST_LENGTH;
	int byteLength = 8;
	char *s = (char*) malloc(str_len + 1);
	int i = 0;
	int outputByte = 0;

	unsigned char *outputBuffer = out_buf;

	// Initial MD5 hash of passphrase
	MD5((const unsigned char *) pphrase, strlen(pphrase), md5_buf);
	for (;;) {
		// Assemble iteration key into buffer s
		// {Previous MD5 Hash} + {[00,99]} + {passphrase}
		if (streamlength <= 0)
			break;
		sprintf(&s[MD5_DIGEST_LENGTH], "%02d%s", i, pphrase);

		memcpy(s, md5_buf, MD5_DIGEST_LENGTH);

		// Take MD5 hash of this iteration key
		MD5((const unsigned char *) s, str_len, md5_buf);

		int byteDiff = streamlength - byteLength;
		// How many bytes to print (up to 8)?
		if (byteDiff < 0) {
			streamlength = 0;
			outputByte = streamlength;
		} else {
			outputByte = byteLength;
			streamlength = byteDiff;

		}

		// Determine location of output (stdout or out_buf)
		if (msg_type == 1) {
			// Default behavior if 'stream' prog is run
			fwrite(md5_buf, 1, outputByte, stdout);
		} else {
			// Print output to out_buf instead for PBM encryption
			memcpy(outputBuffer, md5_buf, outputByte);
			outputBuffer += outputByte;
		}

		// Reset the middle int value if needed
		if (++i == 100)
			i = 0;
	}
	free(s);

// PSEUDO-CODE
	// - Allocate enough memory to hold the whole stream cipher
	// - Take the MD5 hash of the original passphrase
	// - Concat this hash string with an integer {0-99}, and the passphrase again
	// - Take the MD5 of this new string; first 8 bytes are the beginning of the stream cipher
	// - Output these 8 bytes to either stdout or a char* out_buf
	// - Repeat for as long as needed to fill length of cipher
}
