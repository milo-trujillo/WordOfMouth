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
	RelayConfig rc(7777, "10.0.0.1", 7777);
	pthread_t relayThread;
	pthread_create(&relayThread, NULL, startRelaying, (RelayConfig*)&rc);
	cout << "Relaying started." << endl;

	//
	// Anyone elses initialization code goes here
	//

	//Test Cipher
	string alias = "Jazmyn";

	// This code tests the Cypher Alias code and verifies that we can encrypt
	// and decrypt a message
	/*
	string message = "Hello Worlds, boop beep,bop bepbooop";
	string encrypted = cipher_encrypt(alias, message);
	string decrypted = cipher_decrypt(alias, encrypted);
	cout << "Orig: " << message << endl;
	cout << "Done: " << decrypted << endl;
	*/
		
	// Main event loop gets input from user, encodes it, sends it off
	while(true)
	{
		// Other people's code goes here
	}
	return 0;
}
