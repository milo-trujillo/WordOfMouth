#include <stdio.h>
#include <openssl/aes.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
using namespace std;

//string encrypt(unsigned char key[],unsigned char plainText[]);
//string decrypt(unsigned char key[],unsigned char encryptedText[]);
string encrypt(string plainText,string key);
string decrypt(string encryptedText,string key);
int main()//this returns true if it successfully got through all of the code
{
//	string in="This is a really quite considerably longer but still really dumb string.";
//	string password="Even dumber string";
//	cout << in.length() << " " << password.length() << endl;//this is for testing purposes to try and pinpoint the current debug issure where multiples of 8 do things funkier than the Black Eyed Peas
//	string de=decrypt(encrypt(in,password),password);//encrypts then decrypts and saves the result off to a string
//	cout << de << endl;//prints out the string that was just saved above

	string plainTextString="This might be pushing the boundaries just a little bit, since it's lot more than just 17 chars.";
	string keyString="less 8";
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

	unsigned char partialPlainText[16],partialEncryptedText[16],partialDecryptedText[16];
	for(i=0;i<repetitions;i++)
	{
		for(j=0;j<16;j++)//this function breaks up the plainText into chunks that are 16 chars long
		{
			partialPlainText[j]=plainText[(16*i)+j];
		}
		AES_KEY enKey;//creates an AES_KEY variable

		AES_set_encrypt_key(key,128,&enKey);//sets the value of the AES_KEY based on a personal key

		AES_encrypt(partialPlainText,partialEncryptedText,&enKey);//uses the AES_KEY to encrypt the chunk of chars that was created

		for(j=0;j<16;j++)
		{
			encryptedText[(16*i)+j]=partialEncryptedText[j];//this stores the encrypted text chunks into an encrypted text char
		}
	
//	unsigned char encryptedText=encrypt(&key,&plainText);
	


		for(j=0;j<16;j++)
		{
			partialEncryptedText[j]=encryptedText[(16*i)+j];//this breaks up the encrypted text into 16 char chunks
		}

		AES_KEY deKey;//creates an AES_KEY variable

		AES_set_decrypt_key(key,128,&deKey);//sets the value of the AES_KEY based on a personal key

		AES_decrypt(partialEncryptedText,partialDecryptedText,&deKey);//uses the AES_KEY to decrypt the chunk of chars that was created

		for(j=0;j<16;j++)
		{
			decryptedText[(16*i)+j]=partialDecryptedText[j];//this stores the decrypted text chunks into a decrypted text char
		}

	}
//	cout << decryptedText << endl;
//	unsigned char decryptedText=decrypt(&key,&encryptedText);

	stringstream ss;
	ss << decryptedText;
	string finalDecryptedText(ss.str());
	cout << "decrypted text: " << finalDecryptedText << endl;


	return 0;
}


/*



string decrypt(string encryptedTextString,string keyString)
{
	int i,j;
	int encryptedTextLength=encryptedTextString.length();//figures out how long the encryptedTextString is
	int keyLength=keyString.length();//saves off the length of the key string
	unsigned char key[keyLength];//allocates space for arrays of chars equal to the length of the keylength
	int SIZEOFARRAY=(encryptedTextLength*2);//this adjusts the sizes of the arrays according to the size of the input string, with some extra space
	unsigned char decryptedText[SIZEOFARRAY];//sets off enough space for the decryptedText to be stored in an array
	unsigned char encryptedText[encryptedTextLength];//sets off enough space to move the encryptedText to an array
	for(int i=0;i<encryptedTextLength;i++)//converts the plain to an array of chars
	{
		encryptedText[i]=0;//zeroes out the values first
		encryptedText[i]=encryptedTextString[i];//replaces the zeroed value with value of the encrypted string
	}
	for(i=0;i<keyLength;i++)//converts the key to an array of chars for use in the AES encryption thing
	{
		key[i]=0;//zeroes out the values first
		key[i]=keyString[i];//replaces the zeroed value with value of the key
	}
	int repetitions;
//	if((encryptedTextLength%16)==0)
//	{
//		repetitions=(encryptedTextLength/16);
//	}
//	else
//	{
		repetitions=((encryptedTextLength/16)+1);//figures out how many repetitions are needed according to the length of the string
//	}

	for(i=0;i<encryptedTextString.length();i++)
	{
		encryptedText[i]=0;
		encryptedText[i]=encryptedTextString[i];
	}
	for(i=0;i<SIZEOFARRAY;i++)//zeroes out the arrays which was causing weird issues
	{
		decryptedText[i]=0;
	}
	unsigned char partialEncryptedText[16],partialDecryptedText[16];
	for(i=0;i<repetitions;i++)
	{
		for(j=0;j<16;j++)
		{
			partialEncryptedText[j]=encryptedText[(16*i)+j];//this breaks up the encrypted text into 16 char chunks
		}

		AES_KEY deKey;//creates an AES_KEY variable

		AES_set_decrypt_key(key,128,&deKey);//sets the value of the AES_KEY based on a personal key

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
	int i,j;
	int plainTextLength=plainTextString.length();
	int keyLength=keyString.length();//saves off the length of the plaintext and key strings
	unsigned char plainText[plainTextLength],key[keyLength];//allocates space for arrays of chars equal to the length of the input plaintext and key strings
	int SIZEOFARRAY=(plainTextLength*2);//this adjusts the sizes of the arrays according to the size of the input string
	unsigned char encryptedText[SIZEOFARRAY];
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
	int repetitions;
//	if((plainTextLength%16)==0)
//	{
//		repetitions=(plainTextLength/16);
//	}
//	else
//	{
		repetitions=((plainTextLength/16)+1);
//	}

	for(int i=0;i<SIZEOFARRAY;i++)//zeroes out the arrays which was causing weird issues
	{
		encryptedText[i]=0;
	}
	unsigned char partialPlainText[16],partialEncryptedText[16];
	for(i=0;i<repetitions;i++)//repeats this process as many times as necessary
	{
		for(j=0;j<16;j++)//this function breaks up the plainText into chunks that are 16 chars long
		{
			partialPlainText[j]=plainText[(16*i)+j];
		}
		AES_KEY enKey;//creates an AES_KEY variable

		AES_set_encrypt_key(key,128,&enKey);//sets the value of the AES_KEY based on a personal key

		AES_encrypt(partialPlainText,partialEncryptedText,&enKey);//uses the AES_KEY to encrypt the chunk of chars that was created

		for(j=0;j<16;j++)
		{
			encryptedText[(16*i)+j]=partialEncryptedText[j];//this stores the encrypted text chunks into an encrypted text char
		}
	}
	for(i=0;j<16;i++)
	{
		partialEncryptedText[i]=0;
		partialPlainText[i]=0;
	}
	stringstream ss;
	ss << encryptedText;
	string finalEncryptedText(ss.str());
	for(i=0;i<SIZEOFARRAY;i++)
	{
		encryptedText[i]=0;
	}


	return finalEncryptedText;
}*/
