#include <sstream>
#include <iostream>
#include "networking.h"
#include "CipherAlias.h"
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <map>
#include <unistd.h>

using namespace std;

string inPass;
RelayConfig inputPassword();
pair<bool,RelayConfig> decrypt(string inPass);
void encrypt(string inPass);
RelayConfig test();
//pair<return1,return2> foo
//return make_pair(return1,return2);
//if foo.first


//RelayConfig(int l, std::string h, int p, std::string a)
//std::string cipher_encrypt(std::string alias, std::string message);
//std::string cipher_decrypt(std::string alias, std::string message);
//alias is the thing that is used for encrypting/decrypting
//The test files will be called Information.txt and Encrypted.txt



/*Things that this program needs to be able to do:
	-check to make sure that there isn't an empty password file
		-if there is, make one, and request that the user give a password, which will then be used to encrypt and decrypt the information that they give.
	-ask the user to input their password
		-if the user inputs the correct password, decrypt the information and return that info
		-if the user inputs the wrong password, return an error
		-side note: make sure that there's a delay between confirming if the password is correct or not
	-because of the fact that the information is encrypted with the person's "password" the password isn't actually compared to anything,
	and instead relies on the fact that decrypting with the wrong password will return some very odd strings that don't work.

*/

RelayConfig inputPassword()
{
	int decrypted=0,input=0;
	string inPass;//this is supposed to be the password that the user inputs to begin the program
	while(decrypted==0)//repeats until the password is correct
	{
		while(input==0)//while loop to ensure that the user has provided non-empty input
		{
			input=0;
			cout << "Please input your password: ";//this requests their password on-screen
			getline(cin, inPass);
			//cin >> inPass;//this here asks for input and puts it into the variable inPass
			if(inPass.size()!=0)//checks to make sure that the password input isn't completely empty
			{
				cout << "Thank you, attempting to decrypt the information" << endl;//this is pretty straightforward, just tells the user that it's checking whether the password worked
//				encrypt(inPass);
//				decrypt(inPass);
					input=1;
					usleep(3000000);//waits 3 seconds before allowing the user to try the password again
			}
			else
			{
				cout << "Error, empty password field." << endl;
				usleep(3000000);//waits 3 seconds before allowing the user to try the password again
			}
		}
		decrypted=1;
	}
	RelayConfig whatYouNeed=test();
//		RelayConfig whatYouNeed=decrypt(inPass);
	return whatYouNeed;
	
}
/*
pair<bool,RelayConfig> decrypt(string passIn)
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
		deIpAddress=cipher_decrypt( passIn,enIpAddress );
		ss << cipher_decrypt( passIn,enOutPort );
		ss >> deOutPort;
		deAlias=cipher_decrypt( passIn,enAlias );
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
*/
//between this and the last comment is all that's needed to decrypt a txt file that's already encrypted
/*
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
	
	
	
	for(i=0;i<inPass.size();i++)//since the password is no longer needed at all, we redact it
	{
		inPass[i]='X';
	}//end of the redation function
	cout << deInPort << endl;
	cout << deIpAddress << endl;



	return;
}*/

RelayConfig test()
{
	
	string enForeignListen;
	string enIpAddress;
	string enForeignOut;
	string enAlias;
	string enLocalListen;
	string enLocalOut;
	int deLocalListen = -1;
	int deLocalOut = -1;
	int deForeignListen = -1;
	string deIpAddress;
	int deForeignOut = -1;
	string deAlias;
	ifstream myfile("test.txt");//this takes and decrypts the information that its given, whoo!
	stringstream ss;
	if(myfile.is_open())
	{
		getline( myfile,enForeignListen);//gets the first line,saves it to enInPort
		//cout << "enInPort: " << enInPort << endl;//prints this out
		getline( myfile,enIpAddress);//same
		//cout << "enIpAddress: " << enIpAddress << endl;//same
		getline( myfile,enForeignOut);//same
		//cout << "enOutPort: " << enOutPort << endl;//same
		getline( myfile,enAlias);//same
		//cout << "enAlias: " << enAlias << endl;//same
		myfile.close();
		ss << enForeignListen;//reads it line by line, the 1st saved to a variable enInPort, and then decrypts with the password, saving that to return it
		ss >> deForeignListen;
		ss.str(""); // Clear the stringstream
		ss.clear(); // And any weird state it may have entered
		deIpAddress=enIpAddress;
		ss << enForeignOut;
		deAlias=enAlias;
		ss >> deForeignOut;
		ss.str("");
		ss.clear();
		ss << enLocalListen;//reads it line by line, the 1st saved to a variable enInPort, and then decrypts with the password, saving that to return it
		ss >> deLocalListen;
		ss.str(""); // Clear the stringstream
		ss.clear(); // And any weird state it may have entered
		ss << enLocalOut;//reads it line by line, the 1st saved to a variable enInPort, and then decrypts with the password, saving that to return it
		ss >> deLocalOut;
		ss.str(""); // Clear the stringstream
		ss.clear(); // And any weird state it may have entered
		/*
		cout << "Gives:" << deInPort << endl;
		cout << "Gives:" << deIpAddress << endl;
		cout << "Gives:" << deOutPort << endl;
		cout << "Gives:" << deAlias << endl;
		*/
	}

	return RelayConfig(deForeignListen,deIpAddress,deForeignOut,deAlias,deLocalListen,deLocalOut);
}
