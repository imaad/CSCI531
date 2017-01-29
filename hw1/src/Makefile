#
#
##		MAKE FILE FOR HOMEWORK I
##		Author : Imaad Ukaye
##		Course : CSCI-531
## 		DATE   : 01/19/2017
#
#

hw1: hexDump.o encodeBase64.o decodeBase64.o hw1.cpp
	g++ -Wall hw1.cpp hexDump.o encodeBase64.o decodeBase64.o -o hw1
hexDump.o: hexDump.h hexDump.cpp
	g++ -Wall -c hexDump.cpp
encodeBase64.o: encodeBase64.h encodeBase64.cpp
	g++ -Wall -c encodeBase64.cpp
decodeBase64.o: decodeBase64.h decodeBase64.cpp
	g++ -Wall -c decodeBase64.cpp
clean:
	rm -f hw1 *.o *.dat *.hex *.b64 
