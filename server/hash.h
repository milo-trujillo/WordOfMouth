#ifndef HASH_H
#define HASH_H

/*
	Note: hash() takes a pointer to an unallocated char* so that it can reserve
	space for the appropriate hash type.

	It returns a boolean for success or failure. On success, *dst is guaranteed
	to be a string of length getHashLength() containing the binary hash of the
	data. On failure it is guaranteed that dst will not be allocated.

	Since we can hash binary data as well as ascii we cannot trust strlen(),
	and must pass the source length explicitly.
	
*/

int getHashLength();

bool hash(const char* src, const size_t len, char** dst);

/*
	hmacGen takes a cyphered message and the key used to cypher it (along with
	their lengths), and generates a Message Authentication Code, stored in dst,
	along with its length, stored in dlen.
*/

bool genHMAC(
	const char* src, const size_t slen, 
	const char* key, const size_t klen,
	char** dst, size_t* dlen);

#endif
