// These are all needed for networking stuff
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h> 
#include <unistd.h> 

#include <string.h> 	// Needed for memset and strcmp

#include <pthread.h> // Needed for multithreading
#include <iostream> // Needed for C++ strings

#include <errno.h>
#include <stdlib.h>

#include "relay.h"
#include "messages.h"
#include "log.h"
#include "cypher.h"

using namespace std;

const int BUFFER_SIZE = 100; // How many characters to read from network at once
const char* LOCALHOST = "127.0.0.1";

pthread_mutex_t displayLock; // Don't print two messages at once

void displayMessage(const string& msg)
{
	pthread_mutex_lock(&displayLock);

	int sock_desc = socket(AF_INET, SOCK_STREAM, 0);
	if( sock_desc == -1 )
	{
		logErr("Couldn't create socket to display message!");
		reportNetworkError(errno);
		pthread_mutex_unlock(&displayLock);
	}

	struct sockaddr_in conn;
	memset(&conn, 0, sizeof(conn));
	conn.sin_family = AF_INET;
	conn.sin_addr.s_addr = inet_addr(LOCALHOST);
	conn.sin_port = htons(rc->getOutgoingMessagePort());

	if( connect(sock_desc, (sockaddr*)&conn, sizeof(conn)) != 0 )
	{
		logWarn("Cannot connect to display port (message ignored)");
		reportNetworkError(errno);
		close(sock_desc);
		pthread_mutex_unlock(&displayLock);
		return;
	}

	int k = send(sock_desc, msg.c_str(), msg.size(), 0);
	if( k == -1 )
	{
		logErr("Problem during display of message!");
		reportNetworkError(errno);
		close(sock_desc);
		pthread_mutex_unlock(&displayLock);
		return;
	}
	close(sock_desc);
	pthread_mutex_unlock(&displayLock);
}

// Attempts to unwrap any layers of crypto to determine if the message
// belongs to us. If it does, display and return false. Else return true.
bool deliverMessage(const string &msg)
{
	/*
		Right now all we're checking is whether the message was cyphered
		with our username. As more complexity is added we'll need to
		attempt decrypting with each conversation key, and any key-exchange
		pairs we still have.
	*/
	cypheredMessage cm;
	cm.iv = base64Decode(msg.substr(0, msg.find('\n')));
	cm.msg = base64Decode(msg.substr(msg.find('\n') + 1));
	pair<bool, string> ct = decypherMessage(cm, rc->getAlias());
	if( ct.first == true )
	{
		logDebug("Displaying decrypted message");
		displayMessage(ct.second);
		return false;
	}
	else
	{
		logDebug("Relaying message");
		return true;
	}
}

// Handles an individual incoming message, once the socket has already been
// opened
void* handleUserMessage(void* arg)
{
	long sock_desc = (long)arg;
	string destinationAlias;
	string msg; // This is a buffer before relaying the message
	char buf[BUFFER_SIZE]; 
	int k;  
	bool readUsername = false;
	bool errorReading = false;

	while(!readUsername) 
	{      
		// Flush the buffer in case something is left over from a prior read
		char buffer = 0;
		k = recv(sock_desc, &buffer, 1, 0);      

		if (k == -1)
		{
			logErr("Error reading from user.");
			reportNetworkError(errno);
			break;
		}
		if (k == 0) // Client disconnected, time to exit the thread
		{
			errorReading = true;
			break;
		}
		if (k > 0 && buffer != '\n')
			destinationAlias.push_back(buffer); 
		else if( buffer == '\n' )
			readUsername = true;
	}

	while(errorReading == false)
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

	cypheredMessage cm = cypherMessage(msg, destinationAlias);
	if( errorReading == false )
		sendMessage(base64Encode(cm.iv) + "\n" + base64Encode(cm.msg));

	// Further work has nothing to do with the incoming connection
	close(sock_desc);  
	return NULL;
}

bool readMessage()
{
	int sock_desc = socket(AF_INET, SOCK_STREAM, 0);
	if(sock_desc == -1)
	{
		logErr("Problem listening for messages!");
		logDebug("Error creating socket in readMessage");
		reportNetworkError(errno);
		return false;
	}

	struct sockaddr_in server;  
	memset(&server, 0, sizeof(server));  
	server.sin_family = AF_INET;
	// In the future maybe we want to make this a configurable setting, or add
	// some kind of crypto to message submissions. Until then only accepting
	// submitted messages from localhost, so users can't do dangerous things.
	server.sin_addr.s_addr = inet_addr(LOCALHOST); // Only listen on loopback
	server.sin_port = htons(rc->getIncomingMessagePort());  
	if(bind(sock_desc, (sockaddr*)&server, sizeof(server)) != 0)
	{
		logErr("Problem listening for messages!");
		logDebug("Cannot bind socket in readMessage");
		reportNetworkError(errno);
		close(sock_desc);  
		return false;
	}

	if(listen(sock_desc, 20) != 0)
	{
		logErr("Problem listening for messages!");
		logDebug("Cannot listen on socket in readMessage");
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
		if(client_sock_desc == -1)
		{
			logErr("Problem listening for messages!");
			logDebug("Cannot accept client in readMessage");
			reportNetworkError(errno);
			close(sock_desc);  
			return false;
		}

		pthread_t clientThread;
		pthread_create(&clientThread, NULL, handleUserMessage, (void*)client_sock_desc);
	}

	close(sock_desc); // Close the listen sock once we're done
	return true;  
}

void* listenForMessages(void* arg)
{
	// Argument currently unused
	//RelayConfig* rc = (RelayConfig*)arg;
	while(readMessage() == true);
	// If we get here then something has gone wrong and we can no longer relay
	// messages. It's time to shut down the node immediately.
	logErr("Unexpected problem listening for messages! Emergency shutdown underway.");
	exit(1);
	return NULL;
}
