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
typedef enum {LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR} logLevel;

bool startLogging(std::string logpath);
void log(logLevel, std::string msg);

#endif
