#include <iostream>
#include <string>

#include <openssl/blowfish.h>
#include "cipher.h"



//typedef SymmetricCipher Encryption

//This function should encrypt the receiving user's alias and return an encrypted version of the message
std::string CipherAlias::cipher_encrypt(std::string alias, unsigned char* message){
	int message_size = strlen((char*) message);
	
	BF_KEY *key = new BF_KEY;
	BF_set_key(key, alias.size(), (unsigned char*) alias.c_str());
	
	unsigned char *en_mess = new unsigned char[message_size+7];
	unsigned char *en_mess2 = en_mess;
	std::cout<<"Size check: "<<strlen((char*) message)<<std::endl;
	
	
	for(int i = 0; i < message_size/8; i++){
		BF_ecb_encrypt(message, en_mess2, key, BF_ENCRYPT);
		std::cout<<"Message pre: "<<message<<std::endl;
		std::cout<<"En_mess pre: "<<en_mess<<std::endl;
		en_mess2+= 8;
		message += 8;
		std::cout<<"En_mess post: "<<en_mess<<std::endl;
		std::cout<<"Message post: "<<message<<std::endl;
	}
	
	std::cout<<"Post encrypt"<<std::endl;
	
	if(message_size%8 !=0 )
		BF_ecb_encrypt(message, en_mess2, key, BF_ENCRYPT);
	
	std::string encrypted = (char*) en_mess;
	std::cout<<"Encrypted size: "<<strlen((char*) en_mess)<<std::endl;
	return encrypted;
}



std::string CipherAlias::cipher_decrypt(std::string alias, std::string encrypted_message){	
	int message_size = encrypted_message.size();
	
	BF_KEY *key = (BF_KEY*) calloc(1, sizeof(BF_KEY));
	BF_set_key(key, alias.size(), (unsigned char*) alias.c_str());
	
	unsigned char *un_mess = (unsigned char*) calloc(encrypted_message.size(), sizeof(unsigned char) );
	unsigned char *un_mess2 = un_mess;
	unsigned char *another_fucking_char_star = new unsigned char[message_size];
	
	for(int j=0; j<message_size; j++){
		another_fucking_char_star[j] = encrypted_message[j];
	}
	
	for(int i = 0; i < message_size/8; i++){
		BF_ecb_encrypt(another_fucking_char_star, un_mess2, key, BF_DECRYPT);
		un_mess2+= 8;
		another_fucking_char_star += 8;
	}
	
	if(message_size%8 !=0 )
		BF_ecb_encrypt((unsigned char*) encrypted_message.c_str(), un_mess2, key, BF_DECRYPT);

	std::string unencrypted = (char*) un_mess;
	std::cout<<"Decrypted size: "<<unencrypted.size()<<std::endl;
	return unencrypted;
}
