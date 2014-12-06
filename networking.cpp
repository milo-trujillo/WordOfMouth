// These are all needed for networking stuff
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h> 
#include <unistd.h> 

#include <string.h> 	// Needed for memset and strcmp

#include <pthread.h> // Needed for multithreading
#include <iostream> // Needed for C++ strings

#include <list>		// STL linked lists for tracking sent messages

// For error conditions
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

// Example networking code found on https://stackoverflow.com/a/9204831

// Internal header files go here
#include "networking.h"
#include "CipherAlias.h"
#include "keygen.h"
#include "hash.h"
#include "log.h"

using namespace std;

const int BUFFER_SIZE = 100; // How many characters to read from network at once
const unsigned int MAX_MESSAGE_HISTORY = 50; // For preventing looping messages

// Global vars are bad, but at least this is only global to the networking code
RelayConfig* rc = NULL;
list<string> msgHashes; // Tracks the hashes of every message we've seen recently
pthread_mutex_t screenLock; // Don't print two messages to the screen at once
pthread_mutex_t hashLock; // Prevent thread collision accessing msgHashes

void reportNetworkError(int err)
{
	switch(err)
	{
		case ECONNREFUSED:
			logErr("[Errno] Connection refused");
			break;
		case ENETDOWN:
		case ENETUNREACH:
			logErr("[Errno] Routing problem");
			break;
		case EACCES:
			logErr("[Errno] Permission denied");
			break;
		default:
			char errnumber[4];
			sprintf(errnumber, "%d", err);
			string errstring = "[Errno] Unknown error code# ";
			errstring += errnumber;
			logErr(errstring);
	}
}

// Returns if a message has been seen previously (and should be dropped)
bool messageSeen(const string &msg)
{
	string hash = genHash(msg);
	pthread_mutex_lock(&hashLock);

	// Check if we've seen the message yet
	list<string>::iterator hashItr = msgHashes.begin();
	for(; hashItr != msgHashes.end(); hashItr++ )
	{
		if( *hashItr == hash )
		{
			pthread_mutex_unlock(&hashLock);
			return true;
		}
	}
	// No? Alright, log that we've seen it before proceeding
	if( msgHashes.size() == MAX_MESSAGE_HISTORY )
		msgHashes.pop_front();
	msgHashes.push_back(hash);
	pthread_mutex_unlock(&hashLock);
	return false;
}

// Sends a message to the next node
bool sendMessage(string msg)
{
	if( msg.size() == 0 ) return true; // No sense in sending an empty message
	if( rc == NULL )
	{
		logErr("Sending a message before initialization!");
		return false;
	}

	// TODO: Use keys to encrypt message to the next relay in the chain
	//string cyphertext = encryptForRelay(msg);

	// Note: We attach a zero to the end of the message to indicate end of
	// transfer, in case the socket buffer is being filled with something else
	msg.push_back(0);

	// If anything goes wrong sending a message then the parent
	// _probably_ wants to kill the process, but we'll leave it up to them
	int sock_desc = socket(AF_INET, SOCK_STREAM, 0);
	if( sock_desc == -1 )
	{
		logErr("Couldn't create socket to send or relay message!");
		reportNetworkError(errno);
		return false;
	}

	struct sockaddr_in conn;
	memset(&conn, 0, sizeof(conn));
	conn.sin_family = AF_INET;
	conn.sin_addr.s_addr = inet_addr(rc->relayHost.c_str());
	conn.sin_port = htons(rc->relayPort); // Host to network byte order

	if( connect(sock_desc, (sockaddr*)&conn, sizeof(conn)) != 0 )
	{
		logErr("Cannot connect to next relay!");
		reportNetworkError(errno);
		close(sock_desc);
		return false;
	}

	// TODO: Encrypt w/ keys before sending once cyphers are debugged
	int k = send(sock_desc, msg.c_str(), msg.size(), 0);
	if( k == -1 )
	{
		logErr("Could not send message to relay!");
		reportNetworkError(errno);
		close(sock_desc);
		return false;
	}

	close(sock_desc);
	return true;
}

// Handles an individual incoming message, once the socket has already been
// opened
void* handleMessage(void* arg)
{
	long sock_desc = (long)arg;
	string msg; // This is a buffer before relaying the message
	char buf[BUFFER_SIZE]; 
	int k;  

	while(true) 
	{      
		// Flush the buffer in case something is left over from a prior read
		for(int i = 0; i < BUFFER_SIZE; i++)
			buf[i] = 0;

		k = recv(sock_desc, buf, BUFFER_SIZE, 0);      

		if (k == -1)
		{
			logErr("Error reading from client.");
			reportNetworkError(errno);
			break;
		}
		if (k == 0) // Client disconnected, time to exit the thread
			break;
		if (k > 0)
			msg += buf; // Make a C++ string from the C String
	}

	// Further work has nothing to do with the incoming connection
	close(sock_desc);  

	// TODO: Determine if the message is destined for us or should be
	// forwarded. This requires the PGP crypto code to decode the message
	// and the cypher crypto code to see if the message is an attempted
	// key exchange with us.

	// TODO: Decode the data using our private key
	// Once the program is finished we'll be using public/private keypairs
	// for communication between nodes, and will need to decode with our keys.

	if( messageSeen(msg) )
	{
		pthread_mutex_lock(&screenLock);
		logDebug("Warning! Detected looped message!");
		pthread_mutex_unlock(&screenLock);
		return NULL;
	}

	// Cyphers disabled until Staethe updates that code
	/*
	printf("Without processing, I see: %*.*s\n\n\n", msg.size(), msg.size(), msg.c_str());
	printf("Post processing, I see: %*.*s\n\n\n", cipher_decrypt(rc->localAlias, msg).size(), cipher_decrypt(rc->localAlias, msg).size(), cipher_decrypt(rc->localAlias, msg).c_str());

	// This code checks using only cyphers if the received message is destined
	// for us or needs to be forwarded to the next node
	if( data_decoded(cipher_decrypt(rc->localAlias, msg)) )
	{
		string cleartext = cipher_decrypt(rc->localAlias, msg);
		pthread_mutex_lock(&screenLock);
		printf("Message Received\n");
		printf("================\n");
		// 'cout' had buffering problems here
		int msgLength = cleartext.size();
		printf("%*.*s", msgLength, msgLength, cleartext.c_str()); 
		printf("\n"); // Force the screen to flush
		pthread_mutex_unlock(&screenLock);
	}
	else
	{
		logDebug("Relaying message");
		sendMessage(msg);
	}
	*/

	// This is temporary code to test relaying until cyphers are working again
	pthread_mutex_lock(&screenLock);
	printf("Message Received: %*.*s\n", (int)msg.size(), (int)msg.size(), msg.c_str());
	pthread_mutex_unlock(&screenLock);
	sendMessage(msg);

	return NULL;
}

// Does some initial setup, and for each incoming connection kicks off a thread
// running handleMessage
bool relayMessages() 
{  
	int sock_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_desc == -1)
	{
		logErr("Cannot create socket!");
		reportNetworkError(errno);
		return false;
	}

	struct sockaddr_in server;  
	memset(&server, 0, sizeof(server));  
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;  
	server.sin_port = htons(rc->listenPort);  
	if (bind(sock_desc, (sockaddr*)&server, sizeof(server)) != 0)
	{
		logErr("Cannot bind socket!");
		reportNetworkError(errno);
		close(sock_desc);  
		return false;
	}

	if (listen(sock_desc, 20) != 0)
	{
		logErr("Cannot listen on socket!");
		reportNetworkError(errno);
		close(sock_desc);  
		return false;
	}

	while(true)
	{
		struct sockaddr_in client;  
		memset(&client, 0, sizeof(client));  
		socklen_t len = sizeof(client); 
		long client_sock_desc = accept(sock_desc, (sockaddr*)&client, &len); 
		if (client_sock_desc == -1)
		{
			logErr("Cannot accept client!");
			reportNetworkError(errno);
			close(sock_desc);  
			return false;
		}

		pthread_t clientThread;
		pthread_create(&clientThread, NULL, handleMessage, (void*)client_sock_desc);
	}

	close(sock_desc); // Close the listen sock once we're done
	return true;  
} 

bool isValidIpAddress(const char *ipAddress)
{
	struct sockaddr_in sa;
	int result = inet_pton(AF_INET, ipAddress, &(sa.sin_addr));
	return result != 0;
}

void validateRelayConfig(const RelayConfig &test)
{
	if( !isValidIpAddress(test.relayHost.c_str()) )
	{
		logErr("Relay host must be an IP address!");
		throw "bad config";
	}
}

void* startRelaying(void* arg)
{
	rc = (RelayConfig*)arg;
	while(relayMessages() == true);
	// If we get here then something has gone wrong and we can no longer relay
	// messages. It's time to shut down the node immediately.
	logErr("Unexpected problem relaying! Emergency shutdown underway.");
	exit(1);
	return NULL;
}
