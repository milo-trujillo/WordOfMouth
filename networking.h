#ifndef NETWORKING_H
#define NETWORKING_H

#include <iostream>

struct RelayConfig
{
	RelayConfig(int l, std::string h, int p, std::string a) 
	{
		listenPort = l;
		relayHost = h;
		relayPort = p;
		localAlias = a;
	}
	int listenPort;
	std::string relayHost;
	int relayPort;
	std::string localAlias;
};

// Functions here are meant to be accessed from the main interlink code only

// Will throw an exception if network config is impossible
// Please run before trying to start relaying
void validateRelayConfig(const RelayConfig &test);

// This is meant to be called from pthread_create, hence the return and args
// Takes a pointer to a 'RelayConfig'
void* startRelaying(void*);

// Returns true if successful, false otherwise
bool sendMessage(std::string msg);

#endif
