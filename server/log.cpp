#include <iostream>
#include <fstream>
#include <mutex>

#include "log.h"

using namespace std;

static ofstream logfile;
static bool loggingInitialized = false;;
static mutex logLock; // Don't print two messages to the screen at once

bool startLogging(string logpath)
{
	bool succeeded = false;
	logLock.lock();
	logfile.open(logpath.c_str());
	if( logfile.is_open() )
	{
		loggingInitialized = true;
		succeeded = true;
	}
	else
		cerr << "Error opening log file!" << endl;
	logLock.unlock();
	return succeeded;
}

void log(logLevel l, string s)
{
	ostream& log = (loggingInitialized) ? logfile : cerr;
	logLock.lock();
	switch(l)
	{
		case LOG_DEBUG:
			log << "Debug: " << s << endl;
			break;
		case LOG_INFO:
			log << "Info: " << s << endl;
			break;
		case LOG_WARN:
			log << "WARNING: " << s << endl;
			break;
		case LOG_ERROR:
			log << "ERROR: " << s << endl;
			break;
	}
	logLock.unlock();
}
