/*
	This file just coordinates different parts of the codebase, and should do
	little work itself.
*/

#include <thread>

#include "log.h"
#include "daemonize.h"
#include "message.h"
#include "encode.h" // For debugging

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

	// Debugging message ascii encoding / decoding
	Message m("Plaintext demo message");
	log(LOG_INFO, "Startup successful.");
	log(LOG_DEBUG, "Attempting a dump of message contents...");
	string aa = m.getAsciiArmor();
	log(LOG_DEBUG, aa);
	log(LOG_DEBUG, "Attempting to decode message contents...");
	int len;
	char* decoded;
	bool success = asciiDecode(aa.c_str(), &len, &decoded);
	if( success )
		log(LOG_DEBUG, string(decoded));
	else
		log(LOG_DEBUG, "ERROR DECODING");

	return 0;
}
