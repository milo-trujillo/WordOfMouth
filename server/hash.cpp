#include <nettle/sha2.h>

// This is just shorthand so users don't need to know the hash type or
// implementation we're using.
int getHashLength()
{
	return SHA512_DIGEST_SIZE;
}

// At the moment nothing can go wrong, and this always returns true. However,
// if we some day decide to be clever and check for out of memory then the
// infrastucture for error reporting will already be here.
bool hash(const char* src, const int len, char** dst)
{
	sha512_ctx state;
	char* digest = new char[SHA512_DIGEST_SIZE];

	// Note: when updating the hash, length refers to the length of input,
	// but in digest it refers to the length of the destination buffer.
	sha512_init(&state);
	sha512_update(&state, len, (const uint8_t*) src);
	sha512_digest(&state, SHA512_DIGEST_SIZE, (uint8_t*) digest);

	*dst = digest;

	return true;
}
