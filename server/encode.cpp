#include <nettle/base64.h>
#include <nettle/base16.h>
#include <string.h>

// At present nothing can go wrong with ascii encoding, but we'll future 
// proof it with a bool return
bool asciiEncode(const char* src, const size_t srclen, char** dst)
{
	int dstlen = BASE64_ENCODE_LENGTH(srclen) + BASE64_ENCODE_FINAL_LENGTH + 1;
	struct base64_encode_ctx state;
	base64_encode_init(&state);
	char* enc = new char[dstlen];
	*dst = nullptr;

	int encoded_len = base64_encode_update(&state, reinterpret_cast<uint8_t*>(enc), 
		srclen, reinterpret_cast<const uint8_t*>(src));
	if( encoded_len < dstlen )
		encoded_len += base64_encode_final(&state, reinterpret_cast<uint8_t*>(enc) 
			+ encoded_len);
	enc[encoded_len++] = 0; // Don't forget the terminator!
	// TODO: Is the above line a buffer overflow? Does dstlen guarantee us room
	// for a null terminator?

	*dst = enc;
	return true;
}

bool asciiDecode(const char* src, size_t* dstlen, char** dst)
{
	int srclen = strlen(src);
	size_t maxlen = BASE64_DECODE_LENGTH(srclen);
	struct base64_decode_ctx state;
	base64_decode_init(&state);

	char* d = new char[maxlen];
	*dst = nullptr;

	#ifdef OLD_NETTLE
	// Older nettle uses decoded_bytes for size sanity checking, then updates
	// it to contain the number of actually decoded bytes.
	unsigned decoded_bytes = maxlen;
	#else
	size_t decoded_bytes;
	#endif

	bool ok = base64_decode_update(&state, &decoded_bytes, 
		reinterpret_cast<uint8_t*>(d), srclen, 
		reinterpret_cast<const uint8_t*>(src));
	if( !ok )
	{
		delete [] d;
		return false;
	}

	if( decoded_bytes < maxlen )
	{
		ok = base64_decode_final(&state);
		if( !ok )
		{
			delete [] d;
			return false;
		}
	}

	*dst = d;
	*dstlen = decoded_bytes;
	return true;
}

bool hexEncode(const char* src, const size_t srclen, char** dst)
{
	int dstlen = BASE16_ENCODE_LENGTH(srclen) + 1;
	char* enc = new char[dstlen];

	base16_encode_update(reinterpret_cast<uint8_t*>(enc), srclen, 
		reinterpret_cast<const uint8_t*>(src));

	*dst = enc;
	return true;
}

bool hexDecode(const char* src, size_t* dstlen, char** dst)
{
	int srclen = strlen(src);
	size_t maxlen = BASE16_DECODE_LENGTH(srclen);
	struct base16_decode_ctx state;
	base16_decode_init(&state);

	char* d = new char[maxlen];
	*dst = nullptr;

	#ifdef OLD_NETTLE
	// Older nettle uses decoded_bytes for size sanity checking, then updates
	// it to contain the number of actually decoded bytes.
	unsigned decoded_bytes = maxlen;
	#else
	size_t decoded_bytes;
	#endif

	bool ok = base16_decode_update(&state, &decoded_bytes, 
		reinterpret_cast<uint8_t*>(d), srclen, 
		reinterpret_cast<const uint8_t*>(src));
	if( !ok )
	{
		delete [] d;
		return false;
	}

	if( decoded_bytes < maxlen )
	{
		ok = base16_decode_final(&state);
		if( !ok )
		{
			delete [] d;
			return false;
		}
	}

	*dst = d;
	*dstlen = decoded_bytes;
	return true;
}


bool isAscii(const char* data, const size_t len)
{
	for( size_t i = 0; i < len - 1; i++ ) // Check for printable character range
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
