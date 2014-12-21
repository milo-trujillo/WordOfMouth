#include <openssl/sha.h>    // For Sha hashes
#include <iostream>         // For strings
#include <stdio.h>          // For sprintf
#include <strings.h>		// For bzero
#include <time.h>           // For epoch time() function

#include <list>             // STL linked lists for tracking sent messages
#include <map>              // For pairs

using namespace std;

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
	unsigned char buf[SHA512_DIGEST_LENGTH];
	if(!SHA512_Init(&context))
		return "error";

	if(!SHA512_Update(&context, (unsigned char*)input.c_str(), input.size()))
		return "error";

	if(!SHA512_Final(buf, &context))
		return "error";

	string hash;
	for( int i = 0; i < SHA512_DIGEST_LENGTH; i++ )
		hash.push_back((char)buf[i]);
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
