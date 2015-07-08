#include <nettle/base64.h>
#include <nettle/base16.h>
#include <string.h>

// At present nothing can go wrong with ascii encoding, but we'll future 
// proof it with a bool return
bool asciiEncode(const char* src, const int srclen, char** dst)
{
	int dstlen = BASE64_ENCODE_LENGTH(srclen) + BASE64_ENCODE_FINAL_LENGTH + 1;
	struct base64_encode_ctx state;
	base64_encode_init(&state);
	char* enc = new char[dstlen];
	*dst = nullptr;

	int encoded_len = base64_encode_update(&state, (uint8_t*) enc, srclen, (const uint8_t*) src);
	if( encoded_len < dstlen )
		encoded_len += base64_encode_final(&state, ((uint8_t*) enc) + encoded_len);
	enc[encoded_len++] = 0; // Don't forget the terminator!

	*dst = enc;
	return true;
}

bool asciiDecode(const char* src, int* dstlen, char** dst)
{
	int srclen = strlen(src);
	*dstlen = BASE64_DECODE_LENGTH(srclen);
	struct base64_decode_ctx state;
	base64_decode_init(&state);

	char* d = new char[*dstlen];
	*dst = nullptr;

	size_t decoded_bytes;

	// Older versions of libnettle had *slightly* different arguments
	// and those versions are unfortunately still used on FreeBSD.
	#ifdef OLD_NETTLE
	bool ok = base64_decode_update(&state, (unsigned *) &decoded_bytes, (uint8_t*)d, srclen, (const uint8_t*) src);
	#else
	bool ok = base64_decode_update(&state, &decoded_bytes, (uint8_t*)d, srclen, (const uint8_t*) src);
	#endif
	if( !ok )
	{
		delete [] d;
		return false;
	}

	if( decoded_bytes < (size_t) *dstlen )
	{
		ok = base64_decode_final(&state);
		if( !ok )
		{
			delete [] d;
			return false;
		}
	}

	*dst = d;
	return true;
}

bool hexEncode(const char* src, const int srclen, char** dst)
{
	int dstlen = BASE16_ENCODE_LENGTH(srclen) + 1;
	char* enc = new char[dstlen];

	base16_encode_update((uint8_t*) enc, srclen, (const uint8_t*) src);

	*dst = enc;
	return true;
}

bool hexDecode(const char* src, int* dstlen, char** dst)
{
	int srclen = strlen(src);
	*dstlen = BASE16_DECODE_LENGTH(srclen);
	struct base16_decode_ctx state;
	base16_decode_init(&state);

	char* d = new char[*dstlen];
	*dst = nullptr;

	size_t decoded_bytes;

	// Same disclaimer as above
	#ifdef OLD_NETTLE
	bool ok = base16_decode_update(&state, (unsigned *) &decoded_bytes, (uint8_t*)d, srclen, (const uint8_t*) src);
	#else
	bool ok = base16_decode_update(&state, &decoded_bytes, (uint8_t*)d, srclen, (const uint8_t*) src);
	#endif
	if( !ok )
	{
		delete [] d;
		return false;
	}

	if( decoded_bytes < (size_t) *dstlen )
	{
		ok = base16_decode_final(&state);
		if( !ok )
		{
			delete [] d;
			return false;
		}
	}

	*dst = d;
	return true;
}


bool isAscii(const char* data, const int len)
{
	for( int i = 0; i < len - 1; i++ ) // Check for printable character range
	{
		if( data[i] == '\n' || data[i] == '\t' )
			continue;
		if( data[i] < 32 || data[i] > 126 )
			return false;
	}
	if( data[len - 1] != 0 ) // Check for null terminator
		return false;
	return true;
}
