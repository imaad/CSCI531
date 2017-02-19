/*
 * hw3.cpp
 *
 *  Created on: Feb 18, 2017
 *      Author: INDORINAE
 */

#include "keygen.h"
#include "stream.h"
#include <stdio.h>
#include <stdlib.h>

void printCharString(unsigned char* p) {
	int i = 0;
	while (i < 4) {
		if (i % 4 == 0 && i != 0)
			printf("\n");
		printf("%x", p[i]);
		i++;
	}

}
unsigned long getBytes(unsigned char* p, int start) {

	unsigned char* temp = (unsigned char*) malloc(5);
	temp[0] = p[start];
	temp[1] = p[start + 1];
	temp[2] = p[start + 2];
	temp[3] = p[start + 3];
	temp[4] = '\0';
	unsigned long l;
	memcpy(&l, temp, sizeof(long));

	return l;
}
void printState(unsigned char* state) {
	int i = 0;
	while (i < 26) {
		printf("%c", state[i]);
		i++;
	}
	printf("\n");

}
void keygen(char* p, int period) {

	unsigned char* X = (unsigned char*) malloc(200 * period);
	memset(X, 0, 200 * period);
	stream(p, 200 * period, 21, X);
	int skip = -4;
	for (int periodIndex = 0; periodIndex < period; periodIndex++) {
		unsigned char state[] = "abcdefghijklmnopqrstuvwxyz";
		//printCharString(X);
		for (int itr = 0; itr < 2; itr++) {
			int L = 26;
			while (L > 1) {
				unsigned long currentByte = getBytes(X, skip += 4);
				int R = currentByte % L;
				if (R != L - 1) {
					unsigned char temp = state[R];
					state[R] = state[L - 1];
					state[L - 1] = temp;
				}
				L = L - 1;
			}

		}
		printState(state);
	}

}

