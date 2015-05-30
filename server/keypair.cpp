#include <nettle/rsa.h>

/*
	Note: We'll probably want yarrow for a RNG unless we break that into its
	own file. We may also need buffer and sexp. Consult the libnettle keygen
	example included with the library.
*/

bool sign(const char* src, char** dst, const char* privkey)
{
	return false;
}

bool unsign(const char* src, char** dst, const char* pubkey)
{
	return false;
}

bool encrypt(const char* src, char** dst, const char* pubkey)
{
	return false;
}

bool decrypt(const char* src, char** dst, const char* privkey)
{
	return false;
}

