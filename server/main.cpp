/*
	This file just coordinates different parts of the codebase, and should do
	little work itself.
*/

#include <thread>

#include "log.h"
#include "daemonize.h"
#include "message.h"

using namespace std;

int main(int argc, char** argv)
{
	if( daemonize() == false )
	{
		log(LOG_ERROR, "Unable to daemonize - aborting");
		return 1;
	}
	// Note: Daemonizing closes all file handles, do *not* put logging first
	if( startLogging("/tmp/wordofmouth-log.txt") == false )
		return 1;

	log(LOG_INFO, "Done with all tasks, exiting...");

	return 0;
}
