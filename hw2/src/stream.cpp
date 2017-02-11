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
void stream(char *p, int streamlength, int msg_type,
		unsigned char *encryptionBuffer) {

	//The following code has been taken from the source :http://merlot.usc.edu/cs531-s17/homeworks/hw2/
	//Although modified to meet the programming needs
	// Code copied begins here
	unsigned char md5_buf[MD5_DIGEST_LENGTH];
	int len = strlen(p) + 2 + MD5_DIGEST_LENGTH;
	char *s = (char*) malloc(len + 1);
	int outputByte = 0;

	int i = 0;
	unsigned char* out_ptr = encryptionBuffer;
	// Initial MD5 hash of passphrase
	MD5((const unsigned char *) p, strlen(p), md5_buf);
	for (;;) {
		int byteLength = 8;
		if (streamlength <= 0)
			break;
		sprintf(&s[MD5_DIGEST_LENGTH], "%02d%s", i, p);
		memcpy(s, md5_buf, MD5_DIGEST_LENGTH);

		// Take MD5 hash of this iteration key
		MD5((const unsigned char *) s, len, md5_buf);

		// How many bytes to print (up to 8)?
		int outputByteLength = streamlength - byteLength;
		if (outputByteLength >= 0) {
			outputByte = byteLength;
			streamlength = outputByteLength;
		} else {
			outputByte = streamlength;
			streamlength = 0;
		}

		// Determine location of output (stdout or out_buf)
		if (msg_type == 1) {
			// Default behavior if 'stream' prog is run
			fwrite(md5_buf, 1, outputByte, stdout);
		} else {
			// Print output to out_buf instead for PBM encryption
			memcpy(out_ptr, md5_buf, outputByte);
			out_ptr += outputByte;
		}

		// Reset the middle int value if needed
		if (++i == 100)
			i = 0;
	}
	free(s);

	// Code copied ends here

}
