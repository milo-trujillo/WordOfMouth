/*
	This file just coordinates different parts of the codebase, and should do
	little work itself.
*/

#include <pthread.h> // Needed for multithreading stuff
#include <iostream>
#include <stdio.h> // For debugging

#include "NextNodeInfo.h"
#include "relay.h"
#include "messages.h"
#include "CipherAlias.h"
#include "keygen.h"
#include "log.h"

using namespace std;

int main(int argc, char** argv)
{
	//
	// Before anything else we need to read existing configuration (if any)
	// from disk or prompt the user for new information if there's no config
	//
	RelayConfig rc = inputPassword();
	//
	// Next we kick off the networking code. All later aspects of the program
	// either call or are called from the networking code, so this needs to be
	// started first
	//
	validateRelayConfig(rc);
	pthread_t relayThread;
	pthread_t messageThread;
	pthread_create(&relayThread, NULL, startRelaying, (RelayConfig*)&rc);
	cout << "Relaying started." << endl;
	pthread_create(&messageThread, NULL, listenForMessages, (RelayConfig*)&rc);
	cout << "Listening for messages." << endl;
	//
	// Anyone elses initialization code goes here
	//
	char *pubRelayKey, *privRelayKey;
	keyGen(&privRelayKey,&pubRelayKey);

	// This code tests the Cypher Alias code and verifies that we can encrypt
	// and decrypt a message
	// Cypher crypto disabled until Staethe replaces it with AES
	/*
	string message = "Hello Worlds, boop beep,bop bepbooop";
	string encrypted = cipher_encrypt(rc.localAlias, message);
	string decrypted = cipher_decrypt(rc.localAlias, encrypted);
	cout << "Orig: " << message << endl;
	cout << "Done: " << decrypted << endl;
	*/

	// TODO: Switch to daemon
	// This program no longer uses stdin/stdout for user interaction, so there
	// is no longer a need to be associated with a terminal at all. We should
	// daemonize the whole thing and log to a file or syslog.

	// We should never pass this line (relay and message code loops until error)
	pthread_join(messageThread, NULL);
	keyDelete(privRelayKey,pubRelayKey);
	return 0;
}
