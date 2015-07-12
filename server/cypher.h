#ifndef CYPHER_H
#define CYPHER_H

/*
	Note: Wherever you see an argument of char** the function expects a pointer
	to an unallocated char* so that it can allocate a string of appropriate
	size for the calling function.

	They return a boolean for success or failure. Note that in the case of
	failure we guarantee any char** will be unallocated.
*/

bool cypher(
	const char* src, const size_t& slen,
	const char* key, const size_t& klen,
	char** iv, size_t* ivlen,
	char** dst, size_t* dlen);

bool decypher(
	const char* src, const size_t& slen,
	const char* key, const size_t& klen,
	const char* iv, const size_t& ivlen,
	char** dst, size_t* dlen);

// Looking for generating an HMAC? It's in hash.h

#endif
