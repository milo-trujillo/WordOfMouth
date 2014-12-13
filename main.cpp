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
	RelayConfig rc = inputPassword();
	validateRelayConfig(rc);
	pthread_t relayThread;
	pthread_t messageThread;
	pthread_create(&relayThread, NULL, startRelaying, (RelayConfig*)&rc);
	cout << "Relaying started." << endl;
	pthread_create(&messageThread, NULL, listenForMessages, (RelayConfig*)&rc);
	cout << "Listening for messages." << endl;

	char *pubRelayKey, *privRelayKey;
	keyGen(&privRelayKey,&pubRelayKey);
	//
	// Anyone elses initialization code goes here
	//

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
	// Instead of reading and writing everything to the terminal
	// we can read and write with a local socket, enabling the use of a "client"
	// program or making it easy to just log everything to a file when the user
	// is AFK. Then this program can daemonize and run 24/7.

	// We should never pass this line
	pthread_join(messageThread, NULL);
	keyDelete(privRelayKey,pubRelayKey);
	return 0;
}
