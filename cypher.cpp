#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>
#include <openssl/bio.h> // These two for Base64
#include <openssl/evp.h>

#include "hash.h" // For generating digests

using namespace std;

// The salt needs to be consistent across all linked copies of the
// program, but doesn't need to be cryptographically secure, so I've
// just hardcoded a random value in here.
static const unsigned char salt[AES_BLOCK_SIZE] = 
	{ 0xB6, 0x2C, 0x1B, 0x56, 0x2B, 0xD1, 0x74, 0x93, 0xC2, 0xBF,
	  0x41, 0x04, 0x33, 0xED, 0xDC, 0x74 };
static const int KEYLENGTH = 192;

/*
	Shout out to other awesome developers for figuring out OpenSSL!
	Example code for AES here:
	http://stackoverflow.com/questions/18152913/aes-aes-cbc-128-aes-cbc-192-aes-cbc-256-encryption-decryption-with-openssl-c
	And Base64 here:
	https://stackoverflow.com/questions/342409/how-do-i-base64-encode-decode-in-c
*/

// a simple hex-print routine. could be modified to print 16 bytes-per-line
// This is for debugging only
/*
static void hex_print(const void* pv, size_t len)
{
	const unsigned char * p = (const unsigned char*)pv;
	if (NULL == pv)
		printf("NULL");
	else
	{
		size_t i = 0;
		for (; i<len;++i)
			printf("%02X ", *p++);
	}
	printf("\n");
}
*/

string decypher(const string &msg, const string &key)
{
	if( key.size() == 0 )
		throw "No key provided!";
	if( msg.size() == 0 )
		return "";
	// The aes_key must be a binary blob of (at a minimum) 24 characters
	string aes_key = genHash(key);
	const size_t inputslength = msg.size();
	// Decrypted will actually be much smaller than encrypted, but
	// we don't know the exact size, so we're making a huge buffer
	unsigned char aes_input[inputslength];
	unsigned char aes_output[inputslength];
	for( int i = 0; i < inputslength; i++ )
		aes_input[i] = (unsigned char) msg[i];
	bzero(aes_output, sizeof(aes_output));

	// Init vectors are basically salts that AES modifies as it rolls
	unsigned char iv_dec[AES_BLOCK_SIZE];
	memcpy(iv_dec, salt, AES_BLOCK_SIZE);
	
	AES_KEY dec_key;
	AES_set_decrypt_key((unsigned char*) aes_key.c_str(), KEYLENGTH, &dec_key);
	AES_cbc_encrypt(aes_input, aes_output, inputslength, &dec_key, iv_dec, AES_DECRYPT);

	string result;
	for( int i = 0; i < inputslength && aes_output[i] != 0; i++ )
		result.push_back((char)aes_output[i]);
	return result;
}

string cypher(const string &msg, const string &key)
{
	if( key.size() == 0 )
		throw "No key provided!";
	if( msg.size() == 0 )
		return "";
	string aes_key = genHash(key);
	const size_t inputslength = msg.size();
	const size_t outputslength = ((inputslength + AES_BLOCK_SIZE) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
	// Encrypted will be much bigger than decrypted
	unsigned char aes_input[inputslength];
	unsigned char aes_output[outputslength];
	for( int i = 0; i < inputslength; i++ )
		aes_input[i] = (unsigned char) msg[i];
	bzero(aes_output, sizeof(aes_output));

	// Init vectors are basically salts that AES modifies as it rolls
	unsigned char iv_enc[AES_BLOCK_SIZE];
	memcpy(iv_enc, salt, AES_BLOCK_SIZE);
	
	AES_KEY enc_key;
	AES_set_encrypt_key((unsigned char*) aes_key.c_str(), KEYLENGTH, &enc_key);
	AES_cbc_encrypt(aes_input, aes_output, inputslength, &enc_key, iv_enc, AES_ENCRYPT);

	// Remember that an unsigned char array is *not* a string,
	// there's no null terminator, so we need to copy it over byte
	// by byte
	string result;
	for( int i = 0; i < outputslength; i++ )
		result.push_back((char)aes_output[i]);
	return result;
}

string base64Encode(const string &str)
{
	BIO *base64_filter = BIO_new( BIO_f_base64() );
	BIO_set_flags( base64_filter, BIO_FLAGS_BASE64_NO_NL );
	BIO *bio = BIO_new( BIO_s_mem() );
	BIO_set_flags( bio, BIO_FLAGS_BASE64_NO_NL );
	bio = BIO_push( base64_filter, bio );
	BIO_write( bio, str.c_str(), str.length() );
	BIO_flush( bio );

	char *new_data;
	long bytes_written = BIO_get_mem_data( bio, &new_data );

	string result( new_data, bytes_written );
	BIO_free_all( bio );
	return result;
}

string base64Decode(const string &str)
{
	BIO *bio, *base64_filter, *bio_out;
	char inbuf[512];
	int inlen;

	base64_filter = BIO_new( BIO_f_base64() );
	BIO_set_flags( base64_filter, BIO_FLAGS_BASE64_NO_NL );
	bio = BIO_new_mem_buf( (void*)str.c_str(), str.length() );
	bio = BIO_push( base64_filter, bio );
	bio_out = BIO_new( BIO_s_mem() );

	while( (inlen = BIO_read(bio, inbuf, 512)) > 0 )
		BIO_write( bio_out, inbuf, inlen );

	BIO_flush( bio_out );

	char *new_data;
	long bytes_written = BIO_get_mem_data( bio_out, &new_data );

	string result( new_data, bytes_written );
	BIO_free_all( bio );
	BIO_free_all( bio_out );
	return result;
}

// Returns true if all characters within valid ASCII range, false otherwise
bool isReadableText(const string &msg)
{
	for( int i = 0; i < msg.size(); i++ )
		if( msg[i] < 0 || msg[i] > 127 )
			return false;
		// If outside printable ASCII range
		//if( (msg[i] > 126 || msg[i] < 33) && msg[i] != '\n' )
	return true;
}
