/*
	This file just coordinates different parts of the codebase, and should do
	little work itself.
*/

#include <pthread.h> // Needed for multithreading stuff
#include <iostream>

#include "networking.h"
#include "cipher.h"

using namespace std;

int main()
{
	pthread_t relayThread;
	pthread_create(&relayThread, NULL, startRelaying, NULL);
	cout << "Relaying started." << endl;
	// Anyone elses initialization code goes here

	//Test Cipher
	Cipher new_cipher;
	
	std::string alias = "Jazmyn";
	
	unsigned char *message  = (unsigned char *) "Hello Worlds, boop beep,bop bepbooop";
	
	std::cout<<"This should be alias and message: "<<
	alias<<", "<<message<<std::endl; 
	
	std::string encrypted;
	encrypted = new_cipher.cipher_encrypt(alias, message);
	
	std::cout<<"End of encrypted"<<std::endl;
		
	std::string decrypted;
	decrypted = new_cipher.cipher_decrypt(alias, encrypted);
	
	std::cout<<"This is the decrypted version of the message: "<< decrypted<<std::endl; 
		
	// Main event loop gets input from user, encodes it, sends it off
	while(true)
	{
		// Other people's code goes here
	}
	return 0;
}
