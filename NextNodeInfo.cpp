#include <sstream>
#include <iostream>
#include "networking.h"
#include "CipherAlias.h"
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

string inPass;
RelayConfig inputPassword();
RelayConfig decrypt(string inPass);
void encrypt(string inPass);


//RelayConfig(int l, std::string h, int p, std::string a)
//std::string cipher_encrypt(std::string alias, std::string message);
//std::string cipher_decrypt(std::string alias, std::string message);
//alias is the thing that is used for encrypting/decrypting
//The test files will be called Information.txt and Encrypted.txt
int main()
{
	inputPassword();
	
	return 0;
}



RelayConfig inputPassword()
{
	string inPass;
	cout << "Please input your password:" << endl;
	cin >> inPass;
	if(inPass.size()!=0)
	{
		cout << "Thank you, attempting to decrypt the information" << endl;
		encrypt(inPass);
		decrypt(inPass);
	}
	else
	{
		exit(1);
	}
	RelayConfig whatYouNeed=decrypt(inPass);
	return whatYouNeed;
	
}

RelayConfig decrypt(string passIn)
{
	int i;
	string enInPort;
	string enIpAddress;
	string enOutPort;
	string enAlias;
	int deInPort;
	string deIpAddress;
	int deOutPort;
	string deAlias;
//good grief, that's way too many variables

	ifstream myfile("Encrypted.txt");//this takes and decrypts the information that its given, whoo!
	if(myfile.is_open())
	{
		stringstream ss;
		getline( myfile,enInPort);
		cout << "enInPort: " << enInPort << endl;
		getline( myfile,enIpAddress);
		cout << "enIpAddress: " << enIpAddress << endl;
		getline( myfile,enOutPort);
		cout << "enOutPort: " << enOutPort << endl;
		getline( myfile,enAlias);
		cout << "enAlias: " << enAlias << endl;
		ss << cipher_decrypt( passIn,enInPort );//reads it line by line, the 1st saved to a variable enInPort, and then decrypts with the password, saving that to return it
		ss >> deInPort;
		ss << cipher_decrypt( passIn,enIpAddress );
		ss >> deIpAddress;
		ss << cipher_decrypt( passIn,enOutPort );
		ss >> deOutPort;
		ss << cipher_decrypt( passIn,enAlias );
		ss >> deAlias;
		myfile.close();
		cout << "deInPort: " << deInPort << endl;
		cout << "deIpAddress: " << deIpAddress << endl;
		cout << "deOutPort: " << deOutPort << endl;
		cout << "deAlias: " << deAlias << endl;
	}

	for(i=0;i<inPass.size();i++)//since the password is no longer needed at all, we redact it
    {
        inPass[i]='X';
    }//end of the redation function

	return RelayConfig(deInPort,deIpAddress,deOutPort,deAlias);
}

//between this and the last comment is all that's needed to decrypt a txt file that's already encrypted

void encrypt(string passIn)
{	
//after this line is the stuff that encrypts an unencrpted file	
	string inPort;
	string ipAddress;
	string outPort;
	string alias;
	string enInPort;
	string enIpAddress;
	string enOutPort;
	string enAlias;
	int i;

	ifstream myfile("Information.txt");//reads through Information.txt and stores each line into a variable for use in creating the encrypted information
	if(myfile.is_open())//this for sure works, so no problems here
	{
		getline( myfile,inPort);
		getline( myfile,ipAddress);
		getline( myfile,outPort);
		getline( myfile,alias);
		myfile.close();
		enInPort = cipher_encrypt( passIn, inPort );
		enIpAddress = cipher_encrypt( passIn, ipAddress );
		enOutPort = cipher_encrypt( passIn, outPort );
		enAlias = cipher_encrypt( passIn, alias );
		cout << enInPort << endl;
		cout << enIpAddress << endl;
		cout << enOutPort << endl;
		cout << enAlias << endl;
	}
	
	ofstream otherfile("Encrypted.txt");//this is for taking the stuff that was read from Information.txt and storing it into the encrypted txt file
	if(otherfile.is_open())
	{
		otherfile << enInPort << endl;
		otherfile << enIpAddress << endl;
		otherfile << enOutPort << endl;
		otherfile << enAlias << endl;
		otherfile.close();
	}
	
	
	
/*	for(i=0;i<inPass.size();i++)//since the password is no longer needed at all, we redact it
	{
		inPass[i]='X';
	}//end of the redation function
	cout << deInPort << endl;
	cout << deIpAddress << endl;

*/

	return;
}
