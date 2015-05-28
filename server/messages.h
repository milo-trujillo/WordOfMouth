#ifndef MESSAGES_H
#define MESSAGES_H

// This is meant to be called from pthread_create, hence the return and args
// Listens on the network for messages from the user
void* listenForMessages(void*);

// If message is destined for us, displays the message and returns false
// If message needs to be delivered to next relay, returns true
bool deliverMessage(const std::string &msg);

// Displays a message to the user
// TODO: make internal, call from deliverMessage
void displayMessage(const std::string &msg);

#endif
