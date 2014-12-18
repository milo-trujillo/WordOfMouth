// These are all needed for networking stuff
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h> 
#include <unistd.h> 

#include <string.h>       // Needed for memset and strcmp

#include <pthread.h>      // Needed for multithreading
#include <iostream>       // Needed for C++ strings

// For error conditions
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

// Example networking code found on https://stackoverflow.com/a/9204831

// Internal header files go here
#include "relay.h"
#include "messages.h"
#include "keygen.h"
#include "hash.h"
#include "log.h"

using namespace std;

static const int BUFFER_SIZE = 100; // How many bytes to read from network
RelayConfig* rc = NULL;

// This knows how to interpret POSIX 'errno' errors and logs them accordingly
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

// Sends a message to the next node
// We handle relay key encryption here, but expect cypher or conversation keys
// to have already been applied to the message.
bool sendMessage(string msg)
{
	if( msg.size() == 0 ) return true; // No sense in sending an empty message
	if( rc == NULL )
	{
		logErr("Attempting to send a message before initialization!");
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
	conn.sin_addr.s_addr = inet_addr(rc->getRelayHost().c_str());
	conn.sin_port = htons(rc->getRelayPort()); // Host to network byte order

	if( connect(sock_desc, (sockaddr*)&conn, sizeof(conn)) != 0 )
	{
		logErr("Cannot connect to next relay!");
		reportNetworkError(errno);
		close(sock_desc);
		return false;
	}

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

// Handles an individual incoming relay message, once the socket has already
// been opened
void* handleMessage(void* arg)
{
	long sock_desc = (long)arg;
	string msg; // This is a buffer for storing the assembled message
	char buf[BUFFER_SIZE]; // For incrementally reading from the socket
	int k; // For tracking status of the socket

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

	// TODO: Unwrap first layer of crypto (relay keys)
	// Then determine if message is destined for us: 
	//   * if it is, deliver it
	//   * if it is not, forward it.
	// To determine message destination:
	//   * Cypher message with our alias, see if it's an attempted key exchange
	//   * Check if message is signed by any active conversation keypairs
	//      * If it is, unsign and decrypt with appropriate private convo key

	if( messageSeen(msg) )
	{
		logDebug("Warning! Detected looped message!");
		return NULL;
	}

	// TODO: Disable the temporary code below and enable the immediately
	// following code block once we're ready
	/*
	// This will take care of any kind of crypto layers the message may
	// have, display it if appropriate, and tell us whether to relay it
	if( deliverMessage(msg) )
		sendMessage(msg);
	*/

	// This is temporary code to test relaying until cyphers and keys are working
	displayMessage(msg);
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

	// Bind to listen port, standby to receive incoming relay connections
	struct sockaddr_in server;  
	memset(&server, 0, sizeof(server));  
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;  
	server.sin_port = htons(rc->getListenPort());  
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
		pthread_create(&clientThread, NULL, 
			handleMessage, (void*)client_sock_desc);
	}

	close(sock_desc); // Close the listen sock on error or program exit
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
	if( DEBUG_ENABLED )
	{
		// Nasty, nasty typecasting to strings
		logDebug("Relay: " + test.getRelayHost());
		string debug = "Relay Port: ";
		char port[8];
		sprintf(port, "%d", test.getRelayPort());
		debug += (const char*) port;
		logDebug(debug);
		sprintf(port, "%d", test.getListenPort());
		debug = "Listen Port: ";
		debug += (const char*) port;
		logDebug(debug);
		logDebug("Alias: " + test.getAlias());
		sprintf(port, "%d", test.getOutgoingMessagePort());
		debug = "Display Port: ";
		debug += (const char*) port;
		logDebug(debug);
		sprintf(port, "%d", test.getIncomingMessagePort());
		debug = "Incoming Message Port: ";
		debug += (const char*) port;
		logDebug(debug);
		logDebug("Log file: " + test.getLogPath());
	}
	if( !isValidIpAddress(test.getRelayHost().c_str()) )
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
