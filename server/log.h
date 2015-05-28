#ifndef LOG_H
#define LOG_H

#include <iostream> // For strings

/*
	Here are some logging utilities so the rest of the code doesn't have to
	worry about whether we're using log files, stdout, a socket, etc.

	They use mutexes internally, so they can be considered threadsafe and guaranteed
	to print messages without interruptions.
*/

const bool DEBUG_ENABLED = true;

bool startLogging(std::string logpath);

void logErr(std::string s);
void logWarn(std::string s);
void logDebug(std::string s);
void logInfo(std::string s);

#endif
