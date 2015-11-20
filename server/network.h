#ifndef NETWORK_H
#define NETWORK_H

#include "message.h"

/*
	The network module is responsible for everything to do with networking:
	Receiving messages from another node, sending messages to the next node,
	interacting with the client code, and so on.

	During initialization you *must* call startRelay or we won't forward
	messages to the next relay. If this node will have an active user attached
	then you also need to call startClientListen.

	Note that this module only handles network interaction, not interpretation.
	If you're looking for how we interpret data from the client look at client.h
*/

// NOTE: You *must* run setNextRelay before sendMessage!
void setNextRelay(char* relay, int portno);

// These two are for initialization
void startRelay(int port);
void startClientListen(int port);

// Once everything is set up, call this to forward a message to the next node.
void sendMessage(const Message& m);

#endif
