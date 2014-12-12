#include <iostream>
#include <pthread.h> // Needed for multithreading

#include "log.h"

pthread_mutex_t logLock; // Don't print two messages to the screen at once

using namespace std;

void logErr(string s)
{
	pthread_mutex_lock(&logLock);
	cerr << "ERROR: " << s << endl;
	pthread_mutex_unlock(&logLock);
}

void logWarn(string s)
{
	pthread_mutex_lock(&logLock);
	cerr << "WARNING: " << s << endl;
	pthread_mutex_unlock(&logLock);
}

void logDebug(string s)
{
	if( DEBUG_ENABLED )
	{
		pthread_mutex_lock(&logLock);
		cout << "Debug: " << s << endl;
		pthread_mutex_unlock(&logLock);
	}
}

void logInfo(string s)
{
	pthread_mutex_lock(&logLock);
	cout << "Log: " << s << endl;
	pthread_mutex_unlock(&logLock);
}
