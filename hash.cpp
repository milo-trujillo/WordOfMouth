#include <openssl/sha.h>	// For Sha hashes
#include <iostream>			// For strings
#include <stdio.h>			// For sprintf

using namespace std;

const int HASH_LENGTH = 20;

/*
	This is a crude means of generating a hash from any string that is hopefully
	both shorter than that string, and unique.

	The intended purpose is to generate hashes for messages we send, to easily
	identify when a message we've sent has traversed the entire network and
	hit us again. This prevents a 'dead message' from traveling indefinitely
	through the network.
*/
string genHash(string input)
{
	SHA512_CTX context;
	unsigned char buf[HASH_LENGTH];
	if(!SHA512_Init(&context))
		return "error";

	if(!SHA512_Update(&context, (unsigned char*)input.c_str(), input.size()))
		return "error";

	if(!SHA512_Final(buf, &context))
		return "error";

	char conv[HASH_LENGTH * 2];
	string hash;
	for( int i = 0; i < HASH_LENGTH; i++ )
		sprintf(conv + (i*2), "%02x", buf[i]);
	hash = conv;
	return hash;
}
