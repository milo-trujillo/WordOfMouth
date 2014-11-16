#ifndef NETWORKING_H
#define NETWORKING_H

#include <iostream>

// Functions here are meant to be accessed from the main interlink code only

// This is meant to be called from pthread_create, hence the return and args
void* startRelaying(void*);

// Returns true if successful, false otherwise
bool sendMessage(std::string msg);

// bool initNetworking(string ip, int port);

#endif
