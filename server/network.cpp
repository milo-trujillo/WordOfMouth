#include <sys/types.h>  // Some basic structures
#include <sys/socket.h> // Primitive socket structures and functions
#include <netinet/in.h> // For some structures
#include <arpa/inet.h>  // For inet_htons
#include <netdb.h>      // For connect()
#include <string.h>     // For bzero

#include "network.h"
#include "log.h"

// Here are some globals we need for networking
struct hostent* nextRelay = NULL;
int nextRelayPort = -1;

void startRelay(int port)
{

}

void startClientListen(int port)
{

}

// This will set global information for the next relay's IP and port
void setNextRelay(char* relay, int portno)
{
	if( relay == NULL || portno < 1 )
	{
		log(LOG_ERROR, "Invalid relay data, relay must be non-null and portno > 0");
		return;
	}
	if( nextRelay != NULL )
		delete nextRelay;
	nextRelay = gethostbyname(relay);
	if( nextRelay == NULL )
		log(LOG_ERROR, "Invalid hostname for next relay!");
	nextRelayPort = portno;
}

// Creating a sockaddr_in requires lots of smelly typecasting
// So I stuck it in a corner over here.
// Returns success || failure
bool createRelaySocket(struct sockaddr_in& serv_addr)
{
	if( nextRelay == NULL )
	{
		log(LOG_ERROR, "Next relay is not set, cannot create server address!");
		return false;
	}
	bzero(reinterpret_cast<char*>(&serv_addr), sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy(reinterpret_cast<char*>(nextRelay->h_addr), 
		reinterpret_cast<char*>(&serv_addr.sin_addr.s_addr), 
		nextRelay->h_length);
	serv_addr.sin_port = htons(nextRelayPort);
	return true;
}

// Attempts to send a message using buffering
// Later this will also encrypt the message using the next relay's public key
void sendMessage(const Message& m)
{
	int sock, n;
	const int BUFFER_SIZE = 256;
	struct sockaddr_in serv_addr;

	char buffer[BUFFER_SIZE];
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if( sock < 0 )
	{
		log(LOG_ERROR, "Unable to create a socket!");
		return;
	}
	if( createRelaySocket(serv_addr) == false )
	{
		log(LOG_ERROR, "Got null for server address! Cannot send message!");
		return;
	}
	if( connect(sock, reinterpret_cast<const struct sockaddr*>(&serv_addr), 
		sizeof(serv_addr)) < 0 )
	{
		log(LOG_WARN, "Unable to connect to relay! Cannot send message!");
		return;
	}
	bzero(buffer, BUFFER_SIZE);

	int i;
	int bytesSent = 0;
	for( i = 0; i < m.size(); i++ )
	{
		buffer[i % BUFFER_SIZE] = m[i];
		// If buffer is filled then send it off
		if( (i + 1) % BUFFER_SIZE == 0 )
		{
			n = write(sock, buffer, BUFFER_SIZE);
			if( n < 0 )
			{
				log(LOG_WARN, "Problem sending message mid-way!");
				return;
			} else {
				bytesSent += n;
			}
		}
	}
	// Oh hey, is there still data?	Send that, too.
	if( (i + 1) % BUFFER_SIZE != 0 )
	{
		n = write(sock, buffer, i % BUFFER_SIZE);
		if( n < 0 )
		{
			log(LOG_WARN, "Problem sending message mid-way!");
			return;
		} else {
			bytesSent += n;
		}
	}

	// Log our great success!
	char msg[40];
	sprintf(msg, "Sent message, %d bytes", n);
	log(LOG_INFO, msg);
	close(sock);
}
