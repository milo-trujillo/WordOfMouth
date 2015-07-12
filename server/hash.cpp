#include <nettle/sha2.h>
#include <nettle/sha1.h>
#include <nettle/hmac.h>
#include <stdlib.h> // For size_t

// This is just shorthand so users don't need to know the hash type or
// implementation we're using.
int getHashLength()
{
	return SHA512_DIGEST_SIZE;
}

// At the moment nothing can go wrong, and this always returns true. However,
// if we some day decide to be clever and check for out of memory then the
// infrastucture for error reporting will already be here.
bool hash(const char* src, const size_t len, char** dst)
{
	sha512_ctx state;
	char* digest = new char[SHA512_DIGEST_SIZE];

	// Note: when updating the hash, length refers to the length of input,
	// but in digest it refers to the length of the destination buffer.
	sha512_init(&state);
	sha512_update(&state, len, reinterpret_cast<const uint8_t*>(src));
	sha512_digest(&state, SHA512_DIGEST_SIZE, reinterpret_cast<uint8_t*>(digest));

	*dst = digest;

	return true;
}

// While SHA1 has known vulnerabilities, HMACs with SHA1 do not. Since there
// is no difference in security and SHA1 is quite a bit faster than SHA512,
// we use SHA1 for our HMACs.
bool genHMAC(
	const char* src, const size_t slen, 
	const char* key, const size_t klen,
	char** dst, size_t* dlen)
{
	*dlen = SHA1_DIGEST_SIZE;
	*dst = new char[SHA1_DIGEST_SIZE];

	// TODO: Look into purging memory, do we need to delete the ctx safely?
	hmac_sha1_ctx state;
	hmac_sha1_set_key(&state, klen, reinterpret_cast<const uint8_t*>(key));
	hmac_sha1_update(&state, slen, reinterpret_cast<const uint8_t*>(src));
	hmac_sha1_digest(&state, SHA1_DIGEST_SIZE, reinterpret_cast<uint8_t*>(*dst));

	return true;
}
