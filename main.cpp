/*
	This file just coordinates different parts of the codebase, and should do
	little work itself.
*/

#include <pthread.h> // Needed for multithreading stuff
#include <iostream>
#include <stdio.h> // For debugging

#include "networking.h"
#include "CipherAlias.h"
#include "keygen.h"

using namespace std;

int main(int argc, char** argv)
{
	RelayConfig rc(7777, "128.113.194.83", 7777, "Sha");
	pthread_t relayThread;
	pthread_create(&relayThread, NULL, startRelaying, (RelayConfig*)&rc);
	cout << "Relaying started." << endl;

	char *pubRelayKey, *privRelayKey;
	keyGen(&privRelayKey,&pubRelayKey);
	//
	// Anyone elses initialization code goes here
	//

	// This code tests the Cypher Alias code and verifies that we can encrypt
	// and decrypt a message
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

	// Main event loop gets input from user, encodes it, sends it off
	while(true)
	{
		string alias, cleartext, encrypted;
		cout << "Enter destination alias: ";
		getline(cin, alias);
		cout << "Enter message: ";
		getline(cin, cleartext);
		// Cypher crypto disabled until Staethe replaces it with AES
		/*
		cout << "Your message: " << cleartext << endl;
		encrypted = cipher_encrypt(alias, cleartext);
		// The next two lines are debugging
		printf("Encrypted, we see:\n%*.*s\n\n", encrypted.size(), encrypted.size(), encrypted.c_str());
		cout << "To confirm, decrypted is: " << cipher_decrypt(alias, encrypted) << endl;
		*/
		bool successful = sendMessage(cleartext); // TODO: Change to encrypted
		if( successful )
			cout << "Message sent." << endl;
	}
	keyDelete(privRelayKey,pubRelayKey);
	return 0;
}
