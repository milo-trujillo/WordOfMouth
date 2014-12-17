#include <stdio.h>
#include <openssl/aes.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <string>

using namespace std;

//string encrypt(string plainText,string key);
//string decrypt(string encryptedText,string key);
//int main()//this returns true if it successfully got through all of the code
//{
//	string errorIn="This is really  long so I'm going to keep trping until I have reached 72";//this combination of input and password produces an error for encryption.
//	string errorPassword="Evenn longer string that somewhat unnecessary";
//	string in="This is really  long so I'm going to keep typing until I have reached 72";//this combination of input and password works fine for encryption.
//	string password="Evenn longer string that somewhat unnecessary";
/*
	int inLength=in.length();
	int passLength=password.length();
	cout << inLength << " " << passLength << endl;
 //debugging code
*/
//	cout << "initial password: " << password << endl;//debugging stuff
/*	string ide=encrypt(in,password);//encrypts and stores the result to a string
	if(ide!="Error")//checks to make sure that an Error wasn't returned by the encryption, and if it was successful, continues to decryption
	{
		string de=decrypt(ide,password);//encrypts then decrypts and saves the result off to a string
		cout << de << endl;//prints out the string that was just saved above
	}
	else//if the encryption was unsuccessful, it continues to this.
	{
		cout << "Error with encryption." << endl;
	}*/
/* This was the initially working code before separating the encryption and decryption into two functions. It still errors on the bugs the current revision of the separate functions does.
	string plainTextString="This is really  long so I'm going to keep typing until I have reached 72";
	string keyString="Even longer string that somewhat unnecessary";
	int plainTextLength=plainTextString.length();
	int keyLength=keyString.length();//saves off the length of the plaintext and key strings
	int i=0,j=0;
	unsigned char plainText[plainTextLength],key[keyLength];//allocates space for arrays of chars equal to the length of the input plaintext and key strings
	int SIZEOFARRAY=(plainTextLength*2);//this adjusts the sizes of the arrays according to the size of the input string
	for(int i=0;i<plainTextLength;i++)//converts the plain to an array of chars
	{
		plainText[i]=0;
		plainText[i]=plainTextString[i];
	}
	for(i=0;i<keyLength;i++)//converts the key to an array of chars
	{
		key[i]=0;
		key[i]=keyString[i];
	}

	int repetitions=((plainTextLength/16)+1);

	unsigned char decryptedText[SIZEOFARRAY];
	unsigned char encryptedText[SIZEOFARRAY];

	for(int i=0;i<SIZEOFARRAY;i++)//zeroes out the arrays which was causing weird issues
	{
		decryptedText[i]=0;
		encryptedText[i]=0;
	}
	AES_KEY enKey;
	AES_set_encrypt_key(key,128,&enKey);

	AES_KEY deKey;
	AES_set_decrypt_key(key,128,&deKey);


	unsigned char partialPlainText[16],partialEncryptedText[16],partialDecryptedText[16],partEncryptedText[16];
	for(i=0;i<repetitions;i++)
	{
		for(j=0;j<16;j++)//this function breaks up the plainText into chunks that are 16 chars long
		{
			partialPlainText[j]=plainText[(16*i)+j];
		}

		AES_encrypt(partialPlainText,partialEncryptedText,&enKey);//uses the AES_KEY to encrypt the chunk of chars that was created

		for(j=0;j<16;j++)
		{
			encryptedText[(16*i)+j]=partialEncryptedText[j];//this stores the encrypted text chunks into an encrypted text char
		}
	
	


		for(j=0;j<16;j++)
		{
			partEncryptedText[j]=encryptedText[(16*i)+j];//this breaks up the encrypted text into 16 char chunks
		}


		AES_decrypt(partEncryptedText,partialDecryptedText,&deKey);//uses the AES_KEY to decrypt the chunk of chars that was created

		for(j=0;j<16;j++)
		{
			decryptedText[(16*i)+j]=partialDecryptedText[j];//this stores the decrypted text chunks into a decrypted text char
		}

	}

	stringstream ss;
	ss << decryptedText;
	string finalDecryptedText(ss.str());
	cout << "decrypted text: " << finalDecryptedText << endl;
*/

//	return 0;

//}






string decrypt(string encryptedTextString,string keyString)
{
	int i,j;
	int encryptedTextLength=encryptedTextString.length();//figures out how long the encryptedTextString is
	int keyLength=keyString.length();//saves off the length of the key string
	int SIZEOFARRAY=(encryptedTextLength+8);//this adjusts the sizes of the arrays according to the size of the input string, with some extra space
	unsigned char decryptedText[SIZEOFARRAY];//sets off enough space for the decryptedText to be stored in an array
//	cout << "encryptedTextLength: " << encryptedTextLength << endl;
//	cout << "decryptKeyLength: " << keyLength << " key: " << keyString << endl;
//	cout << "modulus: " << encryptedTextLength%(keyLength-1) << endl;
	unsigned char* encryptedText=(unsigned char*)encryptedTextString.c_str();
	unsigned char* key=(unsigned char*)keyString.c_str();
	int repetitions=(encryptedTextLength/16)+1;

	for(i=0;i<SIZEOFARRAY;i++)//zeroes out the arrays which was causing weird issues
	{
		decryptedText[i]=0;
	}
	AES_KEY deKey;
	AES_set_decrypt_key(key,128,&deKey);
	unsigned char partialEncryptedText[16],partialDecryptedText[16];
	for(i=0;i<repetitions;i++)
	{
		for(j=0;j<16;j++)
		{
			partialEncryptedText[j]=0;
			partialEncryptedText[j]=encryptedText[(16*i)+j];//this breaks up the encrypted text into 16 char chunks
		}

		AES_decrypt(partialEncryptedText,partialDecryptedText,&deKey);//uses the AES_KEY to decrypt the chunk of chars that was created

		for(j=0;j<16;j++)
		{
			decryptedText[(16*i)+j]=0;//zeroes out before replacing with the content of the decryption
			decryptedText[(16*i)+j]=partialDecryptedText[j];//this stores the decrypted text chunks into a decrypted text char
		}

	}
	for(i=0;j<16;i++)
	{
		partialEncryptedText[i]=0;
		partialDecryptedText[i]=0;
	}
	stringstream ss;
	ss << decryptedText;
	string finalDecryptedText(ss.str());
	for(i=0;i<SIZEOFARRAY;i++)
	{
		decryptedText[i]=0;
	}
	return finalDecryptedText;
}


string encrypt(string plainTextString,string keyString)
{
	string error="Error";
	int i,j;
	int plainTextLength=plainTextString.length();
	int keyLength=keyString.length();//saves off the length of the plaintext and key strings
	int SIZEOFARRAY=(plainTextLength+8);//this adjusts the sizes of the arrays according to the size of the input string
//	cout << "Plaintext length: " << plainTextLength << endl;//debugging stuff
//	cout << "encryptKeyLength: " << keyLength << " key: " << keyString << endl;//debugging stuff

	unsigned char encryptedText[SIZEOFARRAY];
	unsigned char* plainText=(unsigned char*)plainTextString.c_str();
	unsigned char* key=(unsigned char*)keyString.c_str();
	int repetitions=(plainTextLength/16)+1;
	for(int i=0;i<SIZEOFARRAY;i++)//zeroes out the arrays which was causing weird issues
	{
		encryptedText[i]=0;
	}
	AES_KEY enKey;
	AES_set_encrypt_key(key,128,&enKey);
	unsigned char partialPlainText[16],partialEncryptedText[16];
	for(i=0;i<repetitions;i++)//repeats this process as many times as necessary
	{
		for(j=0;j<16;j++)//this function breaks up the plainText into chunks that are 16 chars long
		{
			partialPlainText[j]=0;
			partialPlainText[j]=plainText[(16*i)+j];
		}
		AES_encrypt(partialPlainText,partialEncryptedText,&enKey);//uses the AES_KEY to encrypt the chunk of chars that was created

		for(j=0;j<16;j++)
		{
			encryptedText[(16*i)+j]=partialEncryptedText[j];//this stores the encrypted text chunks into an encrypted text char
			partialEncryptedText[j]=0;
		}
	}
	for(i=0;j<16;i++)
	{
		partialEncryptedText[i]=0;
		partialPlainText[i]=0;
	}
	stringstream ss;
	ss << encryptedText;
	string finalEncryptedTextString(ss.str());
	for(i=0;i<SIZEOFARRAY;i++)
	{
		encryptedText[i]=0;
	}
//	cout << "finalEncryptedTextLength: " << finalEncryptedTextString.length() << endl;//debugging diddlybop
	if(decrypt(finalEncryptedTextString,keyString)==plainTextString)
	{
		cout << "Encryption successful." << endl;
		return finalEncryptedTextString;
	}
	else
	{
		cout << endl;
		cout << "Error encrypting. Chances are you can fix this by changing one of the n's in your password to something else, or adding an extra letter." << endl;
		cout << endl;
		return error;
	}
}

