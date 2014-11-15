// These are all needed for networking stuff
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <string.h> 	// Needed for memset and strcmp
#include <stdlib.h>  
#include <arpa/inet.h> 
#include <unistd.h> 

// Needed for debugging
#include <iostream>

// Example networking code found on https://stackoverflow.com/a/9204831

// Internal header files go here
#include "networking.h"

using namespace std;

const int BUFFER_SIZE = 100;

bool relayMessages() 
{  
    int sock_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_desc == -1)
    {
        cout << "cannot create socket!" << endl;
        return false;
    }

    struct sockaddr_in server;  
    memset(&server, 0, sizeof(server));  
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;  
    server.sin_port = htons(7777);  
    if (bind(sock_desc, (struct sockaddr*)&server, sizeof(server)) != 0)
    {
        cout << "cannot bind socket!" << endl;
        close(sock_desc);  
        return false;
    }

    if (listen(sock_desc, 20) != 0)
    {
        cout << "cannot listen on socket!" << endl;
        close(sock_desc);  
        return false;
    }

    struct sockaddr_in client;  
    memset(&client, 0, sizeof(client));  
    socklen_t len = sizeof(client); 
    int temp_sock_desc = accept(sock_desc, (struct sockaddr*)&client, &len);  
    if (temp_sock_desc == -1)
    {
        cout << "cannot accept client!" << endl;
        close(sock_desc);  
        return false;
    }

    char buf[BUFFER_SIZE];  
    int k;  

	for(int i = 0; i < BUFFER_SIZE; i++)
		buf[i] = 0;

    while(true) 
    {      
        k = recv(temp_sock_desc, buf, 100, 0);      
        if (k == -1)
        {
            cout << endl << "cannot read from client!" << endl;
            break;
        }
        if (k == 0)
        {
            cout << endl << "client disconnected." << endl;
            break;
        }
        if (k > 0)          
		{
            //printf("%*.*s", k, k, buf);  
			cout << buf;
		}
        if (strcmp(buf, "exit") == 0)         
            break;      
    }

    close(temp_sock_desc);  
    close(sock_desc);  

    cout << "server disconnected" << endl;
    return true;  
} 

void* startRelaying(void*)
{
	while(relayMessages() == true);
	return NULL;
}
