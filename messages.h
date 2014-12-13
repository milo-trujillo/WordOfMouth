#ifndef MESSAGES_H
#define MESSAGES_H

// This is meant to be called from pthread_create, hence the return and args
// Takes a pointer to a 'RelayConfig'
// Listens on the network for messages from the user
void* listenForMessages(void*);

// Displays a message to the user (over the network)
void displayMessage(const std::string& msg);

#endif
