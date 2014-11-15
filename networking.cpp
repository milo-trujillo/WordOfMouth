// These are all needed for networking stuff
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <string.h> 	// Needed for memset and strcmp
#include <stdlib.h>  
#include <arpa/inet.h> 
#include <unistd.h> 

#include <pthread.h> // Needed for multithreading

// Needed for debugging
#include <stdio.h>

// Example networking code found on https://stackoverflow.com/a/9204831

// Internal header files go here
#include "networking.h"

const int BUFFER_SIZE = 100;
const int LISTEN_PORT = 7777;

void* handleMessage(void* arg)
{
	long sock_desc = (long)arg;
    char buf[BUFFER_SIZE];  
    int k;  

	for(int i = 0; i < BUFFER_SIZE; i++)
		buf[i] = 0;

    while(true) 
    {      
        k = recv(sock_desc, buf, BUFFER_SIZE, 0);      
        if (k == -1)
        {
            printf("\ncannot read from client!\n");
            break;
        }
        if (k == 0)
        {
            printf("\nclient disconnected.\n");
            break;
        }
        if (k > 0)          
		{
            printf("%*.*s", k, k, buf);  
			//cout << buf;
		}
        if (strcmp(buf, "exit") == 0)         
            break;      
    }

    close(sock_desc);  
	return NULL;
}

bool relayMessages() 
{  
    int sock_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_desc == -1)
    {
        printf("cannot create socket!\n");
        return false;
    }

    struct sockaddr_in server;  
    memset(&server, 0, sizeof(server));  
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;  
    server.sin_port = htons(LISTEN_PORT);  
    if (bind(sock_desc, (struct sockaddr*)&server, sizeof(server)) != 0)
    {
        printf("cannot bind socket!\n");;
        close(sock_desc);  
        return false;
    }

    if (listen(sock_desc, 20) != 0)
    {
        printf("cannot listen on socket!\n");
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
			printf("cannot accept client!\n");
			close(sock_desc);  
			return false;
		}

		pthread_t clientThread;
		pthread_create(&clientThread, NULL, handleMessage, (void*)client_sock_desc);
		//handleMessage((void*)client_sock_desc);
	}

    close(sock_desc);  
    printf("server disconnected\n");
    return true;  
} 

void* startRelaying(void*)
{
	while(relayMessages() == true);
	return NULL;
}
