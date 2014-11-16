#include <iostream>
#include <string>
#include <openssl/blowfish.h>



//typedef SymmetricCipher Encryption

//This function should encrypt the receiving user's alias and return an encrypted version of the message
std::string cipher_encrypt(const std::string alias, const std::string message){
	int counter = 0;
	std::string encrypted_message = message;
	
	BF_KEY key;
	BF_set_key(&key, alias.size(), (unsigned char*) alias.c_str());
	
	for(int i = 0; i < message.size(); i++){
		unsigned char mess = message[i];
		unsigned char en_mess = ' ';
		BF_ecb_encrypt(&mess, &en_mess, &key, counter);
		counter++;
		encrypted_message[i] = en_mess;
	}
	
	return encrypted_message;
}

std::string cipher_decrypt(std::string alias, std::string encrypted_message){
		int counter = 0;
	std::string decrypted_message = encrypted_message;
	
	BF_KEY key;
	BF_set_key(&key, alias.size(), (unsigned char*) alias.c_str());
	
	for(int i = 0; i < encrypted_message.size(); i++){
		unsigned char mess = encrypted_message[i];
		unsigned char en_mess = ' ';
		BF_ecb_encrypt(&mess, &en_mess, &key, counter);
		counter++;
		decrypted_message[i] = en_mess;
	}
	
	return decrypted_message;
}
