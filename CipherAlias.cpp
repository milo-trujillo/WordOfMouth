#include <iostream>
#include <string.h>
#include <stdlib.h>

#include <openssl/blowfish.h>
#include "CipherAlias.h"



//typedef SymmetricCipher Encryption

//This function should encrypt the receiving user's alias and return an encrypted version of the message
std::string CipherAlias::cipher_encrypt(std::string alias, std::string message){
	// TODO: Figure out the 4-characters off bug
	// If the message is four characters longer than a multiple of 8 then the
	// function flips out and prints a random char at the end. There must be a
	// better solution than this.
	if( message.size() % 8 == 4 )
		message.push_back(0);
	int message_size = message.size();
	int padded_size = message_size + (message_size % 8);

	//std::cout << "Pre-size (msg): " << message_size << std::endl;
	//std::cout << "Pre-size (pad): " << padded_size << std::endl;
	
	BF_KEY *key = new BF_KEY;
	BF_set_key(key, alias.size(), (unsigned char*) alias.c_str());
	
	unsigned char *en_mess = new unsigned char[padded_size];
	unsigned char *en_mess2 = en_mess;
	unsigned char *msg = new unsigned char[message_size];
	unsigned char *full_msg = msg;
	for( int i = 0; i < message_size; i++ )
		msg[i] = message[i];
	for( int i = 0; i < padded_size; i++ )
		en_mess[i] = 0;
	
	for(int i = 0; i < message_size/8; i++){
		BF_ecb_encrypt(msg, en_mess2, key, BF_ENCRYPT);
		en_mess2+= 8;
		msg += 8;
	}

	if(message_size%8 !=0 )
		BF_ecb_encrypt(msg, en_mess2, key, BF_ENCRYPT);
	
	std::string encrypted = (char*) en_mess;
	//std::cout<<"Encrypted size: "<<strlen((char*) en_mess)<<std::endl;

	delete key;
	delete [] en_mess;
	delete [] full_msg;	
	return encrypted;
}



std::string CipherAlias::cipher_decrypt(std::string alias, std::string encrypted_message){	
	int message_size = encrypted_message.size();
	
	BF_KEY *key = (BF_KEY*) calloc(1, sizeof(BF_KEY));
	BF_set_key(key, alias.size(), (unsigned char*) alias.c_str());
	
	unsigned char *un_mess = (unsigned char*) calloc(message_size, sizeof(unsigned char) );
	unsigned char *un_mess2 = un_mess;
	unsigned char *another_fucking_char_star = new unsigned char[message_size];
	unsigned char *full_msg = another_fucking_char_star;

	//std::cout << "Decrypted size (pre-calc): " << message_size << std::endl;
	
	for(int j=0; j<message_size; j++)
		another_fucking_char_star[j] = encrypted_message[j];
	
	for(int i = 0; i < message_size/8; i++){
		BF_ecb_encrypt(another_fucking_char_star, un_mess2, key, BF_DECRYPT);
		un_mess2+= 8;
		another_fucking_char_star += 8;
	}
	
	if(message_size%8 !=0 )
		BF_ecb_encrypt(another_fucking_char_star, un_mess2, key, BF_DECRYPT);

	//std::cout << "Descrypted strlen: " << strlen((char*)un_mess) << std::endl;
	std::string unencrypted = (char*) un_mess;
	//std::cout<<"Decrypted size: "<<unencrypted.size()<<std::endl;
	delete key;
	delete [] un_mess;
	delete [] full_msg;
	return unencrypted;
}
