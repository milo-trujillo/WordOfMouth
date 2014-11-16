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
const int LISTEN_PORT = 7777;
const string RELAY_HOST = "10.0.0.2"; // Example, change later
const int RELAY_PORT = 1234; // Example, change later

// Sends a message to the next node
bool sendMessage(string msg)
{
	//string cyphertext = encryptForRelay(msg);
	return true;
}

void* handleMessage(void* arg)
{
	long sock_desc = (long)arg;
    char buf[BUFFER_SIZE];  
    int k;  

	// Flush the buffer in case something is left over from a prior read
	for(int i = 0; i < BUFFER_SIZE; i++)
		buf[i] = 0;

    while(true) 
    {      
        k = recv(sock_desc, buf, BUFFER_SIZE, 0);      
        if (k == -1)
        {
            printf("\n\tError reading from client.\n");
            break;
        }
        if (k == 0) // Client disconnected, time to exit the thread
            break;
        if (k > 0)          
            printf("%*.*s", k, k, buf); // 'cout' had buffering problems here
    }

    close(sock_desc);  
	return NULL;
}

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
    server.sin_port = htons(LISTEN_PORT);  
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

void* startRelaying(void*)
{
	while(relayMessages() == true);
	// If we get here then something has gone wrong and we can no longer relay
	// messages. It's time to shut down the node immediately.
	exit(1);
	return NULL;
}
