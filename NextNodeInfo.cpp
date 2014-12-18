#include <sstream>
#include <iostream>
#include "relay.h"
#include "CipherAlias.h"
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <map>
#include <unistd.h>
#include "log.h"
#include "cypher.h"

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
//std::string encrypt(std::string alias, std::string message);
//std::string decrypt(std::string alias, std::string message);
//alias is the thing that is used for encrypting/decrypting
//The test files will be called Information.txt and Encrypted.txt

//logErr
//logWarn
//logDebug
//logInfo

//Song lyrics from OMI's "Cheerleader", just for Milo
/*
"She walks like a model,
She grants my wishes like a genie in a bottle,
'Cause I'm the wizard of love,
And I've got the magic wand,
All these other girls are tempting,
But I'm empty when you're gone.
And they're like, 
"Do you need me?
Do you think I'm pretty?
Do I make you feel like cheating?"
And I'm like "No, not really!"
*/


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

RelayConfig relayDecrypt(string passIn)
{
	int i;
	string enForeignListen;
	string enIpAddress;
	string enForeignOut;
	string enAlias;
	string enLocalListen;
	string enLocalOut;
	string enLogFile;
	int deForeignListen;
	string deIpAddress;
	int deForeignOut;
	string deAlias;
	int deLocalListen;
	int deLocalOut;
	string deLogFile;
//good grief, that's way too many variables

	ifstream myfile("Encrypted.txt");//this takes and decrypts the information that its given, whoo!
	if(myfile.is_open())
	{
		stringstream ss;
		getline( myfile,enForeignListen );
		getline( myfile,enIpAddress );
		getline( myfile,enForeignOut );
		getline( myfile,enAlias );
		getline( myfile,enLocalListen );
		getline( myfile,enLocalOut );
		getline( myfile,enLogFile );
		ss << decypher( enForeignListen,passIn );//reads it line by line, the 1st saved to a variable enForeignListen, and then decrypts with the password, saving that to return it
		ss >> deForeignListen;
		ss.str(""); // Clear the stringstream
		ss.clear(); // And any weird state it may have entered
		deIpAddress=decypher( enIpAddress,passIn );
		ss << decypher( enForeignOut,passIn );
		ss >> deForeignOut;
		ss.str(""); // Clear the stringstream
		ss.clear(); // And any weird state it may have entered
		deAlias=decypher( enAlias,passIn );
		ss << decypher( enLocalListen,passIn );
		ss >> deLocalListen;
		ss.str(""); // Clear the stringstream
		ss.clear(); // And any weird state it may have entered
		ss << decypher( enLocalOut,passIn );
		ss >> deLocalOut;
		ss.str(""); // Clear the stringstream
		ss.clear(); // And any weird state it may have entered
		deLogFile=decypher( enLogFile,passIn );
		myfile.close();
		cout << "deForeignListen: " << deForeignListen << endl;
		cout << "deIpAddress: " << deIpAddress << endl;
		cout << "deForeignOut: " << deForeignOut << endl;
		cout << "deAlias: " << deAlias << endl;
		cout << "deLocalListen: " << deLocalListen << endl;
		cout << "deLocalOut: " << deLocalOut << endl;
		cout << "deLogFile: " << deLogFile << endl;
	}

	for(i=0;i<inPass.size();i++)//since the password is no longer needed at all, we redact it
    {
        inPass[i]='X';
    }//end of the redation function
	return RelayConfig(deForeignListen,deIpAddress,deForeignOut,deAlias,deLocalListen,deLocalOut,deLogFile);
}

//between this and the last comment is all that's needed to decrypt a txt file that's already encrypted

void relayEncrypt(string passIn)
{	
//after this line is the stuff that encrypts an unencrpted file	
	string foreignListen;
	string ipAddress;
	string foreignOut;
	string alias;
	string localListen;
	string localOut;
	string logFile;
	string enForeignListen;
	string enIpAddress;
	string enForeignOut;
	string enAlias;
	string enLocalListen;
	string enLocalOut;
	string enLogFile;
	int i;

	ifstream infofile("Information.txt");//reads through Information.txt and stores each line into a variable for use in creating the encrypted information
	if(infofile.is_open())//this for sure works, so no problems here
	{
		getline( infofile,foreignListen );
		getline( infofile,ipAddress );
		getline( infofile,foreignOut );
		getline( infofile,alias );
		getline( infofile,localListen );
		getline( infofile,localOut );
		getline( infofile,logFile );
		infofile.close();
		enForeignListen = cypher( foreignListen,passIn );
		enIpAddress = cypher( ipAddress,passIn );
		enForeignOut = cypher( foreignOut,passIn );
		enAlias = cypher( alias,passIn );
		enLocalListen = cypher( localListen,passIn );
		enLocalOut = cypher( localOut,passIn );
		enLogFile = cypher( logFile,passIn );
		
//		cout << enForeignListen << endl;
//		cout << enIpAddress << endl;
//		cout << enForeignOut << endl;
//		cout << enAlias << endl;
	}
	
	ofstream encryptedfile("Encrypted.txt");//this is for taking the stuff that was read from Information.txt and storing it into the encrypted txt file
	if(encryptedfile.is_open())
	{
		encryptedfile << enForeignListen << endl;
		encryptedfile << enIpAddress << endl;
		encryptedfile << enForeignOut << endl;
		encryptedfile << enAlias << endl;
		encryptedfile << enLocalListen << endl;
		encryptedfile << enLocalOut << endl;
		encryptedfile << enLogFile << endl;
		encryptedfile.close();
	}
	
	
	
	for(i=0;i<inPass.size();i++)//since the password is no longer needed at all, we redact it
	{
		inPass[i]='X';
	}//end of the redaction function
//	cout << deForeignListen << endl;
//	cout << deIpAddress << endl;



	return;
}

RelayConfig test()
{
	
	string enForeignListen="Error";
	string enIpAddress="Error";
	string enForeignOut="Error";
	string enAlias="Error";
	string enLocalListen="Error";
	string enLocalOut="Error";
	string enLogFile="Error";
	string log="";
	int deLocalListen = -1;
	int deLocalOut = -1;
	int deForeignListen = -1;
	string deIpAddress="Error";
	int deForeignOut = -1;
	string deAlias="Error";
	string deLogFile="Error";
	ifstream myfile("test.txt");//this takes and decrypts the information that its given, whoo!
	stringstream ss;
	if(myfile.is_open())
	{
		getline( myfile,enForeignListen );//gets the first line,saves it to enForeignListen
		// We cannot print encrypted information, so these logs have been disabled
		/*
		if(enForeignListen!="Error")
		{
			log="Input encrypted foreign listen port: ";
			log+=enForeignListen;
			logInfo(log);
			log="";
		}
		*/
		//cout << "enForeignListen: " << enForeignListen << endl;//prints this out
		getline( myfile,enIpAddress );//same
		/*
		if(enIpAddress!="Error")
		{
			log="Input encrypted ip address: ";
			log+=enIpAddress;
			logInfo(log);
			log="";
		}
		*/
		//cout << "enIpAddress: " << enIpAddress << endl;//same
		getline( myfile,enForeignOut );//same
		/*
		if(enForeignOut!="Error")
		{
			log="Input encrypted foreign ouput port: ";
			log+=enForeignOut;
			logInfo(log);
			log="";
		}
		*/
		//cout << "enForeignOut: " << enForeignOut << endl;//same
		getline( myfile,enAlias );//same
		/*
		if(enAlias!="Error")
		{
			log="Input encrypted foreign ouput port: ";
			log+=enAlias;
			logInfo(log);
			log="";
		}
		*/
		//cout << "enAlias: " << enAlias << endl;//same
		getline( myfile,enLocalListen );
		getline( myfile,enLocalOut );
		getline( myfile,enLogFile );
		myfile.close();
		ss << enForeignListen;//reads it line by line, the 1st saved to a variable enForeignListen, and then decrypts with the password, saving that to return it
		ss >> deForeignListen;
		ss.str(""); // Clear the stringstream
		ss.clear(); // And any weird state it may have entered
		deIpAddress=enIpAddress;
		ss << enForeignOut;
		deAlias=enAlias;
		ss >> deForeignOut;
		ss.str("");
		ss.clear();
		ss << enLocalListen;//reads it line by line, the 1st saved to a variable enForeignListen, and then decrypts with the password, saving that to return it
		ss >> deLocalListen;
		ss.str(""); // Clear the stringstream
		ss.clear(); // And any weird state it may have entered
		ss << enLocalOut;//reads it line by line, the 1st saved to a variable enForeignListen, and then decrypts with the password, saving that to return it
		ss >> deLocalOut;
		ss.str(""); // Clear the stringstream
		ss.clear(); // And any weird state it may have entered
		ss << enLogFile;
		ss >> deLogFile;
		ss.str("");
		ss.clear();
		/*
		cout << "Gives:" << deForeignListen << endl;
		cout << "Gives:" << deIpAddress << endl;
		cout << "Gives:" << deForeignOut << endl;
		cout << "Gives:" << deAlias << endl;
		*/
	}

	return RelayConfig(deForeignListen,deIpAddress,deForeignOut,deAlias,deLocalListen,deLocalOut,deLogFile);
}
