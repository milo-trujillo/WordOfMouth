#include <iostream>
#include <string>
#include <openssl/blowfish.h>


class CipherAlias{

public:

	public:
		CipherAlias(std::string a){alias = a;}
		
		//Encrypts
		std::string cipher_encrypt(std::string message);
		
		//Decrypts
		std::string cipher_decrypt(std::string encrypted_message);

		bool data_decoded(std::string msg);

	private:
		std::string alias;
	

private:
	
};
