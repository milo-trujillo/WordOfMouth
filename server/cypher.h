#ifndef CYPHER_H
#define CYPHER_H

/*
	Note: These functions take a pointer to an unallocated char* so that they 
	can allocate a string for the calling function.

	They return a boolean for success or failure. Note that in the case of
	failure we guarantee the 'dst' pointer will not be allocated.
*/

bool cypher(const char* src, char** dst, const char* key);
bool decypher(const char* src, char** dst, const char* key);

#endif
