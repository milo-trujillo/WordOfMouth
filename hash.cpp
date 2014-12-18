#include <openssl/sha.h>    // For Sha hashes
#include <iostream>         // For strings
#include <stdio.h>          // For sprintf
#include <strings.h>		// For bzero
#include <time.h>           // For epoch time() function

#include <list>             // STL linked lists for tracking sent messages
#include <map>              // For pairs

using namespace std;

static const int HASH_LENGTH = 20;
static const int DIGEST_192_LENGTH = 24;
static const unsigned int MAX_MESSAGE_AGE = 600; // 10 minutes (epoch time)

// Global vars are bad, but at least this is only global to the hash code
static list<pair<string, time_t> > msgHashes; // Tracks hashes of recent messages
static pthread_mutex_t hashLock; // Prevent thread collision accessing msgHashes

/*
	This is a crude means of generating a hash from any string that is hopefully
	both shorter than that string, and unique.

	The intended purpose is to generate hashes for messages we send, to easily
	identify when a message we've sent has traversed the entire network and
	hit us again. This prevents a 'dead message' from traveling indefinitely
	through the network.
*/
string genHash(const string &input)
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

// Returns if a message has been seen previously (and should be dropped)
bool messageSeen(const string &msg)
{
	string hash = genHash(msg);
	time_t currentTime;
	time(&currentTime);
	pthread_mutex_lock(&hashLock);

	// Check if we've seen the message yet
	bool messageFound = false;
	list<pair<string, time_t> >::iterator hashItr = msgHashes.begin();
	while( hashItr != msgHashes.end() )
	{
		if( hashItr->first == hash )
			messageFound = true;
		if( currentTime - hashItr->second > MAX_MESSAGE_AGE )
			msgHashes.erase(hashItr);
		else
			hashItr++;
	}
	// No? Alright, log that we've seen it before proceeding
	msgHashes.push_back(make_pair(hash, currentTime));
	pthread_mutex_unlock(&hashLock);
	return messageFound;
}

/*
	This generates a 24 character digest based on an input string.
	It is intended for use with the AES cypher code, which requires
	binary keys of specific lengths to function.

	Note: The return value is placed on the stack and must be deallocated or there will be a memory leak!
*/
unsigned char* generate192BitDigest(const string &key)
{
	SHA512_CTX context;
	size_t buf_size = sizeof(unsigned char) * DIGEST_192_LENGTH;
	unsigned char* buf = new unsigned char (buf_size);
	bzero(buf, buf_size);
	if(!SHA512_Init(&context))
	{
		sprintf((char*)buf, "error");
		return buf;
	}

	if(!SHA512_Update(&context, (unsigned char*)key.c_str(), key.size()))
	{
		sprintf((char*)buf, "error");
		return buf;
	}

	if(!SHA512_Final(buf, &context))
	{
		sprintf((char*)buf, "error");
		return buf;
	}

	return buf;
}
