/*
	This file just coordinates different parts of the codebase, and should do
	little work itself.
*/

#include <pthread.h> // Needed for multithreading stuff
#include <iostream>

#include "networking.h"

using namespace std;

int main()
{
	pthread_t relayThread;
	pthread_create(&relayThread, NULL, startRelaying, NULL);
	cout << "Relaying started." << endl;
	// Anyone elses initialization code goes here

	// Main event loop gets input from user, encodes it, sends it off
	while(true)
	{
		// Other people's code goes here
	}
	return 0;
}
