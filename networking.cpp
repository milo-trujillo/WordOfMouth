// These are all needed for networking stuff
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <string.h> 	// Needed for memset and strcmp
#include <stdlib.h>  
#include <arpa/inet.h> 
#include <unistd.h> 

#include <pthread.h> // Needed for multithreading
#include <iostream> // Needed for C++ strings

// For error conditions
#include <stdlib.h>
#include <stdio.h>

// Example networking code found on https://stackoverflow.com/a/9204831

// Internal header files go here
#include "networking.h"

using namespace std;

const int BUFFER_SIZE = 100;

// Global vars are bad, but at least this is only global to the networking code
RelayConfig* rc = NULL;

// Ideally these three should come from configuration we're given
const int LISTEN_PORT = 7777;
const string RELAY_HOST = "127.0.0.1"; // Example, change later
const int RELAY_PORT = 1234; // Example, change later

// Sends a message to the next node
bool sendMessage(string msg)
{
	if( msg.size() == 0 ) return true; // No sense in sending an empty message
	if( rc == NULL )
	{
		printf("Error: Sending a message before initialization!\n");
		return false;
	}

	//string cyphertext = encryptForRelay(msg);

	// If anything goes wrong sending a message then the parent
	// _probably_ wants to kill the process, but we'll leave it up to them
	int sock_desc = socket(AF_INET, SOCK_STREAM, 0);
	if( sock_desc == -1 )
	{
		printf("Error: Couldn't create socket to send or relay message!\n");
		return false;
	}

	struct sockaddr_in conn;
	memset(&conn, 0, sizeof(conn));
	conn.sin_family = AF_INET;
	conn.sin_addr.s_addr = inet_addr(rc->relay_host.c_str());
	conn.sin_port = htons(rc->relay_port); // Host to network byte order

	if( connect(sock_desc, (struct sockaddr*)&conn, sizeof(conn)) != 0 )
	{
		printf("Error: Cannot connect to next relay!\n");
		close(sock_desc);
		return false;
	}

	// TODO: Change this line to send cyphertext once encryption is implemented
	int k = send(sock_desc, msg.c_str(), msg.size(), 0);
	if( k == -1 )
	{
		printf("Error: Could not send message to relay!\n");
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

		// TODO: Determine if the message is destined for us or should be
		// forwarded. This requires the PGP crypto code to decode the message
		// and the cypher crypto code to see if the message is an attempted
		// key exchange with us.

        if (k == -1)
        {
            printf("\n\tError reading from client.\n");
            break;
        }
        if (k == 0) // Client disconnected, time to exit the thread
            break;
        if (k > 0)
		{
			msg += buf; // Make a C++ string from the C String
            printf("%*.*s", k, k, buf); // 'cout' had buffering problems here
		}
    }
	sendMessage(msg);

    close(sock_desc);  
	return NULL;
}

// Does some initial setup, and for each incoming connection kicks off a thread
// running handleMessage
bool relayMessages() 
{  
    int sock_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_desc == -1)
    {
        printf("Cannot create socket!\n");
        return false;
    }

    struct sockaddr_in server;  
    memset(&server, 0, sizeof(server));  
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;  
    server.sin_port = htons(rc->listen_port);  
    if (bind(sock_desc, (struct sockaddr*)&server, sizeof(server)) != 0)
    {
        printf("Error: Cannot bind socket!\n");;
        close(sock_desc);  
		return false;
    }

    if (listen(sock_desc, 20) != 0)
    {
        printf("Error: Cannot listen on socket!\n");
        close(sock_desc);  
        return false;
    }

	while(true)
	{
		struct sockaddr_in client;  
		memset(&client, 0, sizeof(client));  
		socklen_t len = sizeof(client); 
		long client_sock_desc = accept(sock_desc, (struct sockaddr*)&client, &len); 
		if (client_sock_desc == -1)
		{
			printf("Error: Cannot accept client!\n");
			close(sock_desc);  
			return false;
		}

		pthread_t clientThread;
		pthread_create(&clientThread, NULL, handleMessage, (void*)client_sock_desc);
	}

    close(sock_desc); // Close the listen sock once we're done
    return true;  
} 

void* startRelaying(void* arg)
{
	rc = (RelayConfig*)arg;
	while(relayMessages() == true);
	// If we get here then something has gone wrong and we can no longer relay
	// messages. It's time to shut down the node immediately.
	exit(1);
	return NULL;
}
