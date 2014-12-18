#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>

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
	Shout out to the example code over here:
	http://stackoverflow.com/questions/18152913/aes-aes-cbc-128-aes-cbc-192-aes-cbc-256-encryption-decryption-with-openssl-c
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
	unsigned char* aes_key = generate192BitDigest(key);
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
	AES_set_decrypt_key(aes_key, KEYLENGTH, &dec_key);
	AES_cbc_encrypt(aes_input, aes_output, inputslength, &dec_key, iv_dec, AES_DECRYPT);

	string result;
	for( int i = 0; i < inputslength && aes_output[i] != 0; i++ )
		result.push_back((char)aes_output[i]);
	delete aes_key;
	return result;
}

string cypher(const string &msg, const string &key)
{
	unsigned char* aes_key = generate192BitDigest(key);
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
	AES_set_encrypt_key(aes_key, KEYLENGTH, &enc_key);
	AES_cbc_encrypt(aes_input, aes_output, inputslength, &enc_key, iv_enc, AES_ENCRYPT);

	// Remember that an unsigned char array is *not* a string,
	// there's no null terminator, so we need to copy it over byte
	// by byte
	string result;
	for( int i = 0; i < outputslength; i++ )
		result.push_back((char)aes_output[i]);
	delete aes_key;
	return result;
}

bool isReadableText(const string &msg)
{
	for( int i = 0; i < msg.size(); i++ )
		if( msg[i] > 126 || msg[i] < 33 ) // Outside ASCII
			return false;
	return true;
}
