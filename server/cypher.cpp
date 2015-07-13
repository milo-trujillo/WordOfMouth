#include <nettle/cbc.h>
#include <nettle/aes.h>
#include <stdlib.h> // For size_t on some platforms
#include <string.h> // For memcpy, which is used in some nettle macros

// Note: Look into using AES with CBC in nettle. Maybe example here:
// http://nettle.sourcearchive.com/documentation/2.0-1/cbc-test_8c-source.html

// Stub function, need to find a cryptographically secure RNG solution
bool genIV(char** iv, size_t* len)
{
	*iv = new char [AES_BLOCK_SIZE];
	*len = AES_BLOCK_SIZE;

	delete [] iv;
	return false;
}

bool cypher(const char* src, const size_t& slen,
	const char* key, const size_t& klen,
	char** iv, size_t* ivlen,
	char** dst, size_t* dlen)
{
	// We're going to step on these anyway, might as well make them reliable
	// values.
	*iv = nullptr;
	*ivlen = 0;

	if( !genIV(iv, ivlen) )
		return false;

	try
	{
		*dlen = slen;
		*dst = new char[*dlen];
		struct CBC_CTX(struct aes_ctx, AES_BLOCK_SIZE) state;
		aes_set_encrypt_key(&state.ctx, klen, 
			reinterpret_cast<const uint8_t*>(key));
		CBC_SET_IV(&state, *iv);
		CBC_ENCRYPT(&state, aes_encrypt, slen, 
			reinterpret_cast<uint8_t*>(*dst),
			reinterpret_cast<const uint8_t*>(src));
		return true;
	}
	catch(...)
	{
		// Cleanup if memory has been initialized and something went wrong
		delete [] *iv;
		delete [] *dst;
		iv = nullptr;
		dst = nullptr;
		return false;
	}
}

bool decypher(const char* src, const size_t& slen,
	const char* key, const size_t& klen,
	const char* iv, const size_t& ivlen,
	char** dst, size_t* dlen)
{
	try
	{
		*dlen = slen;
		*dst = new char[*dlen];
		struct CBC_CTX(struct aes_ctx, AES_BLOCK_SIZE) state;
		aes_set_decrypt_key(&state.ctx, klen,
			reinterpret_cast<const uint8_t*>(key));
		CBC_SET_IV(&state, iv);
		CBC_DECRYPT(&state, aes_decrypt, slen,
			reinterpret_cast<uint8_t*>(*dst),
			reinterpret_cast<const uint8_t*>(src));
		return true;
	}
	catch(...)
	{
		// If something goes wrong, nuke allocated pointers
		delete [] dst;
		*dst = nullptr;
		return false;
	}
}
