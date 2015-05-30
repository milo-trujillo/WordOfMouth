#ifndef ENCODE_H
#define ENCODE_H

/*
	Note: These functions take a pointer to an unallocated char* so that they 
	can allocate a string for the calling function.

	They return a boolean for success or failure. Note that in the case of
	failure we guarantee the 'dst' pointer will not be allocated.
*/

// These guys do ascii encoding / decoding of binary data in src.
bool asciiEncode(const char* src, char** dst);
bool asciiDecode(const char* src, char** dst);

#endif
