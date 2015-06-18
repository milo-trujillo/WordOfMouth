#ifndef ENCODE_H
#define ENCODE_H

/*
	Note: These functions take a pointer to an unallocated char* so that they 
	can allocate a string for the calling function.

	They return a boolean for success or failure. Note that in the case of
	failure we guarantee the 'dst' pointer will not be allocated.

	Because we are passing in binary data in the char*, not a string, we cannot
	rely on strlen and need to pass the length of the data manually. Similarly,
	when decoding the user must provide an int* to save the decoded length to.
*/

// These guys do ascii encoding / decoding of binary data in src.
bool asciiEncode(const char* src, const int srclen, char** dst);
bool asciiDecode(const char* src, int* dstlen, char** dst);

// This detects whether the current message encoding is suitable for direct
// display on a terminal.
bool isAscii(const char* data, const int len);

#endif
