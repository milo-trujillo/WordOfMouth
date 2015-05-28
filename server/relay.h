#ifndef RELAY_H
#define RELAY_H

#include <iostream> // For string

// Stores the current configuration for this relay
class RelayConfig
{
	public:
		RelayConfig(int l, std::string h, int p, std::string a, 
			int im, int om, std::string logpath) 
		{
			listenPort = l;
			relayHost = h;
			relayPort = p;
			localAlias = a;
			incomingMessagePort = im;
			outgoingMessagePort = om;
			logFile = logpath;
		}
		int getListenPort() const { return listenPort; }
		int getRelayPort() const { return relayPort; }
		int getIncomingMessagePort() const { return incomingMessagePort; }
		int getOutgoingMessagePort() const { return outgoingMessagePort; }
		std::string getRelayHost() const { return relayHost; }
		std::string getAlias() const { return localAlias; }
		std::string getLogPath() const { return logFile; }
	private:
		int listenPort, relayPort, incomingMessagePort, outgoingMessagePort;
		std::string relayHost, localAlias, logFile;
};

// This should only be used by relay and message code
// DO NOT MODIFY NOT THREAD SAFE
extern RelayConfig* rc;

void reportNetworkError(int err);

// Will throw an exception if network config is impossible
// Please run before trying to start relaying
void validateRelayConfig(const RelayConfig &test);

// This is meant to be called from pthread_create, hence the return and args
// Takes a pointer to a 'RelayConfig'
void* startRelaying(void*);

// Returns true if successful, false otherwise
// Message must be text (base64 encode if you're sending binary data)
bool sendMessage(std::string msg);

#endif
