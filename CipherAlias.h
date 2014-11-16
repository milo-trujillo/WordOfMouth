#include <iostream>
#include <string>
#include <openssl/blowfish.h>


class CipherAlias{

public:

	CipherAlias(){}
	
	//Encrypts
	std::string cipher_encrypt(std::string alias, unsigned char* message);
	
	//Decrypts
	std::string cipher_decrypt(std::string alias, std::string encrypted_message);
	

private:
	
};