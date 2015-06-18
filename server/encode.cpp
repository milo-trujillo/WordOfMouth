#include <nettle/base64.h>

// At present nothing can go wrong with ascii encoding, but we'll future 
// proof it with a bool return
bool asciiEncode(const char* src, const int srclen, char** dst)
{
	int dstlen = BASE64_ENCODE_LENGTH(srclen) + BASE64_ENCODE_FINAL_LENGTH + 1;
	struct base64_encode_ctx state;
	base64_encode_init(&state);
	char* enc = new char[dstlen];

	int encoded_len = base64_encode_update(&state, (uint8_t*) enc, srclen, (const uint8_t*) src);
	if( encoded_len < dstlen )
		encoded_len += base64_encode_final(&state, ((uint8_t*) enc) + encoded_len);
	enc[encoded_len++] = '\n';

	*dst = enc;
	return true;
}

bool asciiDecode(const char* src, int* dstlen, char** dst)
{
	return false;
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
