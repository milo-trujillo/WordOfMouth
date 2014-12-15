#include <iostream>
#include <fstream>
#include <pthread.h> // Needed for multithreading

#include "log.h"

using namespace std;

static ofstream logfile;
static bool loggingInitialized = false;;
static pthread_mutex_t logLock; // Don't print two messages to the screen at once

bool startLogging(string logpath)
{
	bool succeeded = false;
	pthread_mutex_lock(&logLock);
	logfile.open(logpath);
	if( logfile.is_open() )
	{
		loggingInitialized = true;
		succeeded = true;
	}
	else
		cerr << "Error opening log file!" << endl;
	pthread_mutex_unlock(&logLock);
	return succeeded;
}

void logErr(string s)
{
	pthread_mutex_lock(&logLock);
	if( loggingInitialized )
		logfile << "ERROR: " << s << endl;
	else
		cerr << "ERROR: " << s << endl;
	pthread_mutex_unlock(&logLock);
}

void logWarn(string s)
{
	pthread_mutex_lock(&logLock);
	if( loggingInitialized )
		logfile << "WARNING: " << s << endl;
	else
		cerr << "WARNING: " << s << endl;
	pthread_mutex_unlock(&logLock);
}

void logDebug(string s)
{
	if( DEBUG_ENABLED )
	{
		pthread_mutex_lock(&logLock);
		if( loggingInitialized )
			logfile << "Debug: " << s << endl;
		else
			cout << "Debug: " << s << endl;
		pthread_mutex_unlock(&logLock);
	}
}

void logInfo(string s)
{
	pthread_mutex_lock(&logLock);
	if( loggingInitialized )
		logfile << "Log: " << s << endl;
	else
		cout << "Log: " << s << endl;
	pthread_mutex_unlock(&logLock);
}
