#ifndef KEYPAIR_H
#define KEYPAIR_H

/*
	Note: These functions take a pointer to an unallocated char* so that they 
	can allocate a string for the calling function.

	They return a boolean for success or failure. Note that in the case of
	failure we guarantee the 'dst' pointer will not be allocated.
*/

// We'll also need key generation and destruction functions, and maybe a class
// to hold that data.

bool sign(const char* src, char** dst, const char* privkey);
bool unsign(const char* src, char** dst, const char* pubkey);

bool encrypt(const char* src, char** dst, const char* pubkey);
bool decrypt(const char* src, char** dst, const char* privkey);

#endif
