/*
	This file just coordinates different parts of the codebase, and should do
	little work itself.
*/

#include <pthread.h> // Needed for multithreading stuff
#include <iostream>

#include "networking.h"
#include "CipherAlias.h"

using namespace std;

int main(int argc, char** argv)
{
	RelayConfig rc(7777, "10.0.0.1", 7777, "Milo");
	pthread_t relayThread;
	pthread_create(&relayThread, NULL, startRelaying, (RelayConfig*)&rc);
	cout << "Relaying started." << endl;

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
		
	// Main event loop gets input from user, encodes it, sends it off
	while(true)
	{
		string alias, cleartext;
		cout << "Enter destination alias: ";
		getline(cin, alias);
		cout << "Enter message: ";
		getline(cin, cleartext);
		bool successful = sendMessage(cipher_encrypt(alias, cleartext));
		if( successful )
			cout << "Message sent." << endl;
	}
	return 0;
}
